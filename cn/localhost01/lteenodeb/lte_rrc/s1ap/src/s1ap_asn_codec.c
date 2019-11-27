/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: s1ap_asn_codec.c,v 1.6 2010/10/13 07:04:17 gur18569 Exp $
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
 * $Log: s1ap_asn_codec.c,v $
 * Revision 1.6  2010/10/13 07:04:17  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.3.4.8  2010/10/11 07:27:12  gur18569
 * commented asn print statements
 *
 * Revision 1.3.4.7  2010/10/07 11:00:35  gur04518
 * map type ASN1V_s1ap_id_Paging to S1_PAGING
 *
 * Revision 1.3.4.6  2010/10/07 07:12:25  gur04518
 * removed the ASN1V_s1ap_id_downlinkNASTransport, ASN1V_s1ap_id_InitialContextSetup, ASN1V_s1ap_id_E_RABRelease and mapped it to ASN_MSG_TO_FORWARD
 *
 * Revision 1.3.4.5  2010/10/04 13:31:54  gur04518
 * removed backslash
 *
 * Revision 1.3.4.4  2010/09/09 10:07:36  gur22047
 * modified during s1ap UT
 *
 * Revision 1.3.4.3  2010/08/05 08:10:44  gur15697
 * merging round 2
 *
 * Revision 1.3.4.2  2010/07/21 08:44:21  gur15697
 * HO ERAB code Merging changes
 * Revision 1.5  2010/07/24 07:18:48  gur04733
 * Checking On 24 July
 *
 * Revision 1.4  2010/06/22 09:31:59  gur21231
 * ERAB RELEASE
 *
 * Revision 1.3.2.2  2010/06/30 11:57:47  gur10248
 * *** empty log message ***
 *
 * Revision 1.3.4.1  2010/06/24 12:56:16  gur22047
 * internal source and s1ap code
 *
 * Revision 1.3  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.12  2009/12/28 10:15:36  gur21006
 * system calls removed
 *
 * Revision 1.1.2.11  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.10  2009/12/27 08:39:04  gur18569
 * removed tabs
 *
 * Revision 1.1.2.9  2009/12/26 12:05:01  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.8  2009/12/24 10:31:05  gur21006
 * Warnings removed
 *
 * Revision 1.1.2.7  2009/12/22 14:38:04  gur18569
 * made mme group ids as list
 *
 * Revision 1.1.2.6  2009/12/18 04:22:26  gur18569
 * setting ue id pair presence fields: spr 183
 *
 * Revision 1.1.2.5  2009/12/15 11:33:52  gur18569
 * removed check to return failure if any MME mesg didn't have either of MME UE S1APID of ENB UE S1APID
 *
 * Revision 1.1.2.4  2009/12/08 14:13:13  gur18569
 * using message id from ASN files SPR:161
 *
 * Revision 1.1.2.3  2009/12/04 13:55:46  gur18569
 * changes to s1 setup resp
 *
 * Revision 1.1.2.2  2009/11/30 08:52:31  gur18569
 * added ASN_MSG_TO_FWD test buffer gen code
 *
 * Revision 1.1.2.1  2009/11/25 13:11:04  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.11  2009/11/25 05:32:38  gur20470
 * Added a flag TESTING_BUFFER_GENERATION for generating asn messages
 *
 * Revision 1.1.2.10  2009/11/23 15:09:36  gur18569
 * initializing memory allocated from rtxallocnode
 *
 * Revision 1.1.2.9  2009/11/22 11:47:35  gur18569
 * initializing allcated memory
 *
 * Revision 1.1.2.8  2009/11/21 06:36:51  gur21006
 * Error check added
 *
 * Revision 1.1.2.7  2009/11/20 08:43:38  gur18569
 * added log strings
 *
 * Revision 1.1.2.6  2009/11/16 10:37:53  gur18569
 * setting p_protocolIE_elem->value.m.iE_ExtensionsPresent in reset ack to zero
 *
 * Revision 1.1.2.5  2009/11/10 11:41:55  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.4  2009/11/09 09:18:23  gur18569
 * removed compilation warnings
 *
 * Revision 1.1.2.3  2009/10/29 05:33:22  gur18569
 * added compose_paging for testing purpose
 *
 * Revision 1.1.2.2  2009/10/28 07:23:17  gur18569
 * changes during functional testing
 *
 * Revision 1.1.2.1  2009/10/23 16:10:17  gur18569
 * Initial version
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "s1ap_asn_codec.h"
#include "s1ap_logger.h"
#include "list_wrap.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "s1ap_error_indication.h"

#include "uecc_s1ap.h"
#include "s1ap_uecc.h"
#include "rrc_logging.h"

#include <lteMisc.h>
 /******************************************************************************
   Private Definitions
  *****************************************************************************/
#define MACRO_ENB_ID_BITSTRING_SIZE            20
#define HOME_ENB_ID_BITSTRING_SIZE            28
#define CSG_ID_BITSTRING_SIZE                27
#define OCTATE_NUM 8
#define MAX_PLMN_VAL                        0x09


#ifdef TESTING_BUFFER_GENERATION
#define RRC_FATAL       1
#define RRC_ERROR       1
#define RRC_WARNING     1
#define RRC_INFO        1
#define RRC_BRIEF       1
#define RRC_DETAILED    1
#define  RRC_S1AP_UT_TRACE_ENTER() RRC_PRINTF("Entering-->> %s \n",__FUNCTION__);
#define  RRC_S1AP_UT_TRACE_EXIT()  RRC_PRINTF("Exiting--->> %s \n",__FUNCTION__);

void write_log(U32 level,const S8* format, ...)
{
    va_list argList;

    S8 userMessageBuffer[100];

    size_t messageLength;

    VA_START(argList, format);
    vsnprintf_wrapper(userMessageBuffer, 100, format, argList);
    va_end_wrapper(argList);

    messageLength = strlen_wrapper((const S8 *)userMessageBuffer);
    if(userMessageBuffer[messageLength - 1] == '\n' && userMessageBuffer[messageLength - 2] != '\n') userMessageBuffer[messageLength - 1] = '\0';

    printf_wrapper("%s\n",userMessageBuffer);
}
#define  RRC_S1AP_TRACE(err_level, err_str,...)\
    write_log(err_level, err_str, ##__VA_ARGS__)
#endif


/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
rrc_return_et parse_asn_served_gummeis(
        parsed_gummei_info  *p_gummei_info,
        s1ap_ServedGUMMEIs  *p_asn_served_gummeis,
        s1ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id);

void parse_asn_reset_type(s1ap_ResetType  * p_reset_type_IE,
        s1ap_reset_info_t * p_reset_info);

rrc_return_et parse_asn_reset_ack_ueId_pair_list(
        s1ap_reset_info_t * p_reset_info,
        OSRTDList * p_list,
        s1ap_error_ind_ie_list_t *p_ie_list,
        U16 *p_index,
        s1ap_error_ind_bool_t *p_send_error_indication,
        rrc_bool_et *p_send_tse);

rrc_return_et parse_asn_reset_ack_ueId_pair_list_no_ei(
        s1ap_reset_info_t * p_reset_info,
        OSRTDList * p_list);

rrc_return_et compose_reset_ack_ueId_pair_list(OSCTXT * p_asn1_ctx,
        s1ap_reset_info_t * p_reset_info,
        s1ap_ResetAcknowledge_protocolIEs * p_list,
        s1ap_UE_associatedLogicalS1_ConnectionListResAck    
         *p_ue_assoc_log_con_list);

rrc_return_et compose_reset_info(OSCTXT * p_asn1_ctx,
        s1ap_reset_info_t * p_reset_info,
        s1ap_Reset_protocolIEs * p_list,
        OSRTDList               *p_ueIdPair_list,
        s1ap_ResetType          *p_reset_type);

rrc_return_et partially_decode_S1AP_EP(OSCTXT* pctxt, 
        s1ap_MME_UE_S1AP_ID * p_mmeUeS1APID,
        s1ap_ENB_UE_S1AP_ID * p_enbUeS1APID,
        U8* p_ue_id_pair_presence);

void determine_s1ap_EP_type(s1ap_ProcedureCode procedureCode,
        U32 message_type,
        s1ap_EP_et *p_s1ap_EP);

rrc_return_et partially_decode_S1AP_PDU (
        OSCTXT* pctxt, 
        s1ap_EP_et * p_s1ap_EP,
        s1ap_ENB_UE_S1AP_ID * p_mmeUeS1APID,
        s1ap_MME_UE_S1AP_ID * p_enbUeS1APID,
        U8* p_ue_id_pair_presence,
        s1ap_ProcedureCode *procedureCode,
        s1ap_message_type_et *msg_type);

rrc_return_et compose_csg_id_list(
        OSCTXT * p_asn1_ctx,
        OSRTDList * p_IE_csg_id_list,
        csg_id_list_t * p_csg_id_list);

rrc_return_et compose_supported_tas(
        OSCTXT * p_asn1_ctx,
        OSRTDList * p_supported_tas_IE,
        supp_ta_list_t * p_supp_tas_info);

rrc_return_et compose_global_enb_id(
        OSCTXT * p_asn1_ctx,
        s1ap_Global_ENB_ID * p_s1ap_IE_gb_enb_id,
        gb_enb_id_t * p_gb_enb_id);

U32  convert_asn_time_to_wait(
        s1ap_TimeToWait_Root asn_time_to_wait);

rrc_return_t compose_asn_plmn_identity(
        s1ap_PLMNidentity *pLMNidentity,
        plmn_identity_t   *plmn_id);

void s1ap_generate_plmn_identity_to_int(
        plmn_identity_t    *p_plmn_identity,
        OSOCTET                 *p_s1ap_plmn_identity);

rrc_return_et parse_s1ap_SONInformation( 
        s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,
        s1ap_SONInformation *p_SONInformation);

rrc_return_et compose_s1ap_target_enb_id(
        OSCTXT *p_asn1_ctx,
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_TargeteNB_ID *p_eNB_ID);

rrc_return_et compose_s1ap_source_enb_id(
        OSCTXT *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_SourceeNB_ID *p_eNB_ID);

rrc_return_et compose_s1ap_son_information(
        OSCTXT *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_SONInformation *p_son_info);

rrc_return_et compose_x2tnl_config_info_ie_extensions(
        OSCTXT *p_asn1_ctx,
        s1ap_X2TNLConfigurationInfo *p_tnl_info,
        s1ap_gb_context_t *p_s1ap_gb_ctx);

rrc_return_et parse_x2tnl_config_info_ie_extensions(
        s1ap_X2TNLConfigurationInfo_iE_Extensions *p_extnl_info,
        s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer);

rrc_return_et parse_s1ap_x2tnl_configuration(
        s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,
        s1ap_SONConfigurationTransfer_iE_Extensions *p_x2_tnl_son_config_info);

rrc_return_et compose_s1ap_x2tnl_configuration(
        OSCTXT *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        /*SPR 17777 +-*/
        s1ap_SONConfigurationTransfer_iE_Extensions *p_x2_tnl_info);
U32 s1ap_decode_overload_gummei_list(
        s1ap_gummei_t *p_gummei_list,
        s1ap_GUMMEIList *p_s1ap_gummei_list);

rrc_return_et s1ap_reset_ack_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_ResetAcknowledge *p_3gpp,
 s1ap_reset_info_t *p_intrl
 );
rrc_return_et s1ap_overload_start_intrl_dec
(
 mme_context_t *p_mme_ctx,
 s1ap_OverloadStart       *p_3gpp,    /* unpacked src */
 parsed_overload_start_t  *p_intrl   /* dest */
 );
rrc_return_et s1ap_enb_confguration_update_failure_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_ENBConfigurationUpdateFailure  *p_3gpp,    /* unpacked src */
 enb_config_update_failure_t  *p_intrl   /* dest */
 );

rrc_return_et s1ap_enb_config_update_ack_intrl_dec 
(
 mme_context_t *p_mme_context,
 s1ap_ENBConfigurationUpdateAcknowledge *p_3gpp    /* unpacked src */

 );
rrc_return_et s1ap_mme_direct_information_transfer_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_MMEDirectInformationTransfer        *p_3gpp,    /* unpacked src */
 mme_direct_info_t                        *p_intrl,   /* dest */
 s1ap_rrm_mme_direct_info_transfer_t    *p_s1ap_rrm_mme_direct_info_transfer
 );
rrc_return_et s1ap_overload_stop_intrl_dec
(
 mme_context_t *p_mme_ctx,
 s1ap_OverloadStop *p_3gpp,        /* unpacked src */
 parsed_overload_stop_t  *p_intrl
 );
rrc_return_et s1ap_mme_initiated_reset_intrl_dec
(
 mme_context_t *p_mme_ctx,
 s1ap_Reset      *p_3gpp,  /* unpacked src */
 reset_req_t     *p_intrl, /* dest */
 rrc_s1ap_error_indication_t *p_error_indication,
 s1ap_error_ind_bool_t *p_send_err_ind
 );
rrc_return_et s1ap_setup_failure_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_S1SetupFailure      *p_3gpp,  /* unpacked src */
 parsed_s1_setup_fail_t   *p_intrl /* dest */
 );
rrc_return_et s1ap_setup_resp_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_S1SetupResponse  *p_3gpp,    /* unpacked src */
 parsed_s1_setup_rsp_t *p_intrl /* dest */
 );

rrc_return_et rrc_s1ap_error_indication_intrl_dec
(
 s1ap_ErrorIndication    *p_3gpp,    /* unpacked src */
 rrc_s1ap_error_indication_t *p_intrl/* dest */
);

rrc_return_et check_plmn_validity(U8 index, U8 octateValue);

rrc_return_et s1ap_kill_req_intrl_dec
(
    s1ap_KillRequest    *p_3gpp,    /* unpacked src */
    rrc_s1ap_kill_request_t *p_intrl,/* dest */
    mme_context_t *p_mme_context
);


/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
U8 time_to_wait_value[] ={1,2,5,10,20,60};

/*****************************************************************************
 * Function Name  : compose_asn_s1_setup_req
 * Inputs         : p_s1_setup_req - Information from which Asn message is
 *                                   created.
 *                  p_asn_msg -  Pointer to The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function composes S1 SETUP REQUEST Asn Message from
 *                  the information provided in p_s1_setup_req
 *****************************************************************************/
rrc_return_et compose_asn_s1_setup_req (
        s1_setup_req_t   *p_s1_setup_req,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    OSRTDListNode               *p_node = PNULL;
    s1ap_S1SetupRequest_protocolIEs_element *     p_protocolIE_elem = PNULL;
    s1ap_S1SetupRequest            * p_s1ap_S1SetupRequest = PNULL;

    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDList  s1SetupRequestIEs_3;
    OSRTDList  s1SetupRequestIEs_5;
    EVENT_EXTERNAL_HEADER                       *p_event_header = PNULL;
    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: S1SetupRequest");
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_S1Setup;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.initiatingMessage->value.t =  T1_s1Setup;

        p_s1ap_S1SetupRequest = 
                  rtxMemAllocType(&asn1_ctx, s1ap_S1SetupRequest);
        if (PNULL == p_s1ap_S1SetupRequest)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_S1SetupRequest (p_s1ap_S1SetupRequest);

        s1ap_pdu.u.initiatingMessage->value.u.s1Setup = p_s1ap_S1SetupRequest;


        /*** Compose id-Global-ENB-ID start ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupRequest_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        if (PNULL == p_protocolIE_elem)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",
                    __FUNCTION__);
            break;
        }
        
        asn1Init_s1ap_S1SetupRequest_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_Global_ENB_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T74__S1SetupRequestIEs_1;

        p_protocolIE_elem->value.u._S1SetupRequestIEs_1 = 
            rtxMemAllocType(&asn1_ctx,s1ap_Global_ENB_ID);
        if (PNULL == p_protocolIE_elem->value.u._S1SetupRequestIEs_1)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_Global_ENB_ID (p_protocolIE_elem->value.u.
                                             _S1SetupRequestIEs_1);

        if ( RRC_FAILURE == compose_global_enb_id(&asn1_ctx,
                    p_protocolIE_elem->value.u._S1SetupRequestIEs_1,
                    &p_s1_setup_req->gb_enb_id))
        {
            RRC_S1AP_TRACE(RRC_ERROR, 
                    "%s:compose_global_enb_id returned Failure.",
                    __FUNCTION__);
            break;
        }

        rtxDListAppendNode(&p_s1ap_S1SetupRequest->protocolIEs, p_node);
        /*** Compose id-Global-ENB-ID end ***/


        /*** Compose id-eNBname start ***/
        if (p_s1_setup_req->bitmask & S1AP_OAM_ENB_NAME_PRESENT)
        {
            rtxDListAllocNodeAndData(&asn1_ctx, 
                    s1ap_S1SetupRequest_protocolIEs_element, 
                    &p_node, 
                    &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",
                        __FUNCTION__);
                break;
            }
            asn1Init_s1ap_S1SetupRequest_protocolIEs_element (
                    p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_s1ap_id_eNBname;
            p_protocolIE_elem->criticality = s1ap_ignore;

            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T74__S1SetupRequestIEs_2;
            p_protocolIE_elem->value.u._S1SetupRequestIEs_2 =
                (s1ap_ENBname)p_s1_setup_req->enb_name;

            rtxDListAppendNode(&p_s1ap_S1SetupRequest->protocolIEs, p_node);
        }
        /*** Compose id-eNBname end ***/

        /*** Compose id-SupportedTAs start ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupRequest_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",
                    __FUNCTION__);
            break;
        }

        
        asn1Init_s1ap_S1SetupRequest_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_SupportedTAs;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T74__S1SetupRequestIEs_3;

        rtxDListInit(&s1SetupRequestIEs_3);
        p_protocolIE_elem->value.u._S1SetupRequestIEs_3  = 
                                               &s1SetupRequestIEs_3;
        if (RRC_FAILURE == compose_supported_tas(&asn1_ctx,
                    p_protocolIE_elem->value.u._S1SetupRequestIEs_3,
                    &p_s1_setup_req->supp_ta_list))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s compose_supported_tas "
                         "returned Failure.",
                    __FUNCTION__);
            break;
        }

        rtxDListAppendNode(&p_s1ap_S1SetupRequest->protocolIEs, p_node);
        /*** Compose id-SupportedTAs end ***/

        /*** Compose id-DefaultPagingDRX start ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupRequest_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_S1SetupRequest_protocolIEs_element (
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_DefaultPagingDRX;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T74__S1SetupRequestIEs_4;

        p_protocolIE_elem->value.u._S1SetupRequestIEs_4  = 
            p_s1_setup_req->default_paging_drx;

        rtxDListAppendNode(&p_s1ap_S1SetupRequest->protocolIEs, p_node);
        /*** Compose id-DefaultPagingDRX end ***/

        /*** Compose id-CSG-IdList start ***/
        if (p_s1_setup_req->bitmask &
                S1AP_OAM_CSG_ID_LIST_PRESENT) 
        {
            rtxDListAllocNodeAndData(&asn1_ctx, 
                    s1ap_S1SetupRequest_protocolIEs_element, 
                    &p_node, 
                    &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                break;
            }
            asn1Init_s1ap_S1SetupRequest_protocolIEs_element (
                    p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_s1ap_id_CSG_IdList;
            p_protocolIE_elem->criticality = s1ap_reject;

            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T74__S1SetupRequestIEs_5;

            rtxDListInit(&s1SetupRequestIEs_5);
            p_protocolIE_elem->value.u._S1SetupRequestIEs_3  = 
                                               &s1SetupRequestIEs_5;
            if ( RRC_FAILURE == compose_csg_id_list(&asn1_ctx,
                        p_protocolIE_elem->value.u._S1SetupRequestIEs_5,
                        &p_s1_setup_req->csg_id_list))
            {
                RRC_S1AP_TRACE(RRC_ERROR, 
                        "compose_csg_id_list returned Failure.");
                break;
            }

            rtxDListAppendNode(&p_s1ap_S1SetupRequest->protocolIEs, p_node);
        }
        /*** Compose id-CSG-IdList end ***/

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_S1_SETUP_REQUEST, MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}


/*****************************************************************************
 * Function Name  : compose_global_enb_id
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_s1ap_IE_gb_enb_id - pointer to s1ap_Global_ENB_ID
 *                  p_gb_enb_id - This contains the information to
 *                                        encode global eNB ID.
 *                  p_s1ap_IE_gb_enb_id - pointer of type s1ap_Global_ENB_ID
 * Outputs        : p_s1ap_IE_gb_enb_id - This structure is populated with
 *                                        information from p_gb_enb_id.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_s1ap_IE_gb_enb_id with 
 *                  information necessary to encode global eNb ID.
 *****************************************************************************/
rrc_return_et compose_global_enb_id(
        OSCTXT * p_asn1_ctx,
        s1ap_Global_ENB_ID * p_s1ap_IE_gb_enb_id,
        gb_enb_id_t * p_gb_enb_id)
{

    RRC_S1AP_UT_TRACE_ENTER();

    /* Set the PLMN ID field */
    p_s1ap_IE_gb_enb_id->pLMNidentity.numocts = MAX_PLMN_ID_BYTES;

    l3_memcpy_wrapper(p_s1ap_IE_gb_enb_id->pLMNidentity.data,
            p_gb_enb_id->plmn_identity.plmn_id,
            MAX_PLMN_ID_BYTES);

    /* If Macro eNB id present set the value */
    if (p_gb_enb_id->enb_id.presence_bitmask & S1AP_OAM_MACRO_ENB_ID_PRESENT)
    {

        p_s1ap_IE_gb_enb_id->eNB_ID.t = T_s1ap_ENB_ID_macroENB_ID;

        p_s1ap_IE_gb_enb_id->eNB_ID.u.macroENB_ID = 
                   rtxMemAllocType(p_asn1_ctx, ASN1BitStr32);
        if (PNULL == p_s1ap_IE_gb_enb_id->eNB_ID.u.macroENB_ID)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return RRC_FAILURE;
        }
        memset_wrapper(p_s1ap_IE_gb_enb_id->eNB_ID.u.macroENB_ID, RRC_NULL, sizeof(ASN1BitStr32));

        p_s1ap_IE_gb_enb_id->eNB_ID.u.macroENB_ID->numbits = 
                                      MACRO_ENB_ID_BITSTRING_SIZE;

        l3_memcpy_wrapper( p_s1ap_IE_gb_enb_id->eNB_ID.u.macroENB_ID->data,
                p_gb_enb_id->enb_id.macro_enb_id.eNB_id,
                MACRO_ENB_ID_OCTET_SIZE);
    }
    else if (p_gb_enb_id->enb_id.presence_bitmask & 
                          S1AP_OAM_HOME_ENB_ID_PRESENT)
    {
        /* Else if Home eNB id present set the value */
        p_s1ap_IE_gb_enb_id->eNB_ID.t = T_s1ap_ENB_ID_homeENB_ID;

        p_s1ap_IE_gb_enb_id->eNB_ID.u.homeENB_ID = 
                           rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
        if (PNULL == p_s1ap_IE_gb_enb_id->eNB_ID.u.homeENB_ID)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return RRC_FAILURE;
        }
        memset_wrapper(p_s1ap_IE_gb_enb_id->eNB_ID.u.homeENB_ID, RRC_NULL, sizeof(ASN1BitStr32));

        p_s1ap_IE_gb_enb_id->eNB_ID.u.homeENB_ID->numbits = 
                                       HOME_ENB_ID_BITSTRING_SIZE;

        l3_memcpy_wrapper( p_s1ap_IE_gb_enb_id->eNB_ID.u.homeENB_ID->data,
                p_gb_enb_id->enb_id.home_enb_id.eNB_id,
                HOME_ENB_ID_OCTET_SIZE);
    }
    else
    {
        /* eNB ID presence bitmask is not set */
        RRC_S1AP_TRACE(RRC_ERROR, 
                "%s: neither Macro or Home ENB ID "
                "Presence bitmask not set",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}



/*****************************************************************************
 * Function Name  : compose_supported_tas
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_supported_tas_IE - pointer to OSRTDList 
 *                  p_supp_tas_info - This contains the information to
 *                                    encode Supported TAs.
 * Outputs        : p_supported_tas_IE - This structure is populated with
 *                                        information from p_supp_tas_info.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_supported_tas_IE with 
 *                  information necessary to encode Supported TAs.
 *****************************************************************************/
rrc_return_et compose_supported_tas(
        OSCTXT * p_asn1_ctx,
        OSRTDList * p_supported_tas_IE,
        supp_ta_list_t * p_supp_tas_info)
{
    OSRTDListNode               *p_node = PNULL;
    s1ap_SupportedTAs_Item      *p_supp_tas_item = PNULL;
    U16                              ta_count = 0;
    U16                              bplmn_count = 0;
    bc_plmn_list_t              * p_bplmn_list = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_supported_tas_IE);
    RRC_ASSERT(PNULL != p_supp_tas_info);

    if (!p_supp_tas_info->num_supported_tais)
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:Num of supported TAIs is zero.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    for (ta_count = 0 ; ta_count < p_supp_tas_info->num_supported_tais; 
            ta_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                s1ap_SupportedTAs_Item, 
                &p_node, 
                &p_supp_tas_item);

        if (PNULL == p_node)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",
                    __FUNCTION__);
            RRC_S1AP_UT_TRACE_EXIT();
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_s1ap_SupportedTAs_Item (p_supp_tas_item);

        /* Compose TAC */
        p_supp_tas_item->tAC.numocts = TAC_OCTET_SIZE;
        l3_memcpy_wrapper(p_supp_tas_item->tAC.data,
                p_supp_tas_info->supp_tais[ta_count].tac,
                TAC_OCTET_SIZE);

        p_bplmn_list = &(p_supp_tas_info->supp_tais[ta_count].bc_plmn_list);

        if (!p_bplmn_list->num_bplmn)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:Num of Broadcast PLMNs is zero.",
                    __FUNCTION__);
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        /* Compose Broadcast PLMNS */
        p_supp_tas_item->broadcastPLMNs.n = p_bplmn_list->num_bplmn;

        for (bplmn_count = 0; bplmn_count < p_supp_tas_item->broadcastPLMNs.n;
                bplmn_count++)
        {
            /* Set the PLMN ID field */
            p_supp_tas_item->broadcastPLMNs.elem[bplmn_count].numocts = 
                                                          MAX_PLMN_ID_BYTES;

            l3_memcpy_wrapper(p_supp_tas_item->broadcastPLMNs.elem[bplmn_count].data,
                    p_bplmn_list->plmn_identity[bplmn_count].plmn_id,
                    MAX_PLMN_ID_BYTES);
        }


        rtxDListAppendNode(p_supported_tas_IE, p_node);
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : compose_csg_id_list
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_IE_csg_id_list - pointer to the OSRTDList
 *                  p_csg_id_list - This contains the information to
 *                                        encode CSG ID list.
 * Outputs        : p_IE_csg_id_list - This structure is populated with
 *                                        information from p_csg_id_list.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_s1ap_IE_gb_enb_id with 
 *                  information necessary to encode CSG Id list.
 *****************************************************************************/
rrc_return_et compose_csg_id_list(
        OSCTXT * p_asn1_ctx,
        OSRTDList * p_IE_csg_id_list,
        csg_id_list_t * p_csg_id_list)
{
    OSRTDListNode               *p_node = PNULL;
    s1ap_CSG_IdList_Item        *p_csg_id_item = PNULL;
    U16                              csg_id_count = 0;


    RRC_S1AP_UT_TRACE_ENTER();

    for (csg_id_count = 0 ; csg_id_count < p_csg_id_list->num_csg_ids;
            csg_id_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                s1ap_CSG_IdList_Item, 
                &p_node, 
                &p_csg_id_item);

        if (PNULL == p_node)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            RRC_S1AP_UT_TRACE_EXIT();
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_s1ap_CSG_IdList_Item (p_csg_id_item);

        /* Compose TAC */
        p_csg_id_item->cSG_Id.numbits = CSG_ID_BITSTRING_SIZE;
        l3_memcpy_wrapper(p_csg_id_item->cSG_Id.data,
                p_csg_id_list->csg_ids[csg_id_count].csg_id,
                CSG_ID_OCTET_SIZE);

        rtxDListAppendNode(p_IE_csg_id_list, p_node);
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : parse_asn_s1_setup_response
 * Inputs         : p_s1ap_gb_ctx - pointer to the S1AP global context
 *                  p_mme_ctx - pointer to MME context 
 *                  p_asn_msg - Pointer to S1 SETUP RESPONSE Asn message to be
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_parsed_s1_setup_rsp - pointer to the parsed S1AP setup
 *                                          responce 
 * Outputs        : p_parsed_s1_setup_rsp - This structure is populated with
 *                                        information from input Asn Message.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_parsed_s1_setup_rsp with 
 *                  information received after decoding S1 SETUP REQUEST 
 *                  Asn Messages.
 *****************************************************************************/
rrc_return_et parse_asn_s1_setup_response (
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        parsed_s1_setup_rsp_t  * p_parsed_s1_setup_rsp)
{
    S1AP_PDU                        s1ap_pdu;
    OSCTXT                          asn1_ctx;
    rrc_return_et                   result = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
 
    RRC_S1AP_TRACE(RRC_DETAILEDALL, "Unused variable(global context):%p",p_s1ap_gb_ctx);
    if (!p_parsed_s1_setup_rsp)
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "p_parsed_s1_setup_rsp is NULL",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: S1SetupResponse %s", change_data_to_str(asn_msg_len, p_asn_msg));
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }
        else
        {
           
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                               (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_SETUP_RESPONSE, MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* Decode message */
        if( RRC_FAILURE == s1ap_setup_resp_intrl_dec(
                    p_mme_ctx,
                    s1ap_pdu.u.successfulOutcome->value.u.s1Setup,
                    p_parsed_s1_setup_rsp
                    ))
        {
            result = RRC_FAILURE;
            break;
        }


    }while (0);

   /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}
/****************************************************************************
 * Function Name  : s1ap_setup_resp_intrl_dec
 * Inputs         : p_mme_context - pointer to the MME context
 *                  p_intrl - pointer to the parsed S1AP setup responce
 *                  p_3gpp - pointer to decoded S1  Setup Response  
 * Outputs        : p_intrl   
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : Gets the Decoded S1 Setup Response
 ****************************************************************************/
rrc_return_et s1ap_setup_resp_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_S1SetupResponse  *p_3gpp,    /* unpacked src */
 parsed_s1_setup_rsp_t *p_intrl /* dest */
 )
{
    U16                      length        = RRC_NULL;
    OSRTDListNode            *p_node       = PNULL;
    U32                      index = 0;
    OSCTXT asn1_ctx;
    s1ap_S1SetupResponse_protocolIEs_element     
        *p_protocol_IE = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;

    s1ap_message_data_t message_map =
    {5, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_MMEname, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_ServedGUMMEIs,s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_s1ap_id_RelativeMMECapacity,s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {3, ASN1V_s1ap_id_MMERelaySupportIndicator, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {4, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    p_intrl->bitmask = 0;

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

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
                (s1ap_S1SetupResponse_protocolIEs_element *)
                p_node->data;        

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MMEname:
                    {
                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_MMEname",__FUNCTION__);

                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._S1SetupResponseIEs_1
                                    ))
                        {

                            length = (U16)strlen_wrapper(
                                    ((const Char8 *)(p_protocol_IE->value.u._S1SetupResponseIEs_1)));

                            if ((MAX_MME_NAME_STR > length) && (RRC_NULL < length))
                            {
                                l3_memcpy_wrapper(p_intrl->p_mme_name,
                                        (p_protocol_IE->value.u._S1SetupResponseIEs_1),
                                        length);
                                p_intrl->bitmask |= 
                                    S1AP_S1_SETUP_RESP_MME_NAME_PRESENT;
                            }
                        }
                    }
                    break;

                case ASN1V_s1ap_id_ServedGUMMEIs:
                    {
                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_ServedGUMMEIs",__FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._S1SetupResponseIEs_2
                                    ))
                        {
                            p_intrl->gummei_info.p_served_gummeis = rrc_mem_get(sizeof(served_gummei_info_t)*                         
                                    ASN1V_s1ap_maxnoofRATs);                                     

                            if (!p_intrl->gummei_info.p_served_gummeis)                          
                            {
                                RRC_S1AP_TRACE(RRC_FATAL,"%s: " 
                                        "Unable to allocatee Mem for served_gummeis",            
                                        __FUNCTION__);                      
                                update_message_map(&message_map, DATA_MISSING, index, p_protocol_IE->id);
                                break;
                            }
                            else
                            {
                                memset_wrapper(p_intrl->gummei_info.p_served_gummeis, 0,
                                        sizeof(served_gummei_info_t) * ASN1V_s1ap_maxnoofRATs);

                                if( RRC_FAILURE == parse_asn_served_gummeis(&p_intrl->gummei_info,                  
                                            p_protocol_IE->                                          
                                            value.u._S1SetupResponseIEs_2,
                                            &message_map,
                                            (U8)index,
                                            (U8)p_protocol_IE->id))
                                {
                                    break;
                                }

                            }
                        }

                    }
                    break;

                case ASN1V_s1ap_id_RelativeMMECapacity:
                    {
                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_RelativeMMECapacity",__FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._S1SetupResponseIEs_3
                                    ))
                        {
                            p_intrl->relative_mme_capacity = p_protocol_IE->value.u._S1SetupResponseIEs_3;

                        }

                    }
                    break;

                case ASN1V_s1ap_id_MMERelaySupportIndicator:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._S1SetupResponseIEs_4
                                    ))
                        {
                            RRC_TRACE(RRC_DETAILED,"%s: "
                                    "ASN1V_s1ap_id_MMERelaySupportIndicator  - NOT HANDLED",__FUNCTION__);
                        }
                    }
                    break;

                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        RRC_TRACE(RRC_DETAILED,"%s: "
                                "ASN1V_s1ap_id_CriticalityDiagnostics",
                                __FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._S1SetupResponseIEs_5
                                    ))
                        {
                            p_intrl->bitmask |= 
                                S1AP_S1_SETUP_RESP_CRITICALITY_DIAGNOSTICS_PRESENT;

                            log_criticality_diagostics(p_protocol_IE->value.u._S1SetupResponseIEs_5);
                        }

                    }
                    break;

                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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
    } while (0);

    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_S1Setup,
                    T_S1AP_PDU_successfulOutcome,
                    s1ap_reject,
                    (rrc_s1ap_error_indication_t *)PNULL)) ) 
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "S1 Setup response - Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}


