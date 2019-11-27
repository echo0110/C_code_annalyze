/**
 * @file	lte_3g_logging.c
 * @brief	LTE 3G logging library implementation
 *
 * @author	Sergiy Kovalchuk (Sergiy.Kovalchuk@aricent.com)
 * @date	2009-02-04 13:32
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "advlogging.h"

#include <assert.h>

// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include <stdarg.h>
#include <string.h>

#include <stdbool.h>

// Time functions
#include <time.h>

// Network logger support
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Stat function for file roller
#include <unistd.h>
#include <sys/stat.h>

// Errno, strerror_r support
#include <errno.h>

// Multithreading support
#include <pthread.h>

// Definitions
#define MAX_TARGET_CONTEXTS_NUMBER		32					/// Maximal count of log targets

/* #define MESSAGE_BUFFER_SIZE 			1024*/				/// Maximal log message buffer size for outgoing message
/* Buffer size has been increased due to 
 *  inclusion of ASN Print PDU logging 
 */ 
#define MESSAGE_BUFFER_SIZE 			35000				/// Maximal log message buffer size for outgoing message


#define GENERIC_MESSAGE_FORMAT_LENGTH	72					/// This value depends of genericMessageFormat

#define USER_MESSAGE_BUFFER_SIZE		MESSAGE_BUFFER_SIZE - GENERIC_MESSAGE_FORMAT_LENGTH

#define MIN_LOG_FILE_SIZE				64535				/// 64 KB
#define MAX_LOG_FILE_SIZE				2147483647			/// 2 GB

#define NET_DEFAULT_IP_ADDRESS			"127.0.0.1"			/// Localhost
#define MAX_HOST_ADDRESS_LENGTH			24					/// 16 bytes of IP address + 1 byte for ':' + 5 bytes for port number and + 1 byte for EOL
#define _LOG_MICROSECONDS

typedef struct
{
    AdvL_LogTarget logTarget;						/// Current log target
    AdvL_LogLevel logLevelLimit;					/// Current log level limit
	
	bool logEnabled;								/// Value indicating does log enabled or disabled

    pthread_mutex_t logMutex;						/// Guard mutex for multithreading environments

    // LogTarget_File specific
    FILE* logFile;									/// Current log file handle
    char* logFileName;								/// Current log file name
	
	unsigned long rolloverSize;						/// Rollover size, MAX_LOG_FILE_SIZE by default

    // LogTarget_Network specific
    int netlogSocket;								/// Current network logging socket, applicable for LogTarget_Network
	struct sockaddr_in socketAddress;				/// Socket address of the UDP server, applicable for LogTarget_Network
}
AdvL_TargetContext;


// Global data
static AdvL_TargetContext* contextsArray[MAX_TARGET_CONTEXTS_NUMBER];
static AdvL_TargetContext* defaultContext = NULL;

/// Log level; facility; file name; file line; function name; message
#define MESSAGE_FORMAT   		"    %-11s %-18s %-30s %-7u %-30s %s\n"

#define DATE_FORMAT 			"%04d.%02d.%02d "
#define TIME_FORMAT				"%02d:%02d:%02d"
#define MICROSECONDS_FORMAT		".%06d"

#if defined(_LOG_DATE)

	static const char* genericMessageFormat = DATE_FORMAT TIME_FORMAT MICROSECONDS_FORMAT MESSAGE_FORMAT;
	
#else
	
	static const char* genericMessageFormat = TIME_FORMAT MICROSECONDS_FORMAT MESSAGE_FORMAT;
	
#endif /* _LOG_DATE */

/// Facility; file name; file line;function name; message
static const char* syslogMessageFormat = "%s  %s %7u  %s  %s";

// ///////////////// ///////////////// ///////////////// ///////////////// ///////////////// ///////////////
// Internal support functions

/**
 * @brief		This function converts AdvL_LogLevel code into standard syslog log level code
 *
 * @param[in]	level - internal log level value
 * @return		Syslog log level value
 */
static int loglevel_to_sysloglevel(AdvL_LogLevel level)
{
    switch (level)
    {
	case LogEmergency:
		return LOG_EMERG;
	case LogAlert:
		return LOG_ALERT;
    case LogFatal:
        return LOG_CRIT;
    case LogError:
        return LOG_ERR;
    case LogWarning:
        return LOG_WARNING;
    case LogNotice:
        return LOG_NOTICE;
    case LogInfo:
        return LOG_INFO;
	case LogBrief:
	case LogDetailed:
	case LogDebug:
	case LogTrace:
    default:
        return LOG_DEBUG;
    }
}

