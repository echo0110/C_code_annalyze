/******************************************************************************
*
*   FILE NAME:
*       rrc_s1ap_asn_intrl_3gpp.h
*
*   DESCRIPTION:
*       This file contains ASN.1 based types for internal representation
*       of S1AP messages in more regular way.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   30 Oct 2009     AlexK       ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_S1AP_ASN_INTRL_3GPP_H_
#define _RRC_S1AP_ASN_INTRL_3GPP_H_

#include "rrc_defines.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_asn_enc_dec.h"
#include "ranap_asn_enc_dec.h"
#include "rrc_rrm_intf.h"
#include "lppa_asn_enc_dec_3gpp.h"

typedef enum
{
	NR
}rrc_s1ap_Secondary_Rat_Type_et;
#define MAX_NUM_OF_TIME_PERIODE 2
#define MAX_NUM_OF_ERAB 256

typedef struct
{
	U8 start_time_stamp[4];
	U8 end__time_stamp[4];
	U64 ul_usage_count;
	U64 dl_usage_count;
}rrc_s1ap_erab_usage_report_item_t;


typedef struct
{
	U16  count;
	rrc_s1ap_erab_usage_report_item_t erab_usage_report_item[MAX_NUM_OF_TIME_PERIODE];	
}rrc_s1ap_erab_usage_report_list_t;

typedef struct
{
	U8 erab_id;
	rrc_s1ap_Secondary_Rat_Type_et secondary_rat_type;
	rrc_s1ap_erab_usage_report_list_t erab_usage_report_list;
	
}rrc_s1ap_SecondaryRATDataUsageReport_ele_t;

typedef struct
{
	U16  count;
	rrc_s1ap_SecondaryRATDataUsageReport_ele_t rrc_s1ap_SecondaryRATDataUsageReport_ele[MAX_NUM_OF_ERAB];
}rrc_s1ap_SecRATDataReport_list_t;

typedef struct
{
	/* Security capabilities - Integrity protection algorithm bits*/
#define UECC_INTEGRITY_PROTECTION_NIA0_BIT 0x01
#define UECC_INTEGRITY_PROTECTION_NIA1_BIT 0x02
#define UECC_INTEGRITY_PROTECTION_NIA2_BIT 0x04
#define UECC_INTEGRITY_PROTECTION_NIA3_BIT 0x08

	
	/* Security capabilities - Ciphering protection algorithm bits*/
#define UECC_CIPHERING_NEA0_BIT 0x01
#define UECC_CIPHERING_NEA1_BIT 0x02
#define UECC_CIPHERING_NEA2_BIT 0x04
#define UECC_CIPHERING_NEA3_BIT 0x08


	U8 ue_NR_Encryption_Algo ;
	U8 ue_NR_Integrity_Protection_Algo;
}rrc_s1ap_Nr_Security_Cap_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_TRACE_ACTIVATION_PRESENT              0x01
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_HANDOVER_RESTRICTION_LIST_PRESENT     0x02
/*SPR_17893_START*/
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_UE_RADIO_CAPABILITY_PRESENT           0x04
/*SPR_17893_END*/
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT 0x08
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CS_FALLBACK_INDICATOR_PRESENT         0x10
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT      0x20
/*Bug_16016 fix starts */
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT         0x40
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_REGISTERED_LAI_PRESENT                0x80
/*Bug_16016 fix ends */
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_GUMMEI_PRESENT         		    0x100
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_MME_UE_S1AP_ID2_PRESENT              0x200
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_NR_SECURITY_CAP_PRESENT              0x400


    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id;
    s1ap_UEAggregateMaximumBitrate   ue_aggregate_maximum_bitrate;
    s1ap_E_RABToBeSetupListCtxtSUReq e_rab_to_be_setup_list;
    s1ap_UESecurityCapabilities      ue_security_capabilities;
    s1ap_SecurityKey                 security_key;
    s1ap_TraceActivation             trace_activation;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_TRACE_ACTIVATION_PRESENT ^*/

    s1ap_HandoverRestrictionList     handover_restriction_list;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_HANDOVER_RESTRICTION_LIST_PRESENT ^*/

/*SPR_17893_START*/
    s1ap_UERadioCapability           ue_radio_capability;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_UE_RADIO_CAPABILITY_PRESENT ^*/
/*SPR_17893_END*/

    s1ap_SubscriberProfileIDforRFP   subscriber_profile_id_for_rfp;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT ^*/

    s1ap_CSFallbackIndicator         cs_fallback_indicator;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CS_FALLBACK_INDICATOR_PRESENT ^*/

    s1ap_SRVCCOperationPossible      srvcc_operation_possible;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT ^*/

/*Bug_16016 fix starts */
    s1ap_CSGMembershipStatus         csg_membership_status;
/*^ O,RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT ^*/
/* CR_761_start */
    s1ap_LAI                         registered_lai;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_REQUEST_REGISTERED_LAI_PRESENT ^*/
/* CR_761_stop */
/*Bug_16016 fix ends */

/*36413_CR0798_start*/ 
    s1ap_GUMMEI 		     gummei;

    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id2;
/*36413_CR0798_stop*/ 
	rrc_s1ap_Nr_Security_Cap_t		nr_security_cap;

} rrc_s1ap_initial_context_setup_request_t; /* 9.1.4.1 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_RESPONSE_E_RAB_FAILED_TO_SETUP_LIST_PRESENT 0x01
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT    0x02

    s1ap_MME_UE_S1AP_ID          mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID          enb_ue_s1ap_id;	
    s1ap_E_RABSetupListCtxtSURes e_rab_setup_list;
    s1ap_E_RABList e_rab_failed_to_setup_list;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_RESPONSE_E_RAB_FAILED_TO_SETUP_LIST_PRESENT ^*/

    s1ap_CriticalityDiagnostics  criticality_diagnostics;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

} rrc_s1ap_initial_context_setup_response_t; /* 9.1.4.2 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_INITIAL_CONTEXT_SETUP_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT 0x01

    s1ap_MME_UE_S1AP_ID         mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID         enb_ue_s1ap_id;
    s1ap_Cause                  cause;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
/*^ O, RRC_S1AP_INITIAL_CONTEXT_SETUP_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

} rrc_s1ap_initial_context_setup_failure_t; /* 9.1.4.3 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_CONTEXT_RELEASE_REQUEST_GW_RELEASE_INDICATION 0x01
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    s1ap_Cause           cause;
    s1ap_GWContextReleaseIndication gw_context_release_indication;
} rrc_s1ap_ue_context_release_request_t; /* 9.1.4.5 */