/*****************************************************************************
 * Function Name  : parse_asn_served_gummeis
 * Inputs         : p_gummei_info - pointer to parsed_gummei_info
 *                  p_asn_served_gummeis - Asn list of served GUMMEIs
 *                  p_ie_order_map - pointer to S1AP message data 
 *                  order_index - type of char
 *                  id - type of char 
 * Outputs        : p_gummei_info - This structure is populated with
 *                                        information from input Asn Message.
 * Returns        : RRC_SUCCESS - Asn decoding was successful.
 *                  RRC_FAILURE - Asn decoding was not successful.
 * Description    : This function populates p_parsed_s1_setup_rsp with 
 *                  served gummei info.
 *****************************************************************************/
rrc_return_et parse_asn_served_gummeis(
        parsed_gummei_info  *p_gummei_info,
        s1ap_ServedGUMMEIs  *p_asn_served_gummeis,
        s1ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id)
{
    U32 index             = RRC_NULL;
    U32 served_plmn_count = RRC_NULL;
    U32 count             = RRC_NULL;
    U32 served_grp_id_index = RRC_NULL;
    U32 served_grp_id_count = RRC_NULL;

    OSRTDListNode                   *p_node = PNULL;
    mme_group_id_list_node_t * p_mme_grp_id_node = PNULL;
    OSRTDListNode                   *p_served_grp_id_node = PNULL;
    served_gummei_info_t * p_served_gummei_info = PNULL;

    rrc_return_et  result    = RRC_SUCCESS;

    p_served_gummei_info = p_gummei_info->p_served_gummeis;
    p_node = p_asn_served_gummeis->head;

    if ((U8)(p_asn_served_gummeis->count) > MAX_RAT_CAPABILITY )
    {

        RRC_S1AP_TRACE(RRC_ERROR,"%s: "
                "p_gummei_info->num_served_gummeis is > MAX_RAT_CAPABILITY",
                __FUNCTION__);
        update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
        result = RRC_FAILURE;
        return result; 
    }

    p_gummei_info->num_served_gummeis = (U8)(p_asn_served_gummeis->count);

    /* Save the decoded Protocol IEs */
    for (index =0; index < p_asn_served_gummeis->count; index++)
    {
        /* List changes */
        list_init(&p_served_gummei_info[index].mme_group_id_list);

        if (!p_node)
        {
            RRC_S1AP_TRACE(RRC_ERROR,"%s: "
                    "next node returned NULL",
                    __FUNCTION__);

            update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            result = RRC_FAILURE;

            return result; 
        }

        p_served_gummei_info[index].servedPLMNs = ((s1ap_ServedGUMMEIsItem *)
                p_node->data)->servedPLMNs;

        if (p_served_gummei_info[index].servedPLMNs.n == RRC_NULL)
        {
            update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            result = RRC_FAILURE;
            return result; 
        }
        for (served_plmn_count = 0; 
             served_plmn_count < p_served_gummei_info[index].servedPLMNs.n; 
             served_plmn_count++)
        {
            for ( count= 0; 
                  count < p_served_gummei_info[index].servedPLMNs.elem[served_plmn_count].numocts;
                  count++)
            {
                /* SPR 1643 covered in this error indication code */
                if (RRC_FAILURE == 
                        check_plmn_validity((U8)count, 
                            p_served_gummei_info[index].servedPLMNs.elem[served_plmn_count].data[count]))
                {
                    update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
                    result = RRC_FAILURE;
                    return result;
                }
            }
        }

        p_served_grp_id_node = ((s1ap_ServedGUMMEIsItem *) p_node->data)->
            servedGroupIDs.head;
        served_grp_id_count = ((s1ap_ServedGUMMEIsItem *) p_node->data)->
            servedGroupIDs.count;

        p_served_gummei_info[index].num_of_mme_group_ids = served_grp_id_count;

        /* List changes */

        if (served_grp_id_count == RRC_NULL)
        {
            update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            result = RRC_FAILURE;
            return result; 
        }
        for (served_grp_id_index = 0;
                served_grp_id_index < served_grp_id_count ;
                served_grp_id_index++)
        {
            if (!p_served_grp_id_node)
            {
                RRC_S1AP_TRACE(RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
                result = RRC_FAILURE;

                return result; 
            }

            /* List changes */
            p_mme_grp_id_node = rrc_mem_get(sizeof(mme_group_id_list_node_t));
            if(PNULL == p_mme_grp_id_node)
            {
                RRC_S1AP_TRACE(RRC_FATAL,"[%s]Memory Allocation Failed:", __FUNCTION__);
                return RRC_FAILURE;
            }
            l3_memcpy_wrapper(&p_mme_grp_id_node->mme_group_id,
                     (s1ap_MME_Group_ID *)(p_served_grp_id_node->data),
                    sizeof(s1ap_MME_Group_ID));
            
            list_push_tail(&p_served_gummei_info[index].mme_group_id_list,
                              &p_mme_grp_id_node->node);

            /* List changes */

            p_served_grp_id_node = p_served_grp_id_node->next;
        }

        p_served_gummei_info[index].served_mmecs = ((s1ap_ServedGUMMEIsItem *)
                p_node->data)->servedMMECs;

        p_node = p_node->next;
    }

    return result;
}



/*****************************************************************************
 * Function Name  : s1ap_partially_decode_asn_msg
 * Inputs         : p_asn_msg - Pointer to Asn message to be partially
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_s1ap_EP - pointer to s1ap EP
 *                  p_mmeUeS1APID - ponter to s1ap_MME_UE_S1AP_ID
 *                  p_enbUeS1APID - pointer to p_enbUeS1APID
 *                  p_ue_id_pair_presence - pointer of type unsigned char 
 *                  procedureCode - pointer to the s1ap ProcedureCode
 *                  message_type - pointer to the S1AP message type
 * Outputs        : p_s1ap_EP - The S1AP EP received.
 *                  p_mmeUeS1APID - The mmeUeS1apId in the Asn Message if 
 *                                  present.
 *                  p_enbUeS1APID - The enbUeS1apId in the Asn Message if 
 *                                  present.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function when called partially decodes a Asn message
 *                  and returns S1AP EP type , mmeUeS1apId and enbUeS1apId.
 *****************************************************************************/
rrc_return_et s1ap_partially_decode_asn_msg (
        U8 * p_asn_msg,
        U16 asn_msg_len,
        s1ap_EP_et * p_s1ap_EP,
        s1ap_MME_UE_S1AP_ID * p_mmeUeS1APID,
        s1ap_ENB_UE_S1AP_ID * p_enbUeS1APID,
        U8 * p_ue_id_pair_presence,
        s1ap_ProcedureCode *procedureCode,
        s1ap_message_type_et  *message_type)
{
    OSCTXT asn1_ctx;

    RRC_S1AP_UT_TRACE_ENTER();
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "s1ap_partially_decoded_t: ASN "
                "context initialization failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    if( RRC_FAILURE == partially_decode_S1AP_PDU(&asn1_ctx,
                p_s1ap_EP,
                p_mmeUeS1APID,
                p_enbUeS1APID,
                p_ue_id_pair_presence,
                procedureCode,
                message_type))
    {
        RRC_S1AP_TRACE(RRC_ERROR, "s1ap_partially_decoded_t: ASN "
                "partially_decode_S1AP_PDU returned failure.");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : partially_decode_S1AP_PDU
 * Inputs         : pctxt - Pointer to OSCTXT.
 *                  p_s1ap_EP - Pointer to  S1AP EP.
 *                  p_mmeUeS1APID - pointer to the s1ap_MME_UE_S1AP_ID
 *                  p_enbUeS1APID - pointer to the s1ap_ENB_UE_S1AP_ID
 *                  p_ue_id_pair_presence - poninter to p_ue_id_pair_presence
 *                  procedureCode - pointer to the s1ap ProcedureCode
 *                  msg_type - pointer to the s1ap message type
 * Outputs        : p_s1ap_EP - The S1AP EP received.
 *                  p_mmeUeS1APID - The mmeUeS1apId in the Asn Message if 
 *                                  present.
 *                  p_enbUeS1APID - The enbUeS1apId in the Asn Message if 
 *                                  present.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function when called partially decodes an Asn message
 *                  and returns S1AP EP type , mmeUeS1apId and enbUeS1apId.
 *                  This function decodes the procedureCode and fill p_s1ap_EP
 *                  by calling determine_s1ap_EP_type. And when EP type is 
 *                  known it calls partially_decode_S1AP_EP to decode 
 *                  enbUeS1apId and mmeUeS1apId.
 *****************************************************************************/
rrc_return_et partially_decode_S1AP_PDU (
        OSCTXT* pctxt, 
        s1ap_EP_et * p_s1ap_EP,
        s1ap_MME_UE_S1AP_ID * p_mmeUeS1APID,
        s1ap_ENB_UE_S1AP_ID * p_enbUeS1APID,
        U8* p_ue_id_pair_presence,
        s1ap_ProcedureCode *procedureCode,
        s1ap_message_type_et *msg_type)
{
    S32 stat = 0;
    OSUINT32 message_type = 0;
    OSBOOL extbit = FALSE;
    s1ap_Criticality criticality;

    RRC_S1AP_UT_TRACE_ENTER();

    if (!pctxt || !p_s1ap_EP || !p_mmeUeS1APID || !p_enbUeS1APID )
    {
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* extension bit */
    stat = DEC_BIT (pctxt, &extbit);
    if (stat != 0) return RRC_FAILURE;

    if (!extbit) 
    {
        stat = pd_ConsUnsigned (pctxt, &message_type, 0, OSUINTCONST(2));
        if (stat != 0) return RRC_FAILURE;

        message_type++;

        switch (message_type) 
        {
            case T_S1AP_PDU_initiatingMessage:
                /* initiatingMessage */
                *msg_type = INITIATING_MESSAGE;
                break;
            case T_S1AP_PDU_successfulOutcome:
                /* successfulOutcome */
                *msg_type = SUCCESSFUL_OUTCOME;
                break;
            case T_S1AP_PDU_unsuccessfulOutcome:
                /* unsuccessfulOutcome */
                *msg_type = UNSUCCESSFUL_OUTCOME;
                break;

            default:
                return RRC_FAILURE;
        }

        /* Decode the procedure code */
        stat = asn1PD_s1ap_ProcedureCode (pctxt, procedureCode);
        if (stat != 0) return RRC_FAILURE;

        determine_s1ap_EP_type(*procedureCode,
                message_type,
                p_s1ap_EP);

        /* decode criticality */
        stat = asn1PD_s1ap_Criticality (pctxt, &criticality);
        if (stat != 0) return RRC_FAILURE;

        /* decode value */
        { OSCTXT lctxt;
            ASN1OpenType openType;

            stat = pd_OpenType (pctxt, &openType.data, &openType.numocts);
            if (stat != 0) return RRC_FAILURE;

            /* decode open type data into specific type field */

            rtxCopyContext (&lctxt, pctxt);

            stat = pd_setp (pctxt, (OSOCTET*)openType.data,
                    openType.numocts, pctxt->buffer.aligned);

            if (RRC_FAILURE == partially_decode_S1AP_EP(pctxt,
                        p_mmeUeS1APID,
                        p_enbUeS1APID,
                        p_ue_id_pair_presence))
            {
                return RRC_FAILURE;
            }
        }
    }
    else 
    {
        return RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : determine_s1ap_EP_type
 * Inputs         : procedureCode - S1Procedure code
 *                  message_type  - type of S1AP message
 *                  p_s1ap_EP     - pointer to the p_s1ap_EP
 * Outputs        : p_s1ap_EP - The S1AP EP received.
 * Returns        : RRC_SUCCESS - Decoded successfully
 *                  RRC_FAILURE - Decode failed.
 * Description    : This function determines S1AP EP type.
 *****************************************************************************/
void determine_s1ap_EP_type(s1ap_ProcedureCode procedureCode,
        U32 message_type,
        s1ap_EP_et *p_s1ap_EP)
{
    RRC_S1AP_UT_TRACE_ENTER();
    switch(message_type)
    {
        case T_S1AP_PDU_initiatingMessage:
            /* initiatingMessage */
            switch(procedureCode)
            {
                case ASN1V_s1ap_id_Reset:
                    *p_s1ap_EP = RESET;
                    break;

                case ASN1V_s1ap_id_Paging:
                    *p_s1ap_EP = S1_PAGING;
                    break;

                case ASN1V_s1ap_id_uplinkNASTransport:
                    *p_s1ap_EP = (s1ap_EP_et)ASN1V_s1ap_id_uplinkNASTransport;
                    break;
                    /*REL 1.2:HANDOVER PROCEDURE START*/                
                case ASN1V_s1ap_id_HandoverResourceAllocation:
                    *p_s1ap_EP = HO_RESOURCE_ALLOCATION;
                    break;
                    /*REL 1.2:HANDOVER PROCEDURE END*/                

                case ASN1V_s1ap_id_OverloadStart:
                    *p_s1ap_EP = MME_OVERLOAD_START;
                    break;

                case ASN1V_s1ap_id_OverloadStop:
                     *p_s1ap_EP = MME_OVERLOAD_STOP;
                     break;

                case ASN1V_s1ap_id_MMEConfigurationUpdate:
                     *p_s1ap_EP = MME_CONFIGURATION_UPDATE;
                     break;

                case ASN1V_s1ap_id_MMEDirectInformationTransfer:
                     *p_s1ap_EP = MME_DIRECT_INFO_TRANSFER;
                     break;

                case ASN1V_s1ap_id_MMEConfigurationTransfer:
                     *p_s1ap_EP = MME_CONFIGURATION_TRANSFER;
                     break;

                case ASN1V_s1ap_id_LocationReportingControl:
                     *p_s1ap_EP = LOCATION_REPORTING_CONTROL;
                     break;

                case ASN1V_s1ap_id_ErrorIndication:
                     *p_s1ap_EP = ERROR_INDICATION;
                     break;

                case ASN1V_s1ap_id_WriteReplaceWarning:
                     *p_s1ap_EP = WRITE_REPLACE_WARNING;
                     break;

                case ASN1V_s1ap_id_Kill:
                     *p_s1ap_EP = KILL_REQUEST ;
                     break ;

                case ASN1V_s1ap_id_downlinkUEAssociatedLPPaTransport:
                     *p_s1ap_EP = LPPA_UE_ASSO_DL_TRANSPORT;
                     break;

                case ASN1V_s1ap_id_downlinkNonUEAssociatedLPPaTransport:
                     *p_s1ap_EP = LPPA_NON_UE_ASSO_DL_TRANSPORT;
                     break ;
                
                default:
                    *p_s1ap_EP = ASN_MSG_TO_FORWARD;
            }

            break;
        case T_S1AP_PDU_successfulOutcome:
            /* successfulOutcome */
            switch(procedureCode)
            {
                case ASN1V_s1ap_id_S1Setup:
                    *p_s1ap_EP = S1_SETUP_RESPONSE;
                    break;

                case ASN1V_s1ap_id_Reset:
                    *p_s1ap_EP = RESET_ACK;
                    break;

                case ASN1V_s1ap_id_ENBConfigurationUpdate:
                     *p_s1ap_EP = eNB_CONFIGURATION_UPDATE_ACK;
                     break;

                default:
                    *p_s1ap_EP = ASN_MSG_TO_FORWARD;
            }

            break;
        case T_S1AP_PDU_unsuccessfulOutcome:
            /* unsuccessfulOutcome */
            switch(procedureCode)
            {
                case ASN1V_s1ap_id_S1Setup:
                    *p_s1ap_EP = S1_SETUP_FAILURE;
                    break;

                case ASN1V_s1ap_id_ENBConfigurationUpdate:
                     *p_s1ap_EP = eNB_CONFIGURATION_UPDATE_FAILURE;
                     break;

                default:
                    *p_s1ap_EP = ASN_MSG_TO_FORWARD;
            }

            break;

        default:
            RRC_S1AP_TRACE(RRC_ERROR, "%s: Partial Dec "
                    "INVALID_S1AP_EP.",__FUNCTION__);
    }

    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : decode_s1ap_ue_s1ap_ids
 * Inputs         : pctxt - Pointer to OSCTXT.
 *                  p_mmeUeS1APID - pointer to the s1ap_MME_UE_S1AP_ID
 *                  p_enbUeS1APID - pointer to the s1ap_ENB_UE_S1AP_ID
 *                  p_ue_id_pair_presence - pointer to ue_id_pair_presence
 * Outputs        : p_mmeUeS1APID - The mmeUeS1apId in the Asn Message if 
 *                                  present.
 *                  p_enbUeS1APID - The enbUeS1apId in the Asn Message if 
 *                                  present.
 *                  p_ue_id_pair_presence - This denotes S1APId's present
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.Either of
 *                                enbUeS1apId or mmeUeS1apId is valid.
 *                  RRC_FAILURE - Asn Encoding was not successful.Neither of
 *                                enbUeS1apId or mmeUeS1apId is valid.
 * Description    : This function when called partially decodes an Asn message
 *                  and returns  mmeUeS1apId and enbUeS1apId.
 *****************************************************************************/
rrc_return_et decode_s1ap_ue_s1ap_ids
(
 OSCTXT                *pctxt,
 s1ap_MME_UE_S1AP_ID   *p_mmeUeS1APID,
 s1ap_ENB_UE_S1AP_ID   *p_enbUeS1APID,
 U8                    *p_ue_id_pair_presence
 )
{
    S32               stat = 0;
    OSBOOL optbit = FALSE;
    OSBOOL extbit = FALSE;
    OSUINT32    ui = 0; 


    RRC_S1AP_UT_TRACE_ENTER();

    /* Decoding s1ap_UE_S1AP_IDs */
    stat = PD_BIT (pctxt, &extbit);
    if((stat != 0) || (extbit))
    {
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    stat = pd_ConsUnsigned (pctxt, &ui, 0, OSUINTCONST(1));
    if (stat != 0)
    {
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    switch(ui)
    {
        /* uE_S1AP_ID_pair
         * */
        case 0:

            /* Decoding
             * asn1PE_s1ap_UE_S1AP_ID_pair
             * */
            stat = PD_BIT (pctxt, &extbit);
            if (stat != 0)
            {
                RRC_S1AP_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }

            stat = PD_BIT (pctxt, &optbit);
            if (stat != 0)
            {
                RRC_S1AP_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }

            stat = asn1PD_s1ap_MME_UE_S1AP_ID (pctxt, p_mmeUeS1APID);
            if (stat != 0)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                        "asn1PD_s1ap_MME_UE_S1AP_ID failed.",__FUNCTION__);

                RRC_S1AP_UT_TRACE_EXIT();

                return RRC_FAILURE;
            }

            stat = asn1PD_s1ap_ENB_UE_S1AP_ID (pctxt, p_enbUeS1APID);
            if (stat != 0)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                        "asn1PD_s1ap_ENB_UE_S1AP_ID.",__FUNCTION__);

                RRC_S1AP_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }

            *p_ue_id_pair_presence |= ENB_UE_S1AP_ID_PRESENT;
            *p_ue_id_pair_presence |= MME_UE_S1AP_ID_PRESENT;

            break;

            /* mME_UE_S1AP_ID
             *              * */
        case 1:
            stat = asn1PD_s1ap_MME_UE_S1AP_ID (pctxt, p_mmeUeS1APID);
            if (stat != 0)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                        "asn1PD_s1ap_MME_UE_S1AP_ID failed.",__FUNCTION__);

                RRC_S1AP_UT_TRACE_EXIT();

                return RRC_FAILURE;
            }
            *p_ue_id_pair_presence |= MME_UE_S1AP_ID_PRESENT;

            break;

        default:
            break;
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : partially_decode_S1AP_EP
 * Inputs         : pctxt - Pointer to OSCTXT.
 *                  p_mmeUeS1APID - pointer to s1ap_MME_UE_S1AP_ID
 *                  p_enbUeS1APID - pointer to s1ap_ENB_UE_S1AP_ID
 *                  p_s1ap_EP - pointer to the S1AP EP
 *                  p_ue_id_pair_presence - pointer to S1APId's presence 
 * Outputs        : p_mmeUeS1APID - The mmeUeS1apId in the Asn Message if 
 *                                  present.
 *                  p_enbUeS1APID - The enbUeS1apId in the Asn Message if 
 *                                  present.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.Either of
 *                                enbUeS1apId or mmeUeS1apId is valid.
 *                  RRC_FAILURE - Asn Encoding was not successful.Neither of
 *                                enbUeS1apId or mmeUeS1apId is valid.
 * Description    : This function when called partially decodes an Asn message
 *                  and returns  mmeUeS1apId and enbUeS1apId.
 *****************************************************************************/
rrc_return_et partially_decode_S1AP_EP(OSCTXT* pctxt, 
        s1ap_MME_UE_S1AP_ID * p_mmeUeS1APID,
        s1ap_ENB_UE_S1AP_ID * p_enbUeS1APID,
        U8* p_ue_id_pair_presence)
{
    U32 index = 0;
    OSBOOL extbit = FALSE;
    S32 stat = 0;
    U32 count = 0;
    s1ap_ProtocolIE_ID protocol_id = 0;
    s1ap_Criticality criticality = 0;
    rrc_return_et    ret_code = RRC_SUCCESS;

    RRC_S1AP_UT_TRACE_ENTER();


    /* extension bit */
    stat = DEC_BIT (pctxt, &extbit);
    if (stat != 0) return RRC_FAILURE;

    /* decode length determinant */
   PU_SETSIZECONSTRAINT (pctxt, OSUINTCONST(0), OSUINTCONST(65535), 0, 0);

    stat = pd_Length (pctxt, (OSUINT32*)(&count));
    if (stat != 0) return RRC_FAILURE;

    if (!count)
    {
        return RRC_SUCCESS;
    }

    count = count < MAX_ELEM_TO_PARTIAL_DECODE ? count : 
        MAX_ELEM_TO_PARTIAL_DECODE;
    /* decode elements */

    for (index = 0; index < count; index++) 
    {
        stat = asn1PD_s1ap_ProtocolIE_ID (pctxt, &protocol_id);
        if (stat != 0) 
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                    "asn1PD_s1ap_ProtocolIE_ID failed.",__FUNCTION__);
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        /* decode criticality */

        stat = asn1PD_s1ap_Criticality (pctxt, &criticality);
        if (stat != 0)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                    "asn1PD_s1ap_Criticality failed.",__FUNCTION__);
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        /* decode value */
        { OSCTXT lctxt;
            ASN1OpenType openType;

            stat = pd_OpenType (pctxt, &openType.data, &openType.numocts);

            if (stat != 0)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                        "pd_OpenType failed.",__FUNCTION__);
                RRC_S1AP_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }

            /* decode open type data into specific type field */

            rtxCopyContext (&lctxt, pctxt);

            stat = pd_setp (pctxt, (OSOCTET*)openType.data,
                    openType.numocts, pctxt->buffer.aligned);

            switch (protocol_id) 
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    stat = asn1PD_s1ap_MME_UE_S1AP_ID (pctxt, 
                            p_mmeUeS1APID);
                    if (stat != 0) 
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                                "asn1PD_s1ap_MME_UE_S1AP_ID failed.",
                                __FUNCTION__);
                        RRC_S1AP_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }
                    else
                    {
                        *p_ue_id_pair_presence |= MME_UE_S1AP_ID_PRESENT;
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    stat = asn1PD_s1ap_ENB_UE_S1AP_ID (pctxt, 
                            p_enbUeS1APID);
                    if (stat != 0)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: "
                                "asn1PD_s1ap_ENB_UE_S1AP_ID failed.",
                                __FUNCTION__);
                        RRC_S1AP_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }
                    else
                    {
                        *p_ue_id_pair_presence |= ENB_UE_S1AP_ID_PRESENT;
                    }
                    break;

                case ASN1V_s1ap_id_UE_S1AP_IDs:

                    ret_code = decode_s1ap_ue_s1ap_ids (pctxt, p_mmeUeS1APID,
                            p_enbUeS1APID, p_ue_id_pair_presence);
                    if (RRC_FAILURE == ret_code)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR,
                                "Decoding of s1ap_ue_s1ap_ids failed.");
                        RRC_S1AP_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }

                    break;

                default:
                    break;
            }
            rtxCopyContext (pctxt, &lctxt);
        }
    }

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : parse_asn_s1_setup_failure
 * Inputs         : p_s1ap_gb_ctx - pointer to S1ApP global context
 *                  p_mme_ctx - pointer to MME context 
 *                  p_asn_msg - Pointer to S1 SETUP FAILURE Asn message to be
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_time_to_wait - pointer to time to wait IE
 *                  cause - pointer to the S1AP cause 
 * Outputs        : p_time_to_wait - This is populated with time to wait IE
 *                                   value in the Asn message if present.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function parses the received S1 SETUP FAILURE Asn
 *                  message and fills p_time_to_wait with time to wait IE value
 *                  if the IE is present in the Asn message.
 *****************************************************************************/
rrc_return_et parse_asn_s1_setup_failure (
        s1ap_gb_context_t    *p_s1ap_gb_ctx,
        mme_context_t        *p_mme_ctx,
        void                 *p_asn_msg,
        U16                  asn_msg_len,
        U32                  *p_time_to_wait,
	s1ap_Cause           *cause)
{
    S1AP_PDU                        s1ap_pdu;
    OSCTXT                          asn1_ctx;
    rrc_return_et                   result = RRC_SUCCESS;
    parsed_s1_setup_fail_t          parsed_s1_setup_fail;
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_S1AP_TRACE(RRC_DETAILED, "Unused variable(global context):%p",p_s1ap_gb_ctx);
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;

    memset_wrapper(&parsed_s1_setup_fail,0,sizeof(parsed_s1_setup_fail_t));
    RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: S1SetupFailure %s", change_data_to_str(asn_msg_len, p_asn_msg));
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",
                    &s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                   (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_SETUP_FAILURE, MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* Decode message */
        if(RRC_FAILURE == s1ap_setup_failure_intrl_dec(
                    p_mme_ctx, 
            s1ap_pdu.u.unsuccessfulOutcome->value.u.s1Setup,
                    &parsed_s1_setup_fail
                    ))
        {
            result = RRC_FAILURE;
            break;

        }

        if (parsed_s1_setup_fail.time_to_wait >= 
                            sizeof(time_to_wait_value)/
                            sizeof(time_to_wait_value[0]))
                    {
                        result = RRC_FAILURE;
                        break;
                    }
        /* SPR 10858 Fix Start */
        if (S1AP_S1_SETUP_FAIL_TIME_TO_WAIT_PRESENT & parsed_s1_setup_fail.bitmask)
        {
            *p_time_to_wait = convert_asn_time_to_wait((s1ap_TimeToWait_Root)parsed_s1_setup_fail.time_to_wait);
        }
        /* SPR 10858 Fix Stop */
	
	     l3_memcpy_wrapper (cause, &parsed_s1_setup_fail.cause, sizeof(s1ap_Cause));
    }while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_setup_failure_intrl_dec
 * Inputs         : p_mme_context - pointer to MME context 
 *                  p_3gpp - pointer to Decoded s1ap_S1SetupResponse
 *                  p_intrl - ponter to the parsed_s1_setup_fail_t
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : s1ap_Cause
 * Description    : Gets the Decoded S1 Setup Failure cause 
 ****************************************************************************/
rrc_return_et s1ap_setup_failure_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_S1SetupFailure      *p_3gpp,  /* unpacked src */
 parsed_s1_setup_fail_t   *p_intrl /* dest */
 )
{
    U32                             index = 0;
    OSRTDListNode                   *p_node = RRC_NULL;
    s1ap_S1SetupFailure_protocolIEs_element         *p_protocol_IE = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;
    OSCTXT asn1_ctx;

    s1ap_message_data_t message_map =
    {3, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_Cause, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_TimeToWait, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }


    p_intrl->bitmask = 0;
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
                (s1ap_S1SetupFailure_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_Cause:
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_Cause",
                                __FUNCTION__);

                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._S1SetupFailureIEs_1
                                    ))
                        {
                            p_intrl->cause =
                                *(p_protocol_IE->value.u._S1SetupFailureIEs_1);
                        }
                    }

                    break;
                case ASN1V_s1ap_id_TimeToWait:
                    {

                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_TimeToWait",
                                __FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &(p_protocol_IE->value.u._S1SetupFailureIEs_2)
                                    ))
                        {
                            p_intrl->time_to_wait = p_protocol_IE->value.u.
                                _S1SetupFailureIEs_2;
                            p_intrl->bitmask |= S1AP_S1_SETUP_FAIL_TIME_TO_WAIT_PRESENT;
                        }

                    }


                    break;
                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {

                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_CriticalityDiagnostics",
                                __FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._S1SetupFailureIEs_3
                                    ))
                        {
                            p_intrl->bitmask |=S1AP_S1_SETUP_FAIL_CRITICALITY_DIAGNOSTICS_PRESENT;

                            /* We have to log this message */
                            log_criticality_diagostics(p_protocol_IE->value.u._S1SetupFailureIEs_3);
                        }


                    }
                    break;
                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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

    } while (0);
    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_S1Setup,
                    T_S1AP_PDU_unsuccessfulOutcome,
                    s1ap_reject,
                    (rrc_s1ap_error_indication_t *)PNULL)) ) 
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "S1 Setup response failure- Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : compose_asn_reset
 * Inputs         : p_reset_req - Information from which Asn message is
 *                                   created.
 *                  p_asn_msg - pointer to asn message
 *                  p_asn_msg_len - pointer to the asn message length 
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function composes RESET REQUEST Asn Message from
 *                  the information provided in p_reset_req
 *****************************************************************************/
rrc_return_et compose_asn_reset(
        reset_req_t   *p_reset_req,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    s1ap_Reset                  *p_s1ap_reset = PNULL;
    S1AP_PDU                    s1ap_pdu;
    OSRTDList               ueIdPair_list;
    s1ap_ResetType          reset_type;
    EVENT_EXTERNAL_HEADER                       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: Reset");
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_Reset;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        /* Set the initiating message type to Reset */
        s1ap_pdu.u.initiatingMessage->value.t =  T1_reset;

        p_s1ap_reset = rtxMemAllocType(&asn1_ctx, s1ap_Reset);
        if (PNULL == p_s1ap_reset)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_Reset (p_s1ap_reset);

        s1ap_pdu.u.initiatingMessage->value.u.reset = p_s1ap_reset;


        if (0 != asn1Append_s1ap_Reset_protocolIEs_1 (&asn1_ctx,
                    &p_s1ap_reset->protocolIEs, 
                    &(p_reset_req->cause)))
        {
            break;
        }

        rtxDListInit(&ueIdPair_list);
        memset_wrapper(&reset_type,0,sizeof(reset_type));
        if (RRC_FAILURE == compose_reset_info(&asn1_ctx,
                    &p_reset_req->reset_info,
                    &p_s1ap_reset->protocolIEs,
                    &ueIdPair_list,
                    &reset_type))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of Reset Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
            
           
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_S1_RESET, MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;

}


/*****************************************************************************
 * Function Name  : compose_reset_info
 * Inputs         : p_asn1_ctx - pointer to p_asn1_ctx 
 *                  p_reset_info - Information from which Asn message is
 *                                   created.
 *                  p_list - poiner to s1ap_Reset_protocolIEs
 *                  p_ueIdPair_list - pointer to OSRTDList
 *                  p_reset_type - pointer to the s1ap_ResetType
 * Outputs        : p_asn1_ctx - Pointer to OSCTXT being populated.
 *                  p_list - Pointer to list of Reset Protocol IEs
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function composes RESET REQUEST protocol IEs from
 *                  the information provided in p_reset_info
 *****************************************************************************/
