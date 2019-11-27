/*****************************************************************************
 *    FILE NAME:
 *        sig_dbg.h
 *
 *    DESCRIPTION:
 *       This file has common porting functions for debug/error/logs.
 *
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/


#ifndef  __SIGTRAN_DBG_H__
#define  __SIGTRAN_DBG_H__

#include <sig_typs.h>
#ifdef __cplusplus
extern "C" {
#endif
    /* Macro for logging */
#define SIGTRAN_REPORT(fmt_str)  sigtran_dbg_log_trace fmt_str

#ifdef SIG_DEBUG_TRACE

#define SIG_DEBUG_PRINT(x) printf x

#define SIG_ASSERT(condition)                                               \
    do {                                                                 \
        if((condition)) {}                                               \
        else                                                             \
        {                                                                \
            printf( "\n\nAssert failed: File %s at Line %d\n",         \
                    __FILE__,__LINE__ );\
            return SIGTRAN_FAILURE;                                        \
        }                                                                \
    } while(0)


#else

#define SIG_DEBUG_PRINT(x) {}
    /* KW Fix */
#define SIG_ASSERT(condition)                                              \
    do {                                                                    \
        if((condition)) {}                                              \
        else                                                            \
        {                                                               \
            return SIGTRAN_FAILURE;                                     \
        }                                                               \
    } while (0)

#endif

#define SIG_TARCE(x)
    /* Exep handling */
    void sigtran_sig_handler( int sig );

    sigtran_return_t sigtran_install_sig_handler(
        sigtran_void_t ( *sig_handler )( sigtran_U16bit ),
        sigtran_error_t         *p_ecode );

    /* Function for Logging Traces ..*/
    void sigtran_dbg_log_trace( char *szFormat, ... );

#ifdef __cplusplus
}
#endif


#endif
