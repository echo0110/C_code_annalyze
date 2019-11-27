/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id:
 ****************************************************************************
 *
 *  File Description :
 *   This file contains decode functions that convert
 *   S1AP ASN.1 encoded messages into ASN.1 based internal representation.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log:
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "rrc_common_utils.h"
#include "rrc_s1ap_uecc_intf.h"
#include "uecc_ue_ctx.h"
#include "uecc_common.h"
#include "uecc_logger.h"
#include "rrc_logging.h"


/****************************************************************************
 * Private Definitions
 ****************************************************************************/

#define IE_NOT_PRESENT 0x7fffffff
#define NORMAL_IE_LEN 100
#define ASN_INTRL_DEC_COPY(\
        copy_func_par,var_3gpp_par,dest_par,ie_id_par,criticality,p_iE_list)\
{\
    if((p_protocol_IE->value.u.var_3gpp_par == IE_NOT_PRESENT) ||\
            (p_protocol_IE->value.u.var_3gpp_par == PNULL))\
    {\
        RRC_TRACE(RRC_INFO,"Abstract Syntax Error in IE");\
        result.u.protocol = s1ap_transfer_syntax_error;\
        break;\
        if(criticality == s1ap_reject)\
        {\
            result.u.protocol = s1ap_abstract_syntax_error_reject;\
            if(p_iE_list != PNULL)\
            {\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iECriticality = s1ap_reject;\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iE_ID = ie_id_par;\
                p_iE_list->iE_list[p_iE_list->ie_cnt++].typeOfError = s1ap_missing;\
                break;\
            }\
        }\
        else if(criticality == s1ap_notify)\
        {  \
            result.u.protocol = s1ap_abstract_syntax_error_ignore_and_notify;\
            if(p_iE_list != PNULL)\
            {\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iECriticality = s1ap_notify;\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iE_ID = ie_id_par;\
                p_iE_list->iE_list[p_iE_list->ie_cnt++].typeOfError = s1ap_missing;\
                break;\
            }\
        }\
    }\
    if(PNULL!=pctxt)\
    {\
        copy_func_par(pctxt,\
                p_protocol_IE->value.u.var_3gpp_par,\
                dest_par);\
    }\
    else\
    {\
        *(dest_par)=\
        *(p_protocol_IE->value.u.var_3gpp_par);\
    }\
}

#define ASN_INTRL_DEC_COPY_VAL(\
        copy_func_par,var_3gpp_par,dest_par,ie_id_par,criticality,p_iE_list)\
{\
    if(p_protocol_IE->value.u.var_3gpp_par == IE_NOT_PRESENT) \
    {\
        RRC_TRACE(RRC_INFO,"Abstract Syntax Error in IE");\
        result.u.protocol = s1ap_transfer_syntax_error;\
        break;\
        if(criticality == s1ap_reject)\
        {\
            result.u.protocol = s1ap_abstract_syntax_error_reject;\
            if(p_iE_list != PNULL)\
            {\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iECriticality = s1ap_reject;\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iE_ID = ie_id_par;\
                p_iE_list->iE_list[p_iE_list->ie_cnt++].typeOfError = s1ap_missing;\
                break;\
            }\
        }\
        else if(criticality == s1ap_notify)\
        {  \
            result.u.protocol = s1ap_abstract_syntax_error_ignore_and_notify;\
            if(p_iE_list != PNULL)\
            {\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iECriticality = s1ap_notify;\
                p_iE_list->iE_list[p_iE_list->ie_cnt].iE_ID = ie_id_par;\
                p_iE_list->iE_list[p_iE_list->ie_cnt++].typeOfError = s1ap_missing;\
                break;\
            }\
        }\
    }\
    if(PNULL!=pctxt)\
    {\
        copy_func_par(pctxt,\
                p_protocol_IE->value.u.var_3gpp_par,\
                &dest_par);\
    }\
    else\
    {\
        (dest_par)=(p_protocol_IE->value.u.var_3gpp_par);\
    }\
}


/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef struct
{
    OSRTDList* p_list;
    OSRTDListNode* p_node;
} asn_intrl_dec_iterator_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions Implementation
 ****************************************************************************/
/****************************************************************************
 * Function Name  : s1ap_process_MME_UE_S1AP_ID
 * Inputs         : p_asn_mme_ue_s1ap_id
 * Outputs        : p_intrl_mme_ue_s1ap_id
 * Returns        : None
 * Description    : This function processes s1ap_MME_UE_S1AP_ID IE
 ****************************************************************************/
