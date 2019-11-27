/****************************************************************************
 *    FILE NAME  : s_uiutil.h
 *
 *    DESCRIPTION: This file contains some utility functions for the SU module
 *
 *    DATE     NAME         REFERENCE              REASON
 *    ----     ----         ---------              ------
 *    09Spe01  gsheoran         -                  Create Original
 *    26Jul02  mrajpal      Rel 4.0                 New Send API Support
 *    08Oct02  mrajpal       SPR 4926            getsockopt params
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef _S_UIUTIL_H_SEEN_
#define _S_UIUTIL_H_SEEN_

#ifdef __cplusplus
extern "C" {
#endif



    typedef struct __sctp_notf_queue_node
    {
        NODE                  __node;
        sctp_callback_func_t  p_callback_func;
        sctp_Pvoid_t          p_appl_data;
        sctp_U32bit           event;
        sctp_notice_ut        notice;

    } sctp_notf_queue_node_st;

    typedef struct __sctp_notf_queue
    {
        sctp_U32bit             first;
        sctp_U32bit             last;
        sctp_Boolean_t          is_empty;
        sctp_U32bit             count;
        sctp_notf_queue_node_st nodes_list[SCTP_MAX_NOTF_Q_SIZE];
    } sctp_notf_queue_st;

    int
    sctp_accept_blocking(
        sctp_ep_st       *p_ep );

    int
    sctp_accept_nonblocking(
        sctp_ep_st       *p_ep );

    int
    sctp_send_general(
        sctp_tcb_st        *p_tcb,
        sctp_sockaddr_st   *p_daddr,
        sctp_U32bit        iovec_len,
        sctp_iovec_st      *p_iovec,
        sctp_stream_t      stream_id,
        sctp_U32bit        life_time,
        sctp_U32bit        ppi,
        sctp_Boolean_t     bundle,
        sctp_Boolean_t     ordered,
        sctp_U32bit        context,
        sctpbuffer_st      **p_p_tx_buf,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_calc_trx_cong_reg(
        sctp_tcb_st         *p_assoc,
        sctp_U32bit         *p_new_cong_reg );

    sctp_return_t sctp_dynamic_ip_supported(
        sctp_tcb_st     *p_ass,
        sctp_asconf_param_type_et   type );

    sctp_return_t sctp_chk_and_send_asconf(
        sctp_tcb_st         *p_ass,
        sctp_bindx_flag_et  flags,
        sctp_Boolean_t      block,
        sctp_error_t        *p_ecode );

#ifdef SCTP_UI_SOCK_API

    int
    sctp_connect_udp(
        sctp_ep_st        *p_ep,
        sctp_sockaddr_st  *p_daddr,
        sctp_port_t       dport,
        int               num_addrs );

    int
    sctp_connect_tcp(
        sctp_ep_st        *p_ep,
        sctp_sockaddr_st  *p_daddr,
        sctp_port_t       dport,
        int               num_addrs );

    int
    sctp_connect_blocking(
        sctp_tcb_st      *p_tcb );

    int
    sctp_receive_blocking(
        sctp_tcb_st        *p_tcb,
        sctp_U8bit         *p_buffer,
        sctp_U32bit        length,
        sctp_U32bit        *p_msg_len,
        sctp_U32bit        flags,
        sctp_Boolean_t     block );


    int
    sctp_setsockopt_sctp(
        sctp_ep_st         *p_ep,
        int                optname,
        const void         *optval,
        sctp_socklen_t     optlen );

    int sctp_set_rcv_snd_sockopt_udp_sctp(
        sctp_ep_st            *p_ep,
        int                    rcv_send_flag,
        const void            *optval );

    int
    sctp_setsockopt_socket(
        sctp_ep_st         *p_ep,
        int                optname,
        const void         *optval,
        sctp_socklen_t      optlen );

    /*SPR 4926*/
    int
    sctp_getsockopt_sctp(
        sctp_ep_st         *p_ep,
        int                optname,
        void               *optval,
        sctp_socklen_t     *optlen );
    /*SPR 4926*/

    int
    sctp_getsockopt_socket(
        sctp_ep_st         *p_ep,
        int                optname,
        void               *optval,
        sctp_socklen_t     *optlen );

    size_t
    sctp_sendto_tcp(
        sctp_ep_st               *p_ep,
        sctp_U8bit               *message,
        sctp_U32bit              length,
        int                      flags,
        const struct sockaddr    *destaddr,
        size_t                   dest_len );

    size_t
    sctp_sendto_udp(
        sctp_ep_st               *p_ep,
        sctp_U8bit               *message,
        sctp_U32bit              length,
        int                      flags,
        const struct sockaddr    *destaddr,
        size_t                   dest_len );

    size_t
    sctp_recvfrom_tcp(
        sctp_ep_st               *p_ep,
        sctp_U8bit               *message,
        sctp_U32bit              length,
        int                      flags,
        struct sockaddr          *srcaddr,
        sctp_socklen_t           *src_len );

    size_t
    sctp_recvfrom_udp(
        sctp_ep_st               *p_ep,
        sctp_U8bit               *message,
        sctp_U32bit              length,
        int                      flags,
        struct sockaddr          *srcaddr,
        sctp_socklen_t           *src_len );

    size_t
    sctp_sendmsg_tcp(
        sctp_ep_st         *p_ep,
        sctp_msghdr_st     *p_message,
        int                flags );

    size_t
    sctp_sendmsg_udp(
        sctp_ep_st         *p_ep,
        sctp_msghdr_st     *p_message,
        int                flags );

    size_t
    sctp_sendx_tcp(
        sctp_ep_st                      *p_ep,
        void                            *msg,
        size_t                          len,
        struct sockaddr                 *addrs,
        int                             addrcnt,
        const struct sctp_sndrcvinfo    *p_sinfo,
        int                             flags );

    size_t
    sctp_sendx_udp(
        sctp_ep_st                      *p_ep,
        void                            *p_msg,
        size_t                          len,
        struct sockaddr                 *addrs,
        int                             addrcnt,
        const struct sctp_sndrcvinfo    *p_sinfo,
        int                             flags );

    size_t
    sctp_send_sendall(
        sctp_ep_st                      *p_ep,
        sctp_msghdr_st                  *p_message,
        const struct sctp_sndrcvinfo    *p_sinfo );

    size_t
    sctp_recvmsg_tcp(
        sctp_ep_st         *p_ep,
        sctp_msghdr_st     *p_message,
        int                flags );

    size_t
    sctp_recvmsg_udp(
        sctp_ep_st         *p_ep,
        sctp_msghdr_st     *p_message,
        int                flags );

    size_t
    sctp_chk_notification(
        sctp_ep_st         *p_ep,
        sctp_msghdr_st     *p_message );


#else  /* SCTP_UI_SOCK_API  */

#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_NO_QUEUE


    sctp_return_t
    sctp_add_notification_to_queue(
        sctp_ep_st    *p_ep,
        sctp_U32bit    event );

    sctp_return_t
    sctp_su_init_notification_queue( void );

    sctp_return_t
    sctp_su_deinit_notification_queue( void );

#endif
#endif




#ifdef __cplusplus
}
#endif


#endif /* SCTP_UI_SOCK_API  */
#endif /* _S_UIUTIL_H_SEEN_ */

