/***************************************************************************
 *  FILE NAME  : s_ostmr.h
 *
 *  DESCRIPTION: Contains the Timer module related functions.
 *
 *  DATE       NAME         REF#         REASON
 *  ------------------------------------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  27Now03    Vivek Bansal              CSPL Changes
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_timer_SEEN__
#define __FILE_sctp_timer_SEEN__

#include <s_cogbl.h>
#include <s_cotyps.h>

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * The Node used for the timer manipulation.
     * Each timer running is an instance of this struture.
     */
    typedef struct
    {
        struct node          __header__;
        sctp_time_t          begin_time;
        sctp_time_t          end_time;
        sctp_U32bit          timer_id;
        sctp_timer_buffer_st timer_buffer;
        sctp_Boolean_t       is_running;
    } sctp_timer_node_st;

    extern void sctp_get_local_sys_time( sctp_time_t *local_time );
    extern sctp_time_t sctp_local_sys_time;

#define __GET_ABSOLUTE_SYSTIME(t)     sctp_get_local_sys_time(t)
#define __GETSYSTIME(t)                 __GET_ABSOLUTE_SYSTIME(t)

    sctp_return_t
    sctp_api_start_timer(
        sctp_timer_buffer_st *p_tbuf,
        sctp_U32bit          timeout,
        sctp_timer_t         *timer_id,
        sctp_error_t         *p_ecode );

    sctp_return_t
    sctp_api_stop_timer(
        sctp_timer_t      *timer_id,
        sctp_U32bit       *time_value,
        sctp_error_t      *p_ecode );


    sctp_return_t
    sctp_init_timer_module(
        sctp_U32bit       max_timers,
        sctp_U32bit       max_assoc,
        sctp_error_t      *p_ecode );

    sctp_void_t
    sctp_deinit_timer_module( sctp_void_t );

    void
    sctp_process_time_tick(
        sctp_timer_t               timer_id,
        sctp_timer_buffer_st       *p_timer_buffer );

    sctp_return_t
    sctp_start_timer(
        sctp_tcb_st      *p_ass,
        sctp_U32bit      event,
        sctp_U32bit      timeout,
        sctp_U32bit      addr_index,
        sctp_timer_t     *timer_id,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_stop_timer(
        sctp_timer_t     *timer_id,
        sctp_U32bit      *time_value,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_calculate_time_difference(
        sctp_time_t      *now,
        sctp_time_t      *then,
        sctp_U32bit      *time_value );

    sctp_return_t
    sctp_timer_get_event_rem_time(
        sctp_timer_t      timer_id,
        sctp_U32bit       *event,
        sctp_U32bit       *addr_index,
        sctp_U32bit       *rem_time );

    sctp_return_t
    sctp_timer_update_assoc_id(
        sctp_tcb_st   *p_tcb );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_timer_SEEN__ */