rrc_return_et compose_reset_info(OSCTXT * p_asn1_ctx,
        s1ap_reset_info_t * p_reset_info,
        s1ap_Reset_protocolIEs * p_list,
        OSRTDList               *p_ueIdPair_list,
        s1ap_ResetType          *p_reset_type)
{
    OSRTDListNode           *p_node = PNULL;
    U32                     index = 0;
    U32                     ueIdPair_list_count =0 ;
    s1ap_UE_associatedLogicalS1_ConnectionListRes_element 
        * p_protocolIE_elem = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    if (!p_reset_info->num_of_items)
    {
        /* S1 interface */

        p_reset_type->t = T_s1ap_ResetType_s1_Interface;
        p_reset_type->u.s1_Interface = s1ap_reset_all;
    }
    else
    {
        /* part of S1 Interface */


        ueIdPair_list_count = p_reset_info->num_of_items;

        for (index = 0; index < ueIdPair_list_count; index++)
        {
            rtxDListAllocNodeAndData(p_asn1_ctx, 
                    s1ap_UE_associatedLogicalS1_ConnectionListRes_element, 
                    &p_node, 
                    &p_protocolIE_elem);

            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                return RRC_FAILURE;
            }
            asn1Init_s1ap_UE_associatedLogicalS1_ConnectionListRes_element (
                   p_protocolIE_elem);

            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1 =
            rtxMemAllocType(p_asn1_ctx, s1ap_UE_associatedLogicalS1_ConnectionItem);
            if (PNULL == p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                break;
            }
            asn1Init_s1ap_UE_associatedLogicalS1_ConnectionItem (
                    p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1);

             p_protocolIE_elem->value.t = T70__UE_associatedLogicalS1_ConnectionItemRes_1;

            p_protocolIE_elem->id = 
                ASN1V_s1ap_id_UE_associatedLogicalS1_ConnectionItem;
            p_protocolIE_elem->criticality = s1ap_reject;

            if (p_reset_info->id_pair[index].bitmask & MME_UE_S1AP_ID_PRESENT)
            {
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1->m.mME_UE_S1AP_IDPresent = 1;
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1->mME_UE_S1AP_ID =
                    p_reset_info->id_pair[index].mmeUeS1apId ;
            }else
            {
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1->m.mME_UE_S1AP_IDPresent = 0;
            }

            if (p_reset_info->id_pair[index].bitmask & ENB_UE_S1AP_ID_PRESENT)
            {
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1->m.eNB_UE_S1AP_IDPresent = 1;
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1->eNB_UE_S1AP_ID =
                    p_reset_info->id_pair[index].enbUeS1apId ;
            }else
            {
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemRes_1->m.eNB_UE_S1AP_IDPresent = 0;
            }

            rtxDListAppendNode(p_ueIdPair_list, p_node);
        }

        p_reset_type->t = T_s1ap_ResetType_partOfS1_Interface;
        p_reset_type->u.partOfS1_Interface = p_ueIdPair_list;
    }

    if ( 0 !=  asn1Append_s1ap_Reset_protocolIEs_2 (p_asn1_ctx,
                p_list,
                p_reset_type))
    {
        return RRC_FAILURE;
    }

    RRC_S1AP_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : compose_asn_reset_ack
 * Inputs         : p_reset_info - Information from which Asn message is
 *                                   created.
 *                  p_asn_msg - pointer to asn message
 *                  p_asn_msg_len - pointer to asn message length 
 *                  p_err_ind - pointer to rrc_s1ap_error_indication_t
 *                  p_send_err_ind - pointer to s1ap_error_ind_bool_t 
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function composes RESET ACK Asn Message from
 *                  the information provided in p_reset_info
 *****************************************************************************/
rrc_return_et compose_asn_reset_ack(
        s1ap_reset_info_t   *p_reset_info,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    s1ap_ResetAcknowledge      *p_s1ap_reset_ack = PNULL;
    S1AP_PDU                    s1ap_pdu;
    s1ap_UE_associatedLogicalS1_ConnectionListResAck    ue_assoc_log_con_list;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_S1AP_TRACE(RRC_INFO, "S1AP_MSG: ResetAcknowledge");
    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "ASN context initialization failed.");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to successful outcome  message */
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx, 
                SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_SuccessfulOutcome (s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode = ASN1V_s1ap_id_Reset;
        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        /* Set the initiating message type to reset ack*/
        s1ap_pdu.u.successfulOutcome->value.t =  T1_reset;

        p_s1ap_reset_ack = rtxMemAllocType(&asn1_ctx, s1ap_ResetAcknowledge);
        if (PNULL == p_s1ap_reset_ack)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_ResetAcknowledge (p_s1ap_reset_ack);

        s1ap_pdu.u.successfulOutcome->value.u.reset = p_s1ap_reset_ack;

        if (p_reset_info->num_of_items)
        {
            rtxDListInit(&ue_assoc_log_con_list);
            compose_reset_ack_ueId_pair_list( &asn1_ctx,
                    p_reset_info,
                    &p_s1ap_reset_ack->protocolIEs,
                    &ue_assoc_log_con_list);
        }


        if ((RRC_TRUE == p_send_err_ind->send_err_indication_reject_ie_present) ||
                (RRC_TRUE == p_send_err_ind->send_err_indication_notify_ie_present))
        {
            /* Fill Criticality diagnostics if its filled */
            if (0 != asn1Append_s1ap_ResetAcknowledge_protocolIEs_2 (&asn1_ctx,
                        &p_s1ap_reset_ack->protocolIEs,
                        &p_err_ind->criticality_diagnostics))
            {
                RRC_S1AP_TRACE(RRC_ERROR, "Criticality Diagnostics Encode failure");
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                return RRC_FAILURE;
            }
        }


        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN_6K, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Reset Ack "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);

            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);

            response = RRC_SUCCESS;
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_S1_RESET_ACKNOWLEDGE, MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}

/*****************************************************************************
 * Function Name  : compose_reset_ack_ueId_pair_list
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT context
 *                  p_reset_info - Information from which Asn message is
 *                                   created.
 *                  p_list - pointer to s1ap_ResetAcknowledge_protocolIEs
 *                  p_ue_assoc_log_con_list - pointer to 
 *                      s1ap_UE_associatedLogicalS1_ConnectionListResAck
 * Outputs        : p_asn1_ctx - Pointer to OSCTXT context being populated.
 *                  p_list - Pointer to Reset Ack protocol IEs list.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function composes REQUEST ACK protocol IEs from
 *                  the information provided in p_reset_info
 *****************************************************************************/
rrc_return_et compose_reset_ack_ueId_pair_list(OSCTXT * p_asn1_ctx,
        s1ap_reset_info_t * p_reset_info,
        s1ap_ResetAcknowledge_protocolIEs * p_list,
        s1ap_UE_associatedLogicalS1_ConnectionListResAck   
        *p_ue_assoc_log_con_list)
{
    U32                     index = 0;
    U32                     ueIdPair_list_count =0 ;
    OSRTDListNode           *p_node = PNULL;
    s1ap_UE_associatedLogicalS1_ConnectionListResAck_element 
        * p_protocolIE_elem = PNULL;



    ueIdPair_list_count = p_reset_info->num_of_items;

    for (index = 0; index < ueIdPair_list_count; index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                s1ap_UE_associatedLogicalS1_ConnectionListResAck_element, 
                &p_node, 
                &p_protocolIE_elem);

        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            return RRC_FAILURE;
        }
        asn1Init_s1ap_UE_associatedLogicalS1_ConnectionListResAck_element (
                p_protocolIE_elem);

        p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1 =
            rtxMemAllocType(p_asn1_ctx, s1ap_UE_associatedLogicalS1_ConnectionItem);
        if (PNULL == p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        asn1Init_s1ap_UE_associatedLogicalS1_ConnectionItem (
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1);

        p_protocolIE_elem->value.t = T72__UE_associatedLogicalS1_ConnectionItemResAck_1;

        p_protocolIE_elem->id = 
            ASN1V_s1ap_id_UE_associatedLogicalS1_ConnectionItem;
        p_protocolIE_elem->criticality = s1ap_ignore;

        p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.iE_ExtensionsPresent = 0;

        if (p_reset_info->id_pair[index].bitmask & MME_UE_S1AP_ID_PRESENT)
        {
            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.mME_UE_S1AP_IDPresent = 1;
            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->mME_UE_S1AP_ID =
                p_reset_info->id_pair[index].mmeUeS1apId ;
        }else
        {
            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.mME_UE_S1AP_IDPresent = 0;
        }

        if (p_reset_info->id_pair[index].bitmask & ENB_UE_S1AP_ID_PRESENT)
        {
            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.eNB_UE_S1AP_IDPresent = 1;
            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->eNB_UE_S1AP_ID =
                p_reset_info->id_pair[index].enbUeS1apId ;
        }else
        {
            p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.eNB_UE_S1AP_IDPresent = 0;
        }

        rtxDListAppendNode(p_ue_assoc_log_con_list, p_node);
    }

    if (0 != asn1Append_s1ap_ResetAcknowledge_protocolIEs_1 (p_asn1_ctx,
                p_list,
                p_ue_assoc_log_con_list))
    {
        return RRC_FAILURE;
    }

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : parse_asn_reset_ack
 * Inputs         : p_mme_context - pointer to MME context
 *                  p_asn_msg - Pointer to RESPONSE Ack Asn message to be
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_reset_info - Pointer to s1ap_reset_info_t
 * Outputs        : p_reset_info - This structure is populated with
 *                                 information from input Asn Message.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_reset_info with 
 *                  information received after decoding REQUEST ACK
 *                  Asn Messages.
 *****************************************************************************/
rrc_return_et parse_asn_reset_ack (
        mme_context_t *p_mme_context,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        s1ap_reset_info_t  * p_reset_info
        )
{
    S1AP_PDU                        s1ap_pdu;
    OSCTXT                          asn1_ctx;
    rrc_return_et                   result = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER           *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    if (!p_reset_info)
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "p_parsed_s1_setup_rsp is NULL",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_context, PNULL, PNULL);
            result = RRC_FAILURE;

            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_RESET_ACKNOWLEDGE, MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        if (RRC_FAILURE == s1ap_reset_ack_intrl_dec(
                    p_mme_context,
                    s1ap_pdu.u.successfulOutcome->value.u.reset,
                    p_reset_info))
        {
            result = RRC_FAILURE;
            break;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_reset_ack_intrl_dec
 * Inputs         : p_mme_context - pointer to MME context
 *                  p_3gpp - pointer to s1ap_ResetAcknowledge
 *                  p_intrl - pointer to s1ap_reset_info_t 
 *                     p_3gpp (Decoded s1ap_S1SetupResponse)
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function resets S1AP resoponse message.
 ****************************************************************************/
rrc_return_et s1ap_reset_ack_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_ResetAcknowledge *p_3gpp,
 s1ap_reset_info_t *p_intrl
 )
{
    OSRTDListNode                   *p_node = RRC_NULL;
    U8 index = 0;
    rrc_return_et result = RRC_SUCCESS;
    s1ap_ResetAcknowledge_protocolIEs_element *p_reset_ack_protocol_IE = PNULL;
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind; 
    rrc_bool_et send_tse = RRC_FALSE;
    OSCTXT asn1_ctx;

    s1ap_message_data_t message_map =
    {2, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_UE_associatedLogicalS1_ConnectionListResAck,
s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    p_node = p_3gpp->protocolIEs.head;
    /* Save the decoded Protocol IEs */
    for (index =0; index < p_3gpp->protocolIEs.count; index++)
    {
        if (!p_node)
        {
            RRC_S1AP_TRACE(RRC_ERROR,"%s: "
                    "next node returned NULL",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        p_reset_ack_protocol_IE = 
            (s1ap_ResetAcknowledge_protocolIEs_element *)
            p_node->data;

        switch (p_reset_ack_protocol_IE->id)
        {
            case ASN1V_s1ap_id_UE_associatedLogicalS1_ConnectionListResAck:
                RRC_S1AP_TRACE(RRC_INFO,"%s: "
                        "ASN1V_s1ap_id_UE_associated"
                        "LogicalS1_ConnectionListResAck",
                        __FUNCTION__);

                if (RRC_SUCCESS == validate_ie_value (
                            &message_map,
                            index,
                            p_reset_ack_protocol_IE->id,
                            p_reset_ack_protocol_IE->value.u._ResetAcknowledgeIEs_1))
                {
                    parse_asn_reset_ack_ueId_pair_list(p_intrl,
                            p_reset_ack_protocol_IE->value.u._ResetAcknowledgeIEs_1,
                            &iE_list,
                            &ie_list_index,
                            &send_err_ind, 
                            &send_tse);

                    /* This will only happen when the element list exceeds
                     * the max number */
                    if (RRC_TRUE == send_tse)
                    {
                        RRC_S1AP_TRACE(RRC_WARNING, "PARSE Error is Detected in "
                                "ASN.1 Sequence, Sending Transfer Syntax Error");
                        s1ap_build_and_send_err_indication(p_mme_context, PNULL, PNULL);
                        result = RRC_FAILURE;
                        break;
                    }
                }

                break;

            case ASN1V_s1ap_id_CriticalityDiagnostics:
                RRC_S1AP_TRACE(RRC_INFO,"%s: "
                        "ASN1V_s1ap_id_CriticalityDiagnostics",
                        __FUNCTION__);
                if (RRC_SUCCESS == validate_ie_value (
                            &message_map,
                            index,
                            p_reset_ack_protocol_IE->id,
                            p_reset_ack_protocol_IE->value.u._ResetAcknowledgeIEs_2))
                {
                    /* We have to log this message */
                    log_criticality_diagostics(p_reset_ack_protocol_IE->value.u._ResetAcknowledgeIEs_2);
                }
                break;

            default:
                RRC_S1AP_TRACE(RRC_ERROR,"%s: "
                        "invalid Protocol IE id",
                        __FUNCTION__);
                /* Class - 1 Error - Unknown IE */
                /* Base on Criticality fill the value */
                add_to_err_ind_ie_list(&iE_list, 
                        p_reset_ack_protocol_IE->criticality,
                        p_reset_ack_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        RRC_FALSE,
                        RRC_FALSE);

        }

        p_node = p_node->next;
    }

    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_Reset,
                    T_S1AP_PDU_successfulOutcome,
                    s1ap_ignore,
                    (rrc_s1ap_error_indication_t *)PNULL)) && 
            (RRC_TRUE == send_err_ind.send_err_indication_reject_ie_present))
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "Reset Ack - Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    return result;
}


/*****************************************************************************
 * Function Name  : parse_asn_reset_ack_ueId_pair_list
 * Inputs         : p_reset_info - pointer to s1ap_reset_info_t
 *                  p_list - This list the list of UE Id pair and is traversed
 *                           while decoding the Asn message.
 *                  p_ie_list - pointer to s1ap_error_ind_ie_list_t
 *                  p_index - pointer to index
 *                  p_send_error_indication - pointer to s1ap_error_ind_bool_t
 *                  p_send_tse - pointer to rrc_bool_et
 * Outputs        : p_reset_info - This structure is populated with
 *                                 information from input Asn Message.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_reset_info with 
 *                  information received after decoding REQUEST ACK
 *                  Asn Messages.
 *****************************************************************************/
rrc_return_et parse_asn_reset_ack_ueId_pair_list(
        s1ap_reset_info_t * p_reset_info,
        OSRTDList * p_list,
        s1ap_error_ind_ie_list_t *p_ie_list,
        U16 *p_index,
        s1ap_error_ind_bool_t *p_send_error_indication,
        rrc_bool_et *p_send_tse)
{
    U32                     index = 0;
    U32                     ueIdPair_list_count =0 ;
    s1ap_UE_associatedLogicalS1_ConnectionListResAck_element 
        * p_protocolIE_elem = PNULL;
    OSRTDListNode            *p_node = PNULL;

    ueIdPair_list_count = p_list->count;

    if (MAX_UE_S1AP_ID_PAIR <= p_list->count)
    {
        *p_send_tse = RRC_TRUE;
    }

    p_node = p_list->head;
    p_reset_info->num_of_items = (U16) ueIdPair_list_count;

    for (index = 0; index < ueIdPair_list_count; index++)
    {
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "parse_asn_reset_ack_ueId_pair_list "
                    "Next node is null");
            return RRC_FAILURE;
        }

        p_protocolIE_elem = 
            (s1ap_UE_associatedLogicalS1_ConnectionListResAck_element *)
            p_node->data;
            if (p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1 == PNULL)
            {
                continue;
            }

        if (ASN1V_s1ap_id_UE_associatedLogicalS1_ConnectionItem == p_protocolIE_elem->id)
        {

            /* These valuse are been checked in obj-sys code */
            /* No need to check the value range */
        p_reset_info->id_pair[index].bitmask = 0;

        if (p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.mME_UE_S1AP_IDPresent)
        {
            p_reset_info->id_pair[index].bitmask |= MME_UE_S1AP_ID_PRESENT;
            p_reset_info->id_pair[index].mmeUeS1apId = 
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->mME_UE_S1AP_ID;
        }

        if (p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.eNB_UE_S1AP_IDPresent)
        {
            p_reset_info->id_pair[index].bitmask |= ENB_UE_S1AP_ID_PRESENT;
            p_reset_info->id_pair[index].enbUeS1apId =
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->eNB_UE_S1AP_ID;
            }
        }
        else
        {
            add_to_err_ind_ie_list(p_ie_list,
                    p_protocolIE_elem->criticality,
                    p_protocolIE_elem->id,
                    p_index,
                    p_send_error_indication,
                    RRC_FALSE,
                    RRC_FALSE);
        }

        p_node = p_node->next;
    }

    return RRC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : parse_asn_reset
 * Inputs         : p_s1ap_gb_ctx - pointer to the S1AP global context 
 *                  p_mme_ctx - pointer to the MME context  
 *                  p_asn_msg - Pointer to RESET Asn message to be
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_reset_req - pointer to reset_req_t
 *                  p_error_indication -pointer to rrc_s1ap_error_indication_t
 *                  p_send_err_ind - pointer to p_send_err_ind
 * Outputs        : p_reset_req - This structure is populated with
 *                                information from input Asn Message.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_parsed_s1_setup_rsp with 
 *                  information received after decoding S1 SETUP REQUEST 
 *                  Asn Messages.
 *****************************************************************************/
rrc_return_et parse_asn_reset(
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        reset_req_t        *p_reset_req,
        rrc_s1ap_error_indication_t *p_error_indication,
        s1ap_error_ind_bool_t *p_send_err_ind)
{
    S1AP_PDU                        s1ap_pdu;
    OSCTXT                          asn1_ctx;
    rrc_return_et                   result = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_S1AP_TRACE(RRC_DETAILED, "Unused variable(global context):%p",p_s1ap_gb_ctx);

    if (!p_reset_req)
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "p_parsed_s1_setup_rsp is NULL",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);
            result = RRC_FAILURE;
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                               (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_RESET, MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* Decode message */
        if (RRC_FAILURE == s1ap_mme_initiated_reset_intrl_dec(
                    p_mme_ctx,
            s1ap_pdu.u.initiatingMessage->value.u.reset,
            p_reset_req,
                    p_error_indication,
                    p_send_err_ind))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "Parse Failure", __FUNCTION__);

            result = RRC_FAILURE;
            break;
        }
        else
        {
        parse_asn_reset_type(p_reset_req->p_reset_type,
                            &p_reset_req->reset_info);
        }

    }while(0);


    rtFreeContext(&asn1_ctx);
    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_mme_initiated_reset_intrl_dec
 * Inputs         : p_mme_ctx - pointer to MME context 
 *                  p_3gpp - pointer to s1ap_Reset
 *                  p_intrl - pointer to reset_req_t
 *                  p_error_indication - pointer  to rrc_s1ap_error_indication_t\
 *                  p_send_err_ind - pointer  to s1ap_error_ind_bool_t
 * Outputs        : p_intrl (internal reset_req_t)
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : Gets the Decoded S1AP RESET message
 ****************************************************************************/
rrc_return_et s1ap_mme_initiated_reset_intrl_dec
(
 mme_context_t *p_mme_ctx,
 s1ap_Reset      *p_3gpp,  /* unpacked src */
 reset_req_t     *p_intrl, /* dest */
 rrc_s1ap_error_indication_t *p_error_indication,
 s1ap_error_ind_bool_t *p_send_err_ind
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_Reset_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    OSCTXT asn1_ctx;

    /* Error Indication */
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;

    s1ap_message_data_t message_map =
    {2, RRC_FALSE, RRC_TRUE, 0, 0, 0,
        {{0, ASN1V_s1ap_id_Cause, s1ap_mandatory, s1ap_ignore, 0, 0, 0, 0},
         {1, ASN1V_s1ap_id_ResetType, s1ap_mandatory, s1ap_reject, 0, 0, 0, 0}}};

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    /* MEM SET */
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(p_send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

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
                (s1ap_Reset_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_Cause:
                    {
                        if (RRC_SUCCESS == 
                                validate_ie_value(
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._ResetIEs_1))
                        {
                            p_intrl->cause = *p_protocol_IE->value.u._ResetIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_ResetType:
                    {
                        if (RRC_SUCCESS == 
                                validate_ie_value(
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._ResetIEs_2))
                        {
                            p_intrl->p_reset_type = p_protocol_IE->value.u._ResetIEs_2;
                        }
                    }
                    break;       

                default:
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    add_to_err_ind_ie_list(&iE_list, 
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            p_send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);
            }

            p_node= p_node->next;            
        }
    } while(0);

    /* Parse the map for Error Indication */
    /* The success means we have sent an EI to MME */
    if (RRC_SUCCESS == parse_message_map(
                &asn1_ctx,
                p_mme_ctx, 
                &message_map, 
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_s1ap_id_Reset,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                p_error_indication))
    {
        result = RRC_FAILURE;
    }


    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}


/*****************************************************************************
 * Function Name  : parse_asn_reset_type
 * Inputs         : p_reset_type_IE - Pointer to reset type IE being parsed.
 *                  p_reset_info - pointer to s1ap_reset_info_t
 * Outputs        : p_reset_info - This structure is populated with
 *                                 information from input Asn Message.
 * Returns        : None
 * Description    : This function populates p_reset_info with 
 *                  information received after decoding protocol IE reset type
 *                  in the Asn Message.
 *****************************************************************************/
void parse_asn_reset_type(s1ap_ResetType  * p_reset_type_IE,
        s1ap_reset_info_t * p_reset_info)
{
    RRC_S1AP_UT_TRACE_ENTER();

    if (p_reset_type_IE->t == T_s1ap_ResetType_s1_Interface)
    {
        p_reset_info->num_of_items = 0;
    }
    else
    {
        parse_asn_reset_ack_ueId_pair_list_no_ei(p_reset_info,
                p_reset_type_IE->u.partOfS1_Interface);
    }

    RRC_S1AP_UT_TRACE_EXIT();
}


/*****************************************************************************
 * Function Name  : convert_asn_time_to_wait
 * Inputs         : asn_time_to_wait 
 * Outputs        : none
 * Returns        : time_to_wait
 * Description    : This function returns the time to wait according to value 
 *                  passed
 *****************************************************************************/
U32  convert_asn_time_to_wait(s1ap_TimeToWait_Root asn_time_to_wait)
{
    U32 time_to_wait = 0;
    switch (asn_time_to_wait)
    {
        case s1ap_v1s:
            {
                time_to_wait = 1000; 
                break;
            }
        case s1ap_v2s:
            {
                time_to_wait = 2000; 
                break;
            }
        case s1ap_v5s:
            {
                time_to_wait = 5000; 
                break;
            }
        case s1ap_v10s:
            {
                time_to_wait = 10000;
                break;
            }
        case s1ap_v20s:
            {
                time_to_wait = 20000;
                break;
            }
        case s1ap_v60s:
            {
                time_to_wait = 60000;
                break;
            }
        default:
            {
                RRC_S1AP_TRACE(RRC_INFO, "%d Invalid Time To Wait Sent in"
                        " S1AP Setup Failure, resetting it to 0s", asn_time_to_wait);
            }
    }

    return time_to_wait;
}




/*****************************************************************************
 * Function Name  : parse_asn_reset_ack_ueId_pair_list_no_ei
 * Inputs         : p_reset_info - pointer to s1ap_reset_info_t
 *                  p_list - This list the list of UE Id pair and is traversed
 *                           while decoding the Asn message.
 * Outputs        : p_reset_info - This structure is populated with
 *                                 information from input Asn Message.
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_reset_info with 
 *                  information received after decoding REQUEST ACK
 *                  Asn Messages.
 *****************************************************************************/
rrc_return_et parse_asn_reset_ack_ueId_pair_list_no_ei(
        s1ap_reset_info_t * p_reset_info,
        OSRTDList * p_list)
{
    U32                     index = 0;
    U32                     ueIdPair_list_count =0 ;
    s1ap_UE_associatedLogicalS1_ConnectionListResAck_element  
        * p_protocolIE_elem = PNULL;
    OSRTDListNode            *p_node = PNULL;

    ueIdPair_list_count = p_list->count;

    p_node = p_list->head;
    p_reset_info->num_of_items = (U16) ueIdPair_list_count;

    for (index = 0; index < ueIdPair_list_count; index++)
    {
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "parse_asn_reset_ack_ueId_pair_list"
                    " Next node is null");
            return RRC_FAILURE;
        }

        p_protocolIE_elem = 
            (s1ap_UE_associatedLogicalS1_ConnectionListResAck_element *)
            p_node->data;
            if ( p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1 == PNULL)
            {
                continue;
            }

        p_reset_info->id_pair[index].bitmask = 0;

        if (p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.mME_UE_S1AP_IDPresent)
        {
            p_reset_info->id_pair[index].bitmask |= MME_UE_S1AP_ID_PRESENT;
            p_reset_info->id_pair[index].mmeUeS1apId = 
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->mME_UE_S1AP_ID;
        }

        if (p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->m.eNB_UE_S1AP_IDPresent)
        {
            p_reset_info->id_pair[index].bitmask |= ENB_UE_S1AP_ID_PRESENT;
            p_reset_info->id_pair[index].enbUeS1apId =
                p_protocolIE_elem->value.u._UE_associatedLogicalS1_ConnectionItemResAck_1->eNB_UE_S1AP_ID;
        }

        p_node = p_node->next;
    }

    return RRC_SUCCESS;
}

/*****************************************************************************
* Function Name  : parse_asn_overload_start 
* Inputs         : p_s1ap_gb_ctx - pointer to S1AP global context
*                  p_mme_ctx - pointer to MME context  
*                  p_asn_msg - pointer to the asn msg 
*                  asn_msg_len - length of asn message
*                  p_parsed_overload_start_msg - pointer to 
*                                                 parsed_overload_start_t
* Outputs        : mme_overload_response - Overload response received.
* Returns        : RRC_SUCCESS - Asn Encoding was successful.
*                  RRC_FAILURE - Asn Encoding was not successful.
* Description    : This function decode the overload start ASN.1 Message 
*****************************************************************************/
rrc_return_et parse_asn_overload_start(
    mme_context_t           *p_mme_ctx,
    void                      *p_asn_msg,
    U16                       asn_msg_len,
        parsed_overload_start_t *p_parsed_overload_start_msg)

