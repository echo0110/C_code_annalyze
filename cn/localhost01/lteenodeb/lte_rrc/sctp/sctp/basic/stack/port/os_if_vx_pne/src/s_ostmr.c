/******************************************************************************
 *    FILE NAME:  s_ostmr.c
 *
 *    DESCRIPTION: This file implements the timer module funcions.
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -           .Original Creation
 *    16Aug01       ygahlaut         -            REL 3.0 Update
 *    12Dec02       mrajpal          -            rel 4.0
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "OS"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_colst.h>
#include <s_comac.h>
#include <s_osmem.h>
#include <s_mmmain.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_ostmr.h>

#define SCTPMAX_TIMERBUFFER_SIZE    40


sctp_U8bit *free_timer_pool;
static sctp_U32bit sctp_max_timers;

LIST    timerlist;

LIST    free_timerlist;

sctp_U32bit debug_timer = 0;

sctp_lock_t sctp_timer_list_lock;
#define SCTP_TIMER_LOCK      {SCTP_LOCK(sctp_timer_list_lock);}
#define SCTP_TIMER_UNLOCK    {SCTP_UNLOCK(sctp_timer_list_lock);}


#ifdef SCTP_CLOCK_THREAD

sctp_time_t sctp_local_sys_time;  /*To maintain SCTP's own clock*/

/***************************************************************************
 *  FUNCTION :
 *      sctp_get_local_sys_time
 *
 *  DESCRIPTION:
 *      This function returns the local system time.
 *
 *  RETURN VALUE:
 *      void
 ****************************************************************************/
void
sctp_get_local_sys_time(
    sctp_time_t *local_time )
{
    if( local_time != NULL )
    {
        local_time->tv_usec = sctp_local_sys_time.tv_usec;
        local_time->tv_sec = sctp_local_sys_time.tv_sec;
    }

}

#endif


/***************************************************************************
 *  FUNCTION :
 *      sctp_calculate_time_difference
 *
 *  DESCRIPTION:
 *      This function calculates the time difference and return difference
 *      in Msec. First argument is expected to be a larger than second.
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_calculate_time_difference(
    sctp_time_t    *now,
    sctp_time_t    *then,
    sctp_U32bit    *time_value )
{
    sctp_time_t tmp_now;

    tmp_now.tv_sec = now->tv_sec;
    tmp_now.tv_usec = now->tv_usec;

    if( ( tmp_now.tv_sec < then->tv_sec ) ||
            ( ( tmp_now.tv_sec == then->tv_sec ) &&
              ( tmp_now.tv_usec < then->tv_usec ) ) )
    {
        return ( SCTP_FAILURE );
    }

    while( tmp_now.tv_usec < then->tv_usec )
    {
        tmp_now.tv_sec--;
        tmp_now.tv_usec += 1000000000;
    }

    *time_value = ( tmp_now.tv_sec - then->tv_sec ) * 1000;
    *time_value += ( tmp_now.tv_usec - then->tv_usec ) / 1000000;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_calculate_time_difference, Value = %u", *time_value ) );

    return ( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION :
 *      sctp_api_start_timer
 *
 *  DESCRIPTION:
 *      API to start a new timer. The timeout value is taken in milli seconds.
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_start_timer(
    sctp_timer_buffer_st *p_tbuffer,
    sctp_U32bit          timeout,
    sctp_U32bit          *timer_id,
    sctp_error_t         *p_ecode )
{
    sctp_timer_node_st *T;
    sctp_timer_node_st *scan;
    sctp_time_t        now;

    SCTP_TIMER_LOCK;
    /* semBTake(sctp_timer_list_lock, WAIT_FOREVER); */

    T = ( sctp_timer_node_st * ) lstFirst( &free_timerlist );

    if( T == NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "Start Timer Failed, NO Resources" ) );

        SCTP_SET_ECODE( p_ecode, E_NORESOURCES );

        SCTP_TIMER_UNLOCK;

        return ( SCTP_FAILURE );
    }

    lstDelete( &free_timerlist, ( NODE * ) T );
    T->is_running = SCTP_TRUE;

    __GETSYSTIME( &now );
    T->begin_time.tv_sec  = now.tv_sec;
    T->begin_time.tv_usec = now.tv_usec;

    T->end_time.tv_sec    = now.tv_sec + timeout / 1000;
    T->end_time.tv_usec   = now.tv_usec + ( timeout % 1000 ) * 1000000;

    T->end_time.tv_sec    += T->end_time.tv_usec / 1000000000;
    T->end_time.tv_usec   =  T->end_time.tv_usec % 1000000000;

    /* Copy the timer buffer */
    T->timer_buffer       = *p_tbuffer;

    scan = ( sctp_timer_node_st * )lstFirst( &timerlist );

    if( scan == NULL )
    {
        lstAdd( &timerlist, ( NODE * )T );
        *timer_id = T->timer_id;

        SCTP_TIMER_UNLOCK;

        return ( SCTP_SUCCESS );
    }

    while( scan && ( ( T->end_time.tv_sec > scan->end_time.tv_sec )
                     || ( ( T->end_time.tv_sec == scan->end_time.tv_sec )
                          && ( T->end_time.tv_usec > scan->end_time.tv_usec ) ) ) )
    {
        scan = ( sctp_timer_node_st * )lstNext( ( NODE * )scan );
    }

    if( scan == NULL )
    {
        /* Reached end of list */
        lstAdd( &timerlist, ( NODE * )T );
    }

    else
    {
        lstInsert( &timerlist, lstPrevious( ( NODE * )scan ), ( NODE * )T );
    }

    *timer_id = T->timer_id;

    SCTP_TIMER_UNLOCK;

    return ( SCTP_SUCCESS );

}

