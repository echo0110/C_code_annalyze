/*****************************************************************************
 *    FILE NAME:
 *               sm_api.c
 *
 *    DESCRIPTION:
 *                This file includes all the sctpmib wrappers api's
 *                required to manage the MIB of sctp
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -------------------------------------------------------------
 *
 *    15 MAY 2001  sigtran team    HSS:40600244   .Original Creation
 *                                 HSS:40600245
 *    28 Sep 2001  Yogesh Gahlaut     -            Rel 3.0 Changes
 *    20 May 2002  Yogesh Gahlaut    SPR 4049 4050 4053 fix
 *    08 Sep 2003  Manish Rajpal                   Rel 5.0 Changes
 *    24 May 2010  Rajnesh         SPR 20411       Warning Fixed
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/


#include  <s_cotyps.h>
#include  <s_stats.h>
#include  <s_db.h>
#include  <s_dbtcb.h>
#include  <s_dbutil.h>
#include  <s_mibtyp.h>
#include  <s_mibdef.h>
#include  <s_uisuif.h>
#include  <s_uismty.h>
#include  <s_in.h>
#include  <s_osmem.h>
#include  <s_dbg.h>

/****************************************************************************
 *   FUNCTION : sctpmib_get_statecontrol_stat
 *
 *   DESCRIPTION  :  This function is used to get information regarding
 *     sctp  state parameters (estab,aborts,shutdowns).
 *
 *   RETURNS   :   Success or Failure and error number .
 *
 ****************************************************************************/
sctp_return_t
sctpmib_get_statecontrol_stat(
    sctpmib_state_stat_params_st    *p_params,
    sctp_error_t                    *p_ecode )
{
#ifdef SCTP_STATS_ENABLED
    sctp_stats_et                 stats_type;
    sctp_U32bit                   assoc_id = 0;
    sctp_U8bit                    *stats_data;
    sctp_proto_internal_stats_st  *prot_stats_data;
    sctp_global_traf_stats_st     *global_stats_data;

    stats_type = SCTP_PROT_INTERNAL_STATS;

    /*allocate memory for protocol stats structure*/
    stats_data = ( sctp_U8bit * )__MALLOC( sizeof( sctp_proto_internal_stats_st ) );

    if( sctp_api_get_stats( stats_type, assoc_id, stats_data, SCTP_FALSE, p_ecode )
            == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Error occured and error no is,%d", *p_ecode ) );
        __FREE( stats_data );
        return( SCTP_FAILURE );
    }

    /* type cast to prot_internal structure */
    prot_stats_data = ( sctp_proto_internal_stats_st * )stats_data;

    /* assigning protocol stats*/
    p_params->curr_estab = prot_stats_data->no_endpoints_marked_up;
    __FREE( prot_stats_data );

    stats_type = SCTP_GLOBAL_TRAFFIC_STATS ; /* selects global stats */
    stats_data = ( sctp_U8bit * )__MALLOC( sizeof( sctp_global_traf_stats_st ) );

    /* get the global stats */
    if( sctp_api_get_stats( stats_type, assoc_id, stats_data, SCTP_FALSE, p_ecode )
            == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Error occured and error no is,%d", *p_ecode ) );
        __FREE( stats_data );
        return( SCTP_FAILURE );
    }

    /* type cast to global_traf structure */
    global_stats_data = ( sctp_global_traf_stats_st * )stats_data;

    /* assigning global stats*/
    p_params->active_estab = global_stats_data->no_assoc_initiated;
    p_params->passive_estab = global_stats_data->no_assoc_accepted;
    p_params->aborted =  global_stats_data->no_ungraceful_shutdown_assocs;
    p_params->shutdown = global_stats_data->no_graceful_shutdown_assocs;

    __FREE( stats_data );
#endif
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_params );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    return( SCTP_SUCCESS );
}

/******************************************************************************
 *   FUNCTION NAME: sctpmib_get_common_stat_parameter
 *
 *   DESCRIPTION:This function is to get common statistics related parameters,
 *
 *   RETURNS    :This will return error number(success or failure),error code
 *               in case of any error and a structure that contains all
 *               statistical parameters.
 *
 *****************************************************************************/
