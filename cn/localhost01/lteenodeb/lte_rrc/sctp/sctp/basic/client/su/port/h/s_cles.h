/*******************************************************************************
 *    FILE NAME:
 *        s_cles.h
 *
 *    DESCRIPTION:
 *        This file list function exported by Enacapsulating shelll at Client
 *                side.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -           .Original Creation
 *    27Now03         Vivek Bansal                 CSPL Changes
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************************/
#ifndef    __FILE_sctp_eshell_SEEN__
#define    __FILE_sctp_eshell_SEEN__

#include <s_clpapi.h>

#define SCTP_CLIENT_INVALID_DEST_ID     SCTP_SU_CLI_MAX_DST

extern sctp_U8bit *su_buffer; /*Malloced to store the SU responses*/

extern sctp_return_t
sctp_dispatch_buffer(
    sctp_U8bit              *p_buffer,
    sctp_len_t              nooctets,
    sctp_src_id_t           src_id,
    sctp_dest_id_t          dest_id,
    sctp_Error_t            *p_ecode );


sigtran_return_t    sctp_client_ipc_recv_data(
    sigtran_U32bit      handle_id,
    sig_sock_mode_et    socket_mode,
    sig_soc_type_et     socket_type,
    sig_sock_fd_t       socket_id,
    sig_q_module_t      *p_from_q_module,
    sigtran_pvoid_t     *p_data,
    sigtran_U32bit      *p_data_len,
    sigtran_error_t     *p_ecode );

sctp_S32bit
sctp_init_client_ipc(
    sig_stack_type_et           src_stack_type,
    sctp_U16bit                 local_udp_port,
    sctp_sockaddr_st            local_ip_address,
    sctp_U16bit                 stack_udp_port,
    sctp_sockaddr_st            remote_ip_address,
    sctp_port_type_et           port_type,
    sctp_error_t                *p_ecode );

#endif