static void s1ap_process_MME_UE_S1AP_ID (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_MME_UE_S1AP_ID         *p_asn_mme_ue_s1ap_id,
    s1ap_MME_UE_S1AP_ID         *p_intrl_mme_ue_s1ap_id,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_MME_UE_S1AP_ID,
                p_asn_mme_ue_s1ap_id,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_mme_ue_s1ap_id = *p_asn_mme_ue_s1ap_id;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_eNB_UE_S1AP_ID
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_ENB_UE_S1AP_ID IE
 ****************************************************************************/
static void s1ap_process_eNB_UE_S1AP_ID (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_ENB_UE_S1AP_ID         *p_asn_enb_ue_s1ap_id,
    s1ap_ENB_UE_S1AP_ID         *p_intrl_enb_ue_s1ap_id,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value( p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_eNB_UE_S1AP_ID,
                p_asn_enb_ue_s1ap_id,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_enb_ue_s1ap_id = *p_asn_enb_ue_s1ap_id;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_uEaggregateMaximumBitrate
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function processes s1ap_UEAggregateMaximumBitrate IE
 ****************************************************************************/
static void s1ap_process_uEaggregateMaximumBitrate (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_UEAggregateMaximumBitrate *p_asn_ie,
    s1ap_UEAggregateMaximumBitrate *p_intrl_ie,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_uEaggregateMaximumBitrate,
                p_asn_ie,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_ie = *p_asn_ie;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_E_RABToBeSetupListCtxtSUReq
 * Inputs         : 
 * Outputs        : p_intrl_ie
 * Returns        : None
 * Description    : This function processes s1ap_E_RABToBeSetupListCtxtSUReq
 ****************************************************************************/
static void s1ap_process_E_RABToBeSetupListCtxtSUReq (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_E_RABToBeSetupListCtxtSUReq *p_asn_ie,
    s1ap_E_RABToBeSetupListCtxtSUReq *p_intrl_ie,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_E_RABToBeSetupListCtxtSUReq,
                p_asn_ie,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_ie = *p_asn_ie;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_UESecurityCapabilities
 * Inputs         : 
 * Outputs        : p_intrl_ie
 * Returns        : None
 * Description    : This function processes s1ap_UESecurityCapabilities IE
 ****************************************************************************/
static void s1ap_process_UESecurityCapabilities (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_UESecurityCapabilities *p_asn_ie,
    s1ap_UESecurityCapabilities *p_intrl_ie,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_UESecurityCapabilities,
                p_asn_ie,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_ie = *p_asn_ie;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_SecurityKey
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_SecurityKey IE
 ****************************************************************************/
static void s1ap_process_SecurityKey (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_SecurityKey            *p_asn_ie,
    s1ap_SecurityKey            *p_intrl_ie,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_SecurityKey,
                p_asn_ie,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_ie = *p_asn_ie;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_TraceActivation
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_TraceActivation IE
 ****************************************************************************/
static void s1ap_process_TraceActivation (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_TraceActivation        *p_asn_ie,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_TraceActivation,
                p_asn_ie,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->trace_activation = *p_asn_ie;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_TRACE_ACTIVATION_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ics_HandoverRestrictionList
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_HandoverRestrictionList IE
 ****************************************************************************/
static void s1ap_process_ics_HandoverRestrictionList (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_HandoverRestrictionList,
                p_protocol_IE->value.u._InitialContextSetupRequestIEs_8,
                (uecc_ue_context_t *)PNULL))
    {
        *(&p_intrl->handover_restriction_list) = 
            *(p_protocol_IE->value.u._InitialContextSetupRequestIEs_8);
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_HANDOVER_RESTRICTION_LIST_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_UERadioCapability
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_UERadioCapability IE
 ****************************************************************************/
static void s1ap_process_UERadioCapability (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
   /*SPR_17893_START*/
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
   /*SPR_17893_END*/
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_UERadioCapability,
                p_protocol_IE->value.u._InitialContextSetupRequestIEs_9,
                (uecc_ue_context_t *)PNULL))
    {
        /*SPR_17893_START*/
        p_intrl->ue_radio_capability = 
            *p_protocol_IE->value.u._InitialContextSetupRequestIEs_9;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_UE_RADIO_CAPABILITY_PRESENT;
        /*SPR_17893_END*/
    }
    /* SPR 16062 Stop */
}

/****************************************************************************
 * Function Name  : s1ap_process_SubscriberProfileIDforRFP
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_SubscriberProfileIDforRFP IE
 ****************************************************************************/
static void s1ap_process_SubscriberProfileIDforRFP (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_SubscriberProfileIDforRFP,
                &p_protocol_IE->value.u._InitialContextSetupRequestIEs_10,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->subscriber_profile_id_for_rfp = 
            p_protocol_IE->value.u._InitialContextSetupRequestIEs_10;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_CSFallbackIndicator
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_CSFallbackIndicator IE
 ****************************************************************************/
static void s1ap_process_CSFallbackIndicator (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_CSFallbackIndicator,
                &p_protocol_IE->value.u._InitialContextSetupRequestIEs_11,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->cs_fallback_indicator = 
            p_protocol_IE->value.u._InitialContextSetupRequestIEs_11;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CS_FALLBACK_INDICATOR_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_SRVCCOperationPossible
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_SRVCCOperationPossible IE
 ****************************************************************************/
static void s1ap_process_SRVCCOperationPossible (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_SRVCCOperationPossible,
                &p_protocol_IE->value.u._InitialContextSetupRequestIEs_12,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->srvcc_operation_possible =
            p_protocol_IE->value.u._InitialContextSetupRequestIEs_12;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_CSGMembershipStatus
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_CSGMembershipStatus IE
 ****************************************************************************/
static void s1ap_process_CSGMembershipStatus (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_CSGMembershipStatus,
                &p_protocol_IE->value.u._InitialContextSetupRequestIEs_13,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->csg_membership_status =
            p_protocol_IE->value.u._InitialContextSetupRequestIEs_13;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_RegisteredLAI
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_RegisteredLAI IE
 ****************************************************************************/
static void s1ap_process_RegisteredLAI (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_RegisteredLAI,
                p_protocol_IE->value.u._InitialContextSetupRequestIEs_14,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->registered_lai = 
            *p_protocol_IE->value.u._InitialContextSetupRequestIEs_14;
        p_intrl->bitmask |=
            RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_REGISTERED_LAI_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_GUMMEI_ID
 * Inputs         : 
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function processes s1ap_GUMMEI_ID IE
 ****************************************************************************/
static void s1ap_process_GUMMEI_ID (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_GUMMEI                 *p_asn_IE,
    s1ap_GUMMEI                 *p_gummei,    
    rrc_bitmask_t               *p_bitmask,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_GUMMEI_ID,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        *p_gummei = *p_asn_IE;
        *p_bitmask |= RRC_S1AP_HANDOVER_REQUEST_GUMMEI_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_MME_UE_S1AP_ID_2
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes MME_UE_S1AP_ID_2
 ****************************************************************************/
static void s1ap_process_MME_UE_S1AP_ID_2 (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_MME_UE_S1AP_ID         *p_asn_IE,
    s1ap_MME_UE_S1AP_ID         *p_mme_ue_s1ap_id2,
    rrc_bitmask_t               *p_bitmask,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_MME_UE_S1AP_ID_2,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
         *p_mme_ue_s1ap_id2 = *p_asn_IE;
         *p_bitmask |= RRC_S1AP_HANDOVER_REQUEST_MME_UE_S1AP_ID2_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_NRUESecurityCapabilities
 * Inputs		  : 
 * Outputs		  : 
 * Returns		  : None
 * Description	  : This function processes NRUESecurityCapabilities
 ****************************************************************************/
static void s1ap_process_NRUESecurityCapabilities (
	uecc_gb_context_t			*p_gb_context,
	uecc_s1ap_message_data_t	*p_message_map,
	s1ap_NRUESecurityCapabilities 		*p_asn_IE,
	rrc_s1ap_Nr_Security_Cap_t 		*p_Nr_Security_Cap,
	rrc_bitmask_t				*p_bitmask,
	rrc_bitmask_t				bitmask,
	U32  index)
{
	if (RRC_SUCCESS == uecc_validate_ie_value(
				p_gb_context,
				p_message_map,
				index,
				ASN1V_s1ap_id_NRUESecurityCapabilities,
				p_asn_IE,
				(uecc_ue_context_t *)PNULL))
	{
		*p_bitmask |= bitmask;
		/*default value*/
		p_Nr_Security_Cap->ue_NR_Encryption_Algo = UECC_CIPHERING_NEA0_BIT;
		p_Nr_Security_Cap->ue_NR_Integrity_Protection_Algo = UECC_INTEGRITY_PROTECTION_NIA0_BIT;
		
		if (0 !=  rtxTestBit(p_asn_IE->nRencryptionAlgorithms.data,
                    p_asn_IE->nRencryptionAlgorithms.numbits,
                    0))
		{
			p_Nr_Security_Cap->ue_NR_Encryption_Algo |= UECC_CIPHERING_NEA1_BIT;
        }

		if (0 !=  rtxTestBit(p_asn_IE->nRencryptionAlgorithms.data,
					p_asn_IE->nRencryptionAlgorithms.numbits,
					1))
		{
			p_Nr_Security_Cap->ue_NR_Encryption_Algo |= UECC_CIPHERING_NEA2_BIT;
		}

		if (0 !=  rtxTestBit(p_asn_IE->nRencryptionAlgorithms.data,
					p_asn_IE->nRencryptionAlgorithms.numbits,
					2))
		{
			p_Nr_Security_Cap->ue_NR_Encryption_Algo |= UECC_CIPHERING_NEA3_BIT;
		}

		if (0 !=  rtxTestBit(p_asn_IE->nRintegrityProtectionAlgorithms.data,
					p_asn_IE->nRintegrityProtectionAlgorithms.numbits,
					0))
		{
			p_Nr_Security_Cap->ue_NR_Integrity_Protection_Algo |= UECC_INTEGRITY_PROTECTION_NIA1_BIT;
		}		

		if (0 !=  rtxTestBit(p_asn_IE->nRintegrityProtectionAlgorithms.data,
					p_asn_IE->nRintegrityProtectionAlgorithms.numbits,
					1))
		{
			p_Nr_Security_Cap->ue_NR_Integrity_Protection_Algo |= UECC_INTEGRITY_PROTECTION_NIA2_BIT;
		}	

		if (0 !=  rtxTestBit(p_asn_IE->nRintegrityProtectionAlgorithms.data,
					p_asn_IE->nRintegrityProtectionAlgorithms.numbits,
					2))
		{
			p_Nr_Security_Cap->ue_NR_Integrity_Protection_Algo |= UECC_INTEGRITY_PROTECTION_NIA3_BIT;
		}						
	}
}


/****************************************************************************
 * Function Name  : s1ap_process_ManagementBasedMDTAllowed
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_ManagementBasedMDTAllowed
 ****************************************************************************/
static void s1ap_process_ManagementBasedMDTAllowed (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_ManagementBasedMDTAllowed *p_asn_IE,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_ManagementBasedMDTAllowed,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        RRC_TRACE(RRC_DETAILED,"%s: "
                "ASN1V_s1ap_id_ManagementBasedMDTAllowed  - NOT HANDLED",__FUNCTION__);
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_HandoverType
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_HandoverType
 ****************************************************************************/
static void s1ap_process_HandoverType (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_HandoverType           *p_asn_IE,
    s1ap_HandoverType           *p_intrl_IE,
    U32  index)
{
    U16  count = 0;
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_HandoverType,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_IE = *p_asn_IE;

        /* Change the presence of the conditional parameter */
        if ((s1ap_utrantolte == *p_intrl_IE) || 
                (s1ap_gerantolte == *p_intrl_IE))
        {
            for (count = 0; count < p_message_map->max_count; count++)
            {
                if (ASN1V_s1ap_id_HandoverType == 
                        p_message_map->msg_map[count].ie_id)
                {
                    p_message_map->msg_map[count].presence = 
                        s1ap_mandatory;
                    break;
                }
            }
        }
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_Cause
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_Cause
 ****************************************************************************/
static void s1ap_process_Cause (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_Cause           *p_asn_IE,
    s1ap_Cause           *p_intrl_IE,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_Cause,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_IE = *p_asn_IE;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_E_RABToBeSetupListHOReq
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_E_RABToBeSetupListHOReq IE
 ****************************************************************************/
static void s1ap_process_E_RABToBeSetupListHOReq (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_E_RABToBeSetupListHOReq  *p_asn_IE,
    s1ap_E_RABToBeSetupListHOReq  *p_intrl_IE,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_E_RABToBeSetupListHOReq,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_IE = *p_asn_IE;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_Source_ToTarget_TransparentContainer
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes
 *                  s1ap_Source_ToTarget_TransparentContainer
 ****************************************************************************/
static void s1ap_process_Source_ToTarget_TransparentContainer (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_Source_ToTarget_TransparentContainer *p_asn_IE,
    s1ap_Source_ToTarget_TransparentContainer *p_transparent_container,
    rrc_source_to_target_transparent_container_t *p_intrl_IE,
    uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
    s1ap_Cause *p_result,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_Source_ToTarget_TransparentContainer,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        *p_transparent_container = *p_asn_IE;

        /* Decode SourceeNB to Target eNB Transparent Container */
        rrc_s1ap_source_enb_to_target_enb_transparent_container_intrl_dec(
                p_transparent_container,
                p_container_asn1_ctxt_arr,
                p_intrl_IE,
                p_result);
    }

    if (S1AP_CAUSE_SUCCESS != p_result->t)
    {
        /* Message parsing error */
        RRC_UECC_TRACE(p_gb_context->context_index,
                p_gb_context->facility_name,
                RRC_WARNING,
                "[S1AP Handover Request ] Parsing error");

        uecc_update_message_map(p_gb_context,
                p_message_map, 
                UECC_INVALID_VALUE, 
                index, 
                ASN1V_s1ap_id_Source_ToTarget_TransparentContainer);
        /* Drop. */
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_request_HandoverRestrictionList
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_HandoverRestrictionList IE
 ****************************************************************************/
static void s1ap_process_ho_request_HandoverRestrictionList (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_HandoverRestrictionList *p_asn_IE,
    rrc_s1ap_handover_request_t  *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_HandoverRestrictionList,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->ho_restriction_list = *p_asn_IE;

        p_intrl->bitmask |=
            RRC_S1AP_HANDOVER_REQUEST_HO_RESTRICTION_LIST_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_request_TraceActivation
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_TraceActivation IE
 ****************************************************************************/
static void s1ap_process_ho_request_TraceActivation (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_TraceActivation        *p_asn_IE,
    rrc_s1ap_handover_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_TraceActivation,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->trace_activation = *p_asn_IE;
        p_intrl->bitmask |=RRC_S1AP_HANDOVER_REQUEST_TRACE_ACTIVATION_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_request_RequestType
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_RequestType IE
 ****************************************************************************/
static void s1ap_process_ho_request_RequestType (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_RequestType            *p_asn_IE,
    rrc_s1ap_handover_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_RequestType,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->request_type = *p_asn_IE;
        p_intrl->bitmask |=
            RRC_S1AP_HANDOVER_REQUEST_REQUEST_TYPE_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_request_SRVCCOperationPossible
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_SRVCCOperationPossible IE
 ****************************************************************************/
static void s1ap_process_ho_request_SRVCCOperationPossible (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_SRVCCOperationPossible *p_asn_IE,
    rrc_s1ap_handover_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_SRVCCOperationPossible,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->srvcc_operation_possible = *p_asn_IE;
        p_intrl->bitmask |=
            RRC_S1AP_HANDOVER_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_SecurityContext
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_SecurityContext IE
 ****************************************************************************/
static void s1ap_process_SecurityContext (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_SecurityContext *p_asn_IE,
    s1ap_SecurityContext *p_intrl_IE,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_SecurityContext,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        *p_intrl_IE = *p_asn_IE;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_request_NASSecurityParameterstoE_UTRAN
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_NASSecurityParameterstoE_UTRAN
 ****************************************************************************/
static void s1ap_process_ho_request_NASSecurityParameterstoE_UTRAN (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_NASSecurityParameterstoE_UTRAN *p_asn_IE,
    rrc_s1ap_handover_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_NASSecurityParameterstoE_UTRAN,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->nas_security_parameters_to_e_utran = *p_asn_IE;
        p_intrl->bitmask |=
            RRC_S1AP_HANDOVER_REQUEST_NAS_SECURITY_PARAMETERS_TO_E_UTRAN_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_request_CSG_Id
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_CSG_Id IE
 ****************************************************************************/
static void s1ap_process_ho_request_CSG_Id (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_CSG_Id *p_asn_IE,
    rrc_s1ap_handover_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_CSG_Id,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->csg_id = *p_asn_IE;
        p_intrl->bitmask |=
            RRC_S1AP_HANDOVER_REQUEST_CSG_ID_PRESENT;
    }
}

/****************************************************************************
 * Function Name  : s1ap_process_ho_reques_CSGMembershipStatus
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : This function processes s1ap_CSGMembershipStatus IE
 ****************************************************************************/
static void s1ap_process_ho_request_CSGMembershipStatus (
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    s1ap_CSGMembershipStatus    *p_asn_IE,
    rrc_s1ap_handover_request_t *p_intrl,
    U32  index)
{
    if (RRC_SUCCESS == uecc_validate_ie_value(
                p_gb_context,
                p_message_map,
                index,
                ASN1V_s1ap_id_CSGMembershipStatus,
                p_asn_IE,
                (uecc_ue_context_t *)PNULL))
    {
        p_intrl->csg_membership_status = *p_asn_IE;
        p_intrl->bitmask |=
            RRC_S1AP_HANDOVER_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT;
    }
}

/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/****************************************************************************
 * Function Name  : rrc_s1ap_pdu_intrl_dec
 * Inputs         : uecc_gb_context_t           *p_uecc_gb_context
 *                  OSCTXT      *pctxt
 *                  U8          *p_buff
 *                  U16         buff_size
 *                  S1AP_PDU    *p_s1ap_pdu
 * Outputs        : p_s1ap_pdu (unpacked S1AP_PDU)
 * Returns        : s1ap_Cause
 * Description    : unpacks S1AP_PDU from ASN
 ****************************************************************************/
rrc_return_et rrc_s1ap_pdu_intrl_dec
(
 uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         buff_size,      /* encoded buf size - just for check*/
 S1AP_PDU    *p_s1ap_pdu     /* unpacked */
 )
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_s1ap_pdu);

    do
    {
        pu_setBuffer(pctxt,(OSOCTET *)p_buff,buff_size,ASN_S1AP_BUF_TYPE);

        if (RT_OK != asn1PD_S1AP_PDU(pctxt,p_s1ap_pdu))
        {
            /* Drop message */
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            result = RRC_FAILURE;
            break;
        }
    } while (0);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* ERAB SETUP Start */

/****************************************************************************
 * Function Name  : rrc_s1ap_erab_setup_request_intrl_dec
 * Inputs         : U8                      mme_id
 *                  s1ap_E_RABSetupRequest *p_3gpp
 *                  rrc_s1ap_erab_setup_request_t *p_intrl
 *                  uecc_gb_context_t   *p_uecc_gb_context
 * Outputs        : p_intrl (internal rrc_s1ap_erab_setup_request_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_erab_setup_request_t from
 *                  s1ap_E_RABSetupRequest
 ****************************************************************************/

rrc_return_et rrc_s1ap_erab_setup_request_intrl_dec
(
 U8                      mme_id,
 s1ap_E_RABSetupRequest *p_3gpp,    /* unpacked src */
 rrc_s1ap_erab_setup_request_t *p_intrl, /* dest */
 uecc_gb_context_t   *p_uecc_gb_context
 )
{
    OSRTDListNode* p_node = PNULL;
    s1ap_E_RABSetupRequest_protocolIEs_element 
        *p_protocol_IE = PNULL;    

    U32  index = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;
    

    uecc_s1ap_message_data_t message_map = 
    {4, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_E_RABToBeSetupListBearerSUReq, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_E_RABSetupRequest_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABSetupRequestIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._E_RABSetupRequestIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABSetupRequestIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id= 
                                p_protocol_IE->value.u._E_RABSetupRequestIEs_2;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABSetupRequestIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->ue_aggregate_maximum_bitrate = 
                                *(p_protocol_IE->value.u._E_RABSetupRequestIEs_3);

                            p_intrl->bitmask |=
                                RRC_S1AP_ERAB_SETUP_REQUEST_UE_AGGREGATE_MAXIMUM_BITRATE_PRESENT;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_E_RABToBeSetupListBearerSUReq:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABSetupRequestIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->e_rab_to_be_setup_list= 
                                *(p_protocol_IE->value.u._E_RABSetupRequestIEs_4);
                        }

                    }
                    break;

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(
                            p_uecc_gb_context,
                            &iE_list, 
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
    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_E_RABSetup,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " ERAB Setup Request");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;

}

/****************************************************************************
 * Function Name  : s1ap_ics_process_protocolIE_id
 * Inputs         : p_protocol_IE
 *                  p_gb_context
 *                  index            
 * Outputs        : p_intrl (rrc_s1ap_initial_context_setup_request_t )
 *                  p_message_map
 *                  p_iE_list
 *                  p_ie_list_index
 *                  p_send_err_ind
 * Returns        : 
 * Description    : This function processes protocol IE id and fill the intrl
 *                  message, for s1ap InitialContextSetup msg.
 ****************************************************************************/
static void s1ap_ics_process_protocolIE_id(
    s1ap_InitialContextSetupRequest_protocolIEs_element *p_protocol_IE,
    uecc_gb_context_t           *p_gb_context,
    uecc_s1ap_message_data_t    *p_message_map,
    U32  index,
    rrc_s1ap_initial_context_setup_request_t *p_intrl,
    s1ap_error_ind_ie_list_t *p_iE_list,
    U16 *p_ie_list_index,
    uecc_s1ap_error_ind_bool_t *p_send_err_ind)
{
    RRC_UT_TRACE_ENTER();
    switch (p_protocol_IE->id)
    {
        case ASN1V_s1ap_id_MME_UE_S1AP_ID:
            s1ap_process_MME_UE_S1AP_ID(
                    p_gb_context, p_message_map, 
                    &p_protocol_IE->value.u._InitialContextSetupRequestIEs_1,
                    &p_intrl->mme_ue_s1ap_id, index);
            break;

        case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
            s1ap_process_eNB_UE_S1AP_ID(
                    p_gb_context, p_message_map,
                    &p_protocol_IE->value.u._InitialContextSetupRequestIEs_2,
                    &p_intrl->enb_ue_s1ap_id, index);
            break;

        case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
            s1ap_process_uEaggregateMaximumBitrate(
                    p_gb_context, p_message_map,
                    p_protocol_IE->value.u._InitialContextSetupRequestIEs_3,
                    &p_intrl->ue_aggregate_maximum_bitrate, index);
            break;

        case ASN1V_s1ap_id_E_RABToBeSetupListCtxtSUReq:
            s1ap_process_E_RABToBeSetupListCtxtSUReq(
                    p_gb_context, p_message_map,
                    p_protocol_IE->value.u._InitialContextSetupRequestIEs_4,
                    &p_intrl->e_rab_to_be_setup_list, index);
            break;

        case ASN1V_s1ap_id_UESecurityCapabilities:
            s1ap_process_UESecurityCapabilities(
                    p_gb_context, p_message_map,
                    p_protocol_IE->value.u._InitialContextSetupRequestIEs_5,
                    &p_intrl->ue_security_capabilities, index);
            break;

        case ASN1V_s1ap_id_SecurityKey:
            s1ap_process_SecurityKey(
                    p_gb_context, p_message_map,
                    p_protocol_IE->value.u._InitialContextSetupRequestIEs_6,
                    &p_intrl->security_key, index);
            break;

        case ASN1V_s1ap_id_TraceActivation:
            s1ap_process_TraceActivation(p_gb_context, p_message_map,
                    p_protocol_IE->value.u._InitialContextSetupRequestIEs_7,
                    p_intrl, index);
            break;

        case ASN1V_s1ap_id_HandoverRestrictionList:
            s1ap_process_ics_HandoverRestrictionList(
                    p_gb_context, p_message_map,
                    p_protocol_IE, p_intrl, index);
            break;

        case ASN1V_s1ap_id_UERadioCapability:
            s1ap_process_UERadioCapability( p_gb_context, p_message_map,
                    /*SPR_17893_START*/
                    p_protocol_IE, p_intrl, index);
                  /*SPR_17893_END*/
            break;

        case ASN1V_s1ap_id_SubscriberProfileIDforRFP:
            s1ap_process_SubscriberProfileIDforRFP(
                    p_gb_context, p_message_map,
                    p_protocol_IE, p_intrl, index);
            break;

        case ASN1V_s1ap_id_CSFallbackIndicator:
            s1ap_process_CSFallbackIndicator( p_gb_context, p_message_map,
                        p_protocol_IE, p_intrl, index);
            break;

        case ASN1V_s1ap_id_SRVCCOperationPossible:
            s1ap_process_SRVCCOperationPossible(
                    p_gb_context, p_message_map,
                    p_protocol_IE, p_intrl, index);
            break;

        case ASN1V_s1ap_id_CSGMembershipStatus:
            s1ap_process_CSGMembershipStatus(p_gb_context, p_message_map,
                        p_protocol_IE, p_intrl, index);
            break;

            /* CR_761_Start */
        case ASN1V_s1ap_id_RegisteredLAI:
            s1ap_process_RegisteredLAI(p_gb_context, p_message_map,
                        p_protocol_IE, p_intrl, index);
            break;
            /* CR_761_Stop */

        case ASN1V_s1ap_id_GUMMEI_ID : 
            s1ap_process_GUMMEI_ID(p_gb_context, p_message_map,
                    p_protocol_IE->value.u._InitialContextSetupRequestIEs_15,
                    &p_intrl->gummei, &p_intrl->bitmask, index);
            break;

        case ASN1V_s1ap_id_MME_UE_S1AP_ID_2 : 
            s1ap_process_MME_UE_S1AP_ID_2(
                    p_gb_context, p_message_map,
                    &p_protocol_IE->value.u._InitialContextSetupRequestIEs_16,
                    &p_intrl->mme_ue_s1ap_id2, &p_intrl->bitmask, index);
            break;

        case ASN1V_s1ap_id_ManagementBasedMDTAllowed : 
            s1ap_process_ManagementBasedMDTAllowed(
                    p_gb_context, p_message_map,
                    &p_protocol_IE->value.u._InitialContextSetupRequestIEs_17,
                    index);
            break;

		case ASN1V_s1ap_id_NRUESecurityCapabilities : 
			s1ap_process_NRUESecurityCapabilities(
					p_gb_context, p_message_map,
					p_protocol_IE->value.u._InitialContextSetupRequestIEs_27,
					&p_intrl->nr_security_cap, &p_intrl->bitmask,
					RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_NR_SECURITY_CAP_PRESENT,index);
			break;

        default:
            uecc_add_to_err_ind_ie_list(
                    p_gb_context,
                    p_iE_list,
                    p_protocol_IE->criticality,
                    p_protocol_IE->id,
                    p_ie_list_index,
                    p_send_err_ind,
                    RRC_FALSE,
                    RRC_FALSE);
    }
    RRC_UT_TRACE_EXIT();
}

/* ERAB SETUP Stop */
/****************************************************************************
 * Function Name  : rrc_s1ap_initial_context_setup_request_intrl_dec
 * Inputs         : OSCTXT *p_asn1_ctx,
 *                  U8 mme_id
 *                  s1ap_InitialContextSetupRequest *p_3gpp
 *                  rrc_s1ap_initial_context_setup_request_t *p_intrl
 *                  rrc_s1ap_error_indication_t *p_err_ind_content
 *                  uecc_s1ap_error_ind_bool_t  *p_send_err_ind
 *                  uecc_gb_context_t           *p_gb_context
 * Outputs        : p_intrl (internal rrc_s1ap_initial_context_setup_request_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_initial_context_setup_request_t from
 *                  s1ap_InitialContextSetupRequest
 ****************************************************************************/
rrc_return_et rrc_s1ap_initial_context_setup_request_intrl_dec
(
 OSCTXT *p_asn1_ctx,
 U8 mme_id,
 s1ap_InitialContextSetupRequest *p_3gpp,    /* unpacked src */
 rrc_s1ap_initial_context_setup_request_t *p_intrl,/* dest */
 rrc_s1ap_error_indication_t *p_err_ind_content,
 uecc_s1ap_error_ind_bool_t  *p_send_err_ind,
 uecc_gb_context_t           *p_gb_context
 )
{
    rrc_return_et result = RRC_SUCCESS;
    U32  index = RRC_NULL;

    OSRTDListNode  *p_node     = PNULL;
    s1ap_InitialContextSetupRequest_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U16 ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;

    //CR_761 : adding LAI

    uecc_s1ap_message_data_t message_map = 
    {17, RRC_TRUE, RRC_TRUE, 0, 0, 0,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_E_RABToBeSetupListCtxtSUReq, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4, ASN1V_s1ap_id_UESecurityCapabilities, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {5, ASN1V_s1ap_id_SecurityKey, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {6, ASN1V_s1ap_id_TraceActivation, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {7, ASN1V_s1ap_id_HandoverRestrictionList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {8, ASN1V_s1ap_id_UERadioCapability, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {9, ASN1V_s1ap_id_SubscriberProfileIDforRFP, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {10, ASN1V_s1ap_id_CSFallbackIndicator, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {11, ASN1V_s1ap_id_SRVCCOperationPossible, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {12, ASN1V_s1ap_id_CSGMembershipStatus, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {13, ASN1V_s1ap_id_RegisteredLAI, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {14, ASN1V_s1ap_id_GUMMEI_ID, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {15, ASN1V_s1ap_id_MME_UE_S1AP_ID_2, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {16, ASN1V_s1ap_id_ManagementBasedMDTAllowed, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {17, ASN1V_s1ap_id_NRUESecurityCapabilities, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}
        }
    };

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));

    p_intrl->bitmask = 0;

    do
    {

        p_node = p_3gpp->protocolIEs.head;

        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_gb_context->context_index,
                        p_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);

                break;
            }

            p_protocol_IE =
                (s1ap_InitialContextSetupRequest_protocolIEs_element *)
                p_node->data;

            s1ap_ics_process_protocolIE_id(p_protocol_IE, 
                    p_gb_context, &message_map, index, p_intrl, 
                    &iE_list, &ie_list_index, p_send_err_ind);

            p_node= p_node->next;            
        }

    } while (0);

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_gb_context,
                p_asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_s1ap_id_InitialContextSetup,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                p_err_ind_content))
    {
        RRC_UECC_TRACE(p_gb_context->context_index,
                p_gb_context->facility_name,
                RRC_INFO, "Error Indication Detected");
        result = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_ue_context_release_command_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8 mme_id
 *                  s1ap_UEContextReleaseCommand    *p_3gpp
 *                  rrc_s1ap_ue_context_release_command_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_ue_context_release_command_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_ue_context_release_command_t from
 *                  s1ap_UEContextReleaseCommand
 ****************************************************************************/
rrc_return_et rrc_s1ap_ue_context_release_command_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8 mme_id,         /* for memory allocation */
 s1ap_UEContextReleaseCommand    *p_3gpp,    /* unpacked src */
 rrc_s1ap_ue_context_release_command_t *p_intrl/* dest */
 )
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode  *p_node     = PNULL;
    s1ap_UEContextReleaseCommand_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16 ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;


    uecc_s1ap_message_data_t message_map =
    {2, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_UE_S1AP_IDs, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_Cause, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_UEContextReleaseCommand_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_UE_S1AP_IDs:
                    {

                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._UEContextReleaseCommand_IEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            l3_memcpy_wrapper(&p_intrl->ue_s1ap_ids,
                                    p_protocol_IE->value.u._UEContextReleaseCommand_IEs_1,
                                    sizeof(s1ap_UE_S1AP_IDs));
                        }
                    }
                    break;                
                case ASN1V_s1ap_id_Cause:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._UEContextReleaseCommand_IEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            l3_memcpy_wrapper(&p_intrl->cause,
                                    p_protocol_IE->value.u._UEContextReleaseCommand_IEs_2,
                                    sizeof(s1ap_Cause));
                        }

                    }
                    break;           
                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list,
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

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_UEContextRelease,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " UE Context Release Command ");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_downlink_nas_transport_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8 mme_id
 *                  s1ap_DownlinkNASTransport *p_3gpp
 *                  rrc_s1ap_downlink_nas_transport_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_downlink_nas_transport_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_downlink_nas_transport_t from
 *                  s1ap_DownlinkNASTransport
 ****************************************************************************/
rrc_return_et rrc_s1ap_downlink_nas_transport_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8 mme_id,
 s1ap_DownlinkNASTransport *p_3gpp,  /* unpacked src */
 rrc_s1ap_downlink_nas_transport_t *p_intrl/* dest */
 )
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode  *p_node     = PNULL;
    s1ap_DownlinkNASTransport_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16 ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map =
    {5, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_NAS_PDU, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3,ASN1V_s1ap_id_HandoverRestrictionList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4,ASN1V_s1ap_id_SubscriberProfileIDforRFP, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {5,ASN1V_s1ap_id_NRUESecurityCapabilities, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name,
                    RRC_ERROR,"%s: "
                    "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_DownlinkNASTransport_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._DownlinkNASTransport_IEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id =
                                p_protocol_IE->value.u._DownlinkNASTransport_IEs_1;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._DownlinkNASTransport_IEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id =
                                p_protocol_IE->value.u._DownlinkNASTransport_IEs_2;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_NAS_PDU:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._DownlinkNASTransport_IEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            l3_memcpy_wrapper(&p_intrl->nas_pdu,
                                    p_protocol_IE->value.u._DownlinkNASTransport_IEs_3,
                                    sizeof(s1ap_NAS_PDU));
                        }
                    }
                    break;

                case ASN1V_s1ap_id_HandoverRestrictionList:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._DownlinkNASTransport_IEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            l3_memcpy_wrapper(&p_intrl->handover_restriction_list,
                                    p_protocol_IE->value.u._DownlinkNASTransport_IEs_4,
                                    sizeof(s1ap_HandoverRestrictionList));
                        }

                        p_intrl->bitmask |=
                            RRC_S1AP_DOWNLINK_NAS_TRANSPORT_HANDOVER_RESTRICTION_LIST_PRESENT;
                    }
                    break;
                 
                case ASN1V_s1ap_id_SubscriberProfileIDforRFP:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._DownlinkNASTransport_IEs_5,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->subs_prof_id =
                                p_protocol_IE->value.u._DownlinkNASTransport_IEs_5;

                            p_intrl->bitmask |=
                                RRC_S1AP_DOWNLINK_NAS_TRANSPORT_SUBSCRIBER_PROFILE_ID_PRESENT;
                        }
                    }
                    break;
				
				case ASN1V_s1ap_id_NRUESecurityCapabilities : 
					s1ap_process_NRUESecurityCapabilities(
							p_uecc_gb_context, &message_map,
							p_protocol_IE->value.u._DownlinkNASTransport_IEs_10,
							&p_intrl->nr_security_cap, &p_intrl->bitmask,
							RRC_S1AP_DOWNLINK_NAS_TRANSPORT_NR_SECURITY_CAP_PRESENT,index);
					break;

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list,
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


    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_downlinkNASTransport,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " DL NAS Transport ");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : s1ap_handover_command_process_protocolIE_id
 * Inputs         : p_protocol_IE
 *                  p_ue_context - pointer to uecc_ue_context_t
 *                  index            
 *                  p_ho_cmd_ctxt
 *                  cause
 * Outputs        : p_intrl (rrc_s1ap_handover_command_t)
 *                  p_message_map - uecc_s1ap_message_data_t
 *                  p_iE_list
 *                  p_ie_list_index
 *                  p_send_err_ind
 * Returns        : 
 * Description    : This function processes protocol IE id and fill the intrl
 *                  message, for s1ap HandoverCommand msg.
 ****************************************************************************/
static void s1ap_handover_command_process_protocolIE_id(
        s1ap_HandoverCommand_protocolIEs_element *p_protocol_IE,
        uecc_ue_context_t           *p_ue_context,
        uecc_s1ap_message_data_t    *p_message_map,
        U32                         index,
        rrc_s1ap_handover_command_t *p_intrl,
        s1ap_error_ind_ie_list_t    *p_iE_list,
        U16                         *p_ie_list_index,
        uecc_s1ap_error_ind_bool_t  *p_send_err_ind,
        OSCTXT                      *p_ho_cmd_ctxt,
        s1ap_Cause cause)
{
    s1ap_Target_ToSource_TransparentContainer container;
    RRC_UT_TRACE_ENTER();

    memset_wrapper(&container, 0, sizeof(s1ap_Target_ToSource_TransparentContainer));

    switch (p_protocol_IE->id)
    {
        case ASN1V_s1ap_id_MME_UE_S1AP_ID:
            {
                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            &p_protocol_IE->value.u._HandoverCommandIEs_1,
                            (uecc_ue_context_t *)PNULL))
                {
                    p_intrl->mme_ue_s1ap_id = 
                        p_protocol_IE->value.u._HandoverCommandIEs_1;
                }
            }
            break;

        case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
            {
                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            &p_protocol_IE->value.u._HandoverCommandIEs_2,
                            (uecc_ue_context_t *)PNULL))
                {
                    p_intrl->enb_ue_s1ap_id= 
                        p_protocol_IE->value.u._HandoverCommandIEs_2;
                }
            }
            break;

        case ASN1V_s1ap_id_HandoverType:
            {
                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            &p_protocol_IE->value.u._HandoverCommandIEs_3,
                            (uecc_ue_context_t *)PNULL))
                {
                    p_intrl->handover_type= 
                        p_protocol_IE->value.u._HandoverCommandIEs_3;
                }
            }
            break;

        case ASN1V_s1ap_id_NASSecurityParametersfromE_UTRAN:
            {
                if ((HANDOVER_TYPE_LTE_TO_UTRAN == p_intrl->handover_type) ||
                        (HANDOVER_TYPE_LTE_TO_GERAN == p_intrl->handover_type))
                {
                    p_message_map->msg_map[3].presence = s1ap_mandatory;

                    if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                                p_gb_context,
                                p_message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._HandoverCommandIEs_4,
                                (uecc_ue_context_t *)PNULL))
                    {
                        p_intrl->nas_security_param_from_E_UTRAN= 
                            *(p_protocol_IE->value.u._HandoverCommandIEs_4);

                        p_intrl->bitmask |= 
                            RRC_S1AP_HANDOVER_COMMAND_NAS_SECURITY_PARAMETERS_FROM_E_UTRAN_PRESENT;
                    }
                }
				/*for bugid 116542 begin*/
				else
				{
					uecc_update_message_map(p_ue_context->p_gb_context,
                            p_message_map, UECC_OCCURANCE, index, (U8)p_protocol_IE->id);
				}
				/*for bugid 116542 end*/
            }
            break;

        case ASN1V_s1ap_id_E_RABSubjecttoDataForwardingList:
            {
		/*SPR 15896 Fix Start*/
                if (p_ue_context->m.traceActivated)
                {
                    P_RRC_TRACE(p_ue_context,set_minimum_medium_both_bitmask(), S1,"\n <ieGroup name=\"E-RABs Subject to Forwarding List \">");
                }
		/*SPR 15896 Fix Stop*/

                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            p_protocol_IE->value.u._HandoverCommandIEs_5,
                            p_ue_context))
                {
                    p_intrl->erab_subject_to_data_forwarding_list= 
                        *(p_protocol_IE->value.u._HandoverCommandIEs_5);

                    p_intrl->bitmask |= 
                        RRC_S1AP_HANDOVER_COMMAND_ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT;
                }

		/*SPR 15896 Fix Start*/
		if (p_ue_context->m.traceActivated) 
		{
			P_RRC_TRACE(p_ue_context,set_minimum_medium_both_bitmask(), S1,"\n </ieGroup>");
		}
		/*SPR 15896 Fix Stop*/
	    }
	    break;

        case ASN1V_s1ap_id_E_RABtoReleaseListHOCmd:
            {
		/*SPR 15896 Fix Start*/
                /* TRACE_fix */
                if (p_ue_context->m.traceActivated)
                {
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), S1, "\n <ieGroup name=\"E-RABs to Release List\">");
                }
		/*SPR 15896 Fix Stop*/
                /* TRACE_fix */

                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            p_protocol_IE->value.u._HandoverCommandIEs_6,
                            p_ue_context))   /* TRACE_fix */
                {
                    p_intrl->erab_failed_list= 
                        *(p_protocol_IE->value.u._HandoverCommandIEs_6);

                    p_intrl->bitmask |= 
                        RRC_S1AP_HANDOVER_COMMAND_E_RAB_FAILED_LIST_PRESENT;
                }
		/*SPR 15896 Fix Start*/
                /* TRACE_fix */
                if (p_ue_context->m.traceActivated)
                {
                    P_RRC_TRACE(p_ue_context, set_minimum_medium_both_bitmask(), S1, "</ieGroup>");
                }
		/*SPR 15896 Fix Stop*/
                /* TRACE_fix */
            }
            break;

        case ASN1V_s1ap_id_Target_ToSource_TransparentContainer:
            {
                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            p_protocol_IE->value.u._HandoverCommandIEs_7,
                            (uecc_ue_context_t *)PNULL))
                {
                    if (HANDOVER_TYPE_INTRA_LTE_S1 == p_intrl->handover_type)
                    {
                        container = *(p_protocol_IE->value.u._HandoverCommandIEs_7); 

                        rrc_s1ap_target_enb_to_source_enb_transparent_container_intrl_dec( 
                                &container,
                                p_intrl,
                                p_ho_cmd_ctxt,
                                &cause);

                        if (S1AP_CAUSE_SUCCESS != cause.t)
                        {
                            RRC_UECC_TRACE(
                                    p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_WARNING,
                                    "[rrc_s1ap_target_enb_to_source_enb_transparent_container_intrl_dec returned]"
                                    "failure");

                            uecc_update_message_map(p_ue_context->
                                    p_gb_context,
                                    p_message_map, UECC_INVALID_VALUE, index, p_protocol_IE->id);
                        }
                    }
                    /* SPR 19592 FIX START*/
                    /* Added for i-Rat HO */
                    else if (HANDOVER_TYPE_LTE_TO_GERAN == p_intrl->handover_type)
                    {
                        p_intrl->target_to_source_transparent_container.bitmask |=
                            RRC_S1AP_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_FOR_GERAN_PRESENT;
			p_intrl->target_to_source_transparent_container.
                                target_to_source_transparent_container_for_geran
                                                = *(p_protocol_IE->value.u._HandoverCommandIEs_7);


                        /* CSR 101813 Fix Start */ 
			 /*           container = *(p_protocol_IE->value.u._HandoverCommandIEs_7); 

                        rrc_s1ap_target_bss_to_source_bss_transparent_container_intrl_dec( 
                                &container,
                                p_intrl,
                                p_ho_cmd_ctxt,
                                &cause);

                        if (S1AP_CAUSE_SUCCESS != cause.t)
                        {
                            RRC_UECC_TRACE(
                                    p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_WARNING,
                                    "[rrc_s1ap_target_bss_to_source_bss_transparent_container_intrl_dec returned]"
                                    "failure");

                            uecc_update_message_map(p_ue_context->
                                    p_gb_context,
                                    p_message_map, UECC_INVALID_VALUE, index, p_protocol_IE->id);
                        }
                     */  /* CSR 101813 Fix Stop */ 
                   /*SPR 19592 FIX END*/ 
                    }
                    else if(HANDOVER_TYPE_LTE_TO_UTRAN == p_intrl->handover_type)
                    {

                       /* CSR 101813 Fix Start */ 
                        p_intrl->target_to_source_transparent_container.bitmask |=
                            RRC_S1AP_TARGET_RNC_TO_SOURCE_RNC_TRANSPARENT_CONTAINER_PRESENT;
            			container = *(p_protocol_IE->value.u._HandoverCommandIEs_7); 

                        rrc_s1ap_target_rnc_to_source_rnc_transparent_container_intrl_dec( 
                                &container,
                                p_intrl,
                                p_ho_cmd_ctxt,
                                &cause);

                        if (S1AP_CAUSE_SUCCESS != cause.t)
                        {
                            RRC_UECC_TRACE(
                                    p_ue_context->p_gb_context->context_index,
                                    p_ue_context->p_gb_context->facility_name,
                                    RRC_WARNING,
                                    "[rrc_s1ap_target_rnc_to_source_rnc_transparent_container_intrl_dec returned]"
                                    "failure");

                            uecc_update_message_map(p_ue_context->
                                    p_gb_context,
                                    p_message_map, UECC_INVALID_VALUE, index, p_protocol_IE->id);
                        }/* CSR 101813 Fix Stop */
                    }
                }
            }
            break;

            /*Secondary transparent container present in case of Inter RAT HO only*/
        case ASN1V_s1ap_id_Target_ToSource_TransparentContainer_Secondary:
            {
                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            p_protocol_IE->value.u._HandoverCommandIEs_8,
                            (uecc_ue_context_t *)PNULL))
                {
                    p_intrl->target_to_source_transparent_container_secondary= 
                        *(p_protocol_IE->value.u._HandoverCommandIEs_8);

                    p_intrl->bitmask |=
                        RRC_S1AP_HANDOVER_COMMAND_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_SECONDARY_PRESENT;
                }
            }
            break;

        case ASN1V_s1ap_id_CriticalityDiagnostics:
            {
                if (RRC_SUCCESS == uecc_validate_ie_value(p_ue_context->
                            p_gb_context,
                            p_message_map,
                            index,
                            (U8)p_protocol_IE->id,
                            p_protocol_IE->value.u._HandoverCommandIEs_9,
                            (uecc_ue_context_t *)PNULL))
                {
                    p_intrl->bitmask |=
                        RRC_S1AP_HANDOVER_COMMAND_CRITICALITY_DIAGNOSTICS_PRESENT;
                    log_uecc_criticality_diagostics(p_ue_context->p_gb_context,
                            p_protocol_IE->value.u._HandoverCommandIEs_9);
                }
            }
            break;

        default:
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name,
                    RRC_WARNING,"%s:"
                    " invalid ProtocolElement IE id",__FUNCTION__);

            uecc_add_to_err_ind_ie_list(p_ue_context->
                    p_gb_context,
                    p_iE_list, 
                    p_protocol_IE->criticality,
                    p_protocol_IE->id,
                    p_ie_list_index,
                    p_send_err_ind,
                    RRC_FALSE,
                    RRC_FALSE);
    }
    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrc_s1ap_handover_command_intrl_dec
 * Inputs         : U8           mme_id
 *                  OSCTXT      *p_ho_cmd_ctxt
 *                  s1ap_HandoverCommand *p_3gpp
 *                  rrc_s1ap_handover_command_t *p_intrl
 *                  uecc_ue_context_t             *p_ue_context  
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_handover_command_t from
 *                  s1ap_HandoverCommand
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_command_intrl_dec
(
 U8           mme_id,
 OSCTXT      *p_ho_cmd_ctxt,         /* for memory allocation */
 s1ap_HandoverCommand *p_3gpp,    /* unpacked src */
 rrc_s1ap_handover_command_t *p_intrl, /* dest */
 uecc_ue_context_t             *p_ue_context
 )
{
    OSRTDListNode* p_node = PNULL;
    s1ap_HandoverCommand_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16 ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    rrc_return_et result = RRC_SUCCESS;
    s1ap_Cause cause;
    OSCTXT asn1_ctx;


    uecc_s1ap_message_data_t message_map = 
    {9, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_HandoverType , s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_NASSecurityParametersfromE_UTRAN, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4, ASN1V_s1ap_id_E_RABSubjecttoDataForwardingList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {5, ASN1V_s1ap_id_E_RABtoReleaseListHOCmd, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {6, ASN1V_s1ap_id_Target_ToSource_TransparentContainer, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {7, ASN1V_s1ap_id_Target_ToSource_TransparentContainer_Secondary,
s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {8, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));
    
    cause.t = T_s1ap_Cause_protocol;

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_ERROR,
                "[s1ap_ErrorIndication] ASN context initialization failed");
        return RRC_FAILURE;
    }

    p_intrl->bitmask = 0;
    p_intrl->target_to_source_transparent_container.bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                        p_ue_context->p_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_HandoverCommand_protocolIEs_element *)
                p_node->data;

            s1ap_handover_command_process_protocolIE_id(p_protocol_IE,
                        p_ue_context, &message_map, index, p_intrl,
                        &iE_list, &ie_list_index, &send_err_ind, 
                        p_ho_cmd_ctxt, cause);

            p_node= p_node->next;            
        }
    } while (0);
    if (RRC_SUCCESS == uecc_parse_message_map(
                p_ue_context->p_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_HandoverPreparation,
                T_S1AP_PDU_successfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                p_ue_context->p_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " Handover Command");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();

    return result;    
}


