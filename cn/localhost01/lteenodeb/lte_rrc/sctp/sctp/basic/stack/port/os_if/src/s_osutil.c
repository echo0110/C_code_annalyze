/***************************************************************************
 *  FILE NAME  : s_osutil.c
 *
 *  DESCRIPTION: Contains the Functions for OS specific utilities.
 *
 *  DATE       NAME         REF#         REASON
 *  ------------------------------------------------------------------------
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  20MAR13    Rohan Pawa   SPR 21107    CSR 24860
 *  05NOV13    Rohan Pawa   SPR 21160    CSR 56189
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "OS"

#define __EXTENSIONS__       1
#include <s_osutil.h>
#include <s_comac.h>
#include <signal.h>
#include <s_err.h>
#include <time.h>
#include <s_dbg.h>
#include <s_osmem.h> // SPR 21160 changes
/***************************************************************************
 *    FUNCTION :
 *        sctp_conv_addr_storg_os_to_sctp
 *
 *    DESCRIPTION:
 *        This function Converts the Storage address from the OS format into the
 *    format used in the SCTP stack. In case one of the addresses is IPv6 but
 *    the flag that system supports is not set then an error is returned.
 *    NOTE: In case the n_os_addr param is 1 then there is no ambiguity in the
 *    value of the port parameter (which is a out paramter) in case the
 *    n_os_addr is > 1 then the value given by *p_port shall be the port in the
 *    first address in the list of os type addresses.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_conv_addr_storg_os_to_sctp(
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

            if( ( ( ( ( *p_ip6 ).sin6_addr.s6_addr32[0] == 0 ) &&
                    ( *p_ip6 ).sin6_addr.s6_addr32[1] == 0 ) ) &&
                    ( ( ( *p_ip6 ).sin6_addr.s6_addr32[2] == 0x0000ffff ) ||
                      ( ( *p_ip6 ).sin6_addr.s6_addr32[2] == 0 ) ) )
            {
                /*Its an Ipv4-mapped address */
                p_sctp_addrs[count].family      = SCTP_AF_INET;
                p_sctp_addrs[count].ip.v4.addr  =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[3] );
            }

            else
            {
                /* Means IPV6 is supported by the OS. convert the address */
                p_sctp_addrs[count].family = SCTP_AF_INET6;
                p_sctp_addrs[count].ip.v6.flow_info = 0;
                p_sctp_addrs[count].ip.v6.scope_id  = 0;
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[0] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[0] );
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[1] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[1] );
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[2] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[2] );
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[3] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[3] );
            }

            /* Copy the port */
            if( ( count == 0 ) && p_port )
            {
                *p_port = ntohs( p_ip6->sin6_port );
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
 *        sctp_conv_addr_os_to_sctp
 *
 *    DESCRIPTION:
 *        This function Converts the IP address from the OS format into the
 *    format used in the SCTP stack. In case one of the addresses is IPv6 but
 *    the flag that system supports is not set then an error is returned.
 *    NOTE: In case the n_os_addr param is 1 then there is no ambiguity in the
 *    value of the port parameter (which is a out paramter) in case the
 *    n_os_addr is > 1 then the value given by *p_port shall be the port in the
 *    first address in the list of os type addresses.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_conv_addr_os_to_sctp(
    sctp_U32bit                n_os_addr,
    struct sockaddr            *p_os_addrs,
    sctp_sockaddr_st           *p_sctp_addrs,
    sctp_port_t                *p_port,
    sctp_error_t               *p_ecode )
{
    sctp_U32bit              count      = 0;
    sctp_sockaddr_storage_st *p_storage = SCTP_NULL;

    if( !p_os_addrs || ( ( n_os_addr < 1 ) && ( n_os_addr > SCTP_MAX_TRANSPORT_ADDR ) ) ) /* SPR 21107 changes */
    {
        SCTP_SET_ECODE( p_ecode, EINV_PARAMS );

        return ( SCTP_FAILURE );
    }

    for( ; count < n_os_addr; count++ )
    {
        /* Convert from the OS format to SCTP format */
        p_storage = ( sctp_sockaddr_storage_st * )p_os_addrs;

        if( SCTP_AF_INET6 == SOCKADDR_STORAGE_FAMILY( ( *p_storage ) ) )
        {
#ifndef SCTP_INET6_SUPPORT
            SCTP_SET_ECODE( p_ecode, EINV_ADDRS );
            return ( SCTP_FAILURE );
#else
            struct sockaddr_in6  *p_ip6 =
                ( struct sockaddr_in6 * ) p_storage;

            if( ( ( ( ( *p_ip6 ).sin6_addr.s6_addr32[0] == 0 ) &&
                    ( *p_ip6 ).sin6_addr.s6_addr32[1] == 0 ) ) &&
                    ( ( ( *p_ip6 ).sin6_addr.s6_addr32[2] == 0x0000ffff ) ||
                      ( ( *p_ip6 ).sin6_addr.s6_addr32[2] == 0 ) ) )
            {
                /*Its an Ipv4-mapped address */
                p_sctp_addrs[count].family      = SCTP_AF_INET;
                p_sctp_addrs[count].ip.v4.addr  =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[3] );
            }

            else
            {
                /* Means IPV6 is supported by the OS. convert the address */
                p_sctp_addrs[count].family = SCTP_AF_INET6;
                p_sctp_addrs[count].ip.v6.flow_info = 0;
                p_sctp_addrs[count].ip.v6.scope_id  = 0;
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[0] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[0] );
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[1] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[1] );
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[2] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[2] );
                p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[3] =
                    ntohl( ( *p_ip6 ).sin6_addr.s6_addr32[3] );
            }

            /* Copy the port */
            if( ( count == 0 ) && p_port )
            {
                *p_port = ntohs( p_ip6->sin6_port );
            }

            p_os_addrs = ( struct sockaddr * )( ( char * )p_os_addrs + sizeof( struct sockaddr_in6 ) );
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

            p_os_addrs = ( struct sockaddr * )( ( char * )p_os_addrs +  sizeof( struct sockaddr_in ) );
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
 *        sctp_conv_addr_sctp_to_sock_os
 *
 *    DESCRIPTION:This function Converts the IP address from the SCTP format
 *                to socket Address format.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_conv_addr_sctp_to_sock_os(
    sctp_U32bit                n_sctp_addr,
    sctp_sockaddr_st           *p_sctp_addrs,
    struct sockaddr            *p_os_addrs,
    sctp_port_t                port,
    sctp_error_t               *p_ecode )
{
    sctp_U32bit              count      = 0;
    sctp_sockaddr_storage_st *p_storage = SCTP_NULL;

    struct sockaddr            *tmp_os_addrs = p_os_addrs;

    if( !p_sctp_addrs || !p_os_addrs || ( n_sctp_addr < 1 ) )
    {
        SCTP_SET_ECODE( p_ecode, EINV_PARAMS );

        return ( SCTP_FAILURE );
    }

    for( ; count < n_sctp_addr; count++ )
    {
        /* Convert from the OS format to SCTP format */
        /* SPR 20732 Starts here */
        p_storage = ( sctp_sockaddr_storage_st * )tmp_os_addrs;
        /* SPR 20732 Ends here */

        if( SCTP_AF_INET6 == p_sctp_addrs[count].family )
        {
#ifndef SCTP_INET6_SUPPORT
            SCTP_SET_ECODE( p_ecode, EINV_ADDRS );
            return ( SCTP_FAILURE );
#else
            struct sockaddr_in6  *p_ip6 =
                ( struct sockaddr_in6 * )( p_storage );
            /* SPR 21160 changes starts*/
            __MEMSET( p_storage, 0, sizeof( sctp_sockaddr_storage_st ) );
            /* SPR 21160 changes ends*/
            p_ip6->sin6_family = SCTP_AF_INET6;
            p_ip6->sin6_port   = htons( port );
            p_ip6->sin6_flowinfo = p_sctp_addrs[count].ip.v6.flow_info;
            p_ip6->sin6_addr.s6_addr32[0] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[0] );
            p_ip6->sin6_addr.s6_addr32[1] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[1] );
            p_ip6->sin6_addr.s6_addr32[2] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[2] );
            p_ip6->sin6_addr.s6_addr32[3] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[3] );
            p_ip6->sin6_scope_id =  p_sctp_addrs[count].ip.v6.scope_id;
            p_ip6->sin6_scope_id =  p_sctp_addrs[count].ip.v6.scope_id;
            tmp_os_addrs = ( struct sockaddr * )( ( char * )tmp_os_addrs + sizeof( struct sockaddr_in6 ) );

