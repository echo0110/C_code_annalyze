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

#ifdef SIG_COMMON_WITH_CSPL
#include <cspl.h>
#endif
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
    HANDLE  *p_sem;

    p_sem = ( HANDLE * ) qvSysMalloc( sizeof( HANDLE ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = CreateSemaphore( NULL, 0, 1, NULL );

    if( NULL == *p_sem )
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
    HANDLE  *p_sem;

    p_sem = ( HANDLE * ) sigtran_os_malloc( sizeof( HANDLE ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = CreateSemaphore( NULL, 0, 1, NULL );

    if( NULL == *p_sem )
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
    CloseHandle( *( HANDLE * )p_sem );
    qvSysFree( p_sem );
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
    WaitForSingleObject( *( HANDLE * )sp, INFINITE );
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
    ReleaseSemaphore( *( HANDLE * ) sp, 1, NULL );
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
    HANDLE  *p_sem;

    p_sem = ( HANDLE * ) qvSysMalloc( sizeof( HANDLE ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = CreateSemaphore( NULL, 0, 1, NULL );

    if( NULL == *p_sem )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        qvSysFree( p_sem );
        return NULL;
    }

    return ( QSEMAPHORE )p_sem;
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
    ReleaseMutex( *( HANDLE * )mutex );
    WaitForSingleObject( *( HANDLE * )thr_cond, INFINITE );
    WaitForSingleObject( *( HANDLE * )mutex, INFINITE );
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
    ReleaseSemaphore( *( HANDLE * ) thr_cond, 1, NULL );
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
    CloseHandle( *( HANDLE * )thr_cond );
    qvSysFree( thr_cond );
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
    return WaitForSingleObject( *( HANDLE * )lock, 0 );
}
#else
/******************************************************************************
**  Function Name       : sg_sem_post
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/

void sg_sem_post( sigtran_pvoid_t p_sem )
{
    ReleaseSemaphore( *( HANDLE * ) p_sem, 1, NULL );
}
/******************************************************************************
**  Function Name       : sg_sem_wait
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/

void sg_sem_wait( sigtran_pvoid_t p_sem )
{
    WaitForSingleObject( *( HANDLE * )p_sem, INFINITE );
}

/******************************************************************************
**  Function Name       : sg_sem_init
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/

sigtran_sem_t sg_sem_init()
{
    HANDLE  *p_sem;

    p_sem = ( HANDLE * ) sigtran_os_malloc( sizeof( HANDLE ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = CreateSemaphore( NULL, 0, 1, NULL );

    if( NULL == *p_sem )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        sigtran_os_free( p_sem );
        return NULL;
    }

    return ( sigtran_sem_t )p_sem;

}
/******************************************************************************
**  Function Name       : sg_sem_destroy
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/

void sg_sem_destroy( sigtran_pvoid_t p_sem )
{
    CloseHandle( *( HANDLE * )p_sem );
    sigtran_os_free( p_sem );
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
    HANDLE     *p_mutex;
#ifdef SIG_COMMON_WITH_CSPL
    p_mutex = ( HANDLE * ) qvSysMalloc( sizeof( HANDLE ) );
#else
    p_mutex = ( HANDLE * ) sigtran_os_malloc( sizeof( HANDLE ) );
#endif

    if( NULL == p_mutex )
    {
        return NULL;
    }

    *p_mutex = CreateMutex( NULL, FALSE, NULL );

    if( NULL == *p_mutex )
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
    WaitForSingleObject( *( HANDLE * )lock, INFINITE );
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
    ReleaseMutex( *( HANDLE * )lock );
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
    CloseHandle( *( HANDLE * )lock );
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
int sg_mutex_try_lock( sigtran_void_t *lock )
{
    return WaitForSingleObject( *( HANDLE * )lock, 0 );
}


