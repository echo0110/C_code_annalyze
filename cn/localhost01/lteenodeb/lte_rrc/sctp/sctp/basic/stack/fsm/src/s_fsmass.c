/***************************************************************************
 *  FILE NAME  : s_fsmass.c
 *
 *  DESCRIPTION: Contains the Functions for the association state machine.
 *
 *  DATE       NAME         REF#         REASON
 *  ------------------------------------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  13Oct00    hbhondwe     SPR472       Fix SPR
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  20May02    ygahlaut     SPR 4033 4038 Fix
 *  28MAy02    ygahlaut     SPR 4151    Fix
 *  27Aug02    mrajpal      SPR 4798     fix
 *  23Nov02    mrajpal      SPR 5085     set primary bug
 *  25Nov02    mrajpal      SPR 5048     moved WAKEUP to delete_ep
 *  07Jan03    mrajpal      SPR 5221     Fix
 *  14Oct03    vibansal                  Changes for mixed
 *                                       IPv4-v6 support
 *  23Feb04    mrajpal      SPR 6631     T5- Shutdown Gaurd Timer
 *  22Oct04    Amit Ray     Rel 6.0      SCTP/FUNC-IG-14
 *  09May08    Raza         Rel 6.2      SPR 18527
 *  24May08    Raza         Rel 6.2      Merged CSR : 1-5056601, 1-6124313
 *  26Nov10    Akanksha     SPR 20749    CSR Fixing Removing UMR and crash due to that
 *  21Apr10    Akanksha     SPR 20382    CSR Fixing -uniform path_error_count check
 *  19May10    Akanksha     SPR 20406    CSR Fixing -Post Restart, handling incomplete
 *  24May10    Rajnesh      SPR 20411    Warning Fixed
 *  25mar11    Akanksha     SPR 20859    Enhancement for Callback
 * 07Jul2011    Vaibhav Banga               changes for klocwork fix for IBM delivery
 *  18Feb13    Rohan Pawa   SPR 21107    CSR 24680
 *  1May13    Pranav Sharma SPR 21107    CSR 44034
 *  1May13    Pranav       SPR 21127    CSR 40731-GCC 4.7 warning removal
 *  21May14   Pinky Sinha   Bug Id 6    CSR  78264
 *  29May17   Neha Gupta    Bug Id 199  CSR  134689
 *  Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "FSM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_osmutx.h>
#include <s_mhbld.h>
#include <s_mhpars.h>
#include <s_mhmac.h>
#include <s_mhproc.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_fsmrx.h>
#include <s_fsmtx.h>
#include <s_fsmtbl.h>
#include <s_fsmdat.h>
#include <s_fsmmtu.h>
#include <s_fsmcng.h>
#include <s_cobuf.h>
#include <s_in.h>
#include <s_stats.h>
#include <s_uisuif.h>
#include <s_fsmdat.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_stats.h>
#include <s_err.h>
#include <s_fsmass.h>
#include <s_dbutil.h>
#include <s_comd5.h>
#include <s_es.h>

#ifdef SCTP_DYNAMIC_IP_SUPPORT
static void sctp_proc_asconf_failure(
    sctp_tcb_st                 *p_ass,
    sctp_asconf_param_ut        *p_param,
    sctp_asconf_response_et     ecode );

static void sctp_proc_asconf_success(
    sctp_tcb_st          *p_ass,
    sctp_asconf_param_ut *p_param );
#endif

/***************************************************************************
 *    FUNCTION :
 *        sctp_assoc_state_machine
 *
 *    DESCRIPTION:
 *        This function extracts the association State from association
 *    structure. From the Association State Machine Table determines
 *    which function should handle this event and state pair and calls that
 *    function.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_assoc_state_machine(
    sctp_tcb_st        *p_ass,
    sctp_U32bit        event )
{
    if( ( !p_ass ) || ( event >= SCTP_EVENT_NUM ) )
    {
        return;
    }

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "New Event for Assoc FSM, Event Id "
                                             "= %u, state = %u", event, p_ass->state ) );

    p_ass->fsm_event = ( sctp_U8bit )event;

    if( sctp_assoc_fsm_ftbl[event][p_ass->state] != NULL )
    {
        ( *sctp_assoc_fsm_ftbl[event][p_ass->state] )( p_ass );
    }

    else
    {
        /* Unexpected event state pair */
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC, ( "Invalid Event for Assoc FSM, "
                                                 "Event Id= %u, state =%u", event, p_ass->state ) );

        SCTP_ERROR( SCTPERROR_FLAG_PROTOCOLERROR, SCTPERROR_LEVEL_CRITICAL,
                    E_INVALID_EVENT_FOR_STATE,
                    "Invalid Event for Association FSM, Association" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_PROTOCOL_ERRORS, 1 );
    }
}


/*
 * FSM Trigger CMD-ABORT
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Close_CmdAbort
 *
 *    DESCRIPTION:
 *        COMMAND Abort in CLOSED state of the association.
 *    Deletes the association if ABORT comes in closed state of the
 *    association
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Close_CmdAbort(
    sctp_tcb_st     *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Close_CmdAbort" ) );

    /* Delete the association if ABORT comes in closed state of the assoc */
    if( p_ass->p_ep->sk_type == SCTP_SOCK_STREAM )
    {
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
    }

    else
    {
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }
}


/***************************************************************************
 *    FUNCTION :
 *    sctpfsm_assoc_Gener_CmdAbort
 *
 *    DESCRIPTION:
 *  Gets the source and the Destination address index, then sends the
 *  abort chunk to the peer and finally deletes the association.
 *
 *    RETURN VALUE:
 *    None
 **************************************************************************/
void
sctpfsm_assoc_Gener_CmdAbort(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit  saddr_index = 0;
    sctp_U32bit  daddr_index = 0;
    sctp_error_t s_err;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_CmdAbort" ) );

    /*
     * Need to send Abort to the destination.
     * So get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Could not get compatible address index for src and dst" ) );

        return;
    }

    /* Send ABORT chunk to peer */
    sctp_send_chunk_abort( p_ass,
                           &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                           p_ass->transmit.that_port,
                           &( p_ass->transmit.remote[daddr_index].that_address ),
                           p_ass->remote_tag,
                           SCTP_TRUE, SCTPERR_USER_INITIATED_ABORT,
                           p_ass->event.cmd_abort.cause_length, p_ass->event.cmd_abort.cause_info,
                           0, p_ass->config.checksum_func );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

    /* Delete the association */
    if( p_ass->p_ep->sk_type == SCTP_SOCK_STREAM )
    {
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
    }

    else
    {
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }
}


/*
 * FSM Trigger CMD-TERM
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Close_CmdTerm
 *
 *    DESCRIPTION:
 *        This function handles graceful close request from ULP in close state.
 *    cleanup the association.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Close_CmdTerm(
    sctp_tcb_st     *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Close_CmdTerm" ) );

    sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_CmdTerm
 *
 *    DESCRIPTION:
 *        This function handles graceful close request from ULP.
 *    when Association State is neither CLOSED nor ESTABLISHED.
 *    Send abort.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_CmdTerm(
    sctp_tcb_st   *p_ass )
{
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;
    sctp_error_t s_err;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_CmdTerm" ) );

    /*
     * Need to send Abort to the destination.
     * So get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Could not get compatible address index for src and dst" ) );

        return;
    }

    /* send abort and clean up the association */
    sctp_send_chunk_abort( p_ass,
                           &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                           p_ass->transmit.that_port,
                           &( p_ass->transmit.remote[daddr_index].that_address ), p_ass->remote_tag,
                           SCTP_FALSE, 0, 0, NULL,           /*Changes for CSR#1-4776901 & CSR#1-4835338 */
                           0, p_ass->config.checksum_func );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

    sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
}


/***************************************************************************
*    FUNCTION :
*        sctpfsm_assoc_ShutA_CmdTerm
*
*    DESCRIPTION:
*        This function handles graceful close request from ULP in any of
*    shutdown state.
*
*    RETURN VALUE:
*        None
**************************************************************************/
void
sctpfsm_assoc_ShutA_CmdTerm(
    sctp_tcb_st      *p_ass )
{
    /* Ignore the request as shutdown has already started */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutA_CmdTerm" ) );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "Ignore Shutdown as shutdowm already started " ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_CmdTerm
 *
 *    DESCRIPTION:
 *        This function handles graceful close request from ULP in established
 *    state. Scan Transmit queue, It there is some data waiting to be acked,
 *    change association state to SHUTDOWN-PENDING.
 *    If no DATA chunk is left in transmit queue send shutdown and change state
 *    to SHUTDOWN-SENT.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_CmdTerm(
    sctp_tcb_st     *p_ass )
{
    sctpbuffer_st   *p_buf;
    sctp_error_t    s_err;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_CmdTerm" ) );

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    if( !p_buf )
    {
        sctp_U32bit saddr_index = 0;
        sctp_U32bit daddr_index = 0;
        sctp_addrinfo_st  to;

        /*
         * Need to send Shutdown to the destination.
         * So get the source address index and the destination addres index.
         */
        if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                            &daddr_index, &s_err ) )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                        ( "Could not get compatible address index for src and dst" ) );

            return;
        }

        /* No pending data */
        to.checksum     = p_ass->config.checksum_func;
        to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
        sctp_send_chunk_shutdown( p_ass,
                                  &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                                  p_ass->transmit.that_port,
                                  &to, p_ass->remote_tag, p_ass->receive.cumtsn );

        /* Start the shutdown timer */
        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMERSHUTDN,
                          p_ass->transmit.remote[daddr_index].rto,
                          daddr_index,
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                          SCTP_NULL );

        /*
         * If SO LINGER flag is set then start the linger timer.
         */
        if( p_ass->p_ep->so_linger.l_onoff )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctpfsm_assoc_Estbl_CmdTerm:Starting the linger timer" ) );


            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERLINGER,
                              p_ass->p_ep->so_linger.l_linger,
                              daddr_index,
                              &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LINGER],
                              SCTP_NULL );
        }

        p_ass->shutdown_retry = 0;
        p_ass->state = SCTPSTATE_SHUTDOWNSENT;
    }

    else
    {
        p_ass->state = SCTPSTATE_SHUTDOWNPEND;
    }

    /* SPR 6631 */
    /* Start the T5 Shutdown Gaurd Timer*/
    if( SCTP_FAILURE == sctp_start_timer( p_ass, SCTPEVENT_TIMER,
                                          ( p_ass->config.rto_max * 5 ), 0,
                                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT],
                                          /*The ASSOCTIMER_INIT is reused to avoid allocating one more timer */
                                          SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Could not start Shutdown Gaurd Timer" ) );
    }

    /* 6631 */
}


/*
 * FSM Trigger EVT-TIMEOUT
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutS_EvtTimer
 *
 *    DESCRIPTION:
 *        Shutdown gaurd timer expires, Abort the association.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutS_EvtTimer(
    sctp_tcb_st   *p_ass )
{
    p_ass->event.cmd_abort.cause_length = 0;
    sctpfsm_assoc_Gener_CmdAbort( p_ass );
}


/*
 * FSM Trigger MSG-INITACK
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgInitack
 *
 *    DESCRIPTION:
 *        Initack not expected. Ignore it.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_MsgInitack(
    sctp_tcb_st   *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgInitack" ) );
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "Initack unexpected ignored .." ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_CookW_MsgInitack
 *
 *    DESCRIPTION:
 *        Initack received in the  Cookie-Wait state.
 *    Do stream negotiation, Update association parameters
 *    based on Initack parameters, Build and send cookie to peer,
 *    Restart Timer T1 and change association state to
 *    Cookie-Sent.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_CookW_MsgInitack(
    sctp_tcb_st  *p_ass )
{
    sctp_U32bit         counter;
    sctp_U16bit         cookie_size, temp_size = 0;
    sctp_U32bit         saddr_index = 0;
    sctp_U32bit         daddr_index = 0;
    sctp_error_t        s_err;
    sctpbuffer_st       *p_sctp_buf          = SCTP_NULL;
    sctp_U8bit          *p_cookie = SCTP_NULL;
    sctp_addrinfo_st    addr_info;
    sctp_U32bit         dest_addr_counter;
    sctp_suppaddr_st    supp_addr_type;
    sctp_U32bit         send_to_addr_index = 0;
    sctp_return_t       ret = SCTP_FAILURE;
    sctp_sockaddr_st    addr_list[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U16bit         i = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_CookW_MsgInitack" ) );

    /* Storing peer's tag, receiving window, tsn in association structure */
    p_ass->remote_tag
        = p_ass->event.msg_initack.initack_chunk->fixed_params.init_tag;

    p_ass->receive.cumtsn
        = p_ass->event.msg_initack.initack_chunk->fixed_params.init_tsn - 1;
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    p_ass->receive.serial_no = p_ass->receive.cumtsn;
#endif
    p_ass->transmit.rwndthere
        = p_ass->event.msg_initack.initack_chunk->fixed_params.a_rwnd;

    /*primary address is set in mh based on set_prim_addr param */

    sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr_type );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_CookW_MsgInitack: Could not get compatible address"
                      " index for src and dst" ) );

        return;
    }

    /* Store All the Addresses received in the Connect Request from the ULP */
    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        addr_list[counter] = p_ass->transmit.remote[counter].that_address;
    }

    for( counter = 0, dest_addr_counter = 0;
            counter < p_ass->event.msg_initack.initack_chunk->num_addrs; counter++ )
    {
        /*
         * Ignore the address type that the local endpoint does not support.
         */
        if( ( ( p_ass->event.msg_initack.initack_chunk->addr_list[counter].family
                == SCTP_AF_INET6 ) && ( supp_addr_type.v6 != SCTP_TRUE ) ) ||
                ( ( p_ass->event.msg_initack.initack_chunk->addr_list[counter].family
                    == SCTP_AF_INET ) && ( supp_addr_type.v4 != SCTP_TRUE ) ) )
        {
            continue;
        }


        p_ass->transmit.remote[dest_addr_counter].that_address
            = p_ass->event.msg_initack.initack_chunk->addr_list[counter];
        /* Update SRC address index, All addresses of same type assumed */

        /* Update Path Error Count Also */
        p_ass->transmit.remote[dest_addr_counter].path_error_threshold
            = p_ass->config.path_max_retrans;

        p_ass->transmit.remote[dest_addr_counter].rto = p_ass->config.rto_init;

        for( i = 0 ; i < p_ass->transmit.num_remote_address; i++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr(
                        &p_ass->transmit.remote[dest_addr_counter].that_address,
                        &addr_list[i] ) )
            {
                /* This Remote Address was received in the CONNECT Request,
                 * Hence Mark it confirmed.
                 */
                p_ass->transmit.remote[dest_addr_counter].is_confirmed   = SCTP_TRUE;
                p_ass->transmit.remote[dest_addr_counter].perf_path_verf = SCTP_FALSE;
                break;
            }
        }

        if( i == p_ass->transmit.num_remote_address )
        {
            /* This is a New Remote Address received in the INIT-ACK,
             * Hence Mark it as Unconfirmed.
             */
            p_ass->transmit.remote[dest_addr_counter].is_confirmed   = SCTP_FALSE;
            p_ass->transmit.remote[dest_addr_counter].perf_path_verf = SCTP_TRUE;
        }

        dest_addr_counter++;

        if( dest_addr_counter == p_ass->event.msg_initack.initack_chunk->num_addrs )
        {
            dest_addr_counter = 0;
        }
    }

    for( daddr_index = 0 ;
            daddr_index < p_ass->event.msg_initack.initack_chunk->num_addrs ;
            daddr_index++ )
    {
        if( SCTP_SUCCESS ==
                sctp_cmp_addr(
                    &p_ass->event.msg_initack.initack_chunk->addr_list[daddr_index],
                    &p_ass->event.msg_initack.from_addr ) )
        {
            break;
        }
    }

    /* Note : As per RFC4960,
     * "The INIT ACK MUST be sent to the source address of the INIT."

       But a Misbehaving SCTP may fill a Invalid IP-Address in the
       INIT-ACK and IP header, hence we will mark the Current Primary
       Address Incorrectly. To Avoid such a situation, we will check
       if the Address being made "Current Primary Path" is already
       Confirmed or not.
     */
    if( p_ass->transmit.remote[daddr_index].is_confirmed == SCTP_TRUE )
    {
        p_ass->transmit.primary_address_index = daddr_index;
    }

    else
    {
        for( daddr_index = 0 ;
                daddr_index < p_ass->transmit.num_remote_address;
                daddr_index++ )
        {
            if( p_ass->transmit.remote[ daddr_index].is_confirmed )
            {
                /* Mark the First Confirmed Address as the Current Primary Path */
                p_ass->transmit.primary_address_index = daddr_index;
                break;
            }
        }
    }

    for( counter = 0, dest_addr_counter = daddr_index;
            counter < p_ass->event.msg_initack.initack_chunk->num_addrs;
            counter++ )
    {


        sctp_db_get_v4_v6_addr_index_with_af( p_ass, &saddr_index,
                                              p_ass->transmit.remote[dest_addr_counter].that_address.family,
                                              &s_err );

        p_ass->transmit.remote[dest_addr_counter].src_addr_index =
            saddr_index;
        saddr_index++;

        if( saddr_index == p_ass->p_ep->naddr )
        {
            saddr_index = 0;
        }

        dest_addr_counter ++;

        if( dest_addr_counter ==
                p_ass->event.msg_initack.initack_chunk->num_addrs )
        {
            dest_addr_counter = 0;
        }

    }

    /*
     * Set the number of destination addresses equal to the number of
     * addresses that we support.
     */
    p_ass->transmit.num_remote_address =
        p_ass->event.msg_initack.initack_chunk->num_addrs;

    /* Stop association INIT timer */
    sctp_stop_timer( &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT], SCTP_NULL,
                     SCTP_NULL );

    /* update ecn capable parameter */
    for( counter = 0;
            counter < p_ass->event.msg_initack.initack_chunk->num_opt_params;
            counter++ )
    {
        if( p_ass->event.msg_initack.initack_chunk->opt_params[counter].param_type
                == SCTPVARPAR_ECNCAPABLE )
        {
            break;
        }
    }

    if( SCTP_IS_ECN_CAPABLE &&
            ( counter != p_ass->event.msg_initack.initack_chunk->num_opt_params ) )
    {
        p_ass->ecn.ecn_capable = 1;
    }

    else
    {
        p_ass->ecn.ecn_capable = 0;
    }

    p_ass->config.adaption_ind = SCTP_NULL_ADAPTION_IND;

    for( counter = 0;
            counter < p_ass->event.msg_initack.initack_chunk->num_opt_params;
            counter++ )
    {
        if( p_ass->event.msg_initack.initack_chunk->opt_params[counter].
                param_type
                == SCTPVARPAR_ADAPTIONIND )
        {
            p_ass->config.adaption_ind =
                p_ass->event.msg_initack.initack_chunk->opt_params[counter].
                param_data.adaption_ind;
            break;
        }
    }

    /* store cookie for restrans */
    cookie_size = p_ass->event.msg_initack.initack_chunk->cookie_len;

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_CookW_MsgInitack: Could not get compatible address"
                      " index for src and dst" ) );

        return;
    }

    if( daddr_index != p_ass->transmit.configured_primary_path_index )
    {

        p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].status = SCTP_COMMUNICATION_LOST;

    }

    /* Handle if we cannot handle the size of cookie sent */
    if( cookie_size > SCTP_MAX_COOKIE_SIZE )
    {
        sctp_addrinfo_st    to;

        to.checksum   = p_ass->config.checksum_func;
        to.p_sockaddr = &( p_ass->transmit.remote[daddr_index].that_address );
        sctp_send_chunk_error( p_ass,
                               &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                               p_ass->transmit.that_port,
                               &to, p_ass->remote_tag,
                               SCTPERR_NORESOURCE, 0, NULL );

        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );

        return;
    }

    p_ass->cookie_size = cookie_size;
    p_cookie = p_ass->event.msg_initack.initack_chunk->cookie;

    while( cookie_size != 0 )
    {
        temp_size = ( SCTP_MAX_FRAGSIZE < cookie_size ) ?
                    SCTP_MAX_FRAGSIZE : cookie_size;

        /* Get a free transmit buffer */
        p_sctp_buf = sctp_get_transmit_buffer( p_ass, temp_size );

        /* SPR : 18527 */
        if( p_sctp_buf == SCTP_NULL )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                        ( "ERROR : sctp_get_transmit_buffer() failed for "
                          "Assoc-ID : %d and temp_size : %d", p_ass->assoc_id, temp_size ) );

            return;
        }

        /* SPR : 18527 */

        /* SPR 20286, 20749 Starts */
        p_sctp_buf->retrans_flag =  SCTP_RETRANSFLAG_NONE;
        p_sctp_buf->dest_addr_index = 0;
        /* SPR 20286, 20749 Ends */

        __MEMCOPY( p_sctp_buf->buffer, p_cookie, temp_size );
        p_sctp_buf->size = temp_size;

        /* SPR Fix 20749 Starts*/

        p_sctp_buf->sent = 1;

        /* SPR Fix 20749 Stops*/
        lstAdd( &p_ass->cookie_list, ( NODE * )p_sctp_buf );
        p_cookie += temp_size;
        cookie_size -= temp_size;
    }

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_CookW_MsgInitack: Send Cookie, Cookie len =%d ",
                  p_ass->cookie_size ) );

    /*Now cookie will be sent to the address from which INIT-ACK has come.
     *For this-(Rel-5.0.5)
     *In init-ack event a new field from_addr is added which is filled in mh.
     *Use this field, to serch the addr index,store it in send_to_addr_index.
     */

    /* calculate addres index */
    send_to_addr_index = 0;
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_initack.from_addr,
                                        &send_to_addr_index );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC, ( "sctpfsm_assoc_CookW_MsgInitack: -> Invalid remote address:" ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC, &( p_ass->event.msg_initack.from_addr ) );
        return;
    }


    /*
     * Check whether cookie should be bundled with error chunk due to unrecog
     * parameters in initack
     */

    addr_info.checksum   = p_ass->config.checksum_func;

    /* Cookie Will be sent to the Confirmed Current Primary Path */
    addr_info.p_sockaddr = &p_ass->transmit.remote[daddr_index].that_address;
    addr_info.p_info     = p_ass;

    saddr_index =   p_ass->transmit.remote[send_to_addr_index].src_addr_index;

    if( p_ass->event.msg_initack.send_err_with_cookie == SCTP_TRUE )
    {
        /* Error chunk has to bundled with cookie chunk */

        sctp_send_cookie_err_chunk_bundled( p_ass,
                                            &p_ass->p_ep->this_address[saddr_index], p_ass->p_ep->this_port,
                                            p_ass->transmit.that_port,
                                            &addr_info,
                                            p_ass->remote_tag,
                                            &p_ass->cookie_list, p_ass->cookie_size, SCTPERR_UNRECOPARAMS,
                                            ( sctp_U16bit )p_ass->event.msg_initack.unrecog_param_len,
                                            p_ass->event.msg_initack.unrecog_params );
    }

    else
    {
        /* Send cookie chunk */
        sctp_send_chunk_cookie( p_ass,
                                &p_ass->p_ep->this_address[saddr_index], p_ass->p_ep->this_port,
                                p_ass->transmit.that_port,
                                &addr_info,
                                p_ass->remote_tag,
                                &p_ass->cookie_list, p_ass->cookie_size );
    }

    /* Start timer, and enter in cookie sent state*/
    p_ass->transmit.remote[daddr_index].path_error_count = 0;

    /* Start the  Cookie timer */
    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMER,
                      p_ass->transmit.remote[daddr_index].rto,
                      0, /* Address index not used */
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
                      SCTP_NULL );

    p_ass->state = SCTPSTATE_COOKIESENT;
}


