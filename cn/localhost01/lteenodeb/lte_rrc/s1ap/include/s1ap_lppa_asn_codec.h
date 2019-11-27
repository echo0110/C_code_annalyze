/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_lppa_asn_codec.h
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the structs and function
 *                     declarations necessary to encode/decode Asn messages
 *                     required by S1AP module for LPPA related messages.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef _S1AP_LPPA_ASN_H_
#define _S1AP_LPPA_ASN_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_defines.h"
#include "rtxsrc/rtxCommon.h"
#include "s1ap_global_ctx.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_s1apRrm_intf.h"
#include "list_wrap.h"
#include "s1ap_enb_config_fsm.h"
#include "rrc_s1ap_asn_intrl_3gpp.h"
#include "s1ap_error_indication.h"
#include "lppa_asn_enc_dec_3gpp.h"
#include "s1ap_asn_codec.h"


#define LPPA_MAX_ASN1_BUF_LEN                   8192

rrc_return_et s1ap_lppa_encode_s1ap_UplinkUEAssociatedLPPaTransport(
    U32    mme_ue_s1ap_id,
    U32    enb_ue_s1ap_id,
    U8     routing_id,
    U16    asn_lppa_pdu_len,
    U8     *p_asn_lppa_pdu,
    U16    *p_asn_s1ap_pdu_len,
    U8     *p_asn_s1ap_pdu_msg);

rrc_return_et s1ap_lppa_encode_s1ap_UplinkNonUEAssociatedLPPaTransport(
        U8     routing_id,
        U16    asn_lppa_pdu_len,
        U8     *p_asn_lppa_pdu,
        U16    *p_asn_s1ap_pdu_len,
        U8     *p_asn_s1ap_pdu_msg);

rrc_return_et  dl_non_ue_associated_lppa_trans_intrl_dec(
        OSCTXT        *p_asn1_ctx,
        mme_context_t *p_mme_context,
        s1ap_DownlinkNonUEAssociatedLPPaTransport  *p_3gpp,  /* unpacked src */
        dl_non_ue_asso_lppa_trans_t  *p_intrl); /* dest */

void  lppa_parse_CriticalityDiagnostics(
        lppa_criticality_diagno_t    *p_CriticalityDiagno,
        lppa_CriticalityDiagnostics  *p_CriticalityDiagnostics);

rrc_return_et lppa_compose_CriticalityDiagnostics(
    lppa_CriticalityDiagnostics *p_asn_crit_diag,
    lppa_criticality_diagno_t   *p_lppa_crit_diag);

void  lppa_parse_cause(
        lppa_cause_t   *p_cause,
        lppa_Cause     *p_Cause);

rrc_return_et  lppa_compose_Cause(
        lppa_Cause   *p_asn_cause,
        lppa_cause_t *p_lppa_cause);

void  lppa_compose_lppa_ECGI(
        lppa_ECGI *p_asn_ecgi,
        lppa_ecgi_t *p_lppa_ecgi);

void lppa_compose_lppa_E_UTRANAccessPointPosition(
    lppa_E_UTRANAccessPointPosition *p_asn_acc_pt_pos,
    lppa_e_utran_access_point_pos_t *p_lppa_acc_pt_pos);

rrc_return_et lppa_compose_lppa_MeasuredResults(
    OSCTXT    *p_asn1_ctx,
    lppa_MeasuredResults *p_asn_meas_results,
    lppa_meas_results_t  *p_lppa_meas_results);

rrc_return_et lppa_compose_E_CID_MeasurementResult(
      OSCTXT    *p_asn1_ctx,
      lppa_E_CID_MeasurementResult *p_asn_meas_result,
      lppa_e_cid_meas_result_t  *p_lppa_meas_result);

rrc_return_et lppa_encode_E_CIDMeasurementInitiationResponse(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_init_resp_t *p_lppa_meas_init_resp);

rrc_return_et lppa_encode_E_CIDMeasurementInitiationFailure(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_init_fail_t *p_lppa_meas_init_fail,
    rrc_lppa_error_indication_t *p_error_indication);

rrc_return_et lppa_encode_E_CIDMeasurementFailureIndication(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_fail_ind_t *p_lppa_meas_fail_ind);

rrc_return_et lppa_encode_E_CIDMeasurementReport(
    U8           *p_asn_msg,
    U16          *p_asn_msg_len,
    s1ap_rrm_lppa_ecid_meas_report_t *p_lppa_meas_report);

rrc_return_et  lppa_internal_encode_otdoa_information_fail(
        s1ap_rrm_lppa_otdoa_info_fail_t  *p_otdoa_fail,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len);

rrc_return_et rrc_lppa_error_indication_tse_intrl_enc(
        OSCTXT      *pctxt,         /* for memory allocation */
        U8          *p_buff,        /* ASN.1 encoded message */
        U16         *p_buff_size,  /* size of result buffer */
        mme_context_t  *p_mme_context);

void  lppa_compose_lppa_pRSMutingConfiguration(
        OSCTXT         *p_asn1_ctx,
        lppa_PRSMutingConfiguration  *p_asn_prs_muting_config,
        lppa_prs_muting_config_t  *p_prs_muting_config);

void s1ap_lppa_oTDOAInformationExchange(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   *p_mme_ctx,
        lppa_OTDOAInformationRequest  *p_otdoa_info_request,
        dl_non_ue_asso_lppa_trans_t   *p_ue_asso_lppa_trans,
        U32             lppa_trans_id);

rrc_return_et  lppa_compose_otdoa_cell_info(
        OSCTXT      *p_asn1_ctx,
        lppa_OTDOACell_Information  *p_cell_info,
        lppa_otdoa_cells_t  *p_otdoa_cells);

