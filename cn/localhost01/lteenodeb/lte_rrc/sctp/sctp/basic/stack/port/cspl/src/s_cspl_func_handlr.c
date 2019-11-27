/******************************************************************************
 * File Name:
 *      s_cspl_func_handlr.c
 *
 * Description:
 *      This file contains definition of all the handler functions which were
 *      registered to CSPL. This includes message handler for all the possible
 *      threads and timer handler.
 *
 * DATE          NAME           REFERENCE       REASON
 * ---------------------------------------------------
 * 06 Oct 03     Vivek Bansal
 * 27 Nov 03     Vivek Bansal
 *
 *****************************************************************************/
#define SCTP_MODULE "SCTP-CSPL MH"

#include <s_es.h>
#include <s_esdef.h>
#include <s_cotyps.h>
#include <s_mmmain.h>
#include <s_uico.h>
#include <s_cspl.h>
#include <s_in.h>
#include <s_es.h>
#include <s_red.h>
#include <s_osmem.h>
#include <s_dbg.h>
#include <s_clpapi.h>
#include <s_clsmap.h>

extern sctp_client_info_st  event_queue;
extern sctp_U16bit          sctp_num_su_connected;

/******************************************************************************
**  Function Name       : sctp_cspl_mesg_handler
**
**    Description       : This function is invoked whenever message is received
**                        from the external interfaces. The external interfaces
**                        inclues IP and Upper layer. The Uppler layer could be
**                        CSPL or non-CSPL based.
**
**    Returns           : int
**
******************************************************************************/
#ifdef SCTP_UI_MESSAGEBASED
int  sctp_cspl_mesg_handler( void *p_message, void *p_module_data )
{
#ifdef SCTP_CLI_RECV_COM_THREAD
    sig_iovec_ipc_buffer_st *p_ipc_buffer;
    sctp_sockaddr_st        from_ip_addr;
    sctp_U16bit             from_port = 0;
    sctp_sockaddr_st        on_ip_addr;
    sctp_U16bit             on_port = 0;
    sctp_TL_packet_st       *p_sctp_packet;
    sig_ip_addr_st          l_sig_on_addr;
    sigtran_return_t        ret_val;
#endif
    sctp_api_id_t           api_id;
    sig_dest_type_et        dest_type;
    sig_q_module_t          mesg_from_q_module;
    sigtran_U16bit          dest_id;
    sigtran_error_t         ecode;
    sctp_U32bit             buffer_size;
#ifdef SCTP_RED_SUPPORT
    sctp_U32bit             event_type;
#endif
    /*
     * Check for the source only if the stack is polling for both SU and TL in
     * the same thread.
     */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_module_data );
    /*to avoid the warning*/



    if( SIGTRAN_FAILURE == sig_get_from_q_module( p_message, &mesg_from_q_module,
                                                  &ecode ) )
    {
        return 1;
    }

    sig_get_q_module_info( SIG_SCTP_STACK, mesg_from_q_module, &dest_type, &dest_id );

#ifdef SCTP_CLI_RECV_COM_THREAD

    if( SIG_DEST_TYPE_TL == dest_type )
    {
        p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )p_message;
        p_sctp_packet = ( sctp_TL_packet_st * )( p_ipc_buffer->p_buffer );

        ret_val = sig_get_conn_local_addr( SIG_SCTP_STACK, p_ipc_buffer->dest_id,
                                           &l_sig_on_addr, &ecode );

        if( SIGTRAN_FAILURE == ret_val )
        {
            return 1;
        }

        SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR( l_sig_on_addr, on_ip_addr, on_port );

        SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR( p_ipc_buffer->ip_addr, from_ip_addr,
                                         from_port );

        /*
         * Message received from the transport layer.
         */

        switch( p_ipc_buffer->sock_type )
        {
            case SIG_SOC_TYPE_IPV4:
                sctp_process_TL_data_ind_inet( p_sctp_packet,
                                               p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                               &on_ip_addr, on_port );
                break;

#ifdef SCTP_INET6_SUPPORT

            case SIG_SOC_TYPE_IPV6:
                sctp_process_TL_data_ind_inet6( p_sctp_packet,
                                                p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                                &on_ip_addr, on_port );
                break;

            case SIG_SOC_TYPE_ICMPV6:
                sctp_process_TL_data_ind_icmp6( p_sctp_packet,
                                                p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                                &on_ip_addr, on_port );
                break;
#endif

            case SIG_SOC_TYPE_UDP_NW:
                sctp_process_TL_data_ind_udp( p_sctp_packet,
                                              p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                              &on_ip_addr, on_port );
                break;

            case SIG_SOC_TYPE_ICMPV4:
                sctp_process_TL_data_ind_icmp4( p_sctp_packet,
                                                p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                                &on_ip_addr, on_port );
                break;

            default:
                break;
        }

        return 1;
    }

