/*****************************************************************************
 *    FILE NAME:
 *        sig_ipc.c
 *
 *    DESCRIPTION:
 *       This file has porting function for IPC related related functions.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    02/11/2003  Vishavdeep Sharma   -            Original Creation
 *    10-JUL09      Kannan v       M3UA Rel 6.2.0  Klocwork warning fix done
 *    13Sep10       Shivani Mehrotra M3UA Rel 6.2.3 Connection Issue in Windows
 *    05Apr11       Ravi J Sharma  SPR 20872       Single Threaded Change
 *    13Sep11       Sandeep Singh  SPR-21022       Vxworks-6.8 Porting Changes
 *  18Feb13    Rohan Pawa   SPR 21107    CSR 24680
 *  1May13     Pranav       SPR 21127      CSR 40731-GCC 4.7 warning removal
 *  1July13    Rohan Pawa   SPR 21141       CSR 50452
 *  25July13   Rohan Pawa   SPR 21144       CSR 51172
 *  25Feb14    Rohan Pawa                   CSR 69326
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include <sig_ipc.h>
#include <sig_err.h>
#include <sig_dbg.h>
#include <sig_defs.h>
#include <sig_util.h>
#include <sig_mem.h>
#include <sig_cspl_wrap.h>
#include <sig_ipc_intr.h>
#include <sig_nw_util.h>
#include <sig_mutx_n_sem.h>
/* SPR 21107 changes */
#include <sig_ipc.h>
/* SPR 21107 changes */
#ifdef SIGTRAN_SINGLE_THREAD_ARICENT_INTERNAL
#ifdef SIGTRAN_SINGLE_THREAD
#include<io.h>
int flag = 0;
#endif
#endif

/* global data of ipc manager */
sig_ipc_mgr_glb_data_st  g_ipc_mgr_data[SIG_MAX_NUM_STACK];

/* largest fd number used in select call */
static sig_sock_fd_t max_fd = 0;

/* global fd set which containd fds for all stacks and connections */
sig_fd_set      sig_gloabl_fdset;
sig_fd_set      *p_sig_gloabl_fdset = &sig_gloabl_fdset;

/* global modified fd set which will be passed to timed wait and
 * used subsequently in read operations*/
sig_fd_set      global_mod_fd_set;

/* global variable to store number of fds
 * modified by select
 * This varibale is used to save unncessary checking of
 * indiviual sockets in case no ativity has taken place
 * on any of the sockets
 */
sigtran_S32bit g_num_fd_mod;

sigtran_U32bit sig_sel_timeval = SIG_SELECT_TIMEOUT;


/******************************************************************************
**  FUNCTION :    sigtran_init_ipc_mgr
**
**  DESCRIPTION:  This function intilise ipc manager
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t  sig_init_ipc_mgr(
    sig_stack_type_et   stack_type,
    sig_get_sm_msg_len_cbk  p_sm_cbk_fn,
    sig_get_su_msg_len_cbk  p_su_cbk_fn,
    sig_get_tl_msg_len_cbk  p_tl_cbk_fn )
{
    sigtran_U16bit  lcl_index;
    static sigtran_boolean_t ipc_mgr_initialised = SIGTRAN_FALSE;

    SIG_DEBUG_PRINT( ( "\nsig_init_ipc_mgr::Init ipc manager"
                       "for stack = %d ", stack_type ) );

    g_ipc_mgr_data[stack_type].stack_reg = SIGTRAN_TRUE;

    /* intilise map for this module */
    for( lcl_index = 0; lcl_index < SOC_MGR_MAX_CONNECTIONS; lcl_index++ )
    {
        g_ipc_mgr_data[stack_type].g_map_key_to_dest_id[lcl_index] =
            SOC_MGR_INVALID_HANDLE;
    }

#ifdef SIG_COMMON_WITH_CSPL

    /* intilise the map for internal Q modules */
    for( lcl_index = 0; lcl_index < SIG_MAX_DEST_TYPE; lcl_index++ )
    {
        g_ipc_mgr_data[stack_type].g_map_for_internal_modules[lcl_index] =
            SOC_INV_Q_MODULE;

    }

