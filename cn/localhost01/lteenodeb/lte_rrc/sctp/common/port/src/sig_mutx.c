/*****************************************************************************
*    FILE NAME:
*        sig_mutx.c
*
*    DESCRIPTION:
*       This file has porting function for Mutex related functions.
*
*    DATE           NAME          REFERENCE        REASON
*    ----------------------------------------------------
*    14Sept02    Sandeep Mahajan     -            .Original Creation
 *    13Jul09   kannan v            rel 6.2.0      gcc warning removal
*    1July13     Rohan Pawa       SPR 21141        CSR 50452
*    6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
*       Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/
#include <sig_typs.h>
#include <sig_os.h>

/* Simple Lock (Mutex) Functions */
/******************************************************************************
**  FUNCTION :    sigtran_mutex_init
**
**  DESCRIPTION:  This function initialises the Mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mutex_init(
    sigtran_mutex_t       *p_mutex )
{
#ifndef SIGTRAN_SINGLE_THREAD
    SIG_ASSERT( ( SIGTRAN_NULL != p_mutex ) );

#ifdef SIG_COMMON_WITH_CSPL
    *p_mutex = qvNewLock();
#else
    *p_mutex = ( sigtran_mutex_t )sg_mutex_create();
#endif

    /*
     * If NULL is returned, then return failure.
     */
    if( NULL == *p_mutex )
    {
        return SIGTRAN_FAILURE;
    }

#else
    p_mutex = p_mutex;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_mutex_deinit
