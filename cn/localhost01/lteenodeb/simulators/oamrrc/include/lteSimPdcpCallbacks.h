
/************************************************************************************
*
*   FILE NAME  : callbacks.h
*
*   DESCRIPTION: External scheduler general callbacks definition.
                 "exit", "sleep", "runscript" command callbacks definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <lteTypes.h>

/************************************************************************************
 * Global variable
 ************************************************************************************/
extern Int isTerminated;

/************************************************************************************
 * FUNCTION NAME    :   userRunScriptCallback
 * DESCRIPTION      :   Run script callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userRunScriptCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSleepCallback
 * DESCRIPTION      :   Sleep
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSleepCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userExitCallback
 * DESCRIPTION      :   Exit ext scheduler
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userExitCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userExitCallback
 * DESCRIPTION      :   Echo user message
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userEchoCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userRedirectLogCallback
 * DESCRIPTION      :   Redirects log output from stderr to file 
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userRedirectLogCallback(UInt32 argc, const UChar8** argv);
UInt32 userDisplayStatsPeriodicCallback(UInt32 argc,const UChar8** argv);
UInt32 userStopDisplayStats(UInt32 argc,const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSetLogLevel
 * DESCRIPTION      :   Sets log level 
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetLogLevel(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userPrintMemPoolState
 * DESCRIPTION      :   Prints out Memory pool state 
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userPrintMemPoolState(UInt32 argc, const UChar8** argv);
/************************************************************************************
 * FUNCTION NAME    :   userGetThroughput
 * DESCRIPTION      :   This function is used to get the throughput
 * PARAMATERS       :   None
 * RETURN VALUE     :   None
 ************************************************************************************/
void userGetThroughput(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userGetCPU
 * DESCRIPTION      :   This function is used to get the CPU Utilization
 * PARAMATERS       :   None
 * RETURN VALUE     :   None
 ************************************************************************************/
void userGetCPU(UInt32 argc, const UChar8** argv);

#endif /*__CALLBACKS_H__*/
