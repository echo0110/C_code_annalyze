/******************************************************************************
 *    FILE NAME:  s_uismap.c
 *
 *    DESCRIPTION:
 *    This file is part of System Management Interface module. It includes
 *    functions the process SM API's.
 *
 *    DATE    NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    29March 2001     Vakul Garg   -        .Original Creation
 *    12Sep 2001       Yogesh Gahlaut        Rel 3.0 Changes
 *    02jan02 hbhondwe   SCTP_IPC_TCP    ipc changes for tcp
 *    07Oct03 Vivek Bansal    Rel 5.0         Changes for CSPL
 *    27Nov03 Vivek Bansal    Rel 5.0         Changes for CSPL
 *    02Nov04  Svakumar     REL 6.0                SCTP/FUNC-CONGCON-5
 *    09May08  Raza         REL 6.2           SPR 18527
 *    15Sep08  Raza         REL 6.2           Changes for Redundancy
 *    14july10 Sharad Kumar Yadav REL 6.2.0 SPR 20526 For CSR# 1-7670333
 *    25Jul10  Rajnesh         SPR Fix20540  Appid updated in Red
 *  Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#define SCTP_MODULE "SM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_cobuf.h>
#include <s_clsmap.h>
#include <s_dbg.h>
#include <s_stats.h>
#include <s_uico.h>
#include <s_in.h>
#include <s_err.h>
#include <s_stats.h>
#include <s_red.h>
#include <s_es.h>
#include <s_uismap.h>
#include <s_uiipc.h>


#ifdef SCTP_UI_SM_MESSAGEBASED


static sctp_U8bit *
sctp_proc_api_init_stack(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_entity_version(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_config_stack_params(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_init_stats(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_enable_stats(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_disable_stats(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_stats(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_error_level(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_set_error_level(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_enable_assoc_trc(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_disable_assoc_trc(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_trace_info(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_config_stack_addrs(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_deinit_stack(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

/* Start SPR 20526 */
static sctp_U8bit *
sctp_proc_api_get_rtt_info(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );
/* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT


static sctp_U8bit *
sctp_proc_api_set_stack_red_state(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_set_stack_proc_state(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_stack_red_db(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_set_stack_red_db(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_assoc_red_db(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_set_assoc_red_db(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

static sctp_U8bit *
sctp_proc_api_get_stack_health(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer );

#endif    /*SCTP_RED_SUPPORT*/

sctp_U8bit  *
( *sctp_ui_proc_sm_api_tbl[SCTP_NUM_SMREQ_API] )
( sctp_U8bit *, sctp_U8bit * ) =
{
    sctp_proc_api_init_stack,  /* init stack */
    sctp_proc_api_get_entity_version,  /* get entity version */
    sctp_proc_api_config_stack_params, /* config stack  */

    sctp_proc_api_init_stats,      /* init statistics */
    sctp_proc_api_enable_stats,    /* enable statistics */
    sctp_proc_api_disable_stats,   /* disable statistics */
    sctp_proc_api_get_stats,       /* get statistics  */

    sctp_proc_api_get_error_level, /*get error level */
    sctp_proc_api_set_error_level, /*set error level */

    sctp_proc_api_enable_assoc_trc, /*Enable assoc trace */
    sctp_proc_api_disable_assoc_trc,   /* send */
    sctp_proc_api_get_trace_info,       /* send Extended */

    sctp_proc_api_config_stack_addrs,
    sctp_proc_api_deinit_stack,
    /* Start SPR 20526 */
    sctp_proc_api_get_rtt_info,
    /* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT

    sctp_proc_api_set_stack_red_state,/* set stack red state */
    sctp_proc_api_set_stack_proc_state,/* set stack proc state */
    sctp_proc_api_get_stack_red_db,/* get stack red db */
    sctp_proc_api_set_stack_red_db,/* set stack red db */
    sctp_proc_api_get_assoc_red_db,/* get assoc red db */
    sctp_proc_api_set_assoc_red_db,/* set assoc red db */
    sctp_proc_api_get_stack_health,/* get stack health */

#endif    /*SCTP_RED_SUPPORT*/
};




/***************************************************************************
 *  FUNCTION :    sctp_process_sm_api_prim_msg
 *
 *  DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    none
 *
 *************************************************************************/
sctp_void_t
sctp_process_sm_api_prim_msg(
    sctp_U8bit             *p_buffer,
    sctp_len_t             nooctets,
    sctp_client_info_st    *from )
{
    sctp_api_id_t    api_id;
    sctp_U16bit      index_api_id;
    sctp_U32bit      noctets;
    sctp_src_id_t    src_id;
    sctp_dest_id_t   dest_id;
    sctp_Error_t     ecode;

    /*
     * Allocate the memory for response message. The memory for this buffer is
     * allocatd using qvMesgAllocFromModule function.
     */

    sctp_U8bit       *p_resp_buffer;
    sctp_U8bit       *p_resp_message;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( nooctets );
    /*to avoid the warning*/



    p_resp_message = __MESG_MALLOC( SCTP_APIS_MESG_POOL, SCTP_MAX_API_SIZE );

    if( SCTP_NULL == p_resp_message )
    {
        return;
    }

    p_resp_buffer = p_resp_message;


    /* get the header fields */
    api_id = sctp_buffer_get_UshortAtOffset( p_buffer, 0 );

    index_api_id = api_id - SCTP_MIN_SMREQ_APIID;

    __MEMSET( p_resp_buffer, 0, SCTP_MAX_API_SIZE );

    /*  based on api id, call a function that will process the req
     *  and return the response
     */
#ifdef SCTP_RED_SUPPORT
    /* The next 4 bytes for event_type.Another 4 bytes for total size. */
    p_resp_buffer = p_resp_buffer + 8;
#endif

    p_resp_buffer = ( *sctp_ui_proc_sm_api_tbl[index_api_id] )( p_buffer,
                                                                p_resp_buffer );

    noctets = p_resp_buffer - p_resp_message;

#ifdef SCTP_RED_SUPPORT
    /* The next 4 bytes for event_type */
    noctets = noctets - 8;
    p_resp_buffer = p_resp_message + 8;
#else
    p_resp_buffer = p_resp_message;
#endif

    sctp_buffer_set_UlongAtOffset( p_resp_buffer,
                                   APIBUF_LENGTH_OFFSET, ( sctp_U32bit )noctets );

    src_id = sctp_buffer_get_UshortAtOffset( p_resp_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_resp_buffer, 6 );
    p_resp_buffer = p_resp_message;

#ifdef SCTP_RED_SUPPORT
    noctets = noctets + 8;
    sctp_buffer_set_UlongAtOffset( p_resp_buffer, 0, ( sctp_U32bit ) SCTP_EVT_RESP_NOTF );
    sctp_buffer_set_UlongAtOffset( p_resp_buffer, 4, noctets );


#endif

    /* call send_from_sctp from here with the resp buffer    */
    send_from_sctp( p_resp_buffer,
                    noctets,
                    dest_id,
                    src_id,
                    SCTP_CLIENT_SM,
                    from,
                    &ecode );
}

