/***************************************************************************
 *  FILE NAME  : s_es.h
 *
 *  DESCRIPTION: Contains the Encapsulation shell functions.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  02jan02    hbhondwe     SCTP_IPC_TCP ipc changes for tcp
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *  27Now03    Vivek Bansal              CSPL Changes
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_eshell_SEEN__
#define    __FILE_sctp_eshell_SEEN__


#ifdef __cplusplus
extern "C" {
#endif

#include  <s_esdef.h>


#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_RED_SUPPORT
    extern sctp_U32bit g_sctp_rm_dest_id;
#endif
#endif

#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_CLI_RECV_COM_THREAD

    void sctp_eshell_start_proc( void );

#endif /* SCTP_CLI_RECV_COM_THREAD */
#endif /* SCTP_RED_SUPPORT */

    extern sctp_client_info_st  sctp_client_list[SCTP_MAX_CLIENT_SUPPORTED];

    extern sctp_S32bit sctp_stack_ipctx_socket_id;
    extern sctp_S32bit sctp_stack_ipcrx_socket_id;


    sctp_return_t
    sctp_init_stack_ipc(
        sctp_U16bit        local_port,
        sctp_sockaddr_st   local_ip_address,
        sctp_port_type_et  port_type,
        sctp_error_t       *p_ecode );

    sigtran_return_t    sctp_ipc_recv_data(
        sigtran_U32bit      handle_id,
        sig_sock_mode_et    socket_mode,
        sig_soc_type_et     socket_type,
        sig_sock_fd_t       socket_id,
        sig_q_module_t      *p_from_q_module,
        sigtran_pvoid_t     *p_data,
        sigtran_U32bit      *p_data_len,
        sigtran_error_t     *p_ecode );

    sctp_void_t sctp_stack_proc_client_msg(
        sctp_U8bit         *p_message,
        sctp_U32bit         buffer_size );

#endif /* SCTP_UI_MESSAGEBASED */

    extern sctp_eshell_info_t sctp_eshell_info;

    extern sctp_sendmessage_fpt sctp_send_message[];

    sctp_return_t
    sctp_eshell_config_address(
        sctp_bindx_flag_et    flag,
        sctp_U32bit           num_local_addrs,
        sctp_sockaddr_st      *p_addr_list,
        sctp_error_t          *p_ecode );

    void
    sctp_send_msg_v4(
        sctp_U8bit           *buffer,
        sctp_U32bit          size,
        sctp_Boolean_t       df_flag,
        sctp_sockaddr_st     *to,
        sctp_sockaddr_st     *from );

    void
    sctp_send_msg_v6(
        sctp_U8bit           *buffer,
        sctp_U32bit          size,
        sctp_Boolean_t       df_flag,
        sctp_sockaddr_st     *to,
        sctp_sockaddr_st     *from );

    void
    sctp_send_pmtu_msg_v4(
        sctp_tcb_st          *p_ass,
        sctp_U32bit          size,
        sctp_sockaddr_st     *p_to );

    sctp_U16bit
    sctp_calc_icmpcksum(
        sctp_U16bit *buffer,
        sctp_U32bit len );

    void
    sctp_send_pmtu_msg_v6(
        sctp_tcb_st          *p_ass,
        sctp_U32bit          size,
        sctp_sockaddr_st     *p_to );

#ifdef SIGTRAN_LOCAL_TIME_THREAD
    sctp_return_t sctp_eshell_start_localtime_thread( sctp_error_t *p_ecode );
#endif

#ifdef SCTP_CLOCK_THREAD

    void *
    sctp_eshell_clock_thread(
        void           *A );

    sctp_return_t sctp_eshell_start_clock_thread( sctp_error_t *p_ecode );

#endif

    void *
    sctp_eshell_master_thread(
        void           *A );

    sctp_return_t
    sctp_es_validate_local_addresses(
        sctp_U32bit    num_local_addrs,
        sctp_sockaddr_list_st local_addr_list,
        sctp_error_t   *p_ecode );

    sctp_return_t
    sctp_init_ehsell(
        sctp_stack_type_et  stack_type,
        sctp_U16bit         standard_udp_port,
        sctp_U32bit         num_local_addrs,
        sctp_sockaddr_list_st local_addr_list,
        sctp_error_t        *p_ecode );

    sctp_return_t
    sctp_init_inet6(
        sctp_U32bit        num_local_addrs,
        sctp_sockaddr_list_st local_addr_list,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_init_inet(
        sctp_U32bit        num_local_addrs,
        sctp_sockaddr_list_st local_addr_list,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_init_udp(
        sctp_U32bit        num_local_addrs,
        sctp_sockaddr_list_st local_addr_list,
        sctp_port_t        standard_udp_port,
        sctp_error_t       *p_ecode );

    void *
    sctp_stack_ipc_master_thread(
        void               *A );

    void *sctp_receive_socket_data( void *p_fd_set );

    sctp_void_t sctp_process_events(
        sctp_U8bit         *p_buffer,
        sctp_U32bit         buffer_size );

#ifdef SCTP_INDICATION_THREAD

    void sctp_signal_indication();
#define SCTP_SIGNAL_INDICATION() sctp_signal_indication()
#else
#define SCTP_SIGNAL_INDICATION()
#endif

    sctp_return_t
    sctp_remove_notifications_from_queue( void );

#ifndef SCTP_INDICATION_THREAD
    sctp_void_t sctp_start_notification_queue_timer( void );
#endif

    sigtran_return_t sctp_get_api_buf_len(
        const sigtran_void_t    *p_msg,
        sigtran_U32bit          *p_msg_len );


#ifdef SCTP_IP_HEADER_PARAMS
    void sctp_get_ipv6_params_from_buffer( sctp_U8bit *buffer,
                                           sctp_sockaddr_st *p_to,
                                           sctp_sockaddr_st *p_from,
                                           sctp_U16bit      *hoplimit,
                                           sctp_U16bit      *tclass );
#endif


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_eshell_SEEN__ */

