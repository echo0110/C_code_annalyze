/****************************************************************************
 *    FILE NAME  : s_uisusk.c
 *
 *    DESCRIPTION: This file contains the functions providing support for the
 *    socket API draft 0.1.
 *
 *    DATE     NAME         REFERENCE              REASON
 *    ----     ----         ---------              ------
 *    12Sep01  gsheoran         -                  Create Original
 *    19Dec01  ygahlaut      SPRXXXY               SPR Fix
 *    20May02  ygahlaut      SPR 4047 4074 4080 Fix
 *    08Oct02  mrajpal       SPR 4926               getsockopt params
 *    29Dec03  vibansal                            Add to listen hash table
 *                                                 only if the listen API is
 *                                                 called.
 *    20Oct04  Amit Ray      REL 6.0               SCTP/FUNC-SOCKAPI-31
 *    20Oct04  Amit Ray      REL 6.0               SCTP/FUNC-SOCKAPI-32
 *    20Oct04  Amit Ray      REL 6.0               SCTP/FUNC-SOCKAPI-33
 *    20Oct04  Amit Ray      REL 6.0               SCTP/FUNC-SOCKAPI-30
 *    09May08  Raza          REL 6.2               SPR 18527
 *    1May13  Pranav        SPR 21127             CSR 40731-GCC 4.7 warning removal
 *    25Oct13  Pinky Sinha   SPR 21157             CSR 56258
 *    21May14  Pinky Sinha   Bug Id6               Coverity fix-78264
 *    15Sep14  Priyanka Mishra   Bug Id 22         CSR 85223
 *    6Oct15   Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
 *    29May17  Neha Gupta       Bug Id 199         CSR  134689
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#define SCTP_MODULE "UI-SK"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_uisusk.h>
#include <s_in.h>
#include <s_dbg.h>
#include <s_stats.h>
#include <s_err.h>
#include <s_db.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_osmutx.h>
#include <s_fsmass.h>
#include <s_dbutil.h>
#include <s_uiutil.h>
#include <s_osutil.h>

/*
 * The Global error code set by the Socket Interface module for all error codes.
 */
sctp_sk_err_et sk_errno;

#ifdef SCTP_UI_SOCK_API

/***************************************************************************
 *    FUNCTION    : sctp_socket
 *
 *    DESCRIPTION : This api gets the end-point id from the endpoint array
 *    created during initlisation.
 *
 *    RETURN VALUE: endpoint id on success and -1  on error
 ******************************************************************************/
int
sctp_socket(
    int         domain,
    int         type,
    int         protocol )
{
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_U32bit     ep_id = SK_FAILURE;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_U32bit     ep_id = SCTP_INVALID_ENDPOINT;
#endif
    sctp_ep_st      *p_ep = SCTP_NULL;
    sctp_error_t    s_err;

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTPERROR_FLAG_APIERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        sk_errno = SK_EACCES;

        return ( SK_FAILURE );
    }

    if( !SCTP_VALID_SK_FAMILY( domain ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EAFNOSUPPORT,
                    "Service User API ERROR" );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SCTPERROR_FLAG_APIERROR" ) );

        sk_errno = SK_EAFNOSUPPORT;

        return ( SK_FAILURE );
    }

    if( !SCTP_VALID_SK_TYPE( type ) )
    {
        /* for wrong parameters */
        /* to store the domain, type and protocol */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EPROTOTYPE,
                    "Service User API ERROR" );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SCTPERROR_FLAG_APIERROR" ) );

        sk_errno = SK_EPROTOTYPE;

        return ( SK_FAILURE );
    }

    if( IPPROTO_SCTP != protocol )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EPROTONOSUPPORT, "Service User API ERROR" );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SCTPERROR_FLAG_APIERROR" ) );

        sk_errno = SK_EPROTONOSUPPORT;

        return ( SK_FAILURE );
    }

    /* calling function with endpoint-id and error as parameters */
    p_ep = sctp_db_get_free_ep( &s_err );

    if( SCTP_NULL == p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_NORESOURCES, "Service User APIERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Get Free endpoint Failed, No Resources" ) );

        sk_errno = SK_EMFILE;

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( p_ep->ep_id );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC, ( "In sctp_socket call" ) );

    /* Set the parameters of the socket/endpoint */
    /* Set the domain field */
    p_ep->family = domain;

    /* Reset the CLOSE_CALLED flag */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_FALSE );

    /* Set the socket type field */
    if( type == SOCK_SEQPACKET )
    {
        p_ep->sk_type = SCTP_SOCK_SEQPACKET;

        /*
         * In case of UDP style sockets we set the socket as writeable as soon
         * as its created. coz u might want to send data to connect to the
         * destination.
         */
        SCTP_SOCK_SET_WRITEABLE( p_ep->flags, SCTP_TRUE );
        /* SPR 4047 */
        SCTP_SOCK_SET_RECVDATAIO( p_ep->ulpqueue.ntfy_flags, 1 );
        SCTP_SOCK_SET_RECVASSOC( p_ep->ulpqueue.ntfy_flags, 1 );
    }

    else
    {
        p_ep->sk_type = SCTP_SOCK_STREAM;
    }

    /* By Default every socket is blocking */
    /* SPR 4080 */
    SCTP_SOCK_SET_BLOCKING( p_ep->flags, SCTP_TRUE );

    /* Return the Socket ID */
    ep_id = p_ep->ep_id;

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( ep_id );

    return ( ep_id );
}


/***************************************************************************
 *    FUNCTION    : sctp_bind
 *
 *    DESCRIPTION : Assigns an address to an unnamed socket.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 **************************************************************************/
int
sctp_bind(
    int                     sd,
    struct sockaddr         *addr,
    sctp_socklen_t          addrlen )
{
    sctp_ep_st            *p_ep = SCTP_NULL;
    sctp_port_t           port;
    sctp_sockaddr_list_st sctp_addr_list;
    sctp_error_t          s_err;
    sctp_U32bit           n_addr = 1;
    sctp_U32bit           count  = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Function sctp_bind " ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !addr )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( ( addr->sa_family == AF_INET ) && ( addrlen < sizeof( struct sockaddr_in ) ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );

    }

