/******************************************************************************
 *    FILE NAME:
 *        s_cles.c
 *
 *    DESCRIPTION:
 *        This file is has encapsulating shell functions at the client side.
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -           .Original Creation
 *    07 Oct 2003     Vivek Bansal    Rel 5.0       Changes for CSPL
 *    27 Nov 2003     Vivek Bansal    Rel 5.0       Changes for CSPL
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include <s_cotyps.h>
#include <s_cogbl.h>
#include <s_clpapi.h>
#include <s_cles.h>
#include <s_closmem.h>

extern sctp_sockaddr_st   sctp_cli_local_ip_address;
extern sctp_U16bit        sctp_cli_local_udp_port;

extern sctp_return_t
sctp_client_ipc_send_data(
    sctp_U8bit                *buffer,
    sctp_len_t                nooctets,
    sctp_src_id_t             src_id,
    sctp_dest_id_t            dest_id );


/***************************************************************************
 *    FUNCTION :
 *        sctp_dispatch_buffer
 *    DESCRIPTION:
 *        Send buffer to stack using IPC send call.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_dispatch_buffer(
    sctp_U8bit              *p_buffer,
    sctp_len_t              nooctets,
    sctp_src_id_t           src_id,
    sctp_dest_id_t          dest_id,
    sctp_Error_t            *p_ecode )
{
    sctp_return_t   ret_val;

    ret_val = sctp_client_ipc_send_data( p_buffer, nooctets, src_id, dest_id );

    if( SCTP_FAILURE == ret_val )
    {
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }

#ifndef SCTP_APPL_CSPL_PORTED
    sctp_client_free_mesg_buffer( &p_buffer );
#endif
    return ( ret_val );
}


