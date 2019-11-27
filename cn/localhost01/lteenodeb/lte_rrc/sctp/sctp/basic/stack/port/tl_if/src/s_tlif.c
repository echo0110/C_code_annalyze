/***************************************************************************
 *  FILE NAME  : s_tlif.c
 *
 *  DESCRIPTION: Contains the Functions for TL interaface module.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  13Oct00    hbhondwe     IWAVE        Linux
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *  27Nov03    Vivek Bansal Rel 5.0      Changes for CSPL
 *  7 JULY08   Shikha KathpalRel 6.2.0    RFC 4960 Upgrade
 *  24May10    Rajnesh      SPR 20411    Warning Fixed
 *  25mar11    Akanksha         SPR 20859    Enhancement for Callback
 * 7Jul11   Vaibhav Banga                   klocwork changes for IBM
 *  1May13    Pranav       SPR 21127    CSR 40731- GCC 4.7 warning removal
 *  25Oct13   Pinky Sinha  SPR 21157    CSR 56258
 *  25Feb14    Rohan Pawa                 CSR 69326
 *  15Sep14   Priyanka Mishra   Bug Id 22     CSR 85223
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "TL"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_mmmain.h>
#include <s_osmem.h>
#include <s_esdef.h>
#include <s_mmmain.h>
#include <s_cobuf.h>
#include <s_dbg.h>
#include <s_es.h>
#include <s_red.h>
#include <s_in.h>
#include <s_tlif.h>
#include <s_ostmr.h>
#include <s_os.h>

#ifdef SCTP_LOAD_TEST

extern struct timeval up_s_time;
extern struct timeval down_e_time;
extern struct timeval down_s_time;
extern sctp_Boolean_t record_downtime;
extern sctp_U32bit    dt_count;
extern unsigned long min_downtime;
extern unsigned long max_downtime;
extern unsigned long avg_downtime;

#endif

/***************************************************************************
 *    FUNCTION :
 *        sctp_TL_socket_close
 *
 *    DESCRIPTION:
 *        Calls the appropriate TL/UDP function to close a socket
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_TL_socket_close(
    sctp_U32bit       socket_id )
{
#ifndef SCTP_TL_EXTERNAL_SUPPORT /*SPR 20859 */
    sig_stack_type_et   stack_type;

#ifndef SCTP_CLI_RECV_COM_THREAD
    stack_type = SIG_SCTP_RECV_STACK;
#else
    stack_type = SIG_SCTP_STACK;
#endif
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_TL_socket_close" ) );

    if( SIGTRAN_FAILURE ==
            sig_close_comm_channel( stack_type, ( sctp_U16bit )socket_id ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSocket close failed" ) );
        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
#else
    SCTP_UNUSED_VARIABLE( socket_id );
    return ( SCTP_SUCCESS );
#endif
}

/******************************************************************************
**  Function Name       : sctp_TL_open_socket
**
**    Description       :Calls the appropriate TL/UDP function to open a socket
**
**    Returns           :
**
**        SCTP_SUCCESS
**        SCTP_FAILURE
**
******************************************************************************/
sctp_return_t sctp_TL_open_socket(
    sctp_sa_family_t      af,
    sctp_U32bit           type,
    sctp_U32bit           protocol_id,
    sctp_sockaddr_st      *p_addr,
    sctp_U16bit           port,
    sctp_Boolean_t        is_read_fd,
    sctp_U32bit           *p_socket_id )
{
#ifndef SCTP_TL_EXTERNAL_SUPPORT /*SPR 20859 */
    sig_soc_type_et     socket_type = SIG_SOC_TYPE_INVALID;
    sig_ip_addr_st      local_sig_addr;
    sig_ip_addr_st      *p_local_sig_addr = &local_sig_addr;
    sig_stack_type_et   stack_type;
    sigtran_error_t     ecode;
    sctp_U16bit         dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nsctp_TL_open_socket" ) );

    /*
     * Depending upon the family, type and the protocol id, set the socket type
     * sigtran common.
     */
    if( AF_INET == af )
    {
        if( SOCK_RAW == type )
        {
            if( IPPROTO_SCTP == protocol_id )
            {
                socket_type = SIG_SOC_TYPE_IPV4;
            }

            else if( IPPROTO_ICMP == protocol_id )
            {
                socket_type = SIG_SOC_TYPE_ICMPV4;
            }
        }

        else if( SOCK_DGRAM == type )
        {
            socket_type = SIG_SOC_TYPE_UDP_NW;
        }
    }

#ifdef SCTP_INET6_SUPPORT

    else if( AF_INET6 == af )
    {
        if( SOCK_RAW == type )
        {
            if( IPPROTO_SCTP == protocol_id )
            {
                socket_type = SIG_SOC_TYPE_IPV6;
            }

            else if( IPPROTO_ICMP == protocol_id )
            {
                socket_type = SIG_SOC_TYPE_ICMPV6;
            }
        }
    }

#endif

    if( SCTP_NULL != p_addr )
    {
        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( ( *p_addr ), port, local_sig_addr );
    }

    else
    {
        p_local_sig_addr = SCTP_NULL;
    }

#ifndef SCTP_CLI_RECV_COM_THREAD
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_RECV_STACK" ) );
    stack_type = SIG_SCTP_RECV_STACK;
#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_STACK" ) );
    stack_type = SIG_SCTP_STACK;
#endif

    if( SIGTRAN_FAILURE ==
            sig_open_comm_channel( stack_type, SIG_SOC_MODE_SERVER,
                                   socket_type, ( sigtran_U8bit )is_read_fd, SIGTRAN_FALSE, p_local_sig_addr,
                                   SIGTRAN_NULL, ( sig_read_cbk_func_ptr_t )
                                   sctp_TL_recv_data, &dest_id, &ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nOpen comm channel failed" ) );
        return ( SCTP_FAILURE );
    }

    *p_socket_id = dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Success in opening socket id = [%u]", dest_id ) );
    return SCTP_SUCCESS;
#else
    SCTP_UNUSED_VARIABLE( af );
    SCTP_UNUSED_VARIABLE( type );
    SCTP_UNUSED_VARIABLE( protocol_id );
    SCTP_UNUSED_VARIABLE( port );
    SCTP_UNUSED_VARIABLE( is_read_fd );
    SCTP_UNUSED_VARIABLE( p_addr );
    SCTP_UNUSED_VARIABLE( p_socket_id );
    return SCTP_SUCCESS;
#endif

}

/***************************************************************************
 *    FUNCTION :
 *        sctp_TL_send_data
 *
 *    DESCRIPTION:
 *        Calls the appropriate UDP/TL function to send data over a socket.
 *    This is the exit point in the stack.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_TL_send_data(
    sctp_U32bit         socket_id,
    sctp_U8bit          *buffer,
    sctp_U32bit         buffer_len,
    sctp_S32bit         send_flags,
    sctp_sockaddr_st    *p_addr,
    sctp_U16bit         dest_port )
{
#ifndef SCTP_TL_EXTERNAL_SUPPORT /*SPR 20859 */
    sigtran_error_t     ecode;
    sig_stack_type_et   stack_type;


#ifdef SCTP_LOAD_TEST
    sctp_U32bit   time_diff;
#endif
    sig_iovec_ipc_buffer_st *p_ipc_buffer = NULL;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( send_flags );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "TL_send_data, socket id = %u, buffer len = %u, TO ->",
                  socket_id, buffer_len ) );

    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_addr );

    SCTP_HEX_DUMP( SCTP_NULL, SCTP_INTF_TRC, buffer, buffer_len );

    p_ipc_buffer = ( sig_iovec_ipc_buffer_st * ) __MESG_MALLOC( SCTP_PDU_SEND_BUFFER_POOL,
                                                                sizeof( sig_iovec_ipc_buffer_st ) );

    if( p_ipc_buffer == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_TL_data_ind_inet : could not get buffer" ) );

        return( SCTP_FAILURE );
    }

