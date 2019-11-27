/*******************************************************************************
 *
 * FILE NAME   : x2ap_ho_report.c
 *
 * DESCRIPTION : This file contains the X2AP HANDOVER REPORT state machine
 *               and the state machine Entry function definition.
 *
 * DATE :   29-Oct-2012  
 *
 * NAME :   Rajat Aggarwal
 *                      
 * REASON : Initial Creation
 *
 * Copyright 2010, Aricent Inc.
 * ******************************************************************************/

#include "rrc_msg_mgmt.h"
#include "x2ap_tracing.h"
#include "rrc_x2apCommon_intf.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_x2apCommon_il_parser.h"
#include "rrc_x2ap_uecc_intf.h"
#include "x2ap_uecc.h"
#include "x2ap_utils.h"
#include "asn_enc_wrappers.h"
#include "asn_dec_wrappers.h"
#include "sm_entry.h"


x2ap_return_et x2ap_process_handover_report(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_enb_build_and_send_handover_report(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        x2ap_ho_report_info_t  *p_x2ap_ho_report);


/****************************************************************************
 * Function Name  : x2ap_process_handover_report
 * Inputs         : p_msg_ptr   -   Pointer to message buffer
 *                : p_x2ap_gl_ctx - pointer to X2AP global context
 *                : p_enb_ctx    -  Pointer to eNB context
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to process the handover report
 *                  mesaage received from peer enb.
 ***************************************************************************/
x2ap_return_et x2ap_process_handover_report(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx)
{
    x2ap_return_et        retVal = X2AP_SUCCESS;
    x2ap_ho_report_ind_t  x2_ho_report_ind;

    x2ap_peer_enb_msg_t* p_peer_enb_msg =
                            (x2ap_peer_enb_msg_t*)p_msg_ptr ;
    /* SPR 23876/SES-470 Fix Start */
    U8 num_cells = X2AP_NULL;
    U8 cell_index = X2AP_NULL;
    /* SPR 23876/SES-470 Fix End */

    X2AP_UT_TRACE_ENTER();

    /* Received pointer should not be NULL */
    X2AP_ASSERT(p_msg_ptr != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_enb_ctx != X2AP_P_NULL);

    X2AP_MEMSET(&x2_ho_report_ind, 0, sizeof(x2ap_ho_report_ind_t));

    if(!(p_x2ap_gb_ctx->x2ap_link_info[p_enb_ctx->peer_enodeb_id].
                connection_status))
    {
        X2AP_TRACE(X2AP_ERROR,"Peer eNB connection is down.\n");
        return X2AP_FAILURE;
    }

    retVal = x2ap_decode_handover_report(p_x2ap_gb_ctx,
                                    &x2_ho_report_ind,
                                    (p_peer_enb_msg)->p_msg,
                                    &(p_peer_enb_msg->msg_len),
                                    p_enb_ctx);

    if (X2AP_SUCCESS == retVal)
    {
        U16 dst_module_id = RRC_RRM_MODULE_ID;
        if(p_x2ap_gb_ctx->is_son_present)
        {
            dst_module_id = RRC_SON_MODULE_ID;
        }

        X2AP_MEMCPY(&x2_ho_report_ind.enb1_gb_id,
                &p_enb_ctx->gb_enb_id, sizeof(x2_gb_enb_id_t));
        X2AP_MEMCPY(&x2_ho_report_ind.enb2_gb_id,
                &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));
        /* SPR 23876/SES-470 Fix Start */
        for(num_cells = X2AP_NULL; num_cells < p_x2ap_gb_ctx->num_served_cells; num_cells++)
        {
            if(!X2AP_MEMCMP(&x2_ho_report_ind.src_ecgi, &p_x2ap_gb_ctx->served_cell_info[num_cells].ecgi, sizeof(x2ap_ecgi_t)))
            {
                if(X2AP_TRUE == p_x2ap_gb_ctx->act_cell_info[num_cells].is_cell_active)
                {
                    cell_index = p_x2ap_gb_ctx->act_cell_info[num_cells].cell_id;
                }
            }
        }
        /* SPR 23876/SES-470 Fix End */

        if( RRC_FAILURE ==  rrc_x2apCommon_send_x2ap_ho_report_ind(
                                        &x2_ho_report_ind,
                                        RRC_X2AP_MODULE_ID,
                                        dst_module_id,
                                        0,
                                        /* SPR 23876/SES-470 Fix Start */
                                        cell_index))
            /* SPR 23876/SES-470 Fix End */
        {
            X2AP_TRACE(X2AP_ERROR,"Failed to send HO_REPORT_IND.\n");
        }
        /* SPR 23876/SES-470 Fix Start */
        else
        {
            X2AP_TRACE(X2AP_INFO,"HO_REPORT_IND sent successfully for cell index %d\n", cell_index);
        }
        /* SPR 23876/SES-470 Fix End */
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;

}

/****************************************************************************
 * Function Name  : x2ap_enb_build_and_send_handover_report
 * Inputs         : p_x2ap_gb_ctx - pointer to  x2ap_gb_context_t
 *                  p_peer_enb_ctx - pointer to x2ap_peer_enb_context_t
 *                  p_x2ap_ho_report - pointer to x2ap_ho_report_info_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS or X2AP_FAILURE
 * Description    : This function is called to build and send
 *                  x2ap_HandoverReport mesaage.
 ***************************************************************************/
x2ap_return_et x2ap_enb_build_and_send_handover_report(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t *p_peer_enb_ctx,
        x2ap_ho_report_info_t  *p_x2ap_ho_report)
{
    x2ap_return_et result = X2AP_FAILURE;
    U8 asn_msg_buff[X2AP_MAX_ASN1_BUF_LEN] = {0};
    U16 asn_msg_len = X2AP_NULL;
    x2ap_peer_enb_msg_t  peer_enb_msg;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(p_x2ap_gb_ctx  != X2AP_P_NULL);
    X2AP_ASSERT(p_peer_enb_ctx != X2AP_P_NULL);
    X2AP_ASSERT(p_x2ap_ho_report != X2AP_P_NULL);

    X2AP_MEMSET(&peer_enb_msg,X2AP_NULL,sizeof(x2ap_peer_enb_msg_t));

    /* Encoding x2ap_HandoverReport */
    if ( !(x2ap_encode_handover_report( 
                                       p_x2ap_ho_report, 
                                       asn_msg_buff, 
                                       &asn_msg_len)))
    {
        X2AP_TRACE(X2AP_ERROR,"Encoding HandoverReport failed.");
        return X2AP_FAILURE;
    }

    if (p_x2ap_gb_ctx->x2ap_link_info[p_peer_enb_ctx->peer_enodeb_id].
            connection_status)
    {
        peer_enb_msg.p_msg = asn_msg_buff;
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
                result =  X2AP_FAILURE;
            }
            else
            {
                X2AP_TRACE(X2AP_INFO,"X2AP_MSG: X2AP HANDOVER REPORT MESSAGE");
                result = X2AP_SUCCESS;
            }
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,"Invalid peer eNB SCTP socket descriptor\n");
            result = X2AP_FAILURE;
        }
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"Peer eNB link down");
        result = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return result;
}
