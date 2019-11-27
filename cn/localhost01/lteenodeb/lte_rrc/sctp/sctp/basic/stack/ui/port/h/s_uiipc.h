/*******************************************************************************
 *    FILE NAME:   s_uiipc.h
 *
 *    DESCRIPTION:
 *    This file list function exported by Enacapsulating shell at Client
 *    side.
 *    This file contains the portable parts of the UI
 *    dir such as send_from_sctp
 *        side.
 *
 *
 *    DATE      NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *    07 Oct 2003     Vivek Bansal    Rel 5.0   Changes for CSPL
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __FILE_sctp_uiipc_SEEN__
#define __FILE_sctp_uiipc_SEEN__

#ifdef __cplusplus
extern "C" {
#endif
    extern sctp_return_t
    send_from_sctp(
        sctp_U8bit          *p_buffer,
        sctp_len_t          nooctets,
        sctp_src_id_t       src_id,
        sctp_dest_id_t      dest_id,
        sctp_client_et      client_type,
        sctp_client_info_st *to_client,
        sctp_Error_t        *p_ecode );

    extern sctp_return_t
    sctp_stack_ipc_send_data(
        sctp_U8bit           *buffer,
        sctp_U32bit          nooctets,
        sctp_U32bit          src_id,
        sctp_U32bit          dest_id,
        sctp_client_et       client_type,
        sctp_client_info_st  *to_client );


    sctp_return_t
    sctp_stack_mesg_queue_send_data(
        sctp_U8bit           *p_buffer,
        sctp_U32bit          nooctets,
        sctp_U32bit          src_id,
        sctp_U32bit          dest_id,
        sctp_client_et       client_type,
        sctp_client_info_st  *to_client );

#endif


#ifdef __cplusplus
}
#endif