#endif

    /* initilise connection table info for the stack */

    soc_mgr_init( &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                  p_sm_cbk_fn,
                  p_su_cbk_fn,
                  p_tl_cbk_fn );

    if( SIGTRAN_FALSE == ipc_mgr_initialised )
    {
        FD_ZERO( p_sig_gloabl_fdset );
        ipc_mgr_initialised = SIGTRAN_TRUE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_open_comm_channel
**
**  DESCRIPTION:  This function starts up a communication server TCP/UDP/RAW IP
**                based on the soket type provide
**
**  RETURN VALUE:
**                on SUCCESS
**                Function returns a valid dest_id by which stack identify this
**                Server for further communication
**                on Failure
**                retrun invalid dest_id = 0
******************************************************************************/

sigtran_return_t sig_open_comm_channel(
    sig_stack_type_et self_type,
    sig_sock_mode_et  sock_mode,
    sig_soc_type_et   sock_type,
    sigtran_boolean_t b_read_fd,
    sigtran_boolean_t b_def_udp_wrt_port,
    sig_ip_addr_st    *p_src_addr,
    sig_ip_addr_st    *p_dest_addr,
    sig_read_cbk_func_ptr_t cbk_fn,
    sigtran_U16bit    *p_dest_id,
    sigtran_error_t    *p_ecode )


{
    sig_shell_open_data_st s_open;

    SIG_DEBUG_PRINT( ( "\nsig_open_comm_channel::entered function" ) );

    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    /* fill the s_open structure call QVOPEN */

    s_open.self_type = self_type ;
    s_open.sock_mode = sock_mode;
    s_open.sock_type = sock_type;
    s_open.read_flag = b_read_fd;
    s_open.b_default_udp_srv = b_def_udp_wrt_port;
    s_open.p_src_ip_addr = p_src_addr;
    s_open.p_dest_ip_addr = p_dest_addr;
    s_open.cbk_fn = cbk_fn;
    s_open.dest_id = SOC_MGR_INVALID_HANDLE;
    s_open.ecode = ESIG_NO_ERROR;

#ifdef SIG_COMMON_WITH_CSPL
    /* call the qvopen */
    sig_qv_open( ( sigtran_pvoid_t )&s_open );
#else
    sig_shell_open( ( sigtran_pvoid_t )&s_open );
#endif

    if( ( s_open.dest_id == SOC_MGR_INVALID_HANDLE ) ||
            ( s_open.ecode != ESIG_NO_ERROR ) )
    {
        /* there is some error */
        SIGTRAN_SET_ECODE( p_ecode, s_open.ecode );
        *p_dest_id = s_open.dest_id;
        SIG_DEBUG_PRINT( ( "\nsig_open_comm_channel::unable to open channel" ) );
        return SIGTRAN_FAILURE;
    }

    else
    {
        *p_dest_id = s_open.dest_id;
        SIG_DEBUG_PRINT( ( "\nsig_open_comm_channel::channel open succ" ) );
        return SIGTRAN_SUCCESS;
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return SIGTRAN_SUCCESS;
#endif
}
/******************************************************************************
**  FUNCTION :    sig_shell_open
**
**  DESCRIPTION:  This function is registered with cspl at shell level to
**                open all the communictaion channels.This function is called
**                by Qvopen  internally
**
**  RETURN VALUE: sigtran_pvoid_t
******************************************************************************/
sigtran_pvoid_t sig_shell_open( sigtran_pvoid_t p_s )
{
    sig_shell_open_data_st *p_s_open;

    sig_stack_type_et self_type;
    sig_sock_mode_et  sock_mode;
    sig_soc_type_et   sock_type;
    sig_ip_addr_st    *p_src_ip_addr;
    sig_ip_addr_st    *p_dest_ip_addr;
    sigtran_boolean_t read_flag, def_udp_srv;
    sig_read_cbk_func_ptr_t   p_read_callback_fn;
    sigtran_U16bit    lcl_dest_id = 0;
    sigtran_error_t   lcl_ecode = 0;


    p_s_open = ( sig_shell_open_data_st * )p_s;
    /* extract the info from struct */
    self_type   = p_s_open->self_type ;
    sock_mode   = p_s_open->sock_mode;
    sock_type   = p_s_open->sock_type;
    p_src_ip_addr = p_s_open->p_src_ip_addr;
    p_dest_ip_addr = p_s_open->p_dest_ip_addr;
    read_flag    = p_s_open->read_flag;
    def_udp_srv  = p_s_open->b_default_udp_srv;
    p_read_callback_fn = p_s_open->cbk_fn;
    lcl_ecode = p_s_open->ecode;
    lcl_dest_id = p_s_open->dest_id;

    if( SIG_SOC_MODE_SERVER == sock_mode )
    {
        SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting up server" ) );

        switch( sock_type )
        {
            case SIG_SOC_TYPE_TCP:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting tcp server" ) );
                    sig_setup_tcp_server( self_type, p_src_ip_addr,
                                          p_dest_ip_addr, &lcl_dest_id,
                                          p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            case SIG_SOC_TYPE_UDP:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting udp server" ) );

                    sig_setup_udp_server( self_type, p_src_ip_addr, p_dest_ip_addr,
                                          read_flag, def_udp_srv, &lcl_dest_id,
                                          p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

                /* following cases are of SCTP N/W interfaces */
            case SIG_SOC_TYPE_UDP_NW:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::sig_setup_udp_nw_server" ) );

                    sig_setup_udp_nw_server( self_type, p_src_ip_addr,
                                             p_dest_ip_addr, read_flag, &lcl_dest_id,
                                             p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            case SIG_SOC_TYPE_IPV4 :
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting raw ipv4 server" ) );
                    sig_setup_raw_ipv4_server( self_type, p_s_open->p_src_ip_addr,
                                               p_s_open->p_dest_ip_addr, read_flag,
                                               &lcl_dest_id, p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            case SIG_SOC_TYPE_IPV6:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting raw ipv6 server" ) );

                    sig_setup_raw_ipv6_server( self_type, p_s_open->p_src_ip_addr,
                                               p_s_open->p_dest_ip_addr, read_flag,
                                               &lcl_dest_id,
                                               p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            case SIG_SOC_TYPE_ICMPV4:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting raw ipv4 server" ) );

                    sig_setup_icmp_ipv4_server( self_type, p_s_open->p_src_ip_addr,
                                                p_s_open->p_dest_ip_addr, read_flag,
                                                &lcl_dest_id,
                                                p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            case SIG_SOC_TYPE_ICMPV6:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setting raw ipv6 server" ) );

                    sig_setup_icmp_ipv6_server( self_type, p_s_open->p_src_ip_addr,
                                                p_s_open->p_dest_ip_addr, read_flag,
                                                &lcl_dest_id, p_read_callback_fn, &lcl_ecode );

                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            default :
                {
                    /* error case */
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::invalid sock type" ) );
                    p_s_open->dest_id = SOC_MGR_INVALID_HANDLE;
                    p_s_open->ecode = ESIG_IPC_INVALID_PARAM;
                }
        }
    }

    else if( SIG_SOC_MODE_CLIENT == sock_mode )
    {
        SIG_DEBUG_PRINT( ( "\nsig_shell_open::set up client" ) );

        switch( sock_type )
        {
            case SIG_SOC_TYPE_TCP:
                {
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::setup tcp client" ) );

                    sig_setup_tcp_client( self_type, p_src_ip_addr,
                                          p_dest_ip_addr, &lcl_dest_id,
                                          p_read_callback_fn, &lcl_ecode );
                    /* set out put paramters */
                    p_s_open->dest_id = lcl_dest_id;
                    p_s_open->ecode = lcl_ecode;
                    break;
                }

            case SIG_SOC_TYPE_UDP:
                {
                    /* error case */
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::UDP cannot be client" ) );

                    p_s_open->dest_id = SOC_MGR_INVALID_HANDLE;
                    p_s_open->ecode = ESIG_IPC_INVALID_PARAM;
                    break;
                }

            default:
                {
                    /* error case */
                    SIG_DEBUG_PRINT( ( "\nsig_shell_open::invalid sock type" ) );
                    p_s_open->dest_id = SOC_MGR_INVALID_HANDLE;
                    p_s_open->ecode = ESIG_IPC_INVALID_PARAM;
                }
        }
    }

    return SIGTRAN_NULL;
}

/******************************************************************************
**  FUNCTION :    sig_setup_tcp_server
**
**  DESCRIPTION:  This function opens a tcp server
**
**  RETURN VALUE:
**                on SUCCESS
**                Function returns a handler by which stack identify this
**                Server for further communication
**                on Failure
**                retrun 0
******************************************************************************/
sigtran_return_t sig_setup_tcp_server(
    sig_stack_type_et   stack_type,
    sig_ip_addr_st      *p_src_addr,
    sig_ip_addr_st      *p_dest_addr,
    sigtran_U16bit      *p_dest_id,
    sig_read_cbk_func_ptr_t  p_cbk_fn,
    sigtran_error_t     *p_ecode )

{
    sigtran_U16bit srv_port, handle;
    /* CSR 69326 fix starts*/
    sigtran_U32bit srv_ip[4];
    /* CSR 69326 fix ends*/
    sigtran_return_t ret_val = SIGTRAN_FAILURE;

    *p_dest_id = SOC_MGR_INVALID_HANDLE;

    SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_server::Set up TCP server" ) );

    /* SPR 21107 starts*/
    if( p_src_addr->ip_type == SIG_TRANS_ADDR_IPV4 )
    {
        /* get the ip and port where server will be open */
        /* CSR 69326 fix starts*/
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, &srv_ip[0],
                                                     &srv_port, SIGTRAN_NULL,
                                                     SIGTRAN_NULL, p_ecode );
        /* CSR 69326 fix ends*/
    }

    /* SPR 21107 ends */
    if( ret_val )
    {
        /* CSR 69326 fix starts*/

        handle = soc_mgr_add_tcp_server(
                     &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                     srv_ip[0],
                     srv_port,
                     p_src_addr,
                     p_dest_addr,
                     p_cbk_fn );
        /* CSR 69326 fix ends*/


        if( SOC_MGR_INVALID_HANDLE == handle )
        {
            SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_server::TCP server setup"\
                               "failed no valid handle left" ) );

            *p_dest_id = SOC_MGR_INVALID_HANDLE;
            SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_NO_HANDLE_AVL );
            return SIGTRAN_FAILURE;
        }

        else
        {
            SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_server::TCP server setup"\
                               " at port = %d ,handle alloted = %d,", srv_port, handle ) );

            *p_dest_id = handle;
            return SIGTRAN_SUCCESS;
        }
    }

    else
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_server::Invalid IPC info" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_INVALID_PARAM );
        return SIGTRAN_FAILURE;
    }
}
/******************************************************************************
**  FUNCTION :    sig_setup_udp_server
**
**  DESCRIPTION:  Function set up a UDP server on given port
**
**  RETURN VALUE:
**                on SUCCESS
**                Function returns a handler by which stack identify this
**                Server for further communication
**                on Failure
**                retrun 0
******************************************************************************/
sigtran_return_t sig_setup_udp_server(
    sig_stack_type_et   stack_type,
    sig_ip_addr_st      *p_src_addr,
    sig_ip_addr_st      *p_dest_addr,
    sigtran_boolean_t   read_flag,
    sigtran_boolean_t   b_def_udp_wrt_port,
    sigtran_U16bit      *p_dest_id,
    sig_read_cbk_func_ptr_t  p_cbk_fn,
    sigtran_error_t     *p_ecode )

{
    sigtran_U16bit srv_port = SOC_INVALID_PORT;
    sigtran_U16bit handle;
    /* CSR 69326 fix starts*/
    sigtran_U32bit srv_ip[4] = {};
    /* CSR 69326 fix ends*/
    sigtran_return_t ret_val = SIGTRAN_SUCCESS;

    if( SIGTRAN_NULL != p_src_addr )
    {
        /* get the ip and port where server will be open */
        /* CSR 69326 fix starts*/
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, &srv_ip[0],
                                                     &srv_port, SIGTRAN_NULL,
                                                     SIGTRAN_NULL, p_ecode );
    }

    /* CSR 69326 fix ends*/

    if( ret_val )
    {
        /* CSR 69326 fix starts*/
        handle = soc_mgr_add_udp_server(
                     &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                     srv_ip[0],
                     srv_port,
                     p_src_addr,
                     p_dest_addr,
                     read_flag,
                     b_def_udp_wrt_port,
                     p_cbk_fn );

        /* CSR 69326 fix ends*/

        if( SOC_MGR_INVALID_HANDLE == handle )
        {
            SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::UDP server setup"\
                               "failed no valid handle left" ) );

            SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_NO_HANDLE_AVL );
            return SIGTRAN_FAILURE;
        }

        else
        {
            *p_dest_id = handle;
            return SIGTRAN_SUCCESS;
        }
    }

    else
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_INVALID_PARAM );
        return SIGTRAN_FAILURE;
    }

}

/******************************************************************************
**  FUNCTION :    sig_setup_tcp_client
**
**  DESCRIPTION:  This function establish a client connection at given ip
**                and port
**
**  RETURN VALUE:
**                on SUCCESS
**                Function returns a handler by which stack identify this
**                Server for further communication
**                on Failure
**                retrun 0
******************************************************************************/
sigtran_return_t sig_setup_tcp_client(
    sig_stack_type_et   stack_type,
    sig_ip_addr_st      *p_src_addr,
    sig_ip_addr_st      *p_dest_addr,
    sigtran_U16bit      *p_dest_id,
    sig_read_cbk_func_ptr_t  p_cbk_fn,
    sigtran_error_t     *p_ecode )

{
    /* CSR 69326 fix starts*/
    sigtran_U16bit self_port = 0, dest_port = 0, handle = 0;
    sigtran_U32bit self_ip[4] = {} , dest_ip[4] = {};
    /* CSR 69326 fix ends*/

    sigtran_return_t ret_val = SIGTRAN_FAILURE;

    /* SPR 21107 starts */
    if( p_src_addr->ip_type == SIG_TRANS_ADDR_IPV4 )
    {
        /* get the ip and port where server will be open */
        /* CSR 69326 fix starts*/
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, &self_ip[0],
                                                     &self_port, SIGTRAN_NULL,
                                                     SIGTRAN_NULL, p_ecode );
        /* CSR 69326 fix ends*/

    }

    /* SPR 21107 ends */
    if( !( ret_val ) )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_client::Invalid IPC info" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_INVALID_PARAM );
        return SIGTRAN_FAILURE;
    }

    else
    {
        /*SPR 21141 changes starts*/
        if( p_dest_addr->ip_type == SIG_TRANS_ADDR_IPV4 )
        {
            /* CSR 69326 fix starts*/
            ret_val = sig_util_ext_ip_port_frm_ipc_info( p_dest_addr, &dest_ip[0],
                                                         &dest_port, SIGTRAN_NULL,
                                                         SIGTRAN_NULL, p_ecode );
            /* CSR 69326 fix ends*/
        }

        /*SPR 21141 changes ends*/
        if( SIGTRAN_FAILURE == ret_val )
        {
            SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_client::Invalid IPC info" ) );
            SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_INVALID_PARAM );
            return SIGTRAN_FAILURE;
        }

        else
        {
            /*CSR 69326 fix starts*/
            handle = soc_mgr_add_tcp_client(
                         &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                         self_ip[0],
                         self_port,
                         dest_ip[0],
                         dest_port,
                         p_src_addr,
                         p_dest_addr,
                         p_cbk_fn
                     );
            /*CSR 69326 fix ends*/

            if( SOC_MGR_INVALID_HANDLE == handle )
            {
                SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_client::TCP client setup "\
                                   " failed no valid handle left" ) );
                SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_NO_HANDLE_AVL );
                return SIGTRAN_FAILURE;
            }

            else
            {
                SIG_DEBUG_PRINT( ( "\nsig_setup_tcp_client::TCP cleint setup"\
                                   " handle alloted %d,", handle ) );

                *p_dest_id = handle;
                return SIGTRAN_SUCCESS;
            }

        }
    }
}
#ifdef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  FUNCTION :    sig_set_q_module
**
**  DESCRIPTION:  Function set Qmodule in common database
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t    sig_set_q_module( sig_stack_type_et stack_type,
                                      sig_dest_type_et  dest_type,
                                      sigtran_U16bit    dest_id,
                                      sig_q_module_t    q_module,
                                      sigtran_error_t   *p_ecode )
{
    /* do validations first */
    SIG_ASSERT( ( dest_id < SOC_MGR_MAX_CONNECTIONS ) &&
                ( dest_type < SIG_MAX_DEST_TYPE ) &&
                ( stack_type != SIG_INVALID_STACK ) &&
                ( stack_type < SIG_MAX_NUM_STACK ) );

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;

    if( dest_id != SOC_MGR_INVALID_HANDLE )
    {
        /* this q_module is for external destination
         for given stack type and dest_id set Qmodule */

        SIG_DEBUG_PRINT( ( "\nsig_set_q_module::Q module set for stack type=%d"\
                           " and handle %d", stack_type, dest_id ) );

        g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[dest_id].q_module =
            q_module;
        /* set destination type here */
        g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[dest_id].dest_type =
            dest_type;


        return SIGTRAN_SUCCESS;
    }

    else
    {
        /* this is case of
         * 1.Q module corresponding to module's own Qmodule
         *   dest_type = INVALID and dest_id =invalid
         * 2. Q module for some internal module
         *   dest_type = TL/APP/SM
         *    dest_id = invalid
         */
        SIG_DEBUG_PRINT( ( "\nsig_set_q_module::Q module set for stack type=%d"\
                           " and dest_type  %d", stack_type, dest_type ) );
        g_ipc_mgr_data[stack_type].g_map_for_internal_modules[dest_type] =
            q_module;
        return SIGTRAN_SUCCESS;
    }
}

