/******************************************************************************
 *  FILE NAME:  s_clsmap.c
 *
 *  DESCRIPTION:
 *  This file is part of SM client build of SCTP stack. Here we do
 *  the static parsing of API structure called by the client code.
 *
 *  DATE        NAME       REFERENCE       REASON
 *  -----------------------------------------------------
 *
 *  23March 2001    Vakul Garg      -      .Original Creation
 *  12Sep   2001    Yogesh Gahlaut         Rel 3.0 Changes
 *  02Jan02     hbhondwe  SCTP_IPC_TCP     tcp changes for ipc
 *  19Sep02      Manish Rajpal             Rel 5.0
 *  27Now03          Vivek Bansal          CSPL Changes
 *  4Feb05       Amit Ray                  Rel 5.0.4
 *  09May08      Raza      REL 6.2         SPR 18527
 *  15Sep08      Raza      REL 6.2         Redundancy Changes.
 *  14july10     Sharad Kumar Yadav        Rel 6.2.0 SPR# 20526 For CSR# 1-7670333
 *  18Feb13    Rohan Pawa   SPR 21107    CSR 24680
 *  1May13     Pranav Sharma SPR 21107     CSR 44034
 *  1May13      Pranav    SPR 21127       CSR 40731-GCC 4.7 warning removal
 *  1July13     Rohan Pawa SPR 21141       CSR 50452
 *  25Feb14     Rohan Pawa                 CSR 69326
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

/*
 * any restriction on send API, max buffer sise.. there should be .. more
 * than 4500 bytes not allowed
 */
#define  SCTP_MODULE "CL-SM"
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_clco.h>
#include <s_clsmap.h>
#include <s_cobuf.h>
#include <s_clessm.h>
#include <s_dbg.h>
#include <s_smipc.h>
#include <s_closmemsm.h>


/******* *  forward declarations    *************/


static sctp_U8bit *
sctp_build_api_init_stack(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_entity_version(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_config_stack_params(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_init_stats(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_enable_stats(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_disable_stats(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_stats(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_error_level(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_set_error_level(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_enable_assoc_trc(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_disable_assoc_trc(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_trace_info(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_config_stack_addrs(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_deinit_stack(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

/* Start SPR 20526 */
static sctp_U8bit *
sctp_build_api_get_rtt_info(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );
/* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT

static sctp_U8bit *
sctp_build_api_set_stack_red_state(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_set_stack_proc_state(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_stack_red_db(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_set_stack_red_db(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_assoc_red_db(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_set_assoc_red_db(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

static sctp_U8bit *
sctp_build_api_get_stack_health(
    sctp_U8bit      *p_buffer,
    sctp_Pvoid_t    p_elements );

#endif    /*SCTP_RED_SUPPORT*/



sctp_U8bit  *
( *sctp_sm_client_build_func_tbl[SCTP_NUM_SMREQ_API] )
( sctp_U8bit *, sctp_Pvoid_t ) =
{
    sctp_build_api_init_stack,          /* init stack */
    sctp_build_api_get_entity_version,  /* get entity version */
    sctp_build_api_config_stack_params, /* config stack  */

    sctp_build_api_init_stats,          /* init statistics */
    sctp_build_api_enable_stats,        /* enable statistics */
    sctp_build_api_disable_stats,       /* disable statistics */
    sctp_build_api_get_stats,           /* get statistics  */

    sctp_build_api_get_error_level,     /*get error level */
    sctp_build_api_set_error_level,     /*set error level */

    sctp_build_api_enable_assoc_trc,    /* Enable assoc trace */
    sctp_build_api_disable_assoc_trc,
    sctp_build_api_get_trace_info,

    sctp_build_api_config_stack_addrs,
    sctp_build_api_deinit_stack,
    /* Start SPR 20526 */
    sctp_build_api_get_rtt_info,
    /* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT

    sctp_build_api_set_stack_red_state,
    sctp_build_api_set_stack_proc_state,
    sctp_build_api_get_stack_red_db,
    sctp_build_api_set_stack_red_db,
    sctp_build_api_get_assoc_red_db,
    sctp_build_api_set_assoc_red_db,
    sctp_build_api_get_stack_health

#endif    /*SCTP_RED_SUPPORT*/

};

/*********** * functions for processing resp apis *****************/

sctp_void_t
sctp_sm_process_stack_buffer(
    sctp_U8bit *p_buffer,
    sctp_len_t nooctets );

static sctp_return_t
sctp_sm_proc_api_init_stack_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_entity_version_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_config_stack_params_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_init_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_enable_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_disable_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_err_level_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_set_err_level_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_enable_assoc_trc_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_disable_assoc_trc_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_trc_info_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_config_stack_addrs_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_deinit_stack_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_rtt_info_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

#ifdef SCTP_RED_SUPPORT


static sctp_return_t
sctp_sm_proc_api_set_stack_red_state_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_set_stack_proc_state_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_stack_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_set_stack_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_assoc_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_set_assoc_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );

static sctp_return_t
sctp_sm_proc_api_get_stack_health_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len );


#endif    /*SCTP_RED_SUPPORT*/


sctp_return_t
( *sctp_sm_client_proc_resp_api_tbl[SCTP_NUM_SMRESP_API] )
( sctp_U8bit *, sctp_len_t ) =
{
    sctp_sm_proc_api_init_stack_resp,
    sctp_sm_proc_api_get_entity_version_resp,
    sctp_sm_proc_api_config_stack_params_resp,

    sctp_sm_proc_api_init_stats_resp,
    sctp_sm_proc_api_enable_stats_resp,
    sctp_sm_proc_api_disable_stats_resp,
    sctp_sm_proc_api_get_stats_resp,

    sctp_sm_proc_api_get_err_level_resp,
    sctp_sm_proc_api_set_err_level_resp,

    sctp_sm_proc_api_enable_assoc_trc_resp,
    sctp_sm_proc_api_disable_assoc_trc_resp,
    sctp_sm_proc_api_get_trc_info_resp,

    sctp_sm_proc_api_config_stack_addrs_resp,
    sctp_sm_proc_api_deinit_stack_resp,
    /* Start SPR 20526 */
    sctp_sm_proc_api_get_rtt_info_resp,
    /* End SPR 20526 */
#ifdef SCTP_RED_SUPPORT
    sctp_sm_proc_api_set_stack_red_state_resp,
    sctp_sm_proc_api_set_stack_proc_state_resp,
    sctp_sm_proc_api_get_stack_red_db_resp,
    sctp_sm_proc_api_set_stack_red_db_resp,
    sctp_sm_proc_api_get_assoc_red_db_resp,
    sctp_sm_proc_api_set_assoc_red_db_resp,
    sctp_sm_proc_api_get_stack_health_resp
#endif    /*SCTP_RED_SUPPORT*/
};

/***************************************************************************
 *  FUNCTION : send_to_sctp_sm
 *
 *  DESCRIPTION:
 *    This function is the main API call function from client
 *    code and it does the possible static parsing on any API
 *    call and then calls the dispatch buffer to send it to
 *    the core stack.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 *************************************************************************/
sctp_return_t send_to_sctp_sm(
    sctp_trans_id_t   transaction_id,
    sctp_api_id_t     api_id,
    sctp_src_id_t     src_id,
    sctp_dest_id_t    dest_id,
    sctp_Pvoid_t      p_elements,
    sctp_Error_t      *p_ecode )
{
    sctp_U16bit         index_api_tbl;
    sctp_U32bit         noctets;
    sctp_return_t       ret;

    /*sm_buffer is global buffer malloced at the time of init*/
    sctp_U8bit     *p_sm_buffer = sctp_client_sm_get_mesg_buffer( SCTP_MAX_API_SIZE );
    sctp_U8bit     *p_buffer = p_sm_buffer;

    /* Update for SPR : 18527 Starts Here */
    if( p_buffer == SCTP_NULL )
    {
        SCTP_CLIENT_TRACE( ( "send_to_sctp_sm :: ERROR : sctp_client_sm_get_mesg_buffer(%d) Failed",
                             SCTP_MAX_API_SIZE ) );
        *p_ecode = E_NORESOURCES;
        return SCTP_FAILURE;
    }

    /* Update for SPR : 18527 Ends Here */

    __MEMSET( p_buffer, 0, SCTP_MAX_API_SIZE );

    /* check if API is valid  */
    if( ( api_id < ( SCTP_MIN_SMREQ_APIID ) ) || ( api_id > SCTP_MAX_SMREQ_APIID ) )
    {
        *p_ecode = SCTP_INVALID_API_ID;
        /*SPR 21141 changes starts*/
        __FREE( p_buffer );
        /*SPR 21141 changes ends*/
        return ( SCTP_FAILURE );
    }

    /* Do I need to check for valid src and dest id */
#ifdef SCTP_RED_SUPPORT

    if( ( p_elements == SCTP_NULL )
            && ( api_id != SCTP_API_GET_ENTITY_VERSION_REQ )
            && ( api_id != SCTP_API_GET_TRACE_INFO_REQ )
            && ( api_id != SCTP_API_GET_STACK_HEALTH_REQ )
            && ( api_id != SCTP_API_GET_STACK_RED_DB_REQ ) )
    {

        *p_ecode = SCTP_INVALID_API_ID;
        /*SPR 21141 changes starts*/
        __FREE( p_buffer );
        /*SPR 21141 changes ends*/

        return ( SCTP_FAILURE );
    }

#else

    if( ( p_elements == SCTP_NULL )
            && ( api_id != SCTP_API_GET_ENTITY_VERSION_REQ )
            && ( api_id != SCTP_API_GET_TRACE_INFO_REQ ) )
    {

        *p_ecode = SCTP_INVALID_API_ID;
        /* SPR 21107 changess */
        __FREE( p_buffer );
        /* SPR 21107 changess */

        return ( SCTP_FAILURE );
    }

#endif /*SCTP_RED_SUPPORT*/

    index_api_tbl = api_id - SCTP_MIN_SMREQ_APIID;

    /*
     * if a valid api buffer build api buffer and  send the buffer to
     * core stack
     */

    p_buffer = sctp_build_api_header( p_buffer, api_id, transaction_id,
                                      src_id, dest_id );

    /* CSR 69326 fix starts */
    if( p_elements == SCTP_NULL )
    {
        *p_ecode = E_INVALID_API_POINTER;
        __FREE( p_buffer );
        return ( SCTP_FAILURE );
    }

    /* CSR 69326 fix ends */

    p_buffer = ( *sctp_sm_client_build_func_tbl[index_api_tbl] )( p_buffer,
                                                                  p_elements );

    noctets = p_buffer - p_sm_buffer;

    /* fill the api length field */
    p_buffer = p_sm_buffer;

    sctp_buffer_set_UlongAtOffset( p_buffer,
                                   APIBUF_LENGTH_OFFSET, ( sctp_U32bit ) noctets );
    /* set p_buffer to point to the first element for dispatch */
    p_buffer = p_sm_buffer;

    ret = sctp_sm_dispatch_buffer( p_buffer, noctets, src_id, dest_id, p_ecode );

    if( SCTP_FAILURE == ret )
    {
        sctp_client_sm_free_mesg_buffer( &p_buffer );
    }

    return ( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_init_stack
 *
 *  DESCRIPTION:
 *   This function build the init_stack API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_init_stack(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_init_stack_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_U8bit  hostname_len, count;
    sctp_sockaddr_st       *p_addr_list;

    sctp_msg_init_stack_st *p_init_stack =
        ( sctp_msg_init_stack_st * ) p_elements;

    /* max_endpoint */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->max_endpoint );

    /* max_total_assoc */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->max_total_assoc );

    /* max_streams_per_assoc */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_init_stack->max_streams_per_assoc );

    /* num_tx_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->num_tx_min_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->num_tx_med_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->num_tx_max_buffers );

    /* num_rx_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->num_rx_min_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->num_rx_med_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->num_rx_max_buffers );

    /* no_of_local_addrs */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->no_of_local_addrs );

    /* seed */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->seed );

    /* reserve_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->min_reserve_buffers );

    /* reserve_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->med_reserve_buffers );

    /* reserve_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->max_reserve_buffers );

    /* stack_type */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->stack_type );

    /* pmtu_capable */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->pmtu_capable );

    /* ecn_capable */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->ecn_capable );

    /* nat_capable */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->nat_capable );

    /* checksum */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stack->checksum );

    p_addr_list = ( sctp_sockaddr_st * )( p_init_stack->list_local_addrs );

    for( count = 0; count < p_init_stack->no_of_local_addrs; count++ )
    {
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &( p_addr_list[count] ) );
    }

    /* udp_port */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_init_stack->standard_udp_port );

    /* max_total_applications */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_init_stack->max_total_appl );

    /* stale_cookie_policy */
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_init_stack->stale_cookie_policy );

    /* ack_policy */
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_init_stack->ack_policy );

    hostname_len = strlen( ( const char * ) & ( p_init_stack->hostname[0] ) );

    if( hostname_len != 0 && hostname_len < SCTP_MAX_HOSTNAME_LEN )
    {
        for( count = 0; count < hostname_len; count++ )
        {
            p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                              p_init_stack->hostname[count] );
        }

    }

    p_buffer = sctp_buffer_set_Uchar( p_buffer, ( sctp_U8bit )'\0' );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION : sctp_build_api_get_entity_version
 *
 *  DESCRIPTION:
 *   This function build API header to be sent across to stack code
 *   API elements are passed to this function and it convert them into
 *   buffer format.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_get_entity_version(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_elements );
    /* SPR 21107 changes end */
    p_elements = SCTP_NULL;
    return p_buffer;

}


