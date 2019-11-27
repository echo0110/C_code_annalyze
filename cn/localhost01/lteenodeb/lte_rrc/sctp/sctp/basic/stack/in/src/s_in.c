/***************************************************************************
 *  FILE NAME  : s_in.c
 *
 *  DESCRIPTION: Contains the Stack Initialization Module.
 *    This file is part of SCTP Stack Initilisation module. Initilisation
 *    module give interface to System Management to Initialise the SCTP
 *    stack. Following is performed at the time of stack Initilisation.
 *    sctp_init_db_module
 *    sctp_init_stats_module
 *    sctp_init_error_module
 *    sctp_init_db_module
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  10Oct02    manish rajpal 4955        UDP port problem
 *  15Sep03    Manish Rajpal             Rel 5.0
 *  27Now03    Vivek Bansal              CSPL Changes
 *  29Dec03    Vivek Bansal              Changes for API de-init
 *  02Nov04    Svakumar     REL 6.0      SCTP/FUNC-CONGCON-5
 *  29Mar11    Supreet Jain     SPR 20861      CSR 2918
 *  29Apr13    virendra     CSR 43698    SPR 21129
 *  1July13     Rohan Pawa  SPR 21141       CSR 50452
 *  28Apr14     Rohan Pawa  Bug-Id 3        CSR 77058
 *  18AUg15    Gurpreet Singh             REL 6.2.0 CSR 105109 Bug ID 68
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "IN"

#include <s_in.h>
#include <s_cobuf.h>
#include <s_cocksm.h>
#include <s_comac.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_cotyps.h>
#include <s_dbtcb.h>
#include <s_stats.h>
#include <s_esdef.h>
#include <s_es.h>
#include <s_tlif.h>
#include <s_ostmr.h>
#include <s_mhmac.h>
#include <s_err.h>
#include <s_fsmmtu.h>
#include <s_err.h>
#include <s_osmem.h>
#include <s_osmutx.h>
#include <s_resnam.h>
#include <s_red.h>
#include <s_uiutil.h>
#include <s_fsmass.h>


/*
 * Global structure containing the stack Initialization params.
 */
sctp_capability_st           sctp_capability;
/*Bug ID 68 changes s*/
sctp_Boolean_t          stop_abort = SCTP_FALSE;
/*Bug ID 68 changes e*/
extern sctp_Boolean_t glb_sctp_stack_env_state;

extern sctp_U16bit g_dummy_sock;
/*Fix Bug-Id 3 starts*/
extern sig_ipc_mgr_glb_data_st  g_ipc_mgr_data[SIG_MAX_NUM_STACK];
/*Fix Bug-Id 3 ends*/
sctp_U32bit sctp_checksum_none( sctp_U8bit *buf, sctp_U32bit len );

sctp_U32bit sctp_checksum_none( sctp_U8bit *buf, sctp_U32bit len )
{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( buf );
    SCTP_UNUSED_VARIABLE( len );
    /*to avoid the warning*/


    /*do nothing*/
    return 0;
}

sctp_compute_cksm_fn_t       sctp_compute_cksm[SCTP_NUM_CHECKSUM] =
{
    generate_crc32c, compute_adler32_cksm, sctp_checksum_none
};

sctp_compute_cksm_iovec_fn_t sctp_compute_cksm_iovec[SCTP_NUM_CHECKSUM] =
{
    sctp_compute_crc32_iovec, sctp_compute_adler32_iovec,
    ( sctp_compute_cksm_iovec_fn_t )sctp_checksum_none
};
/*
 * Contains the state of the SCTP stack entity.
 * This can have following possible
 * values UNINITIALIZED OR ACTIVE
 */
sctp_stack_state_et sctp_stack_state = SCTP_STATE_UNINTIALISED;


sctp_U8bit   *TL_free_pool;

LIST         TL_buffers;

/* The Lock that protects the TL buffer list */
sctp_lock_t  TL_buflist_lock;

#ifdef SCTP_UI_SU_MESSAGEBASED
sctp_lock_t  msgif_lock;
#endif

