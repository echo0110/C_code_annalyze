/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_asn_codec.h,v 1.6 2010/10/13 07:04:05 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the structs and function
 *                     declarations necessary to encode/decode Asn messages
 *                     required by S1AP module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_asn_codec.h,v $
 * Revision 1.6  2010/10/13 07:04:05  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.4.4.3  2010/10/07 10:59:52  gur04518
 * map type ASN1V_s1ap_id_Paging to S1_PAGING
 *
 * Revision 1.4.4.2  2010/10/07 06:56:00  gur04518
 * introduced HO_RESOURCE_ALLOCATION as an enum to map the resource allocation request
 *
 * Revision 1.4.4.1  2010/08/05 08:10:44  gur15697
 * merging round 2
 *
 * Revision 1.5  2010/07/24 07:18:48  gur04733
 * Checking On 24 July
 *
 * Revision 1.4.2.2  2010/06/30 11:59:49  gur10248
 * *** empty log message ***
 *
 * Revision 1.4  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.3  2010/02/18 04:25:20  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.8  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.7  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.6  2009/12/22 14:38:21  gur18569
 * made mme group ids as list
 *
 * Revision 1.1.2.5  2009/12/18 04:20:26  gur18569
 * added ue id pair presence argument
 *
 * Revision 1.1.2.4  2009/12/08 14:13:13  gur18569
 * using message id from ASN files SPR:161
 *
 * Revision 1.1.2.3  2009/12/04 13:54:36  gur18569
 * added s1_setup_resp fields
 *
 * Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.7  2009/11/20 08:40:43  gur18569
 * changes during UT
 *
 * Revision 1.1.2.6  2009/11/10 11:41:24  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.5  2009/11/09 09:02:10  gur18569
 * removed compilation warnings
 *
 * Revision 1.1.2.4  2009/10/29 05:32:22  gur18569
 * added compose_paging for testing purpose
 *
 * Revision 1.1.2.3  2009/10/28 07:18:33  gur18569
 * added structs for asn buff generation
 *
 * Revision 1.1.2.2  2009/10/27 06:14:17  gur18569
 * added ASN_MSG_TO_FORWARD and INVALID_S1AP_EP
 *
 * Revision 1.1.2.1  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_ASN_H_
#define _S1AP_ASN_H_

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

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/


/** test structs */
typedef struct
{
    s1ap_Cause cause;
    U32 time_to_wait;
}s1_setup_failure_t;
/** test structs */

#define MAX_ASN_EP_ID 140
typedef enum {
    S1_SETUP_RESPONSE = MAX_ASN_EP_ID,
    S1_SETUP_FAILURE,
    RESET,
    RESET_ACK,
    HO_RESOURCE_ALLOCATION,
    S1_PAGING,
    ASN_MSG_TO_FORWARD,
    MME_OVERLOAD_START,
    MME_OVERLOAD_STOP,
    eNB_CONFIGURATION_UPDATE_ACK,
    eNB_CONFIGURATION_UPDATE_FAILURE,
    MME_CONFIGURATION_UPDATE,
    MME_DIRECT_INFO_TRANSFER,
    MME_CONFIGURATION_TRANSFER,
    LOCATION_REPORTING_CONTROL,
    ERROR_INDICATION,
    WRITE_REPLACE_WARNING,
    KILL_REQUEST,
    LPPA_UE_ASSO_DL_TRANSPORT,
    LPPA_NON_UE_ASSO_DL_TRANSPORT,
    INVALID_S1AP_EP
}s1ap_EP_et;

