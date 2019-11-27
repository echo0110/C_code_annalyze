/*******************************************************************************
 *    FILE NAME:
 *        s_clessm.c
 *
 *    DESCRIPTION:
 *          This file is has encapsulating shell functions at the client side.
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    23March 2001  Vakul Garg      -           .Original Creation
 *    07 Oct 2003   Vivek Bansal    Rel 5.0     Changes for CSPL
 *    27 Nov 2003   Vivek Bansal    Rel 5.0     Changes for CSPL
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include <s_cotyps.h>
#include <s_cogbl.h>
#include <s_clco.h>
#include <s_clsmap.h>
#include <s_clessm.h>
#include <s_closmemsm.h>


extern sctp_return_t
sctp_sm_client_ipc_send_data(
    sctp_U8bit                *buffer,
    sctp_len_t                nooctets,
    sctp_src_id_t             src_id,
    sctp_dest_id_t            dest_id );


/***************************************************************************
 *    FUNCTION :
 *        sctp_sm_dispatch_buffer
 *    DESCRIPTION:
 *                Send buffer to stack using IPC send call.
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_sm_dispatch_buffer(
    sctp_U8bit              *p_buffer,
    sctp_len_t              nooctets,
    sctp_src_id_t           src_id,
    sctp_dest_id_t          dest_id,
    sctp_Error_t            *p_ecode )
{
    if( sctp_sm_client_ipc_send_data( p_buffer, nooctets, src_id, dest_id )
            == SCTP_FAILURE )
    {
        *p_ecode = E_SYSCALL_FAILURE;
        return ( SCTP_FAILURE );
    }

#ifndef SCTP_APPL_CSPL_PORTED
    sctp_client_sm_free_mesg_buffer( &p_buffer );
#endif

    return ( SCTP_SUCCESS );
}




