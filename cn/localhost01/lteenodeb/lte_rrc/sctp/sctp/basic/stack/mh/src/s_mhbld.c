/*****************************************************************************
 *    FILE NAME:
 *        s_mhbld.c
 *
 *    DESCRIPTION:
 *       This file is part of message Handling module. It includes
 *    message building functions for SCTP chunks to be sent out.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    02Jun00    Sandeep Mahajan     -            .Original Creation
 *    13Oct00    hbhondwe          SPR472         Fix
 *    10Oct00    hbhondwe          IWAVE          Vxworks port.
 *    29Aug01    gsheoran            -            REL 3.0
 *    12july08   Shikha Kathpal                   REL 6.2.0 RFC UPGRADE
 *    29July08   Raza Mohamed                     REL 6.2.0 RFC UPGRADE
 *    25mar,11   Akanksha           SPR 20859     Enhancement for Callback
 *    11Jul,11   Vaibhav Banga                    Klockwork fix for IBM
 *    1May13     Pranav Sharma     SPR 21107      CSR 44034
 *    1May13     Pranav            SPR 21127      CSR 40731-GCC 4.7 warning removal
 *    25Oct13    Pinky Sinha      SPR 21157       CSR 56258
 *    18AUg15    Gurpreet Singh                   REL 6.2.0 CSR 105109 Bug ID 68
 *    21May14    Pinky Sinha      Bug Id 6
 *    17Jul14    Pinky Sinha      Bug Id 13
 *    15Sep14   Priyanka Mishra   Bug Id 22     CSR 85223
*    6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
*     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "MH"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_dbtcb.h>
#include <s_cobuf.h>
#include <s_ostmr.h>
#include <s_es.h>
#include <s_in.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_mhbld.h>
#include <s_mhpars.h>
#include <s_ostmr.h>
#include <s_cocksm.h>
#include <s_dbbuf.h>
#include <s_fsmdat.h>
/* Changes For CSR#1-5037386 */
#include <s_uiutil.h>
#include <s_fsmtx.h>
/* Changes For CSR#1-5037386 Ends */
#include <s_err.h>
#include <s_dbutil.h>

/*Bug ID 68 changes s*/
extern  sctp_Boolean_t  stop_abort;
/*Bug ID 68 changes e*/
sctp_U8bit  *sctp_build_common_header(
    sctp_U8bit      *p_buf,
    sctp_U16bit     srcport,
    sctp_U16bit     dstport,
    sctp_U32bit     verification );

/* Changes For CSR#1-5037386 */
/* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
/***************************************************************************
 *    FUNCTION :
 *        sctp_check_assoc_health_and_reset_assoc_if_reqd
 *
 *    DESCRIPTION: This functions checks th association health and resets
 *          if required
 *
 *    RETURN VALUE:
 *         sctp_void_t
 **************************************************************************/
static sctp_void_t
sctp_check_assoc_health_and_reset_assoc_if_reqd(
    sctp_tcb_st *p_ass,
    sctp_Boolean_t change_flag );

sctp_void_t
sctp_check_assoc_health_and_reset_assoc_if_reqd(
    sctp_tcb_st *p_ass,
    sctp_Boolean_t change_flag )
{
    sctp_U32bit current_txq_tsn = SCTP_INVALID_U32;
    /* Bug Id 6 fix starts */
    /* sctp_U32bit current_cong_level;*/
    sctp_U32bit current_cong_level = 0;
    /* Bug Id 6 fix ends */
    sctpbuffer_st   *p_buf = SCTP_NULL;

    /* Bug Id 13 fix starts */
    /*sctp_calc_trx_cong_reg(p_ass, &current_cong_level);*/
    if( SCTP_SUCCESS == sctp_calc_trx_cong_reg( p_ass, &current_cong_level ) )
    {
        /* Bug Id 13 fix ends */
        if( current_cong_level > 0 )
        {
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

            if( p_buf )
            {
                current_txq_tsn = p_buf->tsn;
            }

            if( ( current_txq_tsn != SCTP_INVALID_U32 ) &&
                    ( current_txq_tsn == p_ass->transmit.last_txq_tsn ) )
            {
                if( change_flag )
                {
                    p_ass->transmit.non_resp_thresh++;
                    SCTP_TRACE( p_ass->p_ep, SCTP_INTF_TRC,
                                ( "sctp_check_assoc_health_and_reset_assoc_if_reqd: Sending HB"
                                  "curr_txq_tsn=%u last_txq_tsn=%u curr_cong_level=%d"
                                  " non_resp_thresh=%d\n",
                                  current_txq_tsn, p_ass->transmit.last_txq_tsn,
                                  current_cong_level, p_ass->transmit.non_resp_thresh ) );

                    if( p_ass->transmit.non_resp_thresh > SCTP_NON_RESP_THRESH )
                    {
                        sctp_U32bit daddr_index =
                            p_ass->transmit.primary_address_index;
                        sctp_U32bit saddr_index =
                            p_ass->transmit.remote[daddr_index].src_addr_index;

                        sctp_send_chunk_abort( p_ass,
                                               &( p_ass->p_ep->this_address[saddr_index] ),
                                               p_ass->p_ep->this_port,
                                               p_ass->transmit.that_port,
                                               &( p_ass->transmit.remote[daddr_index].that_address ),
                                               p_ass->remote_tag,
                                               SCTP_FALSE, 0, 0, NULL, 0, p_ass->config.checksum_func );

                        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

                        sctp_generic_handle_abort_message( p_ass );

                        /* Notify user of endpoint state change */
                        if( change_flag )
                        {
                            sctp_U32bit new_tx_wnd;
                            new_tx_wnd = sctp_db_assoc_tx_window( p_ass );
                            /* SPR 21127 changes start*/
                            SCTP_UNUSED_VARIABLE( new_tx_wnd );
                            /* SPR 21127 changes end*/
                        }

                        return;
                    }
                }

                else
                {
                    SCTP_TRACE( p_ass->p_ep, SCTP_INTF_TRC,
                                ( "sctp_check_assoc_health_and_reset_assoc_if_reqd: HB-Ack recved"
                                  "curr_txq_tsn=%u last_txq_tsn=%u curr_cong_level=%d\n",
                                  current_txq_tsn, p_ass->transmit.last_txq_tsn,
                                  current_cong_level ) );
                }
            }

            else
            {
                p_ass->transmit.non_resp_thresh = 0;
            }
        }

        else
        {
            p_ass->transmit.non_resp_thresh = 0;
        }

        p_ass->transmit.last_txq_tsn = current_txq_tsn;

        if( change_flag )
        {
            sctp_U32bit new_tx_wnd;
            new_tx_wnd = sctp_db_assoc_tx_window( p_ass );
            /* SPR 21127 changes start*/
            SCTP_UNUSED_VARIABLE( new_tx_wnd );
            /* SPR 21127 changes end*/
        }

        return;
        /* Bug Id 13 fix starts */
    }

    else
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_INTF_TRC,
                    ( "sctp_check_assoc_health_and_reset_assoc_if_reqd:error in evaluating transmit congestion region" ) );
        return;
    }

    /* Bug Id 13 fix ends */
}
/* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS*/
/* Changes For CSR#1-5037386 Ends */

/***************************************************************************
 *    FUNCTION :
 *        sctp_build_common_header
 *
 *    DESCRIPTION: Builds common message header
 *
 *    RETURN VALUE:
 *        sctp_U8bit *
 **************************************************************************/