#ifndef SCTP_CLI_RECV_COM_THREAD
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_RECV_STACK" ) );
    stack_type = SIG_SCTP_RECV_STACK;
#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_STACK" ) );
    stack_type = SIG_SCTP_STACK;
#endif

    if( p_addr->family == SCTP_AF_INET )
    {
        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( ( *p_addr ), dest_port, p_ipc_buffer->ip_addr );

#ifdef SCTP_LOAD_TEST

        if( dt_count > 0 )
        {
            __GET_ABSOLUTE_SYSTIME( &down_e_time );

            time_diff = ( down_e_time.tv_sec - down_s_time.tv_sec - 1 ) * 1000000;
            time_diff += down_e_time.tv_usec;
            time_diff += ( 1000000 - down_s_time.tv_usec );

            if( time_diff < min_downtime )
            {
                min_downtime = time_diff;
            }

            if( time_diff > max_downtime )
            {
                max_downtime = time_diff;
            }

            avg_downtime += time_diff;
            record_downtime = SCTP_FALSE;
            dt_count--;
        }

#endif
        p_ipc_buffer->b_is_iovec = SIGTRAN_FALSE;
        p_ipc_buffer->p_buffer = buffer;
        p_ipc_buffer->dest_id = socket_id;
        p_ipc_buffer->buffer_size = buffer_len;

        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_TL,
                                         SOC_MGR_INVALID_HANDLE, ( sigtran_pvoid_t ) p_ipc_buffer,
                                         buffer_len, &ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in sending data" ) );
            return SCTP_FAILURE;
        }
    }

    else if( p_addr->family == SCTP_AF_INET6 )
    {
#ifdef SCTP_INET6_SUPPORT

        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( ( *p_addr ), dest_port, p_ipc_buffer->ip_addr );

        p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.flow_info = 0;
        p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.scope_id = 0;

        p_ipc_buffer->b_is_iovec = 0;
        p_ipc_buffer->p_buffer = buffer;
        p_ipc_buffer->dest_id = socket_id;
        p_ipc_buffer->buffer_size = buffer_len;

        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_TL,
                                         SOC_MGR_INVALID_HANDLE, ( sigtran_pvoid_t ) p_ipc_buffer,
                                         buffer_len, &ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in sending data" ) );
            return SCTP_FAILURE;
        }

#else
        /* IPv6 is not supported */
        /* CSR 69326 fix starts*/
        __FREE( p_ipc_buffer );
        /* CSR 69326 fix ends*/
        return ( SCTP_FAILURE );
#endif /* SCTP_INET6_SUPPORT */
    }

    /* CSR 69326 fix starts */
    else
    {
#ifdef SIG_COMMON_WITH_CSPL
        __FREE( p_ipc_buffer );
#endif
    }

#ifndef SIG_COMMON_WITH_CSPL
    __FREE( p_ipc_buffer );
#endif
    /* CSR 69326 fix ends */
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nSuccess in sending data" ) );
    return ( SCTP_SUCCESS );
#else
    SCTP_UNUSED_VARIABLE( socket_id );
    SCTP_UNUSED_VARIABLE( buffer_len );
    SCTP_UNUSED_VARIABLE( send_flags );
    SCTP_UNUSED_VARIABLE( dest_port );
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( p_addr );
    return ( SCTP_SUCCESS );
