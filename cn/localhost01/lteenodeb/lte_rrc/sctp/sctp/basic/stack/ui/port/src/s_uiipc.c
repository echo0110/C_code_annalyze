/*******************************************************************************
 *    FILE NAME:
 *    s_uiipc.c
 *
 *    DESCRIPTION:
 *    sctp ui ipc related functions
 *
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *    02jan02     hbhondwe   SCTP_IPC_TCP   ipc changes for tcp
 *    29Oct02     mrajpal    SPR 5024        IPC TCP fix
 *    07 Oct 2003 Vivek Bansal    Rel 5.0     Changes for CSPL
 *    27 Nov 2003 Vivek Bansal    Rel 5.0     Changes for CSPL
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#define SCTP_MODULE "UI"

#include <s_cotyps.h>
#include <s_cogbl.h>
#include <s_clpapi.h>
#include <s_uiipc.h>
#include <s_es.h>
#include <s_osmem.h>
#include <s_red.h>
#include <s_clsmap.h>
#include <s_dbg.h>

#ifdef SCTP_UI_MESSAGEBASED


/***************************************************************************
 *    FUNCTION :
 *      send_from_sctp
 *    DESCRIPTION:
 *      Send buffer to client using IPC send call. The function is dependent
 *      on the socket type. If the socket type is TCP based, then this
 *      function will set the message length in the first four bytes of the
 *      message.For this it will additionally allocate the memory for the 4
 *      bytes, insert the message received in the same buffer using CSPL and
 *      send it. The receiving end will have to extract them as segemnt in
 *      case of iovector.
 *
 *    RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ***************************************************************************/
sctp_return_t send_from_sctp(
    sctp_U8bit           *p_buffer,
    sctp_len_t           nooctets,
    sctp_src_id_t        src_id,
    sctp_dest_id_t       dest_id,
    sctp_client_et       client_type,
    sctp_client_info_st  *to_client,
    sctp_Error_t         *p_ecode )
{
#ifdef SCTP_RED_SUPPORT
    sctp_api_id_t               api_id;
#endif
    sctp_U16bit                 handle_id;
    sctp_U8bit                  *p_buffer_to_send = p_buffer;
    sigtran_error_t             ecode;

    sig_get_from_map( SIG_SCTP_STACK, src_id, &handle_id, &ecode );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


#ifdef SCTP_RED_SUPPORT
    /*
     * Move the pointer ahead by 4 bytes for the event type.
     */
    p_buffer = p_buffer + 4;
    /*
     * Move the pointer ahead by 4 bytes for the size.
     */
    p_buffer = p_buffer + 4;

    /*
     * Extract the API header.
     */
    api_id = sctp_buffer_get_UshortAtOffset( p_buffer, 0 );

    /*If stack is standby, do not send the packet*/
    if( sctp_stack_red_state == SCTP_STACK_RED_STATE_STANDBY )
    {
        if( api_id == SCTP_API_INIT_STACK_RESP ||
                ( api_id >= SCTP_API_SET_STACK_RED_STATE_RESP &&
                  api_id <= SCTP_API_GET_STACK_HEALTH_RESP ) )
        {
            /*Then pass the message to the client*/
        }
        else
        {
            /*Otherwise do not pass the message to the client*/
            return ( SCTP_SUCCESS );
        }
    }

#endif /*SCTP_RED_SUPPORT*/

    if( sctp_stack_ipc_send_data( p_buffer_to_send,
                                  nooctets, dest_id, handle_id, client_type, to_client ) == SCTP_FAILURE )
    {
        return( SCTP_FAILURE );

    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_stack_ipc_send_data
 *
 *    DESCRIPTION:
 *        Calls the appropriate UDP/TL function to send data over a socket
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_stack_ipc_send_data(
    sctp_U8bit           *p_buffer,
    sctp_U32bit          nooctets,
    sctp_U32bit          src_id,
    sctp_U32bit          dest_id,
    sctp_client_et       client_type,
    sctp_client_info_st  *to_client )
{
    sigtran_error_t             ecode;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( src_id );
    SCTP_UNUSED_VARIABLE( to_client );
    /*to avoid the warning*/


    if( SCTP_CLIENT_SM == client_type )
    {
        if( SIGTRAN_FAILURE == sig_send( SIG_SCTP_STACK, SIG_DEST_TYPE_SM,
                                         ( sigtran_U16bit )dest_id, p_buffer, nooctets, &ecode ) )
        {
            return SCTP_FAILURE;
        }
    }

    else
    {
        if( SIGTRAN_FAILURE == sig_send( SIG_SCTP_STACK, SIG_DEST_TYPE_APP,
                                         ( sigtran_U16bit )dest_id, p_buffer, nooctets, &ecode ) )
        {
            return SCTP_FAILURE;
        }
    }

    return SCTP_SUCCESS;
}
#endif /* SCTP_UI_MESSAGEBASED */
