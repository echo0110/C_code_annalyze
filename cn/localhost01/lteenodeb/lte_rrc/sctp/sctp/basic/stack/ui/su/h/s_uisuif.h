/****************************************************************************
 *    FILE NAME:
 *    s_uisuif.h
 *
 *    DESCRIPTION:
 *    This file is part of Service User Interface module. It includes
 *    functions that are called by Service user to use services of
 *    SCTP stack.
 *
 *    DATE  NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -       .Original Creation
 *    30Aug  2001   Yogesh Gahlaut           Rel 3.0 Changes
 *    25mar  2011   Akanksha         SPR 20859    Enhancement for Callback
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef    __FILE_sctp_su_if_SEEN__
#define    __FILE_sctp_su_if_SEEN__

#include <s_uismty.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SCTP_UI_SOCK_API

    sctp_return_t
    sctp_api_register_su(
        sctp_U16bit       *application_id,
        sctp_Pvoid_t      callback_func,
        sctp_Pvoid_t      p_appl_data,
        sctp_error_t      *p_ecode );

    sctp_return_t
    sctp_api_deregister_su(
        sctp_U16bit       application_id,
        sctp_error_t      *p_ecode );

    sctp_return_t
    sctp_api_socket(
        sctp_U16bit       application_id,
        sctp_U32bit       addr_family,
        sctp_assoc_t      *association_id,
        sctp_error_t      *errorno );

    /* Callback for Sending Function SPR 20859 Starts*/
#if !defined (SCTP_STRUCT_BASED) && !defined (SCTP_UI_SU_MESSAGEBASED) && !defined (SCTP_UI_SOCK_API)
    sctp_return_t
    sctp_api_socket_ext(
        sctp_U16bit       application_id,
        sctp_U32bit       addr_family,
        sctp_assoc_t      *association_id,
        sctp_Pvoid_t      callback_send_func,
        sctp_error_t      *errorno );
