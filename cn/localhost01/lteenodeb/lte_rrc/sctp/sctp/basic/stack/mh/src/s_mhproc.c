/******************************************************************************
 *    FILE NAME:
 *    s_mhproc.c
 *
 *    DESCRIPTION:
 *    This file is part of message Handling module. It includes
 *    functions that process a sctp chunk before invoking fsm triggers
 *
 *    DATE               NAME     REFERENCE     REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -       .Original Creation
 *    23Sept 2000   Sandeep Mahajan  -        SPR No 473
 *    13Oct  2000   hbhondwe        SPR472    Fix
 *    10Oct  2000   hbhondwe        IWAVE     Vxworks port.
 *    16Jan  2001   vagarg          SPR952    Fix
 *    16Jan 2001    vagarg  -       SPR957    Fix
 *    27Aug01       gsheoran                  Rel 3.0
 *    20May02       ygahlaut        SPR 4073 4035 4086 Fix
 *    30Jan03       mrajpal                   cookie chunk on 64bit OS
 *    16Nov04       svakumar                   Rel 6.0
 *    3Mar05        Amit Ray                   5.0.5 Change Merge
 *    09May08       Raza            REL 6.2    SPR 18527
 *    07july2008    Shikha Kathpal  REL 6.2.0  RFC Upgrade
 *    24May08       Raza            REL 6.2    CSR 1-5991001 Merge
 *    24May10       Akanksha        SPR 20406  CSR 1-8599117-Post Restart,handling incomplete
 *    1May13        Pranav Sharma   SPR 21107  CSR 44034
 *    1May13       Pranav       SPR 21127  CSR 40731-GCC 4.7 warnig removal
 *    25Oct13      Pinky Sinha      SPR 21157  CSR 56258
 *    21May14      Pinky            Bug Id 6
 *    15Sep14   Priyanka Mishra     Bug Id 22     CSR 85223
 *    6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
*     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "MH"

#include <s_includ.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_osmutx.h>
#include <s_cobuf.h>
#include <s_dbtcb.h>
#include <s_fsmass.h>
#include <s_mhbld.h>
#include <s_mhmac.h>
#include <s_mhpars.h>
#include <s_mhproc.h>
#include <s_mmmain.h>
#include <s_mhtbl.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_in.h>
#include <s_tlif.h>
#include <s_uisuif.h>
#include <s_resnam.h>
#include <s_dbutil.h>
#include <s_comd5.h>
#include <s_uiport.h>


static void
sctp_send_abort_restart_with_new_addr(
    sctp_packet_st     *p_sctp,
    sctp_U32bit        init_vtag,
    sctp_U32bit        n_addr,
    sctp_sockaddr_st   *p_addrs );



/*****************************************************************************
 *    FUNCTION :
 *        sctp_chk_addr_unicast
 *
 *    DESCRIPTION:
 *        Checks that the address passed is a valid IPv4/IPv6 unicast address.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 ****************************************************************************/
sctp_return_t
sctp_chk_addr_unicast(
    sctp_sockaddr_st  *p_addr )
{
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_chk_addr_unicast" ) );

    if( SCTP_AF_INET == p_addr->family )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_chk_addr_unicast: Check multicast address V4, Addr:" ) );

        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_addr );

        if( ( ( p_addr->ip.v4.addr & 0XF0000000 ) >= 0XE0000000 ) ||
                /* Check for multicast address*/
                ( ( p_addr->ip.v4.addr & 0XFF ) == 0XFF ) ||
                /* Check for broadcast address */
                ( ( p_addr->ip.v4.addr & 0XFF ) == 0X0 ) )
            /* Check for network address */
        {
            return ( SCTP_FAILURE );
        }
    }

    else
    {
        /* Check that the Address is a valid IPv6 Unicast address */
        /*
         * Unicast addresses are distinguished from multicast addresses by the
         * value of the high-order octet of the addresses: a value of FF
         * (11111111) identifies an address as a multicast address; any other
         * value identifies an address as a unicast address.  Anycast addresses
         * are taken from the unicast address space, and are not syntactically
         * distinguishable from unicast addresses.
         */

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_chk_addr_unicast: Check multicast address V6, Addr = " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_addr );

        if( 0XFF == ( p_addr->ip.v6.addr.s_u6_addr8[15] & 0XFF ) )
        {
            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_addrs_for_unicast_values
 *
 *    DESCRIPTION:
 *        If any of two addresses passed is a non-unicast value, return failure
 *
 *    RETURN VALUE:
 *        IF Not Unicast then retuurn SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_chk_addrs_for_unicast_values(
    sctp_sockaddr_st *p_src_addr,
    sctp_sockaddr_st *p_dst_addr )
{
    if( ( sctp_chk_addr_unicast( p_src_addr ) == SCTP_FAILURE ) ||
            ( sctp_chk_addr_unicast( p_dst_addr ) == SCTP_FAILURE ) )
    {
        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}

/*
 * Functions to process the chunk init
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_init
 *
 *    DESCRIPTION:
 *        This function will process the init chunk received. First
 *        parse init (check Init for all mandatory and optional parameters).
 *        process optional parameters (extract all optional parameters from
 *        the buffer). check if this init is for an existing association
 *        (duplicate init), if so let state machine handle it. If not checks
 *        if there is a sctp server on given destination port; send init reply
 *        in this   case. else reply with abort.
 *
 *        Init can be the only chunk in the SCTP packet, So this function always
 *        return success. Change it if needed.
 *
 *        from_addr  = src_address = tl_packet.src_address
 *        to_address = dest_address = tl_packet.dst_address
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_init(
    sctp_packet_st    *p_sctp,
    sctp_U8bit        *buffer,
    sctp_U32bit       chunk_offset,
    sctp_Boolean_t    last_chunk_flag,
    sctp_tcb_st       *p_assoc,
    sctp_error_t      *p_ecode )
{
    /* Bug Id 6 fix */
    /*sctp_U8bit    unresolved_addr[SCTPPARAM_LEN_IPV6ADDRESS+1];*/
    sctp_U8bit    unresolved_addr[SCTPPARAM_LEN_IPV6ADDRESS + 1] = {0};
    /* Bug Id 6 fix */
    sctp_U32bit   cookie_preservative = 0;
    sctp_error_t  ecode;
    sctp_U32bit   counter;
    sctp_return_t ret;

    sctp_chunk_init_st init_chunk;
    sctp_U8bit    hostname[SCTP_MAX_HOSTNAME_SIZE];
    sctp_U32bit   hostname_len = 0;

    /* for unrecognise parameters */
    sctp_Boolean_t  unreco_param_present = SCTP_FALSE;
    sctp_U16bit     unrecog_param_len = 0;
    sctp_U8bit      invalid_parameters[SCTP_MAX_OPT_PARAMS_TOTAL_SIZE];
    sctp_Boolean_t  hostname_present = SCTP_FALSE;
    sctp_sockaddr_st    *p_prim_addr = SCTP_NULL;

    sctp_U16bit    non_init_params_len = 0;
    sctp_U8bit     non_init_params[SCTP_MAX_OPT_PARAMS_TOTAL_SIZE];

    /*  SPR 21107 changes start*/
    SCTP_UNUSED_VARIABLE( p_prim_addr );
    /* SPR 21107 changes end*/

    /* parse Init chunk for the validity of the message. */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_init" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_INIT_RECV, 1 );

    __MEMSET( ( sctp_U8bit * )&init_chunk, 0, sizeof( sctp_chunk_init_st ) );

    if( sctp_chk_addr_unicast( &( p_sctp->p_tl_packet->src_addr ) ) == SCTP_FAILURE )
    {
        /* If the INIT is from a Non-Unicast Address,
         * Silently Discard the Packet. */
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init : ERROR :: INIT received from a "
                      "Non-Unicase Address : " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                         ( &( p_sctp->p_tl_packet->src_addr ) ) );

        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init : Discarding the Packet" ) );

        return ( SCTP_FAILURE );
    }

    /* Parse the Init chunk and extract the parameters */
    ret = sctp_parse_chk_chunk_init( buffer,
                                     chunk_offset,
                                     &init_chunk,
                                     &unreco_param_present,
                                     &unrecog_param_len,
                                     &invalid_parameters[0],
                                     &unresolved_addr[0],
                                     &non_init_params_len,
                                     non_init_params,
                                     &ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC , ( "Error in Message Parsing" ) );

        switch( ecode )
        {
            case E_INVALID_FIXED_PARAMETERS:
                /* This error message is for Invalid mandatory parameters */
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_init: Invalid Mandatory Parameters" ) );

                sctp_send_chunk_abort( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       init_chunk.fixed_params.init_tag,
                                       SCTP_TRUE,
                                       SCTPERR_INVMANDPARAM,
                                       0,
                                       NULL, 0, p_sctp->checksum );

                return ( SCTP_SUCCESS );
                break;

            case E_UNSUPPORTED_ADDR:
                /* send abort with error chunk,unresolved addr */
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_init: Unsupported address" ) );
                {
                    sctp_U16bit  cause_len;
                    cause_len = sctp_buffer_get_UshortAtOffset( unresolved_addr,
                                                                SCTPPARAMOFFSET_LENGTH );

                    sctp_send_chunk_abort( p_assoc,
                                           &( p_sctp->p_tl_packet->dst_addr ),
                                           p_sctp->dst_port,
                                           p_sctp->src_port,
                                           &( p_sctp->p_tl_packet->src_addr ),
                                           init_chunk.fixed_params.init_tag,
                                           SCTP_TRUE,
                                           SCTPERR_UNRESOLVEADDR,
                                           cause_len,
                                           &unresolved_addr[0], 0, p_sctp->checksum );
                }
                return ( SCTP_SUCCESS );
                break;

            case E_INVALID_ADDR:
                {
                    sctp_U16bit  cause_len;
                    cause_len = sctp_buffer_get_UshortAtOffset( unresolved_addr,
                                                                SCTPPARAMOFFSET_LENGTH );

                    /* send abort with error chunk,unresolved addr */
                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                                ( "sctp_process_chunk_init: INAVLID address" ) );

                    sctp_send_chunk_abort( p_assoc,
                                           &( p_sctp->p_tl_packet->dst_addr ),
                                           p_sctp->dst_port,
                                           p_sctp->src_port,
                                           &( p_sctp->p_tl_packet->src_addr ),
                                           init_chunk.fixed_params.init_tag,
                                           SCTP_TRUE,
                                           SCTPERR_UNRESOLVEADDR,
                                           cause_len,
                                           &unresolved_addr[0], 0, p_sctp->checksum );

                    return ( SCTP_SUCCESS );
                }
                break;


            case E_INVALID_NUM_OPTIONAL_PARAMS:
            case E_INVALID_CHUNK_SIZE:
            case E_INVALID_PARAMETER_LEN:

                sctp_send_chunk_abort( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       init_chunk.fixed_params.init_tag,
                                       SCTP_TRUE,
                                       SCTPERR_PROTOCOL_VIOLATION,
                                       0,
                                       NULL, 0, p_sctp->checksum );

                return ( SCTP_SUCCESS );
                break;

            case E_UNKNOWN_PARAMETER_ID:
                break;

            case E_UNKNOWN_PARAMETER_RETURN_ERROR:
                break;

            case E_UNKNOWN_PARAMETER_INIT_RETURN_ERROR:
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                                ( "sctp_process_chunk_init: "
                                  "Send Error with COOKIE-ACK later due to unknown init parameter type " ) );
                }
                break;

            default:
                /* We should continue Processing. */
                /* return (SCTP_SUCCESS); */
                break;
        }
    }

    /* Cannot receive a INIT with 0 verification tag */
    if( p_sctp->v_tag != 0 )
    {
        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init: INIT chunk with verification "
                      "tag != 0" ) );

        return ( SCTP_SUCCESS ); /* Verification tag not zero, drop message  */
    }

    /* Process the parameters that have been parsed */
    for( counter = 0; counter < init_chunk.num_opt_params; counter++ )
    {
        if( init_chunk.opt_params[counter].param_type == SCTPVARPAR_HOSTNAME )
        {
            /*
             * Drop any other address parameters and delay hostname resolution
             * till cookie arrival.
             */

            init_chunk.num_addrs = 0;
            hostname_present = SCTP_TRUE;

            hostname_len = init_chunk.opt_params[counter].param_len;

            /* Remove length of chunk ID and length */
            hostname_len = hostname_len - 4;

            sctp_buffer_setZero( &hostname[0], SCTP_MAX_HOSTNAME_SIZE );

            sctp_buffer_set_Buffer( &hostname[0], hostname_len,
                                    &init_chunk.opt_params[counter].param_data.hostname[0] );
        }

        else if( init_chunk.opt_params[counter].param_type
                 == SCTPVARPAR_COOKIEPRESV )
        {
            cookie_preservative
                = init_chunk.opt_params[counter].param_data.cookie_preservative;
        }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

        else if( init_chunk.opt_params[counter].param_type
                 == SCTP_ASCONF_PARAM_SET_PRIM )
        {
            p_prim_addr =
                &( init_chunk.opt_params[counter].param_data.pref_primaddr );
        }

#endif

    }/* for loop */


    /* Add src_address to address list if not already present */
    for( counter = 0; counter < init_chunk.num_addrs; counter++ )
    {
        if( SCTP_SUCCESS
                == sctp_cmp_addr( &( p_sctp->p_tl_packet->src_addr ),
                                  &init_chunk.addr_list[counter] ) )
        {
            break;
        }
    }

    if( counter == init_chunk.num_addrs )
    {
        /* Check if number of addresses equal to MAX
           transport addresses supported */

        if( init_chunk.num_addrs >= SCTP_MAX_TRANSPORT_ADDR )
        {
            sctp_send_chunk_abort( p_assoc,
                                   &( p_sctp->p_tl_packet->dst_addr ),
                                   p_sctp->dst_port,
                                   p_sctp->src_port,
                                   &( p_sctp->p_tl_packet->src_addr ),
                                   init_chunk.fixed_params.init_tag,
                                   SCTP_FALSE,
                                   0, 0, NULL, 0, p_sctp->checksum );

            return ( SCTP_SUCCESS );
        }

        /* Adding source address to the list */
        init_chunk.addr_list[init_chunk.num_addrs]
            = p_sctp->p_tl_packet->src_addr;
        init_chunk.num_addrs++;
    }

    /*if set primary parameter is present, swap the address with first index*/
    /*  SPR 21107 changes start*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT

    /* SPR 21107 changes end*/
    if( SCTP_NULL != p_prim_addr )
    {
        for( counter = 0; counter < init_chunk.num_addrs; counter++ )
        {
            if( SCTP_SUCCESS
                    == sctp_cmp_addr( p_prim_addr, &init_chunk.addr_list[counter] ) )
            {
                /*swap this address with first address*/
                if( counter != 0 )
                {
                    init_chunk.addr_list[counter] = init_chunk.addr_list[0];
                    init_chunk.addr_list[0] = *p_prim_addr;
                }

                break;
            }
        }

        /* silently ignore the parameter if could not found the address */
    }

    /*  SPR 21107 changes start*/