sctp_return_t
sctpmib_get_common_stat_parameter(
    sctpmib_other_stat_params_st    *p_params,
    sctp_error_t                    *p_ecode )
{
#ifdef SCTP_STATS_ENABLED
    sctp_stats_et              stat_type =  SCTP_GLOBAL_TRAFFIC_STATS ;
    sctp_U32bit                assoc_id = 1 ;
    sctp_U8bit                 *data;
    sctp_U32bit                ctrl_chunks_sent;
    sctp_U32bit                ctrl_chunks_rec;
    sctp_global_traf_stats_st  *global_ptr;
    sctp_return_t              ret;

    data = ( sctp_U8bit * )__MALLOC( sizeof( sctp_global_traf_stats_st ) );
    ret = sctp_api_get_stats( stat_type, assoc_id, data, SCTP_FALSE, p_ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "there is an error in sctp and that is %d\n", *p_ecode ) );
        __FREE( data );
        return( SCTP_FAILURE );
    }

    global_ptr = ( sctp_global_traf_stats_st * )data;
    /* SPR 4051 */
    ctrl_chunks_sent = global_ptr->no_initack_sent
                       + global_ptr->no_cookie_sent
                       + global_ptr->no_cookieack_sent
                       + global_ptr->no_hb_sent
                       + global_ptr->no_hback_sent
                       + global_ptr->no_abort_sent
                       + global_ptr->no_shut_sent
                       + global_ptr->no_shutack_sent
                       + global_ptr->no_shutcom_sent
                       + global_ptr->no_error_sent
                       + global_ptr->no_ecne_sent
                       + global_ptr->no_cwr_sent
                       + global_ptr->no_init_sent
                       + global_ptr->no_sack_sent
#ifdef SCTP_DYNAMIC_IP_SUPPORT
                       + global_ptr->no_asconf_sent/*rel 4.0*/
                       + global_ptr->no_asconfack_sent/*rel 4.0*/
#endif
                       ;

    ctrl_chunks_rec = global_ptr->no_initack_recv
                      + global_ptr->no_cookie_recv
                      + global_ptr->no_cookieack_recv
                      + global_ptr->no_hb_recv
                      + global_ptr->no_hback_recv
                      + global_ptr->no_abort_recv
                      + global_ptr->no_shut_recv
                      + global_ptr->no_shutack_recv
                      + global_ptr->no_shutcom_recv
                      + global_ptr->no_error_recv
                      + global_ptr->no_ecne_recv
                      + global_ptr->no_cwr_recv
                      + global_ptr->no_init_recv
                      + global_ptr->no_sack_recv
#ifdef SCTP_DYNAMIC_IP_SUPPORT
                      + global_ptr->no_asconf_recv/*rel 4.0*/
                      + global_ptr->no_asconfack_recv/*rel 4.0*/
#endif
                      ;


    p_params->out_of_blue = global_ptr->no_ootb_recv;
    p_params->chk_sum_errs = global_ptr->no_chk_sum_err_recv;
    p_params->ctrl_sent_chunks = ctrl_chunks_sent;
    p_params->data_ord_sent_chunks = global_ptr->no_ord_data_chunks_sent;
    p_params->data_unord_sent_chunks = global_ptr->no_unord_data_chunks_sent;
    p_params->ctrl_recv_chunks = ctrl_chunks_rec;
    p_params->data_ord_recv_chunks = global_ptr->no_ord_data_chunks_recv;
    p_params->data_unord_recv_chunks = global_ptr->no_unord_data_chunks_recv;
    p_params->frag_usr_msg = global_ptr->no_frag_usr_msg;
    p_params->resemble_usr_msg = global_ptr->no_reassemble_usr_msg;
    p_params->sent_packs = global_ptr->no_packets_sent;
    p_params->recv_packs = global_ptr->no_packets_recv;
    p_params->discontinuity_time = global_ptr->discont_time;

    /* SPR_20411_FIX_START -- */
    __FREE( data );
    /* SPR_20411_FIX_END -- */

#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_params );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    return( SCTP_SUCCESS );

}

/*****************************************************************************
 *   FUNCTION NAME: sctpmib_get_config_parameter
 *
 *   DESCRIPTION  :This function is to get config. parameters from sctp.
 *
 *   RETURNS      :This will return pointer structure containing parameters.
 *
 ****************************************************************************/
sctp_return_t
sctpmib_get_config_parameter(
    sctpmib_config_params_st     *p_params,
    sctp_error_t             *p_ecode )
{
#ifdef SCTP_STATS_ENABLED
    p_params->rto_algo = RTO_ALGO_VANJ;
    p_params->rto_max = sctp_db_main.sctp_def_prot_params.rto_max ;
    p_params->rto_min = sctp_db_main.sctp_def_prot_params.rto_min ;
    p_params->rto_initial = sctp_db_main.sctp_def_prot_params.rto_init;
    p_params->max_assoc = sctp_db_main.max_assoc;
    p_params->val_cookie_life
        = sctp_db_main.sctp_def_prot_params.valid_cookie_life;
    p_params->max_init_retr
        = sctp_db_main.sctp_def_prot_params.max_init_retrans;
#else
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_params );
    /*to avoid the warning*/

#endif
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    return( SCTP_SUCCESS );

}


/******************************************************************************
 *      FUNCTION NAME: sctpmib_get_assoc_table_var
 *
 *      DESCRIPTION  : This function is used to get information related to
 *                     sctp  association ( like TCB , STATE , STATUS).
 *
 *      RETURNS  : Function returns success or failure , error no. and
 *                 pointer to structure of data related to an association.
 *****************************************************************************/