typedef struct
{
    s1ap_UE_S1AP_IDs    ue_s1ap_ids;
    s1ap_Cause          cause;
} rrc_s1ap_ue_context_release_command_t; /* 9.1.4.6 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT 0x01
#define RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT 0x02
#define RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT 0x04
#define RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT 0x08

    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
/*^ O, RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT ^*/
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
/*^ O, RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT ^*/
    s1ap_Cause           cause;
/*^ O, RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT ^*/
    s1ap_CriticalityDiagnostics criticality_diagnostics;
/*^ O, RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

} rrc_s1ap_error_indication_t; 

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_LPPA_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT 0x01
#define RRC_LPPA_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT 0x02
#define RRC_LPPA_ERROR_INDICATION_ROUTING_ID_PRESENT     0x04
#define RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT          0x08
#define RRC_LPPA_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT 0x10

    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
/*^ O, RRC_LPPA_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT ^*/
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
/*^ O, RRC_LPPA_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT ^*/
    U8             routing_id;
/*^ O, RRC_LPPA_ERROR_INDICATION_ROUTING_ID_PRESENT ^*/
    lppa_Cause           cause;
/*^ O, RRC_LPPA_ERROR_INDICATION_CAUSE_PRESENT ^*/
    lppa_CriticalityDiagnostics criticality_diagnostics;
/*^ O, RRC_LPPA_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

}rrc_lppa_error_indication_t; 

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_UE_CONTEXT_RELEASE_COMPLETE_CRITICALITY_DIAGNOSTICS_PRESENT 0x01
#define RRC_S1AP_UE_CONTEXT_RELEASE_COMPLETE_SECONDARY_REPORT_PRESENT 0x02

    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
/*^ O, RRC_S1AP_UE_CONTEXT_RELEASE_COMPLETE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
	rrc_s1ap_SecRATDataReport_list_t	 secondary_rat_data_usage_report_list;
	/*^ O, RRC_S1AP_UE_CONTEXT_RELEASE_COMPLETE_SECONDARY_REPORT_PRESENT ^*/	
} rrc_s1ap_ue_context_release_complete_t; /* 9.1.4.7 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_INITIAL_UE_MESSAGE_S_TMSI_PRESENT        0x01
#define RRC_S1AP_INITIAL_UE_MESSAGE_CSG_ID_PRESENT        0x02
#define RRC_S1AP_INITIAL_CELL_ACCESS_MODE_PRESENT         0x04
/*lipa start */
#define RRC_S1AP_TRANSPORT_LAYER_ADDRESS_PRESENT          0x08 
/*lipa end */
/* SPR 13807 Fix Start */
#define RRC_S1AP_INITIAL_UE_MESSAGE_GUMMEI_PRESENT        0x10 
/* SPR 13807 Fix Stop */
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id;
    s1ap_NAS_PDU        nas_pdu;
    s1ap_TAI            tai;
    s1ap_EUTRAN_CGI     eutran_cgi;
    s1ap_RRC_Establishment_Cause  rrc_establishment_cause;
    s1ap_S_TMSI         s_tmsi;
/*^ O, RRC_S1AP_INITIAL_UE_MESSAGE_S_TMSI_PRESENT ^*/

    s1ap_CSG_Id         csg_id;
/*^ O, RRC_S1AP_INITIAL_UE_MESSAGE_CSG_ID_PRESENT ^*/
    /* SPR 13807 Fix Start */
    s1ap_GUMMEI 		     gummei;
/*^ O, RRC_S1AP_INITIAL_UE_MESSAGE_GUMMEI_PRESENT ^*/
    /* SPR 13807 Fix Stop */
    s1ap_CellAccessMode   cell_access_mode;
/*^ O, RRC_S1AP_INITIAL_CELL_ACCESS_MODE_PRESENT ^*/
/* lipa start */
    s1ap_TransportLayerAddress transport_layer_address; 
/*^ O, RRC_S1AP_TRANSPORT_LAYER_ADDRESS_PRESENT ^*/
/* lipa end */

} rrc_s1ap_initial_ue_message_t; /* 9.1.7.1 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_DOWNLINK_NAS_TRANSPORT_HANDOVER_RESTRICTION_LIST_PRESENT 0x01
#define RRC_S1AP_DOWNLINK_NAS_TRANSPORT_SUBSCRIBER_PROFILE_ID_PRESENT     0x02
#define RRC_S1AP_DOWNLINK_NAS_TRANSPORT_NR_SECURITY_CAP_PRESENT     0x04

    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id;
    s1ap_NAS_PDU        nas_pdu;
    s1ap_HandoverRestrictionList handover_restriction_list;
/*^ O, RRC_S1AP_DOWNLINK_NAS_TRANSPORT_HANDOVER_RESTRICTION_LIST_PRESENT ^*/

    s1ap_SubscriberProfileIDforRFP subs_prof_id;
/*^ O, RRC_S1AP_DOWNLINK_NAS_TRANSPORT_SUBSCRIBER_PROFILE_ID_PRESENT ^*/
	rrc_s1ap_Nr_Security_Cap_t		nr_security_cap;

} rrc_s1ap_downlink_nas_transport_t; /* 9.1.7.2 */

