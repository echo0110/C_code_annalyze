/***************************************************************************
 *  FILE NAME  : s_fsmmtu.c
 *
 *  DESCRIPTION: Contains the Path MTU related functions.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "FSM"

/*
 * Internet Protocol's DF(don't fragment) bit is used to implement Path MTU
 * Discovery feature. This bit suggests IP layer not to fragment data
 * associated with this packet. In case if Data size is more than the Hop's
 * MTU and it can't be transferred without fragmentation IP returns ICMP error
 * (fragmentation needed and DF bit set) to the originator. As UDP does not
 * allow applications to change DF bit per socket basis we need to work with
 * raw IP and ICMP sockets to implement this feature.
 *
 * IP socket will be used to send packets on the network to discover Path MTU
 * (with DF bit set) and ICMP socket will be used to read and process ICMP
 * messages received from IP. As IP passes all the messages pertaining
 * to a protocol to all raw sockets registered with same protocol. This ICMP
 * socket will get a lot of messages and most of them may not be ours. So we
 * will process ICMP messages only when we are expecting something. As ICMP
 * message can arrive at any time, we need to do a read on this ICMP socket
 * all the time and this read may be a part of SCTP receive/timeout thread
 * and we activate this read whenever we  expect something.
 *
 * When association comes up and till we have not calculated Path MTU
 * for the destination we assume a default value (first hop MTU) and do not
 * stop/delay traffic on that association. So the process of discovering PMTU
 * once initiated will asynchronously inform association of correct path MTU
 * through a state machine trigger (EVT_NEWPMTU).We will keep a database of
 * Path MTU values calculated for destinations so it could be used across
 * associations.
 *
 * Rediscovering MTU will be done on per association basis. As it needs to
 * done periodically and it make sense to do it only if some one is using it.
 *
 * We need to add a per association timer (SCTPTIMER_Tx1) to support this
 * feature.
 * This timer will be started at the time of initiating PMTU discovery with
 * value X seconds, in case this timer timeout we will consider a packet
 * loss and reinitiate PMTU discovery. If we get a valid reply we will stop
 * the running SCTPTIMER_Tx1 timer and start it again with value Y minutes.
 * Y is the time for rediscovering PMTU.
 */

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_mhbld.h>
#include <s_fsmtbl.h>
#include <s_cobuf.h>
#include <s_in.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_es.h>
#include <s_fsmmtu.h>
#include <s_dbutil.h>



sctp_U8bit      *free_mtu_pool;
LIST            mtu_freelist;
LIST            mtu_activelist;
sctp_timer_t    pmtu_timer_id;


/***************************************************************************
 *    FUNCTION :
 *        sctp_discover_pmtu
 *
 *    DESCRIPTION:
 *        This function assumes next hop MTU as PMTU value. It builds ip
 *    header and ip data portion of buffer. It will set DF bit in
 *    IP header, assumes IP protocol value of 255 (RAW-IP). This is
 *    a reserve value of protocol field. Get a buffer of size
 *    (PMTU-IP header size) for ip data portion and equate first 32bits
 *    as association id and next 32bits as destination address.
 *    (These will be returned in ICMP message). Then send this buffer
 *    on IP socket.
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_discover_pmtu(
    sctp_tcb_st         *p_ass,
    sctp_sockaddr_st    *p_dest_addr )
{

    if( SCTP_AF_INET == p_dest_addr->family )
    {
        /* Send Pmtu Message for IPV4 */
        sctp_send_pmtu_msg_v4( p_ass, SCTP_MAX_MTUSIZE, p_dest_addr );
    }

#ifdef SCTP_INET6_SUPPORT

    else
    {
        sctp_send_pmtu_msg_v6( p_ass, SCTP_MAX_MTUSIZE, p_dest_addr );
    }