/***************************************************************************
 *  FUNCTION :  sctp_build_api_config_stack_params
 *
 *  DESCRIPTION:
 *   This function build the API of configuring the stack parameters
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_config_stack_params(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_config_stack_params_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_config_stack_params_st *p_config_stack =
        ( sctp_msg_config_stack_params_st * ) p_elements;

    /* rto init */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->rto_init );

    /* rto min */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->rto_min );

    /* rto max */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->rto_max );

    /* valid_cookie_life */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->valid_cookie_life );

    /* assoc_max_retrans */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->assoc_max_retrans );

    /* path_max_retrans */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->path_max_retrans );

    /* max_init_retrans */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->max_init_retrans );

    /* max_init_path_retrans */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->max_init_path_retrans );

    /* init_timeo */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->init_timeo );

    /* heartbeat_interval */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_config_stack->heartbeat_interval );

    /* hb_info_length */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_config_stack->hb_info_length );

    /* rto_alpha_percent */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_config_stack->rto_alpha_percent );

    /* rto_beta_percent */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_config_stack->rto_beta_percent );

    /* num_in_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_config_stack->num_in_streams );
    /* num_out_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_config_stack->num_out_streams );

    /* bundling time */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->bundling_time );

    /* sack_period time */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->sack_period );

    /* sack_frequency */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->sack_frequency );

    /* pmtu capable*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->pmtu_capable );

    /* mtu_size */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_config_stack->mtu_size );

#ifdef SCTP_IP_HEADER_PARAMS
    /* ttl */
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_stack->ip_header_params.ttl );
    /* toa */
    p_buffer = sctp_buffer_set_Uchar( p_buffer, p_config_stack->ip_header_params.tos );

#ifdef SCTP_INET6_SUPPORT
    /* hoplimit */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_config_stack->ip_header_params.hoplimit );
    /* tclass */
    p_buffer = sctp_buffer_set_Ushort( p_buffer, p_config_stack->ip_header_params.tclass );

#endif  /*SCTP_INET6_SUPPORT */

#endif  /* SCTP_IP_HEADER_PARAMS */

    /* HB.Max.Burst */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config_stack->hb_max_burst );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}


/***************************************************************************
 *  FUNCTION : sctp_build_api_init_stats
 *
 *  DESCRIPTION:
 *   This function build the buffer for init stats API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_init_stats(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_init_stats_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_init_stats_st *p_init_stats = ( sctp_msg_init_stats_st * )p_elements;

    /* stats_type*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_init_stats->stats_type );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_enable_stats
 *
 *  DESCRIPTION:
 *   This function build the enable stats API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_enable_stats(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_enable_stats_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_enable_stats_st *p_enable_stats =
        ( sctp_msg_enable_stats_st * )p_elements;

    /* stats_type*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_enable_stats->stats_type );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *    FUNCTION : sctp_build_api_disable_stats
 *
 *  DESCRIPTION:
 *   This function build the disable stats API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_disable_stats(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_disable_stats_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_disable_stats_st *p_disable_stats =
        ( sctp_msg_disable_stats_st * )p_elements;

    /* stats_type*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_disable_stats->stats_type );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_get_stats
 *
 *  DESCRIPTION:
 *   This function build the get_stats API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_get_stats(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_stats_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_get_stats_st *p_get_stats = ( sctp_msg_get_stats_st * ) p_elements;

    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_stats->association_id );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_stats->stats_type );
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_stats->reset_flag );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION : sctp_build_api_get_error_level
 *
 *  DESCRIPTION:
 *   This function build the get_error_level API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_get_error_level(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_error_level_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_get_error_level_st *p_get_err_level =
        ( sctp_msg_get_error_level_st * )p_elements;

    /* error flag*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_err_level->error_flag );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION :  sctp_build_api_set_error_level
 *
 *  DESCRIPTION:
 *   This function build the set_error_level API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_set_error_level(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements ) /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_error_level_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_set_error_level_st *p_set_err_level =
        ( sctp_msg_set_error_level_st * )p_elements;

    /* error flag*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_set_err_level->error_flag );

    /* error level*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_set_err_level->error_level );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}


/***************************************************************************
 *  FUNCTION : sctp_build_api_enable_assoc_trc
 *
 *  DESCRIPTION:
 *   This function build the enable_assoc_trc API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_enable_assoc_trc(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_enable_assoc_trc_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_enable_assoc_trc_st *p_enable_trc =
        ( sctp_msg_enable_assoc_trc_st * ) p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_enable_trc->assoc_id );


    /* trace_flag */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_enable_trc->trace_flag );


#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION : sctp_build_api_disable_assoc_trc
 *
 *  DESCRIPTION:
 *   This function build the disable_assoc_trc API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_disable_assoc_trc(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_disable_assoc_trc_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_disable_assoc_trc_st *p_disable_trc =
        ( sctp_msg_disable_assoc_trc_st * ) p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_disable_trc->assoc_id );

    /*trace_flag */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_disable_trc->trace_flag );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;
}

/***************************************************************************
 *  FUNCTION : sctp_build_api_get_trace_info
 *
 *  DESCRIPTION:
 *   This function build the get_trace_info API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_get_trace_info(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_trace_info_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_get_trace_info_st *p_get_trace_info =
        ( sctp_msg_get_trace_info_st * ) p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_trace_info->assoc_id );


#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}



/***************************************************************************
 *  FUNCTION : sctp_build_api_config_stack_addrs
 *
 *  DESCRIPTION:
 *   This function build the config_stack_addrs API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_config_stack_addrs(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_config_stack_addrs_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_config_stack_addrs_st *p_config =
        ( sctp_msg_config_stack_addrs_st * ) p_elements;
    sctp_U32bit count = 0;

    p_buffer = sctp_buffer_set_Ulong( p_buffer, ( sctp_U16bit )p_config->flags );
    /* number of su address */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_config->no_of_addrs );

    for( count = 0; count < p_config->no_of_addrs; count++ )
    {
        /* Address */
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &p_config->list_addrs[count] );
    }

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}


/***************************************************************************
 *  FUNCTION : sctp_build_api_deinit_stack
 *
 *  DESCRIPTION:
 *   This function build the deinit_stack API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_deinit_stack(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_deinit_stack_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_deinit_stack_st *p_config =
        ( sctp_msg_deinit_stack_st * ) p_elements;

    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_config->force );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/* Start SPR 20526 */

/***************************************************************************
 *  FUNCTION :  sctp_build_api_get_rtt_info
 *
 *  DESCRIPTION:
 *   This function build the get_rtt_info API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_rtt_info(
    sctp_U8bit    *p_buffer,   /* pointer to buffer  */
    sctp_Pvoid_t  p_elements ) /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_rtt_info_st ), ( sctp_U8bit * )p_elements );
#else

    sctp_msg_get_rtt_info_st *p_get_rtt_info  =
        ( sctp_msg_get_rtt_info_st * )p_elements;

    /* assciation Id */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_get_rtt_info->association_id );
    /* reset flag */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_get_rtt_info->reset_flag );


#endif /* SCTP_STRUCT_BASED */
    return p_buffer;
}
/* Ends SPR 20526 */


#ifdef SCTP_RED_SUPPORT


/***************************************************************************
 *    FUNCTION : sctp_build_api_set_stack_red_state
 *
 *  DESCRIPTION:
 *   This function build the set_stack_red_state API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_set_stack_red_state(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_stack_red_state_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_set_stack_red_state_st *p_set_red_state =
        ( sctp_msg_set_stack_red_state_st * )p_elements;

    /* stack state*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_set_red_state->stack_state );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *  FUNCTION : sctp_build_api_set_stack_proc_state
 *
 *  DESCRIPTION:
 *   This function build the set_stack_proc_state API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_set_stack_proc_state(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_stack_proc_state_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_set_stack_proc_state_st *p_set_proc_state =
        ( sctp_msg_set_stack_proc_state_st * )p_elements;

    /* stack state*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_set_proc_state->stack_state );

    /* stack state*/
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_set_proc_state->stack_seed );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}


/***************************************************************************
 *  FUNCTION :  sctp_build_api_get_stack_red_db
 *
 *  DESCRIPTION:
 *   This function build the get_stack_red_db API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_get_stack_red_db(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{
    p_elements = NULL;
    /* SPR 21127 changes start */
    SCTP_UNUSED_VARIABLE( p_elements );
    /* SPR 21127 changes end */
    return p_buffer;

}

/***************************************************************************
 *  FUNCTION : sctp_build_api_set_stack_red_db
 *
 *  DESCRIPTION:
 *   This function build the set_stack_red_db API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *sctp_build_api_set_stack_red_db(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements )  /* pointer to api elements */
{

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_stack_red_db_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_set_stack_red_db_st *p_red_db =
        ( sctp_msg_set_stack_red_db_st * )p_elements;
    sctp_U32bit     count;

    /* stack red db*/

    /* max_appl */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_red_db->stack_red_db.max_appl );
    /* num_active_appl */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.num_active_appl );
    /* max_ep */
    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_red_db->stack_red_db.max_ep );
    /* num_active_ep */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.num_active_ep );
    /* last_allocated_ep */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.last_allocated_ep );
    /* max_assoc */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.max_assoc );
    /* tx_max_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.min_tx_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.med_tx_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.max_tx_buffers );
    /* rx_max_buffers */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.min_rx_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.med_rx_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.max_rx_buffers );

    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.min_reserve_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.med_reserve_buffers );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.max_reserve_buffers );
    /* Rel 6.2 Update Ends Here. */

    /* max_streams_per_assoc */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.max_streams_per_assoc );
    /* num_local_addrs */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.num_local_addrs );

    /* active_assoc_list */
    for( count = 0;
            count < p_red_db->stack_red_db.num_active_ep;
            count++ )
    {
        p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                          p_red_db->stack_red_db.active_ep_list[count] );
    }

    /* pmtu_timer_rem_time */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.pmtu_timer_rem_time );
    /* num_active_pmtu_nodes */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.num_active_pmtu_nodes );

    /* pmtu_database */
    for( count = 0;
            count < p_red_db->stack_red_db.num_active_pmtu_nodes;
            count++ )
    {
        p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                           sizeof( sctp_time_t ),
                                           ( sctp_U8bit * )&p_red_db->stack_red_db.
                                           pmtu_database[count].time_stamp );
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &p_red_db->stack_red_db.pmtu_database[count].addr );
        p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                           p_red_db->stack_red_db.pmtu_database[count].mtu_size );
        p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                           p_red_db->stack_red_db.pmtu_database[count].spare );
    }

