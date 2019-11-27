/*******************************************************************************
*
*  FILE NAME   : asn_dec_wrappers.h
*
*  DESCRIPTION : This file contains the structure and function declaration of
*                the x2ap interface procedures.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _ASN_DEC_WRAP_H_
#define _ASN_DEC_WRAP_H_

#include "x2ap_types.h"
#include "commoninc.h"
#include "rrc_x2apEnb_intf.h"
#include "rrc_s1ap_asn_intrl_3gpp.h"
#include "x2ap_db.h"
#include "uecc_x2ap_error_indication.h"
#include "x2ap_error_indication.h"


#define UECC_X2AP_MAX_ASN1_BUF_LEN 8192

typedef enum
{
    X2SETUP_REQ = 0,
    X2_SETUP_RESP,
    X2_SETUP_FAIL,
    ENB_CONFIGURE_UPDATE_REQ,
    ENB_CONFIGURE_UPDATE_RESP,
    ENB_CONFIGURE_UPDATE_FAIL,
    RESOURCE_STATUS_REQUEST,
    RESOURCE_STATUS_RESPONSE,
    RESOURCE_STATUS_FAILURE,
    RESOURCE_STATUS_UPDATE,
    RESET_REQUEST,
    RESET_RESPONSE,
    LOAD_INFORMATION,
    ERROR_INDICATION,
    RADIO_LINK_FAILURE_INDICATION,
    HANDOVER_REPORT,
    MOBILITY_CHANGE_REQUEST,
    MOBILITY_CHANGE_RESPONSE,
    MOBILITY_CHANGE_FAILURE,
    /*x2ap_start*/
    X2AP_UE_ASSO_SIG_MSG_IND_HO_REQ,
    X2AP_UE_ASSO_SIG_MSG_IND_HO_REQ_ACK,
    X2AP_UE_ASSO_SIG_MSG_IND_OTHERS,
    CELL_ACTIVATION_REQUEST,
    CELL_ACTIVATION_RESPONSE,
    CELL_ACTIVATION_FAILURE
#ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    ,ENDC_X2SETUP_REQ
    ,ENDC_X2SETUP_RESP
    ,ENDC_X2SETUP_FAIL
    ,X2AP_UE_ASSO_SIG_MSG_IND_SGNB_REL_REQ
    ,X2AP_UE_ASSO_SIG_MSG_IND_SGNB_ADD_REQ_ACK
    ,X2AP_UE_ASSO_SIG_MSG_IND_SGNB_REL_REQ_ACK
    ,X2AP_UE_ASSO_SIG_MSG_IND_SGNB_ADD_REQ_REJ
    ,ENDC_CONFIGURATION_UPDATE_REQ
    ,ENDC_CONFIGURATION_UPDATE_RESP
    ,ENDC_CONFIGURATION_UPDATE_FAIL
    ,X2AP_UE_ASSO_SIG_MSG_IND_SECONDARY_RAT_DATA_USAGE_REPORT
    /* MENB CHANGES - END */
#endif
}x2ap_EP_et;

/****************************FUNCTION-PROTITYPE******************************/

