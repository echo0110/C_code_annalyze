/*******************************************************************************
 *    FILE NAME:    s_stats.c
 *
 *    DESCRIPTION:
 *    This file is a part of SCTP statistics module. This will include
 *    System Management APIs for enabling/disabling/initialising Statistics.
 *
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *    26Sept 2001     Yogesh Gahlaut   -       Rel 3.0 Changes
 *    7 July 2008     Shikha Kathpal           REL 6.2.0 rfc upgrade
 *    12july10        Sharad Kumar Yadav       Rel 6.2.0  SPR 20526
 *                                             Fix for CSR#1-7670333
 *    15Sep14         Priyanka Mishra   Bug Id 22        CSR 85223
 *  Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/

#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_dbg.h>
#include <s_stats.h>
#include <s_dbtcb.h>
#include <s_mibtyp.h>

/* Start SPR 20526 */
#include <s_in.h>
#include <s_err.h>
/* End SPR 20526 */


#ifdef SCTP_STATS_ENABLED

sctp_stats_state_et sctp_stats_state = SCTP_STATS_STATE_DISABLED;

/*
 * This data struture contains the enable disable state of each stats type
 */
sctp_Boolean_t sctp_stats_table[SCTP_ALL_STATS];

/*
 * Contains the SCTP Internal protocol related statistics of SCTP stack entity.
 */
sctp_proto_internal_stats_st sctp_protocol_stats;

/* Contains the Traffic related statistics of SCTP stack entity.*/
sctp_global_traf_stats_st     sctp_global_traffic_stats;

/* Contains the Errors related statistics for SCTP stack entity.*/
sctp_error_stats_st sctp_error_stats;

/* Contains association specific statistics for SCTP stack entity.*/


/* forward declaration */
static void
sctp_init_prot_internal_stats(
    void );

static void
sctp_init_global_traffic_stats(
    void );

static void
sctp_init_error_stats(
    void );

/* for comilation only need to remove it */
/*Bug Id 22 fix starts*/
/*static sctp_U32bit sctp_stats_max_num_assoc = 0;*/
/*Bug Id 22 fix ends*/
#endif
/***************************************************************************
 *    FUNCTION :    sctp_api_init_stats
 *
 *    DESCRIPTION:
 *    This function initializes all the stats types data structures
 *    to 0 value
 *    RETURN VALUE:
 *    SS7_SUCCESS
 *    SS7_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_api_init_stats(
    sctp_stats_et       stats_type,
    sctp_error_t        *p_ecode )
{
#ifdef SCTP_STATS_ENABLED

    if( sctp_stats_state != SCTP_STATS_STATE_ENABLED )
    {
        *p_ecode = E_STATSDISABLED;
        return( SCTP_FAILURE );
    }

    /* based on    stats type initialize the statistics counters */
    switch( stats_type )
    {
        case SCTP_PROT_INTERNAL_STATS:
            sctp_init_prot_internal_stats();
            break;

        case SCTP_GLOBAL_TRAFFIC_STATS:
            sctp_init_global_traffic_stats();
            break;

        case SCTP_ERROR_STATS:
            sctp_init_error_stats();
            break;

        case SCTP_ASSOC_STATS:
            sctp_init_assoc_stats( SCTP_NULL, SCTP_TRUE );
            break;

        case SCTP_ALL_STATS:
            sctp_init_prot_internal_stats();
            sctp_init_global_traffic_stats();
            sctp_init_error_stats();
            sctp_init_assoc_stats( SCTP_NULL, SCTP_TRUE );
            break;

        default:
            *p_ecode = EINV_TYPEOFSTATS;
            return( SCTP_FAILURE );
    }

    return( SCTP_SUCCESS );
#else

    if( SCTP_NULL != p_ecode )
    {
        *p_ecode = E_STATSDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( stats_type );
    /*to avoid the warning*/


    return( SCTP_FAILURE );
#endif
}

