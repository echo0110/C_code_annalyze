
/*****************************************************************************
 *    FILE NAME:
 *        sig_typs.h
 *
 *    DESCRIPTION  :   Contains the type defs, structures and enums defined for
 *                     use of sigtran stacks.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *    11Aug11     Supreet Jain     SPR 21003       Performance Tunning
 *
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef  __SIGTRAN_TYPES_H__
#define  __SIGTRAN_TYPES_H__

#include <sig_os_port.h>
#ifdef __cplusplus
extern "C" {
#endif
    /*
     * All the os specific files are included from this
     * file .location of file is port/xyz/h
     * xyz is name of OS
     */

#ifdef SIG_COMMON_WITH_CSPL

    /* include header file for cspl */

#include <cspl.h>

    /* cspl related type defs */

    /**********List of types which are expected from CSPL ******/
    typedef void    *QSEMAPHORE;
    typedef void    *QCOND;
    typedef void    *QTHREAD;
    /***********************************************************/

    typedef QMODULE               sig_q_module_t;
    typedef QSHELL                sig_q_shell_st;
    typedef QTUNING               sig_q_tuning_st;
    typedef QSYSOP                sig_q_sysop_st;
    typedef QWAIT                 sig_q_wait_st;
    typedef QTIME                 sig_q_time_st;
    typedef QMANIFEST             sig_q_manifest_st;
    typedef QDRIVER               sig_q_driver_t;
    typedef QTUNING               sig_q_rtuing_t;
    typedef QPOOLSETTUNING        sig_q_rpooltuning_st;
    typedef QTIMER                sigtran_timer_id_t;

    /* QLOCK is pointer to void as defined by CSPL */
    typedef QLOCK                 sigtran_mutex_t;
    typedef QSEMAPHORE            sigtran_sem_t;
    typedef QCOND                 sigtran_thr_cond_t;
    typedef QTHREAD               sigtran_thread_t;
    typedef QMPOOLSET             sigtran_pool_id_t;

#ifdef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    typedef void *( *sig_early_fp_t )( void * );
    typedef void *( *sig_late_fp_t )( void * );
    typedef int ( *sig_message_fp_t )( void *, void * );
    typedef void ( *sig_timer_fp_t )( QTIMER, void *, void * );
    typedef void *( *sig_pack_fp_t )( QMODULE, void *, unsigned int * );
    typedef void *( *sig_unpack_fp_t )( QMODULE, void * );
    typedef void *( *sig_alloc_fp_t )( QMODULE, unsigned int );
#endif

#else
    typedef void                 *sig_q_module_t;
    typedef void                 *sigtran_thr_cond_t;


#ifdef SIGTRAN_PORT_SOLARIS
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
    typedef pthread_mutex_t *sigtran_mutex_t;
    typedef pthread_t       sigtran_thread_t;
    typedef sem_t          *sigtran_sem_t;

#ifdef GW_SOLARIS
#undef GW_SOLARIS
#endif

#define GW_SOLARIS                1
#ifndef _ANSI_C_
#define _ANSI_C_                  1
#endif

#elif SIGTRAN_PORT_TRU64
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
    typedef pthread_mutex_t *sigtran_mutex_t;
    typedef pthread_t       sigtran_thread_t;
    typedef sem_t          *sigtran_sem_t;

#ifdef GW_SOLARIS
#undef GW_SOLARIS
#endif

#define GW_SOLARIS                1
#ifndef _ANSI_C_
#define _ANSI_C_                  1
#endif

#elif SIGTRAN_PORT_LINUX
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
    typedef pthread_mutex_t *sigtran_mutex_t;
    typedef pthread_t       sigtran_thread_t;
    typedef sem_t          *sigtran_sem_t;

#ifdef GW_LINUX
#undef GW_LINUX
#endif

#define GW_LINUX                  1
#ifndef _ANSI_C_
#define _ANSI_C_                  1
#endif

#elif SIGTRAN_PORT_VXWORKS
#include <semLib.h>
#include <semaphore.h>
#include <taskLib.h>
#include <timers.h>
#include <time.h>
#include <sys/times.h>
    typedef SEM_ID          *sigtran_sem_t;
    typedef SEM_ID          *sigtran_mutex_t;
    typedef int             sigtran_thread_t;

#ifdef GW_VXWORKS
#undef GW_VXWORKS
#endif

#define GW_VXWORKS                1

#elif SIGTRAN_PORT_WINXP
#include    <winbase.h>
#include    <windows.h>
#include    <winsock2.h>

    typedef HANDLE *sigtran_mutex_t;
    typedef HANDLE *sigtran_sem_t;
    typedef HANDLE sigtran_thread_t;
    /* VMS */