/*
 * FSM Trigger MSG-COOKIEACK
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgCookieAck
 *
 *    DESCRIPTION:
 *        Handles cookie ack message in unexpected state.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_MsgCookieAck(
    sctp_tcb_st      *p_ass )
{

    /* Discard message */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgCookieAck:"
                                             "Unexp CookieAck, Ignore Chunk " ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_CookE_MsgCookieAck
 *
 *    DESCRIPTION:
 *        Handles Cookie Ack message in cookie echoed state
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_CookE_MsgCookieAck(
    sctp_tcb_st    *p_ass )
{
    sctp_return_t              ret;
    sctp_U32bit                counter;
    sctp_sockaddr_st           *addr_list;
    sctp_endpoint_chg_data_ut  status_data;
    /* Bug Id 6 fix */
    /*sctp_U32bit                addr_index, remaining_time;*/
    sctp_U32bit                addr_index = 0;
    sctp_U32bit                remaining_time = 0;
    /* Bug Id 6 fix */

    sctpbuffer_st              *p_buf;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_CookE_MsgCookieAck" ) );

    /* Stop cookie timer, enter established state*/
    sctp_stop_timer(
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
        &remaining_time, SCTP_NULL );

    p_ass->state = SCTPSTATE_ESTABLISHED;
    p_ass->receive.state = SCTP_RX_STATE_NULL;

    p_ass->transmit.lowest_path_mtu = SCTP_MAX_MTUSIZE;

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        /* Get the path mtu for each of the destination address*/
        ret = sctp_get_pathmtu( p_ass,
                                &p_ass->transmit.remote[counter].that_address,
                                &p_ass->transmit.remote[counter].path_mtu );

        if( ret == SCTP_SUCCESS )
        {
            /*
             * This means path mtu retrieved successfully,
             * update lowest path mtu and start pmtu rediscover timer
             */
            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERPMTU,
                              SCTP_PMTU_REDISCOVER_TIME,
                              counter,
                              &p_ass->transmit.remote[counter].
                              dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
        }

        else
        {
            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERPMTU,
                              SCTP_PMTU_DISCOVER_FAIL_TIME,
                              counter,
                              &p_ass->transmit.remote[counter].dest_timer_id[SCTP_DESTTIMER_PMTU],
                              SCTP_NULL );
        }


        /* Update the lowest value of the lowest_path_mtu of an assoc to the
         * lowest value discovered
         */
        if( p_ass->transmit.remote[counter].path_mtu <
                p_ass->transmit.lowest_path_mtu )
        {
            p_ass->transmit.lowest_path_mtu
                = p_ass->transmit.remote[counter].path_mtu ;
        }

        /* SPR 4151 */
        p_ass->transmit.remote[counter].path_error_threshold
            = p_ass->config.path_max_retrans;
        p_ass->transmit.remote[counter].rto  = p_ass->config.rto_init;
        p_ass->transmit.remote[counter].allow_heartbeat = 1;

        /* Update last used parameter */
        __GETSYSTIME( &p_ass->transmit.remote[counter].last_used_time );

        sctp_slowstart_at_setup( p_ass, counter );

        p_ass->transmit.remote[counter].state  = SCTP_TX_STATE_NULL;

        /* Note : The Addresses already marked UP for communication should
         * not be marked COMMUNICATION_LOST */
        if( p_ass->transmit.remote[counter].status != SCTP_COMMUNICATION_UP )
        {
            p_ass->transmit.remote[counter].status = SCTP_COMMUNICATION_LOST;
        }
    }

    /* start inactivity timer */
    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMERINACTIVE,
                      ( p_ass->config.hb_interval
                        + p_ass->transmit.remote[p_ass->transmit.primary_address_index].rto ),
                      0, /* Address Index */
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
                      SCTP_NULL );

    SCTP_REINIT_ASSOCSTAT( p_ass );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ASSOC_INIT, 1 );

    /* Send an endpoint up notification to ULP*/

    /* status change data */
    status_data.endpoint_up.num_dest_addrs = p_ass->transmit.num_remote_address;

    addr_list = ( sctp_sockaddr_st * ) status_data.endpoint_up.list_of_addrs;

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        addr_list[counter] = p_ass->transmit.remote[counter].that_address;
    }

    status_data.endpoint_up.num_in_streams = p_ass->receive.num_in_streams;
    status_data.endpoint_up.num_out_streams = p_ass->transmit.num_out_streams;

    /* Send User notification*/
    sctp_ntfy_endpt_status_change( p_ass, SCTP_COMMUNICATION_UP,
                                   &status_data, E_NOERROR );

    if( p_ass->config.adaption_ind != SCTP_NULL_ADAPTION_IND )
    {
        sctp_ntfy_adaption_ind( p_ass, p_ass->config.adaption_ind );
    }

    /******************LINUX WARNING FIX***************************/

    addr_index = p_ass->transmit.primary_address_index;

    /******************LINUX WARNING FIX***************************/

    /*  SPR XXXX */
    if( lstCount( &p_ass->transmit.transmit_queue ) > 0 )
    {
        if( p_ass->total_sent > 0 )
        {
            sctp_U32bit saddr_index, daddr_index;
            sctp_error_t s_err;
            sctp_remote_addr_param_st *p_dest;

            /*some data has been sent bundled with cookie, start timer*/
            /* SPR 21107 changess */
            if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index, &daddr_index,
                                                                &s_err ) )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                            ( "sctpfsm_assoc_ShutS_MsgShut: Could not get compatible address"
                              " index for src and dst" ) );
                return;
            }

            /* SPR 21107 changess */

            p_dest =  &p_ass->transmit.remote[daddr_index];
            sctp_start_timer( p_ass, SCTPEVENT_TIMERDATA,
                              remaining_time, daddr_index,
                              &p_dest->dest_timer_id[SCTP_DESTTIMER_RTX],
                              SCTP_NULL );

            p_dest->state = SCTP_TX_STATE_AWTACK;
        }

        addr_index = p_ass->su_event.evt_cmduserdata.addr_index;
        sctp_assoc_state_machine( p_ass, SCTPEVENT_CMDUSERDATA );
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERBUNDLE,
            p_ass->config.bundling_time,
            addr_index,
            &p_ass->transmit.remote[addr_index].
            dest_timer_id[SCTP_DESTTIMER_BUNDLING],
            SCTP_NULL );
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERLIFETIME,
            SCTP_ONE_SECOND_TIME,
            0,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LIFETIME],
            SCTP_NULL );

    }

    /*********/

    /* Free the Cookie List */
    p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );

    while( p_buf )
    {
        sctp_free_transmit_buffer( p_ass, p_buf );
        p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );
    }

    /*Stop the init timeout timer*/
    sctp_stop_timer(
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
        NULL, NULL );



    if( ( p_ass->configured_local_primary_address_index !=
            p_ass->transmit.remote[p_ass->transmit.primary_address_index].src_addr_index ) ||
            ( p_ass->transmit.primary_address_index != p_ass->transmit.configured_primary_path_index ) )
    {

        p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].status = SCTP_COMMUNICATION_LOST;
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

    /* It is possible that ULP has given Address "A" of Peer in the CONNECT Command.
       So SCTP Stack Marked "A" as confirmed and sent INIT to the Peer SCTP.
       Peer SCTP sent Address List (A, B, C) in the INIT-ACK. so SCTP received 2 New
       Addresses which are not confirmed.
       SCTP stack will start the Path Verfication for these New Addresses.
       On the other Hand , if SCTP doesn't receive any New addresses in INIT-ACK,
       sctp_send_chunk_hb_path_verf() will do nothing.
    */
    sctp_send_chunk_hb_path_verf( p_ass );
}


/*
 * Trigger MSG-ABORT
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgAbort
 *
 *    DESCRIPTION:
 *    Handles abort message in SCTPSTATE_COOKIEWAIT, SCTPSTATE_COOKIESENT
 *    and SCTPSTATE_SHUTDOWNSENT state.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgAbort(
    sctp_tcb_st   *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgAbort" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

    if( ( p_ass->state == SCTPSTATE_COOKIEWAIT ) ||
            ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        /*
         * We just delete the association on which we have received the ABORT.
         * There is no need to delete the entire endpoint.
         */
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }

    else
    {
        /* In case state is SCTPSTATE_SHUTDOWNSENT delete ep as well */
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
    }

    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgAbort
 *
 *    DESCRIPTION:
 *        Handles abort message in established state.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_MsgAbort(
    sctp_tcb_st      *p_ass )
{
    sctpbuffer_st *p_buf = ( sctpbuffer_st * )
                           lstFirst( &p_ass->transmit.transmit_queue );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_MsgAbort" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

    if( p_buf == NULL )
    {
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }

    else
    {
        sctp_generic_handle_abort_message( p_ass );
    }
}



/*
 * FSM Trigger MSG-SHUT
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgShut
 *
 *    DESCRIPTION:
 *        Got Shutdown message in Cookie-Wait or Cookie-Echoed State.
 *    Also for Shutdown message in Shutdown-Receive or shutdown-Ack_sent
 *    State. Discard message.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_MsgShut(
    sctp_tcb_st  *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgShut" ) );

    /* Discard Shutdown Chunk */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "Shutdown Chunk in Unexpected State, Ignore Shutdown Chunk " ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/


}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutS_MsgShut
 *
 *    DESCRIPTION:
 *        Shutdown message in Shutdown sent state. Send Shutack, Start Shut
 *    timer, and move to Shut-ack-sent state.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutS_MsgShut(
    sctp_tcb_st   *p_ass )
{
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;
    sctp_error_t s_err;
    sctp_addrinfo_st   to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutS_MsgShut" ) );

    p_ass->shutdown_ack_retry = 0;

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutS_MsgShut: Could not get compatible address"
                      " index for src and dst" ) );

        return;
    }

    to.checksum     = p_ass->config.checksum_func;
    to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
    sctp_send_chunk_shutdownack( p_ass,
                                 &( p_ass->p_ep->this_address[saddr_index] ),
                                 p_ass->p_ep->this_port,
                                 p_ass->transmit.that_port,
                                 &to, p_ass->remote_tag );

    /*
     * stop already running shut down sent timer and start timer for
     * shutack sent
     */
    sctp_stop_timer(
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT], SCTP_NULL,
        SCTP_NULL );

    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMERSHUTDN,
                      p_ass->transmit.remote[daddr_index].rto,
                      daddr_index,
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                      SCTP_NULL );

    /*
     * If SO LINGER flag is set then start the linger timer.
     */
    if( p_ass->p_ep->so_linger.l_onoff )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctpfsm_assoc_Estbl_CmdTerm:Starting the linger timer" ) );

        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMERLINGER,
                          p_ass->p_ep->so_linger.l_linger,
                          0,   /* Address index does not matter in linger timeout. */
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LINGER],
                          SCTP_NULL );
    }

    p_ass->state = SCTPSTATE_SHUTDOWNACKSENT;

    /* Give a shutdown event received indication to the user */
    sctp_ntfy_shutdown_event( p_ass );

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgShut
 *
 *    DESCRIPTION:
 *        This function will handle shutdown message from peer for
 *    association in established, shutdown pending or shurdown
 *    Receivestate. Handle cumulative TSN received in Shutdown
 *    as SACK and update transmit queue.
 *    Look in the transmit queue if there is still any data chunk
 *    awaiting ack change association state to Shutdown-Received,
 *    else send shutdown-ack to peer and close the association
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Estbl_MsgShut(
    sctp_tcb_st   *p_ass )
{
    sctp_U32bit   addr_index = 0;
    sctpbuffer_st *p_buf = SCTP_NULL;
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;
    sctp_error_t  s_err;

    /* SPR_20411_FIX_START -- */
    sctp_return_t    ret = SCTP_FAILURE;
    /* SPR_20411_FIX_END -- */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_MsgShut" ) );

    /* SPR_20411_FIX_START -- */
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_sack.from_addr,
                                        &addr_index );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Estbl_MsgShut: -> Invalid remote address: " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC, &( p_ass->event.msg_sack.from_addr ) );
        return;
    }

    /* SPR_20411_FIX_END -- */

    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_SACK, addr_index );

    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Estbl_MsgShut: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    if( !p_buf )
    {
        sctp_addrinfo_st    to;
        p_ass->shutdown_ack_retry = 0;

        to.checksum     = p_ass->config.checksum_func;
        to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
        sctp_send_chunk_shutdownack( p_ass,
                                     &p_ass->p_ep->this_address[saddr_index], p_ass->p_ep->this_port,
                                     p_ass->transmit.that_port,
                                     &to, p_ass->remote_tag );

        /* Start timer and enter Shut-Ack-Sent state */
        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMERSHUTDN,
                          p_ass->transmit.remote[daddr_index].rto,
                          daddr_index,
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                          SCTP_NULL );

        /*
         * If SO LINGER flag is set then start the linger timer.
         */
        if( p_ass->p_ep->so_linger.l_onoff )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctpfsm_assoc_Estbl_CmdTerm:Starting the linger timer" ) );


            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERLINGER,
                              p_ass->p_ep->so_linger.l_linger,
                              daddr_index,
                              &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LINGER],
                              SCTP_NULL );
        }

        p_ass->state = SCTPSTATE_SHUTDOWNACKSENT;
    }

    else
    {
        p_ass->state = SCTPSTATE_SHUTDOWNRCVD;
    }

    /* Give a shutdown received indicatiaon to the user */
    sctp_ntfy_shutdown_event( p_ass );
}


/*
 * FSM Trigger MSG-SHUTACK
 */


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgShutAck
 *
 *    DESCRIPTION:
 *        This function will handle shutdown-ack message from peer for
 *    association in Cookie Wait or Cookie Sent State.
 *    Send Sutdown Complete with Verification Tag set to same as was received
 *    in Shutdown-Ack chunk.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgShutAck(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;
    sctp_addrinfo_st to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgShutAck" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Gener_MsgShutAck: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[daddr_index].that_address );
    /* send shut-complete , v-tag has to be what we got in shut-ack*/
    sctp_send_chunk_shutdowncomplete( p_ass,
                                      &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                                      p_ass->transmit.that_port,
                                      &to,
                                      p_ass->event.msg_shutack.verification_tag,
                                      1 ); /* Set T-bit here */

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutS_MsgShutAck
 *
 *    DESCRIPTION:
 *        This function will handle shutdown-ack message from peer for
 *    association in shutdown-sent or Shutdown-Ack-Sent state.
 *    Send Shutdown Complete and delete  association.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_ShutS_MsgShutAck(
    sctp_tcb_st   *p_ass )
{
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;
    sctp_addrinfo_st to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutS_MsgShutAck" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutS_MsgShutAck: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    /* match v-tag , If tags do not match drop the message */
    if( p_ass->local_tag != p_ass->event.msg_shutack.verification_tag )
    {
        return;
    }

    /* send shut-complete */
    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[daddr_index].that_address );
    sctp_send_chunk_shutdowncomplete( p_ass,
                                      &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                                      p_ass->transmit.that_port,
                                      &to, p_ass->remote_tag,
                                      0 ); /* T-bit 0*/

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_GRACEFUL_SHUTDOWN, 1 );

    sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
}


/*
 * FSM Trigger EVT-TIMER
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_CookW_EvtTimer
 *
 *    DESCRIPTION:
 *        This function handles Init timeout. while sending INIT chunk
 *    to peer, we start this timer. We should get a reply (Initack)
 *    within this time (RTO). Parameter init_retry is incremented.
 *    If after incrementing this exceeds protocol maximum value
 *    max_init_retrans. We delete this association else we send
 *    INIT chunk again.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_CookW_EvtTimer(
    sctp_tcb_st *p_ass )
{
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;
    sctp_U32bit   new_daddr_index = 0;
    sctp_U32bit   sctp_index = 0;
    sctp_U32bit   count = 0;
    sctp_sa_family_t af_dst;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_CookW_EvtTimer" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_CookW_EvtTimer: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    new_daddr_index = daddr_index;

    SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_T1_EXPIRY, 1 );

    /* update expired timer id in timer structure */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT] = SCTP_NULL_TIMER_ID;

    /* backoff init timer */
    p_ass->transmit.remote[daddr_index].rto *= 2;

    /* Check if the value of rto does not exceed maximum possible value*/
    if( p_ass->transmit.remote[daddr_index].rto > p_ass->config.rto_max )
    {
        p_ass->transmit.remote[daddr_index].rto = p_ass->config.rto_max;
    }

    /* Increment number of INIT retries */
    p_ass->init_retry++;

    /*Compare the init_retry with max_init_path_retrans.*/
    if( p_ass->init_retry_per_path == p_ass->config.max_init_path_retrans )
    {
        /*This is reached if max attempts have been made for particular peer address*/
        new_daddr_index = sctp_get_next_active_addr_index( p_ass, daddr_index );


        if( daddr_index == p_ass->transmit.num_remote_address - 1 )
        {
            af_dst = p_ass->transmit.remote[new_daddr_index].that_address.family;
            sctp_index = saddr_index + 1;

            for( count = 0; count < p_ass->p_ep->naddr; count++ )
            {
                if( sctp_index == p_ass->p_ep->naddr )
                {
                    sctp_index = 0;
                }

                if( af_dst == p_ass->p_ep->this_address[sctp_index].family )
                {
                    saddr_index = sctp_index;
                    break;
                }

                sctp_index++;
            }
        }

        p_ass->init_retry_per_path = 0;
    }

    daddr_index = new_daddr_index;

    p_ass->transmit.primary_address_index = daddr_index;
    p_ass->transmit.remote[daddr_index].src_addr_index = saddr_index;

    /* Send INIT again if allowed by the max number of INIT retransmissions
     * Otherwise delete association
     */
    /* SPR 4033 */
    if( p_ass->init_retry <= p_ass->config.max_init_retrans )
    {
        sctp_suppaddr_st     supp_addr;
        sctp_addrinfo_st     addr_info;

        addr_info.checksum      = p_ass->config.checksum_func;
        addr_info.p_sockaddr    =
            &( p_ass->transmit.remote[daddr_index].that_address );
        addr_info.p_info        = &( p_ass->config.nat_capable );

        sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr );

        sctp_send_chunk_init(
            p_ass,
            &( p_ass->p_ep->this_address[saddr_index] ),
            p_ass->p_ep->this_port,
            p_ass->transmit.that_port,
            &addr_info,
            p_ass->local_tag,
            p_ass->transmit.tsn,
            p_ass->config.num_out_streams,
            p_ass->config.num_in_streams,
            sctp_db_assoc_rx_window( p_ass ),
            p_ass->p_ep->naddr,
            &p_ass->p_ep->this_address[0],
            SCTP_FALSE,
            0UL,
            &supp_addr,
            p_ass->p_ep->hostname,
            p_ass->config.adaption_ind );

        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMER,
                          p_ass->transmit.remote[daddr_index].rto,
                          0,
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT],
                          SCTP_NULL );

        p_ass->init_retry_per_path++;
    }

    else
    {
        /* Delete the tcb not endpoint */
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_CookE_EvtTimer
 *
 *    DESCRIPTION:
 *        This function handles COOKIE timeout. While sending COOKIE
 *    chunk to peer,we start this timer.We should get a reply
 *    (COOKIEACK) within this time(RTO).Parameter cookie_retry
 *    is incremented.If after incrementing this exceeds protocol
 *    maximum value max_init_retrans.We delete this association
 *    else we send COOKIE chunk again.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_CookE_EvtTimer(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   new_daddr_index = 0;
    sctp_U32bit   sctp_index = 0;
    sctp_U32bit   daddr_index = 0;
    sctpbuffer_st *p_buf = SCTP_NULL;
    sctp_sa_family_t af_dst;
    sctp_U16bit      count;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_CookE_EvtTimer" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_CookE_EvtTimer: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_T1_EXPIRY, 1 );

    /* update expired timer id in timer structure */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE] = SCTP_NULL_TIMER_ID;

    /* backoff init timer */
    p_ass->transmit.remote[daddr_index].rto *= 2;

    /* Check if the value of rto does not exceed maximum possible value*/
    if( p_ass->transmit.remote[daddr_index].rto > p_ass->config.rto_max )
    {
        p_ass->transmit.remote[daddr_index].rto = p_ass->config.rto_max;
    }

    p_ass->cookie_retry++;

    /* SPR 4033 */
    /* CSR 1-2294822 max_init_retrans replaced by max_init_path_retrans*/
    if( p_ass->cookie_retry == p_ass->config.max_init_path_retrans )
    {
        /*This is reached if max attempts have been made for particular peer address */
        new_daddr_index = sctp_get_next_active_addr_index( p_ass, daddr_index );

        if( daddr_index == p_ass->transmit.num_remote_address - 1 )
        {
            af_dst = p_ass->transmit.remote[new_daddr_index].that_address.family;
            sctp_index = saddr_index + 1;

            for( count = 0; count < p_ass->p_ep->naddr; count++ )
            {
                if( sctp_index == p_ass->p_ep->naddr )
                {
                    sctp_index = 0;
                }

                if( af_dst == p_ass->p_ep->this_address[sctp_index].family )
                {
                    saddr_index = sctp_index;
                    break;
                }

                sctp_index++;
            }
        }

        p_ass->init_retry_per_path = 0;
    }

    daddr_index = new_daddr_index;

    p_ass->transmit.primary_address_index = daddr_index;
    p_ass->transmit.remote[daddr_index].src_addr_index = saddr_index;

    /* Send COOKIE-ECHO again if allowed by the max number of INIT retransmissions
     * Otherwise delete association.
     */
    if( p_ass->cookie_retry <= p_ass->config.max_init_retrans )
    {
        sctp_addrinfo_st   toaddr;

        toaddr.checksum     = p_ass->config.checksum_func;
        toaddr.p_sockaddr   = &( p_ass->transmit.remote[daddr_index].that_address );
        toaddr.p_info       = p_ass;
        p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->cookie_list );
        sctp_send_chunk_cookie( p_ass,
                                &( p_ass->p_ep->this_address[saddr_index] ),
                                p_ass->p_ep->this_port,
                                p_ass->transmit.that_port,
                                &toaddr,
                                p_ass->remote_tag,
                                &p_ass->cookie_list,
                                p_ass->cookie_size );

        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMER,
                          p_ass->transmit.remote[daddr_index].rto,
                          0,
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
                          SCTP_NULL );
    }

    else
    {
        /* Free the Cookie List */
        p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );

        while( p_buf )
        {
            sctp_free_transmit_buffer( p_ass, p_buf );
            p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );
        }

        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_buf );
    /* SPR 21107 changes end */
}