/* Used for eNB configuration Update */
typedef enum
{
    INVOKED_BY_INVALID,
    INVOKED_BY_OAM,
    INVOKED_BY_TIMER_EXPIRY,
    INVOKED_BY_FAILURE
} enb_config_update_invoked_et;

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
rrc_return_et s1ap_partially_decode_asn_msg (
        U8 * p_asn_msg,
        U16 asn_msg_len,
        s1ap_EP_et * p_s1ap_EP,
        s1ap_MME_UE_S1AP_ID* p_mmeUeS1APID,
        s1ap_ENB_UE_S1AP_ID* p_enbUeS1APID,
        U8 * p_ue_id_pair_presence,
        s1ap_ProcedureCode *procedureCode,
        s1ap_message_type_et  *message_type);

rrc_return_et compose_asn_s1_setup_req (
        s1_setup_req_t   *p_s1_setup_req,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len);

rrc_return_et compose_asn_reset(
        reset_req_t   *p_reset_req,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len);

rrc_return_et compose_asn_reset_ack(
        s1ap_reset_info_t   *p_reset_info,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind);

rrc_return_et parse_asn_s1_setup_response (
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        parsed_s1_setup_rsp_t  * p_parsed_s1_setup_rsp);

rrc_return_et parse_asn_s1_setup_failure (
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        U32                  * p_time_to_wait,
	s1ap_Cause           *cause);

rrc_return_et parse_asn_reset_ack (
        mme_context_t *p_mme_context,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        s1ap_reset_info_t  * p_reset_info);

rrc_return_et parse_asn_reset(
        s1ap_gb_context_t    * p_s1ap_gb_ctx,
        mme_context_t        * p_mme_ctx,
        void                *p_asn_msg,
        U16                 asn_msg_len,
        reset_req_t        * p_reset_req,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind);

rrc_return_et compose_asn_s1_setup_failure (
        s1_setup_failure_t   *p_s1_setup_failure,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len);

rrc_return_et compose_paging (
        U8 * p_asn_msg,
        U16 *p_asn_msg_len);

rrc_return_et parse_asn_overload_start(
     mme_context_t        * p_mme_ctx,
    void  *p_asn_msg,
    U16 asn_msg_len,
    parsed_overload_start_t *p_parsed_overload_start_msg);

mme_overload_response_et convert_overload_response(
        s1ap_OverloadAction asn_overload_response);

rrc_return_et parse_asn_overload_stop(
    mme_context_t           *p_mme_ctx,
        void  *p_asn_msg,
        U16 asn_msg_len,
        parsed_overload_stop_t *p_parsed_overload_stop_msg);

rrc_return_et parse_asn_enb_config_update_ack( 
        mme_context_t           *p_mme_ctx,
        void                      *p_asn_msg,
        U16                       asn_msg_len
        );


rrc_return_et parse_asn_mme_configuration_update(
        OSCTXT *p_asn1_ctx,
        mme_context_t        * p_mme_ctx,
        U8* p_asn_msg,
        U16 asn_msg_len,
        mme_configuration_update_t *p_mme_config_update,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_send_err_ind);

rrc_return_et s1ap_mme_configuration_update_intrl_dec
(
 OSCTXT *p_asn1_ctx,
 mme_context_t *p_mme_ctx,
 s1ap_MMEConfigurationUpdate *p_3gpp,  /* unpacked src */
 mme_configuration_update_t  *p_intrl, /* dest */
 rrc_s1ap_error_indication_t *p_err_ind,
 s1ap_error_ind_bool_t       *p_send_err_ind
 );

rrc_return_et s1ap_mme_configuration_transfer_intrl_dec(
        mme_context_t *p_mme_ctx,
        s1ap_MMEConfigurationTransfer *p_3gpp,
        s1ap_MMEConfigurationTransfer_protocolIEs_element *p_elem
        );


rrc_return_et compose_asn_enb_config_update(
        s1ap_oam_enb_config_update_t *p_enb_config_update,
        /*SPR 17777 +-*/
        U8 *asn_enc_msg_buf,
        U16 *asn_enc_buf_len);/*SPR 17777 +-*/

rrc_return_et compose_asn_enb_direct_info_transfer (
        s1ap_rrm_enb_direct_info_transfer_t *p_enb_info_msg,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len);