/***************************************************************************
 *    FUNCTION :    sctp_api_get_stats
 *
 *    DESCRIPTION:
 *    This function returns the statistics collected for the type
 *    specified in the call
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_api_get_stats(
    sctp_stats_et    stats_type,
    sctp_U32bit      association_id,
    sctp_U8bit       *stats_data,
    sctp_Boolean_t     reset_flag,
    sctp_error_t     *p_ecode )
{
#ifdef SCTP_STATS_ENABLED
    sctp_ep_st    *p_ep;
    sctp_tcb_st   *p_ass;
    sctp_return_t ret =    SCTP_SUCCESS;

    if( ( stats_data == SCTP_NULL ) || ( p_ecode == SCTP_NULL ) )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stats_state != SCTP_STATS_STATE_ENABLED )
    {
        *p_ecode = E_STATSDISABLED;
        return( SCTP_FAILURE );
    }

    /* based on stats type initialize the statistics counters */
    switch( stats_type )
    {
        case SCTP_PROT_INTERNAL_STATS:
            if( sctp_stats_table[stats_type] )
            {
                __MEMCOPY( ( sctp_U8bit * )stats_data,
                           ( sctp_U8bit * )( &sctp_protocol_stats ),
                           sizeof( sctp_proto_internal_stats_st ) );

                /* CSR# 1-4779987 start */
                if( reset_flag == SCTP_TRUE )
                {
                    /* initialise protocol internal stats */
                    sctp_init_prot_internal_stats();
                }

                /* CSR# 1-4779987 end */
            }

            else
            {
                *p_ecode = E_STATSDISABLED;
                ret = SCTP_FAILURE;
            }

            break;

        case SCTP_GLOBAL_TRAFFIC_STATS:
            if( sctp_stats_table[stats_type] )
            {
                __MEMCOPY( ( sctp_U8bit * )stats_data,
                           ( sctp_U8bit * )( &sctp_global_traffic_stats ),
                           sizeof( sctp_global_traf_stats_st ) );

                /* CSR# 1-4779987 start */
                if( reset_flag == SCTP_TRUE )
                {
                    /* initialise global traffic stats */
                    sctp_init_global_traffic_stats();
                }

                /* CSR# 1-4779987 end */
            }

            else
            {
                *p_ecode = E_STATSDISABLED;
                ret = SCTP_FAILURE;
            }

            break;

        case SCTP_ERROR_STATS:
            if( sctp_stats_table[stats_type] )
            {
                __MEMCOPY( ( sctp_U8bit * )stats_data,
                           ( sctp_U8bit * )( &sctp_error_stats ),
                           sizeof( sctp_error_stats_st ) );

                /* CSR# 1-4779987 start */
                if( reset_flag == SCTP_TRUE )
                {
                    /* initialise error stats */
                    sctp_init_error_stats();
                }

                /* CSR# 1-4779987 end */
            }

            else
            {
                *p_ecode = E_STATSDISABLED;
                ret = SCTP_FAILURE;
            }

            break;

        case SCTP_ASSOC_STATS:
            if( sctp_stats_table[stats_type] )
            {
                if( ( p_ep = sctp_db_get_ep_ptr( association_id ) ) == SCTP_NULL
                        || ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 1 )
                        || ( ( p_ass = sctp_db_get_assoc_from_ep( p_ep ) ) == SCTP_NULL ) )
                {
                    *p_ecode =    EINV_ASSOCID;
                    ret = SCTP_FAILURE;
                }

                else
                {
                    __MEMCOPY( ( sctp_U8bit * )stats_data,
                               ( sctp_U8bit * )( &p_ass->stats ),
                               sizeof( sctp_assoc_stats_st ) );

                    /* CSR# 1-4779987 start */
                    if( reset_flag == SCTP_TRUE )
                    {
                        sctp_init_assoc_stats( p_ass, SCTP_FALSE );
                    }

                    /* CSR# 1-4779987 end */
                }
            }

            else
            {
                *p_ecode = E_STATSDISABLED;
                ret = SCTP_FAILURE;
            }

            break;

        default:
            *p_ecode = EINV_TYPEOFSTATS;
            ret = SCTP_FAILURE;
            break;
    }

    return( ret );
#else

    if( SCTP_NULL != p_ecode )
    {
        *p_ecode = E_STATSDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( stats_type );
    SCTP_UNUSED_VARIABLE( association_id );
    SCTP_UNUSED_VARIABLE( reset_flag );
    SCTP_UNUSED_VARIABLE( stats_data );
    /*to avoid the warning*/


    return( SCTP_FAILURE );
#endif
}

/***************************************************************************
 *    FUNCTION :    sctp_api_disable_stats
 *
 *    DESCRIPTION:
 *    This function disables the stats collection for the stats    type
 *    passed in    the call
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_api_disable_stats(
    sctp_stats_et       stats_type,
    sctp_error_t        *p_ecode )
{
#ifdef SCTP_STATS_ENABLED

    if( p_ecode == SCTP_NULL )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stats_state != SCTP_STATS_STATE_ENABLED )
    {
        *p_ecode = E_STATSDISABLED;
        return( SCTP_FAILURE );
    }

    /* based on stats type initialize the statistics counters */
    switch( stats_type )
    {
        case SCTP_PROT_INTERNAL_STATS:
            sctp_stats_table[SCTP_PROT_INTERNAL_STATS] = SCTP_FALSE;
            __GETSYSTIME( &( sctp_global_traffic_stats.discont_time ) );
            break;

        case SCTP_GLOBAL_TRAFFIC_STATS:
            sctp_stats_table[SCTP_GLOBAL_TRAFFIC_STATS] = SCTP_FALSE;
            __GETSYSTIME( &( sctp_global_traffic_stats.discont_time ) );
            break;

        case SCTP_ERROR_STATS:
            sctp_stats_table[SCTP_ERROR_STATS] = SCTP_FALSE;
            break;

        case SCTP_ALL_STATS:
            sctp_stats_table[SCTP_PROT_INTERNAL_STATS] = SCTP_FALSE;
            sctp_stats_table[SCTP_GLOBAL_TRAFFIC_STATS]    = SCTP_FALSE;
            sctp_stats_table[SCTP_ERROR_STATS] = SCTP_FALSE;
            __GETSYSTIME( &( sctp_global_traffic_stats.discont_time ) );

            /*fall on*/
        case SCTP_ASSOC_STATS:
            {
                sctp_U32bit count = 0;
                sctp_stats_table[SCTP_ASSOC_STATS] = SCTP_FALSE;

                for( count = 0; count < sctp_db_main.max_assoc; count++ )
                {
                    sctp_tcb_st  *p_ass = &( sctp_db_main.assoc_table[count] );

                    if( p_ass->open )
                    {
                        __GETSYSTIME( &( p_ass->stats.discont_time ) );
                    }
                }
            }
            break;

        default:
            *p_ecode = EINV_TYPEOFSTATS;
            return( SCTP_FAILURE );
    }

    return( SCTP_SUCCESS );
#else

    if( SCTP_NULL != p_ecode )
    {
        *p_ecode = E_STATSDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( stats_type );
    /*to avoid the warning*/


    return( SCTP_FAILURE );
#endif
}