#ifdef SCTP_ERROR_ENABLED

    /* sctp_error_table */
    for( count = SCTP_MIN_ERROR_FLAG; count <= SCTP_MAX_ERROR_FLAG; count++ )
    {
        p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                          ( sctp_U32bit )p_red_db->stack_red_db.sctp_error_table[count] );
    }

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_STATS_ENABLED

    /* sctp_stats_state */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_red_db->stack_red_db.sctp_stats_state );
    /* sctp_protocol_stats */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_protocol_stats.no_applications_reg );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_protocol_stats.no_associations_open );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_protocol_stats.no_dest_marked_active );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_protocol_stats.no_dest_marked_inactive );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_protocol_stats.no_endpoints_marked_up );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_protocol_stats.no_endpoints_marked_down );

    /* sctp_global_traffic_stats */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_assoc_accepted );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_assoc_initiated );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_graceful_shutdown_assocs );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_ungraceful_shutdown_assocs );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_trans_data_chunks );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_retrans_data_chunks );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_data_chunks_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_ord_data_chunks_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_unord_data_chunks_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_data_chunks_unacked );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_data_chunks_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_ord_data_chunks_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_unord_data_chunks_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_bytes_sent_ok );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_bytes_recvd_ok );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_init_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_init_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_initack_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_initack_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_cookie_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_cookie_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_cookieack_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_cookieack_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_hb_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_hb_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_hback_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_hback_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_sack_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_sack_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_abort_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_abort_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_shut_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_shut_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_shutack_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_shutack_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_shutcom_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_shutcom_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_error_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_error_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_ecne_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_ecne_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_cwr_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_cwr_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.
                                      no_datagrams_recvd_in_error );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_chk_sum_err_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_ootb_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_frag_usr_msg );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_reassemble_usr_msg );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_packets_sent );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_packets_recv );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Buffer( p_buffer, sizeof( sctp_time_t ),
                                       ( sctp_U8bit * )&p_red_db->stack_red_db.
                                       sctp_global_traffic_stats.discont_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_asconf_sent );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_asconf_recv );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_asconfack_sent );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_global_traffic_stats.no_asconfack_recv );
#endif
    /* Rel 6.2 Update Ends Here. */
    /* sctp_error_stats */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_error_stats.API_error_stats_count );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_error_stats.protcol_error_stats_count );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_error_stats.system_error_stats_count );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_error_stats.
                                      system_resource_error_stats_count );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_error_stats.internal_error_stats_count );

    /* sctp_stats_table */
    for( count = 0; count < SCTP_ALL_STATS; count++ )
    {
        p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                          ( sctp_U32bit )p_red_db->stack_red_db.sctp_stats_table[count] );
    }


#endif /* SCTP_STATS_ENABLED */

    /* active_appl_list */
    for( count = 0; count < p_red_db->stack_red_db.max_appl; count++ )
    {
        p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                          p_red_db->stack_red_db.active_appl_list[count].open );
        p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                           p_red_db->stack_red_db.active_appl_list[count].su_id );
    }

    /* addr_list */
    for( count = 0;
            count < p_red_db->stack_red_db.num_local_addrs;
            count++ )
    {
        p_buffer = sctp_buffer_pack_sockaddr( p_buffer,
                                              &( p_red_db->stack_red_db.addr_list[count] ) );
    }

    /* sctp_def_prot_params */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.rto_init );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.rto_min );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.rto_max );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.max_burst );
    /* Rel 6.2 Update Ends Here. */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.valid_cookie_life );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.assoc_max_retrans );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.hb_interval );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.path_max_retrans );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.max_init_retrans );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.max_init_path_retrans );
    /* Rel 6.2 Update Ends Here. */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.init_timeo );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.bundling_time );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.sack_period );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.sack_frequency );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.adaption_ind );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.maxseg );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.hb_info_length );
    /* Rel 6.2 Update Ends Here. */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.mtu_size );
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.rto_alpha_percent );
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.rto_beta_percent );
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.num_in_streams );
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.num_out_streams );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.re_use_addr );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.dont_fragment );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.want_mapped );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.nat_capable );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.pmtu_capable );
    /* Rel 6.2 Update Ends Here. */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.allow_asconf );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.checksum_func );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.ppi );

    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.hb_max_burst );

#ifdef SCTP_IP_HEADER_PARAMS
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.ip_header_params.ttl );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.sctp_def_prot_params.ip_header_params.tos );
#ifdef SCTP_INET6_SUPPORT
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.ip_header_params.hoplimit );
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.sctp_def_prot_params.ip_header_params.tclass );
#endif
#endif

    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.high_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.high_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.med_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.med_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.low_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.low_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.no_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.trx_cong_reg_def.no_cong_lvl_abate );

    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.high_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.high_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.med_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.med_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.low_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.low_cong_lvl_abate );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.no_cong_lvl_onset );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.rcv_cong_reg_def.no_cong_lvl_abate );

    /* Rel 6.2 Update Ends Here */


    /* sctp_stack_init_parameters */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_red_db->stack_red_db.
                                      sctp_stack_init_parameters.ecn_capable );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_red_db->stack_red_db.
                                      sctp_stack_init_parameters.pmtu_capable );
    /* Rel 6.2 Update */
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_red_db->stack_red_db.
                                      sctp_stack_init_parameters.nat_capable );
    /* Rel 6.2 Update Ends Here. */
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.sctp_stack_init_parameters.ack_policy );
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.sctp_stack_init_parameters.stale_cookie_policy );
    p_buffer = sctp_buffer_set_Ulong( p_buffer,
                                      ( sctp_U32bit )p_red_db->stack_red_db.
                                      sctp_stack_init_parameters.stack_type );

    /* init_in_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.init_in_streams );

    /* init_out_streams */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.init_out_streams );

    /* standard_udp_port */
    p_buffer = sctp_buffer_set_Ushort( p_buffer,
                                       p_red_db->stack_red_db.standard_udp_port );

#ifdef SCTP_ERROR_ENABLED
    /* sctp_error_state */
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.sctp_error_state );

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_DEBUG_ENABLED
    /* sctp_gbl_trc */
    p_buffer = sctp_buffer_set_Uchar( p_buffer,
                                      p_red_db->stack_red_db.sctp_gbl_trc );
#endif /* SCTP_DEBUG_ENABLED */

    /*Old mac keys*/
    p_buffer = sctp_buffer_set_Buffer( p_buffer, SCTP_KEYSIZE,
                                       &p_red_db->stack_red_db.old_mac_key[0] );

    /*Current mac keys*/
    p_buffer = sctp_buffer_set_Buffer( p_buffer, SCTP_KEYSIZE,
                                       &p_red_db->stack_red_db.current_mac_key[0] );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}

/***************************************************************************
 *    FUNCTION :  sctp_build_api_get_assoc_red_db
 *
 *  DESCRIPTION:
 *   This function build the get_assoc_red_db API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_assoc_red_db(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements ) /* pointer to api elements */
{
#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_get_assoc_red_db_st ), ( sctp_U8bit * )p_elements );

#else

    sctp_msg_get_assoc_red_db_st *p_get_red_db =
        ( sctp_msg_get_assoc_red_db_st * )p_elements;

    /* association id*/

    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_get_red_db->association_id );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}


/***************************************************************************
 *    FUNCTION :  sctp_build_api_set_assoc_red_db
 *
 *  DESCRIPTION:
 *   This function build the set_assoc_red_db API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_set_assoc_red_db(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements ) /* pointer to api elements */
{
    sctp_msg_set_assoc_red_db_st *p_red_db =
        ( sctp_msg_set_assoc_red_db_st * )p_elements;

#ifdef SCTP_STRUCT_BASED

    p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                       sizeof( sctp_msg_set_assoc_red_db_st ) - sizeof( sctp_U8bit * ),
                                       ( sctp_U8bit * )p_elements );

    p_buffer = sctp_buffer_set_Buffer( p_buffer, p_red_db->len,
                                       p_red_db->assoc_red_db );

#else

    /* association id*/

    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_red_db->assoc_id );

    p_buffer = sctp_buffer_set_Ulong( p_buffer, p_red_db->len );

    p_buffer = sctp_buffer_set_Buffer( p_buffer, p_red_db->len,
                                       p_red_db->assoc_red_db );

#endif /* SCTP_STRUCT_BASED */

    return p_buffer;

}


/***************************************************************************
 *    FUNCTION :
 *   sctp_build_api_get_stack_health
 *
 *  DESCRIPTION:
 *   This function build the get_stack_health API
 *   API elements are passed to this function with the pointer to the buffer
 *   and adds to buffer pointer.
 *  RETURN VALUE:
 *   pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_build_api_get_stack_health(
    sctp_U8bit     *p_buffer,    /* pointer to buffer  */
    sctp_Pvoid_t   p_elements ) /* pointer to api elements */
{
    p_elements = SCTP_NULL;
    /* SPR 21127 changes start */
    SCTP_UNUSED_VARIABLE( p_elements );
    /* SPR 21127 changes end */
    return p_buffer;

}

#endif    /*SCTP_RED_SUPPORT*/



/*
 * Following are the functions to process apis recvd from the stack
 */

/***************************************************************************
 *  FUNCTION :  sctp_sm_process_stack_buffer
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *     void
 *
 **************************************************************************/
sctp_void_t
sctp_sm_process_stack_buffer(
    sctp_U8bit      *p_buffer,
    sctp_len_t      nooctets )
{
    sctp_api_id_t   api_id;
    sctp_U16bit     index_api_id;
    sctp_U16bit     req_len;

    /*
     * check if length > header
     */
    if( nooctets < SCTP_API_HEADER_LEN )
    {
        /* report error */
        return;
    }

    /* get the header fields */
    api_id = sctp_buffer_get_UshortAtOffset( p_buffer, 0 );
    req_len = ( sctp_U16bit ) sctp_buffer_get_UlongAtOffset( p_buffer, 12 );

    if( req_len != nooctets )
    {
        /* error */
        return;
    }

    if( ( api_id >= SCTP_MIN_SMRESP_APIID ) && ( api_id <= SCTP_MAX_SMRESP_APIID ) )
    {
        /* api is resp api */

        index_api_id = api_id - SCTP_MIN_SMRESP_APIID;
        ( *sctp_sm_client_proc_resp_api_tbl[index_api_id] )( p_buffer,
                                                             req_len );
    }

    return;
}



/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_init_stack_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_init_stack_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_init_stack_res_st init_stack_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_init_stack_resp " ) );

    __MEMSET( &init_stack_resp, 0, sizeof( init_stack_resp ) );

    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_init_stack_res_st ), ( sctp_U8bit * )&init_stack_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_stack_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_stack_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CL_REPORT( ( "\nAPI Response::Init Stack:  result:%d,  errorno:%d\n",
                      init_stack_resp.result, init_stack_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_INIT_STACK_RESP, &init_stack_resp );

    return ( ret );
}

