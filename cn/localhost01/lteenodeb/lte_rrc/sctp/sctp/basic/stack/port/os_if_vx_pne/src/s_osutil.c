/***************************************************************************
 *  FILE NAME  : s_osutil.c
 *
 *  DESCRIPTION: Contains the Functions for OS specific utilities.
 *
 *  DATE       NAME         REF#         REASON
 *  ------------------------------------------------------------------------
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  12Dec02    mrajpal                   rel 4.0
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "OS"

#include <s_osutil.h>
#include <s_comac.h>
#include <signal.h>
#include <s_err.h>


/***************************************************************************
 *    FUNCTION :
 *        sctp_conv_addr_os_to_sctp
 *
 *    DESCRIPTION:
 *        This function Converts the IP address from the OS format into the
 *    format used in the SCTP stack. In case one of the addresses is IPv6 but
 *    if the flag that system supports is not set then an error is returned.
 *    NOTE: In case the n_os_addr param is 1 then there is no ambiguity in the
 *    value of the port parameter (which is an out paramter). In case the
 *    n_os_addr is > 1 then the value given by *p_port shall be the port in
 *    the first address in the list of os type addresses.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_conv_addr_os_to_sctp(
    sctp_U32bit                n_os_addr,
    sctp_sockaddr_storage_st   *p_os_addrs,
    sctp_sockaddr_st           *p_sctp_addrs,
    sctp_port_t                *p_port,
    sctp_error_t               *p_ecode )
{
    sctp_U32bit              count      = 0;
    sctp_sockaddr_storage_st *p_storage = SCTP_NULL;

    if( !p_os_addrs || ( n_os_addr < 1 ) )
    {
        SCTP_SET_ECODE( p_ecode, EINV_PARAMS );

        return ( SCTP_FAILURE );
    }

    for( ; count < n_os_addr; count++ )
    {
        /* Convert from the OS format to SCTP format */
        p_storage = &( p_os_addrs[count] );

        if( SCTP_AF_INET6 == SOCKADDR_STORAGE_FAMILY( ( *p_storage ) ) )
        {
#ifndef SCTP_INET6_SUPPORT
            SCTP_SET_ECODE( p_ecode, EINV_ADDRS );
            return ( SCTP_FAILURE );
#else
            struct sockaddr_in6  *p_ip6 =
                ( struct sockaddr_in6 * ) p_storage;

            /* Means IPV6 is supported by the OS. convert the address */
            p_sctp_addrs[count].family = SCTP_AF_INET6;
            p_sctp_addrs[count].ip.v6.flow_info = 0;
            p_sctp_addrs[count].ip.v6.scope_id  = 0;
            p_sctp_addrs[count].ip.v6.addr.u6_addr32[0] =
                ( *p_ip6 ).sin6_addr.s6_addr32[0];
            p_sctp_addrs[count].ip.v6.addr.u6_addr32[1] =
                ( *p_ip6 ).sin6_addr.s6_addr32[1];
            p_sctp_addrs[count].ip.v6.addr.u6_addr32[2] =
                ( *p_ip6 ).sin6_addr.s6_addr32[2];
            p_sctp_addrs[count].ip.v6.addr.u6_addr32[3] =
                ( *p_ip6 ).sin6_addr.s6_addr32[3];

            /* Copy the port */
            if( ( count == 0 ) && !p_port )
            {
                *p_port = ( *p_ip6 ).sin6_port;
            }

#endif
        }

        else if( SOCKADDR_STORAGE_FAMILY( ( *p_storage ) ) == SCTP_AF_INET )
        {
            struct sockaddr_in  *p_in = ( struct sockaddr_in * )p_storage;

            p_sctp_addrs[count].family = SCTP_AF_INET;
            p_sctp_addrs[count].ip.v4.addr
                = ntohl( p_in->sin_addr.s_addr );

            /* Copy the port */
            if( !count && p_port )
            {
                *p_port = ntohs( p_in->sin_port );
            }
        }

        else
        {
            SCTP_SET_ECODE( p_ecode, E_UNSUPP_ADDR_TYPE );
            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_conv_addr_sctp_to_os
 *
 *    DESCRIPTION:This function Converts the IP address from the SCTP format
 *    to the OS format. In case one of the addresses is IPv6 but
 *    if the flag that system supports is not set then an error is returned.
 *    NOTE: In case the n_sctp_addr param is 1 then there is no ambiguity in
 *     the value of the port parameter (which is an out paramter).
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_conv_addr_sctp_to_os(
    sctp_U32bit                n_sctp_addr,
    sctp_sockaddr_st           *p_sctp_addrs,
    sctp_sockaddr_storage_st   *p_os_addrs,
    sctp_port_t                port,
    sctp_error_t               *p_ecode )
{
    sctp_U32bit              count      = 0;
    sctp_sockaddr_storage_st *p_storage = SCTP_NULL;

    if( !p_sctp_addrs || !p_os_addrs || ( n_sctp_addr < 1 ) )
    {
        SCTP_SET_ECODE( p_ecode, EINV_PARAMS );

        return ( SCTP_FAILURE );
    }

    for( ; count < n_sctp_addr; count++ )
    {
        /* Convert from the OS format to SCTP format */
        p_storage = &( p_os_addrs[count] );

        if( SCTP_AF_INET6 == p_sctp_addrs[count].family )
        {
#ifndef SCTP_INET6_SUPPORT
            SCTP_SET_ECODE( p_ecode, EINV_ADDRS );
            return ( SCTP_FAILURE );
#else
            struct sockaddr_in6  *p_ip6 =
                ( struct sockaddr_in6 * )( p_storage );

            /* Means IPV6 is supported by the OS. convert the address */
            p_ip6->sin6_family = SCTP_AF_INET6;
            p_ip6->sin6_port   = port;
            p_ip6->sin6_flowinfo = p_sctp_addrs[count].ip.v6.flow_info;
            p_ip6->sin6_addr._S6_un._S6_u32[0] =
                p_sctp_addrs[count].ip.v6.addr.u6_addr32[0];
            p_ip6->sin6_addr._S6_un._S6_u32[1] =
                p_sctp_addrs[count].ip.v6.addr.u6_addr32[1];
            p_ip6->sin6_addr._S6_un._S6_u32[2] =
                p_sctp_addrs[count].ip.v6.addr.u6_addr32[2];
            p_ip6->sin6_addr._S6_un._S6_u32[3] =
                p_sctp_addrs[count].ip.v6.addr.u6_addr32[3];

            p_ip6->sin6_scope_id =  p_sctp_addrs[count].ip.v6.scope_id;

#endif
        }

        else if( SCTP_AF_INET == p_sctp_addrs[count].family )
        {
            struct sockaddr_in  *p_in =
                ( struct sockaddr_in * )( p_storage );

            p_in->sin_family = SCTP_AF_INET;
            p_in->sin_port   = htons( port );
            p_in->sin_addr.s_addr = htonl( p_sctp_addrs[count].ip.v4.addr );
        }

        else
        {
            SCTP_SET_ECODE( p_ecode, E_UNSUPP_ADDR_TYPE );
            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}

#ifdef SCTP_ERROR_ENABLED
/*********************************************************************
for exception handling
*********************************************************************/
void sctp_os_sig_handler( int os_signal );

/***************************************************************************
 *    FUNCTION :
 *        sctp_os_set_signals
 *
 *    DESCRIPTION:
 *      Sets the signal handler.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_os_set_signals(
    sctp_Boolean_t             flag,
    sctp_error_t               *p_ecode )
{
    if( flag )
    {
        signal( SIGSEGV, sctp_os_sig_handler );
        signal( SIGBUS, sctp_os_sig_handler );
        signal( SIGILL, sctp_os_sig_handler );
    }

    else
    {
        signal( SIGSEGV, SIG_DFL );
        signal( SIGBUS, SIG_DFL );
        signal( SIGILL, SIG_DFL );
    }

    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_os_signal_handler
 *
 *    DESCRIPTION:
 *  Handles the Signal.
 *
 *    RETURN VALUE:
 *        Void
 **************************************************************************/
void sctp_os_sig_handler( int os_signal )
{
    sctp_signal_et  sctp_signal;
    sctp_error_t    ecode;

    switch( os_signal )
    {
        case SIGSEGV:
            sctp_signal = SCTP_SIGNAL_SEGMENTATION_FAULT;
            break;

        case SIGBUS:
            sctp_signal = SCTP_SIGNAL_BUS_ERROR;
            break;

        case SIGILL:
            sctp_signal = SCTP_SIGNAL_ILLEGAL_INSTRUCTION;
            break;

        default:
            sctp_signal = SCTP_SIGNAL_UNKNOWN;
    }

    sctp_err_default_exception_handler( sctp_signal, &ecode );
}
#endif


