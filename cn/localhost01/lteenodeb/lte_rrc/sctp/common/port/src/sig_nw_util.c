/*****************************************************************************
**
**  FILE NAME:
**      sig_nw_util.c
**
******************************************************************************
**
**  DESCRIPTION:
**      This file contains the implementation of the functions
**      related to soc manager in sigtran common
**
**  DATE            NAME                REFERENCE           REASON
**----------------------------------------------------------------------------
**  27OCt2003       Vishavdeep Sharma   HSS:XXXXXXXXX   Original
**  10JUL09         Kannan V            m3ua REL 6.2.0  klocwork fix done
**  10Apr10         Shivi Singh Verma   m3ua REL 6.2.2  klocwork fix done
**  11Aug11         Supreet Jain          SPR 21003       Performance Tunning
**  18Feb13         Rohan Pawa          SPR 21107       CSR 24680
**  1May13          Pranav              SPR 21127       CSR 40731-GCC 4.7 warning removal
**  1July13         Rohan Pawa          SPR 21141       CSR 50452
*     25Feb14     Rohan Pawa                       CSR 69326
**    22Aug14      Pinky sinha          Bug Id 18       CSR 87117
**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
**
*****************************************************************************/
#include <sig_nw_util.h>
#include <sig_util.h>
#include <sig_dbg.h>
#include <sig_ipc.h>
#include <sig_ipc_intr.h>
#include <sig_cspl_wrap.h>
#include <sig_mutx_n_sem.h>

/* CSR 69326 fix starts */
sig_sock_fd_t   g_dummy_fd;
/* CSR 69326 fix Ends */
/*****************************************************************************
**
**  FUNCTION NAME
**      sig_soc_mgr_init
**
**
**  DESCRIPTION
**      Initialize the socket  manager
**
**  INPUTS
**  p_soc_mgr:  socket ipc manager to be initialized
**
**  RETURNS
**      void
**
*****************************************************************************/

void soc_mgr_init
( soc_mgr_st               *p_soc_mgr,
  sig_get_sm_msg_len_cbk p_sm_cbk_fn,
  sig_get_su_msg_len_cbk p_su_cbk_fn,
  sig_get_tl_msg_len_cbk p_tl_cbk_fn )
{
    sigtran_U16bit  i;

    p_soc_mgr->p_sm_cbk_fn = p_sm_cbk_fn;
    p_soc_mgr->p_su_cbk_fn = p_su_cbk_fn;
    p_soc_mgr->p_tl_cbk_fn = p_tl_cbk_fn;

    for( i = 0; i < SOC_MGR_MAX_CONNECTIONS; ++i )
    {
        p_soc_mgr->conn_info[i].sock_mode = SIG_SOC_MODE_INVALID;
        p_soc_mgr->conn_info[i].sock_type = SIG_SOC_TYPE_INVALID;
        p_soc_mgr->conn_info[i].lcl_ip = SOC_INVALID_IP;
        p_soc_mgr->conn_info[i].lcl_port = SOC_INVALID_PORT;
        p_soc_mgr->conn_info[i].rem_ip = SOC_INVALID_IP;
        p_soc_mgr->conn_info[i].rem_port = SOC_INVALID_PORT;
        p_soc_mgr->conn_info[i].p_rd_cbk_fn = SIGTRAN_NULL;
        p_soc_mgr->conn_info[i].fd = SOC_MGR_INVALID_SOC_FD;
        sigtran_mutex_init( &( p_soc_mgr->conn_info[i].fd_mutex ) );
#ifdef SIG_COMMON_WITH_CSPL
        p_soc_mgr->conn_info[i].q_module = SOC_INV_Q_MODULE;
#endif
    }

}
/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_tcp_server
**
**
**  DESCRIPTION
**      Add a server to socket ipc manager,bind the port and
**              returns a handler
**
**
**  RETURNS
**      >= 0 dest_id/ handle on success;
**      0 on failure
**
*****************************************************************************/
sigtran_U16bit soc_mgr_add_tcp_server
(
    soc_mgr_st      *p_soc_mgr,
    sigtran_U32bit   serv_ip,
    sigtran_U16bit       serv_port,
    sig_ip_addr_st       *p_src_addr,
    sig_ip_addr_st       *p_dest_addr,
    sig_read_cbk_func_ptr_t  p_cbk_fn )
{
    sigtran_U32bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd = 0;
    /* set up tcp server*/
    server_fd = soc_mgr_setup_tcp_server( serv_ip, serv_port );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;

    /* CSR 69326 fix ends */
    if( server_fd > 0 )
    {
        /* server port is set */
        /* make entry in the soc manager data base */

        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_TCP;
            p_soc_mgr->conn_info[dest_id].fd = server_fd;

            if( p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
            }

            if( p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].dest_ip_str = *p_dest_addr;
            }

            p_soc_mgr->conn_info[dest_id].lcl_ip = serv_ip;
            p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            sig_add_fd_to_glb_fdset( server_fd );

        }

        else
        {
            /* no handler is availble need to increase max connection*/
            /*close the scoket open*/
            sig_os_close( server_fd );
        }
    }

    return dest_id;

}
/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_tcp_client
**
**
**  DESCRIPTION
**      Add a client to socket ipc manager
**  INPUTS
**
**  RETURNS
**      >= 0 on success;
**      0 on failure
**
*****************************************************************************/
sigtran_S16bit soc_mgr_add_tcp_client
(
    soc_mgr_st          *p_soc_mgr,
    sigtran_U32bit      lcl_ip,
    sigtran_U16bit      lcl_port,
    sigtran_U32bit      serv_ip,
    sigtran_U16bit      serv_port,
    sig_ip_addr_st      *p_src_addr,
    sig_ip_addr_st      *p_dest_addr,
    sig_read_cbk_func_ptr_t  p_cbk_fn )

