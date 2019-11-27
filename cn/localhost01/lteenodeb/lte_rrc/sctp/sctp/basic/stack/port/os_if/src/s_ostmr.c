/******************************************************************************
 *    FILE NAME:  s_ostmr.c
 *
 *    DESCRIPTION: This file implements the timer module funcions.
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -           .Original Creation
 *    16Aug01       ygahlaut         -            REL 3.0 Update
 *    07Oct03       Vivek Bansal     Rel 5.0      Changes for CSPL
 *    27Nov03       Vivek Bansal     Rel 5.0      Changes for CSPL
 *    05Apr11       Ravi J Sharma    SPR 20872    Single Threaded Change
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "OS"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_colst.h>
#include <s_osmem.h>
#include <s_mmmain.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_ostmr.h>
#include <s_osmutx.h>
#include <s_es.h>
#include <s_red.h>
#include <s_comac.h>
#include <sig_os.h>

sctp_pool_id_t       g_sctp_timer_mem_poolid[2];
sctp_timer_list_t    g_sctp_timer_list;


/***************************************************************************
 *  FUNCTION sigtran_timer_list_t
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
    sctp_time_t   *local_time )
{
    if( local_time != SCTP_NULL )
    {
        sigtran_gettimeofday( local_time );
    }
}

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
        tmp_now.tv_usec += 1000000;
    }

    *time_value = ( tmp_now.tv_sec - then->tv_sec ) * 1000;
    *time_value += ( tmp_now.tv_usec - then->tv_usec ) / 1000;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_calculate_time_difference, Value = %u", *time_value ) );

    return ( SCTP_SUCCESS );
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
    sctp_U32bit     max_assoc,
    sctp_error_t    *p_ecode )
{
    /* Not sure if anything is required here for CSPL related to timer*/
    sigtran_return_t        ret_val;
    sigtran_error_t         ecode;
    sigtran_pool_info_st    pool_info[2];

    /*
     * Initialise the sigtran common timer module.
     */

#ifndef SCTP_SINGLE_THREAD_SUPPORT /* SPR 20872*/
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    ret_val = sigtran_timer_init( SIG_SCTP_STACK, SCTP_NULL,
                                  0, max_timers, 0, 0, &g_sctp_timer_list, SIGTRAN_FALSE, &ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    ret_val = sigtran_timer_init( SIG_SCTP_STACK,
                                  ( sig_timer_exp_cbk_ptr_t ) SCTP_NULL,
                                  0, max_timers, 0, 0, &g_sctp_timer_list, SIGTRAN_FALSE, &ecode );
#endif

    if( SIGTRAN_FAILURE == ret_val )
    {
        *p_ecode = E_TIMER_INIT_FAILED;
        return SCTP_FAILURE;
    }

#endif

    /*
     * Initialising the memory pool for timers.
     */
    pool_info[0].buf_size  = sizeof( sctp_timer_buffer_st );
    pool_info[0].num_buf   = max_timers;
    pool_info[1].buf_size  = sizeof( sctp_verf_list_ref_tcb_st );
    pool_info[1].num_buf   = max_assoc;

    ret_val = sigtran_mem_init( SIG_SCTP_STACK, 0, 0, 0, 2, pool_info,
                                &g_sctp_timer_mem_poolid[0], SIGTRAN_FALSE, &ecode );

    if( SIGTRAN_FAILURE == ret_val )
    {
        *p_ecode = E_MALLOC_FAILED;
        return SCTP_FAILURE;
    }

    return SCTP_SUCCESS;
}


/******************************************************************************
**  Function Name       : sctp_deinit_timer_module
**
**    Description       :
**      Deinitializes the timer module.
**
**    Returns           :
**      Void
**
******************************************************************************/
sctp_void_t sctp_deinit_timer_module()
{
    sigtran_error_t         ecode;

    sigtran_timer_deinit( SIG_SCTP_STACK, g_sctp_timer_list, &ecode );

    sigtran_mem_deinit( SIG_SCTP_STACK, 0, 0, 0, 1, 0,
                        &g_sctp_timer_mem_poolid[0], SIGTRAN_FALSE, &ecode );
}