**
**  DESCRIPTION:  This function de-initialises the Mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mutex_deinit(
    sigtran_mutex_t       *p_mutex )
{
#ifndef SIGTRAN_SINGLE_THREAD

    if( SIGTRAN_NULL != *p_mutex )

#ifdef SIG_COMMON_WITH_CSPL
        qvDestroyLock( *p_mutex );

#else
        sg_mutex_destroy( ( sigtran_pvoid_t * )*p_mutex );
#endif
#else
    p_mutex = p_mutex;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_mutex_lock
**
**  DESCRIPTION:  This function Locks the Mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mutex_lock(
    sigtran_mutex_t       *p_mutex )
{
#ifndef SIGTRAN_SINGLE_THREAD
#ifdef SIG_COMMON_WITH_CSPL
    qvLock( *p_mutex );
#else
    sg_mutex_lock( ( sigtran_pvoid_t * )*p_mutex );
#endif
#else
    p_mutex = p_mutex;
#endif
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sigtran_mutex_unlock
**
**  DESCRIPTION:  This function Unlocks the Mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mutex_unlock(
    sigtran_mutex_t       *p_mutex )
{
#ifndef SIGTRAN_SINGLE_THREAD
#ifdef SIG_COMMON_WITH_CSPL
    qvUnlock( *p_mutex );
#else
    sg_mutex_unlock( ( sigtran_pvoid_t * )*p_mutex );
#endif
#else
    p_mutex = p_mutex;
#endif
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sigtran_mutex_trylock
**
**  DESCRIPTION:  This function Locks the Mutex.Its used in situations where you
**                want the lock immediately or you want control to be returned
**                to you if the mutex is not available.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_S32bit sigtran_mutex_trylock(
    sigtran_mutex_t       *p_lock )
{
#ifndef SIGTRAN_SINGLE_THREAD
#ifdef SIG_COMMON_WITH_CSPL
    return ( sigtran_S32bit )qvTryLock( *p_lock );
#else
    return sg_mutex_try_lock( *p_lock );
#endif
#else
    p_lock = p_lock;
    return 1;
#endif
}


/******************************************************************************
**  FUNCTION :    sigtran_sem_wait
**
**  DESCRIPTION:  This function Waits for the Semaphore.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_sem_wait(
    sigtran_sem_t       *p_sem )
{
#ifdef SIG_COMMON_WITH_CSPL
    qvSemWait( *p_sem );
#else
    sg_sem_wait( ( sigtran_pvoid_t )p_sem );
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_sem_post
**
**  DESCRIPTION:  This function Posts the Semaphore.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_sem_post(
    sigtran_sem_t       *p_sem )
{
#ifdef SIG_COMMON_WITH_CSPL
    qvSemPost( *p_sem );
    return SIGTRAN_SUCCESS;
#else
    sg_sem_post( ( sigtran_pvoid_t )p_sem );
#endif
    return SIGTRAN_SUCCESS;
}

/* Read Wreite Lock Related Functions */
/******************************************************************************
**  FUNCTION :    sigtran_rdwt_lock_init
**
**  DESCRIPTION:  This function initialises the Read/Write Lock
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_rdwt_lock_init(
    sigtran_rdwt_lock_t       *p_rdwt_lock )
{
    if( sigtran_mutex_init( &p_rdwt_lock->lock ) != SIGTRAN_SUCCESS )
    {
        return SIGTRAN_FAILURE;
    }

    if( sigtran_sem_init( &p_rdwt_lock->ok_to_read ) != SIGTRAN_SUCCESS )
    {
        /* Call Deinit Functions */
        return SIGTRAN_FAILURE;
    }

    if( sigtran_sem_init( &p_rdwt_lock->ok_to_write ) != SIGTRAN_SUCCESS )
    {
        /* Call Deinit Functions */
        return SIGTRAN_FAILURE;
    }

    /*Bug Id 81 fix s*/
    sigtran_mutex_lock( &p_rdwt_lock->lock );
    /*Bug Id 81 fix e*/
    p_rdwt_lock->active_writers     = 0;
    p_rdwt_lock->waiting_writers    = 0;
    p_rdwt_lock->active_readers     = 0;
    p_rdwt_lock->waiting_readers    = 0;
    /*Bug Id 81 fix s*/
    sigtran_mutex_unlock( &p_rdwt_lock->lock );
    /*Bug Id 81 fix e*/

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_rdwt_lock_deinit
**
**  DESCRIPTION:  This function de-initialises the Read/Write Lock
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_rdwt_lock_deinit(
    sigtran_rdwt_lock_t       *p_rdwt_lock )
{
#ifndef SIGTRAN_SINGLE_THREAD

    if( sigtran_mutex_deinit( &p_rdwt_lock->lock ) != SIGTRAN_SUCCESS )
    {
        return SIGTRAN_FAILURE;
    }

    if( sigtran_sem_deinit( &p_rdwt_lock->ok_to_read ) != SIGTRAN_SUCCESS )
    {
        /* Call Deinit Functions */
        return SIGTRAN_FAILURE;
    }

    if( sigtran_sem_deinit( &p_rdwt_lock->ok_to_write ) != SIGTRAN_SUCCESS )
    {
        /* Call Deinit Functions */
        return SIGTRAN_FAILURE;
    }

    p_rdwt_lock->active_writers     = 0;
    p_rdwt_lock->waiting_writers    = 0;
    p_rdwt_lock->active_readers     = 0;
    p_rdwt_lock->waiting_readers    = 0;
#else
    p_rdwt_lock = p_rdwt_lock;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_rd_lock
**
**  DESCRIPTION:  This function takes the READ access of mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_rd_lock(
    sigtran_rdwt_lock_t       *p_rdwt_lock )
{
#ifndef SIGTRAN_SINGLE_THREAD
    sigtran_U8bit waiting_flag = 0;
    sigtran_mutex_lock( &p_rdwt_lock->lock );

    /* Check that there are no waiting or active writers */
    if( p_rdwt_lock->active_writers + p_rdwt_lock->waiting_writers > 0 )
    {
        /* Increment the count of waiting readers */
        p_rdwt_lock->waiting_readers++;
        waiting_flag = 1;
    }

    else
    {
        sigtran_sem_post( &p_rdwt_lock->ok_to_read );
        p_rdwt_lock->active_readers++;
        sigtran_sem_wait( &p_rdwt_lock->ok_to_read );
    }

    sigtran_mutex_unlock( &p_rdwt_lock->lock );

    if( 1 == waiting_flag )
    {
        sigtran_sem_wait( &p_rdwt_lock->ok_to_read );
    }

#else
    p_rdwt_lock = p_rdwt_lock;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_rd_unlock
**
**  DESCRIPTION:  This function gives the READ access of mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_rd_unlock(
    sigtran_rdwt_lock_t       *p_rdwt_lock )
{
#ifndef SIGTRAN_SINGLE_THREAD
    sigtran_mutex_lock( &p_rdwt_lock->lock );
    p_rdwt_lock->active_readers--;

    /* Check that there are no waiting or active writers */
    if( ( p_rdwt_lock->active_readers == 0 ) &&
            ( p_rdwt_lock->waiting_writers > 0 ) )
    {
        /* Increment the count of waiting readers */
        sigtran_sem_post( &p_rdwt_lock->ok_to_write );
        p_rdwt_lock->active_writers++;
        p_rdwt_lock->waiting_writers--;
    }

    sigtran_mutex_unlock( &p_rdwt_lock->lock );
#else
    p_rdwt_lock = p_rdwt_lock;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_wt_lock
**
**  DESCRIPTION:  This function takes the WRITE access of mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_wt_lock(
    sigtran_rdwt_lock_t       *p_rdwt_lock )
{
#ifndef SIGTRAN_SINGLE_THREAD
    sigtran_U8bit waiting_flag = 0;
    sigtran_mutex_lock( &p_rdwt_lock->lock );

    if( p_rdwt_lock->active_writers + p_rdwt_lock->active_readers > 0 )
    {
        /* Check that nobody else is actively accessing the database */
        p_rdwt_lock->waiting_writers++;
        waiting_flag = 1;
    }

    else
    {
        sigtran_sem_post( &p_rdwt_lock->ok_to_write );
        p_rdwt_lock->active_writers++;
        sigtran_sem_wait( &p_rdwt_lock->ok_to_write );
    }

    sigtran_mutex_unlock( &p_rdwt_lock->lock );

    if( waiting_flag == 1 )
    {
        sigtran_sem_wait( &p_rdwt_lock->ok_to_write );
    }

#else
    p_rdwt_lock = p_rdwt_lock;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_wt_unlock
**
**  DESCRIPTION:  This function gives the WRITE access of mutex.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_wt_unlock(
    sigtran_rdwt_lock_t       *p_rdwt_lock )
{
#ifndef SIGTRAN_SINGLE_THREAD
    sigtran_mutex_lock( &p_rdwt_lock->lock );
    p_rdwt_lock->active_writers--;

    if( p_rdwt_lock->waiting_writers > 0 )
    {
        /* Check that nobody else is actively accessing the database */
        sigtran_sem_post( &p_rdwt_lock->ok_to_write );
        p_rdwt_lock->active_writers++;
        p_rdwt_lock->waiting_writers--;
    }

    else if( p_rdwt_lock->waiting_readers > 0 )
    {
        /* no writer, reader can continue */ \

        do
        {
            sigtran_sem_post( &p_rdwt_lock->ok_to_read );
            p_rdwt_lock->active_readers++;
            p_rdwt_lock->waiting_readers--;
        }
        while( p_rdwt_lock->waiting_readers > 0 );
    }

    sigtran_mutex_unlock( &p_rdwt_lock->lock );
#else
    p_rdwt_lock = p_rdwt_lock;
#endif
    return SIGTRAN_SUCCESS;
}
#ifdef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  Function Name       : sigtran_thr_cond_init
**
**    Description       : This function initialises conditional variable.
**
**    Returns           : SIGTRAN_SUCCESS/ SIGTRAN_FAILURE
**
**
******************************************************************************/
sigtran_return_t sigtran_thr_cond_init(
    sigtran_thr_cond_t          *p_thr_cond )
{
#ifndef SIGTRAN_SINGLE_THREAD
    *p_thr_cond = qvThreadCondInit();

    /*
     * If NULL is returned, then return failure.
     */
    if( NULL == *p_thr_cond )
    {
        return SIGTRAN_FAILURE;
    }

#else
    p_thr_cond = p_thr_cond;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sigtran_thr_cond_wait
**
**    Description       : This function blocks the conditional variable.
**
**    Returns           : SIGTRAN_SUCCESS
**
**
******************************************************************************/
sigtran_return_t sigtran_thr_cond_wait(
    sigtran_thread_cond_wait_t  *p_thr_cond,
    sigtran_mutex_t             *p_mutex )
{
#ifndef SIGTRAN_SINGLE_THREAD
    qvThreadCondWait( *p_thr_cond, *p_mutex );
#else
    p_thr_cond = p_thr_cond;
    p_mutex = p_mutex;
#endif
    return SIGTRAN_SUCCESS;

}

/******************************************************************************
**  Function Name       : sigtran_thr_cond_signal
**
**    Description       : Unblocks atleast one thread that are blocked on
**                        specified conditional variable.
**
**    Returns           : SIGTRAN_SUCCESS
**
**
******************************************************************************/
sigtran_return_t sigtran_thr_cond_signal(
    sigtran_thr_cond_t          *p_thr_cond )
{
#ifndef SIGTRAN_SINGLE_THREAD
    qvThreadCondSignal( *p_thr_cond );
#else
    p_thr_cond = p_thr_cond;
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sigtran_thr_cond_deinit
**
**    Description       : Destroys the give conditional variable
**
**    Returns           : SIGTRAN_SUCCESS
**
**
******************************************************************************/
sigtran_return_t sigtran_thr_cond_deinit(
    sigtran_thr_cond_t          *p_thr_cond )
{
#ifndef SIGTRAN_SINGLE_THREAD
    qvDestroyThreadCond( *p_thr_cond );
#else
    p_thr_cond = p_thr_cond;
#endif
    return SIGTRAN_SUCCESS;
}
#endif /* SIG_COMMON_WITH_CSPL */
/******************************************************************************
**  FUNCTION :    sigtran_sem_init
**
**  DESCRIPTION:  This function initialises the Semaphore.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_sem_init(
    sigtran_sem_t       *p_sem )
{
#ifdef SIG_COMMON_WITH_CSPL
    *p_sem = qvSemInit();
#else
    *p_sem = sg_sem_init();
#endif

    /*
     * If NULL is returned, then return failure.
     */
    if( NULL == *p_sem )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sigtran_sem_deinit
**
**  DESCRIPTION:  This function de-initialises the Semaphore.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_sem_deinit(
    sigtran_sem_t       *p_sem )
{
#ifdef SIG_COMMON_WITH_CSPL
    qvSemDestroy( *p_sem );
#else
    /*SPR 21141 changes starts*/
    sg_sem_destroy( ( sigtran_pvoid_t )*p_sem );
    /*SPR 21141 changes ends*/
#endif

    return SIGTRAN_SUCCESS;
}


