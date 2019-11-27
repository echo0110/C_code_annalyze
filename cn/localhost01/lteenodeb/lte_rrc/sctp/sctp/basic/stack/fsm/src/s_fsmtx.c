/***************************************************************************
 *  FILE NAME  : s_fsmtx.c
 *
 *  DESCRIPTION: Contains the Functions for the Transmit State Machine.
 *
 *  DATE       NAME                   REF#         REASON
 *  -----------------------------------------------------------------------
 *  02Jun00    Samahajan              -            .Create original
 *  16Aug01    gsheoran               -            REL 3.0 Update
 *  20May02    ygahlaut             SPR 4033 Fix
 *  02Nov04    Svakumar             REL 6.0        SCTP/FUNC-CONGCON-5
 *  07jul08    Shikha Kathpal       REL 6.2.0    RFC 4960 UPGRADE
 *  21may10    Shivani Mehrotra     SPR 20408    Fix for SCTP Abort is sent when SACK is
 *                                               received with gaps for TSNs between 2^31 and 2^32
 *  12july10   Sharad Kumar Yadav   SPR 20526    Fix for CSR 1-7670333
 *  27Aug13    Rohan Pawa   CSR 51625      SPR 21147
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "FSM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_osmutx.h>
#include <s_ostmr.h>
#include <s_mhbld.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_fsmtbl.h>
#include <s_fsmcng.h>
#include <s_fsmdat.h>
#include <s_uisuif.h>
#include <s_cobuf.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_fsmtx.h>
#include <s_dbutil.h>
#include <s_fsmass.h>
#include <s_uiutil.h>


static sctp_return_t
sctp_process_param_fraglist(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    *packet_loss,
    sctp_U32bit    *total_bytes_acked_list,
    sctp_U32bit    num_cumacks,
    sctp_error_t   *p_error );


/***************************************************************************
 *    FUNCTION :
 *        sctp_tx_state_machine
 *
 *    DESCRIPTION:
 *        This function extracts the destination State from association
 *    structure (based on association id and destination address).
 *    From the State Machine Table determine which function should
 *    handle this event and state pair and call that function.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_tx_state_machine(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    event,
    sctp_U32bit    addr_index )
{
    sctp_U32bit state = p_ass->transmit.remote[addr_index].state;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "Event for TX FSM,Addr_index = %u, Event=%u,state =%u",
                  addr_index, event, p_ass->transmit.remote[addr_index].state ) );

    if( sctp_tx_fsm_ftbl[event][state] != NULL )
    {
        ( *sctp_tx_fsm_ftbl[event][state] )( p_ass, addr_index );
    }

    else
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "Invalid Event For Tx FSM, Addr_index=%u,Event=%u,state=%u",
                      addr_index, event, p_ass->transmit.remote[addr_index].state ) );

        SCTP_ERROR( SCTPERROR_FLAG_PROTOCOLERROR, SCTPERROR_LEVEL_CRITICAL,
                    E_INVALID_EVENT_FOR_STATE, "Inv Event For Tx FSM, Association Id" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_PROTOCOL_ERRORS, 1 );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_outstanding_list
 *
 *    DESCRIPTION:
 *        This funcations copies the outstanding data of each address.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_get_outstanding_list(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    *p_outstanding_list )
{
    sctp_U32bit counter;

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        p_outstanding_list[counter] =
            p_ass->transmit.remote[counter].outstanding;
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_is_tsn_present
 *
 *    DESCRIPTION:This function checks if tsn is present.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_is_tsn_present(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    tsn )
{
    sctpbuffer_st *p_buf = ( sctpbuffer_st * )

                           lstFirst( &p_ass->transmit.transmit_queue );

    for( ; ( p_buf != NULL ); p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * )p_buf ) )
    {
        if( p_buf->tsn == tsn )
        {
            return ( SCTP_SUCCESS );
        }
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_lowest_tsn_on_addr
 *
 *    DESCRIPTION:This function gets lowest tsn
 *
 *    RETURN VALUE:
 *       SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_get_lowest_tsn_on_addr(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index,
    sctp_U32bit    *lowest_tsn )
{
    sctpbuffer_st *p_buf = ( sctpbuffer_st * )
                           lstFirst( &p_ass->transmit.transmit_queue );

    for( ; ( p_buf != NULL ); p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * )p_buf ) )
    {

        if( ( p_buf->dest_addr_index == addr_index ) && ( p_buf->sent == 1 ) )
        {
            *lowest_tsn = p_buf->tsn;

            return ( SCTP_SUCCESS );
        }
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_update_rto
 *
 *    DESCRIPTION:This function updates rto.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_update_rto(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index,
    sctp_U32bit    rtt )
{
    sctp_S32bit variation;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_update_rto, addr_index = %u, rtt =%u", addr_index , rtt ) );

    switch( p_ass->transmit.remote[addr_index].count )
    {
        case 0:
            p_ass->transmit.remote[addr_index].count++;
            p_ass->transmit.remote[addr_index].srtt = rtt;
            p_ass->transmit.remote[addr_index].rttvar = rtt / 2;

            /* Start SPR 20526 */
            p_ass->transmit.remote[addr_index].min_rtt = rtt;
            p_ass->transmit.remote[addr_index].max_rtt = rtt;
            p_ass->transmit.remote[addr_index].last_recorded_rtt = rtt;
            /* End SPR 20526 */

            if( p_ass->transmit.remote[addr_index].rttvar == 0 )
            {
                p_ass->transmit.remote[addr_index].rttvar = 1;
            }

            p_ass->transmit.remote[addr_index].rto =
                p_ass->transmit.remote[addr_index].srtt
                + ( 4 * p_ass->transmit.remote[addr_index].rttvar );

            if( p_ass->transmit.remote[addr_index].rto
                    < p_ass->config.rto_min )
            {
                p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_min;
            }

            if( p_ass->transmit.remote[addr_index].rto
                    > p_ass->config.rto_max )
            {
                p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
            }

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctp_update_rto: First RTT calculation, rttvar = %u, "
                          "srtt=%u,rto=%u", p_ass->transmit.remote[addr_index].rttvar,
                          p_ass->transmit.remote[addr_index].srtt,
                          p_ass->transmit.remote[addr_index].rto ) );

            break;

        default:

            /* Start SPR 20526 */
            if( p_ass->transmit.remote[addr_index].min_rtt > rtt
                    || p_ass->transmit.remote[addr_index].min_rtt == 0 )
            {
                p_ass->transmit.remote[addr_index].min_rtt = rtt;
            }

            if( p_ass->transmit.remote[addr_index].max_rtt  < rtt
                    || p_ass->transmit.remote[addr_index].max_rtt == 0 )
            {
                p_ass->transmit.remote[addr_index].max_rtt = rtt;
            }

            p_ass->transmit.remote[addr_index].last_recorded_rtt = rtt;
            /* End SPR 20526 */

            variation = ( p_ass->transmit.remote[addr_index].srtt - rtt );

            if( variation < 0 )
            {
                variation = -variation;
            }

            p_ass->transmit.remote[addr_index].rttvar =
                ( 100 - p_ass->config.rto_beta_percent ) *
                p_ass->transmit.remote[addr_index].rttvar / 100 +
                p_ass->config.rto_beta_percent * variation / 100;

            if( p_ass->transmit.remote[addr_index].rttvar == 0 )
            {
                p_ass->transmit.remote[addr_index].rttvar = 1;
            }

            /* SPR FIx 20295 Starts */
            p_ass->transmit.remote[addr_index].srtt =
                ( ( 100 - p_ass->config.rto_alpha_percent ) *
                  p_ass->transmit.remote[addr_index].srtt +
                  p_ass->config.rto_alpha_percent * rtt ) / 100;
            /* SPR Fix 20295 Ends */
            p_ass->transmit.remote[addr_index].rto
                = p_ass->transmit.remote[addr_index].srtt +
                  ( 4 * p_ass->transmit.remote[addr_index].rttvar );

            if( p_ass->transmit.remote[addr_index].rto < p_ass->config.rto_min )
            {
                p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_min;
            }

            if( p_ass->transmit.remote[addr_index].rto > p_ass->config.rto_max )
            {
                p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
            }

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctp_update_rto: RTT calculation, variation=%d, "
                          "rttvar=%u,srtt=%u,rto=%u",
                          variation, p_ass->transmit.remote[addr_index].rttvar,
                          p_ass->transmit.remote[addr_index].srtt,
                          p_ass->transmit.remote[addr_index].rto ) );

            break;
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_next_active_addr_index
 *
 *    DESCRIPTION:This function gets next active address index.
 *
 *    RETURN VALUE:
 *        Returns address index (sctp_index)
 **************************************************************************/