/***************************************************************************
 *    FUNCTION :    sctp_api_enable_stats
 *
 *    DESCRIPTION:
 *    This function enables the stats collection for the stats type
 *    passed in the call
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_api_enable_stats(
    sctp_stats_et       stats_type,
    sctp_error_t        *p_ecode )
{
#ifdef SCTP_STATS_ENABLED

    if( p_ecode == SCTP_NULL )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stats_state != SCTP_STATS_STATE_ENABLED )
    {
        *p_ecode = E_STATSDISABLED;
        return( SCTP_FAILURE );
    }

    /* based on stats type initialize the statistics counters */
    switch( stats_type )
    {
        case SCTP_PROT_INTERNAL_STATS:
            sctp_stats_table[SCTP_PROT_INTERNAL_STATS] = SCTP_TRUE;
            break;

        case SCTP_GLOBAL_TRAFFIC_STATS:
            sctp_stats_table[SCTP_GLOBAL_TRAFFIC_STATS]    = SCTP_TRUE;
            break;

        case SCTP_ERROR_STATS:
            sctp_stats_table[SCTP_ERROR_STATS] = SCTP_TRUE;
            break;

        case SCTP_ASSOC_STATS:
            sctp_stats_table[SCTP_ASSOC_STATS] = SCTP_TRUE;
            break;

        case SCTP_ALL_STATS:
            sctp_stats_table[SCTP_PROT_INTERNAL_STATS] = SCTP_TRUE;
            sctp_stats_table[SCTP_GLOBAL_TRAFFIC_STATS] = SCTP_TRUE;
            sctp_stats_table[SCTP_ERROR_STATS] = SCTP_TRUE;
            sctp_stats_table[SCTP_ASSOC_STATS] = SCTP_TRUE;
            break;

        default:
            *p_ecode = EINV_TYPEOFSTATS;
            return( SCTP_FAILURE );
    }

    return( SCTP_SUCCESS );
#else

    if( SCTP_NULL != p_ecode )
    {
        *p_ecode = E_STATSDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( stats_type );
    /*to avoid the warning*/


    return( SCTP_FAILURE );
#endif
}

#ifdef SCTP_STATS_ENABLED
/***************************************************************************
 *    FUNCTION :    sctp_init_prot_internal_stats
 *
 *    DESCRIPTION:
 *    This function initializes the statistcs count for all parameter
 *    of Internal SCTP protocol statistics.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
static void
sctp_init_prot_internal_stats(
    void )
{
    sctp_protocol_stats.no_applications_reg = 0;
    sctp_protocol_stats.no_associations_open = 0;
    sctp_protocol_stats.no_dest_marked_active = 0;
    sctp_protocol_stats.no_dest_marked_inactive =    0;
    sctp_protocol_stats.no_endpoints_marked_up = 0;
    sctp_protocol_stats.no_endpoints_marked_down = sctp_db_main.max_ep;

    return;

}

/***************************************************************************
 *    FUNCTION :   sctp_init_global_traffic_stats
 *
 *    DESCRIPTION:
 *    This function initializes the statistcs count for all parameter
 *    of global traffic statistics.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
static void
sctp_init_global_traffic_stats(
    void )
{
    sctp_global_traffic_stats.no_assoc_accepted = 0;
    sctp_global_traffic_stats.no_assoc_initiated = 0;
    sctp_global_traffic_stats.no_graceful_shutdown_assocs = 0;
    sctp_global_traffic_stats.no_ungraceful_shutdown_assocs = 0;

    sctp_global_traffic_stats.no_trans_data_chunks = 0;
    sctp_global_traffic_stats.no_retrans_data_chunks = 0;

    sctp_global_traffic_stats.no_data_chunks_sent = 0;
    sctp_global_traffic_stats.no_data_chunks_unacked = 0;
    sctp_global_traffic_stats.no_ord_data_chunks_sent = 0;
    sctp_global_traffic_stats.no_unord_data_chunks_sent = 0;

    sctp_global_traffic_stats.no_data_chunks_recv = 0;
    sctp_global_traffic_stats.no_ord_data_chunks_recv = 0;
    sctp_global_traffic_stats.no_unord_data_chunks_recv = 0;

    sctp_global_traffic_stats.no_bytes_sent_ok = 0;
    sctp_global_traffic_stats.no_bytes_recvd_ok = 0;

    sctp_global_traffic_stats.no_init_sent = 0;
    sctp_global_traffic_stats.no_init_recv = 0;

    sctp_global_traffic_stats.no_initack_sent = 0;
    sctp_global_traffic_stats.no_initack_recv = 0;

    sctp_global_traffic_stats.no_cookie_sent = 0;
    sctp_global_traffic_stats.no_cookie_recv = 0;

    sctp_global_traffic_stats.no_cookieack_sent = 0;
    sctp_global_traffic_stats.no_cookieack_recv = 0;

    sctp_global_traffic_stats.no_hb_sent = 0;
    sctp_global_traffic_stats.no_hb_recv = 0;

    sctp_global_traffic_stats.no_hback_sent = 0;
    sctp_global_traffic_stats.no_hback_recv = 0;

    sctp_global_traffic_stats.no_sack_sent = 0;
    sctp_global_traffic_stats.no_sack_recv = 0;

    sctp_global_traffic_stats.no_abort_sent = 0;
    sctp_global_traffic_stats.no_abort_recv = 0;

    sctp_global_traffic_stats.no_shut_sent = 0;
    sctp_global_traffic_stats.no_shut_recv = 0;

    sctp_global_traffic_stats.no_shutack_sent = 0;
    sctp_global_traffic_stats.no_shutack_recv = 0;

    sctp_global_traffic_stats.no_error_sent = 0;
    sctp_global_traffic_stats.no_error_recv = 0;

    sctp_global_traffic_stats.no_ecne_sent = 0;
    sctp_global_traffic_stats.no_ecne_recv = 0;

    sctp_global_traffic_stats.no_cwr_sent = 0;
    sctp_global_traffic_stats.no_cwr_recv = 0;

    sctp_global_traffic_stats.no_datagrams_recvd_in_error = 0;
    sctp_global_traffic_stats.no_chk_sum_err_recv = 0;

    sctp_global_traffic_stats.no_ootb_recv = 0;

    sctp_global_traffic_stats.no_frag_usr_msg = 0;
    sctp_global_traffic_stats.no_reassemble_usr_msg = 0;

    sctp_global_traffic_stats.no_packets_sent = 0;
    sctp_global_traffic_stats.no_packets_recv = 0;
    /*SPR 21141 changes starts*/
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_global_traffic_stats.no_asconf_sent = 0;
    sctp_global_traffic_stats.no_asconf_recv = 0;

    sctp_global_traffic_stats.no_asconfack_sent = 0;
    sctp_global_traffic_stats.no_asconfack_recv = 0;