x2ap_return_et  x2ap_parse_peer_served_cells_info(
	U16* p_num_srv_cell,
	x2ap_served_cell_info_arr_t       *p_served_cells_info,
	x2ap_ServedCells             *p_ServedCells,
	x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et x2ap_decode_eNB_config_update_req (
    /*SPR_19067_FIX_START*/
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    /*SPR_19067_FIX_STOP*/
    x2ap_enb_config_update_ind_t   *p_enb_conf_req,
	U8                     *p_asn_msg,
	U32                    *p_asn_msg_len,
	x2ap_peer_enb_context_t* p_enb_cntx,
	/*SPR_19067_FIX_START*/ 
    rrc_x2ap_error_indication_t  *p_error_indication,
    x2ap_error_ind_bool_t        *p_send_err_ind,
    OSCTXT                       *p_asn1_ctx); 
    /*SPR_19067_FIX_STOP*/

x2ap_return_et x2ap_decode_x2setup_req (
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2_setup_req_t   *p_x2_setup_req,
        U8                *p_asn_msg,
        U32               *p_asn_msg_len,
        rrc_x2ap_error_indication_t *p_error_indication,
        x2ap_error_ind_bool_t       *p_send_err_ind,
		/*SPR_19067_FIX_START*/
        OSCTXT                      *p_asn1_ctx);
		/*SPR_19067_FIX_STOP*/

x2ap_return_et x2ap_decode_x2setup_resp(
        x2ap_gb_context_t           *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t     *p_enb_ctx,
        x2_setup_resp_t             *p_setup_resp,
        U8                          *p_asn_msg,
		/*SPR_19067_FIX_START*/
        U32                         *p_asn_msg_len,
    	OSCTXT                      *p_asn1_ctx);
        /*SPR_19067_FIX_STOP*/

x2ap_return_et x2ap_decode_x2setup_failure_resp(
        x2ap_gb_context_t           *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t     *p_enb_ctx,
        x2_setup_fail_t             *p_setup_fail,
        U8                          *p_asn_msg,
		/*SPR_19067_FIX_START*/
        U32                         *p_asn_msg_len,
    	OSCTXT                      *p_asn1_ctx);
        /*SPR_19067_FIX_STOP*/

x2ap_return_et x2ap_decode_eNB_config_update_ack(
        x2ap_enb_conf_ack_t         *p_enb_conf_ack,
        U32                         *msg_result,
        U8                          *p_asn_msg,
		/*SPR_19067_FIX_START*/
        U32                         *p_asn_msg_len,
 	rrc_x2ap_error_indication_t *p_error_indication,
	x2ap_error_ind_bool_t       *p_send_err_ind,
    	OSCTXT                      *p_asn1_ctx,
        x2ap_gb_context_t           *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t     *p_enb_ctx);
        /*SPR_19067_FIX_STOP*/

x2ap_return_et x2ap_decode_eNB_config_update_resp_fail(
        x2ap_enb_conf_upd_fail_t    *p_enb_conf_resp_fail,
        U32                         *msg_result,
        U8                          *p_asn_msg,
        U32                         *p_asn_msg_len,
		/*SPR_19067_FIX_START*/
        x2ap_gb_context_t	    *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t	    *p_enb_ctx,
		rrc_x2ap_error_indication_t *p_error_indication,
		x2ap_error_ind_bool_t       *p_send_err_ind,
		OSCTXT                	    *p_asn1_ctx); 
        /*SPR_19067_FIX_STOP*/

x2ap_return_et x2ap_decode_rlf_indication(
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_rlf_ind_info_t   *p_rlf_ind,
	U8                     *p_asn_msg,
	U32                    *p_asn_msg_len,
	x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et x2ap_decode_handover_report(
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_ho_report_ind_t   *p_ho_report_ind,
	U8                     *p_asn_msg,
	U32                    *p_asn_msg_len,
	x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et x2ap_partially_decode_asn_msg(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        U8 * p_asn_msg,
        U16 asn_msg_len,
        x2ap_EP_et  *p_x2ap_EP,
        x2ap_ProcedureCode    *proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId
#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
       ,U32                       * p_mEnbUeX2apId,
        U32                       * p_sgnbUeX2apId
/*NR_DC Code Change Stop*/
#endif
);

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
void x2ap_internal_decode_nr_cg_config(
        x2ap_SgNBtoMeNBContainer    *p_container,
        nr_rrc_CG_Config            *sgnb_to_menb_container,
        OSCTXT                      *p_asn1_ctx,
        x2ap_Cause                  *cause);

x2ap_return_et x2ap_internal_decode_sgnb_addition_request_ack(
    OSCTXT                           *p_asn1_ctx,
    x2ap_SgNBAdditionRequestAcknowledge
                                     *p_3gpp,
    rrc_x2ap_sgnb_addition_req_ack_t
                                     *p_intrl,
    U8                               peer_enodeb_id
);
/*NR_DC Code Change End*/
#endif

x2ap_return_et x2ap_internal_decode_handover_request(
        OSCTXT*                         p_ctx,
        x2ap_HandoverRequest             *p_3gpp,    
        rrc_x2ap_handover_request_t      *p_intrl,
        U8                  peer_enodeb_id,      /* Peer EnodeB ID */
        rrc_x2ap_error_indication_t *p_error_ind_content,
        uecc_x2ap_error_ind_bool_t *p_send_err_ind
        );    

x2ap_return_et x2ap_internal_decode_handover_request_ack(
        OSCTXT*                         p_ctx,
        OSCTXT                           *p_ho_cmd_ctxt,                
        x2ap_HandoverRequestAcknowledge  *p_3gpp,    
        rrc_x2ap_handover_req_ack_t      *p_intrl,    
        U8  peer_enodeb_id); /* Peer EnodeB ID */

x2ap_return_et x2ap_internal_decode_handover_prep_fail(
        OSCTXT*                         p_ctx,
        x2ap_HandoverPreparationFailure     *p_3gpp,  
        rrc_x2ap_handover_prep_fail_t    *p_intrl,
        U8  peer_enodeb_id); /* Peer EnodeB ID */

x2ap_return_et x2ap_internal_decode_sn_status_transfer(
        OSCTXT*                         p_ctx,
        x2ap_SNStatusTransfer            *p_3gpp,    
        rrc_x2ap_sn_status_transfer_t    *p_intrl,
        U8    peer_enodeb_id); /* Peer EnodeB ID */

x2ap_return_et x2ap_internal_decode_ue_context_release(
        OSCTXT*                         p_ctx,
        x2ap_UEContextRelease            *p_3gpp,    
        rrc_x2ap_ue_context_release_t    *p_intrl,
        U8  peer_enodeb_id);


x2ap_return_et x2ap_internal_decode_handover_cancel(
        OSCTXT*                           p_ctx,
        x2ap_HandoverCancel               *p_3gpp,
        rrc_x2ap_handover_cancel_t        *p_intrl,
        U8    peer_enodeb_id);/* Peer EnodeB ID */

x2ap_return_et rrc_x2ap_pdu_intrl_dec(
        OSCTXT         *pctxt,
        U8             *p_buff,   
        U16            buff_size, 
        x2ap_X2AP_PDU  *p_x2ap_pdu);


void rrc_x2ap_last_visited_utran_cell_information_intrl_dec(
        OSCTXT                                  *p_asn1_ctxt,
        x2ap_LastVisitedUTRANCellInformation    *p_utran_cell,
        ranap_LastVisitedUTRANCell_Item         *p_intrl,
        x2ap_Cause                              *cause);

x2ap_return_et rrc_x2ap_error_indication_intrl_dec
(
 x2ap_ErrorIndication    *p_3gpp,    /* unpacked src */
 rrc_x2ap_error_indication_t *p_intrl/* dest */
);

x2ap_return_et  x2ap_parse_cause (
        x2_cause_t    *p_cause,
        x2ap_Cause     *p_Cause,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id);

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/*****************************************************************************
 * Function Name  : x2ap_decode_endc_x2setup_resp
 * Inputs         : x2ap_gb_context_t          *p_x2ap_gl_ctx
 *                  x2ap_peer_enb_context_t    *p_enb_ctx
 *                  en_dc_x2_setup_resp_t      *p_setup_resp
 *                  U8                         *p_asn_msg
 *                  U32                        *p_asn_msg_len
 *                  OSCTXT                     *p_asn1_ctx
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode EN DC X2 Setup response.
 ********************************************************************************/
x2ap_return_et
x2ap_decode_endc_x2setup_resp
(
    x2ap_gb_context_t          *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t    *p_enb_ctx,
    en_dc_x2_setup_resp_t      *p_setup_resp,
    U8                         *p_asn_msg,
    U32                        *p_asn_msg_len,
    OSCTXT                     *p_asn1_ctx
);
/* MENB CHANGES - END */

/*EN-DC_chagnes_start*/
x2ap_return_et x2ap_internal_decode_menb_initiated_sgnb_release_ack
(
 OSCTXT      *p_asn1_ctx,
 x2ap_SgNBReleaseRequestAcknowledge            *p_3gpp,     /* unpacked src */
 rrc_x2ap_sgnb_release_request_ack_t    *p_intrl,    /* dest */
 U8  peer_enodeb_id /* Peer EnodeB ID */
);

x2ap_return_et
x2ap_decode_endc_x2setup_resp
(
    x2ap_gb_context_t           *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t     *p_enb_ctx,
    en_dc_x2_setup_resp_t       *p_setup_resp,
    U8                          *p_asn_msg,
    U32                         *p_asn_msg_len,
    OSCTXT                      *p_asn1_ctx
);

x2ap_return_et
x2ap_decode_endc_x2setup_failure_resp
(
    x2ap_gb_context_t           *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t     *p_enb_ctx,
    x2_setup_fail_t             *p_setup_fail,
    U8                          *p_asn_msg,
    U32                         *p_asn_msg_len,
    OSCTXT                      *p_asn1_ctx
);

x2ap_return_et
x2ap_decode_endc_x2setup_req
(
    x2ap_gb_context_t           *p_x2ap_gl_ctx,
    x2ap_peer_enb_context_t     *p_enb_ctx,
    en_dc_x2_setup_req_t        *p_x2_setup_req,
    U8                          *p_asn_msg,
    U32                         *p_asn_msg_len,
    rrc_x2ap_error_indication_t *p_error_indication,
    x2ap_error_ind_bool_t       *p_send_err_ind,
    OSCTXT                      *p_asn1_ctx
);

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_sgnb_initiated_sgnb_release
 * Inputs         : p_asn1_ctx     - pointer to OSCTXT
 *                  p_3gpp         - x2ap_SgNBReleaseRequired
 *                  p_intrl        - pointer to rrc_x2ap_sgnb_release_required_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode SgNB Initiated SgNB release req
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_sgnb_initiated_sgnb_release
(
     OSCTXT                              *p_asn1_ctx,
     x2ap_SgNBReleaseRequired            *p_3gpp,
     rrc_x2ap_sgnb_release_required_t    *p_intrl,
     U8                                  peer_enodeb_id
);

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_secondary_rat_data_usage_report 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_SNStatusTransfer
 *                  p_intrl - pointer to rrc_x2ap_sn_status_transfer_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode SECONDARY RAT DATA USAGE REPORT
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_secondary_rat_data_usage_report
(
 OSCTXT      *p_asn1_ctx,
 x2ap_SecondaryRATDataUsageReport   *p_3gpp,    /* unpacked src */
 rrc_x2ap_secondary_rat_data_usage_report_t    *p_intrl,    /* dest */
 U8    peer_enodeb_id /* Peer EnodeB ID */
);
/*EN-DC_chagnes_end*/
#endif
#endif

