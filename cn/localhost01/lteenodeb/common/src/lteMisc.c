/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMisc.c,v 1.2.16.1 2010/10/25 10:33:55 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper library for providing misc porting functions. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMisc.c,v $
 * Revision 1.2.16.1  2010/10/25 10:33:55  gur23971
 * warning removed
 *
 * Revision 1.2  2010/03/11 07:32:17  gur18550
 * HigherMimoConfig
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.10.1  2009/06/09 11:24:51  gur19140
 * first wave of Integration
 *
 * Revision 1.4  2009/05/29 02:36:35  gur19479
 * updated for memmove
 *
 * Revision 1.3  2009/05/06 05:39:24  gur20548
 * pdcp common merged with lte common
 *
 * Revision 1.1  2009/05/04 14:52:04  gur20548
 * pdcp files added
 *
 * Revision 1.2  2008/10/22 12:42:39  gur11912
 * fixed for performance changes
 *
 * Revision 1.1  2008/09/18 05:23:38  gur19836
 * Review comments incorporated
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMisc.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
UInt8 higherMimoConfigurationFlag = 0;
#ifdef MAC_AUT_TEST
UInt8 *macPhyBuff_g;
UInt32 msgLen_g;
UInt8 macAutFlag = 0;
Char8 *macRRMBuff_gp;
#endif
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : strNCpy 
 * Inputs         : dest - string destination where string is copied.
 *                  src - source string from where it is copied.
 *                  n - number of bits to be copied.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : This function copies from src string to dest string.
 ****************************************************************************/
Char8 *strNCpy( Char8 *dest, const Char8 *src, size_t n )
{
    return strncpy(dest, src, n);
}

/****************************************************************************
 * Function Name  : strCmp 
 * Inputs         : s1 - String to be compared.
 *                  s2 - String to be compared.
 *                  n - number of bits to be compared.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : Compares two strings.
 ****************************************************************************/
SInt32 strCmp( const Char8 *s1, const Char8 *s2)
{
    return strcmp(s1, s2);
}

/****************************************************************************
 * Function Name  : strNCmp 
 * Inputs         : s1 - String to be compared.
 *                  s2 - String to be compared.
 *                  n - number of bits to be compared.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : Compares two strings.
 ****************************************************************************/
SInt32 strNCmp( const Char8 *s1, const Char8 *s2, size_t n )
{
    return strncmp(s1, s2, n);
}

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
SInt32 memCmp( const void *a, const void *b, size_t n )
{
    return memcmp(a, b, n);
}

/****************************************************************************
 * Function Name  : memCpy 
 * Inputs         : dest - destination where memory is copied.
 *                  src - source from where it is copied.
 *                  n - number of bits to be copied.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Copies memory from src to dest.
 ****************************************************************************/
void *memCpy( void *dest, const void *src, size_t n )
{
    return memcpy(dest, src, n);
}

/****************************************************************************
 * Function Name  : memSet  
 * Inputs         : s - memory to be filled.
 *                  c - character with which memory is to be filled.
 *                  n - number of bytes to be set.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Sets the memory with the specified data.
 ****************************************************************************/
void *memSet( void *s, SInt32 c, size_t n )
{
    return memset(s, c, n);
}
/****************************************************************************
 * Function Name  : memMove
 * Inputs         : d - destination memory to be filled.
 *                  s - source memory.
 *                  n - number of bytes to be fill.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : copies the memory area.
 ****************************************************************************/

void *memMove(void *d, void *s, size_t n)
{
    return memmove(d,s,n);
}

inline SInt32 setsockopt_wrapper(SInt32 sockfd, SInt32 level, SInt32 optname,const void *optval, SInt32 optlen)
{
    return setsockopt(sockfd,level,optname,optval,(socklen_t)optlen);
}

inline SInt32 eventfd_initialize_wrapper(UInt32 initVal,SInt32 flag)
{
    return eventfd(initVal,flag); 
}

inline SInt32 eventfd_read_wrapper(SInt32 fd, eventFD_t *value)
{
#ifdef MAC_AUT_TEST
    if (macAutFlag)
        return 1;
    else
#endif
        return eventfd_read(fd, (eventFD_t* )value);
}

