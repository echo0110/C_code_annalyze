
/*******************************************************************************
*
*  FILE NAME   : x2ap_rlf.c
*
*  DESCRIPTION : This file contains the X2AP RLF state machine 
*                and the state machine Entry function definition. 
*
*  DATE 03-Oct-2012  NAME  Rajat Aggarwal
*                    REFERENCE  36.423_960(3GPP Spec) REASON Initial
*
*  Copyright 2010, Aricent Inc.
*
*******************************************************************************/
#include "x2ap_uecc.h"
#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "rrc_x2apCommon_intf.h"
#include "x2ap_utils.h"
#include "rrc_x2apCommon_il_composer.h"
#include "asn_dec_wrappers.h"
#include "asn_enc_wrappers.h"
#include "sm_entry.h"


x2ap_return_et x2ap_process_radio_link_failure_indication(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_send_rlf_indication(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        U8* p_asn_msg,
        U16 asn_msg_len);

x2ap_return_et x2ap_build_and_send_rlf_indication(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig);

/****************************************************************************
 * Function Name  : x2ap_process_radio_link_failure_indication
 * Inputs         : p_msg_ptr  -    Pointer to message buffer
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the rlf indication
 *                  mesaage received from peer enb.
****************************************************************************/
x2ap_return_et x2ap_process_radio_link_failure_indication(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et       retVal = X2AP_SUCCESS;
    U8 msg_ind_buf[sizeof(x2ap_rlf_ind_info_t)+X2AP_MAX_ASN1_BUF_LEN];
    x2ap_rlf_ind_info_t *p_rlf_ind = 
        (x2ap_rlf_ind_info_t *)msg_ind_buf;

    void           *p_intrl_msg = PNULL;
    x2ap_rlf_ind_info_t *msg_p       = PNULL;

    x2ap_peer_enb_msg_t* p_peer_enb_msg = 
                        (x2ap_peer_enb_msg_t*)p_msg_ptr ;

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    memset_wrapper(msg_ind_buf, 0, sizeof(x2ap_rlf_ind_info_t)+X2AP_MAX_ASN1_BUF_LEN);

    if(!(p_x2ap_gb_ctx->x2ap_link_info[p_enb_ctx->peer_enodeb_id].
                                connection_status))
    {
        X2AP_TRACE(X2AP_ERROR,"Peer eNB connection is down.\n");
        return X2AP_FAILURE;
    }

    retVal = x2ap_decode_rlf_indication(p_x2ap_gb_ctx,
                                    p_rlf_ind,
                                    (p_peer_enb_msg)->p_msg,
                                    &(p_peer_enb_msg->msg_len),
                                    p_enb_ctx);
    /* Send indication to UECC */
    if (X2AP_SUCCESS == retVal)
    {
        p_intrl_msg = x2ap_alloc_intrl_msg(
                        RRC_UECC_MODULE_ID,
                        X2AP_UE_RLF_MSG_IND,
                        (U16)(sizeof(x2ap_rlf_ind_info_t) 
                            + p_rlf_ind->rlf_report.message_buff_size));
        if (PNULL == p_intrl_msg)
        {
            /*out of memory*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        msg_p = (x2ap_rlf_ind_info_t *)((U8 *)p_intrl_msg +
                        RRC_API_HEADER_SIZE);
        msg_p->bitmask = p_rlf_ind->bitmask;
        X2AP_MEMCPY(&msg_p->enb1_gb_id,
                &p_enb_ctx->gb_enb_id, sizeof(x2_gb_enb_id_t));
        X2AP_MEMCPY(&msg_p->enb2_gb_id,
                &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
        msg_p->failure_cell_pci = p_rlf_ind->failure_cell_pci;
        msg_p->re_est_cell_ecgi = p_rlf_ind->re_est_cell_ecgi;
        msg_p->crnti = p_rlf_ind->crnti;
        if (p_rlf_ind->bitmask & X2AP_SHORT_MAC_I_PRESENT)
        {
            msg_p->short_mac_i = p_rlf_ind->short_mac_i;
        }
        if (p_rlf_ind->bitmask & X2AP_RLF_REPORT_CONTAINER_PRESENT)
        {
            msg_p->rlf_report.message_buff_size = 
                p_rlf_ind->rlf_report.message_buff_size;
            X2AP_MEMCPY(msg_p->rlf_report.message_buff_p,
                    p_rlf_ind->rlf_report.message_buff_p,
                    p_rlf_ind->rlf_report.message_buff_size);
        }
        if (p_rlf_ind->bitmask & X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT)
        {
            msg_p->conn_setup_indicator = p_rlf_ind->conn_setup_indicator;
        }


        rrc_send_message(p_intrl_msg,
                RRC_UECCMD_MODULE_ID);
        X2AP_TRACE(X2AP_INFO, "[X2AP_UE_RLF_MSG_IND] send to UECC module.");
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR, "Decoding RLF Indication failed.\n");
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/****************************************************************************
 * Function Name  : x2ap_build_and_send_rlf_indication
 * Inputs         : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_peer_enb_ctx - Pointer to peer eNB context
 *                : p_reestab_conn_setup_reconfig - pointer to 
 *                   x2ap_ue_reestab_conn_setup_reconfig_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to build and send the
 *                  rlf indication mesaage to peer enb.
****************************************************************************/
x2ap_return_et x2ap_build_and_send_rlf_indication(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig)
{
    U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_msg_len = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_x2ap_gb_ctx  != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_reestab_conn_setup_reconfig  != X2AP_P_NULL);

    /* Encoding x2ap_RLFIndication */
    if ( !(x2ap_encode_rlf_indication(
                    p_x2ap_gb_ctx,
                    p_reestab_conn_setup_reconfig,
                    asn_msg_buff,
                    &asn_msg_len)))
    {
        X2AP_TRACE(X2AP_ERROR,"Encoding RLF Indication failed.\n");
        return X2AP_FAILURE;
    }

    if (p_x2ap_gb_ctx->x2ap_link_info[p_peer_enb_ctx->peer_enodeb_id].
            connection_status)
    {
        /* Send RLF Indication to peer eNB */
        if ( !(x2ap_send_rlf_indication(p_x2ap_gb_ctx,
                                            p_peer_enb_ctx,
                                            asn_msg_buff, 
                                            asn_msg_len)))
        {
            X2AP_TRACE(X2AP_ERROR,"Sending RLF Indication failed.\n");
            return X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"Peer eNB link down");
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}


/****************************************************************************
 * Function Name  : x2ap_send_rlf_indication
 * Inputs         : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_peer_enb_ctx  -  Pointer to peer eNB context
 *                : p_asn_msg   -   ASN message to be sent
 *                : asn_msg_len  -  ASN message length
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to send the rlf indication
 *                  mesaage to peer enb.
****************************************************************************/
x2ap_return_et x2ap_send_rlf_indication(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        U8* p_asn_msg,
        U16 asn_msg_len)
{
    x2ap_peer_enb_msg_t  peer_enb_msg;
    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    peer_enb_msg.p_msg = p_asn_msg;
    peer_enb_msg.msg_len = asn_msg_len;
    /* SPR_14436_Fix Start */
    peer_enb_msg.stream_id = p_peer_enb_ctx->stream_info.
	stream_ids[SCTP_NON_UE_ASSOCIATED_STREAM];
    /* SPR_14436_Fix Stop */

    if (INVALID_SCTP_SOCKET != p_peer_enb_ctx->sctp_sd)
    {
        if(!(x2ap_sctp_sm_entry(X2AP_SCTP_SEND_DATA_EV, 
                        (U8*)&peer_enb_msg, p_x2ap_gb_ctx, p_peer_enb_ctx)))
        {
            X2AP_TRACE(X2AP_INFO,"Sending X2AP_SCTP_SEND_DATA_EV failed\n");
            return X2AP_FAILURE;
        }
        else        
        {
            X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2AP RLF INDICATION MESSAGE");
        }

    }
    else
    {
        X2AP_TRACE(X2AP_INFO,"Invalid peer eNB SCTP socket descriptor\n");
        return X2AP_FAILURE;
    }

    return X2AP_SUCCESS;
}

/*CA_PHASE3_INTER_CELL_MRO_START*/
/****************************************************************************
 * Function Name  : x2ap_build_and_send_internal_rlf_indication
 *                : p_x2ap_gl_ctx -  pointer to X2AP global context
 *                : p_reestab_conn_setup_reconfig    -  Pointer to
 *                  x2ap_ue_reestab_conn_setup_reconfig_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the rlf indication
 *                  that is pushed internally.
****************************************************************************/
x2ap_return_et x2ap_build_and_send_internal_rlf_indication(
        x2ap_gb_context_t* p_x2ap_gb_ctx, /*X2AP Global Context*/
        x2ap_ue_reestab_conn_setup_reconfig_t *p_reestab_conn_setup_reconfig
)
{
    x2ap_return_et       retVal = X2AP_SUCCESS;
    U8 msg_ind_buf[sizeof(x2ap_rlf_ind_info_t)+X2AP_MAX_ASN1_BUF_LEN];
 /* SPR 21050 Fix Start */
  /* Code Deleted */
 /* SPR 21050 Fix End */

    void           *p_intrl_msg = PNULL;
    x2ap_rlf_ind_info_t *msg_p       = PNULL;

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_reestab_conn_setup_reconfig != X2AP_P_NULL);

    X2AP_UT_TRACE_ENTER();

    memset_wrapper(msg_ind_buf, 0, sizeof(x2ap_rlf_ind_info_t)+X2AP_MAX_ASN1_BUF_LEN);

    /* Send indication to UECC */
    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_UECC_MODULE_ID,
            X2AP_UE_RLF_MSG_IND,
            (U16)(sizeof(x2ap_rlf_ind_info_t) 
       /* SPR 21050 Fix Start */ 
                + p_reestab_conn_setup_reconfig->rlf_report.message_buff_size)); 
       /* SPR 21050 Fix End */ 
    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    msg_p = (x2ap_rlf_ind_info_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);
    X2AP_MEMCPY(&msg_p->enb1_gb_id,
            &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
    X2AP_MEMCPY(&msg_p->enb2_gb_id,
            &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
    msg_p->failure_cell_pci = p_reestab_conn_setup_reconfig->pci;
    msg_p->re_est_cell_ecgi =  p_x2ap_gb_ctx->served_cell_info[
        p_reestab_conn_setup_reconfig->cell_index].ecgi;
    msg_p->crnti = p_reestab_conn_setup_reconfig->crnti;
    msg_p->bitmask |=
        X2AP_SHORT_MAC_I_PRESENT;
    msg_p->short_mac_i = p_reestab_conn_setup_reconfig->short_mac_i;
    if (p_reestab_conn_setup_reconfig->bitmask & 
            X2AP_RLF_REPORT_CONTAINER_PRESENT)
    {
        msg_p->bitmask |=
            X2AP_RLF_REPORT_PRESENT;
        msg_p->rlf_report.message_buff_size =
            p_reestab_conn_setup_reconfig->rlf_report.message_buff_size;
        X2AP_MEMCPY(msg_p->rlf_report.message_buff_p,
                p_reestab_conn_setup_reconfig->rlf_report.message_buff_p,
                p_reestab_conn_setup_reconfig->rlf_report.message_buff_size);
    }
    /*need to check this should be present or not as conn_setup_indication is
     * r10 parameter*/
/*    if (p_reestab_conn_setup_reconfig->bitmask & X2AP_RLF_CONN_SETUP_INDICATOR)
    {
        msg_p->bitmask |=
            X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT;
        msg_p->conn_setup_indicator = p_reestab_conn_setup_reconfig->conn_setup_indicator;
    }
*/
    /*Flag set true to Identify this RLF indication is pushed internally*/
    msg_p->too_early_inter_cell_ho_flg = RRC_TRUE;

    rrc_send_message(p_intrl_msg,
            RRC_UECCMD_MODULE_ID);
    X2AP_TRACE(X2AP_INFO, "[X2AP_UE_RLF_MSG_IND] send to UECC module.");

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/*CA_PHASE3_INTER_CELL_MRO_STOP*/

