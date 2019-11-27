/***************************************************************************
 *  FILE NAME  : s_fsmass.c
 *
 *  DESCRIPTION: Contains the Functions for the association state machine.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  21Oct04    Amit Ray     Rel 6.0      SCTP/FUNC-IG-12
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "FSM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_cobuf.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_fsmcng.h>


/*
 * SLOW START
 */

/*
 * This feature allows sctp endpoint to probe network condition at association
 * setup and adapt to network conditions after retransmission timer timeout.
 * Parameter cwnd and ssthresh are maintained per destination address to
 * support slow start.
 * This is activated at the following conditions
 * Association establishment
 * Inactivity timeout on a destination
 * Retransmission timeout on a destination
 * SACK chunk received.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_slowstart_at_setup
 *
 *    DESCRIPTION:
 *        This functions adjust the congestion window and
 *    slow start threshold parameters.
 *    cwnd = 2*PMTU
 *    ssthresh = rwnd
 *    equated to rwnd and cwnd is equated to 2*MTU**
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_slowstart_at_setup(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_U32bit first_part;
    sctp_U32bit sec_part;


    first_part = 4 * p_ass->transmit.remote[addr_index].path_mtu ;
    sec_part = ( ( 2 * p_ass->transmit.remote[addr_index].path_mtu ) > 4389 ) ? ( 2 * p_ass->transmit.remote[addr_index].path_mtu ) : 4389;

    p_ass->transmit.remote[addr_index].cwnd = ( first_part < sec_part ) ? first_part : sec_part ;

    p_ass->transmit.remote[addr_index].ssthresh = p_ass->transmit.rwndthere;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_decay_condestion_window
 *
 *    DESCRIPTION:
 *        These functions adjust the congestion window parameter.
 *    cwnd is reduced to maximum of cwnd/2 and 2.MTU. This function
 *    is invoked after a long idle time on a destination.
 *
 *    RETURN VALUE:
 *        None
 *    NOTE: impl. guide -06 says cwnd will be 2*MTU ......
 *          ....no max will be taken as earlier.
 *************************************************************************/
void
sctp_decay_condestion_window(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_U32bit first_part;
    sctp_U32bit sec_part;

    sctp_remote_addr_param_st *dest_params =
        &p_ass->transmit.remote[addr_index];




    first_part = 4 * dest_params->path_mtu ;
    sec_part = ( ( 2 * dest_params->path_mtu ) > 4389 ) ? ( 2 * dest_params->path_mtu ) : 4389;

    dest_params->cwnd = ( first_part < sec_part ) ? first_part : sec_part ;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_slowstart_at_retrx_timeout
 *
 *    DESCRIPTION:
 *        These functions adjust the congestion window (cwnd) and slow start
 *    threshold (ssthresh) parameter after retransmission timeout. on
 *    a destination. ssthresh is equated to maximum of cwnd/2 and 2.MTU
 *    and cwnd is equated to 1*MTU in this case.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_slowstart_at_retrx_timeout(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_remote_addr_param_st *dest_params
        = &p_ass->transmit.remote[addr_index];
    /*
     * implementation guide-12
     */

    if( dest_params->cwnd / 2 > ( sctp_U32bit )( 4 * dest_params->path_mtu ) )
    {
        dest_params->ssthresh = dest_params->cwnd / 2;
    }

    else
    {
        dest_params->ssthresh = 4 * dest_params->path_mtu;
    }

    /*
     * implementation guide-12
     */
    dest_params->cwnd  = 1 * dest_params->path_mtu;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_slowstart_at_sack_chunk
 *
 *    DESCRIPTION:
 *        This functions adjusts/increase the congestion window (cwnd)
 *    parameter after procession sackchunk from peer.These functions
 *    adjust the congestion window (cwnd) and slow start
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_slowstart_at_sack_chunk(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    *total_bytes_acked_list )
{
    sctp_U32bit counter;
    sctp_remote_addr_param_st *dest_params = &p_ass->transmit.remote[0];

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        if( dest_params[counter].cwnd <= dest_params[counter].ssthresh )
        {
            dest_params[counter].cwnd +=
                ( total_bytes_acked_list[counter] < dest_params[counter].path_mtu )
                ? total_bytes_acked_list[counter] : dest_params[counter].path_mtu;
        }
    }
}


/*
 * Congestion avoidance
 */

/*
 * This algorithm is activated after receiving SACK from peer. Congestion
 * window of a destination address is increased by 1*MTU if applicable.
 * Parameters outstanding and partial_bytes_acked  are maintained per
 * destination address. These are initialised to '0' at association setup.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_cong_avoid_at_sack_chunk
 *
 *    DESCRIPTION:
 *        This functions adjusts/increase the congestion window (cwnd)
 *    parameter after procession sack chunk from peer.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_cong_avoid_at_sack_chunk(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    num_chunks_cumtsn,
    sctp_U32bit    *total_bytes_acked_list,
    sctp_U32bit    *outstanding_list )
{
    sctp_U32bit counter;
    sctp_remote_addr_param_st *dest_params = &p_ass->transmit.remote[0];

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        if( ( dest_params[counter].cwnd > dest_params[counter].ssthresh )
                && ( num_chunks_cumtsn != 0 ) )
        {
            dest_params[counter].partial_bytes_acked
            += total_bytes_acked_list[counter];

            if( ( dest_params[counter].partial_bytes_acked
                    >= dest_params[counter].cwnd )
                    && ( outstanding_list[counter] >= dest_params[counter].cwnd ) )
            {
                dest_params[counter].cwnd += dest_params[counter].path_mtu ;

                dest_params[counter].partial_bytes_acked
                -= dest_params[counter].cwnd;
            }
        }

        /* clear partial bytes acked field if all data is acked */
        if( dest_params[counter].outstanding == 0 )
        {
            dest_params[counter].partial_bytes_acked = 0;
        }
    }
}