/****************************************************************************
 * Function Name  : rrc_s1ap_handover_command_container_intrl_dec
 * Inputs         : s1ap_TargeteNB_ToSourceeNB_TransparentContainer *p_container
 *                  rrc_s1ap_handover_command_t *p_intrl
 *                  OSCTXT      *p_asn1_ctx
 *                  s1ap_Cause *cause
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : Decodes the HandoverCommand from
 *                       s1ap_TargeteNB_ToSourceeNB_TransparentContainer
 ****************************************************************************/
void rrc_s1ap_handover_command_container_intrl_dec
(
 s1ap_TargeteNB_ToSourceeNB_TransparentContainer *p_container,
 rrc_s1ap_handover_command_t *p_intrl,
 OSCTXT      *p_asn1_ctx,
 s1ap_Cause *cause
 )
{
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];


    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_intrl);

    memset_wrapper(msg, 0, sizeof(msg));

    l3_memcpy_wrapper(msg, 
            p_container->rRC_Container.data, 
            p_container->rRC_Container.numocts);

    do
    {
        /*get pointer on LLIM message body*/
        pu_setBuffer(p_asn1_ctx, 
                msg,
                p_container->rRC_Container.numocts,
                ASN_RRC_BUF_TYPE);

        if ( RT_OK != asn1PD_HandoverCommand(p_asn1_ctx, 
                    &p_intrl->target_to_source_transparent_container.ho_command))
        {
            /* Drop message */
            RRC_TRACE(RRC_WARNING,
                    "rrc_s1ap_handover_command_container_intrl_dec: ASN decode failed.");
            cause->u.protocol = s1ap_abstract_syntax_error_reject;
            break;
        }

        p_intrl->target_to_source_transparent_container.bitmask |=
            RRC_S1AP_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_HO_COMMAND_PRESENT;

        cause->t = S1AP_CAUSE_SUCCESS;
    } while(0);

    RRC_UT_TRACE_EXIT();

}

