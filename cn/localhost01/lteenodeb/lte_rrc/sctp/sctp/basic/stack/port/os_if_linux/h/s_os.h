/*****************************************************************************
 *    FILE NAME  :  s_os.h
 *
 *    DESCRIPTION:  List of OS specific defines. Especially related to socket
 *                  API support but may be relevant at other places.
 *                  Specifies solaris specific structures.
 *
 *    DATE      NAME       REFERENCE       REASON
 *    ----          ----           ---------       ------
 *    09Aug01       Gautam Sheoran     -           Create Original
 *    02jan02     hbhondwe     SCTP_TCP_IPC  included files for tcp
 *    06Mar14     Rohan Pawa                 CSR 69326
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef _S_OS_H_
#define _S_OS_H_


#include    <sig_os.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Minimum Path MTU for IPv6 */
#define     SCTP_IPV6_MIN_LINK_MTU     1280


#ifndef s6_addr32
#define s6_addr32       _S6_un._S6_u32
#endif

    /* Implementation specific max size */
#define _SCTP_SS_MAXSIZE    128

    /* Implementation specific desired alignment */
#define _SCTP_SS_ALIGNSIZE  (sizeof(uint64_t))


#define SIZEOF_SA_FAMILY   sizeof(sa_family_t)
    /* Instead of the above #define we should have sizeof(sa_family_t) but
    the flag -ansi doesn't permit that */

    /*
     * Definitions used for sockaddr_storage structure paddings design.
     */
#define _SCTP_SS_PAD1SIZE   (_SCTP_SS_ALIGNSIZE - SIZEOF_SA_FAMILY)
#define _SCTP_SS_PAD2SIZE   (_SCTP_SS_MAXSIZE - SIZEOF_SA_FAMILY+ _SCTP_SS_PAD1SIZE \
                             + _SCTP_SS_ALIGNSIZE )
    struct __sctp_sockaddr_storage
    {
        sa_family_t  __ss_family;     /* address family */

        /* Following fields are implementation specific */
        char      __ss_pad1[_SCTP_SS_PAD1SIZE];

        /* 6 byte pad, this is to make implementation
           specific pad up to alignment field that
           follows explicit in the data structure */
        uint64_t  __ss_align;     /* field to force desired structure */

        /* storage alignment */
        char      __ss_pad2[_SCTP_SS_PAD2SIZE];
        /* 112 byte pad to achieve desired size, */
        /* _SCTP_SS_MAXSIZE value minus size of ss_family */
        /* __ss_pad1, __ss_align fields is 112 */
    };

    /* For use in the sctp stack. */
    typedef struct __sctp_sockaddr_storage    sctp_sockaddr_storage_st;

#define SOCKADDR_STORAGE_FAMILY(a)  (a.__ss_family)

#ifndef SCTP_INET6_SUPPORT
#define SCTP_AF_INET6         26
#else
#define SCTP_AF_INET6         AF_INET6
#endif

#define SCTP_AF_INET              AF_INET

#define SCTP_VALID_SK_FAMILY(fa) \
    ((SCTP_AF_INET6 == fa) || (SCTP_AF_INET == fa))

    /* The TCP and UDP style options. */
#define SCTP_SOCK_SEQPACKET       SOCK_SEQPACKET
#define SCTP_SOCK_STREAM          SOCK_STREAM
#define SCTP_VALID_SK_TYPE(type) \
    ((SCTP_SOCK_SEQPACKET == type) || (SCTP_SOCK_STREAM == type))

#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP              132
#endif

    /* These are used for the function pointer table opeartion */
#define SCTP_V4                   0
#define SCTP_V6                   1
#define SCTP_V4_IOV               2
#define SCTP_V6_IOV               3

#define SCTP_MAX_IP_ADDR_TYPE     2

    /*
     * Some types derived from the system types
     * which are used in sctp.
     */
    typedef sa_family_t         sctp_sa_family_t;

    typedef sigtran_U32bit      __sctp_U32bit;

    typedef sigtran_S32bit      __sctp_S32bit;

    typedef __sctp_S32bit       sctp_S16bit;

    typedef  sigtran_U8bit      __sctp_U8bit;

    typedef  sigtran_U16bit     __sctp_U16bit;

    typedef  sigtran_void_t     __sctp_void_t;

    typedef  sigtran_time_t     __sctp_time_t;

    /* Used for timers */
    typedef  sigtran_timer_id_t        sctp_timer_t;

    /* Used in socket API support functions */
#ifndef _SOCKLEN_T
#define _SOCKLEN_T
    typedef  sigtran_U32bit sctp_socklen_t;
#else
    typedef  socklen_t      sctp_socklen_t;
#endif

    typedef sigtran_pool_info_st    sctp_pool_info_st;
    typedef sigtran_pool_id_t       sctp_pool_id_t;
    typedef sigtran_timer_list_t    sctp_timer_list_t;