#endif

    /*SPR 21141 changes ends*/
    return;

}

/***************************************************************************
 *    FUNCTION :    sctp_init_error_stats
 *
 *    DESCRIPTION:
 *    This function initializes the statistcs count for all parameter
 *    of Error statistics.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
static void
sctp_init_error_stats(
    void )
{
    sctp_error_stats.API_error_stats_count = 0;
    sctp_error_stats.protcol_error_stats_count = 0;
    sctp_error_stats.system_error_stats_count = 0;
    sctp_error_stats.system_resource_error_stats_count = 0;
    sctp_error_stats.internal_error_stats_count = 0;

    return;

}

/***************************************************************************
 *    FUNCTION :    sctp_init_assoc_stats
 *
 *    DESCRIPTION:
 *    This function initializes the statistcs count for all parameter
 *    of association statistics.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_init_assoc_stats(
    sctp_tcb_st    *p_ass,
    sctp_Boolean_t flag )
{
    /* SCTP REL 6.2.0 change start */
    sctp_U32bit    count, addr_id, num;
    /* SCTP REL 6.2.0 change end */
    sctp_timestamp cur_time;

    if( flag == SCTP_TRUE )
    {
        for( count = 0; count < sctp_db_main.max_assoc; count++ )
        {
            p_ass = &( sctp_db_main.assoc_table[count] );
            p_ass->stats.no_chunks_retrans = 0;
            p_ass->stats.no_T1_expired = 0;
            p_ass->stats.no_T2_expired = 0;

            /*Buffer Stats*/
            p_ass->stats.buffer_stat.tx_med_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.tx_max_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.tx_max_utilized_instead_med = 0;

            p_ass->stats.buffer_stat.rx_med_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.rx_max_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.rx_max_utilized_instead_med = 0;

            /* SCTP REL 6.2.0  change start */
            for( num = 0; num < SCTP_MAX_NUMSTREAM ; num++ )
            {
                p_ass->stats.stream_stats[num].no_datachunk_sent = 0;
                p_ass->stats.stream_stats[num].no_datachunk_resent = 0;
                p_ass->stats.stream_stats[num].no_datachunk_send_fail = 0;
                p_ass->stats.stream_stats[num].no_datachunk_recd = 0;
                p_ass->stats.stream_stats[num].no_datachunk_recd_fail = 0;
            }

            /* SCTP REL 6.2.0  change end */

            for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                    addr_id++ )
            {
                p_ass->stats.dest_stats[addr_id].no_datagrams_sent = 0;
                p_ass->stats.dest_stats[addr_id].no_datagrams_unacked = 0;
                p_ass->stats.dest_stats[addr_id].no_datagrams_recd = 0;
                p_ass->stats.dest_stats[addr_id].no_bytes_sent = 0;
                p_ass->stats.dest_stats[addr_id].no_bytes_recvd = 0;

            }


        }
    }

    else if( p_ass != SCTP_NULL )
    {
        p_ass->stats.assoc_id = p_ass->p_ep->ep_id;

        __GETSYSTIME( &( cur_time ) );
        p_ass->stats.start_time = cur_time;

        p_ass->stats.no_chunks_retrans = 0;
        p_ass->stats.no_T1_expired = 0;
        p_ass->stats.no_T2_expired = 0;
        p_ass->stats.no_dest_address = p_ass->transmit.num_remote_address;
        /* SCTP REL 6.2.0  change start */
        p_ass->stats.no_in_streams = p_ass->receive.num_in_streams;
        p_ass->stats.no_out_streams = p_ass->transmit.num_out_streams;
        /* SCTP REL 6.2.0  change end */

        /*Buffer Stats*/
        p_ass->stats.buffer_stat.tx_med_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.tx_max_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.tx_max_utilized_instead_med = 0;
        p_ass->stats.buffer_stat.rx_med_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.rx_max_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.rx_max_utilized_instead_med = 0;

        /* SCTP REL 6.2.0  change start */
        for( num = 0; num < SCTP_MAX_NUMSTREAM ; num++ )
        {
            p_ass->stats.stream_stats[num].no_datachunk_sent = 0;
            p_ass->stats.stream_stats[num].no_datachunk_resent = 0;
            p_ass->stats.stream_stats[num].no_datachunk_send_fail = 0;
            p_ass->stats.stream_stats[num].no_datachunk_recd = 0;
            p_ass->stats.stream_stats[num].no_datachunk_recd_fail = 0;
        }

        /* SCTP REL 6.2.0  change end */

        /* For destination Address */
        for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                addr_id++ )
        {
            p_ass->stats.dest_stats[addr_id].dest_address
                = p_ass->transmit.remote[addr_id].that_address;
            p_ass->stats.dest_stats[addr_id].no_datagrams_sent = 0;
            p_ass->stats.dest_stats[addr_id].no_datagrams_unacked = 0;
            p_ass->stats.dest_stats[addr_id].no_datagrams_recd = 0;
            p_ass->stats.dest_stats[addr_id].no_bytes_sent = 0;
            p_ass->stats.dest_stats[addr_id].no_bytes_recvd = 0;

        }

        /* For Local Address */
        for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                addr_id++ )
        {
            p_ass->stats.local_addr_stats[addr_id].local_address
                = p_ass->p_ep->this_address[addr_id];

        }

    }

    return;

}