#endif
    /* SPR 21107 changes end*/

    return sctp_process_chunk_init_after_checks(
               p_sctp,
               buffer,
               &init_chunk,
               chunk_offset,
               last_chunk_flag,
               hostname_present,
               hostname,
               cookie_preservative,
               unreco_param_present,
               unrecog_param_len,
               invalid_parameters,
               non_init_params_len,
               non_init_params,
               p_ecode );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_init_after_checks
 *
 *    DESCRIPTION:
 *        Helper function for process init. Basically processes the init after
 *    all the basic checks have been made.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS or SCTP_FAILURE.
 *************************************************************************/
sctp_return_t
sctp_process_chunk_init_after_checks(
    sctp_packet_st      *p_sctp,
    sctp_U8bit          *buffer,
    sctp_chunk_init_st  *p_init_chunk,
    sctp_U32bit         chunk_offset,
    sctp_Boolean_t      last_chunk_flag,
    sctp_Boolean_t      hostname_present,
    sctp_U8bit          hostname[],
    sctp_U32bit         cookie_preservative,
    sctp_Boolean_t      unreco_param_present,
    sctp_U16bit         unrecog_param_len,
    sctp_U8bit          invalid_parameters[],
    sctp_U16bit         non_init_params_len,
    sctp_U8bit          *p_non_init_params,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit          counter = 0;
    sctp_chunk_cookie_st cookie;
    sctp_ep_st           *p_ep   = SCTP_NULL;
    sctp_tcb_st          *p_ass  = SCTP_NULL;
    sctp_U32bit          init_tag = 0;
    /*Bug Id 22 fix starts*/
    /*sctp_return_t        ret     = 0;*/
    sctp_return_t        ret     = SCTP_FAILURE;
    /*Bug Id 22 fix ends*/
    sctp_Boolean_t       ecn_flag = SCTP_FALSE;
    sctp_U8bit           digest[SCTP_KEYSIZE];
    sctp_Boolean_t       assoc_found = SCTP_FALSE;
    sctp_U32bit          n_new_addrs = 0;
    sctp_sockaddr_list_st sctp_addr_list;
    sctp_sockaddr_list_st sctp_new_addr_list;
    sctp_U32bit           adaption_ind = SCTP_NULL_ADAPTION_IND;
    sctp_suppaddr_st      *p_supported = SCTP_NULL;
    sctp_addrinfo_st      addr_info;
    sctp_Boolean_t       unlock = SCTP_FALSE;
    sctp_chunk_cookie_st *p_cookie = SCTP_NULL;
    sctp_U16bit          cookie_len = 0;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( chunk_offset );
    SCTP_UNUSED_VARIABLE( last_chunk_flag );

    /*to avoid the warning*/



    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_process_chunk_init_after_checks" ) );

    /* Check for duplicate Init case first */
    for( counter = 0; counter < p_init_chunk->num_addrs; counter++ )
    {
        /* Try to locate the association for the address */
        ret = sctp_db_locate_association( p_sctp->src_port,
                                          &( p_init_chunk->addr_list[counter] ),
                                          p_sctp->dst_port,
                                          &( p_sctp->p_tl_packet->dst_addr ),
                                          &p_ass );

        /*
         * Found a match for the address tuple. p_ass is the pointer to the
         * TCB of the already established association.
         */
        if( ret == SCTP_SUCCESS )
        {
            assoc_found = SCTP_TRUE;
            break;
        }
    }

    /* for ipv4, v6 addr support within an assoc...rel 4.0*/

    for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
    {
        if( p_init_chunk->opt_params[counter].param_type
                == SCTPVARPAR_SUPADDRTYPE )
        {
            p_supported = &( p_init_chunk->opt_params[counter].param_data.
                             supported_addrtypes );
            break;
        }
    }

    /*
     * If we were able to find a association
     */
    if( assoc_found )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init_after_checks:Got Duplicate Init " ) );

        /* fill association data */
        p_init_chunk->p_tl_packet = p_sctp->p_tl_packet;
        p_ass->event.msg_init.init_chunk = p_init_chunk;

        /*
         * This is the duplicate init case we need to check whether
         * the init had any extra addresses from what we already have in the
         * association? if yes then return error.
         */
        for( counter = 0; counter < p_ass->transmit.num_remote_address;
                counter++ )
        {
            /* Copy the address from the TCB */
            sctp_addr_list[counter] =
                p_ass->transmit.remote[counter].that_address;
        }

        if( p_ass->transmit.num_remote_address > p_init_chunk->num_addrs )
        {
            sctp_db_get_addrs_not_present(
                p_ass,
                &( n_new_addrs ),
                ( sctp_sockaddr_st * )sctp_new_addr_list,
                p_init_chunk->num_addrs,
                ( sctp_sockaddr_st * )( p_init_chunk->addr_list ),
                p_ass->transmit.num_remote_address,
                ( sctp_sockaddr_st * )sctp_addr_list,
                p_ecode );
        }

        else
        {
            sctp_db_get_addrs_not_present(
                p_ass,
                &( n_new_addrs ),
                ( sctp_sockaddr_st * )sctp_new_addr_list,
                p_ass->transmit.num_remote_address,
                ( sctp_sockaddr_st * )sctp_addr_list,
                p_init_chunk->num_addrs,
                ( sctp_sockaddr_st * )( p_init_chunk->addr_list ), p_ecode );
        }

        /* Are there any new addresses */
        /* SPR 4086 */
        if( n_new_addrs )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "sctp_process_chunk_init_after_checks: init recieved "
                          "assoc exists " ) );

            {
                sctp_U32bit ep_id = p_ass->p_ep->ep_id;
                sctp_U32bit assoc_id = p_ass->assoc_id;
                SCTP_LOCK_EP( ep_id );
                p_ass = sctp_db_get_association_ptr( ep_id, assoc_id );

                if( SCTP_NULL == p_ass )
                {
                    SCTP_UNLOCK_EP( ep_id );
                    /* SPR : 18527 */
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                                ( "ERROR : Association b/w assoc_id : %d and ep_id : %d doesn't exist.",
                                  assoc_id, ep_id ) );

                    return ( SCTP_FAILURE );
                    /* SPR : 18527 */
                }
            }

            if( p_ass->state != SCTPSTATE_COOKIEWAIT )
            {
                /* Build the error message and send it. */
                sctp_send_abort_restart_with_new_addr( p_sctp,
                                                       p_init_chunk->fixed_params.init_tag,
                                                       n_new_addrs, ( sctp_sockaddr_st * ) sctp_new_addr_list );
                /* Send error message */

                SCTP_UNLOCK_EP( p_ass->p_ep->ep_id );
                return ( SCTP_FAILURE );
            }

            /*Init collision: check that addresses are compatible */
            else if( SCTP_FALSE == sctp_db_is_v4_v6_addr_compatible( p_ass->p_ep,
                                                                     p_init_chunk->num_addrs, p_init_chunk->addr_list, p_supported ) )
            {
                /*
                 * Address family does not matched
                 */
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_init_after_checks: Address Family mismatch" ) );

                sctp_send_chunk_abort( p_ass,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       p_init_chunk->fixed_params.init_tag,
                                       SCTP_FALSE,
                                       0,
                                       0,
                                       NULL, 0, p_sctp->checksum );

                SCTP_UNLOCK_EP( p_ass->p_ep->ep_id );

                return ( SCTP_FAILURE );
            }
        }

        SCTP_UNLOCK_EP( p_ass->p_ep->ep_id );
        {
            sctp_U32bit ep_id = p_ass->p_ep->ep_id;
            sctp_U32bit assoc_id = p_ass->assoc_id;
            SCTP_LOCK_EP( ep_id );
            p_ass = sctp_db_get_association_ptr( ep_id, assoc_id );

            /* Invoke the association FSM with event INIT */
            sctp_assoc_state_machine( p_ass, SCTPEVENT_MSGINIT );
            SCTP_UNLOCK_EP( ep_id );
        }
        return ( SCTP_SUCCESS );
    }

    /*
     * Not a duplicate Init, Locate Server listening on this port
     */
    ret = sctp_db_locate_server( p_sctp->dst_port,
                                 &( p_sctp->p_tl_packet->dst_addr ), &p_ep );

    /*get lock of the endpoint */
    if( ret == SCTP_SUCCESS )
    {
        sctp_U32bit assoc_id = p_ep->ep_id;
        SCTP_LOCK_EP( p_ep->ep_id );

        p_ep = sctp_db_get_ep_ptr( assoc_id );

        if( SCTP_NULL == p_ep )
        {
            SCTP_UNLOCK_EP( assoc_id );
            ret = SCTP_FAILURE;
        }
    }

    if( ret != SCTP_SUCCESS )
    {
        /*
         * Message not for any active server or active association, send abort
         */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init_after_checks: No Server, message dropped" ) );

        sctp_send_chunk_abort( p_ass,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_init_chunk->fixed_params.init_tag,
                               SCTP_TRUE,
                               SCTPERR_NORESOURCE,
                               0,
                               NULL, 0, p_sctp->checksum );

        if( unlock == SCTP_TRUE )
        {
            SCTP_UNLOCK_EP( p_ep->ep_id );
        }

        return ( SCTP_SUCCESS );
    }


    if( hostname_present && SCTP_FALSE == p_ep->hostname_supported )
    {
        sctp_U8bit  tmp_buffer[SCTP_MAX_HOSTNAME_SIZE + SCTPPARAMOFFSET_BODY];
        sctp_U8bit  *p_buff = tmp_buffer;
        sctp_U32bit hostname_len;

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init_after_checks: Hostname not supported" ) );

        /* strlen used change if needed  */
        hostname_len = strlen( ( char * )hostname );

        p_buff = sctp_buffer_set_Ushort( p_buff, SCTPVARPAR_HOSTNAME );

        p_buff = sctp_buffer_set_Ushort( p_buff,
                                         ( sctp_U16bit )( hostname_len + SCTPPARAMOFFSET_BODY ) );

        p_buff = sctp_buffer_set_Buffer( p_buff, hostname_len, hostname );

        p_buff = sctp_buffer_PadTo( p_buff, tmp_buffer, 4 );

        sctp_send_chunk_abort( p_ass,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_init_chunk->fixed_params.init_tag,
                               SCTP_TRUE,
                               SCTPERR_UNRESOLVEADDR,
                               ( sctp_U16bit )( p_buff - &tmp_buffer[0] ),
                               &tmp_buffer[0], 0, p_sctp->checksum );

        SCTP_UNLOCK_EP( p_ep->ep_id );

        return ( SCTP_SUCCESS );

    }

    if( SCTP_FALSE == sctp_db_is_v4_v6_addr_compatible( p_ep,
                                                        p_init_chunk->num_addrs, p_init_chunk->addr_list, p_supported ) )
    {
        /*
         * Address family does not matched or have combination of more than one
         * address family
         */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init_after_checks: Address Family mismatch" ) );

        sctp_send_chunk_abort( p_ass,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_init_chunk->fixed_params.init_tag,
                               SCTP_FALSE,
                               0,
                               0,
                               NULL, 0, p_sctp->checksum );
        SCTP_UNLOCK_EP( p_ep->ep_id );

        return ( SCTP_SUCCESS );
    }

    /*
     * Found a server for the INIT. A new incomming conenction.
     * We have a endpoint found , now we donot need to add a association to
     * this endpoint. Rather we create a cookie and send back.
     */

    /*
     * Check if we need to set the ecn flag in the association.
     */
    /*
     * If the incomming init contained a parameter for ecn capable then we
     * need to take care of that.
     */
    /*Bug Id 22 fix starts*/
    /*ecn_flag = 0;*/
    ecn_flag = SCTP_FALSE;
    /*Bug Id 22 fix ends*/

    if( SCTP_IS_ECN_CAPABLE )
    {
        for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
        {
            if( p_init_chunk->opt_params[counter].param_type
                    == SCTPVARPAR_ECNCAPABLE )
            {
                /*Bug Id 22 fix starts*/
                /*ecn_flag = 1;*/
                ecn_flag = SCTP_TRUE;
                /*Bug Id 22 fix ends*/
                break;
            }
        }
    }

    /* Initialize the cookie chunk to be sent to the peer */
    sctp_init_chunk_cookie( &cookie );

    /* Stream negotiation */
    if( p_ep->sk_config.num_out_streams >
            p_init_chunk->fixed_params.num_in_streams )
    {
        /*
         * Number of my out streams are more than peer's
         * incoming streams, negotiate down to lower streams
         */

        SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init_after_checks: Negotiate to lower Streams"
                      ", Peer MIS:%d, My OS:%d", p_init_chunk->fixed_params.num_in_streams,
                      p_ep->sk_config.num_out_streams ) );

        cookie.local_init_parameters.fixed_params.num_out_streams
            = p_init_chunk->fixed_params.num_in_streams;
    }

    else
    {
        cookie.local_init_parameters.fixed_params.num_out_streams
            = p_ep->sk_config.num_out_streams;
    }

    if( p_ep->sk_config.num_in_streams <
            p_init_chunk->fixed_params.num_out_streams )
    {
        /*
         * Cannot accept the stream value give. change it
         */
        cookie.local_init_parameters.fixed_params.num_in_streams
            = p_ep->sk_config.num_in_streams;
    }

    else
    {
        cookie.local_init_parameters.fixed_params.num_in_streams
            = p_init_chunk->fixed_params.num_out_streams;
    }

    for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
    {
        if( p_init_chunk->opt_params[counter].param_type
                == SCTPVARPAR_ADAPTIONIND )
        {
            adaption_ind =
                p_init_chunk->opt_params[counter].param_data.adaption_ind;
            break;
        }
    }

    __GETSYSTIME( &cookie.time_stamp );

    cookie.life_time   = p_ep->sk_config.valid_cookie_life
                         + cookie_preservative;
    cookie.ecn_capable = ecn_flag;

    /* Generate a random init tag for the cookie. */
    do
    {
        init_tag = __RANDOM();
    }
    while( 0 == init_tag );

    cookie.local_init_parameters.fixed_params.init_tag = init_tag;

    cookie.local_init_parameters.fixed_params.a_rwnd
        = sctp_db_get_dafault_rwnd();

    cookie.local_init_parameters.fixed_params.init_tsn = init_tag;
    cookie.local_tie_tag = 0;

    cookie.local_init_parameters.num_addrs = p_ep->naddr;

    for( counter = 0; counter < p_ep->naddr; counter++ )
    {
        cookie.local_init_parameters.addr_list[counter]
            = p_ep->this_address[counter];
    }

    cookie.this_port = p_sctp->dst_port; /*Added in Rel 6.2.0*/
    cookie.local_init_parameters.adaption_ind =
        p_ep->sk_config.adaption_ind;


    cookie.remote_init_parameters.fixed_params.init_tag
        = p_init_chunk->fixed_params.init_tag;

    cookie.remote_init_parameters.fixed_params.a_rwnd
        = p_init_chunk->fixed_params.a_rwnd;

    cookie.remote_init_parameters.fixed_params.num_in_streams
        = p_init_chunk->fixed_params.num_in_streams;

    cookie.remote_init_parameters.fixed_params.num_out_streams
        = p_init_chunk->fixed_params.num_out_streams;

    cookie.remote_init_parameters.fixed_params.init_tsn
        = p_init_chunk->fixed_params.init_tsn;

    cookie.peer_tie_tag = 0;

    cookie.remote_init_parameters.num_addrs = p_init_chunk->num_addrs;

    cookie.remote_init_parameters.hostname_present = hostname_present;

    if( hostname_present == SCTP_TRUE )
    {
        for( counter = 0; hostname[counter] != '\0'; counter++ )
        {
            cookie.remote_init_parameters.hostname[counter] =
                hostname[counter];
        }

        cookie.remote_init_parameters.hostname[counter] = '\0';
    }

    else
    {
        for( counter = 0; counter < p_init_chunk->num_addrs; counter++ )
        {
            cookie.remote_init_parameters.addr_list[counter] =
                p_init_chunk->addr_list[counter];
        }
    }

    /* Store the Peer's Address to which INIT-ACK will be sent. */
    cookie.that_address = p_sctp->p_tl_packet->src_addr;

    cookie.remote_init_parameters.adaption_ind = adaption_ind;
    cookie.dest_port = p_sctp->src_port;

    cookie_len = sizeof( sctp_chunk_cookie_st );

    if( non_init_params_len != 0 )
    {
        /* INTI Chunk received with Unexpected Parameters. */
        cookie_len += non_init_params_len;

        p_cookie = ( sctp_chunk_cookie_st * )__MALLOC( cookie_len );

        /* If Memory Allocation for p_cookie fails,
         * send the original Cookie to the Peer */

        if( p_cookie == SCTP_NULL )
        {
            SCTP_TRACE( p_ep, SCTP_DBG_TRC,
                        ( "sctp_process_chunk_init_after_checks: Memory Allocation [%d Bytes] for Cookie Failed",
                          cookie_len ) );

            p_cookie    = &cookie;
            cookie_len -= non_init_params_len;
        }

        else
        {
            __MEMCOPY( p_cookie, ( &cookie ), sizeof( sctp_chunk_cookie_st ) );

            /* SPR 20568 Starts */
            p_cookie->unexpected_init_params_len = non_init_params_len;
            /* SPR 20568 Ends */

            __MEMCOPY( ( p_cookie + 1 ), p_non_init_params, non_init_params_len );
        }
    }

    else
    {
        p_cookie = &cookie;
    }

    md5digest( ( sctp_U8bit * )( p_cookie ), cookie_len, digest );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        p_cookie->digest[counter] = digest[counter];
    }

    /* send init reply */
    addr_info.checksum   = p_ep->sk_config.checksum_func;
    addr_info.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );
    addr_info.p_info     = &( p_ep->sk_config.nat_capable );

    sctp_send_chunk_initack( p_ass,
                             &( p_sctp->p_tl_packet->dst_addr ),
                             p_sctp->dst_port,  /* my source port */
                             p_sctp->src_port,  /* destination port */
                             &addr_info,
                             p_init_chunk->fixed_params.init_tag,
                             p_cookie,
                             cookie_len,
                             unreco_param_present,
                             unrecog_param_len,
                             &invalid_parameters[0],
                             NULL,
                             p_ep->sk_config.adaption_ind );
    SCTP_UNLOCK_EP( p_ep->ep_id );

    if( non_init_params_len != 0 )
    {
        __FREE( p_cookie );
    }

    return ( SCTP_SUCCESS );
}


