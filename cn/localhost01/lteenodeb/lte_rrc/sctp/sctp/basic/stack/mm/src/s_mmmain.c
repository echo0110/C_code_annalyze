/***************************************************************************
 *  FILE NAME:
 *    s_mmmain.c
 *
 *  DESCRIPTION:
 *    This module has all entry level functions.
 *
 *  DATE     NAME            REF#   REASON
 *  -------  ----            ----   ------
 *  22Mar00  sigtran team    ----   Original
 *  29Aug01  gsheoran        ----   REL 3.0
 *  11Sep03  manish rajpal          Rel 5.0
 *  07Oct03  Vivek Bansal    Rel5.0 Changes for CSPL
 *  27Nov03  Vivek Bansal    Rel5.0 Changes for CSPL
 *  29Mar11    Supreet Jain     SPR 20861      CSR 2918
 *  1May13  Pranav          SPR 21127  CSR 40731-GCC 4.7 warning removal
 *  15Sep14   Priyanka Mishra   Bug Id 22     CSR 85223

 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "MAIN"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_mhtbl.h>
#include <s_db.h>
#include <s_dbtcb.h>
#include <s_mhpars.h>
#include <s_mhmac.h>
#include <s_mhproc.h>
#include <s_fsmass.h>
#include <s_fsmmtu.h>
#include <s_osmutx.h>
#include <s_dbg.h>
#include <s_es.h>
#include <s_err.h>
#include <s_cobuf.h>
#include <s_resnam.h>
#include <s_mmmain.h>
#include <s_in.h>                    /* For TL_buffers */
#include <s_resnam.h>                /* For sctp_cookie_resolver_mutex */
#include <s_select.h>
#include <s_os.h>
#include <s_stats.h>
#include <s_dbutil.h>
#include <s_ostmr.h>

#ifdef SCTP_UI_SU_MESSAGEBASED
extern sctp_lock_t  msgif_lock;
#endif

