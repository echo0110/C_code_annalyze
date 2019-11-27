/********************************************************************************
*
*       FILE NAME   :   logger.c
*
*       DESCRIPTION :   This file contains the functions for the logging purpose.
*
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Aug 20, 2012   TR-069 Amendment 4   Initial Creation
*       Anant K Sharma Mar 25, 2013                         removed compilation warnings 
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <logger.h>
#include <stdarg.h>
#include <map.h>
#include <ipr_cwmp_read_config.h>

#define BUFFER_LIMIT 80
#define LOG_TRACE "LOG_TRACE" 
#define FOPEN fopen
#define MAX_LOG_SIZE 512*1024
#define MAX_LOG_NAME_LEN 266

/******************************************************************************
    Global Variables
******************************************************************************/

/* For trace logs to be printed on console or in file*/ 
short g_logger_log_trace = 0;

/* Level set for the logger */
short g_logger_log_level = 0; 

/* File pointer for logger file name */
FILE *g_logger_log_fp = NULL;

/* Time structure used for logging */
LOGTIME logtime;

/* Map for holding  Modules ==> 0  and 1 ( if logs should be displayed)*/
map_map *g_logger_module_map = NULL;

/* If logger is being initialized or not */
static short IsInit = 0;

/* character value for time */
char g_logger_time_buffer[BUFFER_LIMIT];

/*Filename for LOG_TRACE*/
char *g_logger_filename = NULL;

/* Used for managing max size of logger file*/
static int g_logger_total = 0;
static int g_logger_hit = 0 ;



/******************************************************************************
 Function Definitions
******************************************************************************/

/*****************************************************************************
* FUNCTION: 	 	logger_init	
*
* DESCRIPTION:      The function initialize the logger functionality.
*
* ARGS:             None
*
* RETURNS:          TRUE or FALSE 
******************************************************************************/
int logger_init (void)
{
	LOGTIME t1;
	int rt = TRUE;
	time (&t1);

	g_logger_filename = (char *)ALLOCATE(MAX_LOG_NAME_LEN );
	if(NULL == g_logger_filename)
	{
		perror("calloc failed");
		exit(1);
	}

		    g_logger_log_trace = TRUE;
		    rt = TRUE;

	/*Creating log file for tr069 code */

		snprintf(g_logger_filename , MAX_LOG_NAME_LEN, "%s.log", "tr069_logs") ;

		g_logger_log_fp = FOPEN(g_logger_filename ,"w+");
		if (g_logger_log_fp == NULL)
		{
			ENB_PRINTF  ("\nNot able to open the file ");	
			rt = FALSE;	
		    g_logger_log_fp = stdout;
		} 

	//map Initialization
	if (IsInit != 1)
	    g_logger_module_map = map_new();		
	if (g_logger_module_map == NULL ) 
	{
		rt = FALSE;
		ENB_PRINTF ("\n Map not initialized successfully");
	}
    if (IsInit != 1)
    {
        config_log();
        IsInit = 1;
        if (debugger_port > 0)
        {
            start_debugger();
        }
        else
        {
            TRACE (LOG_CRITICAL ,"DEBUGGER", "Exiting from %s port is not exported",__FUNCTION__);
        }
    }
	return rt;
}

/*****************************************************************************
* FUNCTION:             logger_destroy
* DESCRIPTION:          The function deinitialize and release the memeory usd
			            by logger. 
* ARGS:                 None
* RETURNS:              None 
******************************************************************************/
void logger_destroy (void)
{
	if (g_logger_filename)
	{
		DEALLOCATE(g_logger_filename);
		g_logger_filename = NULL;
	}
	map_free(g_logger_module_map); 
}



/*******************************************************************************************
* FUNCTION:		        logger_addModule 
*
* DESCRIPTION:
*                       The funtion add new Module into the map maintained for modules names
*                       and their log level. 
*
* ARGS:
*                       char * module 	-   Module  name  
*                       int  val 	- log level for that Module.	
*
* RETURNS:
*                       
*
********************************************************************************************/

int logger_addModule (char * module, int val)
{
	int rt = FALSE;
	if (NULL == module)
		return rt; 
	rt = map_put(g_logger_module_map, module , val);
	return rt;
}

/*****************************************************************************
* FUNCTION:		    logger_getModuleLogLevel 
*
* DESCRIPTION:      The function  retrieves the log level from map based 
*                   on module name provided as input.
* ARGS:             const char * module : Module name . 
*
* RETURNS:          none.
******************************************************************************/
void logger_getModuleLogLevel (const char * module, int *val )
{
	if (NULL == module || NULL == val)
		return;
	 map_get(g_logger_module_map, module, val ); 
}  

/*****************************************************************************
* FUNCTION:		        logger_closeLog
* DESCRIPTION:          The function close the open stream which is initialized
*                       during logger initialization.
* ARGS:                 None
*
* RETURNS:
*
******************************************************************************/
int logger_closeLog (void)
{
	int rt = FALSE; 
	rt = fclose(g_logger_log_fp);
	return rt;
}