/***************************************************************************
 **    FUNCTION :
 **        sctpfsm_assoc_CookieWE_TimerInit
 **
 **    DESCRIPTION:This function hadles INIT timer expiry in CookieWait state
 **
 **    RETURN VALUE:
 **        None
 ***************************************************************************/
void
sctpfsm_assoc_CookieWE_TimerInit(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;
    sctp_error_t s_err;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_CookieWE_TimerInit" ) );

    /*
     ** Need to send Abort to the destination.
     ** So get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Could not get compatible address index for src and dst" ) );

        return;
    }

    /* send abort and clean up the association */
    sctp_send_chunk_abort( p_ass,
                           &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                           p_ass->transmit.that_port,
                           &( p_ass->transmit.remote[daddr_index].that_address ), p_ass->remote_tag,
                           SCTP_FALSE, 0, 0, NULL,           /*Changes for CSR#1-4776901 & CSR#1-4835338 */
                           0, p_ass->config.checksum_func );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

    sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );

}

/*
 * FSM Trigger EVT-TIMERSHUTDN
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutS_TimerShut
 *
 *    DESCRIPTION:
 *        This function handles SHUTDOWN timeout. While sending SHUTDOWN
 *    chunk to peer, we start this timer. We should get a reply
 *    (SHUTDOWNACK) within this time (RTO). Parameter shutdown_retry
 *    is incremented. If after incrementing this exceeds protocol
 *    maximum value assoc_max_retrans. We delete this association
 *    else we send SHUTDOWN chunk again.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutS_TimerShut(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit addr_index;
    sctp_U32bit next_addr_index;
    sctp_U32bit saddr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutS_TimerShut" ) );

    SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_T2_EXPIRY, 1 );

    /* update expired timer id in timer structure */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT] = SCTP_NULL_TIMER_ID;

    addr_index = p_ass->event.evt_timeout.addr_index;

    p_ass->transmit.remote[addr_index].rto *= 2;

    /* Check if the value of rto does not exceed maximum possible value*/
    if( p_ass->transmit.remote[addr_index].rto > p_ass->config.rto_max )
    {
        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
    }

    p_ass->error_count++;
    p_ass->shutdown_retry++;
    p_ass->transmit.remote[addr_index].path_error_count++;
    /* update src index */
    sctp_db_update_src_addr_index( p_ass, addr_index );

    /*
     * Send SHUTDOWN again if allowed by the max number of SHUTDOWN
     * retransmissions. Otherwise delete association
     */
    /* SPR 4038 */
    if( p_ass->shutdown_retry <= p_ass->config.assoc_max_retrans )
    {
        sctp_addrinfo_st        to;
        next_addr_index
            = sctp_get_next_active_addr_index( p_ass, addr_index );

        saddr_index
            = p_ass->transmit.remote[next_addr_index].src_addr_index;

        to.checksum     = p_ass->config.checksum_func;
        to.p_sockaddr   = &p_ass->transmit.remote[next_addr_index].that_address;
        sctp_send_chunk_shutdown( p_ass,
                                  &p_ass->p_ep->this_address[saddr_index],
                                  p_ass->p_ep->this_port, p_ass->transmit.that_port,
                                  &to, p_ass->remote_tag, p_ass->receive.cumtsn );

        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERSHUTDN,
            p_ass->transmit.remote[next_addr_index].rto,
            next_addr_index,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
            SCTP_NULL );
    }

    else
    {
        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutA_TimerShut
 *
 *    DESCRIPTION:
 *        This function handles SHUTDOWN-Ack timeout.While sending SHUTDOWN-Ack
 *    chunk to peer, we start this timer. We should get a reply
 *    (SHUTDOWN-COMPLETE) within this time (RTO). Parameter shutdown_ack_retry
 *    is incremented. If after incrementing this exceeds protocol
 *    maximum value assoc_max_retrans. We delete this association
 *    else we send SHUTDOWN-Ack chunk again.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutA_TimerShut(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit addr_index;
    sctp_U32bit next_addr_index;
    sctp_U32bit saddr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutA_TimerShut" ) );

    SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_T2_EXPIRY, 1 );

    /* update expired timer id in timer structure */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT] = SCTP_NULL_TIMER_ID;

    addr_index = p_ass->event.evt_timeout.addr_index;

    p_ass->transmit.remote[addr_index].rto *= 2;

    /* Check if the value of rto does not exceed maximum possible value */
    if( p_ass->transmit.remote[addr_index].rto > p_ass->config.rto_max )
    {
        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
    }

    p_ass->error_count++;
    p_ass->shutdown_ack_retry++;
    p_ass->transmit.remote[addr_index].path_error_count++;

    /* Send SHUTDOWNACK again if allowed by the max number of SHUTDOWNACK
     * retransmissions. Otherwise delete association
     */
    /* SPR 4038 */
    if( p_ass->shutdown_ack_retry <= p_ass->config.assoc_max_retrans )
    {
        sctp_addrinfo_st    to;
        next_addr_index
            = sctp_get_next_active_addr_index( p_ass, addr_index );

        /* update src index of this peer Address */
        sctp_db_update_src_addr_index( p_ass, addr_index );

        saddr_index
            = p_ass->transmit.remote[next_addr_index].src_addr_index;

        to.checksum     = p_ass->config.checksum_func;
        to.p_sockaddr   = &p_ass->transmit.remote[next_addr_index].that_address;
        sctp_send_chunk_shutdownack( p_ass,
                                     &p_ass->p_ep->this_address[saddr_index],
                                     p_ass->p_ep->this_port, p_ass->transmit.that_port,
                                     &to,
                                     p_ass->remote_tag );

        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERSHUTDN,
            p_ass->transmit.remote[next_addr_index].rto,
            next_addr_index,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
            SCTP_NULL );
    }

    else
    {
        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );
        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
    }
}


/*
 * FSM Trigger MSG_SACK
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Close_MsgSack
 *
 *    DESCRIPTION:
 *        Sack in Close State, consider it Out of the Blue and send Abort
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Close_MsgSack(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Close_MsgSack" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Close_MsgSack: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    /* Send Abort */
    sctp_send_chunk_abort( p_ass,
                           &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                           p_ass->transmit.that_port,
                           &p_ass->transmit.remote[daddr_index].that_address,
                           p_ass->event.msg_sack.ver_tag, SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                           0, NULL, 1, p_ass->config.checksum_func );

    p_ass->event.msg_sack.ootb_sack = 1;

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgSack
 *
 *    DESCRIPTION:
 *        Sack in Not to be handled state , Ignore Sack.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgSack(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgSack" ) );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "Sack Chunk in Unexpected State, Ignore Sack Chunk " ) );

    p_ass->event.msg_sack.ootb_sack = 0;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_CookE_MsgSack
 *
 *    DESCRIPTION:
 *        Sack in Cookie -Echoed state.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_CookE_MsgSack(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_CookE_MsgSack" ) );

    p_ass->event.msg_sack.ootb_sack = 0;
}


/***************************************************************************
 *   FUNCTION :
 *        sctpfsm_assoc_ShutP_MsgSack
 *
 *   DESCRIPTION:
 *       SACK in Shutdown-Pending State. Pass event to transmit state
 *   machine. Look if processing of this SACK does clear the
 *   transmit queue. If transmit queue is empty (all pending data
 *   is acked by this SACK chunk) send a Shutdown message to   peer.
 *
 *   RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_ShutP_MsgSack(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit   addr_index = 0;
    sctpbuffer_st *p_buf;
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;

    /* SPR_20411_FIX_START -- */
    sctp_return_t    ret = SCTP_FAILURE;
    /* SPR_20411_FIX_END -- */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutP_MsgSack" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutP_MsgSack: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    /* SPR_20411_FIX_START -- */
    /* SACK in Shutdown Pending state ... */
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_sack.from_addr,
                                        &addr_index );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutP_MsgSack: -> Invalid remote address: " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC, &( p_ass->event.msg_sack.from_addr ) );

        return;
    }

    /* SPR_20411_FIX_END -- */

    sctp_db_reset_error_counters( p_ass, addr_index );

    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_SACK, addr_index );
    p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

    if( p_buf == NULL )
    {
        sctp_addrinfo_st      to;
        p_ass->shutdown_retry = 0;

        /*  No pending data */
        to.checksum     = p_ass->config.checksum_func;
        to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
        sctp_send_chunk_shutdown( p_ass,
                                  &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                                  p_ass->transmit.that_port,
                                  &to, p_ass->remote_tag, p_ass->receive.cumtsn );

        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERSHUTDN,
            p_ass->transmit.remote[daddr_index].rto,
            daddr_index,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
            SCTP_NULL );

        p_ass->state = SCTPSTATE_SHUTDOWNSENT;
    }

    p_ass->event.msg_sack.ootb_sack = 0;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutR_MsgSack
 *
 *    DESCRIPTION:
 *        SACK in Shutdown-Receive State. Pass event to transmit
 *    state machine Look if it does clear the transmit queue.
 *    If transmit queue is empty (all pending  data is  acked by
 *    this SACK chunk) send a Shutdown-ack message to peer.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutR_MsgSack(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit   addr_index = 0;
    sctpbuffer_st *p_buf;
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;

    /* SPR_20411_FIX_START -- */
    sctp_return_t    ret = SCTP_FAILURE;
    /* SPR_20411_FIX_END -- */


    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutR_MsgSack" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutR_MsgSack: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    /* SPR_20411_FIX_START -- */
    /* SACK in Shutdown Received State ... */
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_sack.from_addr,
                                        &addr_index );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutR_MsgSack: -> Invalid remote address: " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC, &( p_ass->event.msg_sack.from_addr ) );

        return;
    }

    /* SPR_20411_FIX_END -- */

    sctp_db_reset_error_counters( p_ass, addr_index );

    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_SACK, addr_index );
    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    if( !p_buf )
    {
        sctp_addrinfo_st  to;
        p_ass->shutdown_ack_retry = 0;

        /*  No pending data*/
        to.checksum     = p_ass->config.checksum_func;
        to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
        sctp_send_chunk_shutdownack( p_ass,
                                     &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                                     p_ass->transmit.that_port, &to, p_ass->remote_tag );

        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMERSHUTDN,
                          p_ass->transmit.remote[daddr_index].rto,
                          daddr_index,
                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                          SCTP_NULL );

        p_ass->state = SCTPSTATE_SHUTDOWNACKSENT;
    }

    p_ass->event.msg_sack.ootb_sack = 0;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgSack
 *
 *    DESCRIPTION:
 *        Got SACK in Established State to be handled in
 *    Transmit state machine.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_MsgSack(
    sctp_tcb_st    *p_ass )
{
    /* Bug Id 6 fix */
#if 0
    sctp_U32bit addr_index;
    sctp_U32bit daddr_index;
    sctp_U32bit saddr_index;
#endif
    sctp_U32bit addr_index = 0;
    sctp_U32bit daddr_index = 0;
    sctp_U32bit saddr_index = 0;
    /* Bug Id 6 fix */
    /* SPR 21127 */
    /*sctp_U32bit dest_addr_index;*/
    sctp_U32bit counter;

    /* SPR_20411_FIX_START -- */
    sctp_return_t    ret = SCTP_FAILURE;
    /* SPR_20411_FIX_END -- */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_MsgSack" ) );

    /* SPR_20411_FIX_START -- */
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_sack.from_addr,
                                        &addr_index );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Estbl_MsgSack: -> Invalid remote address: " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC, &( p_ass->event.msg_sack.from_addr ) );

        return;
    }

    /* SPR_20411_FIX_END -- */

    for( saddr_index = 0 ; saddr_index < p_ass->p_ep->naddr ; saddr_index++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr(
                    &( p_ass->p_ep->this_address[saddr_index] ),
                    &( p_ass->event.msg_sack.to_addr ) ) )
        {
            /* The Address Match */
            break;
        }

    }

    /* Changes for CSR : 1-5056601 */
    if( p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT] )
        /* Changes for CSR : 1-5056601 */
    {
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT],
            SCTP_NULL,
            SCTP_NULL );
    }

    /* Changes for CSR : 1-5056601 */
    if( p_ass->transmit.configured_primary_path_index == addr_index &&
            p_ass->configured_local_primary_address_index == saddr_index )
        /* Changes for CSR : 1-5056601 */
    {
        /* Update the source address index for the destination. */
        p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].src_addr_index
            = p_ass->configured_local_primary_address_index;

        daddr_index = p_ass->transmit.configured_primary_path_index;
        saddr_index = p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].src_addr_index;

        /* SPR 21127 */
        /*for (counter = 0,dest_addr_index=daddr_index; counter < p_ass->transmit.num_remote_address; counter++) */
        for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
        {

            p_ass->transmit.remote[daddr_index].src_addr_index = saddr_index;

            daddr_index++;

            if( daddr_index == p_ass->transmit.num_remote_address )
            {
                daddr_index = 0;
            }

            saddr_index ++;

            if( saddr_index == p_ass->p_ep->naddr )
            {
                saddr_index = 0;
            }
        }

    }


    sctp_db_reset_error_counters( p_ass, addr_index );
    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_SACK, addr_index );

    p_ass->event.msg_sack.ootb_sack = 0;
}


/*
 * FSM Trigger CMD-USERDATA
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_CmdUserData
 *
 *    DESCRIPTION:
 *        Got USERDATA in Established State to be handled in Transmit
 *    state machine.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Estbl_CmdUserData(
    sctp_tcb_st     *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_CmdUserData" ) );

    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_USERDATA,
                           p_ass->su_event.evt_cmduserdata.addr_index );
}


/*
 * FSM Trigger EVT-TIMERDATA.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_EvtTimerData
 *
 *    DESCRIPTION:
 *        This function indicates retransmission timeout. This is handled
 *    in transmit state machine.
 *
 *    RETURN VALUE:
 *        None
**************************************************************************/
void
sctpfsm_assoc_Gener_EvtTimerData(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit addr_index = p_ass->event.evt_timeout.addr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_EvtTimerData" ) );

    /* update expired timer id in timer structure, destination timer*/
    p_ass->transmit.remote[addr_index].dest_timer_id[SCTP_DESTTIMER_RTX]
        = SCTP_NULL_TIMER_ID;

    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_TIMERDATA, addr_index );
}

/*Bug Id 199 changes s*/
/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Shut_EvtTimerHB
 *
 *    DESCRIPTION:
 *        This function handles heartbeat timeout event in following states:
 *        1. SHUTDOWN SENT
 *        2. SHUTDOWN ACK SENT
 *        3. SHUTOWN-RECEIVE
 *        4. SHUTOWN-PENDING 
 *    
 *        It will set the expired timer id as NULL.
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Shut_EvtTimerHB(
    sctp_tcb_st    *p_ass)
{
    sctp_U32bit      addr_index = 0;

    SCTP_TRACE(p_ass->p_ep, SCTP_DBG_TRC, ("sctpfsm_assoc_Shut_EvtTimerHB"));

    addr_index = p_ass->event.evt_timeout.addr_index;
  
    if ( p_ass->transmit.remote[addr_index].is_confirmed == SCTP_TRUE ) 
    {
        /* update expired timer id in timer structure, destination timer */
        p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK] = SCTP_NULL_TIMER_ID;
    }

}
/*Bug Id 199 changes e*/

/*
 * FSM Trigger EVT-TIMERHB
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtTimerHB
 *
 *    DESCRIPTION:
 *        This function handles heartbeat timeout event. Heartbeat
 *    message was sent for a destination address and no ack was received.
 *    following is performed.
 *
 *    1)Increment parameter path_error_count for the same destination if
 *    it is not already path_max_retrans. If after incrementing
 *    path_error_count it becomes equal to path_max_retrans,
 *    send destination status change notification to ULP.
 *
 *    2) Increment parameter assoc_error_count, If this becomes equal to
 *    assoc_max_retrans close the association and inform ULP of
 *    endpoint status change.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_EvtTimerHB(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit      addr_index, sctp_index = 0;
    sctp_sockaddr_st dst_addr;
    sctp_U32bit      saddr_index = 0;
    sctp_Boolean_t   primary_chg_flag = SCTP_FALSE;
    sctp_U32bit      daddr_index = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_EvtTimerHB" ) );

    addr_index = p_ass->event.evt_timeout.addr_index;

    p_ass->transmit.remote[addr_index].path_error_count++;

    /* backoff rto */
    p_ass->transmit.remote[addr_index].rto *= 2;

    if( p_ass->transmit.remote[addr_index].rto > p_ass->config.rto_max )
    {
        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
    }

    /* SPR 4033 */
    if( ( p_ass->transmit.remote[addr_index].path_error_count
            % p_ass->transmit.remote[addr_index].path_error_threshold == 0 )
            && p_ass->transmit.remote[addr_index].status == SCTP_COMMUNICATION_UP )
    {
        p_ass->transmit.remote[addr_index].status = SCTP_COMMUNICATION_LOST;

        /* SPR 6854 */
        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_init;

        /* check whether if the configured primary path index has gone down.
         * If yes, we start a LOCALHBEAT timer after the DEFAULT_HB_INTERVAL *
         * 5 miliseconds.
         */
        daddr_index = p_ass->transmit.configured_primary_path_index;

        /* Changes for CSR#1-4806351
         * There is no need to start local HB if there is only one destination address */
        if( ( p_ass->transmit.remote[daddr_index].src_addr_index == p_ass->configured_local_primary_address_index )
                && ( p_ass->transmit.num_remote_address > 1 ) && ( p_ass->p_ep->naddr > 1 ) )
        {
            sctp_start_timer(
                p_ass,
                SCTPEVENT_TIMER_LOCALHBEAT,
                /* SPR Starts for CSR 1-7626869 */
                SCTP_HB_INTERVAL_TIMEOUT,
                /* SPR Ends for CSR 1-7626869 */
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

        dst_addr = p_ass->transmit.remote[addr_index].that_address;

        /* notify service user */
        sctp_ntfy_dst_status_change( p_ass, &dst_addr, SCTP_ADDRESS_UNREACHABLE,
                                     E_NOERROR );

    }

    sctp_db_update_src_addr_index( p_ass, addr_index );

    /* We only increment the Error count of Association only for the
       Address marked as CONFIRMED */
    if( p_ass->transmit.remote[addr_index].is_confirmed == SCTP_TRUE )
    {
        /* update expired timer id in timer structure, destination timer */
        p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK] = SCTP_NULL_TIMER_ID;

        p_ass->error_count++;
    }

    else
    {
        p_ass->transmit.remote[addr_index].dest_timer_id[SCTP_PATH_VERF_TIMER] = SCTP_NULL_TIMER_ID;

        /* We should send the Heart Beat for Path Verification Purpose immediately. */
        sctp_send_chunk_hb_path_verf( p_ass );
    }

    /* SPR 4033 */
    if( p_ass->error_count > p_ass->config.assoc_max_retrans )
    {
        saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

        sctp_send_chunk_abort( p_ass,
                               &( p_ass->p_ep->this_address[saddr_index] ),
                               p_ass->p_ep->this_port,
                               p_ass->transmit.that_port,
                               &( p_ass->transmit.remote[addr_index].that_address ),
                               p_ass->remote_tag, SCTP_FALSE, 0, 0, NULL, 0,
                               p_ass->config.checksum_func );

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

        if( lstFirst( &p_ass->transmit.transmit_queue ) )
        {
            sctp_generic_handle_abort_message( p_ass );
        }

        else
        {
            sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
        }
    }

    if( primary_chg_flag == SCTP_TRUE )
    {
        /* Changes For CSR#1-5037386 */
        sctp_move_data_to_primary_addr_from_prev( p_ass, addr_index, sctp_index );
        /* Changes For CSR#1-5037386 Ends */
    }

}

/*
 * FSM Trigger EVT-TIMERINACTIVE.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_EvtTimerInac
 *
 *    DESCRIPTION:
 *        This means we have entered shutdown state. Ignore
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_EvtTimerInac(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_EvtTimerInac" ) );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "Stop Inactivity timer, association in shutdown state" ) );

    /* CSR_48008 changes start */
    /* update expired timer id in timer structure, Inactivity timer*/
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY] = SCTP_NULL_TIMER_ID;
    /* CSR_48008 changes End */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_idle_addr_index
 *
 *    DESCRIPTION:
 *        Get the index of the address which has been used earliest
 *    and also returns the time span in which no address will
 *    be idle so as to expire rto + HB timer value.
 *
 *    RETURN VALUE:
 *        SCTP_FAILURE/SCTP_SUCCESS
 *************************************************************************/
sctp_return_t
sctp_get_idle_addr_index(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    *addr_index,
    sctp_U32bit    *next_idle_time )
{
    sctp_time_t    curr_time;
    sctp_U32bit    counter;
    sctp_U32bit    tmp_time_diff;
    sctp_U32bit    max_idle_addr_index = 0;
    sctp_U32bit    max_idle_time = 0;
    sctp_Boolean_t addr_found = SCTP_FALSE;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_get_idle_addr_index" ) );

    *next_idle_time = 0;

    __GETSYSTIME( &curr_time );

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        if( ( p_ass->transmit.remote[counter].allow_heartbeat == 0 )
                || ( sctp_calculate_time_difference( &curr_time,
                                                     &p_ass->transmit.remote[counter].last_used_time, &tmp_time_diff )
                     != SCTP_SUCCESS ) )
        {
            continue;
        }

        if( tmp_time_diff < ( p_ass->transmit.remote[counter].rto
                              + p_ass->config.hb_interval ) )
        {
            /* update next idle time parameter */
            if( addr_found == SCTP_FALSE )
            {
                /* Storing the value least value of idle time*/
                if( *next_idle_time == 0 )
                {
                    *next_idle_time = p_ass->transmit.remote[counter].rto +
                                      p_ass->config.hb_interval - tmp_time_diff;
                }

                else
                {
                    if( ( p_ass->transmit.remote[counter].rto +
                            p_ass->config.hb_interval - tmp_time_diff )
                            < *next_idle_time )
                    {

                        *next_idle_time = p_ass->transmit.remote[counter].rto
                                          + p_ass->config.hb_interval - tmp_time_diff;
                    }
                }
            }/* if */

            continue;
        } /* End of if */

        /* Find the address whose timer has expired earliest
         * Got Idle destination, update idle address parameters
         */
        if( addr_found == SCTP_FALSE )
        {
            addr_found = SCTP_TRUE;
            max_idle_time = tmp_time_diff - p_ass->transmit.remote[counter].rto
                            - p_ass->config.hb_interval;
            max_idle_addr_index = counter;
        }

        else if( ( tmp_time_diff - p_ass->transmit.remote[counter].rto
                   - p_ass->config.hb_interval ) > max_idle_time )
        {
            max_idle_time = tmp_time_diff - p_ass->transmit.remote[counter].rto
                            - p_ass->config.hb_interval;
            max_idle_addr_index = counter;
        }
    } /* for */

    if( addr_found == SCTP_TRUE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_get_idle_addr_index: Idle Address found, Index = %u",
                      max_idle_addr_index ) );

        *addr_index = max_idle_addr_index;

        return ( SCTP_SUCCESS );
    }

    /* No Idle address found, return failure */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_get_idle_addr_index: NO Idle Address found, Next Idle Time = %u",
                  *next_idle_time ) );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtTimerInac
 *
 *    DESCRIPTION:
 *        Inactivity timer expired. Send heartbeat to peer and restart
 *    Inactivity and heartbeat ack timer if allow_heartbeat parameter
 *    is set.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_EvtTimerInac(
    sctp_tcb_st     *p_ass )
{
    sctp_U32bit   addr_index  = 0;
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   next_idle_time;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_EvtTimerInac" ) );

    addr_index = p_ass->event.evt_timeout.addr_index;

    /* update expired timer id in timer structure, destination timer*/
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY] = SCTP_NULL_TIMER_ID;

    /* Get Address Index with longest Idle Time */
    if( sctp_get_idle_addr_index( p_ass, &addr_index, &next_idle_time )
            == SCTP_SUCCESS )
    {
        sctp_addrinfo_st      to;
        /*
         * We got an address where HB need to be sent , Sent Hb and
         * start Hb and Inactivity timer
         */

        /*
         * Get  Source address index corresponding to destination.
         */
        saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

        to.checksum   = p_ass->config.checksum_func;
        to.p_sockaddr = &p_ass->transmit.remote[addr_index].that_address;
        sctp_send_chunk_heartbeat( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port, p_ass->transmit.that_port,
                                   addr_index,
                                   p_ass->remote_tag );

        /* SPR 21127 START*/
        SCTP_UNUSED_VARIABLE( to );
        /* SPR 21127 END*/

        /* If the Destination Address is not Confirmed, this Heartbeat will perform
           Path verification */
        if( p_ass->transmit.remote[addr_index].is_confirmed == SCTP_FALSE )
        {
            sctp_start_timer(
                p_ass,
                SCTPEVENT_TIMERHB,
                p_ass->transmit.remote[addr_index].rto,
                addr_index,
                &p_ass->transmit.remote[addr_index].dest_timer_id[SCTP_PATH_VERF_TIMER],
                SCTP_NULL );
        }

        else
        {
            /* Sending the HeartBeat for Path Validation */

            /*
             * restart heartbeat ack timer
             */
            sctp_stop_timer(
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
                SCTP_NULL, SCTP_NULL );

            sctp_start_timer(
                p_ass,
                SCTPEVENT_TIMERHB,
                p_ass->transmit.remote[addr_index].rto,
                addr_index,
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
                SCTP_NULL );
        }

        /*
         * Start inactivity timer equal to Hb.Interval + rto
         */

        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERINACTIVE,
            ( p_ass->transmit.remote[addr_index].rto
              + p_ass->config.hb_interval ),
            0,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
            SCTP_NULL );

        /* Update last user time */
        __GETSYSTIME( &p_ass->transmit.remote[addr_index].last_used_time );
    }

    else if( next_idle_time != 0 )
    {
        /* No Idle address found, determine time after which some HB need
           to be sent */
        sctp_start_timer(
            p_ass, SCTPEVENT_TIMERINACTIVE,
            next_idle_time, 0,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
            SCTP_NULL );
    }
}