/***************************************************************************
 *    FUNCTION :    sctp_reinit_assoc_stats
 *
 *    DESCRIPTION:
 *    This function re-initializes the statistcs count for all parameter
 *    of association statistics.
 *    This function was called from the sctpfsm_assoc_CookE_MsgCookieAck when
 *    the socket state comes to Established state.
 *    In this function
 *    - the Number of T1-Timer expiry counts(no_T1_expired) are NOT reset to 0.
 *    - the start time of the association is taken
 *    - Number of addresses is updated.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_reinit_assoc_stats(
    sctp_tcb_st    *p_ass,
    sctp_Boolean_t flag )
{
    sctp_U32bit    count, addr_id, num;
    sctp_timestamp cur_time;

    if( flag == SCTP_TRUE )
    {
        for( count = 0; count < sctp_db_main.max_assoc; count++ )
        {
            p_ass = &( sctp_db_main.assoc_table[count] );
            p_ass->stats.no_chunks_retrans = 0;
            p_ass->stats.no_T1_expired = 0;
            p_ass->stats.no_T2_expired = 0;
            p_ass->stats.no_dest_address = 0;
            /* SCTP REL 6.2.0 changes start */
            p_ass->stats.no_in_streams = 0;
            p_ass->stats.no_out_streams = 0;
            /* SCTP REL 6.2.0 changes end */
            /*Buffer Stats*/
            p_ass->stats.buffer_stat.tx_med_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.tx_max_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.tx_max_utilized_instead_med = 0;

            p_ass->stats.buffer_stat.rx_med_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.rx_max_utilized_instead_min = 0;
            p_ass->stats.buffer_stat.rx_max_utilized_instead_med = 0;

            /* SCTP REL 6.2.0  change start */
            for( num = 0; num < SCTP_MAX_NUMSTREAM ; num++ )
            {
                p_ass->stats.stream_stats[num].no_datachunk_sent = 0;
                p_ass->stats.stream_stats[num].no_datachunk_resent = 0;
                p_ass->stats.stream_stats[num].no_datachunk_send_fail = 0;
                p_ass->stats.stream_stats[num].no_datachunk_recd = 0;
                p_ass->stats.stream_stats[num].no_datachunk_recd_fail = 0;
            }

            /* SCTP REL 6.2.0  change end */

            for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                    addr_id++ )
            {
                p_ass->stats.dest_stats[addr_id].no_datagrams_sent = 0;
                p_ass->stats.dest_stats[addr_id].no_datagrams_unacked = 0;
                p_ass->stats.dest_stats[addr_id].no_datagrams_recd = 0;
                p_ass->stats.dest_stats[addr_id].no_bytes_sent = 0;
                p_ass->stats.dest_stats[addr_id].no_bytes_recvd = 0;
                p_ass->stats.dest_stats[addr_id].dest_addr_start_time.tv_sec = 0;
                p_ass->stats.dest_stats[addr_id].dest_addr_start_time.tv_usec = 0;
            }

            /* For Local Address */
            for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                    addr_id++ )
            {
                p_ass->stats.local_addr_stats[addr_id].local_addr_start_time.tv_sec = 0;
                p_ass->stats.local_addr_stats[addr_id].local_addr_start_time.tv_usec = 0;
            }

        }
    }

    else if( p_ass != SCTP_NULL )
    {
        p_ass->stats.assoc_id = p_ass->p_ep->ep_id;
        __GETSYSTIME( &( cur_time ) );
        p_ass->stats.start_time = cur_time;

        p_ass->stats.no_chunks_retrans = 0;

        p_ass->stats.no_T2_expired = 0;
        p_ass->stats.no_dest_address = p_ass->transmit.num_remote_address;
        /* SCTP REL 6.2.0  change start */
        p_ass->stats.no_in_streams = p_ass->receive.num_in_streams;
        p_ass->stats.no_out_streams = p_ass->transmit.num_out_streams;
        /* SCTP REL 6.2.0  change end */

        /*Buffer Stats*/
        p_ass->stats.buffer_stat.tx_med_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.tx_max_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.tx_max_utilized_instead_med = 0;

        p_ass->stats.buffer_stat.rx_med_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.rx_max_utilized_instead_min = 0;
        p_ass->stats.buffer_stat.rx_max_utilized_instead_med = 0;

        /* SCTP REL 6.2.0  change start */
        for( num = 0; num < SCTP_MAX_NUMSTREAM ; num++ )
        {
            p_ass->stats.stream_stats[num].no_datachunk_sent = 0;
            p_ass->stats.stream_stats[num].no_datachunk_resent = 0;
            p_ass->stats.stream_stats[num].no_datachunk_send_fail = 0;
            p_ass->stats.stream_stats[num].no_datachunk_recd = 0;
            p_ass->stats.stream_stats[num].no_datachunk_recd_fail = 0;
        }

        /* SCTP REL 6.2.0  change end */


        /* For destination Address */
        for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                addr_id++ )
        {
            p_ass->stats.dest_stats[addr_id].dest_address
                = p_ass->transmit.remote[addr_id].that_address;
            p_ass->stats.dest_stats[addr_id].no_datagrams_sent = 0;
            p_ass->stats.dest_stats[addr_id].no_datagrams_unacked = 0;
            p_ass->stats.dest_stats[addr_id].no_datagrams_recd = 0;
            p_ass->stats.dest_stats[addr_id].no_bytes_sent = 0;
            p_ass->stats.dest_stats[addr_id].no_bytes_recvd = 0;
            p_ass->stats.dest_stats[addr_id].dest_addr_start_time
                = cur_time;
        }

        /* For Local Address */
        for( addr_id = 0; addr_id < SCTP_MAX_TRANSPORT_ADDR;
                addr_id++ )
        {
            p_ass->stats.local_addr_stats[addr_id].local_address
                = p_ass->p_ep->this_address[addr_id];
            p_ass->stats.local_addr_stats[addr_id].local_addr_start_time
                = cur_time;
        }

    }

    return;

}


