/*******************************************************************************
 *    FILE NAME:
 *    s_red.c
 *
 *    DESCRIPTION:
 *    This file    is part    of redundancy module. Provides APIs to provide
 *    redundancy stop. These APIs are used by message based interface client
 *    message processing module.
 *
 *
 *
 *    DATE  NAME     REFERENCE     REASON    *
 * ----------------------------------------------------- *
 *
 * 3 April 2001        Vakul Garg     -     .Original Creation *
 *    14Jan02 hbhondwe SCTP_IPC_TCP Changes for tcp ipc
 *  07Oct 2003      Manish Rajpal
 *  15Sep 2008      Raza Mohamed    Redundancy Updates for Rel 6.2
 *  25Jul 2010      Rajnesh      SPR Fix20540      Appid updated in Red
 *
 *  Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/

#include <s_red.h>
#include <s_in.h>
#include <s_dbtcb.h>
#include <s_esdef.h>
#include <s_cobuf.h>
#include <s_ostmr.h>
#include <s_dbbuf.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_stats.h>
#include <s_dbtcb.h>
#include <s_mhmac.h>
#include <s_fsmmtu.h>
#include <s_osmem.h>
#include <s_es.h>


#ifdef SCTP_UI_MESSAGEBASED
extern sctp_client_info_st  sctp_client_list[];
#ifdef SCTP_RED_SUPPORT
extern sctp_U32bit g_sctp_rm_dest_id;
#endif
#endif


#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

/* This global variable determines whether stack is ACTIVE or STANDBY */
sctp_stack_red_state_et sctp_stack_red_state = SCTP_STACK_RED_STATE_STANDBY;

/* This global variable determines whether stack is START or STOP */
sctp_stack_proc_state_et    sctp_stack_proc_state =    SCTP_PROC_STATE_STOP;

/*
 * This variable keeps track of the number of assocs to update after
 * sctp_api_set_stack_db()
 */
sctp_U32bit         num_sockets_to_update;

/*  Forward Declarations */

sctp_U8bit *
sctp_get_assoc_db(
    sctp_U8bit     *db_buffer,
    sctp_tcb_st    *assoc );

sctp_U8bit *
sctp_set_assoc_db(
    sctp_U8bit     *assoc_data,
    sctp_tcb_st    *assoc,
    sctp_error_t   *errorno );

