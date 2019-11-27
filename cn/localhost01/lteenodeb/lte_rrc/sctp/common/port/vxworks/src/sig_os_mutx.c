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
    SEM_ID     *p_sem;

    p_sem = ( SEM_ID * ) qvSysMalloc( sizeof( SEM_ID ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    /* SPR : 18800 */
    *p_sem = semCCreate( SEM_Q_FIFO, SEM_EMPTY );

    /* SPR : 18800 */
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

    SEM_ID     *p_sem;

    p_sem = ( SEM_ID * ) sigtran_os_malloc( sizeof( SEM_ID ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = semBCreate( SEM_Q_FIFO, SEM_EMPTY );

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
    semDelete( *( SEM_ID * )p_sem );
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
    semTake( *( SEM_ID * )sp, WAIT_FOREVER );
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
    semGive( *( SEM_ID * )sp );
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
    SEM_ID     *p_sem;

    p_sem = ( SEM_ID * ) qvSysMalloc( sizeof( sem_t ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = semBCreate( SEM_Q_FIFO, SEM_EMPTY );

    if( NULL == *p_sem )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
#ifdef SIG_COMMON_WITH_CSPL
        qvSysFree( p_sem );
#else
        sigtran_os_free( p_sem );
#endif
        return NULL;
    }

    return p_sem;
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
    semGive( *( SEM_ID * )mutex );
    semTake( *( SEM_ID * )thr_cond, NO_WAIT );
    semTake( *( SEM_ID * )thr_cond, WAIT_FOREVER );
    semTake( *( SEM_ID * )mutex, WAIT_FOREVER );
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
    semGive( *( SEM_ID * )thr_cond );
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
    semDelete( *( SEM_ID * )thr_cond );
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
    /*return  sem_trywait((sem_t*)lock);*/

    return semTake( *( SEM_ID * )lock, NO_WAIT );
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
    semGive( *( SEM_ID * )p_sem );
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
    semTake( *( SEM_ID * )p_sem, WAIT_FOREVER );
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
    SEM_ID     *p_sem;

    p_sem = ( SEM_ID * ) sigtran_os_malloc( sizeof( SEM_ID ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = semBCreate( SEM_Q_FIFO, SEM_EMPTY );

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
    semDelete( *( SEM_ID * )p_sem );
    qvSysFree( p_sem );
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
    SEM_ID     *p_sem;

#ifdef SIG_COMMON_WITH_CSPL
    p_sem = ( SEM_ID * ) qvSysMalloc( sizeof( SEM_ID ) );
#else
    p_sem = ( SEM_ID * ) sigtran_os_malloc( sizeof( SEM_ID ) );
#endif

    if( NULL == p_sem )
    {
        return NULL;
    }

    *p_sem = semMCreate( SEM_Q_FIFO );

    if( NULL == *p_sem )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
#ifdef SIG_COMMON_WITH_CSPL
        qvSysFree( p_sem );
#else
        sigtran_os_free( p_sem );
#endif
        return NULL;
    }

    return ( void * )p_sem;
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
    semTake( *( SEM_ID * )lock, WAIT_FOREVER );
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
    semGive( *( SEM_ID * )lock );
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
    semDelete( *( SEM_ID * )lock );
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
    /*return  sem_trywait((sem_t*)p_lock);*/
    return semTake( *( SEM_ID * )p_lock, NO_WAIT );
}
