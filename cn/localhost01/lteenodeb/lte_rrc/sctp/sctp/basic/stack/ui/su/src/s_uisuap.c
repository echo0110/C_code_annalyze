/***************************************************************************
 *  FILE NAME  : s_uisuap.c
 *
 *  DESCRIPTION: Contains the functions that process the SU APIs.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Sandeep      -            .Original Creation
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  02jan02    hbhondwe     SCTP_IPC_TCP ipc changes for tcp
 *  10Oct02    Manish Rajpal SPR 4951    send iovec call
 *  10Oct02    Manish Rajpal SPR 4952    Incorrect Adaption Ind
 *  10Oct02    Manish Rajpal SPR 4953    Resueaddr in Config Assoc
 *  07Oct03    Vivek Bansal  Rel 5.0     Changes for CSPL
 *  27Nov03    Vivek Bansal  Rel 5.0     Changes for CSPL
 *  20Oct04    Amit Ray      Rel 6.0     SCTP/FUNC-GEN-3
 *  02Nov04    Svakumar      REL 6.0     SCTP/FUNC-CONGCON-5
 * 7Jul11       Vaibhav Banga           klocwork fix for SPR 20992
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "UI"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_db.h>
#include <s_dbtcb.h>
#include <s_osmem.h>
#include <s_cobuf.h>
#include <s_clpapi.h>
#include <s_dbg.h>
#include <s_uiipc.h>
#include <s_uisuif.h>
#include <s_uisuap.h>
#include <s_stats.h>
#include <s_es.h>
#include <s_fsmdat.h>

#define NO_ASSOC_ID  0

#ifdef SCTP_UI_SU_MESSAGEBASED
extern sctp_lock_t  msgif_lock;
#endif

#ifndef SCTP_UI_SOCK_API

#ifdef SCTP_UI_SU_MESSAGEBASED


sctp_U8bit  *
( *sctp_ui_proc_su_api_tbl[SCTP_NUM_SUREQUEST_API] )
( sctp_U8bit *, sctp_U8bit * ) =
{
    sctp_proc_api_register_su,     /* register su */
    sctp_proc_api_deregister_su,   /* de-register su */
    sctp_proc_api_socket,          /* socket  */
    sctp_proc_api_bind,            /* bind  */
    sctp_proc_api_listen,         /* listen  */
    sctp_proc_api_config_assoc, /* config socket */
    sctp_proc_api_set_fthresh,   /* set failure threshold */
    sctp_proc_api_connect,     /* connect  */
    sctp_proc_api_connect_ext,   /* connect extended */
    sctp_proc_api_accept,      /* accept */
    sctp_proc_api_accept_ext,  /* accept extended */
    sctp_proc_api_send,      /* send  */
    sctp_proc_api_send_ext,    /* send  Extended*/
    sctp_proc_api_receive,     /* receive */
    sctp_proc_api_receive_ext,   /* receive Extended */
    sctp_proc_api_receive_part,  /* receive partial data    */
    sctp_proc_api_close,       /* close */
    sctp_proc_api_hb_action,   /* heartbeat action */
    sctp_proc_api_set_primary,   /* set primary destination */
    sctp_proc_api_get_srtt,    /*  get srtt */
    sctp_proc_api_get_status,    /* get status */
    sctp_proc_api_get_dest_status,       /* et destination status */
    sctp_proc_api_set_restart_option,   /* set restart option */
    sctp_proc_api_ret_unsent,   /* retrieve unsent  */
    sctp_proc_api_ret_unacked,        /* retrieve unacked */
    sctp_proc_api_flush_buffers,         /*flush assoc. buffers*/
    sctp_proc_api_config_assoc_ctl,      /*config assoc ctl*/
    sctp_proc_api_get_stream_status,     /*Get Stream Status*/
    sctp_proc_api_abort_retrieve,         /*Get Stream Status*/
    sctp_proc_api_send_iovec,     /*sctp_send_iove, never sent to stack */
    sctp_proc_api_bindx,             /*sctp_api_bindx*/
    sctp_proc_api_config_cong_reg_def,   /*sctp_api_config_cong_reg_def*/
    sctp_proc_api_get_cong_reg_def,      /** sctp_api_get_cong_reg_def**/
    sctp_proc_api_config_txrx_num_frag,  /** sctp_api_config_txrx_num_frag**/
    sctp_proc_api_get_free_txrx_num_frag,  /** sctp_api_config_txrx_num_frag**/
    sctp_proc_api_connectx,                 /*Connectx*/
    sctp_proc_api_close_ext
};

/***************************************************************************
 *    FUNCTION :    sctp_process_su_api_prim_msg
 *
 *    DESCRIPTION:
 *    This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    None
 *
 **************************************************************************/
sctp_void_t
sctp_process_su_api_prim_msg(
    sctp_U8bit            *p_buffer,
    sctp_len_t            nooctets,
    sctp_client_info_st   *from )
{
    sctp_api_id_t  api_id;
    sctp_U16bit    index_api_id;
    sctp_U32bit    noctets;
    sctp_return_t  ret;
    sctp_src_id_t  src_id;
    sctp_dest_id_t dest_id;
    sctp_Error_t   ecode;
    /*
     * Allocate the memory for response message. The memory for this pointer
     * is allocated using qvMesgAllocFromModule.
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
    src_id = sctp_buffer_get_UshortAtOffset( p_buffer, 4 );

    index_api_id = api_id - SCTP_MIN_SUREQUST_APIID;

    __MEMSET( p_resp_buffer, 0, SCTP_MAX_API_SIZE );

#ifdef SCTP_RED_SUPPORT
    /* The next 4 bytes for event_type */
    p_resp_buffer = p_resp_buffer + 4;
    /* The next 4 bytes for total size */
    p_resp_buffer = p_resp_buffer + 4;
#endif

    /*
     * based on api id, call a function that will process the req
     * and return the response
     */
    if( ( api_id == SCTP_API_CONNECT_EXT_REQ )
            || ( api_id == SCTP_API_CONNECT_REQ ) ||
            ( api_id == SCTP_API_CONNECTX_REQ ) )
    {
        SCTP_LOCK( msgif_lock );
    }

    /* klocwork fix for SPR 20992 */
    if( *sctp_ui_proc_su_api_tbl[index_api_id] == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "sctp_ui_proc_su_api_tbl is NULL" ) );
        return;
    }

    /* klocwork fix for SPR 20992 */
    p_resp_buffer = ( *sctp_ui_proc_su_api_tbl[index_api_id] )( p_buffer,
                                                                p_resp_buffer );

    noctets  = p_resp_buffer - p_resp_message;

#ifdef SCTP_RED_SUPPORT
    /*
     * Leave the first four bytes for the event type and next 4 bytes
     *  for total size.
     * Start writing the resp buffer after 8 bytes.
     */
    noctets = noctets - 8;
    p_resp_buffer = p_resp_message + 8;
#else
    p_resp_buffer = p_resp_message;
#endif

    sctp_buffer_set_UlongAtOffset( p_resp_buffer, APIBUF_LENGTH_OFFSET,
                                   ( sctp_U32bit )noctets );

    src_id = sctp_buffer_get_UshortAtOffset( p_resp_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_resp_buffer, 6 );

#ifdef SCTP_RED_SUPPORT
    /*
     * Increament the four bytes of the message for the event type.
     * Then set the event type at the zeroth index of the buffer.
     */
    noctets = noctets + 8;
    p_resp_buffer = p_resp_message;
    sctp_buffer_set_UlongAtOffset( p_resp_buffer, 0, ( sctp_U32bit ) SCTP_EVT_RESP_NOTF );
    sctp_buffer_set_UlongAtOffset( p_resp_buffer, 4, noctets );

#endif

    /* call send_from_sctp from here with the resp buffer */
    ret = send_from_sctp( p_resp_buffer,
                          noctets, dest_id, src_id, SCTP_CLIENT_SU, from, &ecode );

    if( ( api_id == SCTP_API_CONNECT_EXT_REQ )
            || ( api_id == SCTP_API_CONNECT_REQ ) ||
            ( api_id == SCTP_API_CONNECTX_REQ ) )
    {
        SCTP_UNLOCK( msgif_lock );
    }

    if( ret != SCTP_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nCould not send resp message" ) );
        __MESG_FREE( p_resp_message );
        return;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Resp message sent" ) );
}

/**************************************************************************
 *    FUNCTION :    sctp_proc_api_register_su
 *
 *    DESCRIPTION:
 *    This function process register user api
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_register_su(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
#ifdef SCTP_STRUCT_BASED
    sctp_msg_register_su_st     reg_req;
    sctp_msg_register_su_res_st reg_resp;
#else
    sctp_U16bit        su_id;
    sctp_U16bit        appl_id;
    sctp_return_t      ret;
    sctp_error_t       ecode;
#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_register_su " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_register_su_st ), ( sctp_U8bit * )&reg_req );

    /* call the api from functional interface */

    reg_resp.result = sctp_api_register_su( &reg_resp.appl_id,
                                            ( sctp_Pvoid_t ) &reg_req.su_id, SCTP_NULL, &reg_resp.errorno );

    /* now form the response buf */
    /* build header ,interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_REGISTER_SU_RESP, transaction_id, dest_id, src_id );

    reg_resp.su_id = reg_req.su_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_register_su_res_st ), ( sctp_U8bit * )&reg_resp );

    if( reg_resp.result == SCTP_SUCCESS )
    {
        sctp_client_list[reg_req.su_id].stack_id = dest_id;
        sctp_client_list[reg_req.su_id].application_id = reg_resp.appl_id;
    }

#else
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &su_id );

    /* call the api from functional interface */

    ret = sctp_api_register_su( &appl_id,
                                ( sctp_Pvoid_t ) &su_id, SCTP_NULL, &ecode );


    /* now form the response buf */
    /* build header, interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_REGISTER_SU_RESP, transaction_id, dest_id, src_id );


    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U32bit )ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U32bit )ecode );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, su_id );

    /* SCTP_IPC_TCP: problem fixed:appl id must be returned  */
    if( ret == SCTP_SUCCESS )
    {
        sctp_client_list[su_id].stack_id = dest_id;
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, appl_id );
        sctp_client_list[su_id].application_id = appl_id;

    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/**************************************************************************
 *    FUNCTION :    sctp_proc_api_deregister_su
 *
 *    DESCRIPTION:
 *    This function processes deregiste user api.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_deregister_su(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

#ifdef SCTP_STRUCT_BASED

    sctp_msg_deregister_su_st      dereg_req;
    sctp_msg_deregister_su_res_st  dereg_resp;

#else
    sctp_U16bit     appl_id;
    sctp_return_t   ret;
    sctp_error_t    ecode;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t transaction_id;
    sctp_src_id_t   src_id;
    sctp_dest_id_t  dest_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_deregister_su " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_deregister_su_st ), ( sctp_U8bit * )&dereg_req );

    /* call the api from functional interface */
    dereg_resp.result =  sctp_api_deregister_su( dereg_req.appl_id,
                                                 &dereg_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DEREGISTER_SU_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    dereg_resp.appl_id = dereg_req.appl_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_deregister_su_res_st ), ( sctp_U8bit * )&dereg_resp );

    if( dereg_resp.result == SCTP_SUCCESS )
    {
        __MEMSET( &sctp_client_list[src_id], 0, sizeof( sctp_client_info_st ) );
    }

#else

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &appl_id );

    /* call the api from functional interface */
    ret = sctp_api_deregister_su( appl_id,
                                  &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_DEREGISTER_SU_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, appl_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

    if( ret == SCTP_SUCCESS )
    {
        __MEMSET( &sctp_client_list[src_id], 0, sizeof( sctp_client_info_st ) );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/**************************************************************************
 *    FUNCTION :    sctp_proc_api_socket
 *
 *    DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_socket(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
#ifdef SCTP_STRUCT_BASED

    sctp_msg_socket_st        socket_req;
    sctp_msg_socket_res_st    socket_resp;

#else

    sctp_U16bit     appl_id;
    sctp_return_t   ret;
    sctp_error_t    ecode;
    sctp_U32bit     assoc_id, addr_family;

#endif /* SCTP_STRUCT_BASED */

    sctp_trans_id_t transaction_id;
    sctp_src_id_t   src_id;
    sctp_dest_id_t  dest_id;


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_socket " ) );

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_socket_st ), ( sctp_U8bit * )&socket_req );

    socket_resp.result =  sctp_api_socket( socket_req.appl_id,
                                           socket_req.addr_family,
                                           &socket_resp.assoc_id,
                                           &socket_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SOCKET_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    socket_resp.appl_id = socket_req.appl_id;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_socket_res_st ), ( sctp_U8bit * )&socket_resp );