#endif
    /*
     * Message received from IPC or from the upper layer via queues.
     */

#ifdef SCTP_UI_MESSAGEBASED
    buffer_size = sctp_buffer_get_UlongAtOffset( p_message, 12 );

#ifdef SCTP_RED_SUPPORT

    if( ( SOC_MGR_INVALID_HANDLE == dest_id ) && ( SIG_DEST_TYPE_INVALID == dest_type ) )
    {
        /*
         * Extract the event type from the received message.
         */
        sctp_buffer_get_Ulong( p_message, &event_type );

        if( SCTP_EVT_CLIENT_MSG == event_type )
        {
            /*
             * Extract the API Id from the buffer. Since the initial bytes contain
             * the event type, extract the API id from the fourth index.
             */
            api_id = sctp_buffer_get_UshortAtOffset( p_message, 4 );

            if( SCTP_API_REGISTER_SU_REQ == api_id )
            {
                if( SIGTRAN_FAILURE ==
                        sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_APP,
                                          SOC_MGR_INVALID_HANDLE, mesg_from_q_module, &ecode ) )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                                ( "\nSet QMODULE for dest Id = [%u] failed", dest_id ) );
                    return 1;
                }
            }

            else if( SCTP_API_INIT_STACK_REQ == api_id )
            {
                if( SIGTRAN_FAILURE ==
                        sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_SM,
                                          SOC_MGR_INVALID_HANDLE, mesg_from_q_module, &ecode ) )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                                ( "\nSet QMODULE for dest Id = [%u] failed", dest_id ) );
                    return 1;
                }
            }
        }
    }

    /*
     * In case redundancy is enabled, send the message received from the redundancy
     * manager to this function. This function further behave depending upon
     * the event type.
     */
    sctp_process_events( p_message, buffer_size );
#else

    /*
     * Extract the API Id from the buffer.
     */
    if( SOC_MGR_INVALID_HANDLE == dest_id )
    {
        api_id = sctp_buffer_get_UshortAtOffset( p_message, 0 );

        if( SCTP_API_REGISTER_SU_REQ == api_id )
        {
            if( SIGTRAN_FAILURE ==
                    sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_APP,
                                      SOC_MGR_INVALID_HANDLE, mesg_from_q_module, &ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nSet QMODULE for dest Id = [%u] failed", dest_id ) );
                return 1;
            }
        }

        else if( SCTP_API_INIT_STACK_REQ == api_id )
        {
            if( SIGTRAN_FAILURE ==
                    sig_set_q_module( SIG_SCTP_STACK, SIG_DEST_TYPE_SM,
                                      SOC_MGR_INVALID_HANDLE, mesg_from_q_module, &ecode ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "\nSet QMODULE for dest Id = [%u] failed", dest_id ) );
                return 1;
            }
        }
    }

    /*
     * This is case when the message is received from SU/SM.
     */
    sctp_stack_proc_client_msg( p_message, buffer_size );
#endif
#endif

    /*
     ** The code for removing sctp notifications added here and in timer handler
     ** so that the notificatin queue is checked for everytime a timer expires or a
     ** message is received on the sockets.
     */
#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_INDICATION_THREAD
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE

    /* dequeue all the notifications from the queue*/
    while( SCTP_SUCCESS == sctp_remove_notifications_from_queue() );

#endif
#endif
#endif
#endif

    return 1;
}
#endif