/***************************************************************************
 *    FUNCTION :    sctp_incr_error_stats
 *
 *    DESCRIPTION:
 *    This function checks if the stats    logging    is enabled. If
 *    enabled, it will increment Error statistics count.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_incr_error_stats(
    sctp_U8bit        parameter_index,
    sctp_U32bit       count )
{
    sctp_U32bit    *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_ERROR_STATS] == SCTP_TRUE )
        {
            if( parameter_index <= SCTP_ERRORSTATS_MAX_MEMBERID )
            {
                stats_ptr = ( sctp_U32bit * )&sctp_error_stats;
                *( stats_ptr + parameter_index ) += count;
            }
        }
    }

    return;
}

/***************************************************************************
 *   FUNCTION :   sctp_decr_error_stats
 *
 *   DESCRIPTION:
 *   This function checks if the stats logging is enabled. If
 *   enabled It    will decrement Error statistics    count.
 *   RETURN VALUE:
 *   None
 ***************************************************************************/
void
sctp_decr_error_stats(
    sctp_U8bit          parameter_index,
    sctp_U32bit         count )

{
    sctp_U32bit    *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_ERROR_STATS] == SCTP_TRUE )
        {
            if( parameter_index <= SCTP_ERRORSTATS_MAX_MEMBERID )
            {
                stats_ptr = ( sctp_U32bit * ) &sctp_error_stats;
                *( stats_ptr + parameter_index ) -= count;
            }
        }
    }

    return;
}

/***************************************************************************
 *    FUNCTION :
 *    sctp_incr_prot_int_stats
 *
 *    DESCRIPTION:
 *    This function checks if the stats logging is enabled. If
 *    enabled It will increment Protocol statistics count.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_incr_prot_int_stats(
    sctp_U8bit          parameter_index,
    sctp_U32bit         count )
{
    sctp_U32bit *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_PROT_INTERNAL_STATS] == SCTP_TRUE )
        {
            if( parameter_index <= SCTP_PROT_STATS_MAX_MEMBERID )
            {
                stats_ptr = ( sctp_U32bit * )&sctp_protocol_stats;
                *( stats_ptr + parameter_index ) += count;
            }
        }
    }

    return;
}

/***************************************************************************
 *    FUNCTION :    sctp_decr_prot_int_stats
 *
 *    DESCRIPTION:
 *    This function checks if the stats logging is enabled. If
 *    enabled It will decrement Protocol statistics count.
 *    RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_decr_prot_int_stats(
    sctp_U8bit       parameter_index,
    sctp_U32bit      count )
{
    sctp_U32bit *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_PROT_INTERNAL_STATS] == SCTP_TRUE )
        {
            if( parameter_index <= SCTP_PROT_STATS_MAX_MEMBERID )
            {
                stats_ptr = ( sctp_U32bit * )&sctp_protocol_stats;
                *( stats_ptr + parameter_index ) -= count;
            }
        }
    }

    return;
}

/***************************************************************************
 *    FUNCTION :   sctp_incr_global_traf_stats
 *
 *   DESCRIPTION:
 *   This function checks if the stats logging is enabled. If
 *   enabled It will increment global traffic statistics count.
 *   RETURN VALUE:
 *   None
 ***************************************************************************/
void
sctp_incr_global_traf_stats(
    sctp_U8bit          parameter_index,
    sctp_U32bit         count )
{
    sctp_U32bit *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_GLOBAL_TRAFFIC_STATS] == SCTP_TRUE )
        {
            /* Changes for SPR-21022 Starts here (Klocwork) */
            if( parameter_index <= SCTP_GLOBALSTATS_MAX_MEMBERID )
                /* Changes for SPR-21022 Ends here (Klocwork) */
            {
                stats_ptr = ( sctp_U32bit * )&sctp_global_traffic_stats;
                *( stats_ptr + parameter_index ) += count;
            }
        }
    }

    return;
}

/***************************************************************************
 *  FUNCTION :   sctp_decr_global_traf_stats
 *
 *  DESCRIPTION:
 *   This function checks if the stats logging is enabled. If
 *   enabled It will decrement global traffic statistics count.
 *  RETURN VALUE:
 *   None
 ***************************************************************************/