/*
 * FSM Trigger MSG-DATA.
 */

/*
 *    These function handles data chunk from peer.
 *    At the receiver side per stream one message queue is maintained.
 *    This queue is implemented as a link list in which datagrams are kept
 *    sorted on the basis of SSN and TSN.
 *    Datagrams are not reassembled at the time they arrive, reassembling
 *    of datagrams is done at the time of SU invoking read request.
 *    To maintain a list of all the gaps in received TSN's, SCTP maintain them
 *    in a list (sacklist maintained per association). While sending sack to
 *    acknowledge a new data chunk this list is also sent to report gaps in
 *    TSN's received.  Send SACK to acknowledge DATA chunk received.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Close_MsgData
 *
 *    DESCRIPTION:
 *        Data in Close State, consider it Out of the Blue and send Abort
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Close_MsgData(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Close_MsgData" ) );

    /*
     * Need to send Abort to the destination.
     * So get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Close_MsgData: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    /* Send Abort */
    sctp_send_chunk_abort( p_ass,
                           &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                           p_ass->transmit.that_port,
                           &( p_ass->transmit.remote[daddr_index].that_address ),
                           p_ass->event.msg_data.ver_tag, SCTP_TRUE, SCTPERR_PROTOCOL_VIOLATION,
                           0, NULL, 1, p_ass->config.checksum_func );

    p_ass->event.msg_data.ootb_data = 1;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgData
 *
 *    DESCRIPTION:
 *        Data in Not to be handled state , Ignore Data.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgData(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgData" ) );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Gener_MsgData: Data Chunk in Unexpected State, "
                  "Ignore Data Chunk " ) );

    p_ass->event.msg_data.ootb_data = 0;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutP_MsgData
 *
 *    DESCRIPTION: Handles DATA message in SHUTDOWN pending state
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutP_MsgData(
    sctp_tcb_st    *p_ass )
{
    sctp_Boolean_t overrule_delayack;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutP_MsgData" ) );

    overrule_delayack = SCTP_TRUE;
    sctp_process_data_message( p_ass, overrule_delayack );

    /* SPR 4038 */
    p_ass->shutdown_retry = 0;
    p_ass->event.msg_data.ootb_data = 0;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutS_MsgData
 *
 *    DESCRIPTION:
 *        Immediately ack Data chunk with Sack, send Shutdown again and
 *    restart shutdown send timer.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutS_MsgData(
    sctp_tcb_st    *p_ass )
{
    /* overrule delay ack in this case */
    sctp_Boolean_t    overrule_delayack;
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;
    sctp_addrinfo_st  to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutS_MsgData" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutS_MsgData: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    overrule_delayack = SCTP_TRUE;
    sctp_process_data_message_in_shutS( p_ass, overrule_delayack );

    /* send shut down message */
    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[daddr_index].that_address );
    sctp_send_chunk_shutdown( p_ass,
                              &( p_ass->p_ep->this_address[saddr_index] ),
                              p_ass->p_ep->this_port,
                              p_ass->transmit.that_port,
                              &to, p_ass->remote_tag, p_ass->receive.cumtsn );

    /* restart the shutdown timer */
    sctp_stop_timer(
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
        SCTP_NULL, SCTP_NULL ); /* time since timer started */

    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMERSHUTDN,
                      p_ass->transmit.remote[daddr_index].rto,
                      daddr_index,
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                      SCTP_NULL );

    /* SPR 4038 */
    p_ass->shutdown_retry = 0;
    p_ass->event.msg_data.ootb_data = 0;
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgData
 *
 *    DESCRIPTION: Handles Data message in ESTABLISHED state
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Estbl_MsgData(
    sctp_tcb_st    *p_ass )
{
    sctp_Boolean_t  overrule_delayack = SCTP_FALSE;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_MsgData" ) );

    if( p_ass->event.msg_data.last_chunk_flag == SCTP_TRUE )
    {
        p_ass->receive.first_data_flag++ ;
    }

    /* SPR 20287 */
    if( ( SCTP_ACK_POLICY  == 0 ) || ( SCTP_ACK_POLICY == 2 ) )
    {
        /* SPR 20287 */
        if( p_ass->receive.first_data_flag >= p_ass->config.sack_frequency )
        {
            p_ass->receive.overrule_delayack = SCTP_TRUE;
            p_ass->receive.first_data_flag = 0;
            overrule_delayack = SCTP_TRUE;
        }

        /* SPR 20287 */
    }

    /* SPR 20287 */

    sctp_process_data_message( p_ass, overrule_delayack );

    p_ass->event.msg_data.ootb_data = 0;
}


/*
 * FSM Trigger MSG-INIT.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgInit
 *
 *    DESCRIPTION:
 *        This function handles duplicate Init,
 *    This is a case of initialization collision. Send Initack in
 *    response to Init received from peer. Verification tag value
 *    in Initack should be equal to one received in Init and Initiate
 *    tag value in Initack should be same to Initiate tag value sent
 *    earlier in Init. Here local and remote tie tag are also being filled.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_MsgInit(
    sctp_tcb_st    *p_ass )
{
    sctp_chunk_cookie_st   cookie;
    sctp_U32bit            local_tag;
    sctp_U32bit            counter;
    sctp_U8bit             digest[SCTP_KEYSIZE];
    sctp_U32bit            adaption_ind = 0;
    sctp_chunk_init_st     *p_init_chunk = p_ass->event.msg_init.init_chunk;
    sctp_addrinfo_st       addr_info;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgInit" ) );

    sctp_init_chunk_cookie( &cookie );

    __GETSYSTIME( &cookie.time_stamp );

    cookie.life_time = p_ass->config.valid_cookie_life;

    /* send initack,send init tag as local tag*/
    local_tag = p_ass->local_tag;

    cookie.local_init_parameters.fixed_params.init_tag = local_tag;

    cookie.local_init_parameters.fixed_params.a_rwnd
        = sctp_db_get_dafault_rwnd();

    /* Do Stream negotiation(bakeoff change) */
    /* SPR 20571 Starts for CSR 1-7241261 */
    if( p_ass->receive.num_in_streams >
            p_init_chunk->fixed_params.num_out_streams )
    {
        cookie.local_init_parameters.fixed_params.num_in_streams
            = p_init_chunk->fixed_params.num_out_streams;
    }

    else
    {
        cookie.local_init_parameters.fixed_params.num_in_streams
            = p_ass->receive.num_in_streams;
    }

    /* SPR 20571 ends for CSR 1-7241261 */

    if( p_ass->transmit.num_out_streams >
            p_init_chunk->fixed_params.num_in_streams )
    {
        cookie.local_init_parameters.fixed_params.num_out_streams
            = p_init_chunk->fixed_params.num_in_streams;
    }

    else
    {
        cookie.local_init_parameters.fixed_params.num_out_streams
            = p_ass->transmit.num_out_streams;
    }

    cookie.local_init_parameters.fixed_params.init_tsn
        = p_ass->transmit.tsn;

    /* Set the Number of local addresses  */

    /*v64*/
    cookie.local_init_parameters.num_addrs
        = p_ass->p_ep->naddr;

    do
    {
        cookie.local_tie_tag = __RANDOM();  /*send random number intead of actual
                                            *tie tag.*/
    }
    while( 0 == cookie.local_tie_tag );

    /*Store this random number*/
    p_ass->local_virtual_tie_tag = cookie.local_tie_tag;

    for( counter = 0; counter < p_ass->p_ep->naddr; counter++ )
    {
        cookie.local_init_parameters.addr_list[counter] =
            p_ass->p_ep->this_address[counter];
    }

    /*v64*/
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

    /* for multi-homing case , number of addresses in association
     * and init chunk should match
     */
    cookie.remote_init_parameters.num_addrs
        = p_init_chunk->num_addrs;

    for( counter = 0; counter < p_init_chunk->num_addrs; counter++ )
    {
        cookie.remote_init_parameters.addr_list[counter]
            = p_init_chunk->addr_list[counter];
    }

    /* Store the Peer's Address to which INIT-ACK is being sent. */
    cookie.that_address = p_init_chunk->p_tl_packet->src_addr;

    for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
    {
        if( p_init_chunk->opt_params[counter].param_type
                == SCTPVARPAR_ADAPTIONIND )
        {
            adaption_ind = p_init_chunk->opt_params[counter].
                           param_data.adaption_ind;
            break;
        }
    }

    cookie.remote_init_parameters.adaption_ind = adaption_ind;

    cookie.remote_init_parameters.hostname_present = SCTP_FALSE;

    for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
    {
        if( p_init_chunk->opt_params[counter].param_type
                == SCTPVARPAR_HOSTNAME )
        {
            sctp_U32bit count = 0;
            sctp_U8bit *p_hostname = p_init_chunk->opt_params[counter].
                                     param_data.hostname;

            cookie.remote_init_parameters.hostname_present = SCTP_TRUE;

            for( count = 0; p_hostname[count] != '\0'; count++ )
            {
                cookie.remote_init_parameters.hostname[count] =
                    p_hostname[count];
            }

            cookie.remote_init_parameters.hostname[count] = '\0';
            break;
        }
    }

    do
    {
        cookie.peer_tie_tag  = __RANDOM(); /*send random number intead of actual
                                            *tie tag.*/
    }
    while( 0 == cookie.peer_tie_tag );

    /*Store this random number*/
    p_ass->peer_virtual_tie_tag = cookie.peer_tie_tag;

    cookie.this_port = p_ass->p_ep->this_port;
    cookie.dest_port = p_ass->transmit.that_port;


    md5digest( ( sctp_U8bit * )( &cookie ), sizeof( cookie ), digest );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        cookie.digest[counter] = digest[counter];
    }

    /* send init reply */

    addr_info.checksum      = p_ass->config.checksum_func;
    addr_info.p_sockaddr    = &( p_init_chunk->p_tl_packet->src_addr );
    addr_info.p_info        = &( p_ass->config.nat_capable );

    sctp_send_chunk_initack( p_ass,
                             &( p_init_chunk->p_tl_packet->dst_addr ),
                             p_ass->p_ep->this_port,
                             p_ass->transmit.that_port, &addr_info,
                             p_init_chunk->fixed_params.init_tag,
                             &cookie, sizeof( sctp_chunk_cookie_st ), SCTP_FALSE, 0, NULL,
                             p_ass->p_ep->hostname, p_ass->config.adaption_ind );
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Restart_MsgInit
 *
 ***************************************************************************
 *    DESCRIPTION:
 *        This function handles duplicate Init,
 *    This will process init in Established, shutdown-sent, showndown-receive
 *    and shutdown-pending state.
 *    Send Initack in response to Init received from peer. Verification tag
 *    value in Initack should be equal to one received in Init and Initiate
 *    tag value in Initack should be new random value .
 *    Here local and remote tie tag are also being filled.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void sctpfsm_assoc_Restart_MsgInit(
    sctp_tcb_st    *p_ass )
{
    sctp_chunk_cookie_st   cookie;
    sctp_U32bit            local_tag;
    sctp_U32bit            counter;
    sctp_U8bit             digest[SCTP_KEYSIZE];
    sctp_U32bit            adaption_ind = 0;
    sctp_chunk_init_st     *p_init_chunk = p_ass->event.msg_init.init_chunk;
    sctp_addrinfo_st       addr_info;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Restart_MsgInit" ) );


    /* do stream negotiate , */
    if( p_ass->transmit.num_out_streams
            > p_init_chunk->fixed_params.num_in_streams )
    {
        sctp_send_chunk_abort( p_ass,
                               &( p_init_chunk->p_tl_packet->dst_addr ), p_ass->p_ep->this_port,
                               p_ass->transmit.that_port,
                               &( p_init_chunk->p_tl_packet->src_addr ),
                               p_init_chunk->fixed_params.init_tag,
                               SCTP_TRUE, SCTPERR_INVALIDSTREAM, 0, NULL, 0,
                               p_ass->config.checksum_func );

        return;
    }

    sctp_init_chunk_cookie( &cookie );

    __GETSYSTIME( &cookie.time_stamp );

    cookie.life_time = p_ass->config.valid_cookie_life;

    /* send initack,send new init tag */
    do
    {
        local_tag = __RANDOM();
    }
    while( 0 == local_tag );

    cookie.local_init_parameters.fixed_params.init_tag = local_tag;
    cookie.local_init_parameters.fixed_params.a_rwnd
        = sctp_db_get_dafault_rwnd();
    cookie.local_init_parameters.fixed_params.num_in_streams
        = p_ass->receive.num_in_streams;
    cookie.local_init_parameters.fixed_params.num_out_streams
        = p_ass->transmit.num_out_streams;
    cookie.local_init_parameters.fixed_params.init_tsn
        = p_ass->transmit.tsn;

    /*v64*/
    cookie.local_init_parameters.num_addrs
        = p_ass->p_ep->naddr;

    do
    {
        cookie.local_tie_tag = __RANDOM();  /*send random number intead of actual
                                            *tie tag.*/
    }
    while( 0 == cookie.local_tie_tag );

    /*Store this random number*/
    p_ass->local_virtual_tie_tag = cookie.local_tie_tag;


    for( counter = 0; counter < p_ass->p_ep->naddr; counter++ )
    {
        cookie.local_init_parameters.addr_list[counter] =
            p_ass->p_ep->this_address[counter];
    }

    /*v64*/

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

    /* for multi-homing case , number of addresses in association
     * and init chunk should match
     */

    cookie.remote_init_parameters.num_addrs
        = p_ass->transmit.num_remote_address;

    cookie.remote_init_parameters.num_addrs
        = p_init_chunk->num_addrs;

    for( counter = 0; counter < p_init_chunk->num_addrs; counter++ )
    {
        cookie.remote_init_parameters.addr_list[counter]
            = p_init_chunk->addr_list[counter];
    }

    /* Store the Peer's Address to which INIT-ACK is being sent. */
    cookie.that_address = p_init_chunk->p_tl_packet->src_addr;

    for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
    {
        if( p_init_chunk->opt_params[counter].param_type
                == SCTPVARPAR_ADAPTIONIND )
        {
            adaption_ind = p_init_chunk->opt_params[counter].
                           param_data.adaption_ind;
            break;
        }
    }

    cookie.remote_init_parameters.adaption_ind = adaption_ind;

    cookie.remote_init_parameters.hostname_present = SCTP_FALSE;

    for( counter = 0; counter < p_init_chunk->num_opt_params; counter++ )
    {
        if( p_init_chunk->opt_params[counter].param_type
                == SCTPVARPAR_HOSTNAME )
        {
            sctp_U32bit count = 0;
            sctp_U8bit *p_hostname = p_init_chunk->opt_params[counter].
                                     param_data.hostname;

            cookie.remote_init_parameters.hostname_present = SCTP_TRUE;

            for( count = 0; p_hostname[count] != '\0'; count++ )
            {
                cookie.remote_init_parameters.hostname[count] =
                    p_hostname[count];
            }

            cookie.remote_init_parameters.hostname[count] = '\0';
            break;
        }
    }

    do
    {
        cookie.peer_tie_tag  = __RANDOM(); /*send random number intead of actual
                                            *tie tag.*/
    }
    while( 0 == cookie.peer_tie_tag );

    /*Store this random number*/
    p_ass->peer_virtual_tie_tag = cookie.peer_tie_tag;

    cookie.this_port =  p_ass->p_ep->this_port;
    cookie.dest_port = p_ass->transmit.that_port;


    md5digest( ( sctp_U8bit * )( &cookie ), sizeof( cookie ), digest );

    for( counter = 0; counter < SCTP_KEYSIZE; counter++ )
    {
        cookie.digest[counter] = digest[counter];
    }

    /* send init reply */
    addr_info.checksum   = p_ass->config.checksum_func;
    addr_info.p_sockaddr = &( p_init_chunk->p_tl_packet->src_addr );
    addr_info.p_info     = &( p_ass->config.nat_capable );

    sctp_send_chunk_initack( p_ass,
                             &( p_init_chunk->p_tl_packet->dst_addr ), p_ass->p_ep->this_port,
                             p_ass->transmit.that_port,
                             &addr_info,
                             p_init_chunk->fixed_params.init_tag,
                             &cookie, sizeof( sctp_chunk_cookie_st ), SCTP_FALSE, 0, NULL,
                             p_ass->p_ep->hostname, p_ass->config.adaption_ind );
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutA_MsgInit
 *
 *    DESCRIPTION:
 *       section 9.2 of draft version 13
 *    Ignore Init and resend ShutdownAck
 *
 *    RETURN VALUE:
 *       None
 **************************************************************************/
void sctpfsm_assoc_ShutA_MsgInit(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit            saddr_index = 0;
    sctp_U32bit            daddr_index = 0;
    sctp_addrinfo_st       to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutA_MsgInit" ) );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "Init Chunk in Shutdown Ack Sent State,Ignore Init and Send ShutAck" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_ShutA_MsgInit: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    to.checksum     = p_ass->config.checksum_func;
    to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
    sctp_send_chunk_shutdownack( p_ass,
                                 &( p_ass->p_ep->this_address[saddr_index] ),
                                 p_ass->p_ep->this_port,
                                 p_ass->transmit.that_port,
                                 &to,
                                 p_ass->remote_tag );

    /* restart shutack timer */
    sctp_stop_timer(
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT], SCTP_NULL,
        SCTP_NULL );

    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMERSHUTDN,
                      p_ass->transmit.remote[daddr_index].rto,
                      daddr_index,
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                      SCTP_NULL );

}

/*
 * FSM Trigger MSG-HEARTBEATACK.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgHBAck
 *
 *    DESCRIPTION:
 *        Handle heartbeat ack. From the time stamp embedded in the
 *    Heartbeat-ack round trip time is calculated. Extract
 *    destination address information from the heartbeat-ack
 *    message. Based on the RTT calculated update retransmission
 *    parameters for the destination address.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgHBAck(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit      counter;
    sctp_U32bit      time_value;
    sctp_U32bit      daddr_index;
    sctp_U32bit      saddr_index;
    sctp_U32bit      dest_addr_index;
    sctp_time_t      now;
    sctp_chunk_hb_st *hback_chunk = p_ass->event.msg_hback.hback_chunk;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgHBAck" ) );

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        if( SCTP_SUCCESS == sctp_cmp_addr(
                    &( p_ass->transmit.remote[counter].that_address ),
                    &( hback_chunk->dest_addr ) ) )
        {
            /* The Address Match */
            break;
        }
    }

    if( counter == p_ass->transmit.num_remote_address )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgHBAck : "
                                                 "Invalid address : " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC,
                         ( &( hback_chunk->dest_addr ) ) );

        return ; /* Invalid addr discard message */
    }

    else
    {
        dest_addr_index = counter;
    }

    /* HB-ACK is for a Unconfirmed Address */
    if( p_ass->transmit.remote[dest_addr_index].is_confirmed == SCTP_FALSE )
    {
        /* Verify that 64-bit Ramdom Nonce in HB-ACK is same as the Nonce sent in HB. */
        if( p_ass->transmit.remote[dest_addr_index].nonce[0] != p_ass->event.msg_hback.hback_chunk->nonce[0] ||
                p_ass->transmit.remote[dest_addr_index].nonce[1] != p_ass->event.msg_hback.hback_chunk->nonce[1] )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgHBAck : "
                                                     "Invalid Nonce received in HB-ACK" ) );

            return ; /* Invalid HB-ACK since 64-bit random nonce doesn't match */
        }

        /* Timer for Path Verification may be running, stop it. */
        sctp_stop_timer(
            &p_ass->transmit.remote[dest_addr_index].dest_timer_id[SCTP_PATH_VERF_TIMER],
            SCTP_NULL,
            SCTP_NULL );

        if( p_ass->conn_accepted == SCTP_TRUE )
        {
            /* notify service user that the Address is now Confirmed. */
            sctp_ntfy_dst_status_change( p_ass,
                                         &p_ass->transmit.remote[dest_addr_index].that_address,
                                         SCTP_ADDRESS_CONFIRMED, E_NOERROR );
        }

        /* Mark Address as CONFIRMED */
        p_ass->transmit.remote[dest_addr_index].is_confirmed = SCTP_TRUE;

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctpfsm_assoc_Gener_MsgHBAck :: Path Verified for Address: " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC,
                         ( &( p_ass->transmit.remote[dest_addr_index].that_address ) ) );
    }

    else
    {
        /*
         * Stop heartbeat Ack Timer
         */
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK], SCTP_NULL,
            SCTP_NULL );
    }

    /* If the local heartbeat timer is running then stop the timer and
     * initiate a fallback to the configured primary path index
     */
    /* Changes for CSR : 1-5056601 */
    if( p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT] )
        /* Changes for CSR : 1-5056601 */
    {
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT],
            SCTP_NULL,
            SCTP_NULL );
    }

    /* Changes for CSR : 1-5056601 */
    if( p_ass->transmit.configured_primary_path_index == hback_chunk->dest_addr_id
            && p_ass->configured_local_primary_address_index == hback_chunk->src_addr_id
            && p_ass->transmit.configured_primary_path_index < SCTP_MAX_TRANSPORT_ADDR ) /* klockwork 6.2.4 fix */
        /* changes for klocwork fix for IBM delivery*/
        /* Changes for CSR : 1-5056601 */
    {

        /* Update the source address index for the destination. */
        p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].src_addr_index =
            p_ass->configured_local_primary_address_index;

        daddr_index = p_ass->transmit.configured_primary_path_index;
        saddr_index = p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].src_addr_index;

        for( counter = 0; ( counter < p_ass->transmit.num_remote_address )
                && ( p_ass->transmit.num_remote_address <= SCTP_MAX_TRANSPORT_ADDR ); counter++ ) /* klckwork 6.2.4 fix */

        {

            p_ass->transmit.remote[daddr_index].src_addr_index = saddr_index;

            daddr_index++;

            if( daddr_index == p_ass->transmit.num_remote_address )
            {
                daddr_index = 0;
            }

            saddr_index ++;

            if( saddr_index == p_ass->p_ep->naddr )
            {
                saddr_index = 0;
            }
        }
    }


    __GETSYSTIME( &now );

    if( sctp_calculate_time_difference( &now, &hback_chunk->time_stamp,
                                        &time_value )  == SCTP_SUCCESS )
    {
        sctp_update_rto( p_ass, dest_addr_index , time_value );
    }

    /* reset retry count used to declare endpoint unreachable,
     * send dest status active only if prev status was inactive
     */
    sctp_db_reset_error_counters( p_ass, dest_addr_index );
}


