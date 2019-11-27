/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name   : oam_agnt_port.h
 *
 * Description : This file contain porting prototypes
 *
 * Revision History:
 *
 * Date              Author          Reference                Comments
 * -----             ------          ---------                --------
   
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/
#ifndef _OAM_AGNT_PORT_H_
#define _OAM_AGNT_PORT_H_
#include <signal.h>

#define OAM_AGNT_OPENDIR(X)   opendir(X)
#define OAM_AGNT_READDIR(X)   readdir(X)
#define OAM_AGNT_SLEEP(X)     sleep(X)
#define OAM_AGNT_VA_START  va_start
#define OAM_AGNT_VA_END    va_end
#define OAM_AGNT_VSPRINTF(X,Y,Z)  vsprintf(X,Y,Z)
#define OAM_AGNT_CPU_ZERO  CPU_ZERO
#define OAM_AGNT_CPU_SET  CPU_SET

#define AGNT_MALLOC              malloc /* Porting changes */
#define AGNT_CALLOC              calloc /* Porting changes */
#define AGNT_FREE                free /* Porting changes */
#define AGNT_SIGTERM             SIGTERM    /* Porting changes */
#define AGNT_SIGKILL             SIGKILL    /* Porting changes */
#define AGNT_SIGINT              SIGINT     /* Porting changes */
#define AGNT_SIGABRT             SIGABRT    /* Porting changes */

#define OAM_AGNT_EXIT_FAILURE               1
#define OAM_AGNT_EXIT_SUCCESS               0

typedef struct sockaddr_in oam_sockaddr_t;
typedef struct in_addr oam_in_addr_t;
typedef struct timeval oam_agnt_timeval;
unsigned int oam_agnt_htons(unsigned int hostshort);

int
oam_agnt_execve (const char *filename, char *const argv[],
		                      char *const envp[]);

void *oam_agnt_memset(void *s, int c, size_t n);

void oam_agnt_perror(const char *string);

void *oam_agnt_malloc (UInt32 size);

void oam_agnt_free (void *m);

Int oam_agnt_vprintf (const Char8 * format, va_list A);

pid_t oam_agnt_fork (void);

pid_t oam_agnt_getpid (void);

int oam_agnt_fprintf(FILE * str, const char *format, ...);

int oam_agnt_sprintf (char *str, const char *format, ...);

FILE *oam_agnt_fopen(const Char8 *filename , const Char8 *mode );

void oam_agnt_fclose(FILE *file_ptr);

char *oam_agnt_strcpy(char *dest, const char *src);

int oam_agnt_strcmp(const char *s1, const char *s2);

char * oam_agnt_strtok(char *str, const char *delim);

char * oam_agnt_strcat(char *dest, const char *src);

SInt8 oam_agnt_system (const char *string);

SInt8 oam_agnt_remove (const char *string);

void oam_agnt_abort(void);

void * oam_agnt_memcpy (void *dest, const void *src, size_t n);

int snscanf(const char *str, int n, const char *format, ...);

void oam_agnt_exit(int status);

size_t oam_agnt_strnlen(const char *s, size_t maxlen);

sighandler_t oam_agnt_signal(int signum, sighandler_t handler);
    
char *oam_agnt_strncpy(Char8 *s1, const Char8 *s2, size_t n);

char *oam_agnt_fgets(char *s, int size, FILE *stream);
int oam_agnt_fflush(FILE *file_ptr);

int oam_agnt_atoi(const char *str);

time_t oam_agnt_time(time_t *t);

struct tm *oam_agnt_localtime(const time_t *timep);

struct tm *oam_agnt_localtime_r(const time_t *timep, struct tm *result);

void (*oam_agnt_register_signal_handler (int sig, void (*oam_agnt_signal_handler) (int))) (int);

size_t oam_agnt_strlen(const char *s);

int oam_agnt_snprintf(char *str, size_t size, const char *format, ...);

int oam_agnt_sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);

int oam_agnt_access(const Char8 *p_pathname, int mode);
int oam_agnt_chmod(const char *pathname, mode_t mode);

oam_agnt_return_et oam_agnt_kill(pid_t proc_id, UInt16 signal);
oam_agnt_return_et oam_agnt_kill_all_child(pid_t proc_id, UInt16 signal);

pid_t oam_agnt_waitpid(pid_t pid, int *status, int options);
char *oam_agnt_get_cwd(char *p_buf, size_t size);
int oam_agnt_pclose(FILE *p_stream);
FILE *oam_agnt_popen(const Char8 *p_cmd, const Char8 *p_mode);
int oam_agnt_socket_close(int fd);
UInt32 oam_agnt_sleep(UInt32 time);
int oam_agnt_rename(Char8 *p_old_name, Char8 *p_new_name);
char *oam_agnt_strerror(int errnum);
#endif