typedef struct
{
    /*lipa start */
    rrc_bitmask_t        bitmask;
#define RRC_LGW_TRANSPORT_LAYER_ADDRESS_PRESENT 0x01
    /* lipa end */
    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id;
    s1ap_NAS_PDU        nas_pdu;
    s1ap_EUTRAN_CGI     eutran_cgi;
    s1ap_TAI            tai;
    /*lipa start */
    s1ap_TransportLayerAddress transport_layer_address; 
/*^ O, RRC_LGW_TRANSPORT_LAYER_ADDRESS_PRESENT ^*/
    /* lipa end */
} rrc_s1ap_uplink_nas_transport_t; /* 9.1.7.3 */

typedef struct
{
    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id;
    s1ap_NAS_PDU        nas_pdu;
    s1ap_Cause          cause;
} rrc_s1ap_nas_non_delivery_indication_t; /* 9.1.7.4 */

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_UERadioCapability  ue_radio_capability;
} rrc_s1ap_ue_capability_info_indication_t; /* 9.1.10 */


typedef struct
{
    U8  count;
    OSCTXT      asn1_ctxt[2];
}uecc_container_asn1_ctxt_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_HO_COMMAND_PRESENT  0x01
/* added for i-Rat HO */
#define RRC_S1AP_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_FOR_GERAN_PRESENT   0x02
#define RRC_S1AP_TARGET_RNC_TO_SOURCE_RNC_TRANSPARENT_CONTAINER_PRESENT     0x04

    HandoverCommand ho_command;
    /* Added for i-Rat HO */
    OSDynOctStr     target_to_source_transparent_container_for_geran;
    /* CSR 101813 Fix Start */
    ranap_TargetRNC_ToSourceRNC_TransparentContainer  targetRNC_ToSourceRNC_TransparentContainer;
    /* CSR 101813 Fix Stop */
}target_to_source_transparent_container_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_COMMAND_NAS_SECURITY_PARAMETERS_FROM_E_UTRAN_PRESENT  0x01
#define RRC_S1AP_HANDOVER_COMMAND_ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT  0x02
#define RRC_S1AP_HANDOVER_COMMAND_E_RAB_FAILED_LIST_PRESENT  0x04
#define RRC_S1AP_HANDOVER_COMMAND_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_SECONDARY_PRESENT  0x08
#define RRC_S1AP_HANDOVER_COMMAND_CRITICALITY_DIAGNOSTICS_PRESENT  0x10

    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_HandoverType                  handover_type;
    s1ap_NASSecurityParametersfromE_UTRAN      nas_security_param_from_E_UTRAN; 
    s1ap_E_RABSubjecttoDataForwardingList          erab_subject_to_data_forwarding_list;
    s1ap_E_RABList                                              erab_failed_list;
    target_to_source_transparent_container_t   target_to_source_transparent_container;
    s1ap_Target_ToSource_TransparentContainer   target_to_source_transparent_container_secondary;
    s1ap_CriticalityDiagnostics     criticality_diagnostics;
}rrc_s1ap_handover_command_t;

/* test buffer generation */
typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_COMMAND_NAS_SECURITY_PARAMETERS_FROM_E_UTRAN_PRESENT  0x01
#define RRC_S1AP_HANDOVER_COMMAND_ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT  0x02
#define RRC_S1AP_HANDOVER_COMMAND_E_RAB_FAILED_LIST_PRESENT  0x04
#define RRC_S1AP_HANDOVER_COMMAND_TARGET_TO_SOURCE_TRANSPARENT_CONTAINER_SECONDARY_PRESENT  0x08
#define RRC_S1AP_HANDOVER_COMMAND_CRITICALITY_DIAGNOSTICS_PRESENT  0x10

    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_HandoverType                  handover_type;
    s1ap_NASSecurityParametersfromE_UTRAN      nas_security_param_from_E_UTRAN; 
    s1ap_E_RABSubjecttoDataForwardingList          erab_subject_to_data_forwarding_list;
    s1ap_E_RABList                                              erab_failed_list;
    s1ap_Target_ToSource_TransparentContainer   target_to_source_transparent_container;
    s1ap_Target_ToSource_TransparentContainer   target_to_source_transparent_container_secondary;
    s1ap_CriticalityDiagnostics     criticality_diagnostics;
}rrc_s1ap_handover_command_test_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_PREPARATION_FAILURE_DIAGNOSTICS_PRESENT  0x01

    s1ap_MME_UE_S1AP_ID        mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID        enb_ue_s1ap_id;
    s1ap_Cause                         cause;
    s1ap_CriticalityDiagnostics    criticality_diagnostics;
/*^ O, RRC_S1AP_HANDOVER_PREPARATION_FAILURE_DIAGNOSTICS_PRESENT  ^*/
}rrc_s1ap_handover_preparation_failure_t;

typedef struct
{
    U8  count;
    s1ap_LastVisitedCell_Item lastVisited_cell_item[MAX_NO_OF_CELLS_UE_HISTORY];
}rrc_ue_history_information_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_SOURCE_ENB_TO_TARGET_ENB_E_RAB_INFO_LIST_PRESENT  0x01
#define RRC_SOURCE_ENB_TO_TARGET_ENB_SUBSCRIBER_PROFILER_ID_FOR_RFP_PRESENT 0x02

   HandoverPreparationInformation_r8_IEs rRC_Container;
   s1ap_E_RABInformationList e_RABInformationList;
   s1ap_EUTRAN_CGI targetCell_ID;
   s1ap_SubscriberProfileIDforRFP subscriberProfileIDforRFP;
   rrc_ue_history_information_t   ue_HistoryInformation;
}rrc_source_enb_to_target_enb_transparent_container_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_SOURCE_ENB_TO_TARGET_ENB_TRANSPARENT_CONTAINER_PRESENT  0x01

    rrc_source_enb_to_target_enb_transparent_container_t source_enb_to_target_enb_transparent_container;
}rrc_source_to_target_transparent_container_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_REQUEST_HO_RESTRICTION_LIST_PRESENT  0x01
#define RRC_S1AP_HANDOVER_REQUEST_TRACE_ACTIVATION_PRESENT  0x02
#define RRC_S1AP_HANDOVER_REQUEST_REQUEST_TYPE_PRESENT  0x04
#define RRC_S1AP_HANDOVER_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT  0x08
#define RRC_S1AP_HANDOVER_REQUEST_NAS_SECURITY_PARAMETERS_TO_E_UTRAN_PRESENT  0x10
    /* CR_592_start */ 
