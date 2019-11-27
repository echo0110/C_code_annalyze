/******************************************************************************
*
*   FILE NAME:
*       rrc_porting.c
*
*   DESCRIPTION:
*       This file contains the Wrapper library for providing  
*       miscellaneous porting functions.
*
*   DATE            AUTHOR        REFERENCE       REASON
*   29 Jun 2013     Hari Oum      ---------       Initial
*
*   Copyright (c) 2013, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#include "rrc_porting.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <sys/ioctl.h>
#include <pthread.h>
#ifdef LKSCTP
#include <netinet/sctp.h>
#endif
/* SPR 16120 Start */
#include <signal.h>
/* SPR 16120 Stop */
pthread_mutex_t region_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sfn_sf_data_available = PTHREAD_COND_INITIALIZER;
pthread_cond_t cdma_time_data_available = PTHREAD_COND_INITIALIZER;




/*****************************************************************************
 * Function Name  : shmget_wrapper 
 * Inputs         : U32 key, size_t size, S32 shmflg
 * Outputs        : None
 * Returns        : Local Time
 * Description    : This function returns the localTime
 ********************************************************************************/
Tm *localtime_wrapper(const time_t *timep)
{
    return localtime(timep);
} 

/*****************************************************************************
 * Function Name  : pthread_attr_destroy_wrapper 
 * Inputs         : pthread_attr_t *attr
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function function initializes the thread attributes object 
 *                  pointed to by attr with default attribute values
 ********************************************************************************/
S32 pthread_attr_destroy_wrapper(pthread_attr_t *attr)
{
    return pthread_attr_destroy(attr);
}
 
/*****************************************************************************
 * Function Name  : pthread_attr_init_wrapper 
 * Inputs         : pthread_attr_t *attr
 * Outputs        : Attribute Object
 * Returns        : Success or Failure
 * Description    : This function initialize and destroy thread attributes object
 ********************************************************************************/
S32 pthread_attr_init_wrapper(pthread_attr_t *attr)
{
    return pthread_attr_init(attr);
}

/*****************************************************************************
 * Function Name  : S32 pthread_attr_setinheritsched_wrapper
 
 * Inputs         : pthread_attr_t *attr, S32 inheritsched
 * Outputs        : Attribute Object
 * Returns        : Success or Failure
 * Description    : This function set inherit  scheduler  attribute  in  thread
 *                  attributes object
 ********************************************************************************/
S32 pthread_attr_setinheritsched_wrapper
(
     pthread_attr_t *attr,
     S32 inheritsched
)
{
    return pthread_attr_setinheritsched(attr, (int)inheritsched); 
}

/*****************************************************************************
 * Function Name  : S32 pthread_attr_setschedparam_wrapper
 
 * Inputs         : pthread_attr_t *attr, SchedParam *param 
 * Outputs        : Attribute Object
 * Returns        : Success or Failure
 * Description    : This function set scheduling  parameter  attributes  in  thread
 *                  attributes object
 ********************************************************************************/
S32 pthread_attr_setschedparam_wrapper
(
    pthread_attr_t *attr, 
    SchedParam *param
)
{
     return pthread_attr_setschedparam(attr, param);
}

/*****************************************************************************
 * Function Name  : S32 pthread_attr_setschedpolicy_wrapper 
 * Inputs         : pthread_attr_t *attr, S32 policy
 * Outputs        : Attribute Object
 * Returns        : Success or Failure
 * Description    : This function set scheduling policy attribute in thread 
 *                  attributes object
 ********************************************************************************/
S32 pthread_attr_setschedpolicy_wrapper
(
    pthread_attr_t *attr, 
    S32 policy
)
{
    return pthread_attr_setschedpolicy(attr, (int)policy);
}

/*****************************************************************************
 * Function Name  : pthread_cancel_wrapper 
 * Inputs         : pthread_t thread
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function send a cancellation request to a thread
 ********************************************************************************/
S32 pthread_cancel_wrapper(pthread_t thread)
{
    return pthread_cancel(thread);
}

/*****************************************************************************
 * Function Name  : pthread_cond_signal_wrapper 
 * Inputs         : pthread_cond_t *cond
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function broadcast or signal a condition
 ********************************************************************************/
