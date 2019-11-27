#ifndef __SCTP_CSPL_H__
#define __SCTP_CSPL_H__

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_esdef.h>
#include <s_tlif.h>
#include <s_uismty.h>
#include <s_in.h>
#include <s_ostmr.h>
#include <s_osmem.h>
#include <s_err.h>
#include <s_mhproc.h>
#include <s_mmmain.h>
#include <s_db.h>
#include <s_dbutil.h>
#include <s_uico.h>
#include <s_red.h>
#include <s_es.h>
#include <s_os.h>
#include <s_dbg.h>
#include <s_os.h>
#include <cspl.h>

#ifdef __cplusplus
extern "C" {
#endif
#define SCTP_TIMER_POOL_AUTO_EXTND      QV_TPOOL_EXT
    typedef struct
    {
        sctp_Boolean_t is_blocked;
        sctp_U16bit    local_port;
    } stack_envt_st;

    sctp_return_t sctp_set_stack_envt( sctp_Boolean_t b_cspl_alrdy_initialised,
                                       void * );

    sctp_void_t cspl_init( sctp_void_t );

    void *cspl_eshell_master_thread( void *A );

    int  sctp_cspl_mesg_handler( void *p_message, void *p_module_data );

    int  sctp_cspl_tl_mesg_handler( void *p_message, void *p_module_data );

    void sctp_timer_handler( void *p_timer_id, void *p_timer_buffer,
                             void *p_module_id );

    int sctp_stack_ipc_thread_mesg_handler( void *p_message, void *p_module_data );

    void *sctp_eshell_clock_thread( void    *A );

    void *sctp_stack_ipc_master_thread( void *A );

    sctp_void_t *sctp_ipc_master_thread( sctp_void_t *p_sctp_driver );

    sigtran_return_t    sctp_dummy_recv_data(
        sigtran_U32bit      handle_id,
        sig_sock_mode_et    socket_mode,
        sig_soc_type_et     socket_type,
        sig_sock_fd_t       socket_id,
        sig_q_module_t      *p_from_q_module,
        sigtran_pvoid_t     *p_data,
        sigtran_U32bit      *p_data_len,
        sigtran_error_t     *p_ecode );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
