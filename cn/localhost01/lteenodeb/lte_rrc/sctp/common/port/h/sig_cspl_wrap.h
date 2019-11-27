/*****************************************************************************
 *    FILE NAME:
 *        sig_cspl_wrap.h
 *
 *    DESCRIPTION:
 *       This file has decl of function which are wrappers over cspl functions
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    02/11/2003  Vishavdeep Sharma   -            Original Creation
 *    05/04/2011  Ravi J Sharma    SPR 20872       Single Threaded Changes
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __SIG_CSPL_WRAP_H__
#define __SIG_CSPL_WRAP_H__

#ifdef SIG_COMMON_WITH_CSPL

#include <sig_typs.h>
#include <sig_defs.h>

#ifdef __cplusplus
extern "C" {
#endif
    /* wrapper over QvSetDestination */
    sigtran_return_t sig_set_destination(
        sig_stack_type_et stack_type,
        sig_dest_type_et  dest_type,
        sigtran_U16bit    handle,
        sigtran_pvoid_t   listner,
        sigtran_pvoid_t   data,
        sig_q_module_t    *p_q_module,
        sigtran_error_t   *p_ecode );

    /* wrapper over QvRegistar */

    sigtran_return_t sig_reg_with_cspl(
        sig_stack_type_et    stack_type,
        sig_q_driver_t       driver_id,
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        const sigtran_S8bit  *p_name,
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        const sigtran_8bit  *p_name,
#endif
        sigtran_U32bit       service,
        sig_q_manifest_st    *p_methods,
        sigtran_pvoid_t      data,
        sigtran_S32bit       max_num_timers,
        sig_q_module_t       *p_q_module,
        sigtran_error_t      *p_ecode );

    sigtran_return_t sig_new_cspl_driver(
        sig_q_driver_t       *p_new_driver_id,
        sigtran_error_t      *p_ecode );

    sigtran_return_t sig_qvRun(
        sigtran_pvoid_t      data,
        sigtran_error_t      *p_ecode );


    sigtran_return_t sig_cspl_input_loop(
        sigtran_pvoid_t      data,
        sigtran_error_t      *p_ecode );


    sigtran_return_t sig_cspl_driver_loop(
        sig_q_driver_t       driver_id,
        sigtran_error_t      *p_ecode );

    sigtran_return_t sig_qv_deliver( sig_q_module_t from_q_module,
                                     sig_q_module_t to_q_module,
                                     sigtran_U32bit prio,
                                     sigtran_pvoid_t message,
                                     sigtran_error_t   *p_ecode );


    sigtran_void_t sig_qv_incoming(
        sig_q_module_t    from_q_module,
        sig_q_module_t    to_q_module,
        sigtran_U32bit    prio,
        sigtran_pvoid_t   message );

    sigtran_return_t    sig_get_from_q_module(
        sigtran_pvoid_t     p_msg_buffer,
        sig_q_module_t      *p_q_module,
        sigtran_error_t     *p_ecode );

    sigtran_return_t    sig_get_to_q_module(
        sigtran_pvoid_t p_msg_buffer,
        sig_q_module_t *p_q_module,
        sigtran_error_t *p_ecode );

    sigtran_return_t    sig_get_msg_size(
        sigtran_pvoid_t  p_msg_buffer,
        sigtran_U32bit  *p_size,
        sigtran_U32bit  *p_attr_size,
        sigtran_error_t  *p_ecode );

    sigtran_pvoid_t sig_qv_open( sigtran_pvoid_t *p_s );

    sigtran_pvoid_t sig_qv_close( sigtran_pvoid_t *p_s );

    sigtran_return_t sig_set_qshell( sig_q_shell_st *p_shell );

    sigtran_return_t sig_get_qshell( sig_q_shell_st **p_p_q_shell );

    sigtran_return_t sig_get_qwait( sig_q_wait_st **p_pq_wait );

    sigtran_return_t sig_set_qwait( sig_q_wait_st *p_wait );

    sigtran_void_t sig_shell_walltime( sig_q_time_st *now );

    sigtran_pvoid_t sig_new_channel( sigtran_void_t );

    sigtran_void_t sig_channel_sleep( sigtran_pvoid_t channel,
                                      const sig_q_time_st *timeout );

    sigtran_void_t sig_channel_wakeup( sigtran_pvoid_t channel );

    sigtran_return_t sig_get_qsysop( sig_q_sysop_st **p_p_q_sysop );

    sigtran_return_t sig_set_qsysop( sig_q_sysop_st *p_q_sysop );


    sigtran_UL32bit sig_get_service_name( sig_q_module_t service );

    sigtran_return_t    sig_qvInit( const sig_q_rtuing_t *p_t,
                                    const sig_q_sysop_st *p_op,
                                    sigtran_error_t  *p_ecode );

    sigtran_return_t    sig_qvSimpleInit( const sig_q_rtuing_t *p_t,
                                          const sig_q_sysop_st *p_op );

    sigtran_return_t sig_rvMsgPoolsExtend( sigtran_pool_id_t *p_pool_set ,
                                           sig_q_rpooltuning_st *p_q_rtuning,
                                           sigtran_error_t  *p_ecode );

    sigtran_void_t *sg_newkey( sigtran_void_t );

    sigtran_void_t *sg_getkey( sigtran_void_t *key );

    sigtran_void_t sg_setkey( sigtran_void_t *key, sigtran_void_t *value );

    sigtran_return_t sig_cspl_set_immediate
    (
        sigtran_U32bit  module_name,
        sigtran_error_t  *p_ecode
    );

    /* SPR 20872: Changes for enabling single threaded support Starts*/
    sigtran_return_t sig_pre_select
    (
        sig_fd_set *p_sig_fds,
        sigtran_time_t  *p_suggested_timeout
    );

    sigtran_return_t sig_schedule
    (
        sig_fd_set      *p_modified_fd_set
    );
    /* SPR 20872: Changes for enabling single threaded support Ends*/



#ifdef __cplusplus
}
#endif
#endif /* SIG_COMMON_WITH_CSPL */
#endif
