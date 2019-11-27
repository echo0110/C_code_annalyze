/******************************************************************************
 *    FILE NAME:
 *        s_smipc.c
 *
 *    DESCRIPTION:
 *    This file contains the SM client side of SCTP code. This file implements
 *    exact IPC mechanism between SM client build and Stack build.
 *      Depending on the choice of IPC chosen, This file need to be ported.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    23March 2001   Vakul Garg      -            Original    Creation
 *    03Jan 2002    hbhondwe        SCTP_IPC_TCP  IPC change to support TCP also
 *    07 Oct 2003   Vivek Bansal    Rel 5.0       Changes for CSPL
 *    27 Nov 2003   Vivek Bansal    Rel 5.0       Changes for CSPL
 *    21 May 2014   Pinky Sinha     Bug Id 6      CSR 78264
 *    15 Sep 2014   Priyanka Mishra     Bug Id 22     CSR 85223
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#define SCTP_MODULE "CL-SM"

#include <s_cotyps.h>
#include <s_cogbl.h>
#include <s_clco.h>
#include <s_clsmap.h>
#include <s_closmemsm.h>
#include <s_dbg.h>
#include <s_smipc.h>
#include <s_clessm.h>


#ifndef SCTP_APPL_CSPL_PORTED
void *sctp_sm_client_eshell_master_thread( void     *A );
#endif

const int       SCTP_CLI_TIME_OUT = 2;

sctp_return_t
sctp_sm_client_ipc_send_data(
    sctp_U8bit        *buffer,
    sctp_len_t        nooctets,
    sctp_src_id_t     src_id,
    sctp_dest_id_t    dest_id );

sig_stack_type_et   g_sm_appl_stack_type;

sctp_S32bit       sctp_sm_cli_ipcrx_socket_id;
sctp_S32bit       sctp_sm_cli_ipctx_socket_id;

/***************************************************************************
 *    FUNCTION :
 *    sctp_sm_client_ipc_send_data
 *
 *    DESCRIPTION:
 *    Calls the appropriate UDP/TL function to send data over    a socket
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_sm_client_ipc_send_data(
    sctp_U8bit          *p_buffer,
    sctp_len_t          nooctets,
    sctp_src_id_t       src_id,
    sctp_dest_id_t      dest_id )
{
    sigtran_error_t     ecode;
    sctp_U16bit         handle_id;

    /*
     * since currently we can only have one instance of SCTP stack ,
     * src id and dest id are not being used. These may be used to
     * customise E-shell.
     */
    SCTP_CLIENT_TRACE( ( "Entering In function sctp_sm_client_ipc_send_data\
                src id = %d  dest_id = %d\n ", src_id, dest_id ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( src_id );
    SCTP_UNUSED_VARIABLE( dest_id );
    /*to avoid the warning*/


    handle_id = sctp_sm_cli_ipctx_socket_id;

    if( SIGTRAN_FAILURE == sig_send( g_sm_appl_stack_type, SIG_DEST_TYPE_TL, handle_id,
                                     p_buffer, nooctets, &ecode ) )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

#ifndef SCTP_APPL_CSPL_PORTED
/***************************************************************************
 *    FUNCTION :
 *        sctp_sm_client_eshell_master_thread
 *
 *    DESCRIPTION:
 *        This function implement the SCTP client receive functions. Here we
 *    receive response/notifications and pass them to be processed.
 *    RETURN VALUE:
 *                None
 *************************************************************************/
void *
sctp_sm_client_eshell_master_thread(
    void *A )
{
    sigtran_time_t  time_val; /* CSR 1-5892901 */
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( A );
    /*to avoid the warning*/

    for( ;; )
    {
        /* CSR 1-5892901 */
        time_val.tv_sec  = SCTP_CLI_TIME_OUT;
        time_val.tv_usec = 0;

        if( sig_ipc_select( &time_val ) > 0 )
        {
            sig_recv_from_ext_module( SCTP_NULL, SCTP_NULL, SCTP_NULL, SCTP_NULL );
        }

        /* CSR 1-5892901 */
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return SCTP_NULL;
#endif
}
#endif

/***************************************************************************
 *    FUNCTION :
 *        sctp_init_sm_client_ipc
 *    DESCRIPTION:
 *        This function is called by the system management to initialize the
 *        SCTP client. This function establishes the communication channel
 *        between the SCTP Client and Stack.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ***************************************************************************/
/*Bug Id 22 fix starts*/
/*sctp_return_t*/
sctp_S32bit
/*Bug Id 22 fix ends*/




sctp_init_sm_client_ipc(
    sig_stack_type_et          src_stack_type,
    sctp_U16bit                local_udp_port,
    sctp_sockaddr_st           local_ip_address,
    sctp_U16bit                stack_udp_port,
    sctp_sockaddr_st           remote_ip_address,
    sctp_port_type_et          port_type,
    sctp_error_t               *p_ecode )
{
    sigtran_error_t         sig_ecode;
    sig_ip_addr_st          local_sig_addr;
    sig_ip_addr_st          remote_sig_addr;
    sctp_U16bit             recv_handle_id = 0;
    sctp_U16bit             send_handle_id = 0;
#ifdef SCTP_APPL_CSPL_PORTED
    sigtran_return_t        ret_val;
    /* Bug id 6 fix starts */
    /*sig_q_module_t          sctp_q_module;*/
    sig_q_module_t          sctp_q_module = SOC_INV_Q_MODULE;
    /* Bug id 6 fix ends */
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

    /*Initialize the buffer in which API elements would be sent*/
    if( SCTP_FAILURE == sctp_client_sm_init_mem_module() )
    {
        /*Bug Id 22 fix starts*/

        /*return 0;*/
        return ( SCTP_FAILURE );
        /*Bug Id 22 fix ends*/

    }

#ifdef SCTP_APPL_CSPL_PORTED

    /*
     * If the SCTP stack is located in the same process as client i.e the client
     * and stack are communicating using message queues.
     */
    if( SCTP_PORT_TYPE_INTERNAL == port_type )
    {
        g_sm_appl_stack_type = src_stack_type;

        /* get Q module for SCTP */
        if( SIGTRAN_FAILURE ==
                sig_get_q_module( SIG_SCTP_STACK, SOC_MGR_INVALID_HANDLE,
                                  SOC_MGR_INVALID_HANDLE, &sctp_q_module, &sig_ecode ) )
        {
            sctp_client_sm_deinit_mem_module();
            /*Bug Id 22 fix starts*/
            /*return 0;*/
            return ( SCTP_FAILURE );
            /*Bug Id 22 fix ends*/
        }

        if( SIGTRAN_FAILURE ==
                sig_set_q_module( src_stack_type, SIG_DEST_TYPE_TL,
                                  SOC_MGR_INVALID_HANDLE, sctp_q_module, &sig_ecode ) )
        {
            sctp_client_sm_deinit_mem_module();
            /*Bug Id 22 fix starts*/

            /*return 0;*/
            return ( SCTP_FAILURE );
            /*Bug Id 22 fix ends*/

        }

        sctp_sm_cli_ipctx_socket_id = SOC_MGR_INVALID_HANDLE;
        return SCTP_SM_CLIENT_INVALID_DEST_ID;
    }

#endif

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
                                       SIG_SOC_TYPE_UDP, SIGTRAN_TRUE, SIGTRAN_FALSE,
                                       &local_sig_addr, SIGTRAN_NULL,
                                       ( sig_read_cbk_func_ptr_t )sctp_sm_client_ipc_recv_data,
                                       &recv_handle_id, &sig_ecode ) )
        {
            sctp_client_sm_deinit_mem_module();
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( -1 );
        }

        sctp_sm_cli_ipcrx_socket_id = recv_handle_id;

        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( local_ip_address,
                                         ( local_udp_port + SCTP_SEND_RECV_PORT_OFFSET ), local_sig_addr );

        /*
         * Port for sending data.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( src_stack_type, SIG_SOC_MODE_SERVER,
                                       SIG_SOC_TYPE_UDP, SIGTRAN_TRUE, SIGTRAN_TRUE, &local_sig_addr,
                                       &remote_sig_addr, ( sig_read_cbk_func_ptr_t )sctp_sm_client_ipc_recv_data,
                                       &send_handle_id, &sig_ecode ) )
        {
            /*
             * Close the channel opened earlier.
             */
            sig_close_comm_channel( src_stack_type, recv_handle_id );
            sctp_client_sm_deinit_mem_module();

            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( -1 );
        }

        sctp_sm_cli_ipctx_socket_id = send_handle_id;
    }

    else /* Port type is TCP */
    {
        /*
         * Open the TCP socket and send connect to the server.
         */
        if( SIGTRAN_FAILURE ==
                sig_open_comm_channel( src_stack_type, SIG_SOC_MODE_CLIENT,
                                       SIG_SOC_TYPE_TCP, SIGTRAN_TRUE, SIGTRAN_TRUE,
                                       &local_sig_addr, &remote_sig_addr,
                                       ( sig_read_cbk_func_ptr_t )sctp_sm_client_ipc_recv_data,
                                       &recv_handle_id, &sig_ecode ) )
        {
            sctp_client_sm_deinit_mem_module();
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( -1 );
        }

        sctp_sm_cli_ipctx_socket_id = recv_handle_id;
        sctp_sm_cli_ipcrx_socket_id = recv_handle_id;
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
    SCTP_CLIENT_TRACE( ( "Connected \r\n" ) );

    /*
     * Store the source stack type. This is required when the client wants to
     * send some message to the lower layer.
     */
#ifndef SCTP_APPL_CSPL_PORTED

    if( SIGTRAN_FAILURE ==
            sigtran_create_thread( &cli_thread_id, sctp_sm_client_eshell_master_thread,
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
        sctp_client_sm_deinit_mem_module();

        *p_ecode = E_ESHELL_INIT_FAILED;
        return ( -1 );
    }

#endif
    g_sm_appl_stack_type = src_stack_type;
    return ( SCTP_SUCCESS );
}


/******************************************************************************
**  Function Name       : sctp_sm_client_ipc_recv_data
**
**    Description       : This function is responsible for reading the message
**                        on the sockets and calls sctp_sm_process_stack_buffer()
**                        to process the buffer.
**
**    Returns           : SIGTRAN_FAILURE
**                        SIGTRAN_SUCCESS
**
**    Functions called  :
**
******************************************************************************/
sigtran_return_t    sctp_sm_client_ipc_recv_data(
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

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( handle_id );
    SCTP_UNUSED_VARIABLE( socket_mode );
    SCTP_UNUSED_VARIABLE( p_from_q_module );
    SCTP_UNUSED_VARIABLE( p_data );
    /*to avoid the warning*/


    p_message = ( sctp_U8bit * )sctp_client_sm_get_mesg_buffer( SCTP_MAX_API_SIZE );

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
                sig_read( g_sm_appl_stack_type, socket_type, socket_id,
                          SCTP_MAX_API_SIZE, SIG_RECV_ALL, &from_addr,
                          ( sigtran_pvoid_t )p_message, p_data_len, p_ecode ) )
        {
            sctp_client_sm_free_mesg_buffer( &p_message );
            return SIGTRAN_FAILURE;
        }

        /*
         * Get the destination id using the IP address and port number.
         */
        if( SIGTRAN_FAILURE ==
                sig_get_dest_id( g_sm_appl_stack_type, socket_type,
                                 SOC_MGR_INVALID_HANDLE, from_addr.ip_addr_ut.addr_v4.addr,
                                 from_addr.ip_addr_ut.addr_v4.port, &dest_id,
                                 &b_is_new_dest_id, p_ecode ) )
        {
            sctp_client_sm_free_mesg_buffer( &p_message );
            return SIGTRAN_FAILURE;
        }
    }

    else
    {
        p_buf_len = arr_buf_len;

        /* Socket type is TCP */
        if( SIGTRAN_FAILURE ==
                sig_read( g_sm_appl_stack_type, socket_type, socket_id,
                          4, SIG_RECV_ALL, &from_addr, ( sigtran_pvoid_t )p_buf_len,
                          p_data_len, p_ecode ) )
        {
            sctp_client_sm_free_mesg_buffer( &p_message );
            return SIGTRAN_FAILURE;
        }

        sig_util_extract_4bytes( &p_buf_len, &data_to_read );

        if( SIGTRAN_FAILURE ==
                sig_read( g_sm_appl_stack_type, socket_type, socket_id,
                          data_to_read, SIG_RECV_ALL, &from_addr,
                          ( sigtran_pvoid_t )p_message, p_data_len, p_ecode ) )
        {
            sctp_client_sm_free_mesg_buffer( &p_message );
            return SIGTRAN_FAILURE;
        }
    }

#ifdef SCTP_APPL_CSPL_PORTED

    if( SIGTRAN_FAILURE ==
            sig_get_q_module( g_sm_appl_stack_type, SIG_DEST_TYPE_TL,
                              handle_id, p_from_q_module, p_ecode ) )
    {
        sctp_client_sm_free_mesg_buffer( &p_message );
        return SIGTRAN_FAILURE;
    }

    *p_data = p_message;
#else
    /*
     * Call the client function directly.
     */
    sctp_sm_process_stack_buffer( p_message, *p_data_len );
    sctp_client_sm_free_mesg_buffer( &p_message );
#endif

    return SIGTRAN_SUCCESS;
}
