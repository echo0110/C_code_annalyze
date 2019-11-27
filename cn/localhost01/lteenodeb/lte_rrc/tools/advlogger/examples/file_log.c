/*******************************************************************************
 *
 *  FILE NAME   : file_log.c
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

int main()
{
	init_log(LogTarget_File, "./file_log_test.log", NULL);
	log_core_message(LogInfo, "File logging example started");
	
	return 0;
}

