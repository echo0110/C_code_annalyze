/**
 * @file	lte_3g_logging.h
 * @brief	LTE 3G logging library definition
 *
 * @author	Sergiy Kovalchuk (Sergiy.Kovalchuk@aricent.com)
 * @date	2009-02-04 13:33
 * 
 * Copyright (c) 2009, Aricent Inc. All Rights Reserved
 */
#ifndef _ADV_LOGGING_H_
#define _ADV_LOGGING_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cspl.h"
#include "lteLogger.h"

/**
 * @brief	This enumeration contains system log levels. 
 *
 * @arg		LogEmergency - Emergency system condition. The program cannot continue due to system core failure
 * @arg		LogAlert - Alert. Action must be taken immediatelly
 * @arg		LogFatal - Fatal error. The program cannot continue and will exit immediatelly.
 * @arg		LogError - Failure in one of program modules or threads, resulting in its termination. Overal application is stable and continue working.
 * @arg		LogWarning - Non-critical, but important comment reveals possible future problems
 * @arg		LogNotice - Normal, but significant condition
 * @arg		LogInfo - Informational message
 * @arg		LogBrief - Brief debug message
 * @arg		LogDetailed - Detailed debug message
 * @arg		LogDebug - Internal debug message
 * @arg		LogTrace - Trace message
 *
 * @note	Generally these log levels are identical to syslog levels
 */
typedef enum 
{
	LogEmergency,
	LogAlert,
	LogFatal,
	LogError,
	LogWarning,
	LogNotice,
	LogInfo,
	LogBrief,
	LogDetailed,
	LogDebug,
	LogTrace
}
AdvL_LogLevel;

/**
 * @brief	This enumeration contains standard log targets for UNIX system
 *
 * @arg		LogTarget_None - log target not specified; there was no init_log() call 
 * @arg		LogTarget_Syslog - syslog logging enabled
 * @arg		LogTarget_Console - console logging enabled
 * @arg		LogTarget_File - file logging enabled
 * @arg		LogTarget_Network - network logging enabled
 */
typedef enum
{
	LogTarget_None,
	
	// UNIX
	LogTarget_Syslog,
	LogTarget_Console,
	LogTarget_File,
	LogTarget_Network,
        LogTarget_Shared_Mem 
}
AdvL_LogTarget;

/**
 * @brief	This enumeration contains log subsystem error codes
 *
 * @arg		LogSuccess - operation completed successfully
 * 
 * @arg		LogInitFailed_TooManyContexts - cannot allocate memory for log context - too many contexts
 * @arg		LogInitFailed_DuplicateCall - duplicate call to init_log() function
 * @arg		LogInitFailed_WrongTarget - wrong target specified - LogTarget_None or not supported
 * @arg		LogInitFailed_SystemError - fopen() call for LogTarget_File failed
 * @arg		LogInitFailed_WrongAddress - wrong address was passed, IP:port pair separated by colon is needed
 * @arg		LogInitFailed_NetworkError - socket connection initialization failed
 * @arg		LogInitFailed_ConnectionRefused - unable to connect to remote network logger daemon
 */
typedef enum
{
	LogSuccess,	
	LogInitFailed_TooManyContexts,
	LogInitFailed_DuplicateCall,
	LogInitFailed_WrongTarget,
	LogInitFailed_SystemError,
	LogInitFailed_WrongAddress,
	LogInitFailed_NetworkError,
	LogInitFailed_ConnectionRefused,
        LogInitFailed_SharedMemError
}
AdvL_LogError;

/// This is context index to identify the logging context to use
typedef unsigned short AdvL_ContextIndex;

typedef enum
{
	LogContext_Default = 0,
    LogContext_S1ap,
    LogContext_X2ap,
	LogContext_None = 0xFFFF
}
AdvL_PredefinedContexts;

/**
 * @brief		The main macros for logging
 *
 * @note		For parameters description see write_log_message() function
 * @note		Each subsystem can implement its own logging function with predefined
 * 				facility name, for example
 * \code
 * #define log_core_message(log_level, format, message...) write_log_message(log_level, __FILE__, __LINE__, "Core", format, ##message)
 * \endcode
 * 
 * @note		This macro uses default (first) logging context
 */
