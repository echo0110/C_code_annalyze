/*
 * Copyright (c) 2003-2017 Objective Systems, Inc.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Objective Systems, Inc.
 *
 * PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 *****************************************************************************/
/**
 * @file rtxHttp.h
 */
#ifndef _RTXHTTP_H_
#define _RTXHTTP_H_

#ifndef _OS_NOSOCKET

#include "rtxsrc/rtxArrayList.h"
#include "rtxsrc/rtxNetUtil.h"

typedef struct OSRTHttpHeader {
   OSUINT8      majorVersion;
   OSUINT8      minorVersion;
   OSINT32      status;
   OSRTArrayList fields;
} OSRTHttpHeader;

typedef struct OSRTHttpContent {
   size_t length;
   OSOCTET* data;
} OSRTHttpContent;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This function executes a full synchronous HTTP CONNECT request
 * to setup a connection through a proxy.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @return - Operation status: 0 if success, negative code if error.
*/
EXTERNRT int rtxHttpConnect (OSRTNETCONN* pNetConn);

/**
 * This function executes a full synchronous HTTP GET request.
 * A network connection is opened and a GET request sent to the given URL.
 * The response is then read and returned, after which the network connection
 * is closed.
 *
 * @param pctxt - Pointer to context structure.
 * @param url - Full URL of get request.
 * @param pContent - Pointer to content buffer to receive response.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpGet
(OSCTXT* pctxt, const char* url, OSRTHttpContent* pContent);

/**
 * This function sends an HTTP GET request to a network connection.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param url - Full URL of get request.  May be set to NULL if URL was
 *          provided earlier in rtxNetInitConn function call.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpSendGetRequest (OSRTNETCONN* pNetConn, const char* url);

/**
 * This function sends an HTTP request to a network connection.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param method - HTTP method to be used for request (GET or POST)
 * @param content - Content to be sent after header.
 * @param contentType - Type of content.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpSendRequest (OSRTNETCONN* pNetConn,
   const char* method, const char* content, const char* contentType);

/**
 * This function receives the initial header returned from an HTTP
 * request.  The header response information is returned in the header
 * structure.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param pHeader - Pointer to header structure to receive returned data.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpRecvRespHdr
(OSRTNETCONN* pNetConn, OSRTHttpHeader* pHeader);

/**
 * This function receives HTTP content.  All content associated with
 * the response header is stored in the given memory buffer.
 *
 * @param pNetConn - Pointer to network connection structure.
 * @param pHeader - Pointer to response header structure describing content.
 * @param pContent - Buffer to receive content.  Dynamic memory is allocated
 *                     for the content using the rtxMemAlloc function.
 * @return - Operation status: 0 if success, negative code if error.
 */
EXTERNRT int rtxHttpRecvContent
(OSRTNETCONN* pNetConn, OSRTHttpHeader* pHeader, OSRTHttpContent* pContent);

#ifdef _TRACE
/**
 * This function prints the given HTTP header to the diagnostics output
 * stream in the context if debug tracing is enabled.
 *
 * @param pctxt - Pointer to context structure.
 * @param pHeader - Pointer to header structure to be printed.
 */
void rtxDiagPrintHttpHeader (OSCTXT* pctxt, const OSRTHttpHeader* pHeader);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _OS_NOSOCKET */

#endif