#endif
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_TL_send_iovec
 *
 *    DESCRIPTION:
 *        Calls the appropriate UDP/TL function to send data over a socket.
 *    This is the exit point in the stack.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_TL_send_iovec(
    sctp_U32bit         socket_id,
    sctp_iovec_st       *p_iovec,
    sctp_U32bit         iovec_len,
    sctp_S32bit         send_flags,
    sctp_sockaddr_st    *p_addr,
    sctp_U16bit         dest_port )
{

#ifndef SCTP_TL_EXTERNAL_SUPPORT /*SPR 20859 */
    sig_stack_type_et       stack_type;
    sig_iovec_ipc_buffer_st *p_ipc_buffer = SCTP_NULL;
    sigtran_error_t         ecode;

#ifdef SCTP_LOAD_TEST
    sctp_U32bit   time_diff;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( send_flags );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "TL_send_data, socket id = %u, buffer len = %u, TO ->",
                  socket_id, iovec_len ) );

    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_addr );

    SCTP_HEX_DUMP_IOV( SCTP_NULL, SCTP_INTF_TRC, p_iovec, iovec_len );

    p_ipc_buffer = ( sig_iovec_ipc_buffer_st * ) __MESG_MALLOC( SCTP_PDU_SEND_BUFFER_POOL,
                                                                sizeof( sig_iovec_ipc_buffer_st ) );

    if( p_ipc_buffer == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_TL_data_ind_inet : could not get buffer" ) );

        return( SCTP_FAILURE );
    }

    /*
     * Convert this into the zero buffer as defined by CSPL.
     */
#ifndef SCTP_CLI_RECV_COM_THREAD
    stack_type = SIG_SCTP_RECV_STACK;
#else
    stack_type = SIG_SCTP_STACK;
#endif

    if( p_addr->family == SCTP_AF_INET )
    {
        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( ( *p_addr ), dest_port, p_ipc_buffer->ip_addr );

#ifdef SCTP_LOAD_TEST

        if( dt_count > 0 )
        {
            __GET_ABSOLUTE_SYSTIME( &down_e_time );

            time_diff = ( down_e_time.tv_sec - down_s_time.tv_sec - 1 ) * 1000000;
            time_diff += down_e_time.tv_usec;
            time_diff += ( 1000000 - down_s_time.tv_usec );

            if( time_diff < min_downtime )
            {
                min_downtime = time_diff;
            }

            if( time_diff > max_downtime )
            {
                max_downtime = time_diff;
            }

            avg_downtime += time_diff;
            record_downtime = SCTP_FALSE;
            dt_count--;
        }

#endif
        p_ipc_buffer->b_is_iovec = SIGTRAN_TRUE;
        p_ipc_buffer->p_buffer = p_iovec;
        p_ipc_buffer->dest_id = socket_id;
        p_ipc_buffer->buffer_size = iovec_len;

        /* SPR_20411_FIX_START -- */

        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_TL,
                                         SOC_MGR_INVALID_HANDLE, ( sigtran_pvoid_t ) p_ipc_buffer,
                                         sizeof( sig_iovec_ipc_buffer_st ), &ecode ) )
            /* SPR_20411_FIX_END -- */

        {
            /* SPR_20411_FIX_START -- */
            __MESG_FREE( p_ipc_buffer );
            /* SPR_20411_FIX_END -- */

            return SCTP_FAILURE;
        }
    }

    else if( p_addr->family == SCTP_AF_INET6 )
    {
#ifdef SCTP_INET6_SUPPORT
        SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR( ( *p_addr ), dest_port, p_ipc_buffer->ip_addr );
        p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.flow_info = 0;
        p_ipc_buffer->ip_addr.ip_addr_ut.addr_v6.scope_id = 0;

        p_ipc_buffer->b_is_iovec = SIGTRAN_TRUE;
        p_ipc_buffer->p_buffer = p_iovec;
        p_ipc_buffer->dest_id = socket_id;
        p_ipc_buffer->buffer_size = iovec_len;
        p_ipc_buffer->send_flags  = send_flags;

        /* SPR_20411_FIX_START -- */

        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_TL,
                                         SOC_MGR_INVALID_HANDLE, ( sigtran_pvoid_t ) p_ipc_buffer,
                                         sizeof( sig_iovec_ipc_buffer_st ), &ecode ) )
            /* SPR_20411_FIX_END -- */

        {
            /* SPR_20411_FIX_START -- */
            __MESG_FREE( p_ipc_buffer );
            /* SPR_20411_FIX_END -- */

            return SCTP_FAILURE;
        }

#else
        /* IPv6 is not supported */
        /* CSR 69326 fix starts*/
        __FREE( p_ipc_buffer );
        /* CSR 69326 fix ends*/
        return ( SCTP_FAILURE );
#endif /* SCTP_INET6_SUPPORT */
    }

    /* CSR 69326 fix starts */
    else
    {
#ifdef SIG_COMMON_WITH_CSPL
        __FREE( p_ipc_buffer );
#endif
    }

#ifndef SIG_COMMON_WITH_CSPL
    __FREE( p_ipc_buffer );
#endif
    /* CSR 69326 fix ends */
    return ( SCTP_SUCCESS );
#else
    SCTP_UNUSED_VARIABLE( socket_id );
    SCTP_UNUSED_VARIABLE( iovec_len );
    SCTP_UNUSED_VARIABLE( send_flags );
    SCTP_UNUSED_VARIABLE( dest_port );
    SCTP_UNUSED_VARIABLE( p_iovec );
    SCTP_UNUSED_VARIABLE( p_addr );
    return ( SCTP_SUCCESS );
#endif

}

