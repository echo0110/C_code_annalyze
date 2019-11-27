
/*****************************************************************************
*    FILE NAME:
*        sig_tmr.c
*
*    DESCRIPTION:
*       This file has porting function for Timer Related functions.
*
*    DATE           NAME          REFERENCE        REASON
*    ----------------------------------------------------
*    14Sept02    Sandeep Mahajan     -            .Original Creation
*    27Dec 02    Anshoo Sharma       -            Added Timer Locking
*    26Sep 03    Vishavdeep Sharma   -            Added cspl based timers
*    05Apr 11    Ravi J Sharma    SPR 20872       Single Threaded Changes
*    1May13      Pranav Sharma       SPR 21107    CSR 44034
*    1May13     Pranav            SPR 21127      CSR 40731-GCC 4.7 warning removal
*     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/
#include <sig_tmr.h>
#include <sig_thr.h>
#include <sig_dbg.h>
#include <sig_cspl_wrap.h>
#include <sig_util.h>

#ifdef SIGTRAN_SINGLE_THREAD
#include <sig_ipc_intr.h>
#endif

#ifndef SIG_COMMON_WITH_CSPL
#include "sig_os.h"
#include "gw_types.h"
#include "gl_mm_interfaces.h"
#include "gl_tm_interfaces.h"
#define SIGTRAN_MAX_TIMER_LISTS    5
sig_timer_exp_cbk_ptr_t gbl_timer_handler_fn = SIGTRAN_NULL;
#endif

#ifdef SIGTRAN_SINGLE_THREAD
extern sig_ipc_mgr_glb_data_st  g_ipc_mgr_data[SIG_MAX_NUM_STACK];
#endif

#ifdef SIG_COMMON_WITH_CSPL

sig_cspl_timer_info_t g_stack_type_to_qmod_map[SIG_MAX_NUM_STACK];

/******************************************************************************
**  FUNCTION : sigtran_check_timer
**
**  DESCRIPTION: This function will check timer expiry
**
**  RETURN VALUE:sigtran_pvoid_t
******************************************************************************/
sigtran_pvoid_t sigtran_check_timer( sigtran_pvoid_t p_param )
{
    sig_q_driver_t q_driver;
    sigtran_error_t e_code;

    q_driver = ( sig_q_driver_t )p_param;
    sig_cspl_driver_loop( q_driver, &e_code );
    return SIGTRAN_NULL;

}