/*
 * End of Functions to process the chunk init
 */


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_initack
 *
 *    DESCRIPTION:
 *        This function will process the initack chunk received. First
 *        parse Initack (check Initack for all mandatory and optional
 *        parameters). Process optional parameters (extract all optional
 *        parameters from the buffer). Check if verification tag is
 *        valid. Locate association for this INITACK. Pass this as
 *        event to association state machine.
 *
 *        InitAck can be the only chunk in the SCTP packet, So this function
 *        always return success. Change it if needed.
 *
 *
 *    DATE         NAME     REFERENCE     REASON
 *    -----------------------------------------------------
 *    16Jan 2001   vagarg    SPR952      Fix
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 ************************************************************************/
sctp_return_t
sctp_process_chunk_initack(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    /* Bug Id 6 fix starts */
    /* sctp_error_t     ecode;*/
    sctp_error_t     ecode = E_NOERROR;
    /* Bug Id 6 fix ends */
    sctp_return_t    ret;
    sctp_U32bit      counter;
    sctp_U8bit       tmp_buffer[SCTP_MAX_HOSTNAME_SIZE + SCTPPARAMOFFSET_BODY];
    /* Bug Id 6 fix starts*/
    /*  sctp_U8bit       unresolved_addr[SCTPPARAM_LEN_IPV6ADDRESS];*/
    sctp_U8bit       unresolved_addr[SCTPPARAM_LEN_IPV6ADDRESS] = {0};
    /* Bug Id 6 fix ends */
    sctp_U8bit       *p_buff;
    sctp_chunk_initack_st initack_chunk;
    sctp_Boolean_t   unreco_param_present = SCTP_FALSE;
    sctp_U16bit      unrecog_param_len = 0;
    sctp_U8bit       invalid_parameters[SCTP_MAX_OPT_PARAMS_TOTAL_SIZE];
    /*Bug Id 22 fix starts*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    /*Bug Id 22 fix ends*/
    sctp_U32bit      prim_addr_index = 0;
    sctp_Boolean_t   prim_param_present = SCTP_FALSE;
    sctp_sockaddr_st    *p_prim_addr = SCTP_NULL;
    /*Bug Id 22 fix starts*/
#endif
    /*Bug Id 22 fix ends*/


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_process_chunk_initack " ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_INITACK_RECV, 1 );

    __MEMSET( ( sctp_U8bit * ) &initack_chunk, 0, sizeof( sctp_chunk_initack_st ) );


    if( sctp_chk_addr_unicast( &( p_sctp->p_tl_packet->src_addr ) ) == SCTP_FAILURE )
    {

        /* If the INIT-ACK is from a Non-Unicast Address,
         * Silently Discard the Packet. */
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack : ERROR :: INIT-ACK received from a "
                      "Non-Unicase Address : " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                         ( &( p_sctp->p_tl_packet->src_addr ) ) );

        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack : Discarding the Packet" ) );

        return ( SCTP_FAILURE );
    }


    /* Parse Initack Chunk  */
    ret = sctp_parse_chk_chunk_initack( buffer, chunk_offset,
                                        &initack_chunk, &unreco_param_present, &unrecog_param_len,
                                        &invalid_parameters[0], &unresolved_addr[0], &ecode );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack: Error in Message Parsing" ) );

        switch( ecode )
        {
            case E_MISSING_PARAMS:
                /* Send Error code missing mandatory parameter */
                p_buff = tmp_buffer;
                p_buff = sctp_buffer_set_Ulong( p_buff, 1UL );
                p_buff = sctp_buffer_set_Ushort( p_buff, SCTPVARPAR_COOKIE );

                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_initack:Send Abort: cookie not "
                              "present" ) );

                sctp_send_chunk_abort( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       initack_chunk.fixed_params.init_tag,
                                       SCTP_TRUE,
                                       SCTPERR_MISSINGPARAM,
                                       ( sctp_U16bit )( p_buff - tmp_buffer ),
                                       tmp_buffer, 0, p_sctp->checksum );

                return ( SCTP_SUCCESS );
                break;

            case E_INVALID_FIXED_PARAMETERS:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_initack: Invalid Mandatory Parameters" ) );

                sctp_send_chunk_abort( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       initack_chunk.fixed_params.init_tag,
                                       SCTP_TRUE,
                                       SCTPERR_INVMANDPARAM,
                                       0,
                                       NULL, 0, p_sctp->checksum );

                return ( SCTP_SUCCESS );
                break;

            case E_UNSUPPORTED_ADDR:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_initack: addr type not supported " ) );
                {
                    sctp_U16bit  cause_len;
                    cause_len = sctp_buffer_get_UshortAtOffset( unresolved_addr,
                                                                SCTPPARAMOFFSET_LENGTH );

                    sctp_send_chunk_abort( p_assoc,
                                           &( p_sctp->p_tl_packet->dst_addr ),
                                           p_sctp->dst_port,
                                           p_sctp->src_port,
                                           &( p_sctp->p_tl_packet->src_addr ),
                                           initack_chunk.fixed_params.init_tag,
                                           SCTP_TRUE,
                                           SCTPERR_UNRESOLVEADDR,
                                           cause_len,
                                           &unresolved_addr[0], 0, p_sctp->checksum );
                }

                return( SCTP_SUCCESS );
                break;

            case E_INVALID_NUM_OPTIONAL_PARAMS:
            case E_INVALID_CHUNK_SIZE:
            case E_INVALID_PARAMETER_LEN:

                sctp_send_chunk_abort( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       initack_chunk.fixed_params.init_tag,
                                       SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                                       0, NULL, 0, p_sctp->checksum );

                return ( SCTP_SUCCESS );
                break;

            case E_UNKNOWN_PARAMETER_RETURN_ERROR:


                break;

            case E_UNKNOWN_PARAMETER_INITACK_RETURN_ERROR:
                {

                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                                ( "sctp_process_chunk_initack: Send Error Chunk bundled with cookie-echo due to unknown parameter type "
                                  "(if possible)" ) );
                }
                break;


            default:
                break;
        }
    }

    /*
     * check in we need to send error chunk in response of unrecognised
     * parameters
     */
    /* If the address type is hostname, resolve address Name */
    for( counter = 0; counter < initack_chunk.num_opt_params; counter++ )
    {
        if( initack_chunk.opt_params[counter].param_type == SCTPVARPAR_HOSTNAME )
        {
            initack_chunk.num_addrs = 0;
            /* RESOLVE hostname and get network addresses */
            ret = sctp_TL_resolve_hostname(
                      &initack_chunk.opt_params[counter].param_data.hostname[0],
                      &initack_chunk.num_addrs, &initack_chunk.addr_list[0] );

            if( ret == SCTP_FAILURE )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_initack: Err in Hostname resolving" ) );

                p_buff = tmp_buffer;
                p_buff = sctp_buffer_set_Ushort( p_buff,
                                                 initack_chunk.opt_params[counter].param_type );
                p_buff = sctp_buffer_set_Ushort( p_buff,
                                                 initack_chunk.opt_params[counter].param_len );
                p_buff = sctp_buffer_set_Buffer( p_buff,
                                                 initack_chunk.opt_params[counter].param_len
                                                 - SCTPPARAMOFFSET_BODY,
                                                 &initack_chunk.opt_params[counter].param_data.
                                                 hostname[0] );

                p_buff = sctp_buffer_PadTo( p_buff, tmp_buffer, 4 );

                sctp_send_chunk_abort( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       initack_chunk.fixed_params.init_tag ,
                                       SCTP_TRUE,
                                       SCTPERR_UNRESOLVEADDR,
                                       ( sctp_U16bit )( p_buff - tmp_buffer ),
                                       tmp_buffer, 0, p_sctp->checksum );

                return ( SCTP_SUCCESS ); /* Drop chunk */
            }
        } /* If Loop */

#ifdef SCTP_DYNAMIC_IP_SUPPORT

        else if( initack_chunk.opt_params[counter].param_type
                 == SCTP_ASCONF_PARAM_SET_PRIM )
        {
            p_prim_addr =
                &( initack_chunk.opt_params[counter].param_data.pref_primaddr );
        }

#endif

    }

    /* Add src_address to address list if not already present */
    for( counter = 0; counter < initack_chunk.num_addrs; counter++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr(
                    &( p_sctp->p_tl_packet->src_addr ),
                    &( initack_chunk.addr_list[counter] ) ) )
        {
            break;
        }
    }

    if( counter == initack_chunk.num_addrs ) /*src address not in the list*/
    {
        /* Check if number of addresses equal to MAX transport addresses
           supported */

        if( initack_chunk.num_addrs == SCTP_MAX_TRANSPORT_ADDR )
        {
            sctp_send_chunk_abort( p_assoc,
                                   &( p_sctp->p_tl_packet->dst_addr ),
                                   p_sctp->dst_port,
                                   p_sctp->src_port,
                                   &( p_sctp->p_tl_packet->src_addr ),
                                   initack_chunk.fixed_params.init_tag,
                                   SCTP_FALSE,
                                   0, 0, NULL, 0, p_sctp->checksum );

            return ( SCTP_SUCCESS );
        }

        /* Increament the no of remote addrs here and the src addr will be
         * added after this if block
         */
        /* SPR 4035 */
        /* SPR 4073 */
        initack_chunk.num_addrs++;

        /*
         * If the source addess is not in the list, store the address received
         * in the packet header into the primary address index and move the
         * primary address to the last location.
         */
        initack_chunk.addr_list[counter] = p_sctp->p_tl_packet->src_addr;
    }


    /*get the index of primary address */
    /*Bug Id 22 fix starts*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT

    /*Bug Id 22 fix ends*/
    if( p_prim_addr )
    {
        for( counter = 0; counter < initack_chunk.num_addrs; counter++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr( p_prim_addr,
                                               &( initack_chunk.addr_list[counter] ) ) )
            {
                prim_addr_index = counter;
                prim_param_present = SCTP_TRUE;
            }
        }
    }

    /*Bug Id 22 fix starts*/