#else

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &appl_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &addr_family );


    /* call the api from functional interface */
    ret = sctp_api_socket( appl_id,
                           addr_family,
                           &assoc_id,
                           &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SOCKET_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, appl_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_bind
 *
 *    DESCRIPTION:
 *    This function procces the bind api.  This function takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts
 *    the api parameters from the buffer and invokes the functional api,
 *    forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_bind(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_bind_st      bind_req;
    sctp_msg_bind_res_st  bind_resp;
#else
    sctp_U16bit            sctp_port;
    sctp_return_t          ret;
    sctp_error_t           ecode;
    sctp_U32bit            assoc_id, count, no_of_su_addrs;
    sctp_U8bit             hostname[SCTP_MAX_HOSTNAME_LEN];
    sctp_sockaddr_list_st  list_su_addrs;
    sctp_sockaddr_st       *p_addr;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_bind_st ), ( sctp_U8bit * )&bind_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_bind " ) );

    /* call the api from functional interface */
    bind_resp.result = sctp_api_bind( bind_req.association_id,
                                      bind_req.sctp_port,
                                      bind_req.no_of_su_addrs,
                                      bind_req.list_su_addrs,
                                      &bind_req.hostname[0],
                                      &bind_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_BIND_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    bind_resp.association_id = bind_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_bind_res_st ), ( sctp_U8bit * )&bind_resp );

#else

    p_addr = ( sctp_sockaddr_st * )list_su_addrs;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &assoc_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_bind " ) );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &sctp_port );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &no_of_su_addrs );

    for( count = 0; count < no_of_su_addrs; count++ )
    {
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &( p_addr[count] ) );
    }

    for( count = 0; count < SCTP_MAX_HOSTNAME_LEN; count++ )
    {
        p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer,
                                              &hostname[count] );

        if( hostname[count] == '\0' )
        {
            break;
        }
    }

    ret = sctp_api_bind( assoc_id, sctp_port, no_of_su_addrs,
                         list_su_addrs, &hostname[0], &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_BIND_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_SRTRUCT_BASED */

    return( p_resp_buffer );
}



/***************************************************************************
 *    FUNCTION :    sctp_proc_api_bindx
 *
 *    DESCRIPTION:
 *    This function processes bindx api.  This function takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms the resp
 *    and returns it.

 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_bindx(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_bindx_st      bindx_req;
    sctp_msg_bindx_res_st  bindx_resp;
#else
    sctp_return_t          ret;
    sctp_error_t           ecode;
    sctp_U32bit            assoc_id, count, no_of_su_addrs;
    sctp_sockaddr_list_st  list_su_addrs;
    sctp_U32bit            flags;
    sctp_sockaddr_st       *p_addr;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_bindx_st ), ( sctp_U8bit * )&bindx_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_bindx " ) );

    bindx_resp.result = sctp_api_bindx( bindx_req.association_id,
                                        bindx_req.no_of_su_addrs,
                                        bindx_req.list_su_addrs,
                                        bindx_req.flags,
                                        SCTP_FALSE, /*do not block*/
                                        &bindx_resp.error );


    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_BINDX_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    bindx_resp.association_id = bindx_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_bindx_res_st ), ( sctp_U8bit * )&bindx_resp );

#else

    p_addr = ( sctp_sockaddr_st * )list_su_addrs;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &assoc_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_bindx " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &flags );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &no_of_su_addrs );

    for( count = 0; count < no_of_su_addrs; count++ )
    {
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &( p_addr[count] ) );
    }

    ret = sctp_api_bindx( assoc_id,
                          no_of_su_addrs,
                          list_su_addrs,
                          ( sctp_bindx_flag_et )flags,
                          SCTP_FALSE, /*set blocking to false*/
                          &ecode );
    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_BINDX_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_SRTRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_listen
 *
 *    DESCRIPTION:
 *     This function processes the listen api.This function takes a buffer
 *     pointer containing the buffer recvd from the client. It extracts the
 *     api parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.

 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_listen(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_listen_st      listen_req;
    sctp_msg_listen_res_st  listen_resp;
#else
    sctp_return_t          ret;
    sctp_error_t           ecode;
    sctp_U32bit            assoc_id, back_log;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_listen_st ), ( sctp_U8bit * )&listen_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_listen " ) );

    /* call the api from functional interface */
    listen_resp.result = sctp_api_listen( listen_req.association_id,
                                          listen_req.back_log,
                                          &listen_resp.error );


    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_LISTEN_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    listen_resp.association_id = listen_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_listen_res_st ), ( sctp_U8bit * )&listen_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &assoc_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_listen " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &back_log );

    ret = sctp_api_listen( assoc_id,
                           back_log,
                           &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_LISTEN_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, assoc_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U32bit )ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U32bit )ecode );

#endif /* SCTP_SRTRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_config_assoc
 *
 *    DESCRIPTION:
 *     This function procces the config association api. This function takes
 *     a buffer pointer containing the buffr recvd from the client. It
 *     extracts the api parameters from the buffer and invokes the functional
 *     api, forms the resp and returns it.

 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_config_assoc(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t     transaction_id;
    sctp_src_id_t       src_id;
    sctp_dest_id_t      dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_config_assoc_st      config_assoc_req;
    sctp_msg_config_assoc_res_st  config_assoc_resp;
#else
    sctp_error_t         ecode;
    sctp_U32bit          association_id;
    sctp_prot_params_st  prot_params;
    sctp_U8bit           u8_temp;
    sctp_U16bit          u16_temp;
    sctp_return_t        ret;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_assoc_st ), ( sctp_U8bit * )&config_assoc_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_assoc " ) );

    /* call the api from functional interface */

    config_assoc_resp.result = sctp_api_config_assoc(
                                   config_assoc_req.association_id,
                                   &config_assoc_req.protocol_parameters,
                                   &config_assoc_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_ASSOC_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    config_assoc_resp.association_id = config_assoc_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_config_assoc_res_st ), ( sctp_U8bit * )&config_assoc_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_assoc " ) );

    /*SPR 4953*/
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.rto_init );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.rto_min );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.rto_max );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.max_burst );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.valid_cookie_life );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.assoc_max_retrans );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.hb_interval );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.path_max_retrans );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.max_init_retrans );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.max_init_path_retrans );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.bundling_time );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.sack_period );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.sack_frequency );
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.pmtu_capable );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&prot_params.pmtu_capable );
#endif
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    prot_params.mtu_size = u16_temp;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          &prot_params.adaption_ind );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    prot_params.hb_info_length = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    prot_params.rto_alpha_percent = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    prot_params.rto_beta_percent = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    prot_params.num_in_streams = u16_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    prot_params.num_out_streams = u16_temp;
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    prot_params.re_use_addr = u8_temp;
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    prot_params.dont_fragment = u8_temp;
    /*SPR 4953*/

#ifdef SCTP_IP_HEADER_PARAMS
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    prot_params.ip_header_params.ttl = u8_temp;
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    prot_params.ip_header_params.tos = u8_temp;
#ifdef SCTP_INET6_SUPPORT
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    prot_params.ip_header_params.hoplimit = u8_temp;
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    prot_params.ip_header_params.tclass = u8_temp;
#endif

#endif

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &prot_params.ppi );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &prot_params.hb_max_burst );
    /* call the api from functional interface */
    ret = sctp_api_config_assoc( association_id,
                                 &prot_params,
                                 &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_ASSOC_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_config_assoc_ctl
 *
 *    DESCRIPTION:
 *     This function is used to processes config_assoc_ctl api. This function
 *     takes a buffer pointer containing the buffr recvd from the client. It
 *     extracts the api parameters from the buffer and invokes the functional
 *     api, forms the resp and returns it.
 *
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_config_assoc_ctl(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_config_assoc_ctl_st      config_assoc_ctl_req;
    sctp_msg_config_assoc_ctl_res_st  config_assoc_ctl_resp;
#else
    sctp_return_t        ret;
    sctp_error_t         ecode;
    sctp_U32bit          association_id;
    sctp_prot_params_et  prot_param;
    sctp_U32bit          param_value;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_assoc_ctl_st ),
                                           ( sctp_U8bit * )&config_assoc_ctl_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_assoc_ctl " ) );


    /* call the api from functional interface */
    config_assoc_ctl_resp.result =  sctp_api_config_assoc_ctl(
                                        config_assoc_ctl_req.association_id,
                                        config_assoc_ctl_req.prot_param,
                                        config_assoc_ctl_req.param_value,
                                        &config_assoc_ctl_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_ASSOC_CTL_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    config_assoc_ctl_resp.association_id = config_assoc_ctl_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_config_assoc_ctl_res_st ),
                                            ( sctp_U8bit * )&config_assoc_ctl_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_assoc_ctl " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&prot_param );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &param_value );


    /* call the api from functional interface */
    ret = sctp_api_config_assoc_ctl( association_id,
                                     prot_param,
                                     param_value,
                                     &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_ASSOC_CTL_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U32bit )ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U32bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}