/******************************************************************************
**  Function Name       : sctp_timer_handler
**
**    Description       : This is a timer expiry handler function. This will call
**                        the stack function which will process on the timer
**                        expiry.
**
**    Returns           :NULL
**
******************************************************************************/
void sctp_timer_handler(
    void    *p_timer_id,
    void    *p_timer_buffer,
    void    *p_module_id )
{
#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_INDICATION_THREAD
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE
    sctp_U32bit           event =
        ( ( sctp_timer_buffer_st * )p_timer_buffer )->event;
#endif
#endif
#endif
#endif

    sctp_process_time_tick( ( sctp_timer_t )p_timer_id,
                            ( sctp_timer_buffer_st * )p_timer_buffer );

#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_INDICATION_THREAD
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE

    /* dequeue all the notifications from the queue*/
    while( SCTP_SUCCESS == sctp_remove_notifications_from_queue() );

    if( event == SCTPEVENT_TIMER_DEQUE )
    {
        sctp_start_notification_queue_timer();
    }

#endif
#endif
#endif
#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_module_id );
    /*to avoid the warning*/

}

/******************************************************************************
**  Function Name       : sctp_cspl_tl_mesg_handler
**
**    Description       : Message handler which get invoked when there is a
**                        a message for SCTP stack from TL
**
**    Returns           : int
**
******************************************************************************/
int  sctp_cspl_tl_mesg_handler( void *p_message, void *p_module_data )
{
    sig_iovec_ipc_buffer_st *p_ipc_buffer;
    sctp_sockaddr_st        from_ip_addr;
    sctp_U16bit             from_port = 0;
    sctp_sockaddr_st        on_ip_addr;
    sctp_U16bit             on_port = 0;
    sctp_TL_packet_st       *p_sctp_packet;
    sig_ip_addr_st          l_sig_on_addr;
    sigtran_return_t        ret_val;
    sig_stack_type_et       stack_type;
    sigtran_error_t         sig_e_code;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_module_data );
    /*to avoid the warning*/


    p_ipc_buffer = ( sig_iovec_ipc_buffer_st * )p_message;
    p_sctp_packet = ( sctp_TL_packet_st * )( p_ipc_buffer->p_buffer );

#ifdef SCTP_CLI_RECV_COM_THREAD
    stack_type = SIG_SCTP_STACK;
#else
    stack_type = SIG_SCTP_RECV_STACK;
#endif
    ret_val = sig_get_conn_local_addr( stack_type, p_ipc_buffer->dest_id,
                                       &l_sig_on_addr, &sig_e_code );

    if( SIGTRAN_FAILURE == ret_val )
    {
        return 1;
    }

    SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR( l_sig_on_addr, on_ip_addr, on_port );

    SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR( p_ipc_buffer->ip_addr, from_ip_addr,
                                     from_port );

    /*
     * Message received from the transport layer.
     */

    switch( p_ipc_buffer->sock_type )
    {
        case SIG_SOC_TYPE_IPV4:
            sctp_process_TL_data_ind_inet( p_sctp_packet,
                                           p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                           &on_ip_addr, on_port );
            break;

#ifdef SCTP_INET6_SUPPORT

        case SIG_SOC_TYPE_IPV6:
            sctp_process_TL_data_ind_inet6( p_sctp_packet,
                                            p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                            &on_ip_addr, on_port );
            break;

        case SIG_SOC_TYPE_ICMPV6:
            sctp_process_TL_data_ind_icmp6( p_sctp_packet,
                                            p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                            &on_ip_addr, on_port );
            break;
#endif

        case SIG_SOC_TYPE_UDP_NW:
            sctp_process_TL_data_ind_udp( p_sctp_packet,
                                          p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                          &on_ip_addr, on_port );
            break;

        case SIG_SOC_TYPE_ICMPV4:
            sctp_process_TL_data_ind_icmp4( p_sctp_packet,
                                            p_ipc_buffer->buffer_size, &from_ip_addr, from_port,
                                            &on_ip_addr, on_port );
            break;

        default:
            break;
    }

    /*
     ** The code for removing sctp notifications added here and in timer handler
     ** so that the notificatin queue is checked for everytime a timer expires or a
     ** message is received on the sockets.
     */
#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_INDICATION_THREAD
#ifndef SCTP_UI_SOCK_API
#ifndef SCTP_NO_QUEUE

    /* dequeue all the notifications from the queue*/
    while( SCTP_SUCCESS == sctp_remove_notifications_from_queue() );

#endif
#endif
#endif
#endif

    return 1;
}