#endif
    /*Bug Id 22 fix ends*/

    ret = SCTP_FAILURE;

    for( counter = 0; counter < initack_chunk.num_addrs; counter++ )
    {
        ret = sctp_db_locate_association( p_sctp->src_port,
                                          &( initack_chunk.addr_list[counter] ),
                                          p_sctp->dst_port,
                                          &( p_sctp->p_tl_packet->dst_addr ),
                                          &p_assoc );

        if( ret == SCTP_SUCCESS )
        {
            break;
        }
    }

    /*get lock of the endpoint */
    if( ret == SCTP_SUCCESS )
    {
        sctp_U32bit ep_id = p_assoc->p_ep->ep_id;
        sctp_U32bit assoc_id = p_assoc->assoc_id;
        SCTP_LOCK_EP( ep_id );

        p_assoc = sctp_db_get_association_ptr( ep_id, assoc_id );

        if( SCTP_NULL == p_assoc )
        {
            SCTP_UNLOCK_EP( ep_id );
            ret = SCTP_FAILURE;
        }
    }

    else
    {
        /*
         * find assoc with dest ipaddr and src addr and with STATE as cookie_Wait
         * if found update same with new dest port number
         * use same dest port now onward.*/
        ret = sctp_db_lct_n_updt_assoc_wo_dprt(
                  p_sctp->src_port,
                  initack_chunk.num_addrs,
                  initack_chunk.addr_list,
                  p_sctp->dst_port,
                  &( p_sctp->p_tl_packet->dst_addr ),
                  &p_assoc );

        /*CSR 1-3095485 */
        if( ret == SCTP_SUCCESS )
        {
            sctp_U32bit ep_id = p_assoc->p_ep->ep_id;
            sctp_U32bit assoc_id = p_assoc->assoc_id;
            SCTP_LOCK_EP( ep_id );

            p_assoc = sctp_db_get_association_ptr( ep_id, assoc_id );

            if( SCTP_NULL == p_assoc )
            {
                SCTP_UNLOCK_EP( ep_id );
                ret = SCTP_FAILURE;
            }
        }

        /* SPR : 18527 */
    }

    /* SPR : 18527 */

    /*CSR 1-3095485 END*/
    if( ret != SCTP_SUCCESS )
    {
        /* send abort */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack: Message for Invalid Association" ) );

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,        /* SPR952 (Rel 2.0, vagarg) */
                               SCTP_FALSE,
                               0,
                               0,
                               NULL, 1, p_sctp->checksum );

        return ( SCTP_SUCCESS );
    }

    /*
     * If the source addess is not in the list, store the address received
     * in the packet header into the primary address index and move the
     * primary address to the last location.
     */

    /*get the index of primary address */
    /*Bug Id 22 fix starts*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT

    /*Bug Id 22 fix ends*/
    if( SCTP_TRUE == prim_param_present )
    {
        p_assoc->transmit.configured_primary_path_index = prim_addr_index;
    }

    /*Bug Id 22 fix starts*/
#endif
    /*Bug Id 22 fix ends*/

    /* ipv4_v6 changed the logic */
    /*don't know if peer supports v6, this function checks for it*/
    if( SCTP_FALSE == sctp_db_is_v4_v6_addr_compatible( p_assoc->p_ep,
                                                        initack_chunk.num_addrs, initack_chunk.addr_list, SCTP_NULL ) )
    {
        /*
         * Address family does not matched or have combination of more than one
         * address family
         */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_init_after_checks: Address Family mismatch" ) );

        sctp_send_chunk_abort( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_FALSE,
                               0,
                               0,
                               NULL, 1, p_sctp->checksum );

        /* delete association */
        sctp_db_delete_ep( p_assoc->p_ep->ep_id, p_assoc, SCTP_FALSE );
        SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );

        return ( SCTP_SUCCESS );
    }

    /* check for verification tag */
    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
        return ( SCTP_FAILURE );
    }

    /* Stream negotiation */
    /*Fix for CSR 1-1789754*/
    if( p_assoc->config.num_in_streams
            > initack_chunk.fixed_params.num_out_streams )
    {
        /* For SPR No 473, negotiate down to lower streams */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack:Negotiate to lower Streams, "
                      "Peer MOS:%d, My IS:%d",
                      initack_chunk.fixed_params.num_out_streams,
                      p_assoc->config.num_in_streams ) );

        p_assoc->receive.num_in_streams
            = initack_chunk.fixed_params.num_out_streams;
    }

    /*end fix*/

    if( p_assoc->config.num_out_streams
            > initack_chunk.fixed_params.num_in_streams )
    {
        /* For SPR No 473, negotiate down to lower streams */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack:Negotiate to lower Streams, "
                      "Peer MIS:%d, My OS:%d",
                      initack_chunk.fixed_params.num_in_streams,
                      p_assoc->config.num_out_streams ) );

        p_assoc->transmit.num_out_streams
            = initack_chunk.fixed_params.num_in_streams;
    }

    /* success fill association data & inform state machine */
    p_assoc->event.msg_initack.initack_chunk = &initack_chunk;
    /*Added to send cookie to the same address from which init-ack is received*/
    p_assoc->event.msg_initack.from_addr = p_sctp->p_tl_packet->src_addr;

    if( unreco_param_present == SCTP_TRUE )
    {
        /*
         * This is the case where we need to skip processing of unrecog param
         * but need to return error to sender, parsing function returns success
         * in this  case. In this   case the error chunk is sent after we have
         * received cookie ack from the peer
         */

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_initack: Unrecognise parameters return Error"
                      " message" ) );

        p_assoc->event.msg_initack.send_err_with_cookie = SCTP_TRUE;

        p_assoc->event.msg_initack.unrecog_param_len = unrecog_param_len;

        p_assoc->event.msg_initack.unrecog_params = &invalid_parameters[0];

        /* Here we bundle error chunk with cookie. It is mandatory to bundle.*/
        /* Error chunk cannot be sent separately in this     case.*/
        /* This was told by Randall at bakeoff in France*/
    }

    else
    {
        p_assoc->event.msg_initack.send_err_with_cookie = SCTP_FALSE;
        p_assoc->event.msg_initack.unrecog_param_len = 0;

        p_assoc->event.msg_initack.unrecog_params = NULL;
    }

    /*
     * Invoke the Association State Machine: INIT ACK Received.
     */
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGINITACK );

    /* CSR# 1-3254474 */
    /* If we have accepted this initack and echoed cookie for this then
     * we should search other assocs in tcb with other ips assigned to
     * the same peer and whose state is cookie_wait and if found we should
     * delete these tcbs as already init_ack is accepted from one of the
     * multiple IPs assigned to the peer endpoint.  */
    if( SCTPSTATE_COOKIESENT == p_assoc->state )
    {
        sctp_tcb_st *p_other_assoc;

        for( counter = 0; counter < initack_chunk.num_addrs; counter++ )
        {
            ret = SCTP_FAILURE;
            p_other_assoc = SCTP_NULL;
            ret = sctp_db_locate_association( p_sctp->src_port,
                                              &( initack_chunk.addr_list[counter] ),
                                              p_sctp->dst_port,
                                              &( p_sctp->p_tl_packet->dst_addr ),
                                              &p_other_assoc );


            if( ret == SCTP_SUCCESS )
            {
                sctp_U32bit other_ep_id = p_other_assoc->p_ep->ep_id;
                sctp_U32bit other_assoc_id = p_other_assoc->assoc_id;

                if( other_ep_id != p_assoc->p_ep->ep_id )
                {
                    SCTP_LOCK_EP( other_ep_id );
                    p_other_assoc = sctp_db_get_association_ptr( other_ep_id,
                                                                 other_assoc_id );

                    if( SCTP_NULL != p_other_assoc )
                    {
                        if( SCTPSTATE_COOKIEWAIT == p_other_assoc->state )
                        {
                            sctp_stop_timer( &p_other_assoc->
                                             assoc_timer_id[SCTP_ASSOCTIMER_INIT],
                                             SCTP_NULL,
                                             SCTP_NULL );
                            sctp_db_delete_ep( p_other_assoc->p_ep->ep_id,
                                               p_other_assoc, SCTP_FALSE );
                        }
                    }

                    SCTP_UNLOCK_EP( other_ep_id );
                }
            }
        }
    }

    /* CSR# 1-3254474 Ends*/


    SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_cookie
 *
 *    DESCRIPTION:
 *        This function will process cookie message received. Which
 *        includes parsing cookie message, locating the server or an
 *        existing association for which cookie is received. If it is
 *        for a existing association (duplicate cookie     case) pass
 *        this event to association state machine. else create a new
 *        socket and send cookie ack.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_cookie(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U16bit          chunk_len;
    sctp_return_t        ret;
    sctp_chunk_cookie_st *p_cookie = SCTP_NULL;
    sctp_U16bit          cookie_len = 0;
    sctp_U16bit          unexpected_init_params_len = 0;
    /* SPR 20568 Starts */
    sctp_chunk_cookie_st *p_temp_cookie = NULL;
    sctp_U16bit          temp_cookie_len = 0;
    sctp_U8bit           *temp_buf = NULL;
    /* SPR 20568 Ends */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_cookie" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_COOKIE_RECV, 1 );

    if( sctp_chk_addr_unicast( &( p_sctp->p_tl_packet->src_addr ) ) == SCTP_FAILURE )
    {
        /* If the COOKIE-ECHO is from a Non-Unicast Address,
         * Silently Discard the Packet. */
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie : ERROR :: COOKIE-ECHO received from a "
                      "Non-Unicase Address : " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                         ( &( p_sctp->p_tl_packet->src_addr ) ) );

        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie : Discarding the Packet" ) );

        return ( SCTP_FAILURE );
    }

    /* SPR 20568 Starts */

    p_temp_cookie = ( sctp_chunk_cookie_st * ) __MALLOC( sizeof( sctp_chunk_cookie_st ) );

    if( p_temp_cookie == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie : Memory Allocation [%d Bytes] for Cookie Failed, "
                      " Drop the COOKIE !!", cookie_len ) );

        return ( SCTP_FAILURE );
    }

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    temp_buf = sctp_buffer_get_cookie( ( buffer + chunk_offset + SCTPCHUNKOFFSET_BODY ), p_temp_cookie );

    unexpected_init_params_len = p_temp_cookie->unexpected_init_params_len;

    temp_cookie_len = ( sctp_U16bit )( temp_buf - ( buffer + chunk_offset + SCTPCHUNKOFFSET_BODY ) ) + unexpected_init_params_len;

    /* If the chunk lenght is incorrect, silently discard the Cookie received */
    if( chunk_len != temp_cookie_len + SCTPCHUNKOFFSET_BODY )
    {
        /* Changes for CSR : 1-5991001  */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie Cookie len Invalid, Drop the Cookie !!" ) );

        return ( SCTP_FAILURE );
        /* Changes for CSR : 1-5991001 End. */
    }

    if( unexpected_init_params_len != 0 )
    {
        cookie_len = sizeof( sctp_chunk_cookie_st ) + unexpected_init_params_len ;

        p_cookie = ( sctp_chunk_cookie_st * )__MALLOC( cookie_len );

        if( p_cookie == SCTP_NULL )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_chunk_cookie : Memory Allocation [%d Bytes] for Cookie Failed, "
                          " Drop the COOKIE !!", cookie_len ) );

            return ( SCTP_FAILURE );
        }

        __MEMCOPY( p_cookie, ( p_temp_cookie ), sizeof( sctp_chunk_cookie_st ) );

        __MEMCOPY( ( p_cookie + 1 ), temp_buf, unexpected_init_params_len );
        __FREE( p_temp_cookie );

    }

    else
    {
        cookie_len = sizeof( sctp_chunk_cookie_st );
        p_cookie = ( sctp_chunk_cookie_st * )__MALLOC( cookie_len );

        if( p_cookie == SCTP_NULL )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_chunk_cookie : Memory Allocation [%d Bytes] for Cookie Failed, "
                          " Drop the COOKIE !!", cookie_len ) );

            return ( SCTP_FAILURE );
        }

        __MEMCOPY( p_cookie, ( p_temp_cookie ), sizeof( sctp_chunk_cookie_st ) );
        __FREE( p_temp_cookie );

    }

    /* SPR 20568 Ends */

    /* DO MAC Check on incoming cookie */
    ret = sctp_is_cookie_valid( p_cookie, cookie_len );

    if( ret == SCTP_FAILURE )
    {
        /* Invalid Cookie, discard message  */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie: Inv Cookie, Mac check failed " ) );

        __FREE( p_cookie );
        return ( SCTP_FAILURE );
    }

    /* Check ver tag in SCTP header is same as that in cookie chunk */
    if( p_cookie->local_init_parameters.fixed_params.init_tag
            != p_sctp->v_tag )
    {
        /* Invalid Cookie, discard message  */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie: Invalid Ver Tag, discard message" ) );

        __FREE( p_cookie );
        return ( SCTP_FAILURE );
    }

    /*Check port numbers in SCTP header are same as that in cookie chunk*/
    if( ( p_cookie->this_port != p_sctp->dst_port ) ||
            ( p_cookie->dest_port != p_sctp->src_port ) )
    {
        /* Invalid Cookie, discard message  */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookie: Invalid port number, discard message" ) );

        __FREE( p_cookie );
        return ( SCTP_FAILURE );
    }


    if( p_cookie->remote_init_parameters.hostname_present == SCTP_TRUE )
    {
        /* hostname present, resolve it in this thread or separate resolver
           thread */

#ifdef SCTP_NAME_RESOLV_THREAD

        /*
         * Else the hostname is present, Send the datagram to the name
         * resolver Lock the cookie queue and get the packet from queue
         */
        SCTP_RESOLVER_LOCK();

        /* Get the cookie packet from the queue */
        lstAdd( &recv_to_res_queue, ( NODE * )p_sctp->p_tl_packet );

        __SEMAPOST( &cookie_queue_count );

        /* Release the cookie queue lock */
        SCTP_RESOLVER_UNLOCK();

        *p_ecode = E_COOKIE_SENT_TO_RESOLVER;

        __FREE( p_cookie );
        return ( SCTP_FAILURE );

#else
        /*
         * There is no separate for hostname resolution thread. So hostname has
         * to be resolved in this thread itself. This hostname resolution may
         * block SCTP user if name resolution takes long time
         */

        /* Resolve hostname here*/
        ret = sctp_TL_resolve_hostname(
                  &( p_cookie->remote_init_parameters.hostname[0] ),
                  &( p_cookie->remote_init_parameters.num_addrs ),
                  &( p_cookie->remote_init_parameters.addr_list[0] ) );

        if( ret == SCTP_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_chunk_cookie: Error in Hostname resolving :%s ",
                          &( p_cookie->remote_init_parameters.hostname[0] ) ) );

            /* Set the error that hostname could not be resolved */
            *p_ecode = E_NAME_RES_FAILED;

            /*don't return from here*/
        }

        else
        {
            p_cookie->remote_init_parameters.hostname_present = SCTP_FALSE;
            /* copy back cookie_chunk struct to buffer*/
            /* SPR 20568 Starts for CSR 6658485 */
            sctp_buffer_set_cookie( ( buffer + chunk_offset + SCTPCHUNKOFFSET_BODY ), p_cookie );
            /* SPR 20568 Starts for CSR 6658485 */
        }

#endif
    }

    /* Hostname is resolved. Call resolved cookie process function */
    ret = sctp_process_resolved_cookie(
              p_sctp,
              buffer,
              chunk_offset,
              last_chunk_flag,
              p_assoc,
              p_ecode );

    __FREE( p_cookie );
    return ret;
}