/***************************************************************************
 *    FUNCTION :
 *        sctp_TL_resolve_hostname
 *
 *    DESCRIPTION:
 *        This function handle hostname with ipv4 addresses.
 *    For IPV6 addresses :
 *        Use the function getipnodesbyname(). This function can only be found
 *    in nodes that support IPV6 and so this cannot replace the gethostbyname
 *    functionality coded below. Getting IPV6 addresses by Hostname is not
 *    supported right now.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_TL_resolve_hostname(
    sctp_U8bit            *hostname,
    sctp_U32bit           *num_of_addrs,
    sctp_sockaddr_list_st addr_list )
{
#ifndef SCTP_TL_EXTERNAL_SUPPORT /* SPR 20859 */
    sig_ip_addr_st      a_sig_ip_addrs[SCTP_MAX_TRANSPORT_ADDR];
    sigtran_error_t     sig_error;
    sctp_U32bit         count;
    sctp_U16bit         port_num;

    *num_of_addrs = SCTP_MAX_TRANSPORT_ADDR;

    if( sig_resolve_hostname( hostname, a_sig_ip_addrs, num_of_addrs,
                              &sig_error ) != SIGTRAN_SUCCESS )
    {
        return SCTP_FAILURE;
    }

    for( count = 0; count < *num_of_addrs; count++ )
    {
        SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR( a_sig_ip_addrs[count],
                                         addr_list[count], port_num );
    }


    /* SPR 21127 CHANGES START */
    SCTP_UNUSED_VARIABLE( port_num );
    /* SPR 21127 CHANGES END */
    return ( SCTP_SUCCESS );
#else
    SCTP_UNUSED_VARIABLE( hostname );
    SCTP_UNUSED_VARIABLE( num_of_addrs );
    SCTP_UNUSED_VARIABLE( addr_list );
    return ( SCTP_SUCCESS );
#endif
}

