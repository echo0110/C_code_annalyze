/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: ltetail.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *   *  This file contains implementaion for unix tail type utility
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltetail.c,v $
 * Initial version
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <pthread.h>
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
#include "reader.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define DEFAULT_WAIT_TIMER 2

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

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
 * Function Name  : main
 * Inputs         : shared memory name, wait timer 
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function initialize reader framework and call
 *                  ltetail utility function.
 *****************************************************************************/
int main (SInt32 argc, Char8 **argv)
{
    unsigned int timer = 0;

    if((argc == 3) || (argc ==2))
    {
        if (argc == 3)
        {
            timer = atoi(argv[2]);
        }
        else
        {
            /* Default timer value */
            timer  = DEFAULT_WAIT_TIMER;
        }
        
        if(0 == reader_init(argv[1]))
        {
            lte_tail_data(timer);
        }
    }
    else
    {
        fprintf(stderr, "%s%s\n", "Invalid option...Usage: ",
                "ltetail <shared memory name> (optional,default=2sec)<wait timer(sec)>");
    }
    
    return 0;
}
