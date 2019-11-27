/*****************************************************************************
 *    FILE NAME:
 *        s_clipc.c
 *
 *    DESCRIPTION:
 *    This file contains the client side of SCTP code. This file implements
 *    exact IPC mechanism between client build and Stack build. Depending
 *      on the choice of IPC chosen, This file    need to    be ported.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000   Sandeep Mahajan  -              Original    Creation
 *    02jan02       hbhondwe        SCTP_IPC_TCP    ipc changes for tcp
 *    07 Oct 2003   Vivek Bansal    Rel 5.0         Changes for CSPL
 *    27 Nov 2003   Vivek Bansal    Rel 5.0         Changes for CSPL
 *    11 Jan 2004   Vivek Bansal    Rel 5.0         Changes for IPC failure
 *                                                  indication to the appl.
 *    21 May 2014   Pinky Sinha      Bug Id 6       CSR 78264
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/

#define SCTP_MODULE "CL-SU"

#include <s_cotyps.h>
#include <s_cogbl.h>
#include <s_clpapi.h>
#include <s_closmem.h>
#include <s_cles.h>


sctp_U16bit         sctp_cli_ipctx_socket_id[SCTP_SU_CLI_MAX_DST];
sctp_U16bit         sctp_cli_ipcrx_socket_id[SCTP_SU_CLI_MAX_DST];
sctp_U32bit         sctp_cli_num_dest = 0;
sig_stack_type_et   g_appl_stack_type;

#ifdef VXWORKS
void start_sctp_client( sig_stack_type_et src_stack_type, int sctp_cli_port,
                        char  *this_host_ip, int sctp_stack_port, char *sctp_stack_ip,
                        sctp_port_type_et port_type );
#endif

sctp_return_t
sctp_client_gen_lcl_ipc_failure_notf(
    sctp_U8bit      *p_message,
    sctp_U32bit     soc_handle_id,
    sctp_error_t    error_code,
    sctp_U32bit     *p_data_len );

sctp_return_t
sctp_client_ipc_send_data(
    sctp_U8bit          *buffer,
    sctp_len_t          nooctets,
    sctp_src_id_t       src_id,
    sctp_dest_id_t      dest_id );


#ifndef SCTP_APPL_CSPL_PORTED
#ifndef SCTP_CLI_NO_THREAD
void *sctp_client_eshell_master_thread( void     *A );
#endif
#endif


/***************************************************************************
 *    FUNCTION :
 *    sctp_client_ipc_send_data
 *
 *    DESCRIPTION:
 *    Calls the appropriate UDP/TL function to send data over    a socket
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t sctp_client_ipc_send_data(
    sctp_U8bit          *p_buffer,
    sctp_len_t          nooctets,
    sctp_src_id_t       src_id,
    sctp_dest_id_t      dest_id )
{
    sigtran_error_t     ecode;
    sctp_U16bit         handle_id;

    SCTP_CLIENT_TRACE( ( "sctp_client_ipc_send_data, \
        src id = %d  dest_id = %d ", src_id, dest_id ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( src_id );
    /*to avoid the warning*/

    /* klockwork fix 6.2.4 */
    if( 0 == dest_id )
    {
        SCTP_CLIENT_TRACE( ( "sctp_client_ipc_send_data: Invalid dest , \
                        dest_id = %d ", dest_id ) );
        return SCTP_FAILURE;

    }

    /*klockwork fix 6.2.4 */

    if( SCTP_CLIENT_INVALID_DEST_ID == dest_id )
    {
        handle_id = SOC_MGR_INVALID_HANDLE;
    }

    else
    {
        if( dest_id > SCTP_SU_CLI_MAX_DST )
        {
            SCTP_CLIENT_TRACE( ( "sctp_client_ipc_send_data: Invalid dest , \
                        dest_id = %d ", dest_id ) );
            return SCTP_FAILURE;
        }

        handle_id = sctp_cli_ipctx_socket_id[dest_id - 1];
    }

    if( SIGTRAN_FAILURE == sig_send( g_appl_stack_type, SIG_DEST_TYPE_TL, handle_id,
                                     p_buffer, nooctets, &ecode ) )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_client_eshell_master_thread
 *
 *    DESCRIPTION:
 *        This function implement the SCTP client receive functions. Here we
 *    receive response/notifications and pass them to be processed.
 *    RETURN VALUE:
 *        None
 *************************************************************************/