/***************************************************************************
 *    FUNCTION : sctp_sm_proc_api_get_entity_version_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_entity_version_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_get_entity_version_res_st ver_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_entity_version_resp " ) );

    __MEMSET( &ver_resp, 0, sizeof( ver_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_entity_version_res_st ), ( sctp_U8bit * )&ver_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&ver_resp.result );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &ver_resp.major_ver );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &ver_resp.minor_ver );

#endif /* SCTP_STRUCT_BASED */


    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CL_REPORT( ( "\nAPI Response::Get Version: result: %d, major ver: %u,\
        minor ver: %u\n", ver_resp.result, ver_resp.major_ver,
                      ver_resp.minor_ver ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_ENTITY_VERSION_RESP, &ver_resp );

    return ( ret );
}

/***************************************************************************
 *    FUNCTION :  sctp_sm_proc_api_config_stack_params_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_config_stack_params_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_config_stack_params_res_st config_stack_params_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_msg_config_stack_params_res_st " ) );

    __MEMSET( &config_stack_params_resp, 0, sizeof( config_stack_params_resp ) );

    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_stack_params_res_st ),
                                           ( sctp_U8bit * )&config_stack_params_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_stack_params_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_stack_params_resp.errorno );


#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CL_REPORT( ( "\nAPI Response::Config Stack:  result:%d,  errorno:%d\n",
                      config_stack_params_resp.result, config_stack_params_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_CONFIG_STACK_PARAMS_RESP, &config_stack_params_resp );

    return ( ret );
}


/***************************************************************************
 *    FUNCTION :  sctp_sm_proc_api_init_stats_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_init_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_init_stats_res_st init_stats_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/


    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_init_stats_resp " ) );

    __MEMSET( &init_stats_resp, 0, sizeof( init_stats_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_init_stats_res_st ), ( sctp_U8bit * )&init_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_stats_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_stats_resp.errorno );


#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Init Stats: result:%d,errorno:%d\n",
                      init_stats_resp.result, init_stats_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_INIT_STATS_RESP, &init_stats_resp );

    return ( ret );
}

/***************************************************************************
 *    FUNCTION :   sctp_sm_proc_api_enable_stats_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_enable_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_enable_stats_res_st enable_stats_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_enable_stats_resp " ) );

    __MEMSET( &enable_stats_resp, 0, sizeof( enable_stats_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_enable_stats_res_st ), ( sctp_U8bit * )&enable_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&enable_stats_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&enable_stats_resp.errorno );


#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Enable Stats: result:%d,errorno:%d\n",
                      enable_stats_resp.result, enable_stats_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_ENABLE_STATS_RESP, &enable_stats_resp );

    return ( ret );
}

/***************************************************************************
 *    FUNCTION :  sctp_sm_proc_api_disable_stats_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_disable_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_disable_stats_res_st disable_stats_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_disable_stats_resp " ) );

    __MEMSET( &disable_stats_resp, 0, sizeof( disable_stats_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_disable_stats_res_st ),
                                           ( sctp_U8bit * )&disable_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&disable_stats_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&disable_stats_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Disable Stats: result:%d,errorno:%d\n",
                      disable_stats_resp.result, disable_stats_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_DISABLE_STATS_RESP, &disable_stats_resp );

    return ( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_get_stats_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_stats_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;
    sctp_U32bit     count;
    sctp_msg_get_stats_res_st get_stats_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_stats_resp " ) );

    __MEMSET( &get_stats_resp, 0, sizeof( get_stats_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_stats_res_st ), ( sctp_U8bit * )&get_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stats_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stats_resp.errorno );

    if( get_stats_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_stats_resp.association_id );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_stats_resp.stats_type );

        switch( get_stats_resp.stats_type )
        {
            case SCTP_PROT_INTERNAL_STATS:
                {
                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.prot_stats.no_applications_reg );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.prot_stats.
                                                          no_associations_open );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.prot_stats.
                                                          no_dest_marked_active );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.prot_stats.
                                                          no_dest_marked_inactive );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.prot_stats.
                                                          no_endpoints_marked_up );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.prot_stats.
                                                          no_endpoints_marked_down );

                    break;
                }

            case SCTP_GLOBAL_TRAFFIC_STATS:
                {
                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_assoc_accepted );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_assoc_initiated );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_graceful_shutdown_assocs );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_ungraceful_shutdown_assocs );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_trans_data_chunks );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_retrans_data_chunks );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_data_chunks_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_ord_data_chunks_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_unord_data_chunks_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_data_chunks_unacked );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_data_chunks_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_ord_data_chunks_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_unord_data_chunks_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_bytes_sent_ok );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_bytes_recvd_ok );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_init_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_init_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_initack_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_initack_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_cookie_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_cookie_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_cookieack_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_cookieack_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_hb_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_hb_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_hback_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_hback_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_sack_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_sack_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_abort_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_abort_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_shut_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_shut_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_shutack_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_shutack_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_shutcom_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_shutcom_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_error_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_error_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_ecne_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_ecne_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_cwr_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_cwr_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_datagrams_recvd_in_error );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_chk_sum_err_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_ootb_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_frag_usr_msg );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.
                                                          no_reassemble_usr_msg );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_packets_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_packets_recv );

                    /* CSR 1-4850698 */
                    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                           sizeof( sctp_time_t ),
                                                           ( sctp_U8bit * )&get_stats_resp.stats_data.traf_stats.
                                                           discont_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_asconf_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_asconf_recv );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_asconfack_sent );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.traf_stats.no_asconfack_recv );
#endif
                    /* CSR 1-4850698 end */

                    break;
                }

            case SCTP_ERROR_STATS:
                {
                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.error_stats.
                                                          API_error_stats_count );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.error_stats.
                                                          protcol_error_stats_count );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.error_stats.
                                                          system_error_stats_count );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.error_stats.
                                                          system_resource_error_stats_count );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.error_stats.
                                                          internal_error_stats_count );

                    break;
                }

            case SCTP_ASSOC_STATS:
                {
                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.assoc_id );

                    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                           sizeof( sctp_time_t ),
                                                           ( sctp_U8bit * )&get_stats_resp.stats_data.assoc_stats.
                                                           start_time );

                    /* CSR 1-4850698 */
                    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                           sizeof( sctp_time_t ),
                                                           ( sctp_U8bit * )&get_stats_resp.stats_data.assoc_stats.
                                                           discont_time );
                    /* CSR 1-4850698 end */

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.no_chunks_retrans );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.no_T1_expired );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.no_T2_expired );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.no_dest_address );

                    for( count = 0;
                            count < get_stats_resp.stats_data.assoc_stats.
                            no_dest_address;
                            count++ )
                    {
                        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                                    &get_stats_resp.stats_data.assoc_stats.
                                                                    dest_stats[count].dest_address );

                        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                              &get_stats_resp.stats_data.assoc_stats.
                                                              dest_stats[count].no_datagrams_sent );

                        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                              &get_stats_resp.stats_data.assoc_stats.
                                                              dest_stats[count].no_datagrams_unacked );

                        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                              &get_stats_resp.stats_data.assoc_stats.
                                                              dest_stats[count].no_datagrams_recd );

                        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                              &get_stats_resp.stats_data.assoc_stats.
                                                              dest_stats[count].no_bytes_sent );

                        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                              &get_stats_resp.stats_data.assoc_stats.
                                                              dest_stats[count].no_bytes_recvd );

                        /* CSR 1-4850698 */
                        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                               sizeof( sctp_time_t ),
                                                               ( sctp_U8bit * )&get_stats_resp.stats_data.assoc_stats.
                                                               dest_stats[count].dest_addr_start_time );
                        /* CSR 1-4850698 end */

                    }

                    /* CSR 1-4850698 */
                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.buffer_stat.
                                                          tx_med_utilized_instead_min );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.buffer_stat.
                                                          tx_max_utilized_instead_min );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.buffer_stat.
                                                          tx_max_utilized_instead_med );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.buffer_stat.
                                                          rx_med_utilized_instead_min );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.buffer_stat.
                                                          rx_max_utilized_instead_min );

                    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                          &get_stats_resp.stats_data.assoc_stats.buffer_stat.
                                                          rx_max_utilized_instead_med );
                    /* CSR 1-4850698 */
                    break;
                }

            case SCTP_ALL_STATS:
                {
                    break;
                }
        }
    }

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

