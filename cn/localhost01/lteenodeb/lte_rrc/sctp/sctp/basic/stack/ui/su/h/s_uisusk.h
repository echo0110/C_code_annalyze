/*****************************************************************************
 *    FILE NAME:
 *          s_uisusk.h
 *
 *    DESCRIPTION:
 *        This file is part of Service User Interface module of SCTP stack.
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    11Sep01     gsheoran      -            Create Original
 *    08Oct02  mrajpal       SPR 4926        getsockopt params
 *    20Oct04  Amit Ray      REL 6.0         SCTP/FUNC-SOCKAPI-32
 *    20Oct04  Amit Ray      REL 6.0         SCTP/FUNC-SOCKAPI-33
 *    22Oct04  Amit Ray      REL 6.0         SCTP/FUNC-SOCKAPI-30
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef _S_UISUSK_H_SEEN_
#define _S_UISUSK_H_SEEN_

#ifdef __cplusplus
extern "C" {
#endif

#include <s_os.h>
#include <s_select.h>


#define SK_FAILURE      (-1)
#define SK_SUCCESS      (0)

    /**
     *  Error Codes enumeration. The List of error codes used by the socket module.
     */
    typedef enum
    {
        SK_EAFNOSUPPORT, /**< The Address family is not supported */
        SK_EACCES, /**< The process does not have access -> Not initailized */
        SK_EPROTONOSUPPORT, /**< Protocol type not supported */
        SK_EPROTOTYPE, /**< The Socket type is not supported by the protocol */
        SK_ENOBUFS, /**< Insuffient buffers/resources */
        SK_EMFILE,  /**< Insufficient endpoints -> insuff socket descriptors */
        SK_ENOTSOCK, /**< The id passed is not a valid socket. */
        SK_EBADF, /**< The file descriptor passed is not valid */
        SK_EADDRNOTAVAIL, /**< Address not available on the machine */
        SK_EADDRINUSE, /**< Address is being used by some other socket. */
        SK_EOPNOTSUPP, /**< operation not supported: Random errors */
        SK_EALREADY,      /**< Connection already exists */
        SK_EINPROGRESS,   /**< Connection is already in progress */
        SK_EISCONN, /**< sd is conn mode and is already connected */
        SK_EAGAIN,  /**< No Connection waiting to be accepted on the server */
        SK_ENOTCONN,/**< The Socket is not connected */
        SK_ENOPROTOOPT,  /**< The option is not supported by the protocol */
        SK_ECONNRESET,   /**< The Peer reset the connection */
        SK_EDESTADDRREQ,  /**< The Socket is not bound and we want to listen */
        SK_EINVAL,        /**< Invalid parameters */
        SK_ECONNREFUSED,  /**< The Connection was refused. */
        SK_ENOCONN,        /**<  Connection doesnot exist */
        SK_ESHUTDOWN,      /**<  Shutdown has been received from peer */
        SK_EFRAGMENT      /*fragmentation is required and user has disabled it*/
    }
    sctp_sk_err_et;

    /** global error number. Contains error occured in last call */
    extern sctp_sk_err_et sk_errno;

