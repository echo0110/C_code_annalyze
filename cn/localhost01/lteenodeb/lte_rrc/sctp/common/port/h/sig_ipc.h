/*****************************************************************************
 *    FILE NAME:
 *        sig_ipc.h
 *
 *    DESCRIPTION:
 *       This file has decl of structures related and porting function
 *       for IPC related functions.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    02/11/2003  Vishavdeep Sharma   -            Original Creation
 *    05/04/2011   Ravi J Sharma    SPR 20872       Single Threaded Change
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __SIG_IPC_H__
#define __SIG_IPC_H__

#include <sig_typs.h>
#ifdef __cplusplus
extern "C" {
#endif

    /* API structure for connection accept, and client register req/resp */
    typedef struct
    {
        sigtran_U16bit  api_id;
        sigtran_U32bit  ip_addr;
        sigtran_U16bit  port;
        sigtran_U32bit  dest_id;
    } sig_ipc_indication_st;



    /*
     * structure used in set destination this structure hold infomation
     * related to per destination and is used at the time of sending messages
     * on to network
     */
    typedef struct
    {
        sigtran_U32bit      dest_id;
        sig_stack_type_et   stack_type;
    } sig_dest_module_data_st;

    typedef struct
    {
        sig_stack_type_et   stack_type;
        /*  socket on which data has been received or to be sent */
        sigtran_U16bit  dest_id;
        /* flag to indicate ivovector */
        sigtran_boolean_t   b_is_iovec;
        /* pointer to the message or iovector*/
        sigtran_pvoid_t p_buffer;
        /* message size or iovec length */
        sigtran_U32bit      buffer_size;
        /* destination from which the data has been received or
         * to which the data has to be sent.  */
        sig_ip_addr_st  ip_addr;
        /* Socket type on which the data has been received */
        sig_soc_type_et   sock_type;
#ifdef SIGTRAN_INET6_SUPPORT
        /* This will contain the IPV6 HOPLIMIT and TCLASS */
        sigtran_U32bit    send_flags;
#endif
    } sig_iovec_ipc_buffer_st;


    /* structure to be passed to shell_open function */

    typedef struct
    {
        sig_stack_type_et self_type;
        sig_sock_mode_et  sock_mode;
        sig_soc_type_et   sock_type;
        sigtran_boolean_t read_flag;
        sigtran_boolean_t b_default_udp_srv;
        sig_ip_addr_st    *p_src_ip_addr;
        sig_ip_addr_st    *p_dest_ip_addr;
        sig_read_cbk_func_ptr_t   cbk_fn;
        sigtran_U16bit    dest_id;
        sigtran_error_t   ecode;
    } sig_shell_open_data_st;

    /* structure to be passed to shell_close function */
    typedef struct
    {
        sig_stack_type_et self_type;
        sigtran_U16bit    dest_id;
        sigtran_error_t  *p_ecode;
    } sig_shell_close_data_st;


    /* function prototypes for the interfaces exposed by IPC Manager to
     * stacks and clients
     */

#ifdef SIG_COMMON_WITH_CSPL
    sigtran_return_t sig_set_q_module(
        sig_stack_type_et  stack_type,
        sig_dest_type_et   dest_type,
        sigtran_U16bit     dest_id,
        sig_q_module_t     q_module,
        sigtran_error_t    *p_ecdoe );

    sigtran_return_t sig_get_q_module(
        sig_stack_type_et stack_type,
        sig_dest_type_et  dest_type,
        sigtran_U16bit    dest_id,
        sig_q_module_t    *p_q_module,
        sigtran_error_t   *p_ecode );