#define log_message(log_level, facility, format, ...)\
write_log_message(LogContext_Default, log_level, __FILE__, __LINE__, __func__ , facility, format, ##__VA_ARGS__)




/**
 * @biref		This function is used to init log subsystem. No separate deinitialization call
 * 				needed
 *
 * @param[in]	target - log target to use
 * @param[in]	initData 	- for LogTarget_Syslog should contain application name, \n
 * 							- for LogTarget_Console is ignored \n
 * 							- for LogTarget_File should contain log file name with path. \n
 * 							- for LogTarget_Network should contain server IP:port pair (10.15.10.25:5000) or 
 * 							  port number for localhost (5000 -> 127.0.0.1:5000)
 * @param[out]	index - context identifier which can be used to explicitly identify logging context
 *
 * @return		LogSuccess - if operation successfull or appropriate error code otherwise
 */
AdvL_LogError init_log(AdvL_LogTarget target, const char* initData, AdvL_ContextIndex* index);

/**
 * @biref		This function is used to deinit log subsystem.
 *
 * @param[out]	index - context identifier which can be used to explicitly identify logging context
 *
 * @return		LogSuccess - if operation successfull or appropriate error code otherwise
 */
AdvL_LogError deinit_log(AdvL_ContextIndex index);

/**
 * @brief		This function is used to set log level limit at runtime. No messages below the limit will be logged
 *
 * @param[in]	index - logging context index, LogContext_Default for default
 * @param[in]	logLevel - log level to limit messages verbosity. Messages of the log level itself will be shown
 */
void set_loglevel_limit(AdvL_ContextIndex index, AdvL_LogLevel logLevel);

/**
 * @brief		This function is used to set file log rollover size
 * 
 * @param[in]	index - logging context index, LogContext_Default for default
 * @param[in]	rolloverSize - maximal size of log file
 */
void set_rollover_size(AdvL_ContextIndex index, unsigned long rolloverSize);

/**
 * @brief		This function enables log level for specified log context
 * @param[in]	index - logging context index, LogContext_Default for default
 */
void enable_log(AdvL_ContextIndex index);

/**
 * @brief		This function disables log level for specified log context
 * @param[in]	index - logging context index, LogContext_Default for default
 */
void disable_log(AdvL_ContextIndex index);

/**
 * @brief		This is actual logging function called by support macroses such as log_message()
 *
 * @param[in]	index - logging context index, LogContext_Default for default
 * @param[in]	logLevel - log level
 * @param[in]	filePath - name of the current source file, supplied by __FILE__ macro
 * @param[in]	line - current line, supplied by __LINE__ macro
 * @param[in]	function_name - current function, supplied by __func__ macro
 * @param[in]	facility - name of current logging subsystem
 * @param[in]   cell_index - index of the cell
 * @param[in]   ue_index - index of the ue
 * @param[in]	format - printf format string
 * @param[in]	... - variable number of parameters accroding to _format parameter
 */
void write_log_message(AdvL_ContextIndex index, AdvL_LogLevel logLevel, const char* filePath, unsigned int line,
	const char* function_name, const char* facility, U8 cell_index, U16 ue_index, const char* format, ...);

/**
 * @brief		This function writes raw message into log target (if supported)
 *
 * @param[in]	index - logging context index, LogContext_Default for default
 * @param[in]	format - printf format string
 * @param[in]	... - variable number of parameters accroding to _format parameter
 */
void write_raw_message(AdvL_ContextIndex index, const char* format, ...);

void write_log_message_asn(AdvL_ContextIndex index, AdvL_LogLevel logLevel, const char* filePath, unsigned int line,
	const char* function_name, const char* facility, U8 cell_index, U16 ue_index, const char* format, ...);
AdvL_LogError check_log_level(AdvL_ContextIndex index, AdvL_LogLevel logLevel, 
        AdvL_LogTarget   *plogTarget);

void set_names(lgr_shm_logging_config_t *thd_cfg,
               U8 index,
               const S8 **thread_name,
               const S8 **shm_name);
char* l3_strncpy(char *dest, const char *src, size_t size);
#ifdef __cplusplus
}
#endif

#endif /* _ADV_LOGGING_H_ */