/*
 * FSM Trigger MSG-COOKIE.
 */


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgCookie
 *
 *    DESCRIPTION:
 *        This function handles duplicate Cookie.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgCookie(
    sctp_tcb_st     *p_ass )
{
    sctp_chunk_cookie_st      *cookie;
    sctp_U32bit               cookie_local_tag;
    sctp_U32bit               cookie_remote_tag;
    sctp_U32bit               cookie_local_tie_tag;
    sctp_U32bit               cookie_remote_tie_tag;
    sctp_U32bit               counter;
    sctp_U32bit               counter1;
    sctpbuffer_st             *p_buf;
    sctp_return_t             ret;
    sctp_sockaddr_st          *addr_list;
    sctp_endpoint_chg_data_ut status_data;
    sctp_U32bit               saddr_index = 0;
    sctp_U32bit               daddr_index = 0;
    sctp_U32bit               saddr_index_new = 0; /*SPR FIX 20406*/
    sctp_U32bit               daddr_index_new = 0; /*SPR FIX 20406*/
    sctp_addrinfo_st          toaddr;
    sctp_suppaddr_st          supp_addr_type;
    sctp_U32bit               dest_addr_counter = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgCookie" ) );
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Gener_MsgCookie: Handle Duplicate Cookie ... " ) );

    cookie = p_ass->event.msg_cookie.cookie_chunk;

    cookie_local_tag
        = cookie->local_init_parameters.fixed_params.init_tag;

    cookie_remote_tag
        = cookie->remote_init_parameters.fixed_params.init_tag;

    cookie_local_tie_tag = cookie->local_tie_tag;
    cookie_remote_tie_tag = cookie->peer_tie_tag;

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Gener_MsgCookie: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    toaddr.checksum     = p_ass->config.checksum_func;
    toaddr.p_sockaddr   = &( p_ass->transmit.remote[daddr_index].that_address );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Gener_MsgCookie: Tags Values  .........::"
                  "\nCookie_local_tag            :%u,"
                  "\nCookie_remote_tag           :%u,"
                  "\nCookie_local_tie_tag        :%u,"
                  "\nCookie_Remote_tie_tag       :%u,"
                  "\nAssoc_Local_tag             :%u,"
                  "\nAssoc_remote_tag            :%u", cookie_local_tag, cookie_remote_tag,
                  cookie_local_tie_tag, cookie_remote_tie_tag, p_ass->local_tag,
                  p_ass->remote_tag ) );

    if( ( p_ass->local_tag != cookie_local_tag )
            && ( p_ass->remote_tag != cookie_remote_tag )
            && ( p_ass->local_virtual_tie_tag == cookie_local_tie_tag )
            && ( p_ass->peer_virtual_tie_tag == cookie_remote_tie_tag ) )
    {
        if( p_ass->state == SCTPSTATE_SHUTDOWNACKSENT )
        {

            sctp_addrinfo_st   to;
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctpfsm_assoc_Gener_MsgCookie: Peer Restart in Shutdown Sent"
                          " State,Send Shutack" ) );

            to.checksum     = p_ass->config.checksum_func;
            to.p_sockaddr   = &p_ass->transmit.remote[daddr_index].that_address;
            sctp_send_chunk_shutdownack( p_ass,
                                         &( p_ass->p_ep->this_address[saddr_index] ),
                                         p_ass->p_ep->this_port,
                                         p_ass->transmit.that_port,
                                         &to, p_ass->remote_tag );

            sctp_send_chunk_error( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &to, p_ass->remote_tag,
                                   SCTPERR_COOKIERECVSHUTDOWN, 0, NULL );

            return;
        }

        /* peer restart case,modify local parameters */
        if( p_ass->restart_option == SCTP_RESTARTOPTION_REQUE )
        {
            sctp_U32bit cong_event_sent = 0;

            /* REQUE all pending datagrams */
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctpfsm_assoc_Gener_MsgCookie: Peer Restart All Pending Data"
                          " requed" ) );
            /*SPR 5222*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT

            if( SCTP_NULL_TIMER_ID !=
                    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "sctpfsm_assoc_Gener_MsgCookie: ASCONF retransmit aborted"
                              " requed" ) );
                SCTP_WAKEUP( p_ass->p_ep->asconf_cond );
            }

#endif
            /*SPR 5222*/
            /* Initialise all but tx queue */
            sctp_db_cleanup_queues( p_ass, SCTP_FALSE );

            if( 0 != p_ass->receive.congestion_event_sent )
            {
                p_ass->receive.congestion_event_sent = 0;
                /* notify su of change in congestion level */
                sctp_ntfy_recv_congestion( p_ass, SCTP_CONGLEVEL_NOCONG );
            }


            /* CSR#1-4913593: sctp_db_delete_assoc_verf() call is moved here
             * from its earlier position few statements below as when this
             * function was called from earlier position p_ass->local_tag
             * was already updated and this function call became in-effective
             * to cleanup association entry from hash corresponding to older
             * v-tag.This was resulting in a leak in assoc-varf hash table */

            /*Delete the assoc with old vtag*/
            sctp_db_delete_assoc_verf( p_ass );

            /*  Initialise association structure */
            cong_event_sent = p_ass->transmit.congestion_event_sent;
            sctp_db_clean_association( p_ass, SCTP_TRUE );
            p_ass->transmit.congestion_event_sent = cong_event_sent;


            /*
             * Now all data structures (except transmit queue) are initialised,
             * All timer are stopped also
             */

            sctp_db_update_assoc_on_cookie( p_ass, cookie );

            p_ass->conn_accepted = SCTP_TRUE;

            /*Delete the assoc with old vtag*/
            /* sctp_db_delete_assoc_verf(p_ass); CSR#1-4913593: this call is
                                                moved above*/

            /*Add the assoc with new vtag*/
            p_ass->local_tag = cookie_local_tag;
            sctp_db_add_assoc_verf( p_ass );

            /* reque all pending datagrams */
            p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

            /* SPR FIX 20406 starts*/

            for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
            {
                /* If this is Peer Address to which INIT-ACK was sent,
                 * mark it as confirmed. */
                if( SCTP_SUCCESS == sctp_cmp_addr(
                            &( p_ass->transmit.remote[counter].that_address ),
                            &( cookie->that_address ) ) )
                {
                    p_ass->transmit.remote[counter].is_confirmed = SCTP_TRUE;


                    if( counter != p_ass->transmit.configured_primary_path_index )
                    {
                        /* Mark the configured primary path Address as DOWN
                         * since we have not received COOKIE-ECHO from Destination
                         * address which has been configured as the Primary Path. */
                        p_ass->transmit.remote[p_ass->transmit.configured_primary_path_index].status
                            = SCTP_COMMUNICATION_LOST;

                        /* Update the Current Primary Path index to the Index of the Confirmed Address */
                        p_ass->transmit.primary_address_index = counter;
                    }

                    break;
                }
            }

            for( counter = 0; counter < p_ass->p_ep->naddr; counter++ )
            {
                if( SCTP_SUCCESS == sctp_cmp_addr( &( p_ass->p_ep->this_address[counter] ), &( p_ass->event.msg_cookie.to_addr ) ) )
                {
                    /* Update the Src addr indx for the Current Primary Path Index. */
                    p_ass->transmit.remote[p_ass->transmit.primary_address_index].src_addr_index = counter;
                    break;
                }
            }

            if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index_new,
                                                                &daddr_index_new, SCTP_NULL ) )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                            ( "sctpfsm_assoc_Gener_MsgCookie: Could not get compatible address "
                              "index for src and dst" ) );

                return;
            }

            for( ; p_buf ; p_buf = ( sctpbuffer_st * )lstNext( ( NODE * ) p_buf ) )
            {
                p_buf->sent = 0;
                p_ass->total_sent = 0;
                p_buf->missed = 0;


                p_buf->dest_addr_index = daddr_index_new;
                p_ass->transmit.remote[daddr_index_new].unsent_packet_count++;
            }

            toaddr.checksum     = p_ass->config.checksum_func;
            toaddr.p_sockaddr   = &( p_ass->transmit.remote[daddr_index_new].that_address );


            sctp_send_chunk_cookieack( p_ass,
                                       &( p_ass->p_ep->this_address[saddr_index_new] ),
                                       p_ass->p_ep->this_port,
                                       p_ass->transmit.that_port,
                                       &toaddr, p_ass->remote_tag );

            for( counter = 0, dest_addr_counter = daddr_index_new; counter < p_ass->transmit.num_remote_address ; counter++ )
            {
                p_ass->transmit.remote[dest_addr_counter].src_addr_index = saddr_index_new;

                saddr_index_new++;

                if( saddr_index_new == p_ass->p_ep->naddr )
                {
                    saddr_index_new = 0;
                }

                dest_addr_counter++;

                if( dest_addr_counter ==  p_ass->transmit.num_remote_address )
                {
                    dest_addr_counter = 0;
                }

            }

            daddr_index = p_ass->transmit.configured_primary_path_index;

            p_ass->transmit.remote[p_ass->transmit.primary_address_index].status
                = SCTP_COMMUNICATION_UP;

            if( ( p_ass->configured_local_primary_address_index !=
                    p_ass->transmit.remote[p_ass->transmit.primary_address_index].src_addr_index ) ||
                    ( p_ass->transmit.configured_primary_path_index !=
                      p_ass->transmit.primary_address_index ) )

            {
                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMER_LOCALHBEAT,
                    SCTP_DEFAULT_HB_INTERVAL,
                    daddr_index,
                    &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT],
                    SCTP_NULL );
            }

            /* send data */
            p_ass->su_event.evt_cmduserdata.addr_index = daddr_index_new;
            /* SPR FIX 20406 stops*/
            sctp_assoc_state_machine( p_ass, SCTPEVENT_CMDUSERDATA );

            /* Send restart notification to SU */
            status_data.endpoint_up.num_dest_addrs =
                p_ass->transmit.num_remote_address;
            status_data.endpoint_up.num_in_streams =
                p_ass->receive.num_in_streams;
            status_data.endpoint_up.num_out_streams =
                p_ass->transmit.num_out_streams;

            addr_list =
                ( sctp_sockaddr_st * ) status_data.endpoint_up.list_of_addrs;

            for( counter = 0; counter < p_ass->transmit.num_remote_address;
                    counter++ )
            {
                /* Reset the address */
                __MEMSET( &addr_list[counter], 0, sizeof( sctp_sockaddr_st ) );
            }

            /* Changes for CSR#1-4248072 */
            p_ass->state  = SCTPSTATE_ESTABLISHED;
            /* Changes for CSR#1-4248072 Ends */

            sctp_ntfy_endpt_status_change( p_ass, SCTP_RESTART, &status_data,
                                           E_NOERROR );

            /* Changes For CSR#1-6124313 */
            /* SPR FIX 20406 starts*/
            for( counter = 0; counter < p_ass->transmit.num_remote_address;
                    counter++ )
            {
                if( p_ass->transmit.remote[counter].status == SCTP_ADDRESS_AVAILABLE )
                {

                    sctp_ntfy_dst_status_change( p_ass,
                                                 &p_ass->transmit.remote[counter].that_address,
                                                 SCTP_ADDRESS_CONFIRMED, E_NOERROR );

                    sctp_ntfy_dst_status_change( p_ass,
                                                 &( p_ass->transmit.remote[counter].that_address ),
                                                 SCTP_ADDRESS_AVAILABLE,
                                                 E_NOERROR );

                }

                else
                {
                    sctp_ntfy_dst_status_change( p_ass,
                                                 &( p_ass->transmit.remote[counter].that_address ),
                                                 SCTP_ADDRESS_UNREACHABLE,
                                                 E_NOERROR );
                }
            }

            /* SPR FIX 20406 stops*/
            /* Changes For CSR#1-6124313 Ends */

            p_ass->config.adaption_ind =
                cookie->remote_init_parameters.adaption_ind;


            if( p_ass->config.adaption_ind != SCTP_NULL_ADAPTION_IND )
            {
                sctp_ntfy_adaption_ind( p_ass, p_ass->config.adaption_ind );
            }
        }

        else  /* retrieval case */
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "Peer Restart Retrival Case, Send Abort" ) );



            /* stop all timers and wait for data to be retrieved */
            for( counter = 0; counter < p_ass->transmit.num_remote_address;
                    counter++ )
            {
                for( counter1 = 0; counter1 < SCTP_DESTTIMER_NUM; counter1++ )
                {
                    sctp_stop_timer(
                        &p_ass->transmit.remote[counter].
                        dest_timer_id[counter1], SCTP_NULL, SCTP_NULL );
                }
            }

            for( counter = 0; counter < SCTP_ASSOCTIMER_NUM; counter++ )
            {
                sctp_stop_timer(
                    &p_ass->assoc_timer_id[counter], SCTP_NULL,
                    SCTP_NULL );
            }

            p_ass->state = SCTPSTATE_CLOSED;

            status_data.endpoint_retrievable.last_sent_tsn
                = p_ass->transmit.tsn - 1;
            status_data.endpoint_retrievable.last_acked_tsn
                = p_ass->receive.cumtsn;

            sctp_ntfy_endpt_status_change( p_ass, SCTP_ENDPOINT_DOWN_RETRIEVABLE,
                                           &status_data, E_NOERROR );

            /*  Also send abort .. */
            sctp_send_chunk_abort( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &( p_ass->transmit.remote[daddr_index].that_address ),
                                   cookie_remote_tag, SCTP_FALSE, 0, 0, NULL, 0,
                                   p_ass->config.checksum_func );

        }

        return;
    }

    if( ( p_ass->local_tag == cookie_local_tag )
            && ( p_ass->remote_tag != cookie_remote_tag ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctpfsm_assoc_Gener_MsgCookie: Handle Cookie, Local Tag match remote"
                      " does not! Association State Changed to ESTABLISHED " ) );

        /* This case is for Init Collision, update TCB with cookie received */
        p_ass->remote_tag =
            cookie->remote_init_parameters.fixed_params.init_tag;
        p_ass->state  = SCTPSTATE_ESTABLISHED;

        p_ass->transmit.rwndthere
            = cookie->remote_init_parameters.fixed_params.a_rwnd;

        p_ass->transmit.num_remote_address
            = cookie->remote_init_parameters.num_addrs;
        p_ass->transmit.primary_address_index = 0;
        p_ass->transmit.configured_primary_path_index = 0;

        p_ass->transmit.lowest_path_mtu = SCTP_MAX_MTUSIZE;

        p_ass->config.adaption_ind =
            cookie->remote_init_parameters.adaption_ind;

        sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr_type );

        for( counter = 0, dest_addr_counter = 0; counter < p_ass->transmit.num_remote_address;
                counter++ )
        {
            /*
             * Ignore the address type that the local endpoint does not support.
             */
            if( ( ( cookie->remote_init_parameters.addr_list[counter].family == SCTP_AF_INET6 ) &&
                    ( supp_addr_type.v6 != SCTP_TRUE ) ) ||
                    ( ( cookie->remote_init_parameters.addr_list[counter].family == SCTP_AF_INET ) &&
                      ( supp_addr_type.v4 != SCTP_TRUE ) ) )
            {
                continue;
            }

            p_ass->transmit.remote[dest_addr_counter].that_address
                = cookie->remote_init_parameters.addr_list[counter];

            p_ass->transmit.remote[dest_addr_counter].rto = p_ass->config.rto_init;
            p_ass->transmit.remote[dest_addr_counter].allow_heartbeat  = 1;
            /* SPR 4151 */
            p_ass->transmit.remote[dest_addr_counter].path_error_threshold
                = p_ass->config.path_max_retrans;

            /* Update last used parameter */
            __GETSYSTIME( &p_ass->transmit.remote[dest_addr_counter].last_used_time );

            ret = sctp_get_pathmtu( p_ass,
                                    &p_ass->transmit.remote[dest_addr_counter].that_address,
                                    &p_ass->transmit.remote[dest_addr_counter].path_mtu );

            if( ret == SCTP_SUCCESS )
            {
                /* This means path mtu retrieved successfully,
                 * start pmtu rediscover timer
                 */

                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERPMTU,
                    SCTP_PMTU_REDISCOVER_TIME,
                    dest_addr_counter,
                    &p_ass->transmit.remote[dest_addr_counter].
                    dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
            }

            else
            {
                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERPMTU,
                    SCTP_PMTU_DISCOVER_FAIL_TIME,
                    dest_addr_counter,
                    &p_ass->transmit.remote[dest_addr_counter].
                    dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
            }

            if( p_ass->transmit.remote[dest_addr_counter].path_mtu <
                    p_ass->transmit.lowest_path_mtu )
            {
                p_ass->transmit.lowest_path_mtu =
                    p_ass->transmit.remote[dest_addr_counter].path_mtu;
            }

            sctp_slowstart_at_setup( p_ass, dest_addr_counter );

            p_ass->transmit.remote[dest_addr_counter].state  = SCTP_TX_STATE_NULL;
            /* SPR 20569 Starts For CSR 1-6443290, CSR 1-7716042, CSR#1-6443823 */
            dest_addr_counter++;
            /* SPR 20569 Ends for CSR 1-6443290,CSR 1-7716042 ,CSR#1-6443823  */
        } /* for .. */

        /* SPR 20569 Starts For CSR 1-6443290, CSR 1-7716042, CSR#1-6443823 */
        p_ass->transmit.num_remote_address = dest_addr_counter;
        /* SPR 20569 Ends for CSR 1-6443290,CSR 1-7716042 ,CSR#1-6443823 */

        p_ass->receive.state = SCTP_RX_STATE_NULL;
        p_ass->receive.cumtsn
            = cookie->remote_init_parameters.fixed_params.init_tsn - 1;
        p_ass->receive.num_in_streams
            = cookie->local_init_parameters.fixed_params.num_in_streams;
        /* SPR 20571 Starts for CSR 1-7241261 */
        p_ass->transmit.num_out_streams
            = cookie->local_init_parameters.fixed_params.num_out_streams;
        /* SPR 20571 Ends for CSR 1-7241261 */

        /* update ecn capable parameter */
        p_ass->ecn.ecn_capable = cookie->ecn_capable;

        for( counter = 0; counter < sctp_db_main.max_streams_per_assoc;
                counter ++ )
        {
            p_ass->transmit.ssn[counter] = 0;
        }

        /*
         * Initialise the sacklist and sackfreelist
         */
        lstInit( &p_ass->receive.sacklist );

        lstInit( &p_ass->receive.sackfreelist );

        for( counter = 0; counter < SCTP_MAX_NUMSACKFRAGS; counter++ )
        {
            lstAdd( &p_ass->receive.sackfreelist,
                    ( NODE * )( &p_ass->receive.sackbuf[counter] ) );
        }

        for( counter = 0; counter < sctp_db_main.max_streams_per_assoc;
                counter++ )
        {
            p_ass->receive.stream[counter].nextssn = 0;
            lstInit( &p_ass->receive.stream[counter].receive_queue );
        }

        /*
         * The primary address index was changed update again.
         * Get the source address index and the destination addres index.
         */
        if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                            &daddr_index, SCTP_NULL ) )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                        ( "sctpfsm_assoc_Gener_MsgCookie: Could not get compatible"
                          " address index for src and dst" ) );

            return;
        }

        /* start inactivity timer */
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERINACTIVE,
            p_ass->config.hb_interval
            + p_ass->transmit.remote[daddr_index].rto,
            0,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
            SCTP_NULL );

        /* Stop any init or cookie timers */
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT],
            SCTP_NULL, SCTP_NULL );

        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
            SCTP_NULL, SCTP_NULL );

        sctp_send_chunk_cookieack( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &toaddr,
                                   p_ass->remote_tag );

        /* Inform SU of State change */
        status_data.endpoint_up.num_dest_addrs
            = p_ass->transmit.num_remote_address;
        addr_list = ( sctp_sockaddr_st * ) status_data.endpoint_up.list_of_addrs;

        for( counter = 0; counter < p_ass->transmit.num_remote_address;
                counter++ )
        {
            addr_list[counter] = p_ass->transmit.remote[counter].that_address;
        }


        status_data.endpoint_up.num_in_streams = p_ass->receive.num_in_streams;
        status_data.endpoint_up.num_out_streams
            = p_ass->transmit.num_out_streams;

        /* Send User notification*/
        sctp_ntfy_endpt_status_change( p_ass, SCTP_COMMUNICATION_UP,
                                       &status_data, E_NOERROR );

        if( p_ass->config.adaption_ind != SCTP_NULL_ADAPTION_IND )
        {
            sctp_ntfy_adaption_ind( p_ass, p_ass->config.adaption_ind );
        }

        return;
    }

    if( ( p_ass->local_tag != cookie_local_tag )
            && ( p_ass->remote_tag == cookie_remote_tag ) )
    {
        /* Leave the timers running */
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctpfsm_assoc_Gener_MsgCookie: Handle Cookie, Remote tag and "
                      "local tag do not match Cookie Chunk dropped !! " ) );

        return; /* Ignore the packet */
    }

    if( ( p_ass->local_tag == cookie_local_tag )
            && ( p_ass->remote_tag == cookie_remote_tag ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctpfsm_assoc_Gener_MsgCookie: Both Local and Remote Tag match, "
                      "Send Cookieack" ) );

        /* duplicate cookie,send cookie ack
         * presently sent on first addr, need to change
         */

        /* Bakeoff change, Treat it as cookie ack if not already
         * in Established state
         */

        if( p_ass->state == SCTPSTATE_COOKIESENT )
        {
            sctpfsm_assoc_CookE_MsgCookieAck( p_ass );
        }

        /* Stop any INIT or COOKIE timers that may have been running. */
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT],
            SCTP_NULL, SCTP_NULL );

        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
            SCTP_NULL, SCTP_NULL );

        sctp_send_chunk_cookieack( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ),
                                   p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &toaddr,
                                   p_ass->remote_tag );

        return;
    }
}