/***************************************************************************
 *  FUNCTION :    sctp_proc_api_init_stack
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_init_stack(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer )
{
    sctp_trans_id_t          transaction_id;
    sctp_src_id_t            src_id;
    sctp_dest_id_t           dest_id;
    sctp_init_options_st     init_option;
    sctp_U32bit              count;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_init_stack_st       init_stack_req;
    sctp_msg_init_stack_res_st   init_stack_resp;
#else
    sctp_return_t            ret;
    sctp_error_t             errorno;
    sctp_U32bit              u32temp;
    sctp_U8bit               hostname[SCTP_MAX_HOSTNAME_LEN];
    sctp_sockaddr_st         *p_addr;
#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_init_stack " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_init_stack_st ), ( sctp_U8bit * )&init_stack_req );

    init_option.max_appl = init_stack_req.max_total_appl;
    init_option.max_endpoint = init_stack_req.max_endpoint;
    init_option.max_tcb = init_stack_req.max_total_assoc;
    init_option.MaxPerAssoc.streams = init_stack_req.max_streams_per_assoc;
    init_option.MaxPerAssoc.min_tx_buffers = init_stack_req.num_tx_min_buffers;
    init_option.MaxPerAssoc.med_tx_buffers = init_stack_req.num_tx_med_buffers;
    init_option.MaxPerAssoc.max_tx_buffers = init_stack_req.num_tx_max_buffers;
    init_option.MaxPerAssoc.min_rx_buffers = init_stack_req.num_rx_min_buffers;
    init_option.MaxPerAssoc.med_rx_buffers = init_stack_req.num_rx_med_buffers;
    init_option.MaxPerAssoc.max_rx_buffers = init_stack_req.num_rx_max_buffers;
    init_option.no_of_local_addrs = init_stack_req.no_of_local_addrs;

    /* Local Addresses */
    for( count = 0; count < init_stack_req.no_of_local_addrs; count++ )
    {
        init_option.list_of_local_addrs[count] =
            init_stack_req.list_local_addrs[count];
    }

    init_option.rand_seed = init_stack_req.seed;
    init_option.min_reserve_buffers = init_stack_req.min_reserve_buffers;
    init_option.med_reserve_buffers = init_stack_req.med_reserve_buffers;
    init_option.max_reserve_buffers = init_stack_req.max_reserve_buffers;
    init_option.capability.stack_type = init_stack_req.stack_type;
    init_option.capability.pmtu_capable = init_stack_req.pmtu_capable;
    init_option.capability.nat_capable = init_stack_req.nat_capable;
    init_option.standard_udp_port = init_stack_req.standard_udp_port;
    init_option.capability.stale_cookie_policy
        = init_stack_req.stale_cookie_policy;
    init_option.capability.ack_policy = init_stack_req.ack_policy;
    init_option.capability.ecn_capable = init_stack_req.ecn_capable;
    init_option.checksum = init_stack_req.checksum;
    init_option.hostname = init_stack_req.hostname;

    /* call the api from functional interface */
    init_stack_resp.result = sctp_api_init_stack( &init_option,
                                                  &init_stack_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_INIT_STACK_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_init_stack_res_st ), ( sctp_U8bit * )&init_stack_resp );

#else

    /* max_total_endpoint */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.max_endpoint );

    /* max_total_assoc */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.max_tcb );

    /* max_streams_per_assoc */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.streams );

    /* num_tx_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.min_tx_buffers );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.med_tx_buffers );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.max_tx_buffers );

    /* num_rx_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.min_rx_buffers );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.med_rx_buffers );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.MaxPerAssoc.max_rx_buffers );

    /* no_of_local_addrs */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.no_of_local_addrs );

    /* seed */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.rand_seed );

    /* reserved_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.min_reserve_buffers );

    /* reserved_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.med_reserve_buffers );

    /* reserved_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &init_option.max_reserve_buffers );

    /* stack_type */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_option.capability.stack_type );

    /* pmtu_capable */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_option.capability.pmtu_capable );

    /* ecn_capable */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_option.capability.ecn_capable );

    /* ecn_capable */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_option.capability.nat_capable );

    /* ecn_capable */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&init_option.checksum );

    /* Local Addresses */
    p_addr = ( sctp_sockaddr_st * ) init_option.list_of_local_addrs;

    for( count = 0; count < init_option.no_of_local_addrs; count++ )
    {
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &p_addr[count] );
    }

    /* udp_port */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &init_option.standard_udp_port );

    /* max_total_applications */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &init_option.max_appl );

    /* stale_cookie_policy */
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &init_option.capability.stale_cookie_policy );

    /* ack_policy */
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &init_option.capability.ack_policy );

    u32temp = strlen( ( const char * )p_req_buffer );


    /* SPR : 18527 */
    if( u32temp >= SCTP_MAX_HOSTNAME_LEN )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "ERROR :: Invalid HostName Length" ) );

        ret = SCTP_FAILURE;
        errorno = EINV_PARAMS;
    }

    else
    {
        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer, u32temp, &hostname[0] );

        hostname[u32temp] = '\0'; /*NULL termination*/

        init_option.hostname = hostname;

        /* call the api from functional interface */
        ret = sctp_api_init_stack( &init_option, &errorno );
    }

    /* SPR : 18527 */

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_INIT_STACK_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *  FUNCTION :    sctp_proc_api_get_entity_version
 *
 *  DESCRIPTION:This function is used to get the stack entity version.
 *
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_entity_version(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer )
{
    sctp_return_t     ret;
    sctp_ver_info_st  ver_info;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_entity_version_res_st  get_entity_version_resp;
#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t  transaction_id;
    sctp_src_id_t    src_id;
    sctp_dest_id_t   dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_stack_entity_version " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    /* call the api from functional interface */
    ret = sctp_api_get_stack_entity_version( &ver_info );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_ENTITY_VERSION_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
#ifdef SCTP_STRUCT_BASED
    get_entity_version_resp.result = ret;
    get_entity_version_resp.major_ver = ver_info.major_ver;
    get_entity_version_resp.minor_ver = ver_info.minor_ver;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_entity_version_res_st ),
                                            ( sctp_U8bit * )&get_entity_version_resp );

#else

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ver_info.major_ver );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ver_info.minor_ver );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}



/***************************************************************************
 *  FUNCTION :    sctp_proc_api_deinit_stack
 *
 *  DESCRIPTION:This api processes deinit of the stack
 *
 *  RETURN VALUE:
 *    pointer     to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_deinit_stack(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_deinit_stack_st      deinit_stack_req;
    sctp_msg_deinit_stack_res_st  deinit_stack_resp;

#else

    sctp_return_t      ret;
    sctp_U32bit        force;
    sctp_error_t       errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_deinit_stack" ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_deinit_stack_st ),
                                           ( sctp_U8bit * )&deinit_stack_req );

    /* call the api from    functional interface */
    deinit_stack_resp.result = sctp_api_deinit_stack(
                                   deinit_stack_req.force,
                                   &( deinit_stack_resp.errorno ) );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DEINIT_STACK_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_deinit_stack_res_st ),
                                            ( sctp_U8bit * )&deinit_stack_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &force );

    /* call the api from functional interface */
    ret = sctp_api_deinit_stack( force, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DEINIT_STACK_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}





/***************************************************************************
 *  FUNCTION :    sctp_proc_api_config_stack_addrs
 *
 *  DESCRIPTION: This function processes the config stack address api.

 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_proc_api_config_stack_addrs(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_config_stack_addrs_st    config_req;
    sctp_msg_config_stack_addrs_res_st  config_resp;
#else
    sctp_return_t          ret;
    sctp_error_t           ecode;
    sctp_U32bit            count, no_of_addrs;
    sctp_sockaddr_list_st  list_addrs;
    sctp_U32bit            flags;
    sctp_sockaddr_st       *p_addr;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_bindx " ) );


#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_stack_addrs_st ), ( sctp_U8bit * ) & ( config_req ) );

    config_resp.result = sctp_api_config_stack_addresses(
                             config_req.flags, config_req.no_of_addrs,
                             config_req.list_addrs, &config_resp.errorno );


    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_STACK_ADDRS_RESP, transaction_id,
                                              dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( config_resp ), ( sctp_U8bit * )&config_resp );

#else

    p_addr = ( sctp_sockaddr_st * )list_addrs;

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &flags );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &no_of_addrs );

    for( count = 0; count < no_of_addrs; count++ )
    {
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &( p_addr[count] ) );
    }

    ret = sctp_api_config_stack_addresses( flags, count,
                                           list_addrs, &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_STACK_ADDRS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_SRTRUCT_BASED */

    return( p_resp_buffer );
}