/***************************************************************************
 *  FUNCTION :   sctp_stack_set_red_state
 *
 *  DESCRIPTION:
 *    This function sets the redundancy    state (ACTIVE or STANDBY) of the
 *    stack. The redundancy states can be set when stack is stopped.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_api_set_red_state(
    sctp_stack_red_state_et    stack_state,
    sctp_error_t               *errorno )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_stack_set_red_state" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_FAILURE;
    }

    if( sctp_stack_proc_state == SCTP_PROC_STATE_START ||
            ( sctp_stack_state  == SCTP_STATE_UNINTIALISED ) )
    {
        /* return error if stack is START or uninitialized */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    sctp_stack_red_state = stack_state;
    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :    sctp_stack_set_proc_state
 *
 *    DESCRIPTION:
 *    This function sets the processing  state (START or STOP) of the
 *    stack.
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_api_set_proc_state(
    sctp_stack_proc_state_et stack_state,
    sctp_U32bit              seed,
    sctp_error_t             *errorno )
{
    sctp_U32bit rem_time;
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_stack_set_proc_state" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_FAILURE;
    }

    if( sctp_stack_state == SCTP_STATE_UNINTIALISED )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    if( stack_state == SCTP_PROC_STATE_START )
    {
        /*Check if all the association databases have been updated*/
        if( num_sockets_to_update != 0 )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            *errorno = E_REQ_IN_WRONG_STATE;
            return SCTP_FAILURE;
        }


        /* If the stack is being started, initialize the seed.
         * This ensures that the new random numbers generated by the all stacks
         * are same
         */

        __INITSEED( seed );

        /* restart the mac key timer */
        /*Stop previously running mac timer*/
        sctp_stop_timer( &mac_timer_id, &rem_time, errorno );

        /*Start the mac timer again*/
        sctp_start_timer( SCTP_NULL,
                          SCTPEVENT_TIMERMAC, SCTP_MAC_KEY_UPDATE_TIME,
                          0, &mac_timer_id, errorno );

    }

    sctp_stack_proc_state = stack_state;
    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :   sctp_api_get_stack_red_db
 *
 *    DESCRIPTION:
 *    This function obtains the database of SCTP stack.
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_api_get_stack_red_db(
    sctp_red_stack_db_st   *db,
    sctp_error_t           *errorno )
{
    sctp_U32bit        count, sctp_index, event, addr_index, rem_time = 0;
    sctp_pmtu_node_st  *pmtu_node;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_api_get_stack_red_db" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_FAILURE;
    }

    if( db == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *errorno = EINV_PARAMS;
        return SCTP_FAILURE;
    }

    if( sctp_stack_state == SCTP_STATE_UNINTIALISED ||
            ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) )
    {
        /* If stack is START or uninitialized, return failure */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    db->max_appl = sctp_db_main.max_appl;
    db->num_active_appl = sctp_db_main.num_active_appl;
    db->max_ep = sctp_db_main.max_ep;
    db->num_active_ep = sctp_db_main.num_active_ep;
    db->last_allocated_ep = sctp_db_main.last_allocated_ep;
    db->max_assoc = sctp_db_main.max_assoc;
    db->min_tx_buffers = sctp_db_main.min_tx_buffers;
    db->med_tx_buffers = sctp_db_main.med_tx_buffers;
    db->max_tx_buffers = sctp_db_main.max_tx_buffers;
    db->min_rx_buffers = sctp_db_main.min_rx_buffers;
    db->med_rx_buffers = sctp_db_main.med_rx_buffers;
    db->max_rx_buffers = sctp_db_main.max_rx_buffers;
    db->min_reserve_buffers = sctp_db_main.min_reserve_buffers;
    db->med_reserve_buffers = sctp_db_main.med_reserve_buffers;
    db->max_reserve_buffers = sctp_db_main.max_reserve_buffers;
    db->max_streams_per_assoc = sctp_db_main.max_streams_per_assoc;
    db->num_local_addrs = sctp_db_main.num_local_addrs;
    db->init_in_streams = sctp_db_main.init_in_streams;
    db->init_out_streams = sctp_db_main.init_out_streams;
    /* Copy whole of the structure for default protocol parameters */
    db->sctp_def_prot_params = sctp_db_main.sctp_def_prot_params;
    db->trx_cong_reg_def = sctp_db_main.trx_cong_reg_def;
    db->rcv_cong_reg_def = sctp_db_main.rcv_cong_reg_def;
    db->sctp_stack_init_parameters = sctp_capability;
    db->standard_udp_port = sctp_eshell_info.udp_port;

    /******************Update list of open sockets************************/
    for( count = 0, sctp_index = 0; count < sctp_db_main.max_ep; count++ )
    {
        if( SCTP_SOCK_IS_IN_USE( sctp_db_main.ep_table[count].flags ) )
        {
            db->active_ep_list[sctp_index] = count;
            sctp_index++;
        }
    }

    /*******************Update pmtu related data**************************/
    sctp_timer_get_event_rem_time( pmtu_timer_id, &event, &addr_index,
                                   &rem_time );

    db->pmtu_timer_rem_time = rem_time;

    /*Update pmtu data*/
    count = 0;

    for( pmtu_node = ( sctp_pmtu_node_st * )lstFirst( &mtu_activelist );
            pmtu_node != NULL;
            pmtu_node = ( sctp_pmtu_node_st * )lstNext( ( NODE * )pmtu_node ) )
    {
        db->pmtu_database[count].time_stamp = pmtu_node->time_stamp;
        db->pmtu_database[count].addr = pmtu_node->addr;
        db->pmtu_database[count].mtu_size = pmtu_node->mtu_size;

        count++;

    }

    /*Update number of pmtu nodes*/
    db->num_active_pmtu_nodes = lstCount( &mtu_activelist );

    /*******************Update list of active applications****************/
    for( count = 0; count < sctp_db_main.max_appl; count++ )
    {
        db->active_appl_list[count].open = sctp_db_main.appl_list[count].open;
        db->active_appl_list[count].su_id = sctp_db_main.appl_list[count].su_id;
        /* SPR Fix 20540 Start --- */
        db->active_appl_list[count].appl_id = sctp_client_list[sctp_db_main.appl_list[count].su_id].application_id;
        /* SPR Fix 20540 End ---- */
    }

    /*******************Update list of transport addresses*****************/
    for( count = 0; count < sctp_db_main.num_local_addrs; count++ )
    {
        db->addr_list[count] = sctp_db_main.addr_list[count];
    }

    /*******************Error module data**********************************/

#ifdef SCTP_ERROR_ENABLED
    db->sctp_error_state = sctp_error_state;

    for( count = SCTP_MIN_ERROR_FLAG; count <= SCTP_MAX_ERROR_FLAG; count++ )
    {
        db->sctp_error_table[count] = sctp_error_table[count];
    }

#endif

    /******************Statistics module data*****************************/
#ifdef SCTP_STATS_ENABLED
    db->sctp_stats_state = sctp_stats_state;
    db->sctp_protocol_stats = sctp_protocol_stats;
    db->sctp_global_traffic_stats = sctp_global_traffic_stats;
    db->sctp_error_stats = sctp_error_stats;

    for( count = 0; count < SCTP_ALL_STATS; count++ )
    {
        db->sctp_stats_table[count] = sctp_stats_table[count];
    }

    /*Association database would be updated in assoc_red_db*/
#endif

#ifdef SCTP_DEBUG_ENABLED

    db->sctp_gbl_trc = sctp_gbl_trace;

#endif

    /*Copying old mac keys*/
    sctp_buffer_set_Buffer( &( db->old_mac_key[0] ), SCTP_KEYSIZE,
                            &sctp_mac_info.old_mac_key[0] );


    /*Copying current mac keys*/
    sctp_buffer_set_Buffer( &( db->current_mac_key[0] ), SCTP_KEYSIZE,
                            &sctp_mac_info.current_mac_key[0] );

    return SCTP_SUCCESS;

}
/***************************************************************************
 *    FUNCTION :    sctp_api_set_stack_red_db
 *
 *    DESCRIPTION:
 *    This function sets the database of SCTP stack by the values passed
 *    in db as parameter.
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_api_set_stack_red_db(
    sctp_red_stack_db_st  *db,
    sctp_error_t          *errorno )
{
    sctp_U32bit           sctp_index, sock_id, rem_time;
    sctp_init_options_st  init_options;
    sctp_return_t         ret;
    sctp_pmtu_node_st     *pmtu_node;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_api_set_stack_red_db" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_FAILURE;
    }

    if( db == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        return SCTP_FAILURE;
    }

    if( sctp_stack_proc_state != SCTP_PROC_STATE_STOP )
    {
        /* If stack is START return failure    */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    if( sctp_stack_state != SCTP_STATE_UNINTIALISED )
    {
        /* If stack is already initialized, return failure */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    init_options.max_appl = db->max_appl;
    init_options.max_endpoint = db->max_ep;
    init_options.max_tcb = db->max_assoc;
    init_options.MaxPerAssoc.streams = db->max_streams_per_assoc;
    init_options.MaxPerAssoc.min_tx_buffers = db->min_tx_buffers;
    init_options.MaxPerAssoc.med_tx_buffers = db->med_tx_buffers;
    init_options.MaxPerAssoc.max_tx_buffers = db->max_tx_buffers;
    init_options.MaxPerAssoc.min_rx_buffers = db->min_rx_buffers;
    init_options.MaxPerAssoc.med_rx_buffers = db->med_rx_buffers;
    init_options.MaxPerAssoc.max_rx_buffers = db->max_rx_buffers;

    /* Rel 6.2 */
    init_options.min_reserve_buffers = db->min_reserve_buffers;
    init_options.med_reserve_buffers = db->min_reserve_buffers;
    init_options.max_reserve_buffers = db->min_reserve_buffers;
    init_options.checksum = db->sctp_def_prot_params.checksum_func;
    /* Rel 6.2 Ends Here. */

    init_options.no_of_local_addrs = db->num_local_addrs;

    for( sctp_index = 0; sctp_index < db->num_local_addrs; sctp_index++ )
    {
        init_options.list_of_local_addrs[sctp_index] = db->addr_list[sctp_index];
    }

    init_options.standard_udp_port = db->standard_udp_port;
    init_options.capability = db->sctp_stack_init_parameters;
    /*
     * No need to pass hostname as it has  been resolved and stored in
     * addr list in active stack
     */
    init_options.hostname = SCTP_NULL;
    ret =  sctp_api_init_stack( &init_options, errorno );

    if( ret == SCTP_FAILURE )
    {
        return SCTP_FAILURE;
    }

    sctp_db_main.sctp_def_prot_params = db->sctp_def_prot_params;
    sctp_db_main.trx_cong_reg_def = db->trx_cong_reg_def;
    sctp_db_main.rcv_cong_reg_def = db->rcv_cong_reg_def;
    sctp_db_main.num_active_appl = db->num_active_appl;
    sctp_db_main.num_active_ep = db->num_active_ep;
    sctp_db_main.last_allocated_ep = db->last_allocated_ep;

    /*Update the number of assoc databases to update*/
    num_sockets_to_update = sctp_db_main.num_active_ep;

    /* update list of applications */
    for( sctp_index = 0; sctp_index < sctp_db_main.max_appl; sctp_index++ )
    {
        sctp_db_main.appl_list[sctp_index].open = db->active_appl_list[sctp_index].open;
        sctp_db_main.appl_list[sctp_index].su_id = db->active_appl_list[sctp_index].su_id;

        if( sctp_db_main.appl_list[sctp_index].open )
        {
            sctp_client_list[sctp_db_main.appl_list[sctp_index].su_id].stack_id =
                sctp_db_main.appl_list[sctp_index].su_id;

            if( SIGTRAN_FAILURE ==
                    sig_set_in_map( SIG_SCTP_STACK, sctp_db_main.appl_list[sctp_index].su_id,
                                    g_sctp_rm_dest_id,  errorno ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nCreating map between src id = [%u], dest id = [%u]",
                              sctp_db_main.appl_list[sctp_index].su_id, g_sctp_rm_dest_id ) );
                return SCTP_FAILURE;
            }

            /* SPR Fix 20540 Start ---- */
            sctp_client_list[sctp_db_main.appl_list[sctp_index].su_id].application_id = db->active_appl_list[sctp_index].appl_id;
            sctp_client_list[sctp_db_main.appl_list[sctp_index].su_id].dest_id =  g_sctp_rm_dest_id;
            /* SPR Fix 20540 End ---- */
        }
    }

    /* update list of sockets created */
    for( sctp_index = 0; sctp_index < sctp_db_main.num_active_ep; sctp_index++ )
    {
        sock_id = db->active_ep_list[sctp_index];
        SCTP_SOCK_SET_IN_USE( sctp_db_main.ep_table[sock_id].flags, SCTP_TRUE );
    }

    /*Update pmtu data*/
    for( sctp_index = 0; sctp_index < db->num_active_pmtu_nodes; sctp_index++ )
    {
        pmtu_node = ( sctp_pmtu_node_st * ) lstGet( &mtu_freelist );

        pmtu_node->time_stamp = db->pmtu_database[sctp_index].time_stamp;
        pmtu_node->addr = db->pmtu_database[sctp_index].addr;
        pmtu_node->mtu_size = db->pmtu_database[sctp_index].mtu_size;

        lstAdd( &mtu_activelist, ( NODE * )pmtu_node );
    }

    /*Stop any pmtu timer running from the time of initialization*/
    sctp_stop_timer( &pmtu_timer_id, &rem_time, errorno );

    /*Start pmtu timer for remaining duration*/
    sctp_start_timer( SCTP_NULL,
                      SCTPEVENT_TIMERPMU_DB_UPDATE,
                      db->pmtu_timer_rem_time,
                      0, &pmtu_timer_id, errorno );

    /*Update error module data*/