#endif
    /* Callback for Sending Function SPR 20859 Stops*/

    sctp_return_t
    sctp_api_close(
        sctp_assoc_t      association_id,
        sctp_Boolean_t    graceful,
        sctp_error_t      *errorno );

    sctp_return_t sctp_api_close_ext(
        sctp_assoc_t association_id,
        sctp_U16bit  cause_len,
        sctp_U8bit   *cause,
        sctp_error_t *errorno );

    sctp_return_t
    sctp_api_bind(
        sctp_assoc_t      association_id,
        sctp_U16bit       sctp_port,
        sctp_U32bit       no_of_su_addrs,
        sctp_sockaddr_list_st list_of_addrs,
        sctp_U8bit        *hostname,
        sctp_error_t      *errorno );

    sctp_return_t
    sctp_api_bindx(
        sctp_U32bit            assoc_id,
        sctp_U32bit            n_addr,
        sctp_sockaddr_list_st  sctp_addr_list,
        sctp_bindx_flag_et     flags,
        sctp_Boolean_t         block,
        sctp_error_t           *p_ecode );

    sctp_return_t
    sctp_api_listen(
        sctp_U32bit        association_id,
        sctp_U32bit        listen_back_log,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_config_assoc(
        sctp_assoc_t      association_id,
        sctp_prot_params_st *prot_params,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_config_assoc_ctl(
        sctp_assoc_t      association_id,
        sctp_prot_params_et  prot_parameter,
        sctp_U32bit        parameter_value,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_config_cong_reg_def(
        sctp_U32bit          assoc_id,
        sctp_cong_reg_def_st cong_reg_def,
        sctp_Boolean_t       is_transmit,
        sctp_error_t         *p_ecode );
    sctp_return_t
    sctp_api_config_txrx_num_frag(
        sctp_U32bit          assoc_id,
        sctp_count_frags_st  count_frags,
        sctp_Boolean_t       is_transmit,
        sctp_Boolean_t       is_increment,
        sctp_error_t         *p_ecode );

    sctp_return_t
    sctp_api_get_free_txrx_num_frag(
        sctp_U32bit          assoc_id,
        sctp_count_frags_st  *tx_num_frags,
        sctp_count_frags_st  *rx_num_frags,
        sctp_error_t         *p_ecode );

    sctp_return_t
    sctp_api_get_cong_reg_def(
        sctp_U32bit          assoc_id,
        sctp_cong_reg_def_st *p_cong_reg_def,
        sctp_Boolean_t       is_transmit,
        sctp_error_t         *p_ecode );

    sctp_return_t
    sctp_api_accept_ext(
        sctp_assoc_t       association_id,
        sctp_U32bit        payload_prot_id,
        sctp_U16bit        *num_out_streams,
        sctp_U16bit        *num_in_streams,
        sctp_U32bit        *nos_of_dest_trans_addrs,
        sctp_sockaddr_list_st list_of_dest_trans_address,
        sctp_U16bit        *sctp_port,
        sctp_assoc_t       *new_association_id,
        sctp_Boolean_t     block,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_accept(
        sctp_assoc_t       association_id,
        sctp_U32bit        payload_prot_id,
        sctp_U32bit        *nos_of_dest_trans_addrs,
        sctp_sockaddr_list_st list_of_dest_trans_addrs,
        sctp_U16bit        *sctp_port,
        sctp_assoc_t       *new_association_id,
        sctp_Boolean_t     block,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_connect_ext(
        sctp_assoc_t       association_id,
        sctp_U16bit        dest_sctp_port,
        sctp_U16bit        *num_out_streams,
        sctp_U16bit        *num_in_streams,
        sctp_U32bit        *nos_of_dest_trans_addrs,
        sctp_sockaddr_list_st list_of_dest_trans_addrs,
        sctp_sockaddr_st    *connect_dest_addr,
        sctp_U32bit        payload_prot_id,
        sctp_Boolean_t     block,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_connectx_ext(
        sctp_U32bit             association_id,
        sctp_U16bit             dest_sctp_port,
        sctp_U32bit             payload_prot_id,
        sctp_Boolean_t          block,
        sctp_U32bit             num_dest_addrs,
        sctp_sockaddr_st        *p_connect_daddrs,
        sctp_U16bit             *p_num_ostreams,
        sctp_U16bit             *p_num_istreams,
        sctp_U32bit             *p_num_daddrs,
        sctp_sockaddr_list_st   list_of_dest_addrs,
        sctp_error_t            *p_ecode );

    sctp_return_t
    sctp_api_connect(
        sctp_assoc_t       association_id,
        sctp_U16bit        dest_sctp_port,
        sctp_U32bit        *nos_of_dest_trans_addrs,
        sctp_sockaddr_list_st list_of_dest_trans_addrs,
        sctp_sockaddr_st   *connect_dest_addr,
        sctp_U32bit        payload_prot_id,
        sctp_Boolean_t     block,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_send_ext(
        sctp_assoc_t       association_id,
        sctp_U32bit        context,
        sctp_sockaddr_st   *dest_addr,
        sctp_U32bit        data_size,
        sctp_Pvoid_t       data_payload,
        sctp_U16bit        stream_id,
        sctp_U32bit        life_time,
        sctp_Boolean_t     order_flag,
        sctp_Boolean_t     bundle_flag,
        sctp_U32bit        payload_prot_id,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_send_ext2(
        sctp_U32bit        assoc_id,
        sctp_U32bit        context,
        sctp_sockaddr_st   *p_daddr,
        sctp_U32bit        data_size,
        sctp_Pvoid_t       p_data,
        sctp_stream_t      stream_id,
        sctp_U32bit        life_time,
        sctp_Boolean_t     order_flag,
        sctp_Boolean_t     bundle_flag,
        sctp_U32bit        payload_prot_id,
        sctpbuffer_st      **p_p_tx_buf,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_api_send(
        sctp_assoc_t       association_id,
        sctp_U32bit        context,
        sctp_U16bit        stream_id,
        sctp_sockaddr_st   *dest_addr,
        sctp_U32bit        data_size,
        sctp_Pvoid_t       data_payload,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_send_iovec(
        sctp_U32bit         assoc_id,
        sctp_U32bit         context,
        sctp_sockaddr_st    *p_daddr,
        sctp_U32bit         iovec_len,
        sctp_iovec_st       *p_iovec,
        sctp_stream_t       stream_id,
        sctp_U32bit         life_time,
        sctp_Boolean_t      order_flag,
        sctp_Boolean_t      bundle_flag,
        sctp_U32bit         payload_prot_id,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_api_send_ext2(
        sctp_U32bit        assoc_id,
        sctp_U32bit        context,
        sctp_sockaddr_st   *p_daddr,
        sctp_U32bit        iovec_len,
        sctp_Pvoid_t       p_iovec,
        sctp_stream_t      stream_id,
        sctp_U32bit        life_time,
        sctp_Boolean_t     order_flag,
        sctp_Boolean_t     bundle_flag,
        sctp_U32bit        payload_prot_id,
        sctpbuffer_st      **p_p_tx_buf,
        sctp_error_t       *p_ecode );


    sctp_return_t
    sctp_api_receive(
        sctp_assoc_t       association_id,
        sctp_U32bit        *payload_prot_id,
        sctp_U32bit        *data_size,
        sctp_U16bit        *stream_id,
        sctp_U8bit         *data,
        sctp_Boolean_t     block,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_receive_ext(
        sctp_assoc_t       association_id,
        sctp_U16bit        stream_id,
        sctp_sockaddr_st   *dest_address,
        sctp_U32bit        *payload_prot_id,
        sctp_U32bit        *data_size,
        sctp_U8bit         *data,
        sctp_Boolean_t     block,
        sctp_Boolean_t     peek_flag,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_receive_ext2(
        sctp_U32bit        assoc_id,
        sctp_U16bit        stream_id,
        sctp_sockaddr_st   *p_dest_address,
        sctp_U32bit        *p_ppi,
        sctp_U32bit        *p_data_size,
        sctp_U8bit         *p_data,
        sctp_Boolean_t     block,
        sctp_Boolean_t     peek_flag,
        sctpbuffer_st      **p_p_buffer,    /*output buffer, */
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_api_receive_partial_data(
        sctp_assoc_t       association_id,
        sctp_U16bit        stream_id,
        sctp_U32bit        *payload_prot_id,
        sctp_U32bit        *data_size,
        sctp_U8bit         *data,
        sctp_U16bit        *ssn,
        sctp_U16bit        *delivery_num,
        sctp_Boolean_t     peek_flag,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_heartbeat_action(
        sctp_assoc_t       association_id,
        sctp_sockaddr_st   *destination_addr,
        sctp_HB_action_et  heartbeat_action,
        sctp_U32bit        heartbeat_interval,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_set_failure_threshold(
        sctp_assoc_t       association_id,
        sctp_sockaddr_st   *destination_addr,
        sctp_U32bit        path_max_retrans,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_set_primary(
        sctp_assoc_t       association_id,
        sctp_sockaddr_st   *src_addr,
        sctp_sockaddr_st   *destination_addr,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_get_srtt(
        sctp_assoc_t       association_id,
        sctp_sockaddr_st   *destination_addr,
        sctp_U32bit        *srtt_value,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_get_status(
        sctp_assoc_t       association_id,
        sctp_status_params_st   *p_status_params,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_get_dest_addr_status(
        sctp_assoc_t       association_id,
        sctp_U32bit        *nos_of_dest_trans_addrs,
        sctp_sockaddr_list_st list_of_dest_trans_addrs,
        sctp_dest_status_param_list_st   *dest_param_list,
        sctp_sockaddr_st   *primary_dest_transport_addr,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_flush_buffers(
        sctp_assoc_t       assoc_id,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_retrieve_unsent(
        sctp_assoc_t       association_id,
        sctp_U32bit        *context,
        sctp_U8bit         *data,
        sctp_U32bit        *data_len,
        sctp_U16bit        *stream_id,
        sctp_U32bit        *payload_prot_id,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_retrieve_unacked(
        sctp_assoc_t       association_id,
        sctp_U32bit        *context,
        sctp_U8bit         *data,
        sctp_U32bit        *data_len,
        sctp_Boolean_t     *incomplete_flag,
        sctp_U16bit        *stream_id,
        sctp_U16bit        *ssn,
        sctp_U32bit        *payload_prot_id,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_set_restart_option(
        sctp_assoc_t       association_id,
        sctp_restart_options_et     restart_option,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_get_stream_status(
        sctp_assoc_t       association_id,
        sctp_U16bit        stream_id ,
        sctp_U16bit        *ssn,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_abort_retrieve(
        sctp_assoc_t       assoc_id,
        sctp_Boolean_t     *data_pending,
        sctp_error_t       *errorno );

    sctp_return_t
    sctp_api_requeue_pending_datagram(
        sctp_assoc_t       association_id,
        sctp_error_t       *errorno );

    void
    sctp_generic_su_notification(
        sctp_ep_st         *p_ep,
        sctp_U32bit        event );

    sctp_return_t
    sctp_parse_api_config_socket(
        sctp_ep_st           *p_ep,
        sctp_prot_params_st  *prot_params,
        sctp_error_t         *p_ecode );

    void
    sctp_process_api_config_socket(
        sctp_ep_st            *p_ep,
        sctp_prot_params_st   *prot_params );

    sctp_return_t
    sctp_process_api_config_assoc_ctl(
        sctp_prot_params_st   *prot_params,
        sctp_prot_params_et   prot_parameter,
        sctp_U32bit           parameter_value,
        sctp_Boolean_t        assoc_state_flag,
        sctp_error_t          *errorno );

    sctp_return_t
    sctp_api_op_error_peer(
        sctp_U32bit     association_id,
        sctp_U16bit     error_code,
        sctp_U16bit     error_length,
        sctp_Pvoid_t    sys_error_info );

    sctp_return_t
    sctp_api_destination_status_chg(
        sctp_U32bit           association_id,
        sctp_sockaddr_st      *destination_ip_addr,
        sctp_sock_spc_event_et   dest_act_status );

    sctp_return_t
    sctp_api_endpoint_status_chg(
        sctp_U32bit                association_id,
        sctp_sock_sac_event_et     status,
        sctp_endpoint_chg_data_ut  *staus_change_data );

    sctp_return_t
    sctp_api_congestion(
        sctp_U32bit         association_id,
        sctp_cong_level_et  cong_level );

    sctp_return_t
    sctp_api_recv_congestion(
        sctp_U32bit         association_id,
        sctp_cong_level_et  cong_level );

    sctp_return_t
    sctp_api_sys_error(
        sctp_U32bit     association_id,
        sctp_error_t    errorno );

    sctp_return_t
    sctp_api_connection_pending(
        sctp_U32bit        association_id );

    sctp_return_t
    sctp_api_data_pending(
        sctp_U32bit    association_id,
        sctp_U16bit    stream_id );

    sctp_return_t
    sctp_api_send_failure(
        sctp_U32bit     association_id,
        sctp_U32bit     context,
        sctp_error_t    cause_code );

    sctp_return_t
    sctp_api_partial_data_pending(
        sctp_U32bit    association_id,
        sctp_U16bit    stream_id );

    sctp_return_t
    sctp_api_free_transmit_buffer(
        sctp_U32bit          assoc_id,
        sctpbuffer_st        *p_buf );

    sctp_return_t
    sctp_api_free_receive_buffer(
        sctp_U32bit          assoc_id,
        sctpbuffer_st        *p_buf );

    sctp_return_t
    sctp_api_adaption_ind(
        sctp_U32bit    assoc_id,
        sctp_U32bit    adaption_bits );

    sctp_return_t
    sctp_api_peer_data_drop(
        sctp_U32bit    assoc_id,
        sctp_error_t   ecode,
        sctp_U8bit     *p_buffer,
        sctp_U32bit    buffer_len );

    sctp_return_t
    sctp_api_shutdown_event(
        sctp_U32bit        association_id );

    sctp_return_t
    sctp_api_recv_mesg(
        sctp_assoc_t                assoc_id,
        sctp_U32bit                 *p_ppi,
        sctp_api_recv_struct_st     *p_recv_struct,
        sctp_U8bit                  *p_data,
        sctp_U32bit                 *p_data_size,
        sctp_error_t                *p_ecode );

#endif /* SCTP_UI_SOCK_API */

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_su_if_SEEN__ */