sctp_U8bit *
sctp_build_common_header(
    sctp_U8bit      *p_buf,
    sctp_U16bit     srcport,
    sctp_U16bit     dstport,
    sctp_U32bit     verification )
{
    p_buf = sctp_buffer_set_Ushort( p_buf, srcport );
    p_buf = sctp_buffer_set_Ushort( p_buf, dstport );
    p_buf = sctp_buffer_set_Ulong( p_buf, verification );
    p_buf = sctp_buffer_set_Ulong( p_buf, 0 ); /* Checksum placeholder */

    return p_buf;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_insert_chksum
 *
 *    DESCRIPTION:
 *        Inserts adler32 checksum
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_insert_chksum(
    sctp_U8bit         *thisposition,
    sctp_U8bit         *origin,
    sctp_checksum_et   checksum )
{
    sctp_U32bit    computed;

    sctp_buffer_set_UlongAtOffset( origin, SCTP_OFFSET_CKSUM, 0 );

    /* Zero Checksum */
    computed = sctp_compute_cksm[checksum]( origin, ( thisposition - origin ) );

    /* Insert Checksum */
    sctp_buffer_set_UlongAtOffset( origin, SCTP_OFFSET_CKSUM, computed );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_init
 *
 *    DESCRIPTION:
 *        Send INIT chunk. The family parameter determines whether the INIT
 *    to be sent can contain the supported address type as IPV6.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_init(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      init_tag,
    sctp_U32bit      init_tsn,
    sctp_U16bit      outstreams,
    sctp_U16bit      instreams,
    sctp_U32bit      rwnd,
    sctp_U32bit      num_addrs,
    sctp_sockaddr_st *addr_list,
    sctp_Boolean_t   cookie_preservative_flag,
    sctp_U32bit      cookie_preservative_time,
    sctp_suppaddr_st *p_supp_family,
    sctp_U8bit       *p_hostname,
    sctp_U32bit      adaption_ind )
{
    sctp_U32bit  i       = 0;
    sctp_U8bit   *buffer;
    sctp_U8bit   *p_buf  = SCTP_NULL, *p_param_start;
    sctp_U8bit   *p_chunk_start = SCTP_NULL;
    sctp_U8bit   *sctp_offset   = SCTP_NULL;
    sctp_Boolean_t nat_capable = *( ( sctp_Boolean_t * )p_to->p_info );
    sctp_Boolean_t  include_ip = !nat_capable;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_init" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;
    p_buf = sctp_build_common_header( p_buf, srcport, dstport, 0 );

    /* Add the INIT chunk */
    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_INIT ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );            /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    p_buf = sctp_buffer_set_Ulong( p_buf, init_tag );  /* Initiate Tag */
    p_buf = sctp_buffer_set_Ulong( p_buf, rwnd );      /* A-RWND */
    p_buf = sctp_buffer_set_Ushort( p_buf, outstreams ); /* Out-Streams */
    p_buf = sctp_buffer_set_Ushort( p_buf, instreams ); /* In-Streams */
    p_buf = sctp_buffer_set_Ulong( p_buf, init_tsn );  /* Initial TSN */


    if( nat_capable && ( 1 < num_addrs ) )
    {
        /*send hostname only if naddr > 1*/
        if( ( p_hostname == SCTP_NULL ) || ( p_hostname[0] == '\0' ) )
        {
            p_hostname = sctp_db_main.hostname;
        }

        if( p_hostname[0] != '\0' )
        {
            /* Send Hostname */
            p_param_start = p_buf;

            p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_HOSTNAME );
            p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /*Length PlaceHolder*/

            while( ( *( p_buf++ ) = *( p_hostname++ ) ) ); /*doing a strcpy*/

            sctp_buffer_set_UshortAtOffset( p_param_start, SCTPPARAMOFFSET_LENGTH,
                                            ( sctp_U16bit )( p_buf - p_param_start ) );
            p_buf = sctp_buffer_PadTo( p_buf, p_param_start, 4 );
        }

        else
        {
            include_ip = SCTP_TRUE;
        }
    }

    if( include_ip )
    {
        /* start spr 21157 */
        for( i = 0; i < num_addrs && i < SCTP_MAX_TRANSPORT_ADDR; i++ )
            /* end spr 21157 */
        {
            /* Set the address according to the type. */
            if( addr_list[i].family == SCTP_AF_INET )
            {
                p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
                p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

                /*  byte ordering only where needed */
                p_buf = sctp_buffer_set_Ulong( p_buf, addr_list[i].ip.v4.addr );
            }

            else
            {
                p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
                p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
                p_buf = sctp_buffer_set_Ulong( p_buf,
                                               addr_list[i].ip.v6.addr.s_u6_addr32[0] );
                p_buf = sctp_buffer_set_Ulong( p_buf,
                                               addr_list[i].ip.v6.addr.s_u6_addr32[1] );
                p_buf = sctp_buffer_set_Ulong( p_buf,
                                               addr_list[i].ip.v6.addr.s_u6_addr32[2] );
                p_buf = sctp_buffer_set_Ulong( p_buf,
                                               addr_list[i].ip.v6.addr.s_u6_addr32[3] );
            }
        }
    }

    if( SCTP_IS_ECN_CAPABLE )
    {
        /* send ecn capable optional parameter */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_ECNCAPABLE );
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPPARAM_LEN_ECNCAPABLE );
    }

    if( cookie_preservative_flag == SCTP_TRUE )
    {
        /* send ecn capable optional parameter */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_COOKIEPRESV );
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPPARAM_LEN_COOKIEPRESV );
        p_buf = sctp_buffer_set_Ulong( p_buf, cookie_preservative_time );
    }

    /*
     * If the stack is IPV6 type then we need to set the supported address
     * type to IPV6 also.
     */
    p_param_start = p_buf;
    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_SUPADDRTYPE );
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /*length placeholder*/

    if( p_supp_family->v4 )
    {
        /* Support Ipv4 */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
    }

    if( p_supp_family->v6 )
    {
        /* Support Ipv4 */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
    }

    if( p_supp_family->hname )
    {
        /* Support Hostnames*/
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_HOSTNAME );
    }

    sctp_buffer_set_UshortAtOffset( p_param_start, SCTPPARAMOFFSET_LENGTH,
                                    ( sctp_U16bit )( p_buf - p_param_start ) );
    /*SPR 5220*/
    p_buf = sctp_buffer_PadTo( p_buf, p_param_start, 4 ); /*padding*/
    /*SPR 5220*/

    /*Check If adaption Indication param needs to be passed*/
    if( SCTP_NULL_ADAPTION_IND != adaption_ind )
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_ADAPTIONIND );
        p_buf = sctp_buffer_set_Ushort( p_buf, 8 );
        /*SPR 5045*/
        p_buf = sctp_buffer_set_Ulong( p_buf, adaption_ind );
        /*SPR 5045*/
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    /***********************************************************************/
    /*Always send the pref_primary address in INIT chunk
     *Consider the first address in always the primary address*/

    /*If the first address in the IPV4 address*/

    if( addr_list[0].family == SCTP_AF_INET )
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_ASCONF_PARAM_SET_PRIM );
        p_buf = sctp_buffer_set_Ushort( p_buf, 16 );  /* Length */

        p_buf = sctp_buffer_set_Ulong( p_buf, 0 );  /*Correlation ID*/
        p_buf = sctp_buffer_set_Ushort( p_buf,
                                        SCTPVARPAR_IPV4ADDRESS );  /*Type*/
        p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

        /*  byte ordering only where needed */
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       addr_list[0].ip.v4.addr ); /*IP Address*/
    }

    else
    {
        /*Primary address in the IPV6 address*/
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_ASCONF_PARAM_SET_PRIM );
        p_buf = sctp_buffer_set_Ushort( p_buf, 28 ); /*Length*/

        p_buf = sctp_buffer_set_Ulong( p_buf, 0 ); /*Correlation ID*/

        p_buf = sctp_buffer_set_Ushort( p_buf,
                                        SCTPVARPAR_IPV6ADDRESS ); /*Type*/

        p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       addr_list[0].ip.v6.addr.s_u6_addr32[0] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       addr_list[0].ip.v6.addr.s_u6_addr32[1] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       addr_list[0].ip.v6.addr.s_u6_addr32[2] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       addr_list[0].ip.v6.addr.s_u6_addr32[3] );
    }

    /************************************************************************/
#endif
    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );  /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_init:Chunk INIT Sent, Src Port=%d, DestPort=%d,"
                  " VerTag=%u, To addr = ", srcport, dstport, 0 ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_INIT_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/
    return;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_initack
 *
 *    DESCRIPTION:
 *        Send INITACK chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_initack(
    sctp_tcb_st           *p_ass,
    sctp_sockaddr_st      *p_from,
    sctp_U16bit           srcport,
    sctp_U16bit           dstport,
    sctp_addrinfo_st      *p_to,
    sctp_U32bit           tag,
    sctp_chunk_cookie_st  *cookie,
    sctp_U16bit            cookie_len,
    sctp_Boolean_t        unreco_param_present,
    sctp_U16bit           total_len,
    sctp_U8bit            *invalid_parameters,
    sctp_U8bit            *p_hostname,
    sctp_U32bit           adaption_ind )
{
    sctp_U32bit    counter;
    sctp_U8bit     *buffer;
    sctp_U8bit     *p_chunk_start;
    sctp_U8bit     *S;
    sctp_U8bit     *p_buf;
    sctp_U8bit     *sctp_offset;
    sctp_Boolean_t nat_capable = *( ( sctp_Boolean_t * )p_to->p_info );
    sctp_Boolean_t  include_ip = !( nat_capable );
    sctpmtubuf_st *p_mtubuf;
    sctp_ep_st    *p_ep   = SCTP_NULL; /* SPR 20859*/
    /* For cookie_len warning removal */
    SCTP_UNUSED_VARIABLE( cookie_len );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_initack" ) );

    if( SCTP_NULL == p_ass )
    {
        p_mtubuf = sctp_db_get_mtu_buffer();

        if( !p_mtubuf )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                        SCTPERROR_LEVEL_CRITICAL, E_NORESOURCES,
                        "At senddata_on_alternate_addr:"
                        " could not get MTU buffer " );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

            return;
        }
    }

    else
    {
        p_mtubuf = &( p_ass->mtu_buffer );
    }

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    /* compose the INIT-ACK message */
    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    /* Add the INIT-ACK chunk */
    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_INITACK );  /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );   /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    /* Initiate Tag */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, cookie->local_init_parameters.fixed_params.init_tag );

    /* A-RWND */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, cookie->local_init_parameters.fixed_params.a_rwnd );

    /* Number of OUT streams */
    p_buf = sctp_buffer_set_Ushort(
                p_buf, cookie->local_init_parameters.fixed_params.num_out_streams );

    /* In-Streams */
    p_buf = sctp_buffer_set_Ushort(
                p_buf, cookie->local_init_parameters.fixed_params.num_in_streams );

    /* Initial TSN */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, cookie->local_init_parameters.fixed_params.init_tsn );

    S = p_buf;

    /* Parameter Type */
    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_COOKIE );

    /* Length - Placeholder */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );

    /* SPR 20568 Starts for CSR 1-6658486 */

    p_buf = sctp_buffer_set_cookie( p_buf, cookie );

    p_buf = sctp_buffer_set_Buffer(
                p_buf, cookie->unexpected_init_params_len, ( sctp_U8bit * )( cookie + 1 ) );

    /* SPR 20568 Ends for CSR 1-6658486 */

    sctp_buffer_set_UshortAtOffset(
        S, SCTPPARAMOFFSET_LENGTH, ( sctp_U16bit )( p_buf - S ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad the Cookie! */

    if( ( nat_capable ) &&
            ( 1 < cookie->local_init_parameters.num_addrs ) )
    {
        if( ( p_hostname == NULL ) || ( p_hostname[0] == '\0' ) )
        {
            p_hostname = sctp_db_main.hostname;
        }

        if( p_hostname[0] != '\0' )
        {
            /* Send Hostname */
            sctp_U8bit *p_start = p_buf;

            p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_HOSTNAME );
            p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /*Length PlaceHolder*/

            while( ( *( p_buf++ ) = *( p_hostname++ ) ) ); /*strcpy*/

            sctp_buffer_set_UshortAtOffset( p_start, SCTPPARAMOFFSET_LENGTH,
                                            ( sctp_U16bit )( p_buf - p_start ) );
            p_buf = sctp_buffer_PadTo( p_buf, p_start, 4 );
        }

        else
        {
            include_ip = SCTP_TRUE;
        }
    }

    for( counter = 0; counter < cookie->local_init_parameters.num_addrs;
            counter++ )
    {
        if( !include_ip )
        {
            break;
        }

        /* Set the address according to the type. */
        if( cookie->local_init_parameters.addr_list[counter].family
                == SCTP_AF_INET )
        {
            p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
            p_buf = sctp_buffer_set_Ushort( p_buf, 8 );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           cookie->local_init_parameters.addr_list[counter].ip.v4.addr );
        }

        else
        {
            p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
            p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           cookie->local_init_parameters.addr_list[counter].
                                           ip.v6.addr.s_u6_addr32[0] );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           cookie->local_init_parameters.addr_list[counter].
                                           ip.v6.addr.s_u6_addr32[1] );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           cookie->local_init_parameters.addr_list[counter].
                                           ip.v6.addr.s_u6_addr32[2] );
            p_buf = sctp_buffer_set_Ulong( p_buf,
                                           cookie->local_init_parameters.addr_list[counter].
                                           ip.v6.addr.s_u6_addr32[3] );
        }
    }

    if( SCTP_IS_ECN_CAPABLE )
    {
        /* send ecn capable optional parameter */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_ECNCAPABLE );
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPPARAM_LEN_ECNCAPABLE );
    }

    /*Check If adaption Indication param needs to be passed*/
    if( SCTP_NULL_ADAPTION_IND != adaption_ind )
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_ADAPTIONIND );
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPPARAM_LEN_ADAPTIONIND );
        /*SPR 5045*/
        p_buf = sctp_buffer_set_Ulong( p_buf, adaption_ind );
        /*SPR 5045*/
    }

    if( unreco_param_present == SCTP_TRUE )
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_UNKNOWNPAR );

        /* Unknown parameter type */
        p_buf = sctp_buffer_set_Ushort( p_buf, ( sctp_U16bit )( total_len    + 4 ) );

        /* length of the tlv parameter */
        p_buf = sctp_buffer_set_Buffer( p_buf, total_len, invalid_parameters );

        /* Unknown parameters */
        p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    /***********************************************************************/
    /*Always send the pref_primary address in INIT-ACK chunk
     *Consider the first address is always the primary address*/

    /*If the first address in the IPV4 address*/

    if( cookie->local_init_parameters.addr_list[0].family
            == SCTP_AF_INET )
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_ASCONF_PARAM_SET_PRIM );
        p_buf = sctp_buffer_set_Ushort( p_buf, 16 );  /* Length */

        p_buf = sctp_buffer_set_Ulong( p_buf, 0 );  /*Correlation ID*/
        p_buf = sctp_buffer_set_Ushort( p_buf,
                                        SCTPVARPAR_IPV4ADDRESS );  /*Type*/
        p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

        /*  byte ordering only where needed */
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       cookie->local_init_parameters.addr_list[0].ip.v4.addr );
    }

    else
    {
        /*Primary address in the IPV6 address*/
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_ASCONF_PARAM_SET_PRIM );
        p_buf = sctp_buffer_set_Ushort( p_buf, 28 ); /*Length*/

        p_buf = sctp_buffer_set_Ulong( p_buf, 0 ); /*Correlation ID*/

        p_buf = sctp_buffer_set_Ushort( p_buf,
                                        SCTPVARPAR_IPV6ADDRESS ); /*Type*/

        p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */

        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       cookie->local_init_parameters.addr_list[0].
                                       ip.v6.addr.s_u6_addr32[0] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       cookie->local_init_parameters.addr_list[0].
                                       ip.v6.addr.s_u6_addr32[1] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       cookie->local_init_parameters.addr_list[0].
                                       ip.v6.addr.s_u6_addr32[2] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       cookie->local_init_parameters.addr_list[0].
                                       ip.v6.addr.s_u6_addr32[3] );

    }

    /************************************************************************/