/*
 * Congestion Control
 */

/*
 * This is activated at two instances.
 * when packet loss are detected , this is handled in function
 * sctp_adjust_cong_param_on_packetloss
 * When Retransmission timer expires. This is done in
 * sctp_slowstart_at_retransmission_timeout
 */

/*
 * Fast Retransmit on Gaps
 */

/* This is activated at two instances.
 * At the time of processing DATA chunks, receiver notices a hole in TSN
 * received. If a hole exists in the received TSN's delayed acknowledge is
 * not applied and SACK is build and sent immediately.
 * At the time of processing SACK chunk, If some TSN's are noticed being missed.
 * (Calculated through TSN fragments). A counter keeps track of how many time a
 * TSN was noticed missed. Parameter missed is added in the buffer structure for
 * the same. This parameter is initialised at the time of buffer allocation and
 * incremented each time we notice a TSN being missed. If any TSN was noticed
 *  missed 4th time following functions are invoked
 *   - sctp_fast_retrns_on_sack
 */


/***************************************************************************
 *    FUNCTION :
 *        sctp_fast_retrns_on_sack
 *
 *    DESCRIPTION:
 *        These functions adjust the congestion window (cwnd) and slow
 *    start threshold (ssthresh) parameters after detection of
 *    missing TSN's. ssthresh is equated to maximum of cwnd/2  and
 *    2.MTU. cwnd is equated to ssthresh.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_fast_retrns_on_sack(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_remote_addr_param_st *dest_params
        = &p_ass->transmit.remote[addr_index];

    /* update cwnd parameters */
    if( p_ass->transmit.fast_recovery_exit == 0 ) /*not in fast recovery*/
    {
        dest_params->ssthresh =
            ( dest_params->cwnd / 2 > ( sctp_U32bit )( dest_params->path_mtu * 4 ) )
            ? dest_params->cwnd / 2 : ( sctp_U32bit )dest_params->path_mtu * 4;

        /* congestion window should not be increased */
        if( dest_params->cwnd > dest_params->ssthresh )
        {
            dest_params->cwnd = dest_params->ssthresh;
        }

        /*enter fast recovery*/
        p_ass->transmit.fast_recovery_exit =
            p_ass->transmit.highest_outstanding;
    }
}