{
    S1AP_PDU s1ap_pdu;
    OSCTXT   asn1_ctx;

    rrc_return_et result = RRC_SUCCESS;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return result;
    }

    /* Init S1AP PDU */
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }

        /* Decode message */
        if (RRC_FAILURE == s1ap_overload_start_intrl_dec(
                    p_mme_ctx,
            s1ap_pdu.u.initiatingMessage->value.u.overloadStart,
                    p_parsed_overload_start_msg))
        {
            result = RRC_FAILURE;
            break;
        }

    }while (0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_overload_start_intrl_dec
 * Inputs         : p_mme_ctx - pointer to MME context 
 *                  p_3gpp - pointer to s1ap_OverloadStart
 *                  p_intrl - pointer to parsed_overload_start_t
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : Gets the Decoded Overload start MME message
 ****************************************************************************/
rrc_return_et s1ap_overload_start_intrl_dec
(
 mme_context_t *p_mme_ctx,
 s1ap_OverloadStart       *p_3gpp,    /* unpacked src */
 parsed_overload_start_t  *p_intrl   /* dest */
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_OverloadStart_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U16  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;

    rrc_return_et result = RRC_SUCCESS;

    /* Message Map */
    s1ap_message_data_t message_map = 
    {3, RRC_FALSE, RRC_FALSE, 0, 0, 0,
        {{0, ASN1V_s1ap_id_OverloadResponse, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_GUMMEIList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_s1ap_id_TrafficLoadReductionIndication, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    do
    {
        /* We have to check 6 things here
         * 1. Class-1 Error Unable to understand the IE or Unknow IE ID
         * 2. Class-2 Logical Range Voilation
         * 3. Class-3 IE Presence/Missing IE Check
         * 4. Class-4 Wrong Order and too many occurrances
         * 5. Class-5 Conditional Presnece/But IE is present 
         * 6. Logical Errors
         */

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

            /* If data is NULL we are not sure for which IE it is
             * We'll detect after parsing the message map */
            p_protocol_IE =
                (s1ap_OverloadStart_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_OverloadResponse:
                    {
                        /* we'll update the map if encountered invalid
                         * value */
                        if (RRC_SUCCESS ==
                                validate_ie_value(
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._OverloadStartIEs_1->u.overloadAction))
                        {
                            /* Assigne the value */
                            p_intrl->overload_action =
                                p_protocol_IE->value.u._OverloadStartIEs_1->u.overloadAction;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_GUMMEIList:
                    {
                        if (RRC_SUCCESS ==
                                validate_ie_value(
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._OverloadStartIEs_2))
                        {
                            p_intrl->bitmask |=
                                S1AP_S1_OVERLOAD_START_GUMMEILIST_PRESENT;
                            p_intrl->s1ap_gummelist =
                                *p_protocol_IE->value.u._OverloadStartIEs_2;
                        } 
                    }
                    break;

                case ASN1V_s1ap_id_TrafficLoadReductionIndication:
                    {
                        if (RRC_SUCCESS ==
                                validate_ie_value(
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._OverloadStartIEs_3))
                        {
                            p_intrl->bitmask |=
                                S1AP_S1_OVERLOAD_START_TRAFFIC_LOAD_RED_IND_PRESENT;
                            p_intrl->s1ap_traffic_load_red_ind = 
                                p_protocol_IE->value.u._OverloadStartIEs_3;
                        } 
                    }
                    break;

                default:
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
            p_node= p_node->next;            
        }

    } while (0);

    /* Parse the map for Error Indication */
    /* The success means we have sent an EI to MME */
    if (RRC_SUCCESS == parse_message_map(
                &asn1_ctx,
                p_mme_ctx, 
                &message_map, 
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_OverloadStart,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
* Function Name  : convert_overload_response
* Inputs         : asn_overload_response - ASN Encoded value
* Outputs        : None 
* Returns        : overload_response - converted value corrosponding to 
*                  mme_overload_response_et enum
* Description    : This function returns the mme_overload_response_et corrosponding
*                  value to the ASN.1 Message
*****************************************************************************/
mme_overload_response_et convert_overload_response(
        s1ap_OverloadAction overload_action)
{
    mme_overload_response_et overload_response = OVERLOAD_START_INVALID_VALUE;
    RRC_S1AP_UT_TRACE_ENTER();

    switch (overload_action)
    {
        case (s1ap_OverloadAction)s1ap_reject_non_emergency_mo_dt:
            {
                overload_response = REJ_ALL_RRC_CONN_MO_DT;
                break;
            }
        case (s1ap_OverloadAction)s1ap_reject_rrc_cr_signalling:
            {
                overload_response = REJ_ALL_RRC_CONN_MO_DT_MO_SIG;
                break;
            }
        case (s1ap_OverloadAction)s1ap_permit_emergency_sessions_and_mobile_terminated_services_only:
            {
                overload_response = PERMIT_EMERGENCY_MO_ACCESS;
                break;
            }
        /* CR 776 changes*/
        case (s1ap_OverloadAction)s1ap_permit_high_priority_sessions_and_mobile_terminated_services_only:
            {
                overload_response = PERMIT_HIGH_PRIORITY_MT_ACCESS;
                break;
            }
        case (s1ap_OverloadAction)s1ap_reject_delay_tolerant_access:
            {
                overload_response = REJ_ALL_RRC_DELAY_TOLERANCE;
                break;
            }
        default:
            {
                /* ERR: */
                RRC_S1AP_TRACE(RRC_ERROR,
                        "%s: %d Incorrect Overload Response recieved",
                        __FUNCTION__, overload_action);
            }
    }
    RRC_S1AP_UT_TRACE_EXIT();

    return overload_response;
}

/*****************************************************************************
* Function Name  : parse_asn_overload_stop
* Inputs         : p_s1ap_gb_ctx - pointer to S1ap global context 
*                  p_mme_ctx - pointer to MME context 
*                  p_asn_msg - pointer to asn msg
*                  asn_msg_len - lenghh of asn msg
* Outputs        : None 
* Returns        : RRC_SUCCESS - Asn Encoding was successful.
*                  RRC_FAILURE - Asn Encoding was not successful.
* Description    : This function decode the overload stop ASN.1 Message 
*****************************************************************************/
rrc_return_et parse_asn_overload_stop(
        mme_context_t           *p_mme_ctx,
        void                            *p_asn_msg,
        U16                             asn_msg_len,
        parsed_overload_stop_t *p_parsed_overload_stop_msg
        )
{
    S1AP_PDU s1ap_pdu;
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_SUCCESS;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Init S1AP PDU */
    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);
            result = RRC_FAILURE;
            break;
        }
        /* Decode message */
        if (RRC_FAILURE == s1ap_overload_stop_intrl_dec(
                    p_mme_ctx,
                    s1ap_pdu.u.initiatingMessage->value.u.overloadStop,
                    p_parsed_overload_stop_msg))
        {
            result = RRC_FAILURE;
            break;
        }
    }while (0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_overload_stop_intrl_dec
 * Inputs         : p_mme_ctx - pointer to MME context 
 *                  p_3gpp - pointer to s1ap_OverloadStop
 * Outputs        : None 
 * Returns        : RRC_SUCCESS?RRC_FAILURE
 * Description    : Gets the Decoded Overload stop MME message
 ****************************************************************************/
rrc_return_et s1ap_overload_stop_intrl_dec
(
 mme_context_t *p_mme_ctx,
 s1ap_OverloadStop *p_3gpp,    /* unpacked src */
 parsed_overload_stop_t  *p_intrl
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_OverloadStop_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_SUCCESS;
    
    /* Message Map */
    s1ap_message_data_t message_map = 
    {1, RRC_FALSE, RRC_FALSE, 0, 0, 0,
        {{0, ASN1V_s1ap_id_GUMMEIList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(s1ap_error_ind_bool_t));

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);

    do
    {
        /* 1. Class-1 Error Unable to understand the IE or Unknow IE ID
        */
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
                (s1ap_OverloadStop_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_GUMMEIList:
                    {
                        if (RRC_SUCCESS ==
                                validate_ie_value(
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._OverloadStopIEs_1))
                        {
                            p_intrl->bitmask |=
                                S1AP_S1_OVERLOAD_STOP_GUMMEILIST_PRESENT;
                            p_intrl->s1ap_gummelist =
                                *p_protocol_IE->value.u._OverloadStopIEs_1;
                        } 
                    }
                    break;

                 default :  
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
            p_node= p_node->next;            
        }

    } while (0);

    if (RRC_SUCCESS == parse_message_map(
                &asn1_ctx,
                p_mme_ctx,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_OverloadStop,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/******************************************************************************
* Function Name  : parse_asn_enb_config_update_ack
* Inputs         : p_s1ap_gb_ctx - pointer to s1ap global contxet 
*                  p_mme_ctx - pointer to MME context 
*                  p_asn_msg - pointer to ASN msg 
*                  asn_msg_len - asn message length 
* Outputs        : None. 
* Returns        : RRC_SUCCESS - Asn Encoding was successful.
*                  RRC_FAILURE - Asn Encoding was not successful.
* Description    : This function decode eNB configuration update in ack mode  
*****************************************************************************/
rrc_return_et parse_asn_enb_config_update_ack( 
        mme_context_t           *p_mme_ctx,
        void                      *p_asn_msg,
        U16                       asn_msg_len
        )

{
    S1AP_PDU s1ap_pdu;
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);
            result = RRC_FAILURE;
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                   (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE,
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* Decode message */
        if (RRC_FAILURE ==s1ap_enb_config_update_ack_intrl_dec(
                    p_mme_ctx, 
                    s1ap_pdu.u.successfulOutcome->value.u.eNBConfigurationUpdate))
        {
            result = RRC_FAILURE;
            break;
        }

    }while (0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}
/****************************************************************************
 * Function Name  :s1ap_enb_config_update_ack_intrl_dec 
 * Inputs         : p_mme_context - pointer to MME context 
 *                  p_3gpp - s1ap_ENBConfigurationUpdateAcknowledge
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE/RRC_PARTIAL_SUCCESS
 * Description    : This function gets the Decoded from MME for eNB 
 *                  Config update for Acknowledge mode
 ****************************************************************************/
rrc_return_et s1ap_enb_config_update_ack_intrl_dec 
(
 mme_context_t *p_mme_context,
 s1ap_ENBConfigurationUpdateAcknowledge *p_3gpp    /* unpacked src */

 )
{
    OSRTDListNode                   *p_node = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    U8 index = 0;
    s1ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element 
        *p_protocol_IE = RRC_NULL;
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;
    OSCTXT asn1_ctx;

    s1ap_message_data_t message_map =
    {1, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

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
                (s1ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_CriticalityDiagnostics",
                                __FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._ENBConfigurationUpdateAcknowledgeIEs_1)
                                    )
                        {
                            /* We have to log this message */
                            log_criticality_diagostics(p_protocol_IE->value.u._ENBConfigurationUpdateAcknowledgeIEs_1);
                        }

                    }
                    break;
                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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

    } while (0);

    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context,
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_ENBConfigurationUpdate,
                    T_S1AP_PDU_successfulOutcome,
                    s1ap_reject,
                    (rrc_s1ap_error_indication_t *)PNULL))  
       ) 
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "enb Config Update ack- Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}



/*****************************************************************************
* Function Name  : parse_asn_enb_config_update_failure
* Inputs         : p_mme_context - pointer to MME context 
*                  p_asn_msg - pointer to asn message
*                  asn_msg_len - asn meg length 
*                  p_enb_config_update_failure - pointer to 
*                                                enb_config_update_failure_t
* Outputs        : None 
* Returns        : RRC_SUCCESS - Asn Encoding was successful.
*                  RRC_FAILURE - Asn Encoding was not successful.
* Description    : This function parses the enb config update failure 
*****************************************************************************/
rrc_return_et parse_asn_enb_config_update_failure(
        mme_context_t *p_mme_context,
        void *p_asn_msg,
        U16 asn_msg_len,
        enb_config_update_failure_t *p_enb_config_update_failure)
{
    S1AP_PDU s1ap_pdu;
    OSCTXT asn1_ctx;
    rrc_return_et result = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_context, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_ENB_CONFIGURATION_UPDATE_FAILURE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* Decode message */
        if (RRC_FAILURE ==  s1ap_enb_confguration_update_failure_intrl_dec(
                    p_mme_context,
            s1ap_pdu.u.unsuccessfulOutcome->value.u.eNBConfigurationUpdate,
                    p_enb_config_update_failure
                    ))
        {
            result = RRC_FAILURE;
            break;
        }

    }while (0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}
/****************************************************************************
 * Function Name  : s1ap_enb_confguration_update_failure_intrl_dec
 * Inputs         : p_mme_context - pointer to MME context
 *                  p_3gpp - Decoded s1ap_S1SetupResponse
 *                  p_intrl - pointer to enb_config_update_failure_t
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : Gets the Decoded MME Configuration Update message
 ****************************************************************************/
rrc_return_et s1ap_enb_confguration_update_failure_intrl_dec
(
 mme_context_t *p_mme_context,
 s1ap_ENBConfigurationUpdateFailure  *p_3gpp,    /* unpacked src */
 enb_config_update_failure_t  *p_intrl   /* dest */

 )
{
    OSRTDListNode                   *p_node = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    U8 index = 0;
    s1ap_ENBConfigurationUpdateFailure_protocolIEs_element
        *p_protocol_IE = RRC_NULL;
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;
    OSCTXT asn1_ctx;

    s1ap_message_data_t message_map =
    {3, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_Cause, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_TimeToWait, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

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
                (s1ap_ENBConfigurationUpdateFailure_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_Cause:
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_Cause",
                                __FUNCTION__);

                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._ENBConfigurationUpdateFailureIEs_1
                                    ))
                        {
                            p_intrl->cause =
                                *(p_protocol_IE->value.u._ENBConfigurationUpdateFailureIEs_1);
                        }
                    }
                    break;
                case ASN1V_s1ap_id_TimeToWait:
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_TimeToWait",
                                __FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &(p_protocol_IE->value.u._ENBConfigurationUpdateFailureIEs_2)
                                    ))
                        {
                            p_intrl->time_to_wait = (s1ap_TimeToWait_Root)p_protocol_IE->value.u.
                                _ENBConfigurationUpdateFailureIEs_2;
                            p_intrl->bitmask |= S1AP_TIME_TO_WAIT_PRESENT;
                        }

                    }
                    break;
                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                "ASN1V_s1ap_id_CriticalityDiagnostics",
                                __FUNCTION__);
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._ENBConfigurationUpdateFailureIEs_3
                                    ))
                        {
                            p_intrl->bitmask |=S1AP_CRITICALITY_DIAGNOSTICS_PRESENT;
                            /* We have to log this message */
                            log_criticality_diagostics(p_protocol_IE->value.u._ENBConfigurationUpdateFailureIEs_3);
                        }

                    }
                    break;
                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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

    } while (0);
    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_ENBConfigurationUpdate,
                    T_S1AP_PDU_unsuccessfulOutcome,
                    s1ap_reject,
                    (rrc_s1ap_error_indication_t *)PNULL)) ) 
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "eNB Config Update failure - Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* etws_start*/
/*****************************************************************************
 * Function Name  : parse_asn_write_replace_warning_req
 * Inputs         : p_asn_msg - Pointer to S1 SETUP RESPONSE Asn message to be
 *                              decoded.
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_rrc_s1ap_write_replace_warning_request - pointer to 
 *                                        rrc_s1ap_write_replace_warning_request_t
 *                  p_mme_ctx - pointer to MME context 
 * Outputs        : p_rrc_s1ap_write_replace_warning_request
 * Returns        : RRC_SUCCESS - Asn Dcoding was successful.
 *                  RRC_FAILURE - Asn Dcoding was not successful.
 * Description    : This function processes write replace warning request.
 *****************************************************************************/
rrc_return_et parse_asn_write_replace_warning_req (
        void                *p_asn_msg,
        OSCTXT              *p_asn1_ctx,
        U16                 asn_msg_len,
        rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_write_replace_warning_request,
        mme_context_t* p_mme_ctx)
{

    S1AP_PDU                        s1ap_pdu;
    rrc_return_et                   result = RRC_SUCCESS;

    RRC_S1AP_UT_TRACE_ENTER();


    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(p_asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
	    /* SPR 13710 Fix Start */
   	    s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);	
	    /* SPR 13710 Fix Stop */
            result = RRC_FAILURE;
            break;
        }

        /* check whether the message is initiating mesg or not*/
        if( (T_S1AP_PDU_initiatingMessage != s1ap_pdu.t) ||
                (ASN1V_s1ap_id_WriteReplaceWarning != 
                 s1ap_pdu.u.initiatingMessage->procedureCode) ||
                (PNULL == s1ap_pdu.u.initiatingMessage->value.u.writeReplaceWarning) ||
                 (T1_writeReplaceWarning !=
                    s1ap_pdu.u.initiatingMessage->value.t))
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
        
        /* Decode message */
        if (RRC_FAILURE == s1ap_write_replace_warning_req_intrl_dec(
            s1ap_pdu.u.initiatingMessage->value.u.writeReplaceWarning,
            p_rrc_s1ap_write_replace_warning_request,
            p_mme_ctx
            ))
          {
            /* Message parsing error */
            RRC_S1AP_TRACE(RRC_WARNING,
                    "[s1ap_write_replace_warning_req] Decoding error");
            result = RRC_FAILURE;
            /* Drop message */
            break;
          }

    }while (0);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}
/* etws_end*/


/*****************************************************************************
 * Function Name  : compose_s1ap_mme_configuration_update_ack
 * Inputs         : p_message - pointer to msg 
 *                  p_msg_buff_len - pointer to the msg length 
 *                  p_err_ind - pointer to rrc_s1ap_error_indication_t
 *                  p_snd_err_ind - pointer to s1ap_error_ind_bool_t
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function builds and sends mme configuration update 
 *                  acknowledgement message
 *****************************************************************************/
rrc_return_et  compose_s1ap_mme_configuration_update_ack(
        U8 *p_message,
        U16*  p_msg_buff_len,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_snd_err_ind)
{
    OSCTXT      asn1_ctx; 
    S1AP_PDU    s1ap_pdu;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    s1ap_MMEConfigurationUpdateAcknowledge  *p_s1ap_MMEConfigurationUpdateAcknowledge =PNULL;
    s1ap_MMEConfigurationUpdateAcknowledge_protocolIEs_element *p_elem = PNULL;

    RRC_ASSERT(PNULL != p_message);
    RRC_ASSERT(PNULL != p_msg_buff_len);
    
    if (RRC_TRUE == p_snd_err_ind->send_err_indication_reject_ie_present)
    {
        return RRC_FAILURE;
    }

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[MME_CONFIG_UPDATE_ACK] "
                "ASN context initialization failed");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        /* INIT */
        asn1Init_S1AP_PDU(&s1ap_pdu);

        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome ;
        s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
                SuccessfulOutcome);

        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_SuccessfulOutcome (s1ap_pdu.u.successfulOutcome);

        s1ap_pdu.u.successfulOutcome->procedureCode = 
            ASN1V_s1ap_id_MMEConfigurationUpdate ;
/* SPR:9937 start */
        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;
/* SPR:9937 stop */
        s1ap_pdu.u.successfulOutcome->value.t = T1_mMEConfigurationUpdate;
        p_s1ap_MMEConfigurationUpdateAcknowledge = rtxMemAllocType(&asn1_ctx,
                s1ap_MMEConfigurationUpdateAcknowledge);


        if (PNULL == p_s1ap_MMEConfigurationUpdateAcknowledge)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_s1ap_MMEConfigurationUpdateAcknowledge(p_s1ap_MMEConfigurationUpdateAcknowledge);

        s1ap_pdu.u.successfulOutcome->value.u.mMEConfigurationUpdate =
            p_s1ap_MMEConfigurationUpdateAcknowledge;

        rtxDListInit(&p_s1ap_MMEConfigurationUpdateAcknowledge->protocolIEs);

        if (RRC_TRUE == p_snd_err_ind->send_err_indication_notify_ie_present)
        {
            p_elem = rtxMemAllocType (&asn1_ctx, 
                    s1ap_MMEConfigurationUpdateAcknowledge_protocolIEs_element);

            if (PNULL == p_elem)
            {
                RRC_TRACE(RRC_WARNING, "MALLOC Failure");
                break;
            }
            p_elem->value.u._MMEConfigurationUpdateAcknowledgeIEs_1 = rtxMemAllocType (&asn1_ctx, 
                   s1ap_CriticalityDiagnostics);

            if (PNULL == p_elem->value.u._MMEConfigurationUpdateAcknowledgeIEs_1)
            {
                RRC_TRACE(RRC_WARNING, "MALLOC Failure");
                break;
            }
            asn1Init_s1ap_CriticalityDiagnostics(p_elem->value.u._MMEConfigurationUpdateAcknowledgeIEs_1);
            p_elem->value.t = T81__MMEConfigurationUpdateAcknowledgeIEs_1;

            p_elem->id = ASN1V_s1ap_id_CriticalityDiagnostics;
            p_elem->criticality = s1ap_ignore;
            l3_memcpy_wrapper(p_elem->value.u._MMEConfigurationUpdateAcknowledgeIEs_1, &(p_err_ind->criticality_diagnostics),
            sizeof(s1ap_CriticalityDiagnostics));

            rtxDListAppend (&asn1_ctx,
                    &p_s1ap_MMEConfigurationUpdateAcknowledge->protocolIEs,
                    p_elem);
        }

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx,
                p_message,
                S1AP_MAX_ASN1_BUF_LEN, 
                TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of MME Configuration "
                    "update ack failed");
            break;
        }
        else
        {
            *p_msg_buff_len = (U16)pe_GetMsgLen(&asn1_ctx);
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                              (U32)*p_msg_buff_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)*p_msg_buff_len,
                        LOCAL_S1_MME_CONFIGURATION_UPDATE_ACKNOWLEDGE,
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_message,                        
                        (U32)*p_msg_buff_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;

}


/*****************************************************************************
 * Function Name  : build_and_send_mme_configuration_update_failure
 * Inputs         : p_message - pointer to the message
 *                  p_message_buf_len - pointer to the message buf length 
 *                  p_err_ind - pointer to rrc_s1ap_error_indication_t
 *                  p_snd_err_ind - pointer to s1ap_error_ind_bool_t
 * Outputs        : failure response
 * Returns        : RRC_SUCCESS - Asn Encoding was successful.
 *                  RRC_FAILURE - Asn Encoding was not successful.
 * Description    : This function builds and send mme_configuration update 
 *                  failure mesage
****************************************************************************/
rrc_return_et  compose_s1ap_mme_configuration_update_failure(
        U8* p_message,
        U16* p_message_buf_len,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_snd_err_ind)
{

    rrc_return_et result = RRC_SUCCESS;
    OSCTXT      asn1_ctx;
    S1AP_PDU    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    s1ap_MMEConfigurationUpdateFailure_protocolIEs_element*
        p_protocolIE_elem = PNULL;

    s1ap_MMEConfigurationUpdateFailure* p_nack_msg = PNULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_message);
    RRC_ASSERT(PNULL != p_message_buf_len);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[compose_s1ap_mme_configuration_update_failure] "
                "ASN context initialization failed");
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Init */
    asn1Init_S1AP_PDU(&s1ap_pdu);

    do
    {
        s1ap_pdu.t =  T_S1AP_PDU_unsuccessfulOutcome;
        s1ap_pdu.u.unsuccessfulOutcome =
            rtxMemAllocType(&asn1_ctx, UnsuccessfulOutcome);

        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome)
        {
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            result = RRC_FAILURE;
            break;
        }

        /* Init */
        asn1Init_UnsuccessfulOutcome(s1ap_pdu.u.unsuccessfulOutcome);

        s1ap_pdu.u.unsuccessfulOutcome->procedureCode =
            ASN1V_s1ap_id_MMEConfigurationUpdate ;
        s1ap_pdu.u.unsuccessfulOutcome->criticality = s1ap_reject;
        s1ap_pdu.u.unsuccessfulOutcome->value.t =
            T1_mMEConfigurationUpdate;
        s1ap_pdu.u.unsuccessfulOutcome->value.u.mMEConfigurationUpdate =
            rtxMemAllocType(&asn1_ctx, s1ap_MMEConfigurationUpdateFailure);

        if (PNULL == 
                s1ap_pdu.u.unsuccessfulOutcome->value.u.mMEConfigurationUpdate)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            result = RRC_FAILURE;
            break;
        }

        p_nack_msg = 
            s1ap_pdu.u.unsuccessfulOutcome->value.u.mMEConfigurationUpdate;

        asn1Init_s1ap_MMEConfigurationUpdateFailure(p_nack_msg);
        rtxDListInit(&p_nack_msg->protocolIEs);

        /*** Compose id: id-Cause ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_MMEConfigurationUpdateFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);


        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        asn1Init_s1ap_MMEConfigurationUpdateFailure_protocolIEs_element(
                p_protocolIE_elem);
        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_Cause;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T82__MMEConfigurationUpdateFailureIEs_1;
        p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_1 =
            rtxMemAllocType(&asn1_ctx, s1ap_Cause );

        if( PNULL == p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_1)
        {

            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_s1ap_Cause (p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_1);
        
        if (RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT & p_err_ind->bitmask)
        {
            l3_memcpy_wrapper(p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_1,
                    &p_err_ind->cause,
                    sizeof(s1ap_Cause));
        }
        else
        {
        p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_1->t =
            T_s1ap_Cause_misc ;
        p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_1->u.misc =
            s1ap_unknown_PLMN ;
        }
        rtxDListAppendNode(&p_nack_msg->protocolIEs, p_node);

        /*** Compose id: id-time to wait ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_MMEConfigurationUpdateFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        asn1Init_s1ap_MMEConfigurationUpdateFailure_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_TimeToWait;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T82__MMEConfigurationUpdateFailureIEs_2;
        p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_2 =
            s1ap_v60s ;
        rtxDListAppendNode(&p_nack_msg->protocolIEs, p_node);

        /*** compose id-CriticalityDiagnostics ***/
        if (RRC_TRUE == p_snd_err_ind->send_err_indication_reject_ie_present || 
                RRC_TRUE == p_snd_err_ind->send_err_indication_notify_ie_present)
        {
        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_MMEConfigurationUpdateFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        asn1Init_s1ap_MMEConfigurationUpdateFailure_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_s1ap_id_CriticalityDiagnostics;
        p_protocolIE_elem->criticality = s1ap_ignore;
            p_protocolIE_elem->value.t = T82__MMEConfigurationUpdateFailureIEs_3;

            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.u._MMEConfigurationUpdateFailureIEs_3 = 
                &p_err_ind->criticality_diagnostics;

            rtxDListAppendNode(&p_nack_msg->protocolIEs, p_node);
        }

        rtxDListInit(&p_nack_msg->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx,
                p_message,
                S1AP_MAX_ASN1_BUF_LEN,
                TRUE);
        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR," ASN1 encoding of MME Configuration "
                    "Update Failure failed");
            result = RRC_FAILURE;
            break;
        }
        else
        {
            *p_message_buf_len= (U16)pe_GetMsgLen(&asn1_ctx);
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 (U32)*p_message_buf_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)*p_message_buf_len,
                        LOCAL_S1_MME_CONFIGURATION_UPDATE_FAILURE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_message,
                        (U32)*p_message_buf_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/*****************************************************************************
* Function Name  : parse_asn_mme_configuration_update
* Inputs         : p_asn1_ctx - pointer to OSCTXT 
*                  p_mme_context - pointer to MME context 
*                  p_asn_msg - pointer to ASN encoded msg
*                  asn_msg_len - length of ASN encoded length 
*                  p_mme_config_update - poiinter to mme_configuration_update_t
*                  p_error_ind_content - pointer to rrc_s1ap_error_indication_t
*                  p_send_err_ind - pointyer to s1ap_error_ind_bool_t
* Outputs        : p_mme_config_update
* Returns        : RRC_SUCCESS /RRC_FAILURE
* Description    : This function parses  ASN encoded mme configuration update 
*                   mesage.
****************************************************************************/
rrc_return_et parse_asn_mme_configuration_update(
        OSCTXT *p_asn1_ctx,
        mme_context_t        * p_mme_ctx,
        U8 *p_asn_msg,
        U16 asn_msg_len,
        mme_configuration_update_t *p_mme_config_update,
        rrc_s1ap_error_indication_t *p_error_ind_content,
        s1ap_error_ind_bool_t *p_send_err_ind)
{
    S1AP_PDU        s1ap_pdu;
    OSCTXT          asn1_ctx;
    rrc_return_et   result = RRC_SUCCESS;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

     RRC_S1AP_UT_TRACE_ENTER();

     RRC_ASSERT(PNULL != p_mme_config_update);
     RRC_ASSERT(PNULL != p_asn_msg);

    if (p_mme_config_update == PNULL)
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "p_mme_config_update is NULL",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Init ASN1 context */
    if (0 != rtInitContext(&asn1_ctx))
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

     asn1Init_S1AP_PDU(&s1ap_pdu);

     /*Set pointer of asn buffer in asn context*/
     pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

     do
     {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);

            result = RRC_FAILURE;
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                               (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_MME_CONFIGURATION_UPDATE, MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* Decode message */
        if(RRC_FAILURE == s1ap_mme_configuration_update_intrl_dec(
                    p_asn1_ctx,
                    p_mme_ctx,
                    s1ap_pdu.u.initiatingMessage->value.u.mMEConfigurationUpdate,
                    p_mme_config_update,
                    p_error_ind_content,
                    p_send_err_ind))
        {
            result = RRC_FAILURE;
            break;
        }

    } while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : s1ap_mme_configuration_update_intrl_dec
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_mme_ctx - pointer to MME context 
 *                  p_3gpp - Decoded s1ap_S1SetupResponse
 *                  p_intrl - pointer to mme_configuration_update_t
 *                  p_err_ind_content - pointer to rrc_s1ap_error_indication_t
 *                  p_send_err_ind - pointer to s1ap_error_ind_bool_t
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : Gets the Decoded MME Configuration Update message
 ****************************************************************************/
rrc_return_et s1ap_mme_configuration_update_intrl_dec
(
 OSCTXT *p_asn1_ctx,
 mme_context_t *p_mme_ctx,
 s1ap_MMEConfigurationUpdate  *p_3gpp,    /* unpacked src */
 mme_configuration_update_t   *p_intrl,   /* dest */
 rrc_s1ap_error_indication_t  *p_err_ind_content,
 s1ap_error_ind_bool_t        *p_send_err_ind
 )
{
	OSRTDListNode  *p_node     = PNULL;
	s1ap_MMEConfigurationUpdate_protocolIEs_element 
		*p_protocol_IE = PNULL;
	U32  index = RRC_NULL;
	U16  ie_list_index = 0;
	U16 order_index = RRC_NULL;
	rrc_return_et result = RRC_SUCCESS;
	s1ap_error_ind_ie_list_t   iE_list;
	parsed_gummei_info   gummei_info;

	s1ap_message_data_t message_map =
	{3, RRC_TRUE, RRC_TRUE, 0, 0, 0,
		{{0, ASN1V_s1ap_id_MMEname, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
			{1, ASN1V_s1ap_id_ServedGUMMEIs, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
			{2, ASN1V_s1ap_id_RelativeMMECapacity, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

	memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
	RRC_UT_TRACE_ENTER();
	RRC_ASSERT(PNULL != p_3gpp);
	RRC_ASSERT(PNULL != p_intrl);

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
				(s1ap_MMEConfigurationUpdate_protocolIEs_element *)
				p_node->data;

			switch (p_protocol_IE->id)
			{
				case ASN1V_s1ap_id_MMEname:
					{
						if (RRC_SUCCESS ==
								validate_ie_value(
									&message_map,
									order_index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._MMEConfigurationUpdateIEs_1))
						{
                            /* SPR 14470 Changes Start */
							p_intrl->bitmask |= MME_CONFIGURATION_MME_NAME_PRESENT;
                            /* SPR 14470 Changes End */
							/* Assigne the value */
							l3_memcpy_wrapper(p_intrl->mme_name,
									p_protocol_IE->value.u._MMEConfigurationUpdateIEs_1,
									strnlen_wrapper((const S8 *)(p_protocol_IE->value.u._MMEConfigurationUpdateIEs_1),
										MAX_MME_NAME_STR));

						}
						break;
					}

				case ASN1V_s1ap_id_ServedGUMMEIs:
					{
						if (RRC_SUCCESS ==
								validate_ie_value(
									&message_map,
									order_index,
									p_protocol_IE->id,
									p_protocol_IE->value.u._MMEConfigurationUpdateIEs_2))
						{
							/* Assigne the value */
							memset_wrapper(&gummei_info,0,sizeof(parsed_gummei_info));
							gummei_info.p_served_gummeis = rrc_mem_get(sizeof(served_gummei_info_t)
									* ASN1V_s1ap_maxnoofRATs);
							p_intrl->gummei_info.p_served_gummeis =  gummei_info.p_served_gummeis;

							if (!p_intrl->gummei_info.p_served_gummeis)
							{
								RRC_S1AP_TRACE(RRC_FATAL,"%s: "
										"Unable to allocate Mem for served_gummeis",
										__FUNCTION__);

								update_message_map(&message_map, DATA_MISSING, index, p_protocol_IE->id);
								break;
							}

							/* spr_11647_fix_start */
							memset_wrapper(p_intrl->gummei_info.p_served_gummeis,
									RRC_NULL,
									sizeof(served_gummei_info_t)*ASN1V_s1ap_maxnoofRATs);

							if( RRC_FAILURE == parse_asn_served_gummeis(&p_intrl->gummei_info,
										p_protocol_IE->value.u._MMEConfigurationUpdateIEs_2,
										&message_map,
										(U8)order_index,
										(U8)p_protocol_IE->id))
							{
								break;
							}
            				/* CSR_00070383 Fix + */
							p_intrl->bitmask |= MME_CONFIGURATION_SERVED_GUMMEI;
            				/* CSR_00070383 Fix - */
							/* spr_11647_fix_stop */

						}
						break;
					}

				case ASN1V_s1ap_id_RelativeMMECapacity:
					{
						if (RRC_SUCCESS ==
								validate_ie_value(
									&message_map,
									order_index,
									p_protocol_IE->id,
									&p_protocol_IE->value.u._MMEConfigurationUpdateIEs_3))
						{
                            /* SPR 14470 Changes Start */
                            p_intrl->bitmask |= MME_CONFIGURATION_REL_MME_CAPACITY_PRESENT;  
                            /* SPR 14470 Changes End */
							/* Assigne the value */
							p_intrl->relative_mme_capacity =
								p_protocol_IE->value.u._MMEConfigurationUpdateIEs_3;
						}
						break;
					}

				default:
					/* Class - 1 Error - Unknown IE */
					/* Base on Criticality fill the value */
					add_to_err_ind_ie_list(&iE_list,
							p_protocol_IE->criticality,
							p_protocol_IE->id,
							&ie_list_index,
							p_send_err_ind,
							RRC_FALSE,
							RRC_FALSE);
			}

			p_node= p_node->next;            
			order_index++;
		}
	} while (0);

	if (RRC_SUCCESS == parse_message_map(
				p_asn1_ctx,
				p_mme_ctx,
				&message_map,
				&iE_list,
				&ie_list_index,
				p_send_err_ind,
				ASN1V_s1ap_id_MMEConfigurationUpdate,
				T_S1AP_PDU_initiatingMessage,
				s1ap_reject,
				p_err_ind_content))
	{
		RRC_TRACE(RRC_INFO, "Error Indication Detected");
		result = RRC_FAILURE;
	}

	RRC_UT_TRACE_EXIT();
	return result;
}

/*****************************************************************************
 * Function Name  : compose_asn_enb_config_update 
 * Inputs         : p_enb_config_update - OAM Sent filled buffer
 *                  p_s1_setup_req - pointer to s1_setup_req_t
 *                  p_mme_ctxt - pointer to MME context 
 *                  asn_enc_msg_buf- pointer to ASN encoded msg buf
 *                  asn_enc_buf_len - pointer to ASN encoded msg buf length 
 * Outputs        : asn_enc_msg_buf - ASN.1 Buffer to be encoded 
 *                  asn_enc_buf_len - ASN.1 Buffer length 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function compose the ASN.1 eNB Configuraion update 
 *                  message
 ******************************************************************************/
rrc_return_et compose_asn_enb_config_update(
        s1ap_oam_enb_config_update_t *p_enb_config_update,
        /*SPR 17777 +-*/
        U8 *asn_enc_msg_buf,
        U16 *asn_enc_buf_len)
        /*SPR 17777 +-*/
{
    rrc_return_et return_value = RRC_SUCCESS;
    /* Encode the ASN Buffer to be sent to MME */
    S1AP_PDU    s1ap_pdu;
    OSCTXT      asn1_ctx;
    OSRTDListNode *p_node = RRC_NULL;
    s1ap_ENBConfigurationUpdate *p_ENBConfigurationUpdate = PNULL;

    s1ap_ENBConfigurationUpdate_protocolIEs_element 
        *p_enb_config_update_protocol_IE = PNULL;
    OSRTDList s1ENBConfigurationUpdateIEs_2;
    OSRTDList s1ENBConfigurationUpdateIEs_3;
    U16 bitmask  = RRC_NULL;

    U8* p_enb_name = PNULL;
    supp_ta_list_t *p_supp_ta_list = PNULL; 
    csg_id_list_t *p_csg_id_list = PNULL;
    U32 default_paging_drx = RRC_NULL; 
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_enb_config_update);
    /*SPR 17777 +-*/
    RRC_ASSERT(PNULL != asn_enc_msg_buf);
    RRC_ASSERT(PNULL != asn_enc_buf_len);
    /*SPR 17777 +-*/

    /* Init ASN1 context */
    if ( RRC_NULL != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, 
                "%s: ASN context initialization failed.",__FUNCTION__);

        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
            if(p_enb_config_update)
            {
                if (S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT & p_enb_config_update->bitmask)
                {
                    bitmask |= S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT;
                    p_enb_name = p_enb_config_update->enb_name;
                }
                if( S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT &
                        p_enb_config_update->bitmask)
                {
                    bitmask |= S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT;
                    p_supp_ta_list = &p_enb_config_update->supp_ta_list;
                }

                if( S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT &
                        p_enb_config_update->bitmask)
                {
                    bitmask |= S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT;
                    p_csg_id_list = &p_enb_config_update->csg_id_list;
                }

                if( S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT  &
                        p_enb_config_update->bitmask)
                {
                    bitmask |= S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT;
                    default_paging_drx = p_enb_config_update->default_paging_drx;
                }
            }

        /* Init S1AP PDU */
        asn1Init_S1AP_PDU(&s1ap_pdu);

        /* Set the PDU Type as Initiation Message*/
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        /*Allocate the memory to intitiating message type*/
        s1ap_pdu.u.initiatingMessage = 
            rtxMemAllocType(&asn1_ctx, InitiatingMessage);

        /* Check for NULL */
        if(PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            return_value = RRC_FAILURE;
            break;
        }

        /* Initialize the memory Allocated */
        asn1Init_InitiatingMessage(s1ap_pdu.u.initiatingMessage);

        /* Assign the procedure code for Enb Config update */
        s1ap_pdu.u.initiatingMessage->procedureCode = 
            ASN1V_s1ap_id_ENBConfigurationUpdate;

        /* Set the Criticality and the Type of message */
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;
        s1ap_pdu.u.initiatingMessage->value.t = T1_eNBConfigurationUpdate;

        /* Allocate the memory for the eNB Config update pointer */
        s1ap_pdu.u.initiatingMessage->value.u.eNBConfigurationUpdate = 
            rtxMemAllocType(&asn1_ctx, s1ap_ENBConfigurationUpdate);

        /* Check for NULL */
        if(PNULL == s1ap_pdu.u.initiatingMessage->value.u.eNBConfigurationUpdate)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
            return_value = RRC_FAILURE;
            break;
        }

        /* Pointer assignment */
        p_ENBConfigurationUpdate = 
            s1ap_pdu.u.initiatingMessage->value.u.eNBConfigurationUpdate;

        /* Init the allocated memory and elements*/
        asn1Init_s1ap_ENBConfigurationUpdate(p_ENBConfigurationUpdate);
        asn1Init_s1ap_ENBConfigurationUpdate_protocolIEs(
                &p_ENBConfigurationUpdate->protocolIEs);

        /* Compose the eNB NAME */
        if ((S1AP_OAM_ENB_CONFIG_UPDATE_ENB_NAME_PRESENT & bitmask) && (PNULL != p_enb_name))
        {
            /* Allocate the memory for the element */
            rtxDListAllocNodeAndData(&asn1_ctx,
                    s1ap_ENBConfigurationUpdate_protocolIEs_element,
                    &p_node,
                    &p_enb_config_update_protocol_IE);

            /* NULL Check for the allocated Memory*/
            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return_value = RRC_FAILURE;
                break;
            }

            /* Initialize the elements */
            asn1Init_s1ap_ENBConfigurationUpdate_protocolIEs_element(
                    p_enb_config_update_protocol_IE);

            p_enb_config_update_protocol_IE->id = ASN1V_s1ap_id_eNBname;
            p_enb_config_update_protocol_IE->criticality = s1ap_ignore;
            p_enb_config_update_protocol_IE->value.t = 
                T77__ENBConfigurationUpdateIEs_1;

            /* Assign the value */
            p_enb_config_update_protocol_IE->value.u._ENBConfigurationUpdateIEs_1 =
                (s1ap_ENBname)p_enb_name; 

            /* Append the created Node */
            rtxDListAppendNode(&p_ENBConfigurationUpdate->protocolIEs, p_node);
        }

        /* Compose TA List */

        if ((S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT & bitmask) && (PNULL != p_supp_ta_list))
        {
            /* allocate the memory for elements */
            rtxDListAllocNodeAndData (
                    &asn1_ctx,
                    s1ap_ENBConfigurationUpdate_protocolIEs_element,
                    &p_node,
                    &p_enb_config_update_protocol_IE);

            /* NULL Check */
            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                return_value = RRC_FAILURE;
                break;
            }

            /* Initialize the memory allocated */
            asn1Init_s1ap_ENBConfigurationUpdate_protocolIEs_element(
                    p_enb_config_update_protocol_IE);

            p_enb_config_update_protocol_IE->id = ASN1V_s1ap_id_SupportedTAs;
            p_enb_config_update_protocol_IE->criticality = s1ap_reject;
            p_enb_config_update_protocol_IE->value.t = 
                T77__ENBConfigurationUpdateIEs_2;

            /* Initialize the Node */
            rtxDListInit(&s1ENBConfigurationUpdateIEs_2);

            /* Assign the pointer intilzlized */
            p_enb_config_update_protocol_IE->value.u._ENBConfigurationUpdateIEs_2 =
                &s1ENBConfigurationUpdateIEs_2;

            /* Populate the TAs list */
            if (RRC_FAILURE == compose_supported_tas(&asn1_ctx,
                        p_enb_config_update_protocol_IE->value.u._ENBConfigurationUpdateIEs_2,
                        p_supp_ta_list))
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s compose_supported_tas "
                        "returned Failure.",
                        __FUNCTION__);
                return_value = RRC_FAILURE;
                break;
            }

            /* Append the Node to the ASN list */
            rtxDListAppendNode(&p_ENBConfigurationUpdate->protocolIEs, p_node);
        }
        /* Compose CSG List */
        /* Check if the CSG List is EMPTY */
        if ((S1AP_OAM_ENB_CONFIG_UPDATE_CSG_ID_LIST_PRESENT & bitmask) && (PNULL != p_csg_id_list))
        {
            /* Allocate the Memory for the Node */
            rtxDListAllocNodeAndData(
                    &asn1_ctx,
                    s1ap_ENBConfigurationUpdate_protocolIEs_element,
                    &p_node,
                    &p_enb_config_update_protocol_IE);

            /* NULL Check */
            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                return_value = RRC_FAILURE;
                break;
            }

            /* Initialize the Protocol IE */
            asn1Init_s1ap_ENBConfigurationUpdate_protocolIEs_element(
                    p_enb_config_update_protocol_IE);

            p_enb_config_update_protocol_IE->id = ASN1V_s1ap_id_CSG_IdList;
            p_enb_config_update_protocol_IE->criticality = s1ap_reject;

            p_enb_config_update_protocol_IE->value.t = 
                T77__ENBConfigurationUpdateIEs_3;

            /* Initialize the CSG ID list pointer */
            rtxDListInit(&s1ENBConfigurationUpdateIEs_3);

            /* Pointer assignment to the initalized pointer type */
            p_enb_config_update_protocol_IE->value.u._ENBConfigurationUpdateIEs_3= 
                &s1ENBConfigurationUpdateIEs_3;

            /* Compose the CSG ID */
            if ( RRC_FAILURE == compose_csg_id_list(&asn1_ctx,
                        p_enb_config_update_protocol_IE->value.u._ENBConfigurationUpdateIEs_3,
                        p_csg_id_list))
            {
                RRC_S1AP_TRACE(RRC_ERROR, 
                        "compose_csg_id_list returned Failure.");
                return_value = RRC_FAILURE;
                break;
            }

            /* Append the populated Node */
            rtxDListAppendNode(&p_ENBConfigurationUpdate->protocolIEs, p_node);
        }

        if (S1AP_OAM_ENB_CONFIG_UPDATE_PAGING_DRX_PRESENT & bitmask)
        {
            /* Compose DRX */
            /* Allocate memory for the Protocol IE node type */
            rtxDListAllocNodeAndData(
                    &asn1_ctx,
                    s1ap_ENBConfigurationUpdate_protocolIEs_element,
                    &p_node,
                    &p_enb_config_update_protocol_IE);

            /* NULL Check */
            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "ASN malloc failed.");
                return_value = RRC_FAILURE;
                break;
            }

            /* Initalize the Protocol IE */
            asn1Init_s1ap_ENBConfigurationUpdate_protocolIEs_element(
                    p_enb_config_update_protocol_IE);

            p_enb_config_update_protocol_IE->id = ASN1V_s1ap_id_DefaultPagingDRX;
            p_enb_config_update_protocol_IE->criticality = s1ap_ignore;
            p_enb_config_update_protocol_IE->value.t = 
                T77__ENBConfigurationUpdateIEs_4;

            /* Assign the value */
            p_enb_config_update_protocol_IE->value.u._ENBConfigurationUpdateIEs_4=
                default_paging_drx;

            /* Append the populated Node */
            rtxDListAppendNode(&p_ENBConfigurationUpdate->protocolIEs, p_node);
        }

        /* ASN Encode message */ 
        pu_setBuffer(&asn1_ctx, asn_enc_msg_buf,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        /* NULL Check */
        if (RRC_NULL != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of ENB Config update"
                    " Failed");
            return_value = RRC_FAILURE;
            break;
        }
        else
        {
            *asn_enc_buf_len = (U16)pe_GetMsgLen(&asn1_ctx);
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                (U32)*asn_enc_buf_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)*asn_enc_buf_len,
                        LOCAL_S1_ENB_CONFIGURATION_UPDATE, MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)asn_enc_msg_buf,
                        (U32)*asn_enc_buf_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
    }while (0);

    /* Free the ASN Context */
    rtFreeContext(&asn1_ctx);
    RRC_S1AP_UT_TRACE_EXIT();
    return return_value;
}

