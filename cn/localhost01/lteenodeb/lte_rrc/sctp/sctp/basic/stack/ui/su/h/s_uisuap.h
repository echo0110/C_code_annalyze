/***************************************************************************
 *  FILE NAME  : s_uisuap.h
 *
 *  DESCRIPTION: Contains the functions for processing SU apis.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef _S_UI_SU_AP_H_SEEN
#define _S_UI_SU_AP_H_SEEN

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCTP_UI_SOCK_API

    extern sctp_return_t
    sctp_ui_send_notifications(
        sctp_U16bit             su_id,
        sctp_U32bit             event,
        sctp_notice_ut          *notf_data );

    extern sctp_void_t
    sctp_process_su_api_prim_msg(
        sctp_U8bit            *p_buffer,
        sctp_len_t            nooctets,
        sctp_client_info_st   *from_client );


#ifdef SCTP_UI_SU_MESSAGEBASED

    sctp_U8bit *
    sctp_proc_api_register_su(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_deregister_su(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_socket(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_bind(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_bindx(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_listen(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_config_assoc(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_config_assoc_ctl(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_set_fthresh(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_connect(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_connect_ext(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_accept(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_accept_ext(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_send(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_send_iovec(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_send_ext(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_receive(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_receive_ext(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_receive_part(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_close(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_close_ext(
        sctp_U8bit *p_req_buffer,
        sctp_U8bit *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_hb_action(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_set_primary(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_get_srtt(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_get_status(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_get_dest_status(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_set_restart_option(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_ret_unsent(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_ret_unacked(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_flush_buffers(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_get_stream_status(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_abort_retrieve(
        sctp_U8bit       *p_req_buffer,
        sctp_U8bit       *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_config_cong_reg_def(
        sctp_U8bit *p_req_buffer,
        sctp_U8bit *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_get_cong_reg_def(
        sctp_U8bit *p_req_buffer,
        sctp_U8bit *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_config_txrx_num_frag(
        sctp_U8bit *p_req_buffer,
        sctp_U8bit *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_get_free_txrx_num_frag(
        sctp_U8bit *p_req_buffer,
        sctp_U8bit *p_resp_buffer );

    sctp_U8bit *
    sctp_proc_api_connectx(
        sctp_U8bit *p_req_buffer,
        sctp_U8bit *p_resp_buffer );


    /* notification sending functions */
    sctp_U8bit *
    sctp_ui_build_op_error_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_dest_status_chg_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_endpoint_status_chg_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_congestion_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_sys_error_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_conn_pending_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_data_pending_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_snd_failure_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_partial_datapending_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_recv_congestion_notf(
        sctp_U8bit       *p_notf_buffer,
        sctp_notice_ut   *notf_data );

    sctp_U8bit *
    sctp_ui_build_api_header(
        sctp_U8bit       *p_buffer,
        sctp_api_id_t    api_id,
        sctp_trans_id_t  transaction_id,
        sctp_src_id_t    src_id,
        sctp_dest_id_t   dest_id );

    sctp_U8bit *
    sctp_ui_build_adaption_ind(
        sctp_U8bit     *p_ntf_buffer,
        sctp_notice_ut *ntf_data );

    sctp_U8bit *
    sctp_ui_build_peer_data_drop_ind(
        sctp_U8bit     *p_ntf_buffer,
        sctp_notice_ut *ntf_data );

    sctp_U8bit *
    sctp_ui_build_shutdown_event_notf(
        sctp_U8bit     *p_ntf_buffer,
        sctp_notice_ut *ntf_data );


#endif /* SCTP_UI_SU_MESSAGEBASED */

#endif /* SCTP_UI_SOCK_API */

#ifdef __cplusplus
}
#endif

#endif /* _S_UI_SU_AP__H_SEEN */