/***************************************************************************
 *    FUNCTION :
 *       sctp_process_resolved_cookie
 *
 *    DESCRIPTION: This function process the cookie chunk after basic validation
 *                 Check and hst name resolution.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_resolved_cookie(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_return_t     ret;
    sctp_error_t      ecode;
    sctp_ep_st        *p_tmp_ep = SCTP_NULL;
    sctp_ep_st        *p_ep = SCTP_NULL;
    sctp_U8bit        *p_buff = SCTP_NULL;
    sctp_U16bit       hostname_len;
    /* Bug Id 6 fix starts */
    /* sctp_assoc_t      assoc_id;*/
    sctp_assoc_t      assoc_id = SCTP_INVALID_ASSOC;
    /* Bug Id 6 fix ends */
    sctp_U32bit       counter;
    sctp_U32bit       i_count;
    sctp_U32bit       time_diff;
    sctp_U32bit       stale_time = 0;
    sctp_time_t       curr_time;
    sctp_addrinfo_st  addr_info;
    sctp_U8bit        tmp_buffer[SCTP_MAX_HOSTNAME_SIZE + SCTPPARAMOFFSET_BODY];
    sctp_U32bit       daddr_index;
    sctp_U32bit       saddr_index;
    sctp_U32bit       dest_addr_counter;
    sctp_U16bit       confirmed_addr_index = SCTP_MAX_TRANSPORT_ADDR;
    sctp_chunk_cookie_st *p_cookie = SCTP_NULL;
    sctp_U16bit       cookie_len = 0;
    sctp_U16bit       unexpected_init_params_len = 0;
    /* SPR 20568 Starts */
    sctp_U8bit        *temp_buf = NULL;
    sctp_U16bit       temp_cookie_len = 0;
    sctp_chunk_cookie_st *p_temp_cookie = NULL;
    sctp_U16bit       chunk_len = 0;
    /* SPR 20568 Ends  */


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_resolved_cookie" ) );

    /* SPR 20568 Starts */

    p_temp_cookie = ( sctp_chunk_cookie_st * ) __MALLOC( sizeof( sctp_chunk_cookie_st ) );

    if( p_temp_cookie == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie : Memory Allocation [%d Bytes] for Cookie Failed, "
                      " Drop the COOKIE !!", cookie_len ) );

        return ( SCTP_FAILURE );
    }

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );


    temp_buf = sctp_buffer_get_cookie( ( buffer + chunk_offset + SCTPCHUNKOFFSET_BODY ), p_temp_cookie );

    unexpected_init_params_len = p_temp_cookie->unexpected_init_params_len;

    temp_cookie_len = ( sctp_U16bit )( temp_buf - ( buffer + chunk_offset + SCTPCHUNKOFFSET_BODY ) ) + unexpected_init_params_len;

    /* If the chunk lenght is incorrect, silently discard the Cookie received */
    if( chunk_len != temp_cookie_len + SCTPCHUNKOFFSET_BODY )
    {
        /* Changes for CSR : 1-5991001  */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie Cookie len Invalid, Drop the Cookie !!" ) );
        /*SPR 20737 Starts here */
        __FREE( p_temp_cookie );
        /*SPR 20737 Ends here */
        return ( SCTP_FAILURE );
        /* Changes for CSR : 1-5991001 End. */
    }



    if( unexpected_init_params_len != 0 )
    {
        cookie_len = sizeof( sctp_chunk_cookie_st ) + unexpected_init_params_len ;

        /*Bug Id 81 fix s*/
        if( cookie_len > SCTP_MAX_COOKIE_SIZE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid cookie size" ) );
            *p_ecode = E_INVALID_PARAMETER_LEN;

            return ( SCTP_FAILURE );
        }

        /*Bug Id 81 fix e*/

        p_cookie = ( sctp_chunk_cookie_st * )__MALLOC( cookie_len );

        if( p_cookie == SCTP_NULL )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_resolved_cookie : Memory Allocation [%d Bytes] for Cookie Failed, "
                          " Drop the COOKIE !!", cookie_len ) );
            /* SPR 20737 Starts here */
            __FREE( p_temp_cookie );
            /* SPR 20737 Ends here */
            return ( SCTP_FAILURE );

        }

        __MEMCOPY( p_cookie, ( p_temp_cookie ), sizeof( sctp_chunk_cookie_st ) );
        /* SPR 20737 Starts here */
        __FREE( p_temp_cookie );
        /* SPR 20737 Ends here */
        __MEMCOPY( ( p_cookie + 1 ), temp_buf, unexpected_init_params_len );

    }

    else
    {
        cookie_len = sizeof( sctp_chunk_cookie_st );
        p_cookie = ( sctp_chunk_cookie_st * )__MALLOC( cookie_len );

        if( p_cookie == SCTP_NULL )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_resolved_cookie : Memory Allocation [%d Bytes] for Cookie Failed, "
                          " Drop the COOKIE !!", cookie_len ) );
            /* SPR 20737 starts here */
            __FREE( p_temp_cookie );
            /* SPR 20737 Ends here */
            return ( SCTP_FAILURE );

        }

        __MEMCOPY( p_cookie, ( p_temp_cookie ), sizeof( sctp_chunk_cookie_st ) );
        /* SPR 20737 Starts here */
        __FREE( p_temp_cookie );
        /* SPR 20737 Ends here */

    }

    /* SPR 20568 ends */

    if( p_cookie->remote_init_parameters.hostname_present == SCTP_TRUE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie: Error in Hostname resolving" ) );

        /* copy hostname TLV in local buffer and send abort*/
        p_buff = &tmp_buffer[0];

        /* strlen used change if needed  */
        hostname_len = strlen( ( char * )
                               & ( p_cookie->remote_init_parameters.hostname[0] ) );

        /* Bug Id 19 fix starts*/
        /* Similar check done in function sctp_chk_api_bind */
        if( hostname_len == 0 || hostname_len >= SCTP_MAX_HOSTNAME_SIZE )
        {
            *p_ecode = EINV_PARAMS;
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_resolved_cookie: hostname length invalid" ) );
            /*Bug Id 81 fix s*/
            __FREE( p_cookie );
            /*Bug Id 81 fix e*/
            return( SCTP_FAILURE );
        }

        /* Bug Id 19 fix ends*/

        p_buff = sctp_buffer_set_Ushort( p_buff, SCTPVARPAR_HOSTNAME );

        p_buff = sctp_buffer_set_Ushort( p_buff,
                                         ( sctp_U16bit )( hostname_len + SCTPPARAMOFFSET_BODY ) );

        p_buff = sctp_buffer_set_Buffer( p_buff, hostname_len,
                                         &( p_cookie->remote_init_parameters.hostname[0] ) );

        p_buff = sctp_buffer_PadTo( p_buff, tmp_buffer, 4 );

        sctp_send_chunk_abort( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_cookie->remote_init_parameters.fixed_params.init_tag,
                               SCTP_TRUE,
                               SCTPERR_UNRESOLVEADDR,
                               ( sctp_U16bit )( p_buff - &tmp_buffer[0] ),
                               &tmp_buffer[0], 0, p_sctp->checksum );

        /* Add the buffer back to the global TL Buffer list */
        SCTP_TL_ADDBUF( p_sctp->p_tl_packet );
        __FREE( p_cookie );
        return ( SCTP_SUCCESS );
    }

    /* Add src address in the cookie packet if not already present  */
    for( counter = 0; counter < p_cookie->remote_init_parameters.num_addrs;
            counter++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr(
                    &( p_cookie->remote_init_parameters.addr_list[counter] ),
                    &( p_sctp->p_tl_packet->src_addr ) ) )
        {
            break;
        }
    }

    if( counter == p_cookie->remote_init_parameters.num_addrs )
    {


        sctp_send_chunk_abort( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_cookie->remote_init_parameters.fixed_params.init_tag,
                               SCTP_FALSE,
                               0, 0, NULL, 0, p_sctp->checksum );

        __FREE( p_cookie );

        return ( SCTP_SUCCESS );
    }

    __GETSYSTIME( &curr_time );

    if( sctp_calculate_time_difference( &curr_time,
                                        &( p_cookie->time_stamp ), &time_diff ) == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie: Invalid Time Stamp in Cookie" ) );

        __FREE( p_cookie );
        return ( SCTP_FAILURE ); /* This should never fail,discard message */
    }

    /* Stale time in Usec */
    if( time_diff > p_cookie->life_time )
    {
        stale_time = ( time_diff - p_cookie->life_time ) * 1000;
    }

    /* This is needed as we do only a buffer copy in send_chunk_error */
    stale_time = SCTP_HTONL( stale_time );

    /* Check for duplicate cookie case first.so try to locate assoc */
    for( i_count = 0; i_count < p_cookie->remote_init_parameters.num_addrs;
            i_count++ )
    {
        /* Try to locate the association for the address */
        ret = sctp_db_locate_association( p_sctp->src_port,
                                          &( p_cookie->remote_init_parameters.addr_list[i_count] ),
                                          p_sctp->dst_port,
                                          &( p_sctp->p_tl_packet->dst_addr ),
                                          &p_assoc );

        /*
         * Found a match for the address tuple. p_ass is the pointer to the
         * TCB of the already established association.
         */
        if( ret == SCTP_SUCCESS )
        {
            break;
        }
    }

    /* Check for Duplicate Cookie case first */
    if( p_assoc != SCTP_NULL )
    {
        if( time_diff > p_cookie->life_time )
        {
            /* cookie too old, send error only if tags match */
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_resolved_cookie: Got Duplicate Stale Cookie,"
                          " Stale time:%u", stale_time ) );

            /* Send error if the tags match for the cookie and assoc */
            if( ( p_assoc->local_tag
                    != p_cookie->local_init_parameters.fixed_params.init_tag )
                    || ( p_assoc->remote_tag
                         != p_cookie->remote_init_parameters.fixed_params.init_tag ) )
            {
                sctp_addrinfo_st    to;

                to.checksum   = p_sctp->checksum;
                to.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );
                sctp_send_chunk_error( p_assoc,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &to,
                                       p_cookie->remote_init_parameters.fixed_params.init_tag,
                                       SCTPERR_STALECOOKIE,
                                       4,
                                       ( sctp_U8bit * ) &stale_time );
            }

            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_resolved_cookie: Got state cookie "
                          "Machine" ) );

            p_assoc->event.msg_cookie.cookie_chunk = p_cookie;
            /* SPR FIX 20406 starts */
            p_assoc->event.msg_cookie.to_addr = p_sctp->p_tl_packet->dst_addr;

            if( unexpected_init_params_len != 0 )
            {
                /* Populate the unrecognized_params to be bundled with COOKIE-ACK */
                p_assoc->unrecognized_params.param_list = ( sctp_U8bit * )( p_cookie + 1 );
                p_assoc->unrecognized_params.length = unexpected_init_params_len;
            }

            /* SPR FIX 20406 stops */
            sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGCOOKIE );

        }

        else
        {
            /* duplicate cookie, inform state machine */
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_resolved_cookie: Got Duplicate Inform State "
                          "Machine" ) );

            p_assoc->event.msg_cookie.cookie_chunk = p_cookie;
            /* SPR FIX 20406 starts */
            p_assoc->event.msg_cookie.to_addr = p_sctp->p_tl_packet->dst_addr;

            if( unexpected_init_params_len != 0 )
            {
                /* Populate the unrecognized_params to be bundled with COOKIE-ACK */
                p_assoc->unrecognized_params.param_list = ( sctp_U8bit * )( p_cookie + 1 );
                p_assoc->unrecognized_params.length = unexpected_init_params_len;
            }

            /* SPR FIX 20406 stops */

            sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGCOOKIE );
        }

        __FREE( p_cookie );
        return ( SCTP_SUCCESS );
    } /* Duplicate Cookie Loop */

    /*
     * Not a duplicate cookie.
     * verify cookie is not old.
     */
    if( time_diff > p_cookie->life_time )
    {
        sctp_addrinfo_st    to;

        to.checksum   = p_sctp->checksum;
        to.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie: OLD Cookie,message dropped, "
                      "stale_time:%u, cookie_l_time:%u", stale_time,
                      p_cookie->life_time ) );

        /* cookie too old, send stale cookie error */
        sctp_send_chunk_error( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &to,
                               p_cookie->remote_init_parameters.fixed_params.init_tag,
                               SCTPERR_STALECOOKIE,
                               4,
                               ( sctp_U8bit * ) &stale_time );

        __FREE( p_cookie );
        return ( SCTP_FAILURE );
    }

    /*
     * Valid cookie message received : Locate a Server for this COOKIE.
     */
    ret = sctp_db_locate_server( p_sctp->dst_port,
                                 &( p_sctp->p_tl_packet->dst_addr ), &p_ep );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie: No Server for cookie,"
                      "TRY to get endpoint created from server endpoint" ) );

        if( p_cookie->this_port != p_sctp->dst_port )
        {
            /*create duplicate ep and set flag it is created from server ep
             * add that ep in the server ep set backlog as 1.
             */
            ret = sctp_db_locate_server( p_cookie->this_port,
                                         &( p_sctp->p_tl_packet->dst_addr ), &p_ep );

            if( ret == SCTP_SUCCESS )
            {
                sctp_U32bit association_id = p_ep->ep_id;
                SCTP_LOCK_EP( p_ep->ep_id );

                p_ep = sctp_db_get_ep_ptr( association_id );

                if( SCTP_NULL == p_ep )
                {
                    SCTP_UNLOCK_EP( association_id );
                    ret = SCTP_FAILURE;
                }
            }

            if( ret != SCTP_SUCCESS )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_resolved_cookie: No Server for cookie, "
                              "message dropped" ) );
                __FREE( p_cookie );
                return SCTP_FAILURE;
            }

            /*
             * Found the server. Now Check that the server backlog is not exceeded.
             * In case the basck log is exceeded then drop the message.
             * The other side can keep retrying to connect!
             */
            if( p_ep->n_assoc >= p_ep->l_backlog )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_resolved_cookie: No Space on Server Backlog queue,"
                              "message dropped" ) );

                SCTP_UNLOCK_EP( p_ep->ep_id );
                __FREE( p_cookie );
                return ( SCTP_FAILURE );
            }

            ret = sctp_db_create_duplicate_ep_with_port( p_ep,
                                                         &p_tmp_ep, p_sctp->dst_port, p_ecode );

            SCTP_UNLOCK_EP( p_ep->ep_id );

            if( ret == SCTP_SUCCESS )
            {
                sctp_U32bit association_id = p_tmp_ep->ep_id;
                SCTP_LOCK_EP( p_tmp_ep->ep_id );

                p_ep = sctp_db_get_ep_ptr( association_id );

                if( SCTP_NULL == p_ep )
                {
                    SCTP_UNLOCK_EP( association_id );
                    ret = SCTP_FAILURE;
                }
            }

            if( ret != SCTP_SUCCESS )
            {
                /* Send Abort with error cause no resources to peer */
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_resolved_cookie: No Resources,message dropped" ) );

                sctp_send_chunk_abort( SCTP_NULL,
                                       &( p_sctp->p_tl_packet->dst_addr ),
                                       p_sctp->dst_port,
                                       p_sctp->src_port,
                                       &( p_sctp->p_tl_packet->src_addr ),
                                       p_cookie->remote_init_parameters.fixed_params.init_tag,
                                       SCTP_TRUE,
                                       SCTPERR_NORESOURCE,
                                       0, NULL, 0, p_sctp->checksum );
                __FREE( p_cookie );
                return ( SCTP_FAILURE );
            }

            SCTPLOCK();
            sctp_db_delete_server_created_port( p_sctp->dst_port,
                                                p_ep->naddr, p_ep->this_address );
            SCTPUNLOCK();
        }
    }

    else
    {
        sctp_U32bit association_id = p_ep->ep_id;
        SCTP_LOCK_EP( p_ep->ep_id );

        p_ep = sctp_db_get_ep_ptr( association_id );

        if( SCTP_NULL == p_ep )
        {
            SCTP_UNLOCK_EP( association_id );
            ret = SCTP_FAILURE;
        }

        if( ret == SCTP_SUCCESS )
        {
            /*
             * Found the server. Now Check that the server backlog is not exceeded.
             * In case the basck log is exceeded then drop the message.
             * The other side can keep retrying to connect!
             */
            if( p_ep->n_assoc >= p_ep->l_backlog )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_resolved_cookie: No Space on Server Backlog queue,"
                              "message dropped" ) );

                SCTP_UNLOCK_EP( p_ep->ep_id );
                __FREE( p_cookie );
                return ( SCTP_FAILURE );
            }

        }
    }

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie: No Server for cookie, "
                      "message dropped" ) );
        __FREE( p_cookie );
        return SCTP_FAILURE;
    }

    /* Add the new association to the database */
    ret = sctp_db_add_association(
              p_ep->ep_id,
              p_sctp->src_port,
              p_cookie,
              &assoc_id, /* The ptr to the new TCB is returned here */
              &ecode );

    p_assoc = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );

    /*
     * This shall add a new association to the  Endpoint specified and then
     * return us a pointer to the tcb.
     */
    /* SPR : 18527 */
    if( ret != SCTP_SUCCESS || p_assoc == SCTP_NULL )
        /* SPR : 18527 */
    {
        /* Send Abort with error cause no resources to peer */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_resolved_cookie: No Resources,message dropped" ) );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_cookie->remote_init_parameters.fixed_params.init_tag,
                               SCTP_TRUE,
                               SCTPERR_NORESOURCE,
                               0, NULL, 0, p_sctp->checksum );

        if( p_ep->create_from_server == SCTP_TRUE )
        {
            /* delete association */
            sctp_db_delete_ep( p_ep->ep_id, p_assoc, SCTP_FALSE );
        }

        SCTP_UNLOCK_EP( p_ep->ep_id );
        __FREE( p_cookie );
        return ( SCTP_FAILURE );
    }

    sctp_db_add_assoc_verf( p_assoc );

    for( counter = 0; counter < p_assoc->transmit.num_remote_address; counter++ )
    {
        /* If this is Peer Address to which INIT-ACK was sent,
         * mark it as confirmed and send a Indication to ULP. */
        if( SCTP_SUCCESS == sctp_cmp_addr(
                    &( p_assoc->transmit.remote[counter].that_address ),
                    &( p_cookie->that_address ) ) )
        {
            p_assoc->transmit.remote[counter].is_confirmed = SCTP_TRUE;

            /* Store the Index of the Confirmed Remote Address */
            confirmed_addr_index = counter;

            if( counter != p_assoc->transmit.configured_primary_path_index )
            {
                /* Mark the configured primary path Address as DOWN
                 * since we have not received COOKIE-ECHO from Destination
                 * address which has been configured as the Primary Path. */
                p_assoc->transmit.remote[p_assoc->transmit.configured_primary_path_index].status
                    = SCTP_COMMUNICATION_LOST;

                /* Update the Current Primary Path index to the Index of the Confirmed Address */
                p_assoc->transmit.primary_address_index = counter;
            }

            break;
        }
    }

    /* start spr 21157 */
    for( counter = 0; counter < p_assoc->p_ep->naddr &&  counter < SCTP_MAX_TRANSPORT_ADDR; counter++ )
        /* end spr 21157 */
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( &( p_assoc->p_ep->this_address[counter] ), &( p_sctp->p_tl_packet->dst_addr ) ) )
        {
            /* Update the Src addr indx for the Current Primary Path Index. */
            p_assoc->transmit.remote[p_assoc->transmit.primary_address_index].src_addr_index = counter;
            break;
        }
    }

    /*SCTP rel 5.0.5*/
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_assoc, &saddr_index,
                                                        &daddr_index, &ecode ) )
    {
        SCTP_TRACE( p_assoc->p_ep, SCTP_ERR_TRC, ( "get address failed index for src and dst" ) );
        __FREE( p_cookie );
        return SCTP_FAILURE;
    }

    /* send Cookie ACK */
    addr_info.checksum   = p_assoc->config.checksum_func;
    addr_info.p_sockaddr = &( p_assoc->transmit.remote[daddr_index].that_address );

    if( unexpected_init_params_len != 0 )
    {
        /* Populate the unrecognized_params to be bundled with COOKIE-ACK */
        p_assoc->unrecognized_params.param_list = ( sctp_U8bit * )( p_cookie + 1 );
        p_assoc->unrecognized_params.length = unexpected_init_params_len;
    }

    sctp_send_chunk_cookieack( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &addr_info,
                               p_assoc->remote_tag );

    for( counter = 0, dest_addr_counter = daddr_index; counter < p_assoc->transmit.num_remote_address ; counter++ )
    {
        p_assoc->transmit.remote[dest_addr_counter].src_addr_index = saddr_index;

        saddr_index++;

        if( saddr_index == p_assoc->p_ep->naddr )
        {
            saddr_index = 0;
        }

        dest_addr_counter++;

        if( dest_addr_counter ==  p_assoc->transmit.num_remote_address )
        {
            dest_addr_counter = 0;
        }

    }

    daddr_index = p_assoc->transmit.configured_primary_path_index;

    p_assoc->transmit.remote[p_assoc->transmit.primary_address_index].status
        = SCTP_COMMUNICATION_UP;

    if( ( p_assoc->configured_local_primary_address_index !=
            p_assoc->transmit.remote[p_assoc->transmit.primary_address_index].src_addr_index ) ||
            ( p_assoc->transmit.configured_primary_path_index !=
              p_assoc->transmit.primary_address_index ) )

    {
        sctp_start_timer(
            p_assoc,
            SCTPEVENT_TIMER_LOCALHBEAT,
            /* SPR 20570 Starts for CSR 1-7626869 */
            SCTP_HB_INTERVAL_TIMEOUT,
            /* SPR 20570 Ends for CSR 1-7626869 */
            daddr_index,
            &p_assoc->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT],
            SCTP_NULL );
    }

    /*
     * Send Indication to the Server that A new connection is received.
     */
    p_assoc->notify.connpending.assoc_id = p_ep->ep_id;

    p_assoc->config.adaption_ind =
        p_cookie->remote_init_parameters.adaption_ind;

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ASSOC_ACC, 1 );

    /*
     * In case of TCP style interface we send a connection pending notification
     * to the endpoint. In case the endpoint is UDP style then we give a
     * endpt status change notification to the endpoint.
     */
    if( SCTP_SOCK_STREAM == p_assoc->p_ep->sk_type )
    {
        /* Send Connection Pending Indication */
        sctp_ntfy_conn_pending( p_assoc );
    }

    else if( SCTP_SOCK_SEQPACKET == p_assoc->p_ep->sk_type )
    {
        /* send endpoint status change indication */
        sctp_endpoint_chg_data_ut      status_data;
        sctp_sockaddr_st               *addr_list;

        /* status change data */
        status_data.endpoint_up.num_dest_addrs =
            p_assoc->transmit.num_remote_address;
        addr_list = ( sctp_sockaddr_st * ) status_data.endpoint_up.list_of_addrs;

        for( counter = 0; counter < p_assoc->transmit.num_remote_address;
                counter++ )
        {
            addr_list[counter] = p_assoc->transmit.remote[counter].that_address;
        }

        status_data.endpoint_up.num_in_streams =
            p_assoc->receive.num_in_streams;

        status_data.endpoint_up.num_out_streams =
            p_assoc->transmit.num_out_streams;

        /* Send User notification*/
        sctp_ntfy_endpt_status_change( p_assoc, SCTP_COMMUNICATION_UP,
                                       &status_data, E_NOERROR );

        /* notify service user that the Address to which INIT-ACK was
         * sent is now Confirmed. */
        if( confirmed_addr_index <  SCTP_MAX_TRANSPORT_ADDR )
        {
            sctp_ntfy_dst_status_change( p_assoc,
                                         &p_assoc->transmit.remote[confirmed_addr_index].that_address,
                                         SCTP_ADDRESS_CONFIRMED, E_NOERROR );

            sctp_ntfy_dst_status_change( p_assoc,
                                         &p_assoc->transmit.remote[confirmed_addr_index].that_address,
                                         SCTP_ADDRESS_AVAILABLE, E_NOERROR );
        }

        p_assoc->conn_accepted = SCTP_TRUE;

    }

    if( p_assoc->config.adaption_ind != SCTP_NULL_ADAPTION_IND )
    {
        sctp_ntfy_adaption_ind( p_assoc, p_assoc->config.adaption_ind );
    }

    /* For someone waiting for an accept */
    if( p_ep->create_from_server == SCTP_TRUE )
    {
        p_tmp_ep = sctp_db_get_ep_ptr( p_ep->server_ep_id );

        /* SPR : 18527 */
        if( p_tmp_ep != SCTP_NULL )
            /* SPR : 18527 */
        {
            SCTP_WAKEUP( p_tmp_ep->cond );
        }
    }

    else
    {
        SCTP_WAKEUP( p_ep->cond );
    }

    SCTP_UNLOCK_EP( p_ep->ep_id );

    /* Start the Path Verification Probing */
    sctp_send_chunk_hb_path_verf( p_assoc );


    __FREE( p_cookie );
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_cookieack
 *
 *    DESCRIPTION:
 *        This function will locate an association for which cookie-ack
 *        is received, verify the tag and pass the message to the
 *        association state machine. Handling of the cookieack message
 *        in various states will be done by state machine functions.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_cookieack(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U32bit         counter;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( chunk_offset );
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_cookieack" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_COOKIEACK_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        /* Drop  SCTP packet and do not send abort */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookieack: Msg for Invalid Association " ) );

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        return ( SCTP_FAILURE );
    }


    /* check for verification tag */
    if( ( p_assoc->local_tag != p_sctp->v_tag ) && ( p_assoc->local_tag != 0 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cookieack: Verification tag mismatch" ) );

        return( SCTP_FAILURE ); /* Drop message, tags don't match */
    }

    /* Invoke the Association State Machine :Event COOKIE ACK */
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGCOOKIEACK );


    for( counter = 0; counter < p_assoc->transmit.num_remote_address; counter++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( &( p_assoc->transmit.remote[counter].that_address ),
                                           &( p_sctp->p_tl_packet->src_addr ) ) )
        {
            /* Changes For CSR#1-6124313 */
            sctp_U32bit old_status;
            old_status = p_assoc->transmit.remote[counter].status;
            /* Changes For CSR#1-6124313 Ends */

            if( p_assoc->transmit.remote[counter].is_confirmed == SCTP_TRUE )
            {
                p_assoc->transmit.remote[counter].status = SCTP_ADDRESS_AVAILABLE;
            }

            /* Changes For CSR#1-6124313 */
            /* notify service user */
            if( old_status != SCTP_COMMUNICATION_UP )
            {
                sctp_ntfy_dst_status_change( p_assoc,
                                             &( p_assoc->transmit.remote[counter].that_address ),
                                             SCTP_ADDRESS_AVAILABLE,
                                             E_NOERROR );
            }

            /* Changes For CSR#1-6124313 Ends */
            if( counter != p_assoc->transmit.configured_primary_path_index )
            {
                p_assoc->transmit.remote[p_assoc->transmit.configured_primary_path_index].status = SCTP_ADDRESS_UNREACHABLE;
                break;
            }
        }
    }


    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_abort
 *
 *    DESCRIPTION:
 *        This function process an abort chunk which includes parsing
 *        the chunk, validating the verification tag according to tag
 *        rules for abort, filling event data for abort and passing
 *        the event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_abort(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_return_t       ret = SCTP_SUCCESS;
    sctp_error_cause_st error_codes[SCTP_MAX_ERROR_CODES];
    sctp_U32bit         num_errors = 0;
    sctp_U8bit          chunk_flag;
    sctp_error_t        ecode;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_abort" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ABORT_RECV, 1 );

    __MEMSET( ( sctp_U8bit * )&error_codes, 0, SCTP_MAX_ERROR_CODES
              * sizeof( sctp_error_cause_st ) );

    ret = sctp_parse_chk_chunk_abort(
              buffer,
              chunk_offset,
              &num_errors,
              &error_codes[0],
              &ecode );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_abort: Error in Message Parsing" ) );

        return ( SCTP_SUCCESS );
    }

    sctp_db_locate_association(
        p_sctp->src_port,
        &( p_sctp->p_tl_packet->src_addr ),
        p_sctp->dst_port,
        &( p_sctp->p_tl_packet->dst_addr ),
        &p_assoc );

    /* Locate association for this message */
    if( !p_assoc )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_abort: Mesg for Invalid Association " ) );

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        return ( SCTP_FAILURE ); /*  Drop SCTP packet having OOTB abort */
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    /* Check that a request for delete has not been sent for the address */
    if( SCTP_NULL_TIMER_ID != p_assoc->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] )
    {
        sctp_U32bit count;
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_abort: Abort for address being deleted, ignore" ) );

        /* An asconf is in progress, check if there are requests for delete */
        for( count = 0; count < p_assoc->cmd_asconf.num_params; count++ )
        {
            sctp_asconf_delip_param_st *p_delip =
                &( p_assoc->cmd_asconf.asconf_params[count].del_ip );

            if( SCTP_ASCONF_PARAM_DEL_IP ==
                    p_assoc->cmd_asconf.asconf_params[count].param_type )
            {
                /* Check that if ABORT has come for this address */
                if( SCTP_SUCCESS == sctp_cmp_addr( &p_delip->ipaddr,
                                                   &p_sctp->p_tl_packet->dst_addr ) )
                {
                    /*Don't do anything, return success */
                    return ( SCTP_SUCCESS );
                }
            }
        }
    }