/*****************************************************************************
* FUNCTION:		        logger_setLogLevel 
*
* DESCRIPTION:
*                       The function sets the log level globally. 
*
* ARGS:
*                       None
*
* RETURNS:
*
******************************************************************************/
void logger_setLogLevel (short level)
{
	g_logger_log_level = level;
}




/*****************************************************************************
* FUNCTION:		_logPrint 
*
* DESCRIPTION:
*                       The function writes to the stream i.e g_logger_log_fp which may be 
*                       stdout or file descriptor.  
*
* ARGS:
*                       None
*
* RETURNS:
*                       No. of bytes written..
*
******************************************************************************/

int _logPrint (const char *format , ...)
{
	va_list A;
	int i;
    va_start(A, format);
    i = vfprintf(g_logger_log_fp,format, A);
    va_end(A);
    return i;  		
}

/*****************************************************************************
* FUNCTION:		_logPrintA 
*
* DESCRIPTION:
*                       None
*
* RETURNS:
*
******************************************************************************/

int _logPrintA (const char *format, va_list A)
{
	int i = 0;
	i = vfprintf(g_logger_log_fp, format, A);
	return i;
}

/*****************************************************************************
* FUNCTION:		checknAssign 
*
* DESCRIPTION:
*                       The function checks the size of logger file in which data
*                       is to be written . if size exceeds than it creates a new
*                       file and close the old file descriptor. 
*
* ARGS:
*                       None
*
* RETURNS:
*			None
******************************************************************************/

void logger_checknAssign (void)
{
  //  time_t t1;
    //int size  = 0 ;

   // time(&t1);

    if (g_logger_total > (MAX_LOG_SIZE))
    {
	static int log = 0;
        g_logger_filename[0] = '\0';
        snprintf(g_logger_filename , MAX_LOG_NAME_LEN , "%s_%d.log", FILE_PATH_NAME,log++) ;
        g_logger_total = 0 ;
    }

    if (g_logger_log_fp != stdout && g_logger_log_fp != NULL)
	{
        fclose(g_logger_log_fp);
	    g_logger_log_fp = NULL;
	}
    else
        return;


    //File Open
    g_logger_log_fp = FOPEN(g_logger_filename ,"a");
    if (g_logger_log_fp == NULL)
    {
        ENB_PRINTF ("\nNot able to open the file ");
        g_logger_log_fp = stdout;
    }
}

/*****************************************************************************
* FUNCTION:		trace 
*
* DESCRIPTION:  The function is called in every module to perform logging.  
*
* ARGS:         int level  - Level to be used.
*               const char* module  - Module name .
*               const char *format  - Format Specifier.
*               ...	- Variable no. of arguments.  
* RETURNS:
******************************************************************************/

inline  int  logger_trace (int level, const char* module, const char *format, ...)
{
	int mod_log_level = 0;
	LOGTIME_INFO timeinfo;
	g_logger_hit++;

	int rt = FALSE;

	if (NULL == module || NULL == format)
		return rt;

	if (!IsInit)
	{
		ENB_PRINTF ("\n Logger Not Intialized \n");
		return rt;
	}

	//get modular log level from module map
	logger_getModuleLogLevel(module,&mod_log_level);        	

	// If Module leel ls given than take that value else take default one.  
	//coverity fix	
	if (level == LOG_NONE) return rt;

	if (level > g_logger_log_level && (mod_log_level > -1 && mod_log_level < level))
		return rt; 
    /*Coverity 86615 Fix Start*/
	/*rt = TRUE;*/
    /*Coverity 86615 Fix Start*/

	if (NULL == g_logger_filename)
	{
		ENB_PRINTF ("\n Logger not Initialized \n");
		return FALSE;
	}


	va_list A;
	va_start(A, format);

	time (&logtime);
	timeinfo = localtime(&logtime);
	if(NULL == timeinfo)
	{
		va_end(A);
		return FALSE;
	}
	strftime(g_logger_time_buffer,BUFFER_LIMIT,"%c",timeinfo);

	_logPrint("\n+ [%s] , [%s]-> ", g_logger_time_buffer , module);	
	_logPrintA(format , A);
	_logPrint("\n");
	va_end(A);
	return TRUE;
}

void *tr_strcat(void *str1, const void *str2)
{
    return strncat(str1, str2, strlen(str2)+ 1);
}

char *tr_strncpy(char *dest, const char *src)
{
    return strncpy(dest,src,strlen(src)+ 1);
}

int tr_sprintf(char *str, const char *format, ...)
{
/*  coverity 86619 fix start */
  int status;
/*  coverity 86619 fix end */
  va_list args;
  va_start (args,format);
  status=vsprintf(str,format,args);
  va_end(args);
  return status; 
}

void *tr_malloc(size_t size)
{
    return malloc (size);
}







	