/******************************************************************************
**  FUNCTION : sigtran_timer_init
**
**  DESCRIPTION: This function will initialize timer manager
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_timer_init(
    sig_stack_type_et        stack_type,
    sig_timer_exp_cbk_ptr_t timer_handler_fn,
    /* following parameters are not being used
     * in cspl*/
    sigtran_U32bit               task_id,
    sigtran_U32bit               max_timers,
    sigtran_U32bit               time_sec,
    sigtran_U32bit               time_usec,
    sigtran_timer_list_t         *p_timer_list_id,
    sigtran_boolean_t            b_start_timer_thread,
    sigtran_error_t              *p_ecode )
{
    sig_q_driver_t   timer_driver;
    sig_q_module_t   module_id;
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872*/
    sigtran_thread_t thread_id;
#endif

    /* for warning removal use unused variables */
    task_id = time_sec = time_usec = 0;
    p_timer_list_id = 0;

    /* SPR 21107 changes start */
    COMMON_UNUSED_VARIABLE( p_timer_list_id );
    COMMON_UNUSED_VARIABLE( time_usec );
    COMMON_UNUSED_VARIABLE( time_sec );
    COMMON_UNUSED_VARIABLE( task_id );
    /* SPR 21107 changes end */

    if( SIGTRAN_TRUE == b_start_timer_thread )
    {
        /* fill the call back in the mainfest structure*/
        sigtran_fill_qmanifest( stack_type,
                                &( g_stack_type_to_qmod_map[stack_type].q_manifest ),
                                timer_handler_fn );

        if( SIGTRAN_SUCCESS != sig_new_cspl_driver( &timer_driver, p_ecode ) )
        {
            return SIGTRAN_FAILURE;
        }

        /* set driver id in data base */
        g_stack_type_to_qmod_map[stack_type].q_driver = timer_driver;

        /* register with the cspl this shall be thread id */
        if( SIGTRAN_SUCCESS != sig_reg_with_cspl(
                    ( stack_type + TIMER_SERVICE_ID_SEED ),
                    timer_driver,
                    0,
                    ( stack_type + TIMER_SERVICE_ID_SEED ),
                    &( g_stack_type_to_qmod_map[stack_type].q_manifest ),
                    0,
                    ( sigtran_S32bit )max_timers,
                    &module_id,
                    p_ecode ) )
        {
            return SIGTRAN_FAILURE;
        }

        /* set Q module in data base */
        g_stack_type_to_qmod_map[stack_type].q_module = module_id;
#ifndef SIGTRAN_SINGLE_THREAD /* SPR 20872 */

        if( SIGTRAN_FAILURE == sigtran_create_thread( &thread_id,
                                                      sigtran_check_timer,
                                                      ( void * )timer_driver,
                                                      p_ecode ) )
        {
            return SIGTRAN_FAILURE;
        }

        else
        {
            return SIGTRAN_SUCCESS;
        }

#endif
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sigtran_timer_set_stack_qmodule
**
**    Description       : This function is used to set the stack qmodule
**
**  RETURN VALUE:         SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sigtran_timer_set_stack_qmodule(
    sig_stack_type_et       stack_type,
    sig_q_module_t          q_module )
{
    g_stack_type_to_qmod_map[stack_type].q_module = q_module;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION : sigtran_start_timer
**
**  DESCRIPTION: This function will start a timer, i.e. store
**               information in timer array
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_start_timer(
    sig_stack_type_et        stack_type,
    sigtran_timer_list_t     timer_list_id,
    sigtran_buffer_t         p_buf,
    sigtran_interval_t       duration,
    sigtran_timer_id_t       *p_timer_id,
    sigtran_callback_ptr_t   callback_fn,
    sigtran_error_t          *p_ecode )
{
    sig_q_module_t    module_id;
    sig_q_time_st     q_timer_duration;

    /*changes for CSPL 1_4_7*/
    sig_q_time_st     exp_time;

    /*sig_q_wait_st     *p_q_wait;*/

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );

    timer_list_id = timer_list_id;
    callback_fn =  callback_fn;
    *p_ecode = 0;

    /*  SPR 21127 changes start */
    COMMON_UNUSED_VARIABLE( timer_list_id );
    /*  SPR 21127 changes end */

    /* get the Qmoudle strored for this stack*/
#ifdef SIGTRAN_SINGLE_THREAD
    module_id = g_ipc_mgr_data[stack_type].g_map_for_internal_modules[SIG_DEST_TYPE_INVALID];
#else
    module_id = g_stack_type_to_qmod_map[stack_type].q_module;
#endif

    /* convert the given duration in QTIME sturcture format */
    sigtran_conv_time_to_qtime( &q_timer_duration, duration );


    /*changes for CSPL 1_4_7*/
    sig_shell_walltime( &exp_time );

    exp_time.s += q_timer_duration.s;
    exp_time.us += q_timer_duration.us;

    exp_time.s += exp_time.us / 1000000UL;
    exp_time.us %= 1000000UL;

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    *p_timer_id = qvAbsoluteTimerStart( module_id, &exp_time,
                                        ( void * )p_buf, 0, p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    *p_timer_id = qvAbsoluteTimerStart( module_id, &exp_time,
                                        ( void * )p_buf, 0, ( int * )p_ecode );
#endif


    /*End changes for CSPL 1_4_7*/

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    SIG_DEBUG_PRINT( ( "\nTimer Started for stack = %d ", stack_type ) );
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION : sigtran_stop_timer
**
**  DESCRIPTION: This function will stop a timer, i.e.  remove information
**               from timer array
**
**  RETURN VALUE:SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_stop_timer(
    sigtran_timer_list_t         timer_list_id,
    sigtran_timer_id_t           timer_id,
    sigtran_buffer_t             *pp_buf,
    sigtran_interval_t           *p_rem_duration,
    sigtran_error_t              *p_ecode )
{
    sig_q_time_st rem_time;
    sigtran_boolean_t b_running;
    sigtran_error_t   error_code;

    timer_list_id = 0;

    /* SPR 21107 changes start */
    COMMON_UNUSED_VARIABLE( timer_list_id );
    /* SPR 21107 changes end */

    if( sigtran_is_timer_running( timer_id, &b_running ) &&
            ( SIGTRAN_TRUE == b_running ) )
    {
        SIG_DEBUG_PRINT( ( "\nTimer Stoped" ) );

        /* get the timer buffers */
        *pp_buf = qvTimerData( timer_id );
        /* stop timer  */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        qvModuleTimerStop( timer_id,
                           &rem_time,
                           &error_code );
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        qvModuleTimerStop( timer_id,
                           &rem_time,
                           ( int * )&error_code );
#endif

        if( QVERROR_NONE != error_code )
        {
            SIGTRAN_SET_ECODE( p_ecode, error_code );
            return SIGTRAN_FAILURE;
        }

        if( p_rem_duration != 0 )
        {
            sigtran_conv_qtime_to_time( rem_time, p_rem_duration );
        }

        return SIGTRAN_SUCCESS;
    }

    SIGTRAN_SET_ECODE( p_ecode, ESIG_TMR_NOT_RUNNING );
    return SIGTRAN_FAILURE;
}

/******************************************************************************
**  FUNCTION : sigtran_get_timer_buf
**
**  DESCRIPTION: This function will get the timer buffer from GL
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_get_timer_buf(
    sigtran_timer_list_t         timer_list_id,
    sigtran_timer_id_t           timer_id,
    sigtran_buffer_t             *pp_buf,
    sigtran_error_t              *p_ecode )

{
    sigtran_boolean_t b_running;

    timer_list_id = timer_list_id;

    if( sigtran_is_timer_running( timer_id, &b_running ) &&
            ( SIGTRAN_TRUE == b_running ) )
    {
        *pp_buf = qvTimerData( timer_id );
        return SIGTRAN_SUCCESS;
    }

    else
    {
        SIGTRAN_SET_ECODE( p_ecode, ESIG_TMR_NOT_RUNNING );
        return SIGTRAN_FAILURE;
    }

}
/******************************************************************************
**  Function Name       : sigtran_get_rem_time
**
**    Description       :Fetches remaining time to expire a timer
**    Functions called  :
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_get_rem_time(
    sigtran_timer_id_t           timer_id,
    sigtran_interval_t           *p_duration,
    sigtran_error_t              *p_ecode )
{
    sig_q_time_st q_time;
    sigtran_boolean_t b_running;

    if( sigtran_is_timer_running( timer_id, &b_running ) &&
            ( SIGTRAN_TRUE == b_running ) )
    {
        qvTimerRemaining( timer_id, &q_time );
        sigtran_conv_qtime_to_time( q_time, p_duration );
        return SIGTRAN_SUCCESS;
    }

    else
    {
        SIGTRAN_SET_ECODE( p_ecode, ESIG_TMR_NOT_RUNNING );
        return SIGTRAN_FAILURE;
    }
}
/******************************************************************************
**  FUNCTION : sigtran_timer_deinit
**
**  DESCRIPTION: All Timer should be stopped before calling this function.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_timer_deinit(
    sig_stack_type_et   stack_type,
    sigtran_timer_list_t timer_list_id,
    sigtran_error_t      *p_ecode )
{
    stack_type = stack_type;
    timer_list_id = timer_list_id;
    p_ecode = p_ecode;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
 **  Function Name       : sigtran_conv_time_to_qtime
 **
 **    Description       : Converts time to qtime
 **
 **  RETURN VALUE        : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
 **
 *****************************************************************************/
sigtran_void_t sigtran_conv_time_to_qtime( sig_q_time_st *p_q_time,
                                           sigtran_interval_t duration )
{
    p_q_time->s = duration / 1000;
    p_q_time->us = ( duration % 1000 ) * 1000;
}

/******************************************************************************
**  Function Name       : sigtran_conv_qtime_to_time
**
**    Description       : Converts qtime to time.
**
**    Returns           : None
**
******************************************************************************/
sigtran_void_t sigtran_conv_qtime_to_time( sig_q_time_st q_time,
                                           sigtran_interval_t *p_duration )
{
    *p_duration = ( q_time.s * 1000 ) + ( q_time.us / 1000 );
}
/*****************************************************************************
 **  Function Name       : sigtran_is_timer_running
 **
 **    Description       : Checks if timer is running
 **
 **    Returns           : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
 **
 *****************************************************************************/
sigtran_return_t sigtran_is_timer_running( sigtran_timer_id_t timer_id,
                                           sigtran_boolean_t  *p_b_running )
{
    *p_b_running = SIGTRAN_FALSE;

    if( SIGTRAN_NULL != timer_id )
    {
        if( qvTimerRunning( timer_id ) )
        {
            *p_b_running = SIGTRAN_TRUE;
        }
    }

    return SIGTRAN_SUCCESS;

}
/******************************************************************************
 **  Function Name       : sigtran_fill_qmanifest
 **
 **    Description       : Fills qmanifest structure
 **
 **    Returns           : None
 **
 **
 *****************************************************************************/
sigtran_void_t sigtran_fill_qmanifest(
    sig_stack_type_et    stack_type,
    sig_q_manifest_st    *p_mainfest,
    sig_timer_exp_cbk_ptr_t timer_handler_fn )
{
    p_mainfest->name = 0;
    p_mainfest->service = stack_type + TIMER_SERVICE_ID_SEED;
    p_mainfest->init.early = 0;
    p_mainfest->init.late = 0;
    p_mainfest->handler.message = 0;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_mainfest->handler.timer = ( void * )timer_handler_fn;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_mainfest->handler.timer = ( sig_timer_fp_t ) timer_handler_fn;
#endif
    p_mainfest->method.pack = 0;
    p_mainfest->method.unpack = 0;
    p_mainfest->method.alloc = 0;
    p_mainfest->destinations = 0;
}

#else
sigtran_boolean_t       sig_tmr_state = SIGTRAN_FALSE;
sigtran_boolean_t       sig_tmr_initialized = SIGTRAN_FALSE;
sigtran_time_t          sig_gbl_time_val;
sigtran_U32bit          sig_num_tmr_lst = 0;
sigtran_timer_list_t    sig_tmr_lst[SIGTRAN_MAX_TIMER_LISTS];
#ifdef UA_TEST_WO_CSPL
static  sigtran_mutex_t sig_tmr_mutex;
#endif

sigtran_pvoid_t sig_timer_callback_fn( sigtran_pvoid_t p_buff )
{
#ifdef UA_TEST_WO_CSPL
    return gbl_timer_handler_fn( SIGTRAN_NULL, p_buff, SIGTRAN_NULL );
#else
    p_buff = p_buff;
    return NULL;
#endif /* UA_TEST_WO_CSPL */

}

/******************************************************************************
**  FUNCTION : sigtran_check_timer
**
**  DESCRIPTION: This function will check timer expiry
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_pvoid_t sigtran_check_timer( sigtran_pvoid_t p_param )
{
#ifdef UA_TEST_WO_CSPL
    sigtran_U32bit   count;
    sigtran_time_t   timer_tick;

    while( 1 )
    {
        /*In linux time val is modified*/
        timer_tick = sig_gbl_time_val;
        select( 0, NULL, NULL, NULL, &timer_tick );

        sigtran_mutex_lock( &sig_tmr_mutex );

        if( sig_tmr_state == SIGTRAN_FALSE )
        {
            sigtran_mutex_unlock( &sig_tmr_mutex );
            continue;
        }

        for( count = 0; count < sig_num_tmr_lst; count++ )
        {
            gl_tm_check_timer_expiry( sig_tmr_lst[count] );
        }

        sigtran_mutex_unlock( &sig_tmr_mutex );
    }

#endif /* UA_TEST_WO_CSPL */

    p_param = p_param;
    return NULL;

}

/******************************************************************************
**  FUNCTION : sigtran_timer_init
**
**  DESCRIPTION: This function will initialize timer manager
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_timer_init(
    sig_stack_type_et       stack_type,
    sig_timer_exp_cbk_ptr_t timer_handler_fn,
    sigtran_U32bit          task_id,
    sigtran_U32bit          max_timers,
    sigtran_U32bit          time_sec,
    sigtran_U32bit          time_usec,
    sigtran_timer_list_t    *p_timer_list_id,
    sigtran_boolean_t       b_start_timer_thread,
    sigtran_error_t         *p_ecode )
{
#ifdef UA_TEST_WO_CSPL
    gw_error_t           ecode;
    gl_timer_list_t      timer_list_id;
    sigtran_thread_t     tmp_th_id;

    /* Initialize the timer manager, if not already initialized */
    if( sig_tmr_state == SIGTRAN_FALSE )
    {

        sig_tmr_state = SIGTRAN_TRUE;

        sig_gbl_time_val.tv_sec  = time_sec;
        sig_gbl_time_val.tv_usec = time_usec;
        sig_num_tmr_lst          = 0;

        if( sig_tmr_initialized == SIGTRAN_FALSE )
        {
            gl_tm_init();

            /* Initialize the Lock Mutex */
            sigtran_mutex_init( &sig_tmr_mutex );

            /* Create Timer Thread */
            sigtran_create_thread( &tmp_th_id, sigtran_check_timer,
                                   ( void * )NULL, p_ecode );

            sig_tmr_initialized = SIGTRAN_TRUE;
        }
    }

    else if( ( time_sec < ( sigtran_U32bit )sig_gbl_time_val.tv_sec ) ||
             ( ( time_sec == ( sigtran_U32bit )sig_gbl_time_val.tv_sec ) &&
               ( time_usec < ( sigtran_U32bit )sig_gbl_time_val.tv_usec ) ) )
    {
        if( sig_num_tmr_lst >= SIGTRAN_MAX_TIMER_LISTS )
        {
            *p_ecode = ESIG_TMR_LIST_CREATE_FAILED;
            return SIGTRAN_FAILURE;
        }

        sig_gbl_time_val.tv_sec = time_sec;
        sig_gbl_time_val.tv_usec = time_usec;
    }

    /* Create a timer list for storing the active timers started by the task
        maximum number of active timers to be limited to 70*/
    if( SIGTRAN_SUCCESS != gl_tm_timer_list_create( task_id, max_timers,
                                                    &timer_list_id, &ecode ) )
    {
        /* Report error alongwith error code handling of error case */
        *p_ecode = ESIG_TMR_LIST_CREATE_FAILED;
        return SIGTRAN_FAILURE;
    }

    sig_tmr_lst[sig_num_tmr_lst] = timer_list_id;
    sig_num_tmr_lst++;
    *p_timer_list_id = timer_list_id;
    gbl_timer_handler_fn = timer_handler_fn;
#else
    /* To be ported */
    timer_handler_fn    = timer_handler_fn;
    task_id             = task_id;
    max_timers          = max_timers;
    time_sec            = time_sec;
    time_usec           = time_usec;
    p_timer_list_id     = p_timer_list_id;
    p_ecode             = p_ecode;

#endif /* UA_TEST_WO_CSPL */
    stack_type = stack_type;
    b_start_timer_thread = b_start_timer_thread;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION : sigtran_timer_deinit
**
**  DESCRIPTION: All Timer should be stopped before calling this function.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_timer_deinit(
    sig_stack_type_et           stack_type,
    sigtran_timer_list_t        timer_list_id,
    sigtran_error_t             *p_ecode )
{
#ifdef UA_TEST_WO_CSPL

    sigtran_U32bit   count;
    gw_error_t       ecode;

    sigtran_mutex_lock( &sig_tmr_mutex );

    /* Delete Timer List */
    gl_tm_delete_timer_list( timer_list_id, &ecode );

    for( count = 0; count < sig_num_tmr_lst; count++ )
    {
        if( sig_tmr_lst[count] != timer_list_id )
        {
            continue;
        }

        for( ; count < sig_num_tmr_lst - 1; count++ )
        {
            sig_tmr_lst[count] = sig_tmr_lst[count + 1];
        }

        sig_num_tmr_lst--;
    }

    if( sig_num_tmr_lst == 0 )
    {
        sig_tmr_state = SIGTRAN_FALSE;
    }

    sigtran_mutex_unlock( &sig_tmr_mutex );
#else
    /* To be ported */
    timer_list_id = timer_list_id;
#endif /* UA_TEST_WO_CSPL */
    stack_type = stack_type;
    p_ecode    = p_ecode;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION : sigtran_start_timer
**
**  DESCRIPTION: This function will start a timer, i.e. store
**               information in timer array
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_start_timer(
    sig_stack_type_et            stack_type,
    sigtran_timer_list_t         timer_list_id,
    sigtran_buffer_t             p_buf,
    sigtran_interval_t           duration,
    sigtran_timer_id_t           *p_timer_id,
    sigtran_callback_ptr_t       callback_fn,
    sigtran_error_t              *p_ecode )
{
#ifdef UA_TEST_WO_CSPL

    gw_error_t         error_code;
    sigtran_return_t   ret_val;
    gl_timer_id_t      gl_timer_id;

    ret_val = gl_tm_start_timer( timer_list_id, duration,
                                 ( gl_timer_call_back_func_t )sig_timer_callback_fn,
                                 p_buf, GW_FALSE, &gl_timer_id, &error_code );

    if( ret_val == SIGTRAN_FAILURE )
    {
        *p_ecode = error_code;
        return SIGTRAN_FAILURE;
    }

    else
    {
        *p_timer_id = ( sigtran_timer_id_t ) gl_timer_id;
    }

#else
    timer_list_id   = timer_list_id;
    p_buf           = p_buf;
    duration        = duration;
    p_timer_id      = p_timer_id;
    p_ecode         = p_ecode;
#endif /* UA_TEST_WO_CSPL */
    stack_type = stack_type;
    callback_fn     = callback_fn;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION : sigtran_stop_timer
**
**  DESCRIPTION: This function will stop a timer, i.e.  remove information
**               from timer array
**
**  RETURN VALUE:SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_stop_timer(
    sigtran_timer_list_t         timer_list_id,
    sigtran_timer_id_t           timer_id,
    sigtran_buffer_t             *pp_buf,
    sigtran_interval_t           *p_rem_duration,
    sigtran_error_t              *p_ecode )
{
#ifdef UA_TEST_WO_CSPL
    gw_error_t       error_code;
    sigtran_return_t ret_val;

    ret_val = gl_tm_stop_timer( timer_list_id, ( gl_timer_id_t )timer_id, ( void * )pp_buf,
                                &error_code );

    if( ret_val == SIGTRAN_FAILURE )
    {
        *p_ecode = error_code;
        return SIGTRAN_FAILURE;
    }

#else
    timer_list_id   = timer_list_id;
    timer_id        = timer_id;
    pp_buf          = pp_buf;
    p_ecode         = p_ecode;
#endif /* UA_TEST_WO_CSPL*/
    p_rem_duration  = p_rem_duration;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION : sigtran_get_timer_buf
**
**  DESCRIPTION: This function will get the timer buffer from GL
**
**  RETURN VALUE:SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_get_timer_buf(
    sigtran_timer_list_t         timer_list_id,
    sigtran_timer_id_t           timer_id,
    sigtran_buffer_t             *pp_buf,
    sigtran_error_t              *p_ecode )

{
#ifdef UA_TEST_WO_CSPL

    sigtran_return_t  ret_val;
    gw_error_t        error_code;

    /*locking would be deadly (cause a deadlock)*/
    ret_val = gl_tm_get_timer_buff( timer_list_id, ( gl_timer_id_t )timer_id,
                                    ( sigtran_pvoid_t )pp_buf, &error_code );

    if( ret_val == SIGTRAN_FAILURE )
    {
        *p_ecode = ( sigtran_error_t )error_code;
        return SIGTRAN_FAILURE;
    }

#else
    timer_list_id   = timer_list_id;
    timer_id        = timer_id;
    pp_buf          = pp_buf;
    p_ecode         = p_ecode;
#endif /* UA_TEST_WO_CSPL */
    return SIGTRAN_SUCCESS;
}

#endif /*SIG_COMMON_WITH_CSPL */