#endif
    chunk_flag = sctp_buffer_get_UcharAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_FLAGS );

    /* check for verification tag */
    if( ( ( ( chunk_flag & 0x01 ) == 0 ) && ( p_assoc->local_tag == p_sctp->v_tag ) )
            || ( ( ( chunk_flag & 0x01 ) == 1 ) && ( p_assoc->remote_tag == p_sctp->v_tag ) ) )
    {
        p_assoc->event.msg_abort.p_chunk = buffer + chunk_offset;
        /* inform state machine */

        if( error_codes[0].cause_code == SCTPERR_USER_INITIATED_ABORT )
        {
            /* Store the "User Initiated Abort Reason". */
            p_assoc->event.msg_abort.p_cause = error_codes[0].cause_info.user_initiated_abort.cause;
            p_assoc->event.msg_abort.cause_length = error_codes[0].cause_len;
        }

        else
        {
            p_assoc->event.msg_abort.p_cause      = SCTP_NULL;
            p_assoc->event.msg_abort.cause_length = 0;
        }

        sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGABORT );
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_abort: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_shutdown
 *
 *    DESCRIPTION:
 *        This function process an shutdown chunk which includes
 *        locating the association, validating the verification tag,
 *        filling  event data for shutdown and passing the event to
 *        association  state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_shutdown(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U32bit        cumtsn;
    sctp_chunk_sack_st sack_chunk;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_shutdown" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SHUT_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        /* No assication return shutack */
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_shutdown: No Association, send ShutAck" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );
        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag, SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* drop message */
    }

    /* check for verification tag */
    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_shutdown: Verification tag mismatch" ) );

        return ( SCTP_FAILURE );
    }

    cumtsn = sctp_buffer_get_UlongAtOffset( buffer, chunk_offset + 4 );

    /* fill in event data  */
    sack_chunk.cumtsn = cumtsn;
    sack_chunk.numfrags =  0;
    sack_chunk.num_dupicate_tsn = 0;

    p_assoc->event.msg_sack.sack_chunk  = &sack_chunk;
    p_assoc->event.msg_sack.from_addr   = p_sctp->p_tl_packet->src_addr;
    p_assoc->event.msg_sack.arwnd_valid = 0;

    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGSACK );
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGSHUT );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_shutdownack
 *
 *    DESCRIPTION:
 *        This function process an shutdown ack chunk  which includes
 *        validating the verification tag according to tag rules for
 *        shutdown-ack, and passing the event to association  state machine.
 *
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_shutdownack(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( chunk_offset );
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );

    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_shutdownack" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SHUTACK_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        sctp_addrinfo_st  to;
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_shutdownack: Mesg for Invalid Association " ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE ); /* Discard packet */
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        /* Addresses are unicast , send shutcomplete,
           use tag of receive message */
        to.checksum     = p_sctp->checksum;
        to.p_sockaddr   = &( p_sctp->p_tl_packet->src_addr );
        sctp_send_chunk_shutdowncomplete( SCTP_NULL,
                                          &( p_sctp->p_tl_packet->dst_addr ),
                                          p_sctp->dst_port,
                                          p_sctp->src_port,
                                          &to,
                                          p_sctp->v_tag, 1 ); /* Set T-bit here */

        return ( SCTP_FAILURE );
    }

    /* Fill up event data */
    p_assoc->event.msg_shutack.verification_tag = p_sctp->v_tag;

    /* inform state machine */
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGSHUTACK );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_shutdowncomplete
 *
 *    DESCRIPTION:
 *        This function process an shutdown complete chunk  which includes
 *        validating the verification tag according to tag rules for
 *        shutdown-complete, and passing the event to association  state machine.
 *
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_shutdowncomplete(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U8bit      chunk_flag;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_process_chunk_shutdowncomplete" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SHUTCOM_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        /* In all cases, address unicast, non-unicast drop Whole packet */
        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        return ( SCTP_FAILURE );
    }

    chunk_flag = sctp_buffer_get_UcharAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_FLAGS );

    /* check for verification tag */
    if( ( ( ( chunk_flag & 0x01 ) == 0 ) && ( p_assoc->local_tag == p_sctp->v_tag ) )
            || ( ( ( chunk_flag & 0x01 ) == 1 ) && ( p_assoc->remote_tag == p_sctp->v_tag ) ) )
    {
        /* inform state machine */
        sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGSHUTCOMPL );
    }

    else
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_shutdowncomplete: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_heartbeat
 *
 *    DESCRIPTION:
 *        This function will first locate the association for which
 *        heartbeat is received, verify that tag is valid and send
 *        the heartbeat ack to peer.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_heartbeat(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U16bit   chunk_len;
    sctp_addrinfo_st  to;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/



    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_heartbeat" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_HB_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_heartbeat: Mesg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );
        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_heartbeat: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message,tags don't match*/
    }

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    /*Bug Id 81 fix s*/
    if( chunk_len <  SCTPCHUNKOFFSET_BODY )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid chunk length" ) );
        *p_ecode = E_INVALID_CHUNK_LEN;

        return ( SCTP_FAILURE );
    }

    /*Bug Id 81 fix e*/

    /* send heartbeat Ack */
    to.checksum   = p_sctp->checksum;
    to.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );
    sctp_send_chunk_heartbeatack( p_assoc,
                                  &( p_sctp->p_tl_packet->dst_addr ),
                                  p_sctp->dst_port,
                                  p_sctp->src_port,
                                  &to, p_assoc->remote_tag,
                                  ( sctp_U16bit )( chunk_len  - 4 ),
                                  buffer + chunk_offset + 4 );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_heartbeatack
 *
 *    DESCRIPTION:
 *        This function will parse heartbeat-ack message. Locate the
 *        association for  which heartbeat-ack is received and verify
 *        that tag is valid. Compute Round trip time for this packet
 *        and update retransmission timer values based on new RTT.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_heartbeatack(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_return_t    ret;
    sctp_error_t     ecode;
    sctp_chunk_hb_st hback_chunk;
    sctp_U32bit      addr_index;
    sctp_TL_packet_st   *p_tl_packet;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/



    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_heartbeatack" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_HBACK_RECV, 1 );

    __MEMSET( ( sctp_U8bit * ) &hback_chunk, 0,
              sizeof( sctp_chunk_hb_st ) );

    /*Fix for SPR 8070*/
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_heartbeatack: Mesg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );
        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    /*end fix*/

    ret = sctp_parse_chk_chunk_heartbeatack( buffer, chunk_offset,
                                             p_assoc, &hback_chunk, &ecode );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_heartbeatack: Error in Message Parsing" ) );

        return ( SCTP_SUCCESS );
    }

    /* warning removal starts -- code lines (FIX for SPR 8070) repeated*/
    /* SPR 21107 changes start */
    /* Locate association for this message */
    /* if (p_assoc == SCTP_NULL)
    {
        SCTP_TRACE(SCTP_NULL, SCTP_DBG_TRC,
            ("sctp_process_chunk_heartbeatack: Mesg for Invalid Association"));

        if (sctp_chk_addrs_for_unicast_values(&(p_sctp->p_tl_packet->src_addr),
            &(p_sctp->p_tl_packet->dst_addr)) != SCTP_SUCCESS)
        {
            return (SCTP_FAILURE);
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT(SCTPSTATS_NO_OOTB_RECV, 1);

        sctp_send_chunk_abort(SCTP_NULL,
            &(p_sctp->p_tl_packet->dst_addr),
            p_sctp->dst_port,
            p_sctp->src_port,
            &(p_sctp->p_tl_packet->src_addr),
            p_sctp->v_tag,
            SCTP_FALSE, SCTPERR_PROTOCOL_VIOLATION,
            0, NULL, 1, p_sctp->checksum);

        return (SCTP_FAILURE);
    } */

    /* warning removal ends -- code lines (FIX for SPR 8070) repeated */
    /* SPR 21107 changes end */


    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_heartbeatack: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    /* Set the Event structure in the TCB */
    p_assoc->event.msg_hback.hback_chunk = &hback_chunk;

    p_tl_packet = p_sctp->p_tl_packet;



    for( addr_index = 0; addr_index < p_assoc->p_ep->naddr; addr_index++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( &( p_assoc->p_ep->this_address[addr_index] ), &( p_tl_packet->dst_addr ) ) )
        {
            p_assoc->event.msg_hback.hback_chunk->src_addr_id = addr_index;
            break;
        }
    }

    for( addr_index = 0; addr_index < p_assoc->transmit.num_remote_address; addr_index++ )
    {

        if( SCTP_SUCCESS == sctp_cmp_addr( &( p_assoc->transmit.remote[addr_index].that_address ),
                                           &( p_tl_packet->src_addr ) ) )
        {
            p_assoc->event.msg_hback.hback_chunk->dest_addr_id = addr_index;
            break;
        }
    }



    /* Invoke the State Machine */
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGHBACK );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_sack
 *
 *    DESCRIPTION:
 *        This function will parse sack message. Locate the association
 *        for which sack is received and verify that tag is valid.
 *        Then it will update event data for association and pass this
 *        event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_sack(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_chunk_sack_st sack_chunk;



    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_sack" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SACK_RECV, 1 );

    ret = sctp_parse_chk_chunk_sack(
              buffer,
              chunk_offset,
              &sack_chunk,
              &ecode );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_sack: Error in Message Parsing" ) );

        return ( SCTP_SUCCESS ); /* Drop message */
    }

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_sack: Msg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_sack: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match */
    }

    p_assoc->event.msg_sack.sack_chunk = &sack_chunk;
    p_assoc->event.msg_sack.from_addr = p_sctp->p_tl_packet->src_addr;
    /* SPR 20573 Starts for CSR 1-7088501 */
    p_assoc->event.msg_sack.to_addr = p_sctp->p_tl_packet->dst_addr;
    /* SPR 20573 Ends for CSR 1-7088501 */
    p_assoc->event.msg_sack.arwnd_valid = 1;

    /* inform state machine*/
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGSACK );

    if( p_assoc->event.msg_sack.ootb_sack == 1 )
    {
        /* Out of the blue  data, do not process est of chunks */
        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/**************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_data
 *
 *    DESCRIPTION:
 *        Locate the association for which sack is received and verify
 *        that tag is valid. Copy  parameters (like stream id , ssn,
 *        ppi, data size and pointer to add in the event structure
 *        and pass the event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_data(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U32bit    tmp_stream;
    sctp_U16bit    stream;
    sctp_U16bit    chunk_len;
    sctp_U32bit    tsn_value;
    /* SPR : 18527 */
    sctp_U32bit    addr_index1, old_addr_index1 = 0;
    /* SPR : 18527 */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_data" ) );

    /* SCTP_INCR_GLOBAL_TRAFFICSTAT(SCTPSTATS_NO_DATACHUNKS_RECV, 1); */

    /* Locate association for this message*/
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_data: Mesg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_data: Verification tag mismatch" ) );

        *p_ecode = E_VERIFICATION_TAG_MISMATCH;

        sctp_ntfy_peer_data_drop( p_assoc, p_ecode, p_sctp->p_tl_packet->buffer,
                                  p_sctp->p_tl_packet->packet_len );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    chunk_len = sctp_buffer_get_UshortAtOffset( buffer,
                                                chunk_offset + SCTPCHUNKOFFSET_LENGTH );

    if( chunk_len == SCTP_MIN_DATACHUNK_LEN )
    {
        tsn_value = sctp_buffer_get_UlongAtOffset(
                        buffer, chunk_offset + SCTPCHUNKOFFSET_TSN );

        sctp_send_chunk_abort( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_assoc->remote_tag,
                               SCTP_TRUE,
                               SCTPERR_NOUSERDATA,
                               SCTPERRORLEN_NOUSERDATA - SCTPERROROFFSET_BODY,
                               ( sctp_U8bit * )( &tsn_value ), 0, p_sctp->checksum );

        p_assoc->event.msg_abort.cause_length = 0;

        sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGABORT );

        return ( SCTP_SUCCESS );
    }

    /* fill in event data  */
    stream = sctp_buffer_get_UshortAtOffset( buffer + chunk_offset,
                                             SCTPCHUNKOFFSET_STREAM );

    p_assoc->event.msg_data.from_addr = p_sctp->p_tl_packet->src_addr;
    p_assoc->event.msg_data.CE_bit    = p_sctp->p_tl_packet->CE_bit;
    p_assoc->event.msg_data.last_chunk_flag = last_chunk_flag;
    p_assoc->event.msg_data.chunk_flag = sctp_buffer_get_UcharAtOffset( buffer,
                                         chunk_offset + SCTPCHUNKOFFSET_FLAGS );

    p_assoc->event.msg_data.chunk_len = chunk_len;
    p_assoc->event.msg_data.chunk_data = buffer + chunk_offset;

    /* code changes for correcting SACK problem */
    if( p_assoc->p_ep->naddr > 1 )
    {
        for( addr_index1 = 0; addr_index1 < p_assoc->p_ep->naddr; addr_index1++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr(
                        &( p_assoc->p_ep->this_address[addr_index1] ),
                        &( p_sctp->p_tl_packet->dst_addr ) ) )
            {
                old_addr_index1 = p_assoc->transmit.src_addr_index;
                p_assoc->transmit.src_addr_index = addr_index1;
                break;
            }
        }
    }

    else
    {
        old_addr_index1 = 0;
        p_assoc->transmit.src_addr_index = 0;
    }

    /* inform state machine  */
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGDATA );

    if( p_assoc->event.msg_data.ootb_data == 1 )
    {

        p_assoc->transmit.src_addr_index = old_addr_index1;
        return ( SCTP_FAILURE );
    }

    /* Out of the blue data, do not process est of chunks */

    if( stream >= p_assoc->receive.num_in_streams )
    {
        sctp_addrinfo_st    to;

        to.checksum   = p_sctp->checksum;
        to.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_data: Invalid Stream %d", stream ) );

        tmp_stream = htonl( stream << 16 ) ; /* use only the fisrt 2 bytes */
        sctp_send_chunk_error( p_assoc,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &to,
                               p_assoc->remote_tag,
                               SCTPERR_INVALIDSTREAM,
                               SCTPERRORLEN_INVALIDSTREAM - SCTPERROROFFSET_BODY,
                               ( sctp_U8bit * ) &tmp_stream );

        p_assoc->transmit.src_addr_index = old_addr_index1;

        return ( SCTP_SUCCESS ); /* Send  Error chunk,Invalid stream*/
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_error
 *
 *    DESCRIPTION:This function will parse error essage. Locate the
 *        association for  which error message  is received and verify
 *        that tag is valid.Send user notification for valid error chunk.
 *
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 ***********************************************************************/
sctp_return_t
sctp_process_chunk_error(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_error_cause_st error_codes[SCTP_MAX_ERROR_CODES];
    sctp_U32bit         num_errors = 0;
    sctp_U32bit         counter;
    sctp_return_t       ret;
    sctp_error_t        ecode;
    /* SPR 21127 changes start */
    /* sctp_Pvoid_t        sys_err_info; */
    /* SPR 21127 changes end */


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_error" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ERROR_RECV, 1 );

    __MEMSET( ( sctp_U8bit * ) &error_codes, 0,
              SCTP_MAX_ERROR_CODES * sizeof( sctp_error_cause_st ) );
    ret = sctp_parse_chk_chunk_error(
              buffer,
              chunk_offset,
              &num_errors,
              &error_codes[0],
              &ecode );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_error: Error in Message Parsing" ) );

        return ( SCTP_SUCCESS );
    }

    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_error: Msg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        /* If any of the error code is STALE-COOKIE , discard message */
        for( counter = 0; counter < num_errors; counter++ )
        {
            if( error_codes[counter].cause_code == SCTPERR_STALECOOKIE )
            {
                return ( SCTP_FAILURE );
            }
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_error: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    /* We got a valid error chunk */
    for( counter = 0; counter < num_errors; counter++ )
    {
        /* Send User notification*/
        switch( error_codes[counter].cause_code )
        {
            case SCTPERR_INVALIDSTREAM:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.invalid_stream );
                break;

            case SCTPERR_MISSINGPARAM:
                /* SPR 21127 changes start */
                /* sys_err_info = &error_codes[counter].cause_info.missing_params; */
                /* SPR 21127 changes end */

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.missing_params );
                break;

            case SCTPERR_STALECOOKIE:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.stale_cookie_error );

                p_assoc->event.msg_err_stale_cookie.stale_cookie_time
                    = error_codes[counter].cause_info.stale_cookie_error.
                      time_value;

                sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGSTALECOOKIE );
                break;

            case SCTPERR_NORESOURCE:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len, SCTP_NULL );
                break;

            case SCTPERR_UNRESOLVEADDR:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.unresolve_addr );
                break;

            case SCTPERR_UNRECOCHUNK:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.unrecognize_chunk_type );