#endif

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_initack:Chunk INITACK Sent,Src Port=%d,"
                  "Dest Port=%d, VerTag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_INITACK_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else if( ( SCTP_SUCCESS == sctp_db_locate_server( srcport, p_from, &p_ep ) ) && ( p_ep->send_func != SCTP_NULL ) )
    {
        p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */
    if( SCTP_NULL == p_ass )
    {
        /*
         * Free the buffer only if it was taken from the global MTU buffer.
         */
        sctp_db_free_mtu_buffer( p_mtubuf );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_cookie
 *
 *    DESCRIPTION:
 *
 *        Send COOKIE chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_cookie(
    sctp_tcb_st       *p_ass,
    sctp_sockaddr_st  *p_from,
    sctp_U16bit       srcport,
    sctp_U16bit       dstport,
    sctp_addrinfo_st  *p_to,
    sctp_U32bit       tag,
    LIST              *p_cookie_list,
    sctp_U16bit       cookiesize )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset = SCTP_NULL;
    sctp_U8bit *p_cookie = SCTP_NULL;
    sctpbuffer_st *p_buffer = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );
    sctp_U16bit df_bit = 0;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( cookiesize );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_cookie" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    /* Add the COOKIE chunk */
    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_COOKIE );  /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length- Placeholder */

    p_buffer = ( sctpbuffer_st * )lstFirst( p_cookie_list );

    while( p_buffer )
    {
        p_cookie = p_buffer->buffer;
        p_buf = sctp_buffer_set_Buffer( p_buf, p_buffer->size, p_cookie );
        p_buffer = ( sctpbuffer_st * )lstNext( ( NODE * )p_buffer );
    }

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );   /* Pad the chunk */

    /*Bundle data chunks and set df_bit if the data is new i.e not retransmitted*/
    p_buf = sctp_add_data_chunks( ( sctp_tcb_st * )( p_to->p_info ), p_buf,
                                  SCTP_DEFAULT_MTUSIZE - ( p_buf - buffer ), &df_bit );

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_cookie:Chunk COOKIE Sent,Src Port=%d,Dest Port=%d,\
        Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_COOKIE_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), df_bit, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), df_bit, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_cookie_err_chunk_bundled
 *
 *    DESCRIPTION:
 *        Send COOKIE chunk with error chunk bundled with it.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_cookie_err_chunk_bundled(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    LIST             *p_cookie_list,
    sctp_U16bit      cookiesize,
    sctp_U16bit      cause_code,
    sctp_U16bit      cause_len,
    sctp_U8bit       *cause_info )
{
    sctp_U8bit *p_buf;
    sctp_U8bit *p_chunk_start;
    sctp_U8bit *sctp_offset;
    sctp_U8bit *p_offset_ecode;
    sctp_U8bit *buffer;
    sctp_U8bit *p_cookie = SCTP_NULL;
    sctpbuffer_st *p_buffer = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf;
    sctp_U16bit df_bit = 0;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( cookiesize );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_cookie_err_chunk_bundled" ) );

    if( SCTP_NULL == p_ass )
    {
        p_mtubuf = sctp_db_get_mtu_buffer();

        if( !p_mtubuf )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                        SCTPERROR_LEVEL_CRITICAL, E_NORESOURCES,
                        "At senddata_on_alternate_addr:"
                        " could not get MTU buffer " );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

            return;
        }
    }

    else
    {
        p_mtubuf = &( p_ass->mtu_buffer );
    }

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    /* Add the COOKIE chunk */

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_COOKIE ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );     /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );    /* Length - Placeholder */

    /* p_buf = sctp_buffer_set_Buffer(p_buf, cookiesize, cookie); */
    p_buffer = ( sctpbuffer_st * )lstFirst( p_cookie_list );

    while( p_buffer )
    {
        p_cookie = p_buffer->buffer;
        p_buf = sctp_buffer_set_Buffer( p_buf, p_buffer->size, p_cookie );
        p_buffer = ( sctpbuffer_st * )lstNext( ( NODE * )p_buffer );
    }


    sctp_buffer_set_UshortAtOffset( p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
                                    ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );   /* Pad the chunk */

    p_chunk_start = p_buf; /* Note down start of error chunk */

    /* Now putting here error chunk so as to bundle it with cookie */
    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_ERROR ); /** Type **/
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );         /** Flags **/
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );   /** Length - Placeholder**/

    p_offset_ecode = p_buf;
    p_buf = sctp_buffer_set_Ushort( p_buf, cause_code ); /**Code - Placeholder **/
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /** Length - Placeholder*/
    p_buf = sctp_buffer_set_Buffer( p_buf, cause_len, cause_info ); /*err cause*/

    sctp_buffer_set_UshortAtOffset(
        p_offset_ecode, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_offset_ecode ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /** Pad this causecode **/

    /* Set length of error chunk */
    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    /*Bundle data chunks */
    p_buf = sctp_add_data_chunks( ( sctp_tcb_st * )( p_to->p_info ), p_buf,
                                  SCTP_DEFAULT_MTUSIZE - ( p_buf - buffer ), &df_bit );

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_cookie_err_chunk_bundled:Chunk COOKIE Sent with err chunk "
                  "bundled, Src Port=%d,Dest Port=%d, Ver Tag=%u,To Addr:",
                  srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_COOKIE_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );


    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), df_bit, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), df_bit, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */

    if( SCTP_NULL == p_ass )
    {
        /*
         * Free the buffer only if it was taken from the global MTU buffer.
         */
        sctp_db_free_mtu_buffer( p_mtubuf );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_cookieack
 *
 *    DESCRIPTION:
 *        Send COOKIEACK chunk
 *
 *    RETURN VALUE:
 *        None
**************************************************************************/
void
sctp_send_chunk_cookieack(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );
    sctp_U8bit *p_offset_ecode = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_cookieack" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    /* Add the COOKIE-ACK chunk */
    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_COOKIEACK ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );      /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );     /* Length - Placeholder */

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start , SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );  /* Pad the chunk */


    /* Bundle the Error Chunk containing the 'Unexpected Parameters'
     * received in INIT Chunk. */
    /* klocwork fix for IBM */
    if( p_ass == SCTP_NULL )
    {
        return;
    }

    /* klocwork fix for IBM */

    if( p_ass->unrecognized_params.length != 0 )
    {
        /* Add the ERROR chunk */
        p_chunk_start = p_buf;

        p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_ERROR ); /* Type */
        p_buf = sctp_buffer_set_Uchar( p_buf, 0 );      /* Flags */
        p_buf = sctp_buffer_set_Ushort( p_buf, 0 );     /* Length - Placeholder */

        p_offset_ecode = p_buf;
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPERR_UNRECOPARAMS ); /* Cause Code - Placeholder */
        p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

        if( p_ass->unrecognized_params.param_list != SCTP_NULL )
        {
            p_buf = sctp_buffer_set_Buffer( p_buf,
                                            p_ass->unrecognized_params.length,
                                            p_ass->unrecognized_params.param_list ); /* err cause */
        }

        /* Fill the Cause-Code Length */
        sctp_buffer_set_UshortAtOffset( p_offset_ecode, SCTPCHUNKOFFSET_LENGTH,
                                        ( sctp_U16bit )( p_buf - p_offset_ecode ) );

        p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad this cause code */

        /* Fill the ERROR Chunk Length */
        sctp_buffer_set_UshortAtOffset(
            p_chunk_start , SCTPCHUNKOFFSET_LENGTH,
            ( sctp_U16bit )( p_buf - p_chunk_start ) );

        p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );  /* Pad the chunk */
    }

    /* Insert the checksum */
    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_cookieack:Chunk COOKIEACK Sent,Src Port=%d,"
                  "Dest Port=%d, Tag=%u,To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_COOKIEACK_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {

        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_abort
 *
 *    DESCRIPTION:
 *        Send ABORT chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_abort(
    sctp_tcb_st       *p_ass,
    sctp_sockaddr_st  *p_from,
    sctp_U16bit       srcport,
    sctp_U16bit       dstport,
    sctp_sockaddr_st  *p_to,
    sctp_U32bit       tag,
    sctp_Boolean_t    include_ecode,
    sctp_U16bit       cause_code,
    sctp_U16bit       cause_len,
    sctp_U8bit        *cause_info,
    sctp_U8bit        chunk_flag,
    sctp_checksum_et  checksum )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start  = SCTP_NULL;
    sctp_U8bit *p_offset_ecode = SCTP_NULL;
    sctp_U8bit *sctp_offset    = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf;
    sctp_ep_st    *p_ep   = SCTP_NULL; /* SPR 20859 */


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_abort" ) );

    /*Bug ID 68 changes s*/
    if( stop_abort == SCTP_TRUE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "No abort to be sent in this case "
                                               "as user requested blocking of the Abort" ) );
        return;
    }

    /*Bug ID 68 changes e*/
    if( SCTP_NULL == p_ass )
    {
        p_mtubuf = sctp_db_get_mtu_buffer();

        if( !p_mtubuf )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                        SCTPERROR_LEVEL_CRITICAL, E_NORESOURCES,
                        "At senddata_on_alternate_addr:"
                        " could not get MTU buffer " );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

            return;
        }
    }

    else
    {
        p_mtubuf = &( p_ass->mtu_buffer );
        sctp_get_confirmed_address( p_ass, p_to, p_from );
    }

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    /* Add the COOKIE chunk */
    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_ABORT ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, chunk_flag ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    if( include_ecode )
    {
        p_offset_ecode = p_buf;
        p_buf = sctp_buffer_set_Ushort( p_buf, cause_code ); /* Len Placeholder */
        p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Len - Placeholder */

        /* Changes for CSR#1-4776901 *
         * Check whether length of err_info field can be accomodated with in
         * MTU size keeping space required for other feilds in view */
        if( cause_len > ( ( sctp_U16bit )( SCTP_MAX_MTUSIZE - ( sctp_U16bit )( p_buf - buffer ) ) ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_send_chunk_abort: ABORT cause has Lenght > MAX_MTU" ) );
            cause_len = ( ( sctp_U16bit )( SCTP_MAX_MTUSIZE - ( sctp_U16bit )( p_buf - buffer ) ) );
        }

        /* Changes for CSR#1-4776901 Ends */

        /* err cause */
        if( cause_len != 0 )
        {
            /* SPR : 18527 */
            if( cause_info != SCTP_NULL )
            {
                p_buf = sctp_buffer_set_Buffer( p_buf, cause_len, cause_info );
            }

            else
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "ERROR :: Sending ABORT Chunk with cause_len = %d and "
                              "cause_info = SCTP_NULL", cause_len ) );
            }

            /* SPR : 18527 */
        }

        sctp_buffer_set_UshortAtOffset(
            p_offset_ecode, SCTPCHUNKOFFSET_LENGTH,
            ( sctp_U16bit )( p_buf - p_offset_ecode ) );

        p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /*Pad this cause code*/
    }

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start , SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );  /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_abort: Chunk ABORT Sent,"
                  "Src Port=%d,Dest Port=%d,Ver Tag=%u, To Addr:",
                  srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ABORT_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to, p_from, SCTP_FALSE );
    }

    else if( ( SCTP_SUCCESS == sctp_db_locate_server( srcport, p_from, &p_ep ) ) && ( p_ep->send_func != SCTP_NULL ) )
    {
        p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */

    if( SCTP_NULL == p_ass )
    {
        /*
         * Free the buffer only if it was taken from the global MTU buffer.
         */
        sctp_db_free_mtu_buffer( p_mtubuf );
    }
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_get_confirmed_address
 *
 *    DESCRIPTION:
 *        Get a Address which is confirmed.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
sctp_void_t sctp_get_confirmed_address(
    sctp_tcb_st *p_ass,
    sctp_sockaddr_st *p_to,
    sctp_sockaddr_st *p_from )
{
    sctp_U32bit addr_index = 0;
    sctp_U32bit daddr_index = 0;
    sctp_U32bit saddr_index = 0;
    /*Bug Id 22 fix starts*/
    /*sctp_error_t s_err =0;*/
    sctp_error_t s_err = E_NOERROR;
    /*Bug Id 22 fix ends*/
    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_from );
    /* SPR 21107 changes end */

    if( p_ass == SCTP_NULL || p_to == SCTP_NULL )
    {
        return ;
    }

    for( ; addr_index < p_ass->transmit.num_remote_address; addr_index++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr( p_to, &( p_ass->transmit.remote[addr_index].that_address ) ) &&
                p_ass->transmit.remote[addr_index].is_confirmed == SCTP_TRUE )
        {
            /* "p_to" Address is confirmed */
            return;
        }
    }

    /* "p_to" is Unconfirmed, select the current primary path index and return it. */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_get_confirmed_address : Could not get compatible address index for src and dst" ) );

        return;
    }

    p_to = &( p_ass->transmit.remote[daddr_index].that_address );
    p_from = &( p_ass->p_ep->this_address[saddr_index] );
    return;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_shutdowncomplete
 *
 *    DESCRIPTION:
 *        Send chunk SHUTDOWN COMPLETE
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_shutdowncomplete(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U8bit       chunk_flag )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf;
    sctp_ep_st    *p_ep   = SCTP_NULL; /* SPR 20859 */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_shutdowncomplete" ) );

    if( SCTP_NULL == p_ass )
    {
        p_mtubuf = sctp_db_get_mtu_buffer();

        if( !p_mtubuf )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                        SCTPERROR_LEVEL_CRITICAL, E_NORESOURCES,
                        "At senddata_on_alternate_addr:"
                        " could not get MTU buffer " );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

            return;
        }
    }

    else
    {
        p_mtubuf = &( p_ass->mtu_buffer );
    }

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_SHUTDOWNCOMP ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, chunk_flag );  /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_shutdowncomplete:Chunk SHUTCOMPLETE Sent,"
                  "SrcPort=%d,Dest Port=%d, Ver Tag=%u, To Addr:", srcport,
                  dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );


    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SHUTCOM_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else if( ( SCTP_SUCCESS == sctp_db_locate_server( srcport, p_from, &p_ep ) ) && ( p_ep->send_func != SCTP_NULL ) )
    {
        p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {

        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/
    if( SCTP_NULL == p_ass )
    {
        /*
         * Free the buffer only if it was taken from the global MTU buffer.
         */
        sctp_db_free_mtu_buffer( p_mtubuf );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_shutdownack
 *
 *    DESCRIPTION:
 *        Send    chunk SHUTDOWN ACK
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_shutdownack(
    sctp_tcb_st       *p_ass,
    sctp_sockaddr_st  *p_from,
    sctp_U16bit       srcport,
    sctp_U16bit       dstport,
    sctp_addrinfo_st  *p_to,
    sctp_U32bit       tag )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_SHUTDOWNACK ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_shutdownack:Chunk SHUTACK Sent,"
                  "Src Port=%d,Dest Port=%d, Ver Tag=%u,To Addr:",
                  srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SHUTACK_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_shutdown
 *
 *    DESCRIPTION:
 *        Send    chunk SHUTDOWN
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_shutdown(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U32bit      cumtsn )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_shutdown" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;
    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_SHUTDOWN ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );    /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );    /* Length - Placeholder */
    p_buf = sctp_buffer_set_Ulong( p_buf, cumtsn );

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );     /* Pad the chunk */

    /* Insert the checksum */
    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_shutdown:Chunk SHUTDOWN Sent,Src Port=%d,"
                  "Dest Port=%d, Tag=%u, To Addr:" , srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SHUT_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_addr_index_for_hb
 *
 *    DESCRIPTION:
 *        Get a Unconfirmed Address to which HeartBeat
 *        can be sent for Path Verification.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t