/****************************************************************************
 * Function Name  : rrc_s1ap_target_enb_to_source_enb_transparent_container_intrl_dec
 * Inputs         : s1ap_Target_ToSource_TransparentContainer *p_container
 *                  rrc_s1ap_handover_command_t *p_intrl
 *                  OSCTXT  *p_ho_cmd_ctxt
 *                  s1ap_Cause *cause
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes s1ap_TargeteNB_ToSourceeNB_TransparentContainer from
 *                  s1ap_Target_ToSource_TransparentContainer
 ****************************************************************************/
void rrc_s1ap_target_enb_to_source_enb_transparent_container_intrl_dec
(
 s1ap_Target_ToSource_TransparentContainer *p_container,
 rrc_s1ap_handover_command_t *p_intrl,
 OSCTXT  *p_ho_cmd_ctxt,
 s1ap_Cause *cause 
 )
{
    OSCTXT  ctxt;
    s1ap_TargeteNB_ToSourceeNB_TransparentContainer
        targeteNB_to_sourceeNB_transparent_container;
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];

    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_intrl);

    do
    {
        /* Init ASN context */
        if (RT_OK != rtInitContext(&ctxt))
        {
            RRC_TRACE(RRC_ERROR,
                    "[rrc_s1ap_target_enb_to_source_enb_transparent_container_intrl_dec]"
                    "ASN context initialization failed");
            RRC_UT_TRACE_EXIT();
            break;
        }

        memset_wrapper(msg, 0, sizeof(msg));
        memset_wrapper(&targeteNB_to_sourceeNB_transparent_container,
                0, sizeof(targeteNB_to_sourceeNB_transparent_container));
        l3_memcpy_wrapper(msg, p_container->data, p_container->numocts);

        pu_setBuffer(&ctxt, msg, 
                p_container->numocts, ASN_S1AP_BUF_TYPE);

        /*SPR_16428_START*/
        if (RT_OK != 
                asn1PD_s1ap_TargeteNB_ToSourceeNB_TransparentContainer(
                    &ctxt, &targeteNB_to_sourceeNB_transparent_container))
        {
            RRC_TRACE(RRC_WARNING, 
                    "asn1PD_s1ap_TargeteNB_ToSourceeNB_TransparentContainer failed.");
            cause->u.protocol = s1ap_transfer_syntax_error;
            break;
        }
        /*SPR_ 16428_END*/
        rrc_s1ap_handover_command_container_intrl_dec(
                &targeteNB_to_sourceeNB_transparent_container,
                p_intrl,
                p_ho_cmd_ctxt,
                cause);

        if (S1AP_CAUSE_SUCCESS != cause->t)
        {
            /* Message parsing error */
            RRC_TRACE(RRC_WARNING,
                    "[rrc_s1ap_handover_command_container_intrl_dec returned]"
                    "failure");

            /* Drop. */
            break;
        }

    }while(0);

    rtFreeContext(&ctxt);

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrc_s1ap_target_rnc_to_source_rnc_transparent_container_intrl_dec
 * Inputs         : s1ap_Target_ToSource_TransparentContainer *p_container
 *                  rrc_s1ap_handover_command_t *p_intrl
 *                  OSCTXT  *p_ho_cmd_ctxt
 *                  s1ap_Cause *cause
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes s1ap_TargetRNC_ToSourceRNC_TransparentContainer from
 *                  s1ap_Target_ToSource_TransparentContainer
 ****************************************************************************/
void rrc_s1ap_target_rnc_to_source_rnc_transparent_container_intrl_dec
(
 s1ap_Target_ToSource_TransparentContainer *p_container,
 rrc_s1ap_handover_command_t *p_intrl,
 OSCTXT  *p_ho_cmd_ctxt,
 s1ap_Cause *cause
 )
{
    /* CSR 101813 Fix Start */
    /*SPR_18125_START*/
    /* Code Removed */
    /*SPR_18125_END*/
    /* CSR 101813 Fix Stop */
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];

    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_intrl);

    do
    {
    	/* CSR 101813 Fix Start */
    	/*SPR_18125_START*/
        memset_wrapper(msg, 0, sizeof(msg));

        l3_memcpy_wrapper(msg, p_container->data, p_container->numocts);
        pu_setBuffer(p_ho_cmd_ctxt, msg, 
                p_container->numocts, ASN_S1AP_BUF_TYPE);
        if (RT_OK != 
                asn1PD_ranap_TargetRNC_ToSourceRNC_TransparentContainer(
                    p_ho_cmd_ctxt,
                    &p_intrl->target_to_source_transparent_container.\
                    targetRNC_ToSourceRNC_TransparentContainer))
    	/*SPR_18125_END*/
        {/* CSR 101813 Fix Stop */
            RRC_TRACE(RRC_WARNING, 
                    "[rrc_s1ap_target_rnc_to_source_rnc_transparent_container_intrl_dec]"
                    "asn1PD_ranap_TargetRNC_ToSourceRNC_TransparentContainer failed.");
            cause->u.protocol = s1ap_abstract_syntax_error_reject;
        }
        if ((cause->u.protocol != s1ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != s1ap_abstract_syntax_error_reject))
        {
            cause->t = S1AP_CAUSE_SUCCESS;
        }
    }while(0);

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrc_s1ap_handover_preparation_failure_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8                              mme_id
 *                  s1ap_HandoverPreparationFailure *p_3gpp
 *                  rrc_s1ap_handover_preparation_failure_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_handover_command_t from
 *                  s1ap_HandoverCommand
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_preparation_failure_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8                              mme_id,
 s1ap_HandoverPreparationFailure *p_3gpp,    /* unpacked src */
 rrc_s1ap_handover_preparation_failure_t *p_intrl/* dest */
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_HandoverPreparationFailure_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map = 
    {4, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_Cause , s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_HandoverPreparationFailure_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {

                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._HandoverPreparationFailureIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._HandoverPreparationFailureIEs_1;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._HandoverPreparationFailureIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id= 
                                p_protocol_IE->value.u._HandoverPreparationFailureIEs_2;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_Cause:
                    {

                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._HandoverPreparationFailureIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->cause = *(p_protocol_IE->value.u._HandoverPreparationFailureIEs_3);
                        }

                    }
                    break;


                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._HandoverPreparationFailureIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->bitmask |=
                                RRC_S1AP_HANDOVER_PREPARATION_FAILURE_DIAGNOSTICS_PRESENT;
                            log_uecc_criticality_diagostics(
                                    p_uecc_gb_context,
                                    p_protocol_IE->value.u._HandoverPreparationFailureIEs_4);
                        }

                    }
                    break;

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_HandoverPreparation,
                T_S1AP_PDU_unsuccessfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " Handover Prep Failure");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;  
}

/****************************************************************************
 * Function Name  : rrc_s1ap_mme_status_transfer_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8 mme_id
 *                  s1ap_MMEStatusTransfer *p_3gpp
 *                  rrc_s1ap_mme_status_transfer_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_handover_command_t from
 *                  s1ap_HandoverCommand
 ****************************************************************************/
rrc_return_et rrc_s1ap_mme_status_transfer_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8 mme_id,
 s1ap_MMEStatusTransfer *p_3gpp,    /* unpacked src */
 rrc_s1ap_mme_status_transfer_t *p_intrl/* dest */
) 
{
    s1ap_MMEStatusTransfer_protocolIEs_element 
        *p_protocol_IE = PNULL;
    OSRTDListNode* p_node = PNULL;
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    rrc_return_et result = RRC_SUCCESS;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map = 
    {3, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_eNB_StatusTransfer_TransparentContainer,
s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_MMEStatusTransfer_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {        
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._MMEStatusTransferIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._MMEStatusTransferIEs_1;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {

                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._MMEStatusTransferIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id= 
                                p_protocol_IE->value.u._MMEStatusTransferIEs_2;
                        }

                    }
                    break;


                case ASN1V_s1ap_id_eNB_StatusTransfer_TransparentContainer:
                    {

                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._MMEStatusTransferIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->e_nb_status_transfer_transparent_container= 
                                *(p_protocol_IE->value.u._MMEStatusTransferIEs_3);
                        }
                    }
                    break;
                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_MMEStatusTransfer,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " MME STATUS Transfer");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();

    return result;    
}


