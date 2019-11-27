
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: socket_utils.h 
 *
 *************************************************************************
 *
 * File Description : This file contains the declaration of functions  
 *                    that are used for sending message to sockets 
 *
 *************************************************************************/


#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#define IP_ADDR_MAX_SIZE 24

/*****************************************************************************
 * Function Name  : send_message_to_udp_socket
 * Inputs         : pMsgBuf
 * Outputs        : 
 * Returns        : 0 - On success.
 *                  -1 - On failure.
 * Description    : send_message_to_udp_socket
 *****************************************************************************/
int send_message_to_udp_socket(char* , unsigned int, int, void *);
#endif
