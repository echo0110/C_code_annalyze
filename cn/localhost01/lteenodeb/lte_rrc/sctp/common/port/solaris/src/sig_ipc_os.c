/*****************************************************************************
 *    FILE NAME:
 *        sig_ipc_os.c
 *
 *    DESCRIPTION:
 *       This file has porting function for IPC related related functions.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    04/11/2003  Vishavdeep Sharma   -            Original Creation
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED  1
#define _XOPEN_VERSION 1
#define __EXTENSIONS__


#include <sig_nw_util.h>
#include <sig_typs.h>
#include <sig_defs.h>
#include <sig_ipc_intr.h>
#include <sig_dbg.h>

#include <sig_os_port.h>
#include <sig_mem.h>

#if !defined (CMSG_SPACE)
/* Amount of space + padding needed for a message of length l */
#define CMSG_SPACE(l)                           \
    ((unsigned int)_CMSG_HDR_ALIGN(sizeof (struct cmsghdr) + (l)))
#endif

#if !defined (CMSG_LEN)
/* Value to be used in cmsg_len, does not include trailing padding */
#define CMSG_LEN(l)                         \
    ((unsigned int)_CMSG_DATA_ALIGN(sizeof (struct cmsghdr)) + (l))
#endif

/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_tcp_client
**
******************************************************************************
**
**  DESCRIPTION
**      This function will set up a TCP socket client
**
**  INPUTS
**
**  RETURNS
**      client fd if succesful
**      0 otherwise
**
*****************************************************************************/