/**
 * @brief		This function return string representation for corresponding log level value
 *
 * @param[in]	level - internal log level value
 * @return		String representation of corresponding log level value
 */
static const char* loglevel_to_string(AdvL_LogLevel level)
{
    switch (level)
    {
	case LogEmergency:
		return "Emergency";
	case LogAlert:
		return "Alert";
    case LogFatal:
        return "Fatal";
    case LogError:
        return "Error";
    case LogWarning:
        return "Warning";
    case LogNotice:
        return "Notice";
    case LogInfo:
        return "Info";
	case LogBrief:
		return "Brief";
	case LogDetailed:
		return "Detailed";
	case LogDebug:
		return "Debug";
	case LogTrace:
	default:
		return "Trace";
    }
}

/**
 * @brief		This function splits file name from its path
 * @note		__FILE__ macro generally returns file path, which may be too long and not needed
 * 
 * @param[in]	filePath - file path
 * @return		Pure file name
 */
static const char* split_file_name_from_path(const char* filePath)
{
	char* namePtr = strrchr(filePath, '/');
	if(namePtr && *(namePtr + 1) != '\0') return namePtr + 1;
	
	return filePath;
}

/**
 * @brief		This function formats generic message with timestamp, log level,
 * facility, file name,line and function name
 *
 * @param[out]	userMessageBuffer - buffer to store formatted message
 * @param[in]	logLevel - log level
 * @param[in]	fileName - name of the current source file, supplied by __FILE__ macro
 * @param[in]	line - current line, supplied by __LINE__ macro
 * @param[in]	function name  - current function, supplied by __func__ macro
 * @param[in]	facility - name of current logging subsystem
 * @param[in]	message - log message
 */
static void format_generic_message(char* messageBuffer, AdvL_LogLevel logLevel, const char* fileName,
                                   unsigned int line, const char* function_name, const char* facility, const char* message)
{
	// Struct tm is set in any case, since it is necessary for parsing hours, minutes and seconds
	struct tm currentTime;
	
	
	// If we do need microseconds, we shall use gettimeofday() instead of time()
	struct timeval timeValue;
	gettimeofday(&timeValue, 0);
	
	localtime_r(&timeValue.tv_sec, &currentTime);
	
	
	
#if defined(_LOG_DATE)
	
	// Corrections to make it human readible
    currentTime.tm_year += 1900;
    currentTime.tm_mon++;
	
	sprintf(messageBuffer, genericMessageFormat,
			currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday,
			currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec,
   
			timeValue.tv_usec, 
   
			loglevel_to_string(logLevel), facility,
            split_file_name_from_path(fileName), line, function_name, message);

#else
	
	sprintf(messageBuffer, genericMessageFormat,
			currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec,
   
			timeValue.tv_usec, 
   
			loglevel_to_string(logLevel), facility,
            split_file_name_from_path(fileName), line, function_name, message);
	
#endif /* defined(_LOG_DATE) */
}

/**
 * @brief		This function writes message buffer into file. If the file reaches maximal log size, the
 * 				function closes it, appends timestamp and opens new one
 *
 * @param[in]	messageBuffer - message buffer to write
 */