{
    sigtran_U16bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   client_fd = 0;

    client_fd = soc_mgr_setup_tcp_client( lcl_ip, lcl_port,
                                          serv_ip, serv_port );
    /* CSR 69326 fix starts */
    g_dummy_fd = client_fd;
    /* CSR 69326 fix ends */

    if( client_fd > 0 )
    {

        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for client */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_CLIENT;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_TCP;
            p_soc_mgr->conn_info[dest_id].fd = client_fd;
            p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
            p_soc_mgr->conn_info[dest_id].dest_ip_str = *p_dest_addr;
            p_soc_mgr->conn_info[dest_id].lcl_ip = lcl_ip;
            p_soc_mgr->conn_info[dest_id].lcl_port = lcl_port;
            p_soc_mgr->conn_info[dest_id].rem_ip = serv_ip;
            p_soc_mgr->conn_info[dest_id].rem_port = serv_port;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            sig_add_fd_to_glb_fdset( client_fd );

        }

        else
        {
            /* no handle is available need to increase max connection */
            /* close the socket open */
            sig_os_close( client_fd );
        }
    }

    return dest_id;

}
/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_udp_server
**
**
**  DESCRIPTION
**      Add UDP  server  socket ipc manager
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_udp_server
(
    soc_mgr_st              *p_soc_mgr,
    sigtran_U32bit          serv_ip,
    sigtran_U16bit          serv_port,
    sig_ip_addr_st          *p_src_addr,
    sig_ip_addr_st          *p_dest_addr,
    sigtran_boolean_t       read_flag,
    sigtran_boolean_t       b_def_udp_wrt_port,
    sig_read_cbk_func_ptr_t  p_cbk_fn )
{
    sigtran_U16bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd;
    sigtran_U32bit  dest_ip = 0;
    sigtran_U16bit  dest_port = 0;
    sigtran_error_t ecode;

    /* check if for this ip and port already a handle exist*/
    sig_chk_if_server_alr_exist( p_soc_mgr, SIG_SOC_TYPE_UDP, serv_ip,
                                 serv_port, &dest_id );

    /*KW fix start*/
    if( dest_id != SOC_MGR_INVALID_HANDLE && ( dest_id < SOC_MGR_MAX_CONNECTIONS ) )
        /*KW fix end*/
    {
        /* for the given ip and port there is already a port
         * bind no need to bind port
         * just do a set udp destination for the remote address
         * given in dest addr
         */

        /* get the ip and port for the destination */
        if( SIGTRAN_NULL != p_dest_addr )
        {
            /*SPR 21141 changes starts*/
            if( SIGTRAN_FAILURE == sig_util_ext_ip_port_frm_ipc_info( p_dest_addr, &dest_ip,
                                                                      &dest_port, SIGTRAN_NULL,
                                                                      SIGTRAN_NULL, &ecode ) )
            {
                SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
            }

            /*SPR 21141 changes ends*/
        }

        /* get a new destnation id for this udp destination */
        dest_id = soc_mgr_add_udp_dest( p_soc_mgr, dest_ip, dest_port, dest_id );
        return dest_id;
    }


    server_fd = soc_mgr_setup_udp_server( serv_ip, serv_port, read_flag );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;
    /* CSR 69326 fix ends */

    if( server_fd > 0 )
    {
        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        /*KW fix start*/
        if( ( dest_id != SOC_MGR_INVALID_HANDLE ) && ( dest_id < SOC_MGR_MAX_CONNECTIONS ) )
        {
            /*KW fix end*/
            /* found a free handle for server fd */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_UDP;

            if( p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
            }

            if( p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].dest_ip_str = *p_dest_addr;

                /* get the ip and port for the destination */
                /* SPR 21107 changess */
                if( SIGTRAN_FAILURE == sig_util_ext_ip_port_frm_ipc_info( p_dest_addr, &dest_ip,
                                                                          &dest_port, SIGTRAN_NULL,
                                                                          SIGTRAN_NULL, &ecode ) )
                {
                    SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
                }

                /* SPR 21107 changess */

            }

            p_soc_mgr->conn_info[dest_id].fd = server_fd;
            p_soc_mgr->conn_info[dest_id].lcl_ip = serv_ip;
            p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            p_soc_mgr->conn_info[dest_id].rem_ip = dest_ip;
            p_soc_mgr->conn_info[dest_id].rem_port = dest_port;

            if( b_def_udp_wrt_port )
            {
                /* this server fd shall be used as default for
                 * sending messages to udp destinations in case
                 * for that destination no specific fd is set
                 */
                p_soc_mgr->udp_server_sock = server_fd;
            }

            /* set the fd is global fd set if it is read fd*/
            if( read_flag )
            {
                sig_add_fd_to_glb_fdset( server_fd );
            }
        }

        else
        {
            sig_os_close( server_fd );
        }
    }

    return dest_id;
}
/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_udp_dest
**
**
**  DESCRIPTION
**      Add UDP  remote udp destination in ipc manager for given stack
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_udp_dest
(
    soc_mgr_st      *p_soc_mgr,
    sigtran_U32bit  dest_ip,
    sigtran_U16bit  dest_port,
    sigtran_U16bit  handle_id )
{
    sigtran_U32bit    dest_id = SOC_MGR_INVALID_HANDLE;

    dest_id = soc_mgr_get_new_handle( p_soc_mgr );

    /* KW fix starts 6.2.2 */
    if( ( dest_id != SOC_MGR_INVALID_HANDLE ) && ( dest_id < SOC_MGR_MAX_CONNECTIONS ) )
        /* KW fix ends 6.2.2 */
    {
        /* found a free handle for server fd */
        p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_CLIENT;
        p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_UDP;

        if( handle_id == SOC_MGR_INVALID_HANDLE )
        {
            p_soc_mgr->conn_info[dest_id].fd = p_soc_mgr->udp_server_sock;
        }

        else
        {
            /* KW fix starts 6.2.2 */
            if( handle_id < SOC_MGR_MAX_CONNECTIONS )
            {
                p_soc_mgr->conn_info[dest_id].fd =  p_soc_mgr->conn_info[handle_id].fd;
            }

            /* KW fix ends 6.2.2 */
        }

        p_soc_mgr->conn_info[dest_id].rem_ip = dest_ip;
        p_soc_mgr->conn_info[dest_id].rem_port = dest_port;
    }

    return dest_id;

}
/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_get_new_handle
**
**
**  DESCRIPTION
**      locate a free socket handle
**
**  INPUTS
**
**  RETURNS
**      handle if successful
**      SOC_MGR_INVALID_HANDLE otherwise
**
*****************************************************************************/