/******************************************************************************
**  Function Name       : sctp_TL_recv_data
**
**    Description       : This function is responsible for receiving messages
**                        from the lower layer. This function gets invoked when
**                        -ever message arraives for SCTP lower layer sockets.
**
**    Returns           :SCTP_SUCCESS/SCTP_FAILURE
**
******************************************************************************/
sigtran_return_t    sctp_TL_recv_data(
    sigtran_U32bit      handle_id,
    sig_sock_mode_et    socket_mode,
    sig_soc_type_et     socket_type,
    sig_sock_fd_t       socket_id,
    sig_q_module_t      *p_from_q_module,
    sigtran_pvoid_t     *p_data,
    sigtran_U32bit      *p_data_len,
    sigtran_error_t     *p_ecode )
{
#ifndef SCTP_TL_EXTERNAL_SUPPORT /* SPR 20859 */
    sig_iovec_ipc_buffer_st *p_ipc_buffer;
    sctp_TL_packet_st       *sctp_packet   = SCTP_NULL;
    sigtran_error_t         ecode;
    sig_stack_type_et       stack_type;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( socket_mode );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nReceiving data from transport layer" ) );

#ifndef SCTP_CLI_RECV_COM_THREAD
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_RECV_STACK" ) );
    stack_type = SIG_SCTP_RECV_STACK;
#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_STACK" ) );
    stack_type = SIG_SCTP_STACK;
#endif

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nDropped messages received in uninitialisaed state" ) );
        return ( SCTP_FAILURE );
    }

    p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )__MESG_MALLOC( SCTP_PDU_SEND_BUFFER_POOL,
                                                               sizeof( sig_iovec_ipc_buffer_st ) );

    if( SCTP_NULL == p_ipc_buffer )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in allocating memory" ) );
        *p_ecode = ESIG_MEM_ALLOC_FAILED;
        return SIGTRAN_FAILURE;
    }

    SCTP_TL_GETBUF( sctp_packet );

    if( SCTP_NULL == sctp_packet )
    {
        __MESG_FREE( p_ipc_buffer );
        *p_ecode = ESIG_MEM_ALLOC_FAILED;
        return SIGTRAN_FAILURE;
    }

    if( SIGTRAN_FAILURE ==
            sig_read( stack_type, socket_type, socket_id,
                      sizeof( sctp_packet->buffer ), SIG_RECV_ALL, &( p_ipc_buffer->ip_addr ),
                      ( sigtran_pvoid_t )( sctp_packet->buffer ), &( p_ipc_buffer->buffer_size ),
                      &ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nRead from socket failed" ) );
        __MESG_FREE( p_ipc_buffer );
        SCTP_TL_ADDBUF( sctp_packet );
        return SIGTRAN_FAILURE;
    }

    if( SIGTRAN_FAILURE ==
            sig_get_q_module( stack_type, SIG_DEST_TYPE_TL, SOC_MGR_INVALID_HANDLE,
                              p_from_q_module,
                              &ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nGet QMODULE failed for TL" ) );
        __MESG_FREE( p_ipc_buffer );
        SCTP_TL_ADDBUF( sctp_packet );
        return SIGTRAN_FAILURE;
    }

    p_ipc_buffer->sock_type = socket_type;
    p_ipc_buffer->dest_id = handle_id;
    p_ipc_buffer->p_buffer = ( sigtran_pvoid_t )sctp_packet;
    *p_data = p_ipc_buffer;
    *p_data_len = p_ipc_buffer->buffer_size;

    SCTP_HEX_DUMP( SCTP_NULL, SCTP_INTF_TRC, sctp_packet->buffer,
                   p_ipc_buffer->buffer_size );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nData recvd successfully" ) );
#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE

    while( SCTP_SUCCESS == sctp_remove_notifications_from_queue() );

#endif
#endif
#endif
    return SCTP_SUCCESS;
#else
    SCTP_UNUSED_VARIABLE( handle_id );
    SCTP_UNUSED_VARIABLE( socket_mode );
    SCTP_UNUSED_VARIABLE( socket_type );
    SCTP_UNUSED_VARIABLE( socket_id );
    SCTP_UNUSED_VARIABLE( p_from_q_module );
    SCTP_UNUSED_VARIABLE( p_data );
    SCTP_UNUSED_VARIABLE( p_data_len );
    SCTP_UNUSED_VARIABLE( p_ecode );
    return ( SCTP_SUCCESS );
#endif

}

/******************************************************************************
**  Function Name       : sctp_process_TL_data_ind_inet
**
**    Description       : This function is responsible for processing the data
**                        received on the raw socket.
**
**    Returns           :void
**
******************************************************************************/
sctp_void_t sctp_process_TL_data_ind_inet(
    sctp_TL_packet_st   *p_sctp_packet,
    sctp_U32bit         buffer_size,
    sctp_sockaddr_st    *p_from_addr,
    sctp_U16bit         from_port_num,
    sctp_sockaddr_st    *p_on_addr,
    sctp_U16bit         on_port_num )
{
    SCTP_IP4                *ipheader = SCTP_NULL;
    /*Bug Id 22 fix starts*/
    /*sctp_Boolean_t          CE_bit  = 0;*/
    sctp_Boolean_t          CE_bit  = SCTP_FALSE;
    /*Bug Id 22 fix ends*/
    sctp_U32bit             ip_header_offset = 0;
#ifdef SCTP_RED_SUPPORT
    sctp_sockaddr_st        tmp_addr;
    sctp_U8bit              *p_red_buffer;
    sctp_U8bit              *p_temp_buffer;
    sctp_U32bit             nooctets;
    sctp_error_t            ecode;
    sig_stack_type_et       stack_type;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( from_port_num );
    SCTP_UNUSED_VARIABLE( p_on_addr );
    SCTP_UNUSED_VARIABLE( on_port_num );
    /*to avoid the warning*/

    /* klockwork fix 6.2.4 */
    if( buffer_size > SCTP_MAX_MTUSIZE )
    {
        return ;
    }

    /* klockwork fix 6.2.4 */

    ipheader = ( SCTP_IP4 * ) & ( ( p_sctp_packet->buffer )[0] );

    /* rfc 4960 upgrade  */
    if( ( ipheader->ip_tos & SCTP_ECN_BIT ) == SCTP_ECN_BIT ) /* ECN and CE bit combined into ECN field */
    {
        CE_bit = SCTP_TRUE;
    }

    else
    {
        CE_bit = SCTP_FALSE;
    }

    /* rfc 4960 upgrade end */
    /* calculate ip    header offset */
    ip_header_offset = 4 * ipheader->ip_hl;

    /* Indicate that IP header is present */
    p_sctp_packet->ip_header_present = SCTP_TRUE;

    /* send this to main module for processing */

    p_sctp_packet->dst_addr.family     = SCTP_AF_INET;
    p_sctp_packet->dst_addr.ip.v4.addr = ntohl( ipheader->ip_dst.s_addr );

    p_sctp_packet->src_addr.family      = SCTP_AF_INET;
    p_sctp_packet->src_addr.ip.v4.addr  = p_from_addr->ip.v4.addr;

    p_sctp_packet->CE_bit =  CE_bit;

#ifdef SCTP_RED_SUPPORT

    /*
     * If the redundancy is enabled, then send the message to the Redundancy
     * Manager. The redundancy manager will re-route the messages to the stack
     * using events mechnism.
     */
    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_ACTIVE )
            && ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) )
    {
        p_red_buffer = ( sctp_U8bit * )__MESG_MALLOC( SCTP_APIS_MESG_POOL,
                                                      SCTP_MAX_RED_MEM );

        if( SCTP_NULL == p_red_buffer )
        {
            return;
        }

        p_temp_buffer = p_red_buffer;

#ifndef SCTP_CLI_RECV_COM_THREAD
        stack_type = SIG_SCTP_RECV_STACK;
#else
        stack_type = SIG_SCTP_STACK;
#endif
        /*
         * Do this processing only if the stack is active. In case of
         * redundant stack, the message is droped.
         */

        /* set event type into the event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_EVT_PEER_MSG );

        /* changes to include length after event type*/

        p_temp_buffer = p_temp_buffer + 4;

        /* set ip header offset into the event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ip_header_offset );

        /* set size of peer msg into the event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, buffer_size );

        /*
         * set the address from which msg is received, into the sending
         * event buffer
         */
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_from_addr );

        /*
         * set the address on which msg is recvd, into the sending event
         * buffer.
         */

        tmp_addr.family     = SCTP_AF_INET;
        tmp_addr.ip.v4.addr = ntohl( ipheader->ip_dst.s_addr );
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, &tmp_addr );

        /* set the CE_bit into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ( sctp_U32bit )CE_bit );

        /* set the Cookie resolver flag into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_FALSE );

        /* set whether IP header present into the sending event buffer*/
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_TRUE );

        /* set the p_sctp_packet data into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer,
                                                buffer_size, &( ( p_sctp_packet->buffer )[0] ) );

        nooctets = p_temp_buffer - p_red_buffer;

        p_temp_buffer = p_red_buffer + 4;

        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               nooctets );

        /*
         * Assuming that the module Id of the Redundancy Manager is same
         * as that of SU, then send the message.
         */
        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_SM,
                                         g_sctp_rm_dest_id, ( sigtran_pvoid_t ) p_red_buffer,
                                         nooctets, &ecode ) )
        {
            __MESG_FREE( p_red_buffer );
            return ;
        }

        SCTP_HEX_DUMP( SCTP_NULL, SCTP_INTF_TRC, ( sctp_U8bit * )p_red_buffer, 1 );
    } /* if (sctp_stack_red_state == SC... */

    SCTP_TL_ADDBUF( p_sctp_packet );
