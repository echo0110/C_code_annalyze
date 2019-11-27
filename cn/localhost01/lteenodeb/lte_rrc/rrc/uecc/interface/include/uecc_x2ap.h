/******************************************************************************
*
*   FILE NAME:
*       uecc_x2ap.h
*
*   DESCRIPTION:
*       This is the interface file of the UECC X2AP.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   22 sep 2011     Hari Oum     ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef UECC_X2AP_H
#define UECC_X2AP_H

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "uecc_global_ctx.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_uecc_error_indication.h"
#include "rrc_x2ap_uecc_intf.h"

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

#define P_SRVD_CELL_INFO(p_srvd_cell_info, index) \
    ((x2ap_srvd_cell_info_t*)((U8*)(p_srvd_cell_info) + \
                               (sizeof(x2ap_srvd_cell_info_t)* index))) 

#define P_SRVD_NR_CELL_INFO(p_srvd_nr_cell_info, index) \
    ((x2ap_srvd_nr_cell_info_t*)((U8*)(p_srvd_nr_cell_info) + \
                               (sizeof(x2ap_srvd_nr_cell_info_t)* index)))                                

/* SPR_7888_fix */
#define X2AP_CAUSE_RADIO_NETWORK_MAX_VALUE  28
#define X2AP_CAUSE_TRANSPORT_MAX_VALUE       1
#define X2AP_CAUSE_PROTOCOL_MAX_VALUE        6
#define X2AP_CAUSE_MISC_MAX_VALUE            4
/* SPR_7888_fix */

rrc_return_et uecc_x2ap_build_x2ap_cause
(
    uecc_gb_context_t*  p_uecc_gb_context,
    x2ap_Cause*         p_cause,
    rrm_cause_t*        p_rrm_cause
);

void uecc_x2ap_process_ue_associated_sig_msg_ind
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
);

/* Bug 4691 Changes Start */
void uecc_x2ap_process_update_nbour_info
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
);
/* Bug 4691 Changes End */

void uecc_x2ap_process_status_msg_ind
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
);

void uecc_x2ap_process_reset_msg_ind
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
);

void uecc_x2ap_process_rlf_msg_ind
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
);

void uecc_x2ap_process_update_nr_nbour_info
(
    void *p_api,
    uecc_gb_context_t *p_uecc_gb_context
);

void x2ap_build_and_send_rlf_resp(
            uecc_gb_context_t *p_uecc_gb_context,
            U8                rlf_ind_flag,
            U8                ho_report_flag,
            uecc_ue_context_t *p_ue_context);

void uecc_x2ap_build_and_send_rlf_resp(
        uecc_gb_context_t *p_uecc_gb_context,
        x2ap_rlf_ind_info_t  *p_rlf_ind,
        U16                ue_index,
        U8                 ue_context_released_flag);

rrc_return_t uecc_check_store_ue_context_timer(
            uecc_ue_context_t *p_ue_context);

/*SPR_16756_START*/
rrc_return_et rrc_x2ap_get_enb_ue_x2ap_id_from_handover_req_ack (
        uecc_gb_context_t*                    p_uecc_gb_context,
        x2ap_HandoverRequestAcknowledge      *p_3gpp,
        x2ap_UE_X2AP_ID                      *p_old_enb_ue_x2ap_id,
        x2ap_UE_X2AP_ID                      *p_new_enb_ue_x2ap_id
);
/*SPR_16756_END*/

void uecc_x2ap_process_handover_request_msg
(
     OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,
    rrc_module_id_t     dst_module_id,
    U8                  *buffer,
    /*BUG 604 changes start*/
    U8                  procedure_code,
    U32                 enb_ue_s1ap_id,
    U16                 source_ue_index,
    U8                  security_key_present
    /*BUG 604 changes stop*/
);

void uecc_x2ap_process_handover_request_ack_msg
(
     OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer,
    /*BUG 604 changes start*/
    U16                  target_ue_index    /* Target UE Index */
    /*BUG 604 changes stop*/
);
void uecc_x2ap_process_handover_prep_failure_msg
(
    OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU      *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t  *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                 peer_enodeb_id,      /* Peer EnodeB ID */
    U8                 *buffer
);

void uecc_x2ap_process_handover_cancel_msg
(
    OSCTXT*             p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
);

void uecc_x2ap_process_ue_context_release_msg
(
    OSCTXT*             p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    U8                  *buffer
);

