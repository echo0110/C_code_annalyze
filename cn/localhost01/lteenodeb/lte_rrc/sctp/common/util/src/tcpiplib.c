/*****************************************************************************
 *    FILE NAME:
 *        tcpiplib.c
 *
 *    DESCRIPTION:
 *       This file has common TCPIP related functions.
 *          written by Gautam Sheoran.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    30Sept02    Sandeep Mahajan     -            .Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include "../h/tcpiplib.h"

#if !defined(SIGTRAN_PORT_VXWORKS) && !defined(SIGTRAN_PORT_WINNT) && !defined(SIGTRAN_PORT_VX_PNE)
#include <sys/time.h>
#include <stropts.h>
#else
#include <time.h>
#ifndef SIGTRAN_PORT_WINNT
#include <sys/times.h>
#include <socket.h>
#include <sockLib.h>
#endif
#endif

#ifndef SIGTRAN_PORT_WINNT
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif
#include <errno.h>
#include <stdio.h>

#ifdef SIGTRAN_PORT_SOLARIS
#include <sys/filio.h>
#include <stropts.h>
#endif

#ifdef SIGTRAN_PORT_TRU64
#include <stropts.h>
#endif



/* Creates the socket(type-SOCK_STREAM) and sets it's mode to Nonblocking */
/************************************************************************
** FUNCTION    :
**              socket_create
**
** DESCRIPTION :
**              Creates the socket(type-SOCK_STREAM) and sets it's mode
**                to Nonblocking
** Return Type :int
*************************************************************************/
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
int socket_create( void )
{
    int sockfd;

    sockfd = socket( AF_INET, SOCK_STREAM, 0 );

#ifdef SIGTRAN_PORT_WINNT

    if( sockfd == INVALID_SOCKET )
#else
    if( sockfd < 0 )
#endif
        return ( TCPIPLIB_ERROR );

    return sockfd;
}
/************************************************************************
** FUNCTION    :
**              __declspec
**
** DESCRIPTION :
**              Closes a socket
** Return Type : int
*************************************************************************/
/* Closes a socket */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
int socket_close( int sockfd )
{
    return close( sockfd );
}

/* Sets up a client socket and connect to the destination */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**              socket_setup_client
**
** DESCRIPTION :
**              Sets up a client socket and connect to the destination
** Return Type : int
*************************************************************************/
int socket_setup_client( int sockfd, ulong serv_ipaddr, ushort serv_tcpport )
{
    struct sockaddr_in saddr;

    saddr.sin_family       = AF_INET;
    saddr.sin_port         = htons( serv_tcpport );
    saddr.sin_addr.s_addr  = htonl( serv_ipaddr );

    if( connect( sockfd, ( struct sockaddr * ) &saddr, sizeof( saddr ) ) < 0 )
    {
#ifdef SIGTRAN_PORT_WINNT

        if( WSAGetLastError() == WSAEWOULDBLOCK )
#else
        if( errno == EINPROGRESS )
#endif
        {
            return TCPIPLIB_SOCKET_CONNECTING;
        }

        return TCPIPLIB_ERROR;
    }

    return TCPIPLIB_SOCKET_CONNECTED;
}


/* Sets up a server socket....  * the server socket is returned .... */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**              socket_setup_server
**
** DESCRIPTION :
**              Sets up a server socket.The server socket is returned.
** Return Type : int
*************************************************************************/
int socket_setup_server( ulong serv_ipaddress, ushort serv_tcpport )
{
    struct sockaddr_in   saddr;
    int                  sockfd;
#ifdef SIGTRAN_PORT_WINNT
    const char FAR       on = 1;
#else
    int on = 1;
#endif

    saddr.sin_family        = AF_INET;
    saddr.sin_port          = htons( serv_tcpport );
    saddr.sin_addr.s_addr   = htonl( serv_ipaddress );

    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
#ifdef SIGTRAN_PORT_WINNT

    if( sockfd == INVALID_SOCKET )
#else
    if( sockfd < 0 )
#endif
    {
        TCPIPLIB_TRACE( ( "could not open socket\n" ) );
        return TCPIPLIB_ERROR;
    }

    if( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, ( char * )&on, sizeof( on ) )
            < 0 )
    {
        TCPIPLIB_TRACE( ( "could not set SO_REUSEADDR option\n" ) );
        close( sockfd );
        return TCPIPLIB_ERROR;
    }

    if( bind( sockfd, ( struct sockaddr * ) &saddr, sizeof( saddr ) ) != 0 )
    {
        TCPIPLIB_TRACE( ( "Bind failure \n" ) );
        close( sockfd );
        return ( TCPIPLIB_ERROR );
    }

    if( listen( sockfd, 5 ) < 0 )
    {
        TCPIPLIB_TRACE( ( "Err listening \n" ) );
        close( sockfd );
        return ( TCPIPLIB_ERROR );
    }

    return ( sockfd );
}


