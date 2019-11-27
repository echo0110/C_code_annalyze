/******************************************************************************
*
*   FILE NAME:
*       rrc_porting.h
*
*   DESCRIPTION:
*       This is the header file which contains miscellaneous porting functions.
*
*   DATE            AUTHOR        REFERENCE       REASON
*   29 Jun 2013     Hari Oum      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_PORTING_H_
#define _RRC_PORTING_H_

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif
#include "rrc_shared_stats.h"

#define RRC_QVLOGVA(X,Y,Z)    qvLogVa(X,(const char*)Y,Z)
#define VA_START va_start
/* coverity fix start */
#define VA_END va_end
/* coverity fix stop */
#define HANDLE_ERROR_EN(en, msg) \
        do { errno = en; perror_wrapper((S8 *)msg); exit_wrapper(0); } while (0)
/* SPR 19288 change start */
typedef struct timespec TimeSpec;
typedef struct timeval timeVal;
typedef struct timezone timeZone;
S32 gettimeofday_wrapper(timeVal *tv,  timeZone *tz);
void *shmat_wrapper(S32 shmid, const void *shmaddr, S32 shmflg);
S32 shmget_wrapper(U32 key, size_t size, S32 shmflg);
S32 pthread_cond_signal_wrapper(pthread_cond_t *cond);
S32 pthread_mutex_lock_wrapper(pthread_mutex_t *mutex);
S32 close_wrapper(S32 fd);
S32 inet_pton_wrapper(S32 af, const char *src, void *dst);
S32 select_wrapper(S32 nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, timeVal *timeout);
S32 socket_wrapper(S32 domain, S32 type, S32 protocol);
S32 fflush_wrapper(FILE *stream);
U32 ntohl_wrapper(uint32_t netlong);
U32 htonl_wrapper(uint32_t hostlong);
void clock_gettime_real_time(TimeSpec *ts_p);
/* SPR 19288 change end */
typedef struct tm Tm;
typedef struct sched_param  SchedParam;
typedef struct sockaddr  SockAddr_genric;
typedef struct in_addr  InAddr;

Tm *localtime_wrapper(const time_t *timep);
S32 pthread_attr_destroy_wrapper(pthread_attr_t *attr);
S32 pthread_attr_init_wrapper(pthread_attr_t *attr);
S32 pthread_attr_setinheritsched_wrapper(pthread_attr_t *attr,S32 inheritsched);
S32 pthread_attr_setschedparam_wrapper(pthread_attr_t *attr, SchedParam *param);
S32 pthread_attr_setschedpolicy_wrapper(pthread_attr_t *attr, S32 policy);
S32 pthread_cancel_wrapper(pthread_t thread);
S32 pthread_cond_wait_wrapper(pthread_cond_t * cond, pthread_mutex_t * mutex);
S32 pthread_mutex_unlock_wrapper(pthread_mutex_t *mutex);
size_t strftime_wrapper(S8 *s, size_t max, const S8 *format,
                       const Tm *tm);

time_t time_wrapper(time_t *t);
S32 ioctl_wrapper(S32 desc, S32 request,...);

S32 accept_wrapper(S32 sockfd, SockAddr_genric *addr, socklen_t *addrlen);
S32 connect_wrapper(S32 sockfd, const SockAddr_genric *addr,
                   socklen_t addrlen);
in_addr_t inet_addr_wrapper(const char *cp);
S32 inet_aton_wrapper(const char *cp, InAddr *inp);

S8 *inet_ntoa_wrapper(InAddr in);
const S8 *inet_ntop_wrapper(S32 af, const void *src,
                             char *dst, socklen_t size);
S32 listen_wrapper(S32 sockfd, S32 backlog);





S32 getsockopt_wrapper(S32 sockfd, S32 level, S32 optname,
                      void *optval, socklen_t *optlen);
/*SPR 15896 Fix Start*/
double difftime_wrapper(struct timespec rawTime);
/*SPR 15896 Fix Stop*/