sctp_return_t
sctpmib_get_assoc_table_var(
    sctp_U32bit             assoc_id,
    sctp_sockaddr_st        *dest_add,
    sctp_sockaddr_st        *p_local_add,
    sctpmib_assoc_table_st  *p_assoc_tab,
    sctp_error_t            *p_ecode )
{
#ifdef SCTP_STATS_ENABLED
    sctp_U8bit               *stats_data;
    sctp_stats_et            stats_type;
    sctp_return_t            ret;
    sctp_assoc_stats_st      *stat_data;
    sctp_tcb_st              *p_ass = NULL;
    sctp_ep_st               *p_ep;
    sctp_U32bit              prim_add_index;
    sctp_U32bit              index_count;
    sctp_U32bit              daddr_index;
    sctp_U32bit              saddr_index;

    stats_type = SCTP_ASSOC_STATS;

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        *p_ecode = EINV_ASSOCID;

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    /*
     * Get the pointer to the endpoint/socket. Also check that the endpoint has
     * a association within it.
     */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        p_ass = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_ass )
    {
        *p_ecode = EINV_ASSOCID;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* assign parameters to sctpmib structure */
    /* SPR 4053 */
    switch( p_ass->state )
    {
        case SCTPSTATE_CLOSED :
            p_assoc_tab->assoc_state = SM_CLOSED;
            break;

        case SCTPSTATE_COOKIEWAIT :
            p_assoc_tab->assoc_state = SM_COOKIE_WAIT;
            break;

        case SCTPSTATE_COOKIESENT :
            p_assoc_tab->assoc_state = SM_COOKIE_ECHOD;
            break;

        case SCTPSTATE_SHUTDOWNPEND :
            p_assoc_tab->assoc_state = SM_SHUTDOWN_PENDING;
            break;

        case SCTPSTATE_SHUTDOWNRCVD :
            p_assoc_tab->assoc_state = SM_SHUTDOWN_RECEIVED;
            break;

        case SCTPSTATE_SHUTDOWNSENT :
            p_assoc_tab->assoc_state = SM_SHUTDOWN_SENT;
            break;

        case SCTPSTATE_SHUTDOWNACKSENT :
            p_assoc_tab->assoc_state = SM_SHUTDOWN_ACK_SENT;
            break;

        case SCTPSTATE_ESTABLISHED :
            p_assoc_tab->assoc_state = SM_ESTABLISHED;
            break;
    }

    p_assoc_tab->assoc_in_streams  = p_ass->receive.num_in_streams;
    p_assoc_tab->assoc_out_streams = p_ass->transmit.num_out_streams;

    /*allocating memory for sctp_assoc_stats_t*/
    stats_data  = ( sctp_U8bit * )__MALLOC( sizeof( sctp_assoc_stats_st ) );
    ret = sctp_api_get_stats( stats_type, assoc_id, stats_data, SCTP_FALSE, p_ecode );

    /* check for error */
    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Error occured in sctp_api_get_stats error no. is %d", *p_ecode ) );
        __FREE( stats_data );
        SCTP_UNLOCK_EP( assoc_id );
        return( SCTP_FAILURE );
    }

    stat_data = ( sctp_assoc_stats_st * )stats_data; /* type casting */

    /* assign parameters to sctpmib structure */
    p_assoc_tab->assoc_T1_expired = stat_data->no_T1_expired;
    p_assoc_tab->assoc_T2_expired = stat_data->no_T2_expired;
    p_assoc_tab->assoc_Rtx_chunks = stat_data->no_chunks_retrans;
    __FREE( stats_data );

    p_assoc_tab->rem_sctp_port = p_ass->transmit.that_port;
    p_assoc_tab->local_sctp_port = p_ep->this_port;
    prim_add_index = p_ass->transmit.primary_address_index;
    p_assoc_tab->rem_prim_add    =
        p_ass->transmit.remote[prim_add_index].that_address;

    if( p_assoc_tab->rem_prim_add.family == SCTP_AF_INET )
    {
        p_assoc_tab->rem_prim_add_type = IPv4;
    }

    else if( p_assoc_tab->rem_prim_add.family == SCTP_AF_INET6 )
    {
        p_assoc_tab->rem_prim_add_type = IPv6;
    }

    else
    {
        p_assoc_tab->rem_prim_add_type = UNKNOWN;
    }

    /*  CSR 1-3307301 */
    if( SCTP_NULL != dest_add )
    {
        for( index_count = 0; index_count < p_ass->transmit.num_remote_address; index_count++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr(
                        &( p_ass->transmit.remote[index_count].that_address ),
                        dest_add ) )
            {
                /* The Address Match */
                break;
            }
        }

        if( index_count == p_ass->transmit.num_remote_address )
        {
            *p_ecode = EINV_ADDRS;

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid Address " ) );
            SCTP_UNLOCK_EP( assoc_id );
            return ( SCTP_FAILURE );
        }

        daddr_index = index_count;
    }

    else
    {
        /************************
         * if no destination address is specified than take peer primary
        * address as the destination address.
         ************************/
        daddr_index = prim_add_index;
    }

    p_assoc_tab->rem_add    =
        p_ass->transmit.remote[daddr_index].that_address;

    if( p_assoc_tab->rem_add.family == SCTP_AF_INET )
    {
        p_assoc_tab->rem_add_type = IPv4;
    }

    else if( p_assoc_tab->rem_add.family == SCTP_AF_INET6 )
    {
        p_assoc_tab->rem_add_type = IPv6;
    }

    else
    {
        p_assoc_tab->rem_add_type = UNKNOWN;
    }

    if( p_ass->transmit.remote[daddr_index].allow_heartbeat == 1 )
    {
        p_assoc_tab->assoc_rem_add_hb_flag = HB_ACTIVE;
    }

    else
    {
        p_assoc_tab->assoc_rem_add_hb_flag = HB_INACTIVE;
    }

    /* CSR 1-3307301 END */
    p_assoc_tab->heart_beat_timer = p_ass->config.hb_interval;

    p_assoc_tab->assoc_max_retr = p_ass->config.assoc_max_retrans;

    /* CSR 1-3307301 */
    if( SCTP_COMMUNICATION_UP == p_ass->transmit.remote[daddr_index].status )
    {
        p_assoc_tab->assoc_rem_add_status = REM_ACTIVE;
    }

    else
    {
        p_assoc_tab->assoc_rem_add_status = REM_INACTIVE;
    }

    p_assoc_tab->assoc_rem_add_rto = p_ass->transmit.remote[daddr_index].rto;
    p_assoc_tab->assoc_rem_add_max_path_retx =
        p_ass->transmit.remote[daddr_index].path_error_threshold;
    /* CSR 1-3307301 END */

    __MEMCOPY( p_assoc_tab->rem_host_name, p_ass->transmit.rem_hostname,
               SCTP_MAX_HOSTNAME_SIZE );

    p_assoc_tab->assoc_start_time = p_ass->stats.start_time;
    p_assoc_tab->assoc_discontinuity_time = p_ass->stats.discont_time;
    p_assoc_tab-> assoc_prim_process = SCTP_MIB_PRIM_PROC_UNKNOWN; /* rel 4.0 */

    p_assoc_tab->assoc_rem_add_start_time =
        p_ass->stats.dest_stats[daddr_index].dest_addr_start_time;

    if( SCTP_NULL != p_local_add )
    {
        for( index_count = 0; index_count < p_ass->p_ep->naddr; index_count++ )
        {
            if( SCTP_SUCCESS == sctp_cmp_addr(
                        &( p_ass->p_ep->this_address[index_count] ),
                        p_local_add ) )
            {
                /* The Address Match */
                break;
            }
        }

        if( index_count == p_ass->p_ep->naddr )
        {
            *p_ecode = EINV_ADDRS;

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid Address " ) );
            SCTP_UNLOCK_EP( assoc_id );
            return ( SCTP_FAILURE );
        }

        saddr_index = index_count;
    }

    else
    {
        /************************
        * if no destination address is specified than take local primary
        * address as the source address.
        ************************/
        saddr_index = p_ass->configured_local_primary_address_index;
    }

    p_assoc_tab->assoc_localadd_start_time =
        p_ass->stats.local_addr_stats[saddr_index].local_addr_start_time;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );
#else
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( assoc_id );
    SCTP_UNUSED_VARIABLE( p_assoc_tab );
    SCTP_UNUSED_VARIABLE( p_ecode );
    SCTP_UNUSED_VARIABLE( dest_add );
    SCTP_UNUSED_VARIABLE( p_local_add );
    /*to avoid the warning*/

#endif
    /*
        SCTP_UNUSED_VARIABLE(dest_add);
    */
    return( SCTP_SUCCESS );
}


/******************************************************************************
 *      FUNCTION NAME: sctpmib_set_assoc_state
 *
 *      DESCRIPTION  : This function is used to set the assoc state
 *
 *      RETURNS  : Function returns success or failure , error no.
 *
 *****************************************************************************/
sctp_return_t
sctpmib_set_assoc_state(
    sctp_U32bit             assoc_id,
    sctpmib_assoc_state_et  assoc_state,
    sctp_error_t            *p_ecode )
{
    /* this api is currently not supported */
    /* this api may not be needed at all also. */
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( assoc_id );
    SCTP_UNUSED_VARIABLE( assoc_state );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    return ( SCTP_SUCCESS );
}