/***************************************************************************
 *  FUNCTION :    sctp_proc_api_config_stack_params
 *
 *  DESCRIPTION:
 *    This function processes the config stack params api.
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_proc_api_config_stack_params(
    sctp_U8bit   *p_req_buffer,
    sctp_U8bit   *p_resp_buffer )
{
    sctp_prot_params_st     proto_params;

#ifdef SCTP_STRUCT_BASED

    sctp_msg_config_stack_params_st      config_stack_params_req;
    sctp_msg_config_stack_params_res_st  config_stack_params_resp;

#else

    sctp_return_t              ret;
    sctp_error_t               errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t            transaction_id;
    sctp_src_id_t              src_id;
    sctp_dest_id_t             dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_stack_params " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_stack_params_st ),
                                           ( sctp_U8bit * )&config_stack_params_req );

    proto_params.rto_init = config_stack_params_req.rto_init;
    proto_params.rto_min = config_stack_params_req.rto_min;
    proto_params.rto_max = config_stack_params_req.rto_max;
    proto_params.valid_cookie_life = config_stack_params_req.valid_cookie_life;
    proto_params.assoc_max_retrans = config_stack_params_req.assoc_max_retrans;
    proto_params.path_max_retrans = config_stack_params_req.path_max_retrans;
    proto_params.max_init_retrans = config_stack_params_req.max_init_retrans;
    proto_params.max_init_path_retrans = config_stack_params_req.max_init_path_retrans;
    proto_params.init_timeo = config_stack_params_req.init_timeo;
    proto_params.hb_interval = config_stack_params_req.heartbeat_interval;
    proto_params.hb_info_length = config_stack_params_req.hb_info_length;
    proto_params.rto_alpha_percent = config_stack_params_req.rto_alpha_percent;
    proto_params.rto_beta_percent = config_stack_params_req.rto_beta_percent;
    proto_params.num_in_streams = config_stack_params_req.num_in_streams;
    proto_params.num_out_streams = config_stack_params_req.num_out_streams;
    proto_params.bundling_time = config_stack_params_req.bundling_time;
    proto_params.sack_period = config_stack_params_req.sack_period;
    proto_params.sack_frequency = config_stack_params_req.sack_frequency;
    proto_params.pmtu_capable = config_stack_params_req.pmtu_capable;
    proto_params.mtu_size = config_stack_params_req.mtu_size;
    proto_params.hb_max_burst = config_stack_params_req.hb_max_burst;
#ifdef SCTP_IP_HEADER_PARAMS
    proto_params.ip_header_params.ttl = config_stack_params_req.ip_header_params.ttl;
    proto_params.ip_header_params.tos = config_stack_params_req.ip_header_params.tos;

#ifdef SCTP_INET6_SUPPORT
    proto_params.ip_header_params.hoplimit = config_stack_params_req.ip_header_params.hoplimit;
    proto_params.ip_header_params.tclass = config_stack_params_req.ip_header_params.tclass;
#endif  /*SCTP_INET6_SUPPORT */

#endif


    /* call the api from    functional interface */
    config_stack_params_resp.result = sctp_api_config_stack_params(
                                          &proto_params,
                                          &config_stack_params_resp.errorno );
    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_STACK_PARAMS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_config_stack_params_res_st ),
                                            ( sctp_U8bit * )&config_stack_params_resp );

#else
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.rto_init );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.rto_min );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.rto_max );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.valid_cookie_life );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.assoc_max_retrans );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.path_max_retrans );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.max_init_retrans );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.max_init_path_retrans );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.init_timeo );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.hb_interval );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.hb_info_length );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.rto_alpha_percent );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.rto_beta_percent );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.num_in_streams );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.num_out_streams );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.bundling_time );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.sack_period );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.sack_frequency );
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.pmtu_capable );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&proto_params.pmtu_capable );
#endif
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.mtu_size );

#ifdef SCTP_IP_HEADER_PARAMS
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &proto_params.ip_header_params.ttl );

    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &proto_params.ip_header_params.tos );

#ifdef SCTP_INET6_SUPPORT
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.ip_header_params.hoplimit );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &proto_params.ip_header_params.tclass );
#endif  /*SCTP_INET6_SUPPORT */

#endif /* SCTP_IP_HEADER_PARAMS */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &proto_params.hb_max_burst );

    /* call the api from    functional interface */
    ret = sctp_api_config_stack_params( &proto_params, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_STACK_PARAMS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_init_stats
 *
 *  DESCRIPTION:
 *    This function processes init stats api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *  RETURN VALUE:
 *    pointer     to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_init_stats(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_init_stats_st      init_stats_req;
    sctp_msg_init_stats_res_st  init_stats_resp;

#else

    sctp_return_t      ret;
    sctp_stats_et      stats_type;
    sctp_error_t       errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_init_stats " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_init_stats_st ),
                                           ( sctp_U8bit * )&init_stats_req );

    /* call the api from    functional interface */
    init_stats_resp.result = sctp_api_init_stats(
                                 init_stats_req.stats_type,
                                 &init_stats_resp.errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_INIT_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_init_stats_res_st ),
                                            ( sctp_U8bit * )&init_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stats_type );

    /* call the api from functional interface */
    ret = sctp_api_init_stats( stats_type, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_INIT_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}



/***************************************************************************
 *  FUNCTION :  sctp_proc_api_enable_stats
 *
 *  DESCRIPTION:
 *    This function processes enable stats api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_enable_stats(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_enable_stats_st      enable_stats_req;
    sctp_msg_enable_stats_res_st  enable_stats_resp;

#else

    sctp_return_t      ret;
    sctp_stats_et      stats_type;
    sctp_error_t       errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_enable_stats " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_enable_stats_st ),
                                           ( sctp_U8bit * )&enable_stats_req );

    /* call the api from functional interface */
    enable_stats_resp.result = sctp_api_enable_stats(
                                   enable_stats_req.stats_type,
                                   &enable_stats_resp.errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ENABLE_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_enable_stats_res_st ),
                                            ( sctp_U8bit * )&enable_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stats_type );

    /* call the api from functional interface */
    ret = sctp_api_enable_stats( stats_type, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ENABLE_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :  sctp_proc_api_disable_stats
 *
 *  DESCRIPTION:
 *     This function processes disable stats api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.
 *   RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_proc_api_disable_stats(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_disable_stats_st      disable_stats_req;
    sctp_msg_disable_stats_res_st  disable_stats_resp;

#else

    sctp_return_t        ret;
    sctp_stats_et        stats_type;
    sctp_error_t         errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t         src_id;
    sctp_dest_id_t     dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_disable_stats    " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_disable_stats_st ),
                                           ( sctp_U8bit * )&disable_stats_req );

    /* call the api from functional interface */
    disable_stats_resp.result = sctp_api_disable_stats(
                                    disable_stats_req.stats_type,
                                    &disable_stats_resp.errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DISABLE_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_disable_stats_res_st ),
                                            ( sctp_U8bit * )&disable_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stats_type );


    /* call the api from    functional interface */
    ret = sctp_api_disable_stats( stats_type, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DISABLE_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :  sctp_proc_api_get_stats
 *
 *  DESCRIPTION:
 *     This function processes get stats api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.

 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_stats(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_get_stats_st      get_stats_req;
    sctp_msg_get_stats_res_st  get_stats_resp;

#else

    sctp_return_t         ret;
    sctp_stats_et         stats_type;
    sctp_U32bit           association_id;
    sctp_error_t          errorno;
    sctp_stats_data_ut    st_data;
    sctp_U32bit           count;
    sctp_U32bit           reset_flag;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t         src_id;
    sctp_dest_id_t     dest_id;


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_stats " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_stats_st ),
                                           ( sctp_U8bit * )&get_stats_req );


    /* call the api from functional interface */
    get_stats_resp.result = sctp_api_get_stats(
                                get_stats_req.stats_type,
                                get_stats_req.association_id,
                                ( sctp_U8bit * )&get_stats_resp.stats_data, get_stats_req.reset_flag,
                                &get_stats_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    get_stats_resp.association_id = get_stats_req.association_id;
    get_stats_resp.stats_type = get_stats_req.stats_type;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_stats_res_st ),
                                            ( sctp_U8bit * )&get_stats_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stats_type );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&reset_flag );
    /* call the api from functional interface */
    ret = sctp_api_get_stats( stats_type, association_id,
                              ( sctp_U8bit * )&st_data, reset_flag, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STATS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                           ( sctp_U32bit )stats_type );

    if( SCTP_SUCCESS == ret )
    {
        switch( stats_type )
        {
            case SCTP_PROT_INTERNAL_STATS:
                {
                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.prot_stats.no_applications_reg );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.prot_stats.no_associations_open );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.prot_stats.no_dest_marked_active );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.prot_stats.no_dest_marked_inactive );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.prot_stats.no_endpoints_marked_up );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.prot_stats.no_endpoints_marked_down );

                    break;
                }

            case SCTP_GLOBAL_TRAFFIC_STATS:
                {
                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_assoc_accepted );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_assoc_initiated );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_graceful_shutdown_assocs );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_ungraceful_shutdown_assocs );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_trans_data_chunks );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_retrans_data_chunks );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_data_chunks_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_ord_data_chunks_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_unord_data_chunks_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_data_chunks_unacked );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_data_chunks_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_ord_data_chunks_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_unord_data_chunks_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_bytes_sent_ok );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_bytes_recvd_ok );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_init_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_init_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_initack_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_initack_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_cookie_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_cookie_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_cookieack_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_cookieack_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_hb_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_hb_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_hback_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_hback_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_sack_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_sack_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_abort_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_abort_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_shut_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_shut_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_shutack_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_shutack_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_shutcom_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_shutcom_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_error_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_error_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_ecne_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_ecne_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_cwr_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_cwr_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_datagrams_recvd_in_error );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_chk_sum_err_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_ootb_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_frag_usr_msg );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_reassemble_usr_msg );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_packets_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_packets_recv );

                    /* CSR 1-4850698 */
                    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                            sizeof( sctp_time_t ),
                                                            ( sctp_U8bit * )&st_data.traf_stats.discont_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT
                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_asconf_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_asconf_recv );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_asconfack_sent );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.traf_stats.no_asconfack_recv );