#elif SIGTRAN_PORT_VMS
#include <starlet.h>
#include <pthread.h>
    typedef pthread_mutex_t *sigtran_mutex_t;
    typedef int          *sigtran_sem_t;
    typedef pthread_t       sigtran_thread_t;
#endif


    typedef void                 *sigtran_pool_id_t;
    typedef void                 *sigtran_timer_id_t;
    typedef struct timeval        sig_q_time_st;
#endif /* SIG_COMMON_WITH_CSPL */


    typedef unsigned char         sigtran_U8bit;
    typedef signed char           sigtran_S8bit;
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    typedef char                  sigtran_8bit;

    typedef unsigned short int    sigtran_U16bit;
    typedef short int             sigtran_S16bit;
    typedef unsigned long  int    sigtran_UL32bit;
    typedef unsigned int          sigtran_U32bit;
    typedef int                   sigtran_S32bit;
    typedef void                  sigtran_void_t;
    typedef void                 *sigtran_pvoid_t;

    typedef struct iovec          sigtran_iovec_st;
    typedef sigtran_pool_id_t     sigtran_pool_data_st;
    typedef sigtran_U16bit        sigtran_pool_type_t;
    typedef sigtran_U32bit        sigtran_error_t;
    typedef sigtran_U8bit         sigtran_boolean_t;
    typedef sigtran_U8bit        *sigtran_buffer_t;
    typedef sigtran_U8bit         sigtran_return_t;
    typedef sigtran_U32bit        sigtran_interval_t;
    typedef struct timeval        sigtran_time_t;
    typedef sigtran_thr_cond_t    sigtran_thread_cond_wait_t;

    typedef sigtran_pvoid_t ( * sigtran_callback_ptr_t )( sigtran_pvoid_t );
    typedef sigtran_pvoid_t ( * sig_timer_exp_cbk_ptr_t )( sigtran_pvoid_t,
                                                           sigtran_pvoid_t,
                                                           sigtran_pvoid_t );
    typedef sigtran_U16bit        sigtran_timer_list_t;
    typedef sigtran_U16bit        sigtran_thread_id_t;
    typedef time_t                sigtran_time_sec_t;

    typedef enum
    {
        SIG_SOC_MODE_INVALID = 0,
        SIG_SOC_MODE_CLIENT,
        SIG_SOC_MODE_SERVER
    } sig_sock_mode_et;

    typedef enum
    {
        SIG_SOC_TYPE_INVALID = 0,
        SIG_SOC_TYPE_UDP,
        SIG_SOC_TYPE_TCP,
        SIG_SOC_TYPE_UDP_NW,
        SIG_SOC_TYPE_IPV4,
        SIG_SOC_TYPE_IPV6,
        SIG_SOC_TYPE_ICMPV4,
        SIG_SOC_TYPE_ICMPV6
    } sig_soc_type_et;

    typedef enum
    {
        SIG_DEST_TYPE_INVALID = 0,
        SIG_DEST_TYPE_APP,
        SIG_DEST_TYPE_SM,
        SIG_DEST_TYPE_TL,
        SIG_MAX_DEST_TYPE
    } sig_dest_type_et;

    typedef enum
    {
        SIG_READ_FLAG_INVALID = 0,
        SIG_RECV_ALL,
        /* SPR 21003 Starts */
        SIG_MSG_PEEK,
        SIG_RECV_DONTWAIT
        /* SPR 21003 Ends */
    } sig_recv_flag_et;

    typedef enum
    {
        SIG_INVALID_STACK = 0,
        SIG_SCTP_STACK,
        SIG_SCTP_RECV_STACK,
        SIG_M3UA_STACK,
        SIG_SUA_STACK,
        SIG_M2PA_STACK,
        SIG_M2UA_STACK,
        SIG_IUA_STACK,
        SIG_SCTP_CLIENT,
        SIG_M3UA_CLIENT,
        SIG_SUA_CLIENT,
        SIG_M2UA_CLIENT,
        SIG_IUA_CLIENT,
        SIG_M2PA_CLIENT,
#ifdef SIGTRAN_SINGLE_THREAD
        SIG_M3UA_APPLICATION_ST,
#endif
        SIG_MAX_NUM_STACK
    } sig_stack_type_et;


