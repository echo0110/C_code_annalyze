/*****************************************************************************
 *    FILE NAME:
 *        sig_defs.h
 *
 *    DESCRIPTION:
 *       This file has hash defines used by sigtran stacks
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *    05APR11     Ravi J Sharma    SPR 20872       Single threaded Change
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/


#ifndef  __SIGTRAN_DEFS_H__
#define  __SIGTRAN_DEFS_H__
#ifdef __cplusplus
extern "C" {
#endif
    /* these hash defines are used in stack code also */
#define SIGTRAN_TRUE         (sigtran_U8bit) 1
#define SIGTRAN_FALSE        (sigtran_U8bit) 0
#define SIGTRAN_SUCCESS       1
#define SIGTRAN_FAILURE       0
#define SIGTRAN_NULL          0
    /* ipc related hash defines */
#define SIG_MAX_IOVEC_SIZE    10
#define SIG_MAX_VALID_MAP_KEY 10
#define SOC_MGR_INVALID_SOC_FD      0
#define SOC_MGR_MAX_CONNECTIONS     10
#define SOC_MGR_INVALID_HANDLE      (sigtran_U16bit)0

#define SOC_INVALID_IP              0
#define SOC_INVALID_PORT            0
#define SOC_INV_Q_MODULE            0

    /* default time out period used in select
     * give value in milliseconds
     */

#define SIG_SELECT_TIMEOUT          50
#define SIG_TIME_UPDATE_INTERVAL    10

    /* API ID  for connection accept, and client register req/resp */
#define SIG_COMMON_API_ID_BASE      5000


#define SIG_API_ACCEPT_CONN_IND     SIG_COMMON_API_ID_BASE+1
#define SIG_API_CLIENT_REG_REQ      SIG_COMMON_API_ID_BASE+2
#define SIG_API_CLIENT_REG_RESP     SIG_COMMON_API_ID_BASE+3
#define SIG_API_CONN_BREAK_IND      SIG_COMMON_API_ID_BASE+4

    /* SPR 20872: Changes for enabling single threaded support Starts*/
#ifndef SIGTRAN_DEFAULT_TO_SEC
#define SIGTRAN_DEFAULT_TO_SEC 0
#endif

#ifndef SIGTRAN_DEFAULT_TO_USEC
#define SIGTRAN_DEFAULT_TO_USEC 200
#endif
    /* SPR 20872: Changes for enabling single threaded support Ends*/


#ifdef __cplusplus
}
#endif

#endif