#endif
        }

        else if( SCTP_AF_INET == p_sctp_addrs[count].family )
        {
            if( SCTP_AF_INET6 == SOCKADDR_STORAGE_FAMILY( ( *p_storage ) ) )
            {
#ifndef SCTP_INET6_SUPPORT
                SCTP_SET_ECODE( p_ecode, EINV_ADDRS );
                return ( SCTP_FAILURE );
#else
                struct sockaddr_in6  *p_ip6 =
                    ( struct sockaddr_in6 * )( p_storage );
                /* SPR 21160 changes starts*/
                __MEMSET( p_storage, 0, sizeof( sctp_sockaddr_storage_st ) );
                /* SPR 21160 changes ends*/

                p_ip6->sin6_family = SCTP_AF_INET6;
                p_ip6->sin6_port   = htons( port );

                p_ip6->sin6_flowinfo = p_sctp_addrs[count].ip.v6.flow_info;
                p_ip6->sin6_addr.s6_addr32[0] = 0;
                p_ip6->sin6_addr.s6_addr32[1] = 0;
                p_ip6->sin6_addr.s6_addr32[2] = 0;
                p_ip6->sin6_addr.s6_addr32[3] =
                    htonl( p_sctp_addrs[count].ip.v4.addr );
                p_ip6->sin6_scope_id =  0;
                tmp_os_addrs = ( struct sockaddr * )( ( char * )tmp_os_addrs + sizeof( struct sockaddr_in6 ) );
#endif
            }

            else
            {
                struct sockaddr_in  *p_in =
                    ( struct sockaddr_in * )( p_storage );
                /* SPR 21160 changes starts*/
                __MEMSET( p_storage, 0, sizeof( sctp_sockaddr_storage_st ) );
                /* SPR 21160 changes ends*/

                p_in->sin_family = p_sctp_addrs[count].family;
                p_in->sin_port   = htons( port );
                p_in->sin_addr.s_addr = htonl( p_sctp_addrs[count].ip.v4.addr );

                tmp_os_addrs = ( struct sockaddr * )( ( char * )tmp_os_addrs +  sizeof( struct sockaddr_in ) );
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
 *    DESCRIPTION: This function Converts the IP address from the SCTP format to
 *    OS format. In case one of the addresses is IPv6 but
 *    if the address family that system supports is not set then an error is returned.
 *    NOTE: In case the n_sctp_addr param is 1 then there is no ambiguity in the
 *    value of the port parameter (which is an out paramter).
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

            p_ip6->sin6_family = SCTP_AF_INET6;
            p_ip6->sin6_port   = port;
            p_ip6->sin6_flowinfo = p_sctp_addrs[count].ip.v6.flow_info;
            p_ip6->sin6_addr.s6_addr32[0] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[0] );
            p_ip6->sin6_addr.s6_addr32[1] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[1] );
            p_ip6->sin6_addr.s6_addr32[2] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[2] );
            p_ip6->sin6_addr.s6_addr32[3] =
                htonl( p_sctp_addrs[count].ip.v6.addr.s_u6_addr32[3] );
            p_ip6->sin6_scope_id =  p_sctp_addrs[count].ip.v6.scope_id;
            p_ip6->sin6_scope_id =  p_sctp_addrs[count].ip.v6.scope_id;

