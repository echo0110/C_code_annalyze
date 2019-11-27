/***************************************************************************
 *  FILE NAME  : s_tlif.h
 *
 *  DESCRIPTION: Contains the Functions exported by tl module.
 *
 *  DATE       NAME         REF#         REASON
 *  ------------------------------------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef    __FILE_sctp_tl_if_SEEN__
#define    __FILE_sctp_tl_if_SEEN__


#ifdef __cplusplus
extern "C" {
#endif

    sctp_return_t sctp_TL_open_socket(
        sctp_sa_family_t      af,
        sctp_U32bit           type,
        sctp_U32bit           protocol_id,
        sctp_sockaddr_st      *p_addr,
        sctp_U16bit           port,
        sctp_Boolean_t        is_read_fd,
        sctp_U32bit           *p_socket_id );

    sctp_return_t
    sctp_TL_socket_close(
        sctp_U32bit          socket_id );

    sctp_return_t
    sctp_TL_send_data(
        sctp_U32bit          socket_id,
        sctp_U8bit           *buffer,
        sctp_U32bit          buffer_len,
        sctp_S32bit          send_flags,
        sctp_sockaddr_st     *p_addr,
        sctp_U16bit          dest_port );

    sctp_return_t
    sctp_TL_send_iovec(
        sctp_U32bit         socket_id,
        sctp_iovec_st       *p_iovec,
        sctp_U32bit         iovec_len,
        sctp_S32bit         send_flags,
        sctp_sockaddr_st    *p_addr,
        sctp_U16bit         dest_port );

    sctp_return_t
    sctp_TL_receive_data(
        sctp_U32bit          socket_id,
        sctp_U8bit           *buffer,
        sctp_U32bit          buffer_len,
        sctp_S32bit          recv_flags,
        sctp_Pvoid_t         from_addr,
        sctp_U32bit          *from_len,
        sctp_U32bit          *read_data_size );

    void
    sctp_process_TL_data_ind(
        fd_set               *F );

    sctp_void_t sctp_process_TL_data_ind_inet(
        sctp_TL_packet_st   *p_sctp_packet,
        sctp_U32bit         buffer_size,
        sctp_sockaddr_st    *p_from_addr,
        sctp_U16bit         from_port_num,
        sctp_sockaddr_st    *p_on_addr,
        sctp_U16bit         on_port_num );

    sctp_void_t sctp_process_TL_data_ind_inet6(
        sctp_TL_packet_st   *p_sctp_packet,
        sctp_U32bit         buffer_size,
        sctp_sockaddr_st    *p_from_addr,
        sctp_U16bit         from_port_num,
        sctp_sockaddr_st    *p_on_addr,
        sctp_U16bit         on_port_num );

    sctp_void_t sctp_process_TL_data_ind_udp(
        sctp_TL_packet_st   *p_sctp_packet,
        sctp_U32bit         buffer_size,
        sctp_sockaddr_st    *p_from_addr,
        sctp_U16bit         from_port_num,
        sctp_sockaddr_st    *p_on_addr,
        sctp_U16bit         on_port_num );

    sctp_void_t sctp_process_TL_data_ind_icmp4(
        sctp_TL_packet_st   *p_sctp_packet,
        sctp_U32bit         buffer_size,
        sctp_sockaddr_st    *p_from_addr,
        sctp_U16bit         from_port_num,
        sctp_sockaddr_st    *p_on_addr,
        sctp_U16bit         on_port_num );

    sctp_void_t sctp_process_TL_data_ind_icmp6(
        sctp_TL_packet_st   *p_sctp_packet,
        sctp_U32bit         buffer_size,
        sctp_sockaddr_st    *p_from_addr,
        sctp_U16bit         from_port_num,
        sctp_sockaddr_st    *p_on_addr,
        sctp_U16bit         on_port_num );

    sctp_return_t sctp_TL_resolve_hostname(
        sctp_U8bit            *hostname,
        sctp_U32bit           *num_of_addrs,
        sctp_sockaddr_list_st addr_list );

    sigtran_return_t    sctp_TL_recv_data(
        sigtran_U32bit      handle_id,
        sig_sock_mode_et    socket_mode,
        sig_soc_type_et     socket_type,
        sig_sock_fd_t       socket_id,
        sig_q_module_t      *p_from_q_module,
        sigtran_pvoid_t     *p_data,
        sigtran_U32bit      *p_data_len,
        sigtran_error_t     *p_ecode );


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_tl_if_SEEN__ */