sctp_get_addr_index_for_hb(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    *addr_index )
{
    sctp_U32bit    counter;
    sctp_time_t    curr_time;
    sctp_U32bit    tmp_time_diff;
    sctp_U32bit    max_time_diff = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_get_addr_index_for_hb" ) );

    __GETSYSTIME( &curr_time );

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        /* If any of the following condition is TRUE then Ignore this Address
           1. Heartbeat Timer for Path Verification is already running.
           2. Address is already confirmed.
           3. Address should not be considered for Path Verification using HB.MAX.BURST
           4. Address has been just Used. */
        if( p_ass->transmit.remote[counter].dest_timer_id[SCTP_PATH_VERF_TIMER] != SCTP_NULL_TIMER_ID ||
                p_ass->transmit.remote[counter].is_confirmed == SCTP_TRUE ||
                p_ass->transmit.remote[counter].perf_path_verf == SCTP_FALSE ||
                ( sctp_calculate_time_difference( &curr_time,
                                                  &p_ass->transmit.remote[counter].last_used_time,
                                                  &tmp_time_diff ) != SCTP_SUCCESS ) )
        {
            continue;
        }

        if( max_time_diff == 0 )
        {
            max_time_diff = tmp_time_diff;
            *addr_index = counter;
        }

        else
        {
            if( tmp_time_diff > max_time_diff )
            {
                max_time_diff = tmp_time_diff;
                *addr_index = counter;
            }
        }
    }

    if( max_time_diff == 0 )
    {
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_hb_path_verf
 *
 *    DESCRIPTION:
 *        Send chunk HEARTBEAT for Path Verification.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void sctp_send_chunk_hb_path_verf( sctp_tcb_st *p_ass )
{
    sctp_U16bit num_hb_sent = 0;
    sctp_U32bit saddr_index = 0;
    sctp_U32bit dest_addr_index = 0;

    for( ; num_hb_sent < p_ass->config.hb_max_burst ; num_hb_sent++ )
    {
        if( sctp_get_addr_index_for_hb( p_ass, &dest_addr_index ) == SCTP_FAILURE )
        {
            /* No more Destination Addresses left to which heartbeat
               could be sent, so return. */
            return;
        }

        /* Get  Source address index corresponding to destination. */
        saddr_index = p_ass->transmit.remote[dest_addr_index].src_addr_index;

        sctp_send_chunk_heartbeat( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   dest_addr_index,
                                   p_ass->remote_tag );

        /* restart heartbeat ack timer */
        sctp_stop_timer(
            &p_ass->transmit.remote[dest_addr_index].dest_timer_id[SCTP_PATH_VERF_TIMER],
            SCTP_NULL,
            SCTP_NULL );

        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERHB,
            p_ass->transmit.remote[dest_addr_index].rto,
            dest_addr_index,
            &p_ass->transmit.remote[dest_addr_index].dest_timer_id[SCTP_PATH_VERF_TIMER],
            SCTP_NULL );

        /* Update last user time */
        __GETSYSTIME( &p_ass->transmit.remote[dest_addr_index].last_used_time );
    }
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_heartbeat
 *
 *    DESCRIPTION:
 *        Send chunk HEARTBEAT
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_heartbeat(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_U32bit      dest_index,
    sctp_U32bit      tag )
{
    sctp_U8bit  *buffer;
    sctp_U8bit  *p_buf  = SCTP_NULL;
    sctp_U8bit  *p_chunk_start = SCTP_NULL;
    sctp_U8bit  *p_offset      = SCTP_NULL;
    sctp_U8bit  *sctp_offset   = SCTP_NULL;
    sctp_U32bit pad_len;
    sctp_time_t cur_time;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );
    sctp_addrinfo_st to;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_heartbeat" ) );

    /* klocwork fix for IBM */
    if( p_ass == SCTP_NULL )
    {
        return ;
    }

    /* klocwork fix for IBM */

    to.p_sockaddr = &p_ass->transmit.remote[dest_index].that_address;
    to.checksum   = p_ass->config.checksum_func;

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;
    __GETSYSTIME( &cur_time );

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;
    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_HEARTBEAT ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );    /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );   /* Length - Placeholder */

    p_offset = p_buf;

    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_HBINFO ); /* Info type */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );   /* len place holder */

    p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_TYPE_TIME ); /* Info time */

    /* Len-Placeholder */
    p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_LEN_TIME );
    p_buf = sctp_buffer_set_Ulong( p_buf, cur_time.tv_sec ); /* sec */
    p_buf = sctp_buffer_set_Ulong( p_buf, cur_time.tv_usec ); /* usec */

    if( to.p_sockaddr->family == SCTP_AF_INET )
    {
        /* Heartbeat Info Type */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_TYPE_ADDRV4 );

        /* Length - Placeholder */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_LEN_ADDRV4 );

        p_buf = sctp_buffer_set_Ulong( p_buf, to.p_sockaddr->ip.v4.addr ); /* dest addr */

    }

    else
    {
        /* Heartbeat Info Type */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_TYPE_ADDRV6 );

        /* Length - Placeholder */
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_LEN_ADDRV6 );

        /* dest addr */
        p_buf = sctp_buffer_set_Ulong( p_buf, to.p_sockaddr->ip.v6.addr.s_u6_addr32[0] );
        p_buf = sctp_buffer_set_Ulong( p_buf, to.p_sockaddr->ip.v6.addr.s_u6_addr32[1] );
        p_buf = sctp_buffer_set_Ulong( p_buf, to.p_sockaddr->ip.v6.addr.s_u6_addr32[2] );
        p_buf = sctp_buffer_set_Ulong( p_buf, to.p_sockaddr->ip.v6.addr.s_u6_addr32[3] );

    }

    /* Info nonce */
    p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_TYPE_NONCE );

    /* Len-Placeholder */
    p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_LEN_NONCE );

    /* If the Address is UNCONFIRMED, add a 64-bit Nonce since this Heartbeat
       will be used for PATH Verification purpose. */
    if( p_ass->transmit.remote[dest_index].is_confirmed == SCTP_FALSE )
    {
        while( ( p_ass->transmit.remote[dest_index].nonce[0] = __RANDOM() ) == 0 );

        while( ( p_ass->transmit.remote[dest_index].nonce[1] = __RANDOM() ) == 0 );

        p_buf = sctp_buffer_set_Ulong( p_buf, p_ass->transmit.remote[dest_index].nonce[0] );
        p_buf = sctp_buffer_set_Ulong( p_buf, p_ass->transmit.remote[dest_index].nonce[1] );
    }

    else
    {
        /* Just increment the p_buff since we will not validate this field
           when we receive Heartbeat-Ack. */
        p_buf += 8;
    }

    /* HB Info 'Type' */
    p_buf = sctp_buffer_set_Ushort( p_buf, SCTP_HBINFO_TYPE_PAD_DATA );

    /* p_buf +- '2' : 2 is the size of 'Length' parameter */
    if( ( p_buf + 2 - p_offset ) < p_ass->config.hb_info_length )
    {
        /* set the length of padding */
        p_buf =
            sctp_buffer_set_Ushort( p_buf,
                                    ( sctp_U16bit )( p_ass->config.hb_info_length -
                                                     ( p_buf - 2 - p_offset ) ) );

        pad_len = p_ass->config.hb_info_length - ( p_buf - p_offset );

        /* In Value Pad the extra length by zeroes */
        p_buf =
            sctp_buffer_PadToLen( p_buf, pad_len );
    }

    else
    {
        /* set the length of padding to 0 as no padding is required*/
        p_buf =
            sctp_buffer_set_Ushort( p_buf, 0 );

        p_ass->config.hb_info_length = p_buf - p_offset;
    }


    /* update heartbeat info TLV lengh */
    sctp_buffer_set_UshortAtOffset( p_offset, SCTPPARAMOFFSET_LENGTH,
                                    ( sctp_U16bit )( p_buf - p_offset ) );
    /* update heartbeat    chunk lengh */
    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );  /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, to.checksum );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_heartbeat:Chunk HB Sent,Src Port=%d,Dest Port=%d, "
                  "Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, to.p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_HB_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, to.p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( to.p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, to.p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops */
    /* Chnages For CSR#1-5037386 */
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
    sctp_check_assoc_health_and_reset_assoc_if_reqd( p_ass, SCTP_TRUE );
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS */
    /* Chnages For CSR#1-5037386 Ends */
    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_heartbeatack
 *
 *    DESCRIPTION:
 *        Send chunk HEARTBEAT ACK.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_heartbeatack(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U16bit      heartbeatsize,
    sctp_U8bit       *heartbeat )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_heartbeatack" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_HEARTBEATACK ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );      /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );     /* Length - Placeholder */
    p_buf = sctp_buffer_set_Buffer( p_buf, heartbeatsize, heartbeat );

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );   /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_heartbeatack:Chunk HBACK Sent, Src Port=%d, Dest Port=%d,"\
                  " Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_HBACK_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* klocwork fix for IBM */
    if( p_ass == SCTP_NULL )
    {
        return ;
    }

    /* klocwork fix for IBM */

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/

    /* Chnages For CSR#1-5037386 */
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
    sctp_check_assoc_health_and_reset_assoc_if_reqd( p_ass, SCTP_FALSE );
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS */
    /* Chnages For CSR#1-5037386 Ends */
    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_add_chunk_sack
 *
 *    DESCRIPTION: Add SACK chunk
 *
 *    RETURN VALUE:
 *        sctp_U8bit *
 **************************************************************************/