void uecc_x2ap_process_sn_status_trnsfr_msg
(
    OSCTXT*             p_asn1_ctx, /* ASN Context Pointer */
    x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
    uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    /*SPR_17976_START*/
    U8                  *buffer,
    void                *p_api
    /*SPR_17976_END*/
);

rrc_return_et uecc_x2ap_build_and_send_handover_request 
(
    uecc_ue_context_t *p_ue_context
);

rrc_return_et uecc_x2ap_build_and_send_handover_request_ack
(
     uecc_ue_context_t *p_ue_context,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 );

rrc_return_et uecc_x2ap_build_rrc_ho_command(
        uecc_ue_context_t   *p_uecc_ue_context,
        rrc_x2ap_handover_req_ack_t* p_handover_request_msg,
        OSCTXT         *pctxt
);

rrc_return_et uecc_x2ap_build_and_send_handover_cancel
(
     uecc_ue_context_t*      p_ue_context
 );

rrc_return_et uecc_x2ap_build_and_send_handover_prep_failure
(
     uecc_ue_context_t*      p_ue_context,
     x2ap_CriticalityDiagnostics *p_criticality_diagnostics
 );

rrc_return_et uecc_x2ap_build_and_send_handover_prep_failure_at_init
(
    uecc_gb_context_t                *p_uecc_gb_context,
    x2ap_UE_X2AP_ID                  old_ue_x2ap_id,
    x2ap_Cause                       *p_failure_cause,
    U8                               peer_enodeb_id,
    rrc_x2ap_error_indication_t *p_err_ind_content,
    U8                               procedure_code 
);

rrc_return_et uecc_x2ap_build_and_send_ue_context_release
(
    uecc_ue_context_t*      p_ue_context
 );

rrc_return_et uecc_x2ap_build_and_send_sn_status_transfer
(
    uecc_ue_context_t*      p_ue_context,
    rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp
 );

rrc_return_et uecc_x2ap_build_x2ap_LastVisitedUTRANCellInformation(
    uecc_ue_context_t                *p_ue_context,
    last_visited_utran_cell_information_t
    *p_last_visited_utran_cell_information,
    OSCTXT *p_asn1_ctx,
    x2ap_LastVisitedUTRANCellInformation *p_utran_cell_info);

rrc_return_et fill_x2ap_cause
(
    uecc_gb_context_t*  p_uecc_gb_context,    
    x2ap_Cause          *p_x2ap_cause,
    rrm_cause_t         *p_rrm_cause
);

rrc_return_et uecc_x2ap_build_and_send_ue_release_ind
(
    uecc_ue_context_t                *p_ue_context
);

rrc_return_et uecc_x2ap_build_and_send_handover_request
(
    uecc_ue_context_t*      p_ue_context
);

void fill_rrm_cause_from_x2ap_cause(
 uecc_gb_context_t*  p_uecc_gb_context,
        rrm_cause_t     *p_rrm_cause,
 x2ap_Cause      *p_x2ap_cause);

void uecc_x2ap_trace_handover_request(
        uecc_ue_context_t    *p_ue_context,
        rrc_x2ap_handover_request_t *p_msg,
        U32                  encoded_msg_len,
        U8                   *buffer,
        U8                   send_or_recv);

void uecc_x2ap_p_rrc_trace_for_cause(
        uecc_ue_context_t    *p_ue_context,
        x2ap_Cause           cause);

rrc_return_et x2ap_build_and_send_reestablishment_ind(
                 uecc_ue_context_t    *p_ue_context);
rrc_return_et x2ap_build_and_send_conn_setup_reconfig_rlf_report(
                 uecc_ue_context_t    *p_ue_context);


/*HOL:error_ind:start*/
void uecc_x2ap_process_error_indication
(
 x2ap_X2AP_PDU            *p_x2ap_pdu,    /* Pointer to x2ap pdu */
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 OSCTXT*             p_asn1_ctx, /* ASN Context Pointer */
 /*SPR_16618_START*/
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 /*SPR_16618_END*/
 U8                  *buffer
 );

rrc_return_et uecc_x2ap_build_and_send_error_indication_unknown_proc(
        uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
        U8                  peer_enodeb_id,      /* Peer EnodeB ID */
        x2ap_ProcedureCode proc_code,
        x2ap_Criticality proc_criticality,
        x2ap_TriggeringMessage triggering_msg
);