/***************************************************************************
 *  FUNCTION :
 *      sctp_api_stop_timer
 *
 *  DESCRIPTION:
 *      Stops the timer that is running.
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_stop_timer(
    sctp_U32bit    timer_id,
    sctp_U32bit    *time_value,
    sctp_error_t   *p_ecode )
{
    sctp_time_t        now;
    sctp_U32bit        time_diff;
    sctp_timer_node_st *scan = ( sctp_timer_node_st * )free_timer_pool;

    SCTP_TIMER_LOCK;

    if( sctp_max_timers >= timer_id )
    {
        scan += timer_id - 1;

        if( SCTP_FALSE == scan->is_running )
        {
            SCTP_SET_ECODE( p_ecode, EINV_PARAMS ); /* no existing timer id */

            SCTP_TIMER_UNLOCK;

            return ( SCTP_FAILURE );
        }

        lstDelete( &timerlist, ( NODE * )scan );
        lstAdd( &free_timerlist, ( NODE * ) scan );
        scan->is_running = SCTP_FALSE;
        __GETSYSTIME( &now );

        if( sctp_calculate_time_difference( &now,
                                            &scan->begin_time, &time_diff ) == SCTP_FAILURE )
        {
            time_diff = 0;
        }

        if( time_value != NULL )
        {
            *time_value = time_diff;
        }

        SCTP_TIMER_UNLOCK;

        return ( SCTP_SUCCESS );
    }

    SCTP_SET_ECODE( p_ecode, EINV_PARAMS ); /* no existing timer id */

    SCTP_TIMER_UNLOCK;

    return ( SCTP_FAILURE );

}

/***************************************************************************
 *    FUNCTION :
 *        sctp_init_timer_module
 *
 *    DESCRIPTION:
 *        Initializes the timer module.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_init_timer_module(
    sctp_U32bit     max_timers,
    sctp_error_t    *p_ecode )
{
    sctp_U32bit        counter;
    sctp_timer_node_st *T;

    free_timer_pool = ( sctp_U8bit * )__MALLOC( max_timers *
                                                sizeof( sctp_timer_node_st ) );

    SCTP_INIT_LOCK( sctp_timer_list_lock );

    if( free_timer_pool == NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "Timer Module Initialisation Failed, No Resources" ) );

        SCTP_SET_ECODE( p_ecode, E_NORESOURCES );

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "Timer Module Initialised Successfully" ) );

    __MEMSET( free_timer_pool, 0, max_timers * sizeof( sctp_timer_node_st ) );

    /* Initialise free list */
    lstInit( &free_timerlist );
    T = ( sctp_timer_node_st * )( free_timer_pool );

    for( counter = 0; counter < max_timers; counter++ )
    {
        T[counter].timer_id = counter + 1 ;
        lstAdd( &free_timerlist, ( NODE * )( &T[counter] ) );
    }

    /* Initialise the timer  list */
    lstInit( &timerlist );


#ifdef SCTP_CLOCK_THREAD
    sctp_local_sys_time.tv_sec = 0;
    sctp_local_sys_time.tv_usec = 0;
#endif

    sctp_max_timers = max_timers;
    return ( SCTP_SUCCESS );

}


/***************************************************************************
 *  FUNCTION :
 *      sctp_process_time_tick
 *
 *  DESCRIPTION:
 *      Processes the timer tick to check if timer expired.
 *
 *  RETURN VALUE:
 *      Void
 ****************************************************************************/
void
sctp_process_time_tick(
    void )
{
    sctp_timer_node_st *T;
    sctp_time_t        now;

    __GETSYSTIME( &now );

    SCTP_TIMER_LOCK;

    for( ; ; )
    {
        T = ( sctp_timer_node_st * )lstFirst( &timerlist );

        if( T == 0 ) /* No timers in list.*/
        {
            break;
        }

        if( ( now.tv_sec > T->end_time.tv_sec )
                || ( ( now.tv_sec == T->end_time.tv_sec )
                     && ( now.tv_usec >= T->end_time.tv_usec ) ) )
        {
            /* Call main module entry level function */
            lstDelete( &timerlist, ( NODE * )T );
            lstAdd( &free_timerlist, ( NODE * )T );
            T->is_running = SCTP_FALSE;

            if( debug_timer == 1 )

            {
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Timeout %lu %lu \r\n", now.tv_sec, T->end_time.tv_sec ) );
            }

            SCTP_TIMER_UNLOCK;

            sctp_process_timer_expiry( T->timer_id, &( T->timer_buffer ) );

            SCTP_TIMER_LOCK;

            /* Repeat the loop from the begining,in case of more timers*/
            continue;
        }

        /*
         * Timer at head of list not ready to expire.
         *  So no need to process any more timers.
         */

        break;

    }

    SCTP_TIMER_UNLOCK;

}