/*
 * FSM Trigger EVT-NEWPMTU.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_EvtNewPmtu
 *
 *    DESCRIPTION:
 *        Ignore this event
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_EvtNewPmtu(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_EvtNewPmtu" ) );
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "Ignore NEWPMTU event" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtNewPmtu
 *
 *    DESCRIPTION:
 *        This function handles new path MTU discovered event for a destination.
 *    Parameter path_mtu maintained per destination is updated with the new
 *    value. Also if this new PMTU is less than the current minimum path MTU
 *    maintained per association (min_path_mtu),update that value.
 *    Stop already running Path MTU timer.
 *    Since PMTU should be rediscovered frequently, start this timer again.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Estbl_EvtNewPmtu(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit addr_index = p_ass->event.evt_pathmtu.addr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_EvtNewPmtu" ) );

    if( SCTP_FALSE == p_ass->config.pmtu_capable )
    {
        return;
    }

    p_ass->transmit.remote[addr_index].path_mtu =
        p_ass->event.evt_pathmtu.path_mtu;

    if( p_ass->transmit.remote[addr_index].path_mtu <
            p_ass->transmit.lowest_path_mtu )
    {
        p_ass->transmit.lowest_path_mtu
            = p_ass->transmit.remote[addr_index].path_mtu;
    }

    /* restart path mtu timer */
    sctp_stop_timer(
        &p_ass->transmit.remote[addr_index].dest_timer_id[SCTP_DESTTIMER_PMTU],
        SCTP_NULL, SCTP_NULL );

    sctp_start_timer(
        p_ass,
        SCTPEVENT_TIMERPMTU,
        SCTP_PMTU_REDISCOVER_TIME,
        addr_index,
        &p_ass->transmit.remote[addr_index].
        dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );

}


/*
 * FSM Trigger EVT-TIMERPMTU.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_EvtTimerPmtu
 *
 *    DESCRIPTION:
 *        Ignore this event
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_EvtTimerPmtu(
    sctp_tcb_st    *p_ass )
{
    if( p_ass )
    {
        /* SPR : 18527 */
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_EvtTimerPmtu" ) );
        /* SPR : 18527 */
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "Ignore Timer PMTU event" ) );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtTimerPmtu
 *
 *    DESCRIPTION:
 *        Handles event of path mtu expiry in established state.
 *    Path mtu is rediscovered in this case.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_EvtTimerPmtu(
    sctp_tcb_st    *p_ass )
{
    sctp_return_t ret;
    sctp_U32bit   addr_index    = p_ass->event.evt_timeout.addr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_EvtTimerPmtu" ) );

    /* update expired timer id in timer structure, destination timer*/
    p_ass->transmit.remote[addr_index].dest_timer_id[SCTP_DESTTIMER_PMTU]
        = SCTP_NULL_TIMER_ID;

    if( SCTP_FALSE == p_ass->config.pmtu_capable )
    {
        return;
    }

    ret = sctp_get_pathmtu( p_ass,
                            &p_ass->transmit.remote[addr_index].that_address,
                            &p_ass->transmit.remote[addr_index].path_mtu );

    if( ret == SCTP_SUCCESS )
    {
        /*
         * This means path mtu retrieved successfully,
         * update lowest path mtu and start pmtu rediscover timer
         */
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERPMTU,
            SCTP_PMTU_REDISCOVER_TIME,
            addr_index,
            &p_ass->transmit.remote[addr_index].
            dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
    }

    else
    {
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERPMTU,
            SCTP_PMTU_DISCOVER_FAIL_TIME,
            addr_index,
            &p_ass->transmit.remote[addr_index].
            dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
    }

    if( p_ass->transmit.remote[addr_index].path_mtu <
            p_ass->transmit.lowest_path_mtu )
    {
        p_ass->transmit.lowest_path_mtu
            = p_ass->transmit.remote[addr_index].path_mtu;
    }

}


/*
 * FSM Trigger EVT-TIMERDEALYACK.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_TimerDeAck
 *
 *    DESCRIPTION:
 *        Delayed ack timer expired, Let receive state machine function handle
 *    it.
 *
 *    RETURN VALUE:
 *       None
 *************************************************************************/
void
sctpfsm_assoc_Gener_TimerDeAck(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_TimerDeAck" ) );

    /* update expired timer id in timer structure */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_DELAYACK] = SCTP_NULL_TIMER_ID;

    sctp_rx_state_machine( p_ass, SCTP_RX_EVENT_DELAYTIMEOUT,
                           p_ass->event.evt_timeout.addr_index );

}


/*
 * FSM Trigger EVT-TIMERLIFETIME.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_TimerLtime
 *
 *    DESCRIPTION:
 *        Scan through the transmit queue and decrement parameter
 *    lifetime for all queued but unsent messages (sent == 0).
 *    If after decrementing lifetime parameter becomes '0'
 *    (lifetime expired). Send notification sctp_api_send_failure
 *    to ULP.Restart this timer if transmit queue is not empty.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_TimerLtime(
    sctp_tcb_st    *p_ass )
{
    sctpbuffer_st *p_buf, *save;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_TimerLtime" ) );

    /* update expired timer id in timer structure */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LIFETIME] = SCTP_NULL_TIMER_ID;

    p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

    for( ; ; )
    {
        if( p_buf == NULL )
        {
            break;
        }

        save = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );

        if( ( p_buf->sent == 1 ) || ( p_buf->lifetime == SCTP_MAX_LIFE_TIME )
                || ( p_buf->begin == 0 ) )
        {
            p_buf = save;
            continue;
        }

        p_buf->lifetime--;

        if( p_buf->lifetime == 0 )
        {
            save = ( sctpbuffer_st * )lstNext( ( NODE * ) p_buf );
            lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )p_buf );
            sctp_free_transmit_buffer( p_ass, p_buf );

            /* also delete , rest of the fragments */
            if( p_buf->end != 1 )
            {
                for( ; ; )
                {
                    p_buf = save;

                    if( ( p_buf == NULL ) || ( p_buf->end == 1 ) )
                    {
                        break;
                    }

                    /* SPR : 18527 */
                    else
                    {
                        save = ( sctpbuffer_st * )lstNext( ( NODE * ) p_buf );
                    }

                    /* SPR : 18527 */
                }
            }

            /* SPR_20411_FIX_START -- */

            if( p_buf == NULL )
            {
                break;
            }

            /* SPR_20411_FIX_END -- */

            /* send notification to SU */
            sctp_ntfy_send_failure( p_ass, p_buf->context, SCTP_DATA_UNSENT,
                                    E_NOERROR, sizeof( sctpbuffer_st ), ( sctp_U8bit * )p_buf );

            SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream, SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );
        }

        p_buf = save;
    }

    /* reatart lifetime timer if atleast one data is there */
    p_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    if( p_buf != NULL )
    {
        sctp_start_timer(
            p_ass, SCTPEVENT_TIMERLIFETIME,
            SCTP_ONE_SECOND_TIME, 0,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LIFETIME],
            SCTP_NULL );
    }

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_TimerLtime:"
                                             "end" ) );
}


/*
 * FSM Trigger CMD-CONNECT.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Close_CmdConnect
 *
 *    DESCRIPTION:
 *        Handles user command connect in closed state
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Close_CmdConnect(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit            saddr_index = 0;
    sctp_U32bit            daddr_index = 0;
    sctp_suppaddr_st       supp_addr;
    sctp_addrinfo_st       addr_info;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Close_CmdConnect" ) );

    p_ass->transmit.primary_address_index = 0 ;

    /*
     * Get the source address index and the destination addres index.
     */

    if( SCTP_FAILURE == sctp_db_get_saddr_index_with_af( p_ass, &saddr_index,
                                                         p_ass->transmit.remote[daddr_index].that_address.family, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Close_CmdConnect: Could not get compatible address "
                      "index for src and dst" ) );

        sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );

        return;
    }

    do
    {
        p_ass->local_tag    = __RANDOM();
    }
    while( 0 == p_ass->local_tag );

    p_ass->transmit.tsn = __RANDOM();
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    p_ass->transmit.serial_no = p_ass->transmit.tsn;
#endif

    p_ass->transmit.num_out_streams = p_ass->config.num_out_streams;
    p_ass->receive.num_in_streams   = p_ass->config.num_in_streams;

    /*v64*/
    sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr );
    p_ass->transmit.remote[daddr_index].src_addr_index = saddr_index;

    addr_info.checksum      = p_ass->config.checksum_func;
    addr_info.p_sockaddr    =
        &( p_ass->transmit.remote[daddr_index].that_address );
    addr_info.p_info        = &( p_ass->config.nat_capable );

    /**** add in assoc hash table **/
    sctp_db_add_assoc_verf( p_ass );

    sctp_send_chunk_init(
        p_ass,
        &( p_ass->p_ep->this_address[saddr_index] ),
        p_ass->p_ep->this_port,
        p_ass->transmit.that_port,
        &addr_info,
        p_ass->local_tag,
        p_ass->transmit.tsn,
        p_ass->config.num_out_streams,
        p_ass->config.num_in_streams,
        sctp_db_assoc_rx_window( p_ass ),
        p_ass->p_ep->naddr,
        &( p_ass->p_ep->this_address[0] ),
        SCTP_FALSE, 0UL,
        &supp_addr,
        p_ass->p_ep->hostname,
        p_ass->config.adaption_ind );
    /*v64*/

    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMER,
                      p_ass->transmit.remote[daddr_index].rto,
                      0, &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT],
                      SCTP_NULL );

    if( 0 != p_ass->config.init_timeo )
    {
        /*
         * Start the timer only if it is non-null.
         */
        sctp_start_timer( p_ass,
                          SCTPEVENT_TIMERINIT,
                          p_ass->config.init_timeo,
                          0, &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_SHUT],
                          SCTP_NULL );
    }

    p_ass->init_retry = 0;
    p_ass->state = SCTPSTATE_COOKIEWAIT;
}


/*
 * FSM Trigger MSG-STALECOOKIE.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_MsgStaleErr
 *
 *    DESCRIPTION: Ignore unexpected stale cookie
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Gener_MsgStaleErr(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_MsgStaleErr" ) );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "Stale Cookie unexpected ignored." ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_CookE_MsgStaleErr
 *
 *    DESCRIPTION:
 *        Handles stale cookie error in Cookie Echoed state
 *
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_CookE_MsgStaleErr(
    sctp_tcb_st     *p_ass )
{
    /* SPR_20411_FIX_START -- */

    sctp_U32bit   rtt = 0;
    /* SPR_20411_FIX_END -- */

    sctp_U32bit   cookie_preservative_time;
    sctp_U32bit   saddr_index = 0;
    sctp_U32bit   daddr_index = 0;
    sctpbuffer_st *p_buf = SCTP_NULL;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_CookE_MsgStaleErr" ) );

    /*
     * Need to send Abort to the destination.
     * So get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_CookE_MsgStaleErr: Could not get compatible address"
                      " index for src and dst" ) );

        return;
    }


    /* Free the Cookie List */
    p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );

    while( p_buf )
    {
        sctp_free_transmit_buffer( p_ass, p_buf );
        p_buf = ( sctpbuffer_st * )lstGet( &p_ass->cookie_list );
    }



    /* Switch on the SCTP stale cookie policy */
    switch( SCTP_SCOOKIE_POLICY )
    {
        case 0: /* re attempt call setup */
            /* stop cookie timer */
            sctp_stop_timer(
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
                SCTP_NULL, SCTP_NULL );

            p_ass->init_retry++;

            /* If max retries not exceeded, send INIT again, start timer
             * Else delete association
             */
            /* SPR 4033 */
            if( p_ass->init_retry <= p_ass->config.max_init_retrans )
            {
                sctp_suppaddr_st     supp_addr;
                sctp_addrinfo_st     addr_info;

                addr_info.checksum      = p_ass->config.checksum_func;
                addr_info.p_sockaddr    =
                    &( p_ass->transmit.remote[daddr_index].that_address );
                addr_info.p_info        = &( p_ass->config.nat_capable );
                sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr );

                sctp_send_chunk_init(
                    p_ass,
                    &( p_ass->p_ep->this_address[saddr_index] ),
                    p_ass->p_ep->this_port,
                    p_ass->transmit.that_port,
                    &addr_info,
                    p_ass->local_tag,
                    p_ass->transmit.tsn,
                    p_ass->config.num_out_streams,
                    p_ass->config.num_in_streams,
                    sctp_db_assoc_rx_window( p_ass ),
                    /*v64*/
                    p_ass->p_ep->naddr,
                    &( p_ass->p_ep->this_address[0] ),
                    /*v64*/
                    SCTP_FALSE, 0,
                    &supp_addr,
                    p_ass->p_ep->hostname,
                    p_ass->config.adaption_ind );

                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMER,
                    p_ass->transmit.remote[daddr_index].rto,
                    0,
                    &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT],
                    SCTP_NULL );

                p_ass->state  = SCTPSTATE_COOKIEWAIT;
            }

            else
            {
                sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
            }

            break;

        case 1: /* discard TCB */
            sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
            break;

        case 2: /* send init with cookie preservative */
            /* stop cookie timer */
            sctp_stop_timer(
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_COOKIE],
                ( sctp_U32bit * )&rtt, SCTP_NULL );

            if( p_ass->event.msg_err_stale_cookie.stale_cookie_time / 1000 <
                    ( rtt + 1000 ) )
            {
                cookie_preservative_time
                    = p_ass->event.msg_err_stale_cookie.stale_cookie_time / 1000;
            }

            else
            {
                cookie_preservative_time = rtt + 1000;
            }

            p_ass->init_retry++;

            /* SPR 4033 */
            if( p_ass->init_retry <= p_ass->config.max_init_retrans )
            {
                sctp_suppaddr_st     supp_addr;
                sctp_addrinfo_st     addr_info;

                addr_info.checksum      = p_ass->config.checksum_func;
                addr_info.p_sockaddr    =
                    &( p_ass->transmit.remote[daddr_index].that_address );
                addr_info.p_info        = &( p_ass->config.nat_capable );
                sctp_db_get_supported_address_types( p_ass->p_ep, &supp_addr );

                sctp_send_chunk_init(
                    p_ass,
                    &( p_ass->p_ep->this_address[saddr_index] ),
                    p_ass->p_ep->this_port,
                    p_ass->transmit.that_port,
                    &addr_info,
                    p_ass->local_tag,
                    p_ass->transmit.tsn,
                    p_ass->config.num_out_streams,
                    p_ass->config.num_in_streams,
                    sctp_db_assoc_rx_window( p_ass ),
                    /*v64*/
                    p_ass->p_ep->naddr,
                    &p_ass->p_ep->this_address[0],
                    /*v64*/
                    SCTP_TRUE,
                    cookie_preservative_time, /* in ms */
                    &supp_addr,
                    p_ass->p_ep->hostname,
                    p_ass->config.adaption_ind );

                sctp_start_timer(
                    p_ass, SCTPEVENT_TIMER,
                    p_ass->transmit.remote[daddr_index].rto,
                    0,
                    &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INIT], SCTP_NULL );

                p_ass->state = SCTPSTATE_COOKIEWAIT;
            }

            else
            {
                sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
            }

            break;

        default:
            break;
    }



}


/*
 * FSM Trigger MSG-ECNE.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgEcne
 *
 *    DESCRIPTION:
 *        Reduce condestion window and send cwr message
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctpfsm_assoc_Estbl_MsgEcne(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit  addr_index  = 0;
    sctp_U32bit  saddr_index = 0;
    sctp_addrinfo_st    to;
    sctp_return_t  ret = SCTP_FAILURE;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_MsgEcne" ) );

    if( p_ass->ecn.ecn_capable != 1 )
    {
        return;    /* association not ecn capable ignore message */
    }

    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_ecne.from_addr,
                                        &addr_index );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Estbl_MsgEcne: -> Invalid remote address: " ) );
        SCTP_TRACE_ADDR( SCTP_NULL, SCTP_ERR_TRC, &( p_ass->event.msg_ecne.from_addr ) );
        return;
    }

    if( p_ass->event.msg_ecne.lowest_tsn != p_ass->ecn.last_recv_lowest_tsn )
    {
        /* got ecne message , decay congestion window */
        p_ass->transmit.remote[addr_index].ssthresh =
            ( p_ass->transmit.remote[addr_index].cwnd / 2
              > ( sctp_U32bit ) 2 * p_ass->transmit.remote[addr_index].path_mtu )
            ? p_ass->transmit.remote[addr_index].cwnd / 2
            : ( sctp_U32bit )( 2 * p_ass->transmit.remote[addr_index].path_mtu );

        p_ass->transmit.remote[addr_index].cwnd
            = p_ass->transmit.remote[addr_index].path_mtu;

        p_ass->ecn.last_recv_lowest_tsn = p_ass->event.msg_ecne.lowest_tsn;
    }

    /* Get a source address for peer address */
    saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->event.msg_ecne.from_addr );
    /* send chunk cwr */
    sctp_send_chunk_cwr( p_ass,
                         &( p_ass->p_ep->this_address[saddr_index] ),
                         p_ass->p_ep->this_port,
                         p_ass->transmit.that_port,
                         &to,
                         p_ass->remote_tag,
                         p_ass->event.msg_ecne.lowest_tsn );

}


/*
 * FSM Trigger MSG-CWR.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgCwr
 *
 *    DESCRIPTION: Handles CWR meaage in ESTABLISED state
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_MsgCwr(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Estbl_MsgCwr" ) );

    if( p_ass->ecn.ecn_capable != 1 )
    {
        return; /* association not ecn capable ignore message */
    }

    if( ( p_ass->ecn.ecn_sent == 1 )
            && ( p_ass->ecn.last_sent_lowest_tsn == p_ass->event.msg_cwr.lowest_tsn ) )
    {
        /* stop timer and update ecn parameters */
        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ECN],
            SCTP_NULL, SCTP_NULL );

        p_ass->ecn.ecn_sent = 0;
    }
}


/*
 * FSM Trigger TIMER-ECN.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtTimerEcn
 *
 *    DESCRIPTION:
 *        Send Ecn again and start timer
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_EvtTimerEcn(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit daddr_index = p_ass->event.evt_timeout.addr_index;
    sctp_U32bit saddr_index = 0;
    sctp_addrinfo_st    to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Close_EvtTimerEcn" ) );

    /* update expired timer id in timer structure */

    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ECN]
        = SCTP_NULL_TIMER_ID;

    /*
     * ECN is an association timer , but we need address info also
     * So call destination timer api
     */

    sctp_start_timer(
        p_ass,
        SCTPEVENT_TIMERECN,
        p_ass->transmit.remote[daddr_index].rto,
        daddr_index,
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ECN], SCTP_NULL );

    /*
     * Get a source address with the same family as the destination
     * address.
     */

    saddr_index = p_ass->transmit.remote[daddr_index].src_addr_index;

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[daddr_index].that_address );

    sctp_send_chunk_ecne( p_ass,
                          &( p_ass->p_ep->this_address[saddr_index] ),
                          p_ass->p_ep->this_port,
                          p_ass->transmit.that_port,
                          &to,
                          p_ass->remote_tag,
                          p_ass->ecn.last_sent_lowest_tsn );
}


/*
 * FSM Trigger MSG-SHUT-COMPLETE.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_ShutA_MsgShutCom
 *
 *    DESCRIPTION:
 *        This function will handle shutdown complete message from peer for
 *    association in shutdown-ack-sent state. Delete  association.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_ShutA_MsgShutCom(
    sctp_tcb_st    *p_ass )
{
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_ShutA_MsgShutCom" ) );

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_GRACEFUL_SHUTDOWN, 1 );

    sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
}


/*
 * FSM Trigger EVENT_TIMER-BUNDLE.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_TimerBundle
 *
 *    DESCRIPTION:
 *        Handle bundling timeout event
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_TimerBundle(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit addr_index = p_ass->event.evt_timeout.addr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_TimerBundle" ) );

    /* update expired timer id in timer structure */
    p_ass->transmit.remote[addr_index].dest_timer_id[SCTP_DESTTIMER_BUNDLING]
        = SCTP_NULL_TIMER_ID;

    sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_USERDATA, addr_index );

}


/*
 * FSM Trigger EVENT_TIMER-LINGER.
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_TimerLinger
 *
 *    DESCRIPTION:
 *        Handle Linger timeout event. Send an abort on the association and
 *    Delete the TCB.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_TimerLinger(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctpfsm_assoc_Gener_TimerLinger" ) );

    /*
     * Get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, SCTP_NULL ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "sctpfsm_assoc_Gener_TimerLinger: Could not get compatible address "
                      "index for src and dst" ) );

        return;
    }

    /*
     * Send ABORT to the destination.
     */
    sctp_send_chunk_abort( p_ass,
                           &( p_ass->p_ep->this_address[saddr_index] ),
                           p_ass->p_ep->this_port,
                           p_ass->transmit.that_port,
                           &( p_ass->transmit.remote[daddr_index].that_address ),
                           p_ass->remote_tag,
                           SCTP_FALSE, 0, 0, NULL, 0, p_ass->config.checksum_func );

    /*
     * Delete the association and Endpoint.
     */
    sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_TRUE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Gener_TimerAutoclose
 *
 *    DESCRIPTION:
 *        Handle Autoclose timer timeout for UDP style sockets. Send a shutdown
 *    on the association. Initiate suhotdown processing and stop the autoclose
 *    timer.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Gener_TimerAutoclose(
    sctp_tcb_st    *p_ass )
{
#ifdef SCTP_UI_SOCK_API
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Gener_TimerAutoclose" ) );
    /* Invoke the shutdown event of the FSM */
    sctp_assoc_state_machine( p_ass, SCTPEVENT_CMDTERM );
#else
    /*
     *in traditional interface this timer is used to handle the data pending
     *notifications on a server
     */
    sctp_U32bit  counter = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "timer_expiry:data_pending on association to be accepted" ) );

    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE] = SCTP_NULL_TIMER_ID;

    /* The Traditional socket interface is enabled */
    if( SCTP_SOCK_IS_LISTENER( p_ass->p_ep->flags ) )
    {
        sctp_start_timer(
            p_ass,
            SCTPEVENT_TIMERAUTOCLOSE,
            100, /* 100 MS only just add in the list at front */
            0,
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
            SCTP_NULL );
        return;
    }

    for( counter = 0; counter < p_ass->receive.num_in_streams; counter++ )
    {
        sctp_U16bit nextssn = p_ass->receive.stream[counter].nextssn_notify;

        while( nextssn != 0 )
        {
            p_ass->notify.datapending.assoc_id = p_ass->p_ep->ep_id;
            p_ass->notify.datapending.stream = counter;

            /* Invoke the generic Notification function. */
            sctp_generic_su_notification( p_ass->p_ep, SCTPNOTIFY_DATAPENDING );
            nextssn--;
        }
    }