/******************************************************************************
**  FUNCTION :    sig_get_q_module
**
**  DESCRIPTION:  Function rteurns Qmodule Corresponding to the stack name
**                and handle given
**
**  RETURN VALUE:
**                Function returns succ if a valid QMOUDLE is set
**                Function returns fail if get is done before set
******************************************************************************/
sigtran_return_t    sig_get_q_module( sig_stack_type_et stack_type,
                                      sig_dest_type_et  dest_type,
                                      sigtran_U16bit    dest_id,
                                      sig_q_module_t    *p_q_module,
                                      sigtran_error_t   *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_q_module ) );

    SIG_ASSERT( ( dest_id < SOC_MGR_MAX_CONNECTIONS ) &&
                ( dest_type < SIG_MAX_DEST_TYPE ) &&
                ( stack_type != SIG_INVALID_STACK ) &&
                ( stack_type < SIG_MAX_NUM_STACK ) );

    if( dest_id != SOC_MGR_INVALID_HANDLE )
    {
        /* this q_module is for external destination */
        /*for given stack type and dest_id get Qmodule*/

        SIG_DEBUG_PRINT( ( "\nsig_get_q_module::Q module get for stack type = %d"
                           " and dest_type  %d", stack_type, dest_type ) );
        *p_q_module =
            g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[dest_id].q_module;
    }

    else
    {
        /* this is case of
         * 1. modules own Qmodule dest_id = INVALID ,dest_type =INVALID
         * 2. Q module for some internal module dest_id = INVALID,
         *    dest_type = APP/SM/TL*/
        SIG_DEBUG_PRINT( ( "\nsig_get_q_module::Q module get for stack type = %d"
                           "and dest_type  %d", stack_type, dest_type ) );
        *p_q_module =
            g_ipc_mgr_data[stack_type].g_map_for_internal_modules[dest_type];
    }

    if( *p_q_module == SOC_INV_Q_MODULE )
    {
        SIG_DEBUG_PRINT( ( "\nsig_get_q_module:: QMODULE not Set" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_QMODULE_NOT_SET );
        return SIGTRAN_FAILURE;
    }

    else
    {
        return SIGTRAN_SUCCESS;
    }
}
#endif
/******************************************************************************
**  FUNCTION :    sig_set_in_map
**
**  DESCRIPTION:  Function set value in the key map to soc manager handle
**
**  RETURN VALUE:
**                Function returns succ if a valid QMOUDLE is set
**                Function returns fail if get is done before set
******************************************************************************/
sigtran_return_t sig_set_in_map(
    sig_stack_type_et stack_type,
    /* map key is any key say logical user id for m3ua */
    sigtran_U32bit    map_key,
    sigtran_U16bit    dest_id,
    sigtran_error_t   *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );

    if( map_key >= SIG_MAX_VALID_MAP_KEY )
    {
        SIG_DEBUG_PRINT( ( "\nsig_set_in_map:: Invalid map key parameter" ) );
        *p_ecode = ESIG_IPC_INVALID_MAY_KEY;
        return SIGTRAN_FAILURE;
    }

    if( dest_id == SOC_MGR_INVALID_HANDLE || dest_id > SOC_MGR_MAX_CONNECTIONS )
    {
        SIG_DEBUG_PRINT( ( "\nsig_set_in_map:: Invalid dest id parameter" ) );
        *p_ecode = ESIG_IPC_INVALID_DEST_ID;
        return SIGTRAN_FAILURE;
    }

    g_ipc_mgr_data[stack_type].g_map_key_to_dest_id[map_key] = dest_id;
    return SIGTRAN_SUCCESS;

}
/******************************************************************************
**  FUNCTION :    sig_get_from_map
**
**  DESCRIPTION:  Function to get value from the key map for getting key for
**                given handle
**
**  RETURN VALUE:
**                Function returns succ if a valid QMOUDLE is set
**                Function returns fail if get is done before set
******************************************************************************/
sigtran_return_t sig_get_from_map(
    sig_stack_type_et stack_type,
    /* map key is any key say logical user id for m3ua */
    sigtran_U32bit    map_key,
    sigtran_U16bit    *p_dest_id,
    sigtran_error_t   *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    *p_dest_id = SOC_MGR_INVALID_HANDLE;

    if( map_key >= SIG_MAX_VALID_MAP_KEY )
    {
        SIG_DEBUG_PRINT( ( "\nsig_get_from_map:: Invalid mep key parameter" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_INVALID_MAY_KEY );
        return SIGTRAN_FAILURE;
    }

    *p_dest_id = g_ipc_mgr_data[stack_type].g_map_key_to_dest_id[map_key];

    if( *p_dest_id == SOC_MGR_INVALID_HANDLE )
    {
        SIG_DEBUG_PRINT( ( "\nsig_get_from_map:: Dest id not set" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_DEST_ID_NOT_SET );
        return SIGTRAN_FAILURE;
    }

    else
    {
        SIG_DEBUG_PRINT( ( "\nsig_get_from_map::"
                           "returning Dest id = %d", *p_dest_id ) );
        return SIGTRAN_SUCCESS;
    }
}
/******************************************************************************
**  FUNCTION :    sig_get_dest_id
**
**  DESCRIPTION:  this function returns dest id for given sock type,ip and port
**
**  RETURN VALUE:  SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_get_dest_id(
    sig_stack_type_et  stack_type,
    sig_soc_type_et    sock_type,
    sigtran_U16bit     mesg_handle_id,
    sigtran_U32bit     ip_addr,
    sigtran_U16bit     port,
    sigtran_U16bit     *p_dest_id,
    sigtran_boolean_t  *p_b_is_new,
    sigtran_error_t    *p_ecode )
{
    sigtran_U16bit dest_id = SOC_MGR_INVALID_HANDLE;
    soc_mgr_st     *p_soc_mgr;
    sigtran_boolean_t b_handle_found = SIGTRAN_FALSE;

    SIG_ASSERT( ( SIGTRAN_NULL != p_b_is_new ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    SIG_DEBUG_PRINT( ( "\nsig_get_dest_id:: enterned function" ) );
    *p_b_is_new = SIGTRAN_FALSE;

    p_soc_mgr = &( g_ipc_mgr_data[stack_type].ipc_conn_info );

    if( SIG_SOC_TYPE_TCP == sock_type )
    {
        /* socket type is tcp handle must have been already created
         * at the time of accept connection or making client connection
         */
        for( dest_id = 0; dest_id < SOC_MGR_MAX_CONNECTIONS; dest_id++ )
        {
            if( p_soc_mgr->conn_info[dest_id].sock_type == SIG_SOC_TYPE_TCP )
            {
                /* compare the remote ip and port */
                if( ( p_soc_mgr->conn_info[dest_id].rem_ip == ip_addr ) &&
                        ( p_soc_mgr->conn_info[dest_id].rem_port == port ) )
                {
                    SIG_DEBUG_PRINT( ( "\nsig_get_dest_id:: destination id =%d "
                                       "found for tcp sock type", dest_id ) );
                    b_handle_found = SIGTRAN_TRUE;
                    *p_dest_id = dest_id;
                    break;
                }
            }
        }

        if( SIGTRAN_FALSE == b_handle_found )
        {
            SIG_DEBUG_PRINT( ( "\nsig_get_dest_id:: destination id does not exist"
                               "for tcp sock type" ) );
            *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
            return SIGTRAN_FAILURE;
        }

        else
        {
            return SIGTRAN_SUCCESS;
        }
    }

    else if( SIG_SOC_TYPE_UDP == sock_type )
    {
        /* for UDP first check if already a handle exist for the
         * given ip and port.else create a new handle and return back handle
         */
        for( dest_id = 1; dest_id < SOC_MGR_MAX_CONNECTIONS; dest_id++ )
        {
            if( p_soc_mgr->conn_info[dest_id].sock_type == SIG_SOC_TYPE_UDP )
            {
                /* compare the remote ip and port */
                if( ( p_soc_mgr->conn_info[dest_id].rem_ip == ip_addr ) &&
                        ( p_soc_mgr->conn_info[dest_id].rem_port == port ) )
                {
                    SIG_DEBUG_PRINT( ( "\nsig_get_dest_id:: destination id =%d "
                                       "found for udp sock type", dest_id ) );
                    *p_dest_id = dest_id;
                    b_handle_found = SIGTRAN_TRUE;
                    break;
                }

            }
        }

        if( SIGTRAN_FALSE == b_handle_found )
        {
            /* create a new dest id */
            dest_id = soc_mgr_add_udp_dest( p_soc_mgr, ip_addr, port,
                                            mesg_handle_id );
            *p_dest_id = dest_id;

            if( dest_id == SOC_MGR_INVALID_HANDLE )
            {
                SIG_DEBUG_PRINT( ( "\nsig_get_dest_id::unable to create  new"
                                   "destination id for udp sock type" ) );
                *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
                return SIGTRAN_FAILURE;
            }

            else
            {
                SIG_DEBUG_PRINT( ( "\nsig_get_dest_id:: new destination id =%d "
                                   "created for udp sock type", dest_id ) );
                *p_b_is_new = SIGTRAN_TRUE;
                return SIGTRAN_SUCCESS;
            }
        }

        else
        {
            /* destination id already exist */
            return SIGTRAN_SUCCESS;
        }
    }

    else /* invalid case */
    {
        SIG_DEBUG_PRINT( ( "\nsig_get_dest_id::Invalid paramter" ) );
        *p_ecode = ESIG_PARM_INVALID;
        return SIGTRAN_FAILURE;
    }
}
/******************************************************************************
**  FUNCTION :    sig_send
**
**  DESCRIPTION:   This is top level function called by sig stacks/clients
**                 to send data.
**
**  RETURNS        SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_send( sig_stack_type_et self_type,
                           sig_dest_type_et  dest_type,
                           sigtran_U16bit    dest_id,
                           sigtran_pvoid_t   data_buf,
                           sigtran_U32bit    data_len,
                           sigtran_error_t   *p_ecode )
{

#ifdef SIG_COMMON_WITH_CSPL
    sigtran_return_t   ret_val = SIGTRAN_FAILURE;
    sig_q_module_t     src_q_module, dest_q_module;
    sigtran_error_t    error_code;

    /* len is not used directly in case of cspl
     * to remove unused parameter warning just
     * use it */
    data_len = data_len;


    SIG_DEBUG_PRINT( ( "\nsig_send::entered function" ) );
    /* get src q module */
    ret_val = sig_get_q_module( self_type,
                                SIG_DEST_TYPE_INVALID,
                                SOC_MGR_INVALID_HANDLE,
                                &src_q_module,
                                p_ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_send::unable to get"
                           "Q module for source module" ) );

        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_QMODULE_NOT_SET );
        return ret_val;
    }

    /* get dest q module */
    ret_val = sig_get_q_module( self_type,
                                dest_type,
                                dest_id,
                                &dest_q_module,
                                p_ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_send::unable to get Q module for dest module" ) );
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_QMODULE_NOT_SET );
        return ret_val;
    }

    sig_qv_deliver( src_q_module, dest_q_module, 0, data_buf, &error_code );

    if( QVERROR_NONE != error_code )
    {
        SIGTRAN_SET_ECODE( p_ecode, error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;

#else
    dest_type = dest_type;
    p_ecode = p_ecode;
    return sig_ipc_mgr_send_api( self_type,
                                 dest_id,
                                 data_buf,
                                 data_len,
                                 SIGTRAN_FALSE );

#endif
}
#ifdef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  FUNCTION :    sigtran_send_to_ext_module
**
**  DESCRIPTION:   This function is registered with CSPL for sending
**                 data on external ipc .
**
**  RETURNS        sigtran_void_t
******************************************************************************/
sigtran_void_t sig_send_to_ext_module(
    sigtran_void_t         *s,
    const sigtran_void_t   *p_message,
    sig_q_module_t          from,
    sig_q_module_t          to,
    sigtran_pvoid_t         dest,
    sigtran_U32bit          size )
{
    sig_iovec_ipc_buffer_st     *p_ipc_buffer;
    sig_stack_type_et           stack_type;
    sig_dest_type_et            dest_type;
    sigtran_U16bit              handle;
    sigtran_UL32bit             service_name;
    sigtran_error_t             ecode;
    sigtran_U32bit              mesg_size;
    soc_mgr_st                  *p_soc_mgr;

    /* these parameters are not used.Remove warning */
    s = s;
    from = from;
    dest = dest;
    size = size;

    SIG_DEBUG_PRINT( ( "\nsig_send_to_ext_module::Entered function" ) );

    /* this function does not return fiailure even if the message
     * buffer does nopt contain io vector
     */

    /* get service name from the q module */
    service_name = sig_get_service_name( to );
    /* get the ipc info from the service name which has
     * stack type dest type and handle id
     */
    sig_get_ipc_info_frm_service_name( service_name, &stack_type,
                                       &dest_type, &handle, &ecode );

    /*KW fix start*/
    if( stack_type >= SIG_MAX_NUM_STACK )
    {
        SIG_DEBUG_PRINT( ( "\nsig_send_to_ext_module::stack type is greater than SIG_MAX_NUM_STACK" ) );
        return;
    }

    /*KW fix end*/
    if( ( ( SIG_SCTP_STACK == stack_type ) || ( SIG_SCTP_RECV_STACK == stack_type ) )
            && ( SOC_MGR_INVALID_HANDLE == handle ) )
    {
        /*
         * This is the case of SCTP destinations where handle is not
         * set for each destination .it is set only for server sockets
         * of type NW_UDP,IPV4,IPV6,ICWP etc etc.
         */

        p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )p_message;
        handle = p_ipc_buffer->dest_id;
        mesg_size = p_ipc_buffer->buffer_size;

        sig_ipc_mgr_send_api( stack_type, handle,
                              p_message, mesg_size, SIGTRAN_FALSE );
    }

    else
    {
        /* For the length of the API.  */
        /* get the pointer to soc manager for the given stack type */
        p_soc_mgr =
            &( g_ipc_mgr_data[stack_type].ipc_conn_info );

        switch( dest_type )
        {
            case SIG_DEST_TYPE_APP:
                p_soc_mgr->p_su_cbk_fn( p_message, &mesg_size );
                SIG_DEBUG_PRINT( ( "\nsig_send_to_ext_module::"
                                   "get message lenght for SU" ) );
                break;

            case SIG_DEST_TYPE_SM:
                p_soc_mgr->p_sm_cbk_fn( p_message, &mesg_size );
                SIG_DEBUG_PRINT( ( "\nsig_send_to_ext_module::"
                                   "get message lenght for SM" ) );
                break;

            case SIG_DEST_TYPE_TL:
                SIG_DEBUG_PRINT( ( "\nsig_send_to_ext_module::"
                                   "get message lenght for TL" ) );
                p_soc_mgr->p_tl_cbk_fn( p_message, &mesg_size );
                break;

            default:
                SIG_DEBUG_PRINT( ( "\nsig_send_to_ext_module::Failure"
                                   "Unable to get message lenght" ) );
                return;
        }

        /* a valid handle is set whenever communication is
         * b/w stack and client */
        sig_ipc_mgr_send_api( stack_type,
                              handle,
                              p_message,
                              mesg_size,
                              SIGTRAN_FALSE );
    }
}
#endif
/******************************************************************************
**  FUNCTION :    sig_ipc_mgr_send_api
**
**  DESCRIPTION:   This function calls approperiate send fucntion for
**                 sending data on to n/w based on dest type.
**
**  RETURNS        SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_ipc_mgr_send_api( sig_stack_type_et stack_type,
                                       sigtran_U16bit    handle,
                                       const sigtran_void_t *p_message,
                                       sigtran_U32bit    size,
                                       sigtran_boolean_t b_is_iovec )
{

    /* get the handle type first */
    sig_sock_mode_et            lcl_sock_mode;
    sig_soc_type_et             sock_type;
    sigtran_U32bit              i;
    sig_sock_fd_t               fd;
    sigtran_S32bit              n_sent;
    sig_iovec_ipc_buffer_st     *p_ipc_buffer;
    sigtran_buffer_t            p_mesg_len;
    sigtran_iovec_st            mesg_iovec[SIG_MAX_IOVEC_SIZE];
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /*KW fix start*/
    sigtran_U8bit               arr_mesg_len[4] = "";
    /*KW fix end*/
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    /*KW fix start*/
    sigtran_8bit                arr_mesg_len[4] = "";
    /*KW fix end*/