rrc_return_et compose_asn_enb_configuration_transfer (
        s1ap_rrm_enb_config_transfer_t *p_enb_config_transfer,
        s1ap_gb_context_t *p_s1ap_gb_ctx,
        U8 * p_asn_msg,
        U16 *p_asn_msg_len);

rrc_return_et parse_s1ap_mme_direct_info_transfer(
        mme_context_t        * p_mme_ctx,
        s1ap_rrm_mme_direct_info_transfer_t *p_mme_direct_info,
        void *p_asn_msg,
        U16 asn_msg_len
);

rrc_return_et parse_s1ap_mme_configuration_transfer(
        mme_context_t        * p_mme_ctx,
        s1ap_rrm_mme_config_transfer_t *p_mme_config_transfer,
        s1ap_gb_context_t *p_s1ap_gb_ctxt,
        void *p_asn_msg,
        U16 asn_msg_len);

rrc_return_et parse_s1ap_TargeteNB_ID(
        rrm_gb_enb_id_t *p_gb_enb_id,
        rrm_supp_ta_t *p_rrm_supp_ta,
        s1ap_TargeteNB_ID *p_s1ap_TargeteNB_ID);

rrc_return_et parse_s1ap_SourceeNB_ID(
        rrm_gb_enb_id_t *p_gb_enb_id,
        rrm_supp_ta_t *p_rrm_supp_ta,
        s1ap_SourceeNB_ID *p_s1ap_TargeteNB_ID);

rrc_return_et parse_error_indication (
    rrc_s1ap_error_indication_t *p_parsed_error_indication,
    void                      *p_asn_msg,
    U16                        asn_msg_len,
    s1ap_gb_context_t          *p_s1ap_gb_ctx);
/* etws_start*/
rrc_return_et s1ap_write_replace_warning_req_intrl_dec
(
    s1ap_WriteReplaceWarningRequest    *p_3gpp,    /* unpacked src */
    rrc_s1ap_write_replace_warning_request_t *p_intrl,/* dest */
    mme_context_t *p_mme_context
);
/* etws_end*/

rrc_return_et rrc_s1ap_error_indication_tse_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size   /* size of result buffer */
);

rrc_return_et  compose_s1ap_mme_configuration_update_ack
(
        U8 *p_message,
        U16*  p_msg_buff_len,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_snd_err_ind
);

rrc_return_et  compose_s1ap_mme_configuration_update_failure
(
        U8* p_message,
        U16* p_message_buf_len,
        rrc_s1ap_error_indication_t *p_err_ind,
        s1ap_error_ind_bool_t *p_snd_err_ind
);

rrc_return_et rrc_s1ap_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 rrc_s1ap_error_indication_t  *p_error_indication
);

rrc_return_et parse_asn_enb_config_update_failure(
        mme_context_t *p_mme_context,
        void *p_asn_msg,
        U16 asn_msg_len,
        enb_config_update_failure_t *p_enb_config_update_failure);

rrc_return_et parse_asn_write_replace_warning_req (
        void                *p_asn_msg,
        OSCTXT              *p_asn1_ctx,
        U16                 asn_msg_len,
        rrc_s1ap_write_replace_warning_request_t *p_rrc_s1ap_write_replace_warning_request,
        mme_context_t *p_mme_context);

rrc_return_et decode_s1ap_asn_kill_request
(
    void                *p_asn_msg,
    OSCTXT              *p_asn1_ctx,
    U16                 asn_msg_len,
    rrc_s1ap_kill_request_t *p_rrc_s1ap_kill_request,
    mme_context_t* p_mme_ctx
);

void s1ap_generate_plmn_identity_to_int
(
    plmn_identity_t    *p_plmn_identity,
    OSOCTET            *p_s1ap_plmn_identity
);

/****************************************************************************
 * Exported Constants
 ****************************************************************************/


#endif  /* _S1AP_ASN_H_ */


