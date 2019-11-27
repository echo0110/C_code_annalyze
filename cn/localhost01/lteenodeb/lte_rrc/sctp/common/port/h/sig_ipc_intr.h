/*****************************************************************************
 *    FILE NAME:
 *        sig_ipc_intr.h
 *
 *    DESCRIPTION:
 *       This file has decl of structures and function
 *       used by ipc manager internally for IPC .
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    02/11/2003  Vishavdeep Sharma   -            Original Creation
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __SIG_IPC_INTR_H__
#define __SIG_IPC_INTR_H__

#include <sig_typs.h>
#include <sig_nw_util.h>
#include <sig_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Data types of ipc manager used internaly by it */

    typedef struct
    {
        /* flag set to true when any stack does init ipc */
        sigtran_boolean_t stack_reg;
        /* connection info per stack for external interfaces  */
        soc_mgr_st ipc_conn_info;
        /* map from dest id  to key */
        sigtran_U32bit g_map_key_to_dest_id[SIG_MAX_VALID_MAP_KEY];
#ifdef SIG_COMMON_WITH_CSPL
        /* info for the internal Q modules */
        sig_q_module_t g_map_for_internal_modules[SIG_MAX_DEST_TYPE];
#endif
    } sig_ipc_mgr_glb_data_st;


    sigtran_return_t sig_setup_tcp_server(
        sig_stack_type_et   stack_type,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sigtran_U16bit      *p_dest_id,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_error_t     *p_ecode );

    sigtran_return_t sig_setup_tcp_server(
        sig_stack_type_et   stack_type,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sigtran_U16bit      *p_dest_id,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_error_t     *p_ecode );


    sigtran_return_t sig_setup_udp_server(
        sig_stack_type_et   stack_type,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sigtran_boolean_t   read_flag,
        sigtran_boolean_t   b_def_udp_wrt_port,
        sigtran_U16bit      *p_dest_id,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_error_t     *p_ecode );

    sigtran_return_t sig_setup_tcp_client(
        sig_stack_type_et   stack_type,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sigtran_U16bit      *p_dest_id,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_error_t     *p_ecode );

    sigtran_return_t sig_setup_raw_ipv4_server(
        sig_stack_type_et           stack_type,
        sig_ip_addr_st              *p_src_addr,
        sig_ip_addr_st              *p_dest_addr,
        sigtran_boolean_t           read_flag,
        sigtran_U16bit              *p_dest_id,
        sig_read_cbk_func_ptr_t     p_cbk_fn,
        sigtran_error_t             *p_ecode );

    sigtran_return_t sig_setup_icmp_ipv4_server(
        sig_stack_type_et           stack_type,
        sig_ip_addr_st              *p_src_addr,
        sig_ip_addr_st              *p_dest_addr,
        sigtran_boolean_t           read_flag,
        sigtran_U16bit              *p_dest_id,
        sig_read_cbk_func_ptr_t     p_cbk_fn,
        sigtran_error_t             *p_ecode );

    sigtran_return_t sig_setup_raw_ipv6_server(
        sig_stack_type_et           stack_type,
        sig_ip_addr_st              *p_src_addr,
        sig_ip_addr_st              *p_dest_addr,
        sigtran_boolean_t           read_flag,
        sigtran_U16bit              *p_dest_id,
        sig_read_cbk_func_ptr_t     p_cbk_fn,
        sigtran_error_t             *p_ecode );

    sigtran_return_t sig_setup_icmp_ipv6_server(
        sig_stack_type_et           stack_type,
        sig_ip_addr_st              *p_src_addr,
        sig_ip_addr_st              *p_dest_addr,
        sigtran_boolean_t           read_flag,
        sigtran_U16bit              *p_dest_id,
        sig_read_cbk_func_ptr_t     p_cbk_fn,
        sigtran_error_t             *p_ecode );

    sigtran_return_t sig_setup_udp_nw_server(
        sig_stack_type_et   stack_type,
        sig_ip_addr_st      *p_src_addr,
        sig_ip_addr_st      *p_dest_addr,
        sigtran_boolean_t   read_flag,
        sigtran_U16bit      *p_dest_id,
        sig_read_cbk_func_ptr_t  p_cbk_fn,
        sigtran_error_t     *p_ecode );



    sigtran_return_t sig_ipc_mgr_send_api(
        sig_stack_type_et   stack_type,
        sigtran_U16bit      handle,
        const sigtran_void_t      *p_message,
        sigtran_U32bit      size,
        sigtran_boolean_t   b_is_iovec );

    sigtran_return_t sig_clr_fd_from_glb_fdset( sig_sock_fd_t fd );

    sigtran_return_t sig_add_fd_to_glb_fdset( sig_sock_fd_t fd );


    sigtran_return_t sig_get_glb_fd_set( fd_set *p_fd_set );

    sigtran_return_t sig_get_glb_mod_fdset( fd_set **p_fd_set );

    sigtran_S32bit sig_os_close( sig_sock_fd_t sockfd );

    sigtran_U32bit sigtran_random( void );

    void sigtran_srandom( sigtran_U32bit value );

#ifdef __cplusplus
}
#endif
#endif