#ifdef SCTP_UI_SOCK_API

    /**
     * creates an endpoint for communication. The socket() function creates
     * an unbound socket in a communications  domain, and returns a socket
     * descriptor that can be used in later function calls that operate on
     * sockets.
     * @param domain Specifies the communications domain in  which a socket
     *               is to be created
     * @param type   Specifies the type of socket to be created.
     *               Can be SCTP_SOCK_STREAM or SCTP_SOCK_SEQPACKET
     * @param protocol     Must be IPPROTO_SCTP for sctp sockets
     * @return socket descriptor if successful, otherwise -1
     */
    int
    sctp_socket(
        int                  domain,
        int                  type,
        int                  protocol );

    /**
     * bind a name to a socket. The bind() function assigns an address to an
     * unnamed socket.
     * @see sctp_socket
     * @param sd    socket descriptor of socket to be bound
     * @param addr  Points to a sockaddr structure containing the address  to
     *              be  bound  to  the  socket.  The length and format of the
     *              address  depend  on the address family of the socket
     * @param addrlen length of structure pointed to by addr
     */
    int
    sctp_bind(
        int                  sd,
        struct sockaddr      *addr,
        sctp_socklen_t       addrlen );


    /**
     * bind more names to a socket. The bindx() function configures addresses of
     * an already bound socket. This is to exploit the multihoming feature of SCTP.
     * @see sctp_socket
     * @see sctp_bind
     * @param sd    socket descriptor of socket to be bound
     * @param addrs Points to a sockaddr_storage structure containing the addresses
     *              to be added or removed from  socket.  The length and format of the
     *              address  depend  on the address family of the socket
     * @param addrcnt number of addresses in the addrs.
     * @param flags Specify the operation to be performed i.e. ADD or REMOVE
     */
    int
    sctp_bindx(
        int                  sd,
        struct sockaddr      *addrs,
        int                  addrcnt,
        int                  flags );

    int
    sctp_close(
        int                  sd );

    int
    sctp_shutdown(
        int                  sd,
        int                  how );

    int
    sctp_listen(
        int                  sd,
        int                  backlog );

    int
    sctp_connectx(
        int sd,
        const struct sockaddr *sock_addrs,
        int addrcnt );

    int
    sctp_accept(
        int                  sd,
        struct sockaddr      *addr,
        sctp_socklen_t       *addrlen );

    int
    sctp_connect(
        int                   sd,
        const struct sockaddr *addr,
        sctp_socklen_t        addrlen );

    int
    sctp_send(
        int                  sd,
        const void           *p_buffer,
        size_t               length,
        int                  flags );

    int
    sctp_recv(
        int                  sd,
        void                 *p_buffer,
        size_t               length,
        int                  flags );

    int
    sctp_sendto(
        int                     sd,
        const void              *p_buffer,
        size_t                  length,
        int                     flags,
        const struct sockaddr   *destaddr,
        sctp_socklen_t          dest_len );

    int
    sctp_recvfrom(
        int                     sd,
        void                    *p_buffer,
        size_t                  length,
        int                     flags,
        struct sockaddr         *srcddr,
        sctp_socklen_t          *src_len );

    int
    sctp_recvmsg(
        int                  sd,
        sctp_msghdr_st       *p_message,
        int                  flags );

    int
    sctp_sendmsg(
        int                  sd,
        const sctp_msghdr_st *p_message,
        int                  flags );

    int
    sctp_setsockopt(
        int                   sd,
        int                   level,
        int                   optname,
        const void            *optval,
        sctp_socklen_t        optlen );

    /*SPR 4926*/
    int
    sctp_getsockopt(
        int                   sd,
        int                   level,
        int                   optname,
        void                  *optval,
        sctp_socklen_t        *optlen );
    /*SPR 4926*/


    int sctp_getpeername(
        int                  sd,
        struct sockaddr      *address,
        sctp_socklen_t       *len );


    int sctp_getsockname(
        int                  sd,
        struct sockaddr      *address,
        sctp_socklen_t       *len );

    int
    sctp_select(
        int                  n_fds,
        sctp_fd_set_st       *p_rfds,
        sctp_fd_set_st       *p_wfds,
        sctp_fd_set_st       *p_efds,
        sctp_time_t          *p_timeout );

    int
    sctp_getpaddrs(
        int                  sd,
        sctp_assoc_t         assoc_id,
        struct sockaddr      **p_addr );

    void
    sctp_freepaddrs(
        struct sockaddr  *p_addr );

    int
    sctp_getladdrs(
        int                     sd,
        sctp_assoc_t            assoc_id,
        struct sockaddr          **p_addr );

    void
    sctp_freeladdrs(
        struct sockaddr        *p_addr );

    int
    sctp_opt_info(
        int                sd,
        sctp_assoc_t       assoc_id,
        int                opt,
        void               *p_arg,
        sctp_socklen_t     *size );

    int
    sctp_peeloff(
        int                sd,
        sctp_assoc_t       assoc_id );

    int sctp_sctp_sendmsg(
        int             sd,
        const void      *msg,
        sctp_socklen_t  sock_len,
        const struct sockaddr *to,
        sctp_socklen_t        len,
        uint32_t        ppid,
        uint32_t        flags,
        uint16_t        stream_no,
        uint32_t        timetolive,
        uint32_t        context );

    int sctp_sctp_send(
        int             sd,
        const void      *msg,
        sctp_socklen_t  len,
        const           sctp_sndrcvinfo_st *sininfo,
        int             flags );

    int
    sctp_sctp_sendx(
        int                             sd,
        const void                      *msg,
        size_t                          len,
        struct sockaddr                 *addrs,
        int                             addrcnt,
        const struct sctp_sndrcvinfo    *sinfo,
        int                             flags );

    int
    sctp_sctp_recvmsg( int s,
                       void *p_msg,
                       size_t p_len,
                       struct sockaddr *p_from,
                       sctp_socklen_t *p_fromlen,
                       struct sctp_sndrcvinfo *p_sinfo,
                       int *p_msg_flags
                     );
#endif /* SCTP_UI_SOCK_API */

#ifdef __cplusplus
}
#endif

#endif /* _S_UISUSK_H_SEEN_ */