/***************************************************************************
 *    FUNCTION :    sctp_proc_api_set_fthresh
 *
 *    DESCRIPTION:
 *     This function processes set failure threshold api. This function takes
 *     a buffer pointer containing the buffr recvd from the client. It
 *     extracts the api parameters from the buffer and invokes the functional
 *     api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_set_fthresh(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t transaction_id;
    sctp_src_id_t   src_id;
    sctp_dest_id_t  dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_set_fthresh_st       fthresh_req;
    sctp_msg_set_fthresh_res_st   fthresh_resp;
#else
    sctp_return_t    ret;
    sctp_error_t     ecode;
    sctp_U32bit      association_id;
    sctp_sockaddr_st dest_address;
    sctp_U32bit      path_max_retrans;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );
    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_fthresh_st ), ( sctp_U8bit * )&fthresh_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_fthresh " ) );

    /* call the api from functional interface */
    fthresh_resp.result = sctp_api_set_failure_threshold(
                              fthresh_req.association_id,
                              &fthresh_req.dest_address,
                              fthresh_req.path_max_retrans,
                              &fthresh_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_FTHRESH_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    fthresh_resp.association_id = fthresh_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_fthresh_res_st ), ( sctp_U8bit * )&fthresh_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_fthresh " ) );

    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &dest_address );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &path_max_retrans );
    /* call the api from functional interface */
    ret = sctp_api_set_failure_threshold( association_id,
                                          &dest_address,
                                          path_max_retrans,
                                          &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_FTHRESH_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_connect
 *
 *    DESCRIPTION:
 *    This function processes the connect api.This function takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts the
 *    api parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_connect(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t           transaction_id;
    sctp_src_id_t             src_id;
    sctp_dest_id_t            dest_id;
    sctp_U32bit               nos_of_dest_trans_addrs;
    sctp_sockaddr_list_st     list_of_dest_trans_addrs;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_connect_st      connect_req;
    sctp_msg_connect_res_st  connect_resp;
#else
    sctp_return_t     ret;
    sctp_error_t      ecode;
    sctp_U32bit       association_id;
    sctp_U16bit       dest_sctp_port;
    sctp_sockaddr_st  connect_dest_address;
    sctp_U32bit       payload_prot_id;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connect_st ), ( sctp_U8bit * )&connect_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_connect " ) );

    /* call the api from functional interface */
    connect_resp.result = sctp_api_connect( connect_req.association_id,
                                            connect_req.dest_sctp_port,
                                            &nos_of_dest_trans_addrs,
                                            list_of_dest_trans_addrs,
                                            &connect_req.connect_dest_address,
                                            connect_req.payload_prot_id,
                                            SCTP_FALSE,
                                            &connect_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONNECT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    connect_resp.association_id =  connect_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_connect_res_st ), ( sctp_U8bit * )&connect_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_connect " ) );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &dest_sctp_port );

    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                &connect_dest_address );

    /* VXWORKS:Undo if apis expect hb */
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &payload_prot_id );
    /* call the api from functional interface */

    ret = sctp_api_connect( association_id,
                            dest_sctp_port,
                            &nos_of_dest_trans_addrs,
                            list_of_dest_trans_addrs,
                            &connect_dest_address,
                            payload_prot_id,
                            SCTP_FALSE,
                            &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONNECT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_connect_ext
 *
 *    DESCRIPTION:
 *    This function connect ext api.This function  takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms the
 *    resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_connect_ext(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t        transaction_id;
    sctp_src_id_t          src_id;
    sctp_dest_id_t         dest_id;
    sctp_U32bit            nos_of_dest_trans_addrs;
    sctp_sockaddr_list_st  list_of_dest_trans_addrs;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_connect_ext_st      connect_ext_req;
    sctp_msg_connect_ext_res_st  connect_ext_resp;
#else
    sctp_return_t       ret;
    sctp_error_t        ecode;
    sctp_U32bit         association_id;
    sctp_U16bit         dest_sctp_port;
    sctp_U16bit         num_out_streams;
    sctp_U16bit         num_in_streams;
    sctp_sockaddr_st    connect_dest_address;
    sctp_U32bit         payload_prot_id;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connect_ext_st ), ( sctp_U8bit * )&connect_ext_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_connect_ext " ) );

    /* call the api from functional interface */
    connect_ext_resp.result = sctp_api_connect_ext(
                                  connect_ext_req.association_id,
                                  connect_ext_req.dest_sctp_port,
                                  &connect_ext_req.num_out_streams,
                                  &connect_ext_req.num_in_streams,
                                  &nos_of_dest_trans_addrs,
                                  list_of_dest_trans_addrs,
                                  &connect_ext_req.connect_dest_address,
                                  connect_ext_req.payload_prot_id,
                                  SCTP_FALSE,
                                  &connect_ext_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONNECT_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    connect_ext_resp.association_id =  connect_ext_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_connect_ext_res_st ), ( sctp_U8bit * )&connect_ext_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_connect_ext " ) );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &dest_sctp_port );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &num_out_streams );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &num_in_streams );

    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                &connect_dest_address );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &payload_prot_id );

    /* call the api from functional interface */
    ret = sctp_api_connect_ext( association_id,
                                dest_sctp_port,
                                &num_out_streams,
                                &num_in_streams,
                                &nos_of_dest_trans_addrs,
                                list_of_dest_trans_addrs,
                                &connect_dest_address,
                                payload_prot_id,
                                SCTP_FALSE,
                                &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONNECT_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :     sctp_proc_api_accept
 *
 *    DESCRIPTION:
 *    This function processes accept api.This function  takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts
 *    the api parameters from the buffer and invokes the functional api,
 *    forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_accept(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t            transaction_id;
    sctp_src_id_t              src_id;
    sctp_dest_id_t             dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_accept_st       accept_req;
    sctp_msg_accept_res_st   accept_resp;
#else
    sctp_return_t              ret;
    sctp_error_t               ecode;
    sctp_U32bit                association_id, count;
    sctp_U32bit                payload_prot_id;
    sctp_U32bit                nos_of_dest_trans_addrs;
    sctp_sockaddr_list_st      list_of_dest_trans_addrs;
    sctp_U16bit                sctp_port;
    sctp_U32bit                new_association_id;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_accept_st ), ( sctp_U8bit * )&accept_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_accept " ) );

    /* call the api from functional interface */
    accept_resp.result =  sctp_api_accept(
                              accept_req.association_id,
                              accept_req.payload_prot_id,
                              &accept_resp.nos_of_dest_trans_addrs,
                              accept_resp.list_of_dest_trans_addrs,
                              &accept_resp.sctp_port,
                              &accept_resp.new_association_id,
                              SCTP_FALSE,
                              &accept_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ACCEPT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    accept_resp.association_id = accept_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_accept_res_st ), ( sctp_U8bit * )&accept_resp );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_accept " ) );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &payload_prot_id );

    /* call the api from functional interface */
    ret = sctp_api_accept(
              association_id,
              payload_prot_id,
              &nos_of_dest_trans_addrs,
              list_of_dest_trans_addrs,
              &sctp_port,
              &new_association_id,
              SCTP_FALSE,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ACCEPT_RESP, transaction_id, dest_id, src_id );

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

        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, sctp_port );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, new_association_id );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_accept_ext
 *
 *    DESCRIPTION:
 *    This function processes accept ext api.This function takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts the
 *    api parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *    RETURN VALUE:
 *     pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_accept_ext(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_accept_ext_st       accept_ext_req;
    sctp_msg_accept_ext_res_st   accept_ext_resp;
#else
    sctp_return_t              ret;
    sctp_error_t               ecode;
    sctp_U32bit                association_id, count;
    sctp_U32bit                payload_prot_id;
    sctp_U32bit                nos_of_dest_trans_addrs;
    sctp_sockaddr_list_st      list_of_dest_trans_addrs;
    sctp_U16bit                sctp_port;
    sctp_U32bit                new_association_id;
    sctp_U16bit                num_out_streams;
    sctp_U16bit                num_in_streams;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_accept_ext_st ), ( sctp_U8bit * )&accept_ext_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_accept_ext " ) );

    /* call the api from functional interface */
    accept_ext_resp.result =  sctp_api_accept_ext(
                                  accept_ext_req.association_id,
                                  accept_ext_req.payload_prot_id,
                                  &accept_ext_req.num_out_streams,
                                  &accept_ext_req.num_in_streams,
                                  &accept_ext_resp.nos_of_dest_trans_addrs,
                                  accept_ext_resp.list_of_dest_trans_address,
                                  &accept_ext_resp.sctp_port,
                                  &accept_ext_resp.new_association_id,
                                  SCTP_FALSE,
                                  &accept_ext_resp.errorno );

    accept_ext_resp.num_out_streams = accept_ext_req.num_out_streams;
    accept_ext_resp.num_in_streams  = accept_ext_req.num_in_streams;

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ACCEPT_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    accept_ext_resp.association_id  = accept_ext_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_accept_ext_res_st ), ( sctp_U8bit * )&accept_ext_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_accept_ext " ) );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &payload_prot_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &num_out_streams );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &num_in_streams );

    /* call the api from functional interface */
    ret = sctp_api_accept_ext(
              association_id,
              payload_prot_id,
              &num_out_streams,
              &num_in_streams,
              &nos_of_dest_trans_addrs,
              list_of_dest_trans_addrs,
              &sctp_port,
              &new_association_id,
              SCTP_FALSE,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ACCEPT_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, num_out_streams );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, num_in_streams );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               nos_of_dest_trans_addrs );

        for( count = 0; count < nos_of_dest_trans_addrs; count++ )
        {
            p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                       &list_of_dest_trans_addrs[count] );
        }

        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, sctp_port );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, new_association_id );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_send
 *
 *    DESCRIPTION:
 *    This function processes send api.This function takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms the
 *    resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_send(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t       transaction_id;
    sctp_src_id_t         src_id;
    sctp_dest_id_t        dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_send_st     send_req;
    sctp_msg_send_res_st  send_resp;
#else
    sctp_return_t       ret;
    sctp_error_t        ecode;
    sctp_U32bit         association_id;
    sctp_U16bit         stream_id;
    sctp_sockaddr_st    dest_address;
    /*sctp_U32bit         count; */
    sctp_U32bit         data_size, context;
    sctp_U8bit          *data_payload;
#endif /* SCTP_STRUCT_BASED */


    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_send_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&send_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_send " ) );

    /* p_req_buffer now points to the data payload first byte */
    send_req.data_payload = p_req_buffer;

    /* call the api from functional interface */
    if( SCTP_IS_INADDR_ANY( send_req.dest_address ) != 1 )
    {
        send_resp.result = sctp_api_send( send_req.association_id,
                                          send_req.context,
                                          send_req.stream_id,
                                          &send_req.dest_address,
                                          send_req.data_size,
                                          send_req.data_payload,
                                          &send_resp.errorno );
    }

    else
    {
        send_resp.result = sctp_api_send( send_req.association_id,
                                          send_req.context,
                                          send_req.stream_id,
                                          SCTP_NULL,
                                          send_req.data_size,
                                          send_req.data_payload,
                                          &send_resp.errorno );
    }

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SEND_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    send_resp.association_id = send_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_send_res_st ), ( sctp_U8bit * )&send_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_send " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &context );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &stream_id );

    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &dest_address );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &data_size );

    /* p_req_buffer now points to the data payload first byte */
    data_payload = p_req_buffer;

    /* call the api from functional interface */
    if( SCTP_IS_INADDR_ANY( dest_address ) != 1 )
    {
        ret = sctp_api_send( association_id,
                             context,
                             stream_id,
                             &dest_address,
                             data_size,
                             data_payload,
                             &ecode );
    }

    else
    {
        ret = sctp_api_send( association_id,
                             context,
                             stream_id,
                             NULL,
                             data_size,
                             data_payload,
                             &ecode );
    }

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SEND_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_send_ext
 *
 *     DESCRIPTION:
 *    This function processes send ext api.This function takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts the
 *    api parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_send_ext(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t     transaction_id;
    sctp_src_id_t       src_id;
    sctp_dest_id_t      dest_id;
    sctp_Boolean_t      order_flag_for_ext;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_send_ext_st     send_exp_req;
    sctp_msg_send_ext_res_st  send_exp_resp;
#else
    sctp_return_t       ret;
    sctp_error_t        ecode;
    sctp_U32bit         association_id;
    sctp_sockaddr_st    dest_address;
    sctp_U32bit         data_size;
    sctp_U8bit          *data_payload;
    sctp_U16bit         stream_id;
    sctp_U32bit         life_time;
    sctp_order_flag_t   order_flag;
    sctp_Boolean_t      bundle_flag;
    sctp_U32bit         payload_prot_id, context;
    sctp_U8bit          u8_temp;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_send_ext " ) );


#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_send_ext_st ) - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )&send_exp_req );

    send_exp_req.data_payload = p_req_buffer;

    if( send_exp_req.order_flag == SCTP_ORDERED )
    {
        order_flag_for_ext = SCTP_TRUE;
    }

    else
    {
        order_flag_for_ext = SCTP_FALSE;
    }

    /* call the api from functional interface */
    /*SPR 5224*/
    send_exp_resp.result  = sctp_api_send_ext(
                                send_exp_req.association_id,
                                send_exp_req.context,
                                &send_exp_req.dest_address,
                                send_exp_req.data_size,
                                send_exp_req.data_payload,
                                send_exp_req.stream_id,
                                send_exp_req.life_time,
                                order_flag_for_ext,
                                send_exp_req.bundle_flag,
                                send_exp_req.payload_prot_id,
                                &send_exp_resp.errorno );
    /*SPR 5224*/

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SEND_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    send_exp_resp.association_id = send_exp_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_send_ext_res_st ), ( sctp_U8bit * )&send_exp_resp );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &data_size );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &context );

    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &dest_address );


    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &life_time );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    order_flag = ( sctp_order_flag_t ) u8_temp;

    if( order_flag == SCTP_ORDERED )
    {
        order_flag_for_ext = SCTP_TRUE;
    }

    else
    {
        order_flag_for_ext = SCTP_FALSE;
    }

    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    bundle_flag = ( sctp_Boolean_t ) u8_temp;
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &stream_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &payload_prot_id );

    /* Now the request buffer poits to data */
    data_payload = p_req_buffer;

    /* call the api from functional interface */
    /*SPR 5224*/
    ret = sctp_api_send_ext( association_id,
                             context,
                             &dest_address,
                             data_size,
                             data_payload,
                             stream_id,
                             life_time,
                             order_flag_for_ext,
                             bundle_flag,
                             payload_prot_id,
                             &ecode );
    /*SPR 5224*/

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SEND_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}



/***************************************************************************
 *    FUNCTION :    sctp_proc_api_send_iovec
 *
 *     DESCRIPTION:
 *      This function processes send iovec api.This function takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts the
 *    api parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_send_iovec(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    /*SPR 4951*/
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_send_iovec: Calling sctp_proc_api_send_ext" ) );
    return sctp_proc_api_send_ext( p_req_buffer, p_resp_buffer );
    /*SPR 4951*/
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_receive
 *
 *    DESCRIPTION:
 *    This function processes receive api.This function takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts
 *    the api parameters from the buffer and invokes the functional api,
 *    forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_receive(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t transaction_id;
    sctp_src_id_t     src_id;
    sctp_dest_id_t    dest_id;
    sctp_U8bit        data_area[MAX_USER_DATA_SIZE];

#ifdef SCTP_STRUCT_BASED
    sctp_msg_receive_st      receive_req;
    sctp_msg_receive_res_st   receive_resp;
#else
    sctp_return_t    ret;
    sctp_error_t     ecode;
    sctp_U32bit      association_id;
    sctp_U32bit      payload_prot_id;
    sctp_U32bit      data_size;
    sctp_U16bit      stream_id;
    sctp_U8bit       *data;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;



#ifdef SCTP_STRUCT_BASED

    receive_resp.data = &data_area[0];
    receive_resp.data_size  = MAX_USER_DATA_SIZE;
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_receive_st ), ( sctp_U8bit * )&receive_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_receive " ) );

    /* call the api from functional interface */
    receive_resp.result = sctp_api_receive( receive_req.association_id,
                                            &receive_resp.protocol_id,
                                            &receive_resp.data_size,
                                            &receive_resp.stream_id,
                                            receive_resp.data,
                                            SCTP_FALSE,
                                            &receive_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RECEIVE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    receive_resp.association_id = receive_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_receive_res_st ) - sizeof( sctp_U8bit * ),
                                            ( sctp_U8bit * )&receive_resp );

    if( receive_resp.result == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                receive_resp.data_size, ( sctp_U8bit * )receive_resp.data );
    }

    else
    {
        /* report error */
    }