#endif
        }

        else if( SCTP_AF_INET == p_sctp_addrs[count].family )
        {
            if( SCTP_AF_INET6 == SOCKADDR_STORAGE_FAMILY( ( *p_storage ) ) )
            {
#ifndef SCTP_INET6_SUPPORT
                SCTP_SET_ECODE( p_ecode, EINV_ADDRS );
                return ( SCTP_FAILURE );
#else
                struct sockaddr_in6  *p_ip6 =
                    ( struct sockaddr_in6 * )( p_storage );

                p_ip6->sin6_family = SCTP_AF_INET6;
                p_ip6->sin6_port   = htons( port );

                p_ip6->sin6_flowinfo = p_sctp_addrs[count].ip.v6.flow_info;
                p_ip6->sin6_addr.s6_addr32[0] = 0;
                p_ip6->sin6_addr.s6_addr32[1] = 0;
                p_ip6->sin6_addr.s6_addr32[2] = 0;
                p_ip6->sin6_addr.s6_addr32[3] =
                    htonl( p_sctp_addrs[count].ip.v4.addr );
                p_ip6->sin6_scope_id =  0;
#endif
            }

            else
            {
                struct sockaddr_in  *p_in =
                    ( struct sockaddr_in * )( p_storage );

                p_in->sin_family = p_sctp_addrs[count].family;
                p_in->sin_port   = htons( port );
                p_in->sin_addr.s_addr = htonl( p_sctp_addrs[count].ip.v4.addr );

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

#ifdef SCTP_DEBUG_ENABLED
/***************************************************************************
 *    FUNCTION :
 *        sctp_get_ctime
 *
 *    DESCRIPTION: Convert the given time in ascii format
 *
 *    RETURN VALUE:
 *          Time as a string.
 **************************************************************************/
char *
sctp_get_ctime(
    time_t         clock_sec,
    char          *time_buffer )
{
    sctp_U32bit     buffer_len = 26;

    return ( ( char * )sigtran_get_ctime_r( &clock_sec,
                                            ( sigtran_S8bit * )time_buffer, &buffer_len ) );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_log_text
 *
 *    DESCRIPTION: This function is for SCTP logging
 *
 *    RETURN VALUE:
 *      U32 bit.
 **************************************************************************/
sctp_U32bit
sctp_log_text( const char *szFormat, ... )
{
     va_list       ap;
    sctp_U32bit ret = 0;

    va_start( ap, szFormat );
    ret = vprintf( szFormat, ap );
    va_end( ap );

    return ret;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_filename
 *
 *    DESCRIPTION: get the filename from given complete path
 *
 *    RETURN VALUE:
 *        pointer to the file name
 **************************************************************************/
char *
sctp_get_filename( const char *filepath )
{
    return ( strrchr( filepath, SCTP_OS_PATH_SEP ) + 1 );
}



#endif


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
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    if( flag )
    {
        signal( SIGSEGV, sctp_os_sig_handler );
        signal( SIGBUS, sctp_os_sig_handler );
        signal( SIGILL, sctp_os_sig_handler );
    }

    else
    {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        signal( SIGSEGV, SCTP_NULL );
        signal( SIGBUS, SCTP_NULL );
        signal( SIGILL, SCTP_NULL );
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        signal( SIGSEGV, ( sctp_signal_hnd_fn_pt )SCTP_NULL );
        signal( SIGBUS, ( sctp_signal_hnd_fn_pt )SCTP_NULL );
        signal( SIGILL, ( sctp_signal_hnd_fn_pt )SCTP_NULL );
#endif
    }

    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_os_sig_handler
 *
 *    DESCRIPTION:
 *      Handles the Signal.
 *
 *    RETURN VALUE:
 *       void
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