/***************************************************************************
 *    FUNCTION :
 *        sctp_process_TL_datagram
 *
 *    DESCRIPTION:
 *        This is an entry level function in this module. This will be
 *        called from the receive/timeout thread after receiving data
 *        on underlying Transport Layer. This function will first  parse
 *        the whole datagram for valid chunks. If datagram has valid chunks
 *        it will call functions to process individual chunks
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_process_TL_datagram(
    sctp_U32bit       ip_header_offset,
    sctp_TL_packet_st *packet,
    sctp_U32bit       size,
    sctp_Boolean_t    resolver_flag )
{
    sctp_return_t      ret;
    sctp_U16bit        chunk_id = 0;
    sctp_U32bit        num_of_chunks = 0;
    sctp_U32bit        sctp_index = 0;
    sctp_chunk_info_st chunk_list[SCTP_MAX_CHUNKS_PERDATAGRAM];
    sctp_error_t       ecode = E_NOERROR;
    sctp_Boolean_t     last_chunk_flag = SCTP_FALSE;
    sctp_Boolean_t     unrecognise_chunk_present = SCTP_FALSE;
    sctp_U16bit        unrecognise_chunk_len = 0;
    sctp_U8bit         unrecognise_chunk[SCTP_MAX_UNKNOWN_CHUNK_LENGTH];
    sctp_Boolean_t     CE_bit   = SCTP_FALSE;
    sctp_packet_st     sctp_packet;
    sctp_tcb_st        *p_assoc = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_process_TL_datagram" ) );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_RECV, 1 );

#ifdef SCTP_UI_SU_MESSAGEBASED
    SCTP_LOCK( msgif_lock );
    SCTP_UNLOCK( msgif_lock );
#endif
    packet->packet_len = size;
    CE_bit = packet->CE_bit;


    /*
     * Parse the TL datagram into chunks. Get the number of chunks in the
     * IP packet.
     */
    ret = sctp_parse_packet_header( &( ( packet->buffer )[0] ) + ip_header_offset,
                                    size - ip_header_offset, &sctp_packet, &ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TL_ADDBUF( packet );
        return;
    }

    /* The TL packet */
    sctp_packet.p_tl_packet = packet;

    sctp_db_locate_association_verf(
        sctp_packet.src_port,
        &( sctp_packet.p_tl_packet->src_addr ),
        sctp_packet.dst_port,
        &( sctp_packet.p_tl_packet->dst_addr ),
        sctp_packet.v_tag,
        &p_assoc );



    if( p_assoc )
    {
        sctp_U32bit ep_id = p_assoc->p_ep->ep_id;
        sctp_assoc_t    assoc_id = p_assoc->assoc_id;
        SCTP_LOCK_EP( ep_id );
        p_assoc = sctp_db_get_association_ptr( ep_id, assoc_id );

        if( !p_assoc )
        {
            SCTP_UNLOCK_EP( ep_id );
        }
    }

    /*
    * Pass the p_assoc to the chunk processing functions.
    * note that this can be NULL.
    */

    ret = sctp_parse_chk_TL_datagram(
              packet->buffer + ip_header_offset,
              size - ip_header_offset,
              p_assoc,
              &sctp_packet,
              &num_of_chunks,
              &chunk_list[0],
              &unrecognise_chunk_present,
              &unrecognise_chunk_len,
              &unrecognise_chunk[0],
              resolver_flag,
              &ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TL_ADDBUF( packet );

        if( p_assoc )
        {
            sctp_ntfy_peer_data_drop( p_assoc, &ecode,
                                      sctp_packet.p_tl_packet->buffer, sctp_packet.p_tl_packet->packet_len );
            SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
        }

        return;
    }

    /*
     * Process the chunks one by one.
     */


    if( ( chunk_list[0].chunk_id == SCTPCHUNK_INIT )
            || ( chunk_list[0].chunk_id == SCTPCHUNK_INITACK ) )
    {
        /*SPR 6274 starts */
        ret = sctp_db_validate_bind_address_ext( 1, &( packet->dst_addr ) );

        if( ret == SCTP_FAILURE )
        {
            SCTP_TL_ADDBUF( packet );
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_TL_datagram: Packet is dropped/invalid dst" ) );

            /* SPR 20861 Starts */
            if( p_assoc )
            {
                SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
            }

            /*SPR 20861 Ends */
            return;
        }

        /*SPR 6274 ends */

        /*
         * If the chunk is bundled with another chunk then discard the packet.
         * Implementation guide (01:2.10i)
         */
        if( num_of_chunks == 1 )
        {
            sctp_db_locate_association(
                sctp_packet.src_port,
                &( sctp_packet.p_tl_packet->src_addr ),
                sctp_packet.dst_port,
                &( sctp_packet.p_tl_packet->dst_addr ),
                &p_assoc );

            if( p_assoc )
            {
                SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
                p_assoc = SCTP_NULL;
            }

            ret = ( *sctp_process_chunk_func_tbl[chunk_list[0].chunk_id] )
                  ( &sctp_packet,
                    &( ( packet->buffer )[0] ) + ip_header_offset,
                    chunk_list[0].chunk_offset,
                    last_chunk_flag,
                    p_assoc,
                    &ecode );
        }

        else
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_TL_datagram: Init /Init-Ack bundled" ) );

            if( p_assoc )
            {
                SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
            }

            SCTP_TL_ADDBUF( packet );
            return;
        }

        if( ret == SCTP_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_process_TL_datagram:Drop Rest of the Chunks in SCTP"
                          " Packet !!" ) );

            SCTP_TL_ADDBUF( packet );

            return;
        }

    }

    else
    {
        if( SCTP_NULL == p_assoc )
        {
            /* SPR 6274 starts */
            ret = sctp_db_validate_bind_address_ext( 1, &( packet->dst_addr ) );

            if( ret == SCTP_FAILURE )
            {
                SCTP_TL_ADDBUF( packet );
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                            ( "sctp_process_TL_datagram: Packet is dropped/invalid dst" ) );
                return;
            }

            /* SPR 6274 Ends */
        }

        for( sctp_index = 0; sctp_index < num_of_chunks; sctp_index++ )
        {
            /* Get the Chunk id */
            chunk_id = chunk_list[sctp_index].chunk_id;

            /*
             * If its the last chunk and the chunk type is data set the last
             * chunk flag. This flag is used for sack processing.
             */
            if( ( SCTPCHUNK_DATA == chunk_id ) && ( sctp_index == ( num_of_chunks - 1 ) ) )
            {
                last_chunk_flag = SCTP_TRUE;
            }

            if( !p_assoc )
            {
                /*could be that association has been established now*/
                sctp_db_locate_association_verf(
                    sctp_packet.src_port,
                    &( sctp_packet.p_tl_packet->src_addr ),
                    sctp_packet.dst_port,
                    &( sctp_packet.p_tl_packet->dst_addr ),
                    sctp_packet.v_tag,
                    &p_assoc );


                if( p_assoc )
                {
                    sctp_U32bit ep_id = p_assoc->p_ep->ep_id;
                    sctp_assoc_t    assoc_id = p_assoc->assoc_id;
                    SCTP_LOCK_EP( ep_id );
                    p_assoc = sctp_db_get_association_ptr( ep_id, assoc_id );

                    if( !p_assoc )
                    {
                        SCTP_UNLOCK_EP( ep_id );
                    }
                }

            }

#ifdef SCTP_NAME_RESOLV_THREAD

            /*
             * If the name resolver thread is enabled and the chunk is cookie
             * then special processing is done for the cookie containing
             * hostnames.
             */
            if( ( chunk_id == SCTPCHUNK_COOKIE ) && ( resolver_flag == SCTP_TRUE ) )
            {
                ret = sctp_process_resolved_cookie(
                          &sctp_packet,
                          &( ( packet->buffer )[0] ) + ip_header_offset,
                          chunk_list[sctp_index].chunk_offset,
                          last_chunk_flag,
                          p_assoc,
                          &ecode );
            }

            else
#endif /* SCTP_NAME_RESOLV_THREAD */
            {
                /* Normal processing for the rest of the chunks */
                ret = ( *sctp_process_chunk_func_tbl[chunk_id] )
                      ( &sctp_packet,
                        &( ( packet->buffer )[0] ) + ip_header_offset,
                        chunk_list[sctp_index].chunk_offset,
                        last_chunk_flag,
                        p_assoc,
                        &ecode );
            }

            if( ret == SCTP_FAILURE )
            {
                switch( ecode )
                {
#ifdef SCTP_NAME_RESOLV_THREAD

                    case E_COOKIE_SENT_TO_RESOLVER:
                        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                                    ( "sctp_process_TL_datagram: Packet added for "
                                      "hostname resolving!!" ) );


                        SCTP_TL_ADDBUF( packet );

                        if( p_assoc )
                        {
                            SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
                        }

                        return;
#endif

                    default:
                        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                                    ( "sctp_process_TL_datagram: Drop Rest of the "
                                      "Chunks in SCTP Packet !!" ) );

                        SCTP_TL_ADDBUF( packet );

                        if( p_assoc )
                        {
                            SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
                        }

                        return;
                }
            }

            /* turn off CE bit if we have a data chunk */
            if( chunk_id == SCTPCHUNK_DATA )
            {
                CE_bit = SCTP_FALSE;
            }

            /* SPR 21127 CHANGES START */
            SCTP_UNUSED_VARIABLE( CE_bit );
            /* SPR 21127 CHANGES END */
        }
    } /* else */

    if( unrecognise_chunk_present == SCTP_TRUE )
    {
        /* SPR 6274 starts */
        ret = sctp_db_validate_bind_address_ext( 1, &( packet->dst_addr ) );

        if( ret == SCTP_FAILURE )
        {
            SCTP_TL_ADDBUF( packet );
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_process_TL_datagram: Packet is dropped/invalid dst" ) );
            return;
        }

        /* SPR 6274 Ends */

        sctp_process_chunk_for_error(
            &sctp_packet,
            unrecognise_chunk_len,
            &unrecognise_chunk[0] );
    }

    /* Add the buffer back to global list */
    SCTP_TL_ADDBUF( packet );

    if( p_assoc )
    {
        SCTP_UNLOCK_EP( p_assoc->p_ep->ep_id );
    }

    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_timer_expiry
 *
 *    DESCRIPTION:
 *    This function handles the timer expiry.
 *
 *    RETURN VALUE:
 *       None
