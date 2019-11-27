/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMisc.h,v 1.1.1.1.16.1 2010/10/25 10:39:21 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Header file for misc wrapper APIs.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMisc.h,v $
 * Revision 1.1.1.1.16.1  2010/10/25 10:39:21  gur23971
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.10.1  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.5  2009/05/29 02:36:22  gur19479
 * updated for memmove
 *
 * Revision 1.4  2009/05/06 05:38:47  gur20548
 * pdcp common merged with lte common
 *
 * Revision 1.1  2009/05/04 14:52:04  gur20548
 * pdcp files added
 *
 * Revision 1.3  2008/10/22 12:41:29  gur11912
 * fixed for performance changes
 *
 * Revision 1.2  2008/09/18 11:08:05  gur19836
 * typedef moved from lteLayer2Types.h
 *
 * Revision 1.1  2008/09/18 05:23:37  gur19836
 * Review comments incorporated
 *
 *
 ****************************************************************************/

#ifndef _LTE_MISC_H_
#define _LTE_MISC_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/


#include "lteTypes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/syscall.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sched.h>
#include <getopt.h>
#include <netdb.h>
#include <execinfo.h>
#include <mqueue.h>
#include <byteswap.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifdef WIN32
#include <winsock.h>
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct sockaddr_in SockAddr;
typedef struct timespec TimeSpec;
typedef struct itimerval iTimerVal;
typedef struct hostent Hostent;

/* typedef struct option Option ;
typedef struct sigaction SigAction;
typedef struct in_addr in_Addr;
*/

typedef struct timeval timeVal;
typedef struct timezone timeZone;
typedef struct sockaddr_in6 sockAddrIn6;
typedef  fd_set fd_Set;
typedef  sigset_t sigSet_t;
typedef  eventfd_t eventFD_t;
typedef struct rlimit 	rLimit;
typedef  struct shmid_ds  Shmid_Ds;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


/****************************************************************************
 *  function(): Description.
 *      This API copies string from src to dest.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
Char8   *strNCpy( Char8 *dest, const Char8 *src, size_t n );


/****************************************************************************
 *  function(): Description.
 *      This API compares two strings.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32  strNCmp( const Char8 *s1, const Char8 *s2, size_t n );
SInt32  strCmp( const Char8 *s1, const Char8 *s2);


/****************************************************************************
 * Function Name  : memCmp 
 * Inputs         : dest - destination where memory is copied.
 *                  src - source from where it is copied.
 *                  n - number of bits to be copied.
 * Outputs        : None.
 * Returns        : -1/0/1 - result of cmp
 * Variables      : None.
 * Description    : Compares memory areas a and b of size n
 ****************************************************************************/
SInt32    memCmp( const void *a, const void *b, size_t n );


