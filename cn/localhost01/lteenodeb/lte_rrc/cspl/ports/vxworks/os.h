/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
#include <vxWorks.h>

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <msgQLib.h>
#include <tickLib.h>
#include <time.h>
#include <sockLib.h>
#include <unistd.h>
#include <ioLib.h>

extern	int		sysClkRateGet(void);
#ifndef	SYS_CLKRATE
#define	SYS_CLKRATE	sysClkRateGet()
#endif

#undef	isxdigit
#undef isprint