#ifdef SCTP_INET6_SUPPORT

    else if( ( addr->sa_family == AF_INET6 ) && ( addrlen < sizeof( struct sockaddr_in6 ) ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

#endif


    /* Convert the address from sockaddr_storage to sockaddr_st */
    if( SCTP_FAILURE == sctp_conv_addr_os_to_sctp( n_addr,
                                                   addr, sctp_addr_list, &port, &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_bind:Invalid address passed" ) );

        return ( SK_FAILURE );
    }

    /*
     * Check that the socket id passed is a valid endpoint.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_bind:end-point does not exist" ) );

        return ( SK_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /*
    * verify port is nonzero
    */
    if( port == 0 )
    {
        /* Get a free port */
        if( !sctp_db_get_free_sctpport( &port ) )
        {
            /* We should have a better error code but we don't */
            sk_errno = SK_EADDRNOTAVAIL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }
    }

    /*
     * Check if the address is SCTP_INADDR_ANY.
     */
    if( SCTP_IS_INADDR_ANY( sctp_addr_list[0] ) )
    {
        /*
         * This case we bind the endpoint to all the local addresses
         */
        /*
         * Select the local addresses from db_main for this address family.
         * We bind to as many addresses possible.
         */
        n_addr = 0;

        for( count = 0; count < sctp_db_main.num_local_addrs; count++ )
        {
            if( p_ep->family == SCTP_AF_INET6 )
            {
                sctp_addr_list[n_addr++] =
                    sctp_db_main.addr_list[count];
            }

            else
            {
                if( p_ep->family == sctp_db_main.addr_list[count].family )
                {
                    /* Copy the address */
                    sctp_addr_list[n_addr++] = sctp_db_main.addr_list[count];
                }
            }
        }

        if( !n_addr )
        {
            /* Could not find an address to which we can bind. Return failure */
            sk_errno = SK_EADDRNOTAVAIL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_bind:Address not of local machine" ) );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }
    }

    else
    {
        /*
         * Verify that the address passed is a valid address for the m/c.
         */
        if( !sctp_db_validate_bind_address( n_addr, sctp_addr_list ) )
        {
            sk_errno = SK_EADDRNOTAVAIL;

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_bind:Address not of local machine" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

        /*
         * Verify that the address type passed is consistent with the family
         * the socket belongs to.
         */

        /* SPR 21127 CHANGE */
        if( SCTP_FAILURE == ( sctp_return_t )sctp_db_is_af_correct( p_ep, n_addr, sctp_addr_list ) )
        {
            sk_errno = SK_EAFNOSUPPORT;

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp-bind:Address family is not right" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }
    }

    /*
     * Check that the socket is not already bound to an address.
     */
    if( SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_bind:The Socket is already bound" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Verify that the address is "Bind'able".
     */
    if( !sctp_db_addr_bindable( port, n_addr, sctp_addr_list ) )
    {
        sk_errno = SK_EADDRINUSE;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_bind:Address is not bindable" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Bind the endpoint to the address passed.
     */
    if( !sctp_db_bind_ep( sd, port, n_addr, sctp_addr_list, &s_err ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( ":SU Module: Socket bound" ) );

        /* Mark the socket as bound */
        SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );

        /* Add the socket/Endpoint to the Bind hash list */
        sctp_db_add_hash( SCTP_BIND_HASH, p_ep );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_SUCCESS );
    }
}


/*****************************************************************************
 *    FUNCTION    : sctp_bindx
 *
 *    DESCRIPTION :To Add/Remove another Address to a socket
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ******************************************************************************/
int
sctp_bindx(
    int                      sd,
    struct sockaddr          *addrs,
    int                      n_addr,
    int                      flags )
{
    sctp_ep_st            *p_ep;
    sctp_sockaddr_list_st sctp_addr_list;
    sctp_error_t          s_err;
    sctp_port_t           port = 0;
    sctp_tcb_st           *p_tcb = NULL;
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_S32bit           counter = 0;
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_bindx" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* SPR : 18527 */
    /* Check if the Number of Addresses Passed is greater than the Max Transport Addresses Supported by SCTP */
    if( n_addr > SCTP_MAX_TRANSPORT_ADDR )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Number of Address(%d) is greater than SCTP_MAX_TRANSPORT_ADDR(%d).",
                      n_addr, SCTP_MAX_TRANSPORT_ADDR ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* SPR : 18527 */

    /* Convert the address from sockaddr_storage to sctp_sockaddr_st */
    if( SCTP_FAILURE == sctp_conv_addr_os_to_sctp( n_addr,
                                                   addrs, sctp_addr_list, &port, &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }


    if( ( SCTP_SOCK_IS_BOUND( p_ep->flags ) && ( port != p_ep->this_port ) ) ||
            ( !SCTP_SOCK_IS_BOUND( p_ep->flags ) && ( SCTP_BINDX_ADD_ADDR != flags ) ) )
    {

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Either Socket is bounded and port is incorrect or socket"
                      "is not bounded and SCTP_BINDX_ADD_ADDR flag is not set" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }


    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /* verify port is not zero */
    if( port == 0 )
    {
        /* get a free port*/

        if( !sctp_db_get_free_sctpport( &port ) )
        {
            sk_errno = SK_EADDRNOTAVAIL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

    }

    /*
     * Verify that the address passed is a valid address for the m/c.
     */
    if( !sctp_db_validate_bind_address( n_addr, sctp_addr_list ) )
    {
        sk_errno = SK_EADDRNOTAVAIL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address not of local machine" ) );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Validate the Bind Flag
     */
    if( !SCTP_VALID_BINDX_FLAGS( flags ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Bindx Flag" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Verify that the address type passed is consistent with the family
     * the socket belongs to.
     */
    /* SPR 21127 CHANGE */
    if( SCTP_FAILURE == ( sctp_return_t )sctp_db_is_af_correct( p_ep, n_addr, sctp_addr_list ) )
    {
        sk_errno = SK_EAFNOSUPPORT;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address family is not right" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    sk_errno = SK_EINVAL;

    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( p_tcb != NULL )
    {
        /*asscoiation exists */
        if( SCTP_FAILURE == sctp_dynamic_ip_supported( p_tcb,
                                                       ( SCTP_BINDX_ADD_ADDR == flags ) ?
                                                       SCTP_ASCONF_PARAM_ADD_IP : SCTP_ASCONF_PARAM_DEL_IP ) )
        {
            sk_errno = SK_EOPNOTSUPP;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Dynamic Ip not supported for association" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

        if( SCTP_BINDX_ADD_ADDR == flags )
        {
            sctp_U32bit     counter1 = 0;

            if( !sctp_db_addr_bindable( p_ep->this_port, n_addr, sctp_addr_list ) )
            {
                sk_errno = SK_EADDRINUSE;

                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            sk_errno, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Address is not bindable" ) );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( sd );

                return ( SK_FAILURE );
            }

            /*above function might return success if RE_USE is set and
              address is already added to ep, check for that */
            if( SCTP_SOCK_IS_REUSEADDR( p_ep->flags ) )
            {
                for( counter = 0; counter < n_addr; counter++ )
                {
                    for( counter1 = 0; counter1 < p_ep->naddr; counter1++ )
                    {
                        if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_addr_list[counter],
                                                           &p_ep->this_address[counter1] ) )
                        {
                            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                        EINV_PARAMS, "Service User API ERROR" );

                            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                        ( "Address is not bindable" ) );

                            /* Unlock the endpoint */
                            SCTP_UNLOCK_EP( sd );

                            return ( SK_FAILURE );
                        }
                    }
                }
            }


            for( counter = 0; counter < n_addr; counter++ )
            {
                sctp_asconf_addip_param_st  *p_add_ip =
                    &( p_tcb->cmd_asconf.asconf_params[counter].add_ip );

                p_add_ip->param_type = SCTP_ASCONF_PARAM_ADD_IP;
                p_add_ip->corr_id = counter;
                p_add_ip->ipaddr = sctp_addr_list[counter];
            }
        }

        else
        {
            /*chek if the addess can be removed*/
            if( SCTP_FAILURE == sctp_db_chk_addr_for_delete( p_ep, p_ep->this_port,
                                                             n_addr, sctp_addr_list, &s_err ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_PARAMS, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "Error Removing address from the Endpoint" ) );

                /* Unlock the Endpoint */
                SCTP_UNLOCK_EP( sd );

                return ( SK_FAILURE );
            }

            for( counter = 0; counter < n_addr; counter++ )
            {
                sctp_asconf_addip_param_st  *p_del_ip =
                    &( p_tcb->cmd_asconf.asconf_params[counter].del_ip );

                p_del_ip->param_type = SCTP_ASCONF_PARAM_DEL_IP;
                p_del_ip->corr_id    = counter;
                p_del_ip->ipaddr     = sctp_addr_list[counter];
            }
        }

        p_tcb->cmd_asconf.num_params = counter;

        if( SCTP_FAILURE == sctp_chk_and_send_asconf( p_tcb, flags,
                                                      SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &s_err ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        EINV_PARAMS, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

        SCTP_UNLOCK_EP( sd );

        return ( SK_SUCCESS );
#endif
    }

    /*
     * Socket is not connected.
     */
    if( SCTP_BINDX_ADD_ADDR == flags )
    {
        /*
         * Verify that the address is "Bind'able".
         */
        if( !sctp_db_addr_bindable( port, n_addr, sctp_addr_list ) )
        {
            sk_errno = SK_EADDRINUSE;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Address is not bindable" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

        /*
         * Add the new addresses into the existing endpoint.
         */


        if( sctp_db_bind_ep( sd, port, n_addr, sctp_addr_list, &s_err ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "SCTP_BINDX_ADD_ADDR Executed" ) );

            SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );

            /* Add the socket/Endpoint to the Bind hash list */
            sctp_db_add_hash( SCTP_BIND_HASH, p_ep );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_SUCCESS );
        }

        else
        {
            sk_errno = SK_EINVAL;
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "SCTP TRACE::SU Module:End-point already bound" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

    }

    else
    {
        /*
         * Remove an address from an endpoint. SCTP_BINDX_REM_ADDR.
         */

        /*
         * Rules for removing an address from an endpoint.
         * 1. The Address should be available in the endpoint else error.
         * 2. The Endpoint should not be set to listen preferably. In case the
         *    Endpoint is set to listen then we should not have any connection
         *    waiting to be accept'ed from this server.
         * 3. The Endpoint cannot have a tcb in any state except CLOSED.
         * The DB function sctp_db_rem_addr_after_chks does all these checks.
         */

        if( !sctp_db_rem_addr_after_chks( p_ep, port, n_addr, sctp_addr_list,
                                          &s_err ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "Error Removing address from the Endpoint" ) );

            /* Unlock the Endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( SK_SUCCESS );
}


/***************************************************************************
 *    FUNCTION    : sctp_listen
 *
 *    DESCRIPTION : To ready the SCTP endpoint for accepting inbound
 *                  associations.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ***************************************************************************/
int
sctp_listen(
    int         sd,
    int         backlog )
{
    sctp_ep_st     *p_ep = SCTP_NULL;
    int            back = 0;
    sctp_error_t   s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_listen" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * If the socket is not bound then we cannot set it for listen.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        sk_errno = SK_EDESTADDRREQ;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }


    /*
     * Check the backlog parameter for the following
     * 1. if <= 0 then set it to 0.
     * 2. if > SCTP_MAX_BACKLOG then set it to SCTP_MAX_BACKLOG.
     */

    if( backlog > SCTP_MAX_BACKLOG )
    {
        back = SCTP_MAX_BACKLOG;
    }

    else if( backlog > 0 )
    {
        back = backlog;
    }

    /* if backlog is < 0 then it will be set to 0. */
    p_ep->l_backlog = back;


    /*
     * Check the socket type. If it is a UDP style socket then return
     * success from here only because a UDP style socket becomes listener
     * as soon as it is bound.
     */
    if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
    {

        if( back > 0 )
        {
            /* Unlock the Endpoint */
            if( !sctp_db_add_hash( SCTP_LISTEN_HASH, p_ep ) )
            {
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "listen: Could not add the ep to the listen hash....." ) );

                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            sk_errno, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_UNLOCK_EP( sd );

                return ( SK_FAILURE );
            }

            SCTP_SOCK_SET_LISTENER( p_ep->flags, SCTP_TRUE );

            p_ep->l_backlog = back;
            SCTP_UNLOCK_EP( sd );

            return ( SK_SUCCESS );
        }

        else
        {
            if( !sctp_db_add_hash( SCTP_LISTEN_HASH, p_ep ) )
            {
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "listen: Could not add the ep to the listen hash....." ) );

                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            sk_errno, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_UNLOCK_EP( sd );

                return ( SK_FAILURE );
            }

            SCTP_SOCK_SET_LISTENER( p_ep->flags, SCTP_FALSE );

            p_ep->l_backlog = back;
            SCTP_UNLOCK_EP( sd );

            return ( SK_SUCCESS );
        }

    }



    /*
     * Check that the socket is in CLOSED state. In no other state are we
     * allowed to set it as a listener. i.e. Check that the number of assocs
     * branching from the endpoint is 0.
     */
    if( p_ep->n_assoc )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }


    /*
     * Check that the socket is already merked a listener.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "The Socket is already set to liisten" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_SUCCESS );
    }

    /*
     * Check if the socket can be marked for listen.
     */
    if( SCTP_FAILURE == sctp_db_is_listenable( p_ep, &s_err ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Add the newly created listener to the Listen hash queue. But before that
     * Remove this endpoint from the Bind hash.
     */
    if( !sctp_db_del_hash( SCTP_BIND_HASH, p_ep ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "listen: Couldnot remove the ep from the bind hash....But..the ep"
                      " should be there at this time: something is seriously wrong." ) );
    }

    if( !sctp_db_add_hash( SCTP_LISTEN_HASH, p_ep ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "listen: Could not add the ep to the listen hash....." ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* Mark the socket as a listener. */
    SCTP_SOCK_SET_LISTENER( p_ep->flags, SCTP_TRUE );

    /* U'r not allowed to write on a listening socket. */
    SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_TRUE );
    SCTP_SOCK_SET_WRITEABLE( p_ep->flags, SCTP_FALSE );


    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( SK_SUCCESS );
}

/***************************************************************************
*    FUNCTION    : sctp_connectx
*
*    DESCRIPTION : It is used to initiate an association to a peer.
*
*    RETURN VALUE: On success 0 otherwise -1
****************************************************************************/

int
sctp_connectx(
    int sd,                        /*Socket descriptor*/
    const struct sockaddr *sock_addrs,  /*List of addresses for the peer*/
    int addrcnt )                  /*No of addresses*/
{
    /* Bug Id 6 fix starts*/
    /*sctp_port_t           port;*/
    sctp_port_t           port = 0;
    /* Bug Id 6 fix ends*/
    /*Bug Id 22 fix starts*/
    /*sctp_return_t         ret;*/
    int ret =  SK_SUCCESS;
    /*Bug Id 22 fix ends*/

    sctp_ep_st            *p_ep;
    sctp_sockaddr_list_st sctp_addr_list;
    sctp_error_t          s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_connect" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !sock_addrs )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* Convert the address from sockaddr_storage to sctp_sockaddr_st */
    if( SCTP_FAILURE == sctp_conv_addr_os_to_sctp( addrcnt,
                                                   ( struct sockaddr * )sock_addrs, sctp_addr_list, &port, &s_err ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * verify port is nonzero
     */
    if( port == 0 )
    {
        sk_errno = SK_EINVAL;

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Zero Port is being used" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Verify that the address type passed is consistent with the family
     * the socket belongs to.
     */
    /* SPR 21127 CHANGE */
    if( SCTP_FAILURE == ( sctp_return_t )sctp_db_is_af_correct( p_ep, addrcnt, sctp_addr_list ) )
    {
        sk_errno = SK_EAFNOSUPPORT;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address family is not right" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Check that the socket should not be a listener.
     * For UDP style, every socket is listner
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        if( ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) ) )
        {
            sk_errno = SK_EOPNOTSUPP;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "The Socket is a server socket" ) );

            /* Unlock the Endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

        ret = sctp_connect_tcp( p_ep, sctp_addr_list, port, addrcnt );
    }

    else
    {
        ret = sctp_connect_udp( p_ep, sctp_addr_list, port, addrcnt );
    }


    /* Unlock The Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );

}

/***************************************************************************
 *    FUNCTION    : sctp_connect
 *
 *    DESCRIPTION : It is used to initiate an association to a peer.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_connect(
    int                           sd,
    const struct sockaddr         *addr,
    sctp_socklen_t                addrlen )
{
    sctp_port_t           port;
    /*Bug Id 22 fix starts*/
    /*sctp_return_t         ret;*/
    int ret =  SK_SUCCESS;
    /*Bug Id 22 fix ends*/
    sctp_ep_st            *p_ep;
    sctp_sockaddr_list_st sctp_addr_list;
    sctp_error_t          s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_connect" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !addr )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( ( addr->sa_family == AF_INET ) && ( addrlen < sizeof( struct sockaddr_in ) ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );

    }

#ifdef SCTP_INET6_SUPPORT

    else if( ( addr->sa_family == AF_INET6 ) && ( addrlen < sizeof( struct sockaddr_in6 ) ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

#endif


    /* Convert the address from sockaddr_storage to sctp_sockaddr_st */
    if( SCTP_FAILURE == sctp_conv_addr_os_to_sctp( 1,
                                                   ( struct sockaddr * )addr, sctp_addr_list, &port, &s_err ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * verify port is nonzero
     */
    if( port == 0 )
    {
        sk_errno = SK_EINVAL;

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Zero Port is being used" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Verify that the address type passed is consistent with the family
     * the socket belongs to.
     */
    /* SPR 21127 CHANGE */
    if( SCTP_FAILURE == ( sctp_return_t )sctp_db_is_af_correct( p_ep, 1, sctp_addr_list ) )
    {
        sk_errno = SK_EAFNOSUPPORT;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address family is not right" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Check that the socket should not be a listener.
     * For UDP style, every socket is listner
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        if( ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) ) )
        {
            sk_errno = SK_EOPNOTSUPP;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "The Socket is a server socket" ) );

            /* Unlock the Endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

        ret = sctp_connect_tcp( p_ep, &( sctp_addr_list[0] ), port, 1 );
    }

    else
    {
        ret = sctp_connect_udp( p_ep, &( sctp_addr_list[0] ), port, 1 );
    }


    /* Unlock The Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_select
 *
 *    DESCRIPTION : It is used select for Read/Write/Error on a number of
 *    sockets. In case the sockets passed are invalid and not "opened" then
 *    we donot return failure but just skip those sockets. In case None of
 *    the sockets are selected because of this then we return SK_FAILURE.
 *
 *    RETURN VALUE: Retruns the number of sockets selected on success and
 *    -1 on error.
 ****************************************************************************/
int
sctp_select(
    int                           n_fds,
    sctp_fd_set_st                *p_rfds,
    sctp_fd_set_st                *p_wfds,
    sctp_fd_set_st                *p_efds,
    sctp_time_t                   *p_timeout )
{
    sctp_select_st    *p_select = SCTP_NULL;
    int               ret = SK_FAILURE;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_select" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* Check the parameters */
    if( !p_rfds && !p_wfds && !p_efds && !p_timeout )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Check that n_fds is !<0 and !>SCTP_FD_SET_SIZE-1
     * Also n_fds cannot be greater than the max endpoint value.
     */
    if( ( n_fds < 0 ) || ( n_fds > SCTP_FD_SETSIZE ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Invoke select processing.
     */
    /* Get a new select node. */
    SCTP_SEL_GET_NODE( p_select );

    if( !p_select )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "select: Could not get select node" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* Initialize the return FD sets. */
    SCTP_FD_ZERO( &( p_select->n_rfds ) );
    SCTP_FD_ZERO( &( p_select->n_wfds ) );
    SCTP_FD_ZERO( &( p_select->n_efds ) );

    /* Set the FD's passed */
    p_select->p_rfds = p_rfds;
    p_select->p_wfds = p_wfds;
    p_select->p_efds = p_efds;
    p_select->p_timeout = p_timeout;
    p_select->nfds   = n_fds;

    /* Invoke select: SCTP_FALSE is to indicate that its not a timeout. */
    ret = sctp_do_select( p_select );

    /* If error then report and increment stats */
    if( SK_FAILURE == ret )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "select: Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "select :Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
    }

    /* Add the select node back to the select free list. */
    SCTP_SEL_INIT_NODE( p_select );
    SCTP_SEL_FREE_NODE( p_select );

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_accept
 *
 *    DESCRIPTION : Remove established assocation from the accept queue of
 *    the end-point and create a new endpoint.
 *
 *    RETURN VALUE: new fd on success and -1 on failure.
 ***************************************************************************/
int
sctp_accept(
    int                     sd,
    struct sockaddr         *addr,
    sctp_socklen_t          *addrlen )
{
    int                  ret     = SCTP_FAILURE;
    sctp_ep_st           *p_ep   = SCTP_NULL;
    sctp_ep_st           *p_newep = SCTP_NULL;
    sctp_tcb_st          *p_tcb  = SCTP_NULL;
    sctp_error_t         s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "In Function sctp_accept" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Accept can be done only on a STREAM (TCP) socket, not supposted on UDP
     * style socket.
     */
    if( SCTP_SOCK_STREAM != p_ep->sk_type )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Endpoint not a STREAM socket" ) );

        /* Unlock The Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Check whether the socket is a server socket.
     */
    if( !SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Endpoint not a listner" ) );

        /* Unlock The Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    if( addr && addrlen )
    {
        SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )addr ) ) =
            ( p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        if( ( addr->sa_family == AF_INET ) &&
                ( *addrlen < sizeof( struct sockaddr_in ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            /* Bug Id 81 fix s*/
            SCTP_UNLOCK_EP( sd );
            /* Bug Id 81 fix e*/

            return ( SK_FAILURE );

        }

#ifdef SCTP_INET6_SUPPORT

        else if( ( addr->sa_family == AF_INET6 ) &&
                 ( *addrlen < sizeof( struct sockaddr_in6 ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            /* Bug Id 81 fix s*/
            SCTP_UNLOCK_EP( sd );
            /* Bug Id 81 fix e*/

            return ( SK_FAILURE );
        }

#endif
    }

    /*
     * Check if the accept is blocking or non blocking.
     */
    if( SCTP_SOCK_IS_BLOCKING( p_ep->flags ) )
    {
        /* The Socket is blocking type */
        ret = sctp_accept_blocking( p_ep );
    }

    else
    {
        /* The Socket is nonblocking */
        ret = sctp_accept_nonblocking( p_ep );
    }

    if( SK_FAILURE != ret )
    {
        /* Lock the Endpoint */
        SCTP_LOCK_EP( ret );

        /* Success: Get the address list of the new accepted endpoint */
        if( addr && addrlen )
        {
            /* SPR : 18527 */
            /* Get the pointer to the assoc in the ep */
            if( ( p_newep = sctp_db_get_ep_ptr( ret ) ) == SCTP_NULL )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "ERROR : sctp_db_get_ep_ptr() failed for ep : %d", ret ) );

                /* Unlock The Endpoint */
                SCTP_UNLOCK_EP( sd );
                /* Bug Id 81 fix s*/
                SCTP_UNLOCK_EP( ret );
                /* Bug Id 81 fix e*/

                return ( SK_FAILURE );
            }

            if( ( p_tcb = sctp_db_get_assoc_from_ep( p_newep ) ) == SCTP_NULL )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "ERROR : sctp_db_get_assoc_from_ep() failed for ep : %d", p_newep->ep_id ) );

                /* Unlock The Endpoint */
                SCTP_UNLOCK_EP( sd );
                /* Bug Id 81 fix s*/
                SCTP_UNLOCK_EP( ret );
                /* Bug Id 81 fix e*/

                return ( SK_FAILURE );
            }

            /* SPR : 18527 */

            SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )addr ) ) =
                ( p_newep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

            /* We just return the primary destination address */
            if( SCTP_FAILURE ==
                    sctp_conv_addr_sctp_to_sock_os( 1, &( p_tcb->transmit.remote[0].
                                                          that_address ), addr, p_tcb->transmit.that_port, &s_err ) )
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Error converting address: Should never happen" ) );
            }

            if( addr->sa_family == AF_INET )
            {
                *addrlen = sizeof( struct sockaddr_in );
            }

#ifdef SCTP_INET6_SUPPORT

            else
            {
                *addrlen = sizeof( struct sockaddr_in6 );
            }

#endif
        }

        /*
         * If there are no more assocs in the endpoint then we set it as not
         * readable/we update the readable status.
         */
        if( p_ep->n_assoc <= 0 )
        {
            sctp_db_update_readable_status( p_ep );
        }

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( ret );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_close
 *
 *    DESCRIPTION : To perform graceful Close on a socket. If close is called
 *    then no socket operation shall be allowed on that socket after that.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ****************************************************************************/
int
sctp_close(
    int               sd )
{
    sctp_ep_st      *p_ep  = SCTP_NULL;
    sctp_tcb_st     *p_tcb = SCTP_NULL;
    sctp_tcb_st     *p_temp = SCTP_NULL;
    int             ret    = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "In Function sctp_close" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

/*Bug Id 199 changes s*/
    SCTP_LOCK_EP(sd); 
/*Bug Id 199 changes e*/

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_EBADF;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "close:Invalid socket descriptor " ) );
/*Bug Id 199 changes s*/
    SCTP_UNLOCK_EP(sd); 
/*Bug Id 199 changes e*/
        return ( SK_FAILURE );
    }

    /* Reset the CLOSE_CALLED flag */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_TRUE );

    /* Changes for CSR#1-4619243 */
/*Bug Id 199 changes s*/
    /*SCTP_LOCK_EP(sd); */ 
/*Bug Id 199 changes e*/
    /* Changes for CSR#1-4619243 */


    /*
     * Different types of close for different types of sockets.
     */
    if( p_ep->n_assoc > 0 )
    {
        /* Close all the assocs in the endpoint */
        p_tcb = ( sctp_tcb_st * )lstFirst( &( p_ep->assoc_list ) );

        if( ( SCTP_SOCK_STREAM == p_ep->sk_type ) &&
                ( p_ep->so_linger.l_onoff == 1 ) &&
                ( p_ep->so_linger.l_linger == 0 ) )
        {
            /* send Abort in this case */
            p_tcb->event.cmd_abort.cause_length = 0;
            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDABORT );
        }

        else
        {
            while( p_tcb )
            {
                p_temp = ( sctp_tcb_st * )lstNext( ( NODE * )p_tcb );
                /* Send Shutdown message for the association */
                sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );
                p_tcb = p_temp;
            }
        }

        /* Mark this endpoint as write blocked */
        SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_TRUE );
    }

    else
    {
        /*
         * The Delete should.
         * 1. Remove the endpoint from any hash queues.
         * 2. Free any memory if we need to do that.
         * 3. Mark the endpoint as free.
         */
        if( SCTP_FAILURE == sctp_db_delete_ep( p_ep->ep_id, SCTP_NULL, SCTP_TRUE ) )
        {
            ret = SK_FAILURE;
        }
    }

    /* Changes for CSR#1-4619243 */
    SCTP_UNLOCK_EP( sd );
    /* Changes for CSR#1-4619243 */


    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_shutdown
 *
 *    DESCRIPTION : To perform shutdown of an association.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ****************************************************************************/
int
sctp_shutdown(
    int               sd,
    int               how )
{
    sctp_ep_st      *p_ep  = SCTP_NULL;
    sctp_tcb_st     *p_tcb = SCTP_NULL;
    /*Bug Id 22 fix starts*/
    /*sctp_return_t   ret    = SK_SUCCESS;*/
    int   ret    = SK_SUCCESS;
    /*Bug Id 22 fix ends*/
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_shutdown" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_EBADF;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "shutdown:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );


    /*
     * Shutdown API is valid only for a TCP Style (Stream) Socket. So if the
     * Sock is UDP style (SEQPACKET) return FAILURE.
     */
    if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "shutdown: The socket is not connected" ) );

        /* Unlock The Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Check if the socket is connected or not.
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_tcb || ( p_tcb->state != SCTPSTATE_ESTABLISHED ) )
    {
        /* There is no connection so cannot invoke shutdown */
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "shutdown: The socket is not connected" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Shutdown Processing based on the how parameter.
     */
    switch( how )
    {
        case SCTP_SHUT_RD:
            /* Disables further receive operations. */
            SCTP_SOCK_SET_RD_BLOCKED( p_ep->flags, SCTP_TRUE );

            break;

        case SCTP_SHUT_WR:
            /* Disables further send operations. */
            SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_TRUE );

            /* Set the CLOSE_CALLED flag */
            SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_TRUE );

            /* Initiate SCTP Shutdown sequence */
            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );
            break;

        case SCTP_SHUT_RDWR:
            /* Disables further send and receive operations. */
            SCTP_SOCK_SET_RD_BLOCKED( p_ep->flags, SCTP_TRUE );
            SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_TRUE );

            /* Set the CLOSE_CALLED flag */
            SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_TRUE );

            /* Initiate SCTP Shutdown sequence */
            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );
            break;

        default:
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "shutdown: the how argument is invalid\n" ) );
            sk_errno  = SK_EINVAL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            ret = ( SK_FAILURE );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_send
 *
 *    DESCRIPTION : To send data on a socket. This function can only be used to
 *    send data on a alrdy connected socket (i.e. TCP Only). As per the draft->
 *    Note, the send and recv calls, when used in the UDP-style model, may
 *    only be used with branched off socket descriptors.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ****************************************************************************/