/****************************************************************************
 * Function Name  : rrc_s1ap_rrc_container_intrl_dec
 * Inputs         : s1ap_RRC_Container *p_rRC_Container
 *                  uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr
 *                  HandoverPreparationInformation_r8_IEs  *p_intrl
 *                  s1ap_Cause                             *cause
 * Outputs        : p_intrl (internal HandoverPreparationInformation_r8_IEs)
 *                : s1ap_Cause (cause)
 * Returns        : void
 * Description    : makes HandoverPreparationInformation_r8_IEs from
 *                  s1ap_RRC_Container
 ****************************************************************************/
void rrc_s1ap_rrc_container_intrl_dec
(
 s1ap_RRC_Container *p_rRC_Container,
 uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
 HandoverPreparationInformation_r8_IEs  *p_intrl,
 s1ap_Cause                             *cause
 )
{
    HandoverPreparationInformation value;
    U8 index;
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];

    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_rRC_Container);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_container_asn1_ctxt_arr);

    index = p_container_asn1_ctxt_arr->count;

    /* Init ASN1 context */
    if ( RT_OK != rtInitContext(&p_container_asn1_ctxt_arr->asn1_ctxt[index]))
    {
        /* Drop message */
        RRC_TRACE(RRC_ERROR,
                "[rrc_s1ap_rrc_container_intrl_dec] ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
    }

    do
    {
        l3_memcpy_wrapper(msg, p_rRC_Container->data, p_rRC_Container->numocts);

        pu_setBuffer(&p_container_asn1_ctxt_arr->asn1_ctxt[index], 
                msg,
                p_rRC_Container->numocts, 
                ASN_RRC_BUF_TYPE);

        if ( RT_OK != asn1PD_HandoverPreparationInformation(
                    &p_container_asn1_ctxt_arr->asn1_ctxt[index], 
                    &value))
        {
            RRC_TRACE(RRC_WARNING, "asn1PD_HandoverPreparationInformation failed.");
            cause->u.protocol = s1ap_transfer_syntax_error;
            break;
        }

        if ((1 == value.criticalExtensions.t) && 
                (PNULL != value.criticalExtensions.u.c1) && 
                (1 == value.criticalExtensions.u.c1->t) && 
                (PNULL != value.criticalExtensions.u.c1->
                 u.handoverPreparationInformation_r8))
        {
            *p_intrl = 
                *(value.criticalExtensions.u.c1->u.handoverPreparationInformation_r8);
        }
        else
        {
            RRC_TRACE(RRC_WARNING, "HandoverPreparationInformation decoding failed.");
            cause->u.protocol = s1ap_transfer_syntax_error;
            break;
        }

        p_container_asn1_ctxt_arr->count++;

        if ((cause->u.protocol != s1ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != s1ap_abstract_syntax_error_reject))
        {
            cause->t = S1AP_CAUSE_SUCCESS;
        }
    } while(0);

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrc_s1ap_last_visited_utran_cell_information_intrl_dec
 * Inputs         : OSCTXT      *p_asn1_ctxt
 *                  s1ap_LastVisitedUTRANCellInformation  *p_utran_cell
 *                  ranap_LastVisitedUTRANCell_Item  *p_intrl
 *                  s1ap_Cause                       *cause
 * Outputs        : p_intrl (internal ranap_LastVisitedUTRANCell_Item)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes ranap_LastVisitedUTRANCell_Item from
 *                  s1ap_LastVisitedUTRANCellInformation
 ****************************************************************************/
void rrc_s1ap_last_visited_utran_cell_information_intrl_dec
(
 OSCTXT      *p_asn1_ctxt,
 s1ap_LastVisitedUTRANCellInformation  *p_utran_cell,
 ranap_LastVisitedUTRANCell_Item  *p_intrl,
 s1ap_Cause                       *cause
 )
{
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];

    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_asn1_ctxt);
    RRC_ASSERT(PNULL != p_utran_cell);
    RRC_ASSERT(PNULL != p_intrl);


    do
    {
        l3_memcpy_wrapper(msg, p_utran_cell->data, p_utran_cell->numocts);

        pu_setBuffer(p_asn1_ctxt, 
                msg,
                p_utran_cell->numocts, 
                ASN_S1AP_BUF_TYPE);

        if ( RT_OK != asn1PD_ranap_LastVisitedUTRANCell_Item (
                    p_asn1_ctxt, 
                    p_intrl))
        {
            RRC_TRACE(RRC_WARNING, 
                    "asn1PD_ranap_LastVisitedUTRANCell_Item failed.");
            cause->u.protocol = s1ap_transfer_syntax_error;
            break;
        }

        if ((cause->u.protocol != s1ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != s1ap_abstract_syntax_error_reject))
        {
            cause->t = S1AP_CAUSE_SUCCESS;
        }
    } while(0);


    RRC_UT_TRACE_EXIT();
    return;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_source_enb_to_target_enb_transparent_container_intrl_dec
 * Inputs         : s1ap_Source_ToTarget_TransparentContainer  *p_buff
 *                  uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr
 *                  rrc_source_to_target_transparent_container_t  *p_intrl
 *                  s1ap_Cause                 *cause
 * Outputs        : p_intrl (internal rrc_s1ap_handover_request_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes rrc_s1ap_handover_request_t from
 *                  s1ap_HandoverRequest
 ****************************************************************************/
void rrc_s1ap_source_enb_to_target_enb_transparent_container_intrl_dec
(
 s1ap_Source_ToTarget_TransparentContainer  *p_buff,        /* ASN.1 encoded message */
 uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
 rrc_source_to_target_transparent_container_t  *p_intrl, /* Encoded message*/
 s1ap_Cause                 *cause
 )
{
    OSRTDListNode* p_node = PNULL;
    s1ap_SourceeNB_ToTargeteNB_TransparentContainer value;
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];
    U8 index = 0, count = 0;
    s1ap_LastVisitedCell_Item *p_last_visited_cell_item = PNULL;

    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_buff);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_container_asn1_ctxt_arr);

    index = p_container_asn1_ctxt_arr->count;

    /* Init ASN1 context */
    if ( RT_OK != rtInitContext(&p_container_asn1_ctxt_arr->asn1_ctxt[index]))
    {
        /* Drop message */
        RRC_TRACE(RRC_ERROR,
                "[rrc_s1ap_rrc_container_intrl_dec] ASN context initialization failed.");
        RRC_UT_TRACE_EXIT();
    }

    do
    {
        l3_memcpy_wrapper(msg, p_buff->data, p_buff->numocts);
        pu_setBuffer(
                &p_container_asn1_ctxt_arr->asn1_ctxt[index], 
                msg, 
                p_buff->numocts, 
                ASN_S1AP_BUF_TYPE);

        if (RT_OK !=  
                asn1PD_s1ap_SourceeNB_ToTargeteNB_TransparentContainer(
                    &p_container_asn1_ctxt_arr->asn1_ctxt[index], &value))
        {
            RRC_TRACE(RRC_WARNING, 
                    "asn1PD_s1ap_SourceeNB_ToTargeteNB_TransparentContainer failed.");
            cause->u.protocol = s1ap_transfer_syntax_error;
            break;
        }

        p_intrl->bitmask |= 
            RRC_SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_PRESENT;

        if (value.m.e_RABInformationListPresent)
        {
            p_intrl->source_enb_to_target_enb_transparent_container.
                e_RABInformationList =  value.e_RABInformationList;
            p_intrl->source_enb_to_target_enb_transparent_container.bitmask |= 
                RRC_SOURCE_ENB_TO_TARGET_ENB_E_RAB_INFO_LIST_PRESENT;

        }

        p_intrl->source_enb_to_target_enb_transparent_container.
            targetCell_ID = value.targetCell_ID;

        if (value.m.subscriberProfileIDforRFPPresent)
        {
            p_intrl->source_enb_to_target_enb_transparent_container.
                subscriberProfileIDforRFP = value.subscriberProfileIDforRFP;
            p_intrl->source_enb_to_target_enb_transparent_container.bitmask |= 
                RRC_SOURCE_ENB_TO_TARGET_ENB_SUBSCRIBER_PROFILER_ID_FOR_RFP_PRESENT;
        }

        p_intrl->source_enb_to_target_enb_transparent_container.
            ue_HistoryInformation.count = 0;
        for (count=0, p_node = value.uE_HistoryInformation.head;
                PNULL != p_node;
                count++, p_node = p_node->next)
        {
            p_last_visited_cell_item = 
                (s1ap_LastVisitedCell_Item*)p_node->data;

            RRC_ASSERT(PNULL != p_last_visited_cell_item);

            p_intrl->source_enb_to_target_enb_transparent_container.ue_HistoryInformation.count++;

            l3_memcpy_wrapper(&p_intrl->source_enb_to_target_enb_transparent_container.
                    ue_HistoryInformation.lastVisited_cell_item[count],
                    p_last_visited_cell_item, sizeof(s1ap_LastVisitedCell_Item));

        }

        p_intrl->bitmask |= 
            RRC_SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_PRESENT;

        p_container_asn1_ctxt_arr->count++;

        /* Decode message */
        rrc_s1ap_rrc_container_intrl_dec(
                &value.rRC_Container,
                p_container_asn1_ctxt_arr,
                &p_intrl->source_enb_to_target_enb_transparent_container.rRC_Container,
                cause);

        if (S1AP_CAUSE_SUCCESS != cause->t)
        {
            /* Message parsing error */
            RRC_TRACE(RRC_WARNING,
                    "[S1AP Handover Request ] Parsing error");
            /* Drop. */
            break;
        }

        if ((cause->u.protocol != s1ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != s1ap_abstract_syntax_error_reject))
        {
            cause->t = S1AP_CAUSE_SUCCESS;
        }
    } while (0);

    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : s1ap_handover_request_process_protocolIE_id
 * Inputs         : p_protocol_IE
 *                  p_uecc_gb_context - pointer to uecc_gb_context_t
 *                  index            
 *                  p_ho_cmd_ctxt
 *                  p_container_asn1_ctxt_arr
 * Outputs        : p_intrl (rrc_s1ap_handover_request_t)
 *                  p_message_map
 *                  p_iE_list
 *                  p_ie_list_index
 *                  p_send_err_ind
 * Returns        : 
 * Description    : This function processes protocol IE id and fill the intrl
 *                  message, for s1ap HandoverRequest msg.
 ****************************************************************************/
