/*****************************************************************************
 * File Name   : oam_port_utils.h
 *
 * Description : This file contains prototypes of the wrapper functions written 
 *               to replace direct system calls
 *
 * Revision History:
 *
 * Date         Author           Reference                   Comments
 * -----        ------           ---------                   --------
 * Nov 2013    Rashi Arora   System calls removal activity   Initial Version
 * July 2014   Aditi Singh   Macro Added
 *
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef OAM_PORT_UTILS_H
#define OAM_PORT_UTILS_H

/****************************************************************************
 ************     INCLUDES *********************************************
 ****************************************************************************/
#include <lteTypes.h>
#include <stdarg.h> 
#include "oam_types.h"
#include <advlogging.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <curl/curl.h>

/****************************************************************************
                              MACROS
 ****************************************************************************/
#define OAM_FTIME           ftime
#define OAM_LOCALTIME       localtime
#define OAM_FTELL           ftell
#define OAM_FCLOSE          fclose
#define OAM_TIME            time
#define OAM_READDIR         readdir
#define OAM_CLOSEDIR        closedir
#define OAM_CTIME           ctime
#define OAM_GMTIME          gmtime    
#define OAM_STRDUP          strdup
#define OAM_MKTIME          mktime
#define OAM_GETCWD          getcwd
#define OAM_INET_PTON       inet_pton
#define OAM_STRRCHR         strrchr
#define OAM_MEMMOVE         memmove
#define OAM_STRTOUL         strtoul
#define OAM_SNPRINTF        snprintf
#define OAM_FGETS           fgets
#define OAM_STRCHR          strchr
#define OAM_CPU_ZERO        CPU_ZERO
#define OAM_CPU_SET         CPU_SET 
#define OAM_FFLUSH          fflush
#define OAM_SETRLIMIT       setrlimit
#define OAM_EXIT            exit
#define OAM_SIGNAL          signal
#define OAM_GETTIMEOFDAY    gettimeofday
#define OAM_CLOSE           close
#define OAM_BIND            bind
#define OAM_RECVFROM        recvfrom
#define OAM_INET_ADDR       inet_addr
#define OAM_SELECT          select
#define OAM_SCHED_SETAFFINITY sched_setaffinity
#define OAM_GETCHAR         getchar
#define OAM_MV              "mv"
#define OAM_CURL_GLOBAL_INIT(X) curl_global_init(X)
#define OAM_CURL_EASY_INIT curl_easy_init
#define OAM_CURL_EASY_SETOPT(X, Y, Z) curl_easy_setopt(X, Y, Z)
#define OAM_CURL_EASY_PERFORM(X) curl_easy_perform(X)
#define OAM_CURL_EASY_CLEANUP(X) curl_easy_cleanup(X)
#define OAM_CURL_GLOBAL_CLEANUP(X) curl_global_cleanup(X)
#define OAM_CURL_EASY_STRERROR(X) curl_easy_strerror(X)
#define OAM_FREEADDRINFO(X)  freeaddrinfo(X)
#define OAM_VA_START  va_start
#define OAM_VA_END    va_end
#define OAM_VSPRINTF(X,Y,Z)  vsprintf(X,Y,Z)
#define OAM_ISDIGIT(X)       isdigit(X)
#define OAM_VA_START  va_start
#define OAM_VA_END    va_end
#define OAM_VSPRINTF(X,Y,Z)  vsprintf(X,Y,Z)
#define OAM_CURL_GLOBAL_INIT(X) curl_global_init(X)
#define OAM_CURL_GLOBAL_CLEANUP(X) curl_global_cleanup(X)
#define OAM_CURL_EASY_SETOPT(X, Y, Z) curl_easy_setopt(X, Y, Z) 
#define OAM_CURL_EASY_INIT curl_easy_init
#define OAM_CURL_EASY_PERFORM(X) curl_easy_perform(X)
#define OAM_CURL_EASY_CLEANUP(X) curl_easy_cleanup(X)
#define OAM_CPU_ZERO  CPU_ZERO
#define OAM_CPU_SET   CPU_SET
#define OAM_OPENDIR(X)   opendir(X)
#define OAM_FREEADDRINFO(X)  freeaddrinfo(X)
/*SPR 22554 Fix Start*/
#define OAM_THREAD_NAME_LEN 20
/*SPR 22554 Fix End*/
/****************************************************************************
                            ENUMERATIONS / STRUCTURES
 ****************************************************************************/

/****************************************************************************
                             FUNCTION PROTOTYPES
 ****************************************************************************/
const char *
oam_inet_ntop(
int af, 
const void *src,
char *dst, socklen_t cnt
);

int oam_inet_pton(int af, const char *src, void *dst);

time_t oam_time(time_t *t);

char *oam_ctime(const time_t *timep);

struct tm *oam_gmtime(const time_t *timep);

char *oam_strdup(const char *s);

struct tm *oam_localtime(const time_t *timep);

struct tm *oam_localtime_r(const time_t *timep, struct tm *result);

time_t oam_mktime(struct tm *tm);

char *oam_getcwd(char *buf, size_t size);

size_t oam_strnlen(const char *s, size_t maxlen);

char *oam_fgets(char *s, int size, FILE *stream);

