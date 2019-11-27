/************************************************************************************
*
*   FILE NAME  : socket_sctp_tools.h
*
*   DESCRIPTION: functions for transferring messages via SCTP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef SOCKET_SCTP_TOOLS_H_
#define SOCKET_SCTP_TOOLS_H_

/************************************************************************************
 * Includes
 ************************************************************************************/
#include "simTypes.h"
#ifdef ARICENT_SCTP_IPR
#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_uisuif.h>
#include <s_uisusk.h>
#include <s_cspl.h>
#include <sctp.h>
#endif
#ifdef LKSCTP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/sctp.h>
#include <netdb.h>
#endif

/****************************************************************************
 * Global definitions
 ****************************************************************************/

#define SCTP_SOCKET_ERROR          (-1)
#define INVALID_SCTP_SOCKET        (-1)

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : sctpSocketInit
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        :
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      :
 * Description    : to initialize TX SCTP socket with address/port
 ****************************************************************************/
/*SPR 20633 Fix Start*/
SocketType sctpSocketInit(void *host_name, UInt16 port, UInt8 isBlocking);
/*SPR 20633 Fix End*/

/************************************************************************************
 * FUNCTION NAME    : sctpSocketClose
 * DESCRIPTION      : Close SCTP socket.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void sctpSocketClose(SocketType* p_socket_fd);

/************************************************************************************
 * FUNCTION NAME    : ReadFromSocket
 * DESCRIPTION      : Reads message from RX SCTP socket
 * PARAMATERS       : buf - data from socket will be placed here.
 *                          Should have SOCKET_BUFFER_SIZE length.
 *                    SInt32 timeout
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 sctpSocketRead(UInt8 *buf,SInt32);

/************************************************************************************
 * FUNCTION NAME    : sctpSocketWrite
 * DESCRIPTION      : Writes message from to TX SCTP socket
 * PARAMATERS       : buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 sctpSocketWrite(UInt8 *buffer, UInt32 bufferLen);

/************************************************************************************
 * FUNCTION NAME    : sctpSocketListenAndAccept
 * DESCRIPTION      : Listen and accept connection on SCTP socket
 * PARAMATERS       : socket_fd
 * RETURN VALUE     : Socket file descriptor or SOCKET_ERROR
 ************************************************************************************/
UInt32 sctpSocketListenAndAccept(SocketType socket_fd);

/************************************************************************************
 * FUNCTION NAME    : sctpWaitReset
 * DESCRIPTION      : wait for reset on SCTP socket
 * PARAMATERS       : none
 * RETURN VALUE     : Read Length / 0
 ************************************************************************************/
UInt32 sctpWaitReset(void);

#endif /* SOCKET_SCTP_TOOLS_H_ */
