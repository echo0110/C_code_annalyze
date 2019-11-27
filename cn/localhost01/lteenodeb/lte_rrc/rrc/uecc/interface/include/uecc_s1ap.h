/******************************************************************************
*
*   FILE NAME:
*       uecc_s1ap.h
*
*   DESCRIPTION:
*       This is the interface file of the UECC S1AP.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   28 Jul 2009     AndriyT     ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_S1AP_H
#define UECC_S1AP_H

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "uecc_global_ctx.h"
#include "rrc_asn_enc_dec.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"
#include "rrc_uecc_error_indication.h"

/* SPR_7888_fix */
#define S1AP_CAUSE_RADIO_NETWORK_MAX_VALUE 38
#define S1AP_CAUSE_TRANSPORT_MAX_VALUE      1
#define S1AP_CAUSE_NAS_MAX_VALUE            4
#define S1AP_CAUSE_PROTOCOL_MAX_VALUE       6
#define S1AP_CAUSE_MISC_MAX_VALUE           5
/* SPR_7888_fix */
/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_get_enb_s1ap_id_from_ue_s1ap_ids
*
*
*   DESCRIPTION:
*       This function retreives enb_s1ap_id from s1ap_UE_S1AP_IDs
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_s1ap_get_enb_s1ap_id_from_ue_s1ap_ids(
    s1ap_UE_S1AP_IDs    *p_ue_s1ap_ids,
    U32                 *enb_ue_s1ap_id
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_get_mme_s1ap_id_from_ue_s1ap_ids
*
*
*   DESCRIPTION:
*       This function retreives mme_s1ap_id from s1ap_UE_S1AP_IDs
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_s1ap_get_mme_s1ap_id_from_ue_s1ap_ids(
    s1ap_UE_S1AP_IDs    *p_ue_s1ap_ids,
    U32                 *mme_ue_s1ap_id
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_p_rrc_trace_for_cause 
*
*
*   DESCRIPTION:
*
*   RETURNS:
*       
*
******************************************************************************/
void  uecc_s1ap_p_rrc_trace_for_cause(
    uecc_ue_context_t    *p_ue_context,
    s1ap_Cause           cause
);
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_ue_associated_sig_msg_ind
*
*   DESCRIPTION:
*       This function processes S1AP_UE_ASSOCIATED_SIG_MSG_IND
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_ue_associated_sig_msg_ind
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_ue_context_release_command
*
*   DESCRIPTION:
*       This function processes s1ap_UEContextReleaseCommand
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_ue_context_release_command
(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    U8                  mme_id,
    U8                  *buffer,
    /*BUG 604 changes start*/
    U32                 buffer_length,
    U16                 ue_index
    /*BUG 604 changes stop*/
);

/* ERAB SETUP Start */

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_erab_setup_request
*
*   DESCRIPTION:
*       This function processes erab_setup_request
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_erab_setup_request
(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    U8          *p_buff,        /* ASN.1 encoded message */
    /*BUG 604 changes start*/
    U16         buff_size,      /* encoded buf size - just for check*/
    U16         ue_index
    /*BUG 604 changes stop*/

);

/* ERAB SETUP Stop */
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_initial_context_setup_request
*
*   DESCRIPTION:
*       This function processes initial_context_setup_request
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_initial_context_setup_request
(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    s1ap_ue_associated_sig_msg_ind_t *p_s1ap_ue_associated_sig_msg_ind, /* UE Associated Sig Msg Ind */
    /*BUG 604 changes start*/
    U16                 buff_size,  /* encoded buf size - just for check*/
    U16                 ue_index
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_downlink_nas_transport
*
*   DESCRIPTION:
*       This function processes s1ap_DownlinkNASTransport
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_downlink_nas_transport
(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    U8                  mme_id,
    U8                  *buffer,
    /*BUG 604 changes start*/
    U16                 buffer_length,
    U16                 ue_index
    /*BUG 604 changes stop*/
);


/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_del_ctx_msg_ind
*
*   DESCRIPTION:
*       This function processes S1AP_DEL_CONTEXT_MSG_IND
*
*   RETURNS:
*       None
*
******************************************************************************/

void uecc_s1ap_process_del_ctx_msg_ind(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);



/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_status_msg_ind
*
*   DESCRIPTION:
*       This function processes S1AP_STATUS_MSG_IND
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_status_msg_ind(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_ue_associated_mme_update_status_ind
*
*   DESCRIPTION:
*       This function processes S1AP_UE_ASSOCIATED_MME_UPDATE_STATUS_IND 
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_ue_associated_mme_update_status_ind(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_reset_msg_ind
*
*   DESCRIPTION:
*       This function processes S1AP_RESET_MSG_IND
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_reset_msg_ind(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_initial_ue_msg_req
*
*   DESCRIPTION:
*       This function builds and sends RRC_S1AP_INITIAL_UE_MSG_REQ message
*       to S1AP module.
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_initial_ue_msg_req
(
    uecc_ue_context_t   *p_ue_context, /* Pointer to the UECC UE context data */
    U8                  rel_of_mme
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_initial_context_setup_response
*
*   DESCRIPTION:
*       This function builds and sends initial_context_setup_response message
*       to S1AP
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_initial_context_setup_response(
    uecc_ue_context_t* p_ue_context);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_initial_context_setup_failure
*
*   DESCRIPTION:
*       This function builds and sends initial_context_setup_failure message
*       to S1AP
*
*   INPUTS : p_ue_context may contain NULL
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_initial_context_setup_failure(
    uecc_ue_context_t   *p_ue_context,
    U32        mme_ue_s1ap_id,
    U32        enb_ue_s1ap_id,
    s1ap_Cause *p_cause,
    s1ap_CriticalityDiagnostics    *p_criticality_diagnostics
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_enb_ue_ctx_release_req
*
*   DESCRIPTION:
*       This function builds and sends RRC_S1AP_ENB_UE_CTX_RELEASE_REQ message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_enb_ue_ctx_release_req(
    uecc_ue_context_t*      p_ue_context,
    s1ap_Cause              *p_cause
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_ue_ctx_release_comp_resp
*
*   DESCRIPTION:
*       This function builds and sends rrc_s1ap_ue_context_release_complete_t
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/

rrc_return_et uecc_s1ap_build_and_send_ue_ctx_release_comp_resp(
    uecc_ue_context_t*      p_ue_context);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_ul_nas_transport_req
*
*   DESCRIPTION:
*       This function builds and sends RRC_S1AP_UL_NAS_TRANSPORT_REQ message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_ul_nas_transport_req(
    uecc_ue_context_t       *p_ue_context,
    DedicatedInfoNAS        *p_dedicatedInfoNAS
);

/* CDMA2000_CSFB_HO start */

/******************************************************************************
*  FUNCTION NAME:uecc_s1ap_build_and_send_uplink_s1_cdma2000_tunneling
*
*  DESCRIPTION:
*  This function builds nad sends UPLINK_S1_CDMA2000_TUNNELING message
*  to S1AP module
*
*  RETURNS:
*  	RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/	
rrc_return_et uecc_s1ap_build_and_send_uplink_s1_cdma2000_tunneling(
    uecc_ue_context_t	    *p_ue_context,
    DedicatedInfoCDMA2000   *p_dedicatedInfo,
    U32 		     dedicatedInfoType
);

/******************************************************************************
*  FUNCTION NAME:uecc_s1ap_fill_octet
*
*  DESCRIPTION:
*  This function is used for fill octets
*  to S1AP module
*
*  RETURNS:
*  	RRC_SUCCESS/RRC_FAILURE
*****************************************************************************/
OSDynOctStr* uecc_s1ap_fill_octet
(
    U8* dest,
    U8*  src,
    U8   size,
    uecc_ue_context_t       *p_ue_context
);

/* CDMA2000_CSFB_HO stop */

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_nas_non_delivery_ind
*
*   DESCRIPTION:
*       This function builds and sends RRC_S1AP_NAS_NON_DEL_IND message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_nas_non_delivery_ind(
    uecc_gb_context_t   *p_uecc_gb_context,
    U32     mme_ue_s1ap_id,
    U32     enb_ue_s1ap_id,
    s1ap_NAS_PDU *p_nas_pdu,
    s1ap_Cause *p_cause
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_ue_capability_info_ind
*
*   DESCRIPTION:
*       This function builds and sends S1AP UE CAPABILITY INFO INDICATION
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_ue_capability_info_ind(
    uecc_ue_context_t*      p_ue_context,
    UECapabilityInformation *p_UECapabilityInformation
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_error_indication
*
*   DESCRIPTION:
*       This function builds and sends s1ap_ErrorIndication message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_error_indication(
    U8                  mme_id,
    s1ap_MME_UE_S1AP_ID mme_ue_s1ap_id,
    rrc_bool_et         mme_ue_id_present,
    s1ap_ENB_UE_S1AP_ID enb_ue_s1ap_id,
    rrc_bool_et         enb_ue_id_present,
    s1ap_CriticalityDiagnostics *p_criticality_diagnostics,
    rrc_bool_et         criticality_diagnostics_present,
    s1ap_Cause*         p_cause,
    uecc_ue_context_t*  p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_s1ap_cause
*
*   DESCRIPTION:
*       This function builds s1ap_Cause
*
*   RETURNS:
*       None
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_s1ap_cause(
    s1ap_Cause*     p_cause,
    rrm_cause_t*    p_rrm_cause,
    uecc_ue_context_t*  p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_ue_release_msg_req
*
*   DESCRIPTION:
*       This function builds and sends S1AP_UE_RELEASE_MSG_REQ message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_ue_release_msg_req(
    U32 enb_ue_s1ap_id,
    rrc_bool_et mme_release_required,
    uecc_ue_context_t*  p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_failure_msg_req
*
*   DESCRIPTION:
*       This function builds and sends S1AP_FAILURE_MSG_REQ message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_failure_msg_req(
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    rrc_uecc_llim_s1u_path_failure_ind_t  *p_llim_s1u_path_failure
);

/* Start E-RAB Modify */
void uecc_s1ap_process_erab_modify_request(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    U8                  *p_buff,        /* ASN.1 encoded message */
    /*BUG 604 changes start*/
    U16                 buff_size,      /* encoded buf size - just for check*/
    U16                 ue_index
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_erab_modify_response
*
*   DESCRIPTION:
*       This function builds and sends E-RAB Modify response message to S1AP
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_erab_modify_response(
    uecc_ue_context_t*             p_ue_context);

/* End E-RAB Modify */


/* ERAB SETUP START */

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_erab_setup_response
*
*   DESCRIPTION:
*       This function builds and sends initial_context_setup_response message
*       to S1AP
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_erab_setup_response(
    uecc_ue_context_t*             p_ue_context);


/* ERAB SETUP STOP */

/* ERB RELEASE COMMAND START */
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_erb_release_command
*
*   DESCRIPTION:
*       This function processes s1ap_ERABReleaseCommand
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_erb_release_command
(
    S1AP_PDU*            p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t*   p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    U8                  *p_buff,        /* ASN.1 encoded message */
    /*BUG 604 changes start*/
    U16                 buff_size,      /* encoded buf size - just for check*/
    U16                 ue_index
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_e_rab_release_response
*
*   DESCRIPTION:
*       This function builds and sends rrc_s1ap_e_rab_release_response_t
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_e_rab_release_response(
    uecc_ue_context_t*             p_ue_context,  
    U32                            mme_ue_s1ap_id,
    U32                            enb_ue_s1ap_id,
    s1ap_CriticalityDiagnostics    *p_criticality_diagnostics,
    rrc_bool_et                    list_populated);

/* ERB RELEASE COMMAND END */
/* ERB RELEASE INDICATION START */
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_e_rab_release_indication
*
*   DESCRIPTION:
*       This function builds and sends rrc_s1ap_e_rab_release_indication_t
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_e_rab_release_indication(
    uecc_ue_context_t*      p_ue_context);
/* ERB RELEASE INDICATION END */

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_ue_context_modification_response
*
*   DESCRIPTION:
*       This function builds and sends ue context modfication response
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/

rrc_return_et uecc_s1ap_build_and_send_ue_context_modification_response(
    uecc_ue_context_t* p_ue_context);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_ue_context_modification_failure
*
*   DESCRIPTION:
*       This function builds and sends s1ap_ueContextModificationFailure message
*       to S1AP
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_ue_context_modification_failure(
    uecc_gb_context_t              *p_uecc_gb_context, /* UECC global context */
    uecc_ue_context_t              *p_ue_context,
    U32                            mme_ue_s1ap_id,
    U32                            enb_ue_s1ap_id,
    s1ap_Cause                     *p_cause,
    s1ap_CriticalityDiagnostics    *p_criticality_diagnostics); 
    

/* HANDOVER_CHANGES_REVANTH_01_begin */
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_handover_failure
*
*   DESCRIPTION:
*       This function builds and sends s1ap_HandoverFailure message
*       to S1AP
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
/* SPR 16149 Fix Start*/
rrc_return_et uecc_s1ap_build_and_send_handover_failure(
    uecc_gb_context_t  *p_uecc_gb_context,
    uecc_ue_context_t  *p_ue_context,
    U32        enb_ue_s1ap_id,
    U32        mme_ue_s1ap_id,
        U8         mme_id,
    s1ap_Cause *p_cause,
    rrc_s1ap_error_indication_t *p_criticality_diagnostics);
/* SPR 16149 Fix Stop*/

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_handover_request
*
*   DESCRIPTION:
*       This function processes S1AP Handover Request message
*
*   RETURNS:     None
*
******************************************************************************/
void uecc_s1ap_process_handover_request
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    s1ap_ue_associated_sig_msg_ind_t *p_s1ap_ue_associated_sig_msg_ind, /* UE Associated Sig Msg Ind */
    U32 buffer_length,
    rrc_module_id_t   dst_module_id
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_mme_status_transfer
*
*   DESCRIPTION:
*       This function processes MME Status Transfer message
*
*   RETURNS:     None
*
******************************************************************************/
void uecc_s1ap_process_mme_status_transfer
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    s1ap_ue_associated_sig_msg_ind_t *p_s1ap_ue_associated_sig_msg_ind,
    /*BUG 604 changes start*/
    U16                 buffer_length,
    U16                 ue_index
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_handover_request_ack
*
*   DESCRIPTION:
*       This function builds and sends HO Request Ack message
*       to S1AP module.
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_handover_request_ack
(
    uecc_ue_context_t*      p_ue_context,
    s1ap_CriticalityDiagnostics *p_criticality_diagnostics
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_handover_notify
*
*   DESCRIPTION:
*       This function builds and sends  s1ap_HandoverNotify message
*       to S1AP module.
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_handover_notify
(
    uecc_ue_context_t*      p_ue_context
);

U32 get_erab_id_from_lc_id
(
    uecc_ue_context_t* p_ue_context,
    rrc_lc_id_t lc_id
);
/*HANDOVER_CHANGES_REVANTH_01_end*/
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_handover_command
*
*   DESCRIPTION:
*       This function processes S1AP Handover Command message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_handover_command
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    U8          *p_buff,        /* ASN.1 encoded message */
    /*BUG 604 changes start*/
    U16         buff_size,      /* encoded buf size - just for check*/
    U16         ue_index
    /*BUG 604 changes stop*/

);



/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_handover_cancel_ack
*
*   DESCRIPTION:
*       This function processes S1AP Handover Command message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_handover_cancel_ack
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    U8                   mme_id,
    U8                  *p_buff,             /* ASN.1 encoded message */
    /*BUG 604 changes start*/
    U16                 buffer_length,        /* encoded buf size - just for check*/
    U16                 ue_index
    /*BUG 604 changes stop*/
);


/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_handover_preparation_failure
*
*   DESCRIPTION:
*       This function processes S1AP Handover Preparation Failure message
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_handover_preparation_failure
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    U8          *p_buff,        /* ASN.1 encoded message */
 /*BUG 604 changes start*/
    U16         buff_size,      /* encoded buf size - just for check*/
    U16                 ue_index
 /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_handover_cancel
*
*   DESCRIPTION:
*       This function builds and sends s1ap_HandoverCancel message
*       to S1AP module.
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_handover_cancel
(
    uecc_ue_context_t*      p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_handover_required
*
*   DESCRIPTION:
*       This function builds and sends s1ap_HandoverRequired message
*       to S1AP module.
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_handover_required
(
    uecc_ue_context_t*      p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_enb_status_transfer
*
*   DESCRIPTION:
*       This function builds and sends  s1ap_ENBStatusTransfer message
*       to S1AP module.
*
*   RETURNS:
*       rrc_return_et
*
******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_enb_status_transfer
(
    uecc_ue_context_t*      p_ue_context,
    rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_s1ap_overload_msg_ind
*
*   DESCRIPTION:
*       This function processes S1AP OVERLOAD IND
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_s1ap_process_s1ap_overload_msg_ind(        
        void                *p_api,             /* Input API buffer */         
        uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);

rrc_return_et uecc_s1ap_build_HandoverPreparationInformation(
        uecc_ue_context_t *p_ue_context,
        OSCTXT            *p_asn1_ctx,
        HandoverPreparationInformation_r8_IEs  *p_handover_prep_info
        );

rrc_return_et uecc_s1ap_build_s1ap_LastVisitedUTRANCellInformation(
        last_visited_utran_cell_information_t 
        *p_last_visited_utran_cell_information,
        OSCTXT *p_asn1_ctx,
        s1ap_LastVisitedUTRANCellInformation *p_utran_cell_info,
        uecc_ue_context_t *p_ue_context
        );

rrc_return_et uecc_s1ap_fillMobilityFromEUTRACommand
(
 uecc_ue_context_t*                      p_ue_context,
 U8                                     *p_data_buffer,
 U16                                    *p_buffer_length,
 OSCTXT                                 *p_asn1_ctx
 );

void uecc_s1ap_process_error_indication
(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    U8                  *buffer,
    /*BUG 604 changes start*/
    U32                 buffer_length,
    U16                 ue_index
    /*BUG 604 changes stop*/
);

/* Trace Start Procedure start */
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_trace_start
*
*   DESCRIPTION:
*       This function processes Trace start message
*
*   RETURNS:     None
*
 ******************************************************************************/
void uecc_s1ap_process_trace_start
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    /*BUG 604 changes start*/
    /* SPR 22628 Fix Start */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    /*SPR 17777 +-*/
    U8                  mme_id
    /* SPR 22628 Fix End */
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_process_deactivate_trace
*
*   DESCRIPTION:
*       This function processes Deactivate Trace message
*
*   RETURNS:     None
*
 ******************************************************************************/
void uecc_s1ap_process_deactivate_trace
(
    S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    /*BUG 604 changes start*/
    /* SPR 22628 Fix Start */
    uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
    U16                 ue_index,
    U8                  mme_id
    /* SPR 22628 Fix End */
    /*BUG 604 changes stop*/
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_trace_failure_indication
*
*   DESCRIPTION:
*       This function builds and sends s1ap_TraceFailureIndication message
*       to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_trace_failure_indication(
    uecc_ue_context_t*  p_ue_context,
    void* p_msg,
    s1ap_Cause*         p_cause,
    U8                  flag
);
/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_cell_traffic_trace
*
*   DESCRIPTION:
*       This function builds and sends rrc_s1ap_cell_traffic_trace_t message
*       to S1AP
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_cell_traffic_trace(
    uecc_ue_context_t              *p_ue_context
); 

/******************************************************************************
*   FUNCTION NAME: uecc_generate_eutran_traceid 
*
*   DESCRIPTION:
*       This function generate eutran_traceid 
*
*   RETURNS:
*         NONE 
*
*******************************************************************************/
void uecc_generate_eutran_traceid(
        uecc_ue_context_t              *p_ue_context
); 
/******************************************************************************
*   FUNCTION NAME: uecc_generate_trace_file_name 
*
*   DESCRIPTION:
*       This function generate trace_file_name
*
*   RETURNS:
*         NONE 
*
*******************************************************************************/
void uecc_generate_trace_file_name(
    uecc_ue_context_t              *p_ue_context,
    //Trace Change Start
    rrc_trace_initiator_et         trace_Initiator
    //Trace Change End
); 


/* Trace Start Procedure end */


/*x2ap-start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_process_path_switch_request_ack
 *
 *   DESCRIPTION:
 *       This function processes S1AP Path Switch Request Ack message
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_s1ap_process_path_switch_request_ack
(
 S1AP_PDU              *p_s1ap_pdu,    /* Pointer to s1ap pdu */
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 U8 mme_id,
 U8                  *p_buff,        /* ASN.1 encoded message */
 /*BUG 604 changes start*/
 U16                 buffer_length,        /* encoded buf size - just for check*/
 U16                 ue_index
 /*BUG 604 changes stop*/
);

/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_process_path_switch_request_failure
 *
 *   DESCRIPTION:
 *       This function processes S1AP Path Switch Request Failure message
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_s1ap_process_path_switch_request_failure
(
 S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 U8                  mme_id,
 U8                  *p_buff,         /* ASN.1 encoded message */
 /*BUG 604 changes start*/
 U16                 buffer_length,    /* encoded buf size - just for check*/
 U16                 ue_index
 /*BUG 604 changes stop*/
 );

/* SPR 21610 Fix Start */
rrc_return_et uecc_s1ap_build_rrc_connection_reconfig(
        uecc_ue_context_t   *p_uecc_ue_context,
         ranap_TargetRNC_ToSourceRNC_TransparentContainer     *p_ranap_target_rnc_to_src_rnc_transparent_cont,
         OSCTXT              *pctxt        /* for memory allocation */
                             );
/* SPR 21610 Fix End */
rrc_return_et uecc_s1ap_build_rrc_con_reconfig
(
        uecc_ue_context_t   *p_uecc_ue_context,
        OSDynOctStr         *p_handoverCommandMessage,
        OSCTXT              *pctxt         /* for memory allocation */
 );
/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_build_and_send_path_switch_request
 *
 *   DESCRIPTION:
 *       This function builds and sends s1ap_PathSwitchRequest message
 *       to S1AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_path_switch_request
(
 uecc_ue_context_t*      p_ue_context,
/* Bug 8198 Fix Start */
  U8   mme_id
/* Bug 8198 Fix End */
 );

/*x2ap-end*/

rrc_return_et validate_gummei_and_fill_mme_id
(
 uecc_ue_context_t*      p_ue_context,
 s1ap_gummei_t*          p_gummei,             
 U32                     *mme_id
 );

void fill_rrm_cause_from_s1ap_cause
(
 rrm_cause_t        *p_rrm_cause,
 s1ap_Cause         *p_s1ap_cause,
 uecc_ue_context_t*  p_ue_context
 );

rrc_return_et uecc_x2ap_build_and_send_ue_release_ind
(
 uecc_ue_context_t*      p_ue_context
);

rrc_return_et uecc_s1ap_build_and_send_ue_release_ind
(
      uecc_ue_context_t                *p_ue_context
);

rrc_return_et uecc_s1ap_build_and_send_ho_ongoing_ue_release_ind
(
    mme_ue_s1ap_id_list_t    *p_mme_ue_s1ap_id_list,
    uecc_gb_context_t        *p_uecc_gb_context  /* UECC global context */
);

void uecc_s1ap_process_ue_rel_req_int(
        void                *p_api,             /* Input API buffer */
        uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);


/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_e_rab_release_indication_arp
*
*   DESCRIPTION:
*       This function builds and sends rrc_s1ap_e_rab_release_indication_t
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_e_rab_release_indication_arp(
    uecc_ue_context_t*      p_ue_context);

/******************************************************************************
 *   FUNCTION NAME: build_and_send_allocate_mme_req
 *
 *   DESCRIPTION:
 *       This function builds and sends S1AP_ALLOCATE_MME_REQ to S1AP
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et build_and_send_allocate_mme_req(
       uecc_ue_context_t* p_ue_context,
/* Bug 8198 Fix Start */
        U8   procedure_code
/* Bug 8198 Fix End */
       
 );

/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_process_alloc_mme_resp
 *
 *   DESCRIPTION:
 *       This function processes S1AP_ALLOCATE_MME_RESP
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
void uecc_s1ap_process_alloc_mme_resp(
        void                *p_api,             /* Input API buffer */
        uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
        );

/******************************************************************************
*   FUNCTION NAME: uecc_fsm_validate_and_fill_erab_forwarding_list
*
*   DESCRIPTION:
*       This function validate the erab_fowarding_list
*
*   RETURNS:
*       RRC_SUCCESS/RRC_PARTIAL_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_fsm_validate_and_fill_erab_forwarding_list(
        uecc_ue_context_t* p_ue_context,
        s1ap_E_RABSubjecttoDataForwardingList *p_erab_forward_list
        );

/*BUG 604 changes start*/
/******************************************************************************
 *   FUNCTION NAME: uecc_s1ap_build_and_send_intra_cell_ho_success_ind
 *
 *   DESCRIPTION:
 *       This function builds and sends S1AP_INTRA_CELL_HO_SUCCESS_IND to S1AP
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_intra_cell_ho_success_ind(
       uecc_ue_context_t* p_ue_context
 );
/*BUG 604 changes stop*/

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_fill_cqi_report_periodic_r10
*
*   DESCRIPTION  : This function builds CQI_ReportConfigSCell_r10
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_fill_cqi_report_periodic_r10 (
        uecc_ue_context_t         *p_ue_context,
        OSCTXT                    *p_asn1_ctx,
        CQI_ReportConfigSCell_r10 *p_CQI_ReportConfigSCell_r10,
        rrc_phy_cqi_report_periodic_r10_t cqi_report_periodic_r10 
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_fill_cqi_format_ind_periodic
*
*   DESCRIPTION  : This function builds
*                  CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_fill_cqi_format_ind_periodic (
        uecc_ue_context_t         *p_ue_context,
        OSCTXT                    *p_asn1_ctx,
        CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10
                                  *p_cqi_FormatIndicatorPeriodic_r10,
        rrc_phy_cqi_format_ind_periodic_r10_t   cqi_format_ind_periodic
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_fill_candidiate_cell_info_list
*
*   DESCRIPTION  : This function builds CandidateCellInfoList_r10.      
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_fill_candidiate_cell_info_list (
    uecc_ue_context_t         *p_ue_context,
    OSCTXT                    *p_asn1_ctx,
    RRM_Config                *p_RRM_Config,
    rrm_rrc_container_t       *p_rrc_container
);

/*SPR_16778_START*/
/******************************************************************************
 *   FUNCTION NAME:fill_s1ap_cause 
 *
 *   DESCRIPTION:
 *       This function converts RRM Cause to S1AP Cause
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 *******************************************************************************/
rrc_return_et fill_s1ap_cause
(
 s1ap_Cause      *p_s1ap_cause,
 rrm_cause_t      *p_rrm_cause,
 uecc_ue_context_t *p_ue_context
 );
/*SPR_16778_END*/

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
rrc_return_et uecc_s1ap_build_e_rab_not_to_be_modified_list(
        uecc_ue_context_t   *p_ue_context,
        OSCTXT              *p_asn1_ctx,
        OSRTDList           *p_erabNotToBeModifiedList
);

rrc_return_et uecc_s1ap_build_e_rab_to_be_modified_list(
        uecc_ue_context_t   *p_ue_context,
        OSCTXT              *p_asn1_ctx,
        OSRTDList           *p_erabToBeModifiedList
);

rrc_return_et uecc_s1ap_build_and_send_erab_modification_ind(
        uecc_ue_context_t*             p_ue_context
);

void uecc_s1ap_process_erab_modification_cnf
(
    S1AP_PDU            *p_s1ap_pdu,    /* Pointer to s1ap pdu */
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                  mme_id,
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         buff_size,      /* encoded buf size - just for check*/
    U16         ue_index
);

/******************************************************************************
*   FUNCTION NAME: uecc_s1ap_build_and_send_e_rab_release_indication_endc
*
*   DESCRIPTION:
*       This function builds and sends rrc_s1ap_e_rab_release_indication_t
*       message to S1AP module
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************/
rrc_return_et uecc_s1ap_build_and_send_e_rab_release_indication_endc
(
    uecc_ue_context_t*      p_ue_context
);
#endif
/* OPTION3X Changes End */
#endif /* UECC_S1AP_H */