#define RRC_S1AP_HANDOVER_REQUEST_CSG_ID_PRESENT 0x20
#define RRC_S1AP_HANDOVER_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT 0x40
    /* CR_592_stop */
   /*36413_CR0798_start*/
    /* SPR 15180 Fix Start */
#define RRC_S1AP_HANDOVER_REQUEST_GUMMEI_PRESENT 0x100
#define RRC_S1AP_HANDOVER_REQUEST_MME_UE_S1AP_ID2_PRESENT 0x200
#define RRC_S1AP_HANDOVER_REQUEST_NR_SECURITY_CAP_PRESENT 0x400

    /* SPR 15180 Fix Stop */
   /*36413_CR0798_stop*/

    s1ap_MME_UE_S1AP_ID        mme_ue_s1ap_id;
    s1ap_HandoverType              handover_type;
    s1ap_Cause                         cause;
    s1ap_UEAggregateMaximumBitrate    ue_aggregate_maximum_bitrate;
    s1ap_E_RABToBeSetupListHOReq      e_rab_to_be_setup_list; 
    rrc_source_to_target_transparent_container_t  source_to_target_transparent_container;
    s1ap_UESecurityCapabilities     ue_security_capabilities;
    s1ap_HandoverRestrictionList    ho_restriction_list;
    s1ap_TraceActivation               trace_activation;
    s1ap_RequestType                   request_type;
    s1ap_SRVCCOperationPossible  srvcc_operation_possible;
    s1ap_SecurityContext               security_context ;
    s1ap_NASSecurityParameterstoE_UTRAN    nas_security_parameters_to_e_utran;
    /* CR_592_start */ 
    s1ap_CSG_Id                     csg_id;
    s1ap_CSGMembershipStatus        csg_membership_status; 
    /* CR_592_stop */
    /*36413_CR0798_start*/
    s1ap_GUMMEI                      gummei;
    s1ap_MME_UE_S1AP_ID        	     mme_ue_s1ap_id2;
    /*36413_CR0798_start*/
	rrc_s1ap_Nr_Security_Cap_t		nr_security_cap;
	/*^ O, RRC_S1AP_HANDOVER_REQUEST_NR_SECURITY_CAP_PRESENT ^*/		
}rrc_s1ap_handover_request_t;

/*** For test buffer generation*/
typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_REQUEST_HO_RESTRICTION_LIST_PRESENT  0x01
#define RRC_S1AP_HANDOVER_REQUEST_TRACE_ACTIVATION_PRESENT  0x02
#define RRC_S1AP_HANDOVER_REQUEST_REQUEST_TYPE_PRESENT  0x04
#define RRC_S1AP_HANDOVER_REQUEST_SRVCC_OPERATION_POSSIBLE_PRESENT  0x08
#define RRC_S1AP_HANDOVER_REQUEST_NAS_SECURITY_PARAMETERS_TO_E_UTRAN_PRESENT  0x10
    /* CR_592_start */ 
#define RRC_S1AP_HANDOVER_REQUEST_CSG_ID_PRESENT 0x20
#define RRC_S1AP_HANDOVER_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT 0x40
    /* CR_592_stop */
    s1ap_MME_UE_S1AP_ID        mme_ue_s1ap_id;
    s1ap_HandoverType              handover_type;
    s1ap_Cause                         cause;
    s1ap_UEAggregateMaximumBitrate    ue_aggregate_maximum_bitrate;
    s1ap_E_RABToBeSetupListHOReq      e_rab_to_be_setup_list; 
    s1ap_Source_ToTarget_TransparentContainer  source_to_target_transparent_container;
    s1ap_UESecurityCapabilities     ue_security_capabilities;
    s1ap_HandoverRestrictionList    ho_restriction_list;
    s1ap_TraceActivation               trace_activation;
    s1ap_RequestType                   request_type;
    s1ap_SRVCCOperationPossible  srvcc_operation_possible;
    s1ap_SecurityContext               security_context ;
    s1ap_NASSecurityParameterstoE_UTRAN    nas_security_parameters_to_e_utran;
    /* CR_592_start */ 
    s1ap_CSG_Id                     csg_id;
    s1ap_CSGMembershipStatus        csg_membership_status; 
    /* CR_592_stop */
}rrc_s1ap_handover_request_test_t;
/*** For test buffer generation*/

typedef struct
{
    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_ENB_StatusTransfer_TransparentContainer  e_nb_status_transfer_transparent_container;
}rrc_s1ap_mme_status_transfer_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_ENB_StatusTransfer_TransparentContainer  enb_status_transfer_transparent_container;
}rrc_s1ap_enb_status_transfer_t;


typedef struct
{
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    s1ap_Cause           cause;
} rrc_s1ap_handover_cancel_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_REQUEST_ACK_FAILED_TO_SETUP_LIST_PRESENT 0x01
#define RRC_S1AP_HANDOVER_REQUEST_ACK_DIAGNOSTICS_PRESENT  0x02
/*CR_592_start*/
#define RRC_S1AP_HANDOVER_REQUEST_ACK_CSG_ID_PRESENT  0x04
/*CR_592_stop*/

     s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
     s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
     s1ap_E_RABAdmittedList  e_rab_admitted_list;
     s1ap_E_RABFailedtoSetupListHOReqAck  e_rab_failed_to_setup_list;
     s1ap_Target_ToSource_TransparentContainer  target_to_source_transparent_container;
/*CR_592_start*/
     s1ap_CSG_Id                     csg_id;