sctp_U32bit
sctp_get_next_active_addr_index(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    last_addr_index )
{
    sctp_U32bit sctp_index, counter;

    sctp_index = last_addr_index;

    for( counter = 0;
            counter < p_ass->transmit.num_remote_address - 1; counter ++ )
    {
        sctp_index++;

        if( sctp_index == p_ass->transmit.num_remote_address )
        {
            sctp_index = 0;
        }

        /* Check that the Communication is UP and the Address is also marked Confirmed. */
        if( p_ass->transmit.remote[sctp_index].status == SCTP_COMMUNICATION_UP &&
                p_ass->transmit.remote[sctp_index].is_confirmed == SCTP_TRUE )
        {
            break;
        }
    }

    /* send data on same address */
    if( counter == p_ass->transmit.num_remote_address - 1 )
    {
        sctp_index = last_addr_index + 1;

        if( sctp_index == p_ass->transmit.num_remote_address )
        {
            sctp_index = 0;
        }
    }

    return ( sctp_index );
}

/* Changes for CSR#1-4777901 */
/***************************************************************************
 *    FUNCTION :
 *        sctp_get_possible_next_active_addr_index
 *
 *    DESCRIPTION: This function gets next possible active address index
 *
 *    RETURN VALUE:
 *        Returns address index (sctp_index)
 **************************************************************************/
sctp_U32bit
sctp_get_possible_next_active_addr_index(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    last_addr_index )
{
    sctp_U32bit sctp_index, counter;

    sctp_index = last_addr_index;

    for( counter = 0;
            counter < p_ass->transmit.num_remote_address - 1; counter ++ )
    {
        sctp_index++;

        if( sctp_index == p_ass->transmit.num_remote_address )
        {
            sctp_index = 0;
        }

        /* Check that the Communication is UP and the Address is also marked Confirmed. */
        if( p_ass->transmit.remote[sctp_index].status == SCTP_COMMUNICATION_UP &&
                p_ass->transmit.remote[sctp_index].is_confirmed == SCTP_TRUE )
        {
            break;
        }
    }

    /* send data on same address */
    if( counter == p_ass->transmit.num_remote_address - 1 )
    {
        sctp_index++;

        if( sctp_index == p_ass->transmit.num_remote_address )
        {
            sctp_index = 0;
        }
    }

    return ( sctp_index );
}
/* Changes for CSR#1-4777901 Ends*/

/***************************************************************************
 *    FUNCTION :
 *       sctp_get_next_saddr_index_with_af
 *
 *    DESCRIPTION:
 *        For the address family specified gets the best suitable source
 *    address index.
 *
 *    RETURN VALUE:
 *        SCTP_FAILURE if there is no source address available corresponding
 *    to the address family.
 *************************************************************************/
