/************************************************************************************
*
*   FILE NAME  : callbacksSockets.h
*
*   DESCRIPTION: External scheduler callbacks implementation for sockets related  commands.
                 "loadtosock", "sendandcheck", "sendcmd" - command callbacks definitions
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __CALLBACKS_SOCKETS_H__
#define __CALLBACKS_SOCKETS_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

/****************************************************************************
 * Global Variables
 ****************************************************************************/

extern Int isReadSocketTaskEnabled;

/************************************************************************************
 * FUNCTION NAME    :   userToSockCallback
 * DESCRIPTION      :   userToSock callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userToSockCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userToSockSctpCallback
 * DESCRIPTION      :   userToSock_Sctp callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userToSockSctpCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSendAndCheckFileCallback
 * DESCRIPTION      :   "sendandcheck" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendAndCheckFileCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSendAndCheckFileSctpCallback
 * DESCRIPTION      :   "sendandcheck_sctp" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendAndCheckFileSctpCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userListenSockCallback
 * DESCRIPTION      :   "listensock" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userListenSockCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userListenAndAccept
 * DESCRIPTION      :   "listenandaccept" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userListenAndAccept(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userWaitResetSctp
 * DESCRIPTION      :   "waitReset_sctp" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userWaitResetSctp(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userEmptySocksCallback
 * DESCRIPTION      :   "emptySocks" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userEmptySocksCallback(UInt32 argc, const UChar8** argv);

#endif /*__CALLBACKS_SOCKETS_H__*/
