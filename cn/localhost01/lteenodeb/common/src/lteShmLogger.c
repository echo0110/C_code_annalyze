/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteShmLogger.c,v 1.1.1.1.16.1 2010/10/25 10:32:08 gur23971 Exp $
 *
 ******************************************************************************
 *
 *  File Description : The file contains the necesssary definitions for
 *                     shared memory logger module.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteShmLogger.c,v $
 * Revision 1.1.1.1.16.1  2010/10/25 10:32:08  gur23971
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.15  2009/12/11 08:20:02  gur11912
 * modified for open sys call
 *
 * Revision 1.14  2009/12/02 07:24:34  gur19413
 * added UT_TESTING flag
 *
 * Revision 1.13  2009/09/02 12:09:40  gur20052
 * Commenting the printf
 *
 * Revision 1.12  2009/07/14 12:37:56  gur18569
 * create log file with time stamp
 *
 * Revision 1.11  2009/07/10 11:04:59  gur19413
 * removed compilation warnings
 *
 * Revision 1.10  2009/07/09 07:39:48  gur18569
 * opening shm per user
 *
 * Revision 1.9  2009/07/08 11:09:37  gur18569
 * changes regarding file permissions
 *
 * Revision 1.8  2009/07/08 09:04:49  gur18569
 * changes regarding num of msgs to write
 *
 * Revision 1.6  2009/06/30 14:07:55  gur18569
 * taking log string length as strlen(buffer)+1
 *
 * Revision 1.5  2009/06/28 07:18:00  gur18569
 * changes in read/write logic
 *
 * Revision 1.4  2009/06/03 05:18:11  gur18569
 * fixed error whn calling lteShmLog before registering
 *
 * Revision 1.3  2009/05/27 15:10:04  gur18569
 * fixed bugs
 *
 * Revision 1.2  2009/05/25 09:40:17  gur18569
 * removed debug printfs
 *
 * Revision 1.1  2009/05/25 09:31:50  gur18569
 * "initial version"
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include <sys/mman.h>
#include <sys/timeb.h>
#include <signal.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif



/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include <lteMisc.h>
#include "lteShmLogger.h"
#include "logging.h"
#include "lteLog.h"
#include "lteSync.h"
#include "lteThread.h"
#include "shm_utils.h"

#include<lteMacComPorting.h>
/******************************************************************************
 Private Definitions
 *****************************************************************************/



/******************************************************************************
 Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* GCC warning Fix Start */
extern void enable_log_level(UInt32 logLevel);
/* GCC warning Fix End */
/* key for local storage for each thread registering for shm logging*/
static pthread_key_t   lteTLSKey = 0;

/*Keeps the info of the thread registered for logging */
static logRegisterInfo shmLogRegArray_g[MAX_REGISTERED_MODULES];

/* This semaphore is necessary to prevent race conditions while multiple
 * parallel registrations*/
static LTE_SEM shmLogRegSem_g;

/* Maintaines the count of total number of registrations*/
static UInt32 shmLogRegArrayCount_g;


UInt8 finishedDumpingArr_g[MAX_REGISTERED_MODULES]; 

UInt8 stopLogging_g;

void dumpLogBeforeExiting(int param)
{
    UInt8 regReqCount = 0;

    stopLogging_g = 1;

    /* +- SPR 17777 */
    fprintf(stderr,"%s invoked param[%d] \n",__func__,param);
    /* +- SPR 17777 */
    for(regReqCount = 0; regReqCount < shmLogRegArrayCount_g; regReqCount++)
    {
        while(!finishedDumpingArr_g[regReqCount]);
    }
    exit(0);
}


/*****************************************************************************
 * Function Name  : initShmLogger
 * Inputs         : 
 * Outputs        : 
 * Returns        : 0 - On success.
 *                  1 - On failure.
 * Description    : This function initializes the structures required for 
 *                  shm logging feature to work.The function also creates 
 *                  shm logger thread.
 *****************************************************************************/
/* + SPR 17439 */
UInt32 initShmLogger(void)
/* - SPR 17439 */    
{
    static UInt8 initOnce;

    if (initOnce)
    {
        return 1;
    }
    if (0  != pthread_key_create(&lteTLSKey,NULL))
    {
        PERROR("pthread_key_create");
        return 1;
    }
    if (-1 == semInit(&shmLogRegSem_g, 1))
    {
        PERROR("sem_init");
        return 1;
    }
    shmLogRegArrayCount_g = 0;

    initOnce = 1;
    
    return 0;
}

/*****************************************************************************
 * Function Name  : resetShmLogger
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function resets the global required by shm logger.
 *                  To be called if we want to reset shm logger.
 *****************************************************************************/
/* + SPR 17439 */
void resetShmLogger(void)
/* - SPR 17439 */
{
    UInt32 regReqCount = 0;
    Char8 moduleName[MAX_MODULE_NAME_SIZE];


    regReqCount = shmLogRegArrayCount_g;
    shmLogRegArrayCount_g = 0;

    while (regReqCount)
    {
        if (shmLogRegArray_g[regReqCount].fd != 0)
        {

		/*Coverity 99392 Fix Start*/ 
		if(SOCKET_ERROR == close_wrapper(shmLogRegArray_g[regReqCount].fd))
		{
			LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
					0,\
					__LINE__,0,0,0,0,\
					0.0,0.0,__FILE__,__func__);
		}
		/*Coverity 99392 Fix End*/

	}
        /* +COVERITY 10250 */
        snprintf(moduleName,MAX_MODULE_NAME_SIZE,"/%s",shmLogRegArray_g[regReqCount].
                                          registeredModuleName);
        /* -COVERITY 10250 */
        shm_unlink(moduleName);
        regReqCount--;
    }
}


/*****************************************************************************
 * Function Name  : loggerCleanupHandler
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Description    : This function is called upon cancellation of logger thread.
 *****************************************************************************/
void loggerCleanupHandler(void * param)
{
    /* +- SPR 17777 */
/*SPR 20636 Changes Start*/
    fprintf(stderr,"logger clean up handler called %zu \n",sizeof(param));
/*SPR 20636 Changes End*/
    /* +- SPR 17777 */
}

/*****************************************************************************
 * Function Name  : lteShmSetModuleLogLevel
 * Inputs         : moduleId - The Index of the module for which log level is
 *                             being set.
 *                  level - value of log level.
 * Outputs        : 
 * Returns        : 0 - success. 
 *                  1 - failure.
 * Description    : This function is called to set log level for a certain 
 *                  module.
 *****************************************************************************/
UInt32 lteShmSetModuleLogLevel(UInt32 moduleId , UInt32 level)
{
    /* SPR 4010 Fix start */
    if (moduleId >= TOTAL_NUM_OF_MODULES)
    /* SPR 4010 Fix End */
    {
        fprintf(stderr,"Module Id is not valid (max Valid id : %d)\n",
                                                 TOTAL_NUM_OF_MODULES);
        return 1;
    }
    /* + SPR_10412 */ 
    else
    {
      enable_log_level(level);  
    }    
    /* - SPR_10412 */ 

    return 0;
}