/*****************************************************************************
 * Function Name  : compose_asn_enb_configuration_transfer 
 * Inputs         : p_enb_config_transfer - Pointer to 
 *                                            s1ap_rrm_enb_config_transfer_t
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_asn_msg - pointer to ASN encoded msg 
 *                  p_asn_msg_len - pointer to ASN encoded msg length 
 * Outputs        : p_enb_config_transfer
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes the ENB configuration Transfer
 *                  msg
 ****************************************************************************/

rrc_return_et compose_asn_enb_configuration_transfer (
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et response = RRC_FAILURE;
    OSCTXT asn1_ctx;
    S1AP_PDU s1ap_pdu;
    OSRTDListNode *p_node = PNULL;

    s1ap_ENBConfigurationTransfer *p_s1ap_eNBConfig_transfer = PNULL;
    s1ap_ENBConfigurationTransfer_protocolIEs_element *p_elem = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to initiatingMessage message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);

        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            response = RRC_FAILURE;
            break;
        }

        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = 
            ASN1V_s1ap_id_eNBConfigurationTransfer;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;

        /* SSIT Fix */
        s1ap_pdu.u.initiatingMessage->value.t = T1_eNBConfigurationTransfer; 
        p_s1ap_eNBConfig_transfer = rtxMemAllocType(&asn1_ctx,
                s1ap_ENBConfigurationTransfer);

        if (PNULL == p_s1ap_eNBConfig_transfer)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            response = RRC_FAILURE;
            break;
        }

        asn1Init_s1ap_ENBConfigurationTransfer(p_s1ap_eNBConfig_transfer);

        s1ap_pdu.u.initiatingMessage->value.u.eNBConfigurationTransfer =
            p_s1ap_eNBConfig_transfer;

        rtxDListInit(&p_s1ap_eNBConfig_transfer->protocolIEs);

        rtxDListAllocNodeAndData(&asn1_ctx,
                s1ap_ENBConfigurationTransfer_protocolIEs_element,
                &p_node,
                &p_elem);

        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            response = RRC_FAILURE;
            break;
        }

        memset_wrapper(p_elem, 
                RRC_NULL,
                sizeof(s1ap_ENBConfigurationTransfer_protocolIEs_element));

       p_elem->value.u._ENBConfigurationTransferIEs_1 = rtxMemAllocType(&asn1_ctx,
                s1ap_SONConfigurationTransfer);

        if (PNULL == p_elem->value.u._ENBConfigurationTransferIEs_1)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            response = RRC_FAILURE;
            break;
        }

        asn1Init_s1ap_SONConfigurationTransfer(p_elem->value.u._ENBConfigurationTransferIEs_1);
        p_elem->value.t = T101__ENBConfigurationTransferIEs_1;

        p_elem->id = ASN1V_s1ap_id_SONConfigurationTransferECT;

        p_elem->criticality = s1ap_ignore;

        if (RRC_FAILURE == compose_s1ap_target_enb_id(
                    &asn1_ctx,
                    p_enb_config_transfer,
                    &p_elem->value.u._ENBConfigurationTransferIEs_1->targeteNB_ID))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: Encoding failure returned by "
                    " compose_s1ap_target_enb_id", __FUNCTION__);
            response = RRC_FAILURE;
            break;
        }

        if (RRC_FAILURE == compose_s1ap_source_enb_id(
                    &asn1_ctx,
                    p_s1ap_gb_ctx,
                    p_enb_config_transfer,
                    &p_elem->value.u._ENBConfigurationTransferIEs_1->sourceeNB_ID))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: Encoding failure returned by "
                    " compose_s1ap_target_enb_id", __FUNCTION__);
            response = RRC_FAILURE;
            break;
        }

        if (RRC_FAILURE == compose_s1ap_son_information(
                    &asn1_ctx,
                    p_s1ap_gb_ctx,
                    p_enb_config_transfer,
                    &p_elem->value.u._ENBConfigurationTransferIEs_1->sONInformation))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: Encoding failure returned by "
                    " compose_s1ap_target_enb_id", __FUNCTION__);
            response = RRC_FAILURE;
            break;
        }
        
        if(p_elem->value.u._ENBConfigurationTransferIEs_1->sONInformation.t == T_s1ap_SONInformation_sONInformationRequest &&
                p_elem->value.u._ENBConfigurationTransferIEs_1->sONInformation.u.sONInformationRequest == s1ap_x2TNL_Configuration_Info)
        {
            p_elem->value.u._ENBConfigurationTransferIEs_1->m.iE_ExtensionsPresent = RRC_TRUE;
            if (RRC_FAILURE == compose_s1ap_x2tnl_configuration(
               &asn1_ctx,
               p_s1ap_gb_ctx,
               /*SPR 17777 +-*/
              &p_elem->value.u._ENBConfigurationTransferIEs_1->iE_Extensions))
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Encoding failure returned by "
                        " compose_s1ap_x2tnl_configuration", __FUNCTION__);
                response = RRC_FAILURE;
                break;
            }
        }


        rtxDListAppendNode(&p_s1ap_eNBConfig_transfer->protocolIEs,p_node);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of eNB Direct Info Transfer "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
        }

    } while(0);

    rtFreeContext(&asn1_ctx);
    return response;
}

/*****************************************************************************
 * Function Name  : compose_s1ap_target_enb_id
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_enb_config_transfer - pointer to 
 *                                             s1ap_rrm_enb_config_transfer_t 
 *                  p_eNB_ID - pointer to s1ap_TargeteNB_ID
 * Outputs        : p_eNB_ID
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes ths target eNB id msg 
 ****************************************************************************/
rrc_return_et compose_s1ap_target_enb_id(
        OSCTXT *p_asn1_ctx,
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_TargeteNB_ID *p_eNB_ID)
{
    rrc_return_et response = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_enb_config_transfer);
    RRC_ASSERT(PNULL != p_eNB_ID);

    compose_asn_plmn_identity(
            &p_eNB_ID->global_ENB_ID.pLMNidentity,
            &p_enb_config_transfer->gb_enb_id.plmn_identity);

    do 
    {
        if (p_enb_config_transfer->gb_enb_id.enb_id.presence_bitmask &
                S1AP_OAM_MACRO_ENB_ID_PRESENT)
        {
            p_eNB_ID->global_ENB_ID.eNB_ID.t = T_s1ap_ENB_ID_macroENB_ID;
            p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID = 
                rtxMemAllocType(p_asn1_ctx, 
                        ASN1BitStr32);

            if (PNULL == p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                response = RRC_FAILURE;
                break;
            }

            memset_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID, RRC_NULL, sizeof(ASN1BitStr32));

            p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID->numbits = 
                ENB_ID_NUMBITS;

            l3_memcpy_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID->data,
                    p_enb_config_transfer->gb_enb_id.enb_id.macro_enb_id.eNB_id,
                    MACRO_ENB_ID_OCTET_SIZE);
        }
        else if(p_enb_config_transfer->gb_enb_id.enb_id.presence_bitmask &
                S1AP_OAM_HOME_ENB_ID_PRESENT)
        {
            p_eNB_ID->global_ENB_ID.eNB_ID.t = T_s1ap_ENB_ID_homeENB_ID;

            p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID = 
                rtxMemAllocType(p_asn1_ctx,
                        ASN1BitStr32);

            if (PNULL == p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                response = RRC_FAILURE;
                break;
            }

            memset_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID, RRC_NULL, sizeof(ASN1BitStr32));

            p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID->numbits = 
                ENB_ID_NUMBIT;
            l3_memcpy_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID->data,
                    p_enb_config_transfer->gb_enb_id.enb_id.home_enb_id.eNB_id,
                    HOME_ENB_ID_OCTET_SIZE);
        }

        /* Fill TAI now */
        compose_asn_plmn_identity(
                &p_eNB_ID->selected_TAI.pLMNidentity,
                &p_enb_config_transfer->target_supp_ta.plmn_identity);

        p_eNB_ID->selected_TAI.tAC.numocts = TAC_OCTET_SIZE;

        l3_memcpy_wrapper(p_eNB_ID->selected_TAI.tAC.data,
                p_enb_config_transfer->target_supp_ta.tac,
                TAC_OCTET_SIZE);
    }while (0);

    return response;
}
/*****************************************************************************
 * Function Name  : compose_s1ap_source_enb_id 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_enb_config_transfer - pointer to 
 *                                            s1ap_rrm_enb_config_transfer_t
 *                  p_eNB_ID - pointer to s1ap_SourceeNB_ID
 * Outputs        : p_eNB_ID
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes s1ap source eNB id msg
 ****************************************************************************/
rrc_return_et compose_s1ap_source_enb_id(
        OSCTXT *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_SourceeNB_ID *p_eNB_ID)
{
    rrc_return_et response = RRC_SUCCESS;

    gb_enb_id_t *p_gb_enb = PNULL;

    RRC_ASSERT(PNULL != p_enb_config_transfer);
    RRC_ASSERT(PNULL != p_eNB_ID);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_asn1_ctx);

    p_gb_enb = &p_s1ap_gb_ctx->s1_setup_info.gb_enb_id;

    if (PNULL == p_gb_enb)
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: Source ENB ID Missing",__FUNCTION__);

        return RRC_FAILURE;
    }

    l3_memcpy_wrapper(&p_eNB_ID->global_ENB_ID.pLMNidentity.data, 
            &p_gb_enb->plmn_identity, MAX_PLMN_ID_BYTES);

    p_eNB_ID->global_ENB_ID.pLMNidentity.numocts = MAX_PLMN_ID_BYTES;


    do 
    {
        if (p_gb_enb->enb_id.presence_bitmask &
                S1AP_OAM_MACRO_ENB_ID_PRESENT)
        {
            p_eNB_ID->global_ENB_ID.eNB_ID.t = T_s1ap_ENB_ID_macroENB_ID;
            p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID = 
                rtxMemAllocType(p_asn1_ctx, 
                        ASN1BitStr32);

            if (PNULL == p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                response = RRC_FAILURE;
                break;
            }

            memset_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID, RRC_NULL, sizeof(ASN1BitStr32));

            p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID->numbits = 
                ENB_ID_NUMBITS;

            l3_memcpy_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID->data,
                    p_gb_enb->enb_id.macro_enb_id.eNB_id,
                    MACRO_ENB_ID_OCTET_SIZE);
        }
        else if(p_gb_enb->enb_id.presence_bitmask &
                S1AP_OAM_HOME_ENB_ID_PRESENT)
        {
            p_eNB_ID->global_ENB_ID.eNB_ID.t = T_s1ap_ENB_ID_homeENB_ID;

            p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID = 
                rtxMemAllocType(p_asn1_ctx,
                        ASN1BitStr32);

            if (PNULL == p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                response = RRC_FAILURE;
                break;
            }

            memset_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID, RRC_NULL, sizeof(ASN1BitStr32));

            p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID->numbits = 
                ENB_ID_NUMBIT;
            l3_memcpy_wrapper(p_eNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID->data,
                    p_gb_enb->enb_id.home_enb_id.eNB_id,
                    HOME_ENB_ID_OCTET_SIZE);
        }

        /* Fill TAI now */
        compose_asn_plmn_identity(
                &p_eNB_ID->selected_TAI.pLMNidentity,
                &p_enb_config_transfer->source_supp_ta.plmn_identity);

        p_eNB_ID->selected_TAI.tAC.numocts = TAC_OCTET_SIZE;

        l3_memcpy_wrapper(p_eNB_ID->selected_TAI.tAC.data,
                p_enb_config_transfer->source_supp_ta.tac,
                TAC_OCTET_SIZE);
    }while (0);

    return response;
}

/*****************************************************************************
 * Function Name  : compose_s1ap_son_information 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_enb_config_transfer - pointer to 
 *                                          s1ap_rrm_enb_config_transfer_t
 *                  p_son_info - pointer to s1ap_SONInformation
 * Outputs        : p_son_info
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes s1ap son info msg.
 ****************************************************************************/
rrc_return_et compose_s1ap_son_information(
        OSCTXT *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_SONInformation *p_son_info)
{
    s1ap_SONInformationReply *p_son_reply = PNULL;
    rrc_return_et response = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    RRC_ASSERT(PNULL != p_enb_config_transfer);
    RRC_ASSERT(PNULL != p_son_info);

    do
    {
        if (ENB_CONFIGURATION_TRANSFER_REQ & p_enb_config_transfer->bitmask)
        {
            p_son_info->t = T_s1ap_SONInformation_sONInformationRequest;
            p_son_info->u.sONInformationRequest = s1ap_x2TNL_Configuration_Info;
        }
        else if (ENB_CONFIGURATION_TRANSFER_RESP & p_enb_config_transfer->bitmask)
        {
            p_son_info->t = T_s1ap_SONInformation_sONInformationReply;

            p_son_info->u.sONInformationReply = rtxMemAllocTypeZ(p_asn1_ctx,
                    s1ap_SONInformationReply);

            p_son_reply = p_son_info->u.sONInformationReply;

            if (PNULL == p_son_reply)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                response = RRC_FAILURE;
                break;
            }

            p_son_reply->m.x2TNLConfigurationInfoPresent = 1;
            if(!(p_s1ap_gb_ctx->x2_enb_comm_info.bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT))
            {
                if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ip_addr == 1 )
                {
                    p_son_reply->
                        x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n = 1;
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN*8;

                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data = 
                        rtxMemAllocZ(p_asn1_ctx,MAX_IP_ADDRESS_LENGTH);

                    if (PNULL == p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                        response = RRC_FAILURE;
                        break;
                    }

                    if (inet_pton_wrapper(AF_INET, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ip_addr[0].ip_addr, 
                                (void*)p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data) != 1)
                    {
                        RRC_TRACE(RRC_WARNING,"Couldn't convert ipv4 address");

                    }
                }
                else if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ip_addr <= MAX_NUM_IP_ADDR)
                {
                    p_son_reply->
                        x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n =
                        MAX_NUM_IP_SUPPORTED;
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN*8;
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].numbits = RRC_MAX_IP_ADDR_LEN*8;

                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data = 
                        rtxMemAllocZ(p_asn1_ctx,MAX_IP_ADDRESS_LENGTH);

                    if (PNULL == p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                        response = RRC_FAILURE;
                        break;
                    }
                    if (inet_pton_wrapper(AF_INET, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ip_addr[0].ip_addr, 
                                (void*)p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data) != 1)
                    {
                        RRC_TRACE(RRC_WARNING,"Couldn't convert ipv4 address");

                    }
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data = 
                        rtxMemAllocZ(p_asn1_ctx,MAX_IP_ADDRESS_LENGTH);

                    if (PNULL == p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                        response = RRC_FAILURE;
                        break;
                    }

                    if (inet_pton_wrapper(AF_INET, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ip_addr[1].ip_addr, 
                                (void*)p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data) != 1)
                    {
                        RRC_TRACE(RRC_WARNING,"Couldn't convert ipv4 address");

                    }
                }
                else
                {
                    /* NO TNL INFO PRESENT */
                    RRC_S1AP_TRACE(RRC_ERROR, "%s: ENB TNL INFO Missing",
                            __FUNCTION__);
                    response = RRC_FAILURE;
                    break;
                }
            }
            else
            {
                if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ipv6_addr == 1 )
                {
                    p_son_reply->
                        x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n = 1;
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN_IPV6*8;

                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data = 
                        rtxMemAllocZ(p_asn1_ctx,MAX_IPV6_ADDRESS_LENGTH);

                    if (PNULL == p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                        response = RRC_FAILURE;
                        break;
                    }
                    if (inet_pton_wrapper(AF_INET6, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ipv6_addr[0].ipv6_addr, 
                                (void*)p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data) != 1)
                    {
                        RRC_TRACE(RRC_WARNING,"Couldn't convert ipv6 address");

                    }
                }
                else if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ipv6_addr <= MAX_NUM_IP_ADDR)
                {
                    p_son_reply->
                        x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n =
                        MAX_NUM_IP_SUPPORTED;
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN_IPV6*8;
                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].numbits = RRC_MAX_IP_ADDR_LEN_IPV6*8;

                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data = 
                        rtxMemAllocZ(p_asn1_ctx,MAX_IPV6_ADDRESS_LENGTH);

                    if (PNULL == p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                        response = RRC_FAILURE;
                        break;
                    }
                    if (inet_pton_wrapper(AF_INET6, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ipv6_addr[0].ipv6_addr, 
                                (void*)p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data) != 1)
                    {
                        RRC_TRACE(RRC_WARNING,"Couldn't convert ipv6 address");

                    }

                    p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data = 
                        rtxMemAllocZ(p_asn1_ctx,MAX_IPV6_ADDRESS_LENGTH);

                    if (PNULL == p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data)
                    {
                        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                        response = RRC_FAILURE;
                        break;
                    }
                    if (inet_pton_wrapper(AF_INET6, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ipv6_addr[1].ipv6_addr, 
                                (void*)p_son_reply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data) != 1)
                    {
                        RRC_TRACE(RRC_WARNING,"Couldn't convert ipv6 address");

                    }
                }
                else
                {
                    /* NO TNL INFO PRESENT */
                    RRC_S1AP_TRACE(RRC_ERROR, "%s: ENB TNL INFO Missing",
                            __FUNCTION__);
                    response = RRC_FAILURE;
                    break;
                }

            }
            if(p_s1ap_gb_ctx->x2_enb_comm_info.bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
            {
                p_son_reply->x2TNLConfigurationInfo.m.iE_ExtensionsPresent = RRC_TRUE;
                if(RRC_NULL == compose_x2tnl_config_info_ie_extensions(p_asn1_ctx,&p_son_reply->x2TNLConfigurationInfo,p_s1ap_gb_ctx))
                 {
                    RRC_S1AP_TRACE(RRC_ERROR, "%s: Composing failed for extended tnl info",
                            __FUNCTION__);
                    response = RRC_FAILURE;
                 }
            }
        }
        else
        {
            /* NO TNL INFO PRESENT */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ENB TNL INFO Missing",
                    __FUNCTION__);
            response = RRC_FAILURE;
            break;
        }
    }while (0);

    return response;
}

/*****************************************************************************
 * Function Name  : process_s1ap_rrm_enb_mme_direct_info_transfer_msg
 * Inputs         : p_enb_info_msg - pointer to 
 *                                       s1ap_rrm_enb_direct_info_transfer_t
 *                  p_asn_msg - pointer to ASN encoded msg 
 *                  p_asn_msg_len -  pointer to ASN encoded msg length
 * Outputs        : None
 * Returns        : RRC_SUCESS/RRC_FAILURE
 * Description    : This function composes  the MME ENB Direct Info Transfer
 *                  msg
 ****************************************************************************/
rrc_return_et compose_asn_enb_direct_info_transfer (
        s1ap_rrm_enb_direct_info_transfer_t *p_enb_info_msg,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et response = RRC_FAILURE;
    OSCTXT asn1_ctx;
    S1AP_PDU s1ap_pdu;
    OSRTDListNode *p_node = PNULL;

    s1ap_ENBDirectInformationTransfer_protocolIEs_element *p_protocolIE_elem = PNULL;
    s1ap_ENBDirectInformationTransfer    *p_eNBDirectInfo = PNULL;
    s1ap_RIMTransfer    *p_rIMTransfer   = PNULL;
    s1ap_GERAN_Cell_ID  *p_gERAN_Cell_ID = PNULL;
    s1ap_TargetRNC_ID  *p_targetRNC_ID = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        asn1Init_S1AP_PDU(&s1ap_pdu);

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to initiatingMessage message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);

        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_eNBDirectInformationTransfer;
        /*SPR_18844_START*/
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_ignore;
        /*SPR_18844_END*/

        /* SSIT fix */
        s1ap_pdu.u.initiatingMessage->value.t =  T1_eNBDirectInformationTransfer;

        p_eNBDirectInfo = rtxMemAllocType(&asn1_ctx, s1ap_ENBDirectInformationTransfer);

        if (PNULL == p_eNBDirectInfo)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_s1ap_ENBDirectInformationTransfer(p_eNBDirectInfo);

        s1ap_pdu.u.initiatingMessage->value.u.eNBDirectInformationTransfer =
            p_eNBDirectInfo;

        rtxDListInit(&p_eNBDirectInfo->protocolIEs);

        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_ENBDirectInformationTransfer_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);

        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        if (PNULL == p_protocolIE_elem)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_ENBDirectInformationTransfer_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_Inter_SystemInformationTransferTypeEDT;
        p_protocolIE_elem->criticality = s1ap_reject;


        p_protocolIE_elem->value.u._ENBDirectInformationTransferIEs_1 =
        rtxMemAllocType(&asn1_ctx, s1ap_Inter_SystemInformationTransferType);

        if (PNULL == p_protocolIE_elem->value.u._ENBDirectInformationTransferIEs_1)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_s1ap_Inter_SystemInformationTransferType(p_protocolIE_elem->
                value.u._ENBDirectInformationTransferIEs_1);
        p_protocolIE_elem->value.t  = T99__ENBDirectInformationTransferIEs_1;

        p_rIMTransfer = rtxMemAllocType(&asn1_ctx, s1ap_RIMTransfer);

        if (PNULL == p_rIMTransfer)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        /* SSIT Fix */
        asn1Init_s1ap_RIMTransfer(p_rIMTransfer);

        p_protocolIE_elem->value.u._ENBDirectInformationTransferIEs_1->t=1;
        p_protocolIE_elem->value.u._ENBDirectInformationTransferIEs_1->u.rIMTransfer = p_rIMTransfer;

        /* Set the RIM Information */ 
        p_rIMTransfer->rIMInformation.numocts = p_enb_info_msg->len_rim_info;

        p_rIMTransfer->rIMInformation.data = p_enb_info_msg->rim_info;

		/*bugid 116872 begin*/
		if(p_enb_info_msg->bitmask)
		{
			/* GERAN CELL ID/TARGET RNC ID presence bitmask is set */
			RRC_S1AP_TRACE(RRC_ERROR, 
					"%s: Don't support GERAN cell ID /Target RNC ID ",__FUNCTION__);
			/*SPR_18125_START*/
			rtFreeContext(&asn1_ctx);
			/*SPR_18125_END*/
			RRC_S1AP_UT_TRACE_EXIT();
			return RRC_FAILURE;
		
		}

		/*bugid 116872 end*/