void
sctp_decr_global_traf_stats(
    sctp_U8bit          parameter_index,
    sctp_U32bit         count )
{

    sctp_U32bit *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_GLOBAL_TRAFFIC_STATS] == SCTP_TRUE )
        {
            /* Changes for SPR-21022 Starts here (Klocwork) */
            if( ( parameter_index <= SCTP_GLOBALSTATS_MAX_MEMBERID ) &&
                    ( parameter_index <=
                      ( sizeof( sctp_global_traffic_stats ) / sizeof( sctp_U32bit ) ) - 1 ) )
                /* Changes for SPR-21022 Ends here (Klocwork) */
            {
                stats_ptr = ( sctp_U32bit * )&sctp_global_traffic_stats;
                *( stats_ptr + parameter_index ) -= count;
            }
        }
    }

    return;
}

/***************************************************************************
 *  FUNCTION :    sctp_incr_assoc_stats
 *
 *  DESCRIPTION:
 *    This function checks if the stats    logging    is enabled. If
 *    enabled It will increment Association statistics count by the count
 *    value passed.
 *  RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_incr_assoc_stats(
    sctp_tcb_st           *p_ass,
    sctp_U32bit           d_addr_id,
    sctp_U8bit            parameter_index,
    sctp_U32bit           count )
{

    /* assumes dest_addr_idx and assoc_id to range from 0 to (max - 1) */
    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_ASSOC_STATS] == SCTP_TRUE )
        {
            if( ( p_ass != SCTP_NULL )    &&
                    ( d_addr_id < SCTP_MAX_TRANSPORT_ADDR ) )
            {
                switch( parameter_index )
                {
                    case SCTPSTATS_ASSOC_DATA_SENT:
                        p_ass->stats.dest_stats[d_addr_id].no_datagrams_sent
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_DATA_UNAC:
                        p_ass->stats.dest_stats[d_addr_id].
                        no_datagrams_unacked += count;
                        break;

                    case SCTPSTATS_ASSOC_DATA_RECV:
                        p_ass->stats.dest_stats[d_addr_id].
                        no_datagrams_recd += count;
                        break;

                    case SCTPSTATS_ASSOC_DATABYTES_SENT:
                        p_ass->stats.dest_stats[d_addr_id].
                        no_bytes_sent += count;
                        break;

                    case SCTPSTATS_ASSOC_DATABYTES_RECV:
                        p_ass->stats.dest_stats[d_addr_id].
                        no_bytes_recvd += count;
                        break;

                    case SCTPSTATS_ASSOC_NO_CHUNKS_RETRANS:
                        p_ass->stats.no_chunks_retrans += count;
                        break;

                    case SCTPSTATS_ASSOC_NO_T1_EXPIRY:
                        p_ass->stats.no_T1_expired += count;
                        break;

                    case SCTPSTATS_ASSOC_NO_T2_EXPIRY:
                        p_ass->stats.no_T2_expired += count;
                        break;

                        /*Buffer Stats*/
                    case SCTPSTATS_ASSOC_T_MED_INSTD_MIN:
                        p_ass->stats.buffer_stat.tx_med_utilized_instead_min
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_T_MAX_INSTD_MIN:
                        p_ass->stats.buffer_stat.tx_max_utilized_instead_min
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_T_MAX_INSTD_MED:
                        p_ass->stats.buffer_stat.tx_max_utilized_instead_med
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_R_MED_INSTD_MIN:
                        p_ass->stats.buffer_stat.rx_med_utilized_instead_min
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_R_MAX_INSTD_MIN:
                        p_ass->stats.buffer_stat.rx_max_utilized_instead_min
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_R_MAX_INSTD_MED:
                        p_ass->stats.buffer_stat.rx_max_utilized_instead_med
                        += count;
                        break;


                    default:
                        break;
                }
            }
        }
    }

    return;
}

/***************************************************************************
 *  FUNCTION :    sctp_decr_assoc_stats
 *
 *  DESCRIPTION:
 *    This function checks if the stats logging is enabled. If
 *    enabled It will decrement Assoc statistics count.
 *  RETURN VALUE:
 *    None
 ***************************************************************************/
void
sctp_decr_assoc_stats(
    sctp_tcb_st          *p_ass,
    sctp_U32bit          dest_addr_idx,
    sctp_U8bit           parameter_index,
    sctp_U32bit          count )
{
    /* assumes dest_addr_idx and assoc_id to range from 0 to (max - 1) */
    sctp_U32bit *stats_ptr;

    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_ASSOC_STATS] == SCTP_TRUE )
        {
            if( ( p_ass != SCTP_NULL ) &&
                    ( dest_addr_idx < SCTP_MAX_TRANSPORT_ADDR ) &&
                    ( parameter_index <= SCTP_ASSOCSTATS_MAX_MEMBERID ) )
            {
                /* point to the first parameter of the    assoc, dest addr */
                stats_ptr = ( sctp_U32bit * )
                            &p_ass->stats.dest_stats[dest_addr_idx].no_datagrams_sent;
                *( stats_ptr +    parameter_index ) -= count;
            }
        }
    }

    return;
}

/***************************************************************************
 *  FUNCTION :   sctp_init_stats_module
 *
 *    DESCRIPTION:
 *   This function inilialises the stats module. Based on number of
 *   maximum association passed    by System Management, It allocates
 *   memory for statistics counters and initialise them.
 *   Stats collections is also enabled.
 *    RETURN VALUE:
 *      None
 ***************************************************************************/
