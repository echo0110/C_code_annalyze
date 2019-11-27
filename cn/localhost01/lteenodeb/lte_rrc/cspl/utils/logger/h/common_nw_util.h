/***************************************************************************
**  FUNCTION: 
**    This file contains the prototypes for all the network related functions.
****************************************************************************
**
**  FILE NAME:
**      common_nw_util.h
**
**  DESCRIPTION:
**
**      DATE        NAME           REFERENCE     REASON
**     -------      ----           ---------     -------
**     30Jan2003  svaid       HSS:00000000  ORIGINAL
**
**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
****************************************************************************/
#ifndef _COMMON_NW_UTIL_H_
#define _COMMON_NW_UTIL_H_

int setup_client( char *serv_ipstr, u_short serv_tcpport );
int setup_server( u_short serv_tcpport );
#ifdef TCP
int make_connection( int sock );
#endif
#ifdef UDP
int nw_peek( int sockfd, char *ptr, int nbytes );
#endif
int nw_read( int sockfd, char *ptr, int nbytes );
int nw_write( int sockfd, char *ptr, int nbytes );
#ifdef TCP
int bind_and_connect ( char * serv_ipstr, 
  					u_short serv_tcpport ,u_short self_bindport);
#endif

#endif