#endif /* SCTP_STRUCT_BASED */

    SCTP_CL_REPORT( ( "\nAPI Response::Get Stats: result:%d,errorno:%d\n",
                      get_stats_resp.result, get_stats_resp.errorno ) );

    if( get_stats_resp.result == SCTP_SUCCESS )
    {
        SCTP_CL_REPORT( ( "\nAssociation ID: %u, Stats type: %d",
                          get_stats_resp.association_id, get_stats_resp.stats_type ) );

        switch( get_stats_resp.stats_type )
        {
            case SCTP_PROT_INTERNAL_STATS:
                {
                    SCTP_CL_REPORT( ( "\n no of applications registered : %u",
                                      get_stats_resp.stats_data.prot_stats.no_applications_reg ) );

                    SCTP_CL_REPORT( ( "\n no_associations_open : %u",
                                      get_stats_resp.stats_data.prot_stats.no_associations_open ) );

                    SCTP_CL_REPORT( ( "\n no of dest marked active : %u",
                                      get_stats_resp.stats_data.prot_stats.
                                      no_dest_marked_active ) );

                    SCTP_CL_REPORT( ( "\n no of dest marked inactive : %u",
                                      get_stats_resp.stats_data.prot_stats.
                                      no_dest_marked_inactive ) );

                    SCTP_CL_REPORT( ( "\n no of endpoints marked up : %u",
                                      get_stats_resp.stats_data.prot_stats.
                                      no_endpoints_marked_up ) );

                    SCTP_CL_REPORT( ( "\n no of endpoints marked down : %u",
                                      get_stats_resp.stats_data.prot_stats.
                                      no_endpoints_marked_down ) );

                    break;
                }

            case SCTP_GLOBAL_TRAFFIC_STATS:
                {
                    SCTP_CL_REPORT( ( "\n no of assoc accepted : %u",
                                      get_stats_resp.stats_data.traf_stats.no_assoc_accepted ) );

                    SCTP_CL_REPORT( ( "\n no of assoc initiated : %u",
                                      get_stats_resp.stats_data.traf_stats.no_assoc_initiated ) );

                    SCTP_CL_REPORT( ( "\n no_graceful_shutdown_assocs : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_graceful_shutdown_assocs ) );

                    SCTP_CL_REPORT( ( "\n no_ungraceful_shutdown_assocs : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_ungraceful_shutdown_assocs ) );

                    SCTP_CL_REPORT( ( "\n no_trans_data_chunks : %u",
                                      get_stats_resp.stats_data.traf_stats.no_trans_data_chunks ) );

                    SCTP_CL_REPORT( ( "\n no_retrans_data_chunks : %u",
                                      get_stats_resp.stats_data.traf_stats.no_retrans_data_chunks ) );

                    SCTP_CL_REPORT( ( "\n no_data_chunks_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_data_chunks_sent ) );

                    SCTP_CL_REPORT( ( "\n no_ord_data_chunks_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_ord_data_chunks_sent ) );

                    SCTP_CL_REPORT( ( "\n no_unord_data_chunks_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_unord_data_chunks_sent ) );

                    SCTP_CL_REPORT( ( "\n no_data_chunks_unacked : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_data_chunks_unacked ) );

                    SCTP_CL_REPORT( ( "\n no_data_chunks_recd : %u",
                                      get_stats_resp.stats_data.traf_stats.no_data_chunks_recv ) );

                    SCTP_CL_REPORT( ( "\n no_ord_data_chunks_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_ord_data_chunks_recv ) );

                    SCTP_CL_REPORT( ( "\n no_unord_data_chunks_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_unord_data_chunks_recv ) );

                    SCTP_CL_REPORT( ( "\n no_bytes_sent_ok : %u",
                                      get_stats_resp.stats_data.traf_stats.no_bytes_sent_ok ) );

                    SCTP_CL_REPORT( ( "\n no_bytes_recvd_ok : %u",
                                      get_stats_resp.stats_data.traf_stats.no_bytes_recvd_ok ) );

                    SCTP_CL_REPORT( ( "\n no_init_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_init_sent ) );

                    SCTP_CL_REPORT( ( "\n no_init_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_init_recv ) );

                    SCTP_CL_REPORT( ( "\n no_initack_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_initack_sent ) );

                    SCTP_CL_REPORT( ( "\n no_initack_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_initack_recv ) );

                    SCTP_CL_REPORT( ( "\n no_cookie_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_cookie_sent ) );

                    SCTP_CL_REPORT( ( "\n no_cookie_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_cookie_recv ) );

                    SCTP_CL_REPORT( ( "\n no_cookieack_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_cookieack_sent ) );

                    SCTP_CL_REPORT( ( "\n no_cookieack_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_cookieack_recv ) );

                    SCTP_CL_REPORT( ( "\n no_hb_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_hb_sent ) );

                    SCTP_CL_REPORT( ( "\n no_hb_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_hb_recv ) );

                    SCTP_CL_REPORT( ( "\n no_hback_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_hback_sent ) );

                    SCTP_CL_REPORT( ( "\n no_hback_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_hback_recv ) );

                    SCTP_CL_REPORT( ( "\n no_sack_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_sack_sent ) );

                    SCTP_CL_REPORT( ( "\n no_sack_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_sack_recv ) );

                    SCTP_CL_REPORT( ( "\n no_abort_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_abort_sent ) );

                    SCTP_CL_REPORT( ( "\n no_abort_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_abort_recv ) );

                    SCTP_CL_REPORT( ( "\n no_shut_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_shut_sent ) );

                    SCTP_CL_REPORT( ( "\n no_shut_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_shut_recv ) );

                    SCTP_CL_REPORT( ( "\n no_shutack_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_shutack_sent ) );

                    SCTP_CL_REPORT( ( "\n no_shutack_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_shutack_recv ) );

                    SCTP_CL_REPORT( ( "\n no_shutcom_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_shutcom_sent ) );

                    SCTP_CL_REPORT( ( "\n no_shutcom_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_shutcom_recv ) );

                    SCTP_CL_REPORT( ( "\n no_error_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_error_sent ) );

                    SCTP_CL_REPORT( ( "\n no_error_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_error_recv ) );

                    SCTP_CL_REPORT( ( "\n no_ecne_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_ecne_sent ) );

                    SCTP_CL_REPORT( ( "\n no_ecne_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_ecne_recv ) );

                    SCTP_CL_REPORT( ( "\n no_cwr_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_cwr_sent ) );

                    SCTP_CL_REPORT( ( "\n no_cwr_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_cwr_recv ) );

                    SCTP_CL_REPORT( ( "\n no_datagrams_recvd_in_error : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_datagrams_recvd_in_error ) );

                    SCTP_CL_REPORT( ( "\n no_chk_sum_err_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_chk_sum_err_recv ) );

                    SCTP_CL_REPORT( ( "\n no_ootb_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_ootb_recv ) );

                    SCTP_CL_REPORT( ( "\n no_frag_usr_msg : %u",
                                      get_stats_resp.stats_data.traf_stats.no_frag_usr_msg ) );

                    SCTP_CL_REPORT( ( "\n no_reassemble_usr_msg : %u",
                                      get_stats_resp.stats_data.traf_stats.
                                      no_reassemble_usr_msg ) );

                    SCTP_CL_REPORT( ( "\n no_packets_sent : %u",
                                      get_stats_resp.stats_data.traf_stats.no_packets_sent ) );

                    SCTP_CL_REPORT( ( "\n no_packets_recv : %u",
                                      get_stats_resp.stats_data.traf_stats.no_packets_recv ) );

                    break;
                }

            case SCTP_ERROR_STATS:
                {
                    SCTP_CL_REPORT( ( "\n API_error_stats_count : %u",
                                      get_stats_resp.stats_data.error_stats.
                                      API_error_stats_count ) );

                    SCTP_CL_REPORT( ( "\n protcol_error_stats_count : %u",
                                      get_stats_resp.stats_data.error_stats.
                                      protcol_error_stats_count ) );

                    SCTP_CL_REPORT( ( "\n system_error_stats_count : %u",
                                      get_stats_resp.stats_data.error_stats.
                                      system_error_stats_count ) );

                    SCTP_CL_REPORT( ( "\n system_resource_error_stats_count : %u",
                                      get_stats_resp.stats_data.error_stats.
                                      system_resource_error_stats_count ) );

                    SCTP_CL_REPORT( ( "\n internal_error_stats_count : %u",
                                      get_stats_resp.stats_data.error_stats.
                                      internal_error_stats_count ) );

                    break;
                }

            case SCTP_ASSOC_STATS:
                {
                    SCTP_CL_REPORT( ( "\n assoc_id : %u",
                                      get_stats_resp.stats_data.assoc_stats.assoc_id ) );

                    SCTP_CL_REPORT( ( "\ntime_stamp: %ld sec",
                                      get_stats_resp.stats_data.assoc_stats.start_time.tv_sec ) );

                    SCTP_CL_REPORT( ( "\ntime_stamp: %ld usec",
                                      get_stats_resp.stats_data.assoc_stats.start_time.tv_usec ) );

                    SCTP_CL_REPORT( ( "\n no_chunks_retrans : %u",
                                      get_stats_resp.stats_data.assoc_stats.no_chunks_retrans ) );

                    SCTP_CL_REPORT( ( "\n no_T1_expired : %u",
                                      get_stats_resp.stats_data.assoc_stats.no_T1_expired ) );

                    SCTP_CL_REPORT( ( "\n no_T2_expired : %u",
                                      get_stats_resp.stats_data.assoc_stats.no_T2_expired ) );

                    SCTP_CL_REPORT( ( "\n no_dest_address : %u",
                                      get_stats_resp.stats_data.assoc_stats.no_dest_address ) );

                    for( count = 0;
                            count < get_stats_resp.stats_data.assoc_stats.
                            no_dest_address;
                            count++ )
                    {
                        SCTP_CL_REPORT( ( "\naddr: " ) );
                        SCTP_CLIENT_TRACE_ADDR(
                            &get_stats_resp.stats_data.assoc_stats.
                            dest_stats[count].dest_address );

                        SCTP_CL_REPORT( ( "\n no_datagrams_sent : %u",
                                          get_stats_resp.stats_data.assoc_stats.
                                          dest_stats[count].no_datagrams_sent ) );

                        SCTP_CL_REPORT( ( "\n no_datagrams_unacked : %u",
                                          get_stats_resp.stats_data.assoc_stats.
                                          dest_stats[count].no_datagrams_unacked ) );

                        SCTP_CL_REPORT( ( "\n no_datagrams_recd : %u",
                                          get_stats_resp.stats_data.assoc_stats.
                                          dest_stats[count].no_datagrams_recd ) );

                        SCTP_CL_REPORT( ( "\n no_bytes_sent : %u",
                                          get_stats_resp.stats_data.assoc_stats.
                                          dest_stats[count].no_bytes_sent ) );

                        SCTP_CL_REPORT( ( "\n no_bytes_recvd : %u",
                                          get_stats_resp.stats_data.assoc_stats.
                                          dest_stats[count].no_bytes_recvd ) );

                    }

                    break;
                }

            case SCTP_ALL_STATS:
                {
                    break;
                }
        }
    }

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_STATS_RESP, &get_stats_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_get_err_level_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_err_level_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_get_error_level_res_st get_error_level_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_err_level_resp " ) );

    __MEMSET( &get_error_level_resp, 0, sizeof( get_error_level_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_error_level_res_st ),
                                           ( sctp_U8bit * )&get_error_level_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_error_level_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_error_level_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_error_level_resp.error_level );


#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CL_REPORT( ( "\nAPI Response::Get Error Level: result:%d, error:%d, Level:%d",
                      get_error_level_resp.result, get_error_level_resp.errorno,
                      get_error_level_resp.error_level ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_ERROR_LEVEL_RESP, &get_error_level_resp );

    return ( ret );
}

/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_set_err_level_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_set_err_level_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_set_error_level_res_st set_error_level_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_set_err_level_resp " ) );

    __MEMSET( &set_error_level_resp, 0, sizeof( set_error_level_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_error_level_res_st ),
                                           ( sctp_U8bit * )&set_error_level_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_error_level_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_error_level_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CL_REPORT( ( "\nAPI Response::Set Error Level: result:%d, error:%d",
                      set_error_level_resp.result, set_error_level_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_SET_ERROR_LEVEL_RESP, &set_error_level_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_enable_assoc_trc_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_enable_assoc_trc_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_enable_assoc_trc_res_st enable_trc_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_enable_assoc_trc_resp " ) );

    __MEMSET( &enable_trc_resp, 0, sizeof( enable_trc_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_enable_assoc_trc_res_st ),
                                           ( sctp_U8bit * )&enable_trc_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&enable_trc_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&enable_trc_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &enable_trc_resp.assoc_id );


#endif /* SCTP_STRUCT_BASED */


    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Enable Trace: result:%d, error:%d, assoc:%u",
                      enable_trc_resp.result, enable_trc_resp.errorno,
                      enable_trc_resp.assoc_id ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_ENABLE_ASSOC_TRC_RESP, &enable_trc_resp );

    return ( ret );
}

/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_disable_assoc_trc_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_disable_assoc_trc_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_disable_assoc_trc_res_st disable_trc_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_disable_assoc_trc_resp " ) );

    __MEMSET( &disable_trc_resp, 0, sizeof( disable_trc_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_disable_assoc_trc_res_st ),
                                           ( sctp_U8bit * )&disable_trc_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&disable_trc_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&disable_trc_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &disable_trc_resp.assoc_id );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Disable Trace: result:%d, error:%d, assoc:%u",
                      disable_trc_resp.result, disable_trc_resp.errorno,
                      disable_trc_resp.assoc_id ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_DISABLE_ASSOC_TRC_RESP, &disable_trc_resp );

    return ( ret );
}

/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_get_trc_info_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_trc_info_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;
    sctp_msg_get_trace_info_res_st trc_info_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_trc_info_resp " ) );

    __MEMSET( &trc_info_resp, 0, sizeof( trc_info_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_trace_info_res_st ),
                                           ( sctp_U8bit * )&trc_info_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&trc_info_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&trc_info_resp.errorno );

    if( trc_info_resp.result == SCTP_SUCCESS )
    {

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&trc_info_resp.trace_info );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &trc_info_resp.assoc_id );

    }

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */

    SCTP_CL_REPORT( ( "\nAPI Response:: Trace Info: result:%d, error:%d",
                      trc_info_resp.result, trc_info_resp.errorno ) );

    if( trc_info_resp.result == SCTP_SUCCESS )
    {
        SCTP_CL_REPORT( ( "\nAssoc ID: %u", trc_info_resp.assoc_id ) );

        SCTP_CL_REPORT( ( "\nTrace Level : %d\n", trc_info_resp.trace_info ) );

    }

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_TRACE_INFO_RESP, &trc_info_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_config_stack_addrs_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_config_stack_addrs_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_config_stack_addrs_res_st config_addr_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_config_stack_addrs_resp " ) );

    __MEMSET( &config_addr_resp, 0, sizeof( config_addr_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( config_addr_resp ),
                                           ( sctp_U8bit * )&config_addr_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_addr_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&config_addr_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Config Stack Addrs: result:%d, error:%d",
                      config_addr_resp.result, config_addr_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_CONFIG_STACK_ADDRS_RESP, &config_addr_resp );

    return ( ret );
}

/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_config_stack_addrs_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_deinit_stack_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_deinit_stack_res_st deinit_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_deinit_stack_resp " ) );

    __MEMSET( &deinit_resp, 0, sizeof( deinit_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( deinit_resp ),
                                           ( sctp_U8bit * )&deinit_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&deinit_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&deinit_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_req_buffer );
    /* SPR 21107 changes end */
    SCTP_CL_REPORT( ( "\nAPI Response::Deinit Stack : result:%d, error:%d",
                      deinit_resp.result, deinit_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_DEINIT_STACK_RESP, &deinit_resp );

    return ( ret );
}

/* Start SPR 20526 */
/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_get_rtt_info_resp
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *     recvd from the client. It extracts the api parameters from the buffer
 *     and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_rtt_info_resp(
    sctp_U8bit              *p_req_buffer,
    sctp_len_t              req_len )
{
    sctp_return_t                   ret = SCTP_SUCCESS;
    sctp_dest_id_t                  dest_id;
    sctp_src_id_t                   src_id;
    sctp_trans_id_t                 transaction_id;
    sctp_msg_get_rtt_info_res_st    get_rtt_info_resp;
    sctp_U32bit count;

#ifndef SCTP_STRUCT_BASED
    sctp_U16bit u16_temp;
#endif /* SCTP_STRUCT_BASED */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_rtt_info_resp " ) );

    __MEMSET( &get_rtt_info_resp, 0, sizeof( sctp_msg_get_rtt_info_res_st ) );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    src_id  = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;
  
    /* Warning Fix S */
    SCTP_UNUSED_VARIABLE( dest_id );
    SCTP_UNUSED_VARIABLE( src_id );
    SCTP_UNUSED_VARIABLE( transaction_id );
    /* Warning Fix E */

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_rtt_info_res_st ),
                                           ( sctp_U8bit * )&get_rtt_info_resp );
#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &get_rtt_info_resp.association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    get_rtt_info_resp.result = ( sctp_return_t )u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    get_rtt_info_resp.errorno = ( sctp_error_t )u16_temp;

    if( get_rtt_info_resp.result == SCTP_SUCCESS )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_rtt_info_resp.nos_of_dest_trans_addrs );

        for( count = 0;
                count < get_rtt_info_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &get_rtt_info_resp.list_of_dest_trans_addrs[count] );
        }

        for( count = 0;
                count < get_rtt_info_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_rtt_info_resp.dest_rtt_param_list[count].min_rtt );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_rtt_info_resp.dest_rtt_param_list[count].max_rtt );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_rtt_info_resp.dest_rtt_param_list[count].last_recorded_rtt );
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_rtt_info_resp.dest_rtt_param_list[count].srtt );

        }

        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &get_rtt_info_resp.primary_dest_transport_addr );
    }

#endif /* SCTP_STRUCT_BASED */
    SCTP_CLIENT_TRACE( ( "API Response::RTT info:Assoc Id%u, result:%d, error:\
        %d, number Addrs=%u", get_rtt_info_resp.association_id,
                         get_rtt_info_resp.result, get_rtt_info_resp.errorno,
                         get_rtt_info_resp.nos_of_dest_trans_addrs ) );

    if( get_rtt_info_resp.result == SCTP_SUCCESS )
    {
        SCTP_CLIENT_TRACE( ( " Primary address:" ) );
        SCTP_CLIENT_TRACE_ADDR(
            &get_rtt_info_resp.primary_dest_transport_addr );

        for( count = 0;
                count < get_rtt_info_resp.nos_of_dest_trans_addrs;
                count++ )
        {
            SCTP_CLIENT_TRACE( ( " Index:%u", count ) );
            SCTP_CLIENT_TRACE_ADDR(
                &get_rtt_info_resp.list_of_dest_trans_addrs[count] );

            SCTP_CLIENT_TRACE( ( " MIN_RTT:%u,MAX_RTT:%u,LAST_RECORDED_RTT:%u,SRTT:%u",
                                 get_rtt_info_resp.dest_rtt_param_list[count].min_rtt,
                                 get_rtt_info_resp.dest_rtt_param_list[count].max_rtt,
                                 get_rtt_info_resp.dest_rtt_param_list[count].last_recorded_rtt,
                                 get_rtt_info_resp.dest_rtt_param_list[count].srtt ) );
        }
    }

    return( ret );
}

/* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT


/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_set_stack_red_state_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_set_stack_red_state_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_set_stack_red_state_res_st set_red_state_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_set_stack_red_state_resp " ) );

    __MEMSET( &set_red_state_resp, 0, sizeof( set_red_state_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_stack_red_state_res_st ),
                                           ( sctp_U8bit * )&set_red_state_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_red_state_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_red_state_resp.errorno );


#endif /* SCTP_STRUCT_BASED */

    SCTP_CL_REPORT( ( "\nAPI Response::Set Red State: result:%u, error:%u",
                      set_red_state_resp.result, set_red_state_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_SET_STACK_RED_STATE_RESP, &set_red_state_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_set_stack_proc_state_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_set_stack_proc_state_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_set_stack_proc_state_res_st set_proc_state_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_set_stack_proc_state_resp " ) );

    __MEMSET( &set_proc_state_resp, 0, sizeof( set_proc_state_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */


#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_stack_proc_state_res_st ),
                                           ( sctp_U8bit * )&set_proc_state_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_proc_state_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_proc_state_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    SCTP_CL_REPORT( ( "\nAPI Response::Set Proc State: result:%u, error:%u",
                      set_proc_state_resp.result, set_proc_state_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_SET_STACK_PROC_STATE_RESP, &set_proc_state_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_get_stack_red_db_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_stack_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;
    sctp_U32bit     count;

    sctp_msg_get_stack_red_db_res_st get_red_db_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_stack_red_db_resp " ) );

    __MEMSET( &get_red_db_resp, 0, sizeof( get_red_db_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_stack_red_db_res_st ),
                                           ( sctp_U8bit * )&get_red_db_resp );

#else

    /* result */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_red_db_resp.result );
    /* errorno */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_red_db_resp.errorno );

    if( SCTP_SUCCESS == get_red_db_resp.result )
    {

        /* max_appl */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_appl );
        /* num_active_appl */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.num_active_appl );
        /* max_ep */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_ep );
        /* num_active_ep */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.num_active_ep );
        /* last_allocated_ep */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.last_allocated_ep );
        /* max_assoc */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_assoc );
        /* tx_max_buffers */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.min_tx_buffers );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.med_tx_buffers );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_tx_buffers );
        /* rx_max_buffers */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.min_rx_buffers );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.med_rx_buffers );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_rx_buffers );
        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.min_reserve_buffers );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.med_reserve_buffers );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_reserve_buffers );
        /* Rel 6.2 Update Ends here. */

        /* max_streams_per_assoc */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.max_streams_per_assoc );
        /* num_local_addrs */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.num_local_addrs );

        /* active_ep_list */
        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_active_ep;
                count++ )
        {
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_red_db_resp.stack_red_db.active_ep_list[count] );
        }

        /* pmtu_timer_rem_time */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.pmtu_timer_rem_time );
        /* num_active_pmtu_nodes */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.num_active_pmtu_nodes );

        /* pmtu_database */
        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_active_pmtu_nodes;
                count++ )
        {
            p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                                   sizeof( sctp_time_t ),
                                                   ( sctp_U8bit * )&get_red_db_resp.stack_red_db.
                                                   pmtu_database[count].time_stamp );
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &get_red_db_resp.stack_red_db.pmtu_database[count].addr );
            p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                                   &get_red_db_resp.stack_red_db.pmtu_database[count].mtu_size );
            p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                                   &get_red_db_resp.stack_red_db.pmtu_database[count].spare );
        }

#ifdef SCTP_ERROR_ENABLED

        /* sctp_error_table */
        for( count = SCTP_MIN_ERROR_FLAG; count <= SCTP_MAX_ERROR_FLAG; count++ )
        {
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  ( sctp_U32bit * )&get_red_db_resp.stack_red_db.
                                                  sctp_error_table[count] );
        }

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_STATS_ENABLED

        /* sctp_stats_state */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_red_db_resp.stack_red_db.sctp_stats_state );
        /* sctp_protocol_stats */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_protocol_stats.
                                              no_applications_reg );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_protocol_stats.
                                              no_associations_open );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_protocol_stats.
                                              no_dest_marked_active );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_protocol_stats.
                                              no_dest_marked_inactive );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_protocol_stats.
                                              no_endpoints_marked_up );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_protocol_stats.
                                              no_endpoints_marked_down );

        /* sctp_global_traffic_stats */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_assoc_accepted );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_assoc_initiated );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_graceful_shutdown_assocs );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_ungraceful_shutdown_assocs );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_trans_data_chunks );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_retrans_data_chunks );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_data_chunks_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_ord_data_chunks_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_unord_data_chunks_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_data_chunks_unacked );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_data_chunks_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_ord_data_chunks_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_unord_data_chunks_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_bytes_sent_ok );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_bytes_recvd_ok );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_init_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_init_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_initack_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_initack_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_cookie_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_cookie_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_cookieack_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_cookieack_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_hb_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_hb_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_hback_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_hback_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_sack_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_sack_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_abort_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_abort_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_shut_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_shut_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_shutack_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_shutack_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_shutcom_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_shutcom_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_error_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_error_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_ecne_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_ecne_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_cwr_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_cwr_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_datagrams_recvd_in_error );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_chk_sum_err_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_ootb_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_frag_usr_msg );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_reassemble_usr_msg );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_packets_sent );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                                              no_packets_recv );
        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer, sizeof( sctp_time_t ),
                                               ( sctp_U8bit * )&get_red_db_resp.stack_red_db.
                                               sctp_global_traffic_stats.discont_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_asconf_sent );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_asconf_recv );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_asconfack_sent );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_asconfack_recv );
#endif
        /* Rel 6.2 Update Ends Here. */

        /* sctp_error_stats */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_error_stats.
                                              API_error_stats_count );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_error_stats.
                                              protcol_error_stats_count );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_error_stats.
                                              system_error_stats_count );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_error_stats.
                                              system_resource_error_stats_count );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_error_stats.
                                              internal_error_stats_count );

        /* sctp_stats_table */
        for( count = 0; count < SCTP_ALL_STATS; count++ )
        {
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  ( sctp_U32bit * )&get_red_db_resp.stack_red_db.
                                                  sctp_stats_table[count] );
        }


#endif /* SCTP_STATS_ENABLED */

        /* active_appl_list */
        for( count = 0;
                count < get_red_db_resp.stack_red_db.max_appl ;
                count++ )
        {
            p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                                  &get_red_db_resp.stack_red_db.
                                                  active_appl_list[count].open );
            p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                                   &get_red_db_resp.stack_red_db.
                                                   active_appl_list[count].su_id );
        }

        /* addr_list */
        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_local_addrs;
                count++ )
        {
            p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                        &get_red_db_resp.stack_red_db.addr_list[count] );
        }

        /* sctp_def_prot_params */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_def_prot_params.rto_init );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_def_prot_params.rto_min );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_def_prot_params.rto_max );
        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_def_prot_params.max_burst );
        /* Rel 6.2 Update Ends Here */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.valid_cookie_life );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.assoc_max_retrans );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_def_prot_params.hb_interval );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.path_max_retrans );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.max_init_retrans );
        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.max_init_path_retrans );
        /* Rel 6.2 Update Ends Here */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.init_timeo );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.bundling_time );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.sack_period );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.sack_frequency );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.adaption_ind );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.maxseg );
        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.hb_info_length );
        /* Rel 6.2 Update Ends Here */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.mtu_size );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.rto_alpha_percent );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.rto_beta_percent );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.num_in_streams );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.num_out_streams );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.re_use_addr );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.dont_fragment );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.want_mapped );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.nat_capable );

        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.pmtu_capable );
        /* Rel 6.2 Update Ends Here */

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.allow_asconf );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.checksum_func );

        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.ppi );

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.hb_max_burst );

