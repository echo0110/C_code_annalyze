/******************************************************************************
 * File Name:
 *      s_cspl_init.c
 *
 * Description:
 *      This file contains the function for registering SCTP stack with CSPL.
 *      This file also contains the various other functions where SCTP stack
 *      threads gets register to the CSPL as a different module.
 *      Additionally also contains pointer global variable for structure
 *      g_sg_sysop_fn and g_sg_qshell_oprs which contains pointer to some OS specific
 *      functions.
 *
 * DATE          NAME           REFERENCE       REASON
 * ---------------------------------------------------
 * 06 Oct 03     Vivek Bansal
 * 27 Nov 03     Vivek Bansal
 * 24 May 10     Rajnesh        SPR 20411       Warning Fixed
 * 05 Apr 11     Ravi J Sharma  SPR 20872       Single Threaded Change
 * 10 Aug 11     Supreet Jain        SPR 21004      CSR 00005758
 * 18Feb13    Rohan Pawa   SPR 21107    CSR 24680
 * 25Feb14    Rohan Pawa                CSR 69326
 * 22Jun17    Neha Gupta   Bug Id 205   CSR 00135594
 *****************************************************************************/

#include <sig_os.h>
#include <s_esdef.h>
#include <s_cspl.h>

#define SIGTRAN_SCTP_STACK_TMR      20

sctp_Boolean_t glb_sctp_stack_env_state = SCTP_FALSE;
sctp_U16bit    g_dummy_sock;

sigtran_sem_t     g_sysop_sema;

#ifdef  SCTP_CMN_RECV_POLL_THREAD
sig_q_driver_t sctp_driver;
#endif

