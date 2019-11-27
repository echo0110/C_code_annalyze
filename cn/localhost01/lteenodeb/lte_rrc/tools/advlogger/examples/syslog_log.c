/*******************************************************************************
 *
 *  FILE NAME   : syslog_log.c
 *
 *  DESCRIPTION : 
 *
 *  Copyright 2009, Aricent Inc.
 *
 *******************************************************************************/
#include <stdio.h>
#include "advlogging.h"

int main()
{
	init_log(LogTarget_Syslog, "LTE 3G Syslog logging test", NULL);

	log_message(LogInfo, "Core", "Syslog logging example started");
	return 0;
}

