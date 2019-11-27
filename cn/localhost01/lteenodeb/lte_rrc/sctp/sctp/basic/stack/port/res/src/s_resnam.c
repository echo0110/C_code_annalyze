/***************************************************************************
 *  FILE NAME  : s_resnam.h
 *
 *  DESCRIPTION: Contains the Name Resolution Module.
 *
 *  DATE     NAME      REF#     REASON
 *  -----------------------------------
 *  4Apr00    sigtran    team         Original
 *  05Oct01  gsheoran   -       Rel 3.0
 *  1May13   Pranav     SPR 21127    CSR 40731 - GCC 4.7 warning removal
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#include <s_cotyps.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_tlif.h>
#include <s_osmutx.h>
#include <s_resnam.h>

#ifdef SCTP_NAME_RESOLV_THREAD

sctp_sema_t         cookie_queue_count;
sctp_lock_t         sctp_cookie_resolver_mutex;

LIST                recv_to_res_queue;
LIST                res_to_recv_queue;

sigtran_thread_t    name_res_thread;

static void *
sctp_resolver_hostname(
    void  *arg );

static sctp_Boolean_t sctp_run_resolver;
/***************************************************************************
 *    FUNCTION :
 *        sctp_resolver_hostname
 *
 *    DESCRIPTION:
 *        This function runs as separate thread in the SCTP stack. This function
 *    is blocked on arrival of SCTP packets (containing    COOKIE)    sent by
 *    receiving thread to it and resolves hostname present in them. The list of
 *    addresses it obtains is put inside the COOKIE. Then the whole of the
 *    datagram is sent back to the receiving thread. Message queues are used to
 *    transfer message.
 *
 *    RETURN VALUE:
 *        NULL
 **************************************************************************/
static void *
sctp_resolver_hostname(
    void *arg )
{
    sctp_return_t           ret;
    sctp_U32bit             ip_header_offset;
    sctp_TL_packet_st       *packet;
    SCTP_IP4                *ipheader;
    sctp_U8bit              *sctp_packet;
    sctp_chunk_cookie_st    *cookie;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_resolver_hostname" ) );

    arg = NULL;

    /* SPR 21127 change start */
    SCTP_UNUSED_VARIABLE( arg );

    /* SPR 21127 change end */
    while( sctp_run_resolver )
    {
        /* Remain blocked for till a packet with a cookie arrives */
        SCTP_SEMA_WAIT( &cookie_queue_count );

        /* Lock the cookie queue and get the packet from queue */
        SCTP_RESOLVER_LOCK();

        /* Get the cookie packet from the queue */
        packet = ( sctp_TL_packet_st * )lstGet( &recv_to_res_queue );

        /* Release the cookie queue lock*/
        SCTP_RESOLVER_UNLOCK();

        if( packet->ip_header_present == SCTP_TRUE )
        {
            ipheader = ( SCTP_IP4 * ) & ( ( packet->buffer )[0] );

            /* calculate ip header offset */
            ip_header_offset = 4 * ipheader->ip_hl;
        }

        else
        {
            ip_header_offset = 0;
        }

        /* Setting the variable to the start of cookie in packet    */
        sctp_packet =  &( ( packet->buffer )[0] ) + ip_header_offset;

        cookie = ( sctp_chunk_cookie_st * )
                 ( sctp_packet + SCTP_OFFSET_FIRSTCHUNK + SCTPCHUNKOFFSET_BODY );

        /* Assuming that hostname has been successfully resolved */
        cookie->remote_init_parameters.hostname_present = SCTP_FALSE;

        /* RESOLVE hostname and get network addresses */
        ret = sctp_TL_resolve_hostname( &( cookie->remote_init_parameters.
                                           hostname[0] ), &( cookie->remote_init_parameters.num_addrs ),
                                        &( cookie->remote_init_parameters.addr_list[0] ) );

        if( SCTP_FAILURE == ret )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "Error in Hostname resolving :%s ",
                          &( cookie->remote_init_parameters.hostname[0] ) ) );

            /* Set the error that hostname could not be resolved */
            cookie->remote_init_parameters.hostname_present = SCTP_TRUE;
        }

        /* Send cookie packet back to the recv thread by adding to out queue */
        SCTP_RESOLVER_LOCK();

        /* Add the cookie packet to outgoing queue */
        lstAdd( &res_to_recv_queue, ( NODE * )packet );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "Resolved cookie available for processing !!!" ) );


        /* Release the cookie processing function lock */
        SCTP_RESOLVER_UNLOCK();

        /* Continuing in the loop */
        continue;
    }

    return SCTP_NULL;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_hostname_resolver_init
 *
 *    DESCRIPTION:
 *        This function initializes the name resolver related semaphores and
 *    mutexs.  It also starts name resolver process in a separate thread.
 *
 *    RETURN VALUE:
 *        SUCCESS/FAILURE
 **************************************************************************/
sctp_return_t
sctp_hostname_resolver_init(
    void )
{
    sctp_error_t          ecode;
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "In function sctp_hostname_resolver_init" ) );

    /* Initialize the cookie queues */
    lstInit( &recv_to_res_queue );
    lstInit( &res_to_recv_queue );

    /* Initialize the counting semaphore to zero */
    if( SCTP_INIT_SEMA( &cookie_queue_count, 0 ) != SIGTRAN_SUCCESS )
    {
        return SCTP_FAILURE;
    }

    /* Initialize the cookie queue mutex */
    sctp_mutex_init( sctp_cookie_resolver_mutex, NULL );

    sctp_run_resolver = SCTP_TRUE;

    /* Start the name resolver function in separate thread*/
    if( sigtran_create_thread( ( sigtran_thread_t * )&name_res_thread,
                               sctp_resolver_hostname, ( void * )NULL, &ecode ) != SIGTRAN_SUCCESS )
    {
        __SEMADEINIT( &cookie_queue_count );
        sctp_mutex_destroy( sctp_cookie_resolver_mutex );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "Error starting Hostname resolver    " ) );

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}




/***************************************************************************
 *    FUNCTION :
 *        sctp_hostname_resolver_deinit
 *
 *    DESCRIPTION:
 *        This function deinitializes the name resolver related semaphores and
 *    mutexs.
 *
 *    RETURN VALUE:
 *        SUCCESS/FAILURE
 **************************************************************************/
sctp_return_t
sctp_hostname_resolver_deinit(
    void )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "In function sctp_hostname_resolver_init" ) );

    sctp_run_resolver = SCTP_FALSE;
    __SEMADEINIT( &cookie_queue_count );
    sctp_mutex_destroy( sctp_cookie_resolver_mutex );

    return ( SCTP_SUCCESS );
}

#endif