#ifndef SCTP_INET6_SUPPORT
    struct sctp_in6_addr
    {
        union
        {
            uint8_t    _S6_u8[16];     /* IPv6 address */
            uint32_t   _S6_u32[4];     /* IPv6 address */
            uint32_t   __S6_align;     /* Align on 32 bit boundary */
        } _S6_un;
    };

    /* We need to define the ipv6 structure. */
    typedef struct __sctp_os_sockaddr_in6_st
    {
        sa_family_t     sin6_family;
        in_port_t       sin6_port;
        uint32_t        sin6_flowinfo;
        struct in6_addr sin6_addr;
        uint32_t        sin6_scope_id;  /* Depends on scope of sin6_addr */
        uint32_t        __sin6_src_id;  /* Impl. specific - UDP replies */
    } sctp_os_sockaddr_in6_st;
#else
    typedef struct sockaddr_in6     sctp_os_sockaddr_in6_st;
#endif

    typedef struct sockaddr_in     sctp_os_sockaddr_in_st;


    /* SCTP IPv6 specific defines */

    /* IPV6 SCope values. */
#define SCTP_IPV6_SCOPE_NODE_LOCAL       0x1
#define SCTP_IPV6_SCOPE_LINK_LOCAL       0x2
#define SCTP_IPV6_SCOPE_SITE_LOCAL       0x5

    /* The header id incase the msg is ICMP6 */
#define SCTP_IP6_NXTHDR_TYPE_ICMP6        58


    /* The Ipv4 Header structure */
    typedef     struct ip         SCTP_IP4;

    /* The ICMP v4 structure */
    typedef     struct icmp       SCTP_ICMP4;

    /* The UDP Header structure */
    typedef     struct    udphdr     SCTP_UDPHDR;

    /* The size of the UDP header. */
#define SCTP_UDP_HDR_SIZE     sizeof(struct udphdr)

#ifdef SCTP_INET6_SUPPORT
    /* The Ipv6 Header structure */
    typedef struct ip6_hdr       SCTP_IP6;

    typedef struct icmp6_hdr     SCTP_ICMP6;

#define SCTP_MAX_IP_HEADER_SIZE sizeof(SCTP_IP6)
#else
#define SCTP_MAX_IP_HEADER_SIZE sizeof(SCTP_IP4)
#endif /* SCTP_INET6_SUPPORT */

#define SCTP_MAX_IP4_HEADER_SIZE sizeof(SCTP_IP4)

#define     __RANDOM()           sigtran_random()

#define     __INITSEED(value)    sigtran_srandom(value)

    /* The IP Version */
#define IP4_VERSION          4
#define IP6_VERSION          6

#define IP_DSCP              0
#define IPHEADER_TTL         0xff

#ifdef SCTP_INET6_SUPPORT

#define IPHEADER_HOPLIMIT   256
#define IPHEADER_TCLASS     256

#endif


    /* #define's for Inaddr Any */
    /* CSR 69326 fix starts*/
    /*
    #define SCTP_IS_INADDR_ANY(sock_addr) \
        ((SCTP_AF_INET == sock_addr.family) ? \
           ((!(sock_addr.ip.v4.addr)) ? SCTP_TRUE: SCTP_FALSE) : \
           ((!(sock_addr.ip.v6.addr.s_u6_addr32[0]) && \
             !(sock_addr.ip.v6.addr.s_u6_addr32[1]) && \
             !(sock_addr.ip.v6.addr.s_u6_addr32[2]) && \
             !(sock_addr.ip.v6.addr.s_u6_addr32[3])) ? SCTP_TRUE : SCTP_FALSE))
    */

    /* CSR 69326 fix ends*/
    /* The SCTP msghdr structure */
    typedef struct __sctp_msghdr
    {
        void    *msg_name;              /* optional address */
        size_t  msg_namelen;            /* size of address */
        struct  iovec *msg_iov;         /* scatter/gather array */
        int     msg_iovlen;             /* # elements in msg_iov */
        void    *msg_control;           /* ancillary data */
        size_t  msg_controllen;         /* ancillary data buffer len */
        int     msg_flags;              /* flags on received message */
    } sctp_msghdr_st;

    typedef struct iovec sctp_iovec_st;


#define SCTP_OS_PATH_SEP '/'

    /* Changes for SPR#15820 *
     * Currently we are supporting cleanup functions only on solaris and linux
     * ports */
#define SCTP_PTHREAD_CLEANUP_PUSH(cleanup_fn_ptr,select_st_ptr) \
    pthread_cleanup_push(cleanup_fn_ptr,(sctp_void_t *)select_st_ptr)

#define SCTP_PTHREAD_CLEANUP_POP(sctp_execute_val) \
    pthread_cleanup_pop(sctp_execute_val)
    /* Changes for SPR#15820 Ends */

#ifdef __cplusplus
}
#endif


#endif /* _S_OS_H_ */

