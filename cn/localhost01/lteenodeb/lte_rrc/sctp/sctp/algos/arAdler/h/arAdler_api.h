/******************************************************************************
*
* FILE NAME    : arAdler_api.h
*
* DESCRIPTION  : Contains the Macro's for the Adler Algo implementation
*
* Revision History :
*
*  DATE            NAME                 REFERENCE       REASON
* ------         ------------           ---------     ----------
* 11-Feb-2009    Rajnesh Tyagi          None          Initial
*
* Copyright 2008, Aricent.
*
*******************************************************************************/
#ifndef __ARADLER_API_H__
#define __ARADLER_API_H__
#include "arAdler.h"
/* windows fix */
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
#include <sig_os_port.h>
#elif defined (SIGTRAN_PORT_VXWORKS)
#include <net/uio.h>
#else
#include <sys/uio.h>
#endif
/*windows fix */

#include <stdio.h>
/* Error Code Values --- */
#define E_NO_ERROR             0
#define E_NULL_BUFFER_RECEIVED 1

#define ARADLER_NULL        NULL
#define ARADLER_INVALID     0xFFFFFFFF
#define ARADLER_TRACE       printf

/* windows fix */
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
#ifndef VOID
typedef void                VOID;
#endif
#else
/* SPR 20870 Starts */
#ifdef SIGTRAN_PORT_VXWORKS
#define VOID arAdler_VOID
#endif
typedef void                VOID;
/* SPR 20870 Ends */
#endif
/* windows fix */
typedef int                 INT4;
typedef short               INT2;
typedef char                INT1;
typedef unsigned int        UINT4;
typedef unsigned short      UINT2;
typedef unsigned char       UINT1;
/*windows fix */
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
#ifndef INT8
typedef __int64     INT8;
#endif
#ifndef UINT8
typedef  unsigned __int64   UINT8;
#endif
#elif defined (SIGTRAN_PORT_VXWORKS)
#ifndef INT8
/*typedef __int64   INT8;*/
/* CSR 1-8128593 fix starts*/
#define INT8 __int64
/* CSR 1-8128593 fix ends*/
#endif
#ifndef UINT8
/* CSR 1-8128593 fix starts*/
#define UINT8 unsigned __int64
/* CSR 1-8128593 fix ends*/
#endif
#else
typedef long long           INT8;
typedef unsigned long long  UINT8;
#endif
typedef struct iovec IOVEC;
/*windows fix */

UINT4 arAdler_compute_chcksum( UINT1 *p_buf, UINT4 len, UINT4 *p_error );
UINT4 arAdler_compute_chcksum_iovec( IOVEC *p_iovec, UINT4 iovec_len, UINT4 *p_error );
#endif
