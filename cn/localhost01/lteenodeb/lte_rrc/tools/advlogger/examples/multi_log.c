/*******************************************************************************
 *
 *  FILE NAME   : multi_log.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include <stdio.h>
#include "advlogging.h"

#define CORE_FACILITY "MultiLog"

/// Example of logging macro for specific facility
#define log_core_message(log_level, format, ...) \
{ \
	write_log_message(fileLogContext, log_level, __FILE__, __LINE__, __func__, CORE_FACILITY, format, ##__VA_ARGS__); \
	write_log_message(syslogContext, log_level, __FILE__, __LINE__, __func__, CORE_FACILITY, format, ##__VA_ARGS__); \
}

static AdvL_ContextIndex fileLogContext;
static AdvL_ContextIndex syslogContext;

int main()
{
	init_log(LogTarget_File, "./multi_log_test.log", &fileLogContext);
	
	log_core_message(LogInfo, "Multi logging example started. This message goes only into file log target");
	
	init_log(LogTarget_Syslog, "Mult-log test", &syslogContext);
	log_core_message(LogInfo, "Syslog logging target has started");
	
	set_loglevel_limit(syslogContext, LogWarning);
	log_core_message(LogWarning, "Since this moment, only Fatal, Error and Warning messages will go to syslog target");
	
	log_core_message(LogDebug, "Some additional debug information...");
	log_core_message(LogError, "Error message - testing of syslog log level filter");

	return 0;
}