#endif
    sigtran_U32bit              length = 0;
    /* SPR 21107 changes */
    sigtran_memset( arr_mesg_len, 0, sizeof( arr_mesg_len ) );
    /* SPR 21107 changes */
    SIG_DEBUG_PRINT( ( "\nsig_ipc_mgr_send_api::Entered function" ) );
#ifndef SIG_COMMON_WITH_CSPL

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        handle = ( ( sig_iovec_ipc_buffer_st * )p_message )->dest_id;
    }

#endif

    /*KW fix start*/
    if( handle >= SOC_MGR_MAX_CONNECTIONS )
    {
        SIG_DEBUG_PRINT( ( "\nsig_ipc_mgr_send_api::"\
                           "INVALID CONNECTION INFO" ) );
        return SIGTRAN_FAILURE;
    }

    /*KW fix end*/
    lcl_sock_mode =
        g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[handle].sock_mode;
    sock_type =
        g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[handle].sock_type;
    fd = g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[handle].fd;

    /*  SPR 21127 changes start */
    COMMON_UNUSED_VARIABLE( lcl_sock_mode );

    /*  SPR 21127 changes end */
    switch( sock_type )
    {
        case SIG_SOC_TYPE_UDP:
            {
                SIG_DEBUG_PRINT( ( "\nsig_ipc_mgr_send_api::"\
                                   "sending data on UDP" ) );


                n_sent = soc_mgr_send_on_udp( fd,
                                              p_message,
                                              size,
                                              g_ipc_mgr_data[stack_type].ipc_conn_info.\
                                              conn_info[handle].rem_ip,
                                              g_ipc_mgr_data[stack_type].ipc_conn_info.\
                                              conn_info[handle].rem_port,
                                              b_is_iovec );

                if( n_sent < 0 )
                {
                    SIG_DEBUG_PRINT( ( "\nsig_ipc_mgr_send_api::"
                                       "Failure in UDP send" ) );
                    return SIGTRAN_FAILURE;
                }

                else
                {
                    return SIGTRAN_SUCCESS;
                }
            }

        case SIG_SOC_TYPE_TCP:
            {
                SIG_DEBUG_PRINT( ( "\nsig_ipc_mgr_send_api::sending data on TCP" ) );

                if( SIGTRAN_FALSE == b_is_iovec )
                {
                    p_mesg_len = ( sigtran_buffer_t )arr_mesg_len;
                    sig_util_set_4bytes( &p_mesg_len, size );

                    /* Changes for SPR-21022 Starts here */
#ifdef VXWORKS_6_9_SUPPORT
                    mesg_iovec[0].iov_base = ( sigtran_8bit * )arr_mesg_len;
#else
                    mesg_iovec[0].iov_base = arr_mesg_len;
#endif
                    /* Changes for SPR-21022 Ends here */
                    mesg_iovec[0].iov_len  = 4;

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
                    mesg_iovec[1].iov_base = ( sigtran_8bit * ) p_message;

#else
                    /* CSR#1-4825810 Changes done for warning removal activity
                     * for compilation with SUN cc */
                    mesg_iovec[1].iov_base = ( sigtran_8bit * ) p_message;
#endif
                    mesg_iovec[1].iov_len  = size;

                    size = 2;
                }

                else
                {
                    for( i = 0; i < size; i++ )
                    {
                        /*
                         * Get the length of the message.
                         */
                        length += ( ( sigtran_iovec_st * )p_message )[i].iov_len;

                        /*
                         * Move the iovectors to the next index in the
                         * mesg_iovec array.
                         */
                        mesg_iovec[i + 1].iov_base =
                            ( ( sigtran_iovec_st * )p_message )[i].iov_base;
                        mesg_iovec[i + 1].iov_len  =
                            ( ( sigtran_iovec_st * )p_message )[i].iov_len;
                    }

                    sig_util_set_4bytes( ( sigtran_buffer_t * )&arr_mesg_len, size );
                    /* Changes for SPR-21022 Starts here */
#ifdef VXWORKS_6_9_SUPPORT
                    mesg_iovec[0].iov_base = ( sigtran_8bit * )arr_mesg_len;
#else
                    mesg_iovec[0].iov_base = arr_mesg_len;
#endif

                    /* Changes for SPR-21022 Ends here */
                    mesg_iovec[0].iov_len  = 4;

                    /*
                     * Increase the size variable to store the length
                     * in the first index.
                     */
                    size = size + 1;
                }

                sigtran_mutex_lock( &( g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[handle].fd_mutex ) );
                n_sent = soc_mgr_nw_write( fd, mesg_iovec, size, SIGTRAN_TRUE );

                sigtran_mutex_unlock( &( g_ipc_mgr_data[stack_type].ipc_conn_info.conn_info[handle].fd_mutex ) );

                if( n_sent < 0 )
                {
                    /* connection broken */
                    /* clear the fd from FD_SET */
                    SIG_DEBUG_PRINT( ( "\nsig_ipc_mgr_send_api::"
                                       "Failure in TCP send" ) );
                    sig_clr_fd_from_glb_fdset( fd );
                    /* close the communication channel */
                    sig_close_comm_channel( stack_type, handle );
                    return SIGTRAN_FAILURE;
                }

                else
                {
                    return SIGTRAN_SUCCESS;
                }

            }

        case SIG_SOC_TYPE_UDP_NW:
        case SIG_SOC_TYPE_IPV4:
        case SIG_SOC_TYPE_ICMPV4:
            {
                p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )p_message;

                n_sent = soc_mgr_send_on_udp( fd,
                                              p_ipc_buffer->p_buffer,
                                              p_ipc_buffer->buffer_size,
                                              p_ipc_buffer->ip_addr.ip_addr_ut.addr_v4.addr,
                                              p_ipc_buffer->ip_addr.ip_addr_ut.addr_v4.port,
                                              p_ipc_buffer->b_is_iovec );

                if( n_sent < 0 )
                {
                    /* connection broken*/
                    return SIGTRAN_FAILURE;
                }

                else
                {
                    return SIGTRAN_SUCCESS;
                }
            }
            break;