#ifdef SCTP_ERROR_ENABLED
    sctp_error_state = db->sctp_error_state;

    for( sctp_index = SCTP_MIN_ERROR_FLAG; sctp_index <= SCTP_MAX_ERROR_FLAG; sctp_index++ )
    {
        sctp_error_table[sctp_index] = db->sctp_error_table[sctp_index];
    }

#endif

#ifdef SCTP_STATS_ENABLED
    sctp_stats_state = db->sctp_stats_state;
    sctp_protocol_stats = db->sctp_protocol_stats;
    sctp_global_traffic_stats = db->sctp_global_traffic_stats;
    sctp_error_stats = db->sctp_error_stats;

    for( sctp_index = 0; sctp_index < SCTP_ALL_STATS; sctp_index++ )
    {
        sctp_stats_table[sctp_index] = db->sctp_stats_table[sctp_index];
    }

#endif

#ifdef SCTP_DEBUG_ENABLED

    sctp_gbl_trace = db->sctp_gbl_trc;

#endif

    /*Copying old mac keys*/
    sctp_buffer_get_Buffer( &db->old_mac_key[0],
                            SCTP_KEYSIZE, &sctp_mac_info.old_mac_key[0] );

    /*Copying current mac keys*/
    sctp_buffer_get_Buffer( &db->current_mac_key[0],
                            SCTP_KEYSIZE, &sctp_mac_info.current_mac_key[0] );

    return SCTP_SUCCESS;
}

/***************************************************************************
 *  FUNCTION :    sctp_get_assoc_db
 *
 *  DESCRIPTION:
 *    This function gets the database of association from stack.
 *
 *  RETURN VALUE:
 *    sctp_U8bit * (pointer to the calculated assoc db)
 *
 *************************************************************************/