rrc_return_et uecc_x2ap_build_and_send_error_indication(
        uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
        U8                  peer_enodeb_id,
        x2ap_UE_X2AP_ID     old_enb_ue_x2ap_id,
        rrc_bool_et         old_enb_ue_x2ap_id_present,
        x2ap_UE_X2AP_ID     new_enb_ue_x2ap_id,
        rrc_bool_et         new_enb_ue_x2ap_id_present,
        x2ap_CriticalityDiagnostics *p_criticality_diagnostics,
        rrc_bool_et         criticality_diagnostics_present,
        x2ap_Cause*         p_cause,
        uecc_ue_context_t*  p_ue_context
        );

/* SPR 16750 Fix Start */
void uecc_x2ap_handle_src_enb_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
);

void uecc_x2ap_handle_trg_enb_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
);

void uecc_x2ap_handle_unknown_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
);
/* SPR 16750 Fix Stop */
/*CA_PHASE3_INTER_CELL_MRO_START*/
rrc_return_et x2ap_build_and_send_rlf_ind(
        uecc_ue_context_t   *p_ue_context, /*UECC UE context*/ 
        uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/*CA_PHASE3_INTER_CELL_MRO_STOP*/


#ifdef ENDC_ENABLED
/******************************************************************************
 *   FUNCTION NAME: uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm
 *
 *   Outputs : None
 *   DESCRIPTION:
 *       This function builds and sends SgNB Release confirm message  
 *       to X2AP module.
 *
 *   RETURNS:
 *       rrc_return_et
 *
 ******************************************************************************/
rrc_return_et
uecc_x2ap_build_and_send_sgnb_initiated_sgnb_release_confirm
(
    uecc_ue_context_t*  p_ue_context
);

/*NR_DC Code Changes Start*/
rrc_return_et uecc_x2ap_build_and_send_sgnb_addtion_request(
    uecc_ue_context_t                       *p_ue_context
    );

void uecc_x2ap_process_sgnb_addition_req_reject
( 
    OSCTXT*             p_asn1_ctx, 
    x2ap_X2AP_PDU       *p_x2ap_pdu,
    uecc_gb_context_t   *p_uecc_gb_context,
    U8                  peer_enodeb_id,
    U8                  *buffer
);

void uecc_x2ap_process_sgnb_addition_req_ack_msg
( 
    OSCTXT*             p_asn1_ctx, 
    x2ap_X2AP_PDU       *p_x2ap_pdu,
    uecc_gb_context_t   *p_uecc_gb_context,
    U8                  peer_enodeb_id,
    U8                  *buffer
);

rrc_return_et uecc_x2ap_handle_sgnb_addition_ack_msg_rcvd(
    uecc_ue_context_t*  p_ue_context,
    void*               p_api
);

rrc_return_et uecc_x2ap_build_and_send_sgnb_reconfiguration_complete(
    uecc_ue_context_t           *p_ue_context
);

rrc_return_et uecc_x2ap_build_and_send_sgnb_reconfig_comp_with_failure(
    uecc_ue_context_t           *p_ue_context
);
/*NR_DC Code Changes Stop*/
/*EN-DC_changes_start*/
rrc_return_et build_and_send_sgnb_release_request
(
 uecc_ue_context_t*  p_ue_context
);

void uecc_x2ap_process_sgnb_release_request_ack(
        OSCTXT*            p_asn1_ctx, /* ASN Context Pointer */
        x2ap_X2AP_PDU       *p_x2ap_pdu, /* pointer to X2AP PDU */
        uecc_gb_context_t   *p_uecc_gb_context, /* pointer to UECC Global context */
        U8                  peer_enodeb_id,      /* Peer EnodeB ID */
        U8                  *buffer
        );
/*Error_Handling_changes*/
typedef enum
{
	first_message,
	first_returned_message,
	middle_message,
	last_message
}msg_type_et;

void uecc_x2ap_handle_endc_error_ind
(
 uecc_gb_context_t   *p_uecc_gb_context,  /* UECC global context */
 rrc_x2ap_error_indication_t     *p_msg,  /* X2AP Error Indication Message */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 U32				 encoded_msg_len,     /* Encoded Message Length */
 U8                  *buffer
);
/*Logical_Error*/
void uecc_x2ap_add_erab_to_failed_list(
        uecc_ue_context_t*  p_ue_context,
        U8 erab_id
        );
/*Logical_Error*/


/*Error_Handling_changes*/
/*EN-DC_changes_end*/
#endif
#endif /* UECC_X2AP_H */

