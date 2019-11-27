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

/* Changes for SPR-21022 Starts here (Vxworks Execution) */
#ifdef VXWORKS_6_9_SUPPORT
#define SIG_MAX_THRS 10
#define SIG_NUM_KEY_ELEMENTS 2
long int test_key[SIG_NUM_KEY_ELEMENTS][SIG_MAX_THRS];
#else
sigtran_pvoid_t sg_task_data;
#endif
/* Changes for SPR-21022 Ends here (Vxworks Execution) */

/* this data sturcture is used in intilising new channel
 * in function sig_new_channel.We donot use and need this
 * data structure directly .This is just to fullfil cspl requirement
 * for using driverloop function and the structure is borrowed as it is
 * from the sample port provided by them
 */
typedef struct __waitdata
{
    SEM_ID  condition;
    SEM_ID  lock;
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
    int priority = 80;
    int task_options = 0;
    int stack_size = 60000;

#ifdef VXWORKS_6_9_64BIT_SUPPORT
    *( int * )p_thr_id = taskSpawn( "Sigtran ES", priority, task_options,
                                    stack_size, ( FUNCPTR )start_func, ( long int )arg, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
#else
    *( int * )p_thr_id = taskSpawn( "Sigtran ES", priority, task_options,
                                    stack_size, ( FUNCPTR )start_func, ( int )arg, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
#endif
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

    ULONG   tick = tickGet();

    now->s = tick / SYS_CLKRATE;
    now->us = ( tick % SYS_CLKRATE ) * ( 1000000 / SYS_CLKRATE );

    /* Uncomment the code below if the above code does not work
    sigtran_time_t  T;

    clock_gettime(CLOCK_REALTIME, &T);
    now->s = T.tv_sec;
    now->us = T.tv_usec;
    */
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

    w->lock = semCCreate( SEM_Q_FIFO, 1 );

    if( NULL == w->lock )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        qvSysFree( w );
        return NULL;
    }

    w->condition = semBCreate( SEM_Q_FIFO, SEM_EMPTY );

    if( NULL == w->condition )
    {
        /*
         * If FAILURE, then de-allocate the memory and return NULL.
         */
        qvSysFree( w );
        return NULL;
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
    ULONG  tick;

    semTake( w->lock, WAIT_FOREVER );

    if( !w->value )
    {
        if( !timeout )
        {
            semGive( w->lock );
            semTake( w->condition, WAIT_FOREVER );
            semTake( w->lock, WAIT_FOREVER );
        }

        else if( timeout->s || timeout->us )
        {
            struct timeval  _T;

            _T.tv_sec = timeout->s;
            _T.tv_usec = timeout->us;

            while( _T.tv_usec >= 1000000L )
            {
                _T.tv_usec -= 1000000L;
                _T.tv_sec++;
            }

            tick = ( _T.tv_sec * SYS_CLKRATE ) + ( ( _T.tv_usec * SYS_CLKRATE ) / 1000000 );
            semGive( w->lock );
            semTake( w->condition, tick );
            semTake( w->lock, WAIT_FOREVER );
        }

        else
        {
            /** Nothing -- zero timeout was specified **/
        }
    }

    w->value = 0;
    semGive( w->lock );
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

    semTake( w->lock, WAIT_FOREVER );
    w->value = 1;
    semGive( w->condition );
    semGive( w->lock );
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
    /* Changes for SPR-21022 Starts here (Vxworks Execution) */
#ifdef VXWORKS_6_9_SUPPORT
    return 0;
#else
    taskVarInit();
#ifdef SIG_VX_CMN_RECV_POLL_THREAD
    taskVarAdd( taskIdSelf(), ( int * )sg_task_data );
#endif
#endif
    /* Changes for SPR-21022 Ends here (Vxworks Execution) */

    return 0;
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
    /* Changes for SPR-21022 Starts here (Vxworks Execution) */
#ifdef VXWORKS_6_9_SUPPORT
    int  x, y;

    for( y = 0; y < SIG_MAX_THRS; y++ )
    {
        if( test_key[0][y] == taskIdSelf() )
        {
            return( ( void * )test_key[1][y] );
        }
    }

    return( 0 );
#else
    key = key;
    return( ( void * )taskVarGet( taskIdSelf(), ( int * )sg_task_data ) );
#endif
    /* Changes for SPR-21022 Ends here (Vxworks Execution) */
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
    /* Changes for SPR-21022 Starts here (Vxworks Execution) */
#ifdef VXWORKS_6_9_SUPPORT
    static int i = 0;
    int j = 0;

    for( j = 0; j < SIG_MAX_THRS; j++ )
    {
        if( test_key[0][j] == taskIdSelf() )
        {
            test_key[0][j] = taskIdSelf();
            test_key[1][j] = ( long int ) value;
        }
    }

    if( j == SIG_MAX_THRS )
    {
        test_key[0][i] = taskIdSelf();
        test_key[1][i] = ( long int ) value;
        i++;
    }

#else
    key = key;
#ifndef SIG_VX_CMN_RECV_POLL_THREAD
    taskVarAdd( taskIdSelf(), ( int * )sg_task_data );
#endif
    taskVarSet( taskIdSelf(), ( int * )sg_task_data, ( int )value );
#endif /*ifdef VXWORKS_6_9_SUPPORT */
    /* Changes for SPR-21022 Ends here (Vxworks Execution) */
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
    int priority = 80;
    int task_options = 0;
    int stack_size = 60000;

#ifdef VXWORKS_6_9_64BIT_SUPPORT
    *( int * )p_thr_id = taskSpawn( "Sigtran ES", priority, task_options,
                                    stack_size, ( FUNCPTR )start_func, ( long int )arg, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
#else
    *( int * )p_thr_id = taskSpawn( "Sigtran ES", priority, task_options,
                                    stack_size, ( FUNCPTR )start_func, ( int )arg, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
#endif
}






