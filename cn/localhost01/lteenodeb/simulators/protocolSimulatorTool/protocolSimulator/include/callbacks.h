/************************************************************************************
*
*   FILE NAME  : callbacks.h
*
*   DESCRIPTION: External scheduler general callbacks definition.
                 "exit", "sleep", "runscript" command callbacks definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

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

/************************************************************************************
 * FUNCTION NAME    :   userSetLogLevel
 * DESCRIPTION      :   Sets log level
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSetLogLevel(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userTestCallback
 * DESCRIPTION      :   "test" callback
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userTestCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userIfFlowCallback
 * DESCRIPTION      :   "if" processor
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 *                      [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_n
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userIfFlowCallback(UInt32 argc, const UChar8** argv,
    UChar8** string_list,
    UInt32 *p_line_no
);

/************************************************************************************
 * FUNCTION NAME    :   userGotoFlowCallback
 * DESCRIPTION      :   "goto" processor
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 *                      [IN] UChar8** string_list
 *                      [IN/OUT]UInt32 *p_line_n
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userGotoFlowCallback(UInt32 argc, const UChar8** argv,
    UChar8** string_list,
    UInt32 *p_line_no
);

#endif /*__CALLBACKS_H__*/