int
sctp_send(
    int               sd,
    const void        *p_buffer,
    size_t            length,
    int               flags )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    sctp_tcb_st    *p_tcb  = SCTP_NULL;
    int            ret;
    int            ret_val = SK_SUCCESS;
    sctp_U32bit    context = 0;
    sctp_error_t   s_err;
    sctp_iovec_st  p_iovec[1];

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_send" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_EBADF;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "send:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Check that the socket is a STREAM socket.
     */
    if( SCTP_SOCK_STREAM != p_ep->sk_type )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "send: The socket is not connected" ) );

        /* Unlock The Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Check if the socket is connected or not.
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_tcb )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "send: The socket is not connected" ) );

        /* Unlock The Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * The socket is connected:send the data
     * 1. Select the stream. select the first stream.
     */

    p_iovec->iov_len  = length;
    p_iovec->iov_base = ( char * )p_buffer;
    ret = sctp_send_general(
              p_tcb,
              SCTP_NULL,  /* Means ->select the primary address */
              1,            /* data size */
              p_iovec,   /* data */
              0,          /* Stream 0 to be used by default */
              SCTP_MAX_LIFE_TIME,
              p_ep->ppi,
              !( SCTP_SOCK_IS_NODELAY( p_ep->ulpqueue.ntfy_flags ) ),
              SCTP_TRUE,
              context,
              NULL,
              &s_err );

    if( 0 == ret )
    {
        /*
         * Also map the sctp error code to the sk_errno codes for the socket
         * interface.
         */
        switch( s_err )
        {
            case E_REQ_IN_WRONG_STATE:
                sk_errno = SK_ENOTCONN;
                break;

            case EINV_PARAMS:
                sk_errno = SK_EOPNOTSUPP; /* have a better error code but what? */
                break;

            case E_NORESOURCES:
                sk_errno = SK_ENOBUFS;
                break;

            case E_FRAGMENTATION:
                sk_errno = SK_EFRAGMENT;

            default:
                break;
        }

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        ret_val = SK_FAILURE;
    }

    else
    {
        /* In case data send was successful, return the number of bytes
         * sent
         */
        ret_val = ret;
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret_val );
}


