/*******************************************************************************
 *    FILE NAME:
 *        s_smipc.h
 *
 *    DESCRIPTION:
 *    This file lists ipc function exported by SM client.
 *
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    23March 2001  Vakul Garg      -           .Original Creation
 *    07 Oct 2003   Vivek Bansal    Rel 5.0     Changes for CSPL
 *    27 Nov 2003   Vivek Bansal    Rel 5.0     Changes for CSPL
 *    15 Sep 2014   Priyanka Mishra     Bug Id 22     CSR 85223
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef    __FILE_sctp_ipc_sm_SEEN__
#define    __FILE_sctp_ipc_sm_SEEN__

#include <s_cotyps.h>

/*Bug Id 22 fix starts*/
/*sctp_return_t*/
sctp_S32bit
/*Bug Id 22 fix ends*/
sctp_init_sm_client_ipc(
    sig_stack_type_et          src_stack_type,
    sctp_U16bit                local_udp_port,
    sctp_sockaddr_st           local_ip_address,
    sctp_U16bit                stack_udp_port,
    sctp_sockaddr_st           remote_ip_address,
    sctp_port_type_et          port_type,
    sctp_error_t               *p_ecode );

#endif