sctp_return_t
sctp_init_stats_module(
    sctp_U32bit           max_num_assoc,
    sctp_error_t          *p_ecode )
{
    sctp_U32bit    sctp_index;
    /*Bug Id 22 fix starts*/
    /*sctp_stats_max_num_assoc = max_num_assoc;*/
    SCTP_UNUSED_VARIABLE( max_num_assoc );
    /*Bug Id 22 fix ends*/

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "SCTP TRACE::STATS Module:In Function sctp_init_stats_module" ) );


    /* initialise association statistics structure */
    sctp_init_assoc_stats( SCTP_NULL, SCTP_TRUE );

    /* initialise protocol internal stats */
    sctp_init_prot_internal_stats();

    /* initialise global traffic stats */
    sctp_init_global_traffic_stats();

    /* initialise error    stats */
    sctp_init_error_stats();

    sctp_stats_state = SCTP_STATS_STATE_ENABLED;

    /* Disable stats for all stats type    */
    for( sctp_index = 0; sctp_index < SCTP_ALL_STATS; sctp_index++ )
    {
        sctp_stats_table[sctp_index] = SCTP_FALSE;
    }

    *p_ecode = E_NOERROR; /* to remove warning */
    return( SCTP_SUCCESS );

}

/* Rel 6.2.0 change start */
/***************************************************************************
 *  FUNCTION :   sctp_incr_assoc_stream_stats
 *
 *    DESCRIPTION:
 *   This function increments the statistics for various streams in
 *   an association.
 *
 *   RETURN VALUE:
 *      None
 ***************************************************************************/
void sctp_incr_assoc_stream_stats(
    sctp_tcb_st           *p_ass,
    sctp_U16bit           stream_id,
    sctp_U8bit            parameter_index,
    sctp_U32bit           count )
{
    /* assumes stream_id and assoc_id to range from 0 to (max - 1) */
    if( sctp_stats_state == SCTP_STATS_STATE_ENABLED )
    {
        if( sctp_stats_table[SCTP_ASSOC_STATS] == SCTP_TRUE )
        {
            if( ( p_ass != SCTP_NULL )    &&
                    ( stream_id < SCTP_MAX_NUMSTREAM ) )
            {
                switch( parameter_index )
                {
                    case SCTPSTATS_ASSOC_STREAM_DATA_SENT:
                        p_ass->stats.stream_stats[stream_id].no_datachunk_sent
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_STREAM_DATA_RESENT:
                        p_ass->stats.stream_stats[stream_id].no_datachunk_resent
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL:
                        p_ass->stats.stream_stats[stream_id].no_datachunk_send_fail
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_STREAM_DATA_RECD:
                        p_ass->stats.stream_stats[stream_id].no_datachunk_recd
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL:
                        p_ass->stats.stream_stats[stream_id].no_datachunk_recd_fail
                        += count;
                        break;

                    case SCTPSTATS_ASSOC_STREAM_DATABYTES_SENT:
                        p_ass->stats.stream_stats[stream_id].no_of_databytes_sent
                        += count;

                    default:
                        break;
                }
            }
        }
    }

    return;
}
/*Rel 6.2.0 change end */
#endif


/* Start SPR 20526 */
/***********************************************************************
 *  FUNCTION :   sctp_api_get_rtt_info
 *
 *  DESCRIPTION:
 *  This API return current value of various association specific parameters.
 *
 *  RETURN VALUE:
 *  SCTP_SUCCESS
 *  SCTP_FAILURE
 ***********************************************************************/
sctp_return_t
sctp_api_get_rtt_info(
    sctp_U32bit                    assoc_id,
    sctp_U32bit                    *p_num_daddrs,
    sctp_sockaddr_list_st          list_of_dest_addrs,
    sctp_rtt_info_list_st          *p_dest_rtt_param_list,
    sctp_sockaddr_st               *p_dpriaddr,
    sctp_Boolean_t                 reset_flag,
    sctp_error_t                   *p_ecode )
{
    sctp_ep_st                *p_ep = SCTP_NULL;
    sctp_tcb_st               *p_ass = SCTP_NULL;
    sctp_U32bit               counter;
    sctp_U32bit               sctp_index;
    /* SPR 20870 Starts */
    sctp_sockaddr_st          *addr_list
        = ( sctp_sockaddr_st * )list_of_dest_addrs;
    sctp_rtt_info_st          *param_list
        = ( sctp_rtt_info_st * )p_dest_rtt_param_list;
    /* SPR 20870 Ends */

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( ( p_num_daddrs  == NULL )
            || ( p_dest_rtt_param_list == NULL )
            || ( p_dpriaddr == NULL ) )
    {
        *p_ecode = E_NORESOURCES;
        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_get_rtt_info " ) );


    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "System Management API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "System Management API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "System Management API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }


    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "System Management API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "System Management API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }


    if( p_num_daddrs )
    {
        *p_num_daddrs = p_ass->transmit.num_remote_address;
    }


    if( addr_list != SCTP_NULL )
    {
        for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
        {
            addr_list[counter] = p_ass->transmit.remote[counter].that_address;
        }
    }

    sctp_index = p_ass->transmit.primary_address_index;

    if( p_dpriaddr )
    {
        *p_dpriaddr = p_ass->transmit.remote[sctp_index].that_address;
    }

    if( param_list != SCTP_NULL )
    {
        for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
        {
            param_list[counter].min_rtt
                = p_ass->transmit.remote[counter].min_rtt;
            param_list[counter].max_rtt
                = p_ass->transmit.remote[counter].max_rtt;
            param_list[counter].last_recorded_rtt
                = p_ass->transmit.remote[counter].last_recorded_rtt;
            param_list[counter].srtt
                = p_ass->transmit.remote[counter].srtt;

            if( reset_flag == SCTP_TRUE )
            {
                p_ass->transmit.remote[counter].min_rtt = 0;
                p_ass->transmit.remote[counter].max_rtt = 0;
                p_ass->transmit.remote[counter].last_recorded_rtt = 0;
            }
        }

    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}
/* Ends SPR 20526 */