#ifdef SCTP_DYNAMIC_IP_SUPPORT
                sctpfsm_assoc_handle_unreco_chunk_err( p_assoc,
                                                       error_codes[counter].cause_info.
                                                       unrecognize_chunk_type.chunk[0] );
#endif
                break;

            case SCTPERR_INVMANDPARAM:
                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len, SCTP_NULL );
                break;

            case SCTPERR_UNRECOPARAMS:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.unrecognize_parameters );
                break;

            case SCTPERR_NOUSERDATA:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len,
                                   &error_codes[counter].cause_info.no_user_data );
                break;

            case SCTPERR_COOKIERECVSHUTDOWN:

                sctp_ntfy_operror( p_assoc, error_codes[counter].cause_code,
                                   error_codes[counter].cause_len, SCTP_NULL );
                break;


            default:
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_chunk_error: Error Code not passed to SU!!,"
                              "ecode =%d", error_codes[counter].cause_code ) );
                break;
        }
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_cwr
 *
 *    DESCRIPTION:
 *        This function will parse cwr message. Locate the association
 *        which cwr is received and verify that tag is valid.
 *        Then it will update event data for association and pass  this
 *        event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_cwr(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/



    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_cwr" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_CWR_RECV, 1 );

    /* Locate association for this message*/
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cwr: Msg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_cwr: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    p_assoc->event.msg_cwr.lowest_tsn = sctp_buffer_get_UlongAtOffset( buffer
                                        + chunk_offset, SCTPCHUNKOFFSET_BODY );

    p_assoc->event.msg_cwr.from_addr = p_sctp->p_tl_packet->src_addr;

    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGCWR );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_ecne
 *
 *    DESCRIPTION:
 *        This function will parse ecne message. Locate the association
 *        for which ecne is received and verify that tag is valid.
 *        Then it will update event data for association and pass  this
 *        event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_ecne(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_ence" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ECNE_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_ecne: Msg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_ecne: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    /* fill in event data  */
    p_assoc->event.msg_ecne.lowest_tsn = sctp_buffer_get_UlongAtOffset( buffer
                                         + chunk_offset, SCTPCHUNKOFFSET_BODY );

    p_assoc->event.msg_ecne.from_addr = p_sctp->p_tl_packet->src_addr;

    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGECNE );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