#ifdef SIGTRAN_INET6_SUPPORT

        case SIG_SOC_TYPE_IPV6:
            {
                p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )p_message;

                n_sent = soc_mgr_send_on_udp_ipv6_ext(
                             fd,
                             p_ipc_buffer->p_buffer,
                             p_ipc_buffer->buffer_size,
                             p_ipc_buffer->send_flags,
                             p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32,
                             p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.port,
                             p_ipc_buffer->b_is_iovec );

                if( n_sent < 0 )
                {
                    /* connection broken*/
                    return SIGTRAN_FAILURE;
                }

                else
                {
                    return SIGTRAN_SUCCESS;
                }
            }
            break;

        case SIG_SOC_TYPE_ICMPV6:
            {
                p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )p_message;

                n_sent = soc_mgr_send_on_udp_ipv6(
                             fd,
                             p_ipc_buffer->p_buffer,
                             p_ipc_buffer->buffer_size,
                             p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32,
                             p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.port,
                             p_ipc_buffer->b_is_iovec );

                if( n_sent < 0 )
                {
                    /* connection broken*/
                    return SIGTRAN_FAILURE;
                }

                else
                {
                    return SIGTRAN_SUCCESS;
                }

            }

#endif

        default:
            {
                return SIGTRAN_FAILURE;
            }
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return SIGTRAN_SUCCESS;
#endif
}
/******************************************************************************
**  FUNCTION :    sig_recv_from_ext_module
**
**  DESCRIPTION:   This function is registered with CSPL for recving
**                 data from external ipc .
**
**  RETURNS         sigtran_pvoid_t
******************************************************************************/
sigtran_pvoid_t sig_recv_from_ext_module(
    sigtran_pvoid_t s,
    sig_q_module_t *p_from,
    sig_q_module_t *p_to,
    sigtran_S8bit  *p_priority )
{
    sigtran_U8bit stack_type = SIG_INVALID_STACK;

    /* these parameters are not used.Remove warning */
    s = s;
    p_priority = p_priority;

    /* check if any of the sockets has got modified in select */
    if( g_num_fd_mod != 0 )
    {
        /* for all the satcks registered check if
         * any data is pending */
        for( stack_type = stack_type + 1; stack_type < SIG_MAX_NUM_STACK ;
                stack_type++ )
        {
            if( SIGTRAN_TRUE == g_ipc_mgr_data[stack_type].stack_reg )
            {
                soc_mgr_chk_activity( &g_ipc_mgr_data[stack_type].ipc_conn_info,
                                      stack_type,
                                      p_from,
                                      p_to );
            }
        }

#ifdef SIGTRAN_SINGLE_THREAD_ARICENT_INTERNAL
#ifdef SIGTRAN_SINGLE_THREAD

        if( sig_is_fd_set_in_glb_fdset( 0 ) )
        {
            start_command_loop( "m3ua_es" );
        }

#endif
#endif
    }

#ifdef SIGTRAN_SINGLE_THREAD_ARICENT_INTERNAL
#ifdef SIGTRAN_SINGLE_THREAD

    if( flag == 0 )
    {
        sig_add_fd_to_glb_fdset( 0 );
        flag = 1;
    }

#endif
#endif
    return SIGTRAN_NULL;
}
/******************************************************************************
**  FUNCTION :    sig_clr_fd_from_glb_fdset
**
**  DESCRIPTION:   This function clear fd from the global fdset
**
**  RETURNS        SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_clr_fd_from_glb_fdset( sig_sock_fd_t fd )
{
    /*klock fix*/
    /*Rel 6.2.3 Connection Establishment Fix*/
#ifndef SIGTRAN_PORT_WINNT
    if( fd < FD_SETSIZE )
#endif
        /*Rel 6.2.3 Connection Establishment Fix*/
    {

        FD_CLR( fd, p_sig_gloabl_fdset );
    }

    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_add_fd_to_glb_fdset
