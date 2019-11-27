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
/*Included for struct timeval*/
#include <times.h>
#include <sockLib.h>
#ifdef __cplusplus
extern "C"
{
#endif
    extern  int     sysClkRateGet( void );
#ifndef SYS_CLKRATE
#define SYS_CLKRATE sysClkRateGet()
#endif

#undef  isxdigit
#undef isprint
#ifdef __cplusplus
}
#endif