inline SInt32 eventfd_write_wrapper(SInt32 fd, eventFD_t value)
{
    return eventfd_write (fd, (eventFD_t) value  );
}

inline SInt32 raise_wrapper(SInt32 sig)
{
    return raise(sig);
}

inline __sighandler_t signal_wrapper (SInt32 signum, void (*handler)(SInt32))
{
    return signal(signum,handler);
}


inline SInt32 setitimer_wrapper(SInt32 which, const iTimerVal *new_value,iTimerVal *old_value)
{
    /* + coverity 32180 */
    return setitimer((__itimer_which_t) which,new_value,old_value);
    /* - coverity 32180 */
}


SInt32 Inet_aton_wrapper(const Char8 *cp, void *inp)
{
    return inet_aton(cp,(struct in_addr *)inp);
}


SInt32 fflush_wrapper(FILE *stream)
{
    return fflush(stream);
}

SInt32 shmget_wrapper(UInt32 key, size_t size, SInt32 shmflg)
{
    return shmget(key,size,shmflg);
}

/*SPR 6949 Fix Start */
SInt32 recvfrom_wrapper(SInt32 sockFD, void * msgBuf, UInt32 msg_len, SInt32 flag,SockAddr *sockAddr_p,SInt32 addrlen )
    /*SPR 6949 Fix End */
{
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
        memcpy(msgBuf,macPhyBuff_g,msg_len);
        return msg_len;
    }
    else
#endif
    {
        SockAddr rrc_addr;
        SockAddr *localSockAddr_p = sockAddr_p;
        UInt32 addr_len   = addrlen;
        if(PNULL == sockAddr_p)
        {  
            localSockAddr_p = &rrc_addr;
            addr_len   = sizeof(rrc_addr);
        }
        return  recvfrom(sockFD, msgBuf, msg_len,flag,(SockAddr*)localSockAddr_p, &addr_len);
    } 
}
/* SPR 20430 Changes start*/
SInt32 sendto_wrapper(SInt32 sockfd, const void *buf, size_t len, SInt32 flags,const  SockAddr *dest_addr, socklen_t addrlen)
/* SPR 20430 Changes end*/
{

#ifdef LINUX_PC_TEST
        SEND_TO_TEST(0,(char *)buf,len);
        return len;
#endif


#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
        macPhyBuff_g = (UInt8 *)malloc(len+1);
        memcpy(macPhyBuff_g,buf,len);
        msgLen_g = len;
        return msgLen_g;
    }
    else
#endif
    {
        return sendto(sockfd,buf, len, flags,(SockAddr *)dest_addr,addrlen);
    }
}

SInt16 recvMsg_wrapper( SInt32 fd,UInt8 *msgbuf,UInt16 msglen,HeaderInfo *hdrInfo,UInt8 ipType)
{

    struct msghdr msg;
    struct iovec iov;
    struct cmsghdr *cmsg = NULL;
    SockAddr addr;
    sockAddrIn6 addr1;
    Char8 ctrlbuf[100];
    SInt16 bytesRead;
    memSet(&msg, 0, sizeof(msg));
    memSet(&iov, 0, sizeof(iov));
    if(ipType == IPV4_FLAG) {
        msg.msg_name =(struct sockaddr *)&addr;
        msg.msg_namelen = sizeof(addr);
    } else {
        msg.msg_name =(struct sockaddr *)&addr1;
        msg.msg_namelen = sizeof(addr1);
    }
    iov.iov_base = msgbuf;
    iov.iov_len = msglen;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = ctrlbuf;
    msg.msg_controllen = 100;
    msg.msg_flags = 0;

    if ((bytesRead=recvmsg(fd,&msg,0)) < 0) {
        perror("recvmsg");
        exit(1);
    }
    if(PNULL != hdrInfo) {
        cmsg= CMSG_FIRSTHDR(&msg);
        memSet(hdrInfo->destIp,0,IPV6_ADDR_SIZE);
        memSet(hdrInfo->srcIp,0,IPV6_ADDR_SIZE);
        for (cmsg= CMSG_FIRSTHDR(&msg);cmsg != NULL;cmsg = CMSG_NXTHDR(&msg, cmsg)) {
            if(ipType == IPV4_FLAG) {
                // ignore the control headers that don't match what we want
                if (cmsg->cmsg_type != IP_PKTINFO && cmsg->cmsg_level != IPPROTO_IP)
                    continue;

                struct in_pktinfo *pi =(struct in_pktinfo*) CMSG_DATA(cmsg);
                // at this point, peeraddr is the source sockaddr
                // pi->ipi_spec_dst is the destination in_addr
                // pi->ipi_addr is the receiving interface in_addr
                memCpy(hdrInfo->destIp,(UInt8 *)&(pi->ipi_addr),IPV4_ADDR_SIZE);
                memCpy(hdrInfo->srcIp,(UInt8 *)&addr.sin_addr.s_addr,IPV4_ADDR_SIZE);
			} else {
                /************GET DESTINATION ADDRESS +*****************************/
                // ignore the control headers that don't match what we want
                if (cmsg->cmsg_type != IPV6_RECVPKTINFO && cmsg->cmsg_level != IPPROTO_IPV6)
                    continue;

                struct in6_pktinfo *pi =(struct in6_pktinfo*) CMSG_DATA(cmsg);
                // at this point, peeraddr is the source sockaddr
                // pi->ipi_spec_dst is the destination in_addr
                // pi->ipi_addr is the receiving interface in_addr
                /************GET DESTINATION ADDRESS -*****************************/

                memCpy(hdrInfo->destIp,(UInt8 *)&(pi->ipi6_addr),IPV6_ADDR_SIZE);
				memCpy(hdrInfo->srcIp,(UInt8 *)&addr1.sin6_addr.s6_addr,IPV6_ADDR_SIZE);
			}
		}
    }

    return bytesRead;

}