**
**  DESCRIPTION:   This function set given fd in the global fdset
**
**  RETURNS:      SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_add_fd_to_glb_fdset( sig_sock_fd_t fd )
{
    /*klock fix*/
    /*Rel 6.2.3 Connection Establishment Fix*/
#ifndef SIGTRAN_PORT_WINNT
    if( fd < FD_SETSIZE )
#endif
        /*Rel 6.2.3 Connection Establishment Fix*/
    {
        FD_SET( fd, p_sig_gloabl_fdset );
    }

    if( fd > max_fd )
    {
        max_fd = fd;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_is_fd_set_in_glb_fdset
**
**  DESCRIPTION:   This function checks if the given fd is set in the global fdset
**                 fdset
**
**  RETURNS:       SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_is_fd_set_in_glb_fdset( sig_sock_fd_t fd )
{
    /*klock fix*/
    /*Rel 6.2.3 Connection Establishment Fix*/
#ifndef SIGTRAN_PORT_WINNT
    if( fd < FD_SETSIZE )
#endif
        /*Rel 6.2.3 Connection Establishment Fix*/
    {
        if( FD_ISSET( fd, p_sig_gloabl_fdset ) )
        {
            return SIGTRAN_SUCCESS;
        }
    }

    return SIGTRAN_FAILURE;

}


/******************************************************************************
**  FUNCTION :    sig_get_glb_fd_set
**
**  DESCRIPTION:   This function gets the global fd set
**
**  RETURNS:       SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_get_glb_fd_set( sig_fd_set *p_fd_set )
{
    *p_fd_set = *p_sig_gloabl_fdset;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_set_glb_fd_set
**
**  DESCRIPTION:   This function gets the global fd set
**
**  RETURNS:       SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_set_glb_fd_set( sig_fd_set *p_fd_set )
{
    p_sig_gloabl_fdset = p_fd_set;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_get_glb_mod_fdset
**
**  DESCRIPTION:   This function gets the modified fd set which we will get
**                 from select in timed wait
**
**  RETURNS:       SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_get_glb_mod_fdset( sig_fd_set **p_fd_set )
{
    *p_fd_set = &global_mod_fd_set;
    return SIGTRAN_SUCCESS;
}

/* SPR 20872: Changes for enabling single threaded support Starts*/
/******************************************************************************
**    Function Name     : sig_get_max_fd
**
**    Description       : Function returns maximum FD value in SIGTRAN
**
**    Returns           : sock_fd
**
******************************************************************************/
sig_sock_fd_t sig_get_max_fd( void )
{
    return max_fd;
}
/* SPR 20872: Changes for enabling single threaded support Ends*/

/******************************************************************************
**  FUNCTION :    sig_close_comm_channel
**
**  DESCRIPTION:   Function to close fd corresponding to given stack type
**                 and dest id.
**  RETURNS :      SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t  sig_close_comm_channel( sig_stack_type_et module_type,
                                          sigtran_U16bit dest_id )
{
    sig_shell_close_data_st s_close;

    s_close.self_type = module_type;
    s_close.dest_id = dest_id;

#ifdef SIG_COMMON_WITH_CSPL
    sig_qv_close( ( void * )&s_close );
#else
    sig_shell_close( ( void * )&s_close );
#endif

    soc_mgr_release_handle( &( g_ipc_mgr_data[module_type].ipc_conn_info ),
                            dest_id );

    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_shell_close
**
**  DESCRIPTION:   Function registeredwith cspl to close communication channel
**  RETURNS :      sigtran_void_t
******************************************************************************/
sigtran_void_t sig_shell_close( sigtran_pvoid_t p_s )
{
    sig_shell_close_data_st *s_close;

    s_close = ( sig_shell_close_data_st * )p_s;

    /* TBD put a check for invalid fd *//*KW fix start*/
    if( ( s_close->self_type < SIG_MAX_NUM_STACK ) && ( s_close->dest_id < SOC_MGR_MAX_CONNECTIONS ) ) /* KW Fix end */
    {
        sig_clr_fd_from_glb_fdset( g_ipc_mgr_data[s_close->self_type].ipc_conn_info.\
                                   conn_info[s_close->dest_id].fd );
        sig_os_close( g_ipc_mgr_data[s_close->self_type].ipc_conn_info.\
                      conn_info[s_close->dest_id].fd );
    }
}
/******************************************************************************
**  FUNCTION :    sig_accept_cli_conn
**
**  DESCRIPTION:   Function is used for accepting tcp client
**  RETURNS        SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_accept_cli_conn(
    sig_stack_type_et self_type,
    sig_sock_fd_t     sock_fd,
    sig_read_cbk_func_ptr_t  p_cbk_fn,
    sigtran_U16bit    *p_dest_id,
    sigtran_U32bit    *ip_addr,
    sigtran_U16bit    *port,
    sigtran_error_t   *p_ecode )
{
    sigtran_U16bit handle;
    sigtran_U32bit lcl_ip_addr;
    sigtran_U16bit lcl_port;

    handle = soc_mgr_aceept_tcp_conn(
                 &( g_ipc_mgr_data[self_type].ipc_conn_info ),
                 sock_fd,
                 p_cbk_fn,
                 &lcl_ip_addr,
                 &lcl_port );

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_NO_HANDLE_AVL );
        return SIGTRAN_FAILURE;
    }

    else
    {
        *p_dest_id = handle;
        *ip_addr = lcl_ip_addr;
        *port = lcl_port;
        return SIGTRAN_SUCCESS;
    }
}
#ifdef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  FUNCTION :    sig_timed_wait
**
**  DESCRIPTION:   Function polls on the global fds
**  RETURNS :      sigtran_void_t
******************************************************************************/
sigtran_void_t sig_timed_wait( sigtran_pvoid_t   s,
                               const  sig_q_time_st *q_time )
{
    sigtran_time_t timeout;
    sig_fd_set         read_fds;

    s = s;

    sig_get_glb_fd_set( &read_fds );

    g_num_fd_mod = 0;

    if( q_time )
    {
        timeout.tv_sec = q_time->s;
        timeout.tv_usec = q_time->us;
    }

    else
    {
        timeout.tv_sec = 0;
        timeout.tv_usec = sig_sel_timeval * 1000;
    }

    if( ( sig_sel_timeval == 0 ) && ( !q_time ) )
    {
        g_num_fd_mod = sig_os_select( max_fd + 1, &read_fds, 0, 0, NULL );
    }

    else
    {
        g_num_fd_mod = sig_os_select( max_fd + 1, &read_fds, 0, 0, &timeout );
    }

    if( g_num_fd_mod >= 0 )
    {
        if( g_num_fd_mod > 0 )
        {
            SIG_DEBUG_PRINT( ( "\nInside timed wait" ) );
            SIG_DEBUG_PRINT( ( "\nData received" ) );
        }

        global_mod_fd_set = read_fds;
    }

    else
    {
        /* error case */
    }

}
#endif
/******************************************************************************
**  FUNCTION :    sig_set_block_select
**
**  DESCRIPTION:  This function sets the block select
**
**  RETURNS :     void
******************************************************************************/
void sig_set_block_select( void )
{

    sig_sel_timeval = 0;
    return ;
}


/******************************************************************************
**  FUNCTION :    sig_ipc_select
**
**  DESCRIPTION:   Function polls on the global fds. This will be used for NON
**                 CSPL Porting.
**  RETURNS :      sigtran_S32bit
******************************************************************************/
sigtran_S32bit sig_ipc_select( sigtran_time_t    *p_timeout )
{
    sig_fd_set         read_fds;
    sigtran_S32bit num;

    sig_get_glb_fd_set( &read_fds );

    num = sig_os_select( max_fd + 1, &read_fds, 0, 0, p_timeout );
    global_mod_fd_set = read_fds;
    g_num_fd_mod = num;
    return num;
}

/******************************************************************************
**  FUNCTION :    sig_read
**
**  DESCRIPTION:  This function is top level function exposed for doing read
**                operations on the sockets
**
**  RETURNS        SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
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
    sigtran_error_t   *p_ecode
)
{
    sigtran_U32bit    ip_addr[4];
    /* SPR 6672 */
    sigtran_S32bit    num_read;
    /* SPR 6672 */
    sigtran_U32bit    flow_info;
    sigtran_U32bit    scope_id;
    sigtran_U16bit    port;

    SIGTRAN_SET_ECODE( p_ecode, ESIG_NO_ERROR );
    self_type = self_type;

    switch( sock_type )
    {
        case SIG_SOC_TYPE_UDP:
        case SIG_SOC_TYPE_UDP_NW:

            /* SPR 6672 */
        case SIG_SOC_TYPE_ICMPV4:

            /* SPR 6672 */
        case SIG_SOC_TYPE_IPV4 :
            {
                p_addr->ip_type = SIG_TRANS_ADDR_IPV4;

                num_read = soc_mgr_nw_read( sock_fd, max_read_size, ( sigtran_U8bit )read_flag,
                                            ip_addr, &port, p_pdata_buf, p_data_len_read );

                if( num_read > 0 )
                {
                    p_addr->ip_addr_ut.addr_v4.addr = ip_addr[0];
                    p_addr->ip_addr_ut.addr_v4.port = port;

                    return SIGTRAN_SUCCESS;
                }

                else
                {
                    SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_READ_FAIL );
                    return SIGTRAN_FAILURE;
                    /* error */
                }

                break;
            }

            /* CSR 1-5892901 Start*/
        case SIG_SOC_TYPE_TCP:
            {
                p_addr->ip_type = SIG_TRANS_ADDR_IPV4;

                num_read = soc_mgr_nw_read( sock_fd, max_read_size, ( sigtran_U8bit )read_flag,
                                            ip_addr, &port, p_pdata_buf, p_data_len_read );

                if( num_read > 0 )
                {
                    p_addr->ip_addr_ut.addr_v4.addr = ip_addr[0];
                    p_addr->ip_addr_ut.addr_v4.port = port;

                    return SIGTRAN_SUCCESS;
                }

                else if( num_read == 0 )
                {
                    /* In case of TCP socket of the recvfrom returns 0 (EOF),
                     * then it means peer has closed the connection */
                    *p_ecode = ESIG_IPC_RECVD_EOF;
                    sig_clr_fd_from_glb_fdset( sock_fd );
                    sig_os_close( sock_fd );
                    return SIGTRAN_FAILURE;
                }

                else
                {
                    *p_ecode = ESIG_IPC_READ_FAIL;
                    return SIGTRAN_FAILURE;
                    /* error */
                }

                break;
            }

            /* CSR 1-5892901 End*/

        case SIG_SOC_TYPE_IPV6:
        case SIG_SOC_TYPE_ICMPV6:
            {
                p_addr->ip_type = SIG_TRANS_ADDR_IPV6;

                num_read = soc_mgr_nw_read_in6( sock_fd, max_read_size, ( sigtran_U8bit )read_flag,
                                                ip_addr, &port, p_pdata_buf, p_data_len_read,
                                                &flow_info, &scope_id );

                if( num_read > 0 )
                {
                    p_addr->ip_addr_ut.addr_v6.ip_addr_ut.addr32[0] = ip_addr[0];
                    p_addr->ip_addr_ut.addr_v6.ip_addr_ut.addr32[1] = ip_addr[1];
                    p_addr->ip_addr_ut.addr_v6.ip_addr_ut.addr32[2] = ip_addr[2];
                    p_addr->ip_addr_ut.addr_v6.ip_addr_ut.addr32[3] = ip_addr[3];
                    p_addr->ip_addr_ut.addr_v6.port = port;
                    p_addr->ip_addr_ut.addr_v6.flow_info = flow_info;
                    p_addr->ip_addr_ut.addr_v6.scope_id = scope_id;

                    return SIGTRAN_SUCCESS;
                }

                else
                {
                    SIGTRAN_SET_ECODE( p_ecode, ESIG_IPC_READ_FAIL );
                    return SIGTRAN_FAILURE;
                    /* error */
                }

                break;
            }

        default:
            {
                return SIGTRAN_FAILURE;
            }
    }
}