/****************************************************************************
 *  function(): Description.
 *      This API copies the memory from src to dest.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    *memCpy( void *dest, const void *src, size_t n );


/****************************************************************************
 *  function(): Description.
 *      This API sets the memory.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    *memSet( void *s, SInt32 c, size_t n );


/****************************************************************************
 *  function(): Description.
 *      This API sets the memory.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

void  *memMove(void *d, void *s, size_t n);

SInt32 raise_wrapper(SInt32 sig);
__sighandler_t signal_wrapper (SInt32 signum, void (*handler)(int));
Char8 *strCpy(Char8 *dest, const Char8 *src);
UDouble32 ceilf_wrapper(UDouble32 x);
void uSleep(UInt32 uSec);
Char8 *strCat(Char8 *dest, const Char8 *src);
Char8 *strnCat(Char8 *dest, const Char8 *src, UInt32 n);
Char8 *strTok(Char8 *str, const Char8 *delim);
void abort_wrapper(void);
UDouble64  log10_wrapper(UDouble64 x);
UDouble64  log_wrapper(UDouble64 x);
UDouble64 ceil_wrapper(UDouble64 x);
UDouble64 floor_wrapper(UDouble64 x);
SInt32 abs_wrapper(SInt32 j);
SInt32 ffs_wrapper(SInt32 j);
void bzero_wrapper(void *s, size_t n);
SInt32 nanoSleep(const TimeSpec *req, TimeSpec *rem);
SInt32 Syscall_SYS_gettid(void);
SInt32 pow_wrapper(double x, double y);
SInt32 getrlimit_wrapper(SInt32 resource, rLimit *rlim);
SInt32 sched_setaffinity_wrapper(pid_t pid, size_t cpusetsize,cpu_set_t *mask);
SInt32 setrlimit_wrapper(SInt32 resource, const rLimit *rlim);
void clock_gettime_real_time(TimeSpec *ts_p);
void clock_gettime_thread_cputime_id(TimeSpec *ts_p);
void pError (Char8 *str);
SInt32 pselect_wrapper(SInt32 nfds, fd_Set *readfds, fd_Set *writefds, fd_Set *exceptfds, const TimeSpec *timeout,const sigSet_t *sigmask);

UInt32 setRlimitOfCore(void);
void setRlimitOfMsgQueue(void);
SInt32 setsockopt_wrapper(SInt32 sockfd, SInt32 level, SInt32 optname,const void *optval, SInt32 optlen);
SInt32 eventfd_initialize_wrapper(UInt32 initVal,SInt32 flag);
SInt32 eventfd_read_wrapper(SInt32 fd, eventFD_t *value);
SInt32 setitimer_wrapper(SInt32 which, const iTimerVal *new_value,iTimerVal *old_value);
SInt32 Inet_aton_wrapper(const Char8 *cp, void *inp);
/* SPR 6949 Fix Start */
SInt32 recvfrom_wrapper(SInt32 sockFD, void * msgBuf, UInt32 msg_len, SInt32 flag,SockAddr*,SInt32 );
/* SPR 6949 Fix End */
SInt32 fflush_wrapper(FILE *stream);
SInt32 mq_unlink_wrapper(const char *name);
SInt32 shmget_wrapper(UInt32 key, size_t size, SInt32 shmflg);
SInt32 select_wrapper(SInt32 nfds, fd_Set *readfds, fd_Set *writefds,fd_Set *exceptfds, timeVal *timeout);
void *shmat_wrapper(SInt32 shmid, const void *shmaddr, SInt32 shmflg);
SInt32 pthread_mutex_lock_wrapper(pthread_mutex_t *mutex);
SInt32 shmctl(int shmid, int cmd, Shmid_Ds *buf);
SInt32 pthread_cond_signal_wrapper(pthread_cond_t *cond);
SInt32 mlockall_wrapper(SInt32 flags);
void *malloc_wrapper(size_t size);
SInt32 scanf_wrapper(const char *format);
SInt32 open2_wrapper(const char *pathname, SInt32 flags);
SInt32 open3_wrapper(const char *pathname,SInt32 flags, mode_t mode);
SInt32 eventfd_write_wrapper(SInt32 fd, eventFD_t value);
SInt32 fcntl2_wrapper(SInt32 fd, SInt32 cmd);
SInt32 fcntl3_wrapper(SInt32 fd, SInt32 cmd1,SInt32 cmd2);
SInt32 bind_wrapper(SInt32 sockfd, const SockAddr *addr, SInt32 addrlen);
/* SPR 20636 Changes Start*/
SInt32 sendto_wrapper(SInt32 sockfd, const void *buf, size_t len, SInt32 flags,const  SockAddr *dest_addr, socklen_t addrlen);
/* SPR 20636 Changes End*/
int shm_open_wrapper(const char *name, int oflag, int mode);
int mq_open_wrapper(const char *name, int oflag, int mode, struct mq_attr *attr);

SInt32 sendto_wrapper_addr6(SInt32 sockfd, const void *buf, UInt32 len, SInt32 flags,const  sockAddrIn6 *dest_addr, SInt32 addrlen);
SInt32 socket_wrapper(SInt32 domain, SInt32 type, SInt32 protocol);
in_addr_t inet_addr_wrapper(const char *cp);
SInt32 backtrace_wrapper(void **buffer,SInt32 size);
Hostent  *gethostbyname_wrapper(const Char8 *name);
SInt32 close_wrapper(SInt32 fd);

SInt32 gettimeofday_wrapper(timeVal *tv,  timeZone *tz);
SInt32 getpid_wrapper(void);
/*Coverity 26298 Fix Start*/
void  convertIPAddr(Char8 *socketInf_p, UInt8 * AddrArray_p, UInt32 size);
/*Coverity 26298 Fix End*/
SInt32 shmctl_wrapper_ipc_rmid(SInt32 shmid);
void lteFD_CLR(SInt32 fd, fd_Set *set);
       
SInt32 fClose(FILE *fd);
SInt32 lteFD_ISSET(SInt32 fd, fd_Set *set);
       
void lteFD_SET(SInt32 fd, fd_Set *set);

void lteFD_ZERO(fd_Set *set);

SInt32 inet_pton_wrapper (SInt32 domain, const Char8 * peerTgInfV6, void *inp);
UInt16 htons_wrapper (UInt16 hostshort);
UInt32 htonl_wrapper(UInt32 hostlong);
UInt32 ntohl_wrapper(UInt32 netshort);
UInt16 ntohs_wrapper(UInt16 netshort);
SInt32 bind_wrapper_addr6(SInt32 sockfd, const sockAddrIn6 *addr, SInt32 addrlen);
size_t strlen_wrapper(const Char8 *s);
SInt32 snprintf_wrapper(Char8 *s, size_t n, const Char8  *format, ...);
SInt32 printf_wrapper(const Char8 *fmt, ...);