sctp_return_t
sctp_get_next_saddr_index_with_af(
    sctp_tcb_st           *p_tcb,
    sctp_U32bit           *p_src_pri, /* primary source address  */
    sctp_sa_family_t      af,         /* Address Family */
    sctp_error_t          *p_ecode )
{
    sctp_U32bit      count = 0;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_get_next_saddr_index_with_af :famliy = %d", af ) );

    ( *p_src_pri )++;

    if( ( *p_src_pri )++ == p_tcb->p_ep->naddr )
    {
        *p_src_pri = 0;
    }

    /* Else find the first suitable source address */
    for( count = *p_src_pri; count < p_tcb->p_ep->naddr; count++ )
    {
        if( af == p_tcb->p_ep->this_address[count].family )
        {
            *p_src_pri = count;
            return ( SCTP_SUCCESS );
        }
    }

    for( count = 0; count < *p_src_pri; count++ )
    {
        if( af == p_tcb->p_ep->this_address[count].family )
        {
            *p_src_pri = count;
            return ( SCTP_SUCCESS );
        }
    }

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                ( "sctp_get_next_saddr_index_with_af: Could not find source address with "
                  "the same family as the destination address " ) );

    *p_ecode = E_ADDR_TYPE_MISMATCH;

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_update_rwnd_retransflag_ontimeout
 *
 *    DESCRIPTION:This function updates retrasmission flag on timeout
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_update_rwnd_retransflag_ontimeout(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    LIST              *queue = &p_ass->transmit.transmit_queue;
    sctpbuffer_st     *p_buf;

    p_buf = ( sctpbuffer_st * )lstFirst( queue );

    for( ; p_buf; p_buf = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf ) )
    {

        if( ( p_buf->sent == 1 ) && ( addr_index == p_buf->dest_addr_index ) )
        {
            if( ( p_buf->retrans_flag == SCTP_RETRANSFLAG_NONE ) ||
                    ( p_buf->retrans_flag == SCTP_RETRANSFLAG_RETRANSMITTED ) )
            {
                p_ass->transmit.rwndthere += p_buf->size;
                /* Changes For CSR#1-5037386 */
                p_ass->transmit.remote[addr_index].retrans_packet_count++;
                /* Changes For CSR#1-5037386 Ends */
            }

            /* Mark for retransmission */
            p_buf->retrans_flag = SCTP_RETRANSFLAG_MARK;
        }
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_param_cumtsn
 *
 *    DESCRIPTION:This processes cumulative tsn parameter.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_process_param_cumtsn(
    sctp_tcb_st            *p_ass,
    sctp_U32bit            *num_cumacks,
    sctp_U32bit            *total_bytes_acked_list,
    sctp_error_t           *p_ecode )
{
    sctp_U32bit   sctp_index;
    sctp_U32bit   rtt = 0;
    sctp_U32bit      old_cong_reg       = 0;
    sctp_U32bit      new_cong_reg       = 0;
    sctp_U32bit   cumtsn = p_ass->event.msg_sack.sack_chunk->cumtsn;
    sctpbuffer_st *next;
    sctpbuffer_st *p_buf
        = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );


    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_process_param_cumtsn:, Cumtsn = %u",
                  p_ass->event.msg_sack.sack_chunk->cumtsn ) );

    /* verify that cumtsn is greater than lowest tsn */
    if( !p_buf )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_param_cumtsn: buffer is NULL" ) );

        *p_ecode = E_NOERROR;

        return ( SCTP_FAILURE );
    }

    if( sctp_compare_tsn_for_smaller( cumtsn, p_buf->tsn - 1 ) == SCTP_SUCCESS )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_param_cumtsn: Invalid Cumtsn Received= %u",
                      p_ass->event.msg_sack.sack_chunk->cumtsn ) );

        *p_ecode = E_TMP_ECODE_FOR_SACK;

        return ( SCTP_FAILURE );
    }

    /* Verify that cumtsn is not greater than the Largest TSN sent so far */
    if( sctp_compare_tsn_for_smaller( cumtsn,
                                      p_ass->transmit.highest_tsn_sent + 1 ) == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC | SCTP_DBG_TRC,
                    ( "sctp_process_param_cumtsn: Invalid Cumtsn Received = %u, "
                      "Highest TSN Sent = %u ",
                      p_ass->event.msg_sack.sack_chunk->cumtsn,
                      p_ass->transmit.highest_tsn_sent ) );

        *p_ecode = E_INVALID_TSN;

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->transmit.fast_recovery_exit != 0 ) &&
            ( SCTP_SUCCESS == sctp_compare_tsn_for_larger( cumtsn,
                                                           p_ass->transmit.fast_recovery_exit - 1 ) ) )
    {
        SCTP_TRACE( p_ass->p_ep,  SCTP_DBG_TRC,
                    ( "sctp_process_param_cumtsn: Exit Fast Recovery" ) );
        p_ass->transmit.fast_recovery_exit = 0;
    }

    /* Processing the acknowledgements that advance the CUMTSN*/
    while( ( p_buf != NULL ) && ( p_buf->sent == 1 )
            && ( ( p_buf->tsn == cumtsn ) ||
                 ( sctp_compare_tsn_for_smaller( p_buf->tsn, cumtsn ) == SCTP_SUCCESS ) ) )
    {
        next = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf );

        /*
         * Use Destination Address index instead.
         */

        sctp_index = p_buf->dest_addr_index;

        p_ass->transmit.remote[sctp_index].num_chunks_await_ack--;
        ( *num_cumacks )++;

        if( p_buf->ack_flag == SCTP_ACKFLAG_UNACKED )
        {
            total_bytes_acked_list[sctp_index] += p_buf->size;

            /* update outstanding octets params */
            p_ass->transmit.remote[sctp_index].outstanding -= p_buf->size;

            p_ass->transmit.total_outstanding -= p_buf->size;
        }

        /* stop timer and update rtt based on last timer value */
        if( p_buf->tsn == p_ass->transmit.remote[sctp_index].lowest_tsn )
        {
            sctp_stop_timer(
                &p_ass->transmit.remote[sctp_index].dest_timer_id[SCTP_DESTTIMER_RTX],
                &rtt,  /* time since timer started */
                SCTP_NULL );

            if( p_buf->retrans_flag == SCTP_RETRANSFLAG_NONE )
            {
                /* CSR 1-2420144 */
                if( rtt <= p_ass->transmit.remote[sctp_index].rto )
                {
                    rtt = p_ass->transmit.remote[sctp_index].rto - rtt;
                }

                else
                {
                    rtt = 0;
                }

                sctp_update_rto( p_ass, sctp_index, rtt );
            }
        }

        lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )p_buf );
        sctp_free_transmit_buffer( p_ass, p_buf );

        p_buf = next;
    }

    if( SCTP_SUCCESS == sctp_calc_trx_cong_reg( p_ass, &new_cong_reg ) )
    {
        old_cong_reg = p_ass->transmit.congestion_event_sent;

        /*
         * Check whether we have new congestion. And if so then give congestion
         * indication to the user.
         */
        if( new_cong_reg != old_cong_reg )
        {
            p_ass->transmit.congestion_event_sent = new_cong_reg;

            /* notify su of change in congestion level */
            sctp_ntfy_congestion( p_ass, new_cong_reg );
        }
    }

    if( p_buf != NULL )
    {
        /* SPR 21147 changes starts */
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "TSN of head of the queue is %u", p_buf->tsn ) );
        /* SPR 21147 changes ends */
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_param_fraglist
 *
 *    DESCRIPTION:This function processes frag list parameter
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_process_param_fraglist(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    *packet_loss,
    sctp_U32bit    *total_bytes_acked_list,
    sctp_U32bit    num_cumacks,
    sctp_error_t   *p_ecode )
{
    sctp_U16bit   first;
    sctp_U16bit   last;
    sctp_U32bit   counter;
    sctp_U32bit   sctp_index;
    sctp_U32bit   segment_last;
    sctp_U32bit   cumtsn = p_ass->event.msg_sack.sack_chunk->cumtsn;
    sctpbuffer_st *p_buf = SCTP_NULL;
    sctpbuffer_st *p_last_missed = SCTP_NULL;
    sctp_U8bit    *P = p_ass->event.msg_sack.sack_chunk->frags;
    sctp_U32bit   highest_tsn = 0, highest_unacked = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_process_param_fraglist: Num Frags=%d",
                  p_ass->event.msg_sack.sack_chunk->numfrags ) );

    segment_last = 0;

    /* Processing the the acknowledgement fragments
    * (that do not advance the CUMTSN)
    */
    for( counter = 0; counter < p_ass->event.msg_sack.sack_chunk->numfrags;
            counter++ )
    {
        P = sctp_buffer_get_Ushort( P, &first );
        P = sctp_buffer_get_Ushort( P, &last );

        /* 2 should be minimum first offset*/
        if( ( first > last ) || ( first < 2 ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_PROTOCOLERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INVALID_SACK_CHUNK, "sctp_process_param_fraglist: Bad Sack"
                        " Chunk,Invalid Fragment Combination" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_PROTOCOL_ERRORS, 1 );

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctp_process_param_fraglist: Invalid Fragment, skipped, "
                          "first:%d, last:%d", first, last ) );

            /*   Skip this fragment only */
            continue;
        }

        /* Verify that first and Last are not greater than the Largest TSN sent so far */
        /*SPR 20408 Starts*/
        if( ( sctp_compare_tsn_for_smaller( first + cumtsn,
                                            p_ass->transmit.highest_tsn_sent + 1 ) == SCTP_FAILURE ) ||
                ( sctp_compare_tsn_for_smaller( last + cumtsn,
                                                /*SPR 20408 Ends*/
                                                p_ass->transmit.highest_tsn_sent + 1 ) == SCTP_FAILURE ) )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC | SCTP_DBG_TRC,
                        ( "sctp_process_param_cumtsn: Invalid GAP ACK : FIRST = %u, LAST = %u, "
                          "Highest TSN Sent = %u ",
                          first, last,
                          p_ass->transmit.highest_tsn_sent ) );

            *p_ecode = E_INVALID_TSN;

            return ( SCTP_FAILURE );
        }

        /* Processing fragment */

        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_process_param_fraglist: Processing fragment, Start:%d, "
                      "End:%d", first, last ) );

        p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

        while( p_buf )
        {

            sctp_index = p_buf->dest_addr_index;

            /* Check if tsn lies within fragment limit */
            if( ( ( ( p_buf->tsn == cumtsn + first )
                    || ( sctp_compare_tsn_for_larger( p_buf->tsn, cumtsn + first ) ==
                         SCTP_SUCCESS ) ) && ( ( p_buf->tsn == cumtsn + last )
                                               || ( sctp_compare_tsn_for_smaller( p_buf->tsn, cumtsn + last ) ==
                                                    SCTP_SUCCESS ) ) ) && ( p_buf->ack_flag == SCTP_ACKFLAG_UNACKED ) )
            {
                /*  mark it as acked by gap blocks */
                p_buf->ack_flag = SCTP_ACKFLAG_ACKBYGAPBLOCK;

                total_bytes_acked_list[sctp_index] += p_buf->size;

                p_ass->transmit.remote[sctp_index].outstanding -= p_buf->size;

                p_ass->transmit.total_outstanding -= p_buf->size;

                highest_tsn = p_buf->tsn;
            }

            else  if( ( sctp_compare_tsn_for_larger( p_buf->tsn, cumtsn + segment_last )
                        == SCTP_SUCCESS )
                      && ( sctp_compare_tsn_for_smaller( p_buf->tsn, cumtsn + first )
                           == SCTP_SUCCESS ) )
            {
                if( p_buf->ack_flag == SCTP_ACKFLAG_UNACKED )
                {
                    p_buf->missed++;

                    /* rfc 4960 upgrade  fix start */
                    if( p_buf->missed >= SCTP_FAST_RETRANS_THRSHOLD )
                    {
                        /* Mark it for retransmision ... */
                        /* Changes For CSR#1-5037386 */
                        if( p_buf->retrans_flag != SCTP_RETRANSFLAG_MARK )
                        {
                            p_buf->retrans_flag = SCTP_RETRANSFLAG_MARK;
                            p_ass->transmit.remote[sctp_index].retrans_packet_count++;
                        }

                        /* Changes For CSR#1-5037386 Ends */
                        packet_loss[sctp_index]++;
                        p_ass->transmit.rwndthere += p_buf->size;
                    }

                    /* rfc 4960 upgrade fix end */
                    p_last_missed = p_buf;
                }

                else
                {
                    /*  Sender dropped previously acked data ..  */
                    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                ( "sctp_process_param_fraglist: Sender dropped "
                                  "previously acked data, tsn:%u", p_buf->tsn ) );

                    /*  actions ...  */
                    p_buf->ack_flag = SCTP_ACKFLAG_UNACKED;
                    p_buf->missed = 1;

                    p_ass->transmit.remote[sctp_index].outstanding += p_buf->size;

                    p_ass->transmit.total_outstanding += p_buf->size;
                }

                highest_unacked = p_buf->tsn;
            }

            p_buf = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf );
        }

        segment_last = last;
    } /* for loop */

    if( highest_tsn == p_ass->transmit.highest_outstanding )
    {
        p_ass->transmit.highest_outstanding = highest_unacked;
    }

    if( ( p_ass->transmit.fast_recovery_exit == 0 ) || /*not in fast recovery*/
            ( num_cumacks == 0 ) )
    {
        /* Follow the HTNA algo*/
        p_buf = p_last_missed;

        while( p_buf )
        {
            if( ( p_buf->ack_flag == SCTP_ACKFLAG_UNACKED ) &&
                    ( SCTP_SUCCESS == sctp_compare_tsn_for_larger( p_buf->tsn,
                                                                   highest_tsn ) ) )
            {
                /* miss count should not have ben incremented for this one */
                /* rfc 4960 upgrade  fix start */
                if( p_buf->missed == SCTP_FAST_RETRANS_THRSHOLD )
                {


                    sctp_index = p_buf->dest_addr_index;

                    /* Changes For CSR#1-5037386 */
                    if( p_buf->retrans_flag != SCTP_RETRANSFLAG_MARK )
                    {
                        p_buf->retrans_flag = SCTP_RETRANSFLAG_MARK;
                        p_ass->transmit.remote[sctp_index].retrans_packet_count++;
                    }

                    /* Changes For CSR#1-5037386 Ends */
                    packet_loss[sctp_index]--;
                    p_ass->transmit.rwndthere += p_buf->size;

                }

                /* rfc 4960 upgrade fix end */
                p_buf->missed--;
            }

            p_buf = ( sctpbuffer_st * )lstPrevious( ( NODE * )p_buf );
        }
    }

    return SCTP_SUCCESS;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_txfsm_Gener_Sack
 *
 *    DESCRIPTION: Handles Tx event SACK
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_txfsm_Gener_Sack(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_remote_addr_param_st *dest_params;
    sctp_U32bit               counter;
    sctp_U32bit               lowest_tsn;
    sctp_U32bit               count;
    sctp_U32bit               num_cumacks = 0;
    sctp_error_t              ecode;
    sctp_U32bit               total_bytes_acked_list[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U32bit               packet_loss[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U32bit               outstanding_list[SCTP_MAX_TRANSPORT_ADDR];

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_txfsm_Gener_Sack: addr_index:%u", addr_index ) );


    __MEMSET( total_bytes_acked_list, 0,
              SCTP_MAX_TRANSPORT_ADDR * sizeof( sctp_U32bit ) );

    __MEMSET( packet_loss, 0,
              SCTP_MAX_TRANSPORT_ADDR * sizeof( sctp_U32bit ) );

    /* Get the outstanding list prior to processing sack  */
    sctp_get_outstanding_list( p_ass, &outstanding_list[0] );

    /* process cumtsn*/
    if( sctp_process_param_cumtsn( p_ass, &num_cumacks,
                                   &total_bytes_acked_list[0], &ecode ) == SCTP_FAILURE )
    {
        if( ecode == E_TMP_ECODE_FOR_SACK )
        {
            SCTP_ERROR( SCTPERROR_FLAG_PROTOCOLERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "sctp_txfsm_Gener_Sack: Invald cumtsn in Sack, May be "
                        "out of order Sack" );
        }

        if( ecode == E_INVALID_TSN )
        {
            /* As per RFC 4960 Section 11.4

               "An SCTP implementation SHOULD abort the association if it receives a
                SACK acknowledging a TSN that has not been sent"
            */

            /* Get the source address index corresponding to the address family. */
            sctp_U32bit saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

            /* send abort and clean up the association */
            sctp_send_chunk_abort( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &( p_ass->transmit.remote[addr_index].that_address ),
                                   p_ass->remote_tag, SCTP_FALSE, 0, 0, NULL, 0,
                                   p_ass->config.checksum_func );

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

            sctp_generic_handle_abort_message( p_ass );

            return;
        }
    }

    /* process fraglist */
    if( sctp_process_param_fraglist( p_ass, &packet_loss[0],
                                     &total_bytes_acked_list[0], num_cumacks, &ecode ) == SCTP_FAILURE )
    {
        if( ecode == E_INVALID_TSN )
        {
            /* Get the source address index corresponding to the address family. */
            sctp_U32bit saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

            /* send abort and clean up the association */
            sctp_send_chunk_abort( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &( p_ass->transmit.remote[addr_index].that_address ),
                                   p_ass->remote_tag, SCTP_FALSE, 0, 0, NULL, 0,
                                   p_ass->config.checksum_func );

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

            sctp_generic_handle_abort_message( p_ass );

            return;
        }
    }

    /* update rwnd parameter , 6.2.1 , C */
    count = p_ass->total_sent;

    /*If advertised window from peer is greater than 0, set zero window
     * probing off*/
    if( ( p_ass->event.msg_sack.arwnd_valid == 1 ) &&
            ( p_ass->event.msg_sack.sack_chunk->arwnd > 0 ) )
    {
        p_ass->zero_win_prob_on = SCTP_FALSE;
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_txfsm_Gener_Sack: Zero window probing turned off!" ) );
    }

    if( count == 0 )
    {
        /* All chunks acked adopt new a_rwnd */
        if( p_ass->event.msg_sack.arwnd_valid == 1 )
        {
            p_ass->transmit.rwndthere = p_ass->event.msg_sack.sack_chunk->arwnd;
        }
    }

    else
    {
        if( p_ass->event.msg_sack.arwnd_valid == 1 )
        {
            if( p_ass->event.msg_sack.sack_chunk->arwnd >=
                    p_ass->transmit.total_outstanding )
            {
                p_ass->transmit.rwndthere =
                    p_ass->event.msg_sack.sack_chunk->arwnd -
                    p_ass->transmit.total_outstanding;
            }

            else
            {
                /*
                 * peer receiver window value, less than my outstanding..,
                 * adopt the rwnd value
                 */
                p_ass->transmit.rwndthere =
                    p_ass->event.msg_sack.sack_chunk->arwnd;
            }
        }
    }

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "\n new total outstanding= %u, acked bytes=%u, rwnd:%u\n",
                  p_ass->transmit.total_outstanding, total_bytes_acked_list[0],
                  p_ass->transmit.rwndthere ) );


    /* update cwnd for all destinations */
    if( num_cumacks )
    {
        /* cumulative tsn advanced , update cwnd */
        sctp_slowstart_at_sack_chunk( p_ass, &total_bytes_acked_list[0] );
    }

    /* Congestion Avoidance */
    sctp_cong_avoid_at_sack_chunk( p_ass, num_cumacks,
                                   &total_bytes_acked_list[0], &outstanding_list[0] );

    dest_params = &p_ass->transmit.remote[0];

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        /* Fast retransmit */
        if( packet_loss[counter] == 1 )
        {
            sctp_fast_retrns_on_sack( p_ass, counter );
        }

        if( dest_params[counter].num_chunks_await_ack == 0 )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctp_txfsm_Gener_Sack: No more data on addr index: "
                          "%u, Stop Timer", counter ) );

            dest_params[counter].state = SCTP_TX_STATE_NULL;

            if( dest_params[counter].dest_timer_id[SCTP_DESTTIMER_RTX]
                    != SCTP_NULL_TIMER_ID )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "sctp_txfsm_Gener_Sack: No data on indx:%u, Timer was "
                              "still running !!", counter ) );

                sctp_stop_timer( &p_ass->transmit.
                                 remote[counter].dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL,
                                 SCTP_NULL );
            }
        }

        else
        {
            /* Check If we need to restart the timer */
            if( sctp_is_tsn_present( p_ass,
                                     p_ass->transmit.remote[counter].lowest_tsn ) != SCTP_SUCCESS )
            {
                /* tsn for which we started timer no longer present,
                 * restart timer
                 */
                if( sctp_get_lowest_tsn_on_addr( p_ass, counter, &lowest_tsn )
                        == SCTP_FAILURE )
                {
                    /* SHOULD NOT HAPPEN */
                    SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                                SCTPERROR_LEVEL_CRITICAL, E_INVALID_SACK_CHUNK,
                                "sctp_txfsm_Gener_Sack: Invalid tsn, This case not "
                                "handled yet" );

                    SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );
                    return;
                }

                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "sctp_txfsm_Gener_Sack: Some more data on addr index: %u"
                              ", New Lowest TSN:%u", counter, lowest_tsn ) );

                dest_params[counter].lowest_tsn = lowest_tsn;

                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERDATA,
                    dest_params[counter].rto,
                    counter,
                    &dest_params[counter].dest_timer_id[SCTP_DESTTIMER_RTX],
                    SCTP_NULL );
            }
        }

        /* Retransmission case */
        if( ( total_bytes_acked_list[counter] != 0 )
                || ( packet_loss[counter] == 1 ) )
        {
            /* Changes For CSR#1-5037386 */
            /*Also need to retransmit all the Marked packets which were not been
             *retransmitted last time because of Pmtu or other constraints */
            //sctp_senddata_on_addr(p_ass, counter);
            /*CSR 58053 starts*/
            if( p_ass->transmit.remote[addr_index].
                    dest_timer_id[SCTP_DESTTIMER_BUNDLING] == SCTP_NULL_TIMER_ID )
            {
                sctp_senddata_on_addr( p_ass, counter );
            }

            else
            {
                if( p_ass->transmit.remote[addr_index].wait_bundle >=
                        ( sctp_U32bit )( p_ass->transmit.lowest_path_mtu - SCTP_BUNDLING_THRESHOLD ) )
                {
                    sctp_senddata_on_addr( p_ass, counter );
                }
            }

            /*CSR 58053 ends*/

            /* Changes For CSR#1-5037386 Ends */
        }
    } /* For Loop */

    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_generic_handle_abort_message
 *
 *    DESCRIPTION:
 *        Handle Abort message.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_generic_handle_abort_message(
    sctp_tcb_st    *p_ass )
{
    /* close this association, send notification to SU */
    sctp_U32bit                counter, counter1;
    sctp_endpoint_chg_data_ut  status_data;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_generic_handle_abort_message" ) );

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        for( counter1 = 0; counter1 < SCTP_DESTTIMER_NUM; counter1++ )
        {
            sctp_stop_timer( &p_ass->transmit.
                             remote[counter].dest_timer_id[counter1], SCTP_NULL, SCTP_NULL );
        }
    }

    for( counter = 0; counter < SCTP_ASSOCTIMER_NUM; counter++ )
    {
        sctp_stop_timer( &p_ass->assoc_timer_id[counter],
                         SCTP_NULL, SCTP_NULL );
    }

    p_ass->state = SCTPSTATE_CLOSED;

    status_data.endpoint_retrievable.last_sent_tsn  = p_ass->transmit.tsn - 1;
    status_data.endpoint_retrievable.last_acked_tsn = p_ass->receive.cumtsn;

    if( p_ass->event.msg_abort.cause_length != 0 )
    {
        status_data.endpoint_retrievable.cause_len = p_ass->event.msg_abort.cause_length;
        status_data.endpoint_retrievable.cause     = p_ass->event.msg_abort.p_cause;
    }

    sctp_ntfy_endpt_status_change( p_ass, SCTP_ENDPOINT_DOWN_RETRIEVABLE,
                                   &status_data, E_NOERROR );

    /* wakeup everybody */
    SCTP_WAKEUP( p_ass->p_ep->cond );
    /*SPR 5222*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    /*Wakeup the ep for asconf*/
    SCTP_WAKEUP( p_ass->p_ep->asconf_cond );
