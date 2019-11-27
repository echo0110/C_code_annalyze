/*****************************************************************************
 *    FILE NAME:
 *        os.h
 *
 *    DESCRIPTION:
 *       This file shall include all the OS files required for vxworks
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
#include    <types.h>
#include    <mbuf.h>
#include    <times.h>
#include    <sockLib.h>
#include    <inetLib.h>
#include    <ioLib.h>
#include    <socket.h>
#include    <socketvar.h>
#include    <inet.h>
#include    <in.h>
#include    <resolvLib.h>
#include    <stdlib.h>
#include    <taskLib.h>
#include    <hostLib.h>
#include    <sys/types.h>
#include    <sys/socket.h>

#include    <netinet/in.h>
#include    <netinet/in_systm.h>
#include    <netinet/ip.h>
#include    <netinet/ip_icmp.h>
#include    <netinet/udp.h>

#ifdef SIGTRAN_INET6_SUPPORT
#include    <netinet6/in6.h>
#include    <netinet6/ip6.h>
#include    <netinet6/icmp6.h>
#endif

#include    <sys/stat.h>
#include    <semaphore.h>
#include    <semLib.h>
#include    <net/uio.h>
#include    <arpa/inet.h>
#include    <netdb.h>
#include    <unistd.h>

#include    <netinet/in.h> /* For sockaddr_in, sockaddr_in6 */
#include    <time.h>
#include    <taskVarLib.h>
#include    <cspl.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct timespec  sigtran_gettime_t;
    typedef  fd_set sig_fd_set;
    typedef  unsigned short int sig_sock_fd_t;

#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP    132
#endif
    extern long        random( void );
#include <tickLib.h>
    extern  int     sysClkRateGet( void );
#ifndef SYS_CLKRATE
#define SYS_CLKRATE sysClkRateGet()
#endif
#ifdef __cplusplus
}
#endif
#endif