/* compilation fix for MAC_PHY_ENDIANESS */
UInt16 bswap16Wrap(UInt16 x);
UInt32 bswap32Wrap(UInt32 x);

/* + Layer2 NON CA Changes */
/*SPR 21653 changes start*/
#define INTERFACE_API_HEADER_LEN    16
/*SPR 21653 changes end*/
/* - Layer2 NON CA Changes */

#define API_HEADER_TRANSID_OFFSET   0
#define API_HEADER_SOURCEID_OFFSET  2
#define API_HEADER_DESTID_OFFSET    4
#define API_HEADER_MSGID_OFFSET     6
#define API_HEADER_MSGLEN_OFFSET    8
#define API_HEADER_CELL_ID_NIL      0

/* + Layer2 NON CA Changes */
#define API_HEADER_CELL_INDEX_OFFSET 10

#define API_RRC_HEADER_RESERVE_1BYTE_OFFSET   11
#define API_RRC_HEADER_RESERVE_4BYTE_OFFSET   12


#define API_HEADER_PADDING_OFFSET   10
#define API_RRC_HEADER_CELL_INDEX_OFFSET   10
#define API_RRM_HEADER_CELL_INDEX_OFFSET   10
#define API_OAM_HEADER_CELL_INDEX_OFFSET   10

#define API_HEADER_API_ID_OFFSET    6

/* - Layer2 NON CA Changes */

#define CELL_INDEX_OFFSET_INR(msg_p)   msg_p += 1

/* +COVERITY 32166 to 32174 */
/* Porting Start*/

    /* SPR 20430 Changes Start */
    /* SPR 20636 Changes Start */
#define BOOL_COMPARE_AND_SWAP(a,b,c) __sync_bool_compare_and_swap((SInt32 *)(a),(SInt32)(b),(SInt32)(c))
#ifdef __x86_64__
#define BOOL_COMPARE_AND_SWAP_ADDR(a,b,c) __sync_bool_compare_and_swap((SInt64 *)(a),(SInt64)(b),(SInt64)(c))
#else
#define BOOL_COMPARE_AND_SWAP_ADDR(a,b,c) __sync_bool_compare_and_swap((SInt32 *)(a),(SInt32)(b),(SInt32)(c))
#endif
    /* SPR 20636 Changes End */
    /* SPR 20430 Changes End */
/* Rel 5.3: Coverity 54147 and 54148 Fix Start */
#define FETCH_AND_ADD(a,b) __sync_fetch_and_add((SInt32 *)(a),(SInt32)(b))
/* Rel 5.3: Coverity 54147 and 54148 Fix End */
#define FETCH_AND_SUB(a,b) __sync_fetch_and_sub((SInt32 *)(a),(SInt32)(b))
#define SUB_AND_FETCH(a,b) __sync_sub_and_fetch((SInt32 *)(a),(SInt32)(b))
#define ADD_AND_FETCH(a,b) __sync_add_and_fetch((SInt32 *)(a),(SInt32)(b))
/* +COVERITY 32166 to 32174 */

/* Porting End*/
#define IPV4_FLAG 1
#define IPV6_FLAG 2
#define IPV6_ADDR_SIZE 16
#define IPV4_ADDR_SIZE 4

typedef struct tddOamRrcReceiveInfoT
{
    UInt8 *macRecvBuf ;
    UInt8 *rlcRecvBuf ;
    UInt8 *pdcpRecvBuf ;
    UInt8 *relayRecvBuf;

    SInt32 macBytesRead;
    SInt32 rlcBytesRead;
    SInt32 pdcpBytesRead;
    SInt32 relayBytesRead;

}tddOamRrcReceiveInfo;
 
typedef struct HeaderInfoT
{
    UInt8 srcIp[IPV6_ADDR_SIZE];
    UInt8 destIp[IPV6_ADDR_SIZE];
}HeaderInfo;
 

SInt16 recvMsg_wrapper( SInt32 fd,UInt8 *msgbuf,UInt16 msglen,HeaderInfo *hdrInfo,UInt8 ipType);


#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
#define RDTSC_MAX ULLONG_MAX
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
}
#elif defined(__x86_64__)

static __inline__ unsigned long long rdtsc(void)
{
#define RDTSC_MAX ULLONG_MAX
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#elif defined(__powerpc__)

static __inline__ unsigned long long rdtsc(void)
{
#define RDTSC_MAX UINT_MAX
  unsigned long long int result=0;
  unsigned long int tempres=0;
  asm volatile ("mfspr %0,526" : "=r"(tempres));
   result = tempres;
  return(result);
}
#endif

#endif