#ifdef SCTP_IP_HEADER_PARAMS
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.ip_header_params.ttl );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_def_prot_params.ip_header_params.tos );

#ifdef SCTP_INET6_SUPPORT
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.ip_header_params.hoplimit );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.
                                               sctp_def_prot_params.ip_header_params.tclass );
#endif
#endif

        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.high_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.high_cong_lvl_abate );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.med_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.med_cong_lvl_abate );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.low_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.low_cong_lvl_abate );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.no_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              trx_cong_reg_def.no_cong_lvl_abate );

        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.high_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.high_cong_lvl_abate );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.med_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.med_cong_lvl_abate );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.low_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.low_cong_lvl_abate );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.no_cong_lvl_onset );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              rcv_cong_reg_def.no_cong_lvl_abate );
        /* Rel 6.2 Update Ends Here */

        /* sctp_stack_init_parameters */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_red_db_resp.stack_red_db.
                                              sctp_stack_init_parameters.ecn_capable );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_red_db_resp.stack_red_db.
                                              sctp_stack_init_parameters.pmtu_capable );

        /* Rel 6.2 Update */
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_red_db_resp.stack_red_db.
                                              sctp_stack_init_parameters.nat_capable );
        /* Rel 6.2 Update Ends Here */

        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_stack_init_parameters.ack_policy );
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.
                                              sctp_stack_init_parameters.stale_cookie_policy );
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_red_db_resp.stack_red_db.
                                              sctp_stack_init_parameters.stack_type );

        /* init_in_streams */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.init_in_streams );

        /* init_out_streams */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.init_out_streams );

        /* standard_udp_port */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &get_red_db_resp.stack_red_db.standard_udp_port );

