/***************************************************************************
 *  FILE NAME  : s_fsmrx.c
 *
 *  DESCRIPTION: Contains the Functions for the Receive State Machine.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  13Oct00    Samahajan    SPR 593      Fix SPR
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  15JUL08    Shikha Kathpal            REL 6.2.0 RFC UPGRADE
 *  24JUL08    Raza Mohamed              CSR : 1-5871995 Merging
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE  "FSM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_mhbld.h>
#include <s_fsmtbl.h>
#include <s_in.h>
#include <s_cobuf.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_fsmrx.h>
#include <s_dbutil.h>


/***************************************************************************
 *    FUNCTION :
 *        sctp_rx_state_machine
 *
 *    DESCRIPTION:
 *        This function extracts the Receive State from association
 *    structure , from the Receive State Machine Table determine
 *    which function should handle this event and state pair and
 *    call that function.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_rx_state_machine(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    event,
    sctp_U32bit    addr_index )
{
    sctp_U32bit state = p_ass->receive.state;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_rx_state_machine: New Event for RX FSM,Addr_index = %u, Event"
                  " Id= %u, state =%u", addr_index, event, state ) );

    if( sctp_rx_fsm_ftbl[event][state] != NULL )
    {
        ( *sctp_rx_fsm_ftbl[event][state] )( p_ass, addr_index );
    }

    else
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC | SCTP_ERR_TRC,
                    ( "sctp_rx_state_machine: Invalid Event for RX FSM, Addr_index = %u,"
                      " Event= %u,state =%u", addr_index, event, state ) );

    }

    return;
}


/*
 * FSM MSG-DATA
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_rxfsm_Null_MsgData
 *
 *    DESCRIPTION:
 *        Got a DATA chunk in the NULL state. We start the delayed
 *    ack timer and delay sending of SACK till that time.
 *    Note: function sctp_update_ack_list must be called before
 *    calling this function (to update the values of parameter cumtsn
 *    and sacklist)
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_rxfsm_Null_MsgData(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_rxfsm_Null_MsgData" ) );

    if( p_ass->receive.overrule_delayack == SCTP_TRUE )
    {
        sctp_addrinfo_st   to;
        /* Changes for CSR#1-5037386 */
        sctp_U32bit        count = 0;
        /* Changes for CSR#1-5037386 Ends */
        sctp_U32bit saddr_index =
            p_ass->transmit.src_addr_index;


        /* Bypass Rx state machine */
        to.checksum   = p_ass->config.checksum_func;
        to.p_sockaddr = &( p_ass->transmit.remote[addr_index].that_address );
        sctp_send_chunk_sack( p_ass,
                              &( p_ass->p_ep->this_address[saddr_index] ),
                              p_ass->p_ep->this_port,
                              p_ass->transmit.that_port,
                              &to,
                              p_ass->remote_tag, p_ass->receive.cumtsn,
                              sctp_db_assoc_rx_window( p_ass ),
                              &p_ass->receive.sacklist,
                              p_ass->receive.number_duplicate_tsn,
                              &p_ass->receive.duplicate_tsn_list[0] );

        p_ass->receive.number_duplicate_tsn = 0;
        p_ass->receive.overrule_delayack    = SCTP_FALSE;
        p_ass->receive.first_data_flag      = 0;

        /* Changes for CSR1-5086391 */
        /* reset prefered sack destination flags for all destinations */
        for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
        {
            p_ass->transmit.remote[count].prefered_sack_dest = SCTP_FALSE;
        }

        /* Changes for CSR1-5086391 */
        return;
    }

    else
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_rxfsm_Null_MsgData: Not"
                                                 "overruling delay ack:not sending sack " ) );
    }

    /*
     * ack policy 0 does not require any timer
     * it requires SACK to be sent every second datagram,
     * For ack policy 1 and 2 start timer
     */
    if( ( SCTP_ACK_POLICY  == 1 )
            || ( SCTP_ACK_POLICY == 2 ) )
    {

        /* This is actally a association timer, but we
         * need destination address to be stored also
         */
        sctp_start_timer( p_ass, SCTPEVENT_TIMERDELAYACK,
                          p_ass->config.sack_period,  addr_index,
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_DELAYACK],
                          SCTP_NULL );
    }

    p_ass->receive.state = SCTP_RX_STATE_SACK_PENDING;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_rxfsm_SackP_MsgData
 *
 *    DESCRIPTION:
 *        Got a DATA chunk in sack_pending state.We stop the delayed
 *    ack timer and send SACK.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_rxfsm_SackP_MsgData(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    /* Changes for CSR#1-5037386 */
    sctp_U32bit count = 0;
    /* Changes for CSR#1-5037386 Ends */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_rxfsm_SackP_MsgData" ) );

    /* for ack policy 0 or 2 send SACK and change receive state */
    if( p_ass->receive.overrule_delayack == SCTP_TRUE )
    {
        sctp_addrinfo_st  to;
        /*
         * Get the source address index corresponding to the address family.
         */

        sctp_U32bit saddr_index =
            p_ass->transmit.src_addr_index;


        to.checksum   = p_ass->config.checksum_func;
        to.p_sockaddr = &( p_ass->transmit.remote[addr_index].that_address );
        sctp_send_chunk_sack( p_ass,
                              &( p_ass->p_ep->this_address[saddr_index] ),
                              p_ass->p_ep->this_port,
                              p_ass->transmit.that_port,
                              &to,
                              p_ass->remote_tag, p_ass->receive.cumtsn,
                              sctp_db_assoc_rx_window( p_ass ),
                              &p_ass->receive.sacklist,
                              p_ass->receive.number_duplicate_tsn,
                              &p_ass->receive.duplicate_tsn_list[0] );

        p_ass->receive.number_duplicate_tsn = 0;
        p_ass->receive.overrule_delayack    = SCTP_FALSE;
        p_ass->receive.first_data_flag      = 0;


        /* stop delay ack timer,If it was running */
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_DELAYACK],
            SCTP_NULL, SCTP_NULL );

        /* Changes for CSR#1-5037386 */
        /* reset prefered sack destination flags for all destinations */
        for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
        {
            p_ass->transmit.remote[count].prefered_sack_dest = SCTP_FALSE;
        }

        /* Changes for CSR#1-5037386 Ends */

        p_ass->receive.state = SCTP_RX_STATE_NULL ;
    }
}