#if 0 //for bugid 116872 . Asn1 does not support.
        if((p_enb_info_msg->bitmask & GERAN_CELL_ID_PRESENT) && \
                (p_enb_info_msg->bitmask & TARGET_RNC_ID_PRESENT ))
        {
            /* Bit set for both GERAN CELL ID/TARGET RNC ID in presence bitmask */
            RRC_S1AP_TRACE(RRC_ERROR, 
                    "%s:Bit set for both GERAN cell ID /Target RNC ID "
                    "in Presence bitmask",__FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_FAILURE;

        }
        /* Fill GERAN ID if bit set in bitmask */
        else if( p_enb_info_msg->bitmask & GERAN_CELL_ID_PRESENT )
        {
            /* Set the Routing Address i.e. GERAN CELL ID */
            p_gERAN_Cell_ID = rtxMemAllocType(&asn1_ctx, s1ap_GERAN_Cell_ID);
            /* SPR 12964 Fix Start */
            if (PNULL == p_gERAN_Cell_ID)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            /* SPR 12964 Fix Stop */

            /* SSIT Fix */
            asn1Init_s1ap_GERAN_Cell_ID(p_gERAN_Cell_ID);

            p_rIMTransfer->m.rIMRoutingAddressPresent = 1;

            p_rIMTransfer->rIMRoutingAddress.t = T_s1ap_RIMRoutingAddress_gERAN_Cell_ID;
            p_rIMTransfer->rIMRoutingAddress.u.gERAN_Cell_ID = p_gERAN_Cell_ID;


            /* Set Routing Area Code */
            p_gERAN_Cell_ID->rAC.numocts = 1;
            p_gERAN_Cell_ID->rAC.data[0] = p_enb_info_msg->geran_cell_id.routing_area_code;


            /* Set CI */ 
            p_gERAN_Cell_ID->cI.numocts = 2;
            l3_memcpy_wrapper(p_gERAN_Cell_ID->cI.data,
                    p_enb_info_msg->geran_cell_id.ci,
                    p_gERAN_Cell_ID->cI.numocts);


            /* Set LAI - LAC */ 
            p_gERAN_Cell_ID->lAI.lAC.numocts = sizeof(p_enb_info_msg->
                    geran_cell_id.lai.location_area_code);
            l3_memcpy_wrapper(p_gERAN_Cell_ID->lAI.lAC.data,
                    p_enb_info_msg->geran_cell_id.lai.location_area_code,
                    p_gERAN_Cell_ID->lAI.lAC.numocts);

            /* Set LAI - PLMN Identity */ 
            compose_asn_plmn_identity(
                    &p_gERAN_Cell_ID->lAI.pLMNidentity,
                    &p_enb_info_msg->geran_cell_id.lai.plmn_identity);
        }
        /* Fill Target RNC ID if bit set in bitmask */
        else if( p_enb_info_msg->bitmask & TARGET_RNC_ID_PRESENT )
        {
            /* Set the Routing Address i.e. GERAN CELL ID */
            p_targetRNC_ID = rtxMemAllocType(&asn1_ctx, s1ap_TargetRNC_ID);
            /* SPR 12964 Fix Start */
            if (PNULL == p_targetRNC_ID)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            /* SPR 12964 Fix Stop */

            asn1Init_s1ap_TargetRNC_ID(p_targetRNC_ID);

            p_rIMTransfer->m.rIMRoutingAddressPresent = 1;

            p_rIMTransfer->rIMRoutingAddress.t = T_s1ap_RIMRoutingAddress_targetRNC_ID;
            p_rIMTransfer->rIMRoutingAddress.u.targetRNC_ID = p_targetRNC_ID;

            /* Set LAI - LAC */ 
            p_targetRNC_ID->lAI.lAC.numocts = sizeof(p_enb_info_msg->
                    target_rnc_id.lai.location_area_code);
            l3_memcpy_wrapper(p_targetRNC_ID->lAI.lAC.data,
                    p_enb_info_msg->target_rnc_id.lai.location_area_code,
                    p_targetRNC_ID->lAI.lAC.numocts);

            /* Set LAI - PLMN Identity */ 
            compose_asn_plmn_identity(
                    &p_targetRNC_ID->lAI.pLMNidentity,
                    &p_enb_info_msg->target_rnc_id.lai.plmn_identity);

            p_targetRNC_ID->m.iE_ExtensionsPresent = 0;

            /* Set RAC if present */
            if(p_enb_info_msg->target_rnc_id.bitmask & RRM_RNC_ROUTING_AREA_CODE_PRESENT)
            {
                /* Set presence bit */
                p_targetRNC_ID->m.rACPresent = 1;

                /* Set Routing Area Code */
                p_targetRNC_ID->rAC.numocts = 1;
                p_targetRNC_ID->rAC.data[0] = p_enb_info_msg->target_rnc_id.routing_area_code;
            }
            /*  CR0885 start */ 
            else
            {
                RRC_S1AP_TRACE(RRC_ERROR, 
                    "%s: In case of UTRAN, RAC IE "
                    "should be present, send failure",__FUNCTION__);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                RRC_S1AP_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            /*  CR0885 end */ 

            /* Set RNC Identity */ 
            p_targetRNC_ID->rNC_ID = p_enb_info_msg->target_rnc_id.rnc_id;

            /* Set Extended RNC ID if present */
            if(p_enb_info_msg->target_rnc_id.bitmask & RRM_RNC_EXTENDED_RNC_ID_PRESENT)
            {
                /* Set presence bit */
                p_targetRNC_ID->m.extendedRNC_IDPresent= 1;

                p_targetRNC_ID->extendedRNC_ID = p_enb_info_msg->target_rnc_id.extended_rnc_id;
            }
        }
        else
        {
            /* GERAN CELL ID/TARGET RNC ID presence bitmask is not set */
            RRC_S1AP_TRACE(RRC_ERROR, 
                    "%s: For both GERAN cell ID /Target RNC ID "
                    "Presence bitmask not set",__FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            RRC_S1AP_UT_TRACE_EXIT();
            return RRC_FAILURE;

        }
#endif

        rtxDListAppendNode(&p_eNBDirectInfo->protocolIEs, p_node);

        rtxDListInit(&p_eNBDirectInfo->extElem1);


        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of eNB Direct Info Transfer "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
            /*SPR 19183 START*/
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);
            /*SPR 19183 END*/
        }
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}

/*****************************************************************************
 * Function Name  : compose_asn_plmn_identity 
 * Inputs         : pLMNidentity - pointer to s1ap_PLMNidentity
 *                  plmn_id - pointer to plmn_identity_t 
 * Outputs        : pLMNidentity
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes the asn encoded plmn identity msg
 ****************************************************************************/
rrc_return_t compose_asn_plmn_identity(
        s1ap_PLMNidentity *pLMNidentity,
        plmn_identity_t   *plmn_id
        )
{
    RRC_S1AP_UT_TRACE_ENTER();

    pLMNidentity->numocts = 3;


    if (plmn_id->presence_bitmask &
            PLMN_IDENTITY_MCC_PRESENCE_FLAG)
    {
        pLMNidentity->data[0] = 
            (OSOCTET)(((0x0F & plmn_id->mcc[1])<<4) | plmn_id->mcc[0]) ;
        pLMNidentity->data[1] = plmn_id->mcc[2];
    }
    else
    {
        pLMNidentity->data[0] = 0xFF ;
        pLMNidentity->data[1] = 0x0F;
    }


    if (plmn_id->mnc.count == MAX_MNC_OCTET_SIZE)
    {
        pLMNidentity->data[1] |= (OSOCTET)((0x0F & plmn_id->mnc.mnc[0])<<4);
        pLMNidentity->data[2] =  
            (OSOCTET)(((0x0F & plmn_id->mnc.mnc[2])<<4) | plmn_id->mnc.mnc[1]);        
    }
    else
    {
        pLMNidentity->data[1] |= ((U8)0x0F)<<4;
        pLMNidentity->data[2] =  
            (OSOCTET)(((0x0F & plmn_id->mnc.mnc[1])<<4) | plmn_id->mnc.mnc[0]);        
    }


    RRC_S1AP_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : parse_s1ap_mme_direct_info_transfer 
 * Inputs         : p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_mme_ctx - pointer to MME context 
 *                  p_mme_direct_info - pointer to 
 *                                       s1ap_rrm_mme_direct_info_transfer_t
 *                  p_asn_msg - pointer to ASN encoded message
 *                  asn_msg_len - asn encoded msg length 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function parses  the MME ENB Direct Info Transfer
 *                  msg.
 ****************************************************************************/
rrc_return_et parse_s1ap_mme_direct_info_transfer(
        mme_context_t        * p_mme_ctx,
        s1ap_rrm_mme_direct_info_transfer_t *p_mme_direct_info,
        void *p_asn_msg,
        U16 asn_msg_len
        )
{
    S1AP_PDU s1ap_pdu;
    OSCTXT                          asn1_ctx;
    rrc_return_et result = RRC_SUCCESS;
    mme_direct_info_t parsed_mme_direct_info;
    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE; 
    }

    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do 
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);
            result = RRC_FAILURE;
            break;
        }
        /*SPR 19183 START*/
        rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                &s1ap_pdu);
        /*SPR 19183 END*/
        memset_wrapper(&parsed_mme_direct_info,0,sizeof(parsed_mme_direct_info));
        /* Decode message */
        if(RRC_FAILURE == s1ap_mme_direct_information_transfer_intrl_dec(
                    p_mme_ctx,
                    s1ap_pdu.u.initiatingMessage->
                    value.u.mMEDirectInformationTransfer,
                    &parsed_mme_direct_info,
                    p_mme_direct_info
                    ))
        {
            result = RRC_FAILURE;
            break;
        }

    } while (0);



    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);



    return result;
}
/****************************************************************************
 * Function Name  : s1ap_mme_direct_information_transfer_intrl_dec
 * Inputs         : p_mme_context - pointer to MME context
 *                  p_3gpp - Decoded s1ap_S1SetupResponse
 *                  p_intrl - pointer to mme_direct_info_t
 *                  p_s1ap_rrm_mme_direct_info_transfer - pointer to 
 *                                   s1ap_rrm_mme_direct_info_transfer_t
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : Gets the Decoded MME Configuration Update message
 ****************************************************************************/
rrc_return_et s1ap_mme_direct_information_transfer_intrl_dec
(
 mme_context_t                           *p_mme_context,
 s1ap_MMEDirectInformationTransfer        *p_3gpp,    /* unpacked src */
 mme_direct_info_t                        *p_intrl ,  
 s1ap_rrm_mme_direct_info_transfer_t    *p_s1ap_rrm_mme_direct_info_transfer 
 )
{
    U32                             index = 0;
    OSRTDListNode                   *p_node = RRC_NULL;
    s1ap_MMEDirectInformationTransfer_protocolIEs_element
        *p_protocol_IE = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    s1ap_error_ind_bool_t send_err_ind ;
    s1ap_RIMTransfer    *p_rIMTransfer = PNULL;
    OSCTXT asn1_ctx;

    s1ap_message_data_t message_map =
    {1, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_Inter_SystemInformationTransferTypeMDT,
s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, RRC_FALSE, sizeof(s1ap_error_ind_bool_t));


    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

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
                (s1ap_MMEDirectInformationTransfer_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_Inter_SystemInformationTransferTypeMDT:
                    {        
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._MMEDirectInformationTransferIEs_1))
                        {

                            l3_memcpy_wrapper(&p_intrl->si_transfer_type,
                                    p_protocol_IE->value.u._MMEDirectInformationTransferIEs_1,
                                    sizeof(s1ap_Inter_SystemInformationTransferType));

                            /* Check if the RIM Transfer is present */
                            if((T_s1ap_Inter_SystemInformationTransferType_rIMTransfer == 
                                        p_intrl->si_transfer_type.t) && 
                                    (PNULL != p_intrl->si_transfer_type.u.rIMTransfer))
                            {
                                /* Pointer Assignment */
                                p_rIMTransfer = p_intrl->si_transfer_type.u.rIMTransfer;

                                /* Check if the data pointer is NULL */
                                if (PNULL == p_rIMTransfer->rIMInformation.data)
                                {
                                    RRC_S1AP_TRACE(RRC_INFO,"%s: "
                                            "RIM Information No present", 
                                            __FUNCTION__);
                                    result = RRC_FAILURE;
                                    break;
                                }
                                /* Get the length of RIM Information */
                                p_s1ap_rrm_mme_direct_info_transfer->len_rim_info = 
                                    p_rIMTransfer->rIMInformation.numocts;

                                /* Copy the RIM Information */
                                l3_memcpy_wrapper(p_s1ap_rrm_mme_direct_info_transfer->rim_info,
                                        p_rIMTransfer->rIMInformation.data,
                                        p_rIMTransfer->rIMInformation.numocts);

                            }

                        }
                    }
                    break;
                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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

            p_node= p_node->next;            

        }
    } while (0);
    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_MMEDirectInformationTransfer,
                    T_S1AP_PDU_initiatingMessage,
                    s1ap_ignore,
                    (rrc_s1ap_error_indication_t *)PNULL)) ) 
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                " MME Direct Info Transfer- Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : s1ap_generate_plmn_identity_to_int 
 * Inputs         : p_plmn_identity - pointer to plmn_identity_t
 *                  p_s1ap_plmn_identity - pointer to OSOCTET 
 * Outputs        : p_plmn_identity
 * Returns        : None
 * Description    : This function generates plmn identity
 ******************************************************************************/
void s1ap_generate_plmn_identity_to_int(
        plmn_identity_t    *p_plmn_identity,
        OSOCTET                 *p_s1ap_plmn_identity)
{
    /* TS 3GPP TS 36.413 V8.5.1 - 9.2.3.8  PLMN Identity
     * - digits 0 to 9, encoded 0000 to 1001,
     * - 1111 used as filler digit,
     * two digits per octet,
     * - bits 4 to 1 of octet n encoding digit 2n-1
     * - bits 8 to 5 of octet n encoding digit 2n
     *
     * -The Selected PLMN identity consists of 3 digits from MCC
     * followed by either
     * -a filler digit plus 2 digits from MNC (in case of 2 digit MNC) or
     * -3 digits from MNC (in case of a 3 digit MNC).
     * */

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
            (p_s1ap_plmn_identity[1] & 0xF0) >> 4;

        /* PLMN Identity 3rd octet : 2nd and 3rd digit of MNC */
        p_plmn_identity->mnc.mnc[1] = 
            (p_s1ap_plmn_identity[2] & 0x0F);
    /*SPR 5517 Fix Stop */
        p_plmn_identity->mnc.mnc[2] = 
            (p_s1ap_plmn_identity[2] & 0xF0) >> 4;
    }
    /*SPR 5517 Fix Stop */
}

/*****************************************************************************
 * Function Name  : parse_s1ap_mme_configuration_transfer
 * Inputs         : p_mme_ctx - pointer to MME context 
 *                  p_mme_config_transfer - pointer to 
 *                                           s1ap_rrm_mme_config_transfer_t
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_asn_msg - pointer to ASN encoded msg 
 *                  asn_msg_len - asn encoded msg length 
 * Outputs        : None
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : This function parses the MME ENB Info Transfer msg
 ****************************************************************************/
rrc_return_et parse_s1ap_mme_configuration_transfer(
        mme_context_t        *p_mme_ctx,
        s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,
        s1ap_gb_context_t *p_s1ap_gb_ctxt,
        void *p_asn_msg,
        U16 asn_msg_len)
{
    S1AP_PDU s1ap_pdu;
    OSCTXT                          asn1_ctx;
    rrc_return_et result = RRC_SUCCESS;
    rrm_gb_enb_id_t target_gb_enb_id;
    plmn_identity_t plmn_id;
    rrm_gb_enb_id_t converted_s1ap_gb_enb_id;
    s1ap_MMEConfigurationTransfer_protocolIEs_element p_elem;

    RRC_ASSERT(PNULL != p_mme_config_transfer);
    RRC_ASSERT(PNULL != p_asn_msg);

    RRC_S1AP_UT_TRACE_ENTER();

    memset_wrapper(&p_elem,RRC_NULL,sizeof(s1ap_MMEConfigurationTransfer_protocolIEs_element));
    memset_wrapper(&converted_s1ap_gb_enb_id, RRC_NULL, sizeof(rrm_gb_enb_id_t));
    memset_wrapper(&plmn_id, RRC_NULL, sizeof(plmn_identity_t));
    memset_wrapper(&target_gb_enb_id, RRC_NULL, sizeof(rrm_gb_enb_id_t));

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE; 
    }

    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do 
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);
            result = RRC_FAILURE;
            break;
        }

        /* SPR 19927 Fix Start */
        rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);
        /* SPR 19927 Fix Stop */

        if(RRC_FAILURE == s1ap_mme_configuration_transfer_intrl_dec(
                    p_mme_ctx,
                    s1ap_pdu.u.initiatingMessage->value.u.mMEConfigurationTransfer,
                    &p_elem))

        {
            result = RRC_FAILURE;
            break;
        }
        if ( p_elem.value.u._MMEConfigurationTransferIEs_1 == PNULL)
        {
            result = RRC_FAILURE;
            break;
        }

        RRC_S1AP_TRACE(RRC_INFO,"%s: "
                "ASN1V_s1ap_id_SONConfigurationTransferMCT",
                __FUNCTION__);

        if (p_elem.value.u._MMEConfigurationTransferIEs_1->m.iE_ExtensionsPresent)
        {
            /* IE Extension not supported */
            RRC_S1AP_TRACE(RRC_INFO, "%s: "
                    "IE Extension  supported",
                    __FUNCTION__);

            //result = RRC_FAILURE;
        }

        /* Since we are the target eNB, 
         * We'll validate if we are really the
         * target. MME might send the message having
         * different target eNB ID */
        if (RRC_FAILURE == 
                parse_s1ap_TargeteNB_ID(
                    &target_gb_enb_id,
                    &p_mme_config_transfer->target_supp_ta,
                    &p_elem.value.u._MMEConfigurationTransferIEs_1->targeteNB_ID))
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "Target eNB ID parse failed Abort"
                    " MME Configuration Transfer",
                    __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        s1ap_generate_plmn_identity_to_int(&plmn_id,
                (OSOCTET*)&p_s1ap_gb_ctxt->s1_setup_info.gb_enb_id.plmn_identity.plmn_id);

        l3_memcpy_wrapper(&converted_s1ap_gb_enb_id.plmn_identity,
                &plmn_id, sizeof(plmn_identity_t));

        l3_memcpy_wrapper(&converted_s1ap_gb_enb_id.enb_id,
                &p_s1ap_gb_ctxt->s1_setup_info.gb_enb_id.enb_id,
                sizeof(rrm_enb_id_info_t));

            /* SPR 19927 8Jun Fix Start */
            converted_s1ap_gb_enb_id.enb_id.macro_enb_id.eNB_id[MACRO_ENB_ID_OCTET_SIZE - 1] =
                         (converted_s1ap_gb_enb_id.enb_id.macro_enb_id.eNB_id[MACRO_ENB_ID_OCTET_SIZE - 1] & 0xF0);
            converted_s1ap_gb_enb_id.enb_id.home_enb_id.eNB_id[HOME_ENB_ID_OCTET_SIZE - 1] =
                         (converted_s1ap_gb_enb_id.enb_id.home_enb_id.eNB_id[HOME_ENB_ID_OCTET_SIZE - 1] & 0xF0);
            /* SPR 19927 8Jun Fix Stop */
        /* now compare the gb_enb_id with the sent 
         * in the mme configuration transfer message */
        if (RRC_NULL != memcmp_wrapper(&target_gb_enb_id.enb_id,
                    &converted_s1ap_gb_enb_id.enb_id,
                    sizeof(rrm_enb_id_info_t)))
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "Target eNB ID in MME Configuration"
                    " Transfer didn't match. The message"
                    " is may be other eNB", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        if (RRC_FAILURE == parse_s1ap_SourceeNB_ID(
                    &p_mme_config_transfer->gb_enb_id,
                    &p_mme_config_transfer->source_supp_ta,
                    &p_elem.value.u._MMEConfigurationTransferIEs_1->sourceeNB_ID))
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    " Error while parsing "
                    "Source eNB ID", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        if (RRC_FAILURE == parse_s1ap_SONInformation(
                    p_mme_config_transfer,
                    &p_elem.value.u._MMEConfigurationTransferIEs_1->sONInformation))
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    " Error while parsing "
                    "SON Information", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
       
        if(p_elem.value.u._MMEConfigurationTransferIEs_1->m.iE_ExtensionsPresent)
        {
            if (RRC_FAILURE == parse_s1ap_x2tnl_configuration(
                        p_mme_config_transfer,
                        &p_elem.value.u._MMEConfigurationTransferIEs_1->iE_Extensions))
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        " Error while parsing "
                        "X2 TNL Configuration", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
    }while(0);


    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_mme_configuration_transfer_intrl_dec
 * Inputs         : p_mme_ctx - pointer to MME context
 *                  p_3gpp - pointer to s1ap_MMEConfigurationTransfer
 *                  p_elem - pointer to 
 *                           s1ap_MMEConfigurationTransfer_protocolIEs_element
 * Outputs        : p_intrl (internal parsed_s1_setup_rsp_t)
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : Gets the Decoded MME Configuration Update message
 ****************************************************************************/
rrc_return_et s1ap_mme_configuration_transfer_intrl_dec(
        mme_context_t *p_mme_ctx,
        s1ap_MMEConfigurationTransfer *p_3gpp,
        s1ap_MMEConfigurationTransfer_protocolIEs_element *p_elem
        )
{

    rrc_return_et result = RRC_SUCCESS;
    U32                             index = 0;
    U16 order_index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    s1ap_error_ind_bool_t send_err_ind ;
    OSCTXT asn1_ctx;

    OSRTDListNode                   *p_node = RRC_NULL;
    s1ap_MMEConfigurationTransfer_protocolIEs_element
        *p_protocol_IE = RRC_NULL;

    memset_wrapper(&send_err_ind, RRC_NULL, sizeof(send_err_ind));

    s1ap_message_data_t message_map =
    {1, RRC_FALSE, RRC_FALSE,0, 0, 0,
        {{0, ASN1V_s1ap_id_SONConfigurationTransferMCT, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(s1ap_error_ind_bool_t));

    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

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
                (s1ap_MMEConfigurationTransfer_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_SONConfigurationTransferMCT:
                    {
                        if (RRC_SUCCESS ==
                                validate_ie_value(
                                    &message_map,
                                    order_index,
                                    p_protocol_IE->id,
                                    p_protocol_IE->value.u._MMEConfigurationTransferIEs_1))
                        {
                            /* Assigne the value */
                            p_elem->value.u._MMEConfigurationTransferIEs_1 =  p_protocol_IE->value.u._MMEConfigurationTransferIEs_1;
                        }
                        break;
                    }

                default:
                    {
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

            }

            p_node= p_node->next;            
            order_index++;
        }            


    } while(0);
    /* Parse the map for Error Indication */
    if (RRC_SUCCESS == parse_message_map(
                &asn1_ctx,
                p_mme_ctx,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_MMEConfigurationTransfer,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_TRACE(RRC_INFO, "Error Indication Detected");
        result = RRC_FAILURE;

    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : parse_s1ap_SourceeNB_ID 
 * Inputs         : p_gb_enb_id - pointer to rrm_gb_enb_id_t
 *                  p_rrm_supp_ta - Pointer to rrm_supp_ta_t 
 *                  p_s1ap_SourceeNB_ID- Pointer to s1ap_SourceeNB_ID
 * Outputs        : p_gb_enb_id 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function parses the source eNB id over s1 interface 
 ****************************************************************************/
rrc_return_et parse_s1ap_SourceeNB_ID(rrm_gb_enb_id_t *p_gb_enb_id,
        rrm_supp_ta_t *p_rrm_supp_ta,
        s1ap_SourceeNB_ID *p_s1ap_SourceeNB_ID)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_gb_enb_id);
    RRC_ASSERT(PNULL != p_s1ap_SourceeNB_ID);

    do
    {
        /** PLMN ID **/
        /* Convert from BCD to Int */
        s1ap_generate_plmn_identity_to_int(&p_gb_enb_id->plmn_identity,
                p_s1ap_SourceeNB_ID->global_ENB_ID.pLMNidentity.data);

        /** ENB ID **/
        if (T_s1ap_ENB_ID_macroENB_ID == 
                p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.t)
        {
            if (PNULL != p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID)
            {
                p_gb_enb_id->enb_id.presence_bitmask |= 
                    S1AP_OAM_MACRO_ENB_ID_PRESENT;

                l3_memcpy_wrapper(p_gb_enb_id->enb_id.macro_enb_id.eNB_id, 
                        p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID->data,
                        MACRO_ENB_ID_OCTET_SIZE);
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "macroENB_ID Pointer NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        else if (T_s1ap_ENB_ID_homeENB_ID == 
                p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.t)
        {
            if (PNULL != p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID)
            {
                p_gb_enb_id->enb_id.presence_bitmask |=
                    S1AP_OAM_HOME_ENB_ID_PRESENT;

                l3_memcpy_wrapper(p_gb_enb_id->enb_id.home_enb_id.eNB_id,
                        p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID->data,
                        HOME_ENB_ID_OCTET_SIZE);

            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "homeENB_ID Pointer NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        else if (T_s1ap_ENB_ID_extElem1 ==
                p_s1ap_SourceeNB_ID->global_ENB_ID.eNB_ID.t)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "OPEN Type not supported", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
        else 
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "Unable to process", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        /** Supported TA **/
        s1ap_generate_plmn_identity_to_int(&p_rrm_supp_ta->plmn_identity,
                p_s1ap_SourceeNB_ID->selected_TAI.pLMNidentity.data);

        /** Copy the TAC **/
        l3_memcpy_wrapper(p_rrm_supp_ta->tac,
                p_s1ap_SourceeNB_ID->selected_TAI.tAC.data,
                p_s1ap_SourceeNB_ID->selected_TAI.tAC.numocts);
    }while(0);

    return result;
}

/*****************************************************************************
 * Function Name  : parse_s1ap_TargeteNB_ID 
 * Inputs         : p_gb_enb_id - pointer to rrm_gb_enb_id_t
 *                  p_rrm_supp_ta - pointer to rrm_supp_ta_t
 *                  p_s1ap_TargeteNB_ID - pointer to s1ap_TargeteNB_ID 
 * Outputs        : p_gb_enb_id
 * Returns        : RRC_FAILURE/RRC_SUCCESS
 * Description    : This parses target eNB id over S1 interface.
 ****************************************************************************/
rrc_return_et parse_s1ap_TargeteNB_ID(rrm_gb_enb_id_t *p_gb_enb_id,
        rrm_supp_ta_t *p_rrm_supp_ta,
        s1ap_TargeteNB_ID *p_s1ap_TargeteNB_ID)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_gb_enb_id);
    RRC_ASSERT(PNULL != p_s1ap_TargeteNB_ID);

    do
    {
        /** PLMN ID **/
        /* Convert from BCD to Int */
        s1ap_generate_plmn_identity_to_int(&p_gb_enb_id->plmn_identity,
                p_s1ap_TargeteNB_ID->global_ENB_ID.pLMNidentity.data);

        /** ENB ID **/
        if (T_s1ap_ENB_ID_macroENB_ID == 
                p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.t)
        {
            if (PNULL != p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID)
            {
                p_gb_enb_id->enb_id.presence_bitmask |= 
                    S1AP_OAM_MACRO_ENB_ID_PRESENT;

                l3_memcpy_wrapper(p_gb_enb_id->enb_id.macro_enb_id.eNB_id, 
                        p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.u.macroENB_ID->data,
                        MACRO_ENB_ID_OCTET_SIZE);
                /* SPR 19927 Fix Start */
                p_gb_enb_id->enb_id.macro_enb_id.eNB_id[MACRO_ENB_ID_OCTET_SIZE - 1] =
                         (p_gb_enb_id->enb_id.macro_enb_id.eNB_id[MACRO_ENB_ID_OCTET_SIZE - 1] & 0xF0);
                /* SPR 19927 Fix Stop */
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "macroENB_ID Pointer NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        else if (T_s1ap_ENB_ID_homeENB_ID == 
                p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.t)
        {
            if (PNULL != p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID)
            {
                p_gb_enb_id->enb_id.presence_bitmask |=
                    S1AP_OAM_HOME_ENB_ID_PRESENT;

                l3_memcpy_wrapper(p_gb_enb_id->enb_id.home_enb_id.eNB_id,
                        p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.u.homeENB_ID->data,
                        HOME_ENB_ID_OCTET_SIZE);
                /* SPR 19927 Fix Start */
                p_gb_enb_id->enb_id.home_enb_id.eNB_id[HOME_ENB_ID_OCTET_SIZE - 1] =
                     (p_gb_enb_id->enb_id.home_enb_id.eNB_id[HOME_ENB_ID_OCTET_SIZE - 1] & 0xF0);
                /* SPR 19927 Fix Stop */
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "homeENB_ID Pointer NULL", __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        else if (T_s1ap_ENB_ID_extElem1 ==
                p_s1ap_TargeteNB_ID->global_ENB_ID.eNB_ID.t)
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "OPEN Type not supported", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
        else 
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                    "Unable to process", __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        /** Supported TA **/
        s1ap_generate_plmn_identity_to_int(&p_rrm_supp_ta->plmn_identity,
                p_s1ap_TargeteNB_ID->selected_TAI.pLMNidentity.data);

        /** Copy the TAC **/
        l3_memcpy_wrapper(p_rrm_supp_ta->tac,
                p_s1ap_TargeteNB_ID->selected_TAI.tAC.data,
                p_s1ap_TargeteNB_ID->selected_TAI.tAC.numocts);

    }while(0);

    return result;
}

/*****************************************************************************
 * Function Name  : parse_s1ap_SONInformation 
 * Inputs         : p_mme_config_transfer - pointer to 
 *                                         s1ap_rrm_mme_config_transfer_t
 *                  p_SONInformation - pointer to s1ap_SONInformation
 * Outputs        : none
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : This function parses the s1ap son information
 ****************************************************************************/
rrc_return_et parse_s1ap_SONInformation( 
        s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,
        s1ap_SONInformation *p_SONInformation)
{
    rrc_return_et result = RRC_SUCCESS;
    s1ap_SONInformationReply *p_s1ap_SONInformationReply = PNULL;

    RRC_ASSERT(PNULL != p_mme_config_transfer);
    RRC_ASSERT(PNULL != p_SONInformation);

    do 
    {
        if (T_s1ap_SONInformation_sONInformationRequest == p_SONInformation->t)
        {
            if (s1ap_x2TNL_Configuration_Info == 
                    p_SONInformation->u.sONInformationRequest)
            {
                p_mme_config_transfer->bitmask |= 
                    MME_CONFIG_TRANSFER_SON_INFO_REQ;
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "SONInformationRequest Type Missing",
                        __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
        }
        else if(T_s1ap_SONInformation_sONInformationReply == p_SONInformation->t)
        {
            p_mme_config_transfer->bitmask |= 
                MME_CONFIG_TRANSFER_SON_INFO_RESP; 
            p_s1ap_SONInformationReply = p_SONInformation->u.sONInformationReply;

            if (PNULL != p_s1ap_SONInformationReply)
            {
                if(p_s1ap_SONInformationReply->
                        m.x2TNLConfigurationInfoPresent)
                {

                    if (p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n == 1)
                    {
                        if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits == 32)
                        {
                            p_mme_config_transfer->number_of_addr = 
                                (U8)p_s1ap_SONInformationReply->x2TNLConfigurationInfo.
                                eNBX2TransportLayerAddresses.n;
                            l3_memcpy_wrapper(
                                    p_mme_config_transfer->ip_addr[0].ip_addr,
                                    p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data, 
                                    RRC_MAX_IP_ADDR_LEN_IPV6/4);
                        }

                        else if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits == 128)
                        {
                            p_mme_config_transfer->bitmask |= 
                                MME_CONFIG_TRANSFER_X2TNL_IPv6;
                            p_mme_config_transfer->number_of_ipv6_addr = 
                                (U8)p_s1ap_SONInformationReply->x2TNLConfigurationInfo.
                                eNBX2TransportLayerAddresses.n;
                            l3_memcpy_wrapper(
                                    p_mme_config_transfer->ip_v6_addr[0].ip_addr,
                                    p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data, 
                                    RRC_MAX_IP_ADDR_LEN_IPV6);

                        }
                        else
                        {
                            RRC_S1AP_TRACE(RRC_WARNING ,"%s: Invalid IP Address",
                                    __FUNCTION__);
                            result = RRC_FAILURE;
                            break;

                        }
                    }
                    else if (p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.n == 2)
                    {
                        if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits == 32)
                        {
                            p_mme_config_transfer->number_of_addr = 1;
                            l3_memcpy_wrapper(
                                    p_mme_config_transfer->ip_addr[0].ip_addr,
                                    p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].data, 
                                    RRC_MAX_IP_ADDR_LEN_IPV6/4);
                        }

                        else if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[0].numbits == 128)
                        {
                            p_mme_config_transfer->bitmask |= 
                                MME_CONFIG_TRANSFER_X2TNL_IPv6;
                            p_mme_config_transfer->number_of_ipv6_addr = 1;
                            l3_memcpy_wrapper(
                                    p_mme_config_transfer->ip_v6_addr[1].ip_addr,
                                    p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data, 
                                    RRC_MAX_IP_ADDR_LEN_IPV6);

                        }
                        else
                        {
                            RRC_S1AP_TRACE(RRC_WARNING ,"%s: Invalid IP Address",
                                    __FUNCTION__);
                            result = RRC_FAILURE;
                            break;
                        }
                        if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].numbits == 32)
                        {
                            p_mme_config_transfer->number_of_addr += 1 ;
                            l3_memcpy_wrapper(
                                    p_mme_config_transfer->ip_addr[1].ip_addr,
                                    p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data, 
                                    RRC_MAX_IP_ADDR_LEN_IPV6/4);
                        }

                        else if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].numbits == 128)
                        {
                            p_mme_config_transfer->bitmask |= 
                                MME_CONFIG_TRANSFER_X2TNL_IPv6;
                            p_mme_config_transfer->number_of_ipv6_addr += 1;
                            l3_memcpy_wrapper(
                                    p_mme_config_transfer->ip_v6_addr[1].ip_addr,
                                    p_s1ap_SONInformationReply->x2TNLConfigurationInfo.eNBX2TransportLayerAddresses.elem[1].data, 
                                    RRC_MAX_IP_ADDR_LEN_IPV6);

                        }
                        else
                        {
                            RRC_S1AP_TRACE(RRC_WARNING ,"%s: Invalid IP Address",
                                    __FUNCTION__);
                            result = RRC_FAILURE;
                            break;
                        }
                    }
                    else
                    {
                        RRC_S1AP_TRACE(RRC_WARNING ,"%s: TNL INFO MISSING",
                                __FUNCTION__);
                        result = RRC_FAILURE;
                        break;
                    }
                    if(p_s1ap_SONInformationReply->x2TNLConfigurationInfo.m.iE_ExtensionsPresent)
                    {
                        p_mme_config_transfer->bitmask |= 
                            MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT;
                        p_mme_config_transfer->bitmask |= 
                            MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT;
                       if (RRC_FAILURE == parse_x2tnl_config_info_ie_extensions(&p_s1ap_SONInformationReply->x2TNLConfigurationInfo.iE_Extensions,p_mme_config_transfer))
                       {
                            result = RRC_FAILURE;
                            break;
                       }
                    }
                }
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                        "SONInformationReply Pointer NULL",
                        __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }

        }
    }while (0);

    return result;
}

