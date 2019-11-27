/*******************************************************************************
 *
 *  FILE NAME   : netlogex.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/stat.h>

#include <errno.h>

#include <signal.h>

#include "advlogging.h"


#define CORE_FACILITY "NetLog"

#define log_core_message(log_level, format, ...)\
write_log_message(LogContext_Default, log_level, __FILE__, __LINE__, __func__, CORE_FACILITY, format, ##__VA_ARGS__)

#define MAX_NUMBER_OF_PENDING_CONNECTIONS		20
#define MAX_ERRNO_DESCRIPTION_LENGTH			512
#define MAX_MESSAGE_BUFFER_LENGTH				1024
#define MAX_HOST_ADDRESS_LENGTH					16

/// Global data
static AdvL_ContextIndex logFileContext = LogContext_None;
static AdvL_ContextIndex networkRepeaterContext = LogContext_None;

static void HandleLastError(const char* description, bool fatal)
{
	char errorBuffer[MAX_ERRNO_DESCRIPTION_LENGTH], *errorPtr = NULL;

	// Note: According to man page, the GNU version of strerror_r should return pointer to the error message, 
	// which may not be the pointer to the buffer that we have sent to it (but some predefined message). So 
	// this is not useless type definition
	
	// Note 2: This restricts our portability. A macroguard should be placed here to check which version of 
	// function use - GNU or XSI-compliant
	errorPtr = (char *)strerror_r(errno, errorBuffer, MAX_ERRNO_DESCRIPTION_LENGTH);
	
	if(fatal)
	{
		log_core_message(LogFatal, "Fatal error: %s (%d): %s", description, errno, errorPtr != NULL ? errorPtr : errorBuffer);
		RRC_EXIT(errno);
	}
	
	log_core_message(LogError, "Error: %s (%d): %s", description, errno, errorPtr != NULL ? errorPtr : errorBuffer);
}

static AdvL_LogError InitLogTargets(char *logFileName, char* repeatTo, long rolloverSize)
{
	AdvL_LogError rc = LogInitFailed_WrongTarget;
	
	if(init_log(LogTarget_File, logFileName, &logFileContext) != LogSuccess)
	{
		// Here we assume, that application is still not in daemon mode
		RRC_FPRINTF(stderr, "Unable open log file: %s\n", strerror(errno));
		return rc;
	}
	
	set_rollover_size(logFileContext, rolloverSize);
	
	if(repeatTo != NULL && (rc = init_log(LogTarget_Network, repeatTo, &networkRepeaterContext)) != LogSuccess)
	{
		log_core_message(LogFatal, "Unable init network repeater: %s\n", strerror(errno));
		return rc;
	}
	
	return LogSuccess;
}

static void WriteLogMessage(const char* messageBuffer)
{
	const char* formatStr = "%s";
	
	// Write message into log contexts
	write_raw_message(logFileContext, formatStr, messageBuffer);
	
	if(networkRepeaterContext != LogContext_None)
		write_raw_message(networkRepeaterContext, formatStr, messageBuffer);
}

static void WriteLogMessageEx(const struct sockaddr_in* clientSocketAddress, const char* messageBuffer)
{
	const char* formatStr = "%s:%u %s";
	char hostAddressStr[MAX_HOST_ADDRESS_LENGTH];
	
	if(!inet_ntop_wrapper(AF_INET, (const void *)&clientSocketAddress->sin_addr, hostAddressStr, sizeof(hostAddressStr)))
	{
		HandleLastError("Cannot receive socket address string", false);
		
		WriteLogMessage(messageBuffer);
		return;
	}
	
	// Write message into log contexts
	write_raw_message(logFileContext, formatStr, hostAddressStr, RRC_NTOHS(clientSocketAddress->sin_port), messageBuffer);
	
	if(networkRepeaterContext != LogContext_None)
		write_raw_message(networkRepeaterContext, formatStr, hostAddressStr, RRC_NTOHS(clientSocketAddress->sin_port), messageBuffer);
}

static void SignalHandler(int signal)
{
	switch(signal)
	{
		case SIGTERM:
			log_core_message(LogWarning, "### Termination signal received");
			RRC_EXIT(EXIT_SUCCESS);
			
		case SIGSEGV:
			log_core_message(LogFatal, "### Segmentation Fault!");
            RRC_ABORT()
			
		case SIGFPE:
			log_core_message(LogFatal, "### Floating point exception raised!");
            RRC_ABORT()
	}
}

static void Usage()
{
	RRC_FPRINTF(stdout,"\nUsage:\n"
			"  netlogexd --port=number [--log-file=file_name] [--repeat-to=destination:port] [--trace] [--rollover-size=size]\n"
			"\nOptions:\n"
			"  %-30s%s\n"
			"  %-30s%s\n"
			"  %-30s%s\n"
			"  %-30s%s\n"
			"  %-30s%s\n"
   			"\nExamples:\n"
			"  netlogexd --port=7381 --log-file=/var/log/netlogexd.log\n"
			"  netlogexd --port=7381 --log-file=./netlogexd.log --repeat-to=192.168.1.1:7381\n"
			"  netlogexd --port=7381 --log-file=./netlogexd.log --trace\n"
			"  netlogexd --port=7381 --log-file=./netlogexd.log --rollover-size=1048576"
			"\n",
				  		
			"--port", "port number to listen for incomming connections",
			"--log-file", "file name for the log file",
   			"--repeat-to", "write log file and repeat message to another netlogd daemon",
	  		"--trace", "do not daemonize the network logger",
	 		"--rollover-size", "maximum size of log file in bytes. When log file size reaches this value, "
					"netlogexd renames it, appends timestamp and opens new instance"
			);
}

int main(int argc, char** argv)
{
	int socketDescriptor;
	struct sockaddr_in socketAddress;
	
	// Daemon variables
	pid_t pid, sid;

	// TCP/IP options
	int reuseAddress = 1;
	
	// Command line options
	int port = -1;
	bool trace = false;
	char* logFileName = NULL;
	char* repeatTo = NULL;
	long rolloverSize = -1;

	// /////////////// /////////////// /////////////// /////////////// ///////////////
	// Generic initialization
	
	// Parsing command line parameters
	while(1)
	{
		static struct option commandLineOptions[] = 
		{
			{"help", 0, 0, 'h'},
			{"port", 1, 0, 'p'},
			{"log-file", 1, 0, 'l'},
			{"trace", 0, 0, 't'},
			{"repeat-to", 1, 0, 'r'},
			{"rollover-size", 1, 0, 's'},
		};

		int option_index = 0;
		int arg_val = getopt_long(argc, argv, "", commandLineOptions, &option_index);
			
		if(arg_val == -1) break;

		switch(arg_val)
		{
			case 'h':
			case '?':
				Usage();
				return EXIT_FAILURE;
			case 'p':
				port = atoi(optarg);
				break;
			case 'l':
				logFileName = optarg;			// It's safe, since optarg points to argv[] array
				break;
			case 't':
				trace = true;
				break;
			case 'r':
				repeatTo = optarg;
				break;
			case 's':
				rolloverSize = atol(optarg);
				break;
		}
	}
	
	// Check required parameters
	if(port == -1 || logFileName == NULL)
	{
		RRC_FPRINTF(stdout,"You must specify both --port and --log-file command line options:\n");

		Usage();
		return EXIT_FAILURE;
	}


	// /////////////// /////////////// /////////////// /////////////// ///////////////
	// Daemon initialization stage
	
	if(!trace)
	{
		pid = fork();

		// Fork failed
		if(pid < 0)
		{
			RRC_FPRINTF(stderr, "Fatal error. Cannot fork process: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
	
		// Parent thread
		else if(pid > 0)
		{
			// Write child process PID file. If we don't have access rights to /var/run directory, 
			// just skip it, this is not mandatory
			const char* pidFileName="/var/run/netlogexd.pid";
			FILE* pidFile = RRC_FOPEN(pidFileName, "w+");
			
			if(pidFile != NULL)
			{
				RRC_FPRINTF(pidFile, "%lu\n", (unsigned long)pid);
				rrc_fclose(pidFile);
			}
			
			return EXIT_SUCCESS;
		}
		
		// Client thread
		
		// Init log targets
		if(InitLogTargets(logFileName, repeatTo, rolloverSize) != LogSuccess)
			return EXIT_FAILURE;
		

		// Change file mode mask
		umask(0);

		// Create new session ID
		sid = setsid();
		if(sid < 0)
		{
			log_core_message(LogFatal, "Cannot get session ID: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}


		// Close standard file descriptors
		close_wrapper(STDIN_FILENO);
		close_wrapper(STDOUT_FILENO);
		close_wrapper(STDERR_FILENO);

		// Catch signals
		signal(SIGHUP, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGABRT, SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
		signal(SIGALRM, SIG_IGN);
		signal(SIGUSR1, SIG_IGN);
		signal(SIGUSR2, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);
		signal(SIGCONT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);

		signal(SIGTERM, SignalHandler);
		signal(SIGSEGV, SignalHandler);
		signal(SIGFPE, SignalHandler);
	}
	else
	{
		// Daemon initialization need its logger at earlier stages, but inside of child thread
		// Here goes initialization for no-daemon mode
		if(InitLogTargets(logFileName, repeatTo, rolloverSize) != LogSuccess)
			return EXIT_FAILURE;
	}

	// /////////////// /////////////// /////////////// /////////////// ///////////////
	// Network initialization part
	
	// Create socket
	if((socketDescriptor = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) == -1)
		HandleLastError("Cannot create socket", true);
	
	log_core_message(LogInfo, "Socket creation successul...");

	// Bind docket
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = RRC_HTONS(port);
	socketAddress.sin_addr.s_addr = INADDR_ANY;
	
	if(setsockopt_wrapper(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) == -1)
		HandleLastError("Cannot set socket options", false);

	if(bind(socketDescriptor, (struct sockaddr* )&socketAddress, sizeof(socketAddress)) == -1)
		HandleLastError("Cannot bind socket to port", true);
	
	log_core_message(LogInfo, "Socket binded to port %d...", port);
	
	while(true)
	{
		char messageBuffer[MAX_MESSAGE_BUFFER_LENGTH];
		socklen_t addressSize = sizeof(socketAddress);
		
		int bytesRead = recvfrom_wrapper(socketDescriptor, messageBuffer, MAX_MESSAGE_BUFFER_LENGTH, 0, (struct sockaddr *)&socketAddress, &addressSize);
		
		if(bytesRead < 0)
		{
			HandleLastError("Cannot receive log data", errno == EBADF ? true : false);
			continue;
		}
		
		// Set EOL constant
		messageBuffer[bytesRead] = '\0';
		
		WriteLogMessage(messageBuffer);
	}
	
	return EXIT_SUCCESS;
}

