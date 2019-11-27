/*****************************************************************************
**
**  FILE NAME:
**      sig_nw_util.h
**
******************************************************************************
**
**  DESCRIPTION:
**      This file contains the definitions and prototypes for
**               socket utilities
**
**  DATE        NAME                REFERENCE   REASON
**----------------------------------------------------------------------------
**  27OCt2003   Vishavdeep Sharma   HSS:XXXXXXXXX   Original
**
**    Copyright (C) 2006 Aricent Inc . All Rights Reserved
**
*****************************************************************************/

#ifndef __SIG_NW_UTIL_H__
#define __SIG_NW_UTIL_H__

#include <sig_typs.h>
#include <sig_defs.h>


/* Data types of socket manager */

/* this structure maintains per connection  information*/
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct
    {
        sig_sock_mode_et        sock_mode;
        sig_soc_type_et         sock_type;
        sig_sock_fd_t           fd;
        sig_ip_addr_st          lcl_ip_str;
        sig_ip_addr_st          dest_ip_str;
        sigtran_U32bit          lcl_ip;
        sigtran_U16bit          lcl_port;
        sigtran_U32bit          rem_ip;
        sigtran_U16bit          rem_port;
        sig_read_cbk_func_ptr_t p_rd_cbk_fn;
        sigtran_mutex_t         fd_mutex;
#ifdef SIG_COMMON_WITH_CSPL
        sig_q_module_t          q_module;
        sig_dest_type_et        dest_type;
#endif
    } soc_mgr_conn_info_st;


    typedef struct
    {
        sig_sock_fd_t           udp_server_sock;
        soc_mgr_conn_info_st    conn_info[SOC_MGR_MAX_CONNECTIONS];
        sig_get_sm_msg_len_cbk  p_sm_cbk_fn;
        sig_get_su_msg_len_cbk  p_su_cbk_fn;
        sig_get_tl_msg_len_cbk  p_tl_cbk_fn;

    } soc_mgr_st;


    /** PROTOTYPES **/

    /* This section contains prototype of interface functions which are
     * exposed to the application using socket manager
     */

    void soc_mgr_init
    ( soc_mgr_st               *p_soc_mgr,
      sig_get_sm_msg_len_cbk p_sm_cbk_fn,
      sig_get_su_msg_len_cbk p_su_cbk_fn,
      sig_get_tl_msg_len_cbk p_tl_cbk_fn );


    sigtran_pvoid_t soc_mgr_chk_activity(
        soc_mgr_st          *p_soc_mgr,
        sig_stack_type_et   stack_type,
        sig_q_module_t      *p_from_mod,
        sig_q_module_t      *p_to_mod );



    sigtran_U16bit soc_mgr_add_tcp_server(
        soc_mgr_st      *p_soc_mgr,
        sigtran_U32bit  serv_ip,
        sigtran_U16bit      serv_port,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sig_read_cbk_func_ptr_t  p_cbk_fn );

    sigtran_S16bit soc_mgr_add_tcp_client(
        soc_mgr_st          *p_soc_mgr,
        sigtran_U32bit  lcl_ip,
        sigtran_U16bit      lcl_port,
        sigtran_U32bit  serv_ip,
        sigtran_U16bit      serv_port,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sig_read_cbk_func_ptr_t  p_cbk_fn );

    sigtran_S32bit soc_mgr_read_api(
        soc_mgr_st         *p_soc_mgr,
        sigtran_U32bit     handle,
        sigtran_pvoid_t    *p_buf,
        sigtran_U32bit     nbytes );

    sigtran_U16bit soc_mgr_add_udp_server(
        soc_mgr_st         *p_soc_mgr,
        sigtran_U32bit     serv_ip,
        sigtran_U16bit     serv_port,
        sig_ip_addr_st     *p_src_addr,
        sig_ip_addr_st     *p_dest_addr,
        sigtran_boolean_t  read_flag,
        sigtran_boolean_t  b_def_udp_wrt_port,
        sig_read_cbk_func_ptr_t  p_cbk_fn );

    sigtran_U16bit soc_mgr_get_new_handle(
        soc_mgr_st       *p_soc_mgr );

    sigtran_void_t soc_mgr_release_handle(
        soc_mgr_st       *p_soc_mgr,
        sigtran_U16bit   handle );

    sigtran_U16bit soc_mgr_aceept_tcp_conn(
        soc_mgr_st         *p_soc_mgr,
        sig_sock_fd_t      sock_fd,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_U32bit    *p_ip_addr,
        sigtran_U16bit    *p_port );

    sig_sock_fd_t soc_mgr_setup_raw_ipv6_server(
        sigtran_U32bit    *p_serv_ip,
        sigtran_U16bit         serv_port,
        sigtran_U32bit         flow_info,
        sigtran_U32bit         scope_id );


    sig_sock_fd_t soc_mgr_setup_tcp_client(
        sigtran_U32bit        lcl_ip,
        sigtran_U16bit        lcl_port,
        sigtran_U32bit        serv_ip,
        sigtran_U16bit        serv_port );

    sig_sock_fd_t soc_mgr_setup_tcp_server(
        sigtran_U32bit        serv_ip,
        sigtran_U16bit        serv_port );

    sig_sock_fd_t soc_mgr_setup_raw_ipv4_server(
        sigtran_U32bit        serv_ip,
        sigtran_U16bit        serv_port );

    sig_sock_fd_t soc_mgr_setup_icmp_ipv4_server(
        sigtran_U32bit        serv_ip,
        sigtran_U16bit         serv_port );

    sig_sock_fd_t soc_mgr_setup_icmp_ipv6_server
    ( sigtran_U32bit         *p_serv_ip,
      sigtran_U16bit         serv_port,
      sigtran_U32bit         flow_info,
      sigtran_U32bit         scope_id );


    sig_sock_fd_t soc_mgr_setup_udp_server
    ( sigtran_U32bit         serv_ip,
      sigtran_U16bit         serv_port,
      sigtran_boolean_t      read_flag );

    sig_sock_fd_t soc_mgr_setup_udp_nw_server
    ( sigtran_U32bit         serv_ip,
      sigtran_U16bit         serv_port );

    sigtran_S32bit soc_mgr_nw_read
    ( sig_sock_fd_t   sockfd,
      sigtran_U32bit  len_to_read,
      sigtran_boolean_t read_flag,
      sigtran_U32bit  *ip_addr,
      sigtran_U16bit  *port,
      sigtran_pvoid_t ptr,
      sigtran_U32bit  *p_len_read );

    int soc_mgr_nw_read_in6( sig_sock_fd_t   sockfd,
                             unsigned int   len_to_read,
                             unsigned char  read_flag,
                             unsigned int   *ip_addr,
                             unsigned short *serv_port,
                             void           *pbuf,
                             unsigned int   *len_read,
                             unsigned int   *p_flow_info,
                             unsigned int   *p_scode_id );

    sigtran_S32bit soc_mgr_nw_write
    ( sig_sock_fd_t      sockfd,
      sigtran_pvoid_t    p_buf,
      sigtran_U32bit     nbytes,
      sigtran_boolean_t  b_is_iovec );

    sigtran_S32bit soc_mgr_send_on_udp
    ( sig_sock_fd_t      sockfd,
      const sigtran_void_t *p_buf,
      sigtran_U32bit     api_len,
      sigtran_U32bit     dest_ip,
      sigtran_U32bit     dest_port,
      sigtran_boolean_t  b_is_iovec );

    sigtran_S32bit soc_mgr_send_on_udp_ipv6
    ( sig_sock_fd_t      sockfd,
      sigtran_pvoid_t    p_buf,
      sigtran_U32bit     api_len,
      sigtran_U32bit     *p_dest_ip,
      sigtran_U32bit     dest_port,
      sigtran_boolean_t  b_is_iovec );

    int soc_mgr_send_on_udp_ipv6_ext
    ( sig_sock_fd_t         sockfd,
      void                *p_buf,
      unsigned int        buffer_len,
      unsigned int        send_flags,
      unsigned int        *p_dest_ip,
      unsigned int        dest_port,
      unsigned char       b_is_iovec );

    void sigtran_fill_cmsg_header
    ( struct cmsghdr *cmsg,
      sigtran_U32bit send_flags );

    sig_sock_fd_t soc_mgr_aceept(
        sig_sock_fd_t   sock,
        sigtran_U32bit  *p_ip,
        sigtran_U16bit  *p_port );

    sigtran_U16bit soc_mgr_add_udp_dest(
        soc_mgr_st       *p_soc_mgr,
        sigtran_U32bit   dest_ip,
        sigtran_U16bit   dest_port,
        sigtran_U16bit   handle_id );

    sigtran_U16bit soc_mgr_add_raw_ipv6_server(
        soc_mgr_st        *p_soc_mgr,
        sigtran_U32bit    *p_serv_ip,
        sigtran_U16bit    serv_port,
        sigtran_U32bit    flow_info,
        sigtran_U32bit    scope_id,
        sig_ip_addr_st    *p_src_addr,
        sig_ip_addr_st    *p_dest_addr,
        sigtran_boolean_t read_flag,
        sig_read_cbk_func_ptr_t     p_cbk_fn );

    sigtran_U16bit soc_mgr_add_icmp_ipv6_server(
        soc_mgr_st        *p_soc_mgr,
        sigtran_U32bit    *p_serv_ip,
        sigtran_U16bit    serv_port,
        sigtran_U32bit    flow_info,
        sigtran_U32bit    scope_id,
        sig_ip_addr_st    *p_src_addr,
        sig_ip_addr_st    *p_dest_addr,
        sigtran_boolean_t read_flag,
        sig_read_cbk_func_ptr_t     p_cbk_fn );

    sigtran_U16bit soc_mgr_add_udp_nw_server(
        soc_mgr_st        *p_soc_mgr,
        sigtran_U32bit    serv_ip,
        sigtran_U16bit    serv_port,
        sig_ip_addr_st    *p_src_addr,
        sig_ip_addr_st    *p_dest_addr,
        sigtran_boolean_t read_flag,
        sig_read_cbk_func_ptr_t  p_cbk_fn );

    sigtran_U16bit soc_mgr_add_raw_ipv4_server(
        soc_mgr_st        *p_soc_mgr,
        sigtran_U32bit    serv_ip,
        sigtran_U16bit    serv_port,
        sig_ip_addr_st    *p_src_addr,
        sig_ip_addr_st    *p_dest_addr,
        sigtran_boolean_t read_flag,
        sig_read_cbk_func_ptr_t     p_cbk_fn );

    sigtran_U16bit soc_mgr_add_icmp_ipv4_server(
        soc_mgr_st        *p_soc_mgr,
        sigtran_U32bit     serv_ip,
        sigtran_U16bit     serv_port,
        sig_ip_addr_st     *p_src_addr,
        sig_ip_addr_st     *p_dest_addr,
        sigtran_boolean_t  read_flag,
        sig_read_cbk_func_ptr_t     p_cbk_fn );

    sigtran_U16bit soc_mgr_add_icmp_ipv6_server(
        soc_mgr_st         *p_soc_mgr,
        sigtran_U32bit     *p_serv_ip,
        sigtran_U16bit     serv_port,
        sigtran_U32bit     flow_info,
        sigtran_U32bit     scope_id,
        sig_ip_addr_st     *p_src_addr,
        sig_ip_addr_st     *p_dest_addr,
        sigtran_boolean_t  read_flag,
        sig_read_cbk_func_ptr_t     p_cbk_fn );

    sigtran_S32bit sig_os_select(
        sigtran_S32bit     numfds,
        sig_fd_set         *p_readfds,
        sig_fd_set         *p_writefds,
        sig_fd_set         *p_exceptfds,
        sigtran_time_t     *p_timeout );

    sigtran_return_t sig_chk_if_server_alr_exist(
        soc_mgr_st         *p_soc_mgr,
        sig_soc_type_et    sock_type,
        sigtran_U32bit     ip_addr,
        sigtran_U16bit     port,
        sigtran_U16bit     *p_dest_id );

    sigtran_U32bit  sig_os_resolve_hostname(
        sigtran_U8bit      *p_hostname,
        sig_ip_addr_st     *p_ip_addr_list,
        sigtran_U32bit      num_addrs );

    sigtran_U32bit sig_os_gethostname(
        sigtran_U8bit      *p_hostname,
        sigtran_U32bit      host_len );

    sigtran_return_t  sig_null_fnc( sigtran_U32bit   handle_id,
                                    sig_sock_mode_et sock_mode,
                                    sig_soc_type_et  sock_type,
                                    sig_sock_fd_t    sock_fd,
                                    sig_q_module_t   *from_q_mod,
                                    sigtran_pvoid_t  *p_data_buf,
                                    sigtran_U32bit   *p_data_len,
                                    sigtran_error_t  *p_ecode );
#ifdef __cplusplus
}
#endif
#endif /* __NW_UTIL_H__ */