/******************************************************************************
**  Function Name       : sctp_set_stack_envt
**
**    Description       : This is the first function which the encaps will have
**                        to invoke before using the SCTP stack. This function
**                        initialises the CSPL stack and starts the CSPL main
**                        thread if required.
**
**    Functions called  :
**
******************************************************************************/
sctp_return_t sctp_set_stack_envt( sctp_Boolean_t b_cspl_alrdy_initialised,
                                   void *p_info )
{

#ifdef SCTP_SINGLE_THREAD_SUPPORT /* SPR 20872  Starts*/
    sig_q_manifest_st  *p_sctp_manifest_info;
    sig_q_driver_t      sctp_driver;
    sig_q_module_t      sctp_module_id;
    sigtran_error_t     ecode;
    sig_q_module_t      sctp_tl_q_module;
    stack_envt_st      *p_stack_envt = ( stack_envt_st * )p_info;
    sig_ip_addr_st      local_sig_addr[SCTP_MAX_TRANSPORT_ADDR];
    sigtran_U8bit       hostname[SCTP_MAX_HOSTNAME_SIZE];
    sigtran_U32bit      num_addr = 1;

    if( SCTP_TRUE == glb_sctp_stack_env_state )
    {
        return ( SCTP_FAILURE );
    }

    if( SCTP_FALSE == b_cspl_alrdy_initialised )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nInitialising CSPL" ) );

        if( sig_qvInit( SCTP_NULL, SCTP_NULL, &ecode ) == SIGTRAN_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in initialising CSPL" ) );
            return ( SCTP_FAILURE );
        }
    }

    /*
     * Initialise the memory module. The memory required is for the messaging.
     */
    if( SCTP_FAILURE ==  sctp_init_mem_module() )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "\nFailure in initialising memory" ) );
        return ( SCTP_FAILURE );
    }

    p_sctp_manifest_info = ( sig_q_manifest_st * )
                           sigtran_malloc( sizeof( sig_q_manifest_st ) );

    if( SIGTRAN_NULL == p_sctp_manifest_info )
    {
        return ( SCTP_FAILURE );
    }

    p_sctp_manifest_info->name = "SCTP STACK";
    p_sctp_manifest_info->service = SIG_SCTP_STACK;
    p_sctp_manifest_info->init.early = SCTP_NULL;
    p_sctp_manifest_info->init.late = SCTP_NULL;

    p_sctp_manifest_info->handler.message = SCTP_NULL;

    p_sctp_manifest_info->handler.timer = sctp_timer_handler;

    /*
     * No specific handlers for other methods.
     */
    p_sctp_manifest_info->method.pack = SCTP_NULL;
    p_sctp_manifest_info->method.unpack = SCTP_NULL;
    p_sctp_manifest_info->method.alloc = SCTP_NULL;

    /*
     * Set the list of destinations.
     */
    p_sctp_manifest_info->destinations = 0;
    sig_new_cspl_driver( &sctp_driver, &ecode );

    /*
     * Register the SCTP stack.
     */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nResgistering SCTP Stack with CSPL" ) );

    if( sig_reg_with_cspl( SIG_SCTP_STACK, sctp_driver,
                           ( const sigtran_8bit * ) "SCTP MODULE", SIG_SCTP_STACK,
                           p_sctp_manifest_info, SCTP_NULL, SCTP_TIMER_POOL_AUTO_EXTND,
                           &sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in registring with CSPL" ) );
        return SCTP_FAILURE;
    }

    if( sig_set_destination( SIG_SCTP_RECV_STACK, SIG_DEST_TYPE_TL,
                             SOC_MGR_INVALID_HANDLE, SCTP_NULL, SCTP_NULL, &sctp_tl_q_module,
                             &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet destination for TL failure" ) );
        return SCTP_FAILURE;
    }

    sig_cspl_set_immediate( sig_get_service_name( sctp_tl_q_module ),
                            &ecode );

    /*
     * Set the destination in the IPC Manager. Before that first initialise the
     * IPC Manager.
     */

    if( sig_init_ipc_mgr( SIG_SCTP_STACK, sctp_get_api_buf_len,
                          sctp_get_api_buf_len, SIGTRAN_NULL ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nInit IPC Manager failed" ) );
        return SCTP_FAILURE;
    }

    if( sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                          SOC_MGR_INVALID_HANDLE, sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet QMODULE for SCTP Failed" ) );
        return SCTP_FAILURE;
    }

    if( sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_TL, SOC_MGR_INVALID_HANDLE,
                          sctp_tl_q_module, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet QMODULE for SCTP TL Failed" ) );
        return SCTP_FAILURE;
    }

    /* SPR 20872 Ends*/
#else

    /* SPR 21004 Starts */
    sigtran_thread_t    thread_id;
    /* SPR 21004 Ends */
    sigtran_error_t     ecode;
    stack_envt_st     *p_stack_envt = ( stack_envt_st * )p_info;
    sig_ip_addr_st   local_sig_addr[SCTP_MAX_TRANSPORT_ADDR];
    sigtran_U8bit    hostname[SCTP_MAX_HOSTNAME_SIZE];
    sigtran_U32bit    num_addr = 1;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nsctp_set_stack_envt" ) );

    if( SCTP_TRUE == glb_sctp_stack_env_state )
    {
        return ( SCTP_FAILURE );
    }


    /*Initailizing the semaphore to wait till the sysop structure is
     * initialized by cspl_eshell_master_thread*/

    /*new function has to be written to initailize the semaphore because CSPL
     * is not yet initialized and hence we cannot use sigtran_sem_init */

    g_sysop_sema = system_sem_init( );

    if( g_sysop_sema == NULL )
    {
        return ( SCTP_FAILURE );
    }


    if( sigtran_create_thread( ( sigtran_thread_t * )&thread_id,
                               cspl_eshell_master_thread, ( void * )b_cspl_alrdy_initialised, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in starting CSPL MAIN thread" ) );
        return ( SCTP_FAILURE );
    }

    sigtran_sem_wait( &g_sysop_sema );

#endif

    if( NULL != p_stack_envt )
    {
        if( p_stack_envt->is_blocked == SIGTRAN_TRUE )
        {
            if( SIGTRAN_FAILURE  == sig_gethostname( hostname,
                                                     SCTP_MAX_HOSTNAME_SIZE, &ecode ) )
            {
                return SCTP_FAILURE;
            }

            if( SIGTRAN_FAILURE  == sig_resolve_hostname( hostname, local_sig_addr,
                                                          &num_addr, &ecode ) )
            {
                return SCTP_FAILURE;
            }

            local_sig_addr[0].ip_addr_ut.addr_v4.port = p_stack_envt->local_port;

            if( SIGTRAN_FAILURE ==
                    sig_open_comm_channel( SIG_SCTP_STACK, SIG_SOC_MODE_SERVER,
                                           SIG_SOC_TYPE_UDP, SIGTRAN_TRUE, SIGTRAN_FALSE, local_sig_addr,
                                           local_sig_addr, ( sig_read_cbk_func_ptr_t )sctp_dummy_recv_data,
                                           &g_dummy_sock, &ecode ) )
            {
                return SCTP_FAILURE;
            }

            sig_set_block_select();
        }
    }

    glb_sctp_stack_env_state = SCTP_TRUE;
    return( SCTP_SUCCESS );
}
/******************************************************************************
**  Function Name       : sctp_dummy_recv_data
**
**    Description       : This is a SCTP dummy function that will receive data.
**
**    Returns           :SCTP_FAILURE/SCTP_SUCCESS
**
******************************************************************************/

sigtran_return_t    sctp_dummy_recv_data(
    sigtran_U32bit      handle_id,
    sig_sock_mode_et    socket_mode,
    sig_soc_type_et     socket_type,
    sig_sock_fd_t       socket_id,
    sig_q_module_t      *p_from_q_module,
    sigtran_pvoid_t     *p_data,
    sigtran_U32bit      *p_data_len,
    sigtran_error_t     *p_ecode )
{
    /* SPR_20411_FIX_START -- */

    sctp_U8bit          p_message[SCTP_MAX_API_SIZE];
    /* SPR_20411_FIX_END -- */

    sig_ip_addr_st      from_addr;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( handle_id );
    SCTP_UNUSED_VARIABLE( socket_mode );
    SCTP_UNUSED_VARIABLE( p_from_q_module );
    SCTP_UNUSED_VARIABLE( p_data );
    /*to avoid the warning*/


    /*    sig_raw_read(g_dummy)sock, */
    /* SPR 21107 changes */
    if( SIGTRAN_FAILURE == sig_read( SIG_SCTP_STACK, socket_type,
                                     socket_id, SCTP_MAX_API_SIZE, SIG_RECV_ALL,
                                     &from_addr, ( sigtran_pvoid_t )p_message, p_data_len, p_ecode ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "\nReading data on UDP socket failed" ) );

    }

    return SCTP_FAILURE;
    /* SPR 21107 changes */
}


/******************************************************************************
**  Function Name       : cspl_eshell_master_thread
**
**    Description       : This thread is responsible for polling the sockets.
**                        This thread polls the sockets, then writes it into
**                        its queues of respective Module. This thread process
**                        on the global g_sockfd_array for timedwait processing.
**                        Changes for SCTP 6.1 - The threading architecture for                           SCTP stack has been enhanced. An option is now
                          provided to create a single thread of execution for
                          FSS SCTP stack. The thread will perform the following
                          function:
                            - Poll on the network
                            - Receive from the internal queue and process the
                            data
                            - Process the SU request in Message based interface.
                          The code for the above mentioned functionality is
                          under the flag SCTP_CMN_RECV_POLL_THREAD.
**
**    Returns           : None
**
******************************************************************************/

sctp_void_t *cspl_eshell_master_thread( void *A )
{
    sigtran_error_t  ecode;
    sig_q_manifest_st   *p_sctp_manifest_info;
    sig_q_module_t      sctp_module_id;
    sig_q_module_t      sctp_tl_q_module;
    sctp_Boolean_t     b_cspl_alrdy_initialised = ( sctp_Boolean_t )A;
#ifndef SCTP_CMN_RECV_POLL_THREAD
    sig_q_driver_t      sctp_new_driver;
#ifndef SCTP_SINGLE_THREAD_SUPPORT /* SPR 20872 */
    /* SPR 21004 Starts */
    sigtran_thread_t    thread_id;
    /* SPR 21004 Ends */
#endif
#endif
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( A );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "cspl_eshell_master_thread" ) );

    if( SCTP_FALSE == b_cspl_alrdy_initialised )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nInitialising CSPL" ) );

#ifndef SCTP_CMN_RECV_POLL_THREAD

        if( sig_qvInit( SCTP_NULL, SCTP_NULL, &ecode ) == SIGTRAN_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in initialising CSPL" ) );
            return ( SCTP_NULL );
        }

#else
        sig_qvSimpleInit( SCTP_NULL, SCTP_NULL );

        if( sctp_driver  == SIGTRAN_FAILURE )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in initialising CSPL" ) );
            return ( SCTP_NULL );
        }