rrc_return_et lppa_compose_otdoa_cells(
        OSCTXT      *p_asn1_ctx,
        OSRTDList   *p_asn_otdoa_cells,
        s1ap_rrm_lppa_otdoa_info_resp_t  *p_otdoa_info_resp);

rrc_return_et  lppa_internal_encode_otdoa_information_resp(
        s1ap_rrm_lppa_otdoa_info_resp_t  *p_otdoa_resp,
        U8                *p_asn_msg,
        U16               *p_asn_msg_len);

rrc_return_et s1ap_parse_asn_dl_ue_associated_lppa_transport(
        OSCTXT      *p_asn1_ctx,
        s1ap_gb_context_t   *p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans);

rrc_return_et s1ap_decode_asn_s1ap_LPPa_PDU(
        s1ap_gb_context_t   *p_s1ap_gb_ctx,
        mme_context_t       * p_mme_ctx,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans,
        dl_non_ue_asso_lppa_trans_t  *p_non_ue_asso_lppa_trans,
        U8     lppa_asso_type);

rrc_return_et  dl_ue_associated_lppa_trans_intrl_dec(
        OSCTXT        *p_asn1_ctx,
        mme_context_t *p_mme_context,
        s1ap_DownlinkUEAssociatedLPPaTransport  *p_3gpp,
        dl_ue_asso_lppa_trans_t  *p_intrl);

void s1ap_lppa_process_e_CIDMeasurementInitiation(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   * p_mme_ctx,
        lppa_E_CIDMeasurementInitiationRequest *p_e_CIDMeasurementInitiation,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans,
        U32             lppa_trans_id,
        s1ap_gb_context_t   *p_s1ap_gb_ctx);   /*spr_11705_fix*/

void s1ap_lppa_process_e_CIDMeasurementTermination(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   * p_mme_ctx,
        lppa_E_CIDMeasurementTerminationCommand *p_e_CIDMeasurementTermination,
        dl_ue_asso_lppa_trans_t *p_ue_asso_lppa_trans,
        U32             lppa_trans_id,
        s1ap_gb_context_t   *p_s1ap_gb_ctx); /*spr_11705_fix*/

void s1ap_lppa_process_errorIndication(
        /*SPR 17777 +-*/
        mme_context_t   * p_mme_ctx,
        lppa_ErrorIndication  *p_errorIndication,
        U32             lppa_trans_id,
        U32             enb_ue_s1ap_id,
        U8              routing_id,
        U8              lppa_asso_type,
        s1ap_gb_context_t   *p_s1ap_gb_ctx); /*spr_11705_fix*/

rrc_return_et s1ap_lppa_decode_e_CIDMeasurementInitiationRequest(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   *p_mme_ctx,
        lppa_E_CIDMeasurementInitiationRequest *p_e_CIDMeasurementInitiation,
        s1ap_rrm_lppa_ecid_meas_init_req_t   *p_lppa_meas_init_req,
        rrc_lppa_error_indication_t *p_error_indication);

rrc_return_et s1ap_lppa_decode_e_CIDMeasurementTermination(
        OSCTXT          *p_asn1_ctx,
        mme_context_t   *p_mme_ctx,
        lppa_E_CIDMeasurementTerminationCommand *p_e_CIDMeasurementTermination,
        s1ap_rrm_lppa_ecid_meas_termination_cmd_t   *p_lppa_meas_term_cmd);

rrc_return_et s1ap_lppa_decode_ErrorIndication(
        /*SPR 17777 +-*/
        lppa_ErrorIndication *p_errorIndication,
        s1ap_rrm_lppa_error_ind_t *p_lppa_error_ind);

rrc_return_et  lppa_internal_decode_otdoa_information_req(
        OSCTXT         *p_asn1_ctx,
        lppa_OTDOAInformationRequest    *p_3gpp,
        s1ap_rrm_lppa_otdoa_info_req_t  *p_intrl,
        mme_context_t *p_mme_context,
        rrc_lppa_error_indication_t *p_error_indication);

rrc_return_et rrc_lppa_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 rrc_lppa_error_indication_t  *p_error_indication,
 mme_context_t  *p_mme_context
 );

rrc_return_et  s1ap_parse_asn_dl_non_ue_associated_lppa_transport(
        OSCTXT      *pctxt,         /* for memory allocation */
        /*SPR 17777 +-*/
        mme_context_t       * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        dl_non_ue_asso_lppa_trans_t *p_non_ue_asso_lppa_trans);

void s1ap_lppa_build_and_send_e_CIDMeasurementInitiationFailure(
        mme_context_t   *p_mme_ctx,
        U32             lppa_trans_id,
        rrc_lppa_error_indication_t *p_error_indication);

void s1ap_lppa_build_and_send_OTDOAInformationFailure(
        mme_context_t   *p_mme_ctx,
        U32             lppa_trans_id,
        rrc_lppa_error_indication_t *p_error_indication);

void lppa_log_criticality_diagostics(
        lppa_CriticalityDiagnostics *p_criticality_diagostics);

rrc_return_et  lppa_encode_ErrorIndication(
        U8                *p_asn_msg,
        U16               *p_asn_msg_len,
        s1ap_rrm_lppa_error_ind_req_t  *p_lppa_error_ind);

rrc_return_et lppa_compose_meas_init_fail_CriticalityDiagnostics(
        OSCTXT                       *p_asn1_ctx,
        lppa_CriticalityDiagnostics  *p_asn_crit_diag,
        lppa_CriticalityDiagnostics  *p_err_ind_crit_diag);

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/


#endif  /* _S1AP_LPPA_ASN_H_ */


