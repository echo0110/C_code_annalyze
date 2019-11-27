/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name   : oam_agnt_main.h
 *
 * Description : 
 * Revision History:
 *
 * Date            Author          Reference             Comments
 * -----           ------          ---------             --------
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/

#ifndef OAM_AGNT_MAIN_H
#define OAM_AGNT_MAIN_H


/*******************************************************************************
 * Includes
 *******************************************************************************/

/*******************************************************************************
 * Type Def
 ********************************************************************************/

/*SPR 21889 +- */

#define OAM_AGNT_MAX_PORT_LENGTH            6
#define OAM_AGNT_MAX_IP_ADD_LEN_STR         64
#define OAM_AGNT_READ_BUFF_SIZE             100 

typedef enum
{
	MODE_TCP,
	MODE_UDP
} mode_et;


typedef struct _com_params_t
{
	mode_et                 mode;
	U8                 port[OAM_AGNT_MAX_PORT_LENGTH];
	U8                 ipaddr[OAM_AGNT_MAX_IP_ADD_LEN_STR];
} com_params_t;

typedef struct _agnt_ext_com_config_t
{
  com_params_t    oam_com_params;
  com_params_t    agnt_com_params; 
} agnt_ext_com_config_t;

/*SPR 21889 +- */

/*******************************************************************************
 * Defines
 ********************************************************************************/
/*******************************************************************************
**  Functions Declaration
 *****************************************************************************/
oam_agnt_return_et
oam_agnt_main(void);

oam_agnt_return_et
oam_agnt_single_thd_init (void);

oam_agnt_return_et
oam_agnt_get_word_from_header (UInt8 *p_header);

SInt8
oam_agnt_system(const char *string);

void *oam_agnt_memcpy(void *dest, const void *src, size_t n);

void oam_agnt_construct_api_header
(
 UInt8          *p_header,      
 UInt8           version_id,   
 UInt16          src_module_id,  
 UInt16          dst_module_id, 
 UInt16          api_id,         
 UInt16          api_buf_size   
);

SInt8
oam_agnt_remove(const char *string);

char *oam_agnt_strcpy(char *dest, const char *src);

void oam_agnt_abort(void);

oam_agnt_return_et oam_agnt_isValidIp4 (UInt8 *p_str);

int oam_agnt_strcmp(const char *s1, const char *s2);

int
oam_agnt_fprintf
(
    FILE *str,
    const char *format,
    ...
);


int
oam_agnt_sprintf (char *str, const char *format, ...);

char *
oam_agnt_strtok(char *str, const char *delim);

char *
oam_agnt_strcat(char *dest, const char *src);

FILE*
oam_agnt_fopen
(
    const Char8 *filename ,
    const Char8 *mode
);

void
oam_agnt_fclose
(
FILE *
);

void *
oam_memset(void *s, int c, size_t n);

int snscanf(const char *str, int n, const char *format, ...);

void oam_agnt_signal_handler(int sig);

void oam_agnt_cleanup(void);

void oam_agnt_exit(int status);

size_t oam_agnt_strnlen(const char *s, size_t maxlen);

sighandler_t oam_agnt_signal(int signum, sighandler_t handler);

char *oam_agnt_strncpy(Char8 *s1, const Char8 *s2, size_t n);

int oam_agnt_snprintf(char *str, size_t size, const char *format, ...);

char *oam_agnt_fgets(char *s, int size, FILE *stream);

int
oam_agnt_atoi
(
    const char *str
);
void
oam_free (void *m);
time_t oam_agnt_time(time_t *t);
struct tm *oam_agnt_localtime(const time_t *timep);
struct tm *oam_agnt_localtime_r(const time_t *timep, struct tm *result);

void (*oam_agnt_register_signal_handler (int sig, void (*oam_agnt_signal_handler) (int))) (int);/* Porting changes */

void oam_agnt_exit(int status);
size_t oam_agnt_strlen(const char *s);
size_t oam_agnt_strnlen(const char *s, size_t maxlen);
char *oam_agnt_strncpy(Char8 *s1, const Char8 *s2, size_t n);
int oam_agnt_snprintf(char *str, size_t size, const char *format, ...);
char *oam_agnt_fgets(char *s, int size, FILE *stream);
int oam_agnt_atoi(const char *str);
time_t oam_agnt_time(time_t *t);
struct tm *oam_agnt_localtime(const time_t *timep);
struct tm *oam_agnt_localtime_r(const time_t *timep, struct tm *result);
int oam_agnt_sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);


#endif
