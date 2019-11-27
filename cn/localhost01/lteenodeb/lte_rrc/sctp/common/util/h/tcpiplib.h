/*****************************************************************************
 *    FILE NAME:
 *        tcpiplib.h
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

#ifndef _TCPIPLIB_H
#define _TCPIPLIB_H


#ifdef SIGTRAN_PORT_WINNT
#include <winsock2.h>
#define ulong    ULONG
#define ushort   USHORT
#define close    closesocket
#endif

#include <sys/types.h>

#if defined (SIGTRAN_PORT_VXWORKS) ||defined (SIGTRAN_PORT_VX_PNE)
#include <types\vxTypes.h>
#include <ioLib.h>
#include <in.h>
#include <inetLib.h>

#define ulong  unsigned long int
#define ushort unsigned short int
#endif

#ifdef SIGTRAN_PORT_LINUX
#include "string.h" /* Required for memset and memcpy */
#endif

#define TCPIPLIB_TRUE                   1
#define TCPIPLIB_FALSE                  0
#define TCPIPLIB_SOCKET_CONNECTING      1
#define TCPIPLIB_SOCKET_CONNECTED       2
#define TCPIPLIB_SOCKET_CANT_CONNECT    -1
#define TCPIPLIB_SUCCESS                1
#define TCPIPLIB_FAILURE                0
#define TCPIPLIB_ERROR                  -1

#ifdef SIGTRAN_DEBUG
#ifndef TCPIPLIB_TRACE
#define TCPIPLIB_TRACE(x) printf x
#endif
#else
#define TCPIPLIB_TRACE(x)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

    /* Error code */
    int err;

    /*
     * These functions are required to initialize the winsock function.....
     */
#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport ) int WsaStartup( void );
    __declspec( dllexport ) int WsaCleanup( void );
#endif

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_create( void );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_close( int sockfd );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_setup_client( int sockfd, ulong serv_ipaddr,
                             ushort serv_tcpport );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_setup_server( ulong serv_ipaddress, ushort serv_tcpport );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_accept_connections( int sock );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_set_nonblocking( int sock, int flag );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_read( int sockfd, char *ptr, int nbytes );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int socket_write( int sockfd, char *ptr, int nbytes );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int udpsocket_create( void );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int udpsocket_setup_server( ushort serv_tcpport );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int udpsocket_write( int sockfd, char *strip, ushort serv_tcpport, char *pbuf,
                         int len );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int udpsocket_get( int sockfd, ulong *ip_addr,  ushort *serv_tcpport,
                       char *pbuf , int *len );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int udpsocket_send( int sockfd, ulong serv_ipaddr, ushort serv_tcpport,
                        char *pbuf, int len );

#ifdef SIGTRAN_PORT_WINNT
    __declspec( dllexport )
#endif
    int udpsocket_read( int sockfd, ulong *inaddr,  ushort *serv_tcpport, char *pbuf, int
                        *len );

#ifdef __cplusplus
}
#endif


#endif