sigtran_U16bit soc_mgr_get_new_handle
( soc_mgr_st            *p_soc_mgr )
{
    sigtran_U16bit  i;
    sigtran_U16bit  handle;

    handle = SOC_MGR_INVALID_HANDLE;

    /* handle 0 is invalid*/
    for( i = 1; i < SOC_MGR_MAX_CONNECTIONS; ++i )
    {
        if( SIG_SOC_MODE_INVALID == p_soc_mgr->conn_info[i].sock_mode )
        {
            handle = i;
            break;
        }
    }

    return handle;
}

/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_release_handle
**
**
**  DESCRIPTION
**      mark the handle as free; this handle can now be reused.
**
**  INPUTS
**
**  RETURNS
**      void
**
*****************************************************************************/

void soc_mgr_release_handle
( soc_mgr_st    *p_soc_mgr,
  sigtran_U16bit     i )
{
    p_soc_mgr->conn_info[i].sock_mode = SIG_SOC_MODE_INVALID;
    p_soc_mgr->conn_info[i].sock_type = SIG_SOC_TYPE_INVALID;
    p_soc_mgr->conn_info[i].lcl_ip = SOC_INVALID_IP;
    p_soc_mgr->conn_info[i].lcl_port = SOC_INVALID_PORT;
    p_soc_mgr->conn_info[i].rem_ip = SOC_INVALID_IP;
    p_soc_mgr->conn_info[i].rem_port = SOC_INVALID_PORT;
    p_soc_mgr->conn_info[i].p_rd_cbk_fn = ( sig_read_cbk_func_ptr_t )sig_null_fnc;
    p_soc_mgr->conn_info[i].fd = SOC_MGR_INVALID_SOC_FD;
#ifdef SIG_COMMON_WITH_CSPL
    p_soc_mgr->conn_info[i].q_module = SOC_INV_Q_MODULE;
#endif

}
/***********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_chk_activity
**
**
**  DESCRIPTION
**      This function checks for any activity on the indiviual conections
**      of stack
**
**  INPUTS
**
**  RETURNS
**
*********************************************************************/

