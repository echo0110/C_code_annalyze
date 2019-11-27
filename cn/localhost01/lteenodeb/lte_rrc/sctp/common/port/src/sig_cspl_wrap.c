
/*****************************************************************************
 *    FILE NAME:
 *        sig_cspl_wrap.c
 *
 *    DESCRIPTION:
 *       This file has wrapping functions over cspl functions
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    14/11/2003  Vishavdeep Sharma   -            Original Creation
 *    05/04/2011  Ravi J Sharma    SPR 20872       Single Thread Change
 *    13/09/2011  Sandeep Singh    SPR-21022       Vxworks-6.8 Porting Changes
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include <sig_cspl_wrap.h>
#include <sig_util.h>
#include <sig_dbg.h>
#include <sig_ipc.h>
#include <sig_tmr.h>
#include <sig_mem.h>
#include <sig_mutx_n_sem.h>

#ifdef SIG_COMMON_WITH_CSPL


sig_q_driver_t  sctp_driver;
static const sig_q_wait_st sig_g_waitstruct =
{
    sig_new_channel,
    sig_channel_sleep,
    sig_channel_wakeup,
    sig_shell_walltime,
};

static sig_q_shell_st sig_g_shell_struct =
{
    sig_timed_wait,
    sig_send_to_ext_module,
    sig_recv_from_ext_module,
    sig_shell_open,
    sig_shell_close,
    0
};

static sig_q_sysop_st   sig_g_sysop_struct =
{
    sigtran_os_malloc,
    sigtran_os_free,
    sg_newkey,
    sg_getkey,
    sg_setkey,
    sg_mutex_create,
    sg_mutex_destroy,
    sg_mutex_lock,
    sg_mutex_unlock,
    0,
    0,
    0
};



sig_q_wait_st  *p_g_wait_struct = ( sig_q_wait_st * ) &sig_g_waitstruct;
sig_q_shell_st *p_g_shell_struct = &sig_g_shell_struct;
sig_q_sysop_st *p_g_shell_sysop_struct = &sig_g_sysop_struct;

/******************************************************************************
**  FUNCTION :    sig_qv_open
**
**  DESCRIPTION:  This function is a wrapper function for CSPL qvOpen.
**
**  RETURN VALUE: SIGTRAN_NULL
******************************************************************************/
sigtran_pvoid_t sig_qv_open( sigtran_pvoid_t *p_s )
{
    sig_q_shell_st *p_shell;
    sig_get_qshell( &p_shell );
    qvOpen( p_shell, p_s );
    return SIGTRAN_NULL;

}
/******************************************************************************
**  FUNCTION :    sig_qv_close
**
**  DESCRIPTION:  This function is a wrapper function for CSPL qvClose.
**
**  RETURN VALUE: SIGTRAN_NULL
******************************************************************************/
sigtran_pvoid_t sig_qv_close( sigtran_pvoid_t *p_s )
{
    sig_q_shell_st *p_shell;
    sig_get_qshell( &p_shell );
    qvClose( p_shell, p_s );
    return SIGTRAN_NULL;

}


/******************************************************************************
**  FUNCTION :    sig_qv_deliver
**
**  DESCRIPTION:  This function is a wrapper function for CSPL qvDeliverEx
*
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_qv_deliver( sig_q_module_t from_q_module,
                                 sig_q_module_t to_q_module,
                                 sigtran_U32bit prio,
                                 sigtran_pvoid_t message,
                                 sigtran_error_t   *p_ecode )

{
    sigtran_error_t    ecode;

    if( p_ecode == SIGTRAN_NULL )
    {
        p_ecode = &ecode;
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    qvDeliverEx( from_q_module, to_q_module, prio, message, p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    qvDeliverEx( from_q_module, to_q_module, prio, message, ( sigtran_S32bit * )p_ecode );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_qv_incoming
**
**  DESCRIPTION: This function is wrapper over QvIncoming
**
**  RETURN VALUE: SIGTRAN_NULL
******************************************************************************/