/*CR_592_stop*/
     s1ap_CriticalityDiagnostics criticality_diagnostics;
} rrc_s1ap_handover_request_ack_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_REQUIRED_DIRECT_FORWARDING_PATH_AVAILABILITY_PRESENT 0x01
#define RRC_S1AP_HANDOVER_REQUIRED_SRVCC_HO_INDICATION_PRESENT 0x02
#define RRC_S1AP_HANDOVER_REQUIRED_SOURCE_TO_TARGET_TRANSPARENT_CONTAINER_SECONDARY_PRESENT 0x04
#define RRC_S1AP_HANDOVER_REQUIRED_MS_CLASSMARK2_PRESENT 0x08
#define RRC_S1AP_HANDOVER_REQUIRED_MS_CLASSMARK3_PRESENT 0x10
/*CR_407_START*/
#define RRC_S1AP_HANDOVER_REQUIRED_PS_SERVICE_NOT_AVAILABLE_PRESENT 0x20
/*CR_407_STOP*/
    /* CR_592_start*/
#define RRC_S1AP_HANDOVER_REQUIRED_CSG_ID_PRESENT           0x40
#define RRC_S1AP_HANDOVER_REQUIRED_CELL_ACCESS_MODE_PRESENT 0x80
    /* CR_592_stop*/
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    s1ap_HandoverType    handover_type;
    s1ap_Cause           cause;
    s1ap_TargetID        target_id;
    s1ap_Direct_Forwarding_Path_Availability  direct_forwarding_path_availability;
    s1ap_SRVCCHOIndication  srvcc_ho_indication;
    s1ap_Source_ToTarget_TransparentContainer  source_to_target_transparent_container;
    s1ap_Source_ToTarget_TransparentContainer  source_to_target_transparent_container_secondary;
    s1ap_MSClassmark2  ms_classmark2;
    s1ap_MSClassmark3  ms_classmark3;

    /* CR_592_start*/
    s1ap_CSG_Id           csg_id;
    s1ap_CellAccessMode   cell_access_mode;
    /* CR_592_stop*/

    /*CR_407_START*/
    s1ap_PS_ServiceNotAvailable ps_service_not_available;
    /*CR_407_STOP*/

} rrc_s1ap_handover_required_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_HANDOVER_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT 0x01

    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_Cause                   cause;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
} rrc_s1ap_handover_failure_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    s1ap_EUTRAN_CGI         e_utran_cgi;
    s1ap_TAI                       tai; 
} rrc_s1ap_handover_notify_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SECURITY_KEY_PRESENT    0x01
#define RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT 0x02
#define RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_AGG_MAX_BIT_RATE_PRESENT 0x04
#define RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_CS_FALLBACK_INDICATOR_PRESENT 0x08
#define RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_UE_SECURITY_CAPABILITIES_PRESENT 0x10
#define RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_REGISTERED_LAI_PRESENT      0x20
#define RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT    0x40
#define RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_NR_SECURITY_CAP_PRESENT    0x80

    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id; 
    s1ap_SecurityKey                 security_key;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SECURITY_KEY_PRESENT ^*/

    s1ap_SubscriberProfileIDforRFP subscriber_profile_id_for_rfp;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT ^*/

    s1ap_UEAggregateMaximumBitrate ue_aggregate_maximum_bitrate;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_AGG_MAX_BIT_RATE_PRESENT ^*/

    s1ap_CSFallbackIndicator cs_fallback_indicator;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_CS_FALLBACK_INDICATOR_PRESENT ^*/

    s1ap_UESecurityCapabilities      ue_security_capabilities;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_UE_SECURITY_CAPABILITIES_PRESENT ^*/
/* CR_761_start */
    s1ap_LAI                          registered_lai;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODFICATION_REQUERST_REGISTERED_LAI_PRESENT ^*/
/* CR_761_stop */
    s1ap_CSGMembershipStatus         csg_membership_status; 
    /*^ O, RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_CSG_MEMBERSHIP_STATUS_PRESENT ^*/
	rrc_s1ap_Nr_Security_Cap_t		nr_security_cap;
    /*^ O, RRC_S1AP_UE_CONTEXT_MODIFICATION_REQUEST_NR_SECURITY_CAP_PRESENT ^*/

}rrc_s1ap_ue_context_modification_request_t;
typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_UE_CONTEXT_MODIFICATION_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT 0x01
    
    s1ap_MME_UE_S1AP_ID          mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID          enb_ue_s1ap_id;
    s1ap_CriticalityDiagnostics  criticality_diagnostics;
/*^ O, RRC_S1AP_UE_CONTEXT_MODIFICATION_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
} rrc_s1ap_ue_context_modification_response_t;
typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_UE_CONTEXT_MODIFICATION_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT 0x01

    s1ap_MME_UE_S1AP_ID         mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID         enb_ue_s1ap_id;
    s1ap_Cause                  cause;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
/*^ O, RRC_S1AP_UE_CONTEXT_MODIFICATION_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
} rrc_s1ap_ue_context_modification_failure_t;

typedef struct
{
    rrc_bitmask_t    bitmask;   
#define RRC_S1AP_ERAB_SETUP_REQUEST_UE_AGGREGATE_MAXIMUM_BITRATE_PRESENT 0x01

    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id;
    s1ap_UEAggregateMaximumBitrate   ue_aggregate_maximum_bitrate;
/*^ O, RRC_S1AP_ERAB_SETUP_REQUEST_UE_AGGREGATE_MAXIMUM_BITRATE_PRESENT ^*/

    s1ap_E_RABToBeSetupListBearerSUReq e_rab_to_be_setup_list;

}rrc_s1ap_erab_setup_request_t;

