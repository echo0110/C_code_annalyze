/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: http.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains common macros for C APIs 
 *      
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Anant K Sharma    Nov 17, 2012   TR-069 Amendment 4   Initial Creation
 * 
******************************************************************************/

#ifndef __HTTP_H__
#define __HTTP_H__

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <signal.h>
#define HTTP_CONNECTION "Http Connection"
#include <logger.h>
#include <lteTypes.h>
#include <ipr_cwmp_enb_wrapper.h>
#include <ip_manager.h>
#include <ManagementServerModel.h>
#include <acs_discovery.h>

/*******************************************************************************
 *
 *               MACROS
 *
 *******************************************************************************/

#define SUCCESS 0
#define FAILURE 1
/* SPR 16451 Fix Start */
#define CRITICAL_ERROR 255
/* SPR 16451 Fix End */
#define EMPTY 2
#define URL_LEN 256
//OAM SI code change #define CERT_PATH "CERT_PATH"

typedef struct MemoryStruct
{
        UInt8 *memory;
        size_t size;
} memory_struct_t;

typedef struct sslctxparm_st 
{
    UInt8 * p12file ;
    const UInt8 * pst ;
    PKCS12 * p12 ;
    EVP_PKEY * pkey ;
    X509 * usercert ;
    STACK_OF(X509) * ca ;
    CURL * curl;
    BIO * errorbio;
    UInt32 accesstype ;
    UInt32 verbose;

} sslctxparm;

/******************************************************************************
*
*	 		FUNCTION PROTOTYPES
*
******************************************************************************/

UInt32 ipr_cwmp_send_http_msg(UInt8 *, UInt8 *, UInt8 **, UInt8 *, UInt8*);
UInt32 ipr_cwmp_send_empty_http_msg(UInt8 *, UInt8 *, UInt8 **, UInt8 *, UInt8*);
size_t ipr_cwmp_WriteMemoryCallback(void *, size_t, size_t, void*);
void ipr_clean_http_session(void);

#endif
