/*******************************************************************************
 *
 *  FILE NAME   : console_log.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include <stdio.h>
#include "advlogging.h"

#define CORE_FACILITY "Core"

/// Example of logging macro for specific facility
#define log_core_message(log_level, format, ...)\
write_log_message(LogContext_Default, log_level, __FILE__, __LINE__, __func__, CORE_FACILITY, format, ##__VA_ARGS__)

void pass_log_messages()
{
	log_core_message(LogTrace, "Trace message");
	log_core_message(LogDebug, "Debug message, integer = %02d, floating-point = %0.3f, substring \"%s\"", 217, 22.347, "_target_substring_");
	log_core_message(LogDetailed, "Detailed debug message");
	log_core_message(LogBrief, "Brief debug message");
	log_core_message(LogInfo, "Info message test");
	log_core_message(LogNotice, "Notice message test");
	log_core_message(LogWarning, "Warning message test");
	log_core_message(LogError, "Error message test");
	log_core_message(LogFatal, "Fatal message test");
	log_core_message(LogAlert, "Alert!!!");
	log_core_message(LogEmergency, " ### System failure");
}

int main()
{
	AdvL_ContextIndex contextIndex;
	
	if(init_log(LogTarget_Console, NULL, &contextIndex) != LogSuccess)
	{
		RRC_FPRINTF(stderr, "Unable to initialize console log target!");
		return -1;
	}
	
	log_core_message(LogInfo, "Console logging example started \n\nPassing not limited messages\n");
	pass_log_messages();

	log_core_message(LogWarning, "\nStage 2 \n\nThe same messages limited to notice level\n");
	
	set_loglevel_limit(contextIndex, LogNotice);
	pass_log_messages();
	
	log_core_message(LogWarning, "\nStage 3 \n\nDisable logs at all\n");
	
	disable_log(contextIndex);
	pass_log_messages();
	
	enable_log(contextIndex);
	log_core_message(LogWarning, "Now you should see the log message again\n");
	
	return 0;
}

