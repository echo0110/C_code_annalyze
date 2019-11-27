/***************************************************************************
 *  FILE NAME  : s_esdef.c
 *
 *  DESCRIPTION: Contains the Encapsulation shell defines.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  07Oct03    Vivek Bansal Rel 5.0      Changes for CSPL
 *  27Now03    Vivek Bansal              CSPL Changes
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_eshell_def_SEEN__
#define __FILE_sctp_eshell_def_SEEN__

#include <s_cotyps.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct
    {
        struct
        {
            /* Socket for ICMPV4 Messagaes */
            sctp_U32bit       icmpV4;

            /* Socket for ICMPV6 Messages */
            sctp_U32bit       icmpV6;

            /* Socket for IPv4 Messages */
            sctp_U32bit       ipV4;

            /* Socket for IPv6 Messages */
            sctp_U32bit       ipV6[SCTP_MAX_TRANSPORT_ADDR];
        } sock;

        sctp_U32bit       udp_socket_id[SCTP_MAX_TRANSPORT_ADDR];
        sctp_U32bit       num_v4_addrs;
        sctp_sockaddr_st  ipv4_addrs[SCTP_MAX_TRANSPORT_ADDR];

#ifdef SCTP_INET6_SUPPORT
        sctp_U32bit       num_v6_addrs;
        sctp_sockaddr_st  ipv6_address[SCTP_MAX_TRANSPORT_ADDR];
#endif

    } sctp_receive_socket_st;

    typedef       struct
    {
        /* The UDP server socket id */
        sctp_U32bit       udp;

        /* The IPV4 socket_id */
        sctp_U32bit       ipV4;

        /* The IPV6 socket id */
        sctp_U32bit       ipV6;
    } sctp_send_socket_st;

    typedef       struct
    {
        /* The Stack type. */
        sctp_stack_type_et      stack_type;

        /* The UDP port for UDP tunnelling. */
        sctp_U16bit             udp_port;

        /* The Send socket id */
        sctp_send_socket_st     send;

        /* The Sockets from which data is received. */
        sctp_receive_socket_st  receive;

    } sctp_eshell_info_t;

    /* This    structure has all sockets related information */
    extern sctp_eshell_info_t sctp_eshell_info;

    /* The IP specific send function ptr table. */
    typedef void ( *sctp_sendmessage_fpt )(
        sctp_U8bit       *buffer,
        sctp_U32bit      size,
        sctp_Boolean_t   df_flag,
        sctp_sockaddr_st *to,
        sctp_sockaddr_st *from );

    void *sctp_receive_timer_thread( void *p_buffer );
    sctp_return_t sctp_start_receive_timer_thread( sctp_error_t *p_ecode );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_eshell_def_SEEN__ */