/***************************************************************************
 *    FUNCTION :
 *       sctp_api_parse_stack_params
 *
 *    DESCRIPTION:
 *        This api checks stack parameters.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_parse_stack_params(
    sctp_prot_params_st *p_prot_params,
    sctp_error_t        *p_ecode )
{
    sctp_U16bit rto_alpha;
    sctp_U16bit rto_beta;
    sctp_U32bit rto_init;
    sctp_U32bit rto_min;
    sctp_U32bit rto_max;
    sctp_prot_params_st tmp_config;
    sctp_U16bit max_hb_infolen;


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_parse_stack_params" ) );

    if( !p_prot_params )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Prot Parameters Structure" ) );

        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    tmp_config = sctp_db_main.sctp_def_prot_params;

    if( p_prot_params->rto_min != 0 )
    {
        if( ( p_prot_params->rto_min <= SCTP_MAX_RTO_MIN ) &&
                ( p_prot_params->rto_min >= SCTP_MIN_RTO_MIN ) )
        {
            rto_min = p_prot_params->rto_min;
        }

        else
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid RTO_MIN Prot Parameters Structure" ) );

            *p_ecode = EINV_RTO_MIN_PARAMS;

            return ( SCTP_FAILURE );
        }
    }

    else
    {
        rto_min = tmp_config.rto_min;
    }

    if( p_prot_params->rto_max != 0 )
    {
        if( ( p_prot_params->rto_max <= SCTP_MAX_RTO_MAX ) &&
                ( p_prot_params->rto_max >= SCTP_MIN_RTO_MAX ) )
        {
            rto_max = p_prot_params->rto_max;
        }

        else
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid RTO_MIN Prot Parameters Structure" ) );

            *p_ecode = EINV_RTO_MAX_PARAMS;

            return ( SCTP_FAILURE );
        }
    }

    else
    {
        rto_max = tmp_config.rto_max;
    }

    /* static checks on RTO parameters */
    if( p_prot_params->rto_init != 0 )
    {
        if( ( p_prot_params->rto_init >= rto_min ) &&
                ( p_prot_params->rto_init <= rto_max ) )
        {
            rto_init = p_prot_params->rto_init;
        }

        else
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid RTO_INIT Prot Parameters Structure" ) );

            *p_ecode = EINV_RTO_INIT_PARAMS;

            return ( SCTP_FAILURE );
        }
    }

    else
    {
        rto_init = tmp_config.rto_init;
    }

    /*
     * rto_max should be more than rto_min, rto_init and rto_max should be
     * more than rto_min.
     */
    if( ( rto_min > rto_init ) || ( rto_min > rto_max ) || ( rto_init > rto_max ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Rto Parameters " ) );

        *p_ecode = EINV_RTO_COMBINATION_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( p_prot_params->valid_cookie_life != 0 )
    {
        if( ( p_prot_params->valid_cookie_life > SCTP_MAX_VALID_COOKIE_LIFE ) ||
                ( p_prot_params->valid_cookie_life < SCTP_MIN_VALID_COOKIE_LIFE ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid cookie life Parameters " ) );

            *p_ecode = EINV_COOKIE_LIFE_PARAMS;

            return ( SCTP_FAILURE );

        }
    }

    if( p_prot_params->hb_interval != 0 )
    {
        if( ( p_prot_params->hb_interval * SCTP_CONFIG_STACK_PARAMS_HB_UNIT > SCTP_MAX_HB_INTERVAL )
                || ( p_prot_params->hb_interval * SCTP_CONFIG_STACK_PARAMS_HB_UNIT < SCTP_MIN_HB_INTERVAL ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid HB_INTERVAL Parameters " ) );

            *p_ecode = EINV_HB_INTERVAL_PARAMS;
            return ( SCTP_FAILURE );
        }
    }

    if( ( p_prot_params->sack_frequency > SCTP_MAX_SACK_FREQUENCY ) ||
            ( p_prot_params->sack_frequency == 0 ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid SACK_FREQUENCY Parameters " ) );

        *p_ecode = EINV_SACK_FREQUENCY_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( p_prot_params->sack_period > SCTP_MAX_DELAYACK_TIME )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid SACK_PERIOD Parameters " ) );

        *p_ecode = EINV_SACK_PERIOD_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( p_prot_params->mtu_size < SCTP_MIN_MTUSIZE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid MTU_SIZE Parameters " ) );

        *p_ecode = EINV_MTU_SIZE_PARAMS;

        return ( SCTP_FAILURE );
    }

    max_hb_infolen = SCTP_IS_STACK_UDP ?
                     SCTP_UDP_MAX_HB_INFOLEN( p_prot_params->mtu_size ) :
                     SCTP_IP_MAX_HB_INFOLEN( p_prot_params->mtu_size );

    if( ( max_hb_infolen < p_prot_params->hb_info_length ) ||
            ( SCTP_MIN_HBINFO_LEN > p_prot_params->hb_info_length ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid HB_INFO_LEN Parameters " ) );

        *p_ecode = EINV_HB_INFO_LEN;

        return ( SCTP_FAILURE );
    }


    /* static checks on rto_alpha and rto_beta parameters*/
    rto_alpha = p_prot_params->rto_alpha_percent;
    rto_beta  = p_prot_params->rto_beta_percent;

    /* alpha & beta should be between 0-100, alpha should be less than beta*/

    if( ( rto_alpha > SCTP_MAX_PERCENTAGE_VALUE )
            || ( rto_beta  > SCTP_MAX_PERCENTAGE_VALUE )
            || ( ( rto_alpha != 0 ) && ( rto_beta != 0 ) && ( rto_alpha > rto_beta ) )
            || ( ( rto_alpha != 0 ) && ( rto_beta == 0 )
                 && ( rto_alpha > tmp_config.rto_beta_percent ) )
            || ( ( rto_alpha == 0 ) && ( rto_beta != 0 )
                 && ( rto_beta  < tmp_config.rto_alpha_percent ) ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::Invalid Rto Percentage Parameters " ) );

        *p_ecode = EINV_ALPHA_BETA_PARAMS;

        return ( SCTP_FAILURE );
    }

    /* static checks on retrans parameters */
    if( ( ( p_prot_params->path_max_retrans != 0 )
            && ( p_prot_params->assoc_max_retrans != 0 )
            && ( p_prot_params->path_max_retrans > p_prot_params->assoc_max_retrans ) )
            || ( ( p_prot_params->path_max_retrans != 0 )
                 && ( p_prot_params->assoc_max_retrans == 0 )
                 && ( p_prot_params->path_max_retrans
                      > tmp_config.assoc_max_retrans ) )
            || ( ( p_prot_params->path_max_retrans == 0 )
                 && ( p_prot_params->assoc_max_retrans != 0 )
                 && ( p_prot_params->assoc_max_retrans
                      < tmp_config.path_max_retrans ) ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Parameter Max. Path Retrans more Max. Assoc Retrans" ) );

        *p_ecode = EINV_PATH_RETRANS_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( ( p_prot_params->num_in_streams > sctp_db_main.max_streams_per_assoc )
            || ( p_prot_params->num_out_streams > sctp_db_main.max_streams_per_assoc ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid stream Parameters " ) );

        *p_ecode = EINV_STREAM_PARAMS;

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_process_api_config_stack
 *
 *    DESCRIPTION:
 *        Set configuration parameters for a stack.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/
void
sctp_process_api_config_stack(
    sctp_prot_params_st   *prot_params )
{

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "In Function sctp_process_api_config_stack " ) );

    if( prot_params->rto_max != 0 )
    {
        sctp_db_main.sctp_def_prot_params.rto_max = prot_params->rto_max;
    }

    if( prot_params->rto_min != 0 )
    {
        sctp_db_main.sctp_def_prot_params.rto_min = prot_params->rto_min;
    }

    if( prot_params->assoc_max_retrans != 0 )
        sctp_db_main.sctp_def_prot_params.assoc_max_retrans =
            prot_params->assoc_max_retrans;

    if( prot_params->path_max_retrans != 0 )
        sctp_db_main.sctp_def_prot_params.path_max_retrans =
            prot_params->path_max_retrans;

    /* heartbeat interval expected for  user in second */
    if( prot_params->hb_interval != 0 )
        sctp_db_main.sctp_def_prot_params.hb_interval =
            prot_params->hb_interval * SCTP_CONFIG_STACK_PARAMS_HB_UNIT;

    if( prot_params->rto_init != 0 )
    {
        sctp_db_main.sctp_def_prot_params.rto_init = prot_params->rto_init;
    }

    if( prot_params->rto_alpha_percent != 0 )
        sctp_db_main.sctp_def_prot_params.rto_alpha_percent =
            prot_params->rto_alpha_percent;

    if( prot_params->rto_beta_percent != 0 )
        sctp_db_main.sctp_def_prot_params.rto_beta_percent =
            prot_params->rto_beta_percent;

    if( prot_params-> valid_cookie_life != 0 )
        sctp_db_main.sctp_def_prot_params.valid_cookie_life =
            prot_params->valid_cookie_life;

    if( prot_params->max_init_retrans != 0 )
        sctp_db_main.sctp_def_prot_params.max_init_retrans =
            prot_params->max_init_retrans;

    if( prot_params->init_timeo != 0 )
        sctp_db_main.sctp_def_prot_params.init_timeo =
            prot_params->init_timeo;

    if( prot_params->max_init_path_retrans != 0 )
        sctp_db_main.sctp_def_prot_params.max_init_path_retrans =
            prot_params->max_init_path_retrans;

    if( prot_params->num_in_streams != 0 )
        sctp_db_main.sctp_def_prot_params.num_in_streams =
            prot_params->num_in_streams;

    if( prot_params->num_out_streams != 0 )
        sctp_db_main.sctp_def_prot_params.num_out_streams =
            prot_params->num_out_streams;

    if( prot_params->bundling_time != 0 )
    {
        sctp_db_main.sctp_def_prot_params.bundling_time = prot_params->bundling_time;
    }

    sctp_db_main.sctp_def_prot_params.sack_period = prot_params->sack_period;

    sctp_db_main.sctp_def_prot_params.sack_frequency = prot_params->sack_frequency;

    sctp_db_main.sctp_def_prot_params.mtu_size = prot_params->mtu_size;

    sctp_db_main.sctp_def_prot_params.pmtu_capable = prot_params->pmtu_capable;

    sctp_db_main.sctp_def_prot_params.hb_info_length = prot_params->hb_info_length;
    sctp_db_main.sctp_def_prot_params.hb_max_burst = prot_params->hb_max_burst;

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_db_main.sctp_def_prot_params.ip_header_params.ttl =  prot_params->ip_header_params.ttl;
    sctp_db_main.sctp_def_prot_params.ip_header_params.tos =  prot_params->ip_header_params.tos;
#ifdef SCTP_INET6_SUPPORT

#if defined (IPV6_HOPLIMIT)
    sctp_db_main.sctp_def_prot_params.ip_header_params.hoplimit =  prot_params->ip_header_params.hoplimit;
#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                ( "sctp_process_api_config_stack : IPV6_HOPLIMIT not defined in the System." ) );
#endif

#if defined (IPV6_TCLASS)
    sctp_db_main.sctp_def_prot_params.ip_header_params.tclass =  prot_params->ip_header_params.tclass;
#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                ( "sctp_process_api_config_stack : IPV6_TCLASS not defined in the System." ) );
#endif

#endif /* SCTP_INET6_SUPPORT */
#endif /* SCTP_IP_HEADER_PARAMS */

    return;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_config_stack_params
 *
 *    DESCRIPTION:
 *        This function parses the stack parameters and then processes the
          configuration parametrs
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_config_stack_params(
    sctp_prot_params_st *prot_params,
    sctp_error_t        *errorno )
{
    if( errorno == NULL || prot_params == NULL )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "SM API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *errorno = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "In Function sctp_api_config_stack_params" ) );

    SCTPLOCK();

    if( sctp_api_parse_stack_params( prot_params, errorno ) == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTPUNLOCK();

        return( SCTP_FAILURE );
    }

    sctp_process_api_config_stack( prot_params );

    SCTPUNLOCK();

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_init_stack_parameters
 *
 *    DESCRIPTION:
 *      Initialise SCTP stack init parameters
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_init_stack_parameters(
    sctp_init_options_st   *init_options,
    sctp_sockaddr_list_st  local_addr_list,
    sctp_error_t           *p_ecode )
{
    sctp_return_t ret;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( local_addr_list );
    /*to avoid the warning*/



    /* Copy the stack capability values */
    sctp_capability = init_options->capability;

    /* Static check on number of local addresses */
    if( ( init_options->no_of_local_addrs > SCTP_MAX_TRANSPORT_ADDR )
            || ( ( init_options->no_of_local_addrs == 0 )
                 && ( init_options->hostname == NULL ) ) )
        /*SPR 21141 changes starts*/
        /*||( (init_options->no_of_local_addrs != 0)
        && (init_options->list_of_local_addrs == NULL)))*/
        /*SPR 21141 changes ends*/

    {
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( ( init_options->hostname != NULL )
            && ( init_options->no_of_local_addrs == 0 ) )
    {
        /* Hostname non NULL, resolve it now and update the init params */

        ret = sctp_TL_resolve_hostname(
                  init_options->hostname,
                  &( init_options->no_of_local_addrs ),
                  init_options->list_of_local_addrs );

        if( ret == SCTP_FAILURE )
        {
            *p_ecode = E_UNKNOWN_HOST;

            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_init_stack
 *
 *    DESCRIPTION:
          This function initialises the stack.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_init_stack(
    sctp_init_options_st *init_options,
    sctp_error_t         *p_ecode )
{
    sctp_return_t         ret;
    sctp_U32bit           max_timers, counter;
    sctp_sockaddr_list_st local_addr_list;
    sctp_time_t           curr_time;
    sctp_TL_packet_st     *tl_packet;
#ifdef SCTP_CMN_RECV_POLL_THREAD
    /* CSR 1-3217548 */
    sctp_timer_t      dummy_timer_id;
#endif
    /*Fix Bug-Id 3 starts*/
    sctp_U32bit i = 0;

    /*Fix Bug-Id 3 ends*/
    if( SCTP_FALSE == glb_sctp_stack_env_state )
    {
        return SCTP_FAILURE;
    }

    if( ( init_options == NULL ) || ( p_ecode == NULL ) )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_UNINTIALISED )
    {
        *p_ecode = EINV_PARAMS;
        return ( SCTP_FAILURE );
    }

    /* static check on init parameters */
    if( ( init_options->MaxPerAssoc.streams > SCTP_MAX_NUMSTREAM )
            || ( init_options->MaxPerAssoc.streams == 0 )

            || ( init_options->max_appl > SCTP_MAX_TOTAL_APP )
            || ( init_options->max_appl == 0 )

            || ( init_options->max_endpoint > SCTP_MAX_TOTAL_ASSOC )
            || ( init_options->max_endpoint == 0 )

            || ( init_options->max_tcb == 0 )

            || (
                ( init_options->MaxPerAssoc.min_tx_buffers == 0 )
                && ( init_options->MaxPerAssoc.med_tx_buffers == 0 )
                && ( init_options->MaxPerAssoc.max_tx_buffers == 0 )
            )
            || (
                ( init_options->MaxPerAssoc.min_rx_buffers == 0 )
                && ( init_options->MaxPerAssoc.med_rx_buffers == 0 )
                && ( init_options->MaxPerAssoc.max_rx_buffers == 0 )
            )

            /* Section 6 of RFC 2960
               A SCTP receiver MUST be able to receive a minimum of 1500 bytes in
               one SCTP packet.  This means that a SCTP endpoint MUST NOT indicate
               less than 1500 bytes in its Initial a_rwnd sent in the INIT or INIT
               ACK.
            */
            || (
                ( SCTP_MIN_INIT_ARWND >
                  init_options->MaxPerAssoc.min_rx_buffers * SCTP_MIN_FRAGSIZE +
                  init_options->MaxPerAssoc.med_rx_buffers * SCTP_MED_FRAGSIZE +
                  init_options->MaxPerAssoc.max_rx_buffers * SCTP_MAX_FRAGSIZE )
            )

            || ( init_options->capability.ack_policy > SCTP_MAX_NUM_ACKPOLICY )
            || ( init_options->capability.stale_cookie_policy >
                 SCTP_MAX_NUM_COOKOEPOLICY )

            || ( ( init_options->capability.stack_type == SCTP_OVER_UDP )
                 && ( init_options->capability.ecn_capable == SCTP_TRUE ) )

            /*SPR 4955*/
            || ( ( init_options->capability.stack_type == SCTP_OVER_UDP )
                 /*SPR 4955*/
                 && ( init_options->standard_udp_port < SCTP_MAX_UDP_RESERVED_PORT ) )

            /*    || ((init_options->checksum < 0)*/
            || ( init_options->checksum >= SCTP_NUM_CHECKSUM )

            || ( ( init_options->capability.pmtu_capable == SCTP_TRUE )
                 && ( init_options->capability.stack_type == SCTP_OVER_UDP ) ) )
    {
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    /* Initilize the TL buffer list lock */
    SCTP_INIT_LOCK( TL_buflist_lock );
#ifdef SCTP_UI_SU_MESSAGEBASED
    SCTP_INIT_LOCK( msgif_lock );
#endif


#ifdef SCTP_DEBUG_ENABLED

    /* Inilialise the debug Module, Disable Tracing for all Trace flags */
    if( sctp_init_debg_module( p_ecode ) == SCTP_FAILURE )
    {
        return ( SCTP_FAILURE );
    }

#endif

    /* Initialise SCTP stack init parameters */
    ret = sctp_init_stack_parameters( init_options,
                                      local_addr_list, p_ecode );

    if( ret == SCTP_FAILURE )
    {
        return ( SCTP_FAILURE );
    }

    /* Initialise db with parameters passed by SM */
    ret = sctp_init_db_module( init_options, p_ecode );

    if( ret == SCTP_FAILURE )
    {
        return ( SCTP_FAILURE );
    }

#ifdef SCTP_STATS_ENABLED

    /*
     * Initialise statistics module, reset counters to zero and enable
     * statistics
     */
    if( sctp_init_stats_module
            ( init_options->max_tcb, p_ecode ) == SCTP_FAILURE )
    {
        /*Fix Bug-Id 3 starts*/
        ret = sctp_deinit_db_module();
        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        return ( SCTP_FAILURE );
    }

#endif

#ifdef SCTP_ERROR_ENABLED

    /* Inilialise the Error Module,Enable Critical erors for all Error Flags */
    if( sctp_init_error_module( p_ecode ) == SCTP_FAILURE )
    {
        /*Fix Bug-Id 3 starts*/
        ret = sctp_deinit_db_module();
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        /*Fix Bug-Id 3 ends*/
        return ( SCTP_FAILURE );
    }

#endif

    /* Lock the TL buffer list */
    SCTP_LOCK( TL_buflist_lock );

    /* Initialize the global TL buffer queue */
    TL_free_pool = ( sctp_U8bit * )
                   __MALLOC( SCTP_MAX_TL_BUFFER_NUM * sizeof( sctp_TL_packet_st ) );

    if( TL_free_pool == SCTP_NULL )
    {
        *p_ecode = E_MALLOC_FAILED;
        SCTP_UNLOCK( TL_buflist_lock );
        /*Fix Bug-Id 3 starts*/
        ret = sctp_deinit_db_module();
        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */

        return ( SCTP_FAILURE );
    }

    tl_packet = ( sctp_TL_packet_st * )( TL_free_pool );
    lstInit( &TL_buffers );

    /* Add the defined number of buffers to the global TL buffers queue */
    for( counter = 0; counter < SCTP_MAX_TL_BUFFER_NUM; counter++ )
    {
        lstAdd( &TL_buffers, ( NODE * ) & ( tl_packet[counter] ) );
    }

    /* Unlock the TL buffer list */
    SCTP_UNLOCK( TL_buflist_lock );


#ifdef SCTP_NAME_RESOLV_THREAD

    /* Initialize the name resolver */
    if( sctp_hostname_resolver_init() == SCTP_FAILURE )
    {
        *p_ecode = E_NAME_RES_FAILED;
        /*Fix Bug-Id 3 starts*/
        SCTP_LOCK( TL_buflist_lock );
        __FREE( TL_free_pool );
        SCTP_UNLOCK( TL_buflist_lock );

        ret = sctp_deinit_db_module();
        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        return ( SCTP_FAILURE );
    }

#endif


    /*
     * Indicate encapsulating  Shell
     * This creates a thread for receiving TL data,
     * ICMP messages and handle timeouts.
     */
    ret = sctp_init_ehsell(
              init_options->capability.stack_type,
              init_options->standard_udp_port,
              init_options->no_of_local_addrs,
              init_options->list_of_local_addrs,
              p_ecode );

    if( ret == SCTP_FAILURE )
    {
        /* SPR 21129 FIX starts */
        SCTP_LOCK( TL_buflist_lock );
        __FREE( TL_free_pool );
        SCTP_UNLOCK( TL_buflist_lock );

        ret = sctp_deinit_db_module();

        /* SPR 21129 FIX ends */
        /*Fix Bug-Id 3 starts*/
        for( i = 0; i < SOC_MGR_MAX_CONNECTIONS; ++i )
        {
            sigtran_mutex_deinit( &( g_ipc_mgr_data[init_options->capability.stack_type].ipc_conn_info.conn_info[i].fd_mutex ) );
        }

        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        return ( SCTP_FAILURE );
    }

    __GET_ABSOLUTE_SYSTIME( &curr_time );

#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    __INITSEED( init_options->rand_seed );

    /* Initialized redundancy module */
    ret = sctp_init_red_module( p_ecode );

    if( ret == SCTP_FAILURE )
    {
        /*Fix Bug-Id 3 starts*/
        SCTP_LOCK( TL_buflist_lock );
        __FREE( TL_free_pool );
        SCTP_UNLOCK( TL_buflist_lock );

        ret = sctp_deinit_db_module();

        for( i = 0; i < SOC_MGR_MAX_CONNECTIONS; ++i )
        {
            sigtran_mutex_deinit( &( g_ipc_mgr_data[init_options->capability.stack_type].ipc_conn_info.conn_info[i].fd_mutex ) );
        }

        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        return SCTP_FAILURE;
    }

#endif /* SCTP_CLI_RECV_COM_THREAD */
#endif /* SCTP_UI_MESSAGEBASED */

#else
    /* SPR 20861 Starts*/
    __INITSEED( curr_time.tv_usec );
    /* SPR 20861 Ends*/

#endif /* SCTP_RED_SUPPORT */

    /*
     * There are 2 global timers, mac keys update timer and
     * pmtu database update timer , rest  all are per association
     * timers
     * A third global dummy Timer is added in the flag
     * SCTP_CMN_RECV_POLL_THREAD which is required to get a
     * minimum timedwait select timeout in Single threaded
     * architecture.
     * CSR 1-3217548
     */
#ifndef SCTP_CMN_RECV_POLL_THREAD
    max_timers
        = ( init_options->max_tcb * SCTP_ASSOCTIMER_NUM )
          + ( init_options->max_tcb * SCTP_DESTTIMER_NUM
              * SCTP_MAX_TRANSPORT_ADDR ) + 2;
#else
    max_timers
        = ( init_options->max_tcb * SCTP_ASSOCTIMER_NUM )
          + ( init_options->max_tcb * SCTP_DESTTIMER_NUM
              * SCTP_MAX_TRANSPORT_ADDR ) + 3;
#endif

#ifdef SCTP_UI_SOCK_API
    /* timers for select call */
    max_timers += SCTP_MAX_SIMUL_SELECT;
#endif

    /*
     * Initialise the timer module
     */

    ret = sctp_init_timer_module( max_timers , init_options->max_tcb, p_ecode );

    if( ret == SCTP_FAILURE )
    {
        /* SPR 21129 FIX starts */
        SCTP_LOCK( TL_buflist_lock );
        __FREE( TL_free_pool );
        SCTP_UNLOCK( TL_buflist_lock );

        ret = sctp_deinit_db_module();

        /* SPR 21129 FIX ends */
        /*Fix Bug-Id 3 starts*/
        for( i = 0; i < SOC_MGR_MAX_CONNECTIONS; ++i )
        {
            sigtran_mutex_deinit( &( g_ipc_mgr_data[init_options->capability.stack_type].ipc_conn_info.conn_info[i].fd_mutex ) );
        }

        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        return ( SCTP_FAILURE );
    }

    /* CSR 1-3897651 */
#if !defined(SCTP_UI_SU_MESSAGEBASED) && !defined(SCTP_UI_SOCK_API)
#ifndef SCTP_NO_QUEUE

    sctp_su_init_notification_queue();
#endif
#endif
    /* END CSR 1-3897651 */
    /* Start the global dummy timer for getting a minimum
     * select timewait
     * CSR 1-3217548
     */
#ifdef SCTP_CMN_RECV_POLL_THREAD
    sctp_start_timer(
        SCTP_NULL,
        SCTPEVENT_TIMER_DUMMY,
        SIG_SELECT_TIMEOUT,
        0, &dummy_timer_id, SCTP_NULL );
#endif

    /* initialise the mac keys */
    sctp_init_mac_keys();

    /* initialise the pmtu database */
    ret = sctp_init_pmtu_database( p_ecode );

    if( ret == SCTP_FAILURE )
    {
        /*Fix Bug-Id 3 starts*/
        SCTP_LOCK( TL_buflist_lock );
        __FREE( TL_free_pool );
        SCTP_UNLOCK( TL_buflist_lock );

        ret = sctp_deinit_db_module();

        for( i = 0; i < SOC_MGR_MAX_CONNECTIONS; ++i )
        {
            sigtran_mutex_deinit( &( g_ipc_mgr_data[init_options->capability.stack_type].ipc_conn_info.conn_info[i].fd_mutex ) );
        }

        /*Fix Bug-Id 3 ends*/
        SCTP_DESTROY_LOCK(TL_buflist_lock); /* Fix for memleak */
        return ( SCTP_FAILURE );
    }

#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

    /* After initialization stack is STOPPED and STANDBY */
    sctp_stack_red_state = SCTP_STACK_RED_STATE_STANDBY;
    sctp_stack_proc_state = SCTP_PROC_STATE_STOP;

#endif /* SCTP_CLI_RECV_COM_THREAD */
#endif /* SCTP_UI_MESSAGEBASED */
#endif /* SCTP_RED_SUPPORT */


    /* Initialise the Protocol State to ACTIVE */
    sctp_stack_state = SCTP_STATE_ACTIVE;
    g_dummy_sock = 0;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_get_stack_entity_version
 *
 *    DESCRIPTION:
 *        This function gets the version information.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_api_get_stack_entity_version(
    sctp_ver_info_st    *ver_info )
{
    ver_info->major_ver    = SCTP_API_VERSION_NO;
    ver_info->minor_ver    = SCTP_MINOR_VERSION_NO;

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_config_stack_addresses
 *
 *    DESCRIPTION:
 *        This function is used to configure the stack address
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_config_stack_addresses(
    sctp_bindx_flag_et  operation,
    sctp_U32bit         naddr,
    sctp_sockaddr_st    *p_addrlist,
    sctp_error_t        *errorno )
{
    sctp_return_t    ret = SCTP_FAILURE;

    if( errorno == NULL || p_addrlist == NULL )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *errorno = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "In Function sctp_api_config_stack_addresses" ) );

    SCTPLOCK();

    ret = sctp_db_validate_config_stack_address( operation, naddr,
                                                 p_addrlist, errorno );

    if( SCTP_FAILURE == ret )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_api_config_stack_addresses:Invalid Addresses" ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTPUNLOCK();

        return SCTP_FAILURE;
    }

    ret = sctp_eshell_config_address( operation, naddr, p_addrlist,
                                      errorno );

    if( SCTP_FAILURE == ret )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "sctp_api_config_stack_addresses:Invalid Addresses" ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTPUNLOCK();

        return SCTP_FAILURE;
    }

    /*everythings fine, now add the addresses to stack list */
    if( SCTP_BINDX_ADD_ADDR == operation )
    {
        sctp_db_add_stack_address( naddr, p_addrlist );
    }

    else
    {
        sctp_db_del_stack_address( naddr, p_addrlist );
    }

    SCTPUNLOCK();

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_deinit_stack
 *
 *    DESCRIPTION:
 *        This function deinits the stack
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_deinit_stack(
    sctp_Boolean_t       forced,
    sctp_error_t         *p_ecode )
{
    sctp_U32bit counter     = 0;
    sctp_tcb_st *p_curr_tcb = SCTP_NULL;
    sctp_tcb_st *p_temp     = SCTP_NULL;
    sctp_ep_st  *p_ep       = SCTP_NULL;
    sctp_U32bit assoc_id    = 0;
    sctp_U32bit sctp_index       = 0;
    sig_stack_type_et       stack_type;

#ifndef SCTP_CLI_RECV_COM_THREAD
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_RECV_STACK" ) );
    stack_type = SIG_SCTP_RECV_STACK;
#else
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStack type SIG_SCTP_STACK" ) );
    stack_type = SIG_SCTP_STACK;
#endif

    if( ( p_ecode == NULL ) )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "DEINIT API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "In Function sctp_api_deinit_stack" ) );

    SCTPLOCK();

    if( ( 0 != sctp_db_main.num_active_ep ) && ( SCTP_FALSE == forced ) )
    {
        *p_ecode = E_ASSOC_EXISTS;
        SCTPUNLOCK();
        return ( SCTP_FAILURE );
    }

    sctp_stack_state = SCTP_STATE_UNINTIALISED;
    SCTPUNLOCK();

    for( counter = 0; counter < sctp_db_main.max_ep; counter++ )
    {
        assoc_id = ( counter + sctp_db_main.last_allocated_ep + 1 ) %
                   sctp_db_main.max_ep;

        if( 1 == SCTP_SOCK_IS_IN_USE( sctp_db_main.ep_table[assoc_id].flags ) )
        {
            p_ep = &sctp_db_main.ep_table[assoc_id];

            /* Lock the Endpoint */
            SCTP_LOCK_EP( assoc_id );

            p_curr_tcb = ( sctp_tcb_st * )lstFirst( &( p_ep->assoc_list ) );

            if( SCTP_NULL != p_curr_tcb )
            {
                /* If any association then call state machine function for all */
                while( p_curr_tcb )
                {
                    p_temp = ( sctp_tcb_st * )lstNext( ( NODE * )p_curr_tcb );
                    p_curr_tcb->event.cmd_abort.cause_length = 0;
                    sctp_assoc_state_machine( p_curr_tcb, SCTPEVENT_CMDABORT );
                    p_curr_tcb = p_temp;
                }
            }

            else
            {
                /* if no assoc simply delete the ep */
                sctp_db_delete_ep( assoc_id, p_curr_tcb, SCTP_TRUE );
            }

            SCTP_UNLOCK_EP( assoc_id );
        }
    }

    if( ( SCTP_IS_STACK_INET ) || ( SCTP_IS_STACK_INET6 ) )
    {
        sig_close_comm_channel( stack_type,
                                ( sctp_U16bit ) sctp_eshell_info.receive.sock.ipV4 );
        sig_close_comm_channel( stack_type,
                                ( sctp_U16bit )sctp_eshell_info.send.ipV4 );
        sig_close_comm_channel( stack_type,
                                ( sctp_U16bit )sctp_eshell_info.receive.sock.icmpV4 );
#ifdef SCTP_INET6_SUPPORT

        if( SCTP_IS_STACK_INET6 )
        {
            sig_close_comm_channel( stack_type, sctp_eshell_info.send.ipV6 );

            /* Changes for VXWORKS-6.9 IPV6 DE-INIT issue S*/
            for( sctp_index = 0; sctp_index < sctp_eshell_info.receive.num_v6_addrs;
                    sctp_index++ )
            {
                sig_close_comm_channel( stack_type,
                                        ( sctp_U16bit )sctp_eshell_info.receive.sock.ipV6[sctp_index] );
            }

            /* Changes for VXWORKS-6.9 IPV6 DE-INIT issue E*/
        }

#endif
    }

    else
    {
        for( sctp_index = 0; sctp_index < sctp_eshell_info.receive.num_v4_addrs; sctp_index++ )
        {
            sig_close_comm_channel( stack_type,
                                    ( sctp_U16bit )sctp_eshell_info.receive.udp_socket_id[sctp_index] );
        }
    }

#if !defined(SCTP_UI_SU_MESSAGEBASED) && !defined(SCTP_UI_SOCK_API)
#ifndef SCTP_NO_QUEUE
    sctp_su_deinit_notification_queue();
#endif
#endif

#ifdef SCTP_NAME_RESOLV_THREAD
    /* Initialize the name resolver */
    sctp_hostname_resolver_deinit();

#endif

    /* sctp_deinit_eshell(); */

    SCTP_LOCK( TL_buflist_lock );
    __FREE( TL_free_pool );
    SCTP_UNLOCK( TL_buflist_lock );

    sctp_deinit_pmtu_database();
    /* Initilize the TL buffer list lock */
    SCTP_DESTROY_LOCK( TL_buflist_lock );

#ifdef SCTP_RED_SUPPORT

    sctp_deinit_red_module();

#endif /* SCTP_RED_SUPPORT */

    /* Initialise db with parameters passed by SM */
    sctp_deinit_db_module();

    sctp_deinit_timer_module();

    return ( SCTP_SUCCESS );
}

#if defined(SCTP_STATS_ENABLED) && defined(SCTP_ASSOC_STATS_RECOVERY_SUPPORT)
sctp_void_t sctp_stk_api_stats_recovery_ind(
    sctp_U32bit association_id,
    sctp_assoc_stats_st  *assoc_stats )
{
    sctp_api_stats_recovery_ind( association_id, assoc_stats );
}
#endif

/*Bug ID 68 changes s*/
/***************************************************************************
 *    FUNCTION :
 *        sctp_api_set_reset_abort_flag
 *
 *    DESCRIPTION:
 *    This function set/reset global variable stop_abort according
 *    to the value passed to the parameter 'stop' of this function.
 *    RETURN VALUE:
 *
 **************************************************************************/
sctp_void_t
sctp_api_set_reset_abort_flag(
    sctp_Boolean_t       stop )
{
    stop_abort = stop;
}
/*Bug ID 68 changes e*/

