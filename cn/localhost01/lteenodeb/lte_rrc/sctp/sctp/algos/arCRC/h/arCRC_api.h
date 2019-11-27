/******************************************************************************
*
* FILE NAME    : arCRC_api.h
*
* DESCRIPTION  : Contains the Macro's and prototypes for the CRC Algo implementation
*
* Revision History :
*
*  DATE            NAME                 REFERENCE       REASON
* ------         ------------           ---------     ----------
* 24-Feb-2009    Shivi Singh Verma          None          Initial
*
* Copyright 2009, Aricent.
*
*******************************************************************************/
#ifndef __ARCRC_API_H__
#define __ARCRC_API_H__
#include <stdio.h>
/* windows fix */
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
#include <sig_os_port.h>
#elif defined (SIGTRAN_PORT_VXWORKS)
#include <net/uio.h>
#else
#include <sys/uio.h>
#endif
/*windows fix */

/* Error Code Values --- */
#define E_NO_ERROR             0
#define E_NULL_BUFFER_RECEIVED 1

#define ARCRC_NULL        NULL
#define ARCRC_INVALID     0xFFFFFFFF
#define ARCRC_POLY        0x1EDC6F41
#define ARCRC_TABLE_SIZE    256
#define ARCRC_CIRCULAR_BASE   4
#define ARCRC_TRACE     printf
#define ARCRC_TRUE 1
#define ARCRC_FALSE 0

#define ARCRC_32C(c,d)  (c = (c>>8)^g_crc[(c^(d))&0xFF])
/*windows fix*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
#ifndef VOID
typedef void                VOID;
#endif
#else
/* SPR 20870 Starts */
#ifdef SIGTRAN_PORT_VXWORKS
#define VOID arCRC_VOID
#endif
typedef void                VOID;
/* SPR 20870 Ends */
#endif
/*windows fix*/

typedef int                 INT4;
typedef short               INT2;
typedef char                INT1;
typedef unsigned int        UINT4;
typedef unsigned short      UINT2;
typedef unsigned char       UINT1;

/*windows fix*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
#ifndef INT8
/*typedef __int64   INT8;*/
/* CSR 1-8128593 fix starts*/
#define INT8 __int64
/* CSR 1-8128593 fix ends*/
#endif
#ifndef UINT8
/*typedef  unsigned __int64 UINT8;*/
/* CSR 1-8128593 fix starts*/
#define UINT8 unsigned __int64
/* CSR 1-8128593 fix ends*/
#endif
/*
#elif defined (SIGTRAN_PORT_VXWORKS)
#ifndef INT8
typedef long long       INT8;
#endif
#ifndef UINT8
typedef  unsigned long long UINT8;
#endif */
#else
/* SPR 20870 Starts */
#ifdef SIGTRAN_PORT_VXWORKS
#define INT8 arCRC_INT8
#define UINT8 arCRC_UINT8
#endif
typedef long long           INT8;
typedef unsigned long long  UINT8;
/* SPR 20870 Ends */
#endif
typedef struct iovec IOVEC;
/*windows fix */

UINT4 arCRC_compute_chcksum( UINT1 *p_dataBuf, UINT4 len, UINT4 *p_error );
UINT4 arCRC_compute_chcksum_iovec( IOVEC *p_iovec, UINT4 iovec_len, UINT4 *p_error );
#endif
