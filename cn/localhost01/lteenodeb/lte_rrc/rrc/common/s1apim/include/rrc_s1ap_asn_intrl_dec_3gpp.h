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
 *
 ****************************************************************************/

#ifndef _RRC_S1AP_ASN_INTRL_DEC_3GPP_H_
#define _RRC_S1AP_ASN_INTRL_DEC_3GPP_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_global_ctx.h"
#include "rrc_s1ap_asn_intrl_3gpp.h"

#include "rrc_uecc_error_indication.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define S1AP_CAUSE_SUCCESS 0
#define S1AP_MAX_IES 32

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

rrc_return_et rrc_s1ap_pdu_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         buff_size,      /* encoded buf size - just for check*/
    S1AP_PDU    *p_s1ap_pdu     /* unpacked */
);

/* ERAB SETUP Start */

rrc_return_et rrc_s1ap_erab_setup_request_intrl_dec
(
    U8                     mme_id,
    s1ap_E_RABSetupRequest *p_3gpp,    /* unpacked src */
    rrc_s1ap_erab_setup_request_t *p_intrl, /* dest */
    uecc_gb_context_t   *p_uecc_gb_context
);


/* ERAB SETUP Stop */
rrc_return_et rrc_s1ap_initial_context_setup_request_intrl_dec
(
    OSCTXT *p_asn1_ctx,
    U8 mme_id,
    s1ap_InitialContextSetupRequest *p_3gpp,    /* unpacked src */
    rrc_s1ap_initial_context_setup_request_t *p_intrl, /* dest */
    rrc_s1ap_error_indication_t *p_err_ind_content,
    uecc_s1ap_error_ind_bool_t  *p_send_err_ind,
    uecc_gb_context_t           *p_gb_context
);

rrc_return_et rrc_s1ap_ue_context_release_command_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                              mme_id,
    s1ap_UEContextReleaseCommand    *p_3gpp,    /* unpacked src */
    rrc_s1ap_ue_context_release_command_t *p_intrl /* dest */
);
rrc_return_et rrc_s1ap_path_switch_req_ack_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8 mme_id,
 s1ap_PathSwitchRequestAcknowledge  *p_3gpp,    /* unpacked src */
 rrc_s1ap_path_switch_request_ack_t *p_intrl    /* dest */
 );

rrc_return_et rrc_s1ap_path_switch_req_fail_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 U8 mme_id,
 s1ap_PathSwitchRequestFailure          *p_3gpp,    /* unpacked src */
 rrc_s1ap_path_switch_request_failure_t *p_intrl    /* dest */
 );


rrc_return_et rrc_s1ap_downlink_nas_transport_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8 mme_id,
    s1ap_DownlinkNASTransport *p_3gpp,  /* unpacked src */
    rrc_s1ap_downlink_nas_transport_t *p_intrl/* dest */
);

rrc_return_et rrc_s1ap_handover_command_intrl_dec
(
    U8           mme_id,
    OSCTXT      *p_ho_cmd_ctxt,         /* for memory allocation */
    s1ap_HandoverCommand *p_3gpp,    /* unpacked src */
    rrc_s1ap_handover_command_t *p_intrl, /* dest */
    uecc_ue_context_t             *p_ue_context
);

void rrc_s1ap_handover_command_container_intrl_dec
(
    s1ap_TargeteNB_ToSourceeNB_TransparentContainer *p_container, 
    rrc_s1ap_handover_command_t *p_intrl, /* dest */
    OSCTXT      *p_asn1_ctx,
    s1ap_Cause  *cause
);

void rrc_s1ap_target_enb_to_source_enb_transparent_container_intrl_dec
(
    s1ap_Target_ToSource_TransparentContainer *p_container,
    rrc_s1ap_handover_command_t *p_intrl, /* dest */
    OSCTXT      *p_asn1_ctx,
    s1ap_Cause *cause
);

rrc_return_et rrc_s1ap_handover_preparation_failure_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                              mme_id, 
    s1ap_HandoverPreparationFailure *p_3gpp,    /* unpacked src */
    rrc_s1ap_handover_preparation_failure_t *p_intrl /* dest */
);

rrc_return_et rrc_s1ap_mme_status_transfer_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */ 
    U8 mme_id,
    s1ap_MMEStatusTransfer *p_3gpp,    /* unpacked src */
    rrc_s1ap_mme_status_transfer_t *p_intrl /* dest */
);

void rrc_s1ap_rrc_container_intrl_dec
(
    s1ap_RRC_Container *p_rRC_Container,
    uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
    HandoverPreparationInformation_r8_IEs  *p_intrl,
    s1ap_Cause                             *cause
);

void rrc_s1ap_source_enb_to_target_enb_transparent_container_intrl_dec
(
    s1ap_Source_ToTarget_TransparentContainer  *p_buff,        /* ASN.1 encoded message */
    uecc_container_asn1_ctxt_t *p_container_asn1_ctxt_arr,
    rrc_source_to_target_transparent_container_t  *p_intrl, /* Encoded message*/
    s1ap_Cause                 *cause
);

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
);

void rrc_s1ap_last_visited_utran_cell_information_intrl_dec
(
    OSCTXT      *p_asn1_ctxt,
    s1ap_LastVisitedUTRANCellInformation  *p_utran_cell,
    ranap_LastVisitedUTRANCell_Item  *p_intrl,
    s1ap_Cause                       *cause
);