#else

    /*
     * The p_sctp_packet element contains the source and destination address,
     * these are extracted from the IP header.
     */
    /* klocwork changes for IBM */
    if( ( ip_header_offset < buffer_size ) && ( buffer_size <= SCTP_MAX_MTUSIZE ) )
    {
        /* klocwork changes for IBM */
        sctp_process_TL_datagram( ip_header_offset, p_sctp_packet, buffer_size,
                                  SCTP_FALSE );
        /* klocwork changes for IBM */
    }

    /* klocwork changes for IBM */
#ifdef SCTP_NAME_RESOLV_THREAD
    sctp_process_cookie_packets();
#endif

#endif

}

/******************************************************************************
**  Function Name       : sctp_process_TL_data_ind_inet6
**
**    Description       :This function is used to process INET6 data only
**
**    Returns           :void
**
******************************************************************************/
#ifdef SCTP_INET6_SUPPORT
sctp_void_t sctp_process_TL_data_ind_inet6(
    sctp_TL_packet_st   *p_sctp_packet,
    sctp_U32bit         buffer_size,
    sctp_sockaddr_st    *p_from_addr,
    sctp_U16bit         from_port_num,
    sctp_sockaddr_st    *p_on_addr,
    sctp_U16bit         on_port_num )
{
    /*Bug Id 22 fix starts*/
    /*sctp_Boolean_t      CE_bit  = 0;*/
    sctp_Boolean_t      CE_bit  = SCTP_FALSE;
    /*Bug Id 22 fix ends*/
    sctp_U32bit         ip_header_offset = 0;
#ifdef SCTP_RED_SUPPORT
    sctp_U8bit         *p_red_buffer;
    sctp_U8bit         *p_temp_buffer;
    sctp_U32bit        nooctets;
    sctp_error_t       ecode;
    sig_stack_type_et  stack_type;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( from_port_num );
    SCTP_UNUSED_VARIABLE( on_port_num );
    /*to avoid the warning*/



    /* No CE bit present */
    CE_bit = SCTP_FALSE;

    /* calculate ip header offset: ip6_plen is in octets */
    ip_header_offset = 0;

    /* Indicate that IP header is present */
    p_sctp_packet->ip_header_present = SCTP_FALSE;

#ifdef SCTP_RED_SUPPORT

    /*
     * If the redundancy is enabled, then send the message to the Redundancy
     * Manager. The redundancy manager will re-route the messages to the stack
     * using events mechnism.
     */
    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_ACTIVE )
            && ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) )
    {
        p_red_buffer = ( sctp_U8bit * )__MESG_MALLOC( SCTP_APIS_MESG_POOL,
                                                      SCTP_MAX_RED_MEM );

        if( SCTP_NULL == p_red_buffer )
        {
            return;
        }

        p_temp_buffer = p_red_buffer;

#ifndef SCTP_CLI_RECV_COM_THREAD
        stack_type = SIG_SCTP_RECV_STACK;
#else
        stack_type = SIG_SCTP_STACK;
#endif
        /*
         * Do this processing only if the stack is active. In case of
         * redundant stack, the message is droped.
         */

        /* set event type into the event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_EVT_PEER_MSG );

        /* changes to include length after event type*/

        p_temp_buffer = p_temp_buffer + 4;


        /* set ip header offset into the event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ip_header_offset );

        /* set size of peer msg into the event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, buffer_size );

        /*
         * set the address from which msg is received, into the sending
         * event buffer
         */

        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_from_addr );

        /*
         * set the address on which msg is recvd, into the sending event
         * buffer
         */
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_on_addr );

        /* set the CE_bit into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ( sctp_U32bit )CE_bit );

        /* set the Cookie resolver flag into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ( sctp_U32bit )SCTP_FALSE );

        /* set whether IP header present into the sending event buffer*/
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ( sctp_U32bit )SCTP_TRUE );

        /* set the p_sctp_packet data into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer,
                                                buffer_size, &( ( p_sctp_packet->buffer )[0] ) );

        nooctets = p_temp_buffer - p_red_buffer;

        p_temp_buffer = p_red_buffer + 4;

        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               nooctets );

        /*
         * Assuming that the module Id of the Redundancy Manager is same
         * as that of SU, then send the message.
         */
        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_SM,
                                         g_sctp_rm_dest_id, ( sigtran_pvoid_t ) p_red_buffer,
                                         nooctets, &ecode ) )
        {
            __MESG_FREE( p_red_buffer );
            return ;
        }
    }

    SCTP_TL_ADDBUF( p_sctp_packet );

#else

    /* send this to main module for processing */
    p_sctp_packet->dst_addr.family     = SCTP_AF_INET6;

    /*
     * Set the address on which the message has been received.
     */
    p_sctp_packet->dst_addr.ip.v6.addr.s_u6_addr32[0] = p_on_addr->ip.v6.addr.s_u6_addr32[0];
    p_sctp_packet->dst_addr.ip.v6.addr.s_u6_addr32[1] = p_on_addr->ip.v6.addr.s_u6_addr32[1];
    p_sctp_packet->dst_addr.ip.v6.addr.s_u6_addr32[2] = p_on_addr->ip.v6.addr.s_u6_addr32[2];
    p_sctp_packet->dst_addr.ip.v6.addr.s_u6_addr32[3] = p_on_addr->ip.v6.addr.s_u6_addr32[3];

    /*
     * Set the address from which the message has been received.
     */
    p_sctp_packet->src_addr.family     = SCTP_AF_INET6;
    p_sctp_packet->src_addr.ip.v6.addr.s_u6_addr32[0] = p_from_addr->ip.v6.addr.s_u6_addr32[0];
    p_sctp_packet->src_addr.ip.v6.addr.s_u6_addr32[1] = p_from_addr->ip.v6.addr.s_u6_addr32[1];
    p_sctp_packet->src_addr.ip.v6.addr.s_u6_addr32[2] = p_from_addr->ip.v6.addr.s_u6_addr32[2];
    p_sctp_packet->src_addr.ip.v6.addr.s_u6_addr32[3] = p_from_addr->ip.v6.addr.s_u6_addr32[3];

    /* Ipv6 does not have a CE bit */
    p_sctp_packet->CE_bit =  SCTP_FALSE;

    /*
     * The p_sctp_packet element contains the source and destination address,
     * these are extracted from the IP header.
     */
    sctp_process_TL_datagram( ip_header_offset, p_sctp_packet, buffer_size,
                              SCTP_FALSE );