/*****************************************************************************
 * Function Name  : parse_error_indication
 * Inputs         : p_parsed_error_indication - pointer to 
 *                                              rrc_s1ap_error_indication_t
 *                  p_asn_msg - Pointer to S1 SETUP FAILURE Asn message to be
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message
 *                  p_s1ap_gb_ctx - pointer to S1AP global context 
 * Outputs        : None
 * Returns        : RRC_SUCCESS/ RRC_FAILURE 
 * Description    : This function parses the error indication 
 *****************************************************************************/
rrc_return_et parse_error_indication (
        rrc_s1ap_error_indication_t *p_parsed_error_indication,
        void                        *p_asn_msg,
        U16                         asn_msg_len,
        s1ap_gb_context_t           *p_s1ap_gb_ctx)
{
    S1AP_PDU                        s1ap_pdu;
    rrc_return_et                   result = RRC_SUCCESS;
    OSCTXT                          asn1_ctx;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U8                          cell_id = RRC_INVALID_CELL_INDEX;

    RRC_S1AP_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                "context initialization failed.",__FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(&asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
        else
        {
           
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU ",&s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                  (U32)asn_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header, (U32)asn_msg_len,
                        LOCAL_S1_ERROR_INDICATION, MESSAGE_DIRECTION_RECEIVED);

                if ((PNULL != p_parsed_error_indication) && (p_parsed_error_indication->bitmask & 
                            RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT))
                {
                    p_event_header->EVENT_PARAM_MMES1APID = p_parsed_error_indication->
                        mme_ue_s1ap_id;
                }
                if ((PNULL != p_parsed_error_indication) && (p_parsed_error_indication->bitmask &
                            RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT))
                {
                    p_event_header->EVENT_PARAM_RAC_UE_REF = p_parsed_error_indication->
                        enb_ue_s1ap_id;

                    /* Get cell id from enb_ue_s1ap_id */
                    cell_id = s1ap_get_cell_id(p_s1ap_gb_ctx, 
                                  p_parsed_error_indication->enb_ue_s1ap_id);
                    if ((cell_id >= MAX_NUM_CELLS) || 
                            (!(p_s1ap_gb_ctx->
                               cell_s1ap_contexts[cell_id])))
                    {
                        RRC_S1AP_TRACE(RRC_INFO,
                                "Cell id is invalid\n");
                    }
                    else
                    {
                        l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID,
                                p_s1ap_gb_ctx->cell_s1ap_contexts[cell_id]->s1ap_cell_area_info.
                                cell_area_info.cell_id, CELL_ID_OCTET_SIZE);

                        p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->EVENT_PARAM_CELL_ID;
                    }
                }

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is successful outcome or not*/
        if ((T_S1AP_PDU_initiatingMessage != s1ap_pdu.t) ||
                (ASN1V_s1ap_id_ErrorIndication != 
                 s1ap_pdu.u.initiatingMessage->procedureCode) ||
                (PNULL == 
                  s1ap_pdu.u.initiatingMessage->value.u.errorIndication) ||
                 (T1_errorIndication != 
                  s1ap_pdu.u.initiatingMessage->value.t))
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        /* Decode message */
        /*klockwork fix*/
        if( PNULL != p_parsed_error_indication )
        {
            result = rrc_s1ap_error_indication_intrl_dec(
                s1ap_pdu.u.initiatingMessage->value.u.errorIndication,
                p_parsed_error_indication);
        }
        /*klockwork fix*/

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_error_indication_intrl_dec
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_3gpp - unpacked rrc_s1ap_error_indication_t
 *                  p_intrl - pointer to rrc_s1ap_error_indication_t
 * Outputs        : p_intrl (internal s1ap_ErrorIndication)
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : makes rrc_s1ap_error_indication_t from
 *                  s1ap_ErrorIndication
 ****************************************************************************/
rrc_return_et rrc_s1ap_error_indication_intrl_dec
(
 s1ap_ErrorIndication    *p_3gpp,    /* unpacked src */
 rrc_s1ap_error_indication_t *p_intrl/* dest */
 )
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode  *p_node     = PNULL;
    s1ap_ErrorIndication_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);

    if (PNULL == p_intrl)
    {
	RRC_TRACE(RRC_ERROR,"%s:"
	    " s1ap_error_indication received null ",__FUNCTION__);
    	RRC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

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
                result = RRC_FAILURE;
                break;
            }

            p_protocol_IE =
                (s1ap_ErrorIndication_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {

                        p_intrl->mme_ue_s1ap_id = 
                            p_protocol_IE->value.u._ErrorIndicationIEs_1;
                        p_intrl->bitmask |=
                            RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT;
                    }
                    break;
                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        p_intrl->enb_ue_s1ap_id = 
                            p_protocol_IE->value.u._ErrorIndicationIEs_2;
                        p_intrl->bitmask |=
                            RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT;
                    }
                    break;

                case ASN1V_s1ap_id_Cause:
                    {
                        p_intrl->cause = 
                            *p_protocol_IE->value.u._ErrorIndicationIEs_3;
                        p_intrl->bitmask |=
                            RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT;
                    }
                    break;
                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        p_intrl->criticality_diagnostics = 
                            *p_protocol_IE->value.u._ErrorIndicationIEs_4;
                        p_intrl->bitmask |=
                            RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT;
                    }
                    break;

                default:
                    RRC_TRACE(RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);
                    break;                  
            }
            p_node= p_node->next;            
        }

    } while (0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_error_indication_tse_intrl_enc
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to ASN.1 encoded message
 *                  p_buff_size - pointer to  size of result buffer
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_error_indication_tse_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size   /* size of result buffer */
 )
{

    S1AP_PDU s1ap_pdu;
    rrc_return_et result = RRC_FAILURE;
    s1ap_ErrorIndication *p_s1ap_ErrorIndication = PNULL;
    s1ap_Cause cause;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt); 
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_buff_size);

    do
    {
        /* S1PDU Init */
        asn1Init_S1AP_PDU(&s1ap_pdu);
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, InitiatingMessage);

        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {       
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
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
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_ErrorIndication = s1ap_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_s1ap_ErrorIndication(p_s1ap_ErrorIndication);

        /* ONLY CAUSE HAS TO BE SENT IN CASE OF TSE */

        /*Fill the cause */

        cause.t = T_s1ap_Cause_protocol;
        cause.u.protocol = s1ap_transfer_syntax_error;

        if (RT_OK != asn1Append_s1ap_ErrorIndication_protocolIEs_3
                (pctxt,
                 &p_s1ap_ErrorIndication->protocolIEs,
                 &cause))
        {
            break;
        }
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_TRACE(RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }
        else
        {
            encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

            
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
              &s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                 encoded_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header,encoded_msg_len,
                        LOCAL_S1_ERROR_INDICATION, MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_buff,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;
    }
    while(0);

    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_error_indication_ase_intrl_enc
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to result ASN.1 encoded message
 *                  p_buff_size -  size of result buffer 
 *                  p_error_indication - pointer to rrc_s1ap_error_indication_t
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 rrc_s1ap_error_indication_t  *p_error_indication
 )
{
    rrc_return_et result = RRC_FAILURE;
    S1AP_PDU s1ap_pdu;
    s1ap_ErrorIndication *p_s1ap_msg;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

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
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
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
            RRC_TRACE(RRC_ERROR, "ASN malloc failed.");
            break;
        }
        p_s1ap_msg = s1ap_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_s1ap_ErrorIndication(p_s1ap_msg);

        if (PNULL != p_error_indication)
        {
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
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, S1AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PE_S1AP_PDU(pctxt, &s1ap_pdu))
        {
            RRC_TRACE(RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }
        else
        {
            encoded_msg_len = (U32)pe_GetMsgLen(pctxt);
            
           
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                                   encoded_msg_len);
            if (PNULL != p_event_header)
            {
                s1ap_fill_protocol_event_params(p_event_header,encoded_msg_len,
                        LOCAL_S1_ERROR_INDICATION, MESSAGE_DIRECTION_SEND);

                if ((PNULL != p_error_indication) && (p_error_indication->bitmask & 
                            RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT))
                {
                    p_event_header->EVENT_PARAM_MMES1APID = p_error_indication->
                        mme_ue_s1ap_id;
                }
                if ((PNULL != p_error_indication) && (p_error_indication->bitmask &
                            RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT))
                {
                    p_event_header->EVENT_PARAM_RAC_UE_REF = p_error_indication->
                        enb_ue_s1ap_id;
                }

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_buff,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }


        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=RRC_SUCCESS;

#ifdef S1AP_UT_TESTING
        asn1Print_s1ap_ErrorIndication("Error Indication", p_s1ap_msg);
#endif
    } while(0);
    RRC_UT_TRACE_EXIT();
    return result;
}


/* etws_start*/
/****************************************************************************
 * Function Name  : s1ap_write_replace_warning_req_intrl_dec
 * Inputs         : p_3gpp - pointer to s1ap_WriteReplaceWarningRequest
 *                  p_intrl - pointer to rrc_s1ap_write_replace_warning_request_t
 *                  p_mme_context - pointer to MME context
 * Outputs        : p_intrl (internal s1ap_ErrorIndication)
 * Returns        : RRC_SUCCESS /RRC_FAILURE
 * Description    : makes rrc_s1ap_write_replace_warning_request_t from
 *                  s1ap_WriteReplaceWarningRequest
 ****************************************************************************/
rrc_return_et s1ap_write_replace_warning_req_intrl_dec
(
 s1ap_WriteReplaceWarningRequest    *p_3gpp,    /* unpacked src */
 rrc_s1ap_write_replace_warning_request_t *p_intrl,/* dest */
 mme_context_t *p_mme_context  /* MME Context */
 )
{
    rrc_return_et result = RRC_SUCCESS; 
    s1ap_WriteReplaceWarningRequest_protocolIEs_element 
        *p_protocol_IE = RRC_NULL;
    U8 index = 0;
    OSRTDListNode* p_node = PNULL;
    s1ap_error_ind_bool_t send_err_ind ;
    /* SPR 13710 Fix Start */
    rrc_s1ap_error_indication_t err_ind_content;
    /* SPR 13710 Fix Stop */
    
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    OSCTXT asn1_ctx;

    s1ap_message_data_t message_map =
    {11, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0,ASN1V_s1ap_id_MessageIdentifier, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {1,ASN1V_s1ap_id_SerialNumber, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {2, ASN1V_s1ap_id_WarningAreaList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {3, ASN1V_s1ap_id_RepetitionPeriod, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {4, ASN1V_s1ap_id_ExtendedRepetitionPeriod, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {5, ASN1V_s1ap_id_NumberofBroadcastRequest, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {6, ASN1V_s1ap_id_WarningType, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {7, ASN1V_s1ap_id_WarningSecurityInfo, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {8, ASN1V_s1ap_id_DataCodingScheme, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {9, ASN1V_s1ap_id_WarningMessageContents, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {10, ASN1V_s1ap_id_ConcurrentWarningMessageIndicator, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}
        }};

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    RRC_UT_TRACE_ENTER();
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_S1AP_TRACE(RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    memset_wrapper (&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t));
    /* SPR 13710 Fix Start */
    memset_wrapper (&err_ind_content, RRC_NULL, sizeof(rrc_s1ap_error_indication_t));
    /* SPR 13710 Fix Stop */
    
    memset_wrapper (&iE_list, RRC_NULL, sizeof(s1ap_error_ind_ie_list_t));
    do
    {

        p_node = p_3gpp->protocolIEs.head;
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE = (s1ap_WriteReplaceWarningRequest_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MessageIdentifier:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_1
                                    ))
                            p_intrl->message_identifier = 
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_1;
                        break;
                    }
                case ASN1V_s1ap_id_SerialNumber:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_2
                                    ))
                            p_intrl->serial_number =
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_2;
                        break;
                    }
                case ASN1V_s1ap_id_WarningAreaList:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_3
                                    ))
                        {
                            p_intrl->warning_area_list =
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_3;

                            p_intrl->bitmask |= 
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_AREA_LIST_PRESENT;
                        }
                        break;
                    }
                case ASN1V_s1ap_id_RepetitionPeriod:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_4
                                    ))
                            p_intrl->repetition_period = 
                                p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_4;
                        break;
                    }
                case ASN1V_s1ap_id_ExtendedRepetitionPeriod:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_5
                                    ))
                        {
                            p_intrl->bitmask |= 
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_EXTENDED_REPETITION_PERIOD_PRESENT;
                            p_intrl->extended_repetition_period =
                                p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_5 ;
                        }
                        break;
                    }

                case ASN1V_s1ap_id_NumberofBroadcastRequest:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_6
                                    ))
                            p_intrl->numberof_broadcast_request =
                                p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_6;
                        break;
                    }
                case ASN1V_s1ap_id_WarningType:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_7
                                    ))
                        {
                            p_intrl->warning_type = 
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_7;
                            p_intrl->bitmask |=
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_TYPE_PRESENT;
                            message_map.msg_map[6].presence = s1ap_mandatory;
                        }
                        break;
                    }
                case ASN1V_s1ap_id_WarningSecurityInfo:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_8
                                    ))

                        {
                            p_intrl->warning_security_info =
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_8;
                            p_intrl->bitmask |=
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_SECURITY_INFO_PRESENT;
                        }
                        break;
                    }
                case ASN1V_s1ap_id_DataCodingScheme:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_9
                                    ))
                        {
                            p_intrl->data_coding_scheme =
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_9;
                            p_intrl->bitmask |=
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_DATA_CODING_SCHEME_PRESENT;
                        }
                        break;
                    }
                case ASN1V_s1ap_id_WarningMessageContents:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_10
                                    ))
                        {
                            p_intrl->warning_message_contents =
                                *p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_10;
                            p_intrl->bitmask |=
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_MESSAGE_CONTENTS_PRESENT;
                        }
                        break;
                    }
                case ASN1V_s1ap_id_ConcurrentWarningMessageIndicator:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_11
                                    ))
                        {
                            p_intrl->bitmask |= 
                                RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_CONCURRENT_WARNING_MESSAGE_INDICATOR_PRESENT;
                            p_intrl->concurrent_warning_message_indicator =
                                p_protocol_IE->value.u._WriteReplaceWarningRequestIEs_11;
                        }
                        break ;
                    }
                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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

                    result = RRC_FAILURE;
            }
            p_node= p_node->next;
        }
    } while (0);

    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_WriteReplaceWarning,
                    T_S1AP_PDU_initiatingMessage,
                    s1ap_reject,
                /* SPR 13710 Fix Start */
                    &err_ind_content)) )
                /* SPR 13710 Fix Stop */
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "Write Replace Warning - Response Message Error Indication Ignored");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}
/* etws_end*/

/****************************************************************************
 * Function Name  : check_plmn_validity
 * Inputs         : index - a unsigned char type
 *                  octateValue - value
 * Outputs        : None 
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : Check the validity of PLMN.
 ****************************************************************************/
rrc_return_et check_plmn_validity(U8 index, U8 octateValue)
{
    rrc_return_et result = RRC_FAILURE;

    switch (index)
    {
        case 0:
        case 2:
            /* Check the first Octate */
            if (((octateValue & 0x0F) <= MAX_PLMN_VAL) && 
                    ((octateValue >> 0x04) <= MAX_PLMN_VAL))
            {
                result = RRC_SUCCESS;
            }

            break;
        case 1:
            /* Check the second Octate */

            /* check for the filler bit at second nibble */ 
            if (((octateValue & 0x0F) <= MAX_PLMN_VAL) &&  
                    (((octateValue >> 0x04) == 0xF) ||
                     ((octateValue >> 0x04) <= MAX_PLMN_VAL))) 
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

#ifdef TESTING_BUFFER_GENERATION

/*****************************************************************************
 * Function Name  : compose_asn_s1_setup_failure
 * Inputs         : p_s1_setup_failure - pointer to s1_setup_failure_t
 *                  p_asn_msg - pointer to result buffer which is asn encoded.
 *                  p_asn_msg_len - pointer to length of asn message encoded.
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function composes S1 SETUP FAILURE Asn Message.This
 *                  function is used for testing purpose only.
 *****************************************************************************/
rrc_return_et compose_asn_s1_setup_failure (
        s1_setup_failure_t   *p_s1_setup_failure,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    s1ap_S1SetupFailure_protocolIEs_element *     p_protocolIE_elem = PNULL;
    s1ap_S1SetupFailure            * p_s1ap_s1_setup_failure = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to T_S1AP_PDU_unsuccessfulOutcome message */
        s1ap_pdu.t = T_S1AP_PDU_unsuccessfulOutcome;

        s1ap_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx, 
                UnsuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.unsuccessfulOutcome)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_UnsuccessfulOutcome (s1ap_pdu.u.unsuccessfulOutcome);

        s1ap_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_s1ap_id_S1Setup;
        s1ap_pdu.u.unsuccessfulOutcome->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.unsuccessfulOutcome->value.t =  T58_s1Setup;

        p_s1ap_s1_setup_failure = rtxMemAllocType(&asn1_ctx, s1ap_S1SetupFailure);
        if (PNULL == p_s1ap_s1_setup_failure)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_s1ap_S1SetupFailure (p_s1ap_s1_setup_failure);

        s1ap_pdu.u.unsuccessfulOutcome->value.u.s1Setup = p_s1ap_s1_setup_failure;

        rtxDListInit(&p_s1ap_s1_setup_failure->protocolIEs);

        /*** Compose id: id-Cause ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupFailure_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        asn1Init_s1ap_S1SetupFailure_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_Cause;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T39__S1SetupFailureIEs_1;

        p_protocolIE_elem->value.u._S1SetupFailureIEs_1 = 
            &p_s1_setup_failure->cause; 

        rtxDListAppendNode(&p_s1ap_s1_setup_failure->protocolIEs, p_node);

        /*** Compose id: id-time to wait ***/
        if (p_s1_setup_failure->time_to_wait)
        {
            rtxDListAllocNodeAndData(&asn1_ctx, 
                    s1ap_S1SetupFailure_protocolIEs_element, 
                    &p_node, 
                    &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            }
            asn1Init_s1ap_S1SetupFailure_protocolIEs_element (
                    p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_s1ap_id_TimeToWait;
            p_protocolIE_elem->criticality = s1ap_ignore;

            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T39__S1SetupFailureIEs_2;

            p_protocolIE_elem->value.u._S1SetupFailureIEs_2 = 
                p_s1_setup_failure->time_to_wait; 

            rtxDListAppendNode(&p_s1ap_s1_setup_failure->protocolIEs, p_node);
        }

        rtxDListInit(&p_s1ap_s1_setup_failure->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}


/*****************************************************************************
 * Function Name  : compose_asn_s1_setup_response
 * Inputs         : p_s1_setup_response - pointer to parsed_s1_setup_rsp_t
 *                  p_asn_msg - pointer to result buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS / RRC_FAILURE 
 * Description    : This function composes S1 SETUP RESPONSE Asn Message.This
 *                  function is used for testing purpose only.
 *****************************************************************************/
rrc_return_et compose_asn_s1_setup_response (
        parsed_s1_setup_rsp_t   *p_s1_setup_response,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    s1ap_MME_Group_ID *p_grp_id;
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    OSRTDListNode               *p_mme_grp_node = PNULL;
    s1ap_S1SetupResponse_protocolIEs_element *     p_protocolIE_elem = PNULL;
    s1ap_S1SetupResponse            * p_s1ap_s1_setup_response = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to successfulOutcome message */
        s1ap_pdu.t = T_S1AP_PDU_successfulOutcome;

        s1ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx, 
                SuccessfulOutcome);
        if (PNULL == s1ap_pdu.u.successfulOutcome)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        memset_wrapper(s1ap_pdu.u.successfulOutcome,0,sizeof(SuccessfulOutcome));

        s1ap_pdu.u.successfulOutcome->procedureCode = ASN1V_s1ap_id_S1Setup;
        s1ap_pdu.u.successfulOutcome->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.successfulOutcome->value.t =  T58_s1Setup;

        p_s1ap_s1_setup_response = rtxMemAllocType(&asn1_ctx, s1ap_S1SetupResponse);
        if (PNULL == p_s1ap_s1_setup_response)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        memset_wrapper(p_s1ap_s1_setup_response,0,sizeof(s1ap_S1SetupResponse));

        s1ap_pdu.u.successfulOutcome->value.u.s1Setup = p_s1ap_s1_setup_response;

        rtxDListInit(&p_s1ap_s1_setup_response->protocolIEs);

        /*** Compose id: id-mme name ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupResponse_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_S1SetupResponse_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_MMEname;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T38__S1SetupResponseIEs_1;

        p_protocolIE_elem->value.u._S1SetupResponseIEs_1 =
            p_s1_setup_response->p_mme_name;

        rtxDListAppendNode(&p_s1ap_s1_setup_response->protocolIEs, p_node);

        /**************served gummeis********************/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupResponse_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_S1SetupResponse_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_ServedGUMMEIs;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T38__S1SetupResponseIEs_2;

        OSRTDList s1ap_ServedGUMMEIs;
        s1ap_ServedGUMMEIsItem * p_s1ap_ServedGUMMEIsItem;
        rtxDListInit(&s1ap_ServedGUMMEIs);
        OSRTDListNode               *p_node2 = PNULL;
        p_protocolIE_elem->value.u._S1SetupResponseIEs_2 =
            &s1ap_ServedGUMMEIs;

        {
            rtxDListAllocNodeAndData(&asn1_ctx, 
                    s1ap_ServedGUMMEIsItem, 
                    &p_node2, 
                    &p_s1ap_ServedGUMMEIsItem);

            OSRTDListNode               *p_node3 = PNULL;
            s1ap_MME_Group_ID * p_s1ap_MME_Group_ID;
            rtxDListAllocNodeAndData(&asn1_ctx, 
                    s1ap_MME_Group_ID, 
                    &p_node3, 
                    &p_s1ap_MME_Group_ID);
            memset_wrapper(p_s1ap_MME_Group_ID,0,sizeof(s1ap_MME_Group_ID));
            rtxDListInit(&p_s1ap_ServedGUMMEIsItem->servedGroupIDs);
            p_s1ap_MME_Group_ID->numocts =2;
            rtxDListAppendNode(&p_s1ap_ServedGUMMEIsItem->servedGroupIDs,
                    p_node3);

            rtxDListInit(&p_s1ap_ServedGUMMEIsItem->extElem1);

            p_s1ap_ServedGUMMEIsItem->servedPLMNs.n =1;
            p_s1ap_ServedGUMMEIsItem->servedPLMNs.elem[0].numocts =3;
            p_s1ap_ServedGUMMEIsItem->servedPLMNs.elem[0].data[0] =0x21;
            p_s1ap_ServedGUMMEIsItem->servedPLMNs.elem[0].data[1] =0x13;
            p_s1ap_ServedGUMMEIsItem->servedPLMNs.elem[0].data[2] =0x32;

            rtxDListInit(&p_s1ap_ServedGUMMEIsItem->servedGroupIDs);


            rtxDListAllocNodeAndData(&asn1_ctx, 
                    s1ap_MME_Group_ID, 
                    &p_mme_grp_node, 
                    &p_grp_id);
            asn1Init_s1ap_MME_Group_ID(p_grp_id);
            p_grp_id->numocts = 2;
            p_grp_id->data[0] = 0;
            p_grp_id->data[1] = 0;

            rtxDListAppendNode(&p_s1ap_ServedGUMMEIsItem->servedGroupIDs,
                    p_mme_grp_node);

            p_s1ap_ServedGUMMEIsItem->servedMMECs.n =1;
            p_s1ap_ServedGUMMEIsItem->servedMMECs.elem[0].numocts =1;
            p_s1ap_ServedGUMMEIsItem->servedMMECs.elem[0].data[0] =1;
        }
        rtxDListAppendNode(&s1ap_ServedGUMMEIs, p_node2);

        rtxDListAppendNode(&p_s1ap_s1_setup_response->protocolIEs, p_node);
        /***********************************/
        /****************************relative mme capacity******/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupResponse_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_S1SetupResponse_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_RelativeMMECapacity;
        p_protocolIE_elem->criticality = s1ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T38__S1SetupResponseIEs_3;
        p_protocolIE_elem->value.u._S1SetupResponseIEs_3 = 1;

        rtxDListAppendNode(&p_s1ap_s1_setup_response->protocolIEs, p_node);
        /***********************************/
        /****************************critical diag******/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_S1SetupResponse_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_S1SetupResponse_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_CriticalityDiagnostics;
        p_protocolIE_elem->criticality = s1ap_ignore;

        s1ap_CriticalityDiagnostics crit_diag;

        memset_wrapper(&crit_diag,0,sizeof(s1ap_CriticalityDiagnostics));
        rtxDListInit(&crit_diag.iEsCriticalityDiagnostics);
        rtxDListInit(&crit_diag.extElem1);
        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T38__S1SetupResponseIEs_4;
        p_protocolIE_elem->value.u._S1SetupResponseIEs_4 = &crit_diag;

        rtxDListAppendNode(&p_s1ap_s1_setup_response->protocolIEs, p_node);
        /***********************************/

        rtxDListInit(&p_s1ap_s1_setup_response->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Resp "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}


/*****************************************************************************
 * Function Name  : compose_asn_initial_ctx_setup_request
 * Inputs         : mmeUeS1apId - ue id over S1 interface in MME
 *                  enbUeS1apId -  ue id over S1 interface in eNB 
 *                  p_asn_msg - pointer to result ASN encoded buffer
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS /RRC_FAILURE 
 * Description    : This function composes INITIAL CTX SETUP Asn Message.This
 *                  function is used for testing purpose only.
 *****************************************************************************/
rrc_return_et compose_asn_initial_ctx_setup_request (
        U32 mmeUeS1apId,
        U32 enbUeS1apId,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    s1ap_InitialContextSetupRequest_protocolIEs_element *     p_protocolIE_elem = PNULL;
    s1ap_InitialContextSetupRequest            * p_initial_ctx_setup = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to initiatingMessage message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_InitialContextSetup;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.initiatingMessage->value.t =  T58_initialContextSetup;

        p_initial_ctx_setup = rtxMemAllocType(&asn1_ctx, s1ap_InitialContextSetupRequest);
        if (PNULL == p_initial_ctx_setup)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        memset_wrapper(p_initial_ctx_setup,0,sizeof(s1ap_InitialContextSetupRequest));

        s1ap_pdu.u.initiatingMessage->value.u.initialContextSetup =
            p_initial_ctx_setup;

        rtxDListInit(&p_initial_ctx_setup->protocolIEs);

        /*** Compose id:id-MME-UE-S1AP-ID ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_InitialContextSetupRequest_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_InitialContextSetupRequest_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_MME_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T20__InitialContextSetupRequestIEs_1;

        p_protocolIE_elem->value.u._InitialContextSetupRequestIEs_1 =
            mmeUeS1apId;

        rtxDListAppendNode(&p_initial_ctx_setup->protocolIEs, p_node);

        /*** Compose id:id-eNB-UE-S1AP-ID ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_InitialContextSetupRequest_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_InitialContextSetupRequest_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_eNB_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T20__InitialContextSetupRequestIEs_2;

        p_protocolIE_elem->value.u._InitialContextSetupRequestIEs_2 =
            enbUeS1apId;

        rtxDListAppendNode(&p_initial_ctx_setup->protocolIEs, p_node);

        rtxDListInit(&p_initial_ctx_setup->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}


/*****************************************************************************
 * Function Name  : compose_asn_dl_nas_transport
 * Inputs         : mmeUeS1apId - ue id over S1 interface in MME
 *                  enbUeS1apId -  ue id over S1 interface in eNB 
 *                  p_asn_msg - pointer to result ASN encoded buffer
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS/RRC_FAILURE 
 * Description    : This function composes DL NAS TRANSPORT Asn Message.This
 *                  function is used for testing purpose only.
 *****************************************************************************/
rrc_return_et compose_asn_dl_nas_transport (
        U32 mmeUeS1apId,
        U32 enbUeS1apId,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    s1ap_DownlinkNASTransport_protocolIEs_element *     p_protocolIE_elem = PNULL;
    s1ap_DownlinkNASTransport            * p_dl_nas_transport = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to initiatingMessage message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_downlinkNASTransport;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.initiatingMessage->value.t =  T58_downlinkNASTransport;

        p_dl_nas_transport = rtxMemAllocType(&asn1_ctx, s1ap_DownlinkNASTransport);
        if (PNULL == p_dl_nas_transport)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        memset_wrapper(p_dl_nas_transport,0,sizeof(s1ap_DownlinkNASTransport));

        s1ap_pdu.u.initiatingMessage->value.u.downlinkNASTransport =
            p_dl_nas_transport;

        rtxDListInit(&p_dl_nas_transport->protocolIEs);

        /*** Compose id:id-MME-UE-S1AP-ID ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_DownlinkNASTransport_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_DownlinkNASTransport_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_MME_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T30__DownlinkNASTransport_IEs_1;

        p_protocolIE_elem->value.u._DownlinkNASTransport_IEs_1 =
            mmeUeS1apId;

        rtxDListAppendNode(&p_dl_nas_transport->protocolIEs, p_node);

        /*** Compose id:id-eNB-UE-S1AP-ID ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_DownlinkNASTransport_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_DownlinkNASTransport_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_eNB_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T30__DownlinkNASTransport_IEs_2;

        p_protocolIE_elem->value.u._DownlinkNASTransport_IEs_2 =
            enbUeS1apId;

        rtxDListAppendNode(&p_dl_nas_transport->protocolIEs, p_node);

        rtxDListInit(&p_dl_nas_transport->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}


/*****************************************************************************
 * Function Name  : compose_paging
 * Inputs         : p_asn_msg - pointer to ASN encoded buffer 
 *                  p_asn_msg_len - Pointer to length of asn message encoded. 
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes  PAGING Asn Message.This function
 *                  is used for testing purpose only.
 *****************************************************************************/
rrc_return_et compose_paging (
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    s1ap_Paging                 *p_paging = PNULL;
    s1ap_Paging_protocolIEs_element * p_protocolIE_elem = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to initiatingMessage message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_Paging;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        /* Set the initiating message type to S1 Setup */
        s1ap_pdu.u.initiatingMessage->value.t =  T58_paging;

        p_paging = rtxMemAllocType(&asn1_ctx, s1ap_Paging);
        if (PNULL == p_paging)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        memset_wrapper(p_paging,0,sizeof(s1ap_Paging));

        s1ap_pdu.u.initiatingMessage->value.u.paging = p_paging;

        rtxDListInit(&p_paging->protocolIEs);


        rtxDListInit(&p_paging->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}

/*****************************************************************************
 * Function Name  : compose_erab_release_command
 * Inputs         : mmeUeS1apId - ue id over S1 interface in MME
 *                  enbUeS1apId - ue id over S1 interface in eNB
 *                  p_asn_msg - pointer to result buffer which is asn encoded
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Outputs        : p_asn_msg - The buffer which is asn encoded.
 *                  p_asn_msg_len - Pointer to length of asn message encoded.
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : This function composes erab release command 
 *                  function is used for testing purpose only.
 *****************************************************************************/
rrc_return_et compose_erab_release_command (
        U32 mmeUeS1apId,
        U32 enbUeS1apId,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len)
{
    rrc_return_et               response = RRC_FAILURE;
    OSCTXT                      asn1_ctx;
    S1AP_PDU                    s1ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    s1ap_E_RABReleaseCommand_protocolIEs_element *     p_protocolIE_elem = PNULL;
    s1ap_E_RABReleaseCommand            * p_erab_release_command = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
                __FUNCTION__);
        RRC_S1AP_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    do
    {
        memset_wrapper(&s1ap_pdu,0,sizeof(S1AP_PDU));

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to initiatingMessage message */
        s1ap_pdu.t = T_S1AP_PDU_initiatingMessage;

        s1ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, 
                InitiatingMessage);
        if (PNULL == s1ap_pdu.u.initiatingMessage)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_InitiatingMessage (s1ap_pdu.u.initiatingMessage);

        s1ap_pdu.u.initiatingMessage->procedureCode = ASN1V_s1ap_id_E_RABRelease;
        s1ap_pdu.u.initiatingMessage->criticality = s1ap_reject;

        s1ap_pdu.u.initiatingMessage->value.t =  T58_e_RABRelease;

        p_erab_release_command = rtxMemAllocType(&asn1_ctx, s1ap_E_RABReleaseCommand);
        if (PNULL == p_erab_release_command)
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        memset_wrapper(p_erab_release_command,0,sizeof(s1ap_E_RABReleaseCommand));

        s1ap_pdu.u.initiatingMessage->value.u.e_RABRelease =
            p_erab_release_command;

        rtxDListInit(&p_erab_release_command->protocolIEs);

        /*** Compose id:id-MME-UE-S1AP-ID ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_E_RABReleaseCommand_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_E_RABReleaseCommand_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_MME_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T17__E_RABReleaseCommandIEs_1;

        p_protocolIE_elem->value.u._E_RABReleaseCommandIEs_1 =
            mmeUeS1apId;

        rtxDListAppendNode(&p_erab_release_command->protocolIEs, p_node);

        /*** Compose id:id-eNB-UE-S1AP-ID ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                s1ap_E_RABReleaseCommand_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        memset_wrapper(p_protocolIE_elem,0,
                sizeof(s1ap_E_RABReleaseCommand_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_s1ap_id_eNB_UE_S1AP_ID;
        p_protocolIE_elem->criticality = s1ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T17__E_RABReleaseCommandIEs_2;

        p_protocolIE_elem->value.u._E_RABReleaseCommandIEs_2 =
            enbUeS1apId;

        rtxDListAppendNode(&p_erab_release_command->protocolIEs, p_node);

        rtxDListInit(&p_erab_release_command->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                S1AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_S1AP_PDU(&asn1_ctx, &s1ap_pdu))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "ASN1 encoding of S1 Setup Request "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= pe_GetMsgLen(&asn1_ctx);
            response = RRC_SUCCESS;
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    RRC_S1AP_UT_TRACE_EXIT();

    return response;
}


#endif /* TESTING_BUFFER_GENERATION */


/*****************************************************************************
 * Function Name  : decode_s1ap_asn_kill_request
 * Inputs         : p_asn_msg - Pointer to kill request Asn message to be
 *                              decoded.
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_rrc_s1ap_kill_request - pointer to rrc_s1ap_kill_request_t
 *                  p_mme_ctx - pointre to MME context 
 * Outputs        : p_rrc_s1ap_kill_request - This structure is populated with
 *                                        information from input Asn Message.
 * Returns        : RRC_SUCCESS/RRC_FAILURE 
 * Description    : This function populatesrwith
 *                  information received after decoding kill_request
 *                  Asn Messages.
 *****************************************************************************/
rrc_return_et decode_s1ap_asn_kill_request (
        void                *p_asn_msg,
        OSCTXT              *p_asn1_ctx,
        U16                 asn_msg_len,
        rrc_s1ap_kill_request_t *p_rrc_s1ap_kill_request,
        mme_context_t* p_mme_ctx
        )
{
    S1AP_PDU                        s1ap_pdu;
    rrc_return_et                   result = RRC_SUCCESS;

    RRC_S1AP_UT_TRACE_ENTER();

    asn1Init_S1AP_PDU(&s1ap_pdu);

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_S1AP_PDU(p_asn1_ctx, &s1ap_pdu) )
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            /* Jitender: Send TSE */
            s1ap_build_and_send_err_indication(p_mme_ctx, PNULL, PNULL);
            result = RRC_FAILURE;
            break;
        }

        /* check whether the message is initiating mesg or not*/
        if( (T_S1AP_PDU_initiatingMessage != s1ap_pdu.t) ||
                (ASN1V_s1ap_id_Kill !=
                 s1ap_pdu.u.initiatingMessage->procedureCode) ||
                (PNULL == s1ap_pdu.u.initiatingMessage->value.u.kill) ||
                 (T1_kill !=
                  s1ap_pdu.u.initiatingMessage->value.t))
        {
            /* Drop message */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ASN "
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        /* Decode message */
        if (RRC_FAILURE == s1ap_kill_req_intrl_dec(
                    s1ap_pdu.u.initiatingMessage->value.u.kill,
                    p_rrc_s1ap_kill_request,
                    p_mme_ctx
                    ))
        {
            /* Message parsing error */
            RRC_S1AP_TRACE(RRC_WARNING,
                    "[s1ap_kill_req] Decoding error");
            result = RRC_FAILURE;
            /* Drop message */
            break;
        }

    }while (0);

    RRC_S1AP_UT_TRACE_EXIT();

    return result;
}

/****************************************************************************
 * Function Name  : s1ap_kill_req_intrl_dec 
 * Inputs         : p_3gpp - pointer to s1ap_KillRequest
 *                  p_intrl - pointer to rrc_s1ap_kill_request_t
 *                  p_mme_context - pointer to MME context
 * Outputs        : p_intrl (internal  s1ap_kill request)
 * Returns        : RRC_SUCCESS/RRC_FAILURE
 * Description    : makes rrc_s1ap_kill_request and fills in to p_intrl. 
 ****************************************************************************/
rrc_return_et s1ap_kill_req_intrl_dec
(
 s1ap_KillRequest    *p_3gpp,    /* unpacked src */
 rrc_s1ap_kill_request_t *p_intrl,/* dest */
 mme_context_t *p_mme_context 
 )
{
    s1ap_KillRequest_protocolIEs_element         *p_protocol_IE = RRC_NULL;
    U8 index = 0;
    OSRTDListNode* p_node = PNULL;
    rrc_return_et    result = RRC_SUCCESS;
    s1ap_error_ind_bool_t send_err_ind ;
    /* SPR 13710 Fix Start */
    rrc_s1ap_error_indication_t  err_ind_content;
    /* SPR 13710 Fix Stop */
    
    s1ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    OSCTXT asn1_ctx ;
    s1ap_message_data_t message_map =
    {3, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0,ASN1V_s1ap_id_MessageIdentifier, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {1,ASN1V_s1ap_id_SerialNumber, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
            {2, ASN1V_s1ap_id_WarningAreaList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}
        }};

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_mme_context);

    RRC_UT_TRACE_ENTER();

    memset_wrapper (&send_err_ind, RRC_NULL, sizeof(s1ap_error_ind_bool_t));
    /* SPR 13710 Fix Start */
    memset_wrapper (&err_ind_content, RRC_NULL, sizeof(rrc_s1ap_error_indication_t));
    /* SPR 13710 Fix Stop */
    
    memset_wrapper (&iE_list, RRC_NULL, sizeof(s1ap_error_ind_ie_list_t));
    do
    {

        p_node = p_3gpp->protocolIEs.head;
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_TRACE(RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE = (s1ap_KillRequest_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MessageIdentifier:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._KillRequestIEs_1
                                    ))
                        {

                            p_intrl->message_identifier =
                                * p_protocol_IE->value.u._KillRequestIEs_1;
                        }
                        break;
                    }
                case ASN1V_s1ap_id_SerialNumber:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._KillRequestIEs_2
                                    ))
                        {
                            p_intrl->serial_number =
                                *p_protocol_IE->value.u._KillRequestIEs_2;
                        }
                        break;

                    }
                case ASN1V_s1ap_id_WarningAreaList:
                    {
                        if (RRC_SUCCESS == validate_ie_value (
                                    &message_map,
                                    index,
                                    p_protocol_IE->id,
                                    &p_protocol_IE->value.u._KillRequestIEs_3
                                    ))
                        {

                            p_intrl->warning_area_list =
                                *p_protocol_IE->value.u._KillRequestIEs_3;

                            p_intrl->bitmask |=
                                RRC_S1AP_KILL_REQUEST_WARNING_AREA_LIST_PRESENT ;
                        }
                        break;
                    }
                default:
                    RRC_S1AP_TRACE(RRC_ERROR,"%s: "
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

                    result = RRC_FAILURE;
            }

            p_node= p_node->next;

        }
    } while (0);

    /* Parse the map for Error Indication */
    if ((RRC_SUCCESS == parse_message_map(
                    &asn1_ctx,
                    p_mme_context, 
                    &message_map, 
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_s1ap_id_Kill,
                    T_S1AP_PDU_initiatingMessage,
                    s1ap_reject,
               /* SPR 13710 Fix Start */
                    &err_ind_content)))
               /* SPR 13710 Fix Stop */
    {
        RRC_S1AP_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                "Kill response - Response Message Error Indication Ignored");

        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : compose_x2tnl_config_info_ie_extensionsnformation 
 * Inputs         : p_asn1_ctx - ASN context for memory allocation 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  
 * Outputs        : None
 * Returns        : RRC_SUCCESS - Asn Dcoding was successful.
 *                  RRC_FAILURE - Asn Dcoding was not successful.
 * Description    : This function used to encode X2TNLConfigurationInfo IE
 *                  extensions
 ****************************************************************************/