typedef struct
{
    rrc_bitmask_t    bitmask;   
#define RRC_S1AP_ERAB_SETUP_RESPONSE_E_RAB_SETUP_LIST_PRESENT           0x01
#define RRC_S1AP_ERAB_SETUP_RESPONSE_E_RAB_FAILED_TO_SETUP_LIST_PRESENT 0x02
#define RRC_S1AP_ERAB_SETUP_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT    0x04

    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id;

    s1ap_E_RABSetupListBearerSURes e_rab_setup_list;
/*^ O, RRC_S1AP_ERAB_SETUP_RESPONSE_E_RAB_SETUP_LIST_PRESENT ^*/

    s1ap_E_RABList e_rab_failed_to_setup_list;
/*^ O, RRC_S1AP_ERAB_SETUP_RESPONSE_E_RAB_FAILED_TO_SETUP_LIST_PRESENT ^*/

    s1ap_CriticalityDiagnostics  criticality_diagnostics;
/*^ O, RRC_S1AP_ERAB_SETUP_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

}rrc_s1ap_erab_setup_response_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_ERAB_MODIFY_UE_AGGREGATE_MAX_BITRATE_PRESENT 0x01
#define RRC_S1AP_SECONDARY_RAT_DATA_USAGE_REQ_PRESENT 0x01


    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;

    s1ap_UEAggregateMaximumBitrate           ue_aggregate_maximum_bitrate;
/*^ O, RRC_S1AP_ERAB_MODIFY_UEAGGREGATE_MAX_BITRATE_PRESENT ^*/

    s1ap_E_RABToBeModifiedListBearerModReq   e_rab_to_be_modified_list;

	s1ap_SecondaryRATDataUsageRequest secondaryRATDataUsageRequest_flag;
} rrc_s1ap_erab_modify_request_t; /* 9.1.3.3 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_ERAB_MODIFY_RESPONSE_TO_BE_MODIFIED_LIST_PRESENT         0x01
#define RRC_S1AP_ERAB_MODIFY_RESPONSE_E_RAB_FAILED_TO_MODIFY_LIST_PRESENT 0x02
#define RRC_S1AP_ERAB_MODIFY_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT     0x04
#define RRC_S1AP_ERAB_MODIFY_RESPONSE_SECONDARY_REPORT_LIST_PRESENT     0x08

    s1ap_MME_UE_S1AP_ID   mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID   enb_ue_s1ap_id;

    s1ap_E_RABModifyListBearerModRes e_rab_modified_list;
/*^ O, RRC_S1AP_ERAB_MODIFY_RESPONSE_TO_BE_MODIFIED_LIST_PRESENT ^*/

    s1ap_E_RABList                   e_rab_failed_to_modify_list;
/*^ O, RRC_S1AP_ERAB_MODIFY_RESPONSE_E_RAB_FAILED_TO_MODIFY_LIST_PRESENT ^*/

    s1ap_CriticalityDiagnostics      criticality_diagnostics;
/*^ O, RRC_S1AP_ERAB_MODIFY_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

	rrc_s1ap_SecRATDataReport_list_t     secondary_rat_data_usage_report_list;
/*^ O, RRC_S1AP_ERAB_MODIFY_RESPONSE_SECONDARY_REPORT_LIST_PRESENT ^*/

} rrc_s1ap_erab_modify_response_t; /* 9.1.3.4 */

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_ERAB_RELEASE_COMMAND_UE_AGGREGATE_MAXIMUM_BIT_RATE_PRESENT \
 0x01
#define RRC_S1AP_ERAB_RELEASE_COMMAND_NAS_PDU_PRESENT   0x02

    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id;
    
    s1ap_UEAggregateMaximumBitrate   ue_aggregate_maximum_bitrate;
/*^ O, RRC_S1AP_ERAB_RELEASE_COMMAND_UE_AGGREGATE_MAXIMUM_BIT_RATE_PRESENT ^*/

    s1ap_E_RABList                   e_rab_to_be_released_list;

    s1ap_NAS_PDU                     nas_pdu;
/*^ O,  RRC_S1AP_ERAB_RELEASE_COMMAND_NAS_PDU_PRESENT ^*/
} rrc_s1ap_erab_release_command_t; 

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_ERAB_RELEASE_RESPONSE_E_RAB_RELEASE_LIST_PRESENT      0x01
#define RRC_S1AP_ERAB_RELEASE_RESPONSE_E_RAB_FAILED_TO_RELEASE_LIST_PRESENT \
0x02

#define RRC_S1AP_ERAB_RELEASE_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT 0x04
#define RRC_S1AP_ERAB_RELEASE_RESPONSE_SECONDARY_REPORT_PRESENT 0x08

    s1ap_MME_UE_S1AP_ID          mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID          enb_ue_s1ap_id;
    
    s1ap_E_RABReleaseListBearerRelComp e_rab_release_list;
/*^ O, RRC_S1AP_ERAB_RELEASE_RESPONSE_E_RAB_RELEASE_LIST_PRESENT ^*/

    s1ap_E_RABList e_rab_failed_to_release_list;
/*^ O, RRC_S1AP_ERAB_RELEASE_RESPONSE_E_RAB_FAILED_TO_RELEASE_LIST_PRESENT ^*/

    s1ap_CriticalityDiagnostics  criticality_diagnostics;
/*^ O, RRC_S1AP_ERAB_RELEASE_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
	rrc_s1ap_SecRATDataReport_list_t     secondary_rat_data_usage_report_list;
/*^ O, RRC_S1AP_ERAB_RELEASE_RESPONSE_SECONDARY_REPORT_PRESENT ^*/	
} rrc_s1ap_erab_release_response_t; 

typedef struct
{
#define RRC_S1AP_ERAB_RELEASE_INDICATION_SECONDARY_REPORT_PRESENT 0x01
    rrc_bitmask_t        bitmask;

    s1ap_MME_UE_S1AP_ID          mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID          enb_ue_s1ap_id;

    s1ap_E_RABList           e_rab_released_list;
	
	rrc_s1ap_SecRATDataReport_list_t	 secondary_rat_data_usage_report_list;
	/*^ O, RRC_S1AP_ERAB_RELEASE_INDICATION_SECONDARY_REPORT_PRESENT ^*/	

} rrc_s1ap_e_rab_release_indication_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
    #define RRC_S1AP_HO_CANCEL_ACK_CRITICALITY_DIAGNOSTICS_PRESENT 0x01

    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;

    s1ap_CriticalityDiagnostics  criticality_diagnostics;