/*
 * TIMER-DELAYACK
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_rxfsm_SackP_TimerDeack
 *
 *    DESCRIPTION:
 *        Delayed SACK timer expired, Build SACK chunk and send to peer.
 *    Also change receive state to NULL.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_rxfsm_SackP_TimerDeack(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_U32bit saddr_index = 0;
    sctp_addrinfo_st to;
    /* Changes for CSR#1-5037386 */
    sctp_U32bit count = 0;
    /* Changes for CSR#1-5037386 Ends */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_rxfsm_SackP_TimerDeack" ) );

    /*
     * Get the source address index corresponding to the address family.
     */
    saddr_index =  p_ass->transmit.src_addr_index;

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[addr_index].that_address );
    sctp_send_chunk_sack( p_ass,
                          &( p_ass->p_ep->this_address[saddr_index] ),
                          p_ass->p_ep->this_port,
                          p_ass->transmit.that_port,
                          &to,
                          p_ass->remote_tag,
                          p_ass->receive.cumtsn,
                          sctp_db_assoc_rx_window( p_ass ), &p_ass->receive.sacklist, 0, NULL );

    p_ass->receive.number_duplicate_tsn = 0;
    p_ass->receive.overrule_delayack    = SCTP_FALSE;

    if( ( SCTP_ACK_POLICY  == 1 )
            || ( SCTP_ACK_POLICY == 2 ) )
    {
        p_ass->receive.first_data_flag      = 0;
    }

    /* Changes for CSR#1-5037386 */
    /* reset prefered sack destination flags for all destinations */
    for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
    {
        p_ass->transmit.remote[count].prefered_sack_dest = SCTP_FALSE;
    }

    /* Changes for CSR#1-5037386 Ends */

    p_ass->receive.state = SCTP_RX_STATE_NULL;
}


/*
 * USER-DATA
 */

/***************************************************************************
 *    FUNCTION :
 *        sctp_rxfsm_Null_UserData
 *
 *    DESCRIPTION:
 *        User Data request, when no Sack is pending
 *
 *    RETURN VALUE:
 *        None
**************************************************************************/
void
sctp_rxfsm_Null_UserData(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    /* No Sack Pending nothing to do */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_rxfsm_Null_UserData: event ignored, addr %u", addr_index ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( addr_index );
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctp_rxfsm_SackP_UserData
 *
 *    DESCRIPTION:
 *        User Data request, when Sack is pending
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_rxfsm_SackP_UserData(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    /* Add Sack Chunk  */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_rxfsm_SackP_UserData: Bundle Sack with user data, addr index"
                  ": %u", addr_index ) );

    p_ass->rx_event.evt_cmduserdata.buffer_offset
        = sctp_add_chunk_sack(
              p_ass,
              p_ass->rx_event.evt_cmduserdata.buffer_offset,
              p_ass->receive.cumtsn, sctp_db_assoc_rx_window( p_ass ),
              &p_ass->receive.sacklist, 0, NULL );

    /* stop delay ack timer,If it was running */
    sctp_stop_timer(
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_DELAYACK],
        SCTP_NULL, SCTP_NULL );

    p_ass->receive.number_duplicate_tsn = 0;
    p_ass->receive.overrule_delayack    = SCTP_FALSE;
    p_ass->receive.state                = SCTP_RX_STATE_NULL;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( addr_index );
    /*to avoid the warning*/

}


