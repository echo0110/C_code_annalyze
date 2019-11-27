/************************************************************************************
*
*   FILE NAME  : sock_tools.h
*
*   DESCRIPTION: functions for transferring messages via UDP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef _SOCK_TOOLS_H_
#define _SOCK_TOOLS_H_

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

typedef enum {
    SIM_SOCKET_TYPE_UDP,
    SIM_SOCKET_TYPE_SCTP,

    SIM_SOCKET_TYPE_LAST
} sim_socket_type_et;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : InitSockets
   PARAMATERS     : UChar8 *fn - file name of config file
 * Outputs        :
 * Returns        : Returns 0 or SOCKET_ERROR
 * Variables      :
 * Description    : init sockets unit.
 ****************************************************************************/
SocketType InitSockets(UChar8 *fn);

/************************************************************************************
 * FUNCTION NAME    : CloseSockets
 * DESCRIPTION      : Closes sockets and deinitializes socket engine.
                      No other socket functions should be called after call to CloseSockets.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void CloseSockets(void);

/************************************************************************************
 * FUNCTION NAME    : ReadFromSocket
 * DESCRIPTION      : Reads message from RX UDP socket for given moduleId
 * PARAMATERS       : sock_type,
 *                    buf - data from socket will be placed here. Should have SOCKET_BUFFER_SIZE length.
 *                    SInt32 timeout
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 ReadFromSocket(sim_socket_type_et sock_type, UInt8 *buf,SInt32);

/************************************************************************************
 * FUNCTION NAME    : WriteToSocket
 * DESCRIPTION      : Writes information from buffer with length bufferLen to UDP socket
 * PARAMATERS       : sock_type,buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 WriteToSocket(sim_socket_type_et sock_type, UInt8 *buffer, UInt32 bufferLen);

/************************************************************************************
 * FUNCTION NAME    : WaitForConnectionOnSocket
 * DESCRIPTION      : Waits and accepts connection on socket
 * PARAMATERS       : sock_type
 * RETURN VALUE     : Returns 0 or SOCKET_ERROR
 ************************************************************************************/
UInt32 WaitForConnectionOnSocket(sim_socket_type_et sock_type);

#endif /* _SOCK_TOOLS_H_ */