#endif
    }

    /*
     * Initialise the memory module. The memory required is for the messaging.
     */
    if( SCTP_FAILURE ==  sctp_init_mem_module() )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                    ( "\nFailure in initialising memory" ) );
        return ( SCTP_NULL );
    }

    p_sctp_manifest_info = ( sig_q_manifest_st * )
                           sigtran_malloc( sizeof( sig_q_manifest_st ) );

    if( SIGTRAN_NULL == p_sctp_manifest_info )
    {
        return ( SCTP_NULL );
    }

    p_sctp_manifest_info->name = "SCTP STACK";
    p_sctp_manifest_info->service = SIG_SCTP_STACK;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_sctp_manifest_info->init.early = SCTP_NULL;
    p_sctp_manifest_info->init.late = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_sctp_manifest_info->init.early = ( sig_early_fp_t ) SCTP_NULL;
    p_sctp_manifest_info->init.late = ( sig_late_fp_t ) SCTP_NULL;
#endif
    /*
     * The common SCTP message handler function. In case if the interface with
     * application is message based, then it will also check for the messages
     * from application else only for the message from the sockets.
     */
#ifdef SCTP_UI_MESSAGEBASED
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_sctp_manifest_info->handler.message = sctp_cspl_mesg_handler;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_sctp_manifest_info->handler.message = ( sig_message_fp_t )sctp_cspl_mesg_handler;
#endif
#else
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_sctp_manifest_info->handler.message = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_sctp_manifest_info->handler.message = ( sig_message_fp_t )SCTP_NULL;
#endif
#endif
    p_sctp_manifest_info->handler.timer = sctp_timer_handler;

    /*
     * No specific handlers for other methods.
     */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_sctp_manifest_info->method.pack = SCTP_NULL;
    p_sctp_manifest_info->method.unpack = SCTP_NULL;
    p_sctp_manifest_info->method.alloc = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_sctp_manifest_info->method.pack = ( sig_pack_fp_t ) SCTP_NULL;
    p_sctp_manifest_info->method.unpack = ( sig_unpack_fp_t ) SCTP_NULL;
    p_sctp_manifest_info->method.alloc = ( sig_alloc_fp_t ) SCTP_NULL;