#else

    data = &data_area[0];

    data_size  = MAX_USER_DATA_SIZE;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_receive " ) );

    /* call the api from functional interface */

    ret = sctp_api_receive( association_id,
                            &payload_prot_id,
                            &data_size,
                            &stream_id,
                            data,
                            SCTP_FALSE,
                            &ecode );


    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RECEIVE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, payload_prot_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, data_size );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, stream_id );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                data_size, ( sctp_U8bit * )data );
    }

    else
    {
        /* report error */
    }

#endif /* SCTP_STRUCT_BASED */


    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_receive_ext
 *
 *    DESCRIPTION:
 *    This function processes receive ext api.This functio takes a buffer
 *    pointer containing the buffer recvd from the client. It extracts the
 *    api parameters from the buffer and invokes the functional api, forms
 *    the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_receive_ext(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t      transaction_id;
    sctp_src_id_t        src_id;
    sctp_dest_id_t       dest_id;
    sctp_U8bit           data_area[MAX_USER_DATA_SIZE];
#ifdef SCTP_STRUCT_BASED
    sctp_msg_receive_ext_st        receive_ext_req;
    sctp_msg_receive_ext_res_st   receive_ext_resp;
#else
    sctp_return_t        ret;
    sctp_error_t         ecode;
    sctp_U32bit          association_id;
    sctp_U16bit          stream_id;
    sctp_sockaddr_st     dest_address;
    sctp_U32bit          payload_prot_id;
    sctp_U32bit          data_size;
    sctp_U8bit           *data;
    sctp_Boolean_t       peek_flag;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    receive_ext_resp.data = &data_area[0];
    receive_ext_resp.data_size = MAX_USER_DATA_SIZE;
    /* SPR : 18527 */
    receive_ext_resp.dest_address.family = 0;
    /* SPR : 18527 */

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_receive_ext_st ), ( sctp_U8bit * )&receive_ext_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_receive_ext " ) );

    /* call the api from functional interface */
    receive_ext_resp.result = sctp_api_receive_ext(
                                  receive_ext_req.association_id,
                                  receive_ext_req.stream_id,
                                  &receive_ext_resp.dest_address,
                                  &receive_ext_resp.protocol_id,
                                  &receive_ext_resp.data_size,
                                  receive_ext_resp.data,
                                  SCTP_FALSE,
                                  receive_ext_req.peek_flag,
                                  &receive_ext_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RECEIVE_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    receive_ext_resp.association_id =  receive_ext_req.association_id;
    receive_ext_resp.stream_id =  receive_ext_req.stream_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_receive_ext_res_st ) - sizeof( sctp_U8bit * ),
                                            ( sctp_U8bit * )&receive_ext_resp );

    if( receive_ext_resp.result == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                receive_ext_resp.data_size, receive_ext_resp.data );
    }

    else
    {
        /* report error */
    }

#else
    /* SPR : 18527 */
    dest_address.family = 0;
    /* SPR : 18527 */

    data = &data_area[0];
    data_size = MAX_USER_DATA_SIZE;

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_receive_ext " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&peek_flag );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &stream_id );


    /* call the api from functional interface */
    ret = sctp_api_receive_ext( association_id,
                                stream_id,
                                &dest_address,
                                &payload_prot_id,
                                &data_size,
                                data,
                                SCTP_FALSE,
                                peek_flag,
                                &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RECEIVE_EXT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer,
                                            ( sctp_U16bit )ecode );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, stream_id );
    p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer, &dest_address );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, payload_prot_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, data_size );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                data_size, ( sctp_U8bit * )data );
    }

    else
    {
        /* report error */
    }

#endif /* SCTP_STRUCT_BASED */


    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_receive_part
 *
 *    DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_receive_part(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t     transaction_id;
    sctp_src_id_t       src_id;
    sctp_dest_id_t      dest_id;
    sctp_U8bit          data_area[MAX_USER_DATA_SIZE];

#ifdef SCTP_STRUCT_BASED
    sctp_msg_receive_part_st     receive_part_req;
    sctp_msg_receive_part_res_st  receive_part_resp;
#else
    sctp_return_t       ret;
    sctp_error_t        ecode;
    sctp_U32bit         association_id;
    sctp_U16bit         stream_id;
    sctp_sockaddr_st    dest_address;
    sctp_U32bit         protocol_id;
    sctp_U32bit         data_size;
    sctp_U8bit          *data;
    sctp_U16bit         ssn;
    sctp_U16bit         delivery_num;
    sctp_Boolean_t      peek_flag;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    /* SPR : 18527 */
    receive_part_resp.dest_address.family = 0;
    /* SPR : 18527 */

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_receive_part_st ), ( sctp_U8bit * )&receive_part_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_receive_part " ) );

    receive_part_resp.data = &data_area[0];
    receive_part_resp.data_size = MAX_USER_DATA_SIZE;
    receive_part_resp.stream_id = receive_part_req.stream_id;

    /* call the api from functional interface */
    receive_part_resp.result = sctp_api_receive_partial_data(
                                   receive_part_req.association_id,
                                   receive_part_req.stream_id,
                                   &receive_part_resp.protocol_id,
                                   &receive_part_resp.data_size,
                                   receive_part_resp.data,
                                   &receive_part_resp.ssn,
                                   &receive_part_resp.delivery_num,
                                   receive_part_req.peek_flag,
                                   &receive_part_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RECEIVE_PART_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    receive_part_resp.association_id = receive_part_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_receive_part_res_st ) - sizeof( sctp_U8bit * ),
                                            ( sctp_U8bit * )&receive_part_resp );

    if( receive_part_resp.result == SCTP_SUCCESS )
    {
        if( receive_part_resp.data_size > MAX_USER_DATA_SIZE )
            p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                    MAX_USER_DATA_SIZE, receive_part_resp.data );

        else
            p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                    receive_part_resp.data_size, receive_part_resp.data );
    }

    else
    {
        /* report error */
    }

#else


    __MEMSET( &dest_address, 0, sizeof( sctp_sockaddr_st ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_receive_part " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer,
                                          ( sctp_U32bit * )&peek_flag );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &stream_id );

    data = &data_area[0];
    data_size = MAX_USER_DATA_SIZE;

    /* call the api from functional interface */
    ret = sctp_api_receive_partial_data( association_id,
                                         stream_id,
                                         &protocol_id,
                                         &data_size,
                                         data,
                                         &ssn,
                                         &delivery_num,
                                         peek_flag,
                                         &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RECEIVE_PART_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, stream_id );

    /* Problem: Addr not got from anywhere */
    p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer, &dest_address );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, protocol_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, data_size );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ssn );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, delivery_num );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                data_size, ( sctp_U8bit * )data );
    }

    else
    {
        /* report error */
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_close
 *
 *    DESCRIPTION:
 *    This function processes close api.This function takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api parameters
 *    from the buffer and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_close(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t  transaction_id;
    sctp_src_id_t    src_id;
    sctp_dest_id_t   dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_close_st        close_req;
    sctp_msg_close_res_st   close_resp;
#else
    sctp_return_t    ret;
    sctp_error_t     ecode;
    sctp_U32bit      association_id;
    sctp_Boolean_t   graceful;
    sctp_U16bit      u16_temp;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_close_st ), ( sctp_U8bit * )&close_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_close    " ) );

    /* call the api from functional interface */
    close_resp.result = sctp_api_close( close_req.association_id,
                                        close_req.graceful,
                                        &close_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CLOSE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    close_resp.association_id = close_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_close_res_st ), ( sctp_U8bit * )&close_resp );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_close    " ) );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &u16_temp );
    graceful = ( sctp_Boolean_t )u16_temp;

    /* call the api from functional interface */
    ret = sctp_api_close( association_id,
                          graceful,
                          &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CLOSE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *    FUNCTION :    sctp_proc_api_close_ext
 *
 *    DESCRIPTION:
 *     This function processes close ext api.This function takes a buffer
 *     pointer containing the buffer recvd from the client. It extracts the
 *     api parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_close_ext(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t  transaction_id;
    sctp_src_id_t    src_id;
    sctp_dest_id_t   dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_close_ext_st   close_req;
    sctp_msg_close_res_st   close_resp;
#else
    sctp_return_t    ret;
    sctp_error_t     ecode;
    sctp_U32bit      association_id;
    sctp_U16bit      cause_len;
    sctp_U8bit      *cause = SCTP_NULL;
#endif /* SCTP_STRUCT_BASED */

    src_id         = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id        = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_close_ext_st ), ( sctp_U8bit * )&close_req );
    close_req.cause = p_req_buffer;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_close_ext    " ) );

    /* call the api from functional interface */
    close_resp.result = sctp_api_close_ext( close_req.association_id,
                                            close_req.cause_len,
                                            close_req.cause,
                                            &close_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CLOSE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    close_resp.association_id = close_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_close_res_st ), ( sctp_U8bit * )&close_resp );
#else

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_close_ext    " ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &cause_len );

    cause = p_req_buffer;

    /* call the api from functional interface */
    ret = sctp_api_close_ext( association_id,
                              cause_len,
                              cause,
                              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CLOSE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_hb_action
 *
 *    DESCRIPTION:
 *     This function processes heartbeat action api.This function takes a
 *     buffer pointer containing the buffer recvd from the client. It
 *     extracts the api parameters from the buffer and invokes the functional
 *     api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_hb_action(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t       transaction_id;
    sctp_src_id_t         src_id;
    sctp_dest_id_t        dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_hb_action_st    hb_action_req;
    sctp_msg_hb_action_res_st   hb_action_resp;
#else
    sctp_return_t       ret;
    sctp_error_t        ecode;
    sctp_U32bit         association_id;
    sctp_sockaddr_st    dest_address;
    sctp_HB_action_et   heartbeat_action;
    sctp_U32bit         heartbeat_interval;
    sctp_U8bit          u8_temp;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_hb_action_st ), ( sctp_U8bit * )&hb_action_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_hb_action " ) );


    /* call the api from functional interface */
    hb_action_resp.result = sctp_api_heartbeat_action(
                                hb_action_req.association_id,
                                &hb_action_req.dest_address,
                                hb_action_req.heartbeat_action,
                                hb_action_req.heartbeat_interval,
                                &hb_action_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_HB_ACTION_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    hb_action_resp.association_id = hb_action_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_hb_action_res_st ), ( sctp_U8bit * )&hb_action_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_hb_action " ) );
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &dest_address );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    heartbeat_action = ( sctp_HB_action_et )u8_temp;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &heartbeat_interval );


    /* call the api from functional interface */
    ret = sctp_api_heartbeat_action( association_id,
                                     &dest_address,
                                     heartbeat_action,
                                     heartbeat_interval,
                                     &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_HB_ACTION_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_set_primary
 *
 *    DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_set_primary(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_set_primary_st       set_primary_req;
    sctp_msg_set_primary_res_st   set_primary_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_sockaddr_st   dest_address, src_address;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_primary_st ), ( sctp_U8bit * )&set_primary_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_primary " ) );

    /* call the api from functional interface */

    set_primary_resp.result = sctp_api_set_primary(
                                  set_primary_req.association_id,
                                  &set_primary_req.src_address,
                                  &set_primary_req.dest_address,
                                  &set_primary_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_PRIMARY_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    set_primary_resp.association_id = set_primary_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_primary_res_st ), ( sctp_U8bit * )&set_primary_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_primary " ) );
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &dest_address );

    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &src_address );


    /* call the api from functional interface */

    ret = sctp_api_set_primary( association_id,
                                &src_address,
                                &dest_address,
                                &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_PRIMARY_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_get_srtt
 *
 *    DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_get_srtt(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_srtt_st      get_srtt_req;
    sctp_msg_get_srtt_res_st   get_srtt_resp;
#else
    sctp_return_t       ret;
    sctp_error_t        ecode;
    sctp_U32bit         association_id;
    sctp_sockaddr_st    dest_address;
    sctp_U32bit         srtt_value;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_srtt_st ), ( sctp_U8bit * )&get_srtt_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_srtt " ) );

    /* call the api from functional interface */
    get_srtt_resp.result = sctp_api_get_srtt(
                               get_srtt_req.association_id,
                               &get_srtt_req.dest_address,
                               &get_srtt_resp.srtt_value,
                               &get_srtt_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_SRTT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    get_srtt_resp.association_id = get_srtt_req.association_id;

    get_srtt_resp.dest_address = get_srtt_req.dest_address;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_srtt_res_st ), ( sctp_U8bit * )&get_srtt_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_srtt " ) );
    p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer, &dest_address );

    ret = sctp_api_get_srtt( association_id,
                             &dest_address,
                             &srtt_value,
                             &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_SRTT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer, &dest_address );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, srtt_value );
    }

    else
    {
        /* error */
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :     sctp_proc_api_get_status
 *
 *    DESCRIPTION:
 *     This function processes get status api.This function takes a buffer
 *     pointer containing the buffe recvd from the client. It extracts the
 *     api parameters from the buffer and invokes the functional api,
 *     forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_get_status(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t       transaction_id;
    sctp_src_id_t         src_id;
    sctp_dest_id_t        dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_status_st      get_status_req;
    sctp_msg_get_status_res_st  get_status_resp;
#else
    sctp_return_t          ret;
    sctp_error_t           ecode;
    sctp_U32bit            association_id;
    sctp_status_params_st  status_params;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_status_st ), ( sctp_U8bit * )&get_status_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_status " ) );


    /* call the api from functional interface */
    get_status_resp.result = sctp_api_get_status(
                                 get_status_req.association_id,
                                 &get_status_resp.assoc_status,
                                 &get_status_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STATUS_RESP, transaction_id, dest_id, src_id );
    /* move the rest of the resp fields */
    get_status_resp.association_id = get_status_req.association_id;

    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_status_res_st ), ( sctp_U8bit * )&get_status_resp );