#ifndef SCTP_APPL_CSPL_PORTED
#ifndef SCTP_CLI_NO_THREAD
void *sctp_client_eshell_master_thread( void *A )
{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( A );
    /*to avoid the warning*/

    for( ;; )
    {
        sig_ipc_select( SCTP_NULL );
        sig_recv_from_ext_module( SCTP_NULL, SCTP_NULL, SCTP_NULL, SCTP_NULL );
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return SCTP_NULL;
#endif
}
#endif /* SCTP_CLI_NO_THREAD */
#endif /* SCTP_APPL_CSPL_PORTED */


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_client_ipc
 *    DESCRIPTION:
 *        Send buffer to stack using IPC send call. If the interface with the
 *        above application is CSPL based, then this function will return the
 *        socket Id which this SCTP client is bound to receive messages from
 *        SCT stack. In this case it is applications responsibility to check
 *        for the message on this socket and invoke the corresponding SCTP
 *        function.
 *
 *    RETURN VALUE:
 *        dest id of stack
 *        SCTP_FAILURE
 ***************************************************************************/
sctp_S32bit
sctp_init_client_ipc(
    sig_stack_type_et           src_stack_type,
    sctp_U16bit                 local_udp_port,
    sctp_sockaddr_st            local_ip_address,
    sctp_U16bit                 stack_udp_port,
    sctp_sockaddr_st            remote_ip_address,
    sctp_port_type_et           port_type,
    sctp_error_t                *p_ecode )
{
    sigtran_error_t         sig_ecode;
    static sctp_Boolean_t   b_client_alrdy_init = SCTP_FAILURE;
    sig_ip_addr_st          local_sig_addr;
    sig_ip_addr_st          remote_sig_addr;
    sctp_U16bit             recv_handle_id = 0;
    sctp_U16bit             send_handle_id = 0;
#ifdef SCTP_APPL_CSPL_PORTED
    sigtran_return_t        ret_val;
    /* Bug Id 6 fix starts*/
    /*sig_q_module_t          sctp_q_module;*/
    sig_q_module_t          sctp_q_module = SOC_INV_Q_MODULE;
    /* Bug Id 6 fix ends*/
#endif
#ifndef SCTP_APPL_CSPL_PORTED
    sigtran_thread_t        cli_thread_id;
#endif

#ifndef SCTP_APPL_CSPL_PORTED
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

    if( SIGTRAN_FAILURE == sig_init_ipc_mgr( SIG_SCTP_CLIENT, SCTP_NULL, SCTP_NULL,
                                             SCTP_NULL ) )
#else

    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    if( SIGTRAN_FAILURE == sig_init_ipc_mgr( SIG_SCTP_CLIENT,
                                             ( sig_get_sm_msg_len_cbk ) SCTP_NULL,
                                             ( sig_get_su_msg_len_cbk ) SCTP_NULL,
                                             ( sig_get_tl_msg_len_cbk ) SCTP_NULL ) )
#endif
    {
        return -1;
    }

    src_stack_type = SIG_SCTP_CLIENT;
#endif

#ifdef SCTP_APPL_CSPL_PORTED

    /*
     * If the SCTP stack is located in the same process as client i.e the client
     * and stack are communicating using message queues.
     */
    if( SCTP_PORT_TYPE_INTERNAL == port_type )
    {
        /*
         * Store the source stack type. This is required when the client wants to
         * send some message to the lower layer.
         */
        g_appl_stack_type = src_stack_type;

        /* get Q module for SCTP */
        if( SIGTRAN_FAILURE ==
                sig_get_q_module( SIG_SCTP_STACK, SOC_MGR_INVALID_HANDLE,
                                  SOC_MGR_INVALID_HANDLE, &sctp_q_module, &sig_ecode ) )
        {
            return -1;
        }

        if( SIGTRAN_FAILURE ==
                sig_set_q_module( src_stack_type, SIG_DEST_TYPE_TL,
                                  SOC_MGR_INVALID_HANDLE, sctp_q_module, &sig_ecode ) )
        {
            return -1;
        }

        if( SCTP_FAILURE == sctp_client_init_mem_module() )
        {
            return -1;
        }

        return SCTP_CLIENT_INVALID_DEST_ID;
    }

#endif

    /*
     * If the number of destinations reaches max, then return error.
     */
    if( SCTP_SU_CLI_MAX_DST == sctp_cli_num_dest )
    {
        return ( -1 );
    }

    SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( local_ip_address, local_udp_port,
                                     local_sig_addr );

    SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( remote_ip_address, stack_udp_port,
                                     remote_sig_addr );

    /*
     * If the client is already initialised, then just open the socket and
     * don't do any other intitialisations.
     */
    if( SCTP_PORT_TYPE_UDP == port_type )
    {
        /*
         * Either do this or set the destination. Since the local address
         * may be same.
         */
        /*
         * Port for receiving data.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( src_stack_type, SIG_SOC_MODE_SERVER,
                                       SIG_SOC_TYPE_UDP, SIGTRAN_TRUE, SIGTRAN_FALSE, &local_sig_addr,
                                       SIGTRAN_NULL, ( sig_read_cbk_func_ptr_t )sctp_client_ipc_recv_data,
                                       &recv_handle_id, &sig_ecode ) )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( -1 );
        }

        sctp_cli_ipcrx_socket_id[sctp_cli_num_dest] = recv_handle_id;

        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( local_ip_address,
                                         ( local_udp_port + SCTP_SEND_RECV_PORT_OFFSET ), local_sig_addr );

        /*
         * Port for sending data.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( src_stack_type, SIG_SOC_MODE_SERVER,
                                       SIG_SOC_TYPE_UDP, SIGTRAN_TRUE, SIGTRAN_TRUE, &local_sig_addr,
                                       &remote_sig_addr, ( sig_read_cbk_func_ptr_t )sctp_client_ipc_recv_data,
                                       &send_handle_id, &sig_ecode ) )
        {
            /*
             * Close the channel opened earlier.
             */
            sig_close_comm_channel( src_stack_type, recv_handle_id );

            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( -1 );
        }

        sctp_cli_ipctx_socket_id[sctp_cli_num_dest] = send_handle_id;
    }

    else /* Port type is TCP */
    {
        /*
         * Open the TCP socket and send connect to the server.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( src_stack_type, SIG_SOC_MODE_CLIENT,
                                       SIG_SOC_TYPE_TCP, SIGTRAN_TRUE, SIGTRAN_TRUE, &local_sig_addr,
                                       &remote_sig_addr, ( sig_read_cbk_func_ptr_t )sctp_client_ipc_recv_data,
                                       &recv_handle_id, &sig_ecode ) )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( -1 );
        }

        sctp_cli_ipcrx_socket_id[sctp_cli_num_dest] = recv_handle_id;
        sctp_cli_ipctx_socket_id[sctp_cli_num_dest] = recv_handle_id;
    }

#ifdef SCTP_APPL_CSPL_PORTED

    if( 0 != send_handle_id )
    {
        ret_val = sig_set_destination( src_stack_type, SIG_DEST_TYPE_TL,
                                       send_handle_id, SCTP_NULL, SCTP_NULL, &sctp_q_module,
                                       &sig_ecode );

        if( SIGTRAN_SUCCESS == ret_val )
        {
            ret_val =
                sig_set_q_module( src_stack_type, SIG_DEST_TYPE_TL, send_handle_id,
                                  sctp_q_module, &sig_ecode );
        }

        if( SIGTRAN_FAILURE == ret_val )
        {
            /*
             * Close the recv channel opened earlier.
             */
            if( 0 != recv_handle_id )
            {
                sig_close_comm_channel( src_stack_type, recv_handle_id );
            }

            /*
             * Close the send channel opened earlier.
             */
            sig_close_comm_channel( src_stack_type, send_handle_id );
            return ( -1 );
        }
    }

    if( 0 != recv_handle_id )
    {
        ret_val = sig_set_destination( src_stack_type, SIG_DEST_TYPE_TL,
                                       recv_handle_id, SCTP_NULL, SCTP_NULL, &sctp_q_module,
                                       &sig_ecode );

        if( SIGTRAN_SUCCESS == ret_val )
        {
            ret_val =
                sig_set_q_module( src_stack_type, SIG_DEST_TYPE_TL, recv_handle_id,
                                  sctp_q_module, &sig_ecode );
        }

        if( SIGTRAN_FAILURE == ret_val )
        {
            /*
             * Close the recv channel opened earlier.
             */
            sig_close_comm_channel( src_stack_type, recv_handle_id );

            /*
             * Close the send channel opened earlier.
             */
            if( 0 != send_handle_id )
            {
                sig_close_comm_channel( src_stack_type, send_handle_id );
            }

            return ( -1 );
        }
    }