sctp_U8bit *
sctp_add_chunk_sack(
    sctp_tcb_st     *p_ass,
    sctp_U8bit      *p_buf,
    sctp_U32bit     cumtsn,
    sctp_U32bit     arwnd,
    LIST            *sacklist,
    sctp_U16bit     num_duplicate_tsn,
    sctp_U32bit     *duplicat_tsn )
{
    sctp_U8bit     *p_chunk_start;
    sctp_U8bit     *p_offset, *p_offset_dup_tsn;
    sctpsackbuf_st *sack_buff;
    sctp_U16bit    counter = 0;
    sctp_U16bit     cur_chunk_size; /*Size of data already filled in packet*/
    sctp_U16bit     pmtu = p_ass->transmit.lowest_path_mtu;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_add_chunk_sack" ) );

    cur_chunk_size = p_buf -  p_mtubuf->buffer;

    if( pmtu < ( cur_chunk_size + 16 ) )
    {
        return p_buf;
    }

    p_chunk_start = p_buf;
    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_SACK ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );      /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );     /* Length - Placeholder */

    p_buf = sctp_buffer_set_Ulong( p_buf, cumtsn ); /* Cumulative TSN */
    p_buf = sctp_buffer_set_Ulong( p_buf, arwnd ); /* Advertised Receiver Window*/

    p_offset = p_buf;

    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Number of Frags */

    /* number duplicate tsn */
    p_offset_dup_tsn = p_buf;
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 );

    cur_chunk_size += 16;

    for( sack_buff = ( sctpsackbuf_st * )lstFirst( sacklist );
            sack_buff && ( pmtu >= ( 4 + cur_chunk_size ) );
            sack_buff = ( sctpsackbuf_st * )lstNext( ( NODE * )sack_buff ) )
    {
        counter++;
        p_buf = sctp_buffer_set_Ushort( p_buf,
                                        ( sctp_U16bit )( sack_buff->first - cumtsn ) );
        p_buf = sctp_buffer_set_Ushort( p_buf,
                                        ( sctp_U16bit )( sack_buff->last - cumtsn ) );
        cur_chunk_size += 4;
    }

    if( pmtu < ( cur_chunk_size + 4 ) )
    {
        sctp_buffer_set_UshortAtOffset( p_offset_dup_tsn, 0, 0 );

        sctp_buffer_set_UshortAtOffset( p_offset, 0, counter );

        sctp_buffer_set_UshortAtOffset(
            p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
            ( sctp_U16bit )( p_buf - p_chunk_start ) );

        p_buf = sctp_buffer_PadTo( p_buf, p_chunk_start, 4 ); /* Pad the chunk */

        return p_buf;
    }

    /* Fill in number of fragmemts */
    sctp_buffer_set_UshortAtOffset( p_offset, 0, counter );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\n Send Sack !!,cumtsn=%u,num_frags=%d, rwnd:%u\n",
                                           cumtsn, counter, arwnd ) );

    /* SPR : 18527 */
    if( duplicat_tsn != SCTP_NULL )
        /* SPR : 18527 */
    {
        /* duplicate TSN */
        for( counter = 0; ( counter < num_duplicate_tsn ) && ( pmtu >= ( 4 + cur_chunk_size ) )  ; counter++ )
        {
            p_buf = sctp_buffer_set_Ulong( p_buf, duplicat_tsn[counter] );
            cur_chunk_size += 4;
        }
    }

    sctp_buffer_set_UshortAtOffset( p_offset_dup_tsn, 0, counter );

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, p_chunk_start, 4 ); /* Pad the chunk */

    return p_buf;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_sack
 *
 *    DESCRIPTION:Send SACK chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_sack(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U32bit      cumtsn,
    sctp_U32bit      arwnd,
    LIST             *sacklist,
    sctp_U16bit      num_duplicate_tsn,
    sctp_U32bit      *duplicat_tsn )
{
    sctp_U8bit    *buffer;
    sctp_U8bit    *p_buf;
    sctp_U8bit    *sctp_offset;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );
    /* SCTP REL 6.2.0 changes start */
    /* Changes for CSR#1-5037386 */
    sctp_U32bit count = 0;

    /* Changes for CSR#1-5037386 Ends */

    /* SCTP REL 6.2.0 changes end */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_sack" ) );

    buffer = p_mtubuf->buffer;

    /* klocwork fix for IBM */
    if( p_ass == SCTP_NULL )
    {
        return ;
    }

    /* klocwork fix for IBM */

    sctp_get_confirmed_address( p_ass, p_to->p_sockaddr, p_from );

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_buf = sctp_add_chunk_sack(
                p_ass,
                p_buf,
                cumtsn,
                arwnd,
                sacklist,
                num_duplicate_tsn,
                duplicat_tsn );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_sack: Chunk SACK Sent, Src Port=%d, Dest Port=%d,"
                  "Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_SACK_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts*/
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/

    /* SCTP Rel 6.2.0 changes start */
    /* To avoid redundancy prefered_sack_dest is set in sctp_send_chunk_sack rather
        than after sctp_send_chunk_sack everytime */
    /* Changes for CSR#1-5037386 */
    /* reset prefered sack destination flags for all destinations */
    for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
    {
        p_ass->transmit.remote[count].prefered_sack_dest = SCTP_FALSE;
    }

    /* Changes for CSR#1-5037386 Ends */
    /* SCTP Rel 6.2.0 changes start */

    return;
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_error
 *
 *    DESCRIPTION:
 *        Send    ERROR chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_error(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U16bit      cause_code,
    sctp_U16bit      cause_len,
    sctp_U8bit       *cause_info )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *p_offset_ecode = SCTP_NULL;
    sctp_U8bit *sctp_offset    = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf;
    sctp_ep_st    *p_ep   = SCTP_NULL; /* SPR 20859 */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_error" ) );

    if( SCTP_NULL == p_ass )
    {
        p_mtubuf = sctp_db_get_mtu_buffer();

        if( !p_mtubuf )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                        SCTPERROR_LEVEL_CRITICAL, E_NORESOURCES,
                        "At senddata_on_alternate_addr:"
                        " could not get MTU buffer " );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

            return;
        }
    }

    else
    {
        p_mtubuf = &( p_ass->mtu_buffer );
        sctp_get_confirmed_address( p_ass, p_to->p_sockaddr, p_from );
    }

    buffer = p_mtubuf->buffer;


    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );
    p_chunk_start = p_buf;
    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_ERROR ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    p_offset_ecode = p_buf;
    p_buf = sctp_buffer_set_Ushort( p_buf, cause_code ); /* Len- Placeholder */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    /* SPR : 18527 */
    if( cause_info != SCTP_NULL )
        /* SPR : 18527 */
    {
        p_buf = sctp_buffer_set_Buffer( p_buf, cause_len, cause_info );    /* err cause */
    }

    sctp_buffer_set_UshortAtOffset( p_offset_ecode, SCTPCHUNKOFFSET_LENGTH,
                                    ( sctp_U16bit )( p_buf - p_offset_ecode ) );


    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    /* SPR 20283 */
    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad this cause code */
    /* SPR 20283 */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_error:Chunk ERROR Sent,Src Port=%d, Dest Port=%d,"
                  "Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ERROR_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else if( ( SCTP_SUCCESS == sctp_db_locate_server( srcport, p_from, &p_ep ) ) && ( p_ep->send_func != SCTP_NULL ) )
    {
        p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/


    if( SCTP_NULL == p_ass )
    {
        /*
         * Free the buffer only if it was taken from the global MTU buffer.
         */
        sctp_db_free_mtu_buffer( p_mtubuf );
    }

    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_ecne
 *
 *    DESCRIPTION:
 *        Send    explicit congestion notification error
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_ecne(
    sctp_tcb_st        *p_ass,
    sctp_sockaddr_st   *p_from,
    sctp_U16bit        srcport,
    sctp_U16bit        dstport,
    sctp_addrinfo_st   *p_to,
    sctp_U32bit        tag,
    sctp_U32bit        lowest_tsn )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_ecne" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_ECNE ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 );  /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */
    /* set the chunk specific parameters */
    p_buf = sctp_buffer_set_Ulong( p_buf, lowest_tsn );

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_ecne:Chunk ECNE Sent, Src Port=%d, Dest Port=%d,\
         Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ECNE_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_cwr
 *
 *    DESCRIPTION:Send CWR chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_cwr(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U32bit      lowest_tsn )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_cwr" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;
    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_CWR );  /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */
    p_buf = sctp_buffer_set_Ulong( p_buf, lowest_tsn );

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );     /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_cwr:Chunk CWR Sent, Src Port=%d, Dest Port=%d,\
               Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_CWR_SENT,    1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/


    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_unrecognise_chunk_error
 *
 *    DESCRIPTION:
 *        Send unrecognised ERROR chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_unrecognise_chunk_error(
    sctp_tcb_st      *p_ass,
    sctp_sockaddr_st *p_from,
    sctp_U16bit      srcport,
    sctp_U16bit      dstport,
    sctp_addrinfo_st *p_to,
    sctp_U32bit      tag,
    sctp_U16bit      cause_code,
    sctp_U16bit      cause_len,
    sctp_U8bit       *cause_info )
{
    sctp_U16bit  error_offset;
    /* Bug Id 81 fix s*/
    /*sctp_U16bit  err_len;*/
    sctp_U16bit  err_len = 0;
    /* Bug Id 81 fix e*/
    sctp_U8bit   *buffer;
    sctp_U8bit   *p_buf  = SCTP_NULL;
    sctp_U8bit   *p_chunk_start = SCTP_NULL;
    sctp_U8bit   *p_offset_ecode = SCTP_NULL;
    sctp_U8bit   *sctp_offset = SCTP_NULL;
    sctpmtubuf_st *p_mtubuf;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_unrecognise_chunk_error" ) );

    if( SCTP_NULL == p_ass )
    {
        p_mtubuf = sctp_db_get_mtu_buffer();

        if( !p_mtubuf )
        {
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                        SCTPERROR_LEVEL_CRITICAL, E_NORESOURCES,
                        "At senddata_on_alternate_addr:"
                        " could not get MTU buffer " );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

            return;
        }
    }

    else
    {
        p_mtubuf = &( p_ass->mtu_buffer );
        sctp_get_confirmed_address( p_ass, p_to->p_sockaddr, p_from );
    }

    buffer = p_mtubuf->buffer;


    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;

    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );
    p_chunk_start = p_buf;
    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_ERROR ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */


    error_offset = 0;

    for( ; ; )
    {
        if( error_offset >= cause_len )
        {
            break;
        }

        p_offset_ecode = p_buf;

        /* Len-Placeholder */
        p_buf = sctp_buffer_set_Ushort( p_buf, cause_code );
        p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

        err_len = sctp_buffer_get_UshortAtOffset(
                      cause_info, error_offset + SCTPERROROFFSET_LENGTH );

        /* errcause */
        /*Bug Id 81 fix s*/
        if( err_len > SCTP_MAX_UNRECOGNISE_CHUNK_LEN )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Invalid chunk length" ) );
            return;
        }

        /*Bug Id 81 fix e*/

        p_buf = sctp_buffer_set_Buffer(
                    p_buf, err_len, cause_info + error_offset );

        sctp_buffer_set_UshortAtOffset(
            p_offset_ecode, SCTPERROROFFSET_LENGTH,
            ( sctp_U16bit )( p_buf - p_offset_ecode ) );

        p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 ); /* Pad this cause code */

        error_offset += sctp_get_parameter_pad_bytes( err_len ) + err_len;

    }

    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_unrecognise_chunk_error:Chunk ERROR Sent,Src Port=%d,"
                  "Dest Port=%d, Ver Tag=%u,To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ERROR_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/


    if( SCTP_NULL == p_ass )
    {
        /*
         * Free the buffer only if it was taken from the global MTU buffer.
         */
        sctp_db_free_mtu_buffer( p_mtubuf );
    }

    return;
}