/***************************************************************************
 *  FUNCTION :
 *      sctp_start_timer
 *
 *  DESCRIPTION:
 *      Starts a timer.
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_start_timer(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    event,
    sctp_U32bit    timeout,
    sctp_U32bit    addr_index,
    sctp_U32bit    *p_tid,
    sctp_error_t   *p_ecode )
{
    sctp_timer_buffer_st tbuf;

    if( p_ass )
    {
        tbuf.p_ass      = p_ass;
        tbuf.assoc_id   = p_ass->assoc_id;
        tbuf.ep_id      = p_ass->p_ep->ep_id;
    }

    tbuf.event      = event;
    tbuf.addr_index = addr_index;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_start_timer" ) );

    if( SCTP_SUCCESS != sctp_api_start_timer( &tbuf, timeout, p_tid, p_ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "Start timer Failed " ) );

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "New Destination Timer, Time=%luMS, Evt=%u, Id=%u, Addr Index=%u",
                  timeout, event, *p_tid, addr_index ) );

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :
 *      sctp_stop_timer
 *
 *  DESCRIPTION:
 *      Stops a Timer.
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_stop_timer(
    sctp_U32bit    *timer_id,
    sctp_U32bit    *time_value,
    sctp_error_t   *p_ecode )
{
    if( *timer_id == SCTP_NULL_TIMER_ID )
    {
        return ( SCTP_SUCCESS );
    }

    if( SCTP_SUCCESS == sctp_api_stop_timer( *timer_id, time_value, p_ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "Timer Stopped,Timer Id = %u", *timer_id ) );

        *timer_id = SCTP_NULL_TIMER_ID;

        return ( SCTP_SUCCESS );
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :
 *      sctp_timer_get_event_rem_time
 *
 *  DESCRIPTION:
 *      Returns time remaining of a timer in milliseconds.
 *  This functions returns 1 ms if timer already expired, 0 if timer
 *  does not exist (in case of failure).
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_timer_get_event_rem_time(
    sctp_U32bit    timer_id,
    sctp_U32bit    *event,
    sctp_U32bit    *addr_index,
    sctp_U32bit    *rem_time )
{
    sctp_timer_node_st    *scan   = SCTP_NULL;
    sctp_timer_buffer_st  *buffer = SCTP_NULL;
    sctp_time_t            now;

    if( timer_id == SCTP_NULL_TIMER_ID )
    {
        *rem_time = 0;
        *event = 0;
        *addr_index = 0;
        return( SCTP_SUCCESS );
    }

    SCTP_TIMER_LOCK;

    scan = ( sctp_timer_node_st * )lstFirst( &timerlist );

    for( ; scan ; scan = ( sctp_timer_node_st * )lstNext( ( NODE * )scan ) )
    {
        if( scan->timer_id == timer_id )
        {
            __GETSYSTIME( &now );

            /* Be careful while modifying call to calculate time difference  */
            /* Order of parameters should be as written below */
            if( sctp_calculate_time_difference( &scan->end_time,
                                                &now, rem_time ) == SCTP_FAILURE )
            {
                /* Return remaining time 1 ms in case of timer alrdy expired */
                *rem_time = 1;
            }

            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "sctp_timer_get_rem_time, Value = %u", *rem_time ) );

            buffer = &( scan->timer_buffer );
            *event = buffer->event;
            *addr_index = buffer->addr_index;

            SCTP_TIMER_UNLOCK;

            return ( SCTP_SUCCESS );
        }
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_timer_get_rem_time,Timer Id not found" ) );

    *rem_time   = 0;
    *event      = 0;
    *addr_index = 0;

    SCTP_TIMER_UNLOCK;

    return ( SCTP_FAILURE );
}





/***************************************************************************
 *  FUNCTION :
 *      sctp_timer_update_assoc_id
 *
 *  DESCRIPTION:
 *      This function update the ep id and assoc ids stored in the timer
 *  to the new values from the tcb. This is called while creating a
 *  duplicate ep.
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_timer_update_assoc_id(
    sctp_tcb_st   *p_tcb )
{
    sctp_timer_node_st *scan;
    SCTP_TIMER_LOCK;
    scan = ( sctp_timer_node_st * )lstFirst( &timerlist );

    for( ; scan ; scan = ( sctp_timer_node_st * )lstNext( ( NODE * )scan ) )
    {
        if( scan->timer_buffer.p_ass == p_tcb )
        {
            scan->timer_buffer.ep_id    = p_tcb->p_ep->ep_id;
            scan->timer_buffer.assoc_id = p_tcb->assoc_id;
        }
    }

    SCTP_TIMER_UNLOCK;
    return SCTP_SUCCESS;
}