/***************************************************************************
 *  FUNCTION :
 *      sctp_process_time_tick
 *
 *  DESCRIPTION:
 *      Processes the timer tick to check if timer expired.
 *
 *  RETURN VALUE:
 *      void
 ****************************************************************************/
void
sctp_process_time_tick(
    sctp_timer_t               timer_id,
    sctp_timer_buffer_st       *p_timer_buffer )
{
#ifdef SCTP_RED_SUPPORT
    sctp_U8bit              *p_red_buffer;
    sctp_U8bit              *p_temp_buffer;
    sctp_U32bit             nooctets;
    sig_stack_type_et       stack_type;
    sctp_error_t            ecode;
#endif

#ifdef SCTP_RED_SUPPORT

    if( sctp_stack_red_state == SCTP_STACK_RED_STATE_ACTIVE )
    {
#ifndef SCTP_CLI_RECV_COM_THREAD
        stack_type = SIG_SCTP_RECV_STACK;
#else
        stack_type = SIG_SCTP_STACK;
#endif

        p_red_buffer = ( sctp_U8bit * )__MESG_MALLOC( SCTP_RDNCY_MESG_POOL,
                                                      SCTP_MAX_RED_MEM );

        if( SCTP_NULL == p_red_buffer )
        {
            return;
        }

        p_temp_buffer = p_red_buffer;

        /* set event type into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               ( sctp_U32bit )SCTP_EVT_TIMER );

        /*changes to include total length = nooctets */

        p_temp_buffer = p_temp_buffer + 4;

        /* set timer_id into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer,
                                                sizeof( sctp_timer_t ),
                                                ( sctp_U8bit * )( &timer_id ) );

        /* set timer buffer data into the sending event buffer */
        p_temp_buffer = sctp_buffer_set_Buffer( p_temp_buffer,
                                                sizeof( sctp_timer_buffer_st ),
                                                ( sctp_U8bit * )( p_timer_buffer ) );

        nooctets = p_temp_buffer - p_red_buffer;

        p_temp_buffer = p_red_buffer + 4;

        p_temp_buffer = sctp_buffer_set_Ulong( p_temp_buffer,
                                               nooctets );


        /*
         * Assuming that the module Id of the Redundancy Manager is same
         * as that of SU, then send the message.
         */
        if( SIGTRAN_FAILURE == sig_send( stack_type, SIG_DEST_TYPE_SM,
                                         g_sctp_rm_dest_id,
                                         ( sigtran_pvoid_t ) p_red_buffer, nooctets, &ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "Cannot Send Timer Event to Queue" ) );
            return;
        }
    }

#else

    sctp_process_timer_expiry( timer_id, p_timer_buffer );

#endif /*SCTP_RED_SUPPORT*/

    /*
     * Dealloc the memory taken for the timer.
     */
    sigtran_free_buffer( SIG_SCTP_STACK, SCTP_FALSE,
                         &( g_sctp_timer_mem_poolid[0] ), ( sigtran_buffer_t * )&p_timer_buffer );

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
    sctp_timer_t   *p_tid,
    sctp_error_t   *p_ecode )
{
    sctp_timer_buffer_st *p_tbuf;
    sigtran_error_t      ecode;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    p_tbuf = ( sctp_timer_buffer_st * )sigtran_get_buffer( SIG_SCTP_STACK,
                                                           SCTP_FALSE, &( g_sctp_timer_mem_poolid[0] ), sizeof( sctp_timer_buffer_st ) );

    if( SCTP_NULL == p_tbuf )
    {
        return SCTP_FAILURE;
    }

    if( p_ass )
    {
        p_tbuf->p_ass      = p_ass;
        p_tbuf->assoc_id   = p_ass->assoc_id;
        p_tbuf->ep_id      = p_ass->p_ep->ep_id;
    }

    p_tbuf->event      = event;
    p_tbuf->addr_index = addr_index;

#ifdef SCTP_CMN_RECV_POLL_THREAD

    if( SCTPEVENT_TIMER_DUMMY != p_tbuf->event )
    {
#endif
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_start_timer" ) );
#ifdef SCTP_CMN_RECV_POLL_THREAD
    }

#endif

    sigtran_start_timer( SIG_SCTP_STACK, g_sctp_timer_list, ( sigtran_buffer_t )p_tbuf,
                         timeout, p_tid, 0, &ecode );

#ifdef SCTP_CMN_RECV_POLL_THREAD

    if( SCTPEVENT_TIMER_DUMMY != p_tbuf->event )
    {
#endif
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "New Destination Timer, Time=%uMS, Evt=%u, Id=%u, Addr Index=%u",
                      timeout, event, *p_tid, addr_index ) );