#ifdef SCTP_NAME_RESOLV_THREAD
    sctp_process_cookie_packets();
#endif
#endif


    /* SPR 21127 change start */
    SCTP_UNUSED_VARIABLE( CE_bit );
    /* SPR 21127 change start */
}
#endif

/******************************************************************************
**  Function Name       : sctp_process_TL_data_ind_udp
**
**    Description       :Handles UDP data only
**
**    Returns           :void
**
******************************************************************************/
sctp_void_t sctp_process_TL_data_ind_udp(
    sctp_TL_packet_st   *p_sctp_packet,
    sctp_U32bit         buffer_size,
    sctp_sockaddr_st    *p_from_addr,
    sctp_U16bit         from_port_num,
    sctp_sockaddr_st    *p_on_addr,
    sctp_U16bit         on_port_num )
{
#ifdef SCTP_RED_SUPPORT
    sctp_U8bit              *p_red_buffer;
    sctp_U8bit              *p_temp_buffer;
    sctp_U32bit             nooctets;
    sctp_error_t            ecode;
    sig_stack_type_et       stack_type;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( from_port_num );
    SCTP_UNUSED_VARIABLE( on_port_num );
    /*to avoid the warning*/

    /* Indicate that IP header is not present */
    p_sctp_packet->ip_header_present = SCTP_FALSE;

#ifdef SCTP_RED_SUPPORT

    /*
     * If the redundancy is enabled, then send the message to the Redundancy
     * Manager. The redundancy manager will re-route the messages to the stack
     * using events mechnism.
     */
    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_ACTIVE )
            && ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) )
    {
        p_red_buffer = ( sctp_U8bit * )__MESG_MALLOC( SCTP_APIS_MESG_POOL,
                                                      SCTP_MAX_RED_MEM );

        if( SCTP_NULL == p_red_buffer )
        {
            return;
        }

        p_temp_buffer = p_red_buffer;

#ifndef SCTP_CLI_RECV_COM_THREAD
        stack_type = SIG_SCTP_RECV_STACK;
#else
        stack_type = SIG_SCTP_STACK;
#endif

        /* set event type into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_EVT_PEER_MSG );

        /* changes to include length after event type*/

        p_temp_buffer = p_temp_buffer + 4;

        /* set ip header offset into the sending event buffer */
        /* 0 is because SCTP is running Over UDP */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, 0 );

        /* set p_sctp_packet size into the buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, buffer_size );

        /*
         * set the address from which msg is received, into the
         * sending event buffer
         */
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_from_addr );

        /*
         * set the address on which msg is recvd, into the sending
         * event buffer.
         */
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_on_addr );

        /* set the CE_bit into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_FALSE );

        /*
         * set the Cookie resolver flag into the sending event
         * buffer.
         */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_FALSE );

        /*
         * set whether IP header present into the sending event
         * buffer.
         */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, ( sctp_U32bit )SCTP_FALSE );

        /* set the p_sctp_packet data into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer,
                                                buffer_size, &( ( p_sctp_packet->buffer )[0] ) );

        nooctets = p_temp_buffer - p_red_buffer;

        p_temp_buffer = p_red_buffer + 4;

        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               nooctets );


        /*
         * Assuming that the module Id of the Redundancy Manager is same
         * as that of SU, then send the message.
         */
        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_SM,
                                         g_sctp_rm_dest_id, ( sigtran_pvoid_t ) p_red_buffer,
                                         nooctets, &ecode ) )
        {
            __MESG_FREE( p_red_buffer );
            return ;
        }
    }

    SCTP_TL_ADDBUF( p_sctp_packet );
#else

    /* send this to main module for processing */
    p_sctp_packet->dst_addr.family     = SCTP_AF_INET;
    p_sctp_packet->dst_addr            = *p_on_addr;
    p_sctp_packet->src_addr.family     = SCTP_AF_INET;
    p_sctp_packet->src_addr.ip.v4.addr = p_from_addr->ip.v4.addr;

    p_sctp_packet->CE_bit = SCTP_FALSE;

    sctp_process_TL_datagram( 0, /* No IP header in this case */
                              p_sctp_packet, buffer_size, SCTP_FALSE );
#ifdef SCTP_NAME_RESOLV_THREAD
    sctp_process_cookie_packets();
#endif

#endif

    return;
}

