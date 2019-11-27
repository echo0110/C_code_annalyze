/***************************************************************************
 *  FILE NAME:
 *      s_mmmain.h
 *
 *  DESCRIPTION:
 *      This module has all entry level functions to stack
 *
 *  DATE     NAME          REF#     REASON
 *  -------  ----          ----     ------
 *  23Mar00  Sigtran Team  ----     Original
 *  07Oct03  Vivek Bansal  Rel 5.0  Changes for CSPL
 *  27Nov03  Vivek Bansal  Rel 5.0  Changes for CSPL
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef     __FILE_sctp_main_SEEN__
#define     __FILE_sctp_main_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    void
    sctp_process_TL_datagram(
        sctp_U32bit                ip_header_offset,
        sctp_TL_packet_st          *packet,
        sctp_U32bit                size,
        sctp_Boolean_t             resolver_flag );

    void
    sctp_process_timer_expiry(
        sctp_timer_t               timer_id,
        sctp_timer_buffer_st       *buffer );

    void
    sctp_process_icmpV4_message(
        sctp_U8bit                 *buffer,
        sctp_U32bit                size,
        sctp_sockaddr_st           *p_from );

    void
    sctp_process_icmpV6_message(
        sctp_U8bit                 *buffer,
        sctp_U32bit                size,
        sctp_sockaddr_st           *p_from );

    void
    sctp_process_cookie_packets( void );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_main_SEEN__ */