#endif
    /*SPR 5222*/
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_txfsm_Null_Userdata
 *
 *    DESCRIPTION:
 *        User DATA request in idle state.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_txfsm_Null_Userdata(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_return_t ret;
    sctp_remote_addr_param_st *dest_params
        = &p_ass->transmit.remote[addr_index];

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_txfsm_Null_Userdata" ) );

    ret = sctp_senddata_on_addr( p_ass, addr_index );

    if( ret == SCTP_SUCCESS )
    {
        dest_params->state = SCTP_TX_STATE_AWTACK;
    }
}


/***************************************************************************
 *    FUNCTION :
 *    sctp_txfsm_Awack_Userdata
 *
 *    DESCRIPTION:This function sends data on address
 *
 *    RETURN VALUE:
 *    None
 **************************************************************************/
void
sctp_txfsm_Awack_Userdata(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_txfsm_Awack_Userdata" ) );

    sctp_senddata_on_addr( p_ass, addr_index );
}


/*
 * Data Timer
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_txfsm_Null_TimerData
 *
 *    DESCRIPTION: Transmit state machine function, data timeout in idle state
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_txfsm_Null_TimerData(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    /* long idle time, decay congestion window */
    sctp_decay_condestion_window( p_ass, addr_index );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_txfsm_Null_TimerData" ) );

}