#endif
}


/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_handle_unreco_chunk_err
 *
 *    DESCRIPTION:
 *        Handle Unrecognised chunk error sent by peer
 *    timer.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_handle_unreco_chunk_err(
    sctp_tcb_st  *p_ass,
    sctp_U8bit   chunk_type )
{
    switch( chunk_type )
    {
#ifdef SCTP_DYNAMIC_IP_SUPPORT

        case SCTPCHUNK_TYPE_ASCONF:
            if( p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] != SCTP_NULL_TIMER_ID )
            {
                sctp_U32bit count;
                /*stop the timer*/
                sctp_stop_timer( &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF],
                                 SCTP_NULL, SCTP_NULL );

                /*indicate failure*/
                for( count = 0; count < p_ass->cmd_asconf.num_params; count++ )
                {
                    sctp_proc_asconf_failure( p_ass,
                                              &p_ass->cmd_asconf.asconf_params[count], 0 );
                }

                SCTP_WAKEUP( p_ass->p_ep->asconf_cond );
            }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            p_ass->restrict.asconf = SCTP_TRUE;
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            p_ass->sctp_restrict.asconf = SCTP_TRUE;
#endif
            break;
#endif
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    /*to avoid the warning*/

}



#ifdef SCTP_DYNAMIC_IP_SUPPORT
/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_CmdAsconf
 *
 *    DESCRIPTION:
 *        Handle ASCONF command from user.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_CmdAsconf(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit  saddr_index = 0;
    sctp_U32bit  daddr_index = 0;
    sctp_error_t s_err;
    sctp_U32bit  pcount = 0, a_index;
    sctp_addrinfo_st  to;
#ifdef SCTP_STATS_ENABLED
    sctp_time_t t_time_stamp;
#endif /* SCTP_STATS_ENABLED */

    /*SPR 5221*/
    sctp_U32bit  naddr_removed = 0;
    /*SPR 5221*/
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Estbl_CmdAsconf" ) );

    /*if it is a request to delete IP, delete it right away*/
    for( pcount = 0; pcount < p_ass->cmd_asconf.num_params; pcount++ )
    {
        if( SCTP_ASCONF_PARAM_DEL_IP ==
                p_ass->cmd_asconf.asconf_params[pcount].param_type )
        {
            sctp_asconf_delip_param_st  *p_param =
                &( p_ass->cmd_asconf.asconf_params[pcount].del_ip );
            sctp_U32bit curr_index = 0;

            sctp_db_validate_local_addr( p_ass->p_ep, &p_param->ipaddr,
                                         &curr_index );

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "local address index %d to be deleted", curr_index ) );

            /* now the last address is swaped with to-be-deleted address
             * check if its already last
             */
            if( curr_index < ( sctp_U32bit )( p_ass->p_ep->naddr - 1 ) )
            {
                p_ass->p_ep->this_address[curr_index] =
                    p_ass->p_ep->this_address[p_ass->p_ep->naddr - 1];
                p_ass->p_ep->this_address[p_ass->p_ep->naddr - 1]
                    = p_param->ipaddr;
                /* Start CSR 1-3307301 */
#ifdef SCTP_STATS_ENABLED
                t_time_stamp = p_ass->stats.local_addr_stats[curr_index].local_addr_start_time;

                p_ass->stats.local_addr_stats[curr_index].local_address =
                    p_ass->stats.local_addr_stats[p_ass->p_ep->naddr - 1].local_address;

                p_ass->stats.local_addr_stats[curr_index].local_addr_start_time =
                    p_ass->stats.local_addr_stats[p_ass->p_ep->naddr - 1].local_addr_start_time;

                p_ass->stats.local_addr_stats[p_ass->p_ep->naddr - 1].local_address =
                    p_param->ipaddr;

                p_ass->stats.local_addr_stats[p_ass->p_ep->naddr - 1].local_addr_start_time =
                    t_time_stamp;
#endif /* SCTP_STATS_ENABLED */
                /* End CSR 1-3307301 */
            }

            /*find an ip address with same family as to-be-deleted,*/
            for( saddr_index = 0;
                    saddr_index < ( sctp_U32bit )( p_ass->p_ep->naddr - 1 ); /*exclude the last address*/
                    saddr_index++ )
            {
                if( p_ass->p_ep->this_address[saddr_index].family ==
                        p_param->ipaddr.family )
                {
                    break;
                }
            }

            /* use the saddr_index in place of curr_index*/
            for( a_index = 0;
                    a_index < p_ass->transmit.num_remote_address;
                    a_index++ )
            {
                if( curr_index == p_ass->transmit.remote[a_index].src_addr_index )
                {
                    p_ass->transmit.remote[a_index].src_addr_index = saddr_index;
                }
            }

            /* the address to be deleted should not be used as source address
             * its already removed if it was the last
             */
            if( curr_index < ( sctp_U32bit )( p_ass->p_ep->naddr - 1 ) )
            {
                for( a_index = 0; a_index < p_ass->transmit.num_remote_address; a_index++ )
                {
                    if( ( sctp_U32bit )( p_ass->p_ep->naddr - 1 ) == p_ass->transmit.remote[a_index].
                            src_addr_index )
                    {
                        p_ass->transmit.remote[a_index].src_addr_index = curr_index;
                    }
                }
            }

            /*SPR 5221*/
            /* we decrement the address count for time being, its restored later
             * in this function
             */
            p_ass->p_ep->naddr--;
            naddr_removed++;
            /*SPR 5221*/
        }/*if del_ip*/

        else if( SCTP_ASCONF_PARAM_ADD_IP ==
                 p_ass->cmd_asconf.asconf_params[pcount].param_type )
        {
            sctp_asconf_addip_param_st  *p_param =
                &( p_ass->cmd_asconf.asconf_params[pcount].add_ip );
            /*again add the address to assoc, but don't source any packets*/
            sctp_db_bind_ep( p_ass->p_ep->ep_id, p_ass->p_ep->this_port, 1,
                             &( p_param->ipaddr ),
                             &s_err );
        }
    } /*for loop*/

    /*
     * Need to send ASCONF to the destination.
     * So get the source address index and the destination addres index.
     */
    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &daddr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Could not get compatible address index for src and dst" ) );
        /*SPR 5221*/
        p_ass->p_ep->naddr += naddr_removed;
        /*SPR 5221*/
        return;
    }

    /* Send ASCONF chunk to peer */
    p_ass->cmd_asconf.serial_no = p_ass->transmit.serial_no++;
    p_ass->cmd_asconf.src_addr  = p_ass->p_ep->this_address[saddr_index];

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[daddr_index].that_address );
    sctp_send_chunk_asconf( p_ass,
                            &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                            p_ass->transmit.that_port,
                            &to, p_ass->remote_tag,
                            &( p_ass->cmd_asconf ) );

    /*start timer T-4*/
    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMERASCONF,
                      p_ass->transmit.remote[daddr_index].rto,
                      daddr_index,
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF],
                      SCTP_NULL );
    /*SPR 5221*/
    p_ass->p_ep->naddr += naddr_removed;
    /*SPR 5221*/
}







/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgAsconf
 *
 *    DESCRIPTION:
 *        Handle Autoclose timer timeout for UDP style sockets. Send a shutdown
 *    on the association. Initiate suhotdown processing and stop the autoclose
 *    timer.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_MsgAsconf(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit     resp_ctr = 0, sctp_index = 0;
    sctp_addr_conf_st  *p_msg = &p_ass->event.msg_addrconf;
    sctp_addr_conf_ack_st  ack_msg;
    sctp_Boolean_t  fail = SCTP_FALSE;
    sctp_error_t    s_err;
    sctp_U32bit saddr_index, daddr_index;
    sctp_addrinfo_st to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Estbl_MsgAsconf" ) );

    if( p_msg->serial_no == p_ass->receive.serial_no )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Duplicate ASCONF received" ) );

        if( p_ass->buf_len != 0 )
        {
            sctp_sockaddr_st *p_to, *p_from;

            /*
             * Need to send ASCONF to the destination.
             * So get the source address index and the destination addres index.
             */
            if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                                &daddr_index, &s_err ) )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                            ( "Could not get compatible address index for src and dst" ) );

                return;
            }

            /*get an address with same family*/
            for( saddr_index = 0; saddr_index < p_ass->p_ep->naddr; saddr_index++ )
            {
                if( p_ass->p_ep->this_address[saddr_index].family ==
                        p_msg->src_addr.family )
                {
                    break;
                }
            }

            p_to   = &p_msg->src_addr;
            p_from = &p_ass->p_ep->this_address[saddr_index];

            /* Callback for Sending Func SPR 20859 Starts */
            if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
            {
                p_ass->p_ep->send_func( p_ass->asconf_buffer, p_ass->buf_len, SCTP_FALSE, p_to, p_from, SCTP_FALSE );
            }

            else
            {
                /* Sends the message to the appropriate node */
                sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
                    p_ass->asconf_buffer, p_ass->buf_len, SCTP_FALSE, p_to, p_from );
            }

            /* Callback for Sending Func SPR 20859 Stops */
            return;
        }
    }

    if( p_msg->serial_no != ( p_ass->receive.serial_no + 1 ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Serial Number don;t match, ignore" ) );

        return;
    }

    ack_msg.serial_no = p_msg->serial_no;

    for( sctp_index = 0; sctp_index < p_msg->num_params; sctp_index++ )
    {
        if( SCTP_FALSE == p_ass->config.allow_asconf )
        {
            /*could not find a source address of same family*/
            ack_msg.asconf_params[resp_ctr].corr_id =
                p_msg->asconf_params[sctp_index].add_ip.corr_id;

            ack_msg.asconf_params[resp_ctr].param_type =
                SCTP_ASCONF_PARAM_ERR_CAUSE;
            ack_msg.asconf_params[resp_ctr].err_cause.cause_code =
                SCTPERR_REFUSEDNOAUTH;
            ack_msg.asconf_params[resp_ctr].err_cause.invalid_param =
                p_msg->asconf_params[sctp_index];
            resp_ctr++;
            fail = SCTP_TRUE;
            break;
        }

        switch( p_msg->asconf_params[sctp_index].param_type )
        {
            case SCTP_ASCONF_PARAM_ADD_IP:
                {
                    if( fail )
                    {
                        break;
                    }

                    /*check if address is already added*/
                    if( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_ass,
                                                                      &( p_msg->asconf_params[sctp_index].add_ip.ipaddr ), &daddr_index ) )
                    {
                        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                                    ( "Address already exists, ignore request" ) );
                        break;
                    }

                    /*check if address can be added to the remote addr list*/
                    if( p_ass->transmit.num_remote_address == SCTP_MAX_TRANSPORT_ADDR )
                    {
                        /*out ouf resources */
                        ack_msg.asconf_params[resp_ctr].corr_id =
                            p_msg->asconf_params[sctp_index].add_ip.corr_id;

                        ack_msg.asconf_params[resp_ctr].param_type =
                            SCTP_ASCONF_PARAM_ERR_CAUSE;
                        ack_msg.asconf_params[resp_ctr].err_cause.cause_code =
                            SCTPERR_OPREFRESOURCESHORT;
                        ack_msg.asconf_params[resp_ctr].err_cause.invalid_param =
                            p_msg->asconf_params[sctp_index];
                        resp_ctr++;
                        fail = SCTP_TRUE;
                        /*must fail subsequent request*/
                    }

                    else
                    {
                        sctp_U32bit addr_index = 0;
                        sctp_asconf_addip_param_st  *p_addip =
                            &( p_msg->asconf_params[sctp_index].add_ip );
                        sctp_U32bit counter = 0;
                        /*find a compatible source address index for new address */
                        addr_index = p_ass->transmit.num_remote_address;
                        saddr_index =
                            p_ass->transmit.remote[addr_index - 1].src_addr_index;

                        for( counter = 0; counter < p_ass->p_ep->naddr; counter++ )
                        {
                            saddr_index = ( saddr_index + 1 ) % p_ass->p_ep->naddr;

                            if( p_addip->ipaddr.family ==
                                    p_ass->p_ep->this_address[saddr_index].family )
                            {
                                break;
                            }
                        }

                        if( counter == p_ass->p_ep->naddr )
                        {
                            /*could not find a source address of same family*/
                            ack_msg.asconf_params[resp_ctr].corr_id =
                                p_msg->asconf_params[sctp_index].add_ip.corr_id;

                            ack_msg.asconf_params[resp_ctr].param_type =
                                SCTP_ASCONF_PARAM_ERR_CAUSE;
                            ack_msg.asconf_params[resp_ctr].err_cause.cause_code =
                                SCTPERR_UNRESOLVEADDR;
                            ack_msg.asconf_params[resp_ctr].err_cause.invalid_param =
                                p_msg->asconf_params[sctp_index];
                            resp_ctr++;
                            fail = SCTP_TRUE;
                            break;
                        }


                        p_ass->transmit.num_remote_address++;

                        p_ass->transmit.remote[addr_index].that_address
                            = p_addip->ipaddr;

                        p_ass->transmit.remote[addr_index].src_addr_index = saddr_index;
                        /**********************************/
                        /* Update last used parameter */
                        __GETSYSTIME( &p_ass->transmit.remote[addr_index].last_used_time );

                        if( SCTP_SUCCESS == sctp_get_pathmtu( p_ass,
                                                              &p_ass->transmit.remote[addr_index].that_address,
                                                              &p_ass->transmit.remote[addr_index].path_mtu ) )
                        {
                            /* This means path mtu retrieved successfully,
                             * start pmtu rediscover timer
                             */

                            sctp_start_timer( p_ass,
                                              SCTPEVENT_TIMERPMTU,
                                              SCTP_PMTU_REDISCOVER_TIME,
                                              addr_index,
                                              &p_ass->transmit.remote[addr_index].
                                              dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
                        }

                        else
                        {
                            sctp_start_timer( p_ass,
                                              SCTPEVENT_TIMERPMTU,
                                              SCTP_PMTU_DISCOVER_FAIL_TIME,
                                              addr_index,
                                              &p_ass->transmit.remote[addr_index].
                                              dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
                        }


                        if( p_ass->transmit.remote[addr_index].path_mtu
                                < p_ass->transmit.lowest_path_mtu )
                        {
                            p_ass->transmit.lowest_path_mtu
                                = p_ass->transmit.remote[addr_index].path_mtu;
                        }

                        sctp_slowstart_at_setup( p_ass, addr_index );

                        p_ass->transmit.remote[addr_index].status = SCTP_COMMUNICATION_UP;
                        p_ass->transmit.remote[addr_index].path_error_threshold
                            = p_ass->config.path_max_retrans;
                        p_ass->transmit.remote[addr_index].allow_heartbeat = SCTP_TRUE;

                        /**********************************/

                        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_init;
                        /*see if success need to be sent explicitly*/

                        /* Start CSR 1-3307301 */
#ifdef SCTP_STATS_ENABLED
                        p_ass->stats.dest_stats[addr_index].dest_address = p_addip->ipaddr;
                        p_ass->stats.dest_stats[addr_index].no_datagrams_sent = 0;
                        p_ass->stats.dest_stats[addr_index].no_datagrams_unacked = 0;
                        p_ass->stats.dest_stats[addr_index].no_datagrams_recd = 0;
                        p_ass->stats.dest_stats[addr_index].no_bytes_sent = 0;
                        p_ass->stats.dest_stats[addr_index].no_bytes_recvd = 0;
                        __GETSYSTIME( &( p_ass->stats.dest_stats[addr_index].dest_addr_start_time ) );
#ifdef TORNADO_VXWORKS
                        p_ass->stats.dest_stats[addr_index].pad[0] = 0;
                        p_ass->stats.dest_stats[addr_index].pad[1] = 0;
#endif
#endif /* SCTP_STATS_ENABLED */
                        /* End CSR 1-3307301 */

                        sctp_ntfy_dst_status_change( p_ass, &p_addip->ipaddr,
                                                     SCTP_ADDRESS_ADDED, E_NOERROR );
                    }
                }
                break;

            case SCTP_ASCONF_PARAM_DEL_IP:
                {
                    sctp_asconf_addip_param_st  *p_delip =
                        &( p_msg->asconf_params[sctp_index].del_ip );

                    sctp_U32bit count = 0, rem_time = 0;
                    sctp_U32bit addr_index = 0, event, timer_addr = SCTP_INVALID_U32;
                    sctp_U32bit num_family_addrs = 0;

                    if( fail )
                    {
                        break;
                    }

                    /*check that this is not the only IP address*/

                    if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_ass,
                                                                      &p_delip->ipaddr, &addr_index ) )
                    {
                        /*address not found, probably already deleted, a confused peer*/
                        break;
                    }

                    for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
                    {
                        if( p_delip->ipaddr.family ==
                                p_ass->transmit.remote[count].that_address.family )
                        {
                            num_family_addrs++;
                        }
                    }

                    if( 1 == num_family_addrs )
                    {
                        ack_msg.asconf_params[resp_ctr].corr_id =
                            p_delip->corr_id;

                        ack_msg.asconf_params[resp_ctr].param_type =
                            SCTP_ASCONF_PARAM_ERR_CAUSE;
                        ack_msg.asconf_params[resp_ctr].err_cause.cause_code =
                            SCTPERR_LASTIPADDRESS;
                        ack_msg.asconf_params[resp_ctr].err_cause.invalid_param =
                            p_msg->asconf_params[sctp_index];
                        resp_ctr++;
                        fail = SCTP_TRUE;
                        break;
                    }

                    /*check that this is not source address*/
                    if( SCTP_SUCCESS == sctp_cmp_addr( &p_msg->src_addr,
                                                       &p_delip->ipaddr ) )
                    {
                        ack_msg.asconf_params[resp_ctr].corr_id =
                            p_delip->corr_id;

                        ack_msg.asconf_params[resp_ctr].param_type =
                            SCTP_ASCONF_PARAM_ERR_CAUSE;
                        ack_msg.asconf_params[resp_ctr].err_cause.cause_code =
                            SCTPERR_SRCIPADDRESS;
                        ack_msg.asconf_params[resp_ctr].err_cause.invalid_param =
                            p_msg->asconf_params[sctp_index];
                        resp_ctr++;
                        fail = SCTP_TRUE;
                        break;
                        /*trying to delete source IP address*/
                    }

                    /*go ahead and delete address*/

                    /*stop the timers running for this destination*/
                    if( SCTP_NULL_TIMER_ID != p_ass->transmit.remote[addr_index].
                            dest_timer_id[SCTP_DESTTIMER_BUNDLING] )
                    {
                        /*send data to this destination*/
                        sctp_tx_state_machine( p_ass, SCTP_TX_EVENT_USERDATA,
                                               addr_index );
                    }

                    for( count = 0; count < SCTP_DESTTIMER_NUM; count++ )
                    {
                        sctp_stop_timer( &( p_ass->transmit.remote[addr_index].
                                            dest_timer_id[count] ), &rem_time, SCTP_NULL );
                    }

                    if( SCTP_NULL_TIMER_ID !=
                            p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK] )
                    {
                        sctp_timer_get_event_rem_time(
                            p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK], &event,
                            &timer_addr, &rem_time );

                        if( timer_addr == addr_index )
                        {
                            sctp_stop_timer(
                                &( p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK] ),
                                &rem_time, SCTP_NULL );
                        }
                    }

                    /* move the data to another address */
                    if( addr_index == --p_ass->transmit.num_remote_address )
                    {
                        sctp_U32bit pr_index = p_ass->transmit.primary_address_index;

                        /*last address successfully deleted*/
                        if( pr_index == addr_index )
                        {
                            pr_index = sctp_get_next_active_addr_index( p_ass, 0 );
                            p_ass->transmit.primary_address_index = pr_index;
                            sctp_ntfy_dst_status_change( p_ass,
                                                         &p_ass->transmit.remote[sctp_index].that_address,
                                                         SCTP_ADDRESS_MADE_PRIM, E_NOERROR );
                        }

                        /* temporarily increment number to allow the function
                         * send data for last address too
                         */
                        p_ass->transmit.num_remote_address++;
                        sctp_move_data_to_primary_addr( p_ass, pr_index );
                        p_ass->transmit.num_remote_address--;

                        sctp_ntfy_dst_status_change( p_ass, &p_delip->ipaddr,
                                                     SCTP_ADDRESS_REMOVED, E_NOERROR );
                        continue;
                    }

                    /*Now copy the last remote info to this index*/
                    p_ass->transmit.remote[addr_index] =
                        p_ass->transmit.remote[p_ass->transmit.num_remote_address];

                    /* Start CSR 1-3307301 */
#ifdef SCTP_STATS_ENABLED

                    p_ass->stats.dest_stats[addr_index].dest_address =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].dest_address;

                    p_ass->stats.dest_stats[addr_index].no_datagrams_sent =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].no_datagrams_sent;

                    p_ass->stats.dest_stats[addr_index].no_datagrams_unacked =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].no_datagrams_unacked;

                    p_ass->stats.dest_stats[addr_index].no_datagrams_recd =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].no_datagrams_recd;

                    p_ass->stats.dest_stats[addr_index].no_bytes_sent =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].no_bytes_sent;

                    p_ass->stats.dest_stats[addr_index].no_bytes_recvd =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].no_bytes_recvd;

                    p_ass->stats.dest_stats[addr_index].dest_addr_start_time =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].dest_addr_start_time;

#ifdef TORNADO_VXWORKS
                    p_ass->stats.dest_stats[addr_index].pad[0] =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].pad[0];
                    p_ass->stats.dest_stats[addr_index].pad[1] =
                        p_ass->stats.dest_stats[p_ass->transmit.num_remote_address].pad[1];
#endif

