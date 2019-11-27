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
 *    1May13      Pranav Sharma      SPR 21107     CSR 44034
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef  __OS_H__
#define __OS_H__

#include    <sys/types.h>
#include    <pthread.h>
#include    <sys/socket.h>

#include    <netinet/in.h>
#include    <netinet/in_systm.h>
#include    <netinet/ip.h>
#include    <netinet/ip_icmp.h>
#include    <netinet/udp.h>
#include    <time.h>

#include    <sys/time.h>
#include    <sys/stat.h>
#include    <sys/ipc.h>
#include    <sys/msg.h>
#include    <sys/sem.h>
#include    <sys/uio.h>
#include    <sys/ioctl.h>
#include    <arpa/inet.h>
#include    <arpa/telnet.h>
#include    <netdb.h>
#include    <unistd.h>

#include    <netinet/in.h> /* For sockaddr_in, sockaddr_in6 */

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <stdarg.h>

#include    <limits.h>
#include    <fcntl.h>
#include    <sys/signal.h>
/* SPR-22189 START */
//#include    <stropts.h>
/* SPR-22189 END */
#include    <poll.h>


#include    <sys/times.h>
#include    <sys/select.h>
#include    <sys/stat.h>
#include    <sys/un.h>
#include    <errno.h>

#include    <semaphore.h>
#include    <sys/sem.h>
#include    <signal.h>

#ifdef SIGTRAN_INET6_SUPPORT
#include    <netinet/ip6.h>
#include    <netinet/icmp6.h>
#endif

#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP    132
#endif

/* SPR 21107 changes start */
#define COMMON_UNUSED_VARIABLE(a) a=a
/* SPR 21107 changes end */

#ifdef __cplusplus
extern "C"
{
#endif
    typedef  fd_set sig_fd_set;
    typedef  unsigned short int sig_sock_fd_t;
    typedef struct timespec sigtran_gettime_t;
#ifdef __cplusplus
}
#endif
#endif