sigtran_pvoid_t soc_mgr_chk_activity
( soc_mgr_st            *p_soc_mgr,
  sig_stack_type_et   stack_type,
  sig_q_module_t      *p_from_mod,
  sig_q_module_t      *p_to_mod )

{
    fd_set           *p_mod_glb_fds;
    sigtran_U32bit   lcl_data_len;
    sig_sock_fd_t    sockfd;
    sigtran_U16bit   dest_id;
    /* SPR 21003 Starts */
    sigtran_pvoid_t  lcl_p_data = SIGTRAN_NULL;
    /* SPR 21003 Ends */
    sigtran_error_t  lcl_ecode;
    soc_mgr_conn_info_st *p_conn_info;
    sigtran_return_t ret_val = SIGTRAN_FAILURE;


    /* get global modified fds */
    sig_get_glb_mod_fdset( &p_mod_glb_fds );

    for( dest_id = 1; dest_id < SOC_MGR_MAX_CONNECTIONS; dest_id++ )
    {
        p_conn_info = &p_soc_mgr->conn_info[dest_id];

        /* socket is intilised */
        if( ( SIG_SOC_MODE_SERVER == p_conn_info->sock_mode ) ||
                ( ( SIG_SOC_TYPE_TCP == p_conn_info->sock_type ) &&
                  ( SIG_SOC_MODE_CLIENT == p_conn_info->sock_mode ) ) )
        {
            sockfd = p_conn_info->fd;

            /*klock fix*/
            /*Rel 6.2.3 Connection Establishment Fix*/
#ifndef SIGTRAN_PORT_WINNT

            if( sockfd < FD_SETSIZE && FD_ISSET( sockfd, p_mod_glb_fds ) )
#else
            if( FD_ISSET( sockfd, p_mod_glb_fds ) )
#endif
            {
#ifdef SIG_COMMON_WITH_CSPL
                /* get to module for this stack */
                ret_val = sig_get_q_module( stack_type,
                                            SIG_DEST_TYPE_INVALID,
                                            SOC_MGR_INVALID_HANDLE,
                                            p_to_mod,
                                            &lcl_ecode );

                if( ret_val == SIGTRAN_FAILURE )
                {
                    return SIGTRAN_FAILURE;
                }

#endif
                stack_type = stack_type;
                p_to_mod = p_to_mod;
                /*throw call back of stack */
                ret_val = p_conn_info->p_rd_cbk_fn( ( sigtran_U32bit )dest_id,
                                                    p_conn_info->sock_mode,
                                                    p_conn_info->sock_type,
                                                    ( sig_sock_fd_t )p_conn_info->fd,
                                                    p_from_mod,
                                                    &lcl_p_data,
                                                    &lcl_data_len,
                                                    &lcl_ecode );


#ifdef SIG_COMMON_WITH_CSPL

                if( ( ret_val == SIGTRAN_SUCCESS ) && ( lcl_p_data != SIGTRAN_NULL )
                        /* KW Fix starts */
                        && ( p_to_mod != SIGTRAN_NULL ) && ( p_from_mod != SIGTRAN_NULL ) )
                    /* KW Fix ends */
                {
                    /* call qvincoming */
                    sig_qv_incoming( *p_from_mod, *p_to_mod, 0, lcl_p_data );
                }

#endif
            }/* end of FD_ISSET*/
        }
    }/* end of for loop */

    /*  SPR 21127 changes start */
    COMMON_UNUSED_VARIABLE( ret_val );
    /*  SPR 21127 changes end */
    return SIGTRAN_NULL;
}
/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_aceept_tcp_conn
**
**
**  DESCRIPTION
**      Aceept a TCP client connection
**  INPUTS
**
**  RETURNS
**      >= 0 on success;
**      0 on failure
**
*****************************************************************************/
sigtran_U16bit soc_mgr_aceept_tcp_conn(
    soc_mgr_st        *p_soc_mgr,
    sig_sock_fd_t     sock_fd,
    sig_read_cbk_func_ptr_t  p_cbk_fn,
    sigtran_U32bit    *p_ip_addr,
    sigtran_U16bit    *p_port )


