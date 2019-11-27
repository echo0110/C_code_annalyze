/*****************************************************************************
 *    FILE NAME:
 *        sig_tmr.h
 *
 *    DESCRIPTION:
 *       This file has common porting functions for timer.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/


#ifndef  __SIGTRAN_TMR_H__
#define __SIGTRAN_TMR_H__


#include <sig_typs.h>
#include <sig_defs.h>
#include <sig_err.h>

#ifdef SIG_COMMON_WITH_CSPL
#ifdef __cplusplus
extern "C" {
#endif
    /* this service id will be added to each
     * stack time while registering timer thread
     * to make service ids unique for timer thread
     *
     */
#define TIMER_SERVICE_ID_SEED   7

    typedef  struct
    {
        /* driver id for timer thread */
        sig_q_driver_t    q_driver;
        /* module id for timer thread */
        sig_q_module_t    q_module;
        /* mainfest for timer thread */
        sig_q_manifest_st  q_manifest;
        /* call back handler for timer expiry */
        sig_timer_exp_cbk_ptr_t  timer_handler_fn;
    } sig_cspl_timer_info_t;

    typedef struct
    {
        sig_q_driver_t driver_id;
        sigtran_error_t e_code;
    } sig_check_timer_exp_st;

    sigtran_void_t sigtran_conv_time_to_qtime(
        sig_q_time_st *p_q_time,
        sigtran_interval_t duration );


    sigtran_return_t sigtran_is_timer_running(
        sigtran_timer_id_t timer_id,
        sigtran_boolean_t  *p_b_running );

    sigtran_void_t sigtran_fill_qmanifest(
        sig_stack_type_et    stack_type,
        sig_q_manifest_st    *p_mainfest,
        sig_timer_exp_cbk_ptr_t timer_handler_fn );

    /* Timer Related Functions */
    sigtran_return_t sigtran_timer_init(
        sig_stack_type_et   stack_type,
        sig_timer_exp_cbk_ptr_t timer_handler_fn,
        /* following parameters are not being used
         * in cspl*/
        sigtran_U32bit      task_id,
        sigtran_U32bit      max_timers,
        sigtran_U32bit      time_sec,
        sigtran_U32bit      time_usec,
        sigtran_timer_list_t  *p_timer_list_id,
        sigtran_boolean_t   b_start_timer_thread,
        sigtran_error_t     *p_ecode );

    sigtran_return_t sigtran_timer_set_stack_qmodule(
        sig_stack_type_et   stack_type,
        sig_q_module_t      q_module );

    sigtran_return_t sigtran_timer_deinit(
        sig_stack_type_et   stack_type,
        sigtran_timer_list_t timer_list_id,
        sigtran_error_t      *p_ecode );

    sigtran_return_t sigtran_start_timer(
        sig_stack_type_et    stack_type,
        sigtran_timer_list_t timer_list_id,
        sigtran_buffer_t     p_buf,
        sigtran_interval_t   duration,
        sigtran_timer_id_t   *p_timer_id,
        sigtran_callback_ptr_t  callback_fn,
        sigtran_error_t        *p_ecode );

    sigtran_return_t sigtran_stop_timer(
        sigtran_timer_list_t   timer_list_id,
        sigtran_timer_id_t     timer_id,
        sigtran_buffer_t       *pp_buf,
        sigtran_interval_t     *p_rem_duration,
        sigtran_error_t        *p_ecode );


    sigtran_return_t sigtran_get_timer_buf(
        sigtran_timer_list_t   timer_list_id,
        sigtran_timer_id_t     timer_id,
        sigtran_buffer_t       *pp_buf,
        sigtran_error_t        *p_ecode );

    sigtran_pvoid_t sigtran_check_timer(
        sigtran_pvoid_t p_param );

    sigtran_return_t sigtran_get_rem_time(
        sigtran_timer_id_t   timer_id,
        sigtran_interval_t   *p_duration,
        sigtran_error_t       *p_ecode );

    sigtran_void_t sigtran_conv_qtime_to_time(
        sig_q_time_st      q_time,
        sigtran_interval_t *p_duration );

#ifdef __cplusplus
}
#endif

#else /* SIG_COMMON_WITH_CSPL */
#ifdef __cplusplus
extern "C" {
#endif
    /* Timer Related Functions */
    sigtran_pvoid_t sig_timer_callback_fn( sigtran_pvoid_t p_buff );

    sigtran_return_t sigtran_timer_init(
        sig_stack_type_et   stack_type,
        sig_timer_exp_cbk_ptr_t timer_handler_fn,
        /* following parameters are not being used
         * in cspl*/
        sigtran_U32bit      task_id,
        sigtran_U32bit      max_timers,
        sigtran_U32bit      time_sec,
        sigtran_U32bit      time_usec,
        sigtran_timer_list_t  *p_timer_list_id,
        sigtran_boolean_t   b_start_timer_thread,
        sigtran_error_t     *p_ecode );

    sigtran_return_t sigtran_timer_deinit(
        sig_stack_type_et   stack_type,
        sigtran_timer_list_t timer_list_id,
        sigtran_error_t      *p_ecode );

    sigtran_return_t sigtran_start_timer(
        sig_stack_type_et    stack_type,
        sigtran_timer_list_t timer_list_id,
        sigtran_buffer_t     p_buf,
        sigtran_interval_t   duration,
        sigtran_timer_id_t   *p_timer_id,
        sigtran_callback_ptr_t  callback_fn,
        sigtran_error_t        *p_ecode );

    sigtran_return_t sigtran_stop_timer(
        sigtran_timer_list_t   timer_list_id,
        sigtran_timer_id_t     timer_id,
        sigtran_buffer_t       *pp_buf,
        sigtran_interval_t     *p_rem_duration,
        sigtran_error_t        *p_ecode );


    sigtran_return_t sigtran_get_timer_buf(
        sigtran_timer_list_t   timer_list_id,
        sigtran_timer_id_t     timer_id,
        sigtran_buffer_t       *pp_buf,
        sigtran_error_t        *p_ecode );

    sigtran_return_t sigtran_get_rem_time(
        sigtran_timer_id_t   timer_id,
        sigtran_interval_t   *p_duration,
        sigtran_error_t       *p_ecode );

    sigtran_pvoid_t sigtran_check_timer(
        sigtran_pvoid_t p_param );

#ifdef __cplusplus
}
#endif
#endif /*SIG_COMMON_WITH_CSPL*/

#ifdef __cplusplus
extern "C" {
#endif

    /* Time Module Functions*/
    sigtran_return_t sigtran_get_time(
        sigtran_time_sec_t     *p_sig_time );

    sigtran_return_t sigtran_gettimeofday(
        sigtran_time_t   *p_sig_time );

    sigtran_S8bit *sigtran_get_ctime_r(
        sigtran_time_sec_t  *p_sig_time,
        sigtran_S8bit       *p_time_buffer,
        sigtran_U32bit      *p_buf_len );

    sigtran_S8bit *sig_os_get_ctime_r(
        sigtran_time_sec_t  *p_sig_time,
        sigtran_S8bit       *p_time_buffer,
        sigtran_U32bit      *p_buf_len );

#ifdef __cplusplus
}
#endif
#endif