#endif
                    /* CSR 1-4850698 end */

                    break;
                }

            case SCTP_ERROR_STATS:
                {
                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.error_stats.API_error_stats_count );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.error_stats.protcol_error_stats_count );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.error_stats.system_error_stats_count );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.error_stats.system_resource_error_stats_count );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.error_stats.internal_error_stats_count );

                    break;
                }

            case SCTP_ASSOC_STATS:
                {
                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.assoc_id );

                    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                            sizeof( sctp_time_t ),
                                                            ( sctp_U8bit * )&st_data.assoc_stats.start_time );

                    /* CSR 1-4850698 */
                    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                            sizeof( sctp_time_t ),
                                                            ( sctp_U8bit * )&st_data.assoc_stats.discont_time );
                    /* CSR 1-4850698 end */

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.no_chunks_retrans );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.no_T1_expired );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.no_T2_expired );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.no_dest_address );

                    for( count = 0;
                            count < st_data.assoc_stats.no_dest_address;
                            count++ )
                    {
                        p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                                   &st_data.assoc_stats.dest_stats[count].dest_address );

                        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                               st_data.assoc_stats.dest_stats[count].
                                                               no_datagrams_sent );

                        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                               st_data.assoc_stats.dest_stats[count].
                                                               no_datagrams_unacked );

                        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                               st_data.assoc_stats.dest_stats[count].
                                                               no_datagrams_recd );

                        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                               st_data.assoc_stats.dest_stats[count].no_bytes_sent );

                        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                               st_data.assoc_stats.dest_stats[count].no_bytes_recvd );

                        /* CSR 1-4850698 */
                        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                                sizeof( sctp_time_t ),
                                                                ( sctp_U8bit * )&st_data.assoc_stats.dest_stats[count].dest_addr_start_time );
                        /* CSR 1-4850698 end */

                    }

                    /* CSR 1-4850698 */

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.buffer_stat.tx_med_utilized_instead_min );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.buffer_stat.tx_max_utilized_instead_min );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.buffer_stat.tx_max_utilized_instead_med );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.buffer_stat.rx_med_utilized_instead_min );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.buffer_stat.rx_max_utilized_instead_min );

                    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                           st_data.assoc_stats.buffer_stat.rx_max_utilized_instead_med );
                    /* CSR 1-4850698 end */
                    break;
                }

            case SCTP_ALL_STATS:
                {
                    break;
                }

        }
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *  FUNCTION :  sctp_proc_api_get_error_level
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_error_level(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_get_error_level_st      get_error_level_req;
    sctp_msg_get_error_level_res_st  get_error_level_resp;

#else

    sctp_U32bit          ret;
    sctp_error_flag_et   error_flag;
    /* SPR : 18527 */
    sctp_error_level_et  error_level = 0;
    /* SPR : 18527 */
    sctp_error_t         errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t     transaction_id;
    sctp_src_id_t       src_id;
    sctp_dest_id_t      dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_error_level " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    /* SPR : 18527 */
    get_error_level_resp.error_level = 0;
    /* SPR : 18527 */

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_error_level_st ),
                                           ( sctp_U8bit * )&get_error_level_req );

    /* call the api from    functional interface */
    get_error_level_resp.result = sctp_api_get_error_level(
                                      get_error_level_req.error_flag,
                                      &get_error_level_resp.error_level,
                                      &get_error_level_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_ERROR_LEVEL_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_error_level_res_st ),
                                            ( sctp_U8bit * )&get_error_level_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&error_flag );

    /* call the api from    functional interface */
    ret = sctp_api_get_error_level( error_flag, &error_level, &errorno );


    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_ERROR_LEVEL_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                           ( sctp_U32bit )error_level );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :  sctp_proc_api_set_error_level
 *
 *  DESCRIPTION:
 *    This function processes set error level api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms the
 *    resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_set_error_level(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_set_error_level_st      set_error_level_req;
    sctp_msg_set_error_level_res_st  set_error_level_resp;

#else

    sctp_U32bit           ret;
    sctp_error_flag_et    error_flag;
    sctp_error_level_et   error_level;
    sctp_error_t          errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_error_level " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_error_level_st ),
                                           ( sctp_U8bit * )&set_error_level_req );

    /* call the api from functional interface */
    set_error_level_resp.result = sctp_api_set_error_level(
                                      set_error_level_req.error_flag,
                                      set_error_level_req.error_level,
                                      &set_error_level_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_ERROR_LEVEL_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_error_level_res_st ),
                                            ( sctp_U8bit * )&set_error_level_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&error_flag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&error_level );

    /* call the api from functional interface */
    ret = sctp_api_set_error_level( error_flag, error_level, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_ERROR_LEVEL_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :  sctp_proc_api_enable_assoc_trc
 *
 *  DESCRIPTION:
 *    This function processes enable assoc trace api.It takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts
 *    the api parameters from the buffer and invokes the functional api,
 *    forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_enable_assoc_trc(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_enable_assoc_trc_st       enable_assoc_trc_req;
    sctp_msg_enable_assoc_trc_res_st   enable_assoc_trc_resp;

#else

    sctp_return_t        ret;
    sctp_U32bit          assoc_id;
    sctp_trc_et          trace_flag;
    sctp_error_t         errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_enable_assos_trc " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_enable_assoc_trc_st ),
                                           ( sctp_U8bit * )&enable_assoc_trc_req );

    /* call the api from functional interface */
    enable_assoc_trc_resp.result = sctp_api_enable_assoc_trc(
                                       enable_assoc_trc_req.assoc_id,
                                       enable_assoc_trc_req.trace_flag,
                                       &enable_assoc_trc_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ENABLE_ASSOC_TRC_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    enable_assoc_trc_resp.assoc_id = enable_assoc_trc_req.assoc_id;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_enable_assoc_trc_res_st ),
                                            ( sctp_U8bit * )&enable_assoc_trc_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &assoc_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&trace_flag );


    /* call the api from functional interface */
    ret = sctp_api_enable_assoc_trc( assoc_id, trace_flag, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ENABLE_ASSOC_TRC_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *  FUNCTION :  sctp_proc_api_disable_assoc_trc
 *
 *  DESCRIPTION:
 *    This function disables assoc trace api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_disable_assoc_trc(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_disable_assoc_trc_st       disable_assoc_trc_req;
    sctp_msg_disable_assoc_trc_res_st   disable_assoc_trc_resp;

#else

    sctp_return_t        ret;
    sctp_U32bit          assoc_id;
    sctp_trc_et          trace_flag;
    sctp_error_t         errorno;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_disable_assos_trc " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_disable_assoc_trc_st ),
                                           ( sctp_U8bit * )&disable_assoc_trc_req );

    /* call the api from    functional interface */
    disable_assoc_trc_resp.result = sctp_api_disable_assoc_trc(
                                        disable_assoc_trc_req.assoc_id,
                                        disable_assoc_trc_req.trace_flag,
                                        &disable_assoc_trc_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DISABLE_ASSOC_TRC_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    disable_assoc_trc_resp.assoc_id = disable_assoc_trc_req.assoc_id;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_disable_assoc_trc_res_st ),
                                            ( sctp_U8bit * )&disable_assoc_trc_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &assoc_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&trace_flag );

    /* call the api from    functional interface */
    ret = sctp_api_disable_assoc_trc( assoc_id, trace_flag, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DISABLE_ASSOC_TRC_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_get_trace_info
 *
 *  DESCRIPTION:
 *    This function processes get trace info api.it takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms the
 *    resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_trace_info(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_get_trace_info_st      get_trace_info_req;
    sctp_msg_get_trace_info_res_st  get_trace_info_resp;

#else

    sctp_U8bit         ret;
    sctp_U32bit        assoc_id;
    /* SPR : 18527 */
    sctp_U8bit         trace_info = 0;
    /* SPR : 18527 */
    sctp_error_t       errorno;

#endif /* SCTP_STRUCT_BASED */


    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_trace_info " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    /* SPR : 18527 */
    get_trace_info_resp.trace_info = 0;
    /* SPR : 18527 */

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_trace_info_st ),
                                           ( sctp_U8bit * )&get_trace_info_req );

    /* call the api from functional interface */
    get_trace_info_resp.result = sctp_api_get_trace_info(
                                     get_trace_info_req.assoc_id,
                                     &get_trace_info_resp.trace_info,
                                     &get_trace_info_resp.errorno );

    get_trace_info_resp.assoc_id = get_trace_info_req.assoc_id;
    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_TRACE_INFO_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_trace_info_res_st ),
                                            ( sctp_U8bit * )&get_trace_info_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &assoc_id );
    /* call the api from functional interface */
    ret = sctp_api_get_trace_info( assoc_id, &trace_info, &errorno );


    /* now form the response buf */
    /* build header , interchange    src and    dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_TRACE_INFO_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, trace_info );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );


#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );

}
/* Start SPR 20526 */
/***************************************************************************
 *  FUNCTION :  sctp_proc_api_get_rtt_info
 *
 *  DESCRIPTION:
 *    This function processes get rtt info api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 *************************************************************************/