#endif

    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_estimate_next_pmtu_V4
 *
 *    DESCRIPTION:
 *        This function is invoked when ICMP message does not suggest any PMTU
 *    value. This function estimate new PMTU value based on last tried
 *    PMTU value (uses binary search on the list of suggested MTU values).
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_estimate_next_pmtu_V4(
    sctp_U32bit    *mtu_size )
{
    sctp_return_t ret = SCTP_SUCCESS;

    switch( *mtu_size )
    {
        case 68: /* official lowest pmtu */
            ret = SCTP_FAILURE;
            break;

        case 296: /* Point to Point */
            *mtu_size  = 68;
            break;

        case 508: /* ARCNET */
            *mtu_size  = 296;
            break;

        case 1006: /* ARPANET/SLIP */
            *mtu_size  = 508;
            break;

        case 1492: /* IEEE 802.3 */
            *mtu_size  = 1006;
            break;

        case 1500: /* Ethernet */
            *mtu_size  = 1492;
            break;

        case 2002: /*IEEE 802.5 */
            *mtu_size  = 1500;
            break;

        case 4352: /*IEEE 802.5 */
            *mtu_size  = 2002;
            break;

        case 8166: /* IEEE 802.4 */
            *mtu_size  = 4352;
            break;

        default:
            ret = SCTP_FAILURE;
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_estimate_next_pmtu_V6
 *
 *    DESCRIPTION:
 *        This function is invoked when ICMP6 message is received and we have
 *    to get the next MTU to try.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_estimate_next_pmtu_V6(
    sctp_U16bit    *mtu_size )
{
    sctp_return_t ret = SCTP_SUCCESS;

    /* If the MTU becomes less than the least then do not decrease further. */
    if( *mtu_size < SCTP_IPV6_MIN_LINK_MTU )
    {
        *mtu_size = SCTP_IPV6_MIN_LINK_MTU;

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_estimate_next_pmtu_V6: The MTU received is less than least"
                      "possible i.e %d", *mtu_size ) );

        return ( SCTP_SUCCESS );
    }

    switch( *mtu_size )
    {
        case 1492: /* IEEE 802.3 */
            *mtu_size  = 1280; /* The IPv6 minimum MTU */
            break;

        case 1500: /* Ethernet */
            *mtu_size  = 1492;
            break;

        case 2002: /*IEEE 802.5 */
            *mtu_size  = 1500;
            break;

        case 4352: /*IEEE 802.5 */
            *mtu_size  = 2002;
            break;

        case 8166: /* IEEE 802.4 */
            *mtu_size  = 4352;
            break;

        default:
            ret = SCTP_FAILURE;
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_dest_parameters
 *
 *    DESCRIPTION:
 *        Access SCTP database table storing PMTU parameters and get PMTU
 *    related parameters for a destination address. If destination
 *    exists this will return Path MTU and time stamp when this was
 *    updated else this function will return failure.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_get_dest_parameters(
    sctp_sockaddr_st   *dest_addr,
    sctp_U16bit        *mtu_size,
    sctp_time_t        *time_stamp )
{
    sctp_pmtu_node_st *T;

    for( T = ( sctp_pmtu_node_st * )lstFirst( &mtu_activelist ); T;
            T = ( sctp_pmtu_node_st * )lstNext( ( NODE * )T ) )
    {
        if( sctp_cmp_addr( &T->addr, dest_addr ) == SCTP_FAILURE )
        {
            continue;
        }

        *mtu_size = T->mtu_size;

        time_stamp->tv_sec = T->time_stamp.tv_sec;
        time_stamp->tv_usec = T->time_stamp.tv_usec;

        return ( SCTP_SUCCESS );
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_set_dest_parameters
 *
 *    DESCRIPTION:
 *        Access SCTP database table storing PMTU parameters and add/modify
 *    PMTU related parameters for a destination address. If destination
 *    address does not exist add PMTU related values else modify them.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_set_dest_parameters(
    sctp_sockaddr_st      *dest_addr,
    sctp_U16bit           mtu_size )
{
    sctp_pmtu_node_st  *p_mtu_node = SCTP_NULL;
    sctp_time_t        curr_time;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_set_dest_parameters" ) );

    p_mtu_node  = ( sctp_pmtu_node_st * ) lstFirst( &mtu_activelist );

    for( ; p_mtu_node ; p_mtu_node = ( sctp_pmtu_node_st * )
                                     lstNext( ( NODE * ) p_mtu_node ) )
    {
        if( sctp_cmp_addr( &p_mtu_node->addr, dest_addr ) == SCTP_FAILURE )
        {
            continue;
        }

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_set_dest_parameters, got matching addr" ) );

        /*  Update node parameters */
        p_mtu_node->mtu_size = mtu_size;

        __GETSYSTIME( &curr_time );
        p_mtu_node->time_stamp.tv_sec = curr_time.tv_sec;
        p_mtu_node->time_stamp.tv_usec = curr_time.tv_usec;

        return ( SCTP_SUCCESS );
    }

    /*
     * Create new element in active list.
     */

    p_mtu_node = ( sctp_pmtu_node_st * ) lstFirst( &mtu_freelist );

    if( !p_mtu_node ) /* Can consider adding element on the fly */
    {
        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_set_dest_parameters, new element added" ) );

    lstDelete( &mtu_freelist, ( NODE * )p_mtu_node );
    lstAdd( &mtu_activelist, ( NODE * )p_mtu_node );

    p_mtu_node->mtu_size = mtu_size;
    p_mtu_node->addr = *dest_addr;
    __GETSYSTIME( &curr_time );
    p_mtu_node->time_stamp.tv_sec = curr_time.tv_sec;
    p_mtu_node->time_stamp.tv_usec = curr_time.tv_usec;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "mtu:%d,addr:",
                                           p_mtu_node->mtu_size ) );

    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC, ( &p_mtu_node->addr ) );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_get_pathmtu
 *
 *    DESCRIPTION:
 *        This function will be called from association state machine function
 *    either at the time of association establishment or after the rediscover
 *    timer haexpired. This function will access the MTU table maintained
 *    by SCTP to access the current Path MTU parameters(MTU size and time stamp)
 *    for a destination.
 *    It will initiate rediscovering Path MTU if the value extracted is too old.
 *    In case the destination address does exist in the table a default value
 *    is returned and the process of discovering MTU is started.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_get_pathmtu(
    sctp_tcb_st            *p_ass,
    sctp_sockaddr_st       *dest_addr,
    sctp_U16bit            *mtu_size )
{
    sctp_return_t ret;
    sctp_time_t   time_stamp;
    sctp_time_t   curr_time;
    sctp_U32bit   time_diff;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_get_pathmtu assoc id =%u, dest addr", p_ass->p_ep->ep_id ) );

    SCTP_TRACE_ADDR( p_ass->p_ep, SCTP_DBG_TRC, dest_addr );

    if( p_ass->config.pmtu_capable == SCTP_FALSE )
    {
        /* PMTU disable ,just return the default mtu */
        *mtu_size  = p_ass->config.mtu_size;

        return ( SCTP_SUCCESS );
    }

    ret = sctp_get_dest_parameters( dest_addr, mtu_size, &time_stamp );

    if( ret == SCTP_SUCCESS )
    {
        __GETSYSTIME( &curr_time );
        ret = sctp_calculate_time_difference( &curr_time, &time_stamp,
                                              &time_diff );

        if( ( ret == SCTP_SUCCESS )  && ( time_diff < SCTP_PMTU_REDISCOVER_TIME ) )
        {
            return( SCTP_SUCCESS );
        }

        else
        {
            /* Rediscover path mtu */
            sctp_discover_pmtu( p_ass, dest_addr );
            *mtu_size = SCTP_DEFAULT_MTUSIZE;

            return( SCTP_FAILURE );
        }
    }

    /* Rediscover pmtu */
    ret = sctp_set_dest_parameters( dest_addr, SCTP_DEFAULT_MTUSIZE );

    if( ret != SCTP_SUCCESS )
    {
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                    E_PMTU_DATABASE_OVERFLOW, "Pmtu database Overflowed t" );

        /* can consider adding element on the fly */
    }

    sctp_discover_pmtu( p_ass, dest_addr );
    *mtu_size  = SCTP_DEFAULT_MTUSIZE;

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_pmtu_database
 *
 *    DESCRIPTION: This function initializes PMTU database and also start
 *        timer for pmtu rediscovery
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_init_pmtu_database(
    sctp_error_t    *p_ecode )
{
    sctp_U32bit       counter;
    sctp_pmtu_node_st *p_mtu_node;

    free_mtu_pool = ( sctp_U8bit * )__MALLOC( SCTP_MAX_PMTU_NODES *
                                              sizeof( sctp_pmtu_node_st ) );

    if( free_mtu_pool == NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "PMTU database Initialisation Failed, No Resources" ) );

        *p_ecode = E_NORESOURCES;

        return ( SCTP_FAILURE );
    }


    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "PMTU database Initialised Successfully" ) );

    __MEMSET( free_mtu_pool, 0, SCTP_MAX_PMTU_NODES * sizeof( sctp_pmtu_node_st ) );

    /* Initialise free list */
    lstInit( &mtu_freelist );
    p_mtu_node = ( sctp_pmtu_node_st * )( free_mtu_pool );

    for( counter = 0; counter < SCTP_MAX_PMTU_NODES; counter++ )
    {
        lstAdd( &mtu_freelist, ( NODE * )( &p_mtu_node[counter] ) );
    }

    /* Initialise active list */
    lstInit( &mtu_activelist );

    /* start mtu refresh timer */
    sctp_start_timer( SCTP_NULL,
                      SCTPEVENT_TIMERPMU_DB_UPDATE,
                      2 * SCTP_PMTU_REDISCOVER_TIME,
                      0, &pmtu_timer_id, SCTP_NULL );

    return ( SCTP_SUCCESS );
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_deinit_pmtu_database
 *
 *    DESCRIPTION:  This function de-initializes PMTU database and stops
 *        corresponding timer.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *************************************************************************/