#endif

    /*
     * Check if client has been already initialized i.e. user just wants to add
     * a new stack. In this case just return the destination id.
     */
    if( SCTP_TRUE == b_client_alrdy_init )
    {
        sctp_cli_num_dest++;
        return ( sctp_cli_num_dest );
    }

    /*Initialize the buffer in which API elements would be sent*/
    if( SCTP_FAILURE == sctp_client_init_mem_module() )
    {
        /*
         * Close the recv channel opened earlier.
         */
        if( 0 != recv_handle_id )
        {
            sig_close_comm_channel( src_stack_type, recv_handle_id );
        }

        /*
         * Close the send channel opened earlier.
         */
        if( 0 != send_handle_id )
        {
            sig_close_comm_channel( src_stack_type, send_handle_id );
        }

        *p_ecode = E_MALLOC_FAILED;  /* No Memory ...*/
        return( -1 );
    }

#ifndef SCTP_APPL_CSPL_PORTED
#ifndef SCTP_CLI_NO_THREAD
    /*
     * Start the client receiver thread only if the Application is not CSPL
     * ported and no client thread is not set.
     */

    if( SIGTRAN_FAILURE ==
            sigtran_create_thread( &cli_thread_id, sctp_client_eshell_master_thread,
                                   ( sigtran_pvoid_t )SCTP_NULL, &sig_ecode ) )
    {
        /*
         * Close the recv channel opened earlier.
         */
        if( 0 != recv_handle_id )
        {
            sig_close_comm_channel( src_stack_type, recv_handle_id );
        }

        /*
         * Close the send channel opened earlier.
         */
        if( 0 != send_handle_id )
        {
            sig_close_comm_channel( src_stack_type, send_handle_id );
        }

        /*
         * De-init the memory module.
         */
        sctp_client_deinit_mem_module();

        *p_ecode = E_ESHELL_INIT_FAILED;
        return ( -1 );
    }

