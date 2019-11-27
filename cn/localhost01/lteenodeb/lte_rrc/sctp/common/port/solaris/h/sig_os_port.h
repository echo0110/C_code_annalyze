/* all the os related files shall be included from this file only */
/*****************************************************************************
 *    FILE NAME:
 *        os.h
 *
 *    DESCRIPTION:
 *       This file shall include all the OS files required for solaris
 *
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Cspl porting
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef  __OS_H__
#define __OS_H__

#include    <unistd.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <time.h>
#include    <stdarg.h>

#include    <limits.h>
#include    <fcntl.h>
#include    <sys/signal.h>
#include    <stropts.h>
#include    <poll.h>


#include    <sys/types.h>
#include    <netinet/in.h>
#include    <sys/socket.h>
#include    <sys/times.h>
#include    <sys/time.h>
#include    <sys/select.h>
#include    <sys/stat.h>
#include    <sys/uio.h>
#include    <sys/un.h>
#include    <arpa/inet.h>
#include    <sys/ipc.h>
#include    <sys/msg.h>

#include    <netdb.h>
#include    <errno.h>

#include    <netinet/in_systm.h>
#include    <netinet/ip.h>
#include    <netinet/ip_icmp.h>
#include    <netinet/udp.h>

#ifdef SIGTRAN_INET6_SUPPORT
#include    <netinet/ip6.h>
#include    <netinet/icmp6.h>
#endif

#include    <pthread.h>
#include    <semaphore.h>

#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP    132
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    typedef  fd_set sig_fd_set;
    typedef  unsigned short int sig_sock_fd_t;

    char *ctime_r( const time_t *, char *, int );
#ifdef __cplusplus
}
#endif
#endif