#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_status " ) );

    /* call the api from functional interface */
    ret = sctp_api_get_status( association_id,
                               &status_params,
                               &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STATUS_RESP, transaction_id, dest_id, src_id );
    /* move the rest of the resp fields */

    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.connection_state );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.num_in_streams );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.num_out_streams );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.curr_receiver_win_size );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.num_data_chnks_awaiting_ack );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.num_data_chnks_awaiting_sent );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.last_acked_TSN );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.last_sent_TSN );

        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                               status_params.num_msgs_awaiting_recv );

        p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                   &status_params.primary_dest_addr );
    }

    else
    {
        /* error... no need to copy any further params */
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_get_dest_status
 *
 *    DESCRIPTION:
 *    This function processes get dest status api.This function takes a
 *    buffer pointer containing the buffer recvd from the client. It
 *    extracts the api parameters from the buffer and invokes the
 *    functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_get_dest_status(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_deststatus_st      get_deststatus_req;
    sctp_msg_get_deststatus_res_st  get_deststatus_resp;
#else
    sctp_return_t                   ret;
    sctp_error_t                    ecode;
    sctp_U32bit                     association_id;
    sctp_U32bit                     nos_of_dest_trans_addrs, count;
    sctp_sockaddr_list_st           list_of_dest_trans_addrs;
    sctp_dest_status_param_list_st  dest_param_list;
    sctp_sockaddr_st                primary_dest_transport_addr;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    __MEMSET( &get_deststatus_resp.list_of_dest_trans_addrs, 0,
              sizeof( sctp_sockaddr_list_st ) );
    __MEMSET( &get_deststatus_resp.dest_param_list, 0,
              sizeof( sctp_dest_status_param_list_st ) );
    __MEMSET( &get_deststatus_resp.primary_dest_transport_addr, 0,
              sizeof( sctp_sockaddr_st ) );

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_deststatus_st ), ( sctp_U8bit * )&get_deststatus_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_dest_status " ) );

    /* call the api from functional interface */
    get_deststatus_resp.result =  sctp_api_get_dest_addr_status(
                                      get_deststatus_req.association_id,
                                      &get_deststatus_resp.nos_of_dest_trans_addrs,
                                      get_deststatus_resp.list_of_dest_trans_addrs,
                                      &get_deststatus_resp.dest_param_list,
                                      &get_deststatus_resp.primary_dest_transport_addr,
                                      &get_deststatus_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_DESTSTATUS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    get_deststatus_resp.association_id = get_deststatus_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_deststatus_res_st ),
                                            ( sctp_U8bit * )&get_deststatus_resp );

#else

    __MEMSET( &list_of_dest_trans_addrs, 0, sizeof( sctp_sockaddr_list_st ) );
    __MEMSET( &dest_param_list, 0, sizeof( sctp_dest_status_param_list_st ) );
    __MEMSET( &primary_dest_transport_addr, 0,
              sizeof( primary_dest_transport_addr ) );

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_dest_status " ) );

    /* call the api from functional interface */
    ret = sctp_api_get_dest_addr_status(
              association_id,
              &nos_of_dest_trans_addrs,
              list_of_dest_trans_addrs,
              &dest_param_list    ,
              &primary_dest_transport_addr,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_DESTSTATUS_RESP, transaction_id, dest_id, src_id );

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
                                                   dest_param_list[count].srtt );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_param_list[count].rto );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_param_list[count].mtu_size );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_param_list[count].cwnd );
            /* Changes for CSR# 1-4267892 */
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_param_list[count].allow_heartbeat );
            p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer,
                                                   dest_param_list[count].status );
            /* Changes for CSR# 1-4267892 Ends*/
        }

        p_resp_buffer = sctp_buffer_pack_sockaddr( p_resp_buffer,
                                                   &primary_dest_transport_addr );
    }

    else
    {
        /* error */
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *    FUNCTION :    sctp_proc_api_set_restart_option
 *
 *    DESCRIPTION:
 *     This function takes a buffer pointer containing the buffer
 *    recvd from the client. It extracts the api parameters from the buffer
 *    and invokes the functional api, forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_set_restart_option(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t     transaction_id;
    sctp_src_id_t       src_id;
    sctp_dest_id_t      dest_id;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_set_restart_option_st       rst_opt_req;
    sctp_msg_set_restart_option_res_st   rst_opt_resp;
#else
    sctp_return_t                 ret;
    sctp_error_t                  ecode;
    sctp_U32bit                   association_id, temp;
    sctp_restart_options_et       restart_option;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_set_restart_option_st ), ( sctp_U8bit * )&rst_opt_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_restart_option " ) );

    /* call the api from functional interface */

    rst_opt_resp.result = sctp_api_set_restart_option(
                              rst_opt_req.association_id,
                              rst_opt_req.restart_option,
                              &rst_opt_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_RESTART_OPTION_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    rst_opt_resp.association_id = rst_opt_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_set_restart_option_res_st ),
                                            ( sctp_U8bit * )&rst_opt_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &temp );

    restart_option = ( sctp_restart_options_et )temp;
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_set_restart_option " ) );

    /* call the api from functional interface */
    ret = sctp_api_set_restart_option( association_id, restart_option, &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_SET_RESTART_OPTION_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_proc_api_ret_unsent
 *
 *    DESCRIPTION:
 *     This function processes retreival unsent api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms the
 *     resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_ret_unsent(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t  transaction_id;
    sctp_src_id_t    src_id;
    sctp_dest_id_t   dest_id;
    sctp_U8bit       data[MAX_USER_DATA_SIZE];

#ifdef SCTP_STRUCT_BASED
    sctp_msg_ret_unsent_st    ret_unsent_req;
    sctp_msg_ret_unsent_res_st   ret_unsent_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id, context, ppi;
    sctp_U16bit        stream_id;
    sctp_U32bit        data_size;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    ret_unsent_resp.data_size = MAX_USER_DATA_SIZE;
    ret_unsent_resp.data = &data[0];

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_ret_unsent_st ), ( sctp_U8bit * )&ret_unsent_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_ret_unsent " ) );

    /* call the api from functional interface */
    ret_unsent_resp.result = sctp_api_retrieve_unsent(
                                 ret_unsent_req.association_id,
                                 &ret_unsent_resp.context,
                                 ret_unsent_resp.data,
                                 &ret_unsent_resp.data_size,
                                 &ret_unsent_resp.stream,
                                 &ret_unsent_resp.ppi,
                                 &ret_unsent_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RETRIEVE_UNSENT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    ret_unsent_resp.association_id = ret_unsent_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_ret_unsent_res_st ) - sizeof( sctp_U8bit * ),
                                            ( sctp_U8bit * )&ret_unsent_resp );

    if( ret_unsent_resp.result == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                ret_unsent_resp.data_size, ret_unsent_resp.data );
    }

#else

    data_size = MAX_USER_DATA_SIZE;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_ret_unsent " ) );

    /* call the api from functional interface */
    ret = sctp_api_retrieve_unsent( association_id,
                                    &context,
                                    &data[0],
                                    &data_size,
                                    &stream_id,
                                    &ppi,
                                    &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RETRIEVE_UNSENT_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, context );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, data_size );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ppi );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, stream_id );
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                data_size, &data[0] );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}
/***************************************************************************
 *    FUNCTION :   sctp_proc_api_ret_unacked
 *
 *    DESCRIPTION:
 *    This function processes retreive unacked api.It takes a buffer pointer
 *    containing the buffer recvd from the client. It extracts the api
 *    parameters from the buffer and invokes the functional api, forms the
 *    resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_ret_unacked(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{

    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
    sctp_U8bit         data[MAX_USER_DATA_SIZE];

#ifdef SCTP_STRUCT_BASED
    sctp_msg_ret_unacked_st      ret_unacked_req;
    sctp_msg_ret_unacked_res_st   ret_unacked_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id, context, ppi;
    sctp_U16bit        stream_id, ssn;
    sctp_Boolean_t     incomplete_flag;
    sctp_U32bit        data_size;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    ret_unacked_resp.data_size  = MAX_USER_DATA_SIZE;
    ret_unacked_resp.data = &data[0];
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_ret_unacked_st ),
                                           ( sctp_U8bit * )&ret_unacked_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_ret_unacked " ) );

    /* call the api from functional interface */
    ret_unacked_resp.result = sctp_api_retrieve_unacked(
                                  ret_unacked_req.association_id,
                                  &ret_unacked_resp.context,
                                  ret_unacked_resp.data,
                                  &ret_unacked_resp.data_size,
                                  &ret_unacked_resp.incomplete_flag,
                                  &ret_unacked_resp.stream,
                                  &ret_unacked_resp.ssn,
                                  &ret_unacked_resp.ppi,
                                  &ret_unacked_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RETRIEVE_UNACKED_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    ret_unacked_resp.association_id = ret_unacked_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_ret_unacked_res_st ) - sizeof( sctp_U8bit * ),
                                            ( sctp_U8bit * )&ret_unacked_resp );

    if( ret_unacked_resp.result == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                ret_unacked_resp.data_size, ret_unacked_resp.data );
    }