/***************************************************************************
 *    FUNCTION :
 *        sctp_txfsm_Awack_TimerData
 *
 *    DESCRIPTION:
 *        Transmit state machine function, data timeout in WAIT state.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_txfsm_Awack_TimerData(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_U32bit      sctp_index = 0;
    sctp_sockaddr_st dest_addr;
    sctp_Boolean_t   primary_chg_flag = SCTP_FALSE;
    sctp_U32bit      daddr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_txfsm_Awack_TimerData" ) );

    /*In case of zero window probing ignore timer expiry*/
    if( p_ass->zero_win_prob_on == SCTP_FALSE )
    {
        /* Timeout waitng for an acknowledgement ... */
        p_ass->transmit.remote[addr_index].path_error_count++;
        p_ass->error_count++;
    }

    if( ( p_ass->transmit.remote[addr_index].path_error_count %
            p_ass->transmit.remote[addr_index].path_error_threshold == 0 ) &&
            p_ass->transmit.remote[addr_index].status == SCTP_COMMUNICATION_UP )
    {
        p_ass->transmit.remote[addr_index].status = SCTP_COMMUNICATION_LOST;
        /* SPR 6854 */
        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_init;

        /* check whether if the configured primary path index has gone down.
         * If yes, we start a LOCALHBEAT timer after the DEFAULT_HB_INTERVAL *
         * 5 miliseconds.
         */
        daddr_index = p_ass->transmit.configured_primary_path_index;

        if( p_ass->transmit.remote[daddr_index].src_addr_index == p_ass->configured_local_primary_address_index )
        {
            sctp_start_timer(
                p_ass,
                SCTPEVENT_TIMER_LOCALHBEAT,
                /* SPR 20570 Starts for CSR 1-7626869 */
                SCTP_HB_INTERVAL_TIMEOUT,
                /* SPR 20570 Ends for CSR 1-7626869 */
                addr_index,
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT],
                SCTP_NULL );
        }

        /* SPR 6854 */

        /* update primary destnation address */
        if( p_ass->transmit.primary_address_index == addr_index )
        {
            sctp_index = sctp_get_next_active_addr_index( p_ass, addr_index );

            if( p_ass->transmit.remote[sctp_index].status == SCTP_COMMUNICATION_UP )
            {
                p_ass->transmit.primary_address_index = sctp_index;
                primary_chg_flag = SCTP_TRUE;
            }
        }

        /* notify user of destination state change */

        dest_addr = p_ass->transmit.remote[addr_index].that_address;

        /* notify service user */
        sctp_ntfy_dst_status_change( p_ass, &dest_addr,
                                     SCTP_ADDRESS_UNREACHABLE, E_NOERROR );
    }

    /* update source addr index */
    sctp_db_update_src_addr_index( p_ass, addr_index );

    /* SPR 4033 */
    if( p_ass->error_count > p_ass->config.assoc_max_retrans )
    {
        /*
         * Get the source address index corresponding to the address family.
         */
        sctp_U32bit saddr_index =
            p_ass->transmit.remote[addr_index].src_addr_index;

        sctp_send_chunk_abort( p_ass,
                               &( p_ass->p_ep->this_address[saddr_index] ),
                               p_ass->p_ep->this_port,
                               p_ass->transmit.that_port,
                               &( p_ass->transmit.remote[addr_index].that_address ),
                               p_ass->remote_tag,
                               SCTP_FALSE, 0, 0, NULL, 0, p_ass->config.checksum_func );

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

        sctp_generic_handle_abort_message( p_ass );

        /* Notify user of endpoint state change */
        return;
    }

    if( p_ass->zero_win_prob_on == SCTP_FALSE )
    {

        sctp_slowstart_at_retrx_timeout( p_ass, addr_index );

        p_ass->transmit.remote[addr_index].rto *= 2;  /* Backoff the timer ...*/

        if( p_ass->transmit.remote[addr_index].rto > p_ass->config.rto_max )
        {
            p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
        }
    }

    /*
     * Update rwnd and mark all of datagrams on this address, available for
     * retransmission
     */
    sctp_update_rwnd_retransflag_ontimeout( p_ass, addr_index );

    if( primary_chg_flag == SCTP_FALSE )
    {
        /* Changes For CSR#1-5037386 */
        sctp_index = sctp_get_possible_next_active_addr_index( p_ass, addr_index );
        sctp_senddata_on_alternate_addr( p_ass, addr_index, sctp_index );
        /* Changes For CSR#1-5037386 Ends */
    }

    else
    {
        /* Changes For CSR#1-5037386 */
        sctp_senddata_on_alternate_addr( p_ass, addr_index, sctp_index );
        sctp_move_data_to_primary_addr_from_prev( p_ass, addr_index, sctp_index );
        /* Changes For CSR#1-5037386 Ends */
    }
}