sctp_U8bit *
sctp_proc_api_get_rtt_info(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_rtt_info_st      get_rtt_info_req;
    sctp_msg_get_rtt_info_res_st  get_rtt_info_resp;

#else
    sctp_return_t                   ret;
    sctp_error_t                    ecode;
    sctp_U32bit                     association_id;
    sctp_Boolean_t                  reset_flag;
    sctp_U32bit                     nos_of_dest_trans_addrs, count;
    sctp_sockaddr_list_st           list_of_dest_trans_addrs;
    sctp_rtt_info_list_st           dest_rtt_param_list;
    sctp_sockaddr_st                primary_dest_transport_addr;

    ecode = E_NOERROR ;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    __MEMSET( &get_rtt_info_resp.list_of_dest_trans_addrs, 0,
              sizeof( sctp_sockaddr_list_st ) );
    __MEMSET( &get_rtt_info_resp.dest_rtt_param_list, 0,
              sizeof( sctp_rtt_info_list_st ) );
    __MEMSET( &get_rtt_info_resp.primary_dest_transport_addr, 0,
              sizeof( sctp_sockaddr_st ) );
    get_rtt_info_resp.errorno = E_NOERROR;

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_rtt_info_st ), ( sctp_U8bit * )&get_rtt_info_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_rtt_info " ) );

    /* call the api from functional interface */
    get_rtt_info_resp.result =  sctp_api_get_rtt_info(
                                    get_rtt_info_req.association_id,
                                    &get_rtt_info_resp.nos_of_dest_trans_addrs,
                                    get_rtt_info_resp.list_of_dest_trans_addrs,
                                    &get_rtt_info_resp.dest_rtt_param_list,
                                    &get_rtt_info_resp.primary_dest_transport_addr,
                                    get_rtt_info_req.reset_flag,
                                    &get_rtt_info_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_RTT_INFO_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    get_rtt_info_resp.association_id = get_rtt_info_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_rtt_info_res_st ),
                                            ( sctp_U8bit * )&get_rtt_info_resp );

#else

    __MEMSET( &list_of_dest_trans_addrs, 0, sizeof( sctp_sockaddr_list_st ) );
    __MEMSET( &dest_rtt_param_list, 0, sizeof( sctp_rtt_info_list_st ) );
    __MEMSET( &primary_dest_transport_addr, 0,
              sizeof( primary_dest_transport_addr ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &reset_flag );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_rtt_info " ) );

    /* call the api from functional interface */
    ret = sctp_api_get_rtt_info(
              association_id,
              &nos_of_dest_trans_addrs,
              list_of_dest_trans_addrs,
              &dest_rtt_param_list    ,
              &primary_dest_transport_addr,
              reset_flag,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_RTT_INFO_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               nos_of_dest_trans_addrs );

        for( count = 0; count < nos_of_dest_trans_addrs; count++ )
        {
            p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                       &list_of_dest_trans_addrs[count] );
        }

        for( count = 0; count < nos_of_dest_trans_addrs; count++ )
        {
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_rtt_param_list[count].min_rtt );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_rtt_param_list[count].max_rtt );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_rtt_param_list[count].last_recorded_rtt );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_rtt_param_list[count].srtt );
        }

        p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                   &primary_dest_transport_addr );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}
/* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_set_stack_red_state
 *
 *  DESCRIPTION:
 *    This function set red state api.It takes a buffer pointer containing
 *    the buffer recvd from the client. It extracts the api parameters
 *    from the buffer and invokes the functional api, forms the resp and
 *    returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_set_stack_red_state(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_set_stack_red_state_st        set_stack_red_state_req;
    sctp_msg_set_stack_red_state_res_st    set_stack_red_state_resp;

#else

    sctp_return_t             ret;
    sctp_error_t              errorno;
    sctp_stack_red_state_et   stack_red_state;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_stack_red_state " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_stack_red_state_st ),
                                           ( sctp_U8bit * )&set_stack_red_state_req );


    /* call the api from functional interface */
    set_stack_red_state_resp.result = sctp_api_set_red_state(
                                          set_stack_red_state_req.stack_state,
                                          &set_stack_red_state_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_STACK_RED_STATE_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_stack_red_state_res_st ),
                                            ( sctp_U8bit * )&set_stack_red_state_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_red_state );


    /* call the api from    functional interface */
    ret = sctp_api_set_red_state( stack_red_state, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_STACK_RED_STATE_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_set_stack_proc_state
 *
 *  DESCRIPTION:
 *     This functionprocesses set state api.It takes a buffer pointer containing
 *     the buffer recvd from the client. It extracts the api parameters from the
 *     buffer and invokes the functional api, forms the resp and returns it
 *  RETURN VALUE:
 *    pointer to buffer
 **
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_set_stack_proc_state(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_set_stack_proc_state_st        set_stack_proc_state_req;
    sctp_msg_set_stack_proc_state_res_st   set_stack_proc_state_resp;

#else

    sctp_U32bit                 seed;
    sctp_return_t               ret;
    sctp_error_t                errorno;
    sctp_stack_proc_state_et    stack_proc_state;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_stack_proc_state " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_stack_proc_state_st ),
                                           ( sctp_U8bit * )&set_stack_proc_state_req );


    /* call the api from functional interface */
    set_stack_proc_state_resp.result = sctp_api_set_proc_state(
                                           set_stack_proc_state_req.stack_state,
                                           set_stack_proc_state_req.stack_seed,
                                           &set_stack_proc_state_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_STACK_PROC_STATE_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_stack_proc_state_res_st ),
                                            ( sctp_U8bit * )&set_stack_proc_state_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_proc_state );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &seed );

    /* call the api from functional interface */
    ret = sctp_api_set_proc_state( stack_proc_state, seed, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_STACK_PROC_STATE_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_get_stack_red_db
 *
 *  DESCRIPTION:
 *    This function get red db api.It takes a buffer pointer containing
 *    the buffer recvd from the client. It extracts the api parameters
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_stack_red_db(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_get_stack_red_db_res_st   get_stack_red_db_resp;

#else

    sctp_return_t              ret;
    sctp_error_t               errorno;
    sctp_red_stack_db_st       stack_db;
    sctp_U32bit                count;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_stack_red_db " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    /* call the api from    functional interface */
    get_stack_red_db_resp.result = sctp_api_get_stack_red_db(
                                       &get_stack_red_db_resp.stack_red_db,
                                       &get_stack_red_db_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STACK_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_stack_red_db_res_st ),
                                            ( sctp_U8bit * )&get_stack_red_db_resp );

#else

    __MEMSET( &stack_db, 0, sizeof( stack_db ) );

    /* call the api from functional interface */
    ret = sctp_api_get_stack_red_db( &stack_db, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STACK_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );
    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );


    if( SCTP_SUCCESS == ret )
    {
        /* stack red db*/

        /* max_appl */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, stack_db.max_appl );
        /* num_active_appl */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.num_active_appl );
        /* max_endpoint */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, stack_db.max_ep );
        /* num_active_ep */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.num_active_ep );
        /* last_allocated_ep */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.last_allocated_ep );
        /* max_assoc */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.max_assoc );
        /* tx_max_buffers */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.min_tx_buffers );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.med_tx_buffers );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.max_tx_buffers );
        /* rx_max_buffers */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.min_rx_buffers );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.med_rx_buffers );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.max_rx_buffers );

        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.min_reserve_buffers );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.med_reserve_buffers );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.max_reserve_buffers );
        /* Rel 6.2 */

        /* max_streams_per_assoc */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.max_streams_per_assoc );

        /* num_local_addrs */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.num_local_addrs );

        /* active_ep_list */
        for( count = 0; count < stack_db.num_active_ep; count++ )
        {
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   stack_db.active_ep_list[count] );
        }

        /* pmtu_timer_rem_time */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.pmtu_timer_rem_time );

        /* num_active_pmtu_nodes */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.num_active_pmtu_nodes );

        /* pmtu_database */
        for( count = 0; count < stack_db.num_active_pmtu_nodes; count++ )
        {
            p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                    sizeof( sctp_time_t ),
                                                    ( sctp_U8bit * )&stack_db.pmtu_database[count].time_stamp );

            p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                       &stack_db.pmtu_database[count].addr );

            p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                    stack_db.pmtu_database[count].mtu_size );

            p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                    stack_db.pmtu_database[count].spare );
        }

#ifdef SCTP_ERROR_ENABLED

        /* sctp_error_table */
        for( count = SCTP_MIN_ERROR_FLAG; count <= SCTP_MAX_ERROR_FLAG; count++ )
        {
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   ( sctp_U32bit )stack_db.sctp_error_table[count] );
        }

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_STATS_ENABLED

        /* sctp_stats_state */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               ( sctp_U32bit )stack_db.sctp_stats_state );
        /* sctp_protocol_stats */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_protocol_stats.no_applications_reg );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_protocol_stats.no_associations_open );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_protocol_stats.no_dest_marked_active );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_protocol_stats.no_dest_marked_inactive );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_protocol_stats.no_endpoints_marked_up );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_protocol_stats.no_endpoints_marked_down );

        /* sctp_global_traffic_stats */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_assoc_accepted );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_assoc_initiated );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_graceful_shutdown_assocs );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_ungraceful_shutdown_assocs );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_trans_data_chunks );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_retrans_data_chunks );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_data_chunks_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_ord_data_chunks_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_unord_data_chunks_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_data_chunks_unacked );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_data_chunks_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_ord_data_chunks_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_unord_data_chunks_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_bytes_sent_ok );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_bytes_recvd_ok );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_init_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_init_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_initack_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_initack_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_cookie_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_cookie_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_cookieack_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_cookieack_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_hb_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_hb_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_hback_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_hback_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_sack_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_sack_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_abort_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_abort_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_shut_sent );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_shut_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_shutack_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_shutack_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_shutcom_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_shutcom_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_error_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_error_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_ecne_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_ecne_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_cwr_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_cwr_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_datagrams_recvd_in_error );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_chk_sum_err_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_ootb_recv );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_frag_usr_msg );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_reassemble_usr_msg );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_packets_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_packets_recv );

        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                sizeof( sctp_time_t ),
                                                ( sctp_U8bit * )&stack_db.sctp_global_traffic_stats.discont_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_asconf_sent );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_asconf_recv );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_asconfack_sent );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_global_traffic_stats.no_asconfack_recv );
#endif
        /* Rel 6.2 Ends Here. */

        /* sctp_error_stats */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_error_stats.API_error_stats_count );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_error_stats.protcol_error_stats_count );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_error_stats.system_error_stats_count );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_error_stats.system_resource_error_stats_count );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_error_stats.internal_error_stats_count );

        /* sctp_stats_table */
        for( count = 0; count < SCTP_ALL_STATS; count++ )
        {
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   ( sctp_U32bit )stack_db.sctp_stats_table[count] );
        }

#endif /* SCTP_STATS_ENABLED */

        /* active_appl_list */
        for( count = 0; count < stack_db.max_appl; count++ )
        {
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   stack_db.active_appl_list[count].open );
            /* SPR FIX 20540 Start --- */
            p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                    stack_db.active_appl_list[count].appl_id );
            /* SPR FIX 20540 End --- */
            p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                    stack_db.active_appl_list[count].su_id );
        }

        /* addr_list */
        for( count = 0; count < stack_db.num_local_addrs; count++ )
        {
            p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                       &stack_db.addr_list[count] );
        }

        /* sctp_def_prot_params */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.rto_init );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.rto_min );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.rto_max );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.max_burst );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.valid_cookie_life );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.assoc_max_retrans );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.hb_interval );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.path_max_retrans );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.max_init_retrans );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.max_init_path_retrans );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.init_timeo );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.bundling_time );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.sack_period );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.sack_frequency );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.adaption_ind );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.maxseg );

        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.hb_info_length );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.mtu_size );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.rto_alpha_percent );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.rto_beta_percent );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.num_in_streams );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.num_out_streams );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.re_use_addr );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.dont_fragment );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.want_mapped );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.nat_capable );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.allow_asconf );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.pmtu_capable );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.checksum_func );

        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.ppi );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.hb_max_burst );