sctp_U8bit *
sctp_get_assoc_db(
    sctp_U8bit     *db_buffer,
    sctp_tcb_st    *assoc )
{
    sctp_U32bit          count, buf_num, timer_num, buf_count, time_rem, event;
    sctp_U32bit          buffer_size, remain_time;
    sctp_time_t          time_diff;
    sctp_red_assoc_db_st *assoc_db;
    sctp_sackfrag_st     *pnode;
    sctp_U32bit          first, last, addr_index;
    sctp_U8bit           *new_offset;
    sctpbuffer_st        *buffer;
    sctp_time_t          curr_time;

    assoc_db = ( sctp_red_assoc_db_st * )db_buffer;

    assoc_db->assoc_id       = assoc->assoc_id;
    /* Rel 6.2 */
    assoc_db->open          = assoc->open;
    /* Rel 6.2 Ends Here. */
    assoc_db->state          = assoc->state;

    for( count = 0; count < SCTP_ASSOCTIMER_NUM; count++ )
    {
        remain_time = 0;
        addr_index = 0;
        event = 0;

        sctp_timer_get_event_rem_time( assoc->assoc_timer_id[count],
                                       &event, &addr_index, &remain_time );

        assoc_db->remain_time[count] = remain_time;
        assoc_db->addr_index[count] = addr_index;
        assoc_db->timer_event[count] = event;
    }

    assoc_db->config           = assoc->config;

    assoc_db->trx_cong_reg_def = assoc->trx_cong_reg_def;
    assoc_db->rcv_cong_reg_def = assoc->rcv_cong_reg_def;

    assoc_db->restart_option   = assoc->restart_option;

    assoc_db->partial_data     = assoc->partial_data;

    assoc_db->init_retry          = assoc->init_retry;
    assoc_db->init_retry_per_path = assoc->init_retry_per_path;
    assoc_db->cookie_retry        = assoc->cookie_retry;
    assoc_db->shutdown_retry      = assoc->shutdown_retry;
    assoc_db->shutdown_ack_retry  = assoc->shutdown_ack_retry;
    assoc_db->error_count         = assoc->error_count;

    assoc_db->configured_local_primary_address_index =
        assoc->configured_local_primary_address_index;

    assoc_db->local_tag  = assoc->local_tag;
    assoc_db->remote_tag = assoc->remote_tag;
    /* Rel 6.2 */
    assoc_db->local_virtual_tie_tag = assoc->local_virtual_tie_tag;
    assoc_db->peer_virtual_tie_tag  = assoc->peer_virtual_tie_tag;
    /* Rel 6.2 Ends Here. */

    assoc_db->total_sent     = assoc->total_sent;

    assoc_db->ecn         = assoc->ecn;
    assoc_db->cookie_size = assoc->cookie_size;

    /* Rel 6.2 */
    assoc_db->zero_win_prob_on = assoc->zero_win_prob_on;
    assoc_db->conn_accepted    = assoc->conn_accepted;
    /* Rel 6.2 Ends Here. */

    /*Filling transmit structure*/
    assoc_db->transmit.congestion_event_sent = assoc->transmit.congestion_event_sent;
    assoc_db->transmit.tsn                   = assoc->transmit.tsn;
    assoc_db->transmit.rwndthere             = assoc->transmit.rwndthere;
    assoc_db->transmit.total_outstanding     = assoc->transmit.total_outstanding;
    assoc_db->transmit.lowest_path_mtu       = assoc->transmit.lowest_path_mtu;
    assoc_db->transmit.num_out_streams       = assoc->transmit.num_out_streams;

    for( count = 0; count < SCTP_MAX_NUMSTREAM; count++ )
    {
        assoc_db->transmit.ssn[count] = assoc->transmit.ssn[count];
    }

    assoc_db->transmit.src_addr_index     = assoc->transmit.src_addr_index;
    assoc_db->transmit.that_port          = assoc->transmit.that_port;
    assoc_db->transmit.num_remote_address = assoc->transmit.num_remote_address;
    assoc_db->transmit.configured_primary_path_index =
        assoc->transmit.configured_primary_path_index;
    assoc_db->transmit.primary_address_index =
        assoc->transmit.primary_address_index;

    assoc_db->transmit.num_tx_buf = lstCount( &assoc->transmit.transmit_queue );

    /* Rel 6.2 Update. */
    for( count = 0; count < SCTP_MAX_HOSTNAME_SIZE; count++ )
    {
        assoc_db->transmit.rem_hostname[count] =
            assoc->transmit.rem_hostname[count];
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    assoc_db->transmit.serial_no = assoc->transmit.serial_no;
#endif
    assoc_db->transmit.wnd_size  = assoc->transmit.wnd_size;

    assoc_db->transmit.last_txq_tsn    = assoc->transmit.last_txq_tsn;
    assoc_db->transmit.non_resp_thresh = assoc->transmit.non_resp_thresh;

    assoc_db->transmit.default_sack_dest_index = assoc->transmit.default_sack_dest_index;

    assoc_db->transmit.fast_recovery_exit  = assoc->transmit.fast_recovery_exit;
    assoc_db->transmit.highest_outstanding = assoc->transmit.highest_outstanding;
    assoc_db->transmit.highest_tsn_sent    = assoc->transmit.highest_tsn_sent;
    /* Rel 6.2 Ends Here. */

    /* Filling receive structure */
    assoc_db->receive.first_data_flag       = assoc->receive.first_data_flag;
    assoc_db->receive.congestion_event_sent = assoc->receive.congestion_event_sent;
    assoc_db->receive.state                 = assoc->receive.state;
    assoc_db->receive.cumtsn                = assoc->receive.cumtsn;
    assoc_db->receive.sacknum               = lstCount( &assoc->receive.sacklist );
    assoc_db->receive.overrule_delayack     = assoc->receive.overrule_delayack;

    for( count = 0; count < SCTP_MAXDUPLICATE_TSNS; count++ )
    {
        assoc_db->receive.duplicate_tsn_list[count] =
            assoc->receive.duplicate_tsn_list[count];
    }

    assoc_db->receive.number_duplicate_tsn = assoc->receive.number_duplicate_tsn;

    assoc_db->receive.num_in_streams = assoc->receive.num_in_streams;

    for( count = 0; count < SCTP_MAX_NUMSTREAM; count++ )
    {
        assoc_db->receive.nextssn[count]        = assoc->receive.stream[count].nextssn;
        assoc_db->receive.nextssn_notify[count] =
            assoc->receive.stream[count].nextssn_notify;
        assoc_db->receive.num_rx_buf[count]     =
            lstCount( &assoc->receive.stream[count].receive_queue );
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    assoc_db->receive.serial_no = assoc->receive.serial_no;
#endif

    assoc_db->receive.wnd_size  = assoc->receive.wnd_size;

    /* Put association statistics */
#ifdef SCTP_STATS_ENABLED
    assoc_db->assoc_stats = assoc->stats;
#endif

    /*
     * Calcuate the new offset in the buffer to put sack chunks,
     * transmit & receive buffers
     */
    new_offset = db_buffer + sizeof( sctp_red_assoc_db_st );

    /* putting remote address parameters */
    for( count = 0; count < assoc->transmit.num_remote_address; count++ )
    {
        new_offset = sctp_buffer_pack_sockaddr( new_offset,
                                                &assoc->transmit.remote[count].that_address );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].src_addr_index );

        /* Fill in last used time. we fill the time difference of last used
         * time and current time (seconds and usec)
         */

#ifdef SCTP_CLOCK_THREAD
        sctp_get_local_sys_time( &curr_time );
#else
        __GETSYSTIME( &curr_time );
#endif

        time_diff.tv_sec = curr_time.tv_sec -
                           assoc->transmit.remote[count].last_used_time.tv_sec;

        if( curr_time.tv_usec >=
                assoc->transmit.remote[count].last_used_time.tv_usec )
        {
            time_diff.tv_usec = curr_time.tv_usec -
                                assoc->transmit.remote[count].last_used_time.tv_usec;
        }

        else
        {
            time_diff.tv_usec = 1000000 + curr_time.tv_usec -
                                assoc->transmit.remote[count].last_used_time.tv_usec;
        }

        new_offset = sctp_buffer_set_Ulong( new_offset, time_diff.tv_sec );
        new_offset = sctp_buffer_set_Ulong( new_offset, time_diff.tv_usec );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].state );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].status );

        for( timer_num = 0; timer_num < SCTP_DESTTIMER_NUM; timer_num++ )
        {
            time_rem = 0;
            addr_index = 0;
            event = 0;
            sctp_timer_get_event_rem_time(
                assoc->transmit.remote[count].dest_timer_id[timer_num],
                &event,
                &addr_index,
                &time_rem );

            new_offset = sctp_buffer_set_Ulong( new_offset, time_rem );

            new_offset = sctp_buffer_set_Ulong( new_offset, addr_index );

            new_offset = sctp_buffer_set_Ulong( new_offset, event );

        }

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].allow_heartbeat );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].path_error_count );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].path_error_threshold );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].partial_bytes_acked );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].lowest_tsn );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].num_chunks_await_ack );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].cwnd );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].ssthresh );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].outstanding );
        /* Rel 6.2 Update */
        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].wait_bundle );
        /* Rel 6.2 Update Ends Here. */

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].rto );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].srtt );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].rttvar );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].count );

        new_offset = sctp_buffer_set_Ushort( new_offset,
                                             assoc->transmit.remote[count].path_mtu );

        /* Rel 6.2 Update */
        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].prefered_sack_dest );
        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].unsent_packet_count );
        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].retrans_packet_count );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].is_confirmed );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].nonce[0] );
        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].nonce[1] );

        new_offset = sctp_buffer_set_Ulong( new_offset,
                                            assoc->transmit.remote[count].perf_path_verf );
        /* Rel 6.2 Update Ends Here. */

    }

    /* putting sack fragments */
    for( count = 0; count < assoc_db->receive.sacknum; count++ )
    {
        pnode = ( sctp_sackfrag_st * )lstNth( &assoc->receive.sacklist, count );
        first = pnode->first;
        last =  pnode->last;
        new_offset = sctp_buffer_set_Ulong( new_offset, first );
        new_offset = sctp_buffer_set_Ulong( new_offset, last );
    }

    /*Putting transmit buffers*/
    buffer = ( sctpbuffer_st * )lstFirst( &assoc->transmit.transmit_queue );

    for( count = 0; count < assoc_db->transmit.num_tx_buf; count++ )
    {
        if( buffer == SCTP_NULL )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "SCTP TRACE::RED Module:In Function sctp_get_assoc_db there is some coding error" ) );
            break;
        }

        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->id );

        buffer_size = buffer->size;
        new_offset = sctp_buffer_set_Ulong( new_offset, buffer_size );

        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->ppi );
        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->tsn );

        new_offset = sctp_buffer_set_Ushort( new_offset, buffer->stream );
        new_offset = sctp_buffer_set_Ushort( new_offset, buffer->ssn );

        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->unordered );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->begin );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->end );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->read_flag );

        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->lifetime );
        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->context );

        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->dest_addr_index );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->sent );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->missed );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->bundle );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->retrans_flag );
        new_offset = sctp_buffer_set_Uchar( new_offset, buffer->ack_flag );
        new_offset = sctp_buffer_set_Buffer( new_offset, buffer->size,
                                             buffer->buffer );

        /* Rel 6.2 Update */
        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->ref_count );
        new_offset = sctp_buffer_set_Ulong( new_offset, buffer->buffer_read_index );
        /* Rel 6.2 Update Ends Here. */

        buffer = ( sctpbuffer_st * )lstNext( ( NODE * )buffer );
    }

    /* Putting receive buffers of each stream */
    for( count = 0; count < assoc->receive.num_in_streams; count++ )
    {
        buf_num = lstCount( &assoc->receive.stream[count].receive_queue );

        buffer = ( sctpbuffer_st * )
                 lstFirst( &assoc->receive.stream[count].receive_queue );

        /*Put buffers in each stream*/
        for( buf_count = 0; buf_count < buf_num; buf_count++ )
        {
            if( buffer == SCTP_NULL )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                            ( "\nThere is some coding error" ) );
                break;
            }

            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->id );

            buffer_size = buffer->size;
            new_offset = sctp_buffer_set_Ulong( new_offset, buffer_size );

            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->ppi );
            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->tsn );

            new_offset = sctp_buffer_set_Ushort( new_offset, buffer->stream );
            new_offset = sctp_buffer_set_Ushort( new_offset, buffer->ssn );

            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->unordered );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->begin );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->end );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->read_flag );

            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->lifetime );
            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->context );

            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->dest_addr_index );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->sent );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->missed );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->bundle );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->retrans_flag );
            new_offset = sctp_buffer_set_Uchar( new_offset, buffer->ack_flag );
            new_offset = sctp_buffer_set_Buffer( new_offset, buffer->size,
                                                 buffer->buffer );
            /* Rel 6.2 Update */
            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->ref_count );
            new_offset = sctp_buffer_set_Ulong( new_offset, buffer->buffer_read_index );
            /* Rel 6.2 Update Ends Here. */

            buffer = ( sctpbuffer_st * )lstNext( ( NODE * )buffer );
        }
    }

    return new_offset;
}

