/*******************************************************************************
 *
 *  FILE NAME   : net_log.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include <stdio.h>
#include <errno.h>

#include <string.h>
#include "advlogging.h"

#define CORE_FACILITY 	"NetMessenger"
#define LOGS_COUNT		1024

/// Example of logging macro for specific facility
#define log_core_message(log_level, format, ...)\
write_log_message(LogContext_Default, log_level, __FILE__, __LINE__, __func__, CORE_FACILITY, format, ##__VA_ARGS__)

typedef struct
{
	AdvL_LogLevel level;
	char* message;
}
NetMessage;

static NetMessage messagesList[] = 
{
	{	LogInfo, "Nice, network logging example has started..."	},
	{	LogNotice, "Another useless message..."	},
	{	LogWarning, "Bark, bark... Good dog!" },
	{	LogError, "The truth is out threre..." },
	{	LogInfo, "Summer bunny, sporty and pretty :)" }
};


int main(int argc, char** argv)
{
	unsigned int i, count;

	if(argc != 2)
	{
		printf("Usage: net_log <IP:port>\n -- for example: net_log 127.0.0.1:7381\n");
		return 1;
	}

	if(init_log(LogTarget_Network, argv[1], NULL) != LogSuccess)
	{
		RRC_FPRINTF(stderr, "Unable to start network logging target: %s\n", strerror(errno));
		return 2;
	}
	
	for(count = 0;count < LOGS_COUNT;++count)
		for(i = 0;i < sizeof(messagesList)/sizeof(NetMessage);++i)
			log_core_message(messagesList[i].level, "%02d: %s", i, messagesList[i].message);
	
	return 0;
}