#endif /* SCTP_CLI_NO_THREAD */
#endif /* SCTP_APPL_CSPL_PORTED */

    /*
     * Mark the flag to indicate that client has already been initialised once.
     */
    b_client_alrdy_init = SCTP_TRUE;
    sctp_cli_num_dest = 1;

    /*
     * Store the source stack type. This is required when the client wants to
     * send some message to the lower layer.
     */
    g_appl_stack_type = src_stack_type;

    return ( sctp_cli_num_dest );
}

/******************************************************************************
**  Function Name       : sctp_client_ipc_recv_data
**
**    Description       : This function is responsible for reading the message
**                        on the sockets and calls the client function directly
**                        to process the buffer.
**
**    Returns           : SIGTRAN_SUCCESS
**                        SIGTRAN_FAILURE
**
**    Functions called  :
**
******************************************************************************/
sigtran_return_t    sctp_client_ipc_recv_data(
    sigtran_U32bit      handle_id,
    sig_sock_mode_et    socket_mode,
    sig_soc_type_et     socket_type,
    sig_sock_fd_t       socket_id,
    sig_q_module_t      *p_from_q_module,
    sigtran_pvoid_t     *p_data,
    sigtran_U32bit      *p_data_len,
    sigtran_error_t     *p_ecode )
{
    sctp_U16bit         dest_id;
    sctp_U8bit          *p_message;
    sig_ip_addr_st      from_addr;
    sctp_U32bit         data_to_read;
    sctp_U8bit          arr_buf_len[4];
    sctp_U8bit          *p_buf_len;
    sigtran_boolean_t   b_is_new_dest_id;
    sigtran_return_t    ret_val = SIGTRAN_SUCCESS;

    p_message = ( sctp_U8bit * )sctp_client_get_mesg_buffer( SCTP_MAX_API_SIZE );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( socket_mode );
    SCTP_UNUSED_VARIABLE( p_from_q_module );
    SCTP_UNUSED_VARIABLE( p_data );
    /*to avoid the warning*/


    if( SCTP_NULL == p_message )
    {
        *p_ecode = ESIG_MEM_ALLOC_FAILED;
        return SIGTRAN_FAILURE;
    }

    /*
     * If the socket type is UDP, then read the message first.
     */
    if( SIG_SOC_TYPE_UDP == socket_type )
    {
        if( SIGTRAN_FAILURE ==
                sig_read( g_appl_stack_type, socket_type, socket_id,
                          SCTP_MAX_API_SIZE, SIG_RECV_ALL, &from_addr,
                          ( sigtran_pvoid_t )p_message, p_data_len, p_ecode ) )
        {
            sctp_client_free_mesg_buffer( &p_message );
            return SIGTRAN_FAILURE;
        }

        /*
         * Get the destination id using the IP address and port number.
         */
        if( SIGTRAN_FAILURE ==
                sig_get_dest_id( g_appl_stack_type, socket_type,
                                 SOC_MGR_INVALID_HANDLE, from_addr.ip_addr_ut.addr_v4.addr,
                                 from_addr.ip_addr_ut.addr_v4.port, &dest_id,
                                 &b_is_new_dest_id, p_ecode ) )
        {
            sctp_client_free_mesg_buffer( &p_message );
            return SIGTRAN_FAILURE;
        }
    }

    else
    {
        p_buf_len = arr_buf_len;

        /* Socket type is TCP */
        if( SIGTRAN_FAILURE ==
                sig_read( g_appl_stack_type, socket_type, socket_id,
                          4, SIG_RECV_ALL, &from_addr, ( sigtran_pvoid_t )p_buf_len,
                          p_data_len, p_ecode ) )
        {
            if( SCTP_FAILURE ==
                    sctp_client_gen_lcl_ipc_failure_notf( p_message,
                                                          handle_id, E_CLI_STK_IPC_FAILURE, p_data_len ) )
            {
                sctp_client_free_mesg_buffer( &p_message );
                return SIGTRAN_FAILURE;
            }

            ret_val = SIGTRAN_FAILURE;
        }

        sig_util_extract_4bytes( &p_buf_len, &data_to_read );

        if( SIGTRAN_SUCCESS == ret_val )
        {
            if( SIGTRAN_FAILURE ==
                    sig_read( g_appl_stack_type, socket_type, socket_id,
                              data_to_read, SIG_RECV_ALL, &from_addr,
                              ( sigtran_pvoid_t )p_message, p_data_len, p_ecode ) )
            {
                if( SCTP_FAILURE ==
                        sctp_client_gen_lcl_ipc_failure_notf( p_message,
                                                              handle_id, E_CLI_STK_IPC_FAILURE, p_data_len ) )
                {
                    sctp_client_free_mesg_buffer( &p_message );
                    return SIGTRAN_FAILURE;
                }

                ret_val = SIGTRAN_FAILURE;
            }
        }
    }

#ifdef SCTP_APPL_CSPL_PORTED

    if( SIGTRAN_FAILURE ==
            sig_get_q_module( g_appl_stack_type, SIG_DEST_TYPE_TL,
                              handle_id, p_from_q_module, p_ecode ) )
    {
        sctp_client_free_mesg_buffer( &p_message );
        return SIGTRAN_FAILURE;
    }

    *p_data = p_message;

    if( SIGTRAN_FAILURE == ret_val )
    {
        sig_close_comm_channel( g_appl_stack_type, handle_id );
    }

#else

    if( SIGTRAN_FAILURE == ret_val )
    {
        sig_close_comm_channel( g_appl_stack_type, ( sigtran_U16bit )handle_id );
    }

    /*
     * Call the client function directly.
     */
    sctp_su_process_stack_buffer( p_message, *p_data_len );
    sctp_client_free_mesg_buffer( &p_message );
#endif

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_client_gen_lcl_ipc_failure_notf
**
**    Description       : Function for generating the notification for the IPC
**                        failuire with the stack.
**
**    Returns           : SCTP_FAILURE
**                        SCTP_SUCCESS
**
**    Functions called  :
**
******************************************************************************/
sctp_return_t sctp_client_gen_lcl_ipc_failure_notf(
    sctp_U8bit      *p_message,
    sctp_U32bit     soc_handle_id,
    sctp_error_t    error_code,
    sctp_U32bit     *p_data_len )
{
    sctp_msg_lcl_ipc_error_notf_st  *p_lcl_ipc_error_notf;
    sctp_U16bit                     count;
    sctp_U8bit                      *p_buffer = p_message;

    /*
     * Serach the handle id in the database.
     */
    for( count = 0; count < sctp_cli_num_dest; count++ )
    {
        if( sctp_cli_ipcrx_socket_id[count] == soc_handle_id )
        {
            break;
        }
    }

    if( count == sctp_cli_num_dest )
    {
        return SCTP_FAILURE;
    }

    p_message =
        sctp_build_api_header( p_message, SCTP_API_LCL_IPC_ERROR_NOTF,
                               SCTP_NOTF_TRANS_ID, ( sctp_U16bit )( count + 1 ), 0 );

    *p_data_len = SCTP_API_HEADER_LEN + sizeof( sctp_msg_lcl_ipc_error_notf_st );

    sctp_buffer_set_UlongAtOffset( p_buffer, APIBUF_LENGTH_OFFSET,
                                   ( sctp_U32bit )*p_data_len );

    p_lcl_ipc_error_notf = ( sctp_msg_lcl_ipc_error_notf_st * )p_message;

    p_lcl_ipc_error_notf->dest_stk_id = count + 1;
    p_lcl_ipc_error_notf->err_code = error_code;

    return SCTP_SUCCESS;
}

#ifdef VXWORKS
/******************************************************************************
**  Function Name       : start_sctp_client
**
**    Description       : Function to start SCTP client
**
**    Returns           : NONE
**
**    Functions called  : sctp_init_client_ipc
**
******************************************************************************/
void start_sctp_client( sig_stack_type_et src_stack_type, int sctp_cli_port,
                        char  *this_host_ip, int sctp_stack_port, char *sctp_stack_ip,
                        sctp_port_type_et port_type )
{
    sctp_error_t     ecode;
    sctp_sockaddr_st sctp_host;
    sctp_sockaddr_st this_host;

    this_host.ip.v4.addr = SCTP_NTOHL( inet_addr( this_host_ip ) );
    this_host.family = 2;
    sctp_host.ip.v4.addr = SCTP_NTOHL( inet_addr( sctp_stack_ip ) );
    sctp_host.family = 2;

    sctp_init_client_ipc( src_stack_type, sctp_cli_port, this_host,
                          sctp_stack_port, sctp_host, port_type, &ecode );
}

#endif