**************************************************************************/
void
sctp_process_timer_expiry(
    sctp_timer_t          timer_id,
    sctp_timer_buffer_st *p_tbuffer )
{
    sctp_U32bit     ep_id     = 0;
    sctp_U32bit     assoc_id  = 0;
    sctp_tcb_st     *p_ass = SCTP_NULL;

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "\nTimer processing ignored as stack not initialized." ) );
        return;
    }

    /*
     * If the event is for the DEQUE, then return from the function.
     */
#ifdef SCTP_NAME_RESOLV_THREAD
    sctp_process_cookie_packets();
#endif

    /* In the Handling of the Timer Expiry of dummy
     * timer the timer is retsrted
     * CSR 1-3217548
     */
#ifdef SCTP_CMN_RECV_POLL_THREAD

    if( p_tbuffer->event == SCTPEVENT_TIMER_DUMMY )
    {
        sctp_timer_t      dummy_timer_id;
        sctp_start_timer(
            SCTP_NULL,
            SCTPEVENT_TIMER_DUMMY,
            SIG_SELECT_TIMEOUT,
            0, &dummy_timer_id, SCTP_NULL );
        return;
    }

#endif

    if( p_tbuffer->event == SCTPEVENT_TIMER_DEQUE )
    {
        return;
    }

    /*  check for mac timer id */
    if( p_tbuffer->event == SCTPEVENT_TIMERMAC )
    {
        sctp_process_mac_timeout();

        return;
    }

    /*  check for pmtu db update timer id */
    if( p_tbuffer->event == SCTPEVENT_TIMERPMU_DB_UPDATE )
    {
        sctp_process_pmtu_dbupdate_timeout();

        return;
    }

    /* Check if its the select timer */
    if( p_tbuffer->event == SCTPEVENT_TIMER_SELECT )
    {
#ifdef SCTP_UI_SOCK_API
        sctp_handle_select_timeout( timer_id );
#endif /* SCTP_UI_SOCK_API */

        return;
    }

    /* SPR 5130 */
    /* no need to check this here */
    /*
     * Check that the association pointer is not NULL
     * Also we need to check if the the association still exists.
     */



    /*SPR 5067*/
    ep_id    = p_tbuffer->ep_id;
    assoc_id = p_tbuffer->assoc_id;
    /* Lock the Endpoint */
    SCTP_LOCK_EP( ep_id );
    p_ass = sctp_db_get_association_ptr( ep_id, assoc_id );

    if( SCTP_NULL == p_ass || !p_ass->open )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_process_timer_expiry: Invalid Association: Timer id "
                      "= %u\n", timer_id ) );

        SCTP_UNLOCK_EP( ep_id );
        return;
    }

    /*SPR 5067*/

    SCTP_TRACE( p_ass->p_ep, SCTP_INTF_TRC,
                ( "sctp_process_timer_expiry:timer Id = %u, Event Id = %u",
                  timer_id, p_tbuffer->event ) );

    p_ass->event.evt_timeout.addr_index = p_tbuffer->addr_index;

    sctp_assoc_state_machine( p_ass, p_tbuffer->event );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( ep_id );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( timer_id );
    /*to avoid the warning*/


    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_icmpV4_message
 *
 *    DESCRIPTION:
 *        This is an entry level function for processing data read from icmp
 *        socket.
 *        The logic is:
 *                      After the association establishment send an an icmp
 *                      echo request message. Now in this function we check
 *                      for the reply . The reply may be an icmp echo reply
 *                      whcih if valid and for one of the existing association
 *                      then we are done and pmtu discovery is successful. If
 *                      an icmp packet with type ICMP_UNREACH is received then
 *                      it will be only processed if its code is
 *                      ICMP_UNREACH_NEEDFRAG which indicates that the pmtu
 *                      packet that we had sent is too big to be carried by
 *                      the underlaying physical medium, hence in that case we
 *                      check the validity of that packet, and send
 *                      another pmtu message with smaller length. After
 *                      reception of every packet, its checksum is validated
 *                      and incase of invalid checksum, we call the
 *                      sctp_error_repoter so that application can take
 *                      necessarry decession about the PMTU discovery process.
 *
 *    RETURN VALUE:
 *       None
