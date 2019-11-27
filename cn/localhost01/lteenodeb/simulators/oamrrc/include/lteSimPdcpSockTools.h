/************************************************************************************
*
*   FILE NAME  : sock_tools.h
*
*   DESCRIPTION: functions for transferring messages via UDP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

#ifndef _SOCK_TOOLS_H_
#define _SOCK_TOOLS_H_

/************************************************************************************
 * Includes
 ************************************************************************************/

#ifdef WIN32
    #include <winsock.h>
    typedef SOCKET SocketType;
    #define SOCKET_FLAGS 0
	#define SOCKET_FLAGS_WAIT 0
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>

    typedef Int SocketType;
    #define SOCKET_ERROR (-1)
    #define SOCKET_FLAGS MSG_DONTWAIT
	#define SOCKET_FLAGS_WAIT 0
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define SOCKET_BUFFER_SIZE	(4*1024)
#define HOSTNAME_MAX_LEN	64
#define PDCP_MODULE_CMD_ID	0xFFFF

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    : InitSocketSys  
 * DESCRIPTION      : Performs initialization of the sockets engine. Should be called 
					  on program startup before using other socket functions
 * PARAMATERS       :   
 * RETURN VALUE     : status (SUCCESS/FAIL)
 ************************************************************************************/
UInt8 InitSocketSys(void);

/************************************************************************************
 * FUNCTION NAME    : CloseSockets
 * DESCRIPTION      : Closes sockets and deinitializes socket engine.
					  No other socket functions should be called after call to CloseSockets.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void CloseSockets(void);

/****************************************************************************
 * Function Name  : InitTxSocket
 * Inputs         : moduleId - module Id
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : init TX socket for given moduleId.
 ****************************************************************************/
SocketType InitTxSocket(Int moduleId);

/****************************************************************************
 * Function Name  : InitTxSocket
 * Inputs         : moduleId - module Id, isBlocking
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : init RX socket for given moduleId.
 ****************************************************************************/
SocketType InitRxSocket(Int moduleId, UInt8 isBlocking);

/************************************************************************************
 * FUNCTION NAME    : ReadFromSocket  
 * DESCRIPTION      : Reads message from RX UDP socket for given moduleId
 * PARAMATERS       : buf - data from socket will be placed here. Should have RX_BUFFER_SIZE length.
					  moduleId - determines module to read from RX socket
 * RETURN VALUE     : Length of read message.
					  If there is no message it returns 0 (no wait for a message).
					  If message length != TLV header length it returns 0.
 ************************************************************************************/
UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId );

/************************************************************************************
 * FUNCTION NAME    : WriteToSocket
 * DESCRIPTION      : Writes information from buffer with length bufferLen to UDP socket
 * PARAMATERS       : buffer,bufferLen
					  moduleId - determines module to write to TX socket
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 WriteToSocket(UInt8 *buffer, UInt32 bufferLen, UInt16 moduleId);

/************************************************************************************
 * FUNCTION NAME    : ReadSocketsConfig
 * DESCRIPTION      : Reads sockets config (host names and port numbers) from config file
 * PARAMATERS       : UChar8 *fn - file name of config file
 * RETURN VALUE     :
 ************************************************************************************/
void ReadSocketsConfig(UChar8 *fn);
 void sendToWireshark(SocketType fd,UInt8 *buffer, UInt32 bufferLen);

#endif /* _SOCK_TOOLS_H_ */