#ifdef SCTP_DYNAMIC_IP_SUPPORT

static sctp_U8bit *
sctp_build_asconf_param(
    sctp_U8bit             *p_buf,
    sctp_asconf_param_ut    *p_asconf_param );

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_asconf
 *
 *    DESCRIPTION:Send  Asconf chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_asconf(
    sctp_tcb_st         *p_ass,
    sctp_sockaddr_st    *p_from,
    sctp_U16bit         srcport,
    sctp_U16bit         dstport,
    sctp_addrinfo_st    *p_to,
    sctp_U32bit         tag,
    sctp_addr_conf_st   *p_addr_conf )
{
    sctp_U8bit *buffer;
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctp_U32bit sctp_index = 0;
    sctpmtubuf_st *p_mtubuf = &( p_ass->mtu_buffer );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_asconf" ) );

    buffer = p_mtubuf->buffer;

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;
    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_TYPE_ASCONF ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    /*parameters in the chunk*/
    p_buf = sctp_buffer_set_Ulong( p_buf, p_addr_conf->serial_no ); /*serial no*/

    /*IP address*/
    if( p_addr_conf->src_addr.family == SCTP_AF_INET )
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
        p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

        p_buf = sctp_buffer_set_Ulong( p_buf, p_addr_conf->src_addr.ip.v4.addr );
    }

    else
    {
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
        p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       p_addr_conf->src_addr.ip.v6.addr.s_u6_addr32[0] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       p_addr_conf->src_addr.ip.v6.addr.s_u6_addr32[1] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       p_addr_conf->src_addr.ip.v6.addr.s_u6_addr32[2] );
        p_buf = sctp_buffer_set_Ulong( p_buf,
                                       p_addr_conf->src_addr.ip.v6.addr.s_u6_addr32[3] );
    }

    for( sctp_index = 0; sctp_index < p_addr_conf->num_params; sctp_index++ )
    {
        p_buf = sctp_build_asconf_param( p_buf,
                                         &( p_addr_conf->asconf_params[sctp_index] ) );
    }

    /*set the length*/
    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, buffer, 4 );     /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_cwr:Chunk CWR Sent, Src Port=%d, Dest Port=%d,\
               Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_CWR_SENT,    1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            buffer, ( p_buf - buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/


    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_build_asconf_param
 *
 *    DESCRIPTION:Builds asconf message parameters.
 *
 *    RETURN VALUE:
 *        sctp_U8bit*
 **************************************************************************/
sctp_U8bit *
sctp_build_asconf_param(
    sctp_U8bit             *p_buf,
    sctp_asconf_param_ut    *p_asconf_param )
{
    sctp_U8bit  *p_param_buf = p_buf;
    sctp_U16bit param_len = 0;
    sctp_U16bit param_type =
        ( sctp_U16bit )p_asconf_param->param_type;

    p_buf += 4; /*skip type and length for now*/

    switch( param_type )
    {
        case SCTP_ASCONF_PARAM_ADD_IP:
        case SCTP_ASCONF_PARAM_DEL_IP:
        case SCTP_ASCONF_PARAM_SET_PRIM:
            {
                /*parameters in the chunk*/
                sctp_asconf_addip_param_st *p_param =
                    &( p_asconf_param->add_ip );
                p_buf = sctp_buffer_set_Ulong( p_buf, p_param->corr_id ); /*serial no*/

                /*IP address*/
                if( p_param->ipaddr.family == SCTP_AF_INET )
                {
                    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
                    p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

                    p_buf = sctp_buffer_set_Ulong( p_buf, p_param->ipaddr.ip.v4.addr );
                }

                else
                {
                    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
                    p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                   p_param->ipaddr.ip.v6.addr.s_u6_addr32[0] );
                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                   p_param->ipaddr.ip.v6.addr.s_u6_addr32[1] );
                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                   p_param->ipaddr.ip.v6.addr.s_u6_addr32[2] );
                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                   p_param->ipaddr.ip.v6.addr.s_u6_addr32[3] );
                }
            }
            break;

        default:
            /*can not come at this place*/
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "invalid asconf param type" ) );
            break;
    }

    param_len   = p_buf - p_param_buf;
    p_param_buf = sctp_buffer_set_Ushort( p_param_buf, param_type );
    p_param_buf = sctp_buffer_set_Ushort( p_param_buf, param_len );
    return p_buf;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_chunk_asconf_ack
 *
 *    DESCRIPTION:Sends asconf ack chunk
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_send_chunk_asconf_ack(
    sctp_tcb_st         *p_ass,     /* SPR 20859*/
    sctp_sockaddr_st    *p_from,
    sctp_U16bit         srcport,
    sctp_U16bit         dstport,
    sctp_addrinfo_st    *p_to,
    sctp_U32bit         tag,
    sctp_addr_conf_ack_st   *p_addr_conf,
    sctp_U8bit          *p_buffer,
    sctp_U32bit         *p_buffer_len )
{
    sctp_U8bit *p_buf  = SCTP_NULL;
    sctp_U8bit *p_chunk_start = SCTP_NULL;
    sctp_U8bit *sctp_offset   = SCTP_NULL;
    sctp_U32bit sctp_index = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_send_chunk_asconf" ) );

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = p_buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = p_buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_buf = sctp_offset;
    p_buf = sctp_build_common_header( p_buf, srcport, dstport, tag );

    p_chunk_start = p_buf;

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_TYPE_ASCONFACK );  /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, 0 ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */

    /*parameters in the chunk*/
    p_buf = sctp_buffer_set_Ulong( p_buf, p_addr_conf->serial_no ); /*serial no*/

    for( sctp_index = 0; sctp_index < p_addr_conf->num_params; sctp_index++ )
    {
        sctp_U8bit  *p_param_buf = p_buf;
        sctp_U16bit param_len = 0;
        sctp_U16bit param_type =
            ( sctp_U16bit )p_addr_conf->asconf_params[sctp_index].param_type;

        p_buf += 8; /*skip type, length and corr_id for now*/

        switch( param_type )
        {
            case SCTP_ASCONF_PARAM_ERR_CAUSE:
                {
                    sctp_U8bit  *p_buf_start = p_buf;
                    p_buf = sctp_buffer_set_Ushort( p_buf,
                                                    p_addr_conf->asconf_params[sctp_index].err_cause.cause_code );
                    /*need to copy the asconf TLV*/
                    p_buf += 2; /*length placeholder*/

                    switch( p_addr_conf->asconf_params[sctp_index].err_cause.cause_code )
                    {
                        case SCTPERR_LASTIPADDRESS:
                        case SCTPERR_SRCIPADDRESS:
                        case SCTPERR_OPREFRESOURCESHORT:
                            p_buf = sctp_build_asconf_param( p_buf,
                                                             &p_addr_conf->asconf_params[sctp_index].err_cause.invalid_param );
                            break;

                        case SCTPERR_UNRESOLVEADDR:
                            {
                                sctp_sockaddr_st *p_ipaddr =
                                    &( p_addr_conf->asconf_params[sctp_index].err_cause.
                                       invalid_param.add_ip.ipaddr );

                                if( p_ipaddr->family == SCTP_AF_INET )
                                {
                                    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV4ADDRESS );
                                    p_buf = sctp_buffer_set_Ushort( p_buf, 8 );  /* Length */

                                    p_buf = sctp_buffer_set_Ulong( p_buf, p_ipaddr->ip.v4.addr );
                                }

                                else
                                {
                                    p_buf = sctp_buffer_set_Ushort( p_buf, SCTPVARPAR_IPV6ADDRESS );
                                    p_buf = sctp_buffer_set_Ushort( p_buf, 20 );  /* Length */
                                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                                   p_ipaddr->ip.v6.addr.s_u6_addr32[0] );
                                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                                   p_ipaddr->ip.v6.addr.s_u6_addr32[1] );
                                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                                   p_ipaddr->ip.v6.addr.s_u6_addr32[2] );
                                    p_buf = sctp_buffer_set_Ulong( p_buf,
                                                                   p_ipaddr->ip.v6.addr.s_u6_addr32[3] );
                                }
                            }
                            break;

                    }

                    sctp_buffer_set_Ushort( p_buf_start + 2,
                                            ( sctp_U16bit )( p_buf - p_buf_start ) );
                }
                break;

            case SCTP_ASCONF_PARAM_SUCCESS_REPORT:
                /*nothing to be done*/
                break;

            default:
                /*can not come at this place*/
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "invalid asconf param type" ) );
                break;
        }

        param_len   = p_buf - p_param_buf;
        p_param_buf = sctp_buffer_set_Ushort( p_param_buf, param_type );
        p_param_buf = sctp_buffer_set_Ushort( p_param_buf, param_len );
        p_param_buf = sctp_buffer_set_Ulong( p_param_buf,
                                             p_addr_conf->asconf_params[sctp_index].corr_id );
    }

    /*add the error codes, if unrecognize parameter present*/
    if( p_addr_conf->unreco_param_len > 0 )
    {
        /*build the error code parameter*/
        sctp_U8bit *p_offset_ecode = p_buf;
        p_buf = sctp_buffer_set_Ushort( p_buf, SCTPERR_UNRECOPARAMS );
        p_buf = sctp_buffer_set_Ushort( p_buf, 0 ); /* Length - Placeholder */
        p_buf = sctp_buffer_set_Buffer( p_buf, p_addr_conf->unreco_param_len,
                                        p_addr_conf->p_unreco_params ); /* err cause */

        sctp_buffer_set_UshortAtOffset( p_offset_ecode, SCTPCHUNKOFFSET_LENGTH,
                                        ( sctp_U16bit )( p_buf - p_offset_ecode ) );

        p_buf = sctp_buffer_PadTo( p_buf, p_offset_ecode, 4 ); /* Pad this cause code */
    }


    /*set the length*/
    sctp_buffer_set_UshortAtOffset(
        p_chunk_start, SCTPCHUNKOFFSET_LENGTH,
        ( sctp_U16bit )( p_buf - p_chunk_start ) );

    p_buf = sctp_buffer_PadTo( p_buf, p_buffer, 4 );     /* Pad the chunk */

    sctp_insert_chksum( p_buf, sctp_offset, p_to->checksum );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_chunk_cwr:Chunk CWR Sent, Src Port=%d, Dest Port=%d,\
               Ver Tag=%u, To Addr:", srcport, dstport, tag ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, p_to->p_sockaddr );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_CWR_SENT,    1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( p_buffer, ( p_buf - p_buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from, SCTP_FALSE );
    }

    else
    {
        /* Sends the message to the appropriate node */
        sctp_send_message[( p_to->p_sockaddr->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
            p_buffer, ( p_buf - p_buffer ), SCTP_FALSE, p_to->p_sockaddr, p_from );
    }

    /* Callback for Sending Func SPR 20859 Stops*/

    *p_buffer_len = p_buf - p_buffer;

    return;
}