#ifdef SIGTRAN_SINGLE_THREAD
#define  SIG_M3UA_APPLICATION 2000
#endif

    /* IP address */

    /*  The IP address type (v4/v6) */

    typedef enum
    {
        SIG_TRANS_ADDR_INVALID = 0,
        SIG_TRANS_ADDR_IPV4,
        SIG_TRANS_ADDR_IPV6
    } sig_trans_ip_add_type_et;

    /* The IPv6 address structure. */
    typedef struct
    {
        sigtran_U32bit   flow_info;
        sigtran_U32bit   scope_id;
        sigtran_U16bit   port;
        union
        {
            sigtran_U8bit    addr8[16];
            sigtran_U16bit   addr16[8];
            sigtran_U32bit   addr32[4];
        } ip_addr_ut;
    } sig_addr_in6_st;

    /* The IPV4 address structure */
    typedef struct
    {
        sigtran_U32bit   addr;
        sigtran_U16bit   port;
    } sig_addr_in4_st;

    /* Each Address element is a of type this. */
    typedef struct
    {
        sig_trans_ip_add_type_et ip_type;
        union
        {
            sig_addr_in4_st    addr_v4;
            sig_addr_in6_st    addr_v6;
        } ip_addr_ut;
    } sig_ip_addr_st;

    /* CSR: 1-6763355 starts here */
    typedef struct
    {
        sigtran_mutex_t     lock;
        sigtran_sem_t       ok_to_read;
        sigtran_sem_t       ok_to_write;
        sigtran_U32bit      active_writers;
        sigtran_U32bit      waiting_writers;
        sigtran_U32bit      active_readers;
        sigtran_U32bit      waiting_readers;
    } sigtran_rdwt_lock_t;
    /* CSR: 1-6763355 ends here */


    typedef sigtran_return_t ( * sig_read_cbk_func_ptr_t ) \
    ( sigtran_U32bit   handle_id,
      sig_sock_mode_et sock_mode,
      sig_soc_type_et  sock_type,
      sig_sock_fd_t    sock_fd,
      sig_q_module_t   *from_q_mod,
      sigtran_pvoid_t  *p_data_buf,
      sigtran_U32bit   *p_data_len,
      sigtran_error_t  *p_ecode );

    typedef sigtran_return_t ( * sig_get_sm_msg_len_cbk ) \
    ( const sigtran_void_t  *p_msg,
      sigtran_U32bit   *p_msg_len );

    typedef sigtran_return_t ( * sig_get_su_msg_len_cbk ) \
    ( const sigtran_void_t  *p_msg,
      sigtran_U32bit   *p_msg_len );

    typedef sigtran_return_t ( * sig_get_tl_msg_len_cbk ) \
    ( const sigtran_void_t  *p_msg,
      sigtran_U32bit   *p_msg_len );


    typedef sigtran_void_t ( * sig_free_func_ptr_t ) \
    ( sigtran_pvoid_t  p_data,
      sigtran_pvoid_t  p_free_args );

    typedef sigtran_pvoid_t ( * sigtran_start_func_ptr_t )\
    ( sigtran_void_t * );

#ifdef SIG_COMMON_WITH_CSPL

    /**********List of all the OS functions and types
    *  which are expected from CSPL ******/

    QSEMAPHORE qvSemInit( void );

    void qvSemDestroy( QSEMAPHORE p_sem );

    void qvSemWait( QSEMAPHORE sp );

    void qvSemPost( QSEMAPHORE sp );

    int qvTryLock( QLOCK lock );

    /********* list of thread related functions expected from cspl **********/

    QCOND qvThreadCondInit( void );

    void qvThreadCondWait( QCOND thr_cond, QLOCK mutex );

    void qvThreadCondSignal( QCOND thr_cond );

    void qvDestroyThreadCond( QCOND thr_cond );

    void qvThreadCreate( void *p_thr_id,
                         void * ( *start_func )( void * ),
                         void *arg );

    QSEMAPHORE system_sem_init( void );
#else
    void sg_sem_post( sigtran_pvoid_t p_sem );
    void sg_sem_wait( sigtran_pvoid_t p_sem );
    sigtran_sem_t sg_sem_init( void );
    void sg_sem_destroy( sigtran_pvoid_t p_sem );



#endif /* SIG_COMMON_WITH_CSPL */
    void *sg_mutex_create( void );

    void sg_mutex_lock( void *lock );

    void sg_mutex_unlock( void *lock );

    void sg_mutex_destroy( void *lock );

    int sg_mutex_try_lock( sigtran_void_t *lock );

    void sg_create_thread( void *p_thr_id,
                           void * ( *start_func )( void * ),
                           void *arg );

#ifdef __cplusplus
}
#endif

#endif /* __SIGTRAN_TYPS_H__  */