sigtran_void_t sig_qv_incoming( sig_q_module_t fromq_module,
                                sig_q_module_t to_q_module,
                                sigtran_U32bit prio,
                                sigtran_pvoid_t message )
{
    qvIncoming( fromq_module, to_q_module, prio, message );
}
/******************************************************************************
**  FUNCTION :    sig_set_destination
**
**  DESCRIPTION:  This function is wrapper over Qvsetdestination
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_set_destination(
    sig_stack_type_et stack_type,
    sig_dest_type_et  dest_type,
    sigtran_U16bit    handle,
    sigtran_pvoid_t   listner,
    sigtran_pvoid_t   data,
    sig_q_module_t    *p_q_module,
    sigtran_error_t   *p_ecode )

{
    sigtran_U32bit   ext_mod_id;
    sig_q_shell_st   *p_q_shell;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );

    listner = listner;
    *p_ecode = 0;

    /* get a new Q moudle from CSPL */

    sig_genrate_public_name( stack_type, dest_type, handle, &ext_mod_id );

    sig_get_qshell( &p_q_shell );

    /* do set destination */
    *p_q_module = qvSetDestination( ext_mod_id,
                                    p_q_shell,
                                    0,
                                    data );

    return SIGTRAN_SUCCESS;

}
/******************************************************************************
**  FUNCTION :    sig_reg_with_cspl
**
**  DESCRIPTION:  This function is wrapper over QvRegisterEx
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_reg_with_cspl(
    sig_stack_type_et       stack_type,
    sig_q_driver_t          driver_id,
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    const sigtran_S8bit     *p_name,
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    const sigtran_8bit     *p_name,
#endif
    sigtran_U32bit          service,
    sig_q_manifest_st       *p_methods,
    sigtran_pvoid_t         data,
    sigtran_S32bit          max_num_timers,
    sig_q_module_t          *p_q_module,
    sigtran_error_t         *p_ecode )
{
    sigtran_U32bit   service_name;
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;

    sig_genrate_public_name( stack_type,
                             SIG_DEST_TYPE_INVALID,
                             SOC_MGR_INVALID_HANDLE,
                             &service_name );

    service = service_name;

#ifdef SIGTRAN_SINGLE_THREAD
    driver_id = 0;
#endif

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    *p_q_module = qvRegisterEx( driver_id, p_name, service,
                                p_methods, data, max_num_timers, p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    *p_q_module = qvRegisterEx( driver_id, p_name, service,
                                p_methods, data, max_num_timers, ( sigtran_S32bit * )p_ecode );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;

}
/******************************************************************************
**  FUNCTION :    sig_new_cspl_driver
**
**  DESCRIPTION:  This function is wrapper over QvDriver
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_new_cspl_driver(
    sig_q_driver_t          *p_new_driver_id,
    sigtran_error_t         *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    *p_new_driver_id = qvNewDriverEx( p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    *p_new_driver_id = qvNewDriverEx( ( sigtran_S32bit * )p_ecode );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;

}
/******************************************************************************
**  FUNCTION :    sig_cspl_driver_loop
**
**  DESCRIPTION:  This function is wrapper over QvDriver
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_cspl_driver_loop(
    sig_q_driver_t          driver_id,
    sigtran_error_t         *p_ecode )
{
    sig_q_wait_st *p_qwait;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;

    sig_get_qwait( &p_qwait );
    qvDriverLoop( driver_id, p_qwait );
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_cspl_input_loop
**
**  DESCRIPTION:  This function is wrapper over QvInputloop
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_cspl_input_loop(
    sigtran_pvoid_t         data,
    sigtran_error_t         *p_ecode )
{
    sig_q_shell_st *p_shell;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;

    sig_get_qshell( &p_shell );
    qvInputLoop( p_shell, data );
    return SIGTRAN_SUCCESS;
}


/******************************************************************************
**  FUNCTION :    sig_qvRun
**
**  DESCRIPTION:  This function is wrapper over QvRun
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_qvRun(
    sigtran_pvoid_t         data,
    sigtran_error_t         *p_ecode )
{
    sig_q_shell_st *p_shell;
    sig_q_wait_st *p_qwait;

    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;

    sig_get_qshell( &p_shell );
    sig_get_qwait( &p_qwait );

    qvRun( p_shell, data, p_qwait, 0 );

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_get_service_name
**
**  DESCRIPTION:  This function is wrapper over qvGetServiceName
**
**  RETURN VALUE: sigtran_UL32bit
******************************************************************************/
sigtran_UL32bit sig_get_service_name( sig_q_module_t service )
{
    return qvGetServiceName( service );
}
/******************************************************************************
**  FUNCTION :    sig_get_from_q_module
**
**  DESCRIPTION:  This function is wrapper over qvFrom
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t    sig_get_from_q_module(
    sigtran_pvoid_t p_msg_buffer,
    sig_q_module_t *p_q_module,
    sigtran_error_t *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;
    *p_q_module = qvFrom( p_msg_buffer );
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_get_to_q_module
**
**  DESCRIPTION:  This function is wrapper over qvTo
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t    sig_get_to_q_module(
    sigtran_pvoid_t p_msg_buffer,
    sig_q_module_t *p_q_module,
    sigtran_error_t *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;
    *p_q_module = qvTo( p_msg_buffer );
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_get_msg_size
**
**  DESCRIPTION:  This function is wrapper over qvMsgSize
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t    sig_get_msg_size(
    sigtran_pvoid_t p_msg_buffer,
    sigtran_U32bit  *p_size,
    sigtran_U32bit  *p_attr_size,
    sigtran_error_t *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    *p_ecode = 0;
    p_attr_size = p_attr_size;
    qvMsgSize( p_msg_buffer, p_size );
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_qvInit
**
**  DESCRIPTION:  This function is wrapper over rvInit
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t    sig_qvInit( const sig_q_rtuing_t *p_t,
                                const sig_q_sysop_st *p_op,
                                sigtran_error_t  *p_ecode )
{
    sig_q_sysop_st *p_q_sysop;
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
    p_t = p_t;
    p_op = p_op;

    sig_get_qsysop( &p_q_sysop );
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
#ifndef SIGTRAN_SINGLE_THREAD
    /* Changes for SPR-21022 Starts here */
