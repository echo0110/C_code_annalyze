/******************************************************************************
 *    FILE NAME:
 *        s_mhbld.h
 *
 *    This file is part of Message Building module. It includes
 *    functions that process a sctp chunk before invoking fsm triggers
 *
 *    DATE               NAME     REFERENCE     REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -       .Original Creation
 *    27Aug01       gsheoran                  Rel 3.0
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __FILE_sctp_mesg_build_SEEN__
#define __FILE_sctp_mesg_build_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    void
    sctp_insert_chksum(
        sctp_U8bit       *thisposition,
        sctp_U8bit       *origin,
        sctp_checksum_et checksum );

    /* SPR 20568 Starts for CSR 1-6658486 */
    sctp_U8bit  *
    sctp_buffer_set_cookie(
        sctp_U8bit  *p_buf,
        sctp_chunk_cookie_st    *cookie );

    sctp_U8bit  *
    sctp_buffer_get_cookie(
        sctp_U8bit  *p_buf,
        sctp_chunk_cookie_st    *cookie );
    /* SPR 20568 Ends for CSR 1-6658486 */

    sctp_U8bit *
    sctp_build_common_header(
        sctp_U8bit       *P,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_U32bit      verification );


    void
    sctp_send_chunk_init(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      init_tag,
        sctp_U32bit      init_tsn,
        sctp_U16bit      outstreams,
        sctp_U16bit      instreams,
        sctp_U32bit      rwnd,
        sctp_U32bit      num_addrs,
        sctp_sockaddr_st *addr_list,
        sctp_Boolean_t   cookie_preservative_flag,
        sctp_U32bit      cookie_preservative_time,
        sctp_suppaddr_st *p_supp_family,
        sctp_U8bit       *p_hostname,
        sctp_U32bit      adaption_ind );

    void
    sctp_send_chunk_initack(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_chunk_cookie_st    *cookie,
        sctp_U16bit      cookie_len,
        sctp_Boolean_t   unreco_param_present,
        sctp_U16bit      total_len,
        sctp_U8bit       *invalid_parameters,
        sctp_U8bit       *p_hostname,
        sctp_U32bit      adaption_ind );

    void
    sctp_send_chunk_cookie(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        LIST             *p_cookie_list,
        sctp_U16bit      cookiesize );

    void
    sctp_send_cookie_err_chunk_bundled(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        LIST             *p_cookie_list,
        sctp_U16bit      cookiesize,
        sctp_U16bit      cause_code,
        sctp_U16bit      cause_len,
        sctp_U8bit       *cause_info );

    void
    sctp_send_chunk_cookieack(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag );

    void
    sctp_send_chunk_abort(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_sockaddr_st *p_to,
        sctp_U32bit      tag,
        sctp_Boolean_t   include_ecode,
        sctp_U16bit      cause_code,
        sctp_U16bit      cause_len,
        sctp_U8bit       *cause_info,
        sctp_U8bit       chunk_flag,
        sctp_checksum_et checksum );

    void
    sctp_send_chunk_shutdowncomplete(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U8bit       chunk_flag );

    void
    sctp_send_chunk_shutdownack(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag );

    void
    sctp_send_chunk_shutdown(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U32bit      cumtsn );

    sctp_void_t sctp_get_confirmed_address(
        sctp_tcb_st *p_ass,
        sctp_sockaddr_st *p_to,
        sctp_sockaddr_st *p_from );

    void sctp_send_chunk_hb_path_verf( sctp_tcb_st *p_ass );

    sctp_return_t sctp_get_addr_index_for_hb(
        sctp_tcb_st *p_ass,
        sctp_U32bit *addr_index );

    void
    sctp_send_chunk_heartbeat(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_U32bit      dest_index,
        sctp_U32bit      tag );

    void
    sctp_send_chunk_heartbeatack(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U16bit      heartbeatsize,
        sctp_U8bit       *heartbeat );

    sctp_U8bit *
    sctp_add_chunk_sack(
        sctp_tcb_st      *p_ass,
        sctp_U8bit       *P,
        sctp_U32bit      cumtsn,
        sctp_U32bit      arwnd,
        LIST             *sacklist,
        sctp_U16bit      num_duplicate_tsn,
        sctp_U32bit      *duplicat_tsn );

    void
    sctp_send_chunk_sack(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U32bit      cumtsn,
        sctp_U32bit      arwnd,
        LIST             *sacklist,
        sctp_U16bit      num_duplicate_tsn,
        sctp_U32bit      *duplicat_tsn );


    void
    sctp_send_chunk_error(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U16bit      cause_code,
        sctp_U16bit      cause_len,
        sctp_U8bit       *cause_info );

    void
    sctp_send_chunk_ecne(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U32bit      lowest_tsn );

    void
    sctp_send_chunk_cwr(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U32bit      lowest_tsn );

    void
    sctp_send_unrecognise_chunk_error(
        sctp_tcb_st      *p_ass,
        sctp_sockaddr_st *p_from,
        sctp_U16bit      srcport,
        sctp_U16bit      dstport,
        sctp_addrinfo_st *p_to,
        sctp_U32bit      tag,
        sctp_U16bit      cause_code,
        sctp_U16bit      cause_len,
        sctp_U8bit       *cause_info );


#ifdef SCTP_DYNAMIC_IP_SUPPORT
    void
    sctp_send_chunk_asconf(
        sctp_tcb_st         *p_ass,
        sctp_sockaddr_st    *p_from,
        sctp_U16bit         srcport,
        sctp_U16bit         dstport,
        sctp_addrinfo_st    *p_to,
        sctp_U32bit         tag,
        sctp_addr_conf_st   *p_addr_conf );

    void
    sctp_send_chunk_asconf_ack(
        sctp_tcb_st         *p_ass,     /* SPR 20859*/
        sctp_sockaddr_st    *p_from,
        sctp_U16bit         srcport,
        sctp_U16bit         dstport,
        sctp_addrinfo_st    *p_to,
        sctp_U32bit         tag,
        sctp_addr_conf_ack_st   *p_addr_conf,
        sctp_U8bit          *p_buffer,
        sctp_U32bit         *p_buffer_len );


#endif
#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_mesg_build_SEEN__ */

