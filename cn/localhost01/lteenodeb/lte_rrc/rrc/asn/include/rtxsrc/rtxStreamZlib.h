/*
 * Copyright (c) 2003-2009 Objective Systems, Inc.
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
/* 
 * @file rtxStreamZlib.h 
 */
#ifndef _RTXSTREAMZLIB_H_
#define _RTXSTREAMZLIB_H_

#include "rtxsrc/osSysTypes.h"
#include "rtxsrc/rtxExternDefs.h"
#include "rtxsrc/rtxStream.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This function initializes a zlib compressed stream. This function should
 *  be called first before any operation with a zlib compressed stream.
 *
 * @param pctxt        Pointer to context structure variable.
 * @param flags        Specifies the access mode for the stream: 
 *                       - OSRTSTRMF_INPUT = input (reading) stream;
 *                       - OSRTSTRMF_OUTPUT = output (writing) stream.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibAttach (OSCTXT* pctxt, OSUINT16 flags);

/*
 * This function finish compressed block. Stream switched to transparent mode.
 *
 * @param pctxt        Pointer to context structure variable.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibEnd (OSCTXT* pctxt);

/*
 * This function free a zlib compressed stream and restore underlying stream. 
 *
 * @param pctxt        Pointer to context structure variable.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibFree (OSCTXT* pctxt);

/*
 * This function finish compressed block and start new block. 
 * Stream remained in compressed mode.
 *
 * @param pctxt        Pointer to context structure variable.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibReset (OSCTXT* pctxt);

/*
 * This function start compression parameters. 
 *
 * @param pctxt        Pointer to context structure variable.
 * @param level        Set compression level 0..9.
 *                       0 - Z_NO_COMPRESSION
 *                       1 - Z_BEST_SPEED
 *                       9 - Z_BEST_COMPRESSION
 *                      -1 - Z_DEFAULT_COMPRESSION - like level 6
 * @param windowBits   Set decompression window size.
 *                       8..15 - ZLIB header (RFC 1950)
 *                       -8..-15 - header is absent (RFC 1951)
 *                       (8..15) + 16 - GZIP header (RFC 1952)
 * @param memLevel     Set memory usage level 1..9.
 *                       1 - use minimum memory
 *                       9 - use maximum memory
 * @param strategy     Set copression algorithm.
 *                       0 - Z_DEFAULT_STRATEGY - common method any data
 *                       other possible values defined in zlib.h
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibSetDeflateParams 
(OSCTXT* pctxt, int level, int windowBits, int memLevel, int strategy);

/*
 * This function start decompression parameter. 
 *
 * @param pctxt        Pointer to context structure variable.
 * @param windowBits   Set decompression window size.
 *                       8..15 - ZLIB header (RFC 1950)
 *                       -8..-15 - header is absent (RFC 1951)
 *                       (8..15) + 16 - GZIP header (RFC 1952)
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibSetInflateParams 
(OSCTXT* pctxt, int windowBits);

/*
 * This function start compressed block. 
 *
 * @param pctxt        Pointer to context structure variable.
 * @return             Completion status of operation: 0 (0) = success,
 *                       negative return value is error.
 */
EXTERNRT int rtxStreamZlibStart (OSCTXT* pctxt);

#ifdef __cplusplus
}
#endif

#endif /* _RTXSTREAMZLIB_H_ */