sctp_return_t
sctp_deinit_pmtu_database( void )
{

    /* start mtu refresh timer */

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "PMTU database Initialised Successfully" ) );
    sctp_stop_timer( &pmtu_timer_id, SCTP_NULL, SCTP_NULL );
    __FREE( free_mtu_pool );


    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_pmtu_dbupdate_timeout
 *
 *    DESCRIPTION: This function process PMTU database update timeout and also
 *       restart timer for pmtu rediscovery.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_process_pmtu_dbupdate_timeout(
    void )
{
    sctp_time_t       curr_time;
    sctp_U32bit       time_diff;
    sctp_return_t     ret;
    sctp_pmtu_node_st *T;
    sctp_pmtu_node_st *save;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_process_pmtu_dbupdate_timeout" ) );

    __GETSYSTIME( &curr_time );

    for( T = ( sctp_pmtu_node_st * )lstFirst( &mtu_activelist ); T; )
    {
        save = ( sctp_pmtu_node_st * )lstNext( ( NODE * )T );
        ret = sctp_calculate_time_difference( &curr_time, &T->time_stamp,
                                              &time_diff );

        if( ( ret == SCTP_SUCCESS )  && ( time_diff < 2 *
                                          SCTP_PMTU_REDISCOVER_TIME ) )
        {
            /* In use node, no action needed */
        }
        else
        {
            /* Node not in use remove this node from pmtu database */
            lstDelete( &mtu_activelist, ( NODE * )T );
            lstAdd( &mtu_freelist, ( NODE * )T );
        }

        T = save;
    }

    /* start mtu refresh timer */
    sctp_start_timer( SCTP_NULL,
                      SCTPEVENT_TIMERPMU_DB_UPDATE,
                      ( 2 * SCTP_PMTU_REDISCOVER_TIME ),
                      0, &pmtu_timer_id, SCTP_NULL );
}

