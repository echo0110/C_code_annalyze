/******************************************************************************
 *    FILE NAME:
 *    s_mhproc.h
 *
 *    DESCRIPTION:
 *    This file is part of message Handling module. It includes
 *    functions that process a sctp chunk before invoking fsm triggers
 *
 *    DATE               NAME     REFERENCE     REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -       .Original Creation
 *    27Aug01       gsheoran                  Rel 3.0
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef  __FILE_sctp_mesg_proc_SEEN__
#define  __FILE_sctp_mesg_proc_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    sctp_return_t
    sctp_process_chunk_data(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_init(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_initack(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_sack(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_heartbeat(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_heartbeatack(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_abort(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_shutdowncomplete(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_shutdown(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_shutdownack(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_error(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_cookie(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_cookieack(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_ecne(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_cwr(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    void
    sctp_process_chunk_for_error(
        sctp_packet_st     *p_sctp,
        sctp_U16bit        unrecognise_chunk_len,
        sctp_U8bit         *unrecognise_chunk );

    sctp_return_t
    sctp_process_resolved_cookie(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_ass,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_chk_addr_unicast(
        sctp_sockaddr_st   *p_addr );

    sctp_return_t
    sctp_chk_addrs_for_unicast_values(
        sctp_sockaddr_st   *p_src_addr,
        sctp_sockaddr_st   *p_dst_addr );

    sctp_return_t
    sctp_process_chunk_init_after_checks(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_chunk_init_st *p_init_chunk,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_Boolean_t     hostname_present,
        sctp_U8bit         hostname[],
        sctp_U32bit        cookie_preservative,
        sctp_Boolean_t     unreco_param_present,
        sctp_U16bit        unrecog_param_len,
        sctp_U8bit         invalid_parameters[],
        sctp_U16bit         non_init_params_len,
        sctp_U8bit          *p_non_init_params,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_asconf(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_assoc,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_process_chunk_asconfack(
        sctp_packet_st     *p_sctp,
        sctp_U8bit         *buffer,
        sctp_U32bit        chunk_offset,
        sctp_Boolean_t     last_chunk_flag,
        sctp_tcb_st        *p_assoc,
        sctp_error_t       *p_ecode );


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_mesg_proc_SEEN__ */