/* Accept connections on the server socket....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**              socket_accept_connections
**
** DESCRIPTION :
**              Accept connections on the server socket.
** Return Type : int
*************************************************************************/
int socket_accept_connections( int sock )
{
    int    newsock = 0, fromlen = 0;
    struct sockaddr_in from;
    struct linger       lg;

    fromlen = sizeof( from );

    if( ( newsock = accept( sock, ( struct sockaddr * )&from, &fromlen ) ) < 0 )
    {
        TCPIPLIB_TRACE( ( "Accept Failure err \n" ) );
        return ( TCPIPLIB_ERROR );
    }

    lg.l_onoff  = TCPIPLIB_TRUE;
    lg.l_linger = 5;

    if( setsockopt( newsock, SOL_SOCKET, SO_LINGER, ( char * )&lg, sizeof( struct linger ) ) < 0 )
    {
        TCPIPLIB_TRACE( ( "setsockopt(SO_LINGER) Failure\n" ) );
        close( newsock );
        return ( TCPIPLIB_ERROR );
    }

    return newsock;
}

/* set the socket nonblocking socket....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**              socket_set_nonblocking
**
** DESCRIPTION :
**              set the socket nonblocking socket
** Return Type : int
*************************************************************************/

int socket_set_nonblocking( int sock, int flag )
{
#ifdef SIGTRAN_PORT_WINNT
    u_long FAR on = 1;
#else
    char  on = 1;
#endif

    if( flag > 0 )
    {
        on = 1;
    }

    else
    {
        on = 0;
    }

#ifdef SIGTRAN_PORT_WINNT

    if( ioctlsocket( sock, FIONBIO, &on ) )
#elif defined (SIGTRAN_PORT_VXWORKS) || defined (SIGTRAN_PORT_VX_PNE)
    if( ioctl( sock, FIONBIO, on ) < ERROR )
#else
    if( ioctl( sock, FIONBIO, &on ) < 0 )
#endif
    {
        TCPIPLIB_TRACE( ( "ioctlsocket(FIONBIO) Failure\n" ) );
        return TCPIPLIB_ERROR;
    }

    return sock;
}

/* Read Data into the buffer from the socket.  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**              socket_read
**
** DESCRIPTION :
**              Read Data into the buffer from the socket.
** Return Type : int
*************************************************************************/

int socket_read( int sockfd, char *ptr, int nbytes )
{
    int nread = 0, ntotal = 0;
#ifndef SIGTRAN_PORT_WINNT
    int flag = 0 | MSG_WAITALL;
#else
    int flag = 0;
#endif

    while( 1 )
    {
        errno = 0;

        if( ntotal >= nbytes )
        {
            break;
        }

        nread = recv( sockfd, ptr, nbytes - ntotal, flag );
        ntotal += nread;
        ptr += nread;
    }

    TCPIPLIB_TRACE( ( "\n TCP SOCK::received bytes =%d\n", ntotal ) );
    return ( ntotal );
}


/* Write Data to the socket.....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**              socket_write
**
** DESCRIPTION :
**              Write Data to the socket.
** Return Type : int
*************************************************************************/

