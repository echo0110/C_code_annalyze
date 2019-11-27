/***********************************************************************
 *    FILE NAME:
 *        sig_thr.h
 *
 *    DESCRIPTION:
 *       This file has common porting functions for threads.
 *
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **********************************************************************/

/* Basic Sigtran Mutex Type needs to be ported */

#ifndef  __SIGTRAN_THR_H__
#define  __SIGTRAN_THR_H__

#include <sig_typs.h>
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct
    {
        sigtran_U8bit       *p_task_name;
        sigtran_U32bit  task_priority;
        sigtran_U32bit  recv_opts;
        sigtran_U32bit  stack_size;
    } sig_thr_st;

#ifndef SIGTRAN_SINGLE_THREAD
    /* Create Thread Function */
    sigtran_return_t sigtran_create_thread(
        sigtran_thread_t             *p_th_id,
        sigtran_callback_ptr_t       start_func,
        sigtran_pvoid_t              pargs,
        sigtran_error_t              *p_ecode );
#endif
    sigtran_return_t sigtran_thr_cond_init(
        sigtran_thr_cond_t          *p_thr_cond );

    sigtran_return_t sigtran_thr_cond_wait(
        sigtran_thread_cond_wait_t      *p_thr_cond,
        sigtran_mutex_t                 *p_mutex );

    sigtran_return_t sigtran_thr_cond_signal(
        sigtran_thr_cond_t          *p_thr_cond );

    sigtran_return_t sigtran_thr_cond_deinit(
        sigtran_thr_cond_t          *p_thr_cond );

#ifdef SIGTRAN_LOCAL_TIME_THREAD
    sigtran_return_t sigtran_create_localtime_thread(
        sigtran_error_t *p_ecode );

    void sigtran_get_local_time( sigtran_time_t *p_sig_time );
#endif

#ifdef __cplusplus
}
#endif
#endif