static void s1ap_handover_request_process_protocolIE_id(
        s1ap_HandoverRequest_protocolIEs_element *p_protocol_IE,
        uecc_gb_context_t           *p_uecc_gb_context,
        uecc_s1ap_message_data_t    *p_message_map,
        U32                         index,
        rrc_s1ap_handover_request_t *p_intrl,
        s1ap_error_ind_ie_list_t    *p_iE_list,
        U16                         *p_ie_list_index,
        uecc_s1ap_error_ind_bool_t  *p_send_err_ind,
        uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
        s1ap_Cause result)
{
    s1ap_Source_ToTarget_TransparentContainer  transparent_container;
    RRC_UT_TRACE_ENTER();

    switch (p_protocol_IE->id)
    {        
        case ASN1V_s1ap_id_MME_UE_S1AP_ID:
            s1ap_process_MME_UE_S1AP_ID(
                    p_uecc_gb_context, p_message_map,
                    &p_protocol_IE->value.u._HandoverRequestIEs_1,
                    &p_intrl->mme_ue_s1ap_id, index);
            break;

        case ASN1V_s1ap_id_HandoverType:
            s1ap_process_HandoverType(p_uecc_gb_context, p_message_map,
                    &p_protocol_IE->value.u._HandoverRequestIEs_2,
                    &p_intrl->handover_type, index);
            break;

        case ASN1V_s1ap_id_Cause:
            s1ap_process_Cause(p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_3,
                    &p_intrl->cause, index);
            break;

        case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
            s1ap_process_uEaggregateMaximumBitrate(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_4,
                    &p_intrl->ue_aggregate_maximum_bitrate, index);
            break;

        case ASN1V_s1ap_id_E_RABToBeSetupListHOReq:
            s1ap_process_E_RABToBeSetupListHOReq(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_5,
                    &p_intrl->e_rab_to_be_setup_list, index);
            break;

        case ASN1V_s1ap_id_Source_ToTarget_TransparentContainer:
            s1ap_process_Source_ToTarget_TransparentContainer(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_6,
                    &transparent_container,
                    &p_intrl->source_to_target_transparent_container,
                    p_container_asn1_ctxt_arr,
                    &result, index);
            break;

        case ASN1V_s1ap_id_UESecurityCapabilities:
            s1ap_process_UESecurityCapabilities(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_7,
                    &p_intrl->ue_security_capabilities, index);
            break;

        case ASN1V_s1ap_id_HandoverRestrictionList:
            s1ap_process_ho_request_HandoverRestrictionList(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_8,
                    p_intrl, index);
            break;

        case ASN1V_s1ap_id_TraceActivation:
            s1ap_process_ho_request_TraceActivation(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_9,
                    p_intrl, index);
            break;

        case ASN1V_s1ap_id_RequestType:
            s1ap_process_ho_request_RequestType(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_10,
                    p_intrl, index);
            break;

        case ASN1V_s1ap_id_SRVCCOperationPossible:
            s1ap_process_ho_request_SRVCCOperationPossible(
                    p_uecc_gb_context, p_message_map,
                    &p_protocol_IE->value.u._HandoverRequestIEs_11,
                    p_intrl, index);
            break;

        case ASN1V_s1ap_id_SecurityContext:
            s1ap_process_SecurityContext(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_12,
                    &p_intrl->security_context, index);
            break;

        case ASN1V_s1ap_id_NASSecurityParameterstoE_UTRAN:
            s1ap_process_ho_request_NASSecurityParameterstoE_UTRAN(
                     p_uecc_gb_context, p_message_map,
                     p_protocol_IE->value.u._HandoverRequestIEs_13,
                     p_intrl, index);
            break;
            /* CR_592_start */

        case ASN1V_s1ap_id_CSG_Id:
            s1ap_process_ho_request_CSG_Id(
                    p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_14,
                    p_intrl, index);
            break;

        case ASN1V_s1ap_id_CSGMembershipStatus:
            s1ap_process_ho_request_CSGMembershipStatus(
                    p_uecc_gb_context, p_message_map,
                    &p_protocol_IE->value.u._HandoverRequestIEs_15,
                    p_intrl, index);
            break;

            /* CR_592_stop */
        case ASN1V_s1ap_id_GUMMEI_ID:
            s1ap_process_GUMMEI_ID(p_uecc_gb_context, p_message_map,
                    p_protocol_IE->value.u._HandoverRequestIEs_16,
                    &p_intrl->gummei, &p_intrl->bitmask, index);
            break;

        case ASN1V_s1ap_id_MME_UE_S1AP_ID_2:
            s1ap_process_MME_UE_S1AP_ID_2(
                    p_uecc_gb_context, p_message_map,
                    &p_protocol_IE->value.u._HandoverRequestIEs_17,
                    &p_intrl->mme_ue_s1ap_id2, &p_intrl->bitmask, index);
            break;

        case ASN1V_s1ap_id_ManagementBasedMDTAllowed:
            s1ap_process_ManagementBasedMDTAllowed(
                    p_uecc_gb_context, p_message_map,
                    &p_protocol_IE->value.u._HandoverRequestIEs_18,
                    index);
            break;
			
		case ASN1V_s1ap_id_NRUESecurityCapabilities : 
			s1ap_process_NRUESecurityCapabilities(
					p_uecc_gb_context, p_message_map,
					p_protocol_IE->value.u._HandoverRequestIEs_27,
					&p_intrl->nr_security_cap, &p_intrl->bitmask,
					RRC_S1AP_HANDOVER_REQUEST_NR_SECURITY_CAP_PRESENT,index);
			break;

        default:
            uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                    p_iE_list,
                    p_protocol_IE->criticality,
                    p_protocol_IE->id,
                    p_ie_list_index,
                    p_send_err_ind,
                    RRC_FALSE,
                    RRC_FALSE);
    }
    RRC_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrc_s1ap_handover_request_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  OSCTXT *p_asn1_ctx
 *                  U8 mme_id
 *                  s1ap_HandoverRequest *p_3gpp
 *                  uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr
 *                  rrc_s1ap_handover_request_t *p_intrl
 *                  rrc_s1ap_error_indication_t *p_error_ind_content
 *                  uecc_s1ap_error_ind_bool_t *p_send_err_ind
 * Outputs        : p_intrl (internal rrc_s1ap_handover_request_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_handover_request_t from
 *                  s1ap_HandoverRequest
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_request_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 OSCTXT *p_asn1_ctx,
 U8 mme_id,
 s1ap_HandoverRequest *p_3gpp,    /* unpacked src */
 uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
 rrc_s1ap_handover_request_t *p_intrl, /* dest */
 rrc_s1ap_error_indication_t *p_error_ind_content,
 uecc_s1ap_error_ind_bool_t *p_send_err_ind
 )
{
    s1ap_Cause result;
    OSRTDListNode  *p_node     = PNULL;
    s1ap_HandoverRequest_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    s1ap_error_ind_ie_list_t iE_list;
    U16  ie_list_index = 0;
    rrc_return_et response = RRC_SUCCESS;

    uecc_s1ap_message_data_t message_map =
    {18, RRC_TRUE, RRC_TRUE, 0, 0, 0,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_HandoverType, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_Cause, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4, ASN1V_s1ap_id_E_RABToBeSetupListHOReq, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {5, ASN1V_s1ap_id_Source_ToTarget_TransparentContainer, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {6, ASN1V_s1ap_id_UESecurityCapabilities, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {7, ASN1V_s1ap_id_HandoverRestrictionList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {8, ASN1V_s1ap_id_TraceActivation, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {9, ASN1V_s1ap_id_RequestType, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {10, ASN1V_s1ap_id_SRVCCOperationPossible, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {11, ASN1V_s1ap_id_SecurityContext, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {12, ASN1V_s1ap_id_NASSecurityParameterstoE_UTRAN, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
/* CR_592_start*/
          {13, ASN1V_s1ap_id_CSG_Id, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {14, ASN1V_s1ap_id_CSGMembershipStatus, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
/* CR_592_stop*/
          {15, ASN1V_s1ap_id_GUMMEI_ID, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {16, ASN1V_s1ap_id_MME_UE_S1AP_ID_2, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {17, ASN1V_s1ap_id_ManagementBasedMDTAllowed, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {18, ASN1V_s1ap_id_NRUESecurityCapabilities, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};

    RRC_UT_TRACE_ENTER();

    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    result.t = T_s1ap_Cause_protocol;
    result.u.protocol = s1ap_unspecified_1;

    p_intrl->bitmask = 0;
    do 
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                result.u.protocol = s1ap_transfer_syntax_error;
                break;
            }

            p_protocol_IE = (s1ap_HandoverRequest_protocolIEs_element *)p_node->data;

            s1ap_handover_request_process_protocolIE_id(p_protocol_IE, 
                        p_uecc_gb_context, &message_map, index, p_intrl, &iE_list,
                        &ie_list_index, p_send_err_ind, p_container_asn1_ctxt_arr, result);

            p_node= p_node->next;            
        }
    }while(0);

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                p_asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_s1ap_id_HandoverResourceAllocation,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                p_error_ind_content))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "Error Indication Detected");
        response = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();

    return response;  

}

/****************************************************************************
 * Function Name  : rrc_s1ap_ue_context_modification_request_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  OSCTXT *p_asn1_ctx
 *                  U8 mme_id
 *                  s1ap_UEContextModificationRequest *p_3gpp
 *                  rrc_s1ap_ue_context_modification_request_t *p_intrl
 *                  rrc_s1ap_error_indication_t *p_error_ind_content
 *                  uecc_s1ap_error_ind_bool_t *p_send_err_ind
 * Outputs        : p_intrl (rrc_s1ap_ue_context_modification_request_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_ue_context_modification_request_t from
 *                  s1ap_UeContextmodification
 ****************************************************************************/
rrc_return_et rrc_s1ap_ue_context_modification_request_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 OSCTXT *p_asn1_ctx,
 U8 mme_id,
 s1ap_UEContextModificationRequest *p_3gpp,    /* unpacked src */
 rrc_s1ap_ue_context_modification_request_t *p_intrl,/* dest */
 rrc_s1ap_error_indication_t *p_error_ind_content,
 uecc_s1ap_error_ind_bool_t *p_send_err_ind
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_UEContextModificationRequest_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    rrc_return_et resp = RRC_SUCCESS;

    uecc_s1ap_message_data_t message_map =
    {9, RRC_TRUE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{ 0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 2, ASN1V_s1ap_id_SecurityKey, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 3, ASN1V_s1ap_id_SubscriberProfileIDforRFP, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 4, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 5, ASN1V_s1ap_id_CSFallbackIndicator, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 6, ASN1V_s1ap_id_UESecurityCapabilities, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 7, ASN1V_s1ap_id_CSGMembershipStatus, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 8, ASN1V_s1ap_id_RegisteredLAI, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 9, ASN1V_s1ap_id_NRUESecurityCapabilities, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}
        }
    };

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);


    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    p_intrl->bitmask = 0;

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_UEContextModificationRequest_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_MME_UE_S1AP_ID,
                                    &p_protocol_IE->value.u._UEContextModificationRequestIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._UEContextModificationRequestIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_eNB_UE_S1AP_ID,
                                    &p_protocol_IE->value.u._UEContextModificationRequestIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id = 
                                p_protocol_IE->value.u._UEContextModificationRequestIEs_2;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_SecurityKey:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_SecurityKey,
                                    p_protocol_IE->value.u._UEContextModificationRequestIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->security_key = 
                                *p_protocol_IE->value.u._UEContextModificationRequestIEs_3;
                            p_intrl->bitmask |= RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SECURITY_KEY_PRESENT;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_SubscriberProfileIDforRFP:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_SubscriberProfileIDforRFP,
                                    &p_protocol_IE->value.u._UEContextModificationRequestIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->subscriber_profile_id_for_rfp = 
                                p_protocol_IE->value.u._UEContextModificationRequestIEs_4;
                            /* SPR 2279 Start */
                            p_intrl->bitmask |=
                                RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT;
                            /* SPR 2279 Stop */
                        }
                    }
                    break;

                case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_uEaggregateMaximumBitrate,
                                    p_protocol_IE->value.u._UEContextModificationRequestIEs_5,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->ue_aggregate_maximum_bitrate =
                                *p_protocol_IE->value.u._UEContextModificationRequestIEs_5;
                            p_intrl->bitmask |= RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_AGG_MAX_BIT_RATE_PRESENT;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_CSFallbackIndicator:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_CSFallbackIndicator,
                                    &p_protocol_IE->value.u._UEContextModificationRequestIEs_6,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->cs_fallback_indicator = 
                                p_protocol_IE->value.u._UEContextModificationRequestIEs_6;
                            /* SPR 2279 Start */
                            p_intrl->bitmask |=
                                RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_CS_FALLBACK_INDICATOR_PRESENT;
                            /* SPR 2279 Stop */
                        }
                    }
                    break;       
                case ASN1V_s1ap_id_UESecurityCapabilities:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_UESecurityCapabilities,
                                    /*SPR_17534_START*/
                                    p_protocol_IE->value.u._UEContextModificationRequestIEs_7,
                                    /*SPR_17534_STOP*/
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->ue_security_capabilities = 
                                *p_protocol_IE->value.u._UEContextModificationRequestIEs_7;
                            p_intrl->bitmask |=
                                RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_UE_SECURITY_CAPABILITIES_PRESENT;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_CSGMembershipStatus:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_CSGMembershipStatus,
                                    &p_protocol_IE->value.u._UEContextModificationRequestIEs_8,
                                    (uecc_ue_context_t *)PNULL))        
                        {
                            p_intrl->csg_membership_status =
                                p_protocol_IE->value.u._UEContextModificationRequestIEs_8;
                            p_intrl->bitmask |=
                                RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT;
                        }
                    }
                    break;
/* CR_761_Start */
                case ASN1V_s1ap_id_RegisteredLAI:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_RegisteredLAI,
                                    p_protocol_IE->value.u._UEContextModificationRequestIEs_9,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->registered_lai = 
                                *p_protocol_IE->value.u._UEContextModificationRequestIEs_9;
                            p_intrl->bitmask |=
                               RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_REGISTERED_LAI_PRESENT; 
                        }

                    }
                    break;
/* CR_761_Stop */
				case ASN1V_s1ap_id_NRUESecurityCapabilities:
					s1ap_process_NRUESecurityCapabilities(
					p_uecc_gb_context, &message_map,
					p_protocol_IE->value.u._UEContextModificationRequestIEs_16,
					&p_intrl->nr_security_cap, &p_intrl->bitmask,
					RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_NR_SECURITY_CAP_PRESENT,index);
					break;
                default:
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list,
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            p_send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);
            }

            p_node= p_node->next;            
        }                    
    } while (0);

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                p_asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_s1ap_id_UEContextModification,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                p_error_ind_content))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "Error Indication Detected");
        resp = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return resp;
}

/* CDMA + CSFB Changes start */
/****************************************************************************
 * Function Name  : rrc_s1ap_downlink_s1_cdma2000_tunneling_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  OSCTXT *p_asn1_ctx
 *                  U8 mme_id
 *                  s1ap_DownlinkS1cdma2000tunneling *p_3gpp
 *                  rrc_s1ap_downlink_s1_cdma2000_tunneling_t *p_intrl
 *                  rrc_s1ap_error_indication_t *p_error_ind_content
 *                  uecc_s1ap_error_ind_bool_t *p_send_err_ind
 * Outputs        : p_intrl (rrc_s1ap_downlink_s1_cdma2000_tunneling_t)
 * Returns        : s1ap_Cause
 * Description    : 
 ****************************************************************************/
rrc_return_et rrc_s1ap_downlink_s1_cdma2000_tunneling_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 OSCTXT *p_asn1_ctx,
 U8 mme_id,
 s1ap_DownlinkS1cdma2000tunnelling *p_3gpp,    /* unpacked src */
 rrc_s1ap_downlink_s1_cdma2000_tunneling_t *p_intrl,/* dest */
 rrc_s1ap_error_indication_t *p_error_ind_content,
 uecc_s1ap_error_ind_bool_t *p_send_err_ind
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_DownlinkS1cdma2000tunnelling_protocolIEs_element
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    rrc_return_et resp = RRC_SUCCESS;

    uecc_s1ap_message_data_t message_map =
    {6, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{ 0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 2, ASN1V_s1ap_id_E_RABSubjecttoDataForwardingList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 3, ASN1V_s1ap_id_cdma2000HOStatus, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 4, ASN1V_s1ap_id_cdma2000RATType, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         { 5, ASN1V_s1ap_id_cdma2000PDU, s1ap_mandatory, s1ap_reject,RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}
        }
    };

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);


    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    p_intrl->bitmask = 0;

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                ( s1ap_DownlinkS1cdma2000tunnelling_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_MME_UE_S1AP_ID,
                                    &p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_1,
                       	            (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_1;
                        }
                    break;
                    }

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_eNB_UE_S1AP_ID,
                                    &p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id = 
                                p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_2;
                        }
                    break;
                    }

                case ASN1V_s1ap_id_E_RABSubjecttoDataForwardingList:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_E_RABSubjecttoDataForwardingList,
                                    p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->erab_subject_to_data_forwarding_list = 
                                *p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_3;
                            p_intrl->bitmask |= RRC_S1AP_DL_CDMA2000_TUNNELING_ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT;
                        }
                    break;
                    }

	        case ASN1V_s1ap_id_cdma2000HOStatus:
			{
			 
			   if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_cdma2000HOStatus,
                                    &p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->cdma2000_ho_status = 
                                p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_4;
                            p_intrl->bitmask |= RRC_S1AP_DL_CDMA2000_TUNNELING_HO_STATUS_PRESENT;
                        }
                    }
                    break;
	
               case ASN1V_s1ap_id_cdma2000RATType:
		{
		   if (RRC_SUCCESS == uecc_validate_ie_value(
                                   p_uecc_gb_context,
                                   &message_map,
                                    index,
                                    ASN1V_s1ap_id_cdma2000RATType,
                                    &p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_5,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->cdma2000_rat_type = 
                                p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_5;
                        }
                    }
                    break;
              case ASN1V_s1ap_id_cdma2000PDU:
		{
                    message_map.msg_map[5].presence = s1ap_mandatory;
                    if (RRC_SUCCESS == uecc_validate_ie_value(
                                        p_uecc_gb_context,
                                        &message_map,
                                        index,
                                        (U8)p_protocol_IE->id,
                                        p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_6,
                                        (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->cdma2000_pdu = 
                                *(p_protocol_IE->value.u._DownlinkS1cdma2000tunnellingIEs_6);
                        }
                    }
                    break;
  
                default:
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list,
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            p_send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);
            }

            p_node= p_node->next;            
        }                    
    } while (0);

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                p_asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_s1ap_id_DownlinkS1cdma2000tunnelling,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                p_error_ind_content))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "Error Indication Detected");
        resp = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return resp;
}

/* CDMA + CSFB Changes end */

/****************************************************************************
 * Function Name  : rrc_s1ap_erab_modify_request_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8                             mme_id
 *                  s1ap_E_RABModifyRequest        *p_3gpp
 *                  rrc_s1ap_erab_modify_request_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_erab_modify_request_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_erab_modify_request_t from
 *                  s1ap_E_RABModifyRequest
 ****************************************************************************/
rrc_return_et rrc_s1ap_erab_modify_request_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8                             mme_id,
 s1ap_E_RABModifyRequest        *p_3gpp,/* unpacked src */
 rrc_s1ap_erab_modify_request_t *p_intrl/* dest */
 )
{
    OSRTDListNode* p_node = PNULL;

    s1ap_E_RABModifyRequest_protocolIEs_element 
        *p_protocol_IE = PNULL;    

    U32  index = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map = 
    {4, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_E_RABToBeModifiedListBearerModReq, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4, ASN1V_s1ap_id_SecondaryRATDataUsageRequest, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));


    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_E_RABModifyRequest_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABModifyRequestIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._E_RABModifyRequestIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABModifyRequestIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id = 
                                p_protocol_IE->value.u._E_RABModifyRequestIEs_2;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABModifyRequestIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->ue_aggregate_maximum_bitrate = 
                                *(p_protocol_IE->value.u._E_RABModifyRequestIEs_3);
                            p_intrl->bitmask |=
                                RRC_S1AP_ERAB_MODIFY_UE_AGGREGATE_MAX_BITRATE_PRESENT;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_E_RABToBeModifiedListBearerModReq:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABModifyRequestIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->e_rab_to_be_modified_list= 
                                *(p_protocol_IE->value.u._E_RABModifyRequestIEs_4);
                        }

                    }
                    break;
				
				case ASN1V_s1ap_id_SecondaryRATDataUsageRequest:
					{
						if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
									&message_map,
									index,
									(U8)p_protocol_IE->id,
									&p_protocol_IE->value.u._E_RABModifyRequestIEs_5,
									(uecc_ue_context_t *)PNULL))
						{
							p_intrl->secondaryRATDataUsageRequest_flag = 
								p_protocol_IE->value.u._E_RABModifyRequestIEs_5;
						}
					}
					break;

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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
    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_E_RABModify,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " ERAB Modify Request");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* ERB RELEASE COMMAND START */