/***************************************************************************
 *  FUNCTION :    sctp_api_get_assoc_red_db
 *
 *  DESCRIPTION:
 *    This function obtains the database of association from redundancy
 *    stack.
 *
 *  RETURN VALUE:
 *    sctp_U8bit * (pointer to the calculated assoc db)
 *
 *************************************************************************/
sctp_U8bit *
sctp_api_get_assoc_red_db(
    sctp_U32bit    assoc_id,
    sctp_U32bit    *len,
    sctp_error_t   *errorno )
{
    sctp_U32bit             count;
    sctp_red_ep_db_st       *ep_db;
    sctp_ep_st              *endpoint;
    sctp_tcb_st             *assoc;
    sctp_U8bit              *new_offset, *tmp_offset;
    sctp_U8bit               *red_mem;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_api_get_assoc_red_db" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_NULL;
    }

    red_mem = ( sctp_U8bit * )__MESG_MALLOC( SCTP_RDNCY_MESG_POOL, SCTP_MAX_RED_MEM );

    if( red_mem == SCTP_NULL )
    {
        *errorno = E_MALLOC_FAILED;  /* No Memory ...*/
        return( SCTP_NULL );
    }

    if( ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) ||
            ( sctp_stack_state == SCTP_STATE_UNINTIALISED ) )
    {
        /* If stack is START return failure */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;

        __MESG_FREE( red_mem );

        return SCTP_NULL;
    }

    if( len == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        __MESG_FREE( red_mem );

        return SCTP_NULL;
    }

    endpoint = &( sctp_db_main.ep_table[assoc_id] );

    if( SCTP_SOCK_IS_IN_USE( endpoint->flags ) == SCTP_FALSE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        __MESG_FREE( red_mem );

        return SCTP_NULL;
    }

    ep_db = ( sctp_red_ep_db_st * )( red_mem );

    ep_db->ep_id          = assoc_id;
    ep_db->server_ep_id   = endpoint->server_ep_id;

    ep_db->num_created_ep = endpoint->num_created_ep;

    for( count = 0; count < endpoint->num_created_ep && count < SCTP_MAX_LISTEN_BACKLOG; count++ )
    {
        ep_db->created_ep_id[count] = endpoint->created_ep_id[count];
    }

    ep_db->ppi              = endpoint->ppi;

    ep_db->sk_config        = endpoint->sk_config;
    ep_db->trx_cong_reg_def = endpoint->trx_cong_reg_def;
    ep_db->rcv_cong_reg_def = endpoint->rcv_cong_reg_def;

    ep_db->n_assoc         = endpoint->n_assoc;
    ep_db->last_used_assoc = endpoint->last_used_assoc;

    ep_db->this_port = endpoint->this_port;
    ep_db->naddr     = endpoint->naddr;

    for( count = 0; count < endpoint->naddr; count++ )
    {
        ep_db->this_address[count] = endpoint->this_address[count];
    }

    for( count = 0; count < SCTP_MAX_HOSTNAME_SIZE; count++ )
    {
        ep_db->hostname[count] = endpoint->hostname[count];
    }


    ep_db->l_backlog = endpoint->l_backlog;
    ep_db->flags     = endpoint->flags;
    ep_db->trc_flags = endpoint->trc_flags;

    ep_db->appl_id   = endpoint->appl_id;

    ep_db->family    = endpoint->family;
    ep_db->sk_type   = endpoint->sk_type;
    ep_db->so_linger = endpoint->so_linger;

    ep_db->moribund  = endpoint->moribund;

    ep_db->hostname_supported = endpoint->hostname_supported;
    ep_db->create_from_server = endpoint->create_from_server;
    ep_db->cong_flag          = endpoint->cong_flag;

    new_offset = red_mem + sizeof( sctp_red_ep_db_st );

    assoc = sctp_db_get_assoc_from_ep( endpoint );

    for( count = 0; count < endpoint->n_assoc; count++ )
    {
        tmp_offset = new_offset;
        /* add a place holder for the length of one assoc db */
        new_offset = sctp_buffer_set_Ulong( new_offset, 0 );
        new_offset = sctp_get_assoc_db( new_offset, assoc );
        *len = new_offset - tmp_offset;
        sctp_buffer_set_UlongAtOffset( tmp_offset, 0, *len );
        assoc = ( sctp_tcb_st * )lstNext( ( NODE * )assoc );
    }

    *len = ( sctp_U32bit )( new_offset - red_mem );
    return red_mem;
}