rrc_return_et compose_x2tnl_config_info_ie_extensions(OSCTXT *p_asn1_ctx,s1ap_X2TNLConfigurationInfo *p_tnl_info,s1ap_gb_context_t *p_s1ap_gb_ctx)
{
	/* coverity_fix_54701_start */
	OSRTDListNode*       p_node = NULL;
	OSRTDListNode*       p_extn_node = NULL;
	s1ap_X2TNLConfigurationInfo_iE_Extensions_element  *p_x2tnl_ie_ext_elem = NULL;
	s1ap_X2TNLConfigurationInfo_iE_Extensions *p_extnl_info = NULL;
	U32 count = 0,n = 0,count1 = 0; 
	p_extnl_info = &p_tnl_info->iE_Extensions;
	s1ap_ENBX2ExtTLA *p_enbx2_extnl_elem = NULL;
	for (count=0; count < p_tnl_info->eNBX2TransportLayerAddresses.n; ++count)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx,
				s1ap_X2TNLConfigurationInfo_iE_Extensions_element,
				&p_node,
				&p_x2tnl_ie_ext_elem);
		if (PNULL == p_node)
		{

			RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN context initialization failed.",
					__FUNCTION__);
			rtxDListFreeAll(p_asn1_ctx, p_extnl_info);
			RRC_S1AP_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
		asn1Init_s1ap_X2TNLConfigurationInfo_iE_Extensions_element(p_x2tnl_ie_ext_elem);
		p_x2tnl_ie_ext_elem->id = ASN1V_s1ap_id_eNBX2ExtendedTransportLayerAddresses; 
		p_x2tnl_ie_ext_elem->criticality = x2ap_ignore;
		if(p_s1ap_gb_ctx->x2_enb_comm_info.bitmask & X2_ENB_COMM_INFO_EXT_TLAS_PRESENT)
		{
			/* SPR 21660 changes start */
            p_x2tnl_ie_ext_elem->extensionValue.t = T271__X2TNLConfigurationInfo_ExtIEs_1;
            /* SPR 21660 changes end */
		}
		p_x2tnl_ie_ext_elem->extensionValue.u._X2TNLConfigurationInfo_ExtIEs_1 =  rtxMemAllocType(p_asn1_ctx,s1ap_ENBX2ExtTLAs);
		if (PNULL == p_x2tnl_ie_ext_elem->extensionValue.u._X2TNLConfigurationInfo_ExtIEs_1)
		{
			RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
			return RRC_FAILURE;
		}
		asn1Init_s1ap_ENBX2ExtTLAs(p_x2tnl_ie_ext_elem->extensionValue.u._X2TNLConfigurationInfo_ExtIEs_1);

		for(count1=0; count1 < p_s1ap_gb_ctx->x2_enb_comm_info.num_tlas; count1++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx,
					s1ap_ENBX2ExtTLA,
					&p_extn_node,
					&p_enbx2_extnl_elem);
			if ((NULL == p_extn_node) || (PNULL == p_enbx2_extnl_elem))
			{
				/* Not enough memory */
				RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed in st_s1ap_compose_x2TNL_config_info_iE_extensions",__FUNCTION__);
				return RRC_FAILURE;
			}

			asn1Init_s1ap_ENBX2ExtTLA (p_enbx2_extnl_elem);
			p_enbx2_extnl_elem->m.iE_ExtensionsPresent = RRC_FALSE;

			if(p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT)
			{
				p_enbx2_extnl_elem->m.iPsecTLAPresent = RRC_TRUE;
				p_enbx2_extnl_elem->iPsecTLA.numbits = p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data_length * 8;
				p_enbx2_extnl_elem->iPsecTLA.data = rtxMemAllocZ(p_asn1_ctx,p_enbx2_extnl_elem->iPsecTLA.numbits/8);
				l3_memcpy_wrapper((void *)p_enbx2_extnl_elem->iPsecTLA.data,
						&p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data,
						p_enbx2_extnl_elem->iPsecTLA.numbits/8);
			}
			if(p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].bitmask & X2_EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT)
			{
				p_enbx2_extnl_elem->m.gTPTLAaPresent = RRC_TRUE;
				p_enbx2_extnl_elem->gTPTLAa.n = p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].num_tlas;
				for(n = 0; n < p_enbx2_extnl_elem->gTPTLAa.n ; n++)
				{
					p_enbx2_extnl_elem->gTPTLAa.elem[n].numbits = p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[n].data_length * 8;
					p_enbx2_extnl_elem->gTPTLAa.elem[n].data = rtxMemAllocZ(p_asn1_ctx,p_enbx2_extnl_elem->gTPTLAa.elem[n].numbits/8);
					l3_memcpy_wrapper((void *)p_enbx2_extnl_elem->gTPTLAa.elem[n].data,
							&p_s1ap_gb_ctx->x2_enb_comm_info.extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[n].data,
							p_enbx2_extnl_elem->gTPTLAa.elem[n].numbits/8);
				}
			}
			if (0 == rtxDListAppend(p_asn1_ctx,p_x2tnl_ie_ext_elem->extensionValue.u._X2TNLConfigurationInfo_ExtIEs_1, (void*)p_enbx2_extnl_elem))
			{
				return (rrc_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
			}

		}

		if (0 == rtxDListAppend(p_asn1_ctx,p_extnl_info, (void*)p_x2tnl_ie_ext_elem))
		{
			return (rrc_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
		}

	}
	return RRC_SUCCESS;
}
/* coverity_fix_54701_stop */

/*****************************************************************************
 * Function Name  : parse_x2tnl_config_info_ie_extensions 
 * Inputs         : p_extnl_info  - Pointer to the
 *                  s1ap_X2TNLConfigurationInfo_iE_Extensions 
 * Outputs        : p_mme_config_transfer
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function used to decode X2TNLConfigurationInfo IE
 *                  extensions
 ****************************************************************************/

rrc_return_et parse_x2tnl_config_info_ie_extensions(s1ap_X2TNLConfigurationInfo_iE_Extensions *p_extnl_info,s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer)
{
    U16 count = 0;
    U16 count1 = 0;
    U16 count2 = 0;
    OSRTDListNode * p_node = PNULL;
    OSRTDListNode * p_node_ext_list = PNULL;
    RRC_ASSERT(PNULL != p_extnl_info);
    RRC_ASSERT(PNULL != p_mme_config_transfer);
    s1ap_ENBX2ExtTLAs *p_x2_tnl_config_info = PNULL;

    p_node = p_extnl_info->head;

    for(count = 0; count < p_extnl_info->count ;count++)
    {
        RRC_ASSERT(PNULL != p_node);
        RRC_ASSERT(PNULL != p_node->data);
        p_x2_tnl_config_info = ((s1ap_X2TNLConfigurationInfo_iE_Extensions_element *)(p_node->data))->extensionValue.u._X2TNLConfigurationInfo_ExtIEs_1;
        p_node_ext_list = p_x2_tnl_config_info->head;
        p_mme_config_transfer->num_tlas = p_x2_tnl_config_info->count;
        for(count1 = 0;count1 < p_mme_config_transfer->num_tlas; count1++)
        {
            if(((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->m.iPsecTLAPresent)
            {
                p_mme_config_transfer->extend_trans_layer_addrs[count1].bitmask |= EXTEND_TRANS_LAYER_ADDRS_IPSEC_ADDRS_PRESENT;
                p_mme_config_transfer->extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data_length = (((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->iPsecTLA.numbits)/8;
                memset_wrapper(p_mme_config_transfer->extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data,
                        0,sizeof(p_mme_config_transfer->extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data));
                l3_memcpy_wrapper(&p_mme_config_transfer->extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data,
                        ((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->iPsecTLA.data,
                        p_mme_config_transfer->extend_trans_layer_addrs[count1].ipsec_trans_layer_addrs.data_length);
            }
            if(((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->m.gTPTLAaPresent)
            {
                p_mme_config_transfer->extend_trans_layer_addrs[count1].num_tlas = ((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->gTPTLAa.n;
                for(count2 = 0; count2 < p_mme_config_transfer->extend_trans_layer_addrs[count1].num_tlas; count2++)
                {
                    p_mme_config_transfer->extend_trans_layer_addrs[count1].bitmask |= EXTEND_TRANS_LAYER_ADDRS_NUM_GTP_ADDRS_PRESENT;
                    p_mme_config_transfer->extend_trans_layer_addrs[count1].bitmask |= EXTEND_TRANS_LAYER_ADDRS_GTP_ADDRS_PRESENT;
                    p_mme_config_transfer->extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[count2].data_length = (((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->gTPTLAa.elem[count2].numbits)/8;
                memset_wrapper(p_mme_config_transfer->extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[count2].data,
                        0,sizeof(p_mme_config_transfer->extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[count2].data));
                    l3_memcpy_wrapper(p_mme_config_transfer->extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[count2].data,
                            ((s1ap_ENBX2ExtTLA *)(p_node_ext_list->data))->gTPTLAa.elem[count2].data,
                            p_mme_config_transfer->extend_trans_layer_addrs[count1].gtp_trans_layer_addrs[count2].data_length);
                }
            }
            p_node_ext_list = p_node_ext_list->next;
        }
        p_node = p_node->next;

    }
    return RRC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : parse_s1ap_x2tnl_configuration 
 * Inputs         : p_x2_tnl_son_config_info  - Pointer to the
 *                  p_mme_config_transfer,s1ap_SONConfigurationTransfer_iE_Extensions
 * Outputs        : p_mme_config_transfer
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : This function used to decode SON_config_transfer IE
 *                  extensions
 ****************************************************************************/

rrc_return_et parse_s1ap_x2tnl_configuration(s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,s1ap_SONConfigurationTransfer_iE_Extensions *p_x2_tnl_son_config_info)
{
    rrc_return_et result = RRC_SUCCESS;
    s1ap_SONConfigurationTransfer_iE_Extensions_element *p_x2_tnl_info_elem = PNULL;
    s1ap_X2TNLConfigurationInfo* p_x2_tnl_config_info = PNULL;
    OSRTDListNode * p_node = PNULL;
    RRC_ASSERT(PNULL != p_mme_config_transfer);
    RRC_ASSERT(PNULL != p_x2_tnl_son_config_info);

    p_node = p_x2_tnl_son_config_info->head;

    RRC_ASSERT(PNULL != p_node);

    p_x2_tnl_info_elem = ((s1ap_SONConfigurationTransfer_iE_Extensions_element *)(p_node->data));

    do 
    {
			/* SPR 21660 changes start */
        if (T233__SONConfigurationTransfer_ExtIEs_1 == p_x2_tnl_info_elem->extensionValue.t)
            /* SPR 21660 changes end */
        {
            p_x2_tnl_config_info = p_x2_tnl_info_elem->extensionValue.u._SONConfigurationTransfer_ExtIEs_1;
            if (p_x2_tnl_config_info->eNBX2TransportLayerAddresses.n == 1)
            {
                if(p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].numbits == 32)
                {
                    p_mme_config_transfer->number_of_addr = 
                        (U8)p_x2_tnl_config_info->eNBX2TransportLayerAddresses.n;
                    l3_memcpy_wrapper(
                        p_mme_config_transfer->ip_addr[0].ip_addr,
                        p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].data, 
                        RRC_MAX_IP_ADDR_LEN_IPV6/4);
                }

                else if(p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].numbits == 128)
                {
                    p_mme_config_transfer->bitmask |= 
                        MME_CONFIG_TRANSFER_X2TNL_IPv6;
                        p_mme_config_transfer->number_of_ipv6_addr = 
                            (U8)p_x2_tnl_config_info->eNBX2TransportLayerAddresses.n;
                        l3_memcpy_wrapper(
                            p_mme_config_transfer->ip_v6_addr[0].ip_addr,
                            p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].data, 
                            RRC_MAX_IP_ADDR_LEN_IPV6);

                }
                else
                {
                    RRC_S1AP_TRACE(RRC_WARNING ,"%s: Invalid IP Address",
                        __FUNCTION__);
                        result = RRC_FAILURE;
                        break;
                }
            }
            else if (p_x2_tnl_config_info->eNBX2TransportLayerAddresses.n == 2)
            {
                if(p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].numbits == 32)
                {
                    p_mme_config_transfer->number_of_addr = 1;
                    l3_memcpy_wrapper(
                        p_mme_config_transfer->ip_addr[0].ip_addr,
                        p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].data, 
                        RRC_MAX_IP_ADDR_LEN_IPV6/4);
                }

                else if(p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[0].numbits == 128)
                {
                    p_mme_config_transfer->bitmask |= 
                        MME_CONFIG_TRANSFER_X2TNL_IPv6;
                        p_mme_config_transfer->number_of_ipv6_addr = 1;
                    l3_memcpy_wrapper(
                        p_mme_config_transfer->ip_v6_addr[1].ip_addr,
                        p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[1].data, 
                        RRC_MAX_IP_ADDR_LEN_IPV6);
                }
                else
                {
                            RRC_S1AP_TRACE(RRC_WARNING ,"%s: Invalid IP Address",
                                    __FUNCTION__);
                            result = RRC_FAILURE;
                            break;
                }
                if(p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[1].numbits == 32)
                {
                    p_mme_config_transfer->number_of_addr += 1 ;
                    l3_memcpy_wrapper(
                        p_mme_config_transfer->ip_addr[1].ip_addr,
                        p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[1].data, 
                        RRC_MAX_IP_ADDR_LEN_IPV6/4);
                }

                else if(p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[1].numbits == 128)
                {
                    p_mme_config_transfer->bitmask |= 
                        MME_CONFIG_TRANSFER_X2TNL_IPv6;
                        p_mme_config_transfer->number_of_ipv6_addr += 1;
                        l3_memcpy_wrapper(
                            p_mme_config_transfer->ip_v6_addr[1].ip_addr,
                            p_x2_tnl_config_info->eNBX2TransportLayerAddresses.elem[1].data, 
                            RRC_MAX_IP_ADDR_LEN_IPV6);
                }
                else
                {
                    RRC_S1AP_TRACE(RRC_WARNING ,"%s: Invalid IP Address",
                        __FUNCTION__);
                    result = RRC_FAILURE;
                    break;
                 }
            }
            else
            {
                RRC_S1AP_TRACE(RRC_WARNING ,"%s: TNL INFO MISSING",
                    __FUNCTION__);
                result = RRC_FAILURE;
                break;
            }
            if(p_x2_tnl_config_info->m.iE_ExtensionsPresent)
            {
                p_mme_config_transfer->bitmask |= 
                    MME_CONFIG_TRANSFER_EXT_NUM_TLAS_PRESENT;
                p_mme_config_transfer->bitmask |= 
                    MME_CONFIG_TRANSFER_EXT_TLAS_PRESENT;
                if (RRC_FAILURE == parse_x2tnl_config_info_ie_extensions(&p_x2_tnl_config_info->iE_Extensions,p_mme_config_transfer))
                {
                    result = RRC_FAILURE;
                    break;
                }
            }
        }
        else
        {
            RRC_S1AP_TRACE(RRC_WARNING, "%s: "
                "x2TNLConfigurationInfo Pointer NULL",
                 __FUNCTION__);
            result = RRC_FAILURE;
            break;
        }

        
    }while (0);

    return result;
    
}

/*****************************************************************************
 * Function Name  : compose_s1ap_x2tnl_configuration 
 * Inputs         : p_asn1_ctx - ASN context for memory allocation 
 *                  p_s1ap_gb_ctx - Pointer to the S1AP global context data
 *                  p_enb_config_transfer - Pointer to the
 *                  s1ap_rrm_enb_config_transfer_t
 * Outputs        : p_x2_tnl_info - pointer to the
 *                  s1ap_SONConfigurationTransfer_iE_Extensions
 * Returns        : RRC_SUCCESS - Asn Dcoding was successful.
 *                  RRC_FAILURE - Asn Dcoding was not successful.
 * Description    : This function used to encode s1ap_X2TNLConfigurationInfo
 ****************************************************************************/

rrc_return_et compose_s1ap_x2tnl_configuration(
        OSCTXT *p_asn1_ctx,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        /*SPR 17777 +-*/
        s1ap_SONConfigurationTransfer_iE_Extensions *p_x2_tnl_info)
{
    rrc_return_et response = RRC_SUCCESS;

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_s1ap_gb_ctx);
    /*SPR 17777 +-*/
    RRC_ASSERT(PNULL != p_x2_tnl_info);

    OSRTDListNode       *p_node = PNULL;

    s1ap_SONConfigurationTransfer_iE_Extensions_element  *p_son_config_transfer_ie_ext_elem = PNULL;
    s1ap_X2TNLConfigurationInfo                          *p_son_config_transfer_x2tnl_info = PNULL;

    rtxDListAllocNodeAndData(p_asn1_ctx,
            s1ap_SONConfigurationTransfer_iE_Extensions_element,
            &p_node,
            &p_son_config_transfer_ie_ext_elem);

    if (NULL == p_node)
    {
        /* Not enough memory */
        RRC_S1AP_TRACE(RRC_ERROR, "%s:ASN malloc failed in compose_s1ap_x2tnl_configuration",__FUNCTION__);
        return RRC_FAILURE;
    }
    asn1Init_s1ap_SONConfigurationTransfer_iE_Extensions_element(p_son_config_transfer_ie_ext_elem);
    p_son_config_transfer_ie_ext_elem->id = ASN1V_s1ap_id_x2TNLConfigurationInfo; 
    p_son_config_transfer_ie_ext_elem->criticality = s1ap_ignore;

    /* SPR 21660 changes start */
    p_son_config_transfer_ie_ext_elem->extensionValue.t = T233__SONConfigurationTransfer_ExtIEs_1; 
    /* SPR 21660 changes end */
    p_son_config_transfer_ie_ext_elem->extensionValue.u._SONConfigurationTransfer_ExtIEs_1 = rtxMemAllocType(p_asn1_ctx,s1ap_X2TNLConfigurationInfo);
    if (PNULL == p_son_config_transfer_ie_ext_elem->extensionValue.u._SONConfigurationTransfer_ExtIEs_1)
    {
        RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
        return RRC_FAILURE;
    }
    p_son_config_transfer_x2tnl_info = p_son_config_transfer_ie_ext_elem->extensionValue.u._SONConfigurationTransfer_ExtIEs_1;
/* SPR_8111 Start */
    asn1Init_s1ap_X2TNLConfigurationInfo (p_son_config_transfer_ie_ext_elem->extensionValue.u._SONConfigurationTransfer_ExtIEs_1);
/* SPR_8111 Stop */
    if(!(p_s1ap_gb_ctx->x2_enb_comm_info.bitmask & ENB_COMM_INFO_IPV6_ADDR_PRESENT))
    {
        if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ip_addr == 1 )
        {
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.n = 1;
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN*8;

            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data = 
                rtxMemAllocZ(p_asn1_ctx,MAX_IP_ADDRESS_LENGTH);

            if (PNULL == p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                return RRC_FAILURE;
            }

            if (inet_pton_wrapper(AF_INET, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ip_addr[0].ip_addr, 
                        (void*)p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data) != 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert ipv4 address");
            }
        }
        else if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ip_addr <= MAX_NUM_IP_ADDR)
        {
            p_son_config_transfer_x2tnl_info->
                eNBX2TransportLayerAddresses.n =
                MAX_NUM_IP_SUPPORTED;
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN*8;
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].numbits = RRC_MAX_IP_ADDR_LEN*8;

            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data = 
                rtxMemAllocZ(p_asn1_ctx,MAX_IP_ADDRESS_LENGTH);

            if (PNULL == p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                return RRC_FAILURE;
            }
            if (inet_pton_wrapper(AF_INET,(const char *) p_s1ap_gb_ctx->x2_enb_comm_info.ip_addr[0].ip_addr, 
                        (void*)p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data) != 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert ipv4 address");

            }
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].data = 
                rtxMemAllocZ(p_asn1_ctx,MAX_IP_ADDRESS_LENGTH);

            if (PNULL == p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].data)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                return RRC_FAILURE; 
            }

            if (inet_pton_wrapper(AF_INET,(const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ip_addr[1].ip_addr, 
                        (void*)p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].data) != 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert ipv4 address");

            }
        }
        else
        {
            /* NO TNL INFO PRESENT */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ENB TNL INFO Missing",
                    __FUNCTION__);
                return RRC_FAILURE;
        }
    }
    else
    {
        if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ipv6_addr == 1 )
        {
            p_son_config_transfer_x2tnl_info->
                eNBX2TransportLayerAddresses.n = 1;
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN_IPV6*8;

            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data = 
                rtxMemAllocZ(p_asn1_ctx,MAX_IPV6_ADDRESS_LENGTH);

            if (PNULL == p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                return RRC_FAILURE;
            }
            if (inet_pton_wrapper(AF_INET6,(const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ipv6_addr[0].ipv6_addr, 
                        (void*)p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data) != 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert ipv6 address");

            }
        }
        else if (p_s1ap_gb_ctx->x2_enb_comm_info.num_ipv6_addr <= MAX_NUM_IP_ADDR)
        {
            p_son_config_transfer_x2tnl_info->
                eNBX2TransportLayerAddresses.n =
                MAX_NUM_IP_SUPPORTED;
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].numbits = RRC_MAX_IP_ADDR_LEN_IPV6*8;
            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].numbits = RRC_MAX_IP_ADDR_LEN_IPV6*8;

            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data = 
                rtxMemAllocZ(p_asn1_ctx,MAX_IPV6_ADDRESS_LENGTH);

            if (PNULL == p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                return RRC_FAILURE;
            }
            if (inet_pton_wrapper(AF_INET6,(const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ipv6_addr[0].ipv6_addr, 
                        (void*)p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[0].data) != 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert ipv6 address");

            }

            p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].data = 
                rtxMemAllocZ(p_asn1_ctx,MAX_IPV6_ADDRESS_LENGTH);

            if (PNULL == p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].data)
            {
                RRC_S1AP_TRACE(RRC_ERROR, "%s: Malloc failure",__FUNCTION__);
                return RRC_FAILURE;
            }
            if (inet_pton_wrapper(AF_INET6, (const char *)p_s1ap_gb_ctx->x2_enb_comm_info.ipv6_addr[1].ipv6_addr, 
                        (void*)p_son_config_transfer_x2tnl_info->eNBX2TransportLayerAddresses.elem[1].data) != 1)
            {
                RRC_TRACE(RRC_WARNING,"Couldn't convert ipv6 address");

            }
        }
        else
        {
            /* NO TNL INFO PRESENT */
            RRC_S1AP_TRACE(RRC_ERROR, "%s: ENB TNL INFO Missing",
                    __FUNCTION__);
                return RRC_FAILURE;
        }

    }
    if(p_s1ap_gb_ctx->x2_enb_comm_info.bitmask & X2_ENB_COMM_INFO_EXT_NUM_TLAS_PRESENT)
    {
        p_son_config_transfer_x2tnl_info->m.iE_ExtensionsPresent = RRC_TRUE;
        if(RRC_NULL == compose_x2tnl_config_info_ie_extensions(p_asn1_ctx,p_son_config_transfer_x2tnl_info,p_s1ap_gb_ctx))
        {
            RRC_S1AP_TRACE(RRC_ERROR, "%s: Composing failed for extended tnl info",
                    __FUNCTION__);
            response = RRC_FAILURE;
        }
    }
    if (0 == rtxDListAppend(p_asn1_ctx,p_x2_tnl_info, (void*)p_son_config_transfer_ie_ext_elem))
    {
        return (rrc_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
    }

    return response;
}

/*****************************************************************************
* Function Name  : s1ap_decode_overload_gummei_list
* Inputs         : asn_overload_response - ASN Encoded value
* Outputs        : p_gummei_list
* Returns        : num_gummei - no of gummei's present 
*                  in the gummmei_list
* Description    : This function is used to decode GUMMEIList
*****************************************************************************/

U32 s1ap_decode_overload_gummei_list(s1ap_gummei_t *p_gummei_list,s1ap_GUMMEIList *p_s1ap_gummei_list)
{

    U8 count = 0;
    U8 num_gummei = 0;
    OSRTDListNode * p_node = PNULL;
    RRC_ASSERT(PNULL != p_gummei_list);
    RRC_ASSERT(PNULL != p_s1ap_gummei_list);


    p_node = p_s1ap_gummei_list->head;

    num_gummei = p_s1ap_gummei_list->count; 

    for(count = 0;count < num_gummei;count++)
    {
        p_gummei_list[count].plmn_identity = ((s1ap_GUMMEI *)(p_node->data))->pLMN_Identity;
        p_gummei_list[count].grp_id = ((s1ap_GUMMEI *)(p_node->data))->mME_Group_ID;
        p_gummei_list[count].mme_code = ((s1ap_GUMMEI *)(p_node->data))->mME_Code;
        p_node = p_node->next;
    }
    return num_gummei;

}