/***************************************************************************
 *    FUNCTION :
 *    sctp_move_data_to_primary_addr
 *
 *    DESCRIPTION:This function moves data to primary address.
 *
 *    RETURN VALUE:
 *    None
 *************************************************************************/
void
sctp_move_data_to_primary_addr(
    sctp_tcb_st                *p_ass,
    sctp_U32bit                pr_addr_index )
{

    sctp_U32bit       count;
    sctp_U32bit       lowest_tsn;
    sctpbuffer_st     *p_buf;
    LIST *queue = &p_ass->transmit.transmit_queue;

    for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
    {
        /* Ignore, if it is primary address */
        if( count == pr_addr_index )
        {
            continue;
        }

        p_buf = ( sctpbuffer_st * )lstFirst( queue );

        for( ; p_buf; p_buf = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf ) )
        {
            if( p_buf->dest_addr_index != count )
            {
                continue;
            }

            if( p_buf->sent == 1 )
            {
                if( ( p_buf->retrans_flag == SCTP_RETRANSFLAG_NONE )
                        || ( p_buf->retrans_flag == SCTP_RETRANSFLAG_RETRANSMITTED ) )
                {
                    p_ass->transmit.rwndthere += p_buf->size;
                }

                /* Mark for retransmission */
                /* Changes For CSR#1-5037386 */
                if( p_buf->ack_flag != SCTP_ACKFLAG_ACKBYGAPBLOCK )
                {
                    p_ass->transmit.remote[count].outstanding -= p_buf->size;
                    p_ass->transmit.remote[count].num_chunks_await_ack--;
                    p_ass->transmit.remote[pr_addr_index].outstanding
                    += p_buf->size;
                    p_ass->transmit.remote[pr_addr_index].num_chunks_await_ack++;

                    if( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )

                    {
                        p_ass->transmit.remote[count].retrans_packet_count--;
                    }

                    p_buf->retrans_flag = SCTP_RETRANSFLAG_MARK;
                    p_ass->transmit.remote[pr_addr_index].retrans_packet_count++;
                    /* Changes for CSR#1-5845954 */
                    p_buf->dest_addr_index = pr_addr_index;
                    /* Changes for CSR#1-5845954 Ends */
                }

                /* Changes For CSR#1-5037386 Ends */
            }

            if( p_buf->sent == 0 )
            {
                p_ass->transmit.remote[p_buf->dest_addr_index].unsent_packet_count--;
                p_ass->transmit.remote[pr_addr_index].unsent_packet_count++;
                /* Changes for CSR#1-5845954 */
                p_buf->dest_addr_index = pr_addr_index;
                /* Changes for CSR#1-5845954 Ends */
            }
        }

        if( p_ass->transmit.remote[count].num_chunks_await_ack != 0 )
        {

        }

        /* Stop Timer on this address */
        sctp_stop_timer(
            &p_ass->transmit.remote[count].
            dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );
        p_ass->transmit.remote[count].state = SCTP_TX_STATE_NULL;

    } /* for loop */

    /* Update lowest TSN on primary address */
    if( sctp_get_lowest_tsn_on_addr( p_ass, pr_addr_index, &lowest_tsn )
            == SCTP_FAILURE )
    {
        return;
    }

    sctp_stop_timer(
        &p_ass->transmit.remote[pr_addr_index].
        dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );

    sctp_start_timer(
        p_ass,
        SCTPEVENT_TIMERDATA,
        p_ass->transmit.remote[pr_addr_index].rto,
        pr_addr_index,
        &p_ass->transmit.remote[pr_addr_index].dest_timer_id[SCTP_DESTTIMER_RTX],
        SCTP_NULL );

    p_ass->transmit.remote[pr_addr_index].lowest_tsn = lowest_tsn;
    p_ass->transmit.remote[pr_addr_index].state = SCTP_TX_STATE_AWTACK;
    return;
}