#ifdef SCTP_CMN_RECV_POLL_THREAD
    }

#endif

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :
 *      sctp_stop_timer
 *
 *  DESCRIPTION:
 *      Stops a timer
 *
 *  RETURN VALUE:
 *      SCTP_SUCCESS
 *      SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_stop_timer(
    sctp_timer_t   *timer_id,
    sctp_U32bit    *time_value,
    sctp_error_t   *p_ecode )
{
    sctp_timer_buffer_st *p_tbuf;
    sigtran_error_t      ecode;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    /*CSR 1-2420144 : '&' Removed before 'time_value' */
    if( SIGTRAN_SUCCESS ==
            sigtran_stop_timer( g_sctp_timer_list, *timer_id,
                                ( sigtran_buffer_t * )&p_tbuf, ( sigtran_interval_t * )time_value, &ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "Timer Stopped,Timer Id = %u", *timer_id ) );

        *timer_id = SCTP_NULL_TIMER_ID;

        /*
         * Dealloc the memory taken for the timer.
         */
        sigtran_free_buffer( SIG_SCTP_STACK, SCTP_FALSE,
                             &( g_sctp_timer_mem_poolid[0] ), ( sigtran_buffer_t * )&p_tbuf );

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
    sctp_timer_t   timer_id,
    sctp_U32bit    *event,
    sctp_U32bit    *addr_index,
    sctp_U32bit    *rem_time )
{
    sctp_timer_buffer_st  *buffer = SCTP_NULL;
    sigtran_error_t       ecode;

    sigtran_get_rem_time( timer_id, ( sigtran_interval_t * )rem_time, &ecode );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_timer_get_rem_time, Value = %u", *rem_time ) );

    if( SIGTRAN_FAILURE ==
            sigtran_get_timer_buf( 0, timer_id, ( sigtran_buffer_t * )&buffer, &ecode ) )
    {
        return SCTP_FAILURE;
    }

    *event = buffer->event;
    *addr_index = buffer->addr_index;

    return ( SCTP_SUCCESS );
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
    sctp_U32bit             scan;
    sctp_U32bit             counter;
    sctp_timer_buffer_st    *p_buffer = SCTP_NULL;
    sigtran_error_t         ecode;

    for( counter = 0; counter < SCTP_MAX_TRANSPORT_ADDR; counter ++ )
    {
        for( scan = 0; scan < SCTP_DESTTIMER_NUM; scan++ )
        {
            /* Check if the timer Id stored is valid, if null then continue */
            if( SCTP_NULL_TIMER_ID == p_tcb->transmit.remote[counter].
                    dest_timer_id[scan] )
            {
                continue;
            }

            p_buffer = SCTP_NULL;

            sigtran_get_timer_buf( 0,
                                   p_tcb->transmit.remote[counter].dest_timer_id[scan],
                                   ( sigtran_buffer_t * )&p_buffer, &ecode );

            if( SCTP_NULL == p_buffer )
            {
                continue;
            }

            /* Update the fields in the timer buffer */
            p_buffer->ep_id = p_tcb->p_ep->ep_id;
            p_buffer->assoc_id = p_tcb->assoc_id;
        }
    }

    for( counter = 0; counter < SCTP_ASSOCTIMER_NUM; counter ++ )
    {
        /* Check if the timer Id stored is valid, if null then continue */
        if( SCTP_NULL_TIMER_ID == p_tcb->assoc_timer_id[counter] )
        {
            continue;
        }

        p_buffer = SCTP_NULL;

        sigtran_get_timer_buf( 0,
                               p_tcb->assoc_timer_id[counter],
                               ( sigtran_buffer_t * )&p_buffer, &ecode );

        if( SCTP_NULL == p_buffer )
        {
            continue;
        }

        p_buffer->ep_id = p_tcb->p_ep->ep_id;
        p_buffer->assoc_id = p_tcb->assoc_id;
    }

    return SCTP_SUCCESS;
}


