/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: signal_handler.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of signal handler and wrappers to 
 *  to register signals.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: signal_handler.c,v $
 * Initial version
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include <errno.h>
#include <sys/types.h> 
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <lteMisc.h>
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
#include "signal_handler.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void signal_handler(int sig_num);


/*****************************************************************************
 * Private Constants
 ****************************************************************************/




/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : signal_handler
 * Inputs         : signal_number 
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used to generate process specific logs
 *****************************************************************************/
static void signal_handler(
        int sig_num)
{
    pthread_t tid = pthread_self();
    pid_t pid = getpid_wrapper();
    char stacktrc_fname[256];
    char cmdline[256];


	snprintf (stacktrc_fname,sizeof(stacktrc_fname),"/tmp/stack.%d", pid);
    snprintf (cmdline,sizeof(cmdline), "/bin/date > %s", stacktrc_fname);
    system (cmdline);
    
    /* Put signal that was caught into the stack file*/
    snprintf (cmdline,sizeof(cmdline),"/bin/echo logging stopped by signal %d in thread %ld >> %s", sig_num, tid, stacktrc_fname);
    system (cmdline);

    /*Print cpu usage, memeory, elapased time and complete arguments into stack file*/
    system (cmdline);
    
    /* Print heap size into stack file */
    snprintf (cmdline,sizeof(cmdline), "/usr/bin/pmap %d >> %s", pid, stacktrc_fname);
    system (cmdline);
    
    /* Print stack trace*/
    snprintf (cmdline, sizeof(cmdline),"/usr/bin/pstack %d >> %s", pid, stacktrc_fname);
    system (cmdline);
    
    /* Print cpu usage, memeory, elapased time and complete arguments into stack file */
    snprintf (cmdline,sizeof(cmdline), "/bin/ps -o pcpu,pmem,vsz,etime,args -p %d >> %s", pid, stacktrc_fname);
    system (cmdline);
 
    /* Produce core file */
    snprintf (cmdline,sizeof(cmdline), "gcore -o /tmp/core %d",pid);
    system (cmdline);

    exit(1);
}
/*****************************************************************************
 * Function Name  : register_signal_handler
 * Inputs         : signal_num, signal handler
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE
 * Variables      : None.
 * Description    : Used for register new signal handler for a signal.
 *****************************************************************************/
UInt32 register_signal_handler(
        UInt32 sig_num, 
        sig_handler handler)
{
    if(SIG_ERR == signal_wrapper(sig_num,handler))
    {
        return LOGGER_FAILURE;
    }
    return LOGGER_SUCCESS;
}

/*****************************************************************************
 * Function Name  : register_all_signal_handler
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE
 * Variables      : None.
 * Description    : Used to register all signals for signal handler
 *****************************************************************************/
/* + SPR 17439 */
UInt32 register_all_signal_handler(void)
/* - SPR 17439 */    
{
    if(LOGGER_SUCCESS == register_signal_handler(SIGILL, signal_handler))
    {
        if(LOGGER_SUCCESS == register_signal_handler(SIGABRT, signal_handler))
        {
            if(LOGGER_SUCCESS == register_signal_handler(SIGFPE, signal_handler))
            {
                if(LOGGER_SUCCESS == register_signal_handler(SIGBUS, signal_handler))
                {
                    if(LOGGER_SUCCESS == register_signal_handler(SIGSEGV, signal_handler))
                    {
                        if(LOGGER_SUCCESS == register_signal_handler(SIGSYS, signal_handler))
                        {
                            return LOGGER_SUCCESS;
                        }
                    }
                }
            }
        }
    }
    return LOGGER_FAILURE;
}

/*****************************************************************************
 * Function Name  : modify_core_path
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.          
 * Variables      : None.
 * Description    : Used to modify core file path and pattern
 *****************************************************************************/
/* + SPR 17439 */
void modify_core_path(void)
/* - SPR 17439 */    
{
    system ("sysctl -w kernel.core_pattern=/tmp/core_%e.%p.%t");
}