#else

    data_size = MAX_USER_DATA_SIZE;
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_ret_unacked " ) );

    /* call the api from functional interface */
    ret = sctp_api_retrieve_unacked( association_id,
                                     &context,
                                     &data[0],
                                     &data_size,
                                     &incomplete_flag,
                                     &stream_id,
                                     &ssn,
                                     &ppi,
                                     &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_RETRIEVE_UNACKED_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );

    if( ret == SCTP_SUCCESS )
    {
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, context );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, data_size );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ppi );
        p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, incomplete_flag );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, stream_id );
        p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ssn );
        p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                                data_size, &data[0] );
    }

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *    FUNCTION :    sctp_proc_api_flush_buffers
 *
 *    DESCRIPTION:
 *     This function processes flush buffers api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_flush_buffers(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_flush_buffers_st    flush_buffers_req;
    sctp_msg_flush_buffers_res_st   flush_buffers_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_flush_buffers_st ), ( sctp_U8bit * )&flush_buffers_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_flush_buffers    " ) );

    /* call the api from functional interface */
    flush_buffers_resp.result = sctp_api_flush_buffers(
                                    flush_buffers_req.association_id,
                                    &flush_buffers_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_FLUSH_BUFFERS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    flush_buffers_resp.association_id = flush_buffers_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_flush_buffers_res_st ),
                                            ( sctp_U8bit * )&flush_buffers_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_flush_buffers    " ) );


    /* call the api from functional interface */
    ret = sctp_api_flush_buffers( association_id, &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_FLUSH_BUFFERS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ( sctp_U16bit )ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/***************************************************************************
 *    FUNCTION :    sctp_proc_api_get_stream_status
 *
 *    DESCRIPTION:
 *     This function processes get stream status api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms the
 *     resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_get_stream_status(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_stream_status_st    get_stream_status_req;
    sctp_msg_get_stream_status_res_st   get_stream_status_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_U16bit        stream_id, ssn;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_stream_status_st ),
                                           ( sctp_U8bit * )&get_stream_status_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_stream_status    " ) );

    /* call the api from functional interface */

    get_stream_status_resp.result = sctp_api_get_stream_status(
                                        get_stream_status_req.association_id,
                                        get_stream_status_req.stream_id,
                                        &get_stream_status_resp.ssn,
                                        &get_stream_status_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STREAM_STATUS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    get_stream_status_resp.association_id =
        get_stream_status_req.association_id;
    get_stream_status_resp.stream_id = get_stream_status_req.stream_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_stream_status_res_st ),
                                            ( sctp_U8bit * )&get_stream_status_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &stream_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_stream_status    " ) );


    /* call the api from functional interface */
    ret = sctp_api_get_stream_status(
              association_id,
              stream_id,
              &ssn,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_STREAM_STATUS_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, stream_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ssn );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_proc_api_abort_retrieve
 *
 *    DESCRIPTION:
 *     This function processes abort retrieve api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms the
 *     resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_abort_retrieve(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_abort_retrieve_st          abort_retrieve_req;
    sctp_msg_abort_retrieve_res_st      abort_retrieve_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_Boolean_t     data_pending;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_abort_retrieve_st ),
                                           ( sctp_U8bit * )&abort_retrieve_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_abort_retrieve   " ) );

    /* call the api from functional interface */

    abort_retrieve_resp.result = sctp_api_abort_retrieve(
                                     abort_retrieve_req.association_id,
                                     &abort_retrieve_resp.data_pending,
                                     &abort_retrieve_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ABORT_RETRIEVE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    abort_retrieve_resp.association_id =
        abort_retrieve_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_abort_retrieve_res_st ),
                                            ( sctp_U8bit * )&abort_retrieve_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_abort_retrieve    " ) );

    /* call the api from functional interface */
    ret = sctp_api_abort_retrieve(
              association_id,
              &data_pending,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_ABORT_RETRIEVE_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, data_pending );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_proc_api_config_cong_reg_def
 *
 *    DESCRIPTION:
 *     This function processes config cong reg def api.It takes a buffer
 *     pointer containing the buffer recvd from the client. It extracts
 *     the api parameters from the buffer and invokes the functional api,
 *     forms the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_config_cong_reg_def(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_config_cong_reg_def_st          config_cong_req;
    sctp_msg_config_cong_reg_def_resp_st     config_cong_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_Boolean_t     is_transmit;
    sctp_cong_reg_def_st cong_reg_def;
    sctp_U8bit          u8_temp;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_cong_reg_def_st ),
                                           ( sctp_U8bit * )&config_cong_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_cong_reg_def" ) );

    /* call the api from functional interface */

    config_cong_resp.result = sctp_api_config_cong_reg_def(
                                  config_cong_req.association_id,
                                  config_cong_req.cong_reg_def,
                                  config_cong_req.is_transmit,
                                  &config_cong_resp.errorno );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_CONG_REG_DEF_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    config_cong_resp.association_id =
        config_cong_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_config_cong_reg_def_resp_st ),
                                            ( sctp_U8bit * )&config_cong_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.no_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.no_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.low_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.low_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.med_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.med_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.high_cong_lvl_abate );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &cong_reg_def.high_cong_lvl_onset );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    is_transmit  = ( sctp_Boolean_t ) u8_temp;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_cong_reg_def    " ) );

    /* call the api from functional interface */
    ret = sctp_api_config_cong_reg_def(
              association_id,
              cong_reg_def,
              is_transmit,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_CONG_REG_DEF_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_proc_api_get_cong_reg_def
 *
 *    DESCRIPTION:This function processes get cong reg def api.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_get_cong_reg_def(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_cong_reg_def_st          config_cong_req;
    sctp_msg_get_cong_reg_def_resp_st     config_cong_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_Boolean_t     is_transmit;
    sctp_cong_reg_def_st cong_reg_def;
    sctp_U8bit          u8_temp;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_cong_reg_def_st ),
                                           ( sctp_U8bit * )&config_cong_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_cong_reg_def" ) );

    /* call the api from functional interface */

    config_cong_resp.result = sctp_api_get_cong_reg_def(
                                  config_cong_req.association_id,
                                  &( config_cong_resp.cong_reg_def ),
                                  config_cong_req.is_transmit,
                                  &( config_cong_resp.errorno ) );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_CONG_REG_DEF_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    config_cong_resp.association_id =
        config_cong_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_cong_reg_def_resp_st ),
                                            ( sctp_U8bit * )&config_cong_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    is_transmit  = ( sctp_Boolean_t ) u8_temp;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_cong_reg_def    " ) );

    /* call the api from functional interface */
    ret = sctp_api_get_cong_reg_def(
              association_id,
              &cong_reg_def,
              is_transmit,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_CONG_REG_DEF_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.no_cong_lvl_onset );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.no_cong_lvl_abate );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.low_cong_lvl_onset );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.low_cong_lvl_abate );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.med_cong_lvl_onset );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.med_cong_lvl_abate );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.high_cong_lvl_onset );
    p_resp_buffer = sctp_buffer_set_Uchar( p_resp_buffer, cong_reg_def.high_cong_lvl_abate );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_proc_api_config_txrx_num_frag
 *
 *    DESCRIPTION:This function processes config txrx num frag api.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_config_txrx_num_frag(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_config_txrx_num_frag_st      config_req;
    sctp_msg_config_txrx_num_frag_resp_st      config_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_Boolean_t     is_transmit;
    sctp_Boolean_t     is_increment;
    sctp_count_frags_st count_frags;
    sctp_U8bit          u8_temp;
#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_config_txrx_num_frag_st ),
                                           ( sctp_U8bit * )&config_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_txrx_num_frag" ) );

    /* call the api from functional interface */

    config_resp.result = sctp_api_config_txrx_num_frag(
                             config_req.association_id,
                             config_req.count_frags,
                             config_req.is_transmit,
                             config_req.is_increment,
                             &( config_resp.errorno ) );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_TXRX_NUM_FRAG_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    config_resp.association_id =
        config_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_config_txrx_num_frag_resp_st ),
                                            ( sctp_U8bit * )&config_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &count_frags.min_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &count_frags.med_num_frag );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &count_frags.max_num_frag );
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    is_transmit  = ( sctp_Boolean_t ) u8_temp;
    p_req_buffer = sctp_buffer_get_Uchar( p_req_buffer, &u8_temp );
    is_increment  = ( sctp_Boolean_t ) u8_temp;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_config_txrx_num_frag    " ) );

    /* call the api from functional interface */
    ret = sctp_api_config_txrx_num_frag(
              association_id,
              count_frags,
              is_transmit,
              is_increment,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONFIG_TXRX_NUM_FRAG_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_proc_api_get_free_txrx_num_frag
 *
 *    DESCRIPTION: This function pocesses get free txrx num frag api.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_proc_api_get_free_txrx_num_frag(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t    transaction_id;
    sctp_src_id_t      src_id;
    sctp_dest_id_t     dest_id;
#ifdef SCTP_STRUCT_BASED
    sctp_msg_get_free_tr_num_frag_st          txrx_req;
    sctp_msg_get_free_tr_num_frag_resp_st     txrx_resp;
#else
    sctp_return_t      ret;
    sctp_error_t       ecode;
    sctp_U32bit        association_id;
    sctp_count_frags_st  tx_num_frags;
    sctp_count_frags_st  rx_num_frags;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;

#ifdef SCTP_STRUCT_BASED

    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_get_free_tr_num_frag_st ),
                                           ( sctp_U8bit * )&txrx_req );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_free_txrx_num_frag" ) );

    /* call the api from functional interface */

    txrx_resp.result = sctp_api_get_free_txrx_num_frag(
                           txrx_req.association_id,
                           &( txrx_resp.tx_num_frags ),
                           &( txrx_resp.rx_num_frags ),
                           &( txrx_resp.errorno ) );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_FREE_TR_NUM_FRAG_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    txrx_resp.association_id =
        txrx_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_get_free_tr_num_frag_resp_st ),
                                            ( sctp_U8bit * )&txrx_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_api_get_free_txrx_num_frag    " ) );

    /* call the api from functional interface */
    ret = sctp_api_get_free_txrx_num_frag(
              association_id,
              &tx_num_frags,
              &rx_num_frags,
              &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_GET_FREE_TR_NUM_FRAG_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ret );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, ecode );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, tx_num_frags.min_num_frag );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, tx_num_frags.med_num_frag );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, tx_num_frags.max_num_frag );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, rx_num_frags.min_num_frag );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, rx_num_frags.med_num_frag );
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, rx_num_frags.max_num_frag );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}




/***************************************************************************
 *    FUNCTION :    sctp_proc_api_connectx
 *
 *    DESCRIPTION:
 *     This function processes connectx api.It takes a buffer pointer
 *     containing the buffer recvd from the client. It extracts the api
 *     parameters from the buffer and invokes the functional api, forms
 *     the resp and returns it.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 *************************************************************************/
sctp_U8bit *
sctp_proc_api_connectx(
    sctp_U8bit *p_req_buffer,
    sctp_U8bit *p_resp_buffer )
{
    sctp_trans_id_t           transaction_id;
    sctp_src_id_t             src_id;
    sctp_dest_id_t            dest_id;
    sctp_U32bit               nos_of_dest_trans_addrs;
    sctp_sockaddr_list_st     list_of_dest_trans_addrs;

#ifdef SCTP_STRUCT_BASED
    sctp_msg_connectx_st      connectx_req;
    sctp_msg_connectx_res_st  connectx_resp;
#else
    sctp_return_t          ret;
    sctp_error_t           ecode;
    sctp_U32bit            association_id;
    sctp_U16bit            dest_sctp_port;
    sctp_U32bit            payload_prot_id;
    sctp_sockaddr_list_st  list_su_addrs;
    sctp_sockaddr_st       *p_addr;
    sctp_U32bit            no_of_su_addrs;
    sctp_U32bit            count;
    sctp_Boolean_t         block;
    sctp_U16bit            p_num_ostreams;
    sctp_U16bit            p_num_istreams;

#endif /* SCTP_STRUCT_BASED */

    src_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 4 );
    dest_id = sctp_buffer_get_UshortAtOffset( p_req_buffer, 6 );
    transaction_id = sctp_buffer_get_UlongAtOffset( p_req_buffer, 8 );

    p_req_buffer = p_req_buffer + SCTP_API_HEADER_LEN;


#ifdef SCTP_STRUCT_BASED
    p_req_buffer = sctp_buffer_get_Buffer( p_req_buffer,
                                           sizeof( sctp_msg_connectx_st ), ( sctp_U8bit * )&connectx_req );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_connect " ) );

    /* call the api from functional interface */
    connectx_resp.result = sctp_api_connectx_ext( connectx_req.association_id,
                                                  connectx_req.dest_sctp_port,
                                                  connectx_req.payload_prot_id,
                                                  connectx_req.block,
                                                  connectx_req.num_dest_addrs,
                                                  connectx_req.connect_dest_address,
                                                  &connectx_req.p_num_ostreams,
                                                  &connectx_req.p_num_istreams,
                                                  &nos_of_dest_trans_addrs,
                                                  list_of_dest_trans_addrs,
                                                  &connectx_resp.error );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONNECTX_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    connectx_resp.association_id =  connectx_req.association_id;
    p_resp_buffer = sctp_buffer_set_Buffer( p_resp_buffer,
                                            sizeof( sctp_msg_connectx_res_st ), ( sctp_U8bit * )&connectx_resp );