static void write_and_roll_file(AdvL_TargetContext* context, char* messageBuffer)
{
    struct stat logStat;
	
	assert(context != NULL && context->logTarget == LogTarget_File);
	
	
	if (stat(context->logFileName, &logStat) == -1)
    {
        fprintf(context->logFile, " ### Log failure, cannot stat log file! ");
        return;
    }

    // If it is time to roll...
    if ((size_t)logStat.st_size > context->rolloverSize - MESSAGE_BUFFER_SIZE)
    {
        const unsigned short _postfixSize = 21;						/// Length of the timestamp for rolled file name. Keep up to date

        int fileNameLength;
        char* rolledFileName;

        time_t currentTimestamp;
        struct tm currentTime;

        // Receive current time
        currentTimestamp = time(0);
        localtime_r(&currentTimestamp, &currentTime);

        // Corrections to make it human readible
        currentTime.tm_year += 1900;
        currentTime.tm_mon++;

        fileNameLength = strlen(context->logFileName) + _postfixSize;
        rolledFileName = (char *)malloc(fileNameLength);


        // Format rolled file name
        sprintf(rolledFileName, "%s_%04d.%02d.%02d_%02d.%02d.%02d", context->logFileName,
                currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday,
                currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

        // Close, rename, reopen new instance

        // Note: during rename() and fopen() we really hope that everything will be OK, since there is no chance to report failure
        // except of syslog() facility.

		fclose(context->logFile);
		rename(context->logFileName, rolledFileName);

		context->logFile = fopen(context->logFileName, "a+");

        free(rolledFileName);
    }

    // And write log
	fputs(messageBuffer, context->logFile);
	fflush(context->logFile);
}

/**
 * @brief	This function automatically deinitializes log contexts via atexit() call
 */
static void deinit_log_contexts()
{
	AdvL_ContextIndex index = 0;
	
	for (index = 0;index < MAX_TARGET_CONTEXTS_NUMBER;++index)
		if (contextsArray[index] != NULL)
        {
			deinit_log(index);
        }
}
/**
 * @brief		This function initializes new empyt context
 * 
 * @param[in]	index - context index in contexts array
 * @return		Pointer to allocated context array structure
 */
static AdvL_TargetContext* init_context(AdvL_ContextIndex index)
{
    AdvL_TargetContext* context;

    contextsArray[index] = (AdvL_TargetContext*)malloc(sizeof(AdvL_TargetContext));
    context = contextsArray[index];

    assert(context != NULL);

    context->logTarget = LogTarget_None;				/// Current log target
    context->logLevelLimit = LogTrace;					/// Current log level limit
	
	context->logEnabled = true;							/// Log is enabled by default

    // LogTarget_File specific
    context->logFile = NULL;							/// Current log file handle
    context->logFileName = NULL;						/// Current log file name
	
	context->rolloverSize = MAX_LOG_FILE_SIZE;
	
    // LogTarget_Network specific
    context->netlogSocket = -1;							/// Current network logging socket, applicable for LogTarget_Network
	
	// Init mutex
	pthread_mutex_init(&context->logMutex, NULL);
	
	return context;
}

// ///////////////// ///////////////// ///////////////// ///////////////// ///////////////// ///////////////
// Public API

/**
 * @brief		This function is used to init log subsystem. No separate deinitialization call needed
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
AdvL_LogError init_log(AdvL_LogTarget target, const char* initData, AdvL_ContextIndex* index)
{
    AdvL_TargetContext* context = NULL;
    AdvL_ContextIndex contextIndex = 0;

    // Pre-initialization for first run
    if (!defaultContext)
    {
		AdvL_ContextIndex i;
		
        // Reset contexts array
        for (i = 0;i < MAX_TARGET_CONTEXTS_NUMBER;++i)
            contextsArray[i] = NULL;

        // Allocate memory for first context
        context = init_context(0);

        // Set default context to first inited one
        defaultContext = context;

        // Initiate atexit for memory deallocation and closing various handles
        atexit(deinit_log_contexts);
    }
    else
    {
		AdvL_ContextIndex i;
		
        // Search for free context
        for (i = 0;i < MAX_TARGET_CONTEXTS_NUMBER;++i)
            if (contextsArray[i] == NULL)
            {
                context = init_context(i);
                contextIndex = i;
				
				break;
            }
		
		if(!context)
			return LogInitFailed_TooManyContexts;
    }


    // Initialization
    switch (target)
    {
    case LogTarget_Syslog:
        assert(initData != NULL);

        openlog(initData, LOG_NDELAY, LOG_USER);
        break;
    case LogTarget_File:
    {
        int _fileNameLength;
        assert(initData != NULL);

	    _fileNameLength = strlen(initData) + 1;

	    context->logFileName = (char*)malloc(_fileNameLength);
	    strncpy(context->logFileName, initData, _fileNameLength);

	    if ((context->logFile = fopen(context->logFileName, "a+")) == NULL)
	        return LogInitFailed_SystemError;
    }
    break;
	case LogTarget_Console:
		// No custom initialization needed
		break;	
    case LogTarget_Network:
    {
        assert(initData != NULL);

        // Create socket
        if ((context->netlogSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
            return LogInitFailed_NetworkError;

        // Split passed IP:port string by IP and port values
        {
			char defaultAddress[] = NET_DEFAULT_IP_ADDRESS;
			char userDefinedAddress[MAX_HOST_ADDRESS_LENGTH];
			
			char* _address;
			char* _portOffset;
			
            int _port;

            // Search for colon
			if((_portOffset = strchr(initData, ':')) != NULL)
			{
				strncpy(userDefinedAddress, initData, sizeof(userDefinedAddress));
				_portOffset = userDefinedAddress + (_portOffset - initData);
				
				// From the next character after colon port number should start
				_port = atoi(_portOffset + 1);

				// Set EOL to the colon position, now initData points to IP address only
				*_portOffset = '\0';
				
				// Set address
				_address = userDefinedAddress;
			}
			else
			{
				_address = defaultAddress;
				_port = atoi(initData);
			}
			
            context->socketAddress.sin_family = AF_INET;
            context->socketAddress.sin_port = htons(_port);
            if (!inet_aton(_address, &context->socketAddress.sin_addr))
                return LogInitFailed_NetworkError;
        }
    }
    break;

    case LogTarget_None:
    default:
        return LogInitFailed_WrongTarget;
    }

    context->logTarget = target;
	if (index) *index = contextIndex;

    return LogSuccess;
}

/**
 * @biref		This function is used to deinit log subsystem.
 *
 * @param[out]	index - context identifier which can be used to explicitly identify logging context
 *
 * @return		LogSuccess - if operation successfull or appropriate error code otherwise
 */
AdvL_LogError deinit_log(AdvL_ContextIndex index)
{
	assert(index < MAX_TARGET_CONTEXTS_NUMBER && contextsArray[index] != NULL);

	AdvL_TargetContext* context = contextsArray[index];

    switch (context->logTarget)
    {
    case LogTarget_Syslog:
        closelog();
        break;
    case LogTarget_File:
        if (context->logFile != NULL)
        {
            fclose(context->logFile);
            free(context->logFileName);
        }
        break;
    case LogTarget_Network:
        if (context->netlogSocket != -1)
            close(context->netlogSocket);
        break;
	case LogTarget_Console:
	case LogTarget_None:
	default:
		break;
    }
			
	// Deinit mutex
	pthread_mutex_destroy(&context->logMutex);
	
    free(context);
	contextsArray[index] = NULL;
    return LogSuccess;
}

/**
 * @brief		This function is used to set log level limit at runtime. No messages below the limit will be logged
 *
 * @param[in]	_index - logging context index, LogContext_Default for default
 * @param[in]	_logLevel - log level to limit messages verbosity. Messages of the log level itself will be shown
 */
void set_loglevel_limit(AdvL_ContextIndex index, AdvL_LogLevel logLevel)
{
    assert(index < MAX_TARGET_CONTEXTS_NUMBER && contextsArray[index] != NULL);
    contextsArray[index]->logLevelLimit = logLevel;
}

/**
 * @brief		This function is used to set file log rollover size
 * 
 * @param[in]	_index - logging context index, LogContext_Default for default
 * @param[in]	_rolloverSize - maximal size of log file
 */
void set_rollover_size(AdvL_ContextIndex index, unsigned long rolloverSize)
{
	assert(index < MAX_TARGET_CONTEXTS_NUMBER && contextsArray[index] != NULL);
	if(contextsArray[index]->logTarget != LogTarget_File || 
		  
		  rolloverSize > MAX_LOG_FILE_SIZE || 
		  rolloverSize < MIN_LOG_FILE_SIZE) return;
	
	contextsArray[index]->rolloverSize = rolloverSize;
}

/**
 * @brief		This function enables log level for specified log context
 * @param[in]	index - logging context index, LogContext_Default for default
 */
void enable_log(AdvL_ContextIndex index)
{
	assert(index < MAX_TARGET_CONTEXTS_NUMBER && contextsArray[index] != NULL);
	contextsArray[index]->logEnabled = true;
}

/**
 * @brief		This function disables log level for specified log context
 * @param[in]	index - logging context index, LogContext_Default for default
 */
void disable_log(AdvL_ContextIndex index)
{
	assert(index < MAX_TARGET_CONTEXTS_NUMBER && contextsArray[index] != NULL);
	contextsArray[index]->logEnabled = false;
}

/**
 * @brief		This is actual logging function called by support macroses such as log_message()
 *
 * @param[in]	index - logging context index, LogContext_Default for default
 * @param[in]	logLevel - log level
 * @param[in]	filePath - name of the current source file, supplied by __FILE__ macro
 * @param[in]	line - current line, supplied by __LINE__ macro
 * @param[in]	function name - current function, supplied by __func__ macro
 * @param[in]	facility - name of current logging subsystem
 * @param[in]	format - printf format string
 * @param[in]	... - variable number of parameters accroding to format parameter
 */
void write_log_message(AdvL_ContextIndex index, AdvL_LogLevel logLevel, const char* filePath, unsigned int line,
	const char* function_name, const char* facility, const char* format, ...)
{
    va_list argList;

    char userMessageBuffer[USER_MESSAGE_BUFFER_SIZE];
    char messageBuffer[MESSAGE_BUFFER_SIZE];
	
	size_t messageLength;

    AdvL_TargetContext* context = (index == LogContext_Default ? defaultContext : contextsArray[index]);
    assert(context != NULL);

    // Exit if log is disabled or the message is going out of the current log level limit
    if (!context->logEnabled || logLevel > context->logLevelLimit) return;

    va_start(argList, format);
/* Changed for testing     vsnprintf(userMessageBuffer, MESSAGE_BUFFER_SIZE, format, argList); */
    vsnprintf(userMessageBuffer, USER_MESSAGE_BUFFER_SIZE, format, argList);
    va_end(argList);
	
	// Suppress redundant '\n' characters (new line character is counted as reduntant, or that has been put by mistake, 
	// when it is last character of the line and previous character is not '\n', i.e. "\n\n" combination will remain uncorrected
	// 
	// If user want have real log lines spacing, it shall put more than one '\n' character at the end of the message to prove that
	// it was not mistake. Therefore, minimal line spacing will be two empty lines
	messageLength = strlen(userMessageBuffer);
	if(userMessageBuffer[messageLength - 1] == '\n' && userMessageBuffer[messageLength - 2] != '\n') userMessageBuffer[messageLength - 1] = '\0';

	pthread_mutex_lock(&context->logMutex);

	if (context->logTarget == LogTarget_Syslog)
    {
        syslog(LOG_USER | loglevel_to_sysloglevel(logLevel), syslogMessageFormat,
               facility, split_file_name_from_path(filePath), line,
               function_name, userMessageBuffer);
    }
	else if (context->logTarget == LogTarget_Console)
    {
        FILE* _consoleHandle = stdout;

        if (logLevel == LogFatal || logLevel == LogError)
            _consoleHandle = stderr;

        format_generic_message(messageBuffer, logLevel, filePath, line,
                function_name, facility, userMessageBuffer);
        fputs(messageBuffer, _consoleHandle);
    }
	else if (context->logTarget == LogTarget_File)
    {
        format_generic_message(messageBuffer, logLevel, filePath, line,
                function_name, facility, userMessageBuffer);
		write_and_roll_file(context, messageBuffer);
        /* hardcoded for console logging */
        FILE* _consoleHandle = stdout;

        if (logLevel == LogFatal || logLevel == LogError)
            _consoleHandle = stderr;

        format_generic_message(messageBuffer, logLevel, filePath, line,
                function_name, facility, userMessageBuffer);
        fputs(messageBuffer, _consoleHandle);
        /*********************************/
    }
	else if (context->logTarget == LogTarget_Network)
    {
		socklen_t addressSize = sizeof(context->socketAddress);
		
        format_generic_message(messageBuffer, logLevel, filePath, line,
                function_name, facility, userMessageBuffer);
		sendto(context->netlogSocket, messageBuffer, strlen(messageBuffer), 0, (struct sockaddr *)&context->socketAddress, addressSize);
    }

	pthread_mutex_unlock(&context->logMutex);
}

/**
 * @brief		This function writes raw message into log target (if supported)
 *
 * @param[in]	index - logging context index, LogContext_Default for default
 * @param[in]	format - printf format string
 * @param[in]	... - variable number of parameters accroding to format parameter
 */
void write_raw_message(AdvL_ContextIndex index, const char* format, ...)
{
    va_list argList;
    char messageBuffer[MESSAGE_BUFFER_SIZE];
	
	AdvL_TargetContext* context = (index == LogContext_Default ? defaultContext : contextsArray[index]);
	assert(context != NULL);
	
    // Syslog target are not supported for raw messages
	if (context->logTarget == LogTarget_Syslog) return;

    va_start(argList, format);
    vsnprintf(messageBuffer, MESSAGE_BUFFER_SIZE, format, argList);
    va_end(argList);

	pthread_mutex_lock(&context->logMutex);
	
	// Console
	if (context->logTarget == LogTarget_Console)
        fputs(messageBuffer, stdout);
    
	// File
	else if (context->logTarget == LogTarget_File)
        write_and_roll_file(context, messageBuffer);
	
	// Network
	else if (context->logTarget == LogTarget_Network)
		send(context->netlogSocket, messageBuffer, strlen(messageBuffer), 0);
    
	pthread_mutex_unlock(&context->logMutex);
}

