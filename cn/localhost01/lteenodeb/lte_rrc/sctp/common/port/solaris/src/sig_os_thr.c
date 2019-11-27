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
/* this data sturcture is used in intilising new channel
 * in function sig_new_channel.We donot use and need this
 * data structure directly .This is just to fullfil cspl requirement
 * for using driverloop function and the structure is borrowed as it is
 * from the sample port provided by them
 */
typedef struct __waitdata
{
    pthread_cond_t  condition;
    pthread_mutex_t lock;
    int             value;
} sig_waitdata;

/******************************************************************************
**  Function Name       : qvThreadCreate
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void qvThreadCreate
( void *p_thr_id,
  void * ( *start_func )( void * ), void *arg )
{
    pthread_create( ( pthread_t * )p_thr_id, 0, start_func, arg );
}

/* following functions are required by cspl in Qwait structure
 * there implementation is borrowed from cspl sample porting code
 */
/******************************************************************************
**  FUNCTION :    sig_shell_walltime
**
**  DESCRIPTION:  This function is registered with QWAIT structure
**
******************************************************************************/
sigtran_void_t sig_shell_walltime( sig_q_time_st *now )
{
    sigtran_time_t T;
#ifdef SIGTRAN_LOCAL_TIME_THREAD
    sigtran_get_local_time( &T );
    now->s  = T.tv_sec;
    now->us = T.tv_usec;
#else
    gettimeofday( &T, 0 );
    now->s  = T.tv_sec;
    now->us = T.tv_usec;
#endif
}
/******************************************************************************
**  FUNCTION :    sig_shell_walltime
**
**  DESCRIPTION:  This function is registered with QWAIT structure
**
******************************************************************************/
sigtran_pvoid_t sig_new_channel( sigtran_void_t )
{
    sig_waitdata    *w = ( sig_waitdata * )qvSysMalloc( sizeof( sig_waitdata ) );

    pthread_mutex_init( ( pthread_mutex_t * ) & ( w->lock ), 0 );
    pthread_cond_init( ( pthread_cond_t * ) & ( w->condition ), NULL );
    w->value = 0;

    return ( void * )w;
}
/******************************************************************************
**  FUNCTION :    sig_channel_sleep
**
**  DESCRIPTION:  This function is registered with QWAIT structure
**
******************************************************************************/

sigtran_void_t sig_channel_sleep( sigtran_pvoid_t channel,
                                  const sig_q_time_st *timeout )

{
    sig_waitdata    *w = ( sig_waitdata * )channel;
    struct timespec T;

    /*klock fix*/
    if( ( pthread_mutex_lock( ( pthread_mutex_t * ) & ( w->lock ) ) ) == 0 )
    {
        if( !w->value )
        {
            if( !timeout )
            {
                /*klock fix*/
                if( ( pthread_cond_wait( ( pthread_cond_t * ) & ( w->condition ),
                                         ( pthread_mutex_t * ) & ( w->lock ) ) ) != 0 )
                {
                    w->value = 0;

                    /*klock fix*/
                    if( ( pthread_mutex_unlock( ( pthread_mutex_t * ) & ( w->lock ) ) ) == 0 )
                    {
                        return;
                    }
                }
            }

            else if( timeout->s || timeout->us )
            {
                struct timeval  _T;

                gettimeofday( &_T, 0 );

                _T.tv_sec += timeout->s;
                _T.tv_usec += timeout->us;

                while( _T.tv_usec >= 1000000L )
                {
                    _T.tv_usec -= 1000000L;
                    _T.tv_sec++;
                }

                T.tv_sec = _T.tv_sec;
                T.tv_nsec = 1000 * _T.tv_usec;

                if( ( pthread_cond_timedwait( ( pthread_cond_t * ) & ( w->condition ),
                                              ( pthread_mutex_t * ) & ( w->lock ), &T ) ) != 0 )
                {
                    w->value = 0;

                    /*klock fix*/
                    if( ( pthread_mutex_unlock( ( pthread_mutex_t * ) & ( w->lock ) ) ) == 0 )
                    {
                        return;
                    }
                }
            }

            else
            {
                /** Nothing -- zero timeout was specified **/
            }
        }

        w->value = 0;

        /*klock fix*/
        if( ( pthread_mutex_unlock( ( pthread_mutex_t * ) & ( w->lock ) ) ) == 0 )
        {
            return;
        }
    }
}
/******************************************************************************
**  FUNCTION :    sig_channel_wakeup
**
**  DESCRIPTION:  This function is registered with QWAIT structure
**
******************************************************************************/

sigtran_void_t sig_channel_wakeup( sigtran_pvoid_t channel )
{
    sig_waitdata    *w = ( sig_waitdata * )channel;

    /*klock fix*/
    if( ( pthread_mutex_lock( ( pthread_mutex_t * ) & ( w->lock ) ) ) == 0 )
    {
        w->value = 1;
        pthread_cond_signal( ( pthread_cond_t * ) & ( w->condition ) );

        /*klock fix*/
        if( ( pthread_mutex_unlock( ( pthread_mutex_t * ) & ( w->lock ) ) ) == 0 )
        {
            return;
        }
    }
}

/******************************************************************************
**  Function Name       : sg_newkey
**
**    Description       :
**
**    Returns           :
**
**    Functions called  : Function for generating new key for CSPL. Reqd for
**                        multi-threading
**
******************************************************************************/
sigtran_void_t *sg_newkey( sigtran_void_t )
{
    pthread_key_t   *key = ( pthread_key_t * )malloc( sizeof( pthread_key_t ) );
    /* Update for SPR : 18527 Starts Here */
    sigtran_void_t *void_ptr = NULL;

    if( key != NULL )
    {
        pthread_key_create( key, 0 );
        void_ptr = ( sigtran_void_t * )key;
    }

    return void_ptr;
    /* Update for SPR : 18527 Ends Here */
}

/******************************************************************************
**  Function Name       : sg_getkey
**
**    Description       :
**
**    Returns           :
**
**    Functions called  : Required for multi-threading.
**
******************************************************************************/
sigtran_void_t *sg_getkey( sigtran_void_t *key )
{
    return  pthread_getspecific( *( ( pthread_key_t * )key ) );
}

/******************************************************************************
**  Function Name       : sg_setkey
**
**    Description       :
**
**    Returns           :
**
**    Functions called  : Required for multi-threading.
**
******************************************************************************/
sigtran_void_t sg_setkey( sigtran_void_t *key, sigtran_void_t *value )
{
    pthread_setspecific( *( ( pthread_key_t * )key ), value );
}

#endif /* SIG_COMMON_WITH_CSPL */


/******************************************************************************
**  Function Name       : sg_create_thread
**
**    Description       :
**
**    Returns           :
**
**    Functions called  :
**
******************************************************************************/
void sg_create_thread
( void *p_thr_id,
  void * ( *start_func )( void * ), void *arg )
{
    pthread_create( ( pthread_t * )p_thr_id, 0, start_func, arg );
}



