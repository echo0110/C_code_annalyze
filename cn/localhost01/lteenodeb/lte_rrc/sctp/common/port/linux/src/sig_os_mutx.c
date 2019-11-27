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
 * 05 Apr 11     Ravi j Sharma  SPR 20872       Single Threaded Change
 *
 *****************************************************************************/
#include <sig_os.h>
#ifdef SIG_COMMON_WITH_CSPL
#include <cspl.h>

/******************************************************************************
**  Function Name       : qvSemInit
**
**    Description       : This function in future should be provided by CSPL
**
**    Returns           : QSEMAPHORE
**
********************************************************************************/

QSEMAPHORE qvSemInit( void )
{
    sem_t     *p_sem;

    p_sem = ( sem_t * ) qvSysMalloc( sizeof( sem_t ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

    if( sem_init( p_sem, 0, 0 ) != 0 )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        qvSysFree( p_sem );
        return NULL;
    }

#endif
    return ( QSEMAPHORE )p_sem;
}

/****************************************************************************
 * Function Name       : sytem_sem_init
**
**    Description       : This function is used for Initailizing the semaphore,
**                        bypassing CSPL. This is required since we need to
**                        initailize a semaphore before the CSPL is initialized.
**
**    Returns           : QSEMAPHORE
**
 * **************************************************************************/
QSEMAPHORE system_sem_init( void )
{
    sem_t     *p_sem;

    p_sem = ( sem_t * ) sigtran_os_malloc( sizeof( sem_t ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

    if( sem_init( p_sem, 0, 0 ) != 0 )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        sigtran_os_free( p_sem );
        return NULL;
    }

#endif
    return ( QSEMAPHORE )p_sem;
}




/******************************************************************************
**  Function Name       : qvSemDestroy
**
**    Description       : This function in future should be provided by CSPL
                          This function is destroying the semapore.
**
**    Returns           : None
**
**
******************************************************************************/
void qvSemDestroy( QSEMAPHORE p_sem )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    sem_destroy( ( sem_t * )p_sem );
#endif
#ifdef SIG_COMMON_WITH_CSPL
    qvSysFree( p_sem );
#else
    sigtran_os_free( p_sem );
#endif
}
/******************************************************************************
**  Function Name       : qvSemWait
**
**    Description       : This function is used for semaphore wait.
**
**    Returns           : None
**
**
******************************************************************************/
void qvSemWait( QSEMAPHORE sp )
{
#ifdef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    sp = sp;
#else
    sem_wait( ( sem_t * ) sp );
#endif
}

/******************************************************************************
**  Function Name       : qvSemPost
**
**    Description       : This function is used for semaphore post.
**
**    Returns           : None
**
**
******************************************************************************/
void qvSemPost( QSEMAPHORE sp )
{
#ifdef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    sp = sp;
#else
    sem_post( ( sem_t * ) sp );
#endif
    return;
}

/******************************************************************************
**  Function Name       : qvThreadCondInit
**
**    Description       :  This function initialises conditional variable.
**
**
**    Returns           :  void *
**
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

#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

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

#endif
    return p_thr_cond;
}

/******************************************************************************
**  Function Name       : qvThreadCondWait
**
**    Description       :This function blocks the conditional variable.
**
**
**    Returns           :None
**
**
******************************************************************************/
void qvThreadCondWait( QCOND thr_cond, QLOCK mutex )
{
    /*KW fix start*/
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    if( pthread_cond_wait( ( pthread_cond_t * )thr_cond,
                           ( pthread_mutex_t * )mutex ) == 0 )
#else
    thr_cond = thr_cond;

    mutex = mutex;
#endif
        return;
    /*KW fix end*/
}

/******************************************************************************
**  Function Name       : qvThreadCondSignal
**
**    Description       :Unblocks atleast one thread that are blocked on
**                       specified conditional variable.
**    Returns           :None
**
**
******************************************************************************/
void qvThreadCondSignal( QCOND thr_cond )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    pthread_cond_signal( thr_cond );
#else
    thr_cond = thr_cond;
#endif
}

/******************************************************************************
**  Function Name       : qvDestroyThreadCond
**
**    Description       :Destroys the give conditional variable
**
**    Returns           :None
**
**
******************************************************************************/
void qvDestroyThreadCond( QCOND thr_cond )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

    /*KW fix start*/
    if( pthread_cond_destroy( ( pthread_cond_t * )thr_cond ) == 0 )
#endif
    {
#ifdef SIG_COMMON_WITH_CSPL
        qvSysFree( thr_cond );
#else
        sigtran_os_free( thr_cond );
#endif
    }

    /*KW fix end*/
}

