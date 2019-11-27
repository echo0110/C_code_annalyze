/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: logging.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of logging framework
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: logging.c,v $
 * Initial version
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h> 
#include <sys/timeb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/ipc.h>
#include<arpa/inet.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include<lteMisc.h>
#include<lteMacComPorting.h>
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "socket_utils.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define OAM_UDP_PORT 3336
#define MAX_BUFFER_SIZE 200
#define OAM_IP_ADDR "127.0.0.1"
/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : send_message_to_udp_socket
 * Inputs         : pMsgBuf
 * Outputs        : 
 * Returns        : 0 - On success.
 *                  1 - On failure.
 * Description    : send_message_to_udp_socket
 *****************************************************************************/

int send_message_to_udp_socket(char* pMsgBuf, unsigned int buf_len,
        int sockfd,
        void *s_addr)
{
    struct sockaddr_in *serv_addr = (struct sockaddr_in *)s_addr;

    if(sendto(sockfd, pMsgBuf,buf_len, 0, (struct sockaddr*)(serv_addr),
                sizeof(struct sockaddr)) == -1)
    {
        perror("sendto() system call failure\n");
       
        /*Coverity 99391 Fix Start*/ 
	if(SOCKET_ERROR == close_wrapper(sockfd))
	{
		LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,L2_SYS_FAIL,\
				0,\
				__LINE__,0,0,0,0,\
				0.0,0.0,__FILE__,__func__);
	}
        /*Coverity 99391 Fix End*/
	 return -1;
    }
    return 0;
}


         
         