/******************************************************************************
**  Function Name       : sig_setup_raw_ipv4_server
**
**    Description       : This function sets up the IPv4 server
**
**
**  RETURNS             : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_setup_raw_ipv4_server(
    sig_stack_type_et           stack_type,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sigtran_U16bit              *p_dest_id,
    sig_read_cbk_func_ptr_t     p_cbk_fn,
    sigtran_error_t             *p_ecode )
{
    sigtran_U16bit      srv_port = SOC_INVALID_PORT;
    sigtran_U16bit      handle;
    /* CSR 69326 fix starts*/
    sigtran_U32bit srv_ip[4] = {};
    /* CSR 69326 fix ends*/
    sigtran_return_t    ret_val = SIGTRAN_SUCCESS;
    sigtran_U32bit      flow_info;
    sigtran_U32bit      scope_id;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    if( SIGTRAN_NULL != p_src_addr )
    {
        /* get the ip and port where server will be open */
        /* CSR 69326 fix starts*/
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, &srv_ip[0],
                                                     &srv_port, &flow_info, &scope_id, p_ecode );
        /* CSR 69326 fix ends*/

    }

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
        *p_ecode = ESIG_IPC_INVALID_PARAM;
        return SIGTRAN_FAILURE;
    }

    /* CSR 69326 fix starts*/
    handle = soc_mgr_add_raw_ipv4_server(
                 &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                 srv_ip[0],
                 srv_port,
                 p_src_addr,
                 p_dest_addr,
                 read_flag,
                 p_cbk_fn );
    /* CSR 69326 fix ends*/

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::UDP server setup failed"\
                           "no valid handle left" ) );

        *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
        return SIGTRAN_FAILURE;
    }

    SIG_DEBUG_PRINT( ( "\nsig_setup_udpserver::UDP server setup"\
                       " at port %d ,handle alloted %d,", srv_port, handle ) );

    *p_dest_id = handle;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sig_setup_icmp_ipv4_server
**
**    Description       : This function set up the ICMP IPv4 server
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_setup_icmp_ipv4_server(
    sig_stack_type_et           stack_type,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sigtran_U16bit              *p_dest_id,
    sig_read_cbk_func_ptr_t     p_cbk_fn,
    sigtran_error_t             *p_ecode )
{
    sigtran_U16bit      srv_port = SOC_INVALID_PORT;
    sigtran_U16bit      handle;
    /* CSR 69326 fix starts*/
    sigtran_U32bit srv_ip[4] = {};
    /* CSR 69326 fix ends*/
    sigtran_return_t    ret_val = SIGTRAN_SUCCESS;
    sigtran_U32bit      flow_info;
    sigtran_U32bit      scope_id;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    if( SIGTRAN_NULL != p_src_addr )
    {
        /* get the ip and port where server will be open */
        /* CSR 69326 fix starts*/
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, &srv_ip[0],
                                                     &srv_port, &flow_info, &scope_id, p_ecode );
        /* CSR 69326 fix ends*/

    }

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_icmp_ipv4_server::Invalid IPC info" ) );
        *p_ecode = ESIG_IPC_INVALID_PARAM;
        return SIGTRAN_FAILURE;
    }

    /* CSR 69326 fix starts*/
    handle = soc_mgr_add_icmp_ipv4_server(
                 &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                 srv_ip[0],
                 srv_port,
                 p_src_addr,
                 p_dest_addr,
                 read_flag,
                 p_cbk_fn );
    /* CSR 69326 fix ends*/

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_icmp ipv4:: server setup failed"\
                           "no valid handle left" ) );

        *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
        return SIGTRAN_FAILURE;
    }

    SIG_DEBUG_PRINT( ( "\nsig_setup_icmp ipv4:: server setup"\
                       " at port %d ,handle alloted %d,", srv_port, handle ) );

    *p_dest_id = handle;
    return SIGTRAN_SUCCESS;
}


/******************************************************************************
**  Function Name       : sig_setup_raw_ipv6_server
**
**    Description       : This function set up the raw IPv6 server
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_setup_raw_ipv6_server(
    sig_stack_type_et           stack_type,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sigtran_U16bit              *p_dest_id,
    sig_read_cbk_func_ptr_t     p_cbk_fn,
    sigtran_error_t             *p_ecode )
{
    sigtran_U16bit      srv_port = SOC_INVALID_PORT;
    sigtran_U16bit      handle;
    sigtran_U32bit      a_srv_ip[4];
    sigtran_U32bit      *p_srv_ip = a_srv_ip;
    sigtran_return_t    ret_val = SIGTRAN_SUCCESS;
    sigtran_U32bit      flow_info = 0;
    sigtran_U32bit      scope_id = 0;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    if( SIGTRAN_NULL != p_src_addr )
    {
        /* get the ip and port where server will be open */
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, a_srv_ip,
                                                     &srv_port, &flow_info, &scope_id, p_ecode );
    }

    else
    {
        p_srv_ip = SIGTRAN_NULL;
    }

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
        *p_ecode = ESIG_IPC_INVALID_PARAM;
        return SIGTRAN_FAILURE;
    }

    handle = soc_mgr_add_raw_ipv6_server(
                 &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                 p_srv_ip,
                 srv_port,
                 flow_info,
                 scope_id,
                 p_src_addr,
                 p_dest_addr,
                 read_flag,
                 p_cbk_fn );

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::UDP server setup failed"\
                           "no valid handle left" ) );

        *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
        return SIGTRAN_FAILURE;
    }

    SIG_DEBUG_PRINT( ( "\nsig_setup_udpserver::UDP server setup"\
                       " at port %d ,handle alloted %d,", srv_port, handle ) );

    *p_dest_id = handle;
    return SIGTRAN_SUCCESS;
}


/******************************************************************************
**  Function Name       : sig_setup_icmp_ipv6_server
**
**    Description       : This function set up the ICMP IPV6 server
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_setup_icmp_ipv6_server(
    sig_stack_type_et           stack_type,
    sig_ip_addr_st              *p_src_addr,
    sig_ip_addr_st              *p_dest_addr,
    sigtran_boolean_t           read_flag,
    sigtran_U16bit              *p_dest_id,
    sig_read_cbk_func_ptr_t     p_cbk_fn,
    sigtran_error_t             *p_ecode )
{
    sigtran_U16bit      srv_port = SOC_INVALID_PORT;
    sigtran_U16bit      handle;
    sigtran_U32bit      a_srv_ip[4];
    sigtran_U32bit      *p_srv_ip = a_srv_ip;
    sigtran_return_t    ret_val = SIGTRAN_FAILURE;
    sigtran_U32bit      flow_info = 0;
    sigtran_U32bit      scope_id = 0;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );

    if( SIGTRAN_NULL != p_src_addr )
    {
        /* get the ip and port where server will be open */
        ret_val = sig_util_ext_ip_port_frm_ipc_info( p_src_addr, a_srv_ip,
                                                     &srv_port, &flow_info, &scope_id, p_ecode );
    }

    else
    {
        p_srv_ip = SIGTRAN_NULL;
    }

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
        *p_ecode = ESIG_IPC_INVALID_PARAM;
        return SIGTRAN_FAILURE;
    }

    handle = soc_mgr_add_icmp_ipv6_server(
                 &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                 p_srv_ip,
                 srv_port,
                 flow_info,
                 scope_id,
                 p_src_addr,
                 p_dest_addr,
                 read_flag,
                 p_cbk_fn );

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::UDP server setup failed"\
                           "no valid handle left" ) );

        *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
        return SIGTRAN_FAILURE;
    }

    SIG_DEBUG_PRINT( ( "\nsig_setup_udpserver::UDP server setup"\
                       " at port %d ,handle alloted %d,", srv_port, handle ) );

    *p_dest_id = handle;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_setup_udp_nw_server
**
**  DESCRIPTION:  Function set up a UDP NW server on given port
**
**  RETURN VALUE:
**                on SUCCESS
**                Function returns a handler by which stack identify this
**                Server for further communication
**                on Failure
**                retrun 0
******************************************************************************/
sigtran_return_t sig_setup_udp_nw_server(
    sig_stack_type_et   stack_type,
    sig_ip_addr_st      *p_src_addr,
    sig_ip_addr_st      *p_dest_addr,
    sigtran_boolean_t   read_flag,
    sigtran_U16bit      *p_dest_id,
    sig_read_cbk_func_ptr_t  p_cbk_fn,
    sigtran_error_t     *p_ecode )