/******************************************************************************
**  Function Name       : sctp_process_TL_data_ind_icmp4
**
**    Description       : Handles ICMP V4 data only
**
**    Returns           : void
**
******************************************************************************/
sctp_void_t sctp_process_TL_data_ind_icmp4(
    sctp_TL_packet_st   *p_sctp_packet,
    sctp_U32bit         buffer_size,
    sctp_sockaddr_st    *p_from_addr,
    sctp_U16bit         from_port_num,
    sctp_sockaddr_st    *p_on_addr,
    sctp_U16bit         on_port_num )
{
#ifdef SCTP_RED_SUPPORT
    sctp_U8bit              *p_red_buffer;
    sctp_U8bit              *p_temp_buffer;
    sctp_U32bit             nooctets;
    sctp_error_t            ecode;
    sig_stack_type_et       stack_type;
#endif


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( from_port_num );
    SCTP_UNUSED_VARIABLE( p_on_addr );
    SCTP_UNUSED_VARIABLE( on_port_num );

    /*to avoid the warning*/


#ifdef SCTP_RED_SUPPORT

    /*
     * If the redundancy is enabled, then send the message to the Redundancy
     * Manager. The redundancy manager will re-route the messages to the stack
     * using events mechnism.
     */
    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_ACTIVE )
            && ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) )
    {
        p_red_buffer = ( sctp_U8bit * )__MESG_MALLOC( SCTP_APIS_MESG_POOL,
                                                      SCTP_MAX_RED_MEM );

        if( SCTP_NULL == p_red_buffer )
        {
            return;
        }

        p_temp_buffer = p_red_buffer;

#ifndef SCTP_CLI_RECV_COM_THREAD
        stack_type = SIG_SCTP_RECV_STACK;
#else
        stack_type = SIG_SCTP_STACK;
#endif
        /*
         * Do this processing only if the stack is active. In case of
         * redundant stack, the message is droped.
         */

        /* set event type into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_EVT_ICMP_MSG );

        /*changes to include the total length =nooctets*/
        p_temp_buffer = p_temp_buffer + 4;

        /* set p_sctp_packet size into the buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, buffer_size );

        /*
         * set the address from which msg is received, into the sending
         * event buffer.
         */
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_from_addr );

        /* set the p_sctp_packet data into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer, buffer_size,
                                                &( ( p_sctp_packet->buffer )[0] ) );

        nooctets = p_temp_buffer - p_red_buffer;

        p_temp_buffer = p_red_buffer + 4;

        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, nooctets );


        /*
         * Assuming that the module Id of the Redundancy Manager is same
         * as that of SU, then send the message.
         */
        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_SM,
                                         g_sctp_rm_dest_id, ( sigtran_pvoid_t ) p_red_buffer,
                                         nooctets, &ecode ) )
        {
            __MESG_FREE( p_red_buffer );
            return ;
        }
    }

#else

    /* IWAVE: Remove ntohl */
    sctp_process_icmpV4_message( &( p_sctp_packet->buffer[0] ), buffer_size,
                                 p_from_addr );

#endif
    SCTP_TL_ADDBUF( p_sctp_packet );

}

/******************************************************************************
**  Function Name       : sctp_process_TL_data_ind_icmp6
**
**    Description       : Handles ICMP V6 data only
**
**    Returns           : void
**
******************************************************************************/
#ifdef SCTP_INET6_SUPPORT
sctp_void_t sctp_process_TL_data_ind_icmp6(
    sctp_TL_packet_st   *p_sctp_packet,
    sctp_U32bit         buffer_size,
    sctp_sockaddr_st    *p_from_addr,
    sctp_U16bit         from_port_num,
    sctp_sockaddr_st    *p_on_addr,
    sctp_U16bit         on_port_num )
{
#ifdef SCTP_RED_SUPPORT
    sctp_U8bit              *p_red_buffer;
    sctp_U8bit              *p_temp_buffer;
    sctp_U32bit             nooctets;
    sctp_error_t            ecode;
    sig_stack_type_et       stack_type;
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( from_port_num );
    SCTP_UNUSED_VARIABLE( on_port_num );
    SCTP_UNUSED_VARIABLE( p_on_addr );
    /*to avoid the warning*/



#ifdef SCTP_RED_SUPPORT

    /*
     * If the redundancy is enabled, then send the message to the Redundancy
     * Manager. The redundancy manager will re-route the messages to the stack
     * using events mechnism.
     */
    if( ( sctp_stack_red_state == SCTP_STACK_RED_STATE_ACTIVE )
            && ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) )
    {
        p_red_buffer = ( sctp_U8bit * )__MESG_MALLOC( SCTP_APIS_MESG_POOL,
                                                      SCTP_MAX_RED_MEM );

        if( SCTP_NULL == p_red_buffer )
        {
            return;
        }

        p_temp_buffer = p_red_buffer;

        /*
         * Do this processing only if the stack is active. In case of
         * redundant stack, the message is droped.
         */
#ifndef SCTP_CLI_RECV_COM_THREAD
        stack_type = SIG_SCTP_RECV_STACK;
#else
        stack_type = SIG_SCTP_STACK;
#endif
        /* set event type into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_EVT_ICMP_MSG );

        /*changes to include total length = nooctets */
        p_temp_buffer = p_temp_buffer + 4;

        /* set p_sctp_packet size into the buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, buffer_size );

        /*
         * set the address from which msg is received, into the sending
         * event buffer.
         */
        p_temp_buffer = sctp_buffer_pack_sockaddr( p_temp_buffer, p_from_addr );

        /* set the p_sctp_packet data into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer, buffer_size,
                                                &( ( p_sctp_packet->buffer )[0] ) );

        nooctets = p_temp_buffer - p_red_buffer;

        p_temp_buffer = p_red_buffer + 4;

        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer, nooctets );


        /*
         * Assuming that the module Id of the Redundancy Manager is same
         * as that of SU, then send the message.
         */
        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_SM,
                                         g_sctp_rm_dest_id, ( sigtran_pvoid_t ) p_red_buffer,
                                         nooctets, &ecode ) )
        {
            __MESG_FREE( p_red_buffer );
            return ;
        }
    }

#else
    sctp_process_icmpV6_message( &( p_sctp_packet->buffer[0] ), buffer_size,
                                 p_from_addr );
#endif

    SCTP_TL_ADDBUF( p_sctp_packet );
}
#endif