SInt32 sendto_wrapper_addr6(SInt32 sockfd, const void *buf, UInt32 len, SInt32 flags,const  sockAddrIn6 *dest_addr, SInt32 addrlen)
{
    return sendto(sockfd,buf, len, flags,(SockAddr *)dest_addr,addrlen);
}

Char8 *strCpy(Char8 *dest, const Char8 *src)
{
    return strcpy(dest, src);
}

UDouble32 ceilf_wrapper(UDouble32 x)
{
    return ceilf(x);
}

SInt32 abs_wrapper(SInt32 j)
{
    return abs(j);
}

SInt32 ffs_wrapper(SInt32 j)
{
    return ffs(j);
}

void uSleep(UInt32 uSec)
{
    usleep(uSec);
}

Char8 *strCat(Char8 *dest, const Char8 *src)
{
    return  strcat(dest,src);
}

Char8 *strnCat(Char8 *dest, const Char8 *src, UInt32 n)
{
    return  strncat(dest,src,n);
}

Char8 *strTok(Char8 *str, const Char8 *delim)
{
    return strtok(str, delim);
}
SInt32 close_wrapper(SInt32 fd)
{
    return close(fd);
}

SInt32 fClose(FILE *fd)
{
    return fclose(fd);
}

Hostent  *gethostbyname_wrapper(const Char8 *name)
{
    return gethostbyname(name);
}

void abort_wrapper(void)
{
    abort();
}

void clock_gettime_real_time(TimeSpec *ts_p)
{
    clock_gettime(CLOCK_REALTIME,ts_p);
}

void clock_gettime_thread_cputime_id(TimeSpec *ts_p)
{
    clock_gettime(CLOCK_THREAD_CPUTIME_ID,ts_p);
}

UDouble64  log10_wrapper(UDouble64 x)
{
    return log10(x);
}

UDouble64  log_wrapper(UDouble64 x)
{
    return log(x);
}

UDouble64 ceil_wrapper(UDouble64 x)
{
    return ceil(x);
}

UDouble64 floor_wrapper(UDouble64 x)
{
    return floor(x);
}

void bzero_wrapper(void *s, size_t n)
{
    bzero(s,n);
}

SInt32 nanoSleep(const TimeSpec *req, TimeSpec *rem)
{
    return nanosleep(req, rem);
}

SInt32 socket_wrapper(SInt32 domain, SInt32 type, SInt32 protocol)
{
    return socket(domain, type, protocol);
}

SInt32 bind_wrapper(SInt32 sockfd, const SockAddr *addr, SInt32 addrlen)
{
    return  bind( sockfd,(SockAddr*)addr,(socklen_t)addrlen);
}