/***************************************************************************
 *    FUNCTION :
 *       sctp_set_pmtu_capable
 *
 *    DESCRIPTION:
 *    This function enable/disable pmtu discover procedure. If procedure is
 *    already started then stop it and start all timers.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_set_pmtu_capable(
    sctp_tcb_st         *p_ass,
    sctp_U32bit         parameter_value,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit count;
    sctp_return_t ret;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_set_pmtu_capable" ) );

    if( ( p_ass->config.pmtu_capable == SCTP_TRUE ) &&
            ( SCTP_FALSE == parameter_value ) )
    {
        for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
        {
            sctp_stop_timer(
                &p_ass->transmit.remote[count].dest_timer_id[SCTP_DESTTIMER_PMTU],
                SCTP_NULL, SCTP_NULL );
        }

        p_ass->config.pmtu_capable = parameter_value;
    }

    else if( ( p_ass->config.pmtu_capable == SCTP_FALSE ) &&
             ( SCTP_TRUE == parameter_value ) )
    {
        p_ass->config.pmtu_capable = parameter_value;

        for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
        {
            ret = sctp_get_pathmtu( p_ass,
                                    &p_ass->transmit.remote[count].that_address,
                                    &p_ass->transmit.remote[count].path_mtu );

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
                    count,
                    &p_ass->transmit.remote[count].
                    dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
            }

            else
            {
                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERPMTU,
                    SCTP_PMTU_DISCOVER_FAIL_TIME,
                    count,
                    &p_ass->transmit.remote[count].
                    dest_timer_id[SCTP_DESTTIMER_PMTU], SCTP_NULL );
            }

            if( p_ass->transmit.remote[count].path_mtu <
                    p_ass->transmit.lowest_path_mtu )
            {
                p_ass->transmit.lowest_path_mtu
                    = p_ass->transmit.remote[count].path_mtu;
            }
        }
    }

    else
    {
        *p_ecode = E_PMTU_CAPABLE_ALREADY_SET;
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}
/***************************************************************************
 *    FUNCTION :
 *        sctp_set_mtu_size
 *
 *    DESCRIPTION:
 *    This function is used to set the mtu size in assoc table
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t  sctp_set_mtu_size(
    sctp_tcb_st         *p_ass,
    sctp_U32bit         parameter_value,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit count;

    /*** You can change mtu_size only in case of PMTU
     * procedure is disabled.
     * If enable return failure with error
     * else set mtu size in all dest path.
     * ****/
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "In function sctp_set_mtu_size" ) );

    if( p_ass->config.pmtu_capable == SCTP_TRUE )
    {
        *p_ecode = E_PMTU_PROCEDURE_DISABLED;
        return SCTP_FAILURE;
    }

    else
    {
        p_ass->transmit.lowest_path_mtu = parameter_value;
        p_ass->config.mtu_size  = parameter_value;

        for( count = 0; count < p_ass->transmit.num_remote_address; count++ )
        {
            p_ass->transmit.remote[count].path_mtu = parameter_value;
        }
    }

    return SCTP_SUCCESS;
}