size_t oam_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

size_t oam_fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream);

//int oam_setrlimit(int resource, const struct rlimit *rlim);

//char *oam_strrchr(const char *s, int c);

int oam_getchar(void);

sighandler_t oam_signal(int signum, sighandler_t handler);

int oam_getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                                                                     struct addrinfo **res);
int oam_sched_setaffinity(pid_t pid, size_t cpusetsize,
                             cpu_set_t *mask);

const char *
oam_inet_ntop
(
    int af, 
    const void *src,
    char *dst, socklen_t cnt
);

int oam_fflush(FILE *stream);

UInt16 
oam_htons
(
    UInt16 hostshort
);

int 
oam_strcmp
(
    const char *s1, 
    const char *s2
);

char*
oam_strstr
(
    const char *haystack,
    const char *needle
);

void*
oam_memset
(
    void *s, 
    int c, 
    size_t n
);

int 
oam_memcmp
(
    const void *s1, 
    const void *s2, 
    size_t n
);

void*
oam_memcpy
(
    void *, 
    const void *,
    size_t
);

void *
oam_malloc
(
    UInt32 size
);

void 
oam_free 
(
    void *m
);

/* SPR 14997 START */
int
oam_abs
(
 int num
);
/* SPR 14997 END */

/*Coverity FIX 33228 */
int   
oam_snprintf
(
    char *str,
    size_t size,
    const char *format, ...
);

int   
oam_sprintf
(
    char *str,
    const char *format, ...
);

int 
oam_fprintf
(
    FILE *str, 
    const char *format, 
    ...
);

int 
snscanf
(
    const char *str, 
    int         n, 
    const char *format, 
    ...
);

char *
oam_strtok
(
    char *s,
    const char *delim
);

char*
oam_strcpy
(
    char       *dest, 
    const char *src
);

void*
oam_strcat
(
    void       *str1,
    const void *str2
);

Char8*
oam_strncat
(
    Char8 *s1, 
    const Char8 *s2, 
    size_t n
);

int
oam_strncasecmp
(
    const char *s1, 
    const char *s2, 
    size_t       n
);

/*SPR 21889 Start*/
char* oam_strerror(int err);
/*SPR 21889 End */

oam_pid_t 
oam_fork
(
    void
);

oam_pid_t 
oam_getpid
(
    void
);

int  
oam_execve
(
    const  char  *filename, 
    char  *const argv [], 
    char *const envp[]
);

int 
oam_system
(
    const char *string
);

int 
oam_getifaddrs
(
    oam_ifaddrs_t **ifap
);

void 
oam_freeifaddrs
(
    oam_ifaddrs_t *ifa
);

void 
oam_perror
(
    const char *string
);

char*
oam_strncpy
(
    Char8 *s1, 
    const Char8 *s2, 
    size_t n
);

FILE*
oam_fopen
(
    const Char8 *filename ,
    const Char8 *mode 
);

void
oam_fclose
(
FILE *
);


UInt16 
oam_pow
(
    UInt16 x, 
    UInt16 y
);

int 
oam_atoi
(
    const char *str 
);

long long 
oam_atoll
(
    const char *str
);

const char* 
oam_strchr
(
    const char *str, 
    char        var
);

int
oam_strncmp(
        const char *str1,
        const char *str2,
        size_t length
        );

SInt32
oam_remove 
(
    const char *string
);
oam_return_et
oam_kill
(
    UInt16 proc_id,
    UInt16 signal
);
oam_length_t
oam_strlen
(
    const char *str
);
/*SPR 22554 Fix Start*/
int
oam_pthread_setname_np
(
pthread_t *thread,
Char8 *threadName
);
/*SPR 22554 Fix End*/
int 
oam_pthread_create
(
    pthread_t            *thread,
    const pthread_attr_t *attr,
    void                 *(*start_routine)(void *),
    void                 *arg
);
oam_pid_t
oam_waitpid
(
    pid_t pid, 
    int *stat_loc, 
    int options
);
/* + SPR 17439 */
void 
oam_abort
(
void
);
/* - SPR 17439 */

int 
oam_inet_pton(int af, const char * src, void * dst);

UInt32
oam_atol(const char *str);


SInt32 oam_strcasecmp(const Char8 *s1, const Char8 *s2);

UInt64 oam_strtoull(const Char8* str, Char8** endptr, SInt32 base);

Char8* oam_strrchr(Char8 *str, Char8 var);

void oam_mv(const Char8* src, const Char8* dst);

int oam_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

int oam_pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);

time_t oam_time(time_t *t);

int oam_inet_aton(const char *cp, struct in_addr *inp);

int oam_getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                                                                     struct addrinfo **res);

char *oam_fgets(char *s, int size, FILE *stream);
struct tm *oam_localtime(const time_t *timep);
size_t oam_fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream);

size_t oam_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

int oam_fflush(FILE *stream);

long oam_strtol(const char *str, char ** endptr, int base);

Char8* oam_strtok_r(Char8 *s, const Char8 *delim, Char8 **save_ptr);

 /*SPR 22398 Fix Start*/
UInt32 oam_ntohs(UInt16 val);
 /*SPR 22398 Fix End*/
#endif /* OAM_PORT_UTILS_H */