**************************************************************************/
void
sctp_process_icmpV4_message(
    sctp_U8bit       *buffer,
    sctp_U32bit      size,
    sctp_sockaddr_st *p_from )
{
    sctp_return_t    ret;
    sctp_tcb_st      *p_ass = SCTP_NULL;
    sctp_U32bit      addr_index;
    sctp_U16bit      iphdr_len;
    sctp_U16bit      old_iphdr_len;
    sctp_U32bit      last_pmtu_len = 0;
    sctp_U32bit      new_pmtu_len;
    SCTP_IP4         *ipheader;
    SCTP_IP4         *prev_ipheader;
    SCTP_ICMP4       *icmp_msg;
    sctp_U16bit      icmp_header_len = 8;

    sctp_U32bit      local_tag = 0;
    sctp_U16bit      that_port = 0;
    sctp_U16bit      this_port = 0;
    sctp_sockaddr_st src_ip_addr_from_header;
    sctp_sockaddr_st dest_ip_addr_from_header;
    sctp_pmtu_node_st *T;
    sctp_U32bit      pvt_offset = 0;
    sctp_U16bit      checksum_icmp = 0;
    sctp_U16bit      checksum_save = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "sctp_process_icmpV4_message, ICMP message from :" ) );

    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_from );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_from );
    /*to avoid the warning*/

    /* Check if size is big enough for valid icmp message */
    if( size < ( sizeof( SCTP_IP4 ) + icmp_header_len + 8 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_icmpV4_message: bad length" ) );

        return;
    }

    ipheader = ( SCTP_IP4 * ) buffer;
    iphdr_len = ipheader->ip_hl << 2;

    /*Extract ICMP Header*/
    icmp_msg = ( struct icmp * )( buffer + iphdr_len );

    /*Check if icmp packet is of our concern*/
    if( ( icmp_msg->icmp_type != ICMP_ECHOREPLY ) && ( icmp_msg->icmp_type != ICMP_UNREACH ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "\nsctp_process_icmpV4_message: Not an ICMP that we are looking for\n" ) );
        return;
    }

    if( ( icmp_msg->icmp_type == ICMP_UNREACH ) &&
            ( icmp_msg->icmp_code != ICMP_UNREACH_NEEDFRAG ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "\nsctp_process_icmpV4_message: Not an ICMP that we are looking for\n" ) );
        return;
    }

    /*Extract the dest address*/
    dest_ip_addr_from_header.family = SCTP_AF_INET;
    dest_ip_addr_from_header.ip.v4.addr  = ntohl( ipheader->ip_src.s_addr );

    /*Extract the src address*/
    src_ip_addr_from_header.family = SCTP_AF_INET;
    src_ip_addr_from_header.ip.v4.addr  = ntohl( ipheader->ip_dst.s_addr );

    ret = sctp_db_validate_bind_address_ext( 1, &src_ip_addr_from_header );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_process_icmpV4_message: Packet is dropped/invalid dst" ) );
        return;
    }

    if( icmp_msg->icmp_type == ICMP_ECHOREPLY )
    {
        /*In case of echo reply there is no old ip header*/
        pvt_offset = iphdr_len + sizeof( struct icmp );
    }

    else if( icmp_msg->icmp_type == ICMP_UNREACH )
    {
        if( size < ( sizeof( SCTP_IP4 ) + sizeof( SCTP_IP4 ) + icmp_header_len + 8 ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_process_icmpV4_message: bad length" ) );

            return;
        }

        /*Extract the old ip header*/
        prev_ipheader = ( SCTP_IP4 * )( buffer + iphdr_len + icmp_header_len );

        /*Get the length of old ip header*/
        old_iphdr_len = prev_ipheader->ip_hl << 2;

        pvt_offset = iphdr_len + sizeof( struct icmp ) + old_iphdr_len ;
    }

    /*Verify the checksum*/
    checksum_save = icmp_msg->icmp_cksum;
    icmp_msg->icmp_cksum = 0;
    checksum_icmp =  sctp_calc_icmpcksum( ( sctp_U16bit * )icmp_msg, size - iphdr_len );

    if( checksum_icmp != checksum_save )
    {
        /*May be Spooffed packet.Put some application reporter here*/
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "process_icmpv4:ICMP checksum mismatch.Packet dropped\n" ) );
        /*Call Error Repoter*/
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PMTUCHECKSUM_PARAMS , "Invalid ICMP Checksum" );
        return ;
    }

    /*Extract the private to find the assoc*/
    local_tag = sctp_buffer_get_UlongAtOffset( buffer + pvt_offset , 0 );
    that_port = sctp_buffer_get_UshortAtOffset( buffer + pvt_offset , 4 );
    this_port = sctp_buffer_get_UshortAtOffset( buffer + pvt_offset, 6 );

    /*Check if any value is not missing*/
    if( local_tag == 0 || that_port == 0 || this_port == 0 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_ICMPv4_message: Not finding required/"
                      "information. Droping ICMP \n" ) );
        return;
    }

    /*Find Out the association*/
    sctp_db_locate_association_verf(
        that_port,
        &( dest_ip_addr_from_header ),
        this_port,
        &( src_ip_addr_from_header ),
        local_tag,
        &p_ass );

    /*Can not find the association*/
    if( !p_ass )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_ICMPv4_message: for invalid Endpoint/"
                      "association\n" ) );

        return;
    }

    /*Search the pmtu node list to get last mtu size*/
    for( T = ( sctp_pmtu_node_st * )lstFirst( &mtu_activelist ); T;
            T = ( sctp_pmtu_node_st * )lstNext( ( NODE * )T ) )
    {
        if( sctp_cmp_addr( &T->addr, &( dest_ip_addr_from_header ) )
                == SCTP_FAILURE )
        {
            continue;
        }

        /*get the last mtu size mtu size*/
        last_pmtu_len = T->proposed_mtu_size ;
    }

    /*May be the reply message for a echo*/
    if( icmp_msg->icmp_type == ICMP_ECHOREPLY )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_ICMPV4_message: ICMP_UNREACH_PROTOCOL " ) );

        /* packet went through to host, pmtu detected, inform fsm */
        if( size != last_pmtu_len )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_process_icmpV4_message: bad length" ) );
            return;
        }

        if( ( p_ass != NULL )
                && ( sctp_db_validate_remote_addr( p_ass, &dest_ip_addr_from_header,
                                                   &addr_index ) == SCTP_SUCCESS ) )
        {
            p_ass->event.evt_pathmtu.addr_index = addr_index;
            p_ass->event.evt_pathmtu.path_mtu = last_pmtu_len;

            /* update pmtu database */
            sctp_set_dest_parameters( &dest_ip_addr_from_header,
                                      ( sctp_U16bit )last_pmtu_len );
            sctp_assoc_state_machine( p_ass, SCTPEVENT_EVTNEWPMTU );
        }
    }

    else
    {
        /*Must be the case of Don't Frag set*/
        new_pmtu_len = last_pmtu_len;
        /*Don't rely on the router information.Apply step down value*/
        ret = sctp_estimate_next_pmtu_V4( &new_pmtu_len );

        if( ret != SCTP_SUCCESS )
        {
            /*Unable to get the new pmtu value*/
            return;
        }

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_ICMPV4_message:"
                      "dest_addr = %x, new pmtu= %u, last pmtu= %u",
                      dest_ip_addr_from_header.ip.v4.addr, new_pmtu_len,
                      last_pmtu_len ) );

        sctp_send_pmtu_msg_v4( p_ass, new_pmtu_len, &dest_ip_addr_from_header );

    }

    return ;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_icmpV6_message
 *
 *    DESCRIPTION:
 *        This is an entry level function for processing data read from icmp
 *        socket for IP v6.
 *
 *    RETURN VALUE:
 *        None