/***************************************************************************
 *    FUNCTION :
 *    sctp_move_data_to_primary_addr_from_prev
 *
 *    DESCRIPTION:This function moves data to primary address from previous.
 *
 *    RETURN VALUE:
 *    None
 *************************************************************************/
void
sctp_move_data_to_primary_addr_from_prev(
    sctp_tcb_st                *p_ass,
    sctp_U32bit                prev_index,
    sctp_U32bit                pr_addr_index )
{

    sctp_U32bit       lowest_tsn;
    sctpbuffer_st     *p_buf;
    LIST *queue = &p_ass->transmit.transmit_queue;

    if( prev_index == pr_addr_index )
    {
        return;
    }

    if( p_ass->transmit.remote[prev_index].status == SCTP_COMMUNICATION_LOST )
    {
        p_buf = ( sctpbuffer_st * )lstFirst( queue );

        for( ; p_buf; p_buf = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf ) )
        {

            if( p_buf->dest_addr_index != prev_index )
            {
                continue;
            }

            if( p_buf->sent == 1 )
            {
                if( ( p_buf->retrans_flag == SCTP_RETRANSFLAG_NONE )
                        || ( p_buf->retrans_flag == SCTP_RETRANSFLAG_RETRANSMITTED ) )
                {
                    p_ass->transmit.rwndthere += p_buf->size;
                }

                /* Mark for retransmission */
                if( p_buf->ack_flag != SCTP_ACKFLAG_ACKBYGAPBLOCK )
                {
                    p_ass->transmit.remote[prev_index].outstanding -= p_buf->size;
                    p_ass->transmit.remote[prev_index].num_chunks_await_ack--;
                    p_ass->transmit.remote[pr_addr_index].outstanding
                    += p_buf->size;
                    p_ass->transmit.remote[pr_addr_index].num_chunks_await_ack++;

                    if( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )

                    {
                        p_ass->transmit.remote[prev_index].retrans_packet_count--;
                    }

                    /* Changes For CSR#1-5037386 */
                    p_buf->retrans_flag = SCTP_RETRANSFLAG_MARK;
                    p_ass->transmit.remote[pr_addr_index].retrans_packet_count++;
                    /* Changes For CSR#1-5845954 */
                    p_buf->dest_addr_index = pr_addr_index;
                    /* Changes For CSR#1-5845954 Ends */
                }

                /* Changes For CSR#1-5037386 Ends */
            }

            /* Changes For CSR#1-5037386 */
            if( p_buf->sent == 0 )
            {
                p_ass->transmit.remote[prev_index].unsent_packet_count--;
                p_ass->transmit.remote[pr_addr_index].unsent_packet_count++;
                /* Changes For CSR#1-5845954 */
                p_buf->dest_addr_index = pr_addr_index;
                /* Changes For CSR#1-5845954 Ends */
            }

            /* Changes For CSR#1-5037386 Ends */

        }

        /* Stop Timer on this address */
        sctp_stop_timer(
            &p_ass->transmit.remote[prev_index].
            dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );
        p_ass->transmit.remote[prev_index].state = SCTP_TX_STATE_NULL;

    }

    else
    {
        p_buf = ( sctpbuffer_st * )lstFirst( queue );

        for( ; p_buf; p_buf = ( sctpbuffer_st * )lstNext( ( NODE * )p_buf ) )
        {
            if( p_buf->dest_addr_index != prev_index )
            {
                continue;
            }

            if( p_buf->sent == 0 )
            {
                p_ass->transmit.remote[prev_index].unsent_packet_count--;
                p_ass->transmit.remote[pr_addr_index].unsent_packet_count++;
                p_buf->dest_addr_index = pr_addr_index;
            }
        }
    }

    if( SCTP_NULL_TIMER_ID != p_ass->transmit.remote[prev_index].
            dest_timer_id[SCTP_DESTTIMER_BUNDLING] )
    {
        sctp_stop_timer(
            &p_ass->transmit.remote[prev_index].
            dest_timer_id[SCTP_DESTTIMER_BUNDLING], SCTP_NULL, SCTP_NULL );
    }

    if( ( p_ass->transmit.remote[pr_addr_index].unsent_packet_count > 0 )
            && ( SCTP_NULL_TIMER_ID == p_ass->transmit.remote[pr_addr_index].
                 dest_timer_id[SCTP_DESTTIMER_BUNDLING] ) )
    {
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERBUNDLE,
            p_ass->config.bundling_time,
            pr_addr_index,
            &p_ass->transmit.remote[pr_addr_index]
            .dest_timer_id[SCTP_DESTTIMER_BUNDLING],
            SCTP_NULL );

    }

    /* Update lowest TSN on primary address */
    if( sctp_get_lowest_tsn_on_addr( p_ass, pr_addr_index, &lowest_tsn )
            == SCTP_FAILURE )
    {
        return;
    }

    sctp_stop_timer(
        &p_ass->transmit.remote[pr_addr_index].
        dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );

    sctp_start_timer(
        p_ass,
        SCTPEVENT_TIMERDATA,
        p_ass->transmit.remote[pr_addr_index].rto,
        pr_addr_index,
        &p_ass->transmit.remote[pr_addr_index].dest_timer_id[SCTP_DESTTIMER_RTX],
        SCTP_NULL );

    p_ass->transmit.remote[pr_addr_index].lowest_tsn = lowest_tsn;
    p_ass->transmit.remote[pr_addr_index].state = SCTP_TX_STATE_AWTACK;
    return;
}
