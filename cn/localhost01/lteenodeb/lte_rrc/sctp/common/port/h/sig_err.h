/*****************************************************************************
 *    FILE NAME:
 *        sig_err.h
 *
 *    DESCRIPTION:
 *       This file has common defines for error codes for common
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/


#ifndef  __SIGTRAN_ERR_H__
#define  __SIGTRAN_ERR_H__
#ifdef __cplusplus
extern "C" {
#endif
#define SIGTRAN_ECODE_BASE               10000

#define ESIG_NO_ERROR                    SIGTRAN_ECODE_BASE + 1
#define ESIG_TMR_LIST_CREATE_FAILED      SIGTRAN_ECODE_BASE + 2
#define ESIG_MEM_INIT_FAILED             SIGTRAN_ECODE_BASE + 3
#define ESIG_PARM_INVALID                SIGTRAN_ECODE_BASE + 4
#define ESIG_IPC_NO_HANDLE_AVL           SIGTRAN_ECODE_BASE + 5
#define ESIG_IPC_INVALID_PARAM           SIGTRAN_ECODE_BASE + 6
#define ESIG_IPC_QMODULE_NOT_SET         SIGTRAN_ECODE_BASE + 7
#define ESIG_IPC_INVALID_MAY_KEY         SIGTRAN_ECODE_BASE + 8
#define ESIG_IPC_INVALID_DEST_ID         SIGTRAN_ECODE_BASE + 9
#define ESIG_IPC_READ_FAIL               SIGTRAN_ECODE_BASE + 10
#define ESIG_IPC_DEST_ID_NOT_SET         SIGTRAN_ECODE_BASE + 11
#define ESIG_MEM_ALLOC_FAILED            SIGTRAN_ECODE_BASE + 12
#define ESIG_TMR_NOT_RUNNING             SIGTRAN_ECODE_BASE + 14
#define ESIG_IPC_RECVD_EOF               SIGTRAN_ECODE_BASE + 15 /* CSR 1-5892901 */
#ifdef __cplusplus
}
#endif

#endif