sig_sock_fd_t soc_mgr_setup_tcp_client
( sigtran_U32bit         lcl_ip,
  sigtran_U16bit         lcl_port,
  sigtran_U32bit         serv_ip,
  sigtran_U16bit         serv_port )
{
    sig_sock_fd_t       sockfd;
    struct sockaddr_in  saddr;

    lcl_ip = lcl_ip;
    lcl_port = lcl_port;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons( serv_port );
    saddr.sin_addr.s_addr = htonl( serv_ip );

    if( ( sigtran_S16bit )( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        return 0;
    }

    if( connect( sockfd, ( struct sockaddr * )&saddr,
                 sizeof( struct sockaddr_in ) ) < 0 )
    {
        close( sockfd );
        return 0;
    }

    return sockfd;
}

/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_tcp_server
**
******************************************************************************
**
**  DESCRIPTION
**      This function will set up a tcp server
**
**  INPUTS
**      serv_tcpport:   port of the server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**      make_connection() must be used to accept connections on this server
**
*****************************************************************************/

sig_sock_fd_t soc_mgr_setup_tcp_server
( sigtran_U32bit         serv_ip,
  sigtran_U16bit          serv_port )
{
    const int       on = 1;     /* for setsockopt() */
    sig_sock_fd_t   sock;
    struct sockaddr_in  saddr;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons( serv_port );
    saddr.sin_addr.s_addr = htonl( serv_ip );

    if( ( sigtran_S16bit )( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        return 0;
    }

    if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, ( void * )&on,
                    sizeof( int ) ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( listen( sock, 5 ) < 0 )
    {
        close( sock );
        return 0;
    }

    return sock;
}



/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_nw_write
**
******************************************************************************
**
**  DESCRIPTION
**      This function will write the specified number of bytes into the socket
**  stream
**
**
**  RETURNS
**      number of bytes written
**
*****************************************************************************/

int soc_mgr_nw_write
( sig_sock_fd_t sockfd,
  void            *p_buf,
  unsigned int    nbytes,
  unsigned char   b_is_iovec )
{
    int             nleft;
    int             nwritten;
    unsigned int    nsent;
    struct msghdr   msg;

    if( 0 == b_is_iovec )
    {
        nleft = nbytes;

        while( nleft > 0 )
        {
            nwritten = send( sockfd, ( char * ) p_buf, nleft, 0 );

            if( nwritten < 0 )
            {
                return nwritten;    /* error return < 0 */
            }

            nleft -= nwritten;
            p_buf = ( char * )( nwritten + ( char * )p_buf );
            /*(char *)p_buf += nwritten;*/
        }

        return ( nbytes - nleft );
    }

    else
    {
        /*
         * If the data passed is in the form of iovec.
         */
        msg.msg_iov          = p_buf;
        msg.msg_iovlen       = nbytes;

#ifndef _XPG4_2
        msg.msg_accrights    = NULL;
        msg.msg_accrightslen = 0;
#else
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
#endif

        msg.msg_name         = ( caddr_t )NULL;
        msg.msg_namelen      = 0;

        nsent = sendmsg( sockfd, &msg, 0 );
        return nsent;
    }
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_udp_server
**
************************************************************************
**
**  DESCRIPTION
**      This function will set up a udp server
**
**  INPUTS
**      serv_udpport:   port of the udp server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**
**********************************************************************/

sig_sock_fd_t soc_mgr_setup_udp_server
( sigtran_U32bit         serv_ip,
  sigtran_U16bit         serv_port,
  sigtran_boolean_t      read_flag )
{
    const int       on = 1;     /* for setsockopt() */
    sig_sock_fd_t   sock;
    struct sockaddr_in  saddr;

    serv_ip = serv_ip;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons( serv_port );
    saddr.sin_addr.s_addr = htonl( INADDR_ANY );

    SIG_DEBUG_PRINT( ( "INSIDE :: soc_mgr_setup_udp_server" ) );

    if( ( sigtran_S16bit )( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        return 0;
    }

    if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, ( void * )&on,
                    sizeof( int ) ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( read_flag )
    {
        /* bind this socket only if this is read socket*/
        if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
        {
            close( sock );
            return 0;
        }
    }

    return sock;
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_send_api_on_udp
**
**********************************************************************
**
**  DESCRIPTION
**      This function will send an api to destinatio ip and port
**      given on udp
**
**  INPUTS
**  sockfd: socket stream descriptor from which the bytes are to be sent
**  p_buf:  buffer from which the bytes are to be sent
**  nbytes: number of bytes to be sent
**  dest_ip : destination ip address
**  port :   destination ip address
**
**  RETURNS
**      number of bytes sent
**      -1 on failure
**
***********************************************************************/

int soc_mgr_send_on_udp
( sig_sock_fd_t         sockfd,
  const void          *p_buf,
  unsigned int        buffer_len,
  unsigned int        dest_ip,
  unsigned int        dest_port,
  unsigned char       b_is_iovec )
{
    int                     nsent;
    struct sockaddr_in      entity_addr;
    struct msghdr           msg;

    memset( &entity_addr, 0, sizeof( struct sockaddr_in ) );

    entity_addr.sin_family      = AF_INET;
    entity_addr.sin_port        = htons( dest_port );
    entity_addr.sin_addr.s_addr = htonl( dest_ip );

    if( SIGTRAN_FALSE == b_is_iovec )
    {
        /*
         * If the data passed is not in the form of iovec.
         */
        if( ( nsent = sendto( sockfd, ( char * )p_buf, buffer_len, 0,
                              ( struct sockaddr * )&entity_addr,
                              sizeof( struct sockaddr ) ) ) == -1 )
        {
            return nsent;
        }
    }

    else
    {
        /*
         * If the data passed is in the form of iovec.
         */
        msg.msg_iov          = ( struct iovec * ) p_buf;
        msg.msg_iovlen       = buffer_len;
#ifndef _XPG4_2
        msg.msg_accrights    = NULL;
        msg.msg_accrightslen = 0;
#else
        msg.msg_control      = NULL;
        msg.msg_controllen   = 0;
#endif
        msg.msg_name         = ( caddr_t )&entity_addr;
        msg.msg_namelen      = sizeof( entity_addr );

        nsent = sendmsg( sockfd, &msg, 0 );
    }

    return nsent;
}

/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_nw_read
**
******************************************************************************
**
**  DESCRIPTION
**      This function will read the specified number of bytes from the socket
**
*****************************************************************************/
int soc_mgr_nw_read( sig_sock_fd_t  sockfd,
                     unsigned int   len_to_read,
                     unsigned char  read_flag,
                     unsigned int   *ip_addr,
                     unsigned short *serv_port,
                     void           *pbuf,
                     unsigned int   *len_read )
{
    long int                msgsize;
    int                     flag;
    struct sockaddr_in      saddr;
    int                     saddrlen = sizeof( struct sockaddr_in );

    if( read_flag == SIG_RECV_ALL )
    {
        flag = MSG_WAITALL;
    }

    else
    {
        flag = MSG_PEEK;
    }


    if( ( msgsize = recvfrom( sockfd, ( char * )pbuf, len_to_read, flag,
                              ( struct sockaddr * )&saddr, ( int * )&saddrlen ) ) < 0 )
    {
        *len_read = 0;
        /* perror("socks: ");*/
        return -1; /* CSR 1-5892901 */
    }

    if( ip_addr )
    {
        *ip_addr = ntohl( saddr.sin_addr.s_addr );
    }

    if( serv_port )
    {
        *serv_port = ntohs( saddr.sin_port );
    }

    *len_read = msgsize;

    return msgsize;
}


/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_nw_read_in6
**
******************************************************************************
**
**  DESCRIPTION
**      This function will read the specified number of bytes from the socket
**
*****************************************************************************/
int soc_mgr_nw_read_in6( sig_sock_fd_t   sockfd,
                         unsigned int   len_to_read,
                         unsigned char  read_flag,
                         unsigned int   *p_dest_ip,
                         unsigned short *serv_port,
                         void           *pbuf,
                         unsigned int   *len_read,
                         unsigned int   *p_flow_info,
                         unsigned int   *p_scode_id )
{
#ifdef SIGTRAN_INET6_SUPPORT
    long int                msgsize;
    int                     flag;
    struct sockaddr_in6     saddr_in6;
    int                     saddrlen = sizeof( struct sockaddr_in6 );

    if( read_flag == SIG_RECV_ALL )
    {
        flag = MSG_WAITALL;
    }

    else
    {
        flag = MSG_PEEK;
    }


    if( ( msgsize = recvfrom( sockfd, ( char * )pbuf, len_to_read, flag,
                              ( struct sockaddr * )&saddr_in6, ( int * )&saddrlen ) ) < 0 )
    {
        *len_read = 0;
        /* perror("socks: ");*/
        return 0;
    }

    if( p_dest_ip )
    {
        p_dest_ip[0] = ntohl( saddr_in6.sin6_addr._S6_un._S6_u32[0] );
        p_dest_ip[1] = ntohl( saddr_in6.sin6_addr._S6_un._S6_u32[1] );
        p_dest_ip[2] = ntohl( saddr_in6.sin6_addr._S6_un._S6_u32[2] );
        p_dest_ip[3] = ntohl( saddr_in6.sin6_addr._S6_un._S6_u32[3] );
        *p_flow_info = ntohl( saddr_in6.sin6_flowinfo );
        *p_scode_id = ntohl( saddr_in6.sin6_scope_id );
    }

    if( serv_port )
    {
        *serv_port = ntohs( saddr_in6.sin6_port );
    }

    *len_read = msgsize;

    return msgsize;
#else
    sockfd = sockfd;
    len_read = len_read;
    read_flag = read_flag;
    p_dest_ip = p_dest_ip;
    serv_port = serv_port;
    pbuf = pbuf;
    len_to_read = len_to_read;
    p_flow_info = p_flow_info;
    p_scode_id = p_scode_id;
    return ( -1 );
#endif
}
/*****************************************************************************
**
**  FUNCTION NAME
**      soc_mgr_aceept
**
******************************************************************************
**
**  DESCRIPTION
**      This function will wait for connection from client on the socket set up
**  setup_server() function call. It will return after accepting a connection
**  from the client
**
**  INPUTS
**  sock:   Socket descriptor or handle on which to wait for connection
**
**  RETURNS
**      client fd if succesful
**      -1 otherwise
**
*****************************************************************************/
sig_sock_fd_t soc_mgr_aceept
( sig_sock_fd_t sock,
  unsigned int  *ip,
  unsigned short  *port )
{
    int         newsock, fromlen;
    struct sockaddr_in  from;
    struct linger       lg;

    fromlen = sizeof( from );

    if( ( newsock = accept( sock, ( struct sockaddr * )&from, &fromlen ) ) < 0 )
    {
        perror( "\nsoc_mgr_make_connection::ACEEPT ERROR " );
        return ( -1 );
    }

    lg.l_onoff = 1;
    /* l_linger is set to default */
    lg.l_linger = 5;

    if( setsockopt( newsock, SOL_SOCKET, SO_LINGER,
                    ( char * )&lg, sizeof( struct linger ) ) < 0 )
    {
        perror( "\nsoc_mgr_make_connection::SET SOC OPTION ERROR" );
        close( newsock );
        return ( -1 );
    }

    /* fill output parameters */
    *ip = from.sin_addr.s_addr;
    *port = from.sin_port;

    return newsock;
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_raw_ipv4_server
**
************************************************************************
**
**  DESCRIPTION
**      This function will set up a raw ipv4 server
**
**  INPUTS
**      serv_udpport:   port of the raw ipv4 server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**
**********************************************************************/

sig_sock_fd_t soc_mgr_setup_raw_ipv4_server
( sigtran_U32bit         serv_ip,
  sigtran_U16bit         serv_port )
{
    const char  on = 1;     /* for setsockopt() */
    sig_sock_fd_t   sock;
    struct sockaddr_in  saddr;

    SIG_DEBUG_PRINT( ( "INSIDE :: soc_mgr_setup_raw_ipv4_server" ) );

    if( ( sigtran_S16bit )( sock = socket( AF_INET, SOCK_RAW, IPPROTO_SCTP ) ) < 0 )
    {
        return 0;
    }

    if( setsockopt( sock, IPPROTO_IP, IP_HDRINCL, ( void * )&on,
                    sizeof( int ) ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( SOC_INVALID_IP != serv_ip )
    {
        memset( &saddr, 0, sizeof( struct sockaddr_in ) );
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons( serv_port );
        saddr.sin_addr.s_addr = htonl( serv_ip );

        if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
        {
            close( sock );
            return 0;
        }
    }

    return sock;
}


/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_icmp_ipv4_server
**
************************************************************************
**
**  DESCRIPTION
**      This function will set up a ICMP ipv4 server
**
**  INPUTS
**      serv_udpport:   port of the ICMP ipv4 server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**
**********************************************************************/

sig_sock_fd_t soc_mgr_setup_icmp_ipv4_server
( sigtran_U32bit         serv_ip,
  sigtran_U16bit         serv_port )
{
    const char  on = 1;     /* for setsockopt() */
    sig_sock_fd_t   sock;
    struct sockaddr_in  saddr;

    SIG_DEBUG_PRINT( ( "INSIDE :: soc_mgr_setup_icmp_ipv4_server" ) );

    if( ( sigtran_S16bit )( sock = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP ) ) < 0 )
    {
        return 0;
    }

    if( setsockopt( sock, IPPROTO_IP, IP_HDRINCL, ( void * )&on,
                    sizeof( int ) ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( fcntl( sock, F_SETFL, O_NONBLOCK ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( SOC_INVALID_IP != serv_ip )
    {
        memset( &saddr, 0, sizeof( struct sockaddr_in ) );
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons( serv_port );
        saddr.sin_addr.s_addr = htonl( serv_ip );

        if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
        {
            close( sock );
            return 0;
        }
    }

    return sock;
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_send_api_on_udp_ipv6
**
**********************************************************************
**
**  DESCRIPTION
**      This function will send an api to destinatio ip and port
**      given on udp for ipv6 addresses
**
**  INPUTS
**  sockfd: socket stream descriptor from which the bytes are to be sent
**  p_buf:  buffer from which the bytes are to be sent
**  nbytes: number of bytes to be sent
**  dest_ip : destination ip address
**  port :   destination ip address
**
**  RETURNS
**      number of bytes sent
**      -1 on failure
**
***********************************************************************/

int soc_mgr_send_on_udp_ipv6
( sig_sock_fd_t         sockfd,
  void                *p_buf,
  unsigned int        buffer_len,
  unsigned int        *p_dest_ip,
  unsigned int        dest_port,
  unsigned char       b_is_iovec )
{
#ifdef SIGTRAN_INET6_SUPPORT
    int         nsent;
    struct sockaddr_in6     entity_addr;
    struct msghdr           msg;

    sockfd = sockfd;


    memset( &entity_addr, 0, sizeof( struct sockaddr_in6 ) );

    entity_addr.sin6_family      = AF_INET6;
    entity_addr.sin6_port        = htons( dest_port );
    entity_addr.sin6_addr._S6_un._S6_u32[0] = p_dest_ip[0];
    entity_addr.sin6_addr._S6_un._S6_u32[1] = p_dest_ip[1];
    entity_addr.sin6_addr._S6_un._S6_u32[2] = p_dest_ip[2];
    entity_addr.sin6_addr._S6_un._S6_u32[3] = p_dest_ip[3];
    entity_addr.sin6_flowinfo   = 0;
    entity_addr.sin6_scope_id   = 0;

    if( SIGTRAN_FALSE  == b_is_iovec )
    {
        /*
         * If the data passed is not in the form of iovec.
         */
        if( ( nsent = sendto( sockfd, ( char * )p_buf, buffer_len, 0,
                              ( struct sockaddr * )&entity_addr,
                              sizeof( entity_addr ) ) ) == -1 )
        {
            return nsent;
        }
    }

    else
    {
        /*
         * If the data passed is in the form of iovec.
         */
        msg.msg_iov          = p_buf;
        msg.msg_iovlen       = buffer_len;
#ifndef _XPG4_2
        msg.msg_accrights    = NULL;
        msg.msg_accrightslen = 0;
#else
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
#endif
        msg.msg_name         = ( caddr_t )&entity_addr;
        msg.msg_namelen      = sizeof( entity_addr );

        nsent = sendmsg( sockfd, &msg, 0 );
    }

    return nsent;
#else
    p_buf = p_buf;
    buffer_len = buffer_len;
    sockfd = sockfd;
    p_dest_ip = p_dest_ip;
    dest_port = dest_port;
    b_is_iovec = b_is_iovec;
    return ( -1 );
#endif
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_send_on_udp_ipv6_ext
**
**********************************************************************
**
**  DESCRIPTION
**      This function will send an api to destinatio ip and port
**      given on udp for ipv6 addresses
**
**  INPUTS
**  sockfd: socket stream descriptor from which the bytes are to be sent
**  p_buf:  buffer from which the bytes are to be sent
**  nbytes: number of bytes to be sent
**  dest_ip : destination ip address
**  port :   destination ip address
**
**  RETURNS
**      number of bytes sent
**      -1 on failure
**
***********************************************************************/
int soc_mgr_send_on_udp_ipv6_ext
( sig_sock_fd_t         sockfd,
  void                *p_buf,
  unsigned int        buffer_len,
  unsigned int        send_flags,
  unsigned int        *p_dest_ip,
  unsigned int        dest_port,
  unsigned char       b_is_iovec )
{
#ifdef SIGTRAN_INET6_SUPPORT
    int    nsent;
    struct sockaddr_in6     entity_addr;
    struct msghdr           msg;
    struct cmsghdr *cmsg = NULL;
    int cmsg_len = 0;

    sockfd = sockfd;

    memset( &entity_addr, 0, sizeof( struct sockaddr_in6 ) );

    entity_addr.sin6_family      = AF_INET6;
    entity_addr.sin6_port        = htons( dest_port );
    entity_addr.sin6_addr._S6_un._S6_u32[0] = p_dest_ip[0];
    entity_addr.sin6_addr._S6_un._S6_u32[1] = p_dest_ip[1];
    entity_addr.sin6_addr._S6_un._S6_u32[2] = p_dest_ip[2];
    entity_addr.sin6_addr._S6_un._S6_u32[3] = p_dest_ip[3];
    entity_addr.sin6_flowinfo   = 0;
    entity_addr.sin6_scope_id   = 0;

    if( SIGTRAN_FALSE  == b_is_iovec )
    {
        /*
         * If the data passed is not in the form of iovec.
         */
        if( ( nsent = sendto( sockfd, ( char * )p_buf, buffer_len, 0,
                              ( struct sockaddr * )&entity_addr,
                              sizeof( entity_addr ) ) ) == -1 )
        {
            return nsent;
        }
    }

    else
    {
#if defined (IPV6_TCLASS) && defined (IPV6_HOPLIMIT)
        cmsg_len = ( CMSG_SPACE( sizeof( sigtran_S32bit ) ) * 2 ) ;
        cmsg     = ( struct cmsghdr * ) sigtran_os_malloc( cmsg_len );
#else
#if defined (IPV6_TCLASS) || defined (IPV6_HOPLIMIT )
        cmsg_len = CMSG_SPACE( sizeof( sigtran_S32bit ) ) ;
        cmsg     = ( struct cmsghdr * ) sigtran_os_malloc( cmsg_len );
#endif
#endif

        /*
         * If the data passed is in the form of iovec.
         */
        msg.msg_iov          = p_buf;
        msg.msg_iovlen       = buffer_len;
#if defined (IPV6_TCLASS) || defined (IPV6_HOPLIMIT )

        if( cmsg != SIGTRAN_NULL )
        {
            sigtran_fill_cmsg_header( cmsg, send_flags );
        }

#endif
#ifndef _XPG4_2
        msg.msg_accrights    = cmsg;
        msg.msg_accrightslen = cmsg_len;
#else
        msg.msg_control      = cmsg;
        msg.msg_controllen   = cmsg_len;
#endif
        msg.msg_name         = ( caddr_t )&entity_addr;
        msg.msg_namelen      = sizeof( entity_addr );

        nsent = sendmsg( sockfd, &msg, 0 );
#if defined (IPV6_TCLASS) || defined (IPV6_HOPLIMIT )
        sigtran_os_free( cmsg );
#endif
    }

    return nsent;

#else
    p_buf = p_buf;
    buffer_len = buffer_len;
    sockfd = sockfd;
    p_dest_ip = p_dest_ip;
    dest_port = dest_port;
    b_is_iovec = b_is_iovec;
    send_flags = send_flags;
    return ( -1 );
#endif
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_raw_ipv6_server
**
************************************************************************
**
**  DESCRIPTION
**      This function will set up a raw ipv6 server
**
**  INPUTS
**      serv_udpport:   port of the raw ipv6 server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**
**********************************************************************/
sig_sock_fd_t soc_mgr_setup_raw_ipv6_server
( sigtran_U32bit         *p_serv_ip,
  sigtran_U16bit         serv_port,
  sigtran_U32bit         flow_info,
  sigtran_U32bit         scope_id )
{
#ifdef SIGTRAN_INET6_SUPPORT
    sig_sock_fd_t   sock;
    struct sockaddr_in6 saddr;

    SIG_DEBUG_PRINT( ( "INSIDE :: soc_mgr_setup_raw_ipv6_server" ) );

    if( ( sigtran_S16bit )( sock = socket( AF_INET6, SOCK_RAW, IPPROTO_SCTP ) ) < 0 )
    {
        return 0;
    }

    if( SIGTRAN_NULL != p_serv_ip )
    {
        memset( &saddr, 0, sizeof( struct sockaddr_in6 ) );

        saddr.sin6_family                   = AF_INET6;
        saddr.sin6_port                     = htons( serv_port );
        saddr.sin6_addr._S6_un._S6_u32[0]   = htonl( p_serv_ip[0] );
        saddr.sin6_addr._S6_un._S6_u32[1]   = htonl( p_serv_ip[1] );
        saddr.sin6_addr._S6_un._S6_u32[2]   = htonl( p_serv_ip[2] );
        saddr.sin6_addr._S6_un._S6_u32[3]   = htonl( p_serv_ip[3] );

        saddr.sin6_scope_id                 = scope_id;
        saddr.sin6_flowinfo                 = flow_info;

        if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
        {
            close( sock );
            return 0;
        }
    }

    if( fcntl( sock, F_SETFL, O_NONBLOCK ) < 0 )
    {
        close( sock );
        return 0;
    }

    return sock;
#else
    p_serv_ip = p_serv_ip,
    serv_port = serv_port;
    flow_info = flow_info;
    scope_id = scope_id;
    return ( -1 );
#endif
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_icmp_ipv6_server
**
************************************************************************
**
**  DESCRIPTION
**      This function will set up a ICMP ipv6 server
**
**  INPUTS
**      serv_udpport:   port of the ICMP ipv6 server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**
**********************************************************************/

sig_sock_fd_t soc_mgr_setup_icmp_ipv6_server
( sigtran_U32bit         *p_serv_ip,
  sigtran_U16bit         serv_port,
  sigtran_U32bit         flow_info,
  sigtran_U32bit         scope_id )
{
#ifdef SIGTRAN_INET6_SUPPORT
    sig_sock_fd_t   sock;
    struct sockaddr_in6 saddr;

    SIG_DEBUG_PRINT( ( "INSIDE :: soc_mgr_setup_raw_ipv6_server" ) );

    if( ( sigtran_S16bit )( sock = socket( AF_INET6, SOCK_RAW, IPPROTO_ICMP ) ) < 0 )
    {
        return 0;
    }

    if( SIGTRAN_NULL != p_serv_ip )
    {
        memset( &saddr, 0, sizeof( struct sockaddr_in6 ) );

        saddr.sin6_family                   = AF_INET6;
        saddr.sin6_port                     = htons( serv_port );
        saddr.sin6_addr._S6_un._S6_u32[0]   = htonl( p_serv_ip[0] );
        saddr.sin6_addr._S6_un._S6_u32[1]   = htonl( p_serv_ip[1] );
        saddr.sin6_addr._S6_un._S6_u32[2]   = htonl( p_serv_ip[2] );
        saddr.sin6_addr._S6_un._S6_u32[3]   = htonl( p_serv_ip[3] );

        saddr.sin6_scope_id                 = scope_id;
        saddr.sin6_flowinfo                 = flow_info;

        if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
        {
            close( sock );
            return 0;
        }
    }

    if( fcntl( sock, F_SETFL, O_NONBLOCK ) < 0 )
    {
        close( sock );
        return 0;
    }

    return sock;
#else
    p_serv_ip = p_serv_ip,
    serv_port = serv_port;
    flow_info = flow_info;
    scope_id = scope_id;
    return ( -1 );
#endif
}

/**********************************************************************
**
**  FUNCTION NAME
**      soc_mgr_setup_udp_nw_server
**
************************************************************************
**
**  DESCRIPTION
**      This function will set up a udp server
**
**  INPUTS
**      serv_udpport:   port of the udp server
**
**  RETURNS
**      server fd if succesful
**      0 otherwise
**
**  NOTE
**
**********************************************************************/

sig_sock_fd_t soc_mgr_setup_udp_nw_server
( sigtran_U32bit         serv_ip,
  sigtran_U16bit         serv_port )
{
    const int       on = 1;     /* for setsockopt() */
    sig_sock_fd_t   sock;
    struct sockaddr_in  saddr;

    SIG_DEBUG_PRINT( ( "INSIDE :: soc_mgr_setup_udp_server" ) );

    if( ( sigtran_S16bit )( sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 )
    {
        return 0;
    }

    if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, ( void * )&on,
                    sizeof( int ) ) < 0 )
    {
        close( sock );
        return 0;
    }

    if( SOC_INVALID_IP != serv_ip )
    {
        memset( &saddr, 0, sizeof( saddr ) );
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons( serv_port );
        saddr.sin_addr.s_addr = htonl( serv_ip );

        SIG_DEBUG_PRINT( ( "\nBind on IP = [%u] and Port Number = [%u]\n", serv_ip, serv_port ) );

        if( bind( sock, ( struct sockaddr * )&saddr, sizeof( saddr ) ) < 0 )
        {
            close( sock );
            return 0;
        }
    }

    if( fcntl( sock, F_SETFL, O_NONBLOCK ) < 0 )
    {
        close( sock );
        return 0;
    }

    return sock;
}


/******************************************************************************
**  Function Name       : sig_os_select
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
sigtran_S32bit sig_os_select(
    sigtran_S32bit numfds,
    sig_fd_set *p_readfds,
    sig_fd_set *p_writefds,
    sig_fd_set *p_exceptfds,
    sigtran_time_t *p_timeout )
{
    numfds = numfds;
    return select( FD_SETSIZE, p_readfds, p_writefds, p_exceptfds, p_timeout );
}

/******************************************************************************
**  Function Name       : sig_os_resolve_hostname
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
sigtran_U32bit  sig_os_resolve_hostname(
    sigtran_U8bit      *p_hostname,
    sig_ip_addr_st     *p_ip_addr_list,
    sigtran_U32bit      num_addrs )
{
    struct hostent      *hostptr;

#ifdef SIGTRAN_INET6_SUPPORT
    unsigned int        counter_v4 = 0, counter_v6 = 0;
    struct in6_addr     *p_addr_ptr6;
    int                 error_num;
#else
    unsigned int        counter;
    struct in_addr      *p_addr_ptr;
#endif

#ifdef SIGTRAN_INET6_SUPPORT
    /* for flag = 0 & AF_INET6, it will serch for IPv6 address only */

    hostptr = getipnodebyname( ( char * )p_hostname, AF_INET6, 0, &error_num );

    if( ( hostptr != NULL ) && ( hostptr->h_length == 16 ) )
    {
        for( counter_v6 = 0; counter_v6 < num_addrs; counter_v6++ )
        {
            if( ( p_addr_ptr6 = ( struct in6_addr * ) hostptr->h_addr_list[counter_v6] )
                    != NULL )
            {
                /* now set the addr elements */
                p_ip_addr_list[counter_v6].ip_type  = SIG_TRANS_ADDR_IPV6;
                p_ip_addr_list[counter_v6].ip_addr_ut.addr_v6.scope_id = 0;
                p_ip_addr_list[counter_v6].ip_addr_ut.addr_v6.flow_info = 0;

                p_ip_addr_list[counter_v6].ip_addr_ut.addr_v6.ip_addr_ut.addr32[0] =
                    p_addr_ptr6->_S6_un._S6_u32[0];
                p_ip_addr_list[counter_v6].ip_addr_ut.addr_v6.ip_addr_ut.addr32[1] =
                    p_addr_ptr6->_S6_un._S6_u32[1];
                p_ip_addr_list[counter_v6].ip_addr_ut.addr_v6.ip_addr_ut.addr32[2] =
                    p_addr_ptr6->_S6_un._S6_u32[2];
                p_ip_addr_list[counter_v6].ip_addr_ut.addr_v6.ip_addr_ut.addr32[3] =
                    p_addr_ptr6->_S6_un._S6_u32[3];
            }

            else
            {
                break;
            }
        } /* end of for loop */
    } /* end of if hostptr*/

    counter_v4 = counter_v6;

    /* search for IPv4 addr if we have any more addr_list entry */
    if( counter_v6 < num_addrs )
    {
        /* for flag = 0 & AF_INET, it will serch for IPv4 address only */
        hostptr = getipnodebyname( ( char * )p_hostname, AF_INET, 0, &error_num );

        if( ( hostptr != NULL ) && ( hostptr->h_length == 4 ) )
        {
            struct in_addr *p_inaddr;

            for( ; counter_v4 < num_addrs; counter_v4++ )
            {
                p_inaddr = ( struct in_addr * ) hostptr->h_addr_list[counter_v4 -
                                                                     counter_v6];

                if( p_inaddr != NULL )
                {
                    p_ip_addr_list[counter_v4].ip_type  = SIG_TRANS_ADDR_IPV4;
                    p_ip_addr_list[counter_v4].ip_addr_ut.addr_v4.addr =
                        ntohl( p_inaddr->s_addr );
                }

                else
                {
                    break;
                }
            }
        }
    }

    /* The Number of addresses that we could find for IPV6 & IPV4 */
    return ( counter_v4 );

#else /* the host does not support IPV6 */

    if( ( hostptr = gethostbyname( ( const char * )p_hostname ) ) == NULL )
    {
        return ( 0 );
    }

    if( hostptr->h_length != 4 )
    {
        return ( 0 );
    }

    /*
     * Set As many IPV4 addresses that you could find corresponding to the
     * Host name.
     */
    for( counter = 0; counter < num_addrs; counter++ )
    {
        if( ( p_addr_ptr = ( struct in_addr * ) hostptr->h_addr_list[counter] )
                == SIGTRAN_NULL )
        {
            break;
        }

        p_ip_addr_list[counter].ip_type  = SIG_TRANS_ADDR_IPV4;
        p_ip_addr_list[counter].ip_addr_ut.addr_v4.addr =
            ntohl( p_addr_ptr->s_addr );
    }

    /* The Number of addresses that you could find for IPV4 */
    return( counter );
#endif
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return ( 0 );
#endif
}

/******************************************************************************
**  Function Name       : sig_os_gethostname
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
sigtran_U32bit  sig_os_gethostname(
    sigtran_U8bit      *p_hostname,
    sigtran_U32bit      host_len )
{
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    return gethostname( p_hostname, host_len );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return gethostname( ( sigtran_8bit * )p_hostname, host_len );
#endif

}
/******************************************************************************
**  Function Name       : sig_os_close
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
sigtran_S32bit sig_os_close( sig_sock_fd_t sockfd )
{
    return close( sockfd );
}
/******************************************************************************
 * **  FUNCTION :    sigtran_random
 * **
 * **  DESCRIPTION:  Returns  radom number
 * **
 * **  RETURN VALUE: random number
 * ******************************************************************************/
sigtran_U32bit sigtran_random( void )
{
    return random();
}

/******************************************************************************
* **
 * **  DESCRIPTION:  Returns  radom number
 * **
 * **  RETURN VALUE: random number
 * ******************************************************************************/
void sigtran_srandom( sigtran_U32bit value )
{
    srandom( value );
    return;
}

/***************************************************************************
 *    FUNCTION :
 *        sigtran_fill_cmsg_header
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *       void
 **************************************************************************/
void sigtran_fill_cmsg_header( struct cmsghdr *cmsg, sigtran_U32bit send_flags )
{
#if defined (IPV6_HOPLIMIT)
    sigtran_S32bit hoplimit = -1;
#endif

#if defined (IPV6_TCLASS)
    sigtran_S32bit tclass = -1;
#endif

#if defined (IPV6_HOPLIMIT)

    if( ( hoplimit = send_flags >> 16 ) > 255 )
    {
        hoplimit  = -1;
    }

    cmsg->cmsg_level = IPPROTO_IPV6;
    cmsg->cmsg_type  = IPV6_HOPLIMIT;
    cmsg->cmsg_len   = CMSG_LEN( sizeof( sigtran_S32bit ) );
    memcpy( CMSG_DATA( cmsg ), &hoplimit, sizeof( sigtran_S32bit ) );
#endif

#if defined (IPV6_TCLASS)

#if defined (IPV6_HOPLIMIT)
    /* Increment the cmsg pointer. */
    cmsg = ( struct cmsghdr * )( ( sigtran_U8bit * )cmsg + CMSG_SPACE( sizeof( sigtran_S32bit ) ) );
#endif

    if( ( tclass = ( ( send_flags << 16 ) >> 16 ) ) > 255 )
    {
        tclass = -1;
    }

    cmsg->cmsg_level = IPPROTO_IPV6;
    cmsg->cmsg_type  = IPV6_TCLASS;
    cmsg->cmsg_len   = CMSG_LEN( sizeof( sigtran_S32bit ) );
    memcpy( CMSG_DATA( cmsg ), &tclass, sizeof( sigtran_S32bit ) );
#else
    cmsg = cmsg;
    send_flags = send_flags;
#endif

}