/****************************************************************************
 *    FUNCTION    : sctp_recv
 *
 *    DESCRIPTION :To Receive data from the connected socket
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ****************************************************************************/
int
sctp_recv(
    int               sd,
    void              *p_buffer,
    size_t            length,
    int               flags )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    sctp_tcb_st    *p_tcb  = SCTP_NULL;
    int            ret     = SK_FAILURE;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_recv" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !p_buffer || !length )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recv:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recv:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Check that the socket is a STREAM socket and not a server i.e Listener.
     */
    /*SPR 5094*/
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
        /*SPR 5094*/
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_recv: Listener socket " ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Check if the socket is connected or not.
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_tcb )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recv: The socket is not connected" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* SPR 4074 */
    if( SCTP_SOCK_IS_RD_BLOCKED( p_ep->flags ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recv: The socket is Read Blocked" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Receive data from the connected socket.
     * If no data and the socket is blocking type then block.
     * If no data and the socket type is Nonblocking then return 0.
     */
    /* Check if we have any date to be returned */
    if( !sctp_db_check_data_ind( p_tcb ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "recv: No data to be read." ) );

        /* If non blocking and no data then return SK_EAGAIN */
        if( !SCTP_SOCK_IS_BLOCKING( p_ep->flags ) )
        {
            sk_errno = SK_EAGAIN;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "recv: There is no data on a nonblocking socket" ) );

            /* Unlock the Endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( SK_FAILURE );
        }

        /*
         * The Socket is blocking. Do a blocking receive.
         */
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_recv:the socket is blocking.doing blocking receive." ) );
    }

    /* There is some data to be read and in no blocking case. */
    ret = sctp_receive_blocking( p_tcb, p_buffer, length,
                                 ( sctp_U32bit * )&length, flags,
                                 SCTP_SOCK_IS_BLOCKING( p_ep->flags ) );

    if( SK_SUCCESS == ret )
    {
        /* The actual length of the data read. */
        ret = length;
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_sendto
 *
 *    DESCRIPTION : The  sendto()  function sends a message through a
 *    connection-mode or connectionless-mode socket. If the socket
 *    is connectionless-mode, the message  will  be  sent  to  the
 *    address  specified  by destaddr.  If the socket is connection-
 *    mode, the destination address in destaddr is ignored.
 *
 *    RETURN VALUE: Returns the number of bytes on success and (-1) on error.
 ****************************************************************************/
int
sctp_sendto(
    int                    sd,
    const void             *message,
    size_t                 length,
    int                    flags,
    const struct sockaddr  *destaddr,
    sctp_socklen_t         dest_len )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    int            ret     = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_sendto" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* message cannot be null */
    if( !message )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendto:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendto:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Cannot invoke sendto on a Listner socket which is TCP style.
     * In case of UDP, any socket that is bound is a listner.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) &&
            ( SCTP_SOCK_STREAM == p_ep->sk_type ) )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendto:Listener socket" ) );

        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    if( destaddr )
    {
        if( ( destaddr->sa_family == AF_INET ) && ( dest_len < sizeof( struct sockaddr_in ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            /* Bug Id 81 fix s*/
            SCTP_UNLOCK_EP( sd );
            /* Bug Id 81 fix e*/

            return ( SK_FAILURE );

        }

#ifdef SCTP_INET6_SUPPORT

        else if( ( destaddr->sa_family == AF_INET6 ) && ( dest_len < sizeof( struct sockaddr_in6 ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            /* Bug Id 81 fix s*/
            SCTP_UNLOCK_EP( sd );
            /* Bug Id 81 fix e*/

            return ( SK_FAILURE );
        }

#endif
    }

    /*
     * Different behaviour in case of TCP and UDP.
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        ret = sctp_sendto_tcp( p_ep, ( sctp_U8bit * )message, length, flags,
                               destaddr, dest_len );
    }

    else
    {
        ret = sctp_sendto_udp( p_ep, ( sctp_U8bit * )message, length, flags,
                               destaddr, dest_len );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}

/****************************************************************************
 *    FUNCTION    : sctp_recvfrom
 *
 *    DESCRIPTION : The  recvfrom() function receives a data msg from a
 *    connection.
 *
 *    RETURN VALUE: Upon successful completion, recvfrom() returns the length
 *    of the  message  in  bytes.  If no messages are available to be
 *    received recvfrom() returns 0.  Otherwise, -1 is returned and errno is
 *    set to indicate the error.
 ****************************************************************************/
int
sctp_recvfrom(
    int                  sd,
    void                 *message,
    size_t               length,
    int                  flags,
    struct sockaddr      *srcaddr,
    sctp_socklen_t       *src_len )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    int            ret     = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_recvfrom" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !message )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvfrom:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvfrom:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Check that the socket is a STREAM socket and not a server i.e Listener.
     */
    if( ( SCTP_SOCK_STREAM == p_ep->sk_type )
            && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) ) )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_recvfrom: Listener socket" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )srcaddr ) ) =
        ( p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

    if( ( srcaddr->sa_family == AF_INET ) && ( *src_len < sizeof( struct sockaddr_in ) ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        /* Bug Id 81 fix s*/
        SCTP_UNLOCK_EP( sd );
        /* Bug Id 81 fix e*/
        return ( SK_FAILURE );

    }

#ifdef SCTP_INET6_SUPPORT

    else if( ( srcaddr->sa_family == AF_INET6 ) && ( *src_len < sizeof( struct sockaddr_in6 ) ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR: Invalid addresses" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Bug Id 81 fix s*/
        SCTP_UNLOCK_EP( sd );
        /* Bug Id 81 fix e*/
        return ( SK_FAILURE );
    }

#endif


    /*
     * Different behaviour in case of TCP and UDP.
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        ret = sctp_recvfrom_tcp( p_ep, message, length, flags, srcaddr, src_len );
    }

    else
    {
        ret = sctp_recvfrom_udp( p_ep, message, length, flags, srcaddr, src_len );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}

/****************************************************************************
 *    FUNCTION    : sctp_sendmsg
 *
 *    DESCRIPTION : The  sendmsg()  function sends a message through a
 *    connection-mode or connectionless-mode socket. If the socket
 *    is connectionless-mode, the message  will  be  sent  to  the
 *    address  specified  by msghdr.  If the socket is connection-
 *    mode, the destination address in msghdr is ignored.
 *
 *    RETURN VALUE: Returns the number of bytes on success and (-1) on error.
 ****************************************************************************/
int
sctp_sendmsg(
    int                  sd,
    const sctp_msghdr_st *p_message,
    int                  flags )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    int            ret     = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_sendmsg" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* p_message cannot be null */
    if( !p_message )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendmsg:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendmsg:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Cannot invoke sendmsg on a Listner socket which is TCP style.
     * In case of UDP, any socket that is bound is a listner.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags )
            && ( SCTP_SOCK_STREAM == p_ep->sk_type ) )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendmsg:Listener socket" ) );

        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Different behaviour in case of TCP and UDP.
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        ret = sctp_sendmsg_tcp( p_ep, ( sctp_msghdr_st * ) p_message, flags );
    }

    else
    {
        ret = sctp_sendmsg_udp( p_ep, ( sctp_msghdr_st * ) p_message, flags );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_recvmsg
 *
 *    DESCRIPTION : The  recvmsg() function receives a msg from a connection.
 *    It is implemented such at right now it gives either a indication or a
 *    data element back. Just one. not multiple.
 *    The only flag this is supported is MSG_PEEK.
 *
 *    RETURN VALUE: Upon successful completion, recvmsg() returns the length  of
 *    the  message  in  bytes.  If no messages are available to be
 *    received and the peer has  performed  an  orderly  shutdown,
 *    recvmsg() returns 0.  Otherwise, -1 is returned and errno is
 *    set to indicate the error.
 ****************************************************************************/
int
sctp_recvmsg(
    int                  sd,
    sctp_msghdr_st       *p_message,
    int                  flags )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    int            ret     = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_recvmsg" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !p_message )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvmsg:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvmsg:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Different behaviour in case of TCP and UDP.
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        ret = sctp_recvmsg_tcp( p_ep, p_message, flags );
    }

    else
    {
        ret = sctp_recvmsg_udp( p_ep, p_message, flags );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_setsockopt
 *
 *    DESCRIPTION : Applications use setsockopt to set socket options
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ***************************************************************************/
int
sctp_setsockopt(
    int                   sd,
    int                   level,
    int                   optname,
    const void            *optval,
    sctp_socklen_t        optlen )
{
    sctp_ep_st  *p_ep = SCTP_NULL;
    int         ret   = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_setsockopt" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_EBADF;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "setsockopt:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Check the optval pointer */
    if( !optval )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "setsockopt:Null OPtion Value Pointer " ) );

        return ( SK_FAILURE );
    }


    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Check that the level parameter is IPPROTP_SCTP
     */
    if( ( IPPROTO_SCTP != level ) && ( SOL_SOCKET != level ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "setsockopt:Invalid socket descriptor " ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* SPR : 18527 */
    if( level == IPPROTO_SCTP )
    {

        /* Set the SCTP level options */
        ret =  sctp_setsockopt_sctp( p_ep, optname, optval, optlen );
    }

    else if( level == SOL_SOCKET )
    {
        /* Set the Socket level options */
        ret = sctp_setsockopt_socket( p_ep, optname, optval, optlen );

    }

    /* SPR : 18527 */
    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_getsockopt
 *
 *    DESCRIPTION : Applications use getsockopt to get socket options
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ***************************************************************************/
int
sctp_getsockopt(
    int                   sd,
    int                   level,
    int                   optname,
    /* SPR 4926 */
    void                  *optval,
    sctp_socklen_t        *optlen )
/*SPR 4926*/
{
    sctp_ep_st  *p_ep = SCTP_NULL;
    int         ret   = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_getsockopt" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_EBADF;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "setsockopt:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Check that the level parameter is IPPROTP_SCTP or SOL_SOCKET and that
     * optval is not NULL. SOL_SOCKET is used to set the options at the EP
     * level, whereas IPPROTO_SCTP is used to set options at the Assoc level.
     */
    if( ( ( IPPROTO_SCTP != level ) && ( SOL_SOCKET != level ) ) ||
            ( !optval ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "setsockopt:Invalid Parameters " ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* SPR : 18527 */
    if( level == IPPROTO_SCTP )
    {
        /* Set the SCTP level options */
        ret =  sctp_getsockopt_sctp( p_ep, optname, optval, optlen );
    }

    else if( level == SOL_SOCKET )
    {
        /* Set the Socket level options */
        ret = sctp_getsockopt_socket( p_ep, optname, optval, optlen );
    }

    /* SPR : 18527 */

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}


/*****************************************************************************
 *    FUNCTION    : sctp_getpeername
 *
 *    DESCRIPTION : to retrieve the primary socket address of the peer.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 *
 ****************************************************************************/
int
sctp_getpeername(
    int                     sd,
    struct sockaddr         *p_addr,
    sctp_socklen_t          *len )
{
    sctp_ep_st       *p_ep  = SCTP_NULL;
    sctp_tcb_st      *p_tcb = SCTP_NULL;
    sctp_sockaddr_st *p_sctp_addr = SCTP_NULL;
    sctp_error_t     s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_getpeername" ) );

    if( !len || !p_addr )
    {
        sk_errno = SK_EINVAL;

        return ( SK_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpeername:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * If the socket is not of type TCP, or listener or association does not
     * exist then return failure.
     */
    if( ( SCTP_SOCK_STREAM != p_ep->sk_type )
            || SCTP_SOCK_IS_LISTENER( p_ep->flags )
            || ( p_ep->n_assoc == 0 ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpeername:Invalid param\n" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Get the TCB for the endpoint
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_tcb || ( SCTPSTATE_ESTABLISHED != p_tcb->state ) )
    {
        /* There is no connection */
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpeername: The socket is not connected" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* Return the peer primary address */
    p_sctp_addr = &( p_tcb->transmit.remote[p_tcb->transmit.
                                            primary_address_index].that_address );

    SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_addr ) ) =
        ( p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

    if( SCTP_FAILURE == sctp_conv_addr_sctp_to_sock_os( 1, p_sctp_addr,
                                                        p_addr, p_tcb->transmit.that_port,
                                                        &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpeername: Error converting address " ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    if( p_addr->sa_family == AF_INET )
    {
        *len = sizeof( struct sockaddr_in );
    }

#ifdef SCTP_INET6_SUPPORT

    else
    {
        *len = sizeof( struct sockaddr_in6 );
    }

#endif


    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( SK_SUCCESS );
}


/****************************************************************************
 *    FUNCTION    : sctp_getsockname
 *
 *    DESCRIPTION : Applications use to retrieve the locally-bound socket
 *    address of the specified socket.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 ****************************************************************************/
int
sctp_getsockname(
    int               sd,
    struct sockaddr   *p_addr,
    sctp_socklen_t    *len )
{
    sctp_ep_st       *p_ep  = SCTP_NULL;
    sctp_sockaddr_st *p_sctp_addr = SCTP_NULL;
    sctp_error_t     s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_getsockname" ) );

    if( !len || !p_addr )
    {
        sk_errno = SK_EINVAL;
        return ( SK_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getsockname:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * If the socket is not bound then we cannot return the sock name, so just
     * check the sockname and return it.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getsockname: Invalid param: socket is not bound\n" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* Return the peer primary address */
    p_sctp_addr = &( p_ep->this_address[0] );
    SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_addr ) ) =
        ( p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

    if( SCTP_FAILURE == sctp_conv_addr_sctp_to_sock_os( 1, p_sctp_addr,
                                                        p_addr, p_ep->this_port, &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getsockname: Cannot convert address\n" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );

    }

    if( p_addr->sa_family == AF_INET )
    {
        *len = sizeof( struct sockaddr_in );
    }

#ifdef SCTP_INET6_SUPPORT

    else
    {
        *len = sizeof( struct sockaddr_in6 );
    }

#endif


    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( SK_SUCCESS );
}


/****************************************************************************
 *    FUNCTION    : sctp_getpaddrs
 *
 *    DESCRIPTION : Applications use this to get the peer address list
 *
 *    RETURN VALUE: Retruns number of address on success and returns 0 if no
 *    address exists. This API WILL NOT return (-1).
 ****************************************************************************/
int
sctp_getpaddrs(
    int                      sd,
    sctp_assoc_t             assoc_id,
    struct sockaddr          **p_addr )
{
    sctp_ep_st       *p_ep        = SCTP_NULL;
    sctp_tcb_st      *p_tcb       = SCTP_NULL;
    sctp_sockaddr_list_st     sctp_addr_list;
    sctp_sockaddr_storage_st  *p_storage = SCTP_NULL;
    sctp_U32bit      n_addr       = 0;
    sctp_U32bit      count        = 0;
    sctp_error_t     s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_getpaddrs" ) );

    if( !p_addr )
    {
        sk_errno = SK_EINVAL;

        return ( SK_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpaddrs:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * If the socket is not of type TCP, or listener or association does not
     * exist then return failure.
     */
    if( ( SCTP_SOCK_IS_LISTENER( p_ep->flags )
            && ( SCTP_SOCK_STREAM == p_ep->sk_type ) ) || ( !p_ep->n_assoc ) )
    {
        sk_errno = SK_EINVAL;
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpaddrs:Invalid param\n" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Get the TCB for the endpoint
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
    }

    else
    {
        p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
    }

    if( !p_tcb )
    {
        /* There is no connection */
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpaddrs: The socket is not connected" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( 0 ); /* According to the API doc */
    }

    /* Malloc for the sockaddr_storage */
    n_addr = p_tcb->transmit.num_remote_address;

    p_storage = ( sctp_sockaddr_storage_st * )
                malloc( sizeof( sctp_sockaddr_storage_st ) * n_addr );

    if( !p_storage )
    {
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpaddrs: No memory" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Copy the peer addrs from the transmit structure in the TCB into the
     * local array.
     */
    for( count = 0; count < n_addr; count++ )
    {
        sctp_addr_list[count] = p_tcb->transmit.remote[count].that_address;
    }

    SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_storage ) ) =
        ( p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

    if( SCTP_FAILURE == sctp_conv_addr_sctp_to_sock_os( n_addr, sctp_addr_list,
                                                        ( struct sockaddr * )p_storage, p_tcb->transmit.that_port, &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getpaddrs: Cannot convert address\n" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );
        /* SPR : 18527 */
        free( p_storage );
        /* SPR : 18527 */
        return ( SK_FAILURE );
    }

    *p_addr = ( struct sockaddr * )p_storage;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( n_addr );
}


/****************************************************************************
 *    FUNCTION    : sctp_freepaddrs
 *
 *    DESCRIPTION : Free the paddr list allocated in getpaddrs
 *
 *    RETURN VALUE: void
 ****************************************************************************/
void
sctp_freepaddrs(
    struct sockaddr *p_addr )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_freepaddrs" ) );

    if( !p_addr )
    {
        free( p_addr );
    }
}


/****************************************************************************
 *    FUNCTION    : sctp_getladdrs
 *
 *    DESCRIPTION : Applications use this to get the local address list
 *
 *    RETURN VALUE: Returns number of address on success and returns 0 if no
 *    address exists. This API WILL NOT return (-1).
 ****************************************************************************/
int
sctp_getladdrs(
    int                      sd,
    sctp_assoc_t             assoc_id,
    struct sockaddr          **p_addr )
{
    sctp_ep_st       *p_ep        = SCTP_NULL;
    sctp_sockaddr_storage_st  *p_storage = SCTP_NULL;
    sctp_U32bit      n_addr       = 0;
    sctp_error_t     s_err;
    sctp_sockaddr_st *p_local_addr = NULL;
    sctp_port_t      port = 0;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( assoc_id );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_getladdrs" ) );

    if( !p_addr )
    {
        sk_errno = SK_EINVAL;
        return ( SK_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( 0 != sd )
    {
        if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
        {
            sk_errno = SK_ENOTSOCK;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "getladdrs:Invalid socket descriptor " ) );

            return ( SK_FAILURE );
        }

        /* Lock the endpoint */
        SCTP_LOCK_EP( sd );

        if( !( SCTP_SOCK_IS_BOUND( p_ep->flags ) ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "getladdrs:Invalid socket descriptor " ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( sd );

            return ( 0 );
        }

        p_local_addr = p_ep->this_address;
        port         = p_ep->this_port;
        n_addr = p_ep->naddr;
    }

    else
    {
        n_addr = sctp_db_main.num_local_addrs;
    }

    /* Malloc for the sockaddr_storage */
    p_storage = ( sctp_sockaddr_storage_st * )
                malloc( sizeof( sctp_sockaddr_storage_st ) * n_addr );

    if( !p_storage )
    {
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getladdrs: No memory" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    if( p_ep && p_ep->sk_config.want_mapped )
    {
        SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_storage ) ) =
            SCTP_AF_INET6;
    }

    else
    {
        SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_storage ) ) =
            SCTP_AF_INET;
    }

    if( SCTP_FAILURE == sctp_conv_addr_sctp_to_sock_os( n_addr, p_local_addr,
                                                        ( struct sockaddr * )p_storage, port, &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "getladdrs: cannot convert address " ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );
        /* SPR : 18527 */
        free( p_storage );
        /* SPR : 18527 */
        return ( SK_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "just befre p_addr" ) );

    *p_addr = ( struct sockaddr * )p_storage;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( n_addr );
}


/****************************************************************************
 *    FUNCTION    : sctp_freeladdrs
 *
 *    DESCRIPTION : Free the laddr list allocated in getladdrs
 *
 *    RETURN VALUE: void
 ****************************************************************************/
void
sctp_freeladdrs(
    struct sockaddr *p_addr )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_freeladdrs" ) );

    if( !p_addr )
    {
        free( p_addr );
    }
}


/****************************************************************************
 *    FUNCTION    : sctp_opt_info
 *
 *    DESCRIPTION : Interface wich allows information to be passed both in to
 *    and out of the SCTP stack.
 *
 *    RETURN VALUE: 0 on success -1 on failure
 ****************************************************************************/
int
sctp_opt_info(
    int                sd,
    sctp_assoc_t       assoc_id,
    int                opt,
    void               *p_args,
    sctp_socklen_t     *size )
{
    int ret;
    sctp_ep_st  *p_ep = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "In Function sctp_opt_info" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    switch( opt )
    {
        case SCTP_PEER_ADDR_PARAMS :
            {
                ret = sctp_setsockopt_sctp( p_ep, opt , p_args, *size );
            }
            break;

        default:
            {
                *( ( sctp_U32bit * )p_args ) = assoc_id;
                ret = sctp_getsockopt_sctp( p_ep, opt, p_args, size );
            }
            break;
    }

    SCTP_UNLOCK_EP( sd );
    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_peeloff
 *
 *    DESCRIPTION : Peels off an association from a UDP style socket and
 *    creates a new socket for the association.
 *
 *    RETURN VALUE: Retruns the new socket descriptor on success. (-1) on
 *    failure.
 ****************************************************************************/
int
sctp_peeloff(
    int                sd,
    sctp_assoc_t       assoc_id ) /* The Assoc to be brannched */
{
    sctp_ep_st           *p_ep   = SCTP_NULL;
    sctp_tcb_st          *p_tcb  = SCTP_NULL;
    sctp_ep_st           *p_new_ep = SCTP_NULL;
    sctp_error_t         s_err;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "In Function sctp_accept" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Check whether the socket is a server socket.
     */
    if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Endpoint not a listner" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* Get the pointer to the TCB */
    p_tcb = sctp_db_get_association_ptr( sd, assoc_id );

    if( !p_tcb )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "The tcb does not exist" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Create a new endpoint with the tcb that's extracted.
     */
    if( !sctp_db_create_duplicate_ep( p_ep, p_tcb, &p_new_ep, &s_err ) )
    {
        /* Unknown error occured while creating the new ep */
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "nonblocking_accept:Accept failed: couldnot accept new assoc." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /* Set the branched off socket to be a Stream socket. */
    p_new_ep->sk_type = SCTP_SOCK_STREAM;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( p_new_ep->ep_id );
}


/****************************************************************************
 *    FUNCTION    : sctp_sctp_sendmsg
 *
 *    DESCRIPTION : A new API to to assist the user with the advanced
 *                  features of SCTP.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 *
 ****************************************************************************/
int sctp_sctp_sendmsg(
    int             sd,
    const void      *msg,
    sctp_socklen_t  len,
    const struct sockaddr *to_addr,
    sctp_socklen_t       tolen,
    uint32_t        ppid,
    uint32_t        flags,
    uint16_t        stream_no,
    uint32_t        timetolive,
    uint32_t        context )
{
    sctp_iovec_st  p_iovec[1];
    sctp_return_t   ret;
    sctp_msghdr_st   message;
    sctp_sndrcvinfo_st     *p_sri  = SCTP_NULL;
    struct cmsghdr          *p_cmsg = SCTP_NULL;
    char   cbuf[sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr )];
    size_t cmsglen = sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_sctp_sendmsg" ) );

    p_iovec[0].iov_base = ( char * )msg;
    p_iovec[0].iov_len  = len;

    p_cmsg = ( struct cmsghdr * )cbuf;
    p_cmsg->cmsg_level = IPPROTO_SCTP;
    p_cmsg->cmsg_type  = SCTP_SNDRCV;
    p_cmsg->cmsg_len   = sizeof( struct cmsghdr ) + sizeof( sctp_sndrcvinfo_st );

    p_sri  = ( sctp_sndrcvinfo_st * )( ( char * )p_cmsg + sizeof( struct cmsghdr ) );
    p_sri->sinfo_stream   = stream_no;
    p_sri->sinfo_flags    = flags;
    p_sri->sinfo_context  = context;
    p_sri->sinfo_ppid     = ppid;
    p_sri->sinfo_timetolive = timetolive;

    message.msg_name        = ( struct sockaddr * ) to_addr;
    message.msg_namelen     = tolen;
    message.msg_iov         = ( struct iovec * )p_iovec;
    message.msg_iovlen      = 1;
    message.msg_control     = p_cmsg;
    message.msg_controllen  = cmsglen;

    /* start spr 21157 */
    if( message.msg_iovlen == 1 )
    {
        /* end spr 21157 */

        ret = sctp_sendmsg( sd, &message, flags );
        /* start spr 21157 */
    }

    /* end spr 21157 */

    return ret;
}

/****************************************************************************
 *    FUNCTION    : sctp_sctp_send
 *
 *    DESCRIPTION : A new API to send data without cmsg header.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 *
 ****************************************************************************/
int sctp_sctp_send(
    int             sd,
    const void      *msg,
    sctp_socklen_t  len,
    const           sctp_sndrcvinfo_st *sininfo,
    int             flags )

{
    sctp_iovec_st  p_iovec[1];
    /*Bug Id 22 fix starts*/
    /*sctp_return_t   ret;*/
    int ret = SK_SUCCESS;
    /*Bug Id 22 fix ends*/
    sctp_msghdr_st   message;
    struct cmsghdr          *p_cmsg = SCTP_NULL;
    char   cbuf[sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr )];
    size_t cmsglen = sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_sctp_send" ) );

    p_iovec[0].iov_base = ( char * )msg;
    p_iovec[0].iov_len  = len;

    p_cmsg = ( struct cmsghdr * )cbuf;
    p_cmsg->cmsg_level = IPPROTO_SCTP;
    p_cmsg->cmsg_type  = SCTP_SNDRCV;
    p_cmsg->cmsg_len   = sizeof( struct cmsghdr ) + sizeof( sctp_sndrcvinfo_st );

    __MEMCOPY( ( cbuf + sizeof( struct cmsghdr ) ) , sininfo, sizeof( sctp_sndrcvinfo_st ) );

    message.msg_name        = NULL;
    message.msg_namelen     = 0;
    message.msg_iov         = ( struct iovec * )p_iovec;
    message.msg_iovlen      = 1;
    message.msg_control     = p_cmsg;
    message.msg_controllen  = cmsglen;

    /* start spr 21157 */
    if( message.msg_iovlen == 1 )
    {
        /* end spr 21157 */

        ret = sctp_sendmsg( sd, &message, flags );
        /* start spr 21157 */
    }

    /* end spr 21157 */

    return ret;
}

/****************************************************************************
 *    FUNCTION    : sctp_sctp_sendx
 *
 *    DESCRIPTION : A new API to send data without cmsg header with multiple
 *    dest addrs.
 *
 *    RETURN VALUE: 0 on success and -1 on failure
 *
 ****************************************************************************/
int sctp_sctp_sendx(
    int                             sd,
    const void                      *msg,
    size_t                          len,
    struct sockaddr                 *addrs,
    int                             addrcnt,
    const struct sctp_sndrcvinfo    *sinfo,
    int                             flags )
{
    sctp_ep_st     *p_ep   = SCTP_NULL;
    int            ret     = SK_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_sctp_sendx" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* message cannot be null */
    if( !msg )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendto:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( sd ) ) )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendto:Invalid socket descriptor " ) );

        return ( SK_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( sd );

    /*
     * Cannot invoke sendx on a Listner socket which is TCP style.
     * In case of UDP, any socket that is bound is a listner.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) &&
            ( SCTP_SOCK_STREAM == p_ep->sk_type ) )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sendto:Listener socket" ) );

        SCTP_UNLOCK_EP( sd );

        return ( SK_FAILURE );
    }

    /*
     * Different behaviour in case of TCP and UDP.
     */
    if( SCTP_SOCK_STREAM == p_ep->sk_type )
    {
        ret = sctp_sendx_tcp( p_ep, ( sctp_U8bit * )msg, len,
                              addrs, addrcnt, sinfo, flags );
    }

    else
    {
        ret = sctp_sendx_udp( p_ep, ( sctp_U8bit * )msg, len,
                              addrs, addrcnt, sinfo, flags );
    }

    /* Unlock the Endpoint */
    SCTP_UNLOCK_EP( sd );

    return ( ret );
}

/****************************************************************************
 *    FUNCTION    : sctp_sctp_recvmsg
 *
 *    DESCRIPTION :This function is used to receive message
 *
 *    RETURN VALUE:SK_FAILURE/SK_SUCCESS
 *
 ****************************************************************************/
int
sctp_sctp_recvmsg( int s,
                   void *p_msg,
                   size_t p_len,
                   struct sockaddr *p_from,
                   sctp_socklen_t *p_fromlen,
                   struct sctp_sndrcvinfo *p_sinfo,
                   int *p_msg_flags
                 )
{
    int            recv_ret = SK_SUCCESS;
    static         sctp_msghdr_st       message;
    sctp_iovec_st  p_iovec[1];
    int            flags = 0;
    char           addr_buf[sizeof( sctp_sockaddr_storage_st )];
    char           cbuf[1000];
    /* Changes for CSR#1-7977745 start*/
    *p_msg_flags = 0;
    /* Changes for CSR#1-7977745 end*/

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_sctp_recvmsg" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        sk_errno = SK_EACCES;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    if( !p_msg )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvmsg:Some parameters are invalid" ) );

        return ( SK_FAILURE );
    }

    p_iovec[0].iov_base = ( char * )p_msg;
    p_iovec[0].iov_len  = p_len;

    message.msg_iov = ( struct  iovec * )p_iovec;
    message.msg_iovlen      = 1;
    message.msg_control = cbuf;
    message.msg_controllen = 1000;
    message.msg_name  = addr_buf;
    message.msg_namelen = sizeof( sctp_sockaddr_storage_st );


    recv_ret = sctp_recvmsg( s, &message, flags );

    /* Changes for CSR#1-7977745 start*/
    if( recv_ret >= 0 )
    {
        /* Changes for CSR#1-7977745 end*/
        *p_msg_flags = message.msg_flags;
        *p_from = *( struct sockaddr * )( message.msg_name );
        *p_fromlen = message.msg_namelen;
        *p_sinfo = *( sctp_sndrcvinfo_st * )( ( char * )message.msg_control
                                              + sizeof( struct cmsghdr ) );
    }

    else
    {
        /*Stack error no has been set already*/
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    return recv_ret;
}

#endif /* SCTP_UI_SOCK_API */
