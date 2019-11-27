/*********************************************************************************    
**********************************************************************************
**    FILE NAME:
**        logger_nw_recv.h
**
**    DESCRIPTION:	Contains all logger types
**
**    DATE              AUTHOR              REF         REASON
**    ----              ------              ---         ------
**    06Feb03           svaid           ---         Original
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*******************************************************************************/
#ifndef __LOGGER_NW_RECV_H__
#define __LOGGER_NW_RECV_H__

S16 logger_server_start(unsigned short port);
S16 logger_wait_for_client_connection(S16 server_fd);
S16 logger_recv(S32	p_fd);


#endif
