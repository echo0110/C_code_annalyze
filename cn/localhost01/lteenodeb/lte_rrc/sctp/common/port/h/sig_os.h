/**********************************************************************
 *    FILE NAME:
 *        sig_os.h
 *
 *    DESCRIPTION:
 *       This file has common porting functions.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    14Sept02    Sandeep Mahajan     -            .Original Creation
 *    17Sept02    Yogesh Gahlaut      -            Modification from SUA
 *    11Sept03    Vivek Bansal        -            Changes for CSPL
 *                                                 Integration
 *    12NOV03     Vishavdeep Sharma   -            Rel 5.0 Cspl porting
 *    23Mar05     Amit Ray            -            Lynx Port
 *
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **********************************************************************/


#ifndef  __SIGTRAN_OS_H__
#define  __SIGTRAN_OS_H__

/*
 * Do some Checking stuff to make sure that 1 and only
 * one port is defined
 * Can also make some default choice. Should Not.
 */

#ifdef SIGTRAN_PORT_KNOWN
#undef SIGTRAN_PORT_KNOWN
#endif

#ifdef  SIGTRAN_PORT_TRU64
#define SIGTRAN_PORT_KNOWN
/*Since Sun OS can do with POSIX, use posix*/
#define SIGTRAN_PORT_POSIX
#endif

#ifdef  SIGTRAN_PORT_SOLARIS
#define SIGTRAN_PORT_KNOWN
/*Since Sun OS can do with POSIX, use posix*/
#define SIGTRAN_PORT_POSIX
#endif

#ifdef  SIGTRAN_PORT_LINUX

#ifdef  SIGTRAN_PORT_KNOWN
#error "OS port defined twice"

#else
#define LINUX                   1
#define SIGTRAN_PORT_KNOWN

#define SIGTRAN_PORT_POSIX
#endif
#endif


#ifdef  SIGTRAN_PORT_LYNX

#ifdef  SIGTRAN_PORT_KNOWN
#error "OS port defined twice"

#else
/* Change LYNX to FSS_LYNX for CSR 1-2509772*/
#define FSS_LYNX                   1
#define SIGTRAN_PORT_KNOWN

#define SIGTRAN_PORT_POSIX
#endif
#endif



#ifdef  SIGTRAN_PORT_VXWORKS
#ifdef  SIGTRAN_PORT_KNOWN
#error "OS port defined twice"
#else
#define SIGTRAN_PORT_KNOWN
#endif
#endif


#ifdef  SIGTRAN_PORT_VX_PNE
#ifdef  SIGTRAN_PORT_KNOWN
#error "OS port defined twice"
#else
#define SIGTRAN_PORT_KNOWN
#endif
#endif


#ifdef  SIGTRAN_PORT_WINXP
#ifdef  SIGTRAN_PORT_KNOWN
#error "OS port defined twice"
#else
#define SIGTRAN_PORT_KNOWN
#endif
#endif

/*
 * Now we check if we know about the OS
 */
#ifndef SIGTRAN_PORT_KNOWN
#error  "OS type not specified, please use one of the flags"
#endif

/* this is the only file which stacks need to include to use
 * sig common .All the common files are included through this file
 */

#include <sig_os_port.h>
#include <sig_typs.h>
#include <sig_defs.h>
#include <sig_dbg.h>
#include <sig_err.h>
#include <sig_ipc.h>
#include <sig_mem.h>
#include <sig_mutx_n_sem.h>
#include <sig_thr.h>
#include <sig_tmr.h>
#include <sig_util.h>
#include <sig_cspl_wrap.h>
#include <sig_ipc_intr.h>

#endif /* __SIGTRAN_OS_H__  */