**************************************************************************/
void
sctp_process_icmpV6_message(
    sctp_U8bit       *buffer,
    sctp_U32bit      size,
    sctp_sockaddr_st *p_from )
{


#ifdef SCTP_INET6_SUPPORT
    SCTP_IP6         *ip6header = SCTP_NULL;
    SCTP_IP6         *prev_ip6header;
    SCTP_ICMP6       *icmp6_msg  = SCTP_NULL;
    sctp_U32bit      old_pmtu;
    sctp_U32bit      new_pmtu;
    sctp_U16bit      tmp_pmtu;
    sctp_sockaddr_st dest_addr;
    sctp_U32bit      ip6hdr_len   = sizeof( SCTP_IP6 );
    sctp_U32bit      icmp6hdr_len = sizeof( SCTP_ICMP6 );
    sctp_U8bit       *data_offset = SCTP_NULL;
    sctp_tcb_st      *p_ass = SCTP_NULL;
    sctp_assoc_t     assoc_id;
    sctp_U32bit      ep_id = SCTP_INVALID_ENDPOINT;
    /*Bug Id 22 fix starts*/
    /*sctp_return_t    ret = 0;*/
    sctp_return_t    ret = SCTP_FAILURE;
    /*Bug Id 22 fix ends*/
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( size );
    SCTP_UNUSED_VARIABLE( p_from );
    /*to avoid the warning*/

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "sctp_process_ICMPV6_message: from\n" ) );
    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, p_from );

    /* Set the destination address */
    dest_addr.family = SCTP_AF_INET6;

    /*
     * check if the size is less than the minimum if yes then reject the msg.
     */
    if( size < ( ip6hdr_len * 2 + icmp6hdr_len + 28 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_icmpV6_message: Size is less than minimum size " ) );

        return;
    }

    /* Get a pointer to the IPv6 header */
    ip6header = ( SCTP_IP6 * ) buffer;

    /* If the next header is not ICMP6 then reject the message */
    if( SCTP_IP6_NXTHDR_TYPE_ICMP6 != ip6header->ip6_nxt )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_icmpV6_message: The next hdr is not ICMP6" ) );

        return;
    }

    /*
     * Get the index into the next header.
     */
    icmp6_msg = ( SCTP_ICMP6 * )( buffer + ip6hdr_len );

    /* The ICMP message is "Packet too BIG" */
    if( ICMP6_PACKET_TOO_BIG == icmp6_msg->icmp6_type )
    {
        /* Extract the new MTU field */
        new_pmtu = SCTP_NTOHL( icmp6_msg->icmp6_data32[0] );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_ICMPV6_message: new path mtu =  %u", new_pmtu ) );

        /* This is original ip header sent */
        prev_ip6header = ( SCTP_IP6 * )( buffer + ip6hdr_len +
                                         icmp6hdr_len );

        /* Extract the last PMTU length that was sent out */
        old_pmtu = SCTP_NTOHL( prev_ip6header->ip6_plen );

        /* The offset for sctp specific data */
        data_offset = buffer + ip6hdr_len * 2 + icmp6hdr_len;

        /* Extract the Endpoint id from the buffer */
        ep_id = sctp_buffer_get_UlongAtOffset( data_offset, 0 );

        /* Extract the association id in the endpoint */
        assoc_id = sctp_buffer_get_UlongAtOffset( data_offset, 4 );

        /* Extract the IP V6 address */
        dest_addr.ip.v6.addr.s_u6_addr32[0] =
            sctp_buffer_get_UlongAtOffset( data_offset, 8 );
        dest_addr.ip.v6.addr.s_u6_addr32[0] =
            sctp_buffer_get_UlongAtOffset( data_offset, 12 );
        dest_addr.ip.v6.addr.s_u6_addr32[0] =
            sctp_buffer_get_UlongAtOffset( data_offset, 16 );
        dest_addr.ip.v6.addr.s_u6_addr32[0] =
            sctp_buffer_get_UlongAtOffset( data_offset, 20 );

        /* Get the pointer to the tcb */
        p_ass = sctp_db_get_association_ptr( ep_id, assoc_id );

        if( !p_ass )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_process_ICMPv6_message:invalid Endpoint/association\n" ) );

            return;
        }

        /* Lock the endpoint */
        SCTP_LOCK_EP( ep_id );
        /* Get the pointer to the tcb again*/
        p_ass = sctp_db_get_association_ptr( ep_id, assoc_id );

        if( !p_ass )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_process_ICMPv4_message: for invalid Endpoint/"
                          "association\n" ) );
            SCTP_UNLOCK_EP( ep_id );
            return;
        }

        tmp_pmtu = new_pmtu;

        /* The Code parameter in the ICMP6 packet is ignored */

        /* Get the next plausible pmtu */
        ret = sctp_estimate_next_pmtu_V6( &tmp_pmtu );

        if( ret != SCTP_SUCCESS )
        {
            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( ep_id );

            return;
        }

        new_pmtu = tmp_pmtu;

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_process_ICMPV6_message: ep_id = %u, assoc_id = %u, "
                      " new pmtu= %d, last pmtu= %d, Dest address = ", ep_id,
                      assoc_id, new_pmtu, old_pmtu ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_INTF_TRC, &dest_addr );

        /* Send a PMTU discovery message with the next lower MTU. */
        sctp_send_pmtu_msg_v6( p_ass, new_pmtu, &dest_addr );
    } /* If loop */

    /* Unlock the endpoint if it was locked */
    SCTP_UNLOCK_EP( ep_id );

