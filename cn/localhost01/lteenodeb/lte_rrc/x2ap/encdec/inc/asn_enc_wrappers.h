/*******************************************************************************
 *
 *  FILE NAME   : asn_enc_wrappers.h
 *
 *  DESCRIPTION : This file contains the structure and function declaration of 
 *                the x2ap interface procedures. 
 *
 *  DATE	24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
 *
 *  Copyright 2007, Aricent Inc.
 *
 *******************************************************************************/
#ifndef _ASN_ENC_WRAP_H_
#define _ASN_ENC_WRAP_H_

#include "commoninc.h"
#include "rrc_x2apEnb_intf.h"
#include "rrc_x2apCsc_intf.h"
#include "x2ap_types.h"
#include "x2ap_error_indication.h"
#include "x2ap_db.h"

/******************************FUNCTION-PROTOTYPE******************************/

x2ap_return_et x2ap_encode_eNB_Configuration_Update_failure_resp(
        x2ap_enb_conf_upd_fail_t *p_enb_conf_upd_fail,
        U8                        *p_asn_msg,
        U16                       *p_asn_msg_len);

x2ap_return_et x2ap_encode_x2setup_req(
        x2_setup_req_t   *p_s1_setup_req,
        U8               *p_asn_msg,
        U16              *p_asn_msg_len);

x2ap_return_et x2ap_encode_x2setup_resp(
        x2_setup_resp_t   *p_x2_setup_resp,
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len);

x2ap_return_et x2ap_encode_x2setup_failure_resp (
        x2_setup_fail_t   *p_x2_setup_resp,
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len,
        rrc_x2ap_error_indication_t *p_error_indication,
        x2ap_error_ind_bool_t *p_snd_err_ind);

x2ap_return_et x2ap_encode_eNB_Configuration_Update_req(
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	x2ap_enb_config_update_req_t   *p_eNB_config_upd_req,
	U8                     *p_asn_msg,
	U16                    *p_asn_msg_len);

x2ap_return_et x2ap_encode_eNB_Configuration_Update_ack(
        x2ap_enb_conf_ack_t        *p_enb_conf_ack,
        U8                          *p_asn_msg,
        U16                         *p_asn_msg_len);

x2ap_return_et x2ap_internal_encode_handover_req(
        OSCTXT                         *pctxt,        
        U8                             *p_buff,      
        U16                            *p_buff_size, 
        rrc_x2ap_handover_request_t    *p_handover_req);

x2ap_return_et x2ap_internal_encode_handover_req_ack(
        OSCTXT                         *pctxt,     
        U8                             *p_buff,    
        U16                            *p_buff_size,
        rrc_x2ap_handover_req_ack_t    *p_handover_req_ack);

x2ap_return_et x2ap_internal_encode_handover_preparation_failure(
        OSCTXT                         *pctxt,      
        U8                             *p_buff,  
        U16                            *p_buff_size,   
        rrc_x2ap_handover_prep_fail_t  *p_handover_prep_fail);

x2ap_return_et x2ap_internal_encode_sn_status_transfer(
        OSCTXT                         *pctxt,        
        U8                             *p_buff,   
        U16                            *p_buff_size, 
        rrc_x2ap_sn_status_transfer_t  *p_sn_status_transfer);

x2ap_return_et x2ap_internal_encode_ue_context_release(
        OSCTXT                         *pctxt,      
        U8                             *p_buff,      
        U16                            *p_buff_size,  
        rrc_x2ap_ue_context_release_t  *p_ue_context_release);

x2ap_return_et x2ap_internal_encode_handover_cancel(
        OSCTXT                         *pctxt,       
        U8                             *p_buff,   
        U16                            *p_buff_size,
        rrc_x2ap_handover_cancel_t     *p_handover_cancel);

rrc_return_et x2ap_internal_encode_rrc_context(
    OSCTXT            *pctxt, /* for memory allocation */
    x2ap_RRC_Context  *p_rrc_context,

    /* source */
    HandoverPreparationInformation *p_ho_preparation_information);

x2ap_return_et x2ap_internal_encode_handover_command(
    OSCTXT      *pctxt,         /* for memory allocation */
HandoverCommand* p_ho_cmd_msg,
x2ap_TargeteNBtoSource_eNBTransparentContainer* p_container);

x2ap_return_et x2ap_encode_rlf_indication(
        x2ap_gb_context_t      * p_x2ap_gb_ctx,
        x2ap_ue_reestab_conn_setup_reconfig_t  *p_reestab_conn_setup_reconfig,
        U8                     *p_asn_msg,
        U16                    *p_asn_msg_len);

x2ap_return_et x2ap_encode_handover_report(
        x2ap_ho_report_info_t   *p_ho_report_ind,
        U8                     *p_asn_msg,
        U16                    *p_asn_msg_len);