/*Lockless Implementation start*/
/*Bug 9305 start*/
void sync_fetch_and_or_wrapper(U32 *health_monitor_val, U32 value,QLOCK lock);
void sync_fetch_and_and_wrapper(U32 *health_monitor_val, U32 value,QLOCK lock);
void sync_fetch_and_add_wrapper(U32 *health_monitor_val, U32 value,QLOCK lock);
/*Bug 9305 end*/
/*Lockless Implementation stop*/

/*port changes start*/
void* memset_wrapper(void *s, S32 c, size_t n);
void* l3_memcpy_wrapper(void *dest, const void *src, size_t n);
S32 memcmp_wrapper(const void *s1, const void *s2, size_t n);
S32 strncmp_wrapper(const S8 *s1, const S8 *s2, size_t n);
S8* l3_strncat_wrapper(S8 *dest, const S8 *src, S32 n);
S8* l3_strtok_wrapper(S8 *str, const S8 *delim);
size_t strftime_wrapper(S8 *s, size_t max, const S8 *format, const struct tm *tm);
S32 tolower_wrapper(S32 c);
 void fprintf_wrapper(FILE *stream, const S8 *format, ...); 
void vsnprintf_wrapper(S8 *str, size_t size, const S8 *format, va_list ap);
S32 puts_wrapper(const S8 *s);
FILE* fopen_wrapper(const S8 *path, const S8 *mode);
S8* l3_strncpy_wrapper(S8 *dest, const S8 *src, size_t n);
S32 strcmp_wrapper(const S8 *s1, const S8 *s2);
S8* strchr_wrapper(const S8 *s, S32 c);
size_t strnlen_wrapper(const S8 *s, size_t maxlen);
size_t fwrite_wrapper(const void *ptr, size_t size, size_t nmemb, FILE *stream);
void* malloc_wrapper(size_t size);
S8* fgets_wrapper(S8 *s, S32 size, FILE *stream);
U16 ntohs_wrapper(uint16_t hostshort);
U16 htons_wrapper(uint16_t netshort);
void abort_wrapper(void);
void exit_wrapper(S32 e);
void perror_wrapper(S8 *msg);
void va_end_wrapper(va_list ap);
void srand_wrapper(S32 seed);
void free_wrapper(void *ptr);

S32  
rrc_fread
(
    void *ptr, 
    U32 size, 
    U32 nmemb, 
    FILE *stream
);

S32 
rrc_fseek
(
    FILE *stream, 
    U32 offset, 
    U32 whence
);

S32 
rrc_ftell
(
    FILE *stream
);

S32 
rrc_fclose
(
    FILE *fp
);

S32 shmctl_wrapper(S32 shmid, S32 cmd, void *buf);

S32 fcntl_wrapper(S32 fd, S32 cmd, S32 n );

S32 atoi_wrapper(const U8 *nptr);

void bzero_wrapper(void *s, size_t n);

S32 clock_gettime_wrapper(clockid_t clk_id, struct timespec *res);

S32 getsockname_wrapper(S32 sockfd, struct sockaddr *addr, socklen_t *addrlen);

void sleep_wrapper(S32 time);

S8 *strdup_wrapper(const S8 *s);

#ifdef LKSCTP
void sctp_freepaddrs_wrapper(struct sockaddr *p_addr);
#endif

void get_sys_time(sys_time_t *sys_time);
/*port changes stop*/

/* SPR 13645 Fix Start */
void ltm_off_value_wrapper(U8* csfb_param_ltm_off_value);
/* SPR 13645 Fix Stop */

/* SPR 16120 Start */
/* SPR 17213 Fix Start */
void rrc_signal_handler(void);
/* SPR 17213 Fix End */
/* SPR 16120 Stop */
/*SPR 22554 Fix Start*/
U16
pthread_setname_np_wrapper
(
pthread_t *thread,
U8 *threadName
);
/*SPR 22554 Fix End*/
#endif /* _RRC_PORTING_H_ */