/***************************************************************************
 *  FUNCTION :    sctp_set_assoc_db
 *
 *  DESCRIPTION:
 *    This function sets the database of association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_U8bit *
sctp_set_assoc_db(
    sctp_U8bit     *assoc_data,
    sctp_tcb_st    *assoc,
    sctp_error_t   *errorno )
{
    sctp_red_assoc_db_st *assoc_db;
    sctp_time_t          time_diff, curr_time;
    sctp_U8bit           *new_offset;
    sctp_U32bit          count, timer_num, event, time_rem, addr_index,
                         buffer_size, buffer_id, buf_num, buf_count;

    sctp_sackfrag_st     *sackfrag;
    sctpbuffer_st        *buffer;

    assoc_db                  = ( sctp_red_assoc_db_st * )assoc_data;

    assoc->assoc_id           = assoc_db->assoc_id;
    /* Rel 6.2 Update */
    assoc->open               = assoc_db->open;
    /* Rel 6.2 Update Ends Here. */
    assoc->state              = assoc_db->state;

    for( count = 0; count < SCTP_ASSOCTIMER_NUM; count++ )
    {
        /* If time remaining is 0, the timer was not running and we
         * should continue. If it is 1, timer was running and has expired.
         * In this case the timer is started with value 1 ms as given in
         * remaining time so that it expires quickly.
         * If time remaining has some other value, we start the timer
         * for given duration
         */

        if( assoc_db->remain_time[count] == 0 )
        {
            continue;
        }

        else
        {
            /*Otherwise start the timer with remaining time*/
            sctp_start_timer( assoc,
                              assoc_db->timer_event[count],
                              assoc_db->remain_time[count],
                              assoc_db->addr_index[count],
                              &assoc->assoc_timer_id[count], errorno );
        }
    }

    assoc->config           = assoc_db->config;

    assoc->trx_cong_reg_def = assoc_db->trx_cong_reg_def;
    assoc->rcv_cong_reg_def = assoc_db->rcv_cong_reg_def;

    assoc->restart_option   = assoc_db->restart_option;

    assoc->partial_data     = assoc_db->partial_data;

    assoc->init_retry       = assoc_db->init_retry;
    /* Rel 6.2 Update */
    assoc->init_retry_per_path = assoc_db->init_retry_per_path;
    /* Rel 6.2 Update Ends Here. */
    assoc->cookie_retry       = assoc_db->cookie_retry;
    assoc->shutdown_retry     = assoc_db->shutdown_retry;
    assoc->shutdown_ack_retry = assoc_db->shutdown_ack_retry;
    assoc->error_count        = assoc_db->error_count;

    assoc->configured_local_primary_address_index =
        assoc_db->configured_local_primary_address_index;

    assoc->local_tag          = assoc_db->local_tag;
    assoc->remote_tag         = assoc_db->remote_tag;

    /* Rel 6.2 Update */
    assoc->local_virtual_tie_tag = assoc_db->local_virtual_tie_tag;
    assoc->peer_virtual_tie_tag = assoc_db->peer_virtual_tie_tag;
    /* Rel 6.2 Update Ends Here. */

    assoc->total_sent         = assoc_db->total_sent;

    assoc->ecn = assoc_db->ecn;
    assoc->cookie_size = assoc_db->cookie_size;

    /* Rel 6.2 Update */
    assoc->zero_win_prob_on = assoc_db->zero_win_prob_on;
    assoc->conn_accepted    = assoc_db->conn_accepted;
    /* Rel 6.2 Update Ends Here. */

    /*Getting free tx and rx buffers */
    if( sctp_db_get_assoc_freelists( assoc, errorno ) == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::RED Module:Get Assoc Freelist Failed !!!" ) );
        return( SCTP_FAILURE );
    }

    /*Filling transmit structure*/
    assoc->transmit.congestion_event_sent = assoc_db->transmit.congestion_event_sent;
    assoc->transmit.tsn                   = assoc_db->transmit.tsn;
    assoc->transmit.rwndthere             = assoc_db->transmit.rwndthere;
    assoc->transmit.total_outstanding = assoc_db->transmit.total_outstanding;
    assoc->transmit.lowest_path_mtu   = assoc_db->transmit.lowest_path_mtu;
    assoc->transmit.num_out_streams   = assoc_db->transmit.num_out_streams;

    for( count = 0; count < SCTP_MAX_NUMSTREAM; count++ )
    {
        assoc->transmit.ssn[count] = assoc_db->transmit.ssn[count];
    }

    assoc->transmit.src_addr_index     = assoc_db->transmit.src_addr_index;
    assoc->transmit.that_port          = assoc_db->transmit.that_port;
    assoc->transmit.num_remote_address = assoc_db->transmit.num_remote_address;
    assoc->transmit.configured_primary_path_index =
        assoc_db->transmit.configured_primary_path_index;
    assoc->transmit.primary_address_index =
        assoc_db->transmit.primary_address_index;

    /* Rel 6.2 Update. */
    for( count = 0; count < SCTP_MAX_HOSTNAME_SIZE; count++ )
    {
        assoc->transmit.rem_hostname[count] =
            assoc_db->transmit.rem_hostname[count];
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    assoc->transmit.serial_no = assoc_db->transmit.serial_no;
#endif
    assoc->transmit.wnd_size  = assoc_db->transmit.wnd_size;

    assoc->transmit.last_txq_tsn    = assoc_db->transmit.last_txq_tsn;
    assoc->transmit.non_resp_thresh = assoc_db->transmit.non_resp_thresh;

    assoc->transmit.default_sack_dest_index = assoc_db->transmit.default_sack_dest_index;

    assoc->transmit.fast_recovery_exit  = assoc_db->transmit.fast_recovery_exit;
    assoc->transmit.highest_outstanding = assoc_db->transmit.highest_outstanding;
    assoc->transmit.highest_tsn_sent    = assoc_db->transmit.highest_tsn_sent;
    /* Rel 6.2 Ends Here. */

    /*Filling receive structure*/
    ( assoc->receive ).first_data_flag       = assoc_db->receive.first_data_flag;
    ( assoc->receive ).congestion_event_sent = assoc_db->receive.congestion_event_sent;
    ( assoc->receive ).state                 = assoc_db->receive.state;
    ( assoc->receive ).cumtsn                = assoc_db->receive.cumtsn;
    ( assoc->receive ).overrule_delayack     = assoc_db->receive.overrule_delayack;

    for( count = 0; count < SCTP_MAXDUPLICATE_TSNS; count++ )
    {
        ( assoc->receive ).duplicate_tsn_list[count] =
            assoc_db->receive.duplicate_tsn_list[count];
    }

    ( assoc->receive ).number_duplicate_tsn = assoc_db->receive.number_duplicate_tsn;

    ( assoc->receive ).num_in_streams       = assoc_db->receive.num_in_streams;

    for( count = 0; count < SCTP_MAX_NUMSTREAM; count++ )
    {
        ( assoc->receive ).stream[count].nextssn =
            assoc_db->receive.nextssn[count];

        ( assoc->receive ).stream[count].nextssn_notify =
            assoc_db->receive.nextssn_notify[count];
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    ( assoc->receive ).serial_no = assoc_db->receive.serial_no;
#endif

    ( assoc->receive ).wnd_size  = assoc_db->receive.wnd_size;

    /*Put the association statistics*/
#ifdef SCTP_STATS_ENABLED
    assoc->stats = assoc_db->assoc_stats;
#endif

    /*Getting offset of cookie*/
    new_offset = assoc_data + sizeof( sctp_red_assoc_db_st );

    /*Copying remote address parameters*/
    for( count = 0; count < assoc->transmit.num_remote_address; count++ )
    {
        /* get the address from the buffer */
        new_offset = sctp_buffer_unpack_sockaddr( new_offset,
                                                  &( assoc->transmit ).remote[count].that_address );

        /* get the src addr index */
        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].src_addr_index );

        /* Fill in last used time. subtract time_diff from current time */

#ifdef SCTP_CLOCK_THREAD
        sctp_get_local_sys_time( &curr_time );
#else
        __GETSYSTIME( &curr_time );
#endif

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            ( sctp_U32bit * )&time_diff.tv_sec );

        ( assoc->transmit ).remote[count].last_used_time.tv_sec =
            curr_time.tv_sec - time_diff.tv_sec;

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            ( sctp_U32bit * )&time_diff.tv_usec );

        if( curr_time.tv_usec >= time_diff.tv_usec )
        {
            ( assoc->transmit ).remote[count].last_used_time.tv_usec =
                curr_time.tv_usec - time_diff.tv_usec;
        }

        else
        {
            ( assoc->transmit ).remote[count].last_used_time.tv_usec =
                1000000 + curr_time.tv_usec - time_diff.tv_usec;
        }

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].state );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].status );

        for( timer_num = 0; timer_num < SCTP_DESTTIMER_NUM; timer_num++ )
        {
            new_offset = sctp_buffer_get_Ulong( new_offset, &time_rem );
            new_offset = sctp_buffer_get_Ulong( new_offset, &addr_index );
            new_offset = sctp_buffer_get_Ulong( new_offset, &event );

            /* If time remaining is 0, the timer was not running and we
             * should continue. If it is 1, timer was running and has expired.
             * In this case the timer is started with value 1 ms as given in
             * remaining time so that it expires quickly.
             * If time remaining has some other value, we start the timer
             * for given duration
             */

            if( time_rem == 0 )
            {
                continue;
            }

            else
            {
                /* Start the timer even if it is expired. In that case time_rem
                 * will have value of 1. Thus timer expires as soon stack starts
                 * again
                 */

                sctp_start_timer( assoc, event, time_rem, addr_index,
                                  &( assoc->transmit ).remote[count].dest_timer_id[timer_num],
                                  errorno );
            }
        }

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].allow_heartbeat );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].path_error_count );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].path_error_threshold );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].partial_bytes_acked );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].lowest_tsn );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].num_chunks_await_ack );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].cwnd );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].ssthresh );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].outstanding );

        /* Rel 6.2 */
        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].wait_bundle );
        /* Rel 6.2 Ends Here. */

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].rto );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].srtt );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].rttvar );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].count );

        new_offset = sctp_buffer_get_Ushort( new_offset,
                                             &( assoc->transmit ).remote[count].path_mtu );

        /* Rel 6.2 */
        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].prefered_sack_dest );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].unsent_packet_count );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].retrans_packet_count );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].is_confirmed );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].nonce[0] );
        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].nonce[1] );

        new_offset = sctp_buffer_get_Ulong( new_offset,
                                            &( assoc->transmit ).remote[count].perf_path_verf );
        /* Rel 6.2 Update Ends Here. */

    }

    /* The various queues like sackfreelist, sacklist, transmit and receive
     * queues have been initialized at time of stack init
     */

    /*Adding sack fragments to sacklist*/
    for( count = 0; count < assoc_db->receive.sacknum; count++ )
    {
        /*Get free sack fragment from sackfreelist*/
        sackfrag = ( sctp_sackfrag_st * )lstGet( &( assoc->receive ).sackfreelist );

        new_offset = sctp_buffer_get_Ulong( new_offset, &( sackfrag->first ) );
        new_offset = sctp_buffer_get_Ulong( new_offset, &( sackfrag->last ) );

        /* Put sack buffer to sacklist */
        lstAdd( &( assoc->receive ).sacklist, ( NODE * )sackfrag );

    }

    /* Putting transmit buffers */
    for( count = 0; count < assoc_db->transmit.num_tx_buf; count++ )
    {
        new_offset = sctp_buffer_get_Ulong( new_offset, &buffer_id );
        buffer = sctp_get_transmit_buffer( assoc, buffer_id );
        /* Buffer ID has been passed instead of size in order to have
         * buffer of identical size
         */

        buffer->id = buffer_id;

        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer_size ) );
        buffer->size = buffer_size;

        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->ppi ) );
        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->tsn ) );

        new_offset = sctp_buffer_get_Ushort( new_offset, &( buffer->stream ) );
        new_offset = sctp_buffer_get_Ushort( new_offset, &( buffer->ssn ) );

        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->unordered ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->begin ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->end ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->read_flag ) );

        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->lifetime ) );
        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->context ) );

        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->dest_addr_index ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->sent ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->missed ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->bundle ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->retrans_flag ) );
        new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->ack_flag ) );

        new_offset = sctp_buffer_get_Buffer( new_offset, buffer_size,
                                             buffer->buffer );

        /* Rel 6.2 Update */
        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->ref_count ) );
        new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->buffer_read_index ) );
        /* Rel 6.2 Update Ends Here. */

        /* Adding transmit buffer to transmit queue */
        lstAdd( &( ( assoc->transmit ).transmit_queue ), ( NODE * )buffer );

    }

    /* putting receive buffers */
    for( count = 0; count < assoc_db->receive.num_in_streams; count++ )
    {
        buf_num = assoc_db->receive.num_rx_buf[count];

        for( buf_count = 0; buf_count < buf_num; buf_count++ )
        {
            new_offset = sctp_buffer_get_Ulong( new_offset, &buffer_id );
            buffer = sctp_get_receive_buffer( assoc, buffer_id );

            /* Buffer ID has been passed instead of size in order to have
             * buffer of identical size.
             */

            buffer->id = buffer_id;

            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer_size ) );
            buffer->size = buffer_size;

            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->ppi ) );
            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->tsn ) );

            new_offset = sctp_buffer_get_Ushort( new_offset, &( buffer->stream ) );
            new_offset = sctp_buffer_get_Ushort( new_offset, &( buffer->ssn ) );

            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->unordered ) );
            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->begin ) );
            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->end ) );
            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->read_flag ) );

            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->lifetime ) );
            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->context ) );

            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->dest_addr_index ) );
            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->sent ) );
            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->missed ) );
            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->bundle ) );

            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->retrans_flag ) );

            new_offset = sctp_buffer_get_Uchar( new_offset, &( buffer->ack_flag ) );

            new_offset = sctp_buffer_get_Buffer( new_offset, buffer_size, buffer->buffer );

            /* Rel 6.2 Update */
            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->ref_count ) );
            new_offset = sctp_buffer_get_Ulong( new_offset, &( buffer->buffer_read_index ) );
            /* Rel 6.2 Update Ends Here. */

            /*Adding the receive buffer to the stream receive list*/
            lstAdd( &( assoc->receive ).stream[count].receive_queue,
                    ( NODE * )buffer );
        }
    }

    return new_offset;
}
/***************************************************************************
 *  FUNCTION :    sctp_api_set_assoc_red_db
 *
 *  DESCRIPTION:
 *    This function sets the assocoation database of redundancy instance.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_api_set_assoc_red_db(
    sctp_U32bit    assoc_id,
    sctp_U8bit     *assoc_data,
    sctp_error_t   *errorno )

{
    sctp_tcb_st          *assoc = SCTP_NULL;
    sctp_ep_st           *endpoint;
    sctp_red_ep_db_st    *ep_db;
    sctp_U8bit           *new_offset, *tmp_offset;
    sctp_U32bit          count, len;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_api_set_assoc_red_db" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_FAILURE;
    }

    if( ( sctp_stack_proc_state == SCTP_PROC_STATE_START ) ||
            ( sctp_stack_state == SCTP_STATE_UNINTIALISED ) )
    {
        /* If stack is START return failure */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    if( assoc_data == SCTP_NULL || ( assoc_id > sctp_db_main.max_ep - 1 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        return SCTP_FAILURE;
    }

    ep_db = ( sctp_red_ep_db_st * )assoc_data;

    if( ep_db->ep_id != assoc_id )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        return SCTP_FAILURE;
    }

    if( num_sockets_to_update == 0 )
    {
        /* If number of assocs to update is 0, return failure */
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    /* Extract the association */
    endpoint = &sctp_db_main.ep_table[assoc_id];

    /* If association is not active, return error
     * This active/inactive state is set by sctp_api_set_stack_db
     */
    if( SCTP_SOCK_IS_IN_USE( endpoint->flags ) != SCTP_TRUE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        return SCTP_FAILURE;
    }

    /* If socket's address family is not none, it means assoc data base has
     * been updated previously by sctp_api_set_assoc_db().
     * Return error in this case
     */
    if( SCTP_VALID_SK_FAMILY( endpoint->family ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = E_REQ_IN_WRONG_STATE;
        return SCTP_FAILURE;
    }

    endpoint->ep_id        = ep_db->ep_id;
    endpoint->server_ep_id = ep_db->server_ep_id;

    endpoint->num_created_ep = ep_db->num_created_ep;

    for( count = 0; count < ep_db->num_created_ep && count < SCTP_MAX_LISTEN_BACKLOG; count++ )
    {
        endpoint->created_ep_id[count]  = ep_db->created_ep_id[count];
    }

    endpoint->ppi              = ep_db->ppi;

    endpoint->sk_config        = ep_db->sk_config;
    endpoint->trx_cong_reg_def = ep_db->trx_cong_reg_def;
    endpoint->rcv_cong_reg_def = ep_db->rcv_cong_reg_def;

    endpoint->n_assoc         = ep_db->n_assoc;
    endpoint->last_used_assoc = ep_db->last_used_assoc;

    endpoint->this_port = ep_db->this_port;
    endpoint->naddr = ep_db->naddr;

    for( count = 0; count < ep_db->naddr && count < SCTP_MAX_TRANSPORT_ADDR; count++ )
    {
        endpoint->this_address[count] = ep_db->this_address[count];
    }

    for( count = 0; count < SCTP_MAX_HOSTNAME_SIZE; count++ )
    {
        endpoint->hostname[count] = ep_db->hostname[count];
    }

    endpoint->l_backlog = ep_db->l_backlog;
    endpoint->flags     = ep_db->flags;
    endpoint->trc_flags = ep_db->trc_flags;

    endpoint->appl_id   = ep_db->appl_id;

    endpoint->family    = ep_db->family;
    endpoint->sk_type   = ep_db->sk_type;
    endpoint->so_linger = ep_db->so_linger;

    endpoint->moribund = ep_db->moribund;

    endpoint->hostname_supported = ep_db->hostname_supported;
    endpoint->create_from_server = ep_db->create_from_server;
    endpoint->cong_flag          = ep_db->cong_flag;

    if( SCTP_SOCK_IS_HASHED( endpoint->flags ) )
    {
        if( ( SCTP_SOCK_STREAM == endpoint->sk_type ) &&
                ( !SCTP_SOCK_IS_LISTENER( endpoint->flags ) ) )
        {
            sctp_db_add_hash( SCTP_BIND_HASH, endpoint );
        }

        else
        {
            sctp_db_add_hash( SCTP_LISTEN_HASH, endpoint );
        }
    }

    new_offset = assoc_data + sizeof( sctp_red_ep_db_st );

    for( count = 0; count < endpoint->n_assoc; count++ )
    {
        assoc = sctp_db_get_free_association( errorno );

        if( assoc == SCTP_NULL )
        {
            /* error No Resources */
            return SCTP_FAILURE;
        }

        /* add this assoc to the Endpoint */
        lstAdd( &( endpoint->assoc_list ), ( NODE * )assoc );
        assoc->p_ep = endpoint;

        tmp_offset = new_offset;
        new_offset = sctp_buffer_get_Ulong( new_offset, &len );
        new_offset = sctp_set_assoc_db( new_offset, assoc, errorno );

        if( ( sctp_U32bit )( new_offset - tmp_offset ) != len )
        {
            /* error */
            return SCTP_FAILURE;
        }

        sctp_db_add_assoc_verf( assoc );
    }

    /* Decrement the number of associations to update by 1*/
    num_sockets_to_update--;

    return SCTP_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :
 *    sctp_api_get_stack_health
 *
 *    DESCRIPTION:
 *    This function gives information about the state and health of stack
 *    stack.
 *
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_api_get_stack_health(
    sctp_stack_red_state_et    *red_state,
    sctp_stack_proc_state_et   *proc_state,
    sctp_error_t               *errorno )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_api_get_stack_health" ) );

    if( errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return SCTP_FAILURE;
    }

    if( red_state == SCTP_NULL ||
            proc_state == SCTP_NULL ||
            errorno == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *errorno = EINV_PARAMS;
        return SCTP_FAILURE;
    }

    *red_state =  sctp_stack_red_state;
    *proc_state = sctp_stack_proc_state;
    return SCTP_SUCCESS;
}

/***************************************************************************
 *  FUNCTION :
 *    sctp_init_red_module
 *
 *  DESCRIPTION:
 *    This function initializes the redundancy module.
 *    It allocate the memory in which redundancy APIs return database.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_init_red_module(
    sctp_error_t  *errorno )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_init_red_module" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( errorno );
    /*to avoid the warning*/


    return SCTP_SUCCESS;
}


/***************************************************************************
 *  FUNCTION :
 *    sctp_deinit_red_module
 *
 *  DESCRIPTION:
 *    This function deinitializes the redundancy module.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t
sctp_deinit_red_module()
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::RED Module:In Function sctp_init_red_module" ) );

    return SCTP_SUCCESS;
}




#endif     /* SCTP_CLI_RECV_COM_THREAD    */
#endif     /* SCTP_UI_MESSAGEBASED */
#endif     /* SCTP_RED_SUPPORT    */