#else

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &association_id );
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_proc_connectx " ) );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &dest_sctp_port );

    p_addr = ( sctp_sockaddr_st * )list_su_addrs;

    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &no_of_su_addrs );

    for( count = 0; count < no_of_su_addrs; count++ )
    {
        p_req_buffer = sctp_buffer_unpack_sockaddr( p_req_buffer,
                                                    &( p_addr[count] ) );
    }

    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, ( sctp_U16bit * )&block );
    p_req_buffer = sctp_buffer_get_Ulong( p_req_buffer, &payload_prot_id );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &p_num_ostreams );
    p_req_buffer = sctp_buffer_get_Ushort( p_req_buffer, &p_num_istreams );

    /* call the api from functional interface */

    ret = sctp_api_connectx_ext( association_id,
                                 dest_sctp_port,
                                 payload_prot_id,
                                 block,
                                 no_of_su_addrs,
                                 p_addr,
                                 &p_num_ostreams,
                                 &p_num_istreams,
                                 &nos_of_dest_trans_addrs,
                                 list_of_dest_trans_addrs,
                                 &ecode );

    /* now form the response buf */
    /* build header , interchange src and dest */
    p_resp_buffer = sctp_ui_build_api_header( p_resp_buffer,
                                              SCTP_API_CONNECTX_RESP, transaction_id, dest_id, src_id );

    /* move the rest of the resp fields */
    p_resp_buffer = sctp_buffer_set_Ulong( p_resp_buffer, association_id );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ret );
    p_resp_buffer = sctp_buffer_set_Ushort( p_resp_buffer, ( sctp_U16bit )ecode );

#endif /* SCTP_STRUCT_BASED */

    return( p_resp_buffer );
}


/************* functions to send notifications **********/
/***************************************************************************
 *    FUNCTION :    sctp_ui_send_notifications
 *
 *    DESCRIPTION:
 *     This function build notifications in buffer format and sends.
 *     buffer  format.
 *    RETURN VALUE:
 *    SCTP_SUCCESS
 *        SCTP_FALILURE
 *
 **************************************************************************/
sctp_return_t
sctp_ui_send_notifications(
    sctp_U16bit            su_id,
    sctp_U32bit            event,
    sctp_notice_ut         *ntf_data )
{
    sctp_U8bit              *p_ntf_buffer;
    sctp_U8bit              *p_notf_buffer_offset;
    sctp_U16bit             noctets;
    sctp_return_t           ret = SCTP_SUCCESS;
    sctp_Error_t            ecode;
    /*
     * Allocate the memory for response message. The memory for this pointer
     * is allocated using qvMesgAllocFromModule.
     */
    sctp_U8bit       *p_notify_message;

#ifdef SCTP_SEND_DATA_WITH_IND
    sctp_U8bit    *p_header_offset;
#endif

    p_notify_message = __MESG_MALLOC( SCTP_APIS_MESG_POOL, SCTP_MAX_API_SIZE );

    if( SCTP_NULL == p_notify_message )
    {
        return SCTP_FAILURE;
    }

    p_ntf_buffer = p_notify_message;

    __MEMSET( p_ntf_buffer, 0, SCTP_MAX_API_SIZE );

#ifdef SCTP_RED_SUPPORT
    p_ntf_buffer = p_ntf_buffer + 8;
    p_notf_buffer_offset = &p_notify_message[8];
#else
    p_notf_buffer_offset = &p_notify_message[0];
#endif

    p_ntf_buffer = sctp_ui_build_api_header( p_ntf_buffer,
                                             0, /* place holder for API id */
                                             SCTP_NOTF_TRANS_ID,
                                             sctp_client_list[su_id].stack_id,  /*stack id */
                                             su_id );

    switch( event )
    {
        case SCTPNOTIFY_OPERROR:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_OP_ERROR_NOTF );
            p_ntf_buffer =  sctp_ui_build_op_error_notf( p_ntf_buffer,
                                                         ntf_data );
            break;

        case SCTPNOTIFY_DESTSTATCHANGE:
            sctp_buffer_set_Ushort( p_notf_buffer_offset,
                                    SCTP_API_DESTINATION_STATUS_CHG_NOTF );
            p_ntf_buffer =  sctp_ui_build_dest_status_chg_notf( p_ntf_buffer,
                                                                ntf_data );
            break;

        case SCTPNOTIFY_ENDSTATCHANGE:
            sctp_buffer_set_Ushort( p_notf_buffer_offset,
                                    SCTP_API_ENDPOINT_STATUS_CHG_NOTF );
            p_ntf_buffer = sctp_ui_build_endpoint_status_chg_notf( p_ntf_buffer,
                                                                   ntf_data );
            break;

        case SCTPNOTIFY_CONGESTION:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_CONGESTION_NOTF );
            p_ntf_buffer = sctp_ui_build_congestion_notf( p_ntf_buffer,
                                                          ntf_data );
            break;

        case SCTPNOTIFY_SYSERROR:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_SYS_ERROR_NOTF );
            p_ntf_buffer =  sctp_ui_build_sys_error_notf( p_ntf_buffer,
                                                          ntf_data );
            break;

        case SCTPNOTIFY_CONNPENDING:
            sctp_buffer_set_Ushort( p_notf_buffer_offset,
                                    SCTP_API_CONNECTION_PENDING_NOTF );
            p_ntf_buffer = sctp_ui_build_conn_pending_notf( p_ntf_buffer,
                                                            ntf_data );
            break;

        case SCTPNOTIFY_DATAPENDING:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_DATA_PENDING_NOTF );

#ifdef SCTP_SEND_DATA_WITH_IND

            for( ; ; )
            {
                p_header_offset = p_ntf_buffer;

                p_ntf_buffer = sctp_ui_build_data_pending_notf( p_header_offset,
                                                                ntf_data );

                if( p_ntf_buffer == SCTP_NULL )
                {
                    __MESG_FREE( p_notify_message );
                    return( SCTP_SUCCESS );
                }

                noctets = p_ntf_buffer - &p_notify_message[0];
#ifdef SCTP_RED_SUPPORT

                noctets = noctets - 8;
                p_ntf_buffer = &p_notify_message[8];

#else

                p_ntf_buffer = &p_notify_message[0];

#endif
                sctp_buffer_set_UlongAtOffset( p_ntf_buffer,
                                               APIBUF_LENGTH_OFFSET, ( sctp_U32bit )noctets );
                p_ntf_buffer = &p_notify_message[0];

#ifdef SCTP_RED_SUPPORT

                noctets = noctets + 8;
                sctp_buffer_set_UlongAtOffset( p_ntf_buffer, 0,
                                               ( sctp_U32bit )SCTP_EVT_RESP_NOTF );
                sctp_buffer_set_UlongAtOffset( p_ntf_buffer, 4,
                                               noctets );

#endif

                /* Call send_from_sctp from here with the notf buffer */

                if( send_from_sctp( p_ntf_buffer, noctets, su_id,
                                    sctp_client_list[su_id].stack_id, SCTP_CLIENT_SU,
                                    SCTP_NULL, &ecode ) != SCTP_SUCCESS )
                {
                    /* some Error */
                    __MESG_FREE( p_notify_message );
                    return( SCTP_FAILURE );
                }

                p_notify_message = __MESG_MALLOC( SCTP_APIS_MESG_POOL, SCTP_MAX_API_SIZE );

                if( SCTP_NULL == p_notify_message )
                {
                    return SCTP_FAILURE;
                }

                p_ntf_buffer = p_notify_message;

                __MEMSET( p_ntf_buffer, 0, SCTP_MAX_API_SIZE );

#ifdef SCTP_RED_SUPPORT

                p_ntf_buffer = p_ntf_buffer + 8;
                p_notf_buffer_offset = &p_notify_message[8];
#else
                p_notf_buffer_offset = &p_notify_message[0];
#endif

                p_ntf_buffer = sctp_ui_build_api_header( p_ntf_buffer,
                                                         0, /* place holder for API id */
                                                         SCTP_NOTF_TRANS_ID,
                                                         sctp_client_list[su_id].stack_id,  /*stack id */
                                                         su_id );

                sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_DATA_PENDING_NOTF );
            }

#else

            p_ntf_buffer = sctp_ui_build_data_pending_notf( p_ntf_buffer,
                                                            ntf_data );
            break;

#endif /* SCTP_SEND_DATA_WITH_IND */

        case SCTPNOTIFY_SENDFAILURE:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_SEND_FAILURE_NOTF );
            p_ntf_buffer = sctp_ui_build_snd_failure_notf( p_ntf_buffer,
                                                           ntf_data );
            break;

        case SCTPNOTIFY_PARTIAL_DATAPENDING:
            sctp_buffer_set_Ushort( p_notf_buffer_offset,
                                    SCTP_API_PART_DATA_PENDING_NOTF );
            p_ntf_buffer = sctp_ui_build_partial_datapending_notf( p_ntf_buffer,
                                                                   ntf_data );
            break;

        case SCTPNOTIFY_RECVCONGESTION:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_RECV_CONGESTION_NOTF );
            p_ntf_buffer = sctp_ui_build_recv_congestion_notf( p_ntf_buffer,
                                                               ntf_data );
            break;

        case SCTPNOTIFY_ADAPTION_IND:
            /*SPR 4952*/
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_ADAPTION_IND_NOTF );
            /*SPR 4952*/
            p_ntf_buffer = sctp_ui_build_adaption_ind( p_ntf_buffer,
                                                       ntf_data );
            break;

        case SCTPNOTIFY_PEER_DATA_DROP:
            sctp_buffer_set_Ushort( p_notf_buffer_offset, SCTP_API_PEER_DATA_DROP_NOTF );
            p_ntf_buffer = sctp_ui_build_peer_data_drop_ind( p_ntf_buffer,
                                                             ntf_data );

        case SCTPNOTIFY_SHUTDOWN_EVENT:
            sctp_buffer_set_Ushort( p_notf_buffer_offset,
                                    SCTP_API_SHUTDOWN_EVENT_NOTF );
            p_ntf_buffer = sctp_ui_build_shutdown_event_notf( p_ntf_buffer,
                                                              ntf_data );
            break;
    }

    noctets = p_ntf_buffer - &p_notify_message[0];

#ifdef SCTP_RED_SUPPORT
    noctets = noctets - 8;
    p_ntf_buffer = &p_notify_message[8];
#else
    p_ntf_buffer = &p_notify_message[0];
#endif

    sctp_buffer_set_UlongAtOffset( p_ntf_buffer,
                                   APIBUF_LENGTH_OFFSET, ( sctp_U32bit )noctets );

    p_ntf_buffer = &p_notify_message[0];

#ifdef SCTP_RED_SUPPORT
    /*
     * Set the event type incase of redundancy.
     */
    noctets = noctets + 8;
    sctp_buffer_set_UlongAtOffset( p_ntf_buffer, 0,
                                   ( sctp_U32bit )SCTP_EVT_RESP_NOTF );
    sctp_buffer_set_UlongAtOffset( p_ntf_buffer, 4,
                                   noctets );

#endif

    /* call send_from_sctp from here with the notf buffer */
    ret = send_from_sctp( p_ntf_buffer,
                          noctets, su_id, sctp_client_list[su_id].stack_id, SCTP_CLIENT_SU,
                          SCTP_NULL, &ecode );

    if( ret != SCTP_SUCCESS )
    {
        /* Some error */
        __MESG_FREE( p_notify_message );
    }

    return( ret );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_op_error_notf
 *
 *    DESCRIPTION:
 *     This function build op_peer error notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_op_error_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{

#ifndef SCTP_STRUCT_BASED

    sctp_U32bit count;

#endif /* SCTP_STRUCT_BASED */

    sctp_msg_op_error_peer_notf_st  *error_notf;

    error_notf = ( sctp_msg_op_error_peer_notf_st * ) &ntf_data->operror;

    /* Set the API Id first */
#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_op_error_peer_notf_st )
                                           - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )error_notf );

    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           error_notf->err_length,
                                           ( sctp_U8bit * )error_notf->sys_error_info );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          error_notf->association_id );

    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           error_notf->err_code );

    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           error_notf->err_length );

    for( count = 0; count < error_notf->err_length; count++ )
    {
        sctp_buffer_set_Uchar( p_ntf_buffer, error_notf->sys_error_info[count] );
    }

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_op_error_notf    " ) );


    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_dest_chg_notf
 *
 *    DESCRIPTION:
 *     This function build destination status change notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_dest_status_chg_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_destination_status_chg_notf_st  *dest_status_chg;

    dest_status_chg =
        ( sctp_msg_destination_status_chg_notf_st * ) &ntf_data->dststatchange;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_destination_status_chg_notf_st ),
                                           ( sctp_U8bit * )dest_status_chg );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          dest_status_chg->association_id );
    p_ntf_buffer = sctp_buffer_pack_sockaddr( p_ntf_buffer,
                                              &dest_status_chg->destination_ip_add );

    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           ( sctp_U16bit )dest_status_chg->dest_act_status );

