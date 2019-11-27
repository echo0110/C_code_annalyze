/************************************************************************************
*
*   FILE NAME  : socket_udp_tools.c
*
*   DESCRIPTION: functions for transferring messages via UDP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

#include "simTypes.h"
#include "simLogger.h"

#include "socket_udp_tools.h"

/************************************************************************************
 * Local definitions
 ************************************************************************************/

/************************************************************************************
 Local functions definitions
 ************************************************************************************/

/************************************************************************************
 * Global variables
 ************************************************************************************/

/************************************************************************************
 * Local variables
 ************************************************************************************/

/************************************************************************************
  Functions implementation
 ************************************************************************************/

/****************************************************************************
 * Function Name  : udpSocketInitTx
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize UDP socket with address/port
 ****************************************************************************/
 SocketType udpSocketInitTx(void *host_name, struct sockaddr_in *s, UInt16 port, UInt8 isBlocking)
{
#ifdef WIN32
    unsigned long nb = 1;
#endif
    SocketType sockfd = SOCKET_ERROR;
    struct hostent *host_p = (struct hostent *)gethostbyname(host_name);

    if ( PNULL == host_p )
    {
        SIM_LOG(SIM_LOG_CRITICAL,"UDP: Invalid host name. [%s]", host_name);
        return SOCKET_ERROR;
    }

    SIM_LOG(SIM_LOG_INFO, "UDP: Creating socket Host['%s'], Port[%i]",
            host_name, port);

    if ( SOCKET_ERROR == (sockfd = socket(PF_INET, SOCK_DGRAM, 0)) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "UDP: creating socket error: %s",
            strerror(errno));
        return SOCKET_ERROR;
    }

    (*s).sin_family = AF_INET; /*Was:PF_INET*/
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
    (*s).sin_port = htons(port);
    memset(&((*s).sin_zero),0,8);
    SIM_LOG(SIM_LOG_INFO, "UDP: sin_port[%u]: sin_addr[%d]",
            (*s).sin_port, (*s).sin_addr.s_addr);

    if ( SIM_FALSE == isBlocking )
    {
#ifdef WIN32
        /* non-blocking socket */
        if(SOCKET_ERROR==ioctlsocket(sockfd, FIONBIO, (UInt32 *)&nb))
        {
            SIM_LOG(SIM_LOG_CRITICAL,"UDP: Error switching to non-blocking socket mode");
            return SOCKET_ERROR;
        }
#else
        {
            int fl = fcntl(sockfd, F_GETFL, 0);
            fcntl(sockfd, F_SETFL, fl | O_NONBLOCK);
        }
#endif
        SIM_LOG(SIM_LOG_INFO, "UDP: Socket blocking mode is on");
    }

    return sockfd;
}

/****************************************************************************
 * Function Name  : udpSocketInitRx
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize RX UDP socket with address/port
 ****************************************************************************/
SocketType udpSocketInitRx(void *host_name, struct sockaddr_in *s, UInt16 port, UInt8 isBlocking)
{
    SocketType socket_fd = SOCKET_ERROR;

    socket_fd = udpSocketInitTx(host_name, s, port, isBlocking);
    if (SOCKET_ERROR == socket_fd)
    {
        return SOCKET_ERROR;
    }

    /* Bind the port to specific socket for listening */
    if ( SOCKET_ERROR == bind(socket_fd, (struct sockaddr*)s, sizeof(struct sockaddr_in)) )
    {
        SIM_LOG(SIM_LOG_CRITICAL, "UDP: Bind failed %s", strerror(errno));
        return SOCKET_ERROR;
    }

    SIM_LOG( SIM_LOG_DETAIL, "UDP: binding succeeded" );
    return socket_fd;
}

/************************************************************************************
 * FUNCTION NAME    : udpSocketClose
 * DESCRIPTION      : Close UDP socket.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void udpSocketClose(SocketType* p_socket_fd)
{
    if ( *p_socket_fd != SOCKET_ERROR )
    {
#ifdef WIN32
        closesocket(*p_socket_fd);
#else
        close(*p_socket_fd);
#endif
        *p_socket_fd = SOCKET_ERROR;
    }
}

/************************************************************************************
 * FUNCTION NAME    : udpSocketRead
 * DESCRIPTION      : Reads message from RX UDP socket
 * PARAMATERS       : SocketType socket_fd
 *                    buf - data from socket will be placed here.
 *                          Should have SOCKET_BUFFER_SIZE length.
 *                    SInt32 timeout
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 udpSocketRead(SocketType socket_fd, UInt8 *buf, SInt32 timeout)
{
    Int bytesRead = 0;
    UInt32 addrLen = sizeof(struct sockaddr_in);
    struct sockaddr_in sockAddr;
    int sock_flags = SOCKET_FLAGS;
    fd_set set;
    SInt32 sel_ret = -1;
    struct timeval sel_timeout;

    if ( SOCKET_ERROR == socket_fd )
    {
        return 0; /* socket is not initialized */
    }


    FD_ZERO(&set);
    FD_SET(socket_fd, &set);

    sel_timeout.tv_sec = 0;
    sel_timeout.tv_usec = timeout * 1000;

    sel_ret = select(socket_fd+1, &set, 0, 0, &sel_timeout);

    if (sel_ret == -1)
    {
        SIM_LOG(SIM_LOG_WARNING, "select failed in func %s \n",__func__);
        abort();
    }
    else if (sel_ret == 0)
    {
        return 0;
    }

    //  if ( SIM_TRUE == simSocketRxBlock )
    {
        sock_flags = SOCKET_FLAGS_WAIT;
    }

    bytesRead = recvfrom(
                    socket_fd,
                    buf, SOCKET_BUFFER_SIZE,
                    sock_flags,
                    (struct sockaddr*)&sockAddr, &addrLen);

    if( bytesRead <= 0)
    {
        return 0; /*if no message, bytesRead will be -1 */
    }

    return bytesRead;
}

/************************************************************************************
 * FUNCTION NAME    : udpSocketWrite
 * DESCRIPTION      : Writes message from to TX UDP socket
 * PARAMATERS       : buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 udpSocketWrite(SocketType socket_fd, struct sockaddr* pSockAddr, UInt8 *buffer, UInt32 bufferLen)
{
    Int res_len = 0;

    res_len = sendto(
                socket_fd,
                buffer, bufferLen,
                0,
                (struct sockaddr*)pSockAddr, sizeof(struct sockaddr_in) );

    if ( res_len != bufferLen )
    {
        SIM_LOG(SIM_LOG_DETAIL, "UDP: Write failed res_len=%i bufferLen=%i",
                res_len, bufferLen);
    }

    return res_len;
}