int socket_write( int sockfd, char *ptr, int nbytes )
{
    int nleft = 0,
        nwritten = 0;

    nleft = nbytes;

    while( nleft > 0 )
    {
#ifdef SIGTRAN_PORT_WINNT
        nwritten = send( sockfd, ptr, nleft, 0 );
#else
        nwritten = write( sockfd, ptr, nleft );
#endif

        if( nwritten < 0 )
        {
            return TCPIPLIB_ERROR;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return ( nbytes - nleft );
}


/* Create a SOCK_DGRAM....  * set options for the socket....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**               udpsocket_create
**
** DESCRIPTION :
**              Create a SOCK_DGRAM.Set options for the socket
** Return Type : int
*************************************************************************/

int udpsocket_create( void )
{
    int sockfd;
#ifdef SIGTRAN_PORT_WINNT
    ulong FAR on = 1;
#else
    const char on = 1;
#endif

    sockfd = socket( AF_INET, SOCK_DGRAM, 0 );

#ifdef SIGTRAN_PORT_WINNT

    if( sockfd == INVALID_SOCKET )
#else
    if( sockfd < 0 )
#endif
        return TCPIPLIB_ERROR;

#ifdef SIGTRAN_PORT_WINNT

    if( ioctlsocket( sockfd, FIONBIO, &on ) )
#elif defined(SIGTRAN_PORT_VXWORKS) || defined (SIGTRAN_PORT_VX_PNE)
    if( ioctl( sockfd, FIONBIO, on ) == ERROR )
#else
    if( ioctl( sockfd, FIONBIO, &on ) < 0 )
#endif
    {
        close( sockfd );
        return ( TCPIPLIB_ERROR );
    }

    return sockfd;

}


/* Set up a UDP socket server.....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**               udpsocket_setup_server
**
** DESCRIPTION :
**              Set up a UDP socket server
** Return Type : int
*************************************************************************/

int udpsocket_setup_server( ushort serv_port )
{
    int                  sock;
    struct sockaddr_in   saddr;
    int  on = 1;

    saddr.sin_family = AF_INET;
    saddr.sin_port   = htons( serv_port );
    saddr.sin_addr.s_addr = htonl( INADDR_ANY );

    sock = socket( AF_INET, SOCK_DGRAM, 0 );

#ifdef SIGTRAN_PORT_WINNT

    if( sock == INVALID_SOCKET )
#else
    if( sock < 0 )
#endif
    {
        TCPIPLIB_TRACE( ( "could not open socket\n" ) );
        return TCPIPLIB_ERROR;
    }

#ifdef SIGTRAN_PORT_WINNT

    if( ioctlsocket( sock, FIONBIO, &on ) )
#elif defined(SIGTRAN_PORT_VXWORKS) || defined (SIGTRAN_PORT_VX_PNE)
    if( ioctl( sock, FIONBIO, ( int )&on ) < 0 )
#else
    if( ioctl( sock, FIONBIO, &on ) < 0 )
#endif
    {
        close( sock );
        return ( TCPIPLIB_ERROR );
    }

    if( bind( sock, ( struct sockaddr * ) &saddr, sizeof( saddr ) ) < 0 )
    {
        close( sock );
        return TCPIPLIB_ERROR;
    }

    return sock;
}


/* Write to a SOCK_DGRAM socket.....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**               udpsocket_write
**
** DESCRIPTION :
**               Write to a SOCK_DGRAM socket
** Return Type : int
*************************************************************************/

int udpsocket_write( int sockfd, char *strip, ushort serv_port, char *pbuf, int len )
{
    struct sockaddr_in      saddr;
    ulong                   serv_ipaddr;

    memset( &saddr, 0, sizeof( struct sockaddr_in ) );

    if( ( serv_ipaddr = inet_addr( strip ) ) == -1 )
    {
        return TCPIPLIB_ERROR;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port   = htons( serv_port );
    memcpy( ( char * )&saddr.sin_addr.s_addr, ( char * )&serv_ipaddr, sizeof( long ) );

    if( sendto( sockfd, pbuf, len, 0, ( struct sockaddr * )&saddr,
                sizeof( struct sockaddr_in ) ) < 0 )
    {
        return TCPIPLIB_ERROR;
    }

    return TCPIPLIB_SUCCESS;
}

/* Send to a SOCK_DGRAM socket.....  */
#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**               udpsocket_send
**
** DESCRIPTION :
**               Send to a SOCK_DGRAM socket
** Return Type : int
*************************************************************************/

int udpsocket_send( int sockfd, ulong ipaddr, ushort serv_port, char *pbuf, int len )
{
    struct sockaddr_in      saddr;

    memset( &saddr, 0, sizeof( struct sockaddr_in ) );

    if( ipaddr == -1 )
    {
        return TCPIPLIB_ERROR;
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port   = htons( serv_port );
    saddr.sin_addr.s_addr = htonl( ipaddr );

    if( sendto( sockfd, pbuf, len, 0, ( struct sockaddr * )&saddr,
                sizeof( struct sockaddr_in ) ) < 0 )
    {
        return TCPIPLIB_ERROR;
    }

    return TCPIPLIB_SUCCESS;
}

#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**               udpsocket_get
**
** DESCRIPTION :
**               Get from a SOCK_DGRAM socket
** Return Type : int
*************************************************************************/

int udpsocket_get( int sockfd, ulong *ip_addr,  ushort *serv_port,
                   char *pbuf , int *len )
{
    long int                msgsize;
    struct sockaddr_in      saddr;
    int                     saddrlen = sizeof( struct sockaddr_in );

    if( ( msgsize = recvfrom( sockfd, ( char * )pbuf, *len, 0,
                              ( struct sockaddr * )&saddr, ( int * )&saddrlen ) ) < 0 )
    {
        return TCPIPLIB_ERROR;
    }

    if( ip_addr )
    {
        *ip_addr = ntohl( saddr.sin_addr.s_addr );
    }

    if( serv_port )
    {
        *serv_port = ntohs( saddr.sin_port );
    }

    *len = msgsize;

    return TCPIPLIB_SUCCESS;
}

#ifdef SIGTRAN_PORT_WINNT
__declspec( dllexport )
#endif
/************************************************************************
** FUNCTION    :
**               udpsocket_read
**
** DESCRIPTION :
**               Read from socket datagram socket.
**
** Return Type : int
*************************************************************************/

int udpsocket_read(
    int             sockfd,
    ulong           *ip_addr,
    ushort          *serv_port,
    char            *pbuf,
    int              *len )
{
    long int                msgsize;
    struct sockaddr_in      saddr;
    int                     saddrlen = sizeof( struct sockaddr_in );

    if( ( msgsize = recvfrom( sockfd, ( char * )pbuf, *len, 0,
                              ( struct sockaddr * )&saddr, ( int * )&saddrlen ) ) < 0 )
    {
        return TCPIPLIB_ERROR;
    }

    if( ip_addr )
    {
        *ip_addr = ntohl( saddr.sin_addr.s_addr );
    }

    if( serv_port )
    {
        *serv_port = ntohs( saddr.sin_port );
    }

    *len = msgsize;

    return TCPIPLIB_SUCCESS;
}

/* WINDOWS SPECIFIC FUNCTIONS  */

#ifdef SIGTRAN_PORT_WINNT
/* This function starts up the WinSock interface for this process */
/************************************************************************
** FUNCTION    :
**               WsaStartup
**
** DESCRIPTION :
**               This function starts up the WinSock interface for this
**                 process
**
** Return Type : int
*************************************************************************/

__declspec( dllexport ) int WsaStartup( void )
{
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = 0x0101; // Winsock Version 1.1

    err = WSAStartup( wVersionRequested, &wsaData );

    if( err )
    {
        TCPIPLIB_TRACE( ( "WSAStartup Error\n" ) );
    }

    return err;

}


/* This function cleans up the WinSock interface for this process.  */
/************************************************************************
** FUNCTION    :
**               WsaCleanup
**
** DESCRIPTION :
**               This function cleans up the WinSock interface for this
**                 process.
**
** Return Type : int
*************************************************************************/

__declspec( dllexport ) int WsaCleanup( void )
{
    err = WSACleanup();

    if( err )
    {
        TCPIPLIB_TRACE( ( "WSACleanup Error\n" ) );
    }

    return err;

}

#endif