#ifdef SCTP_ERROR_ENABLED
        /* sctp_error_state */
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_error_state );

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_DEBUG_ENABLED
        /* sctp_gbl_trc */
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &get_red_db_resp.stack_red_db.sctp_gbl_trc );
#endif /* SCTP_DEBUG_ENABLED */

        /*Old mac keys*/
        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer, SCTP_KEYSIZE,
                                               &get_red_db_resp.stack_red_db.old_mac_key[0] );

        /*Current mac keys*/
        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer, SCTP_KEYSIZE,
                                               &get_red_db_resp.stack_red_db.current_mac_key[0] );
    }

#endif /* SCTP_STRUCT_BASED */


    SCTP_CL_REPORT( ( "\nAPI Response::Get Stack Red DB: result:%u, error:%u",
                      get_red_db_resp.result, get_red_db_resp.errorno ) );

    if( get_red_db_resp.result == SCTP_SUCCESS )
    {

        SCTP_CL_REPORT( ( "\nMax app : %u",
                          get_red_db_resp.stack_red_db.max_appl ) );

        SCTP_CL_REPORT( ( "\nNum_active_appl : %u",
                          get_red_db_resp.stack_red_db.num_active_appl ) );

        SCTP_CL_REPORT( ( "\nMax_ep : %u", get_red_db_resp.stack_red_db.max_ep ) );

        SCTP_CL_REPORT( ( "\nNum_active_ep : %u",
                          get_red_db_resp.stack_red_db.num_active_ep ) );

        SCTP_CL_REPORT( ( "\nLast_allocated_ep : %u",
                          get_red_db_resp.stack_red_db.last_allocated_ep ) );

        SCTP_CL_REPORT( ( "\nMax_assoc : %u",
                          get_red_db_resp.stack_red_db.max_assoc ) );

        SCTP_CL_REPORT( ( "\ntx_min_buffers : %u",
                          get_red_db_resp.stack_red_db.min_tx_buffers ) );

        SCTP_CL_REPORT( ( "\ntx_med_buffers : %u",
                          get_red_db_resp.stack_red_db.med_tx_buffers ) );

        SCTP_CL_REPORT( ( "\ntx_max_buffers : %u",
                          get_red_db_resp.stack_red_db.max_tx_buffers ) );

        SCTP_CL_REPORT( ( "\nrx_max_buffers : %u",
                          get_red_db_resp.stack_red_db.min_rx_buffers ) );

        SCTP_CL_REPORT( ( "\nrx_max_buffers : %u",
                          get_red_db_resp.stack_red_db.min_rx_buffers ) );

        SCTP_CL_REPORT( ( "\nrx_max_buffers : %u",
                          get_red_db_resp.stack_red_db.min_rx_buffers ) );

        SCTP_CL_REPORT( ( "\nmax_streams_per_assoc : %u",
                          get_red_db_resp.stack_red_db.max_streams_per_assoc ) );

        SCTP_CL_REPORT( ( "\nnum_local_addrs : %u",
                          get_red_db_resp.stack_red_db.num_local_addrs ) );

        SCTP_CL_REPORT( ( "\ninit_in_streams : %hu",
                          get_red_db_resp.stack_red_db.init_in_streams ) );

        SCTP_CL_REPORT( ( "\ninit_out_streams : %hu",
                          get_red_db_resp.stack_red_db.init_out_streams ) );

        SCTP_CL_REPORT( ( "\nstandard_udp_port : %hu",
                          get_red_db_resp.stack_red_db.standard_udp_port ) );

        SCTP_CL_REPORT( ( "\npmtu timer remaining time : %u",
                          get_red_db_resp.stack_red_db.pmtu_timer_rem_time ) );

        SCTP_CL_REPORT( ( "\nnum pmtu nodes : %u",
                          get_red_db_resp.stack_red_db.num_active_pmtu_nodes ) );

        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_active_ep;
                count++ )
        {
            SCTP_CL_REPORT( ( "\nActive association ID: %u",
                              get_red_db_resp.stack_red_db.active_ep_list[count] ) );
        }

        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_active_pmtu_nodes;
                count++ )
        {

            SCTP_CL_REPORT( ( "\ntime_stamp: %ld sec",
                              get_red_db_resp.stack_red_db.pmtu_database[count].
                              time_stamp.tv_sec ) );

            SCTP_CL_REPORT( ( "\ntime_stamp: %ld usec",
                              get_red_db_resp.stack_red_db.pmtu_database[count].
                              time_stamp.tv_usec ) );

            SCTP_CL_REPORT( ( "\naddr: " ) );
            SCTP_CLIENT_TRACE_ADDR(
                &( get_red_db_resp.stack_red_db.pmtu_database[count].addr ) );

            SCTP_CL_REPORT( ( "\nmtu size: %hu",
                              get_red_db_resp.stack_red_db.pmtu_database[count].mtu_size ) );

            SCTP_CL_REPORT( ( "\nspare: %hu",
                              get_red_db_resp.stack_red_db.pmtu_database[count].spare ) );

        }

        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_local_addrs;
                count++ )
        {
            SCTP_CL_REPORT( ( "\nLocal address : " ) );
            SCTP_CLIENT_TRACE_ADDR(
                &get_red_db_resp.stack_red_db.addr_list[count] );
        }

        for( count = 0;
                count < get_red_db_resp.stack_red_db.num_active_appl;
                count++ )
        {
            if( get_red_db_resp.stack_red_db.active_appl_list[count].open == 1 )
            {
                SCTP_CL_REPORT( ( "\nActive appl ID : %u", count ) );
                SCTP_CL_REPORT( ( "\tSU ID : %hu",
                                  get_red_db_resp.stack_red_db.active_appl_list[count].su_id ) );
            }
        }


#ifdef SCTP_ERROR_ENABLED
        SCTP_CL_REPORT( ( "\nsctp error table : " ) );

        for( count = 0; count < SCTP_MAX_ERROR_FLAG + 1; count++ )
        {
            SCTP_CL_REPORT( ( "\nError Level : %hu",
                              get_red_db_resp.stack_red_db.sctp_error_table[count] ) );
        }

#endif

