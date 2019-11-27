/***************************************************************************
 *  FILE NAME  : s_dbutil.h
 *
 *  DESCRIPTION: Contains the utility functions for Database access.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef _S_DBUTIL_H_SEEN_
#define _S_DBUTIL_H_SEEN_

#ifdef __cplusplus
extern "C" {
#endif

#include "s_os.h"

    void
    sctp_db_update_src_addr_index(
        sctp_tcb_st         *p_ass,
        sctp_U32bit         addr_index );

    void
    sctp_db_reset_error_counters(
        sctp_tcb_st     *p_ass,
        sctp_U32bit     addr_index );


    sctp_return_t
    sctp_cmp_addr(
        sctp_sockaddr_st  *p_a1,
        sctp_sockaddr_st  *p_a2 );

    sctp_return_t
    sctp_db_get_addrs_not_present(
        sctp_tcb_st       *p_ass,
        sctp_U32bit       *p_new_naddr,
        sctp_sockaddr_st  *p_new_list,
        sctp_U32bit       naddr1,
        sctp_sockaddr_st  *p_addr_list1,
        sctp_U32bit       naddr2,
        sctp_sockaddr_st  *p_addr_list2,
        sctp_error_t      *p_err );

    /*
     * Functions that handle the notification processing.
     * These functions are called to send notifications to the ULP.
     */

    sctp_return_t
    sctp_ntfy_operror(
        sctp_tcb_st    *p_tcb,
        sctp_U16bit    e_code,
        sctp_U16bit    e_length,
        sctp_Pvoid_t   sys_err_info );

    sctp_return_t
    sctp_ntfy_dst_status_change(
        sctp_tcb_st            *p_tcb,
        sctp_sockaddr_st       *p_dst_addr,
        sctp_sock_spc_event_et status,
        sctp_U32bit            e_code );


    sctp_return_t
    sctp_ntfy_endpt_status_change(
        sctp_tcb_st               *p_tcb,
        sctp_sock_sac_event_et    status,
        sctp_endpoint_chg_data_ut *p_status_data,
        sctp_error_t              e_code );


    sctp_return_t
    sctp_ntfy_congestion(
        sctp_tcb_st        *p_tcb,
        sctp_cong_level_et cong_level );


    sctp_return_t
    sctp_ntfy_sys_error(
        sctp_tcb_st   *p_tcb,
        sctp_error_t  err_no );


    sctp_return_t
    sctp_ntfy_conn_pending(
        sctp_tcb_st     *p_tcb );


    sctp_return_t
    sctp_ntfy_data_pending(
        sctp_tcb_st     *p_tcb,
        sctp_U16bit     stream,
        sctp_U32bit     ssn,
        sctp_U32bit     no_notify );


    sctp_return_t
    sctp_ntfy_send_failure(
        sctp_tcb_st        *p_tcb,
        sctp_U32bit        context,
        sctp_sock_ssf_flag_et flags,
        sctp_error_t       cause_code,/* Cause for failure */
        sctp_U16bit        info_len,  /* Length of the info string */
        sctp_U8bit         *p_info )   /* Optiional data to be sent back */;


    sctp_return_t
    sctp_ntfy_partial_data(
        sctp_tcb_st     *p_tcb,
        sctp_U16bit     stream );

    sctp_return_t
    sctp_ntfy_adaption_ind(
        sctp_tcb_st     *p_tcb,
        sctp_U32bit     adaption_bits );

    sctp_return_t
    sctp_ntfy_recv_congestion(
        sctp_tcb_st        *p_tcb,
        sctp_cong_level_et cong_level );


    sctp_return_t
    sctp_ntfy_shutdown_event(
        sctp_tcb_st    *p_tcb );

    sctp_return_t
    sctp_ntfy_peer_data_drop(
        sctp_tcb_st    *p_tcb,
        sctp_error_t   *p_ecode,
        sctp_U8bit     *p_buffer,
        sctp_U32bit    buffer_len );


    sctp_Boolean_t
    sctp_db_is_af_correct(
        sctp_ep_st            *p_ep,
        sctp_U32bit           n_addr,
        sctp_sockaddr_list_st addr_list );

    sctp_Boolean_t
    sctp_db_is_v4_v6_addr_compatible(
        sctp_ep_st              *p_ep,
        sctp_U32bit              n_addr,
        sctp_sockaddr_list_st    addr_list,
        sctp_suppaddr_st        *p_peer_support );

    sctp_Boolean_t
    sctp_db_is_af_same(
        sctp_ep_st            *p_ep,
        sctp_U32bit           n_addr,
        sctp_sockaddr_list_st addr_list );

    sctp_return_t
    sctp_db_get_primary_addr_index(
        sctp_tcb_st           *p_tcb,
        sctp_U32bit           *p_src_pri,
        sctp_U32bit           *p_dst_pri,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_db_get_saddr_index_with_af(
        sctp_tcb_st           *p_tcb,
        sctp_U32bit           *p_src_pri,
        sctp_sa_family_t      af,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_db_get_v4_v6_addr_index_with_af(
        sctp_tcb_st           *p_tcb,
        sctp_U32bit           *p_src_pri,
        sctp_sa_family_t      af,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_db_get_supported_address_types(
        sctp_ep_st              *p_ep,
        sctp_suppaddr_st        *p_supp_addr );

    sctp_return_t
    sctp_db_get_laddr_index(
        sctp_ep_st            *p_ep,
        sctp_sockaddr_st      *p_sockaddr,
        sctp_U32bit           *p_index,
        sctp_error_t          *p_ecode );

    sctp_return_t
    sctp_db_get_raddr_index(
        sctp_tcb_st           *p_tcb,
        sctp_sockaddr_st      *p_sockaddr,
        sctp_U32bit           *p_index,
        sctp_error_t          *p_ecode );

    void
    sctp_db_update_readable_status(
        sctp_ep_st            *p_ep );

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    sctp_return_t
    sctp_ntfy_local_addr_change(
        sctp_tcb_st             *p_tcb,
        sctp_sockaddr_st        *p_addr,
        sctp_addrconf_status_et status,
        sctp_asconf_response_et ecode );

#endif


#ifdef __cplusplus
}
#endif

#endif /* _S_DBUTIL_H_SEEN_ */