SInt32 bind_wrapper_addr6(SInt32 sockfd, const sockAddrIn6 *addr, SInt32 addrlen)
{
    return  bind( sockfd,(SockAddr*)addr,(socklen_t)addrlen);
}

SInt32 Syscall_SYS_gettid(void)
{
    return  syscall(SYS_gettid);
}

SInt32 getpid_wrapper(void)
{
    return getpid();
}

SInt32 fcntl2_wrapper(SInt32 fd, SInt32 cmd)
{
    return fcntl(fd,cmd);
}

SInt32 fcntl3_wrapper(SInt32 fd, SInt32 cmd1,SInt32 cmd2)
{
    return fcntl(fd,cmd1,cmd2);
}

SInt32 gettimeofday_wrapper(timeVal *tv,  timeZone *tz)
{
    return  gettimeofday(tv,tz);
}
in_addr_t inet_addr_wrapper(const char *cp)
{
    return inet_addr(cp);
}

SInt32 backtrace_wrapper(void **buffer,SInt32 size)
{
    return backtrace(*buffer,size);
}

SInt32 select_wrapper(SInt32 nfds, fd_Set *readfds, fd_Set *writefds,
        fd_Set *exceptfds, timeVal *timeout)
{
    return  select(nfds,readfds,writefds,exceptfds,timeout);
}

SInt32 pow_wrapper(double x, double y)
{
    return pow(x,y);
}

SInt32 getrlimit_wrapper(SInt32 resource, rLimit *rlim)
{
    /* + coverity 32181 */
    return getrlimit((__rlimit_resource_t) resource,rlim);
    /* - coverity 32181 */
}

SInt32 sched_setaffinity_wrapper(pid_t pid, size_t cpusetsize,cpu_set_t *mask)
{
    return sched_setaffinity(pid,cpusetsize,mask);
}

SInt32 setrlimit_wrapper(SInt32 resource, const rLimit *rlim)
{
    /* + coverity 32182 */
    return setrlimit((__rlimit_resource_t) resource,rlim);
    /* - coverity 32182 */
}

void *shmat_wrapper(SInt32 shmid, const void *shmaddr, SInt32 shmflg)

{
    return shmat(shmid,shmaddr,shmflg);
}

void *malloc_wrapper(size_t size)
{
    return malloc(size);

}

SInt32 shmctl_wrapper_ipc_rmid(SInt32 shmid)
{
    return  shmctl(shmid,IPC_RMID,PNULL);
}

inline SInt32 mlockall_wrapper(SInt32 flags)
{
    return  mlockall(flags);
}
inline SInt32 pselect_wrapper(SInt32 nfds, fd_Set *readfds, fd_Set *writefds, fd_Set *exceptfds, const TimeSpec *timeout,const sigSet_t *sigmask)
{
    return pselect(nfds,readfds,writefds,exceptfds,timeout,sigmask);
}

inline void pError (Char8 *str)
{
    perror(str); 
    return;
}

void setRlimitOfMsgQueue(void)
{
    struct rlimit rLimit;
    /* +COVERITY 32181 */
    getrlimit_wrapper(RLIMIT_MSGQUEUE, &rLimit);
    /* +COVERITY 32181 */
    rLimit.rlim_cur = rLimit.rlim_cur * 10;
    rLimit.rlim_max = rLimit.rlim_max * 10;
    /* +COVERITY 32182 */
    setrlimit_wrapper(RLIMIT_MSGQUEUE, &rLimit);
    /* +COVERITY 32182 */
}

UInt32 setRlimitOfCore(void)
{
    struct rlimit rLimit;

    /* Get the maximum core file size */
    /* +COVERITY 32181 */
    if (-1 == getrlimit_wrapper(RLIMIT_CORE, &rLimit)) {
        perror("getrlimit_wrapper");
        return 0;
    }
    /* +COVERITY 32181 */

    /* Set current core file size to maximum */
    rLimit.rlim_cur = rLimit.rlim_max;
    /* +COVERITY 32182 */
    if (-1 == setrlimit_wrapper(RLIMIT_CORE, &rLimit)) {
        perror("setrlimit_wrapper");
        return 0;
    }
    /* +COVERITY 32182 */
    return 1;
}