{
    sigtran_U16bit   dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t    client_fd = 0;

    client_fd = soc_mgr_aceept( sock_fd, p_ip_addr, p_port );
    /* CSR 69326 fix starts */
    g_dummy_fd = client_fd;

    /* CSR 69326 fix ends */
    if( client_fd > 0 )
    {

        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for client */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_CLIENT;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_TCP;
            p_soc_mgr->conn_info[dest_id].fd = client_fd;
            p_soc_mgr->conn_info[dest_id].rem_ip = *p_ip_addr;
            p_soc_mgr->conn_info[dest_id].rem_port = *p_port;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            sig_add_fd_to_glb_fdset( client_fd );

        }

        else
        {
            /* no handle is available need to increase max connection */
            /* close the socket open */
            sig_os_close( client_fd );
        }
    }

    return dest_id;
}
/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_raw_ipv4_server
**
**
**  DESCRIPTION
**      Add RAW IPv4 server socket ipc manager
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_raw_ipv4_server
(
    soc_mgr_st       *p_soc_mgr,
    sigtran_U32bit       serv_ip,
    sigtran_U16bit       serv_port,
    sig_ip_addr_st      *p_src_addr,
    sig_ip_addr_st      *p_dest_addr,
    sigtran_boolean_t   read_flag,
    sig_read_cbk_func_ptr_t     p_cbk_fn )
{
    sigtran_U32bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd;

    server_fd = soc_mgr_setup_raw_ipv4_server( serv_ip, serv_port );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;
    /* CSR 69326 fix ends */

    if( server_fd > 0 )
    {
        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for server fd */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_IPV4;

            /*
             * Add source address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
                p_soc_mgr->conn_info[dest_id].lcl_ip = serv_ip;
            }

            /*
             * Add destination address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_dest_addr;
                p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            }

            p_soc_mgr->conn_info[dest_id].fd = server_fd;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            p_soc_mgr->udp_server_sock = server_fd;

            /* set the fd is global fd set if it is read fd*/
            if( read_flag )
            {
                sig_add_fd_to_glb_fdset( server_fd );
            }

            /* Bug Id 18 fix starts*/
            else
            {
                sigtran_UL32bit recv_buf_size = 0;
                sigtran_U32bit  recv_buf_len = sizeof( recv_buf_size );

                if( setsockopt( server_fd, SOL_SOCKET, SO_RCVBUF, ( const void * )&recv_buf_size, recv_buf_len ) < 0 )
                {
                    SIG_DEBUG_PRINT( ( "\nERROR:: send socket SO_RCVBUF setsockOpt failed" ) );
                }

                else
                {
                    SIG_DEBUG_PRINT( ( "\nINFO:: successfully set send socket SO_RCVBUF : %u", recv_buf_size ) );
                }
            }

            /* Bug Id 18 fix ends */
            return dest_id;
        }

        else
        {
            sig_os_close( server_fd );
            return dest_id;
        }
    }

    else /* unable to open raw ip server */
    {
        return dest_id;
    }
}