#endif /* SCTP_STATS_ENABLED */
                    /* End CSR 1-3307301 */

                    /*now check if primary address has changed */
                    if( p_ass->transmit.primary_address_index == addr_index )
                    {
                        sctp_ntfy_dst_status_change( p_ass,
                                                     &p_ass->transmit.remote[addr_index].that_address,
                                                     SCTP_ADDRESS_MADE_PRIM, E_NOERROR );
                    }

                    else if( p_ass->transmit.primary_address_index ==
                             p_ass->transmit.num_remote_address )
                    {
                        p_ass->transmit.primary_address_index =
                            addr_index;
                    }

                    /*move data on this addess to new address*/
                    sctp_move_data_to_primary_addr( p_ass,
                                                    p_ass->transmit.primary_address_index );

                    /* move the timers running to new index*/
                    if( timer_addr == ( p_ass->transmit.num_remote_address ) )
                    {
                        sctp_start_timer( p_ass, SCTPEVENT_TIMERHB,
                                          rem_time, addr_index,
                                          &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
                                          SCTP_NULL );
                    }

                    timer_addr = p_ass->transmit.num_remote_address;

                    for( count = 0; count < SCTP_DESTTIMER_NUM; count++ )
                    {
                        if( SCTP_NULL_TIMER_ID != p_ass->transmit.remote[timer_addr].
                                dest_timer_id[count] )
                        {
                            sctp_timer_get_event_rem_time( p_ass->transmit.remote[timer_addr].
                                                           dest_timer_id[count], &event, &timer_addr, &rem_time );

                            sctp_stop_timer( &( p_ass->transmit.remote[timer_addr].
                                                dest_timer_id[count] ), &rem_time, SCTP_NULL );

                            sctp_start_timer( p_ass, event, rem_time, addr_index,
                                              &( p_ass->transmit.remote[addr_index].dest_timer_id[count] ),
                                              SCTP_NULL );
                        }
                    }

                    sctp_ntfy_dst_status_change( p_ass, &p_delip->ipaddr,
                                                 SCTP_ADDRESS_REMOVED, E_NOERROR );
                }
                break;

            case SCTP_ASCONF_PARAM_SET_PRIM:
                {
                    sctp_U32bit         addr_index;
                    sctp_asconf_setprim_param_st  *p_setprim
                        = &( p_msg->asconf_params[sctp_index].set_primary );

                    for( addr_index = 0;
                            addr_index < p_ass->transmit.num_remote_address;
                            addr_index++ )
                    {
                        if( sctp_cmp_addr( &p_setprim->ipaddr ,
                                           &( p_ass->transmit.remote[addr_index].that_address ) ) )
                        {
                            break;
                        }
                    }

                    if( addr_index == p_ass->transmit.num_remote_address )
                    {
                        break;
                    }

                    if( p_ass->transmit.primary_address_index != addr_index )
                    {
                        p_ass->transmit.primary_address_index = addr_index;
                        sctp_move_data_to_primary_addr( p_ass, addr_index );
                        sctp_ntfy_dst_status_change( p_ass, &p_setprim->ipaddr,
                                                     SCTP_ADDRESS_MADE_PRIM, E_NOERROR );
                    }

                    if( fail )
                    {
                        /*must explicitly notify success*/
                        ack_msg.asconf_params[resp_ctr].corr_id =
                            p_setprim->corr_id;

                        ack_msg.asconf_params[resp_ctr].param_type =
                            SCTP_ASCONF_PARAM_SUCCESS_REPORT;
                        resp_ctr++;
                    }
                }
                break;
        }
    }

    /*
     * Need to send ASCONF to the destination.
     * Must be sent to the source address in packet
     */

    ack_msg.p_unreco_params     = p_msg->p_unreco_params;
    ack_msg.unreco_param_len    = p_msg->unreco_param_len;
    ack_msg.num_params          = resp_ctr;

    /*get an address with same family*/
    for( saddr_index = 0; saddr_index < p_ass->p_ep->naddr; saddr_index++ )
    {
        if( p_ass->p_ep->this_address[saddr_index].family ==
                p_msg->src_addr.family )
        {
            break;
        }
    }

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &p_msg->src_addr;
    sctp_send_chunk_asconf_ack(
        p_ass,
        &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
        p_ass->transmit.that_port,
        &to, p_ass->remote_tag,
        &ack_msg, p_ass->asconf_buffer, &p_ass->buf_len ); /* SPR 20859*/

    p_ass->receive.serial_no = p_msg->serial_no;
}



/*
 * FSM Trigger EVT-TIMERASCONF
 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtTimerAsconf
 *
 *    DESCRIPTION:
 *        This function handles ASCONF timeout event. ASCONF
 *    message was sent for a destination address and no ack was received.
 *    following is performed.
 *
 *    1)Increment parameter path_error_count for the same destination if
 *    it is not already path_max_retrans. If after incrementing
 *    path_error_count it becomes equal to path_max_retrans,
 *    send destination status change notification to ULP.
 *
 *    2) Increment parameter assoc_error_count, If this becomes equal to
 *    assoc_max_retrans close the association and inform ULP of
 *    endpoint status change.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_EvtTimerAsconf(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit      count = 0;
    sctp_U32bit      saddr_index, addr_index;
    sctp_U32bit      pindex = 0;
    sctp_sockaddr_st dst_addr;
    sctp_Boolean_t   primary_chg_flag = SCTP_FALSE;
    sctp_addrinfo_st to;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Gener_TimerASCONF" ) );

    /*the timer is set to NULL only after call to uddate_src_index*/
    addr_index = p_ass->event.evt_timeout.addr_index;


    p_ass->transmit.remote[addr_index].path_error_count++;
    p_ass->error_count++;

    /* backoff rto */
    p_ass->transmit.remote[addr_index].rto *= 2;

    if( p_ass->transmit.remote[addr_index].rto > p_ass->config.rto_max )
    {
        p_ass->transmit.remote[addr_index].rto = p_ass->config.rto_max;
    }

    /* SPR 4033 */
    if( ( p_ass->transmit.remote[addr_index].path_error_count
            % p_ass->transmit.remote[addr_index].path_error_threshold == 0 )
            && p_ass->transmit.remote[addr_index].status == SCTP_COMMUNICATION_UP ) /* SPR 20382*/
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "path error count crosses threshold" ) );

        p_ass->transmit.remote[addr_index].status = SCTP_COMMUNICATION_LOST;

        /* update primary destnation address */
        if( p_ass->transmit.primary_address_index == addr_index )
        {
            pindex = sctp_get_next_active_addr_index( p_ass, addr_index );

            if( p_ass->transmit.remote[pindex].status == SCTP_COMMUNICATION_UP )
            {
                p_ass->transmit.primary_address_index = pindex;
                primary_chg_flag = SCTP_TRUE;
            }
        }

        /* notify user of destination state change */

        dst_addr = p_ass->transmit.remote[addr_index].that_address;

        /* notify service user */
        sctp_ntfy_dst_status_change( p_ass, &dst_addr, SCTP_ADDRESS_UNREACHABLE,
                                     E_NOERROR );

    }

    sctp_db_update_src_addr_index( p_ass, addr_index );

    /* update expired timer id in timer structure, destination timer */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK]
        = SCTP_NULL_TIMER_ID;

    /* SPR 4033 */
    if( p_ass->error_count > p_ass->config.assoc_max_retrans )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "ASCONF max reatransmission reached, aborting" ) );

        saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

        sctp_send_chunk_abort( p_ass,
                               &( p_ass->p_ep->this_address[saddr_index] ),
                               p_ass->p_ep->this_port,
                               p_ass->transmit.that_port,
                               &( p_ass->transmit.remote[addr_index].that_address ),
                               p_ass->remote_tag, SCTP_FALSE, 0, 0, NULL, 0,
                               p_ass->config.checksum_func );

        for( count = 0; count < p_ass->cmd_asconf.num_params; count++ )
        {
            /*retransmnission fails*/
            sctp_proc_asconf_failure( p_ass,
                                      &p_ass->cmd_asconf.asconf_params[count],
                                      0 );
        }

        SCTP_WAKEUP( p_ass->p_ep->asconf_cond );

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

        if( lstFirst( &p_ass->transmit.transmit_queue ) )
        {
            sctp_generic_handle_abort_message( p_ass );
        }

        else
        {
            sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
        }

        return;
    }

    if( primary_chg_flag == SCTP_TRUE )
    {
        sctp_move_data_to_primary_addr( p_ass, pindex );
    }


    /*resend the ASCONF message*/
    /*return FAILURE/WakeUP if assciation gets deleted */
    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] = SCTP_NULL_TIMER_ID;

    /*
     * Need to send ASCONF to the alternate destination .
     * So get the source address index and the destination addres index.
     */
    addr_index = sctp_get_next_active_addr_index( p_ass,
                                                  p_ass->event.evt_timeout.addr_index );
    saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &( p_ass->transmit.remote[addr_index].that_address );

    sctp_send_chunk_asconf( p_ass,
                            &( p_ass->p_ep->this_address[saddr_index] ),
                            p_ass->p_ep->this_port,
                            p_ass->transmit.that_port,
                            &to, p_ass->remote_tag,
                            &( p_ass->cmd_asconf ) );

    /*start timer T-4*/
    sctp_start_timer( p_ass,
                      SCTPEVENT_TIMERASCONF,
                      p_ass->transmit.remote[addr_index].rto,
                      addr_index,
                      &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF],
                      SCTP_NULL );
}

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_MsgAsconfAck
 *
 *    DESCRIPTION: Handles ASCONF ACK message in association ESTABLISHED state
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_MsgAsconfAck(
    sctp_tcb_st    *p_ass )
{
    sctp_U32bit      sctp_rindex = 0;
    sctp_addr_conf_ack_st  *p_msg =  &p_ass->event.msg_addrconfack;
    sctp_U32bit     saddr_index, daddr_index;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctpfsm_assoc_Estbl_MsgAsconfAck" ) );

    if( p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] == SCTP_NULL_TIMER_ID )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "ASCONF ACK not expected" ) );

        if( SCTP_FAILURE == sctp_compare_tsn_for_smaller(
                    p_msg->serial_no, p_ass->transmit.serial_no ) )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sending abort" ) );

            if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass,
                                                                &saddr_index, &daddr_index, SCTP_NULL ) )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                            ( "Could not get compatible address index" ) );

                return;
            }

            /* Send ABORT chunk to peer */
            sctp_send_chunk_abort( p_ass,
                                   &( p_ass->p_ep->this_address[saddr_index] ), p_ass->p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   &( p_ass->transmit.remote[daddr_index].that_address ), p_ass->remote_tag,
                                   SCTP_TRUE, SCTPERR_ILLEGALASCONFACK, 0, NULL, 0,
                                   p_ass->config.checksum_func );

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN, 1 );

            sctp_db_delete_ep( p_ass->p_ep->ep_id, p_ass, SCTP_FALSE );
        }

        return;
    }

    if( p_msg->serial_no != p_ass->cmd_asconf.serial_no )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                    ( "Serial Number don;t match, ignore" ) );

        return;
    }

    /*stop the T-4 timer*/
    SCTP_TRACE( p_ass->p_ep, SCTP_ERR_TRC,
                ( "Stopping the asconf timer" ) );

    sctp_stop_timer( &( p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] ),
                     SCTP_NULL, SCTP_NULL );

    if( SCTP_SUCCESS == sctp_db_validate_remote_addr( p_ass,
                                                      &( p_msg->from_addr ), &daddr_index ) )
    {
        sctp_db_reset_error_counters( p_ass, daddr_index );
    }

    for( sctp_rindex = 0; sctp_rindex < p_msg->num_params; sctp_rindex++ )
    {
        sctp_asconf_ack_param_st *p_ack_param =
            &( p_msg->asconf_params[sctp_rindex] );

        if( p_ack_param->corr_id >= p_ass->cmd_asconf.num_params )
        {
            continue;
        }

        if( p_ack_param->param_type == SCTP_ASCONF_PARAM_ERR_CAUSE )
        {
            p_ass->asconf_response[p_ack_param->corr_id] = 1;

            switch( p_ack_param->err_cause.cause_code )
            {
                case SCTPERR_UNRECOCHUNK:

                    /*restrict further requests on this association*/
                    switch( p_ack_param->err_cause.invalid_param.param_type )
                    {
                        case SCTP_ASCONF_PARAM_ADD_IP:
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
                            p_ass->restrict.addip = SCTP_TRUE;
#else
                            /* CSR#1-4825810 Changes done for warning removal activity
                             * for compilation with SUN cc */
                            p_ass->sctp_restrict.addip = SCTP_TRUE;
#endif
                            break;

                        case SCTP_ASCONF_PARAM_DEL_IP:
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
                            p_ass->restrict.delip = SCTP_TRUE;
#else
                            /* CSR#1-4825810 Changes done for warning removal activity
                             * for compilation with SUN cc */
                            p_ass->sctp_restrict.delip = SCTP_TRUE;
#endif
                            break;

                        case SCTP_ASCONF_PARAM_SET_PRIM:
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
                            p_ass->restrict.setprim = SCTP_TRUE;
#else
                            /* CSR#1-4825810 Changes done for warning removal activity
                             * for compilation with SUN cc */
                            p_ass->sctp_restrict.setprim = SCTP_TRUE;
#endif
                            break;
                    }

                    p_ass->asconf_response[p_ack_param->corr_id] =
                        SCTP_ASCONF_UNRECOPARAM;
                    break;

                case SCTPERR_LASTIPADDRESS:
                    p_ass->asconf_response[p_ack_param->corr_id] =
                        SCTP_ASCONF_ELASTIP;
                    break;

                case SCTPERR_OPREFRESOURCESHORT:
                    p_ass->asconf_response[p_ack_param->corr_id] =
                        SCTP_ASCONF_ERESOURCE;
                    break;

                case SCTPERR_SRCIPADDRESS:
                    p_ass->asconf_response[p_ack_param->corr_id] =
                        SCTP_ASCONF_ESRCADDR;
                    break;

                default:
                    p_ass->asconf_response[p_ack_param->corr_id] =
                        SCTP_ASCONF_EUNKNOWN;

            }
        }

        else
        {
            p_ass->asconf_response[p_ack_param->corr_id] =
                SCTP_ASCONF_SUCCESS;
        }
    }

    for( sctp_rindex = 0; sctp_rindex < p_ass->cmd_asconf.num_params; sctp_rindex++ )
    {

        if( SCTP_ASCONF_NORESPONSE == p_ass->asconf_response[sctp_rindex] )
        {
            p_ass->asconf_response[sctp_rindex] = SCTP_ASCONF_IMPLIED_SUCCESS;
        }

        else if( SCTP_ASCONF_SUCCESS != p_ass->asconf_response[sctp_rindex] )
        {
            /*found an error*/
            break;
        }

        sctp_proc_asconf_success( p_ass,
                                  &p_ass->cmd_asconf.asconf_params[sctp_rindex] );
    }

    for( ; sctp_rindex < p_ass->cmd_asconf.num_params; sctp_rindex++ )
    {
        if( p_ass->asconf_response[sctp_rindex] == SCTP_ASCONF_SUCCESS )
        {
            sctp_proc_asconf_success( p_ass,
                                      &p_ass->cmd_asconf.asconf_params[sctp_rindex] );
        }

        else if( SCTP_ASCONF_NORESPONSE == p_ass->asconf_response[sctp_rindex] )
        {
            p_ass->asconf_response[sctp_rindex] = SCTP_ASCONF_EUNKNOWN;
        }

        sctp_proc_asconf_failure( p_ass,
                                  &p_ass->cmd_asconf.asconf_params[sctp_rindex],
                                  p_ass->asconf_response[sctp_rindex] );
    }

    SCTP_WAKEUP( p_ass->p_ep->asconf_cond );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_proc_asconf_failure
 *
 *    DESCRIPTION: This function handles ASCONF failure
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void sctp_proc_asconf_failure(
    sctp_tcb_st             *p_ass,
    sctp_asconf_param_ut    *p_param,
    sctp_asconf_response_et  ecode )
{
    sctp_U32bit saddr;
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_proc_asconf_failure" ) );

    switch( p_param->param_type )
    {
        case SCTP_ASCONF_PARAM_ADD_IP:

            /*removed the added address*/
            for( saddr = p_ass->p_ep->naddr; saddr > 0; saddr-- )
            {
                if( SCTP_SUCCESS == sctp_cmp_addr( &p_param->add_ip.ipaddr,
                                                   &p_ass->p_ep->this_address[saddr] ) )
                {
                    break;
                }
            }

            if( saddr == 0 )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "could not find the address in ep" ) );
            }

            else
            {
                --p_ass->p_ep->naddr;

                for( ; saddr < ( p_ass->p_ep->naddr ) && ( p_ass->p_ep->naddr <= SCTP_MAX_TRANSPORT_ADDR ); saddr++ ) /*klckwrk 6.2.4 fix*/
                {
                    p_ass->p_ep->this_address[saddr] =
                        p_ass->p_ep->this_address[saddr + 1];
                }
            }

            sctp_ntfy_local_addr_change( p_ass, &p_param->add_ip.ipaddr,
                                         SCTP_LOCAL_ADDRESS_ADD_FAIL, ecode );
            /* Indicate the user */
            break;

        case SCTP_ASCONF_PARAM_DEL_IP:
            sctp_ntfy_local_addr_change( p_ass, &p_param->del_ip.ipaddr,
                                         SCTP_LOCAL_ADDRESS_REM_FAIL, ecode );
            break;

        case SCTP_ASCONF_PARAM_SET_PRIM:
            sctp_ntfy_local_addr_change( p_ass, &p_param->del_ip.ipaddr,
                                         SCTP_LOCAL_ADDRESS_MADE_PRIM_FAIL, ecode );
            break;
    }
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_proc_asconf_success
 *
 *    DESCRIPTION: This function handles Success of ASCONF processing
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void sctp_proc_asconf_success(
    sctp_tcb_st          *p_ass,
    sctp_asconf_param_ut *p_param )
{

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_proc_asconf_failure" ) );

    switch( p_param->param_type )
    {
        case SCTP_ASCONF_PARAM_ADD_IP:
            {
                sctp_U32bit saddr, naddr;
                sctp_U32bit count;

                /* address already Bounnd */
                /* find its index and distribute */
                for( saddr = ( p_ass->p_ep->naddr - 1 ); ( saddr > 0 )
                        && ( p_ass->p_ep->naddr <= SCTP_MAX_TRANSPORT_ADDR ); --saddr ) /*klckwrk 6.2.4 fix*/
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr( &p_param->add_ip.ipaddr,
                                                       &p_ass->p_ep->this_address[saddr] ) )
                    {
                        break;
                    }
                }

                if( 0 == saddr )
                {
                    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                ( "incoreect address in asconf, ignore" ) );
                    break;
                }

                /*
                 * arrange the source addresses on the association
                 * to be fair, this address should be distributed for
                 * num_remote/num_local address
                 */
                naddr = p_ass->transmit.num_remote_address / p_ass->p_ep->naddr;

                for( count = 0; naddr && ( count < p_ass->p_ep->naddr )
                        && ( p_ass->p_ep->naddr <= SCTP_MAX_TRANSPORT_ADDR ); count++ ) /* klockwork 6.2.4 fix */
                {
                    if( p_param->add_ip.ipaddr.family ==
                            p_ass->transmit.remote[count].that_address.family )
                    {
                        p_ass->transmit.remote[count].src_addr_index = saddr;
                        naddr--;
                    }
                }

                /* Start CSR 1-3307301 */
#ifdef SCTP_STATS_ENABLED
                p_ass->stats.local_addr_stats[saddr].local_address =
                    p_ass->p_ep->this_address[saddr];
                __GETSYSTIME( &( p_ass->stats.local_addr_stats[saddr].local_addr_start_time ) );
#endif /* SCTP_STATS_ENABLED */
                /* End CSR 1-3307301 */

                /* Indicate the user */
                sctp_ntfy_local_addr_change( p_ass, &p_param->add_ip.ipaddr,
                                             SCTP_LOCAL_ADDRESS_ADDED, SCTP_ASCONF_SUCCESS );
            }
            break;

        case SCTP_ASCONF_PARAM_DEL_IP:
            /*address allready deleted. indicate the user*/
            {
                sctp_U32bit count;

                for( count = p_ass->p_ep->naddr; count > 0; count-- )
                {
                    if( SCTP_SUCCESS == sctp_cmp_addr( &p_ass->p_ep->this_address[count],
                                                       &p_param->del_ip.ipaddr ) )
                    {
                        break;
                    }
                }

                /* Proceed further only if the p_param->del_ip.ipaddr is found
                 * in p_ass->p_ep->naddr */
                if( count > 0 )
                {
                    /*reference of address already deleted, just delete this entry */
                    --p_ass->p_ep->naddr;

                    for( ; ( count < p_ass->p_ep->naddr )
                            && ( p_ass->p_ep->naddr <= SCTP_MAX_TRANSPORT_ADDR ); count++ ) /* klockwork 6.2.4 fix */
                    {
                        p_ass->p_ep->this_address[count] =
                            p_ass->p_ep->this_address[count + 1];
                        /* Start CSR 1-3307301 */
#ifdef SCTP_STATS_ENABLED
                        p_ass->stats.local_addr_stats[count].local_address =
                            p_ass->stats.local_addr_stats[count + 1].local_address;
                        p_ass->stats.local_addr_stats[count].local_addr_start_time =
                            p_ass->stats.local_addr_stats[count + 1].local_addr_start_time;
#endif /* SCTP_STATS_ENABLED */
                        /* End CSR 1-3307301 */
                    }

                    /*indicate the user*/
                    sctp_ntfy_local_addr_change( p_ass, &p_param->del_ip.ipaddr,
                                                 SCTP_LOCAL_ADDRESS_REMOVED, SCTP_ASCONF_SUCCESS );
                }
            }
            break;

        case SCTP_ASCONF_PARAM_SET_PRIM:
            {
                /*SPR 5085*/
                /*removed*/
                /*SPR 5085*/
                sctp_ntfy_local_addr_change( p_ass, &p_param->set_primary.ipaddr,
                                             SCTP_LOCAL_ADDRESS_MADE_PRIM, SCTP_ASCONF_SUCCESS );
            }
            break;
    }
}

#endif

/* SPR 6854 */

/***************************************************************************
 *    FUNCTION :
 *        sctpfsm_assoc_Estbl_EvtTimerLHbeat
 *
 *    DESCRIPTION:
 *    This function is invoked from the fsm, when the
 *    SCTP_ASSOCTIMER_LOCALHBEAT timer has expired, after the configured
 *    primary path was marked inactive. We simply send a heart beat chunk
 *    to the destination with the source address as our primary ip address.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctpfsm_assoc_Estbl_EvtTimerLHbeat(
    sctp_tcb_st  *p_ass )
{
    sctp_addrinfo_st      to;
    sctp_U32bit saddr_index = 0;
    sctp_U32bit daddr_index = 0;

    /* The source address is the primary address that was configured while
     * creating the association */
    saddr_index = p_ass->configured_local_primary_address_index;
    /* The configured primary path index at the time of association creation */
    daddr_index = p_ass->transmit.configured_primary_path_index;

    /*if(p_ass->transmit.remote[daddr_index].status ==
     * SCTP_COMMUNICATION_UP)*/
    to.checksum   = p_ass->config.checksum_func;
    to.p_sockaddr = &p_ass->transmit.remote[daddr_index].that_address;

    /* SPR 21127 starts*/
    SCTP_UNUSED_VARIABLE( to );
    /* SPR 21127 ends */
    /* send the heart beat chunk */
    sctp_send_chunk_heartbeat( p_ass,
                               &( p_ass->p_ep->this_address[saddr_index] ),
                               p_ass->p_ep->this_port, p_ass->transmit.that_port,
                               daddr_index,
                               p_ass->remote_tag );

    p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT] = SCTP_NULL_TIMER_ID;

    /* Start the local heartbeat timer again */
    sctp_start_timer(
        p_ass,
        SCTPEVENT_TIMER_LOCALHBEAT,
        /* SPR 20570 Starts for CSR 1-7626869 */
        SCTP_HB_INTERVAL_TIMEOUT + p_ass->transmit.remote[daddr_index].rto,
        /* SPR 20570 Ends for CSR 1-7626869 */
        daddr_index,
        &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_LOCALHBEAT],
        SCTP_NULL );

}

/* SPR 6854 */
