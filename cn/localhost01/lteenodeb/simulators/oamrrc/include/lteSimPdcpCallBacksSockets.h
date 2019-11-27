
/************************************************************************************
*
*   FILE NAME  : callbacksSockets.h 
*
*   DESCRIPTION: External scheduler callbacks implementation for sockets related  commands.
                 "loadtosock", "sendandcheck", "sendcmd" - command callbacks definitions
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

#ifndef __CALLBACKS_SOCKETS_H__
#define __CALLBACKS_SOCKETS_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "lteTypes.h"

/************************************************************************************
 * FUNCTION NAME    :   userRRC2SockCallback
 * DESCRIPTION      :   RRC callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userToSockCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSendAndCheckFileCallback
 * DESCRIPTION      :   "sendandcheck" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendAndCheckFileCallback(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   userSendCommand
 * DESCRIPTION      :   Send command line to PDCP host
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendCommand(UInt32 argc, const UChar8** argv);

/************************************************************************************
 * FUNCTION NAME    :   sendSrbCallback
 * DESCRIPTION      :   send srb data to SRB PDCP Entity
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 sendSrbCallback(UInt32 argc, const UChar8 **argv);

#endif /*__CALLBACKS_SOCKETS_H__*/