#endif

    /*
     * Set the list of destinations.
     */
    p_sctp_manifest_info->destinations = 0;

    /*
     * Register the SCTP stack.
     */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nResgistering SCTP Stack with CSPL" ) );

#ifndef SCTP_CMN_RECV_POLL_THREAD

    if( sig_new_cspl_driver( &sctp_new_driver, &ecode ) == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in creating a new driver" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return ( SCTP_NULL );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nResgistering SCTP Stack with CSPL" ) );

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

    if( sig_reg_with_cspl( SIG_SCTP_STACK, sctp_new_driver,
                           ( const sigtran_S8bit * ) "SCTP MODULE", SIG_SCTP_STACK,
                           p_sctp_manifest_info, SCTP_NULL, SCTP_TIMER_POOL_AUTO_EXTND,
                           &sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
#else

    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    if( sig_reg_with_cspl( SIG_SCTP_STACK, sctp_new_driver,
                           ( const sigtran_8bit * ) "SCTP MODULE", SIG_SCTP_STACK,
                           p_sctp_manifest_info, SCTP_NULL, SCTP_TIMER_POOL_AUTO_EXTND,
                           &sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
#endif
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in registring with CSPL" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/

        return SCTP_NULL;
    }

#else

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

    if( sig_reg_with_cspl( SIG_SCTP_STACK, 0,
                           ( const sigtran_S8bit * ) "SCTP MODULE", SIG_SCTP_STACK,
                           p_sctp_manifest_info, SCTP_NULL, SCTP_TIMER_POOL_AUTO_EXTND,
                           &sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
#else

    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    if( sig_reg_with_cspl( SIG_SCTP_STACK, 0,
                           ( const sigtran_8bit * ) "SCTP MODULE", SIG_SCTP_STACK,
                           p_sctp_manifest_info, SCTP_NULL, SCTP_TIMER_POOL_AUTO_EXTND,
                           &sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
#endif
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in registring with CSPL" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return SCTP_NULL;
    }

#endif

#ifdef SCTP_CLI_RECV_COM_THREAD

    if( sig_set_destination( SIG_SCTP_STACK, SIG_DEST_TYPE_TL,
                             SOC_MGR_INVALID_HANDLE, SCTP_NULL, SCTP_NULL, &sctp_tl_q_module,
                             &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet destination for TL failure" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return SCTP_NULL;
    }

#else

    if( sig_set_destination( SIG_SCTP_RECV_STACK, SIG_DEST_TYPE_TL,
                             SOC_MGR_INVALID_HANDLE, SCTP_NULL, SCTP_NULL, &sctp_tl_q_module,
                             &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet destination for TL failure" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return SCTP_NULL;
    }

#endif

    sig_cspl_set_immediate( sig_get_service_name( sctp_tl_q_module ),
                            &ecode );

    /*
     * Set the destination in the IPC Manager. Before that first initialise the
     * IPC Manager.
     */

    if( sig_init_ipc_mgr( SIG_SCTP_STACK, sctp_get_api_buf_len,
                          sctp_get_api_buf_len, SIGTRAN_NULL ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nInit IPC Manager failed" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return SCTP_NULL;
    }

    if( sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_INVALID,
                          SOC_MGR_INVALID_HANDLE, sctp_module_id, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet QMODULE for SCTP Failed" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return SCTP_NULL;
    }

    if( sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_TL, SOC_MGR_INVALID_HANDLE,
                          sctp_tl_q_module, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet QMODULE for SCTP TL Failed" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return SCTP_NULL;
    }

#ifdef SCTP_CLI_RECV_COM_THREAD
#ifndef SCTP_CLOCK_THREAD
    /*
     * This is the case, where there is single thread for communication with
     * the application, from the lower layer and from the timer, then set the
     * qmodule as set earlier in the function set stack environment for the
     * timer database.
     */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nSetting QMODULE for SCTP Stack Timer" ) );
    sigtran_timer_set_stack_qmodule( SIG_SCTP_STACK, sctp_module_id );
#endif
#endif


#ifdef SCTP_CMN_RECV_POLL_THREAD

    sigtran_sem_post( &g_sysop_sema );
    sig_qvRun( SCTP_NULL, &ecode );

#else /*Else of SCTP_CMN_RECV_POLL_THREAD */

#ifndef SCTP_SINGLE_THREAD_SUPPORT /* SPR 20872 */

    if( sigtran_create_thread( ( sigtran_thread_t * )&thread_id, sctp_ipc_master_thread,
                               ( void * )sctp_new_driver,  &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in starting SCTP Stack thread" ) );
        /* CSR 69326 fix starts*/
        __FREE( p_sctp_manifest_info );
        /* CSR 69326 fix ends*/
        return ( SCTP_NULL );
    }

#endif

    if( SCTP_FALSE == b_cspl_alrdy_initialised )
    {
        /*
         * This thread is responsible for polling the
         * sockets. Tis thread polls the sockets, then writes it into its queues of respective
         * Module. This thread process on the global g_sockfd_array for timedwait processing.
        */

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStarting CSPL Input Loop" ) );

        sigtran_sem_post( &g_sysop_sema );
        sig_cspl_input_loop( SCTP_NULL, &ecode );

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "\nStarting new thread for SCTP Stack" ) );
    }

    /*Bug Id 205 changes s*/
    /*This fix is done when CSPL is already initialized through application*/
    else
    {
        sigtran_sem_post( &g_sysop_sema );
    }

    /*Bug Id 205 changes e*/
#endif /*End of SCTP_CMN_RECV_POLL_THREAD */
    /* CSR 69326 fix starts*/
    __FREE( p_sctp_manifest_info );
    /* CSR 69326 fix ends*/

    return SCTP_NULL;
}

#ifdef SIGTRAN_LOCAL_TIME_THREAD
/***************************************************************************
 *    FUNCTION :
 *        sctp_eshell_start_localtime_thread
 *
 *    DESCRIPTION:
 *        This function starts a Local time thread.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t sctp_eshell_start_localtime_thread( sctp_error_t *p_ecode )
{
    sigtran_error_t ecode;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "sctp_eshell_start_localtime_thread: Starting Local Time Thread." ) );

    if( sigtran_create_localtime_thread( &ecode ) != SIGTRAN_SUCCESS )
    {
        *p_ecode = E_ESHELL_INIT_FAILED;
        return ( SCTP_FAILURE );
    }

    return SCTP_SUCCESS;
}

#endif

#ifdef SCTP_CLOCK_THREAD
/***************************************************************************
 *    FUNCTION :
 *        sctp_eshell_start_clock_thread
 *
 *    DESCRIPTION:
 *        This function implement the SCTP Clock thread. It updates the time
 *        and processes the timer tick.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FALILURE
 **************************************************************************/
sctp_return_t sctp_eshell_start_clock_thread( sctp_error_t *p_ecode )
{
    sig_q_manifest_st   *p_sctp_manifest_info;
    sig_q_driver_t      sctp_timer_driver;
    sig_q_module_t      sctp_timer_module_id;
    sigtran_error_t     ecode;
    /* SPR 21004 Starts */
    sigtran_thread_t         th_id;
    /* SPR 21004 Starts */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_eshell_clock_thread" ) );

    p_sctp_manifest_info = ( sig_q_manifest_st * )
                           sigtran_malloc( sizeof( sig_q_manifest_st ) );

    if( SIGTRAN_NULL == p_sctp_manifest_info )
    {
        *p_ecode = E_ESHELL_INIT_FAILED;
        return ( SCTP_FAILURE );
    }

    p_sctp_manifest_info->name = "SCTP CLOCK THREAD";
    p_sctp_manifest_info->service = SIGTRAN_SCTP_STACK_TMR;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_sctp_manifest_info->init.early = SCTP_NULL;
    p_sctp_manifest_info->init.late = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_sctp_manifest_info->init.early = ( sig_early_fp_t ) SCTP_NULL;
    p_sctp_manifest_info->init.late = ( sig_late_fp_t ) SCTP_NULL;
#endif
    /*
     * The common SCTP message handler function. In case if the interface with
     * application is message based, then it will also check for the messages
     * from application else only for the message from the sockets.
     */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
    p_sctp_manifest_info->handler.message = sctp_cspl_mesg_handler;
#else
    p_sctp_manifest_info->handler.message = SCTP_NULL;
#endif
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
#ifdef SCTP_UI_MESSAGEBASED
    p_sctp_manifest_info->handler.message = ( sig_message_fp_t ) sctp_cspl_mesg_handler;
#else
    p_sctp_manifest_info->handler.message = ( sig_message_fp_t )SCTP_NULL;
#endif
#endif
    p_sctp_manifest_info->handler.timer = sctp_timer_handler;

    /*
     * No specific handlers for other methods.
     */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_sctp_manifest_info->method.pack = SCTP_NULL;
    p_sctp_manifest_info->method.unpack = SCTP_NULL;
    p_sctp_manifest_info->method.alloc = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_sctp_manifest_info->method.pack = ( sig_pack_fp_t ) SCTP_NULL;
    p_sctp_manifest_info->method.unpack = ( sig_unpack_fp_t ) SCTP_NULL;
    p_sctp_manifest_info->method.alloc = ( sig_alloc_fp_t ) SCTP_NULL;
#endif

    /*
     * Set the list of destinations.
     */

    p_sctp_manifest_info->destinations = 0;

    /*
     * Register the SCTP stack.
     */

    sig_new_cspl_driver( &sctp_timer_driver, &ecode );

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

    if( SIGTRAN_FAILURE == sig_reg_with_cspl( SIG_SCTP_STACK, sctp_timer_driver,
                                              ( const sigtran_S8bit * )"SCTP CLOCK THREAD",
                                              SIGTRAN_SCTP_STACK_TMR, p_sctp_manifest_info, SCTP_NULL,
                                              SCTP_TIMER_POOL_AUTO_EXTND, &sctp_timer_module_id, &ecode ) )
#else

    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    if( SIGTRAN_FAILURE == sig_reg_with_cspl( SIG_SCTP_STACK, sctp_timer_driver,
                                              ( const sigtran_8bit * )"SCTP CLOCK THREAD",
                                              SIGTRAN_SCTP_STACK_TMR, p_sctp_manifest_info, SCTP_NULL,
                                              SCTP_TIMER_POOL_AUTO_EXTND, &sctp_timer_module_id, &ecode ) )
#endif
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in registring with CSPL" ) );
        return SCTP_FAILURE;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nSetting QMODULE for SCTP Stack Timer" ) );
    sigtran_timer_set_stack_qmodule( SIG_SCTP_STACK, sctp_timer_module_id );

    if( sigtran_create_thread( ( sigtran_thread_t * )&th_id,
                               &sctp_eshell_clock_thread, ( void * )sctp_timer_driver,
                               &ecode ) != SIGTRAN_SUCCESS )
    {
        *p_ecode = E_ESHELL_INIT_FAILED;
        return ( SCTP_FAILURE );
    }

    return SCTP_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_eshell_clock_thread
**
**    Description       : Function for checking timer expiry.
**
**    Returns           : SCTP_NULL
**
******************************************************************************/
sctp_void_t *sctp_eshell_clock_thread( sctp_void_t *p_sctp_driver )
{
    sigtran_error_t ecode;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_eshell_clock_thread" ) );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStarting driver loop for Timer Expiry" ) );
    sig_cspl_driver_loop( ( sig_q_driver_t )p_sctp_driver, &ecode );

    return SCTP_NULL;
}

#endif

#ifndef SCTP_CLI_RECV_COM_THREAD
/***************************************************************************
 *    FUNCTION :
 *        sctp_start_receive_timer_thread
 *
 *    DESCRIPTION:
 *        This thread is responsible for processing the data received from the
 *        lower layer. If the SCTP_LOCK_THREAD is disabled then we donot even
 *        check for timer expiry in this thread.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t sctp_start_receive_timer_thread( sctp_error_t *p_ecode )
{
#ifndef SCTP_CMN_RECV_POLL_THREAD
    sig_q_driver_t          sctp_recv_drv;
#ifndef SCTP_SINGLE_THREAD_SUPPORT /* SPR 20872 */
    /* SPR 21004 Starts */
    sigtran_thread_t         th_id;
    /* SPR 21004 Starts */
#endif

#endif
    sig_q_manifest_st       *p_sctp_manifest_info;
    static sig_q_module_t   sctp_recv_module_id;
    static sig_q_module_t   sctp_tl_q_module;
    sigtran_error_t         ecode;
    static sctp_Boolean_t   is_first_time = SCTP_TRUE;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "\nsctp_receive_timer_thread" ) );

    if( SCTP_TRUE == is_first_time )
    {
        p_sctp_manifest_info = ( sig_q_manifest_st * )
                               sigtran_malloc( sizeof( sig_q_manifest_st ) );

        if( SIGTRAN_NULL == p_sctp_manifest_info )
        {
            *p_ecode = E_ESHELL_INIT_FAILED;
            return ( SCTP_FAILURE );
        }

        p_sctp_manifest_info->name = "SCTP RECEIVER THREAD";
        p_sctp_manifest_info->service = SIG_SCTP_RECV_STACK;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        p_sctp_manifest_info->init.early = SCTP_NULL;
        p_sctp_manifest_info->init.late = SCTP_NULL;
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        p_sctp_manifest_info->init.early = ( sig_early_fp_t ) SCTP_NULL;
        p_sctp_manifest_info->init.late = ( sig_late_fp_t ) SCTP_NULL;
#endif
        /*
         * The common SCTP message handler function. In case if the interface with
         * application is message based, then it will also check for the messages
         * from application else only for the message from the sockets.
         */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        p_sctp_manifest_info->handler.message = sctp_cspl_tl_mesg_handler;
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        p_sctp_manifest_info->handler.message = ( sig_message_fp_t ) sctp_cspl_tl_mesg_handler;
#endif
        p_sctp_manifest_info->handler.timer = sctp_timer_handler;

        /*
         * No specific handlers for other methods.
         */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        p_sctp_manifest_info->method.pack = SCTP_NULL;
        p_sctp_manifest_info->method.unpack = SCTP_NULL;
        p_sctp_manifest_info->method.alloc = SCTP_NULL;
#else
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        p_sctp_manifest_info->method.pack = ( sig_pack_fp_t ) SCTP_NULL;
        p_sctp_manifest_info->method.unpack = ( sig_unpack_fp_t ) SCTP_NULL;
        p_sctp_manifest_info->method.alloc = ( sig_alloc_fp_t ) SCTP_NULL;
#endif
        /*
         * Set the list of destinations.
         */

        p_sctp_manifest_info->destinations = 0;

        /*
         * Register the SCTP stack.
         */

#ifndef SCTP_CMN_RECV_POLL_THREAD

        sig_new_cspl_driver( &sctp_recv_drv, &ecode );

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

        if( SIGTRAN_FAILURE == sig_reg_with_cspl( SIG_SCTP_RECV_STACK, sctp_recv_drv,
                                                  ( const sigtran_S8bit * )"SCTP RECEIVER THREAD",
                                                  SIG_SCTP_RECV_STACK, p_sctp_manifest_info, SCTP_NULL,
                                                  SCTP_TIMER_POOL_AUTO_EXTND, &sctp_recv_module_id, &ecode ) )
#else

        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        if( SIGTRAN_FAILURE == sig_reg_with_cspl( SIG_SCTP_RECV_STACK, sctp_recv_drv,
                                                  ( const sigtran_8bit * )"SCTP RECEIVER THREAD",
                                                  SIG_SCTP_RECV_STACK, p_sctp_manifest_info, SCTP_NULL,
                                                  SCTP_TIMER_POOL_AUTO_EXTND, &sctp_recv_module_id, &ecode ) )
#endif
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in registring with CSPL" ) );
            return SCTP_FAILURE;
        }

#else

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

        if( SIGTRAN_FAILURE == sig_reg_with_cspl( SIG_SCTP_RECV_STACK, sctp_driver,
                                                  ( const sigtran_S8bit * )"SCTP RECEIVER THREAD",
                                                  SIG_SCTP_RECV_STACK, p_sctp_manifest_info, SCTP_NULL,
                                                  SCTP_TIMER_POOL_AUTO_EXTND, &sctp_recv_module_id, &ecode ) )
#else

        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        if( SIGTRAN_FAILURE == sig_reg_with_cspl( SIG_SCTP_RECV_STACK, sctp_driver,
                                                  ( const sigtran_8bit * )"SCTP RECEIVER THREAD",
                                                  SIG_SCTP_RECV_STACK, p_sctp_manifest_info, SCTP_NULL,
                                                  SCTP_TIMER_POOL_AUTO_EXTND, &sctp_recv_module_id, &ecode ) )
#endif
        {
            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nFailure in registring with CSPL" ) );
            return SCTP_FAILURE;
        }

#endif
    }

#ifndef SCTP_CLOCK_THREAD
    /*
     * Set the module id for the timers also. This is incase only if the clock
     * thread is not invoked separately.
     */
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nSetting QMODULE for SCTP Stack Timer" ) );
    sigtran_timer_set_stack_qmodule( SIG_SCTP_STACK, sctp_recv_module_id );
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nSetting QMODULE for SCTP Stack" ) );

    if( sig_set_q_module( SIG_SCTP_RECV_STACK, SIG_DEST_TYPE_INVALID,
                          SOC_MGR_INVALID_HANDLE, sctp_recv_module_id, &ecode )
            != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet QMODULE for SCTP Failed" ) );
        return SCTP_FAILURE;
    }

    if( sig_get_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_TL, SOC_MGR_INVALID_HANDLE,
                          &sctp_tl_q_module, &ecode ) != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nGet QMODULE for SCTP TL Failed" ) );
        return SCTP_FAILURE;
    }

    if( sig_set_q_module( SIG_SCTP_RECV_STACK, SIG_DEST_TYPE_TL,
                          SOC_MGR_INVALID_HANDLE, sctp_tl_q_module, &ecode )
            != SIGTRAN_SUCCESS )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC, ( "\nSet QMODULE for SCTP TL Failed" ) );
        return SCTP_FAILURE;
    }

