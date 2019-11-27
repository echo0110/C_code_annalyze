/************************************************************************************
*
*   FILE NAME  : socket_udp_tools.h
*
*   DESCRIPTION: functions for transferring messages via UDP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef SOCKET_UDP_TOOLS_H_
#define SOCKET_UDP_TOOLS_H_

/************************************************************************************
 * Includes
 ************************************************************************************/
#include "simTypes.h"
#ifndef WIN32
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : udpSocketInitTx
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize TX UDP socket with address/port
 ****************************************************************************/
 SocketType udpSocketInitTx(void *host_name, struct sockaddr_in *s, UInt16 port, UInt8 isBlocking);

/****************************************************************************
 * Function Name  : udpSocketInitRx
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize RX UDP socket with address/port
 ****************************************************************************/
SocketType udpSocketInitRx(void *host_name, struct sockaddr_in *s, UInt16 port, UInt8 isBlocking);

/************************************************************************************
 * FUNCTION NAME    : udpSocketClose
 * DESCRIPTION      : Close UDP socket.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void udpSocketClose(SocketType* p_socket_fd);

/************************************************************************************
 * FUNCTION NAME    : ReadFromSocket
 * DESCRIPTION      : Reads message from RX UDP socket
 * PARAMATERS       : SocketType socket_fd
 *                    buf - data from socket will be placed here.
 *                          Should have SOCKET_BUFFER_SIZE length.
 *                    SInt32 timeout
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 udpSocketRead(SocketType socket_fd, UInt8 *buf,SInt32);

/************************************************************************************
 * FUNCTION NAME    : udpSocketWrite
 * DESCRIPTION      : Writes message from to TX UDP socket
 * PARAMATERS       : buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 udpSocketWrite(SocketType socket_fd, struct sockaddr* pSockAddr, UInt8 *buffer, UInt32 bufferLen);

#endif /* SOCKET_UDP_TOOLS_H_ */