/****************************************************************************
 * Function Name  : rrc_s1ap_erb_release_command_intrl_dec
 * Inputs         : OSCTXT *p_asn1_ctx
 *                  U32 mme_id
 *                  s1ap_E_RABReleaseCommand* p_3gpp
 *                  rrc_s1ap_erab_release_command_t* p_intrl
 *                  rrc_s1ap_error_indication_t *p_error_ind_content
 *                  uecc_s1ap_error_ind_bool_t *p_send_err_ind
 *                  rrc_bool_et *p_list_populated
 *                  uecc_gb_context_t               *p_gb_context
 * Outputs        : p_intrl (internal rrc_s1ap_erb_release_command_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_e_rab_release_command_t from
 *                  s1ap_E_RABReleaseCommand
 ****************************************************************************/
rrc_return_et rrc_s1ap_erb_release_command_intrl_dec
(
 OSCTXT *p_asn1_ctx,
 U32 mme_id,         /* for memory allocation */
 s1ap_E_RABReleaseCommand* p_3gpp,    /* unpacked src */
 rrc_s1ap_erab_release_command_t* p_intrl, /* dest */
 rrc_s1ap_error_indication_t *p_error_ind_content,
 uecc_s1ap_error_ind_bool_t *p_send_err_ind,
 rrc_bool_et *p_list_populated,
 uecc_gb_context_t               *p_gb_context
 )
{
    OSRTDListNode  *p_node     = PNULL;
    s1ap_E_RABReleaseCommand_protocolIEs_element 
        *p_protocol_IE = PNULL;

    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    rrc_return_t result = RRC_SUCCESS;
    s1ap_error_ind_ie_list_t   iE_list;

    uecc_s1ap_message_data_t message_map =
    {5, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_optional, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_E_RABToBeReleasedList, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4, ASN1V_s1ap_id_NAS_PDU, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));


    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                RRC_UECC_TRACE(p_gb_context->context_index,
                        p_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_E_RABReleaseCommand_protocolIEs_element *)
                p_node->data;       
            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABReleaseCommandIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id =
                                p_protocol_IE->value.u._E_RABReleaseCommandIEs_1;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABReleaseCommandIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id =
                                p_protocol_IE->value.u._E_RABReleaseCommandIEs_2;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABReleaseCommandIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            l3_memcpy_wrapper(&p_intrl->ue_aggregate_maximum_bitrate,
                                    p_protocol_IE->value.u._E_RABReleaseCommandIEs_3,
                                    sizeof(s1ap_UEAggregateMaximumBitrate));
                            p_intrl->bitmask |=
                                RRC_S1AP_ERAB_RELEASE_COMMAND_UE_AGGREGATE_MAXIMUM_BIT_RATE_PRESENT;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_E_RABToBeReleasedList:
                    {
                        if (S1AP_MAX_IES <= p_protocol_IE->value.u._E_RABReleaseCommandIEs_4->count)
                        {
                            uecc_s1ap_build_and_send_err_indication(
                            p_gb_context,
                            RRC_S1AP_MODULE_ID,
                            (U8)mme_id,PNULL);
                            break;
                        }

                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABReleaseCommandIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->e_rab_to_be_released_list = 
                                *(p_protocol_IE->value.u._E_RABReleaseCommandIEs_4);
                            *p_list_populated = RRC_TRUE;

                            /* ID of E_RAB Item is verified in uecc_validate_ie_value function.
                             * No need to check here*/
                        }
                    }
                    break;

                case ASN1V_s1ap_id_NAS_PDU:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABReleaseCommandIEs_5,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            l3_memcpy_wrapper(&p_intrl->nas_pdu,
                                    p_protocol_IE->value.u._E_RABReleaseCommandIEs_5,
                                    sizeof(s1ap_NAS_PDU));
                            p_intrl->bitmask |=
                                RRC_S1AP_ERAB_RELEASE_COMMAND_NAS_PDU_PRESENT;
                        }
                    }
                    break;

                default:
                    RRC_UECC_TRACE(p_gb_context->context_index,
                            p_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);
                    uecc_add_to_err_ind_ie_list(
                            p_gb_context,
                            &iE_list,
                            p_protocol_IE->criticality,
                            p_protocol_IE->id,
                            &ie_list_index,
                            p_send_err_ind,
                            RRC_FALSE,
                            RRC_FALSE);

            }
            p_node= p_node->next;            
        }
    } while (0);

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_gb_context,
                p_asn1_ctx,
                (U8)mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_s1ap_id_E_RABRelease,
                T_S1AP_PDU_initiatingMessage,
                s1ap_reject,
                p_error_ind_content))
    {
        RRC_UECC_TRACE(p_gb_context->context_index,
                p_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " ERAB Release Command");
        result = RRC_FAILURE;
    }

    RRC_UT_TRACE_EXIT();
    return (rrc_return_et)result;
}
/* ERB RELEASE COMMAND END */

/* Trace Start Procedure start */
/****************************************************************************
 * Function Name  : RRC_TRACE_start_intrl_dec
 * Inputs         : uecc_gb_context_t
 *                  s1ap_TraceStart* p_3gpp
 *                  rrc_s1ap_trace_start_t* p_intrl
 *                  s1ap_Cause            *cause
 * Outputs        : p_intrl (internal RRC_TRACE_start_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes RRC_TRACE_start_t from
 *                  s1ap_TraceStart
 ****************************************************************************/
void rrc_s1ap_trace_start_intrl_dec
(
 uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */ 
 s1ap_TraceStart* p_3gpp,    /* unpacked src */
 rrc_s1ap_trace_start_t* p_intrl, /* dest */
 s1ap_Cause            *cause
 )
{
    OSRTDListNode  *p_node = PNULL;

    s1ap_TraceStart_protocolIEs_element *p_protocol_IE = PNULL;
    U32            index = RRC_NULL;
    RRC_UT_TRACE_ENTER();
 
    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);


    cause->t = T_s1ap_Cause_protocol;
    cause->u.protocol = s1ap_unspecified_1;

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {

            if (!p_node)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                cause->u.protocol = s1ap_transfer_syntax_error;
                break;
            }
            p_protocol_IE = (s1ap_TraceStart_protocolIEs_element*)p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    l3_memcpy_wrapper(&p_intrl->mme_ue_s1ap_id,
                            &(p_protocol_IE->value.u._TraceStartIEs_1),
                            sizeof(p_protocol_IE->value.u._TraceStartIEs_1));
                    break;
                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:

                    l3_memcpy_wrapper(&p_intrl->enb_ue_s1ap_id,
                            &(p_protocol_IE->value.u._TraceStartIEs_2),
                            sizeof(p_protocol_IE->value.u._TraceStartIEs_2));

                    break;

                case ASN1V_s1ap_id_TraceActivation:

                    p_intrl->trace_activation = *p_protocol_IE->value.u._TraceStartIEs_3;
                    break;

            }
            p_node = p_node->next;

        }

        cause->t = S1AP_CAUSE_SUCCESS;
    } while (0);

    RRC_UT_TRACE_EXIT();
    return;
}
/* Trace Start Procedure end */

/* Deactivate Trace Procedure start */
/****************************************************************************
 * Function Name  : rrc_s1ap_deactivate_trace_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  OSCTXT*      pctxt
 *                  s1ap_DeactivateTrace* p_3gpp
 *                  rrc_s1ap_deactivate_trace_t* p_intrl
 *                  s1ap_Cause                  *cause
 * Outputs        : p_intrl (internal rrc_s1ap_deactivate_trace_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes rrc_s1ap_deactivate_trace_t from
 *                  s1ap_DeactivateTrace
 ****************************************************************************/
void rrc_s1ap_deactivate_trace_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 s1ap_DeactivateTrace* p_3gpp,    /* unpacked src */
 rrc_s1ap_deactivate_trace_t* p_intrl, /* dest */
 s1ap_Cause                  *cause
 )
{
    OSRTDListNode*                p_node = PNULL;
    s1ap_DeactivateTrace_protocolIEs_element *p_protocol_IE = PNULL;

    U32     index = RRC_NULL;
    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {

            if (!p_node)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                cause->u.protocol = s1ap_transfer_syntax_error;
                break;
            }
            p_protocol_IE = (s1ap_DeactivateTrace_protocolIEs_element*)p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    l3_memcpy_wrapper(&p_intrl->mme_ue_s1ap_id,
                            &(p_protocol_IE->value.u._DeactivateTraceIEs_1),
                            sizeof(p_protocol_IE->value.u._DeactivateTraceIEs_1));
                    break;
                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    l3_memcpy_wrapper(&p_intrl->enb_ue_s1ap_id,
                            &(p_protocol_IE->value.u._DeactivateTraceIEs_2),
                            sizeof(p_protocol_IE->value.u._DeactivateTraceIEs_2));

                    break;
                case ASN1V_s1ap_id_E_UTRAN_Trace_ID:
                    p_intrl->e_utran_trace_id = *p_protocol_IE->value.u._DeactivateTraceIEs_3;
                    break;

            }
            p_node = p_node->next;

        }

        cause->t = S1AP_CAUSE_SUCCESS;
    } while (0);

    RRC_UT_TRACE_EXIT();
    return;
}
/* Deactivate Trace Procedure end */

/****************************************************************************
 * Function Name  : rrc_s1ap_handover_cancel_ack_intrl_dec
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8 mme_id
 *                  s1ap_HandoverCancelAcknowledge *p_3gpp
 *                  rrc_s1ap_handover_cancel_ack_t *p_intrl  
 * Outputs        : p_intrl (internal rrc_s1ap_handover_cancel_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_handover_cancel from
 *                  s1ap_HandoverCancelAcknowledge
 ****************************************************************************/
rrc_return_et rrc_s1ap_handover_cancel_ack_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8 mme_id, 
 s1ap_HandoverCancelAcknowledge *p_3gpp,    /* unpacked src */
 rrc_s1ap_handover_cancel_ack_t *p_intrl/* dest */
 )
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode  *p_node     = PNULL;
    s1ap_HandoverCancelAcknowledge_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map = 
    {3, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_HandoverCancelAcknowledge_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._HandoverCancelAcknowledgeIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._HandoverCancelAcknowledgeIEs_1;
                        }

                    } 
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._HandoverCancelAcknowledgeIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id= 
                                p_protocol_IE->value.u._HandoverCancelAcknowledgeIEs_2;
                        }

                    }
                    break;

                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._HandoverCancelAcknowledgeIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->bitmask |=
                                RRC_S1AP_HO_CANCEL_ACK_CRITICALITY_DIAGNOSTICS_PRESENT;
                            log_uecc_criticality_diagostics(
                                    p_uecc_gb_context,
                                    p_protocol_IE->value.u._HandoverCancelAcknowledgeIEs_3);
                        }

                    }
                    break;
                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_HandoverCancel,
                T_S1AP_PDU_successfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " Handover Cancel Ack");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;    
}

/****************************************************************************
 * Function Name  : rrc_s1ap_get_enb_ue_s1ap_id_from_handover_command
 * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *                  U8                         mme_id
 *                  s1ap_HandoverCommand      *p_3gpp
 *                  s1ap_ENB_UE_S1AP_ID       *p_enb_ue_s1ap_id
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 * Returns        : s1ap_Cause
 * Description    : Gets the enb_ue_s1ap_id  from
 *                  s1ap_HandoverCommand
 ****************************************************************************/
rrc_return_et rrc_s1ap_get_enb_ue_s1ap_id_from_handover_command (
        uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
        U8                         mme_id,
        s1ap_HandoverCommand      *p_3gpp,  /* unpacked src */
        s1ap_ENB_UE_S1AP_ID       *p_enb_ue_s1ap_id  /* dest */
        )
{
    OSRTDListNode                   *p_node = RRC_NULL;
    s1ap_HandoverCommand_protocolIEs_element
        *p_protocol_IE = RRC_NULL;    
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    rrc_return_et result = RRC_SUCCESS;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map = 
    {2, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_HandoverCommand_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {        
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._HandoverCommandIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            /* We will do nothing as this function is used 
                             * only to get enb Id */

                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._HandoverCommandIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            *(p_enb_ue_s1ap_id)= 
                                p_protocol_IE->value.u._HandoverCommandIEs_2;
                        }
                    }
                    break;
                default:
                    break;                  
            }
            p_node= p_node->next;            
        }                    
    }while (0);

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_HandoverPreparation,
                T_S1AP_PDU_successfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " Handover Command");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;    
}

/****************************************************************************
 *   * Function Name  : uecc_s1ap_location_repoting_control_intrl_dec
 *   * Inputs         : uecc_gb_context_t   *p_uecc_gb_context
 *   *                  U8 mme_id
 *                      s1ap_LocationReportingControl                 *p_3gpp
 *                      rrc_s1ap_reporting_control_indication_msg_t   *p_intrl   
 *   * Outputs        : p_ind_msg decoded message
 *   * Returns        : RRC_SUCCESS/RRC_FAILURE
 *   * Description    : decodes the asn message and fills the structure of 
 *   *                  after decoding
 *   ****************************************************************************/
rrc_return_et uecc_s1ap_location_repoting_control_intrl_dec (
        uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
        U8 mme_id,
        s1ap_LocationReportingControl                 *p_3gpp,  /* unpacked src */
        rrc_s1ap_reporting_control_indication_msg_t   *p_intrl  /* dest */
        )
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode* p_node = PNULL;
    s1ap_LocationReportingControl_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    OSCTXT asn1_ctx;

    uecc_s1ap_message_data_t message_map = 
    {3, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_RequestType, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_LocationReportingControl_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._LocationReportingControlIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._LocationReportingControlIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._LocationReportingControlIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id = 
                                p_protocol_IE->value.u._LocationReportingControlIEs_2;
                        }
                    }
                    break;        

                case ASN1V_s1ap_id_RequestType:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._LocationReportingControlIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            /* SPR 19838 : coverity 11320 fix start */
                            if(PNULL ==  p_intrl->p_request_type)
                            {
                                /* SPR 19838 : coverity 11320 fix end */
                                p_intrl->p_request_type = rrc_mem_get (sizeof(s1ap_RequestType));
                                if (PNULL != p_intrl->p_request_type)
                                {
                                    memset_wrapper (p_intrl->p_request_type, RRC_NULL,
                                            sizeof(s1ap_RequestType));
                                    p_intrl->p_request_type->eventType =
                                        p_protocol_IE->value.u._LocationReportingControlIEs_3->eventType;
                                    p_intrl->p_request_type->reportArea =
                                        p_protocol_IE->value.u._LocationReportingControlIEs_3->reportArea;
                                }
                                /* SPR 19838 : coverity 11320 fix start */
                            }
                            /* SPR 19838 : coverity 11320 fix end */
                        }
                    }
                    break;       

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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

    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_LocationReportingControl,
                T_S1AP_PDU_initiatingMessage,
                s1ap_ignore,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " Location Reporting Control ");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_uecc_error_indication_intrl_dec
 * Inputs         : uecc_gb_context_t           *p_uecc_gb_context
 *                  s1ap_ErrorIndication    *p_3gpp
 *                  rrc_s1ap_error_indication_t *p_intrl
 * Outputs        : p_intrl (internal s1ap_ErrorIndication)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_error_indication_t from
 *                  s1ap_ErrorIndication
 ****************************************************************************/
