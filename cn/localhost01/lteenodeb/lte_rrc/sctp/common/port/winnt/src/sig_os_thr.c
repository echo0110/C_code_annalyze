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
    HANDLE  condition;
    HANDLE  lock;
    int     value;
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
    CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )start_func,
                  ( LPVOID )arg, 0, ( LPDWORD )p_thr_id );
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
#ifdef SIGTRAN_LOCAL_TIME_THREAD
    sigtran_time_t T;
    sigtran_get_local_time( &T );

    now->s  = T.tv_sec;
    now->us = T.tv_usec;
#else
    DWORD   ms = GetTickCount();

    now->s = ms / 1000;
    now->us = 1000 * ( ms % 1000 );
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

    w->lock = CreateMutex( NULL, FALSE, NULL );

    if( NULL == w->lock )
    {
        qvSysFree( w );
        return 0;
    }

    w->condition = CreateSemaphore( NULL, 0, 1, NULL );

    if( NULL == w->condition )
    {
        qvSysFree( w );
        return 0;
    }

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
    DWORD           dwMilliseconds;

    WaitForSingleObject( w->lock, INFINITE );

    if( !w->value )
    {
        if( !timeout )
        {
            ReleaseMutex( w->lock );
            WaitForSingleObject( w->condition, INFINITE );
            WaitForSingleObject( w->lock, INFINITE );
        }

        else if( timeout->s || timeout->us )
        {
            dwMilliseconds = ( timeout->s * 1000 ) +
                             ( timeout->us / 1000 );

            ReleaseMutex( w->lock );
            WaitForSingleObject( w->condition, dwMilliseconds );
            WaitForSingleObject( w->lock, INFINITE );
        }

        else
        {
            /** Nothing -- zero timeout was specified **/
        }
    }

    w->value = 0;
    ReleaseMutex( w->lock );
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

    WaitForSingleObject( w->lock, INFINITE );
    w->value = 1;
    ReleaseSemaphore( w->condition, 1, NULL );
    ReleaseMutex( w->lock );
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
    return ( sigtran_void_t * )TlsAlloc();
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
    return  TlsGetValue( ( DWORD )key );
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
    TlsSetValue( ( DWORD )key, value );
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
    CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )start_func,
                  ( LPVOID )arg, 0, ( LPDWORD )p_thr_id );
}
