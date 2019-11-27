/* all the os related files shall be included from this file only */
/*****************************************************************************
 *    FILE NAME:
 *        os.h
 *
 *    DESCRIPTION:
 *       This file shall include all the OS files required for Winnt
 *
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Cspl porting
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef  __OS_H__
#define __OS_H__
/*TBD add here all the files for windows */
/* include cspl header file also here */

#include "stdio.h"
#include <ctype.h>
#include <string.h>
#include "sys/types.h"
#include "sys/timeb.h"
#include <winsock2.h>
#include "winbase.h"
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

#ifdef SIG_COMMON_WITH_CSPL
#include <cspl.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Definition of iovector.
     */
    typedef char FAR                   *caddr_t;
    /*The follwoing structure is compatible with system's WSABUF */

    typedef struct iovec
    {
        u_long    iov_len;     // buffer length
        caddr_t   iov_base; // pointer to buffer
    } sigtran_iovec_st;

    typedef fd_set sig_fd_set;
    typedef SOCKET sig_sock_fd_t;
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP    132
#endif
#define IP_HDRINCL      2 /* header is included with data (raw) */
#define SIGBUS          SIGABRT

#ifdef __cplusplus
}
#endif

#endif
