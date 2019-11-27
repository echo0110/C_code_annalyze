/*******************************************************************************
 *    FILE NAME:
 *        s_clessm.h
 *
 *    DESCRIPTION:
 *        This file list function exported by Enacapsulating shelll at Client
 *                side.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    23March 2001  Vakul Garg      -           .Original Creation
 *    27Now03       Vivek Bansal                 CSPL Changes
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#ifndef    __FILE_sctp_eshell_sm_SEEN__
#define    __FILE_sctp_eshell_sm_SEEN__

#define SCTP_SM_CLIENT_INVALID_DEST_ID     1

extern sctp_return_t
sctp_sm_dispatch_buffer(
    sctp_U8bit                *p_buffer,
    sctp_len_t                nooctets,
    sctp_src_id_t            src_id,
    sctp_dest_id_t            dest_id,
    sctp_Error_t            *p_ecode );


extern sigtran_return_t    sctp_sm_client_ipc_recv_data(
    sigtran_U32bit      handle_id,
    sig_sock_mode_et    socket_mode,
    sig_soc_type_et     socket_type,
    sig_sock_fd_t       socket_id,
    sig_q_module_t      *p_from_q_module,
    sigtran_pvoid_t     *p_data,
    sigtran_U32bit      *p_data_len,
    sigtran_error_t     *p_ecode );

#endif