#ifdef SCTP_STATS_ENABLED
        SCTP_CL_REPORT( ( "\n sctp stats state : %hu",
                          get_red_db_resp.stack_red_db.sctp_stats_state ) );

        SCTP_CL_REPORT( ( "\n no of applications registered : %u",
                          get_red_db_resp.stack_red_db.sctp_protocol_stats.
                          no_applications_reg ) );

        SCTP_CL_REPORT( ( "\n no_associations_open : %u",
                          get_red_db_resp.stack_red_db.sctp_protocol_stats.
                          no_associations_open ) );

        SCTP_CL_REPORT( ( "\n no of dest marked active : %u",
                          get_red_db_resp.stack_red_db.sctp_protocol_stats.
                          no_dest_marked_active ) );

        SCTP_CL_REPORT( ( "\n no of dest marked inactive : %u",
                          get_red_db_resp.stack_red_db.sctp_protocol_stats.
                          no_dest_marked_inactive ) );

        SCTP_CL_REPORT( ( "\n no of endpoints marked up : %u",
                          get_red_db_resp.stack_red_db.sctp_protocol_stats.
                          no_endpoints_marked_up ) );

        SCTP_CL_REPORT( ( "\n no of endpoints marked down : %u",
                          get_red_db_resp.stack_red_db.sctp_protocol_stats.
                          no_endpoints_marked_down ) );

        SCTP_CL_REPORT( ( "\n no of assoc accepted : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_assoc_accepted ) );

        SCTP_CL_REPORT( ( "\n no of assoc initiated : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_assoc_initiated ) );

        SCTP_CL_REPORT( ( "\n no_graceful_shutdown_assocs : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_graceful_shutdown_assocs ) );

        SCTP_CL_REPORT( ( "\n no_ungraceful_shutdown_assocs : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_ungraceful_shutdown_assocs ) );

        SCTP_CL_REPORT( ( "\n no_trans_data_chunks : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_trans_data_chunks ) );

        SCTP_CL_REPORT( ( "\n no_retrans_data_chunks : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_retrans_data_chunks ) );

        SCTP_CL_REPORT( ( "\n no_data_chunks_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_data_chunks_sent ) );

        SCTP_CL_REPORT( ( "\n no_ord_data_chunks_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_ord_data_chunks_sent ) );

        SCTP_CL_REPORT( ( "\n no_unord_data_chunks_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_unord_data_chunks_sent ) );

        SCTP_CL_REPORT( ( "\n no_data_chunks_unacked : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_data_chunks_unacked ) );

        SCTP_CL_REPORT( ( "\n no_data_chunks_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_data_chunks_recv ) );

        SCTP_CL_REPORT( ( "\n no_ord_data_chunks_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_ord_data_chunks_recv ) );

        SCTP_CL_REPORT( ( "\n no_unord_data_chunks_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_unord_data_chunks_recv ) );

        SCTP_CL_REPORT( ( "\n no_bytes_sent_ok : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_bytes_sent_ok ) );

        SCTP_CL_REPORT( ( "\n no_bytes_recvd_ok : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_bytes_recvd_ok ) );

        SCTP_CL_REPORT( ( "\n no_init_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_init_sent ) );

        SCTP_CL_REPORT( ( "\n no_init_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_init_recv ) );

        SCTP_CL_REPORT( ( "\n no_cookie_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_cookie_sent ) );

        SCTP_CL_REPORT( ( "\n no_cookie_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_cookie_recv ) );

        SCTP_CL_REPORT( ( "\n no_cookieack_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_cookieack_sent ) );

        SCTP_CL_REPORT( ( "\n no_cookieack_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_cookieack_recv ) );

        SCTP_CL_REPORT( ( "\n no_hb_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_hb_sent ) );

        SCTP_CL_REPORT( ( "\n no_hb_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.no_hb_recv ) );

        SCTP_CL_REPORT( ( "\n no_hback_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_hback_sent ) );

        SCTP_CL_REPORT( ( "\n no_hback_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_hback_recv ) );

        SCTP_CL_REPORT( ( "\n no_sack_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_sack_sent ) );

        SCTP_CL_REPORT( ( "\n no_sack_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_sack_recv ) );

        SCTP_CL_REPORT( ( "\n no_abort_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_abort_sent ) );

        SCTP_CL_REPORT( ( "\n no_abort_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_abort_recv ) );

        SCTP_CL_REPORT( ( "\n no_shut_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_shut_sent ) );

        SCTP_CL_REPORT( ( "\n no_shut_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_shut_recv ) );

        SCTP_CL_REPORT( ( "\n no_shutack_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_shutack_sent ) );

        SCTP_CL_REPORT( ( "\n no_shutack_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_shutack_recv ) );

        SCTP_CL_REPORT( ( "\n no_shutcom_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_shutcom_sent ) );

        SCTP_CL_REPORT( ( "\n no_shutcom_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_shutcom_recv ) );

        SCTP_CL_REPORT( ( "\n no_error_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_error_sent ) );

        SCTP_CL_REPORT( ( "\n no_error_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_error_recv ) );

        SCTP_CL_REPORT( ( "\n no_ecne_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_ecne_sent ) );

        SCTP_CL_REPORT( ( "\n no_ecne_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_ecne_recv ) );

        SCTP_CL_REPORT( ( "\n no_cwr_sent : %u", get_red_db_resp.stack_red_db.
                          sctp_global_traffic_stats.no_cwr_sent ) );

        SCTP_CL_REPORT( ( "\n no_cwr_recv : %u", get_red_db_resp.stack_red_db.
                          sctp_global_traffic_stats.no_cwr_recv ) );

        SCTP_CL_REPORT( ( "\n no_datagrams_recvd_in_error : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_datagrams_recvd_in_error ) );

        SCTP_CL_REPORT( ( "\n no_chk_sum_err_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_chk_sum_err_recv ) );

        SCTP_CL_REPORT( ( "\n no_ootb_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_ootb_recv ) );

        SCTP_CL_REPORT( ( "\n no_frag_usr_msg : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_frag_usr_msg ) );

        SCTP_CL_REPORT( ( "\n no_reassemble_usr_msg : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_reassemble_usr_msg ) );

        SCTP_CL_REPORT( ( "\n no_packets_sent : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_packets_sent ) );

        SCTP_CL_REPORT( ( "\n no_packets_recv : %u",
                          get_red_db_resp.stack_red_db.sctp_global_traffic_stats.
                          no_packets_recv ) );

        SCTP_CL_REPORT( ( "\n API_error_stats_count : %u",
                          get_red_db_resp.stack_red_db.sctp_error_stats.
                          API_error_stats_count ) );

        SCTP_CL_REPORT( ( "\n protcol_error_stats_count : %u",
                          get_red_db_resp.stack_red_db.sctp_error_stats.
                          protcol_error_stats_count ) );

        SCTP_CL_REPORT( ( "\n system_error_stats_count : %u",
                          get_red_db_resp.stack_red_db.sctp_error_stats.
                          system_error_stats_count ) );

        SCTP_CL_REPORT( ( "\n system_resource_error_stats_count : %u",
                          get_red_db_resp.stack_red_db.sctp_error_stats.
                          system_resource_error_stats_count ) );

        SCTP_CL_REPORT( ( "\n internal_error_stats_count : %u",
                          get_red_db_resp.stack_red_db.sctp_error_stats.
                          internal_error_stats_count ) );

#endif

        SCTP_CL_REPORT( ( "\necn_capable : %u",
                          get_red_db_resp.stack_red_db.sctp_stack_init_parameters.
                          ecn_capable ) );

        SCTP_CL_REPORT( ( "\npmtu_capable : %u",
                          get_red_db_resp.stack_red_db.sctp_stack_init_parameters.
                          pmtu_capable ) );

        SCTP_CL_REPORT( ( "\nack_policy : %u",
                          get_red_db_resp.stack_red_db.sctp_stack_init_parameters.
                          ack_policy ) );

        SCTP_CL_REPORT( ( "\nstale_cookie_policy : %u",
                          get_red_db_resp.stack_red_db.sctp_stack_init_parameters.
                          stale_cookie_policy ) );

        SCTP_CL_REPORT( ( "\nstack_type : %u",
                          get_red_db_resp.stack_red_db.sctp_stack_init_parameters.
                          stack_type ) );

        SCTP_CL_REPORT( ( "\nrto_init : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.rto_init ) );

        SCTP_CL_REPORT( ( "\nrto_min : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.rto_min ) );

        SCTP_CL_REPORT( ( "\nrto_max : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.rto_max ) );

        SCTP_CL_REPORT( ( "\nvalid_cookie_life : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          valid_cookie_life ) );

        SCTP_CL_REPORT( ( "\nassoc_max_retrans : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          assoc_max_retrans ) );

        SCTP_CL_REPORT( ( "\nhb_interval : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.hb_interval ) );

        SCTP_CL_REPORT( ( "\npath_max_retrans : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          path_max_retrans ) );

        SCTP_CL_REPORT( ( "\nmax_init_retrans : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          max_init_retrans ) );

        SCTP_CL_REPORT( ( "\ninit_timeo : %u",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          init_timeo ) );

        SCTP_CL_REPORT( ( "\nrto_alpha_percent : %hu",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          rto_alpha_percent ) );

        SCTP_CL_REPORT( ( "\nrto_beta_percent : %hu",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.
                          rto_beta_percent ) );

        SCTP_CL_REPORT( ( "\nnum_in_streams : %hu",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.num_in_streams ) );

        SCTP_CL_REPORT( ( "\nnum_out_streams : %hu",
                          get_red_db_resp.stack_red_db.sctp_def_prot_params.num_out_streams ) );


#ifdef SCTP_ERROR_ENABLED

        SCTP_CL_REPORT( ( "\nsctp_error_state : %hd",
                          ( sctp_U16bit )get_red_db_resp.stack_red_db.sctp_error_state ) );

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_DEBUG_ENABLED

        SCTP_CL_REPORT( ( "\nsctp_gbl_trc : %hd",
                          ( sctp_U16bit )get_red_db_resp.stack_red_db.sctp_gbl_trc ) );

#endif /* SCTP_DEBUG_ENABLED */

    }

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_STACK_RED_DB_RESP, &get_red_db_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :  sctp_sm_proc_api_set_stack_red_db_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_set_stack_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_set_stack_red_db_res_st set_red_db_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_set_stack_red_db_resp " ) );

    __MEMSET( &set_red_db_resp, 0, sizeof( set_red_db_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_stack_red_db_res_st ),
                                           ( sctp_U8bit * )&set_red_db_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_red_db_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_red_db_resp.errorno );

#endif /* SCTP_STRUCT_BASED */

    SCTP_CL_REPORT( ( "\nAPI Response::Set Stack Red DB: result:%u, error:%u",
                      set_red_db_resp.result, set_red_db_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_SET_STACK_RED_DB_RESP, &set_red_db_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_get_assoc_red_db_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_assoc_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_get_assoc_red_db_res_st get_red_db_resp;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_assoc_red_db_resp " ) );

    __MEMSET( &get_red_db_resp, 0, sizeof( get_red_db_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_assoc_red_db_res_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&get_red_db_resp );

    if( get_red_db_resp.result == SCTP_SUCCESS )
    {
        get_red_db_resp.assoc_red_db = p_req_buffer;


    }

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_red_db_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_red_db_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_red_db_resp.assoc_id );

    if( get_red_db_resp.result == SCTP_SUCCESS )
    {

        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&get_red_db_resp.len );
        /*p_req_buffer now points to assoc db*/
        get_red_db_resp.assoc_red_db = p_req_buffer;

    }



#endif /* SCTP_STRUCT_BASED */

    SCTP_CL_REPORT( ( "\nAPI Response::Get Assoc Red DB: result:%u, error:%u",
                      get_red_db_resp.result, get_red_db_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_ASSOC_RED_DB_RESP, &get_red_db_resp );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_set_assoc_red_db_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_set_assoc_red_db_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_set_assoc_red_db_res_st set_red_db_resp;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_set_assoc_red_db_resp " ) );

    __MEMSET( &set_red_db_resp, 0, sizeof( set_red_db_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_assoc_red_db_res_st ),
                                           ( sctp_U8bit * )&set_red_db_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_red_db_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&set_red_db_resp.errorno );

#endif /* SCTP_STRUCT_BASED */


    SCTP_CL_REPORT( ( "\nAPI Response::Set Assoc Red DB: result:%u, error:%u",
                      set_red_db_resp.result, set_red_db_resp.errorno ) );

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_SET_ASSOC_RED_DB_RESP, &set_red_db_resp );

    return ( ret );
}



/***************************************************************************
 *  FUNCTION : sctp_sm_proc_api_get_stack_health_resp
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_return_t
sctp_sm_proc_api_get_stack_health_resp(
    sctp_U8bit   *p_req_buffer,
    sctp_len_t   req_len )
{
    sctp_return_t   ret = SCTP_SUCCESS;
    sctp_trans_id_t transaction_id;

    sctp_msg_get_stack_health_res_st get_stack_health_resp;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( req_len );
    /*to avoid the warning*/

    SCTP_CLIENT_TRACE( ( "Entering sctp_sm_proc_api_get_stack_health_resp " ) );

    __MEMSET( &get_stack_health_resp, 0, sizeof( get_stack_health_resp ) );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* move the rest of the resp fields */

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_stack_health_res_st ),
                                           ( sctp_U8bit * )&get_stack_health_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stack_health_resp.result );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stack_health_resp.errorno );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stack_health_resp.red_state );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&get_stack_health_resp.proc_state );

#endif /* SCTP_STRUCT_BASED */

    SCTP_CL_REPORT( ( "\nAPI Response::Get Stack Health: result:%u, error:%u",
                      get_stack_health_resp.result, get_stack_health_resp.errorno ) );

    if( get_stack_health_resp.result == SCTP_SUCCESS )
    {
        SCTP_CL_REPORT( ( "\nStack proc state : %u, Stack red state : %u",
                          get_stack_health_resp.proc_state, get_stack_health_resp.red_state ) );
    }

    sctp_process_api_sm_resp( transaction_id,
                              SCTP_API_GET_STACK_HEALTH_RESP, &get_stack_health_resp );

    return ( ret );
}

#endif    /*SCTP_RED_SUPPORT*/