/*^ O, RRC_S1AP_HO_CANCEL_ACK_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
} rrc_s1ap_handover_cancel_ack_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_Cause              cause;
}rrc_s1ap_failure_report_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_EUTRAN_CGI         eutran_cgi;
    s1ap_TAI                tai;
    s1ap_RequestType        request_type;
}rrc_s1ap_location_report_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_RequestType        *p_request_type;
}rrc_s1ap_reporting_control_indication_msg_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_AREA_LIST_PRESENT  0x01
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_EXTENDED_REPETITION_PERIOD_PRESENT  0x02
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_TYPE_PRESENT  0x04
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_SECURITY_INFO_PRESENT 0x08
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_DATA_CODING_SCHEME_PRESENT  0x10
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_MESSAGE_CONTENTS_PRESENT  0x20
#define RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_CONCURRENT_WARNING_MESSAGE_INDICATOR_PRESENT  0x40

    s1ap_MessageIdentifier           message_identifier;
    s1ap_SerialNumber                serial_number;
    s1ap_WarningAreaList             warning_area_list;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_AREA_LIST_PRESENT ^*/
    s1ap_RepetitionPeriod            repetition_period;
    s1ap_ExtendedRepetitionPeriod    extended_repetition_period;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_EXTENDED_REPETITION_PERIOD_PRESENT ^*/
    s1ap_NumberofBroadcastRequest    numberof_broadcast_request; 
    s1ap_WarningType                 warning_type;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_TYPE_PRESENT ^*/
    s1ap_WarningSecurityInfo         warning_security_info;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_SECURITY_INFO_PRESENT ^*/
    s1ap_DataCodingScheme            data_coding_scheme;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_DATA_CODING_SCHEME_PRESENT ^*/
    s1ap_WarningMessageContents      warning_message_contents;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_MESSAGE_CONTENTS_PRESENT ^*/
     s1ap_ConcurrentWarningMessageIndicator  concurrent_warning_message_indicator;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_CONCURRENT_WARNING_MESSAGE_INDICATOR_PRESENT ^*/     
}rrc_s1ap_write_replace_warning_request_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_WRITE_REPLACE_WARNING_RESPONSE_BROADCAST_COMPLETED_AREA_LIST_PRESENT  0x01
#define RRC_S1AP_WRITE_REPLACE_WARNING_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT  0x02

    s1ap_MessageIdentifier           message_identifier;
    s1ap_SerialNumber                serial_number;
    s1ap_BroadcastCompletedAreaList  broadcast_completed_area_list;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_RESPONSE_BROADCAST_COMPLETED_AREA_LIST_PRESENT ^*/
    s1ap_CriticalityDiagnostics      criticality_diagnostics;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
}rrc_s1ap_write_replace_warning_response_t;

typedef struct
{
        rrc_bitmask_t        bitmask;
#define RRC_S1AP_KILL_REQUEST_WARNING_AREA_LIST_PRESENT  0x01
        
    s1ap_MessageIdentifier           message_identifier;
    s1ap_SerialNumber                serial_number;
    s1ap_WarningAreaList             warning_area_list;
/*^ O, RRC_S1AP_WRITE_REPLACE_WARNING_REQUEST_WARNING_AREA_LIST_PRESENT ^*/
}rrc_s1ap_kill_request_t;


typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_KILL_RESPONSE_BROADCAST_CANCELLED_AREA_LIST_PRESENT  0x01
#define RRC_S1AP_KILL_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT  0x02

    s1ap_MessageIdentifier           message_identifier;
    s1ap_SerialNumber                serial_number;
    s1ap_BroadcastCancelledAreaList  broadcast_cancelled_area_list;
/*^ O, RRC_S1AP_KILL_RESPONSE_BROADCAST_CANCELLED_AREA_LIST_PRESENT ^*/
    s1ap_CriticalityDiagnostics      criticality_diagnostics;
/*^ O, RRC_S1AP_KILL_RESPONSE_CRITICALITY_DIAGNOSTICS_PRESENT ^*/
}rrc_s1ap_kill_response_t;

/* Trace Start Procedure start */
typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_TraceActivation    trace_activation;
} rrc_s1ap_trace_start_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_E_UTRAN_Trace_ID   e_utran_trace_id;
    s1ap_Cause              cause;
} rrc_s1ap_trace_failure_indication_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_E_UTRAN_Trace_ID   e_utran_trace_id;
} rrc_s1ap_deactivate_trace_t;

typedef struct
{
    s1ap_MME_UE_S1AP_ID     mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID     enb_ue_s1ap_id;
    s1ap_E_UTRAN_Trace_ID   e_utran_trace_id;
    s1ap_EUTRAN_CGI         e_utran_cgi;
    s1ap_TransportLayerAddress trace_collection_entity; 
} rrc_s1ap_cell_traffic_trace_t;
/* Trace Start Procedure end */
 
/* CDMA + CSFB Changes start */
typedef struct
{

    rrc_bitmask_t        bitmask;
#define RRC_S1AP_DL_CDMA2000_TUNNELING_ERAB_SUBJECT_TO_DATA_FORWARDING_LIST_PRESENT  0x01
#define RRC_S1AP_DL_CDMA2000_TUNNELING_HO_STATUS_PRESENT  0x02
    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id; 
    s1ap_E_RABSubjecttoDataForwardingList          erab_subject_to_data_forwarding_list;
    s1ap_Cdma2000HOStatus cdma2000_ho_status; 
    s1ap_Cdma2000RATType cdma2000_rat_type;
    s1ap_Cdma2000PDU cdma2000_pdu ;
}rrc_s1ap_downlink_s1_cdma2000_tunneling_t;

typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_UL_CDMA2000_TUNNELING_HO_REQUIRED_IND_PRESENT  0x01
#define RRC_S1AP_UL_CDMA2000_TUNNELING_1X_SRVCC_INFO_PRESENT  0x02
#define RRC_S1AP_UL_CDMA2000_TUNNELING_1X_RAND_PRESENT  0x04
#define RRC_S1AP_UL_CDMA2000_TUNNELING_ROUND_TRIP_DELAY_ESTIMATION_INFO_PRESENT  0x08
    s1ap_MME_UE_S1AP_ID              mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID              enb_ue_s1ap_id; 
    s1ap_Cdma2000RATType cdma2000_rat_type;
    s1ap_Cdma2000SectorID  cdma2000_sector_id;
    s1ap_Cdma2000HORequiredIndication  cdma2000_ho_required_ind;
    s1ap_Cdma2000OneXSRVCCInfo cdma2000_1x_srvcc;
    s1ap_Cdma2000OneXRAND cdma2000_1x_rand;
    s1ap_Cdma2000PDU cdma2000_pdu ;
    s1ap_EUTRANRoundTripDelayEstimationInfo round_trip_delay_estimation_info ;
}rrc_s1ap_uplink_s1_cdma2000_tunneling_t;

/* CDMA + CSFB Changes end*/

/*x2ap-start*/
typedef struct
{
    rrc_bitmask_t        bitmask;
#define RRC_S1AP_PATH_SWITCH_REQUEST_CELL_ACCESS_MODE_PRESENT 0x01 
#define RRC_S1AP_PATH_SWITCH_REQUEST_CSG_ID_PRESENT 0x02
#define RRC_S1AP_PATH_SWITCH_REQUEST_SOURCE_GUMMEI_PRESENT 0x04
#define RRC_S1AP_PATH_SWITCH_REQUEST_NR_SECURITY_CAP_PRESENT                0x08

    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_E_RABToBeSwitchedDLList    erab_to_be_switched_dl_list;
    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_EUTRAN_CGI                 eutran_cgi;
    s1ap_TAI                        tai; 
    s1ap_UESecurityCapabilities     ue_security_capabilities;
/*36413_CR0798_start*/ 
    s1ap_CSG_Id         csg_id;
    s1ap_CellAccessMode   cell_access_mode;
    s1ap_GUMMEI                      gummei;
/*36413_CR0798_stop*/ 
	rrc_s1ap_Nr_Security_Cap_t		nr_security_cap;
} rrc_s1ap_path_switch_request_t;

typedef struct
{
    rrc_bitmask_t                   bitmask;
#define RRC_S1AP_PATH_SWITCH_REQUEST_ACK_UE_AGGREGATE_MAXIMUM_BITRATE_PRESENT   0x01
#define RRC_S1AP_PATH_SWITCH_REQUEST_ACK_E_RAB_TO_BE_SWITCHED_UL_LIST_PRESENT   0x02
#define RRC_S1AP_PATH_SWITCH_REQUEST_ACK_E_RAB_TO_BE_RELEASED_LIST_PRESENT      0x04
#define RRC_S1AP_PATH_SWITCH_REQUEST_ACK_CRITICALITY_DIAGNOSTICS_PRESENT        0x08
#define RRC_S1AP_PATH_SWITCH_REQUEST_ACK_MME_UE_S1AP_ID2_PRESENT                0x10
#define RRC_S1AP_PATH_SWITCH_REQUEST_ACK_NR_SECURITY_CAP_PRESENT                0x20

    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_UEAggregateMaximumBitrate  ue_aggregate_maximum_bitrate;
    s1ap_E_RABToBeSwitchedULList    e_rab_to_be_switched_ul_list; 
    s1ap_E_RABList                  e_rab_to_be_released_list;
    s1ap_SecurityContext            security_context; 
    s1ap_CriticalityDiagnostics     criticality_diagnostics;
    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id2;
	rrc_s1ap_Nr_Security_Cap_t		nr_security_cap;	
} rrc_s1ap_path_switch_request_ack_t;

typedef struct
{
    rrc_bitmask_t                   bitmask;
#define RRC_S1AP_PATH_SWITCH_REQUEST_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT  0x01
    s1ap_MME_UE_S1AP_ID             mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID             enb_ue_s1ap_id;
    s1ap_Cause                      cause;
    s1ap_CriticalityDiagnostics     criticality_diagnostics;
} rrc_s1ap_path_switch_request_failure_t;

/*x2ap-end*/

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
/* E-RAB Modification Confirm Procedure */
typedef struct
{
    rrc_bitmask_t                           bitmask;
#define RRC_S1AP_ERAB_MODIFICATION_CNF_ERAB_MODIFY_LIST_CNF_PRESENT     0x01
    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id;
    s1ap_E_RABModifyListBearerModConf  erab_modify_list_cnf;
}rrc_s1ap_erab_modification_cnf_t;

/* E-RAB Modification Indication Procedure */
typedef struct
{
    rrc_bitmask_t                              bitmask;
#define RRC_S1AP_ERAB_MODIFICATION_IND_ERAB_NOT_TO_BE_MODIFIED_LIST_IND_PRESENT     0x01
    s1ap_MME_UE_S1AP_ID                        mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID                        enb_ue_s1ap_id;
    s1ap_E_RABToBeModifiedListBearerModInd     erab_to_be_modified_list_ind;
    s1ap_E_RABNotToBeModifiedListBearerModInd  erab_not_to_be_modified_list_ind;
}rrc_s1ap_erab_modification_ind_t;

/* SECONDARY RAT DATA USAGE REPORT */
typedef enum
{
	HANDOVER_PREPARATION
}rrc_s1ap_Handover_Flag_et;
typedef struct
{
    rrc_bitmask_t                              bitmask;
#define RRC_S1AP_SECONDARY_RAT_DATA_USAGE_REP_HANDOVER_FLAG_PRESENT     0x01
    s1ap_MME_UE_S1AP_ID                        mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID                        enb_ue_s1ap_id;
    rrc_s1ap_SecRATDataReport_list_t     secondary_rat_data_usage_report_list;
    rrc_s1ap_Handover_Flag_et  handover_flag;
}rrc_s1ap_secondary_rat_data_usage_rep_t;

#endif
/* OPTION3X Changes End */

#endif /* _RRC_S1AP_ASN_INTRL_3GPP_H_ */