rrc_return_et rrc_uecc_error_indication_intrl_dec
(
 uecc_gb_context_t           *p_uecc_gb_context,  /* UECC global context */
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
    /*To remove warning*/
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
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
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
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
 * Inputs         : OSCTXT      *pctxt
 *                  U8          *p_buff
 *                  U16         *p_buff_size
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_uecc_error_indication_tse_intrl_enc
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
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;

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
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));

                p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + encoded_msg_len);
                p_event_header->event_id = LOCAL_S1_ERROR_INDICATION;
                p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

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
 * Inputs         : OSCTXT      *pctxt
 *                  U8          *p_buff
 *                  U16         *p_buff_size
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer) 
 * Returns        : RRC_SUCCESS / RRC_FAILURE
 * Description    : packs p_error_indication into S1AP_PDU ASN
 ****************************************************************************/
rrc_return_et rrc_uecc_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_s1ap_error_indication_t  *p_error_indication
 )
{
    rrc_return_et result = RRC_FAILURE;
    S1AP_PDU s1ap_pdu;
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
            RRC_TRACE(RRC_ERROR,
                    "ASN1 encoding failed.");
            break;
        }
        else
        {
            *p_buff_size = (U16)pe_GetMsgLen(pctxt);

           
            rrc_asn1PrtToStr_S1AP_PDU(RRC_ASN,"S1AP_PDU",
                    &s1ap_pdu);

            result=RRC_SUCCESS;
        }
    } while(0);
    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_path_switch_req_ack_intrl_dec
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  U8 mme_id
 *                  s1ap_PathSwitchRequestAcknowledge  *p_3gpp
 *                  rrc_s1ap_path_switch_request_ack_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_path_switch_request_ack_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_path_switch_req_ack from
 *                  s1ap_PathSwitchRequestAcknowledge
 ****************************************************************************/
rrc_return_et rrc_s1ap_path_switch_req_ack_intrl_dec
(
 uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
 U8 mme_id,
 s1ap_PathSwitchRequestAcknowledge  *p_3gpp,    /* unpacked src */
 rrc_s1ap_path_switch_request_ack_t *p_intrl    /* dest */
 )
{
    rrc_return_et  result    = RRC_SUCCESS;
    OSRTDListNode  *p_node   = PNULL;
    s1ap_PathSwitchRequestAcknowledge_protocolIEs_element
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    OSCTXT asn1_ctx;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    
    uecc_s1ap_message_data_t message_map = 
    {8, RRC_FALSE, RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_uEaggregateMaximumBitrate, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_E_RABToBeSwitchedULList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {4, ASN1V_s1ap_id_E_RABToBeReleasedList, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {5, ASN1V_s1ap_id_SecurityContext, s1ap_mandatory, s1ap_reject, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {6, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {7, ASN1V_s1ap_id_MME_UE_S1AP_ID_2, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {8, ASN1V_s1ap_id_NRUESecurityCapabilities, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_PathSwitchRequestAcknowledge_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id =
                                p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id =
                                p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_2;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_uEaggregateMaximumBitrate:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->bitmask |=
                                RRC_S1AP_PATH_SWITCH_REQUEST_ACK_UE_AGGREGATE_MAXIMUM_BITRATE_PRESENT;
                            p_intrl->ue_aggregate_maximum_bitrate = 
                                    *p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_3;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_E_RABToBeSwitchedULList:
                    {      
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->bitmask |=    
                                RRC_S1AP_PATH_SWITCH_REQUEST_ACK_E_RAB_TO_BE_SWITCHED_UL_LIST_PRESENT;
                            p_intrl->e_rab_to_be_switched_ul_list = 
                                *p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_4;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_E_RABToBeReleasedList:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_5,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->bitmask |=
                                RRC_S1AP_PATH_SWITCH_REQUEST_ACK_E_RAB_TO_BE_RELEASED_LIST_PRESENT;
                            p_intrl->e_rab_to_be_released_list = 
                                    *p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_5;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_SecurityContext:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_6,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->security_context =
                                    *p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_6;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_7,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->bitmask |=
                                RRC_S1AP_PATH_SWITCH_REQUEST_ACK_CRITICALITY_DIAGNOSTICS_PRESENT;
                            log_uecc_criticality_diagostics( 
                                    p_uecc_gb_context, 
                                    p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_7);
                        }

                    }
                    break;
                case ASN1V_s1ap_id_MME_UE_S1AP_ID_2 :
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    ASN1V_s1ap_id_MME_UE_S1AP_ID_2,
                                    &p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_8,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->bitmask |=
                                RRC_S1AP_PATH_SWITCH_REQUEST_ACK_MME_UE_S1AP_ID2_PRESENT;
                            p_intrl->mme_ue_s1ap_id2 =
                                p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_8;
                                                
                        }
                    }
                    break;

				case ASN1V_s1ap_id_NRUESecurityCapabilities : 
					s1ap_process_NRUESecurityCapabilities(
							p_uecc_gb_context, &message_map,
							p_protocol_IE->value.u._PathSwitchRequestAcknowledgeIEs_15,
							&p_intrl->nr_security_cap, &p_intrl->bitmask,
							RRC_S1AP_PATH_SWITCH_REQUEST_ACK_NR_SECURITY_CAP_PRESENT,index);
					break;

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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

    if (RRC_SUCCESS == uecc_parse_message_map(p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_PathSwitchRequest,
                T_S1AP_PDU_successfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                " ERAB Setup Request");
        result = RRC_FAILURE;
    }
    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_s1ap_path_switch_req_fail_intrl_dec
 * Inputs         : uecc_gb_context_t          *p_uecc_gb_context
 *                  U8 mme_id
 *                  s1ap_PathSwitchRequestFailure          *p_3gpp
 *                  rrc_s1ap_path_switch_request_failure_t *p_intrl
 * Outputs        : p_intrl (internal rrc_s1ap_path_switch_request_ack_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_path_switch_req_failure from
 *                  s1ap_PathSwitchRequestFailure
 ****************************************************************************/
rrc_return_et rrc_s1ap_path_switch_req_fail_intrl_dec
(
 uecc_gb_context_t          *p_uecc_gb_context,  /* UECC global context */
 U8 mme_id,
 s1ap_PathSwitchRequestFailure          *p_3gpp,    /* unpacked src */
 rrc_s1ap_path_switch_request_failure_t *p_intrl    /* dest */
 )
{
    rrc_return_et  result    = RRC_SUCCESS;
    OSRTDListNode  *p_node   = PNULL;
    s1ap_PathSwitchRequestFailure_protocolIEs_element
        *p_protocol_IE = PNULL;
    U32  index = RRC_NULL;
    OSCTXT asn1_ctx;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    
    uecc_s1ap_message_data_t message_map = 
    {4, RRC_FALSE, RRC_TRUE, RRC_NULL, RRC_NULL, RRC_NULL,
        { {0, ASN1V_s1ap_id_MME_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {2, ASN1V_s1ap_id_Cause, s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
          {3, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};
    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_PathSwitchRequestFailure_protocolIEs_element*)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._PathSwitchRequestFailureIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id =
                                p_protocol_IE->value.u._PathSwitchRequestFailureIEs_1;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._PathSwitchRequestFailureIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id =
                                p_protocol_IE->value.u._PathSwitchRequestFailureIEs_2;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_Cause:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestFailureIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->cause =
                                *p_protocol_IE->value.u._PathSwitchRequestFailureIEs_3;
                        }
                    }
                    break;
                case ASN1V_s1ap_id_CriticalityDiagnostics:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._PathSwitchRequestFailureIEs_4,
                                    (uecc_ue_context_t *)PNULL))
                        {
                        p_intrl->bitmask |=
                            RRC_S1AP_PATH_SWITCH_REQUEST_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT;
                        log_uecc_criticality_diagostics(
                            p_uecc_gb_context,
                            p_protocol_IE->value.u._PathSwitchRequestFailureIEs_4);
                        }
                    }
                    break;
                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);
                    uecc_add_to_err_ind_ie_list(p_uecc_gb_context,
                            &iE_list, 
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
    if (RRC_SUCCESS == uecc_parse_message_map(p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_PathSwitchRequest,
                T_S1AP_PDU_unsuccessfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_TRACE(RRC_INFO, "ERROR INDICATION DETECTED,"
                " ERAB Setup Request");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;
}

/* CSR 101813 Fix Start */
/****************************************************************************
 * Function Name  : rrc_s1ap_target_bss_to_source_bss_transparent_container_intrl_dec
 * Inputs         : s1ap_Target_ToSource_TransparentContainer *p_container
 *                  rrc_s1ap_handover_command_t *p_intrl
 *                  OSCTXT  *p_ho_cmd_ctxt
 *                  s1ap_Cause *cause
 * Outputs        : p_intrl (internal rrc_s1ap_handover_command_t)
 *                : cause (s1ap_Cause)
 * Returns        : void
 * Description    : makes ranap_TargetBSS_ToSourceBSS_TransparentContainer from
 *                  s1ap_Target_ToSource_TransparentContainer
 ****************************************************************************/
void rrc_s1ap_target_bss_to_source_bss_transparent_container_intrl_dec
(
 s1ap_Target_ToSource_TransparentContainer *p_container,
 rrc_s1ap_handover_command_t *p_intrl,
 OSCTXT  *p_ho_cmd_ctxt,
 s1ap_Cause *cause
 )
{
    /*SPR_18125_START*/
    /* Code Removed */
    /*SPR_18125_END*/
    OSOCTET msg[S1AP_MAX_ASN1_BUF_LEN];

    RRC_UT_TRACE_ENTER();
    cause->t = T_s1ap_Cause_protocol;

    RRC_ASSERT(PNULL != p_intrl);
    
    do
    {
    	/*SPR_18125_START*/
        memset_wrapper(msg, 0, sizeof(msg));
        l3_memcpy_wrapper(msg, p_container->data, p_container->numocts);
    
	pu_setBuffer(p_ho_cmd_ctxt, msg, 
                p_container->numocts, ASN_S1AP_BUF_TYPE);

        if (RT_OK != 
                asn1PD_ranap_TargetBSS_ToSourceBSS_TransparentContainer(
                    p_ho_cmd_ctxt, 
                    &p_intrl->target_to_source_transparent_container.\
                    target_to_source_transparent_container_for_geran))
    	/*SPR_18125_END*/
        {
            RRC_TRACE(RRC_WARNING, 
                    "[rrc_s1ap_target_bss_to_source_bss_transparent_container_intrl_dec]"
                    "asn1PD_ranap_TargetBSS_ToSourceBSS_TransparentContainer failed.");
            cause->u.protocol = s1ap_abstract_syntax_error_reject;
        }
        if ((cause->u.protocol != s1ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != s1ap_abstract_syntax_error_reject))
        {
            cause->t = S1AP_CAUSE_SUCCESS;
        }

    }while(0);

    RRC_UT_TRACE_EXIT();
}
/* CSR 101813 Fix Stop */

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : rrc_s1ap_erab_modification_cnf_intrl_dec
 * Inputs         : U8                      mme_id
 *                  s1ap_E_RABModificationConfirm *p_3gpp
 *                  rrc_s1ap_erab_modification_cnf_t *p_intrl
 *                  uecc_gb_context_t   *p_uecc_gb_context
 * Outputs        : p_intrl (internal rrc_s1ap_erab_modification_cnf_t)
 * Returns        : s1ap_Cause
 * Description    : makes rrc_s1ap_erab_modification_cnf_t from
 *                  s1ap_E_RABModificationConfirm
 ****************************************************************************/

rrc_return_et rrc_s1ap_erab_modification_cnf_intrl_dec
(
 U8                      mme_id,
 s1ap_E_RABModificationConfirm *p_3gpp,    /* unpacked src */
 rrc_s1ap_erab_modification_cnf_t *p_intrl, /* dest */
 uecc_gb_context_t   *p_uecc_gb_context
 )
{
    OSRTDListNode* p_node = PNULL;
    OSCTXT asn1_ctx;
    s1ap_E_RABModificationConfirm_protocolIEs_element 
        *p_protocol_IE = PNULL;    

    U32  index = RRC_NULL;
    rrc_return_et result = RRC_SUCCESS;
    U16  ie_list_index = 0;
    s1ap_error_ind_ie_list_t   iE_list;
    uecc_s1ap_error_ind_bool_t send_err_ind;
    

    uecc_s1ap_message_data_t message_map = 
    {7, RRC_FALSE,RRC_FALSE, RRC_NULL, RRC_NULL, RRC_NULL,
        {{0, ASN1V_s1ap_id_MME_UE_S1AP_ID,s1ap_mandatory, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {1, ASN1V_s1ap_id_eNB_UE_S1AP_ID,s1ap_mandatory,s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {2, ASN1V_s1ap_id_E_RABModifyListBearerModConf,s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {3, ASN1V_s1ap_id_E_RABFailedToModifyListBearerModConf, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {4, ASN1V_s1ap_id_E_RABToBeReleasedListBearerModConf, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {5, ASN1V_s1ap_id_CriticalityDiagnostics, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL},
         {6, ASN1V_s1ap_id_CSGMembershipStatus, s1ap_optional, s1ap_ignore, RRC_NULL, RRC_NULL, RRC_NULL, RRC_NULL}}};


    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_3gpp);
    RRC_ASSERT(PNULL != p_intrl);
    RRC_ASSERT(PNULL != p_uecc_gb_context);
    memset_wrapper(&iE_list, 0, sizeof(s1ap_error_ind_ie_list_t));
    memset_wrapper(&send_err_ind, 0, sizeof(uecc_s1ap_error_ind_bool_t));

    /* Init ASN context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_ERROR,
                "[s1ap_E_RABModificationConfirm] ASN context initialization failed");
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
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name,
                        RRC_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (s1ap_E_RABModificationConfirm_protocolIEs_element *)
                p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_s1ap_id_MME_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABModificationConfirmIEs_1,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->mme_ue_s1ap_id = 
                                p_protocol_IE->value.u._E_RABModificationConfirmIEs_1;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_eNB_UE_S1AP_ID:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    &p_protocol_IE->value.u._E_RABModificationConfirmIEs_2,
                                    (uecc_ue_context_t *)PNULL))
                        {
                            p_intrl->enb_ue_s1ap_id= 
                                p_protocol_IE->value.u._E_RABModificationConfirmIEs_2;
                        }
                    }
                    break;

                case ASN1V_s1ap_id_E_RABModifyListBearerModConf:
                    {
                        if (RRC_SUCCESS == uecc_validate_ie_value(
                                    p_uecc_gb_context,
                                    &message_map,
                                    index,
                                    (U8)p_protocol_IE->id,
                                    p_protocol_IE->value.u._E_RABModificationConfirmIEs_3,
                                    (uecc_ue_context_t *)PNULL))
                        {

                            p_intrl->erab_modify_list_cnf = 
                                *(p_protocol_IE->value.u._E_RABModificationConfirmIEs_3);

                            p_intrl->bitmask |=
                                RRC_S1AP_ERAB_MODIFICATION_CNF_ERAB_MODIFY_LIST_CNF_PRESENT;
                        }
                    }
                    break;

                default:
                    RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                            p_uecc_gb_context->facility_name,
                            RRC_WARNING,"%s:"
                            " invalid ProtocolElement IE id",__FUNCTION__);

                    uecc_add_to_err_ind_ie_list(
                            p_uecc_gb_context,
                            &iE_list, 
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
    if (RRC_SUCCESS == uecc_parse_message_map(
                p_uecc_gb_context,
                &asn1_ctx,
                mme_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_s1ap_id_E_RABModificationIndication,
                T_S1AP_PDU_successfulOutcome,
                s1ap_reject,
                (rrc_s1ap_error_indication_t *)PNULL))
    {
        RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                p_uecc_gb_context->facility_name,
                RRC_INFO, "ERROR INDICATION DETECTED,"
                " ERAB Modification Confirmation");
        result = RRC_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    RRC_UT_TRACE_EXIT();
    return result;

}
#endif
/* OPTION3X Changes End */