x2ap_return_et rrc_x2ap_error_indication_tse_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size   /* size of result buffer */
 );

x2ap_return_et rrc_x2ap_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 rrc_x2ap_error_indication_t  *p_error_indication
 );

x2ap_return_et rrc_uecc_x2ap_error_indication_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_error_indication_t  *p_error_indication
);

#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
x2ap_return_et x2ap_internal_encode_nr_rrc_radio_bearer_config
(
    OSCTXT                   *pctxt,  /* for memory allocation */
    nr_rrc_RadioBearerConfig *p_radio_bearer_config,
    OSDynOctStr              *p_oct_str
);
/* OPTION3X Changes End */
/*NR_DC Code Change Start*/
x2ap_return_et x2ap_internal_encode_menb_to_sgnb_container
(
    OSCTXT                   *pctxt,  /* for memory allocation */
    nr_rrc_CG_ConfigInfo     *p_nr_cg_config_info,
    x2ap_MeNBtoSgNBContainer *p_container
);

x2ap_return_et x2ap_encode_sgNB_addition_request(
        OSCTXT                          *pctxt,
        U8                              *p_asn_msg,
        U16                             *p_asn_msg_len,
        rrc_x2ap_sgnb_addition_request_t    *p_sgnb_addition_req
);

x2ap_return_et x2ap_encode_sgNB_reconfig_complete(
        OSCTXT                          *pctxt,
        U8                              *p_asn_msg,
        U16                             *p_asn_msg_len,
        rrc_x2ap_sgnb_reconfig_complete_t
                                        *p_sgnb_reconfig_complete
);
/*NR_DC Code Change End*/
/* MENB CHANGES - START */
/*****************************************************************************
* FUNCTION NAME  : x2ap_encode_endc_x2setup_req
* INPUT          : x2_setup_req_t       *p_x2_endc_setup_req
*                  U8                   *p_asn_msg
*                  U16                  *p_asn_msg_len
* OUTPUT         : U8                   *p_asn_msg
*                  U16                  *p_asn_msg_len
* DESCRIPTION    : This function encode the EN DC X2 setup request
* RETURNS        : X2AP_SUCCESS - if successfully encoded the message
*                  X2AP_FAILURE - if fails to encode the message
*******************************************************************************/
x2ap_return_et
x2ap_encode_endc_x2setup_req
(
    en_dc_x2_setup_req_t     *p_x2_endc_setup_req,
    U8                     *p_asn_msg,
    U16                    *p_asn_msg_len
);
/* MENB CHANGES - END */

/*EN-DC_changes_start*/
x2ap_return_et x2ap_internal_encode_sgnb_release_request
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 /* source */
 rrc_x2ap_sgnb_release_request_t  *p_sgnb_rel_request
 );

x2ap_return_et x2ap_encode_endc_x2setup_failure_resp
(
    x2_setup_fail_t                *p_x2_setup_resp,
    U8                             *p_asn_msg,
    U16                            *p_asn_msg_len,
    rrc_x2ap_error_indication_t    *p_error_indication,
    x2ap_error_ind_bool_t          *p_snd_err_ind
);

x2ap_return_et
x2ap_encode_endc_x2setup_resp
(
    en_dc_x2_setup_resp_t   *p_x2_setup_resp,
    U8                      *p_asn_msg,
    U16                     *p_asn_msg_len
);

/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_sgnb_release_confirm
 * INPUTS        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 * OUTPUTS       : p_buff (ASN.1 encoded message),
 *                 p_buff_size (size of result buffer)
 * RETURNS       : X2AP_SUCCESS / X2AP_FAILURE
 * DESCRIPTION   : This function used to encode SgNB release confirm.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_sgnb_release_confirm
(
    OSCTXT                            *pctxt,
    U8                                *p_buff,
    U16                               *p_buff_size,
    rrc_x2ap_sgnb_release_confirm_t   *p_sgnb_rel_confirm
);
x2ap_return_et x2ap_encode_ENDC_Configuration_Update_ack (
        x2ap_endc_config_update_ack_t   *p_endc_conf_ack,
        UInt8                              *p_asn_msg,
        UInt16                             *p_asn_msg_len);
        
x2ap_return_et x2ap_encode_endc_config_update_failure(
        x2ap_endc_config_update_failure_t   *p_endc_config_update_fail,
        UInt8                                  *p_asn_msg,
        UInt16                                 *p_asn_msg_len);
        
x2ap_return_et x2ap_encode_ENDC_Configuration_Update_req
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_endc_config_update_req_t   *p_endc_config_update_req,
    U8                     *p_asn_msg,
    U16                    *p_asn_msg_len
);
/*EN-DC_changes_end*/
#endif
#endif