#ifndef SCTP_SINGLE_THREAD_SUPPORT /* SPR 20872 */
#ifndef SCTP_CMN_RECV_POLL_THREAD

    if( SCTP_TRUE == is_first_time )
    {
        is_first_time = SCTP_FALSE;

        if( sigtran_create_thread( ( sigtran_thread_t * )&th_id, &sctp_receive_timer_thread,
                                   ( void * )sctp_recv_drv, &ecode ) != SIGTRAN_SUCCESS )
        {
            return SCTP_FAILURE;
        }
    }

#endif
#endif
    return SCTP_SUCCESS;
}

/******************************************************************************
**  Function Name       : sctp_receive_timer_thread
**
**    Description       : Starts driver loop fo TL
**
**    Returns           :SCTP_NULL
**
**
**
******************************************************************************/
sctp_void_t *sctp_receive_timer_thread( sctp_void_t *p_sctp_driver )
{
    sigtran_error_t ecode;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_receive_timer_thread" ) );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStarting Driver Loop for TL" ) );
    sig_cspl_driver_loop( ( sig_q_driver_t )p_sctp_driver, &ecode );

    return SCTP_NULL;
}
#endif

/******************************************************************************
**  Function Name       : sctp_ipc_master_thread
**
**    Description       : This thread is prime thread for SCTP stack. All the
**                        polling for CSPL queues for SCTP stack is done in this
**                        thread.
**
**    Returns           :SCTP_NULL
**
******************************************************************************/
sctp_void_t *sctp_ipc_master_thread( sctp_void_t *p_sctp_driver )
{
    sigtran_error_t ecode;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_ipc_master_thread" ) );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "\nStarting CSPL Driver Loop" ) );
    sig_cspl_driver_loop( ( sig_q_driver_t )p_sctp_driver, &ecode );

    return SCTP_NULL;
}