#endif /* SCTP_STRUCT_BASED */

    if( dest_status_chg->dest_act_status == SCTP_ADDRESS_AVAILABLE )
    {
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_DEST_ACTIVE, 1 );
    }

    else if( dest_status_chg->dest_act_status == SCTP_ADDRESS_UNREACHABLE )
    {
        SCTP_INCR_PROT_INTSTATS( SCTPSTATS_NO_DEST_INACTIVE, 1 );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_dest_status_chg_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_endpoint_status_chg_notf
 *
 *    DESCRIPTION:
 *     This function build endpoint status change notificaion.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_endpoint_status_chg_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_endpoint_status_chg_notf_st  *endpoint_status_chg;

#ifndef SCTP_STRUCT_BASED

    sctp_U32bit   last_sent_tsn, last_acked_tsn, num_addrs;
    sctp_U16bit   in_streams = 0, out_streams = 0, that_port = 0, cause_len = 0;

#endif /* SCTP_STRUCT_BASED */

    endpoint_status_chg =
        ( sctp_msg_endpoint_status_chg_notf_st * )&ntf_data->endstatchange;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_endpoint_status_chg_notf_st ),
                                           ( sctp_U8bit * )endpoint_status_chg );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          endpoint_status_chg->association_id );

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          ( sctp_U32bit )endpoint_status_chg->endpoint_status );

    if( ( endpoint_status_chg->endpoint_status == SCTP_COMMUNICATION_UP ) ||
            ( endpoint_status_chg->endpoint_status == SCTP_RESTART ) )
    {
        num_addrs = endpoint_status_chg->status_change_data.
                    endpoint_up.num_dest_addrs;
        in_streams = endpoint_status_chg->status_change_data.
                     endpoint_up.num_in_streams;
        out_streams = endpoint_status_chg->status_change_data.
                      endpoint_up.num_out_streams;

        p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer, num_addrs );
        p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer, in_streams );
        p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer, out_streams );

        for( ; num_addrs > 0; num_addrs-- )
        {
            p_ntf_buffer = sctp_buffer_pack_sockaddr( p_ntf_buffer,
                                                      &endpoint_status_chg->status_change_data.endpoint_up.
                                                      list_of_addrs[num_addrs - 1] );
        }
    }

    else if( endpoint_status_chg->endpoint_status
             == SCTP_ENDPOINT_DOWN_RETRIEVABLE )
    {
        last_sent_tsn = endpoint_status_chg->status_change_data.
                        endpoint_retrievable.last_sent_tsn;
        last_acked_tsn = endpoint_status_chg->status_change_data.
                         endpoint_retrievable.last_acked_tsn;

        p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer, last_sent_tsn );

        p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer, last_acked_tsn );
    }

    else if( endpoint_status_chg->endpoint_status == SCTP_COMMUNICATION_LOST )
    {
        num_addrs = endpoint_status_chg->status_change_data.
                    endpoint_down.num_dest_addrs;
        that_port = endpoint_status_chg->status_change_data.
                    endpoint_down.that_port;

        p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer, num_addrs );
        p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer, that_port );

        for( ; num_addrs > 0; num_addrs-- )
        {
            p_ntf_buffer = sctp_buffer_pack_sockaddr( p_ntf_buffer,
                                                      &endpoint_status_chg->status_change_data.endpoint_down.
                                                      list_of_addrs[num_addrs - 1] );
        }

        /* Set the Cause Length */
        cause_len = endpoint_status_chg->status_change_data.
                    endpoint_down.cause_len;
        p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer, cause_len );

        if( cause_len != 0 )
        {
            p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer, cause_len,
                                                   endpoint_status_chg->status_change_data.endpoint_down.cause );
        }
    }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    else if( endpoint_status_chg->endpoint_status == SCTP_ENDPOINT_ADDR_CHNG )
    {
        p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                              endpoint_status_chg->status_change_data.endpoint_addr_chng.status );
        p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                              endpoint_status_chg->status_change_data.endpoint_addr_chng.response );
        p_ntf_buffer = sctp_buffer_pack_sockaddr( p_ntf_buffer,
                                                  &endpoint_status_chg->status_change_data.endpoint_addr_chng.
                                                  src_addr );
    }

#endif

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_endpoint_status_chg_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_congestion_notf
 *
 *    DESCRIPTION:
 *     This function build congestion notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_congestion_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_congestion_notf_st  *cong;

    cong = ( sctp_msg_congestion_notf_st * )&ntf_data->congestion;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_congestion_notf_st ), ( sctp_U8bit * )cong );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          cong->association_id );
    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           ( sctp_U16bit )cong->cong_level );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_congestion_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_sys_error_notf
 *
 *    DESCRIPTION:
 *     This function build sys error notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_sys_error_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_sys_error_notf_st  *sys_error;

    sys_error = ( sctp_msg_sys_error_notf_st * )&ntf_data->syserror;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_sys_error_notf_st ), ( sctp_U8bit * )sys_error );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          sys_error->association_id );
    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           ( sctp_U16bit )sys_error->errorno );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_sys_error_notf " ) );

    return( p_ntf_buffer );
}


/***************************************************************************
 *    FUNCTION :    sctp_ui_build_conn_pending_notf
 *
 *    DESCRIPTION:
 *     This function build connection pending notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_conn_pending_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_connection_pending_notf_st  *conn_pending;

    conn_pending =
        ( sctp_msg_connection_pending_notf_st * )&ntf_data->connpending;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_connection_pending_notf_st ),
                                           ( sctp_U8bit * )conn_pending );
#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          conn_pending->association_id );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_conn_pending_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_data_pending_notf
 *
 *    DESCRIPTION:
 *     This function build data pending notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_data_pending_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_data_pending_notf_st  *data_pending;
#ifdef SCTP_SEND_DATA_WITH_IND
    sctp_U8bit          *p_tmp;
    sctp_U32bit         ppi, buf_size;
    sctp_error_t        ecode;
    sctp_tcb_st         *p_tcb = SCTP_NULL;
    sctp_ep_st          *p_ep  = SCTP_NULL;
#endif
    data_pending =
        ( sctp_msg_data_pending_notf_st * )&ntf_data->datapending;

#ifdef SCTP_STRUCT_BASED

#ifdef SCTP_SEND_DATA_WITH_IND
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_data_pending_notf_st )
                                           - sizeof( sctp_U32bit )
                                           - sizeof( sctp_U32bit )
                                           - sizeof( sctp_U8bit * ),
                                           ( sctp_U8bit * )data_pending );
#else
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_data_pending_notf_st ),
                                           ( sctp_U8bit * )data_pending );

#endif /*SCTP_SEND_DATA_WITH_IND */

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          data_pending->association_id );
    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           data_pending->stream_id );

#endif /* SCTP_STRUCT_BASED */

#ifdef SCTP_SEND_DATA_WITH_IND
    p_tmp = p_ntf_buffer;

    /* Place holders for message size and PPI */
    p_ntf_buffer = p_ntf_buffer + sizeof( sctp_U32bit ) + sizeof( sctp_U32bit );

    /* Calculate max message size that can be copied */
    buf_size = SCTP_MAX_API_SIZE - SCTP_API_HEADER_LEN
               - sizeof( sctp_msg_data_pending_notf_st );

    p_ep = sctp_db_get_ep_ptr( data_pending->association_id );

    if( p_ep )
    {
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
    }

    /* Get the association from the endpoint */
    if( !p_ep || !p_tcb )
    {
        return( SCTP_NULL );
    }

    /* This can never fail */
    if( sctp_read_message_list( p_tcb, data_pending->stream_id, buf_size,
                                p_ntf_buffer, &buf_size, &ppi, SCTP_FALSE,
                                &ecode ) == SCTP_FAILURE )
    {
        return( SCTP_NULL );
    }

    /* build message size and ppi parameters */
    p_tmp = sctp_buffer_set_Ulong( p_tmp, ppi );

    p_tmp = sctp_buffer_set_Ulong( p_tmp, buf_size );

    p_ntf_buffer += buf_size;
#endif /* SCTP_SEND_DATA_WITH_IND */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_data_pending_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_snd_failure_notf
 *
 *    DESCRIPTION:
 *     This function build send failure notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_snd_failure_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{

    sctp_msg_send_failure_notf_st  *snd_failure_notf;

    snd_failure_notf =
        ( sctp_msg_send_failure_notf_st * )&ntf_data->sendfailure;

#ifdef SCTP_STRUCT_BASED

    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_send_failure_notf_st ), ( sctp_U8bit * )snd_failure_notf );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          snd_failure_notf->association_id );

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          snd_failure_notf->context );

    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           ( sctp_U16bit )snd_failure_notf->cause_code );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_snd_failure_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_ui_build_partial_datapending_notf
 *
 *    DESCRIPTION:
 *     This function build partial datapending notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *sctp_ui_build_partial_datapending_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{

    sctp_msg_partial_datapending_notf_st  *partial_datapending_notf;


    partial_datapending_notf =
        ( sctp_msg_partial_datapending_notf_st * )&ntf_data->partial_datapending;

#ifdef SCTP_STRUCT_BASED

    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_partial_datapending_notf_st ),
                                           ( sctp_U8bit * )partial_datapending_notf );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          partial_datapending_notf->assoc_id );

    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           partial_datapending_notf->stream );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_partial_datapending_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :  sctp_ui_build_recv_congestion_notf
 *
 *    DESCRIPTION:
 *     This function build receive congestion notification.
 *
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_recv_congestion_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_recv_congestion_notf_st  *cong;

    cong = ( sctp_msg_recv_congestion_notf_st * )&ntf_data->recv_congestion;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_recv_congestion_notf_st ), ( sctp_U8bit * )cong );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          cong->association_id );
    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           ( sctp_U16bit )cong->cong_level );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_recv_congestion_notf " ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :  sctp_ui_build_adadptation_ind
 *
 *    DESCRIPTION:
 *     This function build adaptation indication.
 *     buffer  format.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_adaption_ind(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_adaption_ind_notf_st  *p_adapi;

    p_adapi = ( sctp_msg_adaption_ind_notf_st * )&ntf_data->adaption_ind;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_recv_congestion_notf " ) );

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_adaption_ind_notf_st ), ( sctp_U8bit * )p_adapi );

#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          p_adapi->assoc_id );
    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          p_adapi->adaption_bits );

#endif /* SCTP_STRUCT_BASED */

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :   sctp_ui_build_peer_data_drop_ind
 *
 *    DESCRIPTION:
 *     This function build peer data drop indication.
 *     buffer  format.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_peer_data_drop_ind(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_peer_data_drop_notf_st  *peer_data_drop;
#ifndef SCTP_STRUCT_BASED
    sctp_U32bit                      count;
#endif

    peer_data_drop =
        ( sctp_msg_peer_data_drop_notf_st * )&ntf_data->peer_data_drop;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_peer_data_drop_notf_st ),
                                           ( sctp_U8bit * )peer_data_drop );
#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          peer_data_drop->association_id );
    p_ntf_buffer = sctp_buffer_set_Ushort( p_ntf_buffer,
                                           ( sctp_U16bit )peer_data_drop->ecode );
    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          peer_data_drop->buffer_len );

    for( count = 0; count < SCTP_MAX_MTUSIZE; count++ )
    {
        sctp_buffer_set_Uchar( p_ntf_buffer, peer_data_drop->buffer[count] );
    }

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_peer_data_drop_ind" ) );

    return( p_ntf_buffer );
}

/***************************************************************************
 *    FUNCTION :    sctp_ui_build_shutdown_event_notf
 *
 *    DESCRIPTION:
 *     This function builds shutdown event notification.
 *    RETURN VALUE:
 *    pointer  to buffer
 *
 **************************************************************************/
sctp_U8bit *
sctp_ui_build_shutdown_event_notf(
    sctp_U8bit     *p_ntf_buffer,
    sctp_notice_ut *ntf_data )
{
    sctp_msg_shutdown_event_notf_st  *shutdown_event;

    shutdown_event =
        ( sctp_msg_shutdown_event_notf_st * )&ntf_data->shutdown_event;

#ifdef SCTP_STRUCT_BASED
    p_ntf_buffer = sctp_buffer_set_Buffer( p_ntf_buffer,
                                           sizeof( sctp_msg_shutdown_event_notf_st ),
                                           ( sctp_U8bit * )shutdown_event );
#else

    p_ntf_buffer = sctp_buffer_set_Ulong( p_ntf_buffer,
                                          shutdown_event->association_id );

#endif /* SCTP_STRUCT_BASED */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "Entering sctp_ui_build_shutdown_event_notf " ) );

    return( p_ntf_buffer );
}


#endif /*SCTP_UI_SU_MESSAGEBASED*/

#endif /* SCTP_UI_SOCK_API */