#endif

/* SPR 20568 Starts for CSR 1-6658486 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_buffer_set_cookie
 *
 *    DESCRIPTION: This function fills the buffer from cookie
 *
 *    RETURN VALUE:
 *        sctp_U8bit *
 **************************************************************************/
sctp_U8bit  *
sctp_buffer_set_cookie(
    sctp_U8bit  *p_buf,
    sctp_chunk_cookie_st    *p_cookie )
{
    sctp_U32bit    counter;

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        p_buf = sctp_buffer_set_Uchar(
                    p_buf, p_cookie->digest[counter] );
    }

    p_buf = sctp_buffer_set_Buffer( p_buf,
                                    sizeof( sctp_time_t ), ( sctp_U8bit * )&p_cookie->time_stamp );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->life_time );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->ecn_capable );

    /* Initiate Tag */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_init_parameters.fixed_params.init_tag );

    /* A-RWND */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_init_parameters.fixed_params.a_rwnd );

    /* In-Streams */
    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->local_init_parameters.fixed_params.num_in_streams );

    /* Number of OUT streams */
    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->local_init_parameters.fixed_params.num_out_streams );


    /* Initial TSN */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_init_parameters.fixed_params.init_tsn );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_init_parameters.hostname_present );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_init_parameters.num_addrs );


    for( counter = 0; counter < p_cookie->local_init_parameters.num_addrs;
            counter++ )
    {
        p_buf = sctp_buffer_set_Buffer(
                    p_buf, sizeof( sctp_sockaddr_st ), ( sctp_U8bit * )&p_cookie->local_init_parameters.addr_list[counter] );
    }

    for( counter = 0; counter < SCTP_MAX_HOSTNAME_SIZE; counter++ )
    {
        p_buf = sctp_buffer_set_Uchar(
                    p_buf, p_cookie->local_init_parameters.hostname[counter] );
    }

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_init_parameters.adaption_ind );


    /* Initiate Tag */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->remote_init_parameters.fixed_params.init_tag );

    /* A-RWND */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->remote_init_parameters.fixed_params.a_rwnd );

    /* In-Streams */
    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->remote_init_parameters.fixed_params.num_in_streams );

    /* Number of OUT streams */
    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->remote_init_parameters.fixed_params.num_out_streams );


    /* Initial TSN */
    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->remote_init_parameters.fixed_params.init_tsn );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->remote_init_parameters.hostname_present );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->remote_init_parameters.num_addrs );


    for( counter = 0; counter < p_cookie->remote_init_parameters.num_addrs;
            counter++ )
    {
        p_buf = sctp_buffer_set_Buffer(
                    p_buf, sizeof( sctp_sockaddr_st ), ( sctp_U8bit * )&p_cookie->remote_init_parameters.addr_list[counter] );
    }

    for( counter = 0; counter < SCTP_MAX_HOSTNAME_SIZE; counter++ )
    {
        p_buf = sctp_buffer_set_Uchar(
                    p_buf, p_cookie->remote_init_parameters.hostname[counter] );
    }

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->remote_init_parameters.adaption_ind );

    /* To make inline with 6.2.0 */
    p_buf = sctp_buffer_set_Buffer(
                p_buf, sizeof( sctp_sockaddr_st ), ( sctp_U8bit * )&p_cookie->that_address );

    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->dest_port );
    /* To make inline with 6.2.0 */

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->local_tie_tag );

    p_buf = sctp_buffer_set_Ulong(
                p_buf, p_cookie->peer_tie_tag );

    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->this_port );
    p_buf = sctp_buffer_set_Ushort(
                p_buf, p_cookie->unexpected_init_params_len );

    return p_buf;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_buffer_get_cookie
 *
 *    DESCRIPTION: This function fills the cookie chunk from buffer
 *
 *    RETURN VALUE:
 *        sctp_U8bit *
 **************************************************************************/