rrc_return_et rrc_s1ap_ue_context_modification_request_intrl_dec
(
     uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
     OSCTXT *p_asn1_ctx,
     U8 mme_id, 
     s1ap_UEContextModificationRequest *p_3gpp,    /* unpacked src */
     rrc_s1ap_ue_context_modification_request_t *p_intrl, /* dest */
     rrc_s1ap_error_indication_t *p_error_ind_content,
     uecc_s1ap_error_ind_bool_t *p_send_err_ind
);

rrc_return_et rrc_s1ap_erab_modify_request_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                       mme_id,
    s1ap_E_RABModifyRequest * p_3gpp,        /* unpacked src */
    rrc_s1ap_erab_modify_request_t * p_intrl /* dest */
);

/* ERB RELEASE COMMAND START */
rrc_return_et rrc_s1ap_erb_release_command_intrl_dec
(
    OSCTXT *p_asn1_ctx,
    U32 mme_id,         /* for memory allocation */
    s1ap_E_RABReleaseCommand *p_3gpp,    /* unpacked src */
    rrc_s1ap_erab_release_command_t *p_intrl, /* dest */
    rrc_s1ap_error_indication_t *p_error_ind_content,
    uecc_s1ap_error_ind_bool_t  *p_send_err_ind,
    rrc_bool_et *p_list_populated,
    uecc_gb_context_t *p_gb_context
);
/* ERB RELEASE COMMAND END */

/* Trace Start Procedure start */
void rrc_s1ap_trace_start_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    s1ap_TraceStart *p_3gpp,    /* unpacked src */
    rrc_s1ap_trace_start_t *p_intrl, /* dest */
    s1ap_Cause             *cause
);
/* Trace Start Procedure end */
/* Deactivate Trace Procedure start */
void rrc_s1ap_deactivate_trace_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    s1ap_DeactivateTrace *p_3gpp,    /* unpacked src */
    rrc_s1ap_deactivate_trace_t *p_intrl, /* dest */
    s1ap_Cause                  *cause
);
/* Deactivate Trace Procedure end */

void rrc_s1ap_target_rnc_to_source_rnc_transparent_container_intrl_dec
(
    s1ap_Target_ToSource_TransparentContainer *p_container,
    rrc_s1ap_handover_command_t *p_intrl, /* dest */
     OSCTXT  *p_ho_cmd_ctxt,
    s1ap_Cause                  *cause /*SPR 17777 +-*/
);

rrc_return_et rrc_s1ap_handover_cancel_ack_intrl_dec
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                             mme_id,
    s1ap_HandoverCancelAcknowledge *p_3gpp,    /* unpacked src */
    rrc_s1ap_handover_cancel_ack_t *p_intrl /* dest */
);

/* BUG fix for Multi UE */

rrc_return_et rrc_s1ap_get_enb_ue_s1ap_id_from_handover_command
(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
    U8                   mme_id,
    s1ap_HandoverCommand *p_3gpp,    /* unpacked src */
    s1ap_ENB_UE_S1AP_ID *p_enb_ue_s1ap_id /* dest */
);

rrc_return_et uecc_s1ap_location_repoting_control_intrl_dec(
    uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */    
    U8 mme_id,
    s1ap_LocationReportingControl                 *p_3gpp,  /* unpacked src */
    rrc_s1ap_reporting_control_indication_msg_t   *p_intrl  /* dest */
);


rrc_return_et rrc_s1ap_error_indication_intrl_dec
(
    s1ap_ErrorIndication    *p_3gpp,    /* unpacked src */
    rrc_s1ap_error_indication_t *p_intrl/* dest */
);
rrc_return_et rrc_uecc_error_indication_intrl_dec
(
     uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
     s1ap_ErrorIndication    *p_3gpp,    /* unpacked src */
     rrc_s1ap_error_indication_t *p_intrl/* dest */
);
rrc_return_et rrc_uecc_error_indication_tse_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size   /* size of result buffer */
);
rrc_return_et rrc_uecc_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_s1ap_error_indication_t  *p_error_indication
);

/* CDMA + CSFB Changes start: */
rrc_return_et rrc_s1ap_downlink_s1_cdma2000_tunneling_intrl_dec
(
 uecc_gb_context_t   *p_uecc_gb_context, /* UECC global context */
 OSCTXT *p_asn1_ctx,
 U8 mme_id,
 s1ap_DownlinkS1cdma2000tunnelling *p_3gpp,    /* unpacked src */
 rrc_s1ap_downlink_s1_cdma2000_tunneling_t *p_intrl,/* dest */
 rrc_s1ap_error_indication_t *p_error_ind_content,
 uecc_s1ap_error_ind_bool_t *p_send_err_ind
 );
/* CDMA + CSFB Changes end: */
/* CSR 101813 Fix Start */ 
void rrc_s1ap_target_bss_to_source_bss_transparent_container_intrl_dec
(
 s1ap_Target_ToSource_TransparentContainer *p_container,
 rrc_s1ap_handover_command_t *p_intrl,
 OSCTXT  *p_ho_cmd_ctxt,
 s1ap_Cause *cause/*SPR 17777 +-*/
 );
/* CSR 101813 Fix Stop */ 

/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
rrc_return_et rrc_s1ap_erab_modification_cnf_intrl_dec
(
    U8                     mme_id,
    s1ap_E_RABModificationConfirm *p_3gpp,    /* unpacked src */
    rrc_s1ap_erab_modification_cnf_t *p_intrl, /* dest */
    uecc_gb_context_t   *p_uecc_gb_context
);
#endif
/* OPTION3X Changes End */
 
#endif /* _RRC_S1AP_ASN_INTRL_DEC_3GPP_H_ */