/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_icmp_ipv4_server
**
**
**  DESCRIPTION
**      Add ICMP IPv4 server socket ipc manager
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_icmp_ipv4_server
(
    soc_mgr_st                  *p_soc_mgr,
    sigtran_U32bit              serv_ip,
    sigtran_U16bit              serv_port,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sig_read_cbk_func_ptr_t     p_cbk_fn )
{
    sigtran_U32bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd;

    server_fd = soc_mgr_setup_icmp_ipv4_server( serv_ip, serv_port );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;
    /* CSR 69326 fix ends */

    if( server_fd > 0 )
    {
        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for server fd */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_ICMPV4;

            /*
             * Add source address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
                p_soc_mgr->conn_info[dest_id].lcl_ip = serv_ip;
            }

            /*
             * Add destination address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_dest_addr;
                p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            }

            p_soc_mgr->conn_info[dest_id].fd = server_fd;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            p_soc_mgr->udp_server_sock = server_fd;

            /* set the fd is global fd set if it is read fd*/
            if( read_flag )
            {
                sig_add_fd_to_glb_fdset( server_fd );
            }

            return dest_id;
        }

        else
        {
            sig_os_close( server_fd );
            return dest_id;
        }
    }

    else /* unable to open raw ip server */
    {
        return dest_id;
    }
}

/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_raw_ipv6_server
**
**
**  DESCRIPTION
**      Add RAW IPv6 server socket ipc manager
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_raw_ipv6_server
(
    soc_mgr_st                  *p_soc_mgr,
    sigtran_U32bit              *p_serv_ip,
    sigtran_U16bit              serv_port,
    sigtran_U32bit              flow_info,
    sigtran_U32bit              scope_id,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sig_read_cbk_func_ptr_t     p_cbk_fn )
{
    sigtran_U32bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd;

    server_fd = soc_mgr_setup_raw_ipv6_server( p_serv_ip, serv_port, flow_info, scope_id );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;
    /* CSR 69326 fix ends */

    if( server_fd > 0 )
    {
        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for server fd */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_IPV6;

            /*
             * Add source address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
            }

            /*
             * Add destination address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_dest_addr;
                p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            }

            p_soc_mgr->conn_info[dest_id].fd = server_fd;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            p_soc_mgr->udp_server_sock = server_fd;

            /* set the fd is global fd set if it is read fd*/
            if( read_flag )
            {
                sig_add_fd_to_glb_fdset( server_fd );
            }

            return dest_id;
        }

        else
        {
            sig_os_close( server_fd );
            return dest_id;
        }
    }

    else /* unable to open raw ip server */
    {
        return dest_id;
    }
}


/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_icmp_ipv6_server
**
**
**  DESCRIPTION
**      Add ICMP IPv6 server socket ipc manager
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_icmp_ipv6_server
(
    soc_mgr_st                  *p_soc_mgr,
    sigtran_U32bit              *p_serv_ip,
    sigtran_U16bit              serv_port,
    sigtran_U32bit              flow_info,
    sigtran_U32bit              scope_id,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sig_read_cbk_func_ptr_t     p_cbk_fn )
{
    sigtran_U32bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd;

    server_fd = soc_mgr_setup_icmp_ipv6_server(
                    p_serv_ip, serv_port, flow_info, scope_id );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;
    /* CSR 69326 fix ends */

    if( server_fd > 0 )
    {
        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for server fd */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_ICMPV6;

            /*
             * Add source address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
            }

            /*
             * Add destination address only if the address is non-null.
             */
            if( SIGTRAN_NULL != p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_dest_addr;
                p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            }

            p_soc_mgr->conn_info[dest_id].fd = server_fd;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            p_soc_mgr->udp_server_sock = server_fd;

            /* set the fd is global fd set if it is read fd*/
            if( read_flag )
            {
                sig_add_fd_to_glb_fdset( server_fd );
            }

            return dest_id;
        }

        else
        {
            sig_os_close( server_fd );
            return dest_id;
        }
    }

    else /* unable to open raw ip server */
    {
        return dest_id;
    }
}