{
    sigtran_U16bit srv_port = SOC_INVALID_PORT;
    sigtran_U16bit handle;
    /* CSR 69326 fix starts*/
    sigtran_U32bit srv_ip[4] = {};
    /* CSR 69326 fix ends*/
    sigtran_return_t ret_val = SIGTRAN_SUCCESS;

    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_id ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );

    if( SIGTRAN_NULL != p_src_addr )
    {
        /* get the ip and port where server will be open */
        /* CSR 69326 fix starts*/
        ret_val = sig_util_ext_ip_port_frm_ipc_info(
                      p_src_addr, &srv_ip[0], &srv_port, SIGTRAN_NULL, SIGTRAN_NULL,
                      p_ecode );
        /* CSR 69326 fix ends*/

    }

    if( SIGTRAN_FAILURE == ret_val )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::Invalid IPC info" ) );
        *p_ecode = ESIG_IPC_INVALID_PARAM;
        return SIGTRAN_FAILURE;
    }

    /* CSR 69326 fix starts*/
    handle = soc_mgr_add_udp_nw_server(
                 &( g_ipc_mgr_data[stack_type].ipc_conn_info ),
                 srv_ip[0],
                 srv_port,
                 p_src_addr,
                 p_dest_addr,
                 read_flag,
                 p_cbk_fn );
    /* CSR 69326 fix ends*/

    if( SOC_MGR_INVALID_HANDLE == handle )
    {
        SIG_DEBUG_PRINT( ( "\nsig_setup_udp_server::UDP server setup failed"\
                           "no valid handle left" ) );

        *p_ecode = ESIG_IPC_NO_HANDLE_AVL;
        return SIGTRAN_FAILURE;
    }

    SIG_DEBUG_PRINT( ( "\nsig_setup_udpserver::UDP server setup"\
                       " at port %d ,handle alloted %d,", srv_port, handle ) );

    *p_dest_id = handle;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_get_conn_local_addr
**
**  DESCRIPTION:  Function retun the local ip address information corresponding
**                to a given handle
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t  sig_get_conn_local_addr(
    sig_stack_type_et   self_type,
    sigtran_U16bit      dest_id,
    sig_ip_addr_st      *p_local_addr,
    sigtran_error_t     *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_local_addr ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );

    /*KW fix start*/
    if( dest_id >= SOC_MGR_MAX_CONNECTIONS )
    {
        SIGTRAN_SET_ECODE( p_ecode, ESIG_PARM_INVALID );
        SIG_DEBUG_PRINT( ( "\nsig_get_conn_local_addr: INVALID dest_id" ) );

        return SIGTRAN_FAILURE;
    }

    /*KW fix end*/
    *p_local_addr =
        g_ipc_mgr_data[self_type].ipc_conn_info.conn_info[dest_id].lcl_ip_str;
    SIGTRAN_SET_ECODE( p_ecode, ESIG_NO_ERROR );
    return SIGTRAN_SUCCESS;

}
#ifdef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  FUNCTION :    sig_get_q_module_info
**
**  DESCRIPTION:  For given Q module function returns the destination type
**                APP/TL extracting it from the corresponding servive name
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_get_q_module_info(
    sig_stack_type_et    self_stack,
    sig_q_module_t       q_module,
    sig_dest_type_et     *p_dest_type,
    sigtran_U16bit       *p_dest_id )
{
    sigtran_UL32bit    service_name;
    sig_stack_type_et  dest_stack_type;
    sig_dest_type_et   dest_type, db_dest_type;
    sigtran_error_t    ecode;

    SIG_DEBUG_PRINT( ( "\nsig_get_q_module_info::entered function" ) );
    SIG_ASSERT( ( SIGTRAN_NULL != q_module ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_dest_type ) );

    /*
     * By default set the destination type to application.
     */
    *p_dest_type = SIG_DEST_TYPE_APP;

    /* get service name from the q module */
    service_name = sig_get_service_name( q_module );

    /* get the ipc info from the service name which has
     * stack type dest type and handle id
     */
    sig_get_ipc_info_frm_service_name( service_name,
                                       &dest_stack_type,
                                       &dest_type,
                                       p_dest_id,
                                       &ecode );

    SIG_DEBUG_PRINT( ( "\nsig_get_q_module_info::stack type is %d"
                       " dest type is %d, handle id is %d", dest_stack_type, \
                       dest_type, *p_dest_id ) );

    /* check if dest_stack is valid. SUA,M3uA etc......*/
    if( ( SIG_INVALID_STACK < dest_stack_type ) &&
            ( SIG_MAX_NUM_STACK > dest_stack_type ) )
    {
        /*
         * Case of communication with SCTP through CSPL queues. In this case the
         * message will be received from SCTP with the stack type set to
         * SIG_SCTP_STACK and other parameters set to INVALID.
         */
        if( ( SIG_SCTP_STACK != self_stack ) && ( SIG_SCTP_STACK == dest_stack_type ) &&
                ( SIG_DEST_TYPE_INVALID == dest_type ) && ( SOC_MGR_INVALID_HANDLE == *p_dest_id ) )
        {
            SIG_DEBUG_PRINT( ( "\nsig_get_q_module_info::"
                               "case of SCTP through CSPL Queues" ) );
            *p_dest_type = SIG_DEST_TYPE_TL;

        }

        /*
         * check stack type is right and  a valid handle is present
         */
        else if( ( self_stack == dest_stack_type ) && ( SOC_MGR_MAX_CONNECTIONS >= *p_dest_id ) )
        {
            /*
             * This is the case when the SCTP is connected through sockets.
             * In this case the message will be received on the socked id as
             * opened by the SCTP client using the application stack type.
             * Hence the stack type for socket will be same as that of the self
             * stack.
             */
            /* if destination type is TL */
            if( SIG_DEST_TYPE_TL == dest_type )
            {
                /* check in data base the destination type is
                 * TL
                 */
                if( SOC_MGR_INVALID_HANDLE != *p_dest_id )
                {
                    if( *p_dest_id < SOC_MGR_MAX_CONNECTIONS ) /* KW fix */
                    {
                        db_dest_type =
                            g_ipc_mgr_data[dest_stack_type].ipc_conn_info.\
                            conn_info[*p_dest_id].dest_type;

                        if( db_dest_type == dest_type )
                        {
                            SIG_DEBUG_PRINT( ( "\nsig_get_q_module_info::"
                                               " dest type is SIG_DEST_TYPE_TL" ) );
                            *p_dest_type = SIG_DEST_TYPE_TL;
                        }
                    }
                }

                else
                {
                    if( g_ipc_mgr_data[dest_stack_type].
                            g_map_for_internal_modules[dest_type] == q_module )
                    {
                        SIG_DEBUG_PRINT( ( "\nsig_get_q_module_info::"
                                           " dest type is SIG_DEST_TYPE_TL" ) );
                        *p_dest_type = SIG_DEST_TYPE_TL;
                    }
                }
            }
        }
    }

    return SIGTRAN_SUCCESS;
}
#endif /* SIG_COMMON_WITH_CSPL */

/******************************************************************************
**  Function Name       : sig_resolve_hostname
**
**    Description       : Function for resolving the hostname
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_resolve_hostname(
    sigtran_U8bit      *p_hostname,
    sig_ip_addr_st     *p_ip_addr_list,
    sigtran_U32bit     *p_num_addrs,
    sigtran_error_t    *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_hostname ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_ip_addr_list ) );
    SIG_ASSERT( ( SIGTRAN_NULL != p_num_addrs ) );
    SIG_ASSERT( ( 0 != *p_num_addrs ) );

    SIGTRAN_SET_ECODE( p_ecode, ESIG_NO_ERROR );
    *p_num_addrs = sig_os_resolve_hostname( p_hostname, p_ip_addr_list, *p_num_addrs );

    if( 0 == *p_num_addrs )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sig_resolve_hostname
**
**    Description       : Function for resolving the hostname
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_gethostname(
    sigtran_U8bit      *p_hostname,
    sigtran_U32bit      host_len,
    sigtran_error_t    *p_ecode )
{
    SIGTRAN_SET_ECODE( p_ecode, ESIG_NO_ERROR );

    if( 0 == sig_os_gethostname( p_hostname, host_len ) )
    {
        return SIGTRAN_SUCCESS;
    }

    else
    {
        return SIGTRAN_FAILURE;
    }

}

#ifndef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  Function Name       : sig_get_q_module
**
**    Description       : Function to get Queue Module
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t    sig_get_q_module( sig_stack_type_et stack_type,
                                      sig_dest_type_et  dest_type,
                                      sigtran_U16bit    dest_id,
                                      sig_q_module_t    *p_q_module,
                                      sigtran_error_t   *p_ecode )
{
    stack_type = stack_type;
    dest_type  = dest_type;
    dest_id    = dest_id;
    p_q_module = p_q_module;
    p_ecode    = p_ecode;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  Function Name       : sig_resolve_hostname
**
**    Description       : Function to set the destination
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_set_destination(
    sig_stack_type_et stack_type,
    sig_dest_type_et  dest_type,
    sigtran_U16bit    handle,
    sigtran_pvoid_t   listner,
    sigtran_pvoid_t   data,
    sig_q_module_t    *p_q_module,
    sigtran_error_t   *p_ecode )
{
    stack_type = stack_type;
    dest_type  = dest_type;
    handle     = handle;
    listner    = listner;
    data       = data;
    p_q_module = p_q_module;
    p_ecode    = p_ecode;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sig_resolve_hostname
**
**    Description       : Function to set the Queue module
**
**    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t    sig_set_q_module( sig_stack_type_et stack_type,
                                      sig_dest_type_et  dest_type,
                                      sigtran_U16bit    dest_id,
                                      sig_q_module_t    q_module,
                                      sigtran_error_t   *p_ecode )
{
    stack_type = stack_type;
    dest_type  = dest_type;
    dest_id    = dest_id;
    q_module   = q_module;
    p_ecode    = p_ecode;
    return SIGTRAN_SUCCESS;
}
#endif