#endif

    sigtran_return_t sig_get_dest_id(
        sig_stack_type_et  stack_type,
        sig_soc_type_et    sock_type,
        /* handle of the fd which will be used
         * as send fd for this channel
         */
        sigtran_U16bit     mesg_dest_id,
        sigtran_U32bit     ip_addr,
        sigtran_U16bit     port,
        sigtran_U16bit     *p_dest_id,
        sigtran_boolean_t  *p_b_is_new,
        sigtran_error_t    *p_ecode );

    sigtran_return_t sig_get_ipc_info_frm_service_name(
        sigtran_UL32bit  service_name,
        sig_stack_type_et *p_stack_type,
        sig_dest_type_et  *p_dest_type,
        sigtran_U16bit    *p_dest_id,
        sigtran_error_t   *p_ecode );

    sigtran_return_t  sig_init_ipc_mgr(
        sig_stack_type_et   stack_type,
        sig_get_sm_msg_len_cbk  p_sm_cbk_fn,
        sig_get_su_msg_len_cbk  p_su_cbk_fn,
        sig_get_tl_msg_len_cbk  p_tl_cbk_fn );


    sigtran_return_t sig_set_in_map(
        sig_stack_type_et stack_type,
        /* map key is any key say logical user id for m3ua */
        sigtran_U32bit    map_key,
        sigtran_U16bit    dest_id,
        sigtran_error_t   *p_code );

    sigtran_return_t sig_get_from_map(
        sig_stack_type_et stack_type,
        /* map key is any key say logical user id for m3ua */
        sigtran_U32bit    map_key,
        sigtran_U16bit    *p_dest_id,
        sigtran_error_t   *p_code );

    sigtran_return_t sig_open_comm_channel(
        sig_stack_type_et self_type,
        sig_sock_mode_et  sock_mode,
        sig_soc_type_et   sock_type,
        /* flag true indicates polling is
         * required on this fd,else only for send*/
        sigtran_boolean_t b_read_fd,
        /* flag true indicates if this udp socket
         * is to be used as default write socket
         * while sending msgs to udp destinations
         */
        sigtran_boolean_t b_def_udp_wrt_port,
        sig_ip_addr_st    *p_src_addr,
        sig_ip_addr_st    *p_dest_addr,
        sig_read_cbk_func_ptr_t cbk_fn,
        sigtran_U16bit    *p_dest_id,
        sigtran_error_t    *p_ecode );


    sigtran_return_t  sig_close_comm_channel(
        sig_stack_type_et module_type,
        sigtran_U16bit dest_id );

    sigtran_return_t sig_send(
        sig_stack_type_et self_type,
        sig_dest_type_et  dest_type,
        sigtran_U16bit    dest_id,
        sigtran_pvoid_t   data_buf,
        sigtran_U32bit    data_len,
        sigtran_error_t   *p_ecode );

    sigtran_return_t sig_read(
        sig_stack_type_et self_type,
        sig_soc_type_et   sock_type,
        sig_sock_fd_t     sock_fd,
        sigtran_U32bit    max_read_size,
        /* raed flag PEEK/ALL */
        sig_recv_flag_et  read_flag,
        sig_ip_addr_st    *p_addr,
        sigtran_pvoid_t   p_pdata_buf,
        sigtran_U32bit    *p_data_len_read,
        sigtran_error_t   *p_ecode );

    sigtran_return_t sig_accept_cli_conn(
        sig_stack_type_et self_type,
        sig_sock_fd_t     sock_fd,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_U16bit    *p_dest_id,
        sigtran_U32bit    *ip_addr,
        sigtran_U16bit    *port,
        sigtran_error_t     *p_ecode );

    sigtran_return_t  sig_get_conn_local_addr(
        sig_stack_type_et   self_type,
        sigtran_U16bit      dest_id,
        sig_ip_addr_st      *p_local_addr,
        sigtran_error_t     *p_ecode );

    /* open function to be registered with CSPL */
    sigtran_pvoid_t sig_shell_open( sigtran_pvoid_t p_s );

    /* close function to be registered with CSPL */

    sigtran_void_t sig_shell_close( sigtran_pvoid_t p_s );

    /* recv function to be registered with CSPL */
    sigtran_pvoid_t sig_recv_from_ext_module(
        sigtran_pvoid_t s,
        sig_q_module_t *p_from,
        sig_q_module_t *p_to,
        sigtran_S8bit  *p_priority );

#ifdef SIG_COMMON_WITH_CSPL
    sigtran_return_t sig_get_q_module_info(
        sig_stack_type_et    self_stack,
        sig_q_module_t       q_module,
        sig_dest_type_et     *p_dest_type,
        sigtran_U16bit       *p_dest_id );

    /* send function to be registered with CSPL */
    sigtran_void_t sig_send_to_ext_module(
        sigtran_void_t         *s,
        const sigtran_void_t   *message,
        sig_q_module_t          from,
        sig_q_module_t          to,
        sigtran_pvoid_t         dest,
        sigtran_U32bit          size );

#endif
    sigtran_return_t sig_get_glb_fd_set( sig_fd_set *p_fd_set ); /* SPR 20872 */

    sigtran_return_t sig_set_glb_fd_set( sig_fd_set *p_fd_set );

    sigtran_return_t sig_is_fd_set_in_glb_fdset( sig_sock_fd_t fd );


    /* timed wait function  to be registered with CSPL */
    sigtran_void_t sig_timed_wait(
        sigtran_pvoid_t         s,
        const sig_q_time_st     *q_time );

    /* SPR 20872: Changes for enabling single threaded support Starts*/
    sig_sock_fd_t sig_get_max_fd( void );
    /* SPR 20872: Changes for enabling single threaded support Ends*/

    void sig_set_block_select( void );

    sigtran_S32bit sig_ipc_select(
        sigtran_time_t    *p_timeout );

    sigtran_return_t sig_resolve_hostname(
        sigtran_U8bit      *p_hostname,
        sig_ip_addr_st     *p_ip_addr_list,
        sigtran_U32bit     *p_num_addrs,
        sigtran_error_t    *p_ecode );

    sigtran_return_t sig_gethostname(
        sigtran_U8bit      *p_hostname,
        sigtran_U32bit      host_len,
        sigtran_error_t    *p_ecode );
#ifndef SIG_COMMON_WITH_CSPL
    sigtran_return_t    sig_get_q_module( sig_stack_type_et stack_type,
                                          sig_dest_type_et  dest_type,
                                          sigtran_U16bit    dest_id,
                                          sig_q_module_t    *p_q_module,
                                          sigtran_error_t   *p_ecode );
    sigtran_return_t sig_set_destination(
        sig_stack_type_et stack_type,
        sig_dest_type_et  dest_type,
        sigtran_U16bit    handle,
        sigtran_pvoid_t   listner,
        sigtran_pvoid_t   data,
        sig_q_module_t    *p_q_module,
        sigtran_error_t   *p_ecode );

    sigtran_return_t    sig_set_q_module( sig_stack_type_et stack_type,
                                          sig_dest_type_et  dest_type,
                                          sigtran_U16bit    dest_id,
                                          sig_q_module_t    q_module,
                                          sigtran_error_t   *p_ecode );

#endif

#ifdef __cplusplus
}
#endif
#endif