/********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_add_udp_nw_server
**
**
**  DESCRIPTION
**      Add UDP  server  socket ipc manager
**  RETURN
**      Success - dest_id/handler
**      Failure - 0
**
**
***********************************************************************/
sigtran_U16bit soc_mgr_add_udp_nw_server
(
    soc_mgr_st              *p_soc_mgr,
    sigtran_U32bit          serv_ip,
    sigtran_U16bit          serv_port,
    sig_ip_addr_st          *p_src_addr,
    sig_ip_addr_st          *p_dest_addr,
    sigtran_boolean_t       read_flag,
    sig_read_cbk_func_ptr_t  p_cbk_fn )
{
    sigtran_U32bit  dest_id = SOC_MGR_INVALID_HANDLE;
    sig_sock_fd_t   server_fd;

    server_fd = soc_mgr_setup_udp_nw_server( serv_ip, serv_port );
    /* CSR 69326 fix starts */
    g_dummy_fd = server_fd;

    /* CSR 69326 fix ends */
    if( server_fd > 0 )
    {
        dest_id = soc_mgr_get_new_handle( p_soc_mgr );

        if( dest_id != SOC_MGR_INVALID_HANDLE )
        {
            /* found a free handle for server fd */
            p_soc_mgr->conn_info[dest_id].sock_mode = SIG_SOC_MODE_SERVER;
            p_soc_mgr->conn_info[dest_id].sock_type = SIG_SOC_TYPE_UDP_NW;

            if( p_src_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_src_addr;
            }

            if( p_dest_addr )
            {
                p_soc_mgr->conn_info[dest_id].lcl_ip_str = *p_dest_addr;
            }

            p_soc_mgr->conn_info[dest_id].fd = server_fd;
            p_soc_mgr->conn_info[dest_id].lcl_ip = serv_ip;
            p_soc_mgr->conn_info[dest_id].lcl_port = serv_port;
            p_soc_mgr->conn_info[dest_id].p_rd_cbk_fn = p_cbk_fn;
            p_soc_mgr->udp_server_sock = server_fd;

            /* set the fd is global fd set if it is read fd*/
            if( read_flag )
            {
                sig_add_fd_to_glb_fdset( server_fd );
            }

            return dest_id;

        }

        else
        {
            sig_os_close( server_fd );
            return dest_id;
        }
    }

    else /* unable to open udp server */
    {
        return dest_id;
    }
}
/******************************************************************************
**  FUNCTION :    sig_chk_if_server_alr_exist
**
**  DESCRIPTION:  This function checks for given server ip and port if
**                already udp server port is bind.Function returns a valid
**                dest id if srever already exist otherwise return invalid id
**
**    RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_chk_if_server_alr_exist(
    soc_mgr_st         *p_soc_mgr,
    sig_soc_type_et    sock_type,
    sigtran_U32bit     ip_addr,
    sigtran_U16bit     port,
    sigtran_U16bit     *p_dest_id )
{
    sigtran_U16bit dest_id = SOC_MGR_INVALID_HANDLE;

    SIG_DEBUG_PRINT( ( "\nsig_chk_if_server_alr_exist:: enterned function" ) );


    if( SIG_SOC_TYPE_UDP == sock_type )
    {
        /* check if already a handle exist for the
         * given ip and port
         */
        for( dest_id = 1; dest_id < SOC_MGR_MAX_CONNECTIONS; dest_id++ )
        {
            if( p_soc_mgr->conn_info[dest_id].sock_type == SIG_SOC_TYPE_UDP )
            {
                /* compare the local ip and port */
                if( ( p_soc_mgr->conn_info[dest_id].lcl_ip == ip_addr ) &&
                        ( p_soc_mgr->conn_info[dest_id].lcl_port == port ) )
                {
                    SIG_DEBUG_PRINT( ( "\nnsig_chk_if_server_alr_exist::"
                                       "Server alrerady open return same dest id" ) );
                    *p_dest_id = dest_id;
                    break;
                }

            }
        }
    }

    else /* invalid case */
    {
        SIG_DEBUG_PRINT( ( "\nnsig_chk_if_server_alr_exist::Invalid paramter" ) );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_null_fnc
**
**  DESCRIPTION:  This is a null/dummy function
**
**    RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_null_fnc( sigtran_U32bit   handle_id,
                               sig_sock_mode_et sock_mode,
                               sig_soc_type_et  sock_type,
                               sig_sock_fd_t    sock_fd,
                               sig_q_module_t   *from_q_mod,
                               sigtran_pvoid_t  *p_data_buf,
                               sigtran_U32bit   *p_data_len,
                               sigtran_error_t  *p_ecode )
{
    handle_id = handle_id;
    sock_mode = sock_mode;
    sock_type = sock_type;
    sock_fd = sock_fd;
    from_q_mod = from_q_mod;
    p_data_buf = p_data_buf;
    p_data_len = p_data_len;
    p_ecode = p_ecode;

    return SIGTRAN_NULL;
}
