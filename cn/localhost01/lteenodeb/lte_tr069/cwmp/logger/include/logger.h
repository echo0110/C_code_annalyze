#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <time.h>

#define LOG_NONE 	0 
#define LOG_CRITICAL 	1
#define LOG_MAJOR      2
#define LOG_MINOR      3
/* SPR 19152 FIXED START */
#define LOG_DETAIL 	4
#define LOG_INFO 	5
/* SPR 19152 FIXED END */

#define TRUE 		1
#define FALSE 		0

#define FILE_PATH_NAME "./Logs/cpe.log" 
#define TRACE logger_trace
#define PTHREAD_STACK_SIZE 1024*1024
#include <cspl.h>
/* SPR 22314 Start */
/* Code Removed */
/* SPR 22314 End */
/*

typedef struct LOGTIME{
	unsigned long s; //Seconds
	unsigned long us; //Micro seconds
}LOGTIME;
*/

typedef time_t LOGTIME;
typedef struct tm * LOGTIME_INFO;


int  logger_init(void);
inline int logger_trace(int level, const char * ,const char *format, ...);
void logger_setLogLevel(short level);  

//added following interfaces to remove warnings from stub.c and debugger.c
void logger_checknAssign (void);
int logger_addModule (char * module, int val);
void config_log (void);
int start_debugger(void);

//added following wrapper functions

void *tr_strcat(void *str1, const void *str2);

char *tr_strncpy(char *dest, const char *src);

int tr_sprintf(char *str, const char *format, ...);

void *tr_malloc(size_t size);

#endif
