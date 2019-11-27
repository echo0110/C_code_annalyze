/*****************************************************************************
 *    FILE NAME:
 *        sig_mutx_n_sem.h
 *
 *    DESCRIPTION:
 *       This file has common porting functions for semaphores and locks.
 *
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

/* Basic Sigtran Mutex Type needs to be ported */

#ifndef  __SIGTRAN_MUTEX_N_SEM_H__
#define  __SIGTRAN_MUTEX_N_SEM_H__

#include <sig_typs.h>

#ifdef __cplusplus
extern "C" {
#endif
    /* Simple Lock (Mutex) Functions */
    sigtran_return_t sigtran_mutex_init(
        sigtran_mutex_t       *p_lock );

    sigtran_return_t sigtran_mutex_deinit(
        sigtran_mutex_t       *p_lock );

    sigtran_S32bit sigtran_mutex_trylock(
        sigtran_mutex_t       *p_lock );

    sigtran_return_t sigtran_mutex_lock(
        sigtran_mutex_t       *p_lock );

    sigtran_return_t sigtran_mutex_unlock(
        sigtran_mutex_t       *p_lock );

    /* Simple Lock (Semaphore) Functions */
    sigtran_return_t sigtran_sem_init(
        sigtran_sem_t         *p_lock );

    sigtran_return_t sigtran_sem_deinit(
        sigtran_sem_t         *p_lock );

    sigtran_return_t sigtran_sem_wait(
        sigtran_sem_t         *p_lock );

    sigtran_return_t sigtran_sem_post(
        sigtran_sem_t         *p_lock );

    /* Read Wreite Lock Related Functions */
    sigtran_return_t sigtran_rdwt_lock_init(
        sigtran_rdwt_lock_t       *p_lock );

    sigtran_return_t sigtran_rdwt_lock_deinit(
        sigtran_rdwt_lock_t       *p_lock );

    sigtran_return_t sigtran_rd_lock(
        sigtran_rdwt_lock_t       *p_lock );

    sigtran_return_t sigtran_rd_unlock(
        sigtran_rdwt_lock_t       *p_lock );

    sigtran_return_t sigtran_wt_lock(
        sigtran_rdwt_lock_t       *p_lock );

    sigtran_return_t sigtran_wt_unlock(
        sigtran_rdwt_lock_t       *p_lock );
#ifdef __cplusplus
}
#endif

#endif
