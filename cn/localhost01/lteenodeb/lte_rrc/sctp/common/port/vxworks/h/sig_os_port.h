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
 *    13Sept11    Sandeep Singh    SPR-21022       Vxworks-6.8 Porting Changes
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef  __OS_H__
#define __OS_H__

#include    <unistd.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
#include    <types.h>
#else
#include    <sys/types.h>
#endif
/* Changes for SPR-21022 Ends here */
#include    <mbuf.h>
/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
#include    <sys/times.h>
#endif
/* Changes for SPR-21022 Ends here */
#include    <sockLib.h>
#include    <inetLib.h>
#include    <ioLib.h>
#include    <socket.h>
/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
#include    <socketvar.h>
#endif
/* Changes for SPR-21022 Ends here */
#include    <inet.h>
#include    <in.h>
/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
#include    <resolvLib.h>
#endif
/* Changes for SPR-21022 Ends here */
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
#include    <netinet/ip6.h>
/* Changes for SPR-21022 Starts here (IPv6) */
#ifndef VXWORKS_6_9_SUPPORT
#include    <netinet/icmp6.h>
#endif
/* Changes for SPR-21022 Ends here (IPv6) */
#endif

#include    <sys/stat.h>
/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
#include    <sys/times.h>
#endif
/* Changes for SPR-21022 Ends here */
#include    <semaphore.h>
#include    <semLib.h>
#include    <net/uio.h>
#include    <arpa/inet.h>
#include    <netdb.h>
#include    <unistd.h>

#include    <netinet/in.h> /* For sockaddr_in, sockaddr_in6 */
#include    <time.h>
/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
#include    <taskVarLib.h>
#else
extern void bzero( char *, int );
#endif
/* Changes for SPR-21022 Ends here */

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