S32 pthread_cond_signal_wrapper(pthread_cond_t *cond)
{
    return pthread_cond_signal(cond);
}

/*****************************************************************************
 * Function Name  : S32 pthread_cond_wait_wrapper 
 * Inputs         : pthread_cond_t* cond, pthread_mutex_t* mutex
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This functions shall block on a condition variable
 ********************************************************************************/
S32 pthread_cond_wait_wrapper
(
    pthread_cond_t* cond,
    pthread_mutex_t* mutex
)
{
    return pthread_cond_wait(cond,mutex);
}

/*****************************************************************************
 * Function Name  : pthread_mutex_lock_wrapper 
 * Inputs         : pthread_mutex_t *mutex
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function locks a mutex
 ********************************************************************************/
S32 pthread_mutex_lock_wrapper(pthread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

/*****************************************************************************
 * Function Name  : pthread_mutex_unlock_wrapper 
 * Inputs         : pthread_mutex_t *mutex
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function unlocks a mutex
 ********************************************************************************/
S32 pthread_mutex_unlock_wrapper(pthread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

/*****************************************************************************
 * Function Name  : ioctl_wrapper 
 * Inputs         : S32 desc, S32 request , variable no of arguments
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function manipulates the underlying device parameters of 
 *                  special files
 ********************************************************************************/
S32 ioctl_wrapper
(
    S32 desc, 
    S32 request,
    ...
)
{
    return ioctl(desc,request);
}



/*****************************************************************************
 * Function Name  : accept_wrapper 
 * Inputs         : sockfd,SockAddr_genric,socklen_t
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function system call is used with connection-based
 *                  socket types
 ********************************************************************************/

S32 accept_wrapper
(
    S32 sockfd, 
    SockAddr_genric *addr, 
    socklen_t *addrlen
)
{
    return accept((int)sockfd, addr, addrlen);
}


/*****************************************************************************
 * Function Name  : connect_wrapper 
 * Inputs         : sockfd,SockAddr_genric,socklen_t
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function system call connects the socket referred to by
 *                   the file descriptor sockfd to the address specified by addr
 ********************************************************************************/
S32 connect_wrapper
(
    S32 sockfd, 
    const SockAddr_genric *addr,
    socklen_t addrlen
)
{
    return connect((int)sockfd, addr, addrlen);
}


/*****************************************************************************
 * Function Name  : inet_aton_wrapper 
 * Inputs         : const char *cp,InAddr
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : converts the Internet host address cp from the IPv4
 *                  numbers-and-dots notation into binary form (in network byte order) and
 *        stores it in the structure that inp points to
 ********************************************************************************/
S32 inet_aton_wrapper
(
    const char *cp, 
    InAddr *inp
)
{
    return inet_aton((const char *)cp, inp);
}

/*****************************************************************************
 * Function Name  : inet_ntoa_wrapper
 * Inputs         : InAddr
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function function converts the Internet host address
 *                  in, given in network byte order, 
 *                  to a string in IPv4 dotted-decimal nota-tion
 ********************************************************************************/
S8 *inet_ntoa_wrapper
(
    InAddr in
)
{
    return (void *)inet_ntoa(in);
}

/*****************************************************************************
 * Function Name  : inet_ntop_wrapper 
 * Inputs         : af,src,dst,socklen_t
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function converts  the network address structure src
 *                  in the af address family into a character string
 ********************************************************************************/
const S8 *inet_ntop_wrapper
(
    S32 af, 
    const void *src,
    char *dst, 
    socklen_t size
)
{
    return (void *)inet_ntop((int)af, (const void *)src, (char *)dst, (socklen_t)size);
}



/*****************************************************************************
 * Function Name  : listen_wrapper 
 * Inputs         : sockfd,backlog
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function  marks the socket referred to by sockfd as a
 *                  passive socket, that is, 
 *                  as a socket that will be used to accept incoming
 *                  connec-tion requests using accept
 ********************************************************************************/

S32 listen_wrapper
(
    S32 sockfd, 
    S32 backlog
)
{
    return listen((int)sockfd, (int)backlog);
}





/*****************************************************************************
 * Function Name  : getsockopt_wrapper
 * Inputs         : sockfd,level,optname,optval,socklen_t
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This Function manipulate options for the socket 
 *                  referred to by the file
 *                  descriptor sockfd
 ********************************************************************************/
S32 getsockopt_wrapper
(
    S32 sockfd, 
    S32 level, 
    S32 optname,
    void *optval, 
    socklen_t *optlen
)
{
    return getsockopt(sockfd,level,optname,optval,optlen);
}



/*****************************************************************************
 * Function Name  : time_wrapper 
 * Inputs         : time_t    *t
 * Outputs        : None
 * Returns        : tiem_t
 * Description    : This function  returns the time since the Epoch (00:00:00
 *                  UTC, January 1, 1970), measured in seconds 
 ********************************************************************************/

time_t time_wrapper
(
    time_t *t
)
{
    return time(t);
}

/*SPR 15896 Fix Start*/
/*****************************************************************************
 * Function Name  : difftime_wrapper 
 * Inputs         : struct timespec rawTime
 * Outputs        : None
 * Returns        : Double
 * Description    : This function function  returns  the number of seconds
 *                  elapsed between current time and start time
 ********************************************************************************/
double difftime_wrapper
(
    struct timespec rawTime
)
{
    double currentTime = 0.0;
    double rawStartTime = 0.0;
    struct timespec currentTimeStamp = {0};
/* SPR 19288 change start */
    clock_gettime_wrapper(CLOCK_MONOTONIC_RAW,&currentTimeStamp);
/* SPR 19288 change end */
    currentTime = currentTimeStamp.tv_sec + currentTimeStamp.tv_nsec/1000000000.0;
    rawStartTime = rawTime.tv_sec + rawTime.tv_nsec/1000000000.0;
    return currentTime - rawStartTime;
}
/*SPR 15896 Fix Stop*/

/*****************************************************************************
 * Function Name  : sync_fetch_and_or_wrapper
 * Inputs         : int health_monitor_val
 *                  0/1
 * Outputs        : None
 * Returns        : void
 * Description    : This function function does the atomic operation "or"
 ********************************************************************************/
void sync_fetch_and_or_wrapper
(
 /*Bug 9305 start*/
    U32 *health_monitor_val,
    U32 value,
 /*Bug 9305 end*/
   /*Lockless Implementation start*/
    QLOCK lock
   /*Lockless Implementation stop*/ 
)
{
   /*Lockless Implementation start*/
#ifdef LOCKLESS_IMPL 
   /*type of health_monitor_val may need some modifications in case the int and
     U32 have different size */
   __sync_fetch_and_or(health_monitor_val,value);
    printf("Unused variable :%p",lock);
#else
	if (PNULL != lock)
        qvLock(lock);

    *health_monitor_val = ((*health_monitor_val) | value);

	if (PNULL != lock)
        qvUnlock(lock);
#endif
   /*Lockless Implementation stop*/ 
}

/*****************************************************************************
 * Function Name  : sync_fetch_and_and_wrapper
 * Inputs         : int health_monitor_val
 *                  0/1
 * Outputs        : None
 * Returns        : void
 * Description    : This function function  does the atomic operation "and"
 ********************************************************************************/
void sync_fetch_and_and_wrapper
(
 /*Bug 9305 start*/
    U32 *health_monitor_val,
    U32 value,
 /*Bug 9305 end*/
   /*Lockless Implementation start*/
    QLOCK  lock
   /*Lockless Implementation stop*/
)
{
   /*Lockless Implementation start*/
#ifdef LOCKLESS_IMPL 
   /*type of health_monitor_val may need some modifications in case the int and
     U32 have different size */
    __sync_fetch_and_and(health_monitor_val,value);
    printf("Unused variable :%p",lock);
#else
	if (PNULL != lock)
    	qvLock(lock);

    *health_monitor_val = ((*health_monitor_val) & value);

	if (PNULL != lock)
    	qvUnlock(lock);
#endif
   /*Lockless Implementation stop*/ 
}

/*****************************************************************************
 * Function Name  : sync_fetch_and_add_wrapper
 * Inputs         : int health_monitor_val
 *                  0/1
 * Outputs        : None
 * Returns        : void
 * Description    : This function function does the atomic operation "add"
 ********************************************************************************/
void sync_fetch_and_add_wrapper
(
 /*Bug 9305 start*/
    U32 *health_monitor_val,
    U32 value,
 /*Bug 9305 end*/
   /*Lockless Implementation start*/
    QLOCK lock
   /*Lockless Implementation stop*/ 
)
{
   /*Lockless Implementation start*/
#ifdef LOCKLESS_IMPL 
   /*type of health_monitor_val may need some modifications in case the int and
     U32 have different size */
    __sync_fetch_and_add(health_monitor_val,value);
    printf("Unused variable :%p",lock);
#else
	if (PNULL != lock)
    qvLock(lock);

    *health_monitor_val = ((*health_monitor_val) + value);

	if (PNULL != lock)
    qvUnlock(lock);
#endif
   /*Lockless Implementation stop*/ 
}


/*port changes start*/
/*****************************************************************************
 * Function Name  : memset_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of memset
 ********************************************************************************/
void* memset_wrapper(void *s, S32 c, size_t n)
{
    return(void*)memset((void *)s, (int) c, (size_t) n);
}

/*****************************************************************************
 * Function Name  : l3_memcpy_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of memcpy
 ********************************************************************************/
void* l3_memcpy_wrapper(void *dest, const void *src, size_t n)
{
    return(void *)memcpy((void *)dest, (const void *)src, (size_t)n);
}

/*****************************************************************************
 * Function Name  : memcmp_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of memcmp
 ********************************************************************************/
S32 memcmp_wrapper(const void *s1, const void *s2, size_t n)
{
    return(S32) memcmp((const void *)s1, (const void *)s2, (size_t)n);
}

/*****************************************************************************
 * Function Name  : strncmp_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strncmp
 ********************************************************************************/
S32 strncmp_wrapper(const S8 *s1, const S8 *s2, size_t n)
{
    return(S32) strncmp((const char *)s1, (const char *)s2, (size_t)n);
}


/*****************************************************************************
 * Function Name  : l3_strncat_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strcat
 ********************************************************************************/
S8* l3_strncat_wrapper(S8 *dest, const S8 *src, S32 n)
{
    return(S8*)strncat((char *)dest, (const char *)src, (size_t)n);
}

/*****************************************************************************
 * Function Name  : l3_strtok_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strtok
 ********************************************************************************/
S8* l3_strtok_wrapper(S8 *str, const S8 *delim)
{
    return(S8*)strtok((char *)str, (const char *)delim);
}

/*****************************************************************************
 * Function Name  : strlen_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strlen
 ********************************************************************************/
/*size_t strlen_wrapper(const S8 *s)
{
    return(size_t) strlen((const char *)s);
}*/

/*****************************************************************************
 * Function Name  : strftime_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strftime
 ********************************************************************************/
size_t strftime_wrapper(S8 *s, size_t max, const S8 *format, const struct tm *tm)
{
    return(size_t) strftime((char *)s, (size_t)max, (const char *)format, (const struct tm *)tm);
}

/*****************************************************************************
 * Function Name  : tolower_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of toupper
 ********************************************************************************/
S32 tolower_wrapper(S32 c)
{
    return(S32) toupper((int)c);
}

/*****************************************************************************
 * Function Name  : fprintf_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of fprintf
 ********************************************************************************/
void fprintf_wrapper(FILE *stream, const S8 *format, ...)
{
    va_list args;
    
    VA_START( args, format );
    vfprintf((FILE *)stream, (const char*)format, args);
    VA_END( args );
}

/*****************************************************************************
 * Function Name  : vsnprintf_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of vsnprintf
 ********************************************************************************/
void vsnprintf_wrapper(S8 *str, size_t size, const S8 *format, va_list ap)
{
    vsnprintf((char *)str, (size_t)size, (const char *)format, ap);
}


/*****************************************************************************
 * Function Name  : puts_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of puts
 ********************************************************************************/
S32 puts_wrapper(const S8 *s)
{
    return(S32) puts((const char *)s);
}

/*****************************************************************************
 * Function Name  : fopen_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of fopen
 ********************************************************************************/
FILE* fopen_wrapper(const S8 *path, const S8 *mode)
{
    return(FILE*)fopen((const char *)path, (const char *)mode);
}

/*****************************************************************************
 * Function Name  : l3_strncpy_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strncpy
 ********************************************************************************/
S8* l3_strncpy_wrapper(S8 *dest, const S8 *src, size_t n)
{
    return(S8*)strncpy((char *)dest, (const char *)src, (size_t)n);
}

/*****************************************************************************
 * Function Name  : strcmp_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strcmp
 ********************************************************************************/
S32 strcmp_wrapper(const S8 *s1, const S8 *s2)
{
    return(S32)strcmp((const char *)s1, (const char *)s2);
}

/*****************************************************************************
 * Function Name  : strchr_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strchr
 ********************************************************************************/
S8* strchr_wrapper(const S8 *s, S32 c)
{
    return(S8*)strchr((const char *)s, (int)c);
}

/*****************************************************************************
 * Function Name  : strnlen_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of strnlen
 ********************************************************************************/
size_t strnlen_wrapper(const S8 *s, size_t maxlen)
{
    return(size_t)strnlen((const char *)s, (size_t)maxlen);
}

/*****************************************************************************
 * Function Name  : fwrite_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of fwrite
 ********************************************************************************/
size_t fwrite_wrapper(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return(size_t)fwrite((const void *)ptr, (size_t)size, (size_t)nmemb, (FILE *)stream);
}

/*****************************************************************************
 * Function Name  : malloc_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of malloc
 ********************************************************************************/
/*void* malloc_wrapper(size_t size)
{
    return(void*)malloc((size_t)size);
}*/

/*****************************************************************************
 * Function Name  : fgets_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of fgets
 ********************************************************************************/
/*S8* fgets_wrapper(S8 *s, S32 size, FILE *stream)
{
    return(S8*)fgets((char *)s, (int)size, (FILE *)stream);
}*/

/*****************************************************************************
 * Function Name  : fflush_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of fflush
 ********************************************************************************/
/*S32 fflush_wrapper(FILE *stream)
{
    return(S32)fflush((FILE *)stream);
}*/

/*****************************************************************************
 * Function Name  : ntohl_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of ntohl
 ********************************************************************************/
/*U32 ntohl_wrapper(uint32_t netlong)
{
    return(U32)ntohl((uint32_t)netlong);
}*/

/*****************************************************************************
 * Function Name  : ntohs_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of htons
 ********************************************************************************/
/*U16 ntohs_wrapper(uint16_t netshort)
{
    return(U16)ntohs((uint16_t)netshort);
}*/

/*****************************************************************************
 * Function Name  : htonl_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of htonl
 ********************************************************************************/
/*U32 htonl_wrapper(uint32_t hostlong)
{
    return(U32)htonl((uint32_t)hostlong);
}*/

/*****************************************************************************
 * Function Name  : htons_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of ntohs
 ********************************************************************************/
/*U16 htons_wrapper(uint16_t hostshort)
{
    return(U16)htons((uint16_t)hostshort);
} */

/*****************************************************************************
 * Function Name  : abort_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of abort
 ********************************************************************************/
/*void abort_wrapper(void)
{
    return(void)abort();
}*/

/*****************************************************************************
 * Function Name  : exit_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of exit
 ********************************************************************************/
void exit_wrapper(S32 e)
{
    return(void)exit((int)e);
}

/*****************************************************************************
 * Function Name  : perror_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of perror
 ********************************************************************************/
void perror_wrapper(S8 *msg)
{
    return(void)perror((char *)msg);
}

/*****************************************************************************
 * Function Name  : va_end_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of va_end
 ********************************************************************************/
void va_end_wrapper(va_list ap)
{
    return(void)va_end(ap);
}

/*****************************************************************************
 * Function Name  : srand_wrapper
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : Wrapper function of srand
 ********************************************************************************/
void srand_wrapper(S32 seed)
{
    return(void)srand((unsigned int)seed);
}


/*****************************************************************************
 * Function Name  : rrc_fread 
 * Inputs         : void *ptr
 *                  U32 size
 *                  U32 nmemb
 *                  FILE *stream
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function reads nmemb elements of data, each size bytes 
 *                  long, from the stream pointed to by stream storing them at the 
 *                  location given by ptr. 
 ********************************************************************************/
S32 rrc_fread(void *ptr, U32 size, U32 nmemb, FILE *stream)
{
    return (S32)fread(ptr,size,nmemb,stream);
}

/*****************************************************************************
 * Function Name  : rrc_fseek 
 * Inputs         : FILE *stream
 *                  U32 offset
 *                  U32 whence
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This functiom sets the file position indicator for the stream 
 *                  pointed to by stream 
 ********************************************************************************/
S32 rrc_fseek(FILE *stream, U32 offset, U32 whence)
{
    return (S32)fseek(stream,(S32)offset,(S32)whence);
}

/*****************************************************************************
 * Function Name  : rrc_ftell 
 * Inputs         : FILE *stream
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function sets the position of file descriptor at  stream 
 ********************************************************************************/
S32 rrc_ftell(FILE *stream)
{
    return (S32)ftell(stream);
}

/*****************************************************************************
 * Function Name  : rrc_fclose 
 * Inputs         : FILE *fp
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : Thid function closes the passed file descriptor
 ********************************************************************************/
S32 rrc_fclose(FILE *fp)
{
    return fclose(fp);
}

/*****************************************************************************
 * Function Name  : rrc_fclose 
 * Inputs         : FILE *fp
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : Thid function closes the passed file descriptor
 ********************************************************************************/

void free_wrapper(void *ptr)
{
    return free(ptr);
}

/*****************************************************************************
 * Function Name  : shmctl_wrapper 
 * Inputs         : S32 shmid, S32 cmd, void *buf 
 * Outputs        : buf
 * Returns        : success
 * Description    : Wrapper function of shmctl
 ********************************************************************************/
S32 shmctl_wrapper(S32 shmid, S32 cmd, void *buf)
{
    return (S32)shmctl((int)shmid, (int)cmd, (struct shmid_ds*)buf);
}


/*****************************************************************************
 * Function Name  : fcntl_wrapper
 * Inputs         : S32 fd, S32 cmd, S32 n
 * Outputs        : None
 * Returns        : The new descriptor
 * Description    : Wrapper function of fcntl
 ********************************************************************************/
S32 fcntl_wrapper(S32 fd, S32 cmd, S32 n )
{
    return (S32)fcntl((int)fd, (int)cmd, n);
}


/*****************************************************************************
 * Function Name  : atoi_wrapper
 * Inputs         : nptr  
 * Outputs        : nptr
 * Returns        : The converted value of nptr
 * Description    : Wrapper function of atoi
 ********************************************************************************/
S32 atoi_wrapper(const U8 *nptr)
{
    return (int)atoi((const char*)nptr);
}

/*****************************************************************************
 * Function Name  : bzero_wrapper
 * Inputs         : s, n
 * Outputs        : s
 * Returns        : None
 * Description    : Wrapper function of bzero
 ********************************************************************************/
/*void bzero_wrapper(void *s, size_t n)
{    
    return bzero(s, n);
}*/

/*****************************************************************************
 * Function Name  : clock_gettime_wrapper
 * Inputs         : clockid_t clk_id
 *                : struct timespec *res
 * Outputs        : res
 * Returns        : Success or Failure
 * Description    : Wrapper function of clock_gettime
 ********************************************************************************/
S32 clock_gettime_wrapper(clockid_t clk_id, struct timespec *res)
{
    return (S32) clock_gettime(clk_id, res);
}

/*****************************************************************************
 * Function Name  : getsockname_wrapper
 * Inputs         : S32 sockfd
 *                : struct sockaddr *addr
 *                : socklen_t *addrlen
 * Outputs        : addr, addrlen
 * Returns        : Success or Failure
 * Description    : Wrapper function of getsockname
 ********************************************************************************/
S32 getsockname_wrapper(S32 sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return (S32) getsockname((int) sockfd, addr, addrlen);
}
/*****************************************************************************
 * Function Name  : sleep_wrapper
 * Inputs         : S32 time
 * Outputs        : None
 * Returns        : None
 * Description    : Wrapper function of sleep
 ********************************************************************************/
void sleep_wrapper(S32 time)
{
    sleep(time);
}

/*****************************************************************************
 * Function Name  : strdup_wrapper
 * Inputs         : const S8 *s
 * Outputs        : s
 * Returns        : pointer to the duplicated string s
 * Description    : Wrapper function of strdup
 ********************************************************************************/
S8 *strdup_wrapper(const S8 *s)
{
    return (S8*) strdup((const char *)s);
}

#ifdef LKSCTP
/*****************************************************************************
 * Function Name  : sctp_freepaddrs_wrapper
 * Inputs         : struct sockaddr *p_addr
 * Outputs        : None
 * Returns        : sockaddr p_addr
 * Description    : Wrapper function of sctp_freepaddrs
 ********************************************************************************/
void sctp_freepaddrs_wrapper(struct sockaddr *p_addr)
{
    /*Bug#9832 fix start*/
    if (!p_addr)
    {
        free(p_addr);
    }
    /*Bug#9832 fix stop*/
}
#endif

/************************************************************************************
 *   FUNCTION NAME : get_sys_time
 *   INPUT         : sys_time_t *sys_time
 *   OUTPUT        : None
 *   RETURNS       : sys_time_t
 *   DESCRIPTIOPN  : This is a porting function which returns CDMA time and Curent Timestamp
 *************************************************************************************/
void get_sys_time(sys_time_t *sys_time)
{
    clock_gettime_real_time(&(sys_time->timestamp));
    clock_gettime_real_time(&(sys_time->cdma_time));    

#ifdef RRC_DEBUG
    RRC_TRACE(RRC_INFO,"%s: CDMA_Time [%llu]sec [%u]nsec",__FUNCTION__, sys_time->cdma_time.tv_sec, sys_time->cdma_time.tv_nsec);
    RRC_TRACE(RRC_INFO,"%s: Timestamp [%llu]sec [%u]nsec",__FUNCTION__, sys_time->timestamp.tv_sec, sys_time->timestamp.tv_nsec);
#endif
}


/*port changes stop*/

/* SPR 13645 Fix Start */
/************************************************************************************
 *   FUNCTION NAME : ltm_off_value_wrapper
 *   INPUT         : U8*	csfb_param_ltm_off_value
 *   OUTPUT        : None
 *   RETURNS       : void
 *   DESCRIPTIOPN  : This is a porting function which assigns LTM_OFF value 
 *************************************************************************************/
void ltm_off_value_wrapper(U8* csfb_param_ltm_off_value)
{
	*csfb_param_ltm_off_value = CSFB_PARAM_1XRT_LTM_OFF_VALUE;  
}

/* SPR 13645 Fix Stop */

#ifdef LINUX_PC_TEST
void rrc_handleFatalSignal(int signum)
{
	signal(signum, SIG_DFL);
	fprintf(stderr,"rrc_handleFatalSignal----quit!\n");
	exit(0);

	return ;
}

void rrc_handleSigInt(int signum)
{
	signal(signum, SIG_DFL);
	fprintf(stderr,"rrc_handleSigInt----quit!\n");
	exit(0);

	return ;
}
#endif

/* SPR 16120 Start */
/************************************************************************************
 *   FUNCTION NAME : rrc_signal_handler
 *   INPUT         : None
 *   OUTPUT        : None
 *   RETURNS       : void
 *   DESCRIPTIOPN  : This is a porting function to handle signals
 *************************************************************************************/
void rrc_signal_handler()
{
    signal(SIGPIPE, SIG_IGN);

#ifdef LINUX_PC_TEST
	signal(SIGQUIT, rrc_handleFatalSignal);
	signal(SIGILL, rrc_handleFatalSignal);
	signal(SIGABRT, rrc_handleFatalSignal);
	signal(SIGFPE, rrc_handleFatalSignal);
	signal(SIGSEGV, rrc_handleFatalSignal);
	signal(SIGBUS, rrc_handleFatalSignal);
	signal(SIGINT, rrc_handleSigInt);
#endif
}
/* SPR 16120 Stop */
/*SPR 22554 Fix Start*/
/************************************************************************************
 *   FUNCTION NAME : pthread_setname_np_wrapper
 *   INPUT         : thread,threadName
 *   OUTPUT        : 
 *   RETURNS       : 
 *   DESCRIPTIOPN  : This is a porting function to set name of thread
 *************************************************************************************/
U16
pthread_setname_np_wrapper
(
pthread_t *thread,
U8 *threadName
)
{
    return (pthread_setname_np(*thread, (const char *)threadName));
}
/*SPR 22554 Fix End*/