#ifdef SCTP_IP_HEADER_PARAMS
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.ip_header_params.ttl );
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.sctp_def_prot_params.ip_header_params.tos );
#ifdef SCTP_INET6_SUPPORT
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.ip_header_params.hoplimit );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.sctp_def_prot_params.ip_header_params.tclass );
#endif
#endif
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.high_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.high_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.med_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.med_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.low_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.low_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.no_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.trx_cong_reg_def.no_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.high_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.high_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.med_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.med_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.low_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.low_cong_lvl_abate );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.no_cong_lvl_onset );

        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.rcv_cong_reg_def.no_cong_lvl_abate );
        /* Rel 6.2 Ends here. */

        /* sctp_stack_init_parameters */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               ( sctp_U32bit )stack_db.sctp_stack_init_parameters.ecn_capable );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               ( sctp_U32bit )stack_db.sctp_stack_init_parameters.pmtu_capable );
        /* Rel 6.2 */
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               ( sctp_U32bit )stack_db.sctp_stack_init_parameters.nat_capable );
        /* Rel 6.2 Ends Here */
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.sctp_stack_init_parameters.ack_policy );
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.sctp_stack_init_parameters.stale_cookie_policy );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               ( sctp_U32bit )stack_db.sctp_stack_init_parameters.stack_type );

        /* init_in_streams */
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.init_in_streams );

        /* init_out_streams */
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.init_out_streams );

        /* standard_udp_port */
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                                stack_db.standard_udp_port );

#ifdef SCTP_ERROR_ENABLED
        /* sctp_error_state */
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.sctp_error_state );

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_DEBUG_ENABLED
        /* sctp_trace_state */
        p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer,
                                               stack_db.sctp_gbl_trc );
#endif /* SCTP_DEBUG_ENABLED */

        /*Old mac keys*/
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer, SCTP_KEYSIZE,
                                                &stack_db.old_mac_key[0] );

        /*Current mac keys*/
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer, SCTP_KEYSIZE,
                                                &stack_db.current_mac_key[0] );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :  sctp_proc_api_set_stack_red_db
 *
 *  DESCRIPTION:
 *    This function processes set red db api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_set_stack_red_db(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
#ifdef SCTP_STRUCT_BASED

    sctp_msg_set_stack_red_db_st       set_stack_red_db_req;
    sctp_msg_set_stack_red_db_res_st   set_stack_red_db_resp;

#else

    sctp_return_t              ret;
    sctp_error_t               errorno;
    sctp_red_stack_db_st       stack_db;
    sctp_U32bit                count;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_stack_red_db " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_stack_red_db_st ),
                                           ( sctp_U8bit * )&set_stack_red_db_req );

    /* call the api from    functional interface */
    set_stack_red_db_resp.result = sctp_api_set_stack_red_db(
                                       &set_stack_red_db_req.stack_red_db,
                                       &set_stack_red_db_resp.errorno );


    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_STACK_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_stack_red_db_res_st ),
                                            ( sctp_U8bit * )&set_stack_red_db_resp );

#else

    /* max_appl */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &stack_db.max_appl );
    /* num_active_appl */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.num_active_appl );
    /* max_ep */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &stack_db.max_ep );
    /* num_active_ep */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.num_active_ep );
    /* last_allocated_ep */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.last_allocated_ep );
    /* max_assoc */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &stack_db.max_assoc );
    /* tx_max_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.min_tx_buffers );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.med_tx_buffers );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.max_tx_buffers );
    /* rx_max_buffers */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.min_rx_buffers );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.med_rx_buffers );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.max_rx_buffers );

    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.min_reserve_buffers );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.med_reserve_buffers );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.max_reserve_buffers );
    /* Rel 6.2 */

    /* max_streams_per_assoc */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.max_streams_per_assoc );
    /* num_local_addrs */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.num_local_addrs );

    /* active_ep_list */
    for( count = 0; count < stack_db.num_active_ep; count++ )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &stack_db.active_ep_list[count] );
    }

    /* pmtu_timer_rem_time */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.pmtu_timer_rem_time );
    /* num_active_pmtu_nodes */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.num_active_pmtu_nodes );

    /* pmtu_database */
    for( count = 0; count < stack_db.num_active_pmtu_nodes; count++ )
    {
        p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                               sizeof( sctp_time_t ),
                                               ( sctp_U8bit * )&stack_db.pmtu_database[count].time_stamp );
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &stack_db.pmtu_database[count].addr );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &stack_db.pmtu_database[count].mtu_size );
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &stack_db.pmtu_database[count].spare );
    }

#ifdef SCTP_ERROR_ENABLED

    /* sctp_error_table */
    for( count = SCTP_MIN_ERROR_FLAG; count <= SCTP_MAX_ERROR_FLAG; count++ )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&stack_db.sctp_error_table[count] );
    }

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_STATS_ENABLED

    /* sctp_stats_state */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_db.sctp_stats_state );
    /* sctp_protocol_stats */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_protocol_stats.no_applications_reg );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_protocol_stats.no_associations_open );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_protocol_stats.no_dest_marked_active );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_protocol_stats.no_dest_marked_inactive );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_protocol_stats.no_endpoints_marked_up );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_protocol_stats.no_endpoints_marked_down );

    /* sctp_global_traffic_stats */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_assoc_accepted );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_assoc_initiated );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_graceful_shutdown_assocs );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_ungraceful_shutdown_assocs );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_trans_data_chunks );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_retrans_data_chunks );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_data_chunks_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_ord_data_chunks_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_unord_data_chunks_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_data_chunks_unacked );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_data_chunks_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_ord_data_chunks_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_unord_data_chunks_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_bytes_sent_ok );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_bytes_recvd_ok );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_init_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_init_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_initack_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_initack_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_cookie_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_cookie_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_cookieack_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_cookieack_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_hb_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_hb_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_hback_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_hback_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_sack_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_sack_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_abort_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_abort_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_shut_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_shut_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_shutack_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_shutack_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_shutcom_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_shutcom_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_error_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_error_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_ecne_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_ecne_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_cwr_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_cwr_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_datagrams_recvd_in_error );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_chk_sum_err_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_ootb_recv );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_frag_usr_msg );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_reassemble_usr_msg );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_packets_sent );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_packets_recv );

    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_set_Buffer( p_req_buffer,
                                           sizeof( sctp_time_t ),
                                           ( sctp_U8bit * )&stack_db.sctp_global_traffic_stats.discont_time );

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_asconf_sent );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_asconf_recv );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_asconfack_sent );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_global_traffic_stats.no_asconfack_recv );
#endif

    /* Rel 6.2 Ends Here */

    /* sctp_error_stats */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_error_stats.API_error_stats_count );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_error_stats.protcol_error_stats_count );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_error_stats.system_error_stats_count );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_error_stats.system_resource_error_stats_count );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_error_stats.internal_error_stats_count );

    /* sctp_stats_table */
    for( count = 0; count < SCTP_ALL_STATS; count++ )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              ( sctp_U32bit * )&stack_db.sctp_stats_table[count] );
    }