sctp_U8bit  *
sctp_buffer_get_cookie(
    sctp_U8bit  *p_buf,
    sctp_chunk_cookie_st    *p_cookie )
{
    sctp_U32bit    counter;

    __MEMSET( p_cookie, 0, sizeof( sctp_chunk_cookie_st ) );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        p_buf = sctp_buffer_get_Uchar(
                    p_buf, &p_cookie->digest[counter] );
    }

    p_buf = sctp_buffer_get_Buffer(
                p_buf, sizeof( sctp_time_t ), ( sctp_U8bit * )&p_cookie->time_stamp );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->life_time );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->ecn_capable );

    /* Initiate Tag */
    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_init_parameters.fixed_params.init_tag );

    /* A-RWND */
    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_init_parameters.fixed_params.a_rwnd );

    /* In-Streams */
    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->local_init_parameters.fixed_params.num_in_streams );

    /* Number of OUT streams */
    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->local_init_parameters.fixed_params.num_out_streams );


    /* Initial TSN */
    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_init_parameters.fixed_params.init_tsn );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_init_parameters.hostname_present );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_init_parameters.num_addrs );


    for( counter = 0; counter < p_cookie->local_init_parameters.num_addrs;
            counter++ )
    {
        p_buf = sctp_buffer_get_Buffer(
                    p_buf, sizeof( sctp_sockaddr_st ), ( sctp_U8bit * )&p_cookie->local_init_parameters.addr_list[counter] );
    }

    for( counter = 0; counter < SCTP_MAX_HOSTNAME_SIZE; counter++ )
    {
        p_buf = sctp_buffer_get_Uchar(
                    p_buf, &p_cookie->local_init_parameters.hostname[counter] );
    }

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_init_parameters.adaption_ind );


    /* Initiate Tag */
    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->remote_init_parameters.fixed_params.init_tag );

    /* A-RWND */
    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->remote_init_parameters.fixed_params.a_rwnd );

    /* In-Streams */
    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->remote_init_parameters.fixed_params.num_in_streams );

    /* Number of OUT streams */
    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->remote_init_parameters.fixed_params.num_out_streams );


    /* Initial TSN */
    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->remote_init_parameters.fixed_params.init_tsn );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->remote_init_parameters.hostname_present );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->remote_init_parameters.num_addrs );


    for( counter = 0; counter < p_cookie->remote_init_parameters.num_addrs;
            counter++ )
    {
        /* Set the address according to the type. */
        p_buf = sctp_buffer_get_Buffer(
                    p_buf, sizeof( sctp_sockaddr_st ), ( sctp_U8bit * )&p_cookie->remote_init_parameters.addr_list[counter] );
    }

    for( counter = 0; counter < SCTP_MAX_HOSTNAME_SIZE; counter++ )
    {
        p_buf = sctp_buffer_get_Uchar(
                    p_buf, &p_cookie->remote_init_parameters.hostname[counter] );
    }

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->remote_init_parameters.adaption_ind );

    /* To make inline with 6.2.0 */
    p_buf = sctp_buffer_get_Buffer(
                p_buf, sizeof( sctp_sockaddr_st ), ( sctp_U8bit * )&p_cookie->that_address );

    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->dest_port );
    /* To make inline with 6.2.0 */

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->local_tie_tag );

    p_buf = sctp_buffer_get_Ulong(
                p_buf, &p_cookie->peer_tie_tag );

    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->this_port );

    p_buf = sctp_buffer_get_Ushort(
                p_buf, &p_cookie->unexpected_init_params_len );
    return p_buf;

}

/* SPR 20568 Ends for CSR 1-6658486 */