/*Coverity 26298 Fix Start*/
void convertIPAddr(Char8 *socketInf_p, UInt8 * AddrArray_p, UInt32 size)
{
    snprintf(socketInf_p,size,"%d.%d.%d.%d",AddrArray_p[0],AddrArray_p[1],AddrArray_p[2],AddrArray_p[3]);
}
/*Coverity 26298 Fix End*/

void lteFD_CLR(SInt32 fd, fd_Set *set)
{
    FD_CLR(fd,set);
}

SInt32 lteFD_ISSET(SInt32 fd, fd_Set *set)
{
    return FD_ISSET(fd,set);
}

void lteFD_SET(SInt32 fd, fd_Set *set)
{
    FD_SET(fd,set);
}

void lteFD_ZERO(fd_Set *set)
{
    FD_ZERO(set);
}

SInt32 inet_pton_wrapper (SInt32 domain, const Char8 * peerTgInfV6, void *inp)
{
    return inet_pton(domain, peerTgInfV6, (struct in_addr *)inp);
}

UInt32 htonl_wrapper(UInt32 hostlong)
{
    return htonl(hostlong);
}

UInt16 htons_wrapper (UInt16 hostshort)
{
    return htons(hostshort);
}

UInt16 ntohs_wrapper(UInt16 netshort)
{
    return ntohs(netshort);
}

UInt32 ntohl_wrapper(UInt32 netlong)
{
    return ntohl(netlong);
}

size_t strlen_wrapper(const Char8 *s)
{
    return strlen(s);
}

SInt32 snprintf_wrapper(Char8 *s, size_t n, const Char8  *format, ...)
{
    SInt16 retVal = 0;

    /*531 - coverity 57588*/
/* SPR 19288 change start */
/* SPR 19288 change end */
    /* SPR 20430 Changes Start */
#ifdef __x86_64__
    va_list args = {{0}};
#else
    va_list args = {0};
#endif
    /* SPR 20430 Changes End */
    /*531 - coverity 57588*/
    va_start(args, format);
    retVal = vsnprintf(s, n, format, args);
    va_end(args);
    return retVal;
}

SInt32 printf_wrapper(const Char8 *fmt, ...)
{
    SInt32 retVal = 0;
    /* SPR 20430 Changes Start */
#ifdef __x86_64__
    /*531 - coverity 57588*/
    va_list args = {{0}};
    /*531 - coverity 57588*/
#else
    /* SPR 20430 Changes End */
    /*531 - coverity 57588*/
/* SPR 19288 change start */
    va_list args = {0};
/* SPR 19288 change end */
    /*531 - coverity 57588*/
    /* SPR 20430 Changes Start */
#endif
    /* SPR 20430 Changes End */
    va_start(args, fmt);
    retVal = vprintf(fmt, args);
    va_end(args);
    return retVal;
}

/* compilation fix for MAC_PHY_ENDIANESS */
/* wrapper function for bswap_16 */
UInt16 bswap16Wrap(UInt16 x)
{
    return bswap_16(x);
}

/* wrapper function for bswap_32 */
UInt32 bswap32Wrap(UInt32 x)
{
    return bswap_32(x);
}

//refer to /dev/shm to check the attributes of handler files
int shm_open_wrapper(const char *name, int oflag, int mode)
{
    int shm_fd = 0;
    mode_t omask;
    mode_t mode_set = (S_IRWXU|S_IRWXG|S_IRWXO);//0777

    if (0 != mode)
        mode_set = (mode_t)mode;

    //umask() sets the calling process's file mode creation mask, if not zero some bits in mode_set would not work
    omask = umask(0);   
    shm_fd = shm_open(name, oflag, mode_set);
    umask(omask);

    return shm_fd;
}

//refer to /dev/mqueue to check the attributes of handler files
int mq_open_wrapper(const char *name, int oflag, int mode, struct mq_attr *attr)
{
    int mq_fd = 0;
    mode_t omask;
    mode_t mode_set = (S_IRWXU|S_IRWXG|S_IRWXO);//0777

    if (0 != mode)
        mode_set = (mode_t)mode;

    //umask() sets the calling process's file mode creation mask, if not zero some bits in mode_set would not work
    omask = umask(0);   
    mq_fd = mq_open(name, oflag, mode_set, attr);
    umask(omask);

    return mq_fd;
}

