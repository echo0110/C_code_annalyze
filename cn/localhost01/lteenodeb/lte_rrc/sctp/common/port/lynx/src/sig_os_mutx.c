/******************************************************************************
 * File Name:
 *      s_cspl_mutex.c
 *
 * Description:
 *      This file contains definition of semaphore related functions which
 *      will be different for various platforms.
 *
 * DATE          NAME           REFERENCE       REASON
 * ---------------------------------------------------
 * 06 Oct 03     Vivek Bansal
 *
 *****************************************************************************/
#include <sig_os.h>
#include <cspl.h>

/******************************************************************************
**  Function Name       : qvSemInit
**
**    Description       : This function in future should be provided by CSPL
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
#ifdef SIG_COMMON_WITH_CSPL

QSEMAPHORE qvSemInit( void )
{
    sem_t     *p_sem;

    p_sem = ( sem_t * ) qvSysMalloc( sizeof( sem_t ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    if( sem_init( p_sem, 0, 0 ) != 0 )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        qvSysFree( p_sem );
        return NULL;
    }

    return ( QSEMAPHORE )p_sem;
}

/****************************************************************************
 * Function Name       : sytem_sem_init
**
**    Description       : This function is used for Initailizing the semaphore,                           bypassing CSPL. This is required since we need to init                          ailize a semaphore before the CSPL is initialized.
**
**    Returns           :
**
**    Functions called  :

 *
 *
 * **************************************************************************/
QSEMAPHORE system_sem_init( void )
{
    sem_t     *p_sem;

    p_sem = ( sem_t * ) sigtran_os_malloc( sizeof( sem_t ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    if( sem_init( p_sem, 0, 0 ) != 0 )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        sigtran_os_free( p_sem );
        return NULL;
    }

    return ( QSEMAPHORE )p_sem;
}



/******************************************************************************
**  Function Name       : qvSemDestroy
**
**    Description       : This function in future should be provided by CSPL
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvSemDestroy( QSEMAPHORE p_sem )
{
    sem_destroy( ( sem_t * )p_sem );
#ifdef SIG_COMMON_WITH_CSPL
    qvSysFree( p_sem );
#else
    sigtran_os_free( p_sem );
#endif
}

/******************************************************************************
**  Function Name       : qvSemWait
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvSemWait( QSEMAPHORE sp )
{
    sem_wait( ( sem_t * ) sp );
}

/******************************************************************************
**  Function Name       : qvSemPost
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvSemPost( QSEMAPHORE sp )
{
    sem_post( ( sem_t * ) sp );
}

/******************************************************************************
**  Function Name       : qvThreadCondInit
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
QCOND qvThreadCondInit( void )
{
    pthread_cond_t  *p_thr_cond;

    p_thr_cond = ( pthread_cond_t * ) qvSysMalloc( sizeof( pthread_cond_t ) );

    if( NULL == p_thr_cond )
    {
        return NULL;
    }

    if( pthread_cond_init( p_thr_cond, NULL ) != 0 )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
#ifdef SIG_COMMON_WITH_CSPL
        qvSysFree( p_thr_cond );
#else
        sigtran_os_free( p_thr_cond );
#endif

        return NULL;
    }

    return p_thr_cond;
}

/******************************************************************************
**  Function Name       : qvThreadCondWait
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvThreadCondWait( QCOND thr_cond, QLOCK mutex )
{
    pthread_cond_wait( ( pthread_cond_t * )thr_cond,
                       ( pthread_mutex_t * )mutex );
}

/******************************************************************************
**  Function Name       : qvThreadCondSignal
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvThreadCondSignal( QCOND thr_cond )
{
    pthread_cond_signal( thr_cond );
}

/******************************************************************************
**  Function Name       : qvDestroyThreadCond
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvDestroyThreadCond( QCOND thr_cond )
{
    pthread_cond_destroy( ( pthread_cond_t * )thr_cond );
#ifdef SIG_COMMON_WITH_CSPL
    qvSysFree( thr_cond );
#else
    sigtran_os_free( thr_cond );
#endif
}

/******************************************************************************
**  Function Name       : qvTryLock
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
int qvTryLock( QLOCK lock )
{
    return  pthread_mutex_trylock( ( pthread_mutex_t * )lock );
}
#endif /*SIG_COMMON_WITH_CSPL*/

/******************************************************************************
**  Function Name       : sg_mutex_create
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void  *sg_mutex_create( void )
{
    pthread_mutex_t     *p_mutex;
#ifdef SIG_COMMON_WITH_CSPL
    p_mutex = ( pthread_mutex_t * ) qvSysMalloc( sizeof( pthread_mutex_t ) );
#else
    p_mutex = ( pthread_mutex_t * ) sigtran_os_malloc( sizeof( pthread_mutex_t ) );
#endif

    if( NULL == p_mutex )
    {
        return NULL;
    }

    if( pthread_mutex_init( p_mutex, NULL ) != 0 )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
#ifdef SIG_COMMON_WITH_CSPL
        qvSysFree( p_mutex );
#else
        sigtran_os_free( p_mutex );

#endif
        return NULL;
    }

    return ( void * )p_mutex;
}

/******************************************************************************
**  Function Name       : sg_mutex_lock
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void    sg_mutex_lock( void *lock )
{
    pthread_mutex_lock( ( pthread_mutex_t * )lock );
}

/******************************************************************************
**  Function Name       : sg_mutex_unlock
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void    sg_mutex_unlock( void *lock )
{
    pthread_mutex_unlock( ( pthread_mutex_t * )lock );
}

/******************************************************************************
**  Function Name       : sg_mutex_destroy
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void    sg_mutex_destroy( void *lock )
{
    pthread_mutex_destroy( ( pthread_mutex_t * )lock );
#ifdef SIG_COMMON_WITH_CSPL
    qvSysFree( lock );
#else
    sigtran_os_free( lock );
#endif
}
/******************************************************************************
**  FUNCTION :    sigtran_os_malloc
**
**  DESCRIPTION:  This function Get memory by using Malloc
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_void_t *sigtran_os_malloc(
    sigtran_U32bit         size )
{
    return malloc( size );

}
/******************************************************************************
**  FUNCTION :    sigtran_os_free
**
**  DESCRIPTION:  This function Free memory taken by Malloc
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_void_t  sigtran_os_free(
    sigtran_void_t       *p_buffer )
{
    free( p_buffer );
}


/******************************************************************************
**  Function Name       : sg_mutex_try_lock
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
int sg_mutex_try_lock( sigtran_void_t *p_lock )
{
    return  pthread_mutex_trylock( ( pthread_mutex_t * )p_lock );
}