#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buffer );
    SCTP_UNUSED_VARIABLE( size );
    SCTP_UNUSED_VARIABLE( p_from );
    /*to avoid the warning*/

    return;
}


#ifdef SCTP_NAME_RESOLV_THREAD

/***************************************************************************
 *    FUNCTION :
 *        sctp_process_cookie_packets
 *
 *    DESCRIPTION:
 *        This function is called from the main receiving thread in a loop.
 *        This function checks if the queue containing cookie packets has
 *        some packets to process. If queue is empty, it returns immediately
 *        otherwise it processes the cookie and data chunks bundled within the
          packet if any.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_process_cookie_packets( void )
{
    sctp_TL_packet_st *packet;
    SCTP_IP4          *ipheader;
    sctp_U32bit       ip_header_offset;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_process_cookie_packets" ) );
    /*Get the packet containing cookie from the queue*/
    SCTP_RESOLVER_LOCK();

    if( lstCount( &res_to_recv_queue ) == 0 )
    {
        /* No packet containing cookie present */
        SCTP_RESOLVER_UNLOCK();

        return;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_process_cookie_packets" ) );

    /* Get the cookie packet */
    packet = ( sctp_TL_packet_st * )lstGet( &res_to_recv_queue );

    /* Release the cookie processing function lock */
    SCTP_RESOLVER_UNLOCK();

    if( SCTP_IS_STACK_INET )
    {
        ipheader = ( SCTP_IP4 * ) & ( ( packet->buffer )[0] );

        /* calculate ip header offset */
        ip_header_offset = 4 * ipheader->ip_hl;
    }

    else
    {
        ip_header_offset = 0;
    }

    sctp_process_TL_datagram(
        ip_header_offset,
        packet,
        packet->packet_len,
        SCTP_TRUE );
}

#endif /* SCTP_NAME_RESOLV_THREAD */