/******************************************************************************
**  Function Name       : qvTryLock
**
**    Description       : Locks the mutex object referenced by lock.
**
**    Returns           : On success returns zero
**
**
******************************************************************************/
int qvTryLock( QLOCK lock )
{
#ifdef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    lock = lock;
    return 0;
#else
    return  pthread_mutex_trylock( ( pthread_mutex_t * )lock );
#endif
}

#else
/******************************************************************************
**  Function Name       : sg_sem_post
**
**    Description       : Unlocks the semaphore
**
**    Returns           : None
**
**
******************************************************************************/

void sg_sem_post( sigtran_pvoid_t p_sem )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    sem_post( ( sem_t * ) p_sem );
#else
    p_sem = p_sem;
#endif
}
/******************************************************************************
**  Function Name       : sg_sem_wait
**
**    Description       : Locks the semaphore
**
**    Returns           : None
**
**
******************************************************************************/

void sg_sem_wait( sigtran_pvoid_t p_sem )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    sem_wait( ( sem_t * ) p_sem );
#else
    p_sem = p_sem;
#endif
}
/******************************************************************************
**  Function Name       : sg_sem_init
**
**    Description       : Used to initialise unnamed semaphore.
**
**    Returns           : Void *
**
**
******************************************************************************/

sigtran_sem_t sg_sem_init()
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
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

    return p_sem;
#else
    sigtran_sem_t     *p_sem;

    p_sem = ( sigtran_sem_t * ) sigtran_os_malloc( sizeof( sigtran_sem_t ) );

    if( NULL == p_sem )
    {
        return NULL;
    }

    return NULL;
#endif
}
/******************************************************************************
**  Function Name       : sg_sem_destroy
**
**    Description       : Used to destroy unnamed semaphore.
**
**    Returns           : None
**
**
******************************************************************************/

void sg_sem_destroy( sigtran_pvoid_t p_sem )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    sem_destroy( ( sem_t * )p_sem );
#endif
    sigtran_os_free( p_sem );
}
#endif /*SIG_COMMON_WITH_CSPL*/

/******************************************************************************
**  Function Name       : sg_mutex_create
**
**    Description       : This function will create the mutex
**
**    Returns           : void *
**
******************************************************************************/
void  *sg_mutex_create( void )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
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

#else
    int     *p_mutex;
#ifdef SIG_COMMON_WITH_CSPL
    p_mutex = ( int * ) qvSysMalloc( sizeof( int ) );
#else
    p_mutex = ( int * ) sigtran_os_malloc( sizeof( int ) );
#endif

#endif
    return ( void * )p_mutex;
}

/******************************************************************************
**  Function Name       : sg_mutex_lock
**
**    Description       : Locks the mutex object referenced by lock.
**
**    Returns           : None
**
**
******************************************************************************/
void    sg_mutex_lock( void *lock )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

    /*KW fix start*/
    if( pthread_mutex_lock( ( pthread_mutex_t * )lock ) == 0 )
    {
        return;
    }

    /*KW fix end*/
#else
    lock = lock;
    return;
#endif
}

/******************************************************************************
**  Function Name       : sg_mutex_unlock
**
**    Description       : UnLocks the mutex object referenced by lock.
**
**    Returns           : None
**
**
******************************************************************************/
void    sg_mutex_unlock( void *lock )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

    /*KW fix start*/
    if( pthread_mutex_unlock( ( pthread_mutex_t * )lock ) == 0 )
    {
        return;
    }

    /*KW fix end*/
#else
    lock = lock;
    return;
#endif
}

/******************************************************************************
**  Function Name       : sg_mutex_destroy
**
**    Description       : Destroys the mutex object referenced by lock.
**
**    Returns           : None
**
**
******************************************************************************/
void    sg_mutex_destroy( void *lock )
{
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

    if( pthread_mutex_destroy( ( pthread_mutex_t * )lock ) == 0 )
#endif
    {
#ifdef SIG_COMMON_WITH_CSPL
        qvSysFree( lock );
#else
        sigtran_os_free( lock );
#endif
    }
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
**    Description       : locks mutex
**
**    Returns           : Returns 0 on success.
**
**
******************************************************************************/
int sg_mutex_try_lock( sigtran_void_t *p_lock )
{
#ifdef SIGTRAN_SINGLE_THREAD /* SPR 20872 */
    p_lock = p_lock;
    return 0;
#else
    return  pthread_mutex_trylock( ( pthread_mutex_t * )p_lock );
#endif
}
