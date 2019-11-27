/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_server.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains functions and macros for server.c 
 *
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *
 *   2012/11/07 gur24604  Initial Version 1.0
 ******************************************************************************/

#ifndef _IPR_CWMP_SERVER_H_
#define _IPR_CWMP_SERVER_H_

/******************************************************************************
   Macros
******************************************************************************/

#include <lteTypes.h>
#include <event_handler.h>
#include <ipr_cwmp_enb_wrapper.h>

#define MYPORT 15284
//OAM SI code change #define ENV_MYPORT "ENV_MYPORT"
//#define MYPORT 19737
#define SERVER "webserver/1.1"
#define PROTOCOL "HTTP/1.1"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define PORT 80

#define DIGEST 1
#define BASIC (1 != DIGEST)
#define MAX_BUFFER_LENGTH 2048
#define MAX_HEADER_BUFFER_LEN 5000
#define IP_ADDR_LEN 40
#define STR_LEN 32
#define MAX_STR_LEN 64


/******************************************************************************
  Global Variables
******************************************************************************/

/*connection requests recieved */
extern UInt32 g_ipr_cwmp_con_req;

/* No. of Connection requests recieved */
extern UInt32 g_ipr_cwmp_con_req_counter;

/* Variable used in Session Manager to indicate if Session is opened */
extern UInt32 g_ipr_cwmp_session_open;

/******************************************************************************
  Function Declarations
******************************************************************************/

ipr_cwmp_ret_t ipr_cwmp_isbase64(UInt8 c);

inline UInt8 ipr_cwmp_value(UInt8 c);

ipr_cwmp_ret_t ipr_cwmp_UnBase64(UInt8 *dest, const UInt8 *src, UInt32 srclen);

void ipr_cwmp_decoder(UInt8 *buffer, UInt8 *auth[2]);

void ipr_cwmp_Authentication_Parser(UInt8 *);

void ipr_cwmp_send_headers(UInt32 connfd, UInt32 status, UInt8 *title, UInt8 *extra, UInt8 *mime, UInt32 length, time_t date, UInt8 *ip_address);

void ipr_cwmp_digest_http_header(UInt8 *buffer, UInt8 *ip_address);

#endif