*    FUNCTION :
*       sctp_process_chunk_for_error
*
*
*    DESCRIPTION:
*       This function will parse unknown chunk. Locate the association
*       from which unknown chunk is received and verify that tag is valid.
*       Then it will take appropriate action for that unknown chunk type
*       depending on the two MSBs of chunk type.
*
*    RETURN VALUE:
*       None
 **************************************************************************/
void
sctp_process_chunk_for_error(
    sctp_packet_st   *p_sctp,
    sctp_U16bit      unrecognise_chunk_len,
    sctp_U8bit       *unrecognise_chunk )
{
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_return_t ret;
    sctp_addrinfo_st   to;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_for_error" ) );


    /* Locate association for this message */
    ret = sctp_db_locate_association(
              p_sctp->src_port,
              &( p_sctp->p_tl_packet->src_addr ),
              p_sctp->dst_port,
              &( p_sctp->p_tl_packet->dst_addr ),
              &p_ass );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_for_error: Msg for Invalid Association" ) );

        return;    /* Drop message */
    }

    if( p_ass->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_for_error: Verification tag mismatch" ) );

        return;    /* Drop message, tags don't match */
    }

    /* There has to be one err code per unrecognised chunk, bundle err codes */

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );
    sctp_send_unrecognise_chunk_error(
        p_ass,
        &( p_sctp->p_tl_packet->dst_addr ),
        p_sctp->dst_port,
        p_sctp->src_port,
        &to,
        p_ass->remote_tag,
        SCTPERR_UNRECOCHUNK,
        unrecognise_chunk_len,
        unrecognise_chunk );

    return;
}



/***************************************************************************
 *    FUNCTION :
 *       sctp_send_error_restart_with_new_addr
 *
 *
 *    DESCRIPTION:
 *       This send an error message to the peer saying that restart attempted
 *    with new addresses in the init. The addresses passed are to be sent in
 *    the info field of the error message.
 *
 *    RETURN VALUE:
 *       None
 **************************************************************************/
void
sctp_send_abort_restart_with_new_addr(
    sctp_packet_st     *p_sctp,
    sctp_U32bit        init_vtag,
    sctp_U32bit        n_addr,
    sctp_sockaddr_st   *p_addrs )
{
    sctp_U8bit  info[SCTP_MAX_TRANSPORT_ADDR * sizeof( sctp_sockaddr_st )];
    sctp_U16bit info_len   = 0;
    sctp_U32bit count      = 0;
    sctp_U8bit  *p_buf     = ( sctp_U8bit * ) info;
    sctp_U8bit  *p_temp_buf = ( sctp_U8bit * ) info;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_error_restart_with_new_addr" ) );

    /* Pack the addresses into the info buffers */
    for( count = 0; count < n_addr; count++ )
    {
        /* Set the address according to the type. */
        if( p_addrs[count].family == SCTP_AF_INET )
        {
            p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
            p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

            /*  byte ordering only where needed */
            p_buf = sctp_buffer_set_Ulong( p_buf, p_addrs[count].ip.v4.addr );
        }

        else
        {
            p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
            p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           p_addrs[count].ip.v6.addr.s_u6_addr32[0] );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           p_addrs[count].ip.v6.addr.s_u6_addr32[1] );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           p_addrs[count].ip.v6.addr.s_u6_addr32[2] );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           p_addrs[count].ip.v6.addr.s_u6_addr32[3] );
        }
    }

    info_len = p_buf - p_temp_buf;

    /* Send the abort chunk */
    sctp_send_chunk_abort( SCTP_NULL,
                           &( p_sctp->p_tl_packet->dst_addr ),
                           p_sctp->dst_port,
                           p_sctp->src_port,
                           &( p_sctp->p_tl_packet->src_addr ),
                           init_vtag,
                           SCTP_TRUE,
                           SCTPERR_RESTARTNEWADDRS,
                           info_len,
                           ( sctp_U8bit * ) info,
                           0, p_sctp->checksum );
}


#ifdef SCTP_DYNAMIC_IP_SUPPORT
/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_asconf
 *
 *    DESCRIPTION:
 *        This function will parse asconf message. Locate the association
 *        for which asconf is received and verify that tag is valid.
 *        Then it will update event data for association and pass  this
 *        event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_asconf(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    sctp_U32bit     ep_id = SCTP_INVALID_ASSOC;
    sctp_U16bit     unrecog_param_len = 0;
    /* klockwork fix 6.2.4 */
    sctp_U8bit      *invalid_parameters = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_asconf" ) );

    invalid_parameters = ( sctp_U8bit * ) __MALLOC( SCTP_MAX_OPT_PARAMS_TOTAL_SIZE );

    if( invalid_parameters == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_asconf : Memory Allocation [%d Bytes] for Cookie Failed, "
                      " Drop ASCONF !!", SCTP_MAX_OPT_PARAMS_TOTAL_SIZE ) );

        return ( SCTP_FAILURE );
    }

    /* klockwork fix 6.2.4 */


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    /*to avoid the warning*/


    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ASCONF_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        sctp_sockaddr_st saddr;
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_asconf: Msg for Invalid Association" ) );

        /*try to locate the assoc using source address parameter*/
        if( SCTP_NULL == sctp_parse_chk_addr_param( buffer + chunk_offset + 8,
                                                    SCTPPARAM_LEN_IPV6ADDRESS, &saddr, p_ecode ) )
        {
            /**/
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "invalid source address parameter" ) );
            /* SPR 20737 starts here */
            __FREE( invalid_parameters );/* klockwork fix 6.2.4 */
            /* SPR 20737 Ends here */
            return SCTP_SUCCESS;
        }

        sctp_db_locate_association( p_sctp->src_port, &saddr, p_sctp->dst_port,
                                    &p_sctp->p_tl_packet->dst_addr, &p_assoc );

        if( SCTP_NULL != p_assoc )
        {
            sctp_U32bit assoc_id = p_assoc->assoc_id;
            ep_id = p_assoc->p_ep->ep_id;

            SCTP_LOCK_EP( ep_id );

            p_assoc = sctp_db_get_association_ptr( ep_id, assoc_id );

            if( SCTP_NULL == p_assoc )
            {
                SCTP_UNLOCK_EP( ep_id );
            }
        }

        if( p_assoc == SCTP_NULL )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "Could not locate valid assoc" ) );

            if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                                   &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
            {
                /* SPR 20737 starts here */
                __FREE( invalid_parameters );/* klockwork fix 6.2.4 */
                /* SPR 20737 Ends here */
                return ( SCTP_FAILURE );
            }

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "unsupported address Sendig abort" ) );

            sctp_send_chunk_abort( SCTP_NULL,
                                   &( p_sctp->p_tl_packet->dst_addr ),
                                   p_sctp->dst_port,
                                   p_sctp->src_port,
                                   &( p_sctp->p_tl_packet->src_addr ),
                                   p_sctp->v_tag,
                                   SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                                   0, NULL, 1, p_sctp->checksum );
            /* SPR 20737 Starts here */
            __FREE( invalid_parameters );/* klockwork fix 6.2.4 */
            /* SPR 20737 Ends here */
            return ( SCTP_FAILURE ); /* Drop message */
        }
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_asconf: Verification tag mismatch" ) );

        if( SCTP_INVALID_ASSOC != ep_id )
        {
            SCTP_UNLOCK_EP( ep_id );
        }

        /* SPR 20737 starts here */
        __FREE( invalid_parameters ); /* klockwork fix 6.2.4 */
        /* SPR 20737 Ends here */
        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    p_assoc->event.msg_addrconf.p_unreco_params  = invalid_parameters;
    p_assoc->event.msg_addrconf.unreco_param_len = 0;

    if( SCTP_SUCCESS != sctp_parse_chk_chunk_asconf( buffer, chunk_offset,
                                                     &( p_assoc->event.msg_addrconf ), p_ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_asconf: error parsing chunk" ) );

        switch( *p_ecode )
        {
            case E_UNKNOWN_PARAMETER_RETURN_ERROR:
                {
                    sctp_addrinfo_st    to;

                    to.checksum   = p_sctp->checksum;
                    to.p_sockaddr = &( p_sctp->p_tl_packet->src_addr );

                    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                                ( "sctp_process_chunk_asconf: Send Error due to parameter "
                                  "type " ) );

                    sctp_send_chunk_error( p_assoc,
                                           &( p_sctp->p_tl_packet->dst_addr ),
                                           p_sctp->dst_port,
                                           p_sctp->src_port,
                                           &to,
                                           p_assoc->remote_tag,
                                           SCTPERR_UNRECOPARAMS,
                                           unrecog_param_len,
                                           &invalid_parameters[0] );
                }
                break;

            default:
                break;
        }

        /* check for possible errors */
        if( SCTP_INVALID_ASSOC != ep_id )
        {
            SCTP_UNLOCK_EP( ep_id );
        }

        /* SPR 20737 Starts here */
        __FREE( invalid_parameters );/* klockwork fix 6.2.4 */
        /* SPR 20737 Ends here */
        return ( SCTP_SUCCESS );
    }

    p_assoc->event.msg_addrconf.src_addr = p_sctp->p_tl_packet->src_addr;
    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGASCONF );

    if( SCTP_INVALID_ASSOC != ep_id )
    {
        SCTP_UNLOCK_EP( ep_id );
    }

    /* SPR 20737 Starts here */
    __FREE( invalid_parameters );/* klockwork fix 6.2.4 */
    /* SPR 20737 Ends here */
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_process_chunk_asconfack
 *
 *    DESCRIPTION:
 *        This function will parse asconf ack message. Locate the association
 *        for which ack is received and verify that tag is valid.
 *        Then it will update event data for association and pass  this
 *        event to association state machine.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_process_chunk_asconfack(
    sctp_packet_st     *p_sctp,
    sctp_U8bit         *buffer,
    sctp_U32bit        chunk_offset,
    sctp_Boolean_t     last_chunk_flag,
    sctp_tcb_st        *p_assoc,
    sctp_error_t       *p_ecode )
{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( last_chunk_flag );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_chunk_ence" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ASCONFACK_RECV, 1 );

    /* Locate association for this message */
    if( p_assoc == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_asconf: Msg for Invalid Association" ) );

        if( sctp_chk_addrs_for_unicast_values( &( p_sctp->p_tl_packet->src_addr ),
                                               &( p_sctp->p_tl_packet->dst_addr ) ) != SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_OOTB_RECV, 1 );

        sctp_send_chunk_abort( SCTP_NULL,
                               &( p_sctp->p_tl_packet->dst_addr ),
                               p_sctp->dst_port,
                               p_sctp->src_port,
                               &( p_sctp->p_tl_packet->src_addr ),
                               p_sctp->v_tag,
                               SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                               0, NULL, 1, p_sctp->checksum );

        return ( SCTP_FAILURE ); /* Drop message */
    }

    if( p_assoc->local_tag != p_sctp->v_tag )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_ecne: Verification tag mismatch" ) );

        return ( SCTP_FAILURE ); /* Drop message, tags don't match*/
    }

    if( SCTP_SUCCESS != sctp_parse_chk_chunk_asconfack( buffer, chunk_offset,
                                                        &( p_assoc->event.msg_addrconfack ), p_ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_chunk_asconf: Verification tag mismatch" ) );

        /* check for possible errors */
        return ( SCTP_FAILURE );
    }

    p_assoc->event.msg_addrconfack.from_addr = p_sctp->p_tl_packet->src_addr;

    sctp_assoc_state_machine( p_assoc, SCTPEVENT_MSGASCONFACK );

    return ( SCTP_SUCCESS );
}


#endif
