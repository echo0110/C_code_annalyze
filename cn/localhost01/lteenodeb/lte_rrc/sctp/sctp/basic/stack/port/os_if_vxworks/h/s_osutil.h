/*****************************************************************************
 *    FILE NAME  :  s_osutil.h
 *
 *    DESCRIPTION:  List of OS specific functions.
 *
 *    DATE      NAME       REFERENCE       REASON
 *    ----          ----           ---------       ------
 *    09Aug01       Gautam Sheoran     -           Create Original
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef _S_OS_UTIL_H_
#define _S_OS_UTIL_H_

#include <s_cotyps.h>
#include <s_os.h>

#ifdef __cplusplus
extern "C" {
#endif


    sctp_return_t
    sctp_conv_addr_storg_os_to_sctp(
        sctp_U32bit                n_os_addr,
        sctp_sockaddr_storage_st   *p_os_addrs,
        sctp_sockaddr_st           *p_sctp_addrs,
        sctp_port_t                *p_port,
        sctp_error_t               *p_ecode );

    sctp_return_t
    sctp_conv_addr_sctp_to_sock_os(
        sctp_U32bit                n_sctp_addr,
        sctp_sockaddr_st           *p_sctp_addrs,
        struct sockaddr            *p_os_addrs,
        sctp_port_t                port,
        sctp_error_t               *p_ecode );

    sctp_return_t
    sctp_conv_addr_os_to_sctp(
        sctp_U32bit                n_os_addr,
        struct sockaddr            *p_os_addrs,
        sctp_sockaddr_st           *sctp_addr_list,
        sctp_port_t                *p_port,
        sctp_error_t               *p_ecode );

    sctp_return_t
    sctp_conv_addr_sctp_to_os(
        sctp_U32bit                n_sctp_addr,
        sctp_sockaddr_st           *p_sctp_addrs,
        sctp_sockaddr_storage_st   *p_os_addrs,
        sctp_port_t                port,
        sctp_error_t               *p_ecode );

#ifdef SCTP_DEBUG_ENABLED

    char *
    sctp_get_ctime(
        time_t        clock_sec,
        char         *time_buffer );

#endif

#ifdef SCTP_ERROR_ENABLED

    sctp_return_t
    sctp_os_set_signals(
        sctp_Boolean_t             flag,
        sctp_error_t               *p_ecode );

#endif
#ifdef __cplusplus
}
#endif

#endif /* _S_OS_UTIL_H_ */