#endif /* SCTP_STATS_ENABLED */

    /* active_appl_list */
    for( count = 0; count < stack_db.max_appl; count++ )
    {
        p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                              &stack_db.active_appl_list[count].open );
        /* SPR FIX 20540 Start --- */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &stack_db.active_appl_list[count].appl_id );
        /* SPR FIX 20540 End --- */
        p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                               &stack_db.active_appl_list[count].su_id );
    }

    /* addr_list */
    for( count = 0; count < stack_db.num_local_addrs; count++ )
    {
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &stack_db.addr_list[count] );
    }

    /* sctp_def_prot_params */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.rto_init );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.rto_min );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.rto_max );
    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.max_burst );
    /* Rel 6.2 Ends Here. */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.valid_cookie_life );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.assoc_max_retrans );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.hb_interval );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.path_max_retrans );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.max_init_retrans );
    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.max_init_path_retrans );
    /* Rel 6.2 Ends Here. */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.init_timeo );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.bundling_time );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.sack_period );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.sack_frequency );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.adaption_ind );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.maxseg );
    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.hb_info_length );
    /* Rel 6.2 Ends Here. */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.mtu_size );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.rto_alpha_percent );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.rto_beta_percent );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.num_in_streams );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.num_out_streams );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.re_use_addr );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.dont_fragment );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.want_mapped );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.nat_capable );
    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.pmtu_capable );
    /* Rel 6.2 Ends Here. */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.allow_asconf );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.checksum_func );
    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.ppi );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.hb_max_burst );

#ifdef SCTP_IP_HEADER_PARAMS
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.ip_header_params.ttl );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.sctp_def_prot_params.ip_header_params.tos );
#ifdef SCTP_INET6_SUPPORT
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.ip_header_params.hoplimit );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.sctp_def_prot_params.ip_header_params.tclass );
#endif
#endif
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.high_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.high_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.med_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.med_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.low_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.low_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.no_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.trx_cong_reg_def.no_cong_lvl_abate );

    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.high_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.high_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.med_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.med_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.low_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.low_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.no_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.rcv_cong_reg_def.no_cong_lvl_abate );
    /* Rel 6.2 Ends Here */


    /* sctp_stack_init_parameters */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_db.sctp_stack_init_parameters.ecn_capable );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_db.sctp_stack_init_parameters.pmtu_capable );
    /* Rel 6.2 */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_db.sctp_stack_init_parameters.nat_capable );
    /* Rel 6.2 Ends Here. */
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.sctp_stack_init_parameters.ack_policy );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.sctp_stack_init_parameters.stale_cookie_policy );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&stack_db.sctp_stack_init_parameters.stack_type );

    /* init_in_streams */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.init_in_streams );

    /* init_out_streams */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.init_out_streams );

    /* standard_udp_port */
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer,
                                           &stack_db.standard_udp_port );

#ifdef SCTP_ERROR_ENABLED
    /* sctp_error_state */
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.sctp_error_state );

#endif /* SCTP_ERROR_ENABLED */

#ifdef SCTP_DEBUG_ENABLED
    /* sctp_gbl_trce */
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                          &stack_db.sctp_gbl_trc );
#endif /* SCTP_DEBUG_ENABLED */

    /*Old mac keys*/
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer, SCTP_KEYSIZE,
                                           &stack_db.old_mac_key[0] );

    /*Current mac keys*/
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer, SCTP_KEYSIZE,
                                           &stack_db.current_mac_key[0] );


    /* call the api from    functional interface */
    ret = sctp_api_set_stack_red_db( &stack_db, &errorno );


    /* now form the response buf */
    /* build header , interchange    src and    dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_STACK_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_get_assoc_red_db
 *
 *  DESCRIPTION:
 *     This function processes assoc red db api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_assoc_red_db(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
#ifdef SCTP_STRUCT_BASED

    sctp_msg_get_assoc_red_db_st        get_assoc_red_db_req;
    sctp_msg_get_assoc_red_db_res_st    get_assoc_red_db_resp;

#else

    sctp_return_t        ret;
    sctp_error_t         errorno;
    sctp_U32bit          len, assoc_id;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;
    sctp_U8bit         *assoc_db;


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_assoc_red_db " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_assoc_red_db_st ),
                                           ( sctp_U8bit * )&get_assoc_red_db_req );

    /* call the api from functional interface */
    assoc_db = sctp_api_get_assoc_red_db( get_assoc_red_db_req.association_id,
                                          &get_assoc_red_db_resp.len,
                                          &get_assoc_red_db_resp.errorno );

    if( assoc_db == SCTP_NULL )
    {
        get_assoc_red_db_resp.result = SCTP_FAILURE;
    }

    else
    {
        get_assoc_red_db_resp.result = SCTP_SUCCESS;
    }

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_ASSOC_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    get_assoc_red_db_resp.assoc_id = get_assoc_red_db_req.association_id;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_assoc_red_db_res_st ) - sizeof( sctp_U8bit * ),
                                            ( sctp_U8bit * )&get_assoc_red_db_resp );

    if( assoc_db != SCTP_NULL )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                get_assoc_red_db_resp.len, assoc_db );

        __MESG_FREE( assoc_db );
    }

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &assoc_id );

    /* call the api from functional interface */
    assoc_db = sctp_api_get_assoc_red_db( assoc_id, &len, &errorno );

    if( assoc_db == SCTP_NULL )
    {
        ret = SCTP_FAILURE;
    }

    else
    {
        ret = SCTP_SUCCESS;
    }

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_ASSOC_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );


    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, len );
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer, len, assoc_db );
        __MESG_FREE( assoc_db );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :   sctp_proc_api_set_assoc_red_db
 *
 *  DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_set_assoc_red_db(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_set_assoc_red_db_st       set_assoc_red_db_req;
    sctp_msg_set_assoc_red_db_res_st   set_assoc_red_db_resp;

#else

    sctp_return_t        ret;
    sctp_error_t         errorno;
    sctp_U32bit          len;
    sctp_U32bit          assoc_id;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_assoc_red_db " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_assoc_red_db_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&set_assoc_red_db_req );


    /*Now p_req_buffer points to the assoc_red_data*/
    set_assoc_red_db_req.assoc_red_db = p_req_buffer;

    /* call the api from functional interface */
    set_assoc_red_db_resp.result = sctp_api_set_assoc_red_db(
                                       set_assoc_red_db_req.assoc_id,
                                       set_assoc_red_db_req.assoc_red_db,
                                       &set_assoc_red_db_resp.errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_ASSOC_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_assoc_red_db_res_st ),
                                            ( sctp_U8bit * )&set_assoc_red_db_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &assoc_id );
    p_req_buffer =  sctp_buffer_get_Ulong( p_req_buffer, &len );

    /* call the api from functional interface */
    /*Now p_req_buffer points to the assoc_red_data*/
    ret = sctp_api_set_assoc_red_db( assoc_id, p_req_buffer, &errorno );

    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_ASSOC_RED_DB_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *  FUNCTION :    sctp_proc_api_get_stack_health
 *
 *  DESCRIPTION:
 *     This function processes get stack health api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.
 *  RETURN VALUE:
 *    pointer to buffer
 *
 **************************************************************************/
static sctp_U8bit *
sctp_proc_api_get_stack_health(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
#ifdef SCTP_STRUCT_BASED

    sctp_msg_get_stack_health_res_st   get_stack_health_resp;

#else

    sctp_return_t             ret;
    sctp_error_t              errorno;
    sctp_stack_red_state_et   red_state;
    sctp_stack_proc_state_et  proc_state;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_stack_health " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer +    SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    /* call the api from    functional interface */
    get_stack_health_resp.result = sctp_api_get_stack_health(
                                       &get_stack_health_resp.red_state,
                                       &get_stack_health_resp.proc_state,
                                       &get_stack_health_resp.errorno );


    /* now form the response buf */
    /* build header , interchange    src and    dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STACK_HEALTH_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_stack_health_res_st ),
                                            ( sctp_U8bit * )&get_stack_health_resp );

#else

    /* call the api from functional interface */
    ret = sctp_api_get_stack_health( &red_state, &proc_state, &errorno );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STACK_HEALTH_RESP,
                                              transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, errorno );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, red_state );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, proc_state );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

#endif /* SCTP_RED_SUPPORT */
#endif /* SCTP_UI_SM_MESSAGEBASED */