#ifdef VXWORKS_6_9_SUPPORT
    qvInitEx( 0, p_q_sysop, ( sigtran_S32bit * )p_ecode );
    /* Changes for SPR-21022 Ends here */
#else
    qvInitEx( 0, p_q_sysop, p_ecode );
#endif
#else
    qvSimpleInit( p_q_sysop );
#endif
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
#ifndef SIGTRAN_SINGLE_THREAD
    qvInitEx( 0, p_q_sysop, ( sigtran_S32bit * )p_ecode );
#else
    *p_ecode = QVERROR_NONE;  /* SPR 20872*/
    qvSimpleInit( p_q_sysop );
#endif
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_qvSimpleInit
**
**  DESCRIPTION:  This function is wrapper over qvSimpleInit
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t    sig_qvSimpleInit( const sig_q_rtuing_t *p_t,
                                      const sig_q_sysop_st *p_op )
{
    sig_q_sysop_st *p_q_sysop;

    p_t = p_t;
    p_op = p_op;

    sig_get_qsysop( &p_q_sysop );
    sctp_driver = qvSimpleInit( p_q_sysop );

    return SIGTRAN_SUCCESS;
}







/******************************************************************************
**  FUNCTION :    sig_rvMsgPoolsExtend
**
**  DESCRIPTION:  This function is wrapper over rvInit
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_rvMsgPoolsExtend( sigtran_pool_id_t *p_pool_set ,
                                       sig_q_rpooltuning_st *p_q_rtuning,
                                       sigtran_error_t  *p_ecode )
{
    SIG_ASSERT( ( SIGTRAN_NULL != p_ecode ) );
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    qvMsgPoolSetExtend( p_pool_set, p_q_rtuning, p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    qvMsgPoolSetExtend( p_pool_set, p_q_rtuning, ( sigtran_S32bit * )p_ecode );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_get_qwait
**
**  DESCRIPTION:  This function is returns global pointer for wait structure
**
**    RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sig_get_qwait( sig_q_wait_st **p_pq_wait )
{
    *p_pq_wait = ( sig_q_wait_st * )p_g_wait_struct;
    return SIGTRAN_SUCCESS;

}
/******************************************************************************
**  FUNCTION :    sig_set_qwait
**
**  DESCRIPTION:  This function is sets global pointer for wait structure
**
**    RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_set_qwait( sig_q_wait_st *p_wait )
{
    p_g_wait_struct = p_wait;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_set_qshell
**
**  DESCRIPTION:  This function sets global pointer for shell structure
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_set_qshell( sig_q_shell_st *p_shell )
{
    p_g_shell_struct = p_shell;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_get_qshell
**
**  DESCRIPTION:  This function returns global pointer for shell structure
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_get_qshell( sig_q_shell_st **p_p_q_shell )
{
    *p_p_q_shell = ( sig_q_shell_st * ) p_g_shell_struct;
    return SIGTRAN_SUCCESS;
}
/******************************************************************************
**  FUNCTION :    sig_get_qsysop
**
**  DESCRIPTION:  This function is returns global pointer for sysos structure
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_get_qsysop( sig_q_sysop_st **p_p_q_sysop )
{
    *p_p_q_sysop = ( sig_q_sysop_st * ) p_g_shell_sysop_struct;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sig_set_qsysop
**
**  DESCRIPTION:  This function is set the global pointer for sysop structure
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_set_qsysop( sig_q_sysop_st *p_q_sysop )
{
    p_g_shell_sysop_struct = p_q_sysop;
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  Function Name       : sig_cspl_set_imediate
**
**  Description       : This is a wrapper for   qvSetImmediate
**
**  RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
**
******************************************************************************/
sigtran_return_t sig_cspl_set_immediate
(
    sigtran_U32bit  module_name,
    sigtran_error_t  *p_ecode
)
{
    sigtran_error_t error_code;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    qvSetImmediate( module_name, &error_code );
#else
    qvSetImmediate( module_name, ( sigtran_S32bit * )&error_code );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        SIGTRAN_SET_ECODE( p_ecode, error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}


/* SPR 20872: Changes for enabling single threaded support Starts*/
extern sig_fd_set       global_mod_fd_set;
extern sigtran_S32bit       g_num_fd_mod;

/******************************************************************************
**    Function Name     : sig_pre_select
**
**    Description       : This function provides the SIGTRAN's global FD set
**            if p_sig_fds is not NULL and a suggested value of timeout.
**            If no timer is running it would return
**                SIGTRAN_DEFAULT_TO_SEC as secs
**            and SIGTRAN_DEFAULT_TO_USEC as usecs.
**
**  RETURN VALUE        : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
**
******************************************************************************/
sigtran_return_t sig_pre_select
(
    sig_fd_set *p_sig_fds,
    sigtran_time_t  *p_suggested_timeout
)
{
    sig_q_wait_st *p_qwait;
    sig_q_time_st now, diff;
    const sig_q_time_st *wakeup;

    if( p_sig_fds )
    {
        sig_get_glb_fd_set( p_sig_fds );
    }

    sig_get_qwait( &p_qwait );
    p_qwait->walltime( &now );
    wakeup = qvSchedule( &now );

    if( wakeup )
    {
        if( qvTimeDiff( wakeup, &now, &diff ) < 0 )
        {
            diff.s = diff.us = 0;
        }
    }

    else
    {
        /* default timed wait */
        diff.s = SIGTRAN_DEFAULT_TO_SEC;
        diff.us = SIGTRAN_DEFAULT_TO_USEC;
    }

    p_suggested_timeout->tv_sec = diff.s;
    p_suggested_timeout->tv_usec = diff.us;
    return SIGTRAN_SUCCESS;
}


/******************************************************************************
**    Function Name     : sig_schedule
**
**    Description       : This function checks and receives data from SIGTRAN fdsal FD set
**            and also checks for SIGTRAN timer expiries.
**
**    RETURN VALUE      : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
**
******************************************************************************/
sigtran_return_t sig_schedule
(
    sig_fd_set      *p_modified_fd_set
)
{
    int policy = 0;
    sig_q_wait_st *p_qwait;
    sig_q_time_st now;

    if( p_modified_fd_set != SIGTRAN_NULL )
    {
        global_mod_fd_set = *p_modified_fd_set;
        g_num_fd_mod = sig_get_max_fd();

        while( 1 )
        {
            QMODULE         from, to;
            void            *buffer;
            signed char     priority;

            from = to = 0;
            priority = 0;

            if( ( buffer = sig_recv_from_ext_module( NULL, &from, &to, &priority ) ) == 0 )
            {
                break;
            }

            qvIncoming( from, to, priority, buffer );

            if( !policy )
            {
                break;
            }
        }
    }

    sig_get_qwait( &p_qwait );
    p_qwait->walltime( &now );
    qvSchedule( &now );
    return SIGTRAN_SUCCESS;
}
/* SPR 20872: Changes for enabling single threaded support Ends*/

#endif /* SIG_COMMON_WITH_CSPL */








