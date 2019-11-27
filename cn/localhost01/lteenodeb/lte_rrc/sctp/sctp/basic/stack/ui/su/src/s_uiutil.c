/****************************************************************************
 *    FILE NAME  : s_uiutil.c
 *
 *    DESCRIPTION: This file contains some utility functions for the SU module
 *
 *    DATE     NAME         REFERENCE              REASON
 *    ----     ----         ---------              ------
 *    09Sep01  gsheoran         -                  Create Original
 *    19Dec01  ygahlaut     SPRXXXX                SPR Fix
 *    20May02  ygahlaut     SPR 4074 4058 4068 Fix
 *    28May02  ygahlaut     SPR 4160 4153 4154 Fix
 *    27Aug02  mrajpal                              SPR 4798
 *    26Jul02  mrajpal      Rel 4.0                 New Send API Support
 *    08Oct02  mrajpal      SPR 4925                Read/Write getsockopt
 *    08Oct02  mrajpal      SPR 4926                getsockopt params
 *    10Oct02  mrajpal      SPR 4937                recvmsg failure
 *    10Oct02  mrajpal      SPR 4929                select with recv call
 *    21Nov02  mrajpal      SPR 5079                set_primary addr
 *    25Nov02  mrajpal      SPR 5092, 5094          udp connect
 *    06Jan03  mrajpal      SPR 5225                SPR Fix
 *    06Jan03  mrajpal      SPR 5218                SPR Fix
 *    27Apr04  mrajpal      SPR 6647                SPR Fix
 *    19Oct04  Amit Ray     REL 6.0                SCTP_6_0_SCTP/FUNC-CONGCON-4
 *    21Oct04  Amit Ray     REL 6.0                SCTP/FUNC-IG-13
 *    26Oct04  Svakumar     REL 6.0                SCTP/FUNC-SOCKAPI-36
 *    26Oct04  Svakumar     REL 6.0                SCTP/FUNC-SOCKAPI-35
 *    27Oct04  Amit Ray     REL 6.0                SCTP/FUNC-SOCKAPI-34
 *    28Oct04  Amit Ray     REL 6.0                SCTP/FUNC-ASSOCST-12
 *    02Nov04  Svakumar     REL 6.0                SCTP/FUNC-CONGCON-5
 *    09May08  Raza         REL 6.2                SPR 18527
 *    07JULY08 Shikha       REL6.2.0               rfc upgrade
 *    08JULY08 Raza         REL 6.2                rfc upgrade
 *    24JULY08 Raza         REL 6.2                CSR: 1-5465175
 *    18Feb13  Rohan Pawa   SPR 21107          CSR 24680
 *    1May13  Pranav       SPR 21127              CSR 40731- GCC 4.7 warning removal
 *    5May13  Pranav Sharma SPR 21107              CSR 44034
 *    1July13 Rohan Pawa   SPR 21141              CSR 50452
 *    25Oct13 Pinky Sinha  SPR 21157              CSR 56258
 *    6Mar14  Rohan Pawa                          CSR 69326
 *    5May14  Rohan Pawa   Bug 5                      CSR 77081
 *    17Jul14    Pinky Sinha      Bug Id 13       CSR  84956
 *    15Sep14   Priyanka Mishra   Bug Id 22     CSR 85223
*     6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "UI"

#include <s_cotyps.h>
#include <s_cogbl.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_dbutil.h>
#include <s_fsmdat.h>
#include <s_uiutil.h>
#include <s_dbg.h>
#include <s_stats.h>
#include <s_os.h>
#include <s_osutil.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_in.h>
#include <s_err.h>
#include <s_fsmass.h>
#include <s_uisusk.h>
#include <s_comac.h>
#include <s_mhbld.h>
#include <s_es.h>

#ifdef SCTP_LOAD_TEST
#define TIME_DIFF(A,B) ((((A.tv_sec - B.tv_sec)*1000000) + A.tv_usec) - B.tv_usec)

extern sctp_Boolean_t  record_uptime;
extern struct timeval up_s_time, up_e_time;
extern unsigned long min_uptime;
extern unsigned long max_uptime;
extern unsigned long avg_uptime;
unsigned long  time_diff = 0;

#endif


/***************************************************************************
 *    FUNCTION    : sctp_accept_blocking
 *
 *    DESCRIPTION : It is used to do a blocking accept.
 *
 *    RETURN VALUE: On success returns the new assoc id otherwise -1
 ****************************************************************************/
int
sctp_accept_blocking(
    sctp_ep_st  *p_ep )
{
    sctp_tcb_st  *p_tcb = SCTP_NULL;
    sctp_U32bit  ep_id  = p_ep->ep_id;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_accept_blocking" ) );

    /*
     * Non blocking Accept.
     * 1. Get the first association from the endpoint queue.
     * 2. Decrement the number of associations pending.
     * 3. Create a new EP for the extracted tcb.
     * 4. Return the ep_id of the newly created Endpoint.
     */

    if( p_ep->num_created_ep > 0 )
    {
        /* return first new ep.*/
        p_ep = sctp_db_get_ep_ptr( p_ep->created_ep_id[p_ep->num_created_ep - 1] );
    }

    /* SPR : 18527 */
    if( ( p_ep != SCTP_NULL ) && !( p_tcb = sctp_db_get_assoc_from_ep( p_ep ) ) )
        /* SPR : 18527 */
    {
        /*
         * If we cannot get an assoc then we need to block.
         * We sould be woken up when we get a new connection on this server.
         */

        /* Put the endpoint to sleep till we get a response */
        SCTP_SLEEP( p_ep->cond, p_ep->lock );
    }

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_tcb );
    /* SPR 21107 changes end */

    /* Get the ptr again */
    p_ep = sctp_db_get_ep_ptr( ep_id );

    if( !p_ep )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_accept_blocking:failed: couldnot get id after sleep." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Once we're out of the block we know that we have an association waiting
     * to be accepted so we invoke the nonblocking function.
     */
    return sctp_accept_nonblocking( p_ep );
}


/***************************************************************************
 *    FUNCTION    : sctp_accept_nonblocking
 *
 *    DESCRIPTION : It is used to do a nonblocking accept.
 *
 *    RETURN VALUE: On success returns the new assoc id otherwise -1
 ****************************************************************************/
int
sctp_accept_nonblocking(
    sctp_ep_st  *p_ep )
{
    sctp_tcb_st  *p_tcb    = SCTP_NULL;
    sctp_ep_st   *p_new_ep = SCTP_NULL;
    sctp_error_t s_err;
    sctp_endpoint_chg_data_ut      status_data;
    sctp_sockaddr_st               *addr_list;
    sctp_U32bit                     counter;
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_accept_nonblocking" ) );

    if( p_ep->num_created_ep > 0 )
    {
        /* return first new ep.*/
        p_ep->num_created_ep--;
        return p_ep->created_ep_id[p_ep->num_created_ep];
    }

    /*
     * Non blocking Accept.
     * 1. Get the first association from the endpoint queue.
     * 2. Decrement the number of associations pending.
     * 3. Create a new EP for the extracted tcb.
     * 4. Return the ep_id of the newly created Endpoint.
     */
    if( !( p_tcb = sctp_db_get_assoc_from_ep( p_ep ) ) )
    {
        /* If we cannot get an assoc then we return an error. */
        sk_errno = SK_EAGAIN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_accept_nonblocking:Accept failed: No assoc's." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Create a new endpoint with the tcb that's extracted.
     */
    if( !sctp_db_create_duplicate_ep( p_ep, p_tcb, &p_new_ep, &s_err ) )
    {
        /* Unknown error occured while creating the new ep */
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_accept_nonblocking:failed: couldnot accept new assoc." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*Send the notification*/

    status_data.endpoint_up.num_dest_addrs =
        p_tcb->transmit.num_remote_address;

    addr_list = ( sctp_sockaddr_st * ) status_data.endpoint_up.list_of_addrs;

    for( counter = 0; counter < p_tcb->transmit.num_remote_address;
            counter++ )
    {
        addr_list[counter] = p_tcb->transmit.remote[counter].that_address;
    }

    status_data.endpoint_up.num_in_streams =
        p_tcb->receive.num_in_streams;

    status_data.endpoint_up.num_out_streams =
        p_tcb->transmit.num_out_streams;

    /* Send User notification*/
    sctp_ntfy_endpt_status_change( p_tcb, SCTP_COMMUNICATION_UP,
                                   &status_data, E_NOERROR );

    /* Send User Notification for all the Confirmed and Available Addresses. */
    for( counter = 0; counter < p_tcb->transmit.num_remote_address; counter++ )
    {
        if( p_tcb->transmit.remote[counter].is_confirmed == SCTP_TRUE )
        {
            sctp_ntfy_dst_status_change( p_tcb,
                                         &p_tcb->transmit.remote[counter].that_address,
                                         SCTP_ADDRESS_CONFIRMED, E_NOERROR );
        }

        if( p_tcb->transmit.remote[counter].status == SCTP_ADDRESS_AVAILABLE )
        {
            sctp_ntfy_dst_status_change( p_tcb,
                                         &p_tcb->transmit.remote[counter].that_address,
                                         SCTP_ADDRESS_AVAILABLE, E_NOERROR );
        }

        p_tcb->conn_accepted = SCTP_TRUE;
    }

    /*
     * Return the new Endpoint id
     */
    return ( p_new_ep->ep_id );
}


/***************************************************************************
 *    FUNCTION    : sctp_send_general
 *
 *    DESCRIPTION : It is used to send data on an association. It is assumed
 *    that the  endpoint is locked at this moment before sending the data.
 *
 *    RETURN VALUE: On success number of bytes sent
 ****************************************************************************/
int
sctp_send_general(
    sctp_tcb_st        *p_tcb,
    sctp_sockaddr_st   *p_daddr,
    sctp_U32bit        iovec_len,
    sctp_iovec_st      *p_iovec,
    sctp_stream_t      stream_id,
    sctp_U32bit        life_time,
    sctp_U32bit        ppi,
    sctp_Boolean_t     bundle,
    sctp_Boolean_t     ordered,
    sctp_U32bit        context,
    sctpbuffer_st      **p_p_tx_buf,
    sctp_error_t       *p_ecode )
{
    sctpbuffer_st    *p_sctp_buf          = SCTP_NULL;
    sctp_U32bit      dest_addr_index      = 0;
    sctp_U32bit      max_frag_size        = 0;
    sctp_U32bit      num_buffers          = 0;
    sctp_U32bit      counter              = 0;
    sctp_U32bit      free_buffer_count    = 0;
    sctp_U32bit      old_tx_window        = 0;
    sctp_U32bit      old_cong_reg       = 0;
    sctp_U32bit      new_cong_reg       = 0;
    sctp_U32bit      buffer_size          = 0;
    sctp_U32bit      user_data_size       = 0, data_size = 0;
    sctp_U32bit iov_ctr = 0, iov_rem_bytes = 0;
    char             *p_iovec_buf = SCTP_NULL;
    sctp_U8bit        max_buff_req;
    sctp_U8bit        med_buff_req;
    sctp_U8bit        min_buff_req;
    sctp_Boolean_t    complex_tech_applied = SCTP_FALSE;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_send_general" ) );

    /*
     * If the connection is not established then return failure.
     * Else even if the connection is established check if we're allowed to send
     * On this socket.
     */
    /* pass the UDP style socket here without check */
    if( ( p_tcb->p_ep->sk_type == SCTP_SOCK_STREAM ) &&
            ( ( !SCTP_SOCK_IS_WRITEABLE( p_tcb->p_ep->flags ) ) ||
              ( SCTPSTATE_ESTABLISHED != p_tcb->state ) ) )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;

        sk_errno = SK_ESHUTDOWN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_send_general:API Requested in Invalid State" ) );

        SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, stream_id,
                                     SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );

        return ( 0 );
    }

    /*calculate data_size*/
    for( counter = 0 ; counter < iovec_len ; counter++ )
    {
        if( p_iovec[counter].iov_len > 0 )
        {
            data_size += p_iovec[counter].iov_len;

            if( SCTP_NULL == p_iovec[counter].iov_base )
            {
                SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_send_general: Null data " ) );
                SCTP_SET_ECODE( p_ecode, EINV_PARAMS );

                SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, stream_id,
                                             SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );

                goto RET_ERROR;
            }
        }
    }

    user_data_size = data_size;

    /*
     * verify stream/datasize and payload parameters
     * The check is >= because we have stream count from 0.
     */
    if( ( stream_id >= p_tcb->transmit.num_out_streams )
            || ( !data_size ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_send_general: Invalid stream Parameters " ) );
        SCTP_SET_ECODE( p_ecode, EINV_PARAMS );

        goto RET_ERROR;
    }

    /*
     * If the destiantion address parameter is not null then check its validity.
     * Abt the dest addr parameter we move like this.. if thhis field is
     * INADDR_ANY then we choose a primary address. If this field is null then
     * we also choose the primary address else we try to validate the remote
     * address and send on that.
     */
    if( !p_daddr || SCTP_IS_INADDR_ANY( ( *p_daddr ) ) )
    {
        /* Choose the primary address as the destination address. */
        dest_addr_index = p_tcb->transmit.primary_address_index;
    }

    else
    {
        /* The address passed is ok: Check its validity */
        if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb, p_daddr,
                                                          &dest_addr_index ) )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_send_general: Invalid destination address passed" ) );

            SCTP_SET_ECODE( p_ecode, EINV_PARAMS );
            SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, stream_id,
                                         SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );

            goto RET_ERROR;
        }
    }

    /*If the address is given and its status is not UP then choose primary
     *to send data */

    if( p_daddr &&
            ( p_tcb->transmit.remote[dest_addr_index].status != SCTP_COMMUNICATION_UP ||
              p_tcb->transmit.remote[dest_addr_index].is_confirmed == SCTP_FALSE ) )
    {
        dest_addr_index = p_tcb->transmit.primary_address_index;
    }

    /*
     * Check whether the we have transmit buffers to hold all the data that is
     * passed from the ULP.
     */
    max_frag_size = SCTP_IS_STACK_UDP ?
                    SCTP_UDP_MAX_FRAG_SIZE( p_tcb->transmit.lowest_path_mtu ) :
                    SCTP_IP_MAX_FRAG_SIZE( p_tcb->transmit.lowest_path_mtu );


    /* SPR 6621*/
    if( max_frag_size > SCTP_MAX_FRAGSIZE )
    {
        max_frag_size = SCTP_MAX_FRAGSIZE;
    }

    /* SPR 6621*/


    if( ( p_tcb->config.maxseg != 0 ) &&
            ( max_frag_size >= p_tcb->config.maxseg ) )
    {
        max_frag_size = p_tcb->config.maxseg;
    }

    /*SPR 4975*/
    /*SPR 5225*/
    if( max_frag_size >= data_size )
        /*SPR 5225*/
    {
        max_frag_size = data_size;
    }

    else
    {
        if( p_tcb->config.dont_fragment )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_send_general: Fragmentation required\n" ) );
            SCTP_SET_ECODE( p_ecode, E_FRAGMENTATION );
            SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, stream_id,
                                         SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );

            goto RET_ERROR;
        }
    }

    /*SPR 4975*/

    /* Check if we have free buffers to take in this data message. */
    free_buffer_count = sctp_db_assoc_tx_buffer_num( p_tcb, max_frag_size );

    /* Number of buffers needed to store this message */
    num_buffers = data_size / max_frag_size;

    if( data_size % max_frag_size != 0 )
    {
        num_buffers++;
    }

    if( num_buffers > free_buffer_count )
    {
        num_buffers = sctp_db_assoc_tx_buffer_num_recompute( p_tcb,
                                                             max_frag_size, data_size, &max_buff_req, &med_buff_req, &min_buff_req );

        /*Buffer can not be allocated anyway. Hence return error*/

        if( num_buffers == 0 )
        {

            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_send_general: No Transmit Buffer for Send: setting the "
                          "socket as not WRITEABLE" ) );
            SCTP_SET_ECODE( p_ecode, E_NORESOURCES );

            SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, stream_id,
                                         SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );

            /* Set the socket as not writeable */
            /*SPR 5226*/
            /*Check if congestion has occurred */
            if( 3 == p_tcb->transmit.congestion_event_sent )
            {
                SCTP_SOCK_SET_WRITEABLE( p_tcb->p_ep->flags, SCTP_FALSE );
            }

            /*SPR 5226*/
            goto RET_ERROR;
        }

        else
        {
            complex_tech_applied = SCTP_TRUE ;
        }
    }

    /* Extract the old tx window. */
    old_tx_window = sctp_db_assoc_tx_window( p_tcb );


    /* SPR 21127 CHANGES START */
    SCTP_UNUSED_VARIABLE( old_tx_window );
    /* SPR 21127 CHANGES END */

    /* Fragment and send the message */
    iov_ctr = 0;
    iov_rem_bytes = p_iovec[iov_ctr].iov_len;
    p_iovec_buf   = p_iovec[iov_ctr].iov_base;

    for( counter = 0; counter < num_buffers; counter++ )
    {
        sctp_U8bit *p_buf;

        if( complex_tech_applied == SCTP_TRUE )
        {
            if( min_buff_req )
            {
                buffer_size = ( SCTP_MIN_FRAGSIZE < max_frag_size ) ?
                              SCTP_MIN_FRAGSIZE : max_frag_size;

                min_buff_req--;
            }

            else if( med_buff_req )
            {
                buffer_size = ( SCTP_MED_FRAGSIZE < max_frag_size ) ?
                              SCTP_MED_FRAGSIZE : max_frag_size;

                med_buff_req--;
            }

            else if( max_buff_req )
            {
                buffer_size = ( SCTP_MAX_FRAGSIZE < max_frag_size ) ?
                              SCTP_MAX_FRAGSIZE : max_frag_size;

                max_buff_req--;
            }

        }

        else
        {
            buffer_size = ( data_size < max_frag_size ) ? data_size : max_frag_size;
        }

        /* Get a free transmit buffer */
        p_sctp_buf = sctp_get_transmit_buffer( p_tcb, buffer_size );

        /* Unable to get Transmit buffer */
        if( !p_sctp_buf )
        {
            SCTP_SET_ECODE( p_ecode, E_NORESOURCES );
            SCTP_INCR_ASSOC_STREAM_STAT( p_tcb, stream_id,
                                         SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL, 1 );

            goto RET_ERROR;

        }

        p_sctp_buf->ack_flag     = SCTP_ACKFLAG_UNACKED;
        p_sctp_buf->retrans_flag = SCTP_RETRANSFLAG_NONE;
        p_sctp_buf->begin        = ( counter == 0 ) ? 1 : 0;
        p_sctp_buf->end          = ( counter == num_buffers - 1 ) ? 1 : 0;
        p_sctp_buf->unordered    = ordered ? 0 : 1;
        p_sctp_buf->stream       = stream_id;

        /* Change for SPR No 8665*/
        p_sctp_buf->size         = 0;

        p_sctp_buf->ssn          = 0;

        /* Rel 6.2.0 change start */
#if defined (SCTP_HOST_PPID)
        p_sctp_buf->ppi          = ntohl( ppi );
#else
        p_sctp_buf->ppi          = ppi;
#endif
        /* Rel 6.2.0 change end */

        /* Set the Destination Address */
        p_sctp_buf->dest_addr_index = dest_addr_index;

        /* Set the lifetime parameter */
        p_sctp_buf->lifetime = ( !life_time ) ? SCTP_MAX_LIFE_TIME : life_time;

        /* Set the context */
        p_sctp_buf->context = context;

        /* Fill in the local housekeeping data here */
        p_sctp_buf->sent   = 0;
        /* Changes For CSR#1-5037386 */
        p_tcb->transmit.remote[dest_addr_index].unsent_packet_count++;
        /* Changes For CSR#1-5037386 Ends */
        p_sctp_buf->missed = 0;

        /*SPR 4798 */
        /*Bug Id 22 fix starts*/
        /*bundle = (p_tcb->config.bundling_time == 0)?0:bundle;*/
        bundle = ( p_tcb->config.bundling_time == 0 ) ? SCTP_FALSE : bundle;
        /*Bug Id 22 fix ends*/
        /*SPR 4798 */

        p_sctp_buf->bundle = ( bundle ) ? 1 : 0;

        p_buf = p_sctp_buf->buffer;

        /* Copy the user data into the buffer */
        do
        {
            /* this iovec could be continuing from previos frag
             * copy remaining bytes
             */
            sctp_U32bit n_bytes =
                ( iov_rem_bytes > buffer_size ) ? buffer_size : iov_rem_bytes;

            __MEMCOPY( p_buf, p_iovec_buf, n_bytes );


            /* Change for SPR No 8665*/
            p_sctp_buf->size         += n_bytes;

            if( ( iov_rem_bytes -= n_bytes ) > 0 )
            {
                /*buffer exhausted*/
                p_iovec_buf += n_bytes;
                break;
            }

            /*move to next iov*/
            if( ++iov_ctr < iovec_len )
            {
                iov_rem_bytes = p_iovec[iov_ctr].iov_len;
                p_iovec_buf   = p_iovec[iov_ctr].iov_base;
            }

            else
            {
                break;
            }

            if( ( buffer_size -= n_bytes ) > 0 )
            {
                p_buf += n_bytes;
            }
        }
        while( 1 ); /*already checkin for iov_ctr < iov_len above)*/

        data_size -= p_sctp_buf->size;
        lstAdd( &p_tcb->transmit.transmit_queue, ( NODE * )p_sctp_buf );
    } /* for */

    /* Fill up data for the SU event */
    p_tcb->su_event.evt_cmduserdata.addr_index = dest_addr_index;

    /* Check for UDP style socket if the desired association is not in
     * established state donot send data now, it will be sent as
     * COOKIE ACK arrives.
     */

    if( ( p_tcb->p_ep->sk_type == SCTP_SOCK_STREAM ) ||
            ( ( p_tcb->p_ep->sk_type == SCTP_SOCK_SEQPACKET ) &&
              ( ( SCTP_SOCK_IS_WRITEABLE( p_tcb->p_ep->flags ) ) ||
                ( SCTPSTATE_ESTABLISHED == p_tcb->state ) ) ) )
    {

        /* Start life time timer if required */
        /* SPR 5003 */
        if( ( 0 != life_time ) &&
                ( SCTP_MAX_LIFE_TIME != life_time ) )
        {
            if( p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_LIFETIME] ==
                    SCTP_NULL_TIMER_ID )
            {
                sctp_start_timer(
                    p_tcb,
                    SCTPEVENT_TIMERLIFETIME,
                    SCTP_ONE_SECOND_TIME,
                    0,
                    &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_LIFETIME],
                    SCTP_NULL );
            }
        }

        /* SPR 5003 */

        /* SPR 2468 */
        /* start bundling timer */

        if( bundle == SCTP_FALSE )
        {
            sctp_stop_timer( &p_tcb->transmit.remote[dest_addr_index].
                             dest_timer_id[SCTP_DESTTIMER_BUNDLING],
                             SCTP_NULL, SCTP_NULL );

            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDUSERDATA );
        }

        else
        {
            if( p_tcb->transmit.remote[dest_addr_index].
                    dest_timer_id[SCTP_DESTTIMER_BUNDLING] == SCTP_NULL_TIMER_ID )
            {
                p_tcb->transmit.remote[dest_addr_index].wait_bundle =
                    SCTP_MAX_ALLHEADERS_LEN + SCTP_OFFSET_BODY +
                    SCTP_MIN_DATACHUNK_LEN + user_data_size + 4;

                if( p_tcb->transmit.remote[dest_addr_index].wait_bundle >=
                        ( sctp_U32bit )( p_tcb->transmit.lowest_path_mtu - SCTP_BUNDLING_THRESHOLD ) )
                {
                    sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDUSERDATA );
                }

                else
                {
                    sctp_start_timer(
                        p_tcb,
                        SCTPEVENT_TIMERBUNDLE,
                        p_tcb->config.bundling_time,
                        dest_addr_index,
                        &p_tcb->transmit.remote[dest_addr_index].
                        dest_timer_id[SCTP_DESTTIMER_BUNDLING],
                        SCTP_NULL );
                }
            }

            else
            {
                p_tcb->transmit.remote[dest_addr_index].wait_bundle +=
                    SCTP_MIN_DATACHUNK_LEN + user_data_size + 4;

                if( p_tcb->transmit.remote[dest_addr_index].wait_bundle >=
                        ( sctp_U32bit )( p_tcb->transmit.lowest_path_mtu - SCTP_BUNDLING_THRESHOLD ) )
                {
                    sctp_stop_timer( &p_tcb->transmit.remote[dest_addr_index].
                                     dest_timer_id[SCTP_DESTTIMER_BUNDLING],
                                     SCTP_NULL, SCTP_NULL );
                    sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDUSERDATA );
                }
            }
        }

        /*************/
    }

    if( SCTP_SUCCESS == sctp_calc_trx_cong_reg( p_tcb, &new_cong_reg ) )
    {
        old_cong_reg = p_tcb->transmit.congestion_event_sent;

        /*
         * Check whether we have new congestion. And if so then give congestion
         * indication to the user.
         */
        if( new_cong_reg != old_cong_reg )
        {
            p_tcb->transmit.congestion_event_sent = new_cong_reg;

            /* notify su of change in congestion level */
            sctp_ntfy_congestion( p_tcb, new_cong_reg );
        }
    }

    /*Return the Tx buffer pointer*/
    if( p_p_tx_buf )
    {
        if( 1 == num_buffers )
        {
            *p_p_tx_buf = p_sctp_buf;
            p_sctp_buf->ref_count++;
        }

        else
        {
            *p_p_tx_buf = SCTP_NULL;
        }
    }

    return ( user_data_size );

RET_ERROR:
    SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                ECODE( p_ecode ), "Service User API ERROR" );

    SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );


    return ( 0 );
}

/***************************************************************************
**
** FUNCTION NAME:       sctp_calc_trx_cong_reg
**
** DESCRIPTION  :       This function evaluate transmit congestion region.
**                      If there is change in cong region give notify.
**
** RETURNS      :      SCTP_SUCCESS
**                      SCTP_FALILURE
**
***************************************************************************/
sctp_return_t   sctp_calc_trx_cong_reg(
    sctp_tcb_st         *p_assoc,
    sctp_U32bit         *p_new_cong_reg )
{
    sctp_U32bit      new_buffer_occupancy = 0;
    sctp_U32bit      max_tx_window        = 0;
    sctp_U32bit      new_tx_window        = 0;
    sctp_U32bit      old_cong_reg     = p_assoc->transmit.congestion_event_sent;
    sctp_cong_reg_def_st cong_reg_def = p_assoc->trx_cong_reg_def;

    new_tx_window = sctp_db_assoc_tx_window( p_assoc );
    max_tx_window = sctp_db_assoc_max_tx_window( p_assoc );

    /* Calculate buffer occupancy */
    new_buffer_occupancy =  100 - ( 100 * new_tx_window / max_tx_window );

    /*
     * Check whether we have a congestion. And if so then give congestion
     * indication to the user.
     */
    if( new_buffer_occupancy >= cong_reg_def.no_cong_lvl_onset
            && new_buffer_occupancy <= cong_reg_def.low_cong_lvl_abate )
    {
        /* Congestion region is NCR */
        *p_new_cong_reg = 0;
        return SCTP_SUCCESS;
    }

    else if( new_buffer_occupancy >= cong_reg_def.low_cong_lvl_onset
             && new_buffer_occupancy <= cong_reg_def.med_cong_lvl_abate )
    {
        /* Congestion region is LCR */
        *p_new_cong_reg = 1;
        return SCTP_SUCCESS;
    }

    else if( new_buffer_occupancy >= cong_reg_def.med_cong_lvl_onset
             && new_buffer_occupancy <= cong_reg_def.high_cong_lvl_abate )
    {
        /* Congestion region is MCR */
        *p_new_cong_reg = 2;
        return SCTP_SUCCESS;
    }

    else if( new_buffer_occupancy >= cong_reg_def.high_cong_lvl_onset )
    {
        /* Congestion region is HCR */
        *p_new_cong_reg = 3;
        return SCTP_SUCCESS;
    }

    else if( new_buffer_occupancy > cong_reg_def.low_cong_lvl_abate
             && new_buffer_occupancy < cong_reg_def.low_cong_lvl_onset )
    {
        if( ( old_cong_reg == 0 ) ||
                ( old_cong_reg == 1 ) )
        {
            *p_new_cong_reg = old_cong_reg;
        }

        else
        {
            /* Congestion region is LCR */
            *p_new_cong_reg = 1;
        }

        return SCTP_SUCCESS;
    }

    else if( new_buffer_occupancy > cong_reg_def.med_cong_lvl_abate
             && new_buffer_occupancy < cong_reg_def.med_cong_lvl_onset )
    {
        if( ( old_cong_reg == 1 ) ||
                ( old_cong_reg == 2 ) )
        {
            *p_new_cong_reg = old_cong_reg;
        }

        else if( old_cong_reg == 3 )
        {
            /* Congestion region is MCR */
            *p_new_cong_reg = 2;
        }

        else if( old_cong_reg == 0 )
        {
            /* Congestion region is LCR */
            *p_new_cong_reg = 1;
        }

        return SCTP_SUCCESS;
    }

    else if( new_buffer_occupancy > cong_reg_def.high_cong_lvl_abate
             && new_buffer_occupancy < cong_reg_def.high_cong_lvl_onset )
    {
        if( ( old_cong_reg == 2 ) ||
                ( old_cong_reg == 3 ) )
        {
            *p_new_cong_reg = old_cong_reg;
        }

        else
        {
            /* Congestion region is MCR */
            *p_new_cong_reg = 2;
        }

        return SCTP_SUCCESS;
    }

    else
    {
        /* populate Error code */
        return SCTP_FAILURE;
    }
}
/***************************************************************************
 *    FUNCTION    : sctp_dynamic_ip_supported
 *
 *    DESCRIPTION : This function would set the SCTP restrict options in
 *                  SCTP association database.
 *    RETURN VALUE: SCTP_SUCCESS
 *                      SCTP_FALILURE
 ****************************************************************************/
sctp_return_t sctp_dynamic_ip_supported(
    sctp_tcb_st                 *p_ass,
    sctp_asconf_param_type_et   type )
{
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_dyn_ip_supported" ) );

#ifdef SCTP_DYNAMIC_IP_SUPPORT

    if( ( SCTP_SOCK_IS_LISTENER( p_ass->p_ep->flags ) ) ||
            SCTP_SOCK_STREAM != p_ass->p_ep->sk_type )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "can't do for listner/udp socket" ) );
        /*don;t support it for UDP style*/
        return ( SCTP_FAILURE );
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

    if( SCTP_TRUE == p_ass->restrict.asconf )
#else

    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    if( SCTP_TRUE == p_ass->sctp_restrict.asconf )
#endif
    {
        return ( SCTP_FAILURE );
    }

    switch( type )
    {
        case SCTP_ASCONF_PARAM_SET_PRIM:
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            if( SCTP_TRUE == p_ass->restrict.setprim )
#else

            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            if( SCTP_TRUE == p_ass->sctp_restrict.setprim )
#endif
            {
                return ( SCTP_FAILURE );
            }

            break;

        case SCTP_ASCONF_PARAM_ADD_IP:
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            if( SCTP_TRUE == p_ass->restrict.addip )
#else

            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            if( SCTP_TRUE == p_ass->sctp_restrict.addip )
#endif
            {
                return ( SCTP_FAILURE );
            }

        case SCTP_ASCONF_PARAM_DEL_IP:
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            if( SCTP_TRUE == p_ass->restrict.delip )
#else

            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            if( SCTP_TRUE == p_ass->sctp_restrict.delip )
#endif
            {
                return ( SCTP_FAILURE );
            }

            break;

    }

    return ( SCTP_SUCCESS );

#else

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ass );
    SCTP_UNUSED_VARIABLE( type );
    /*to avoid the warning*/

    return ( SCTP_FAILURE );
#endif
}


#ifdef SCTP_DYNAMIC_IP_SUPPORT

/***************************************************************************
 *    FUNCTION    : sctp_chk_and_send_asconf
 *
 *    DESCRIPTION : It is used to send data on an association. It is assumed
 *    that the  endpoint is locked at this moment before sending the data.
 *
 *    RETURN VALUE: SCTP_SUCCESS
 *                  SCTP_FALILURE
 ****************************************************************************/
sctp_return_t sctp_chk_and_send_asconf(
    sctp_tcb_st         *p_ass,
    sctp_bindx_flag_et  flags, /*currently unused*/
    sctp_Boolean_t      block,
    sctp_error_t        *p_ecode )
{
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_chk_and_send_asconf" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    /*to avoid the warning*/


    /*initialize the response array */
    if( SCTP_NULL_TIMER_ID !=
            p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ASCONF] )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;
        sk_errno = SK_EINPROGRESS;

        return ( SCTP_FAILURE );
    }

    else
    {
        sctp_U32bit sctp_index;
        sctp_U32bit num_v4 = 0, num_v6 = 0, rem_v4 = 0, rem_v6 = 0;

        /*
         * Check that if we are going to delete, it does not remove
         * all the addresses of a family
         */

        /*Count the number of address for each family*/
        for( sctp_index = 0; sctp_index < p_ass->p_ep->naddr; sctp_index++ )
        {
            if( SCTP_AF_INET == p_ass->p_ep->this_address[sctp_index].family )
            {
                num_v4++;
            }

            else
            {
                num_v6++;
            }
        }

        /*check the number of address in each family at peer*/
        for( sctp_index = 0; sctp_index < p_ass->transmit.num_remote_address; sctp_index++ )
        {
            if( SCTP_AF_INET == p_ass->transmit.remote[sctp_index].
                    that_address.family )
            {
                rem_v4++;
            }

            else
            {
                rem_v6++;
            }
        }

        for( sctp_index = 0; sctp_index < p_ass->cmd_asconf.num_params; sctp_index++ )
        {
            if( SCTP_ASCONF_PARAM_DEL_IP ==
                    p_ass->cmd_asconf.asconf_params[sctp_index].param_type )
            {
                if( SCTP_AF_INET == p_ass->cmd_asconf.asconf_params[sctp_index].
                        del_ip.ipaddr.family )
                {
                    num_v4--;
                }

                else
                {
                    num_v6--;
                }
            }

            p_ass->asconf_response[sctp_index] = SCTP_ASCONF_NORESPONSE;
        }

        if( ( ( 0 != rem_v4 ) && ( 0 == num_v4 ) ) ||
                ( ( 0 != rem_v6 ) && ( 0 == num_v6 ) ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                        ( "Trying to delete all Address of family" ) );
            *p_ecode = EINV_ADDRS;
            return ( SCTP_FAILURE );
        }

        sctp_assoc_state_machine( p_ass, SCTPEVENT_CMDASCONF );
    }

    if( block )
    {
        sctp_U32bit sctp_index;
        sctp_U32bit ep_id, assoc_id;

        ep_id = p_ass->p_ep->ep_id;
        assoc_id = p_ass->assoc_id;

        SCTP_SLEEP( p_ass->p_ep->asconf_cond, p_ass->p_ep->lock );

        p_ass = sctp_db_get_association_ptr( ep_id, assoc_id );

        if( !p_ass )
        {
            *p_ecode = EINV_ASSOCID;
            sk_errno = SK_ENOTCONN;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        EINV_ASSOCID, "send_asconf: Service User API ERROR" );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "send_asconf:Something happened to the ep while we were "
                          " blocked." ) );

            return ( SCTP_FAILURE );
        }

        for( sctp_index = 0; sctp_index < p_ass->cmd_asconf.num_params; sctp_index++ )
        {
            if( ( SCTP_ASCONF_SUCCESS != p_ass->asconf_response[sctp_index] ) &&
                    ( SCTP_ASCONF_IMPLIED_SUCCESS != p_ass->asconf_response[sctp_index] ) )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "send_asconf:one or more requests failed" ) );
                *p_ecode = EINV_PARAMS;
                sk_errno = SK_EINVAL;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

                if( p_ass->restrict.asconf )
#else

                /* CSR#1-4825810 Changes done for warning removal activity
                 * for compilation with SUN cc */
                if( p_ass->sctp_restrict.asconf )
#endif
                {
                    sk_errno = SK_EOPNOTSUPP;
                }

                return ( SCTP_FAILURE );
            }
        }
    }

    return ( SCTP_SUCCESS );
}



#endif



#ifdef SCTP_UI_SOCK_API
static size_t
sctp_send_iovec(
    sctp_tcb_st        *p_tcb,
    sctp_msghdr_st     *p_message,
    sctp_U32bit        addr_index,
    sctp_Boolean_t     ordered,
    sctp_U32bit        ppi,
    sctp_stream_t      stream,
    sctp_U32bit        context,
    sctp_U32bit        timtolive );

static int
sctp_receive_blocking_ext(
    sctp_tcb_st        *p_tcb,
    sctp_iovec_st      *p_iovec,
    sctp_U32bit        iov_len,
    sctp_U32bit        *p_msg_len,
    sctp_U32bit        flags,
    sctp_sockaddr_st   *p_daddr,
    sctp_sndrcvinfo_st *p_sndrcv,
    sctp_U32bit        *p_msg_flags,
    sctp_Boolean_t     block,
    sctp_error_t       *p_ecode );

static int
sctp_set_paddrparams(
    sctp_tcb_st        *p_tcb,
    const void         *p_args );

static int
sctp_get_paddrparams(
    sctp_tcb_st        *p_tcb,
    void               *p_args );

static int
sctp_get_paddrinfo(
    sctp_tcb_st        *p_tcb,
    void               *p_args );


/***************************************************************************
 *    FUNCTION    : sctp_connect_udp
 *
 *    DESCRIPTION : It is used to do a UDP style connect.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_connect_udp(
    sctp_ep_st        *p_ep,
    sctp_sockaddr_st  *p_daddr,
    sctp_port_t       dport,
    int               num_addrs )
{
    sctp_port_t      port = 0;
    sctp_error_t     s_err;
    sctp_tcb_st      *p_tcb = SCTP_NULL;
    int              ret = SK_FAILURE;

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_connect_udp" ) );

    /*
     * In case n_assoc in the endpoint table is > 1 then we need to see that
     * whether the endpoint already has a connection to the said destination.
     */
    if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_ep, dport, num_addrs, p_daddr,
                                              &p_tcb, &s_err ) )
    {
        /*
         * just return from here in udp style
         * no need to check more
         */
        return ( SK_SUCCESS );
    }

    /*
     * Check that the endpoint is bound. If not bind to the local addr list.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        /* Get a free port */
        if( !sctp_db_get_free_sctpport( &port ) )
        {
            /* We should have a better error code but we don't */
            sk_errno = SK_EADDRNOTAVAIL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "sctp_connect_udp: Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

        /*
         * Bind the endpoint with the local addresses of the same type.
         */
        if( !sctp_db_bind_ep_with_af_check( p_ep->ep_id, port, &s_err ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "sctp_connect_udp: Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

        if( !sctp_db_add_hash( SCTP_LISTEN_HASH, p_ep ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sendmsg: Could not add the ep to the listen hash....." ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

        p_ep->this_port = port;
        /* Mark the socket as bound */
        SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );

        /* Mark the socket as a listener. */
        SCTP_SOCK_SET_LISTENER( p_ep->flags, SCTP_TRUE );

        /* Set the Socket Default backlog queue */
        p_ep->l_backlog = SCTP_MAX_LISTEN_BACKLOG;
    }

    /* Socket is bound Now */
    /*
     * Check that the number of already existing connections do not exceed the
     * MAX backlog length.
     */
    if( p_ep->n_assoc >= SCTP_MAX_BACKLOG )
    {
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * In case n_assoc in the endpoint table is > 1 then we need to see that
     * whether the endpoint already has a connection to the said destination.
     */
    if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_ep, dport, num_addrs, p_daddr,
                                              &p_tcb, &s_err ) )
    {
        /*
         * The Endpoint already has a connection to the said destination.
         * Check the connection state.
         */
        if( SCTPSTATE_ESTABLISHED == p_tcb->state )
        {
            sk_errno = SK_EISCONN;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "sctp_connect_tcp: Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

        else
        {
            sk_errno = SK_EINPROGRESS;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "sctp_connect_tcp: Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }
    }

    /*
     * Check that the address that we're connecting to is connect'able.
     */
    if( !sctp_db_addr_connectable( p_ep, dport, num_addrs, p_daddr, &s_err ) )
    {
        sk_errno = SK_EADDRINUSE;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address is not Connectable" ) );

        return ( SK_FAILURE );
    }

    /*
     * Now we have to connect: But before we do so we need to make a new
     * TCB and attach the TCB to the endpoint.
     */
    p_tcb = sctp_db_create_tcb( p_ep, dport, num_addrs, p_daddr, &s_err );

    if( SCTP_NULL == p_tcb )
    {
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Check whether the user wanted to invoke blocking connect!!!!!
     * Check whether the blocking flag is set for the endpoint.
     */
    if( SCTP_SOCK_IS_BLOCKING( p_ep->flags ) )
    {
        /* The Socket is blocking type */
        ret = sctp_connect_blocking( p_tcb );
    }

    else
    {
        /* The Socket is non blocking type */
        /* Invoke the FSM to connect: This shall send the init */
        sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDCONNECT );
        ret = SK_SUCCESS;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_connect_tcp
 *
 *    DESCRIPTION : It is used to do a TCP style connect.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_connect_tcp(
    sctp_ep_st        *p_ep,
    sctp_sockaddr_st  *p_daddr,
    sctp_port_t       dport,
    int               num_addrs )
{
    sctp_error_t     s_err;
    sctp_tcb_st      *p_tcb = SCTP_NULL;
    int              ret  = SK_FAILURE;

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_connect_tcp" ) );

    /*
     * Check that the endpoint is bound. If not return error.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        sctp_port_t sctp_port;
        /*
         * Here socket is not bound so first we have to bind it.
         * we select a unused port and copy it to ep .
         */
        ret  = sctp_db_get_free_sctpport( &sctp_port );

        if( ret == SCTP_FAILURE )
        {
            /* No free port*/
            sk_errno = SK_EADDRNOTAVAIL;

            return ( SCTP_FAILURE );
        }

        p_ep->this_port = sctp_port;

        /* Set the local addresses */
        if( !sctp_db_bind_ep_with_af_check( p_ep->ep_id, sctp_port, &s_err ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        SK_EINVAL, "Service User API ERROR" );

            return ( SCTP_FAILURE );
        }

        /* Set the socket as bound */
        SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );
        sctp_db_add_hash( SCTP_BIND_HASH, p_ep );
    }

    /*
     * In case n_assoc in the endpoint table is > 1 then we need to see that
     * whether the endpoint already has a connection to the said destination.
     */
    if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_ep, dport, num_addrs, p_daddr,
                                              &p_tcb, &s_err ) )
    {
        /*
         * The Endpoint already has a connection to the said destination.
         * Check the connection state.
         */
        if( SCTPSTATE_ESTABLISHED == p_tcb->state )
        {
            sk_errno = SK_EISCONN;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "sctp_connect_tcp: Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

        else
        {
            sk_errno = SK_EINPROGRESS;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "sctp_connect_tcp: Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }
    }

    /*
     * We reach here it means that we have a connection existing. return error.
     */
    if( p_ep->n_assoc >= 1 )
    {
        sk_errno = SK_EISCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Check that the address that we're connecting to is connect'able.
     */
    if( !sctp_db_addr_connectable( p_ep, dport, num_addrs, p_daddr, &s_err ) )
    {
        sk_errno = SK_EADDRINUSE;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address is not Connectable" ) );

        return ( SK_FAILURE );
    }

    /*
     * Now we have to connect: But before we do so we need to make a new
     * TCB and attach the TCB to the endpoint.
     */
    p_tcb = sctp_db_create_tcb( p_ep, dport, num_addrs, p_daddr, &s_err );

    if( SCTP_NULL == p_tcb )
    {
        sk_errno = SK_ENOBUFS;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Check whether the user wanted to invoke blocking connect!!!!!
     * Check whether the blocking flag is set for the endpoint.
     */
    if( SCTP_SOCK_IS_BLOCKING( p_ep->flags ) )
    {
        /* The Socket is blocking type */
        ret = sctp_connect_blocking( p_tcb );
    }

    else
    {
        /* The Socket is non blocking type */
        /* Invoke the FSM to connect: This shall send the init */
        sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDCONNECT );
        ret = SK_SUCCESS;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_connect_blocking
 *
 *    DESCRIPTION : It is used to do a blocking connect to a destination.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_connect_blocking(
    sctp_tcb_st      *p_tcb )
{
    sctp_U32bit    ep_id    = p_tcb->p_ep->ep_id;
    sctp_ep_st     *p_ep    = SCTP_NULL;
    sctp_assoc_t   assoc_id = p_tcb->assoc_id;

    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC, ( "sctp_connect_blocking" ) );

    /* Invoke the FSM to connect: This shall send the init */
    sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDCONNECT );

    /* Put the Current Thread to sleep Till we get a response. */
    SCTP_SLEEP( p_tcb->p_ep->cond, p_tcb->p_ep->lock );

    /*
     * Woken up after attempting a blocking connect. Now ...
     * 1. either we're connected.
     * 2. or we failed to connect successfully.
     * Return an appropriate value in either case.
     */
    p_ep = sctp_db_get_ep_ptr( ep_id );

    if( !p_ep )
    {
        sk_errno = SK_ENOTSOCK;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_blocking: Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_connect_blocking:Something happened to the ep while we were "
                      " blocked." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Try to a get a pointer for the assoc "assoc_id" inside the endpoint
     */
    if( !sctp_db_get_association_ptr( p_ep->ep_id, assoc_id ) )
    {
        /* SPR 6744 */
        sk_errno = SK_ECONNREFUSED;
        /* SPR 6744 */

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_blocking: Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_connect_blocking:Couldnot get assoc within the ep blocked." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /*
     * Now that we have the association for which we initiated the connect
     * check its state to see whether actually we succeded in the connect.
     */
    if( SCTPSTATE_ESTABLISHED != p_tcb->state )
    {
        sk_errno = SK_ECONNREFUSED;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_connect_blocking: Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_connect_blocking: Connect failed." ) );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Changes For CSR#1-4619243 */
        return ( SK_FAILURE );
        /* Changes For CSR#1-4619243 */

    }

    /* Successfully connected to the destination */

    return ( SK_SUCCESS );
}

/***************************************************************************
 *    FUNCTION    : sctp_receive_blocking
 *
 *    DESCRIPTION : It is used to do a blocking receive.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_receive_blocking(
    sctp_tcb_st      *p_tcb,
    sctp_U8bit       *p_buffer,
    sctp_U32bit      length,
    sctp_U32bit      *p_msg_len,
    sctp_U32bit      flags,
    sctp_Boolean_t   block )
{
    sctp_U32bit    counter;
    /*Bug Id 22 fix starts*/
    /*sctp_return_t  ret = SK_FAILURE;*/
    int ret = SK_FAILURE;
    /*Bug Id 22 fix ends*/

    sctp_stream_t  stream_id;
    /* start spr 21157 */
    sctp_error_t   s_err = E_NOERROR;
    /* end spr 21157  */
    sctp_U32bit    ep_id;
    sctp_U32bit    assoc_id;
    sctp_ep_st     *p_ep = SCTP_NULL;
    sctp_U32bit    ppi = p_tcb->p_ep->ppi;
    sctp_U32bit    tmp32 = 0;
    sctp_Boolean_t ordered = SCTP_TRUE;
    sctp_sockaddr_st    sockaddr;
    sctp_Boolean_t partial_data_delv = SCTP_FALSE;
    sctp_iovec_st  p_iovec[1];

    p_iovec->iov_base = ( char * )p_buffer;
    p_iovec->iov_len  = length;

    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC, ( "sctp_receive_blocking" ) );

    for( ; ; )
    {
        for( counter = 0; counter < p_tcb->receive.num_in_streams; counter++ )
        {
            ret = sctp_read_message_list_ext( p_tcb,
                                              ( sctp_U16bit )counter,
                                              1,
                                              ( sctp_U8bit * )p_iovec,
                                              p_msg_len,
                                              &ppi,
                                              flags | MSG_TRUNC, /* Truncate the message by default */
                                              &sockaddr,
                                              &tmp32,
                                              &tmp32,
                                              &tmp32,
                                              &ordered,
                                              &partial_data_delv,
                                              &s_err );

            /* Variable nextssn is incremented by above called function */
            if( ret == SCTP_SUCCESS )
            {
                stream_id = ( sctp_U16bit )counter;

                /*
                 * Check if the if any partial indication has been given
                 * data being delivered previously. If yes, then clear it...
                 */

                if( ( p_tcb->partial_data.partial_data_pending == 1 ) &&
                        ( p_tcb->partial_data.stream == stream_id ) &&
                        ( p_tcb->partial_data.ssn ==
                          p_tcb->receive.stream[stream_id].nextssn - 1 ) )
                {
                    p_tcb->partial_data.partial_data_pending = 0;
                }

                /*SPR 4929*/
                sctp_db_clear_data_ind( p_tcb, 1 );
                sctp_db_update_readable_status( p_tcb->p_ep );
                /*SPR 4929*/

                return ( SK_SUCCESS );
            }
        }

        /* Check if its a blocking receive */
        if( block )
        {
            switch( s_err )
            {
                case E_NOTHING_IN_QUEUE:
                    /* Store the ep_id for later */
                    ep_id    = p_tcb->p_ep->ep_id;
                    assoc_id = p_tcb->assoc_id;

                    /* Sleep till we have something to read */
                    SCTP_SLEEP( p_tcb->p_ep->cond, p_tcb->p_ep->lock );

                    /* Get the ptr again */
                    p_ep = sctp_db_get_ep_ptr( ep_id );

                    if( p_ep )
                    {
                        p_tcb = sctp_db_get_association_ptr( ep_id, assoc_id );
                    }

                    if( !p_ep || !p_tcb )
                    {
                        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                    EINV_ASSOCID, "sctp_receive_blocking:Service User API ERROR" );

                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "sctp_receive_blocking: Association deleted." ) );

                        s_err = E_ASSOC_DELETED;
                        sk_errno = SK_ECONNRESET;

                        return ( SK_FAILURE );
                    }

                    if( ( p_tcb->state  == SCTPSTATE_CLOSED )
                            || ( p_tcb->state == SCTPSTATE_COOKIEWAIT )
                            || ( p_tcb->state == SCTPSTATE_COOKIESENT ) )
                    {
                        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                    E_REQ_IN_WRONG_STATE, "sctp_receive_blocking: Service User"
                                    " API ERROR" );

                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,  1 );
                        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "sctp_receive_blocking:API Requested in Invalid State " ) );

                        s_err = E_REQ_IN_WRONG_STATE;
                        sk_errno = SK_ENOTCONN;

                        /* Do a review of the readable staus of the endpoint. */
                        sctp_db_update_readable_status( p_tcb->p_ep );

                        return ( SK_FAILURE );
                    }

                    break;

                default:
                    sk_errno = SK_EAGAIN;
                    return SK_FAILURE;
            }
        }

        else
        {
            break;
        }
    }

    /* Do a review of the readable staus of the endpoint. */
    sctp_db_update_readable_status( p_tcb->p_ep );

    /* No data available for read */
    return ( SK_FAILURE );
}


/***************************************************************************
 *    FUNCTION    : sctp_receive_blocking_ext
 *
 *    DESCRIPTION : It is used to do a blocking receive and returns some
 *    additional information.
 *
 *    RETURN VALUE: On success returns the number of bytes read and returns
 *    SK_FAILURE on Failure. If a notification is pending, returns SK_SUCCESS
 ****************************************************************************/
static int
sctp_receive_blocking_ext(
    sctp_tcb_st        *p_tcb,
    sctp_iovec_st      *p_iovec,
    sctp_U32bit        iov_len,
    sctp_U32bit        *p_msg_len,
    sctp_U32bit        flags,
    sctp_sockaddr_st   *p_daddr,
    sctp_sndrcvinfo_st *p_sndrcv,
    sctp_U32bit        *p_msg_flags,
    sctp_Boolean_t     block,
    sctp_error_t       *p_ecode )
{
    sctp_U32bit    counter;
    sctp_return_t  ret;
    sctp_stream_t  stream_id;
    sctp_U32bit    ep_id;
    sctp_ep_st     *p_ep = SCTP_NULL;
    sctp_U32bit    assoc_id;
    sctp_U32bit    ssn, tsn;
    sctp_U32bit    context;
    sctp_Boolean_t unordered = SCTP_FALSE;
    sctp_U32bit    ppi = p_tcb->p_ep->ppi;
    sctp_Boolean_t partial_data_delv = SCTP_FALSE;

    SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC, ( "sctp_receive_blocking_ext" ) );

    if( ( p_iovec->iov_base == SCTP_NULL ) || ( p_iovec->iov_len == 0 ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                    ( "sctp_receive_blocking_ext:null buffer\n" ) );
        sk_errno = SK_EINVAL;
        return ( SK_FAILURE );
    }

    for( ; ; )
    {
        for( counter = 0; counter < p_tcb->receive.num_in_streams; counter++ )
        {
            ret = sctp_read_message_list_ext( p_tcb,
                                              ( sctp_U16bit )counter,
                                              iov_len,
                                              ( sctp_U8bit * ) p_iovec,
                                              p_msg_len,
                                              &ppi,
                                              flags,
                                              p_daddr,
                                              &( ssn ),
                                              &( tsn ),
                                              &( context ),
                                              &( unordered ),
                                              &( partial_data_delv ),
                                              p_ecode );

            /* Variable nextssn is incremented by above called function */
            if( ret == SCTP_SUCCESS )
            {
                stream_id = ( sctp_U16bit )counter;

                /*
                 * Check if the if any partial indication has been given
                 * data being delivered previously. If yes, then clear it...
                 */

                if( ( p_tcb->partial_data.partial_data_pending == 1 ) &&
                        ( p_tcb->partial_data.stream == stream_id ) &&
                        ( p_tcb->partial_data.ssn ==
                          p_tcb->receive.stream[stream_id].nextssn - 1 ) )
                {
                    p_tcb->partial_data.partial_data_pending = 0;
                }

                /* Fill up the snd rcv info */
                if( p_sndrcv )
                {
                    p_sndrcv->sinfo_stream   = stream_id;
                    p_sndrcv->sinfo_assoc_id = p_tcb->assoc_id;
                    p_sndrcv->sinfo_ppid     = ppi;
                    p_sndrcv->sinfo_ssn      = ssn;
                    p_sndrcv->sinfo_context  = context;
                    p_sndrcv->sinfo_flags    = 0;
                    p_sndrcv->sinfo_tsn      = tsn;

                    /* Set the unordered flags */
                    if( unordered )
                    {
                        p_sndrcv->sinfo_flags |= SCTP_MSG_UNORDERED;
                    }

                    /*
                     * If the complete data has been read, then set the flag to
                     * indicate end of message.
                     */
                    if( !( partial_data_delv ) )
                    {
                        *p_msg_flags |= MSG_EOR;
                    }
                }

                return ( *p_msg_len );
            }
        }

        /* Check if its a blocking receive */
        if( block )
        {
            switch( *p_ecode )
            {
                case E_NOTHING_IN_QUEUE:
                    /* Store the ep_id for later */
                    ep_id    = p_tcb->p_ep->ep_id;
                    assoc_id = p_tcb->assoc_id;

                    /* Sleep till we have something to read */
                    SCTP_SLEEP( p_tcb->p_ep->cond, p_tcb->p_ep->lock );

                    /* Get the ptr again */
                    p_ep = sctp_db_get_ep_ptr( ep_id );

                    if( p_ep )
                    {
                        /*SPR 5048*/
                        /*check for any notifications*/
                        if( ( flags & SCTP_MSG_NOTIFICATION ) &&
                                ( p_ep->ulpqueue.n_ind > 0 ) )
                        {
                            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                                        ( "sctp_receive_blocking_ext:notification pending" ) );
                            return ( SK_SUCCESS );
                        }

                        /*SPR 5048*/

                        p_tcb = sctp_db_get_association_ptr( ep_id, assoc_id );
                    }

                    if( !p_ep || !p_tcb )
                    {
                        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                    EINV_ASSOCID, "sctp_receive_blockign_ext: "
                                    "Service User API ERROR" );

                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "sctp_receive_blocking_ext: Association deleted." ) );

                        *p_ecode = E_ASSOC_DELETED;
                        sk_errno = SK_ECONNRESET;

                        return ( SK_FAILURE );
                    }

                    if( ( p_tcb->state  == SCTPSTATE_CLOSED )
                            || ( p_tcb->state == SCTPSTATE_COOKIEWAIT )
                            || ( p_tcb->state == SCTPSTATE_COOKIESENT ) )
                    {
                        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                    E_REQ_IN_WRONG_STATE, "sctp_receive_blocking_ext: "
                                    "Service User API ERROR" );

                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,  1 );
                        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "sctp_receive_blocking_ext: API Requested in Inv State " ) );

                        *p_ecode = E_REQ_IN_WRONG_STATE;
                        sk_errno = SK_ENOTCONN;

                        /* Do a review of the readable staus of the endpoint. */
                        sctp_db_update_readable_status( p_tcb->p_ep );

                        return ( SK_FAILURE );
                    }

                    /*SPR 4937*/
                    break;

                    /*SPR 4937*/
                default:
                    sk_errno = SK_EAGAIN;
                    return SK_FAILURE;

            }
        }

        else
        {
            break;
        }
    }

    sk_errno = SK_EAGAIN;

    /* Do a review of the readable staus of the endpoint. */
    sctp_db_update_readable_status( p_tcb->p_ep );

    /* No data available for read */
    return ( SK_FAILURE );
}


/***************************************************************************
 *    FUNCTION    : sctp_setsockopt_sctp
 *
 *    DESCRIPTION : It is used to set the socket option at the SCTP level.
 *    i.e. for TCP and server sockets it will be same as at socket level.
 *         for UDP it shall be for individual associtions.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_setsockopt_sctp(
    sctp_ep_st            *p_ep,
    int                   optname,
    const void            *optval,
    sctp_socklen_t         optlen )
{
    sctp_assoc_t assoc_id = SCTP_INVALID_ASSOC;
    int          ret      = SK_SUCCESS;
    sctp_tcb_st  *p_tcb   = SCTP_NULL;
    sctp_prot_params_st   *p_config = NULL;
    sctp_Boolean_t assoc_or_ep = SCTP_FALSE;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( optlen );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_setsockopt_sctp" ) );

    switch( optname )
    {

        case SCTP_SO_RCVBUF:
            {
                sctp_error_t    ecode;

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    /*Now we can do it for udp also*/
                    if( SK_FAILURE == sctp_set_rcv_snd_sockopt_udp_sctp( p_ep, 1, optval ) )
                    {
                        sk_errno = SK_EINVAL;
                        ret = SK_FAILURE;

                    }
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

                    if( !p_tcb )
                    {
                        ret = SK_FAILURE;
                        break;
                    }

                    if( SCTP_FAILURE == sctp_db_set_rx_buffers( p_tcb, *( ( int * )optval ), &ecode ) )
                    {

                        sk_errno = SK_EINVAL;
                        ret = SK_FAILURE;

                    }
                }
            }
            break;

        case SCTP_SO_SNDBUF:
            {
                sctp_error_t    ecode;

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    /*Now we can do it for udp also*/
                    if( SK_FAILURE == sctp_set_rcv_snd_sockopt_udp_sctp( p_ep, 0, optval ) )
                    {
                        sk_errno = SK_EINVAL;
                        ret = SK_FAILURE;

                    }

                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

                    if( !p_tcb )
                    {
                        sk_errno = SK_ENOTCONN;
                        ret = SK_FAILURE;
                        break;
                    }

                    if( SCTP_FAILURE == sctp_db_set_tx_buffers( p_tcb, *( ( int * )optval ),
                                                                &ecode ) )
                    {
                        sk_errno = SK_EINVAL;
                        ret = SK_FAILURE;
                    }
                }
            }
            break;

        case SCTP_SO_RTOINFO:
            {
                sctp_rtoinfo_st *p_info = ( sctp_rtoinfo_st * )optval;
                sctp_U32bit  rto_init, rto_max, rto_min;

                /* Tune RTO parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->srto_assoc_id;

                    p_tcb =
                        sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                p_config = ( ( p_tcb ) ? & ( p_tcb->config ) : & ( p_ep->sk_config ) );

                if( p_info->srto_initial != 0 )
                {
                    rto_init = p_info->srto_initial;
                }

                else
                {
                    rto_init = p_config->rto_init;
                }

                if( p_info->srto_max != 0 )
                {
                    rto_max = p_info->srto_max;
                }

                else
                {
                    rto_max = p_config->rto_max;
                }

                if( p_info->srto_min != 0 )
                {
                    rto_min = p_info->srto_min;
                }

                else
                {
                    rto_min = p_config->rto_min;
                }

                if( ( rto_min > rto_init ) || ( rto_min > rto_max ) ||
                        ( rto_init > rto_max ) )
                {
                    sk_errno = SK_EINVAL;
                    SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                sk_errno, "sctp_setsockopt_sctp: Service User API ERROR" );

                    SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "Invalid Values of Parameters " ) );

                    ret = SK_FAILURE;
                    break;
                }

                /* Copy the RTO info into the assoc structure */

                p_config->rto_init = rto_init;
                p_config->rto_min  = rto_min;
                p_config->rto_max  = rto_max;
            }
            break;

        case SCTP_SO_ASSOCINFO:
            {
                sctp_assocparams_st *p_info = ( sctp_assocparams_st * )optval;
                sctp_U32bit  path_maxrtx = 0;

                /* Tune Assoc parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->sasoc_assoc_id;

                    p_tcb =
                        sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                p_config = ( ( p_tcb ) ? & ( p_tcb->config ) : & ( p_ep->sk_config ) );

                if( p_info->sasoc_asocmaxrxt > 0 )
                {
                    /*check that it is not greater than sum of all path maxrxt */
                    /*SPR 21072 start*/
                    /*Code Deleted*/
                    path_maxrtx = p_config->path_max_retrans;

                    if( path_maxrtx >  p_info->sasoc_asocmaxrxt )
                        /*SPR 21072 stop*/
                    {
                        sk_errno = SK_EINVAL;
                        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                    sk_errno, "sctp_setsockopt_sctp: Service User API ERROR" );

                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "Invalid Values of Parameters, assoc maxrxt less than path's" ) );

                        ret = SK_FAILURE;
                        break;
                    }

                    p_config->assoc_max_retrans = p_info->sasoc_asocmaxrxt;
                }

                if( p_info->sasoc_cookie_life > 0 )
                {
                    p_config->valid_cookie_life =
                        p_info->sasoc_cookie_life;
                }
            }
            break;

        case SCTP_SO_INITMSG:
            {
                sctp_initmsg_st *p_info = ( sctp_initmsg_st * )optval;

                if( ( p_info->sinit_max_instreams > sctp_db_main.init_in_streams )
                        || ( p_info->sinit_num_ostreams >
                             sctp_db_main.init_out_streams ) )
                {
                    sk_errno = SK_EINVAL;
                    SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                sk_errno, "sctp_setsockopt_sctp: Service User API ERROR" );

                    SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "Invalid Values of Parameters " ) );

                    ret = SK_FAILURE;
                    break;
                }

                /* Tune Init parameters */
                if( p_info->sinit_max_instreams > 0 )
                {
                    p_ep->sk_config.num_in_streams =
                        p_info->sinit_max_instreams;
                }

                if( p_info->sinit_num_ostreams > 0 )
                {
                    p_ep->sk_config.num_out_streams =
                        p_info->sinit_num_ostreams;
                }

                if( p_info->sinit_max_attempts > 0 )
                {
                    p_ep->sk_config.max_init_retrans =
                        p_info->sinit_max_attempts;
                }

                p_ep->sk_config.init_timeo = p_info->sinit_max_init_timeo;
            }
            break;

        case SCTP_SO_PRIMARY_ADDR:
            {
                /*SPR 5079*/
                sctp_setprim_st *p_info = ( sctp_setprim_st * )optval;
                /*SPR 5079*/
                sctp_sockaddr_st    sock_addr;
                sctp_port_t         port;
                sctp_U32bit         sctp_index;

                /* SPR : 18527 */
                sock_addr.family = 0;
                /* SPR : 18527 */

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->ssp_assoc_id;

                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    sk_errno = SK_ENOTCONN;
                    ret = SK_FAILURE;

                    break;
                }

                /*
                * Check that the address passed is valid.
                * Convert the address from sockaddr_storage to sctp_sockaddr_st
                * check that the address is part of the endpoint and get the
                * Index of the address in the addr list. Set this address in
                * the transmit structure.
                */
                /* SPR 21107 changes
                /sctp_conv_addr_os_to_sctp(1, (struct sockaddr *)&(p_info->ssp_addr), &sock_addr,
                &port, SCTP_NULL);*/
                if( SCTP_FAILURE ==
                        sctp_conv_addr_os_to_sctp( 1, ( struct sockaddr * ) & ( p_info->ssp_addr ), &sock_addr,
                                                   &port, SCTP_NULL ) )
                {
                    sk_errno = SK_EINVAL;

                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "sctp_sendmsg_tcp: couldnot convert address " ) );

                    return ( SK_FAILURE );
                    /* SPR 21107 changes */
                }

                if( SCTP_FAILURE ==
                        sctp_db_validate_local_addr( p_tcb->p_ep, &sock_addr, &sctp_index ) )
                {
                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                                ( "sctp_setsockopt_sctp: Couldnot get address index" ) );

                    sk_errno = SK_ENOPROTOOPT;
                    ret = SK_FAILURE;
                    break;
                }

                /*asscoiation exists */
                if( SCTP_FAILURE == sctp_dynamic_ip_supported( p_tcb,
                                                               SCTP_ASCONF_PARAM_SET_PRIM ) )
                {
                    sk_errno = SK_EOPNOTSUPP;

                    SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                sk_errno, "Service User API ERROR" );

                    SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "Set primary operation not supported" ) );
                    ret = SK_FAILURE;

                    break;
                }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

                else
                {
                    sctp_asconf_setprim_param_st  *p_set_prim =
                        &p_tcb->cmd_asconf.asconf_params[0].set_primary;
                    sctp_error_t    s_err;
                    p_tcb->cmd_asconf.num_params = 1;
                    p_set_prim->corr_id = 0;
                    p_set_prim->ipaddr = sock_addr;
                    p_set_prim->param_type = SCTP_ASCONF_PARAM_SET_PRIM;

                    /*SPR 5079*/
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

                    if( SCTP_FAILURE == sctp_chk_and_send_asconf( p_tcb,
                                                                  SCTP_ASCONF_PARAM_SET_PRIM,
                                                                  SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &s_err ) )
#else

                    /* CSR#1-4825810 Changes done for warning removal activity
                     * for compilation with SUN cc */
                    if( SCTP_FAILURE == sctp_chk_and_send_asconf( p_tcb,
                                                                  ( sctp_bindx_flag_et ) SCTP_ASCONF_PARAM_SET_PRIM,
                                                                  SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &s_err ) )
#endif
                    {
                        ret = SK_FAILURE;
                    }

                    /*SPR 5079*/
                }

#endif
            }
            break;

        case SCTP_SO_PEER_PRIMARY_ADDR:
            {
                sctp_setpeerprim_st  *p_info = ( sctp_setpeerprim_st * )optval;
                sctp_U32bit          sctp_index   = 0;
                sctp_port_t          port    = 0;
                sctp_sockaddr_st     sock_addr;

                /* SPR : 18527 */
                sock_addr.family = 0;
                /* SPR : 18527 */

                /* Tune Assoc parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->sspp_assoc_id;

                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( SCTP_NULL == p_tcb )
                {
                    sk_errno = SK_ENOTCONN;
                    ret = SK_FAILURE;

                    break;
                }

                /*
                * Check that the address passed is valid.
                * Convert the address from sockaddr_storage to sctp_sockaddr_st
                * check that the address is part of the endpoint and get the
                * Index of the address in the addr list. Set this address in
                * the transmit structure.
                */

                sctp_conv_addr_storg_os_to_sctp( 1, &( p_info->sspp_addr ), &sock_addr,
                                                 &port, SCTP_NULL );

                if( SCTP_FAILURE ==
                        sctp_db_validate_remote_addr( p_tcb, &sock_addr, &sctp_index ) )
                {
                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                                ( "sctp_setsockopt_sctp: Couldnot get address index" ) );

                    sk_errno = SK_EINVAL;
                    ret = SK_FAILURE;
                    break;
                }

                if( p_tcb->transmit.primary_address_index
                        == p_tcb->transmit.configured_primary_path_index )
                {
                    p_tcb->transmit.primary_address_index =  sctp_index;
                    p_tcb->transmit.configured_primary_path_index = sctp_index;
                }

                else
                {
                    p_tcb->transmit.configured_primary_path_index = sctp_index;
                }
            }
            break;

        case SCTP_SO_NODELAY:
            {
                /*
                * This works like disabling bundling. If u set the option
                * NODELAY then the packets are sent as soon as possible
                * without any delay.
                    */
                sctp_S32bit on = *( ( int * )optval );

                if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    SCTP_SOCK_SET_NODELAY( p_ep->flags, ( on ? 1 : 0 ) );
                }

                else
                {
                    p_tcb->config.bundling_time =
                        ( on ? 0 : p_ep->sk_config.bundling_time );
                }
            }
            break;

        case SCTP_SO_AUTOCLOSE:
            {
                /* Only valid for UDP style sockets. */
                sctp_S32bit on = *( ( int * )optval );

                /* Set the AUTO Close flag if UDP style socket. */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    /*
                     * If on != 0 then set the autoclose option. else
                     * autoclose is disabled.
                     */
                    if( on > 0 )
                    {
                        SCTP_SOCK_SET_AUTOCLOSE( p_ep->flags, SCTP_TRUE );

                        /* Save the Auto close timeout field. */
                        p_ep->aclose_time = on;
                    }

                    else if( on == 0 )
                    {
                        SCTP_SOCK_SET_AUTOCLOSE( p_ep->flags, SCTP_FALSE );
                        p_ep->aclose_time = 0;
                    }

                    else
                    {
                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "Invalid Values of Parameters " ) );

                        sk_errno = SK_EINVAL;
                        ret      = SK_FAILURE;
                    }
                }

                else
                {
                    sk_errno = SK_ENOPROTOOPT;
                    ret      = SK_FAILURE;
                }
            }
            break;

        case SCTP_SO_SET_EVENTS:
            {
                sctp_event_subscribe_st *p_info =
                    ( sctp_event_subscribe_st * )optval;

                SCTP_SOCK_SET_RECVDATAIO( p_ep->ulpqueue.ntfy_flags,
                                          ( p_info->sctp_data_io_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVASSOC( p_ep->ulpqueue.ntfy_flags,
                                         ( p_info->sctp_association_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVPADDR( p_ep->ulpqueue.ntfy_flags,
                                         ( p_info->sctp_address_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVSENDFAIL( p_ep->ulpqueue.ntfy_flags,
                                            ( p_info->sctp_send_failure_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVPEERERR( p_ep->ulpqueue.ntfy_flags,
                                           ( p_info->sctp_peer_error_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVSHUTDOWN( p_ep->ulpqueue.ntfy_flags,
                                            ( p_info->sctp_shutdown_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVADAPTION( p_ep->ulpqueue.ntfy_flags,
                                            ( p_info->sctp_adaption_layer_event ? 1 : 0 ) );

                SCTP_SOCK_SET_RECVPARTDELIVERY( p_ep->ulpqueue.ntfy_flags,
                                                ( p_info->sctp_partial_delivery_event ? 1 : 0 ) );
            }
            break;

        case SCTP_PEER_ADDR_PARAMS:
            {
                sctp_paddrparams_st *p_info = ( sctp_paddrparams_st * )optval;

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->spp_assoc_id;

                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( SCTP_NULL == p_tcb )
                {
                    sk_errno = SK_ENOTCONN;
                    ret = SK_FAILURE;
                    break;
                }

                ret = sctp_set_paddrparams( p_tcb, optval );
            }
            break;

        case SCTP_SO_SET_ADAPTION_LAYER:
            {
                sctp_setadaption_st *p_ind = ( sctp_setadaption_st * )optval;

                p_ep->sk_config.adaption_ind = p_ind->ssb_adaption_ind;
            }
            break;

        case SCTP_SO_DISABLE_FRAGMENTS:
            {
                sctp_S32bit on = *( ( int * )optval );

                if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

                    if( !p_tcb )
                    {
                        /*Bug Id 22 fix starts*/
                        /*p_ep->sk_config.dont_fragment = on?1:0;*/
                        p_ep->sk_config.dont_fragment = on ? SCTP_TRUE : SCTP_FALSE;
                        /*Bug Id 22 fix ends*/

                        break;
                    }

                    /*Bug Id 22 fix starts*/
                    /*p_tcb->config.dont_fragment = on?1:0;*/
                    p_tcb->config.dont_fragment = on ? SCTP_TRUE : SCTP_FALSE;
                    /*Bug Id 22 fix ends*/

                }

                else
                {
                    for( p_tcb = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );
                            ( p_tcb != SCTP_NULL );
                            p_tcb = ( sctp_tcb_st * )lstNext( ( NODE * )p_tcb ) )
                    {
                        /*Bug Id 22 fix starts*/
                        /*p_tcb->config.dont_fragment = on?1:0;*/
                        p_tcb->config.dont_fragment = on ? SCTP_TRUE : SCTP_FALSE;
                        /*Bug Id 22 fix ends*/
                        assoc_or_ep = SCTP_TRUE;
                    }

                    if( !assoc_or_ep )
                    {
                        /*Bug Id 22 fix starts*/
                        /*p_ep->sk_config.dont_fragment = on?1:0;*/
                        p_ep->sk_config.dont_fragment = on ? SCTP_TRUE : SCTP_FALSE;
                        /*Bug Id 22 fix ends*/

                    }

                }
            }
            break;

        case SCTP_SO_DEFAULT_SEND_PARAM:
            {
                sctp_sndrcvinfo_st *p_info = ( sctp_sndrcvinfo_st * )optval;
                sctp_U32bit max_streams = sctp_db_main.init_out_streams;

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->sinfo_assoc_id;

                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( p_tcb )
                {
                    p_config = &p_tcb->config;
                    max_streams = p_tcb->transmit.num_out_streams;
                }

                else
                {
                    p_config = &p_ep->sk_config;
                }

                /*check that parameters are valid */
                if( ( p_info->sinfo_stream > max_streams ) ||
                        ( ( SCTP_SOCK_SEQPACKET == p_ep->sk_type ) &&
                          ( ( p_info->sinfo_flags & SCTP_MSG_ABORT ) ||
                            ( p_info->sinfo_flags & SCTP_MSG_EOF ) ) ) )

                {
                    sk_errno = SK_EINVAL;
                    ret = SK_FAILURE;
                    break;
                }

                p_config->sndrcvinfo_valid = SCTP_TRUE;
                p_config->def_sndrcvinfo = *p_info;
            }
            break;

        case SCTP_MAXSEG:
            {
                sctp_S32bit size = *( ( sctp_S32bit * )optval );

                if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

                    if( !p_tcb )
                    {
                        p_ep->sk_config.maxseg = size;
                        break;
                    }

                    p_tcb->config.maxseg = size;
                }

                /*UDP style socket*/
                else
                {
                    for( p_tcb = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );
                            ( p_tcb != SCTP_NULL );
                            p_tcb = ( sctp_tcb_st * )lstNext( ( NODE * )p_tcb ) )
                    {
                        p_tcb->config.maxseg = size;
                        assoc_or_ep = SCTP_TRUE;
                    }

                    if( !assoc_or_ep )
                    {
                        p_ep->sk_config.maxseg = size;
                    }

                }
            }

            break;
            /* Bug Id 5 changes starts */
#ifdef SCTP_IP_HEADER_PARAMS

        case SCTP_IP_DSCP:
            {
                sctp_assoc_ip_hdr_params_st *p_info = ( sctp_assoc_ip_hdr_params_st * )optval;

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->sip_assoc_id;

                    p_tcb =
                        sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                p_config = ( ( p_tcb ) ? & ( p_tcb->config ) : & ( p_ep->sk_config ) );

#ifdef SCTP_INET6_SUPPORT
                if( SCTP_AF_INET6 == p_ep->family )
                {
                    p_config->ip_header_params.tclass =
                        p_info->sip_tclass;
                }
#endif

                if( SCTP_AF_INET == p_ep->family )
                {
                    p_config->ip_header_params.tos =
                        p_info->sip_tos;
                }
            }
            break;
#endif

            /* Bug Id 5 changes ends */
        default:
            sk_errno = SK_ENOPROTOOPT;
            ret = ( SK_FAILURE );
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_set_rcv_snd_sockopt_udp_sctp
 *
 *    DESCRIPTION : It is used to set the socket option for UDP style socket.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int sctp_set_rcv_snd_sockopt_udp_sctp(
    sctp_ep_st            *p_ep,
    int                    rcv_send_flag,
    const void            *optval )
{
    sctp_tcb_st *p_ass;
    sctp_error_t ecode;
    int opt_name;
    sctp_socklen_t opt_len;
    sctp_U32bit old_opt_val;
    int ret;

    /*rcv_send_flag =1 indicates the call is for SCTP_SO_RCVBUF*/

    opt_name = ( rcv_send_flag == 1 ) ? SCTP_SO_RCVBUF : SCTP_SO_SNDBUF;
    opt_len = sizeof( int );
    /*store the old option value for failure recovery*/

    if( p_ep != SCTP_NULL )
    {
        /*Extract the first node*/
        p_ass = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

        /*Initially store the assoc_id in the opt_val so that it
         * is available in sctp_getsockopt_sctp*/

        old_opt_val = p_ass->assoc_id;

        /*store the old option value for failure recovery*/
        if( SK_SUCCESS != sctp_getsockopt_sctp( p_ep, opt_name,
                                                ( void * )&old_opt_val, &opt_len ) )
        {
            return SK_FAILURE;
        }


        /*Set the socket option for all association*/
        for( ; ( p_ass != SCTP_NULL );
                p_ass = ( sctp_tcb_st * )lstNext( ( NODE * )p_ass ) )
        {
            if( rcv_send_flag == 1 )
            {
                ret = sctp_db_set_rx_buffers( p_ass, *( ( int * )optval ), &ecode );

                /*error occur while set socket option, hence do fall back for
                 *all the association*/
                if( ret == SCTP_FAILURE )
                {
                    for( p_ass = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );
                            ( p_ass != SCTP_NULL );
                            p_ass = ( sctp_tcb_st * )lstNext( ( NODE * )p_ass ) )
                    {
                        /* Bug Id 81 fix s */
                        // ret =sctp_db_set_rx_buffers(p_ass, old_opt_val, &ecode);
                        sctp_db_set_rx_buffers( p_ass, old_opt_val, &ecode );
                        /* Bug Id 81 fix e */
                    }

                    return SK_FAILURE;

                }
            }

            else
            {
                ret = sctp_db_set_tx_buffers( p_ass, *( ( int * )optval ), &ecode );

                /*error occur while set socket option, hence do fall back for
                 *all the association*/
                if( ret == SCTP_FAILURE )
                {

                    for( p_ass = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );
                            ( p_ass != SCTP_NULL );
                            p_ass = ( sctp_tcb_st * )lstNext( ( NODE * )p_ass ) )
                    {
                        /* Bug Id 81 fix s */
                        // ret =sctp_db_set_tx_buffers(p_ass, old_opt_val, &ecode);
                        sctp_db_set_tx_buffers( p_ass, old_opt_val, &ecode );
                        /* Bug Id 81 fix e */
                    }

                    return SK_FAILURE;

                }

            }
        }

        return SK_SUCCESS;
    }

    return  SK_FAILURE;
}



/***************************************************************************
 *    FUNCTION    : sctp_setsockopt_socket
 *
 *    DESCRIPTION : It is used to set the socket option at the SOCKET level(ep).
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_setsockopt_socket(
    sctp_ep_st            *p_ep,
    int                   optname,
    const void            *optval,
    sctp_socklen_t         optlen )
{
    int ret = 0;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( optlen );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_setsockopt_socket" ) );

    switch( optname )
    {

        case SCTP_SO_REUSEADDR:
            {
                /*
                 * Specifies that the rules used  in  validating
                 * addresses  supplied to bind should allow
                 * reuse of local addresses,  if  this  is  sup-
                 * ported by the protocol.  This option takes an
                 * int value.
                 */
                sctp_S32bit on = *( ( int * )optval );

                SCTP_SOCK_SET_REUSEADDR( p_ep->flags, ( on ? 1 : 0 ) );
            }
            break;

        case SCTP_SO_RTOINFO:
            {
                sctp_rtoinfo_st *p_info = ( sctp_rtoinfo_st * )optval;
                sctp_U32bit rto_init, rto_max, rto_min;

                /* tune RTO parameters */
                if( p_info->srto_initial != 0 )
                {
                    rto_init = p_info->srto_initial;
                }

                else
                {
                    rto_init = p_ep->sk_config.rto_init;
                }

                if( p_info->srto_max != 0 )
                {
                    rto_max = p_info->srto_max;
                }

                else
                {
                    rto_max = p_ep->sk_config.rto_max;
                }

                if( p_info->srto_min != 0 )
                {
                    rto_min = p_info->srto_min;
                }

                else
                {
                    rto_min = p_ep->sk_config.rto_min;
                }

                if( ( rto_min > rto_init ) || ( rto_min > rto_max ) ||
                        ( rto_init > rto_max ) )
                {
                    sk_errno = SK_EINVAL;
                    SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                sk_errno,
                                "sctp_setsockopt_socket: Service User API ERROR" );

                    SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "Invalid Values of Parameters " ) );

                    ret = SK_FAILURE;
                    break;
                }

                /* Copy the RTO info into the assoc structure */
                p_ep->sk_config.rto_init = rto_init;
                p_ep->sk_config.rto_min  = rto_min;
                p_ep->sk_config.rto_max  = rto_max;
            }
            break;

        case SCTP_SO_ASSOCINFO:
            {
                sctp_assocparams_st *p_info = ( sctp_assocparams_st * )optval;
                sctp_U32bit path_maxrtx;

                path_maxrtx = p_ep->sk_config.path_max_retrans;

                if( 0 != p_info->sasoc_asocmaxrxt )
                {
                    if( path_maxrtx >  p_info->sasoc_asocmaxrxt )
                    {
                        sk_errno = SK_EINVAL;
                        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                    sk_errno,
                                    "sctp_setsockopt_socket: Service User API ERROR" );

                        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "Invalid Values of Parameters " ) );

                        ret = SK_FAILURE;
                        break;
                    }

                    /* Tune Assoc parameters */
                    p_ep->sk_config.assoc_max_retrans = p_info->sasoc_asocmaxrxt;
                }

                if( 0 < p_info->sasoc_cookie_life )
                {
                    p_ep->sk_config.valid_cookie_life =
                        p_info->sasoc_cookie_life;
                }
            }
            break;

        case SCTP_SO_INITMSG:
            {
                sctp_initmsg_st *p_info = ( sctp_initmsg_st * )optval;

                if( ( p_info->sinit_max_instreams > sctp_db_main.init_in_streams )
                        || ( p_info->sinit_num_ostreams >
                             sctp_db_main.init_out_streams ) )
                {
                    sk_errno = SK_EINVAL;
                    SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                sk_errno,
                                "sctp_setsockopt_socket: Service User API ERROR" );

                    SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "Invalid Values of Parameters " ) );

                    ret = SK_FAILURE;
                    break;
                }

                /* Tune Init parameters */
                if( p_info->sinit_max_instreams > 0 )
                {
                    p_ep->sk_config.num_in_streams =
                        p_info->sinit_max_instreams;
                }

                if( p_info->sinit_num_ostreams > 0 )
                {
                    p_ep->sk_config.num_out_streams =
                        p_info->sinit_num_ostreams;
                }

                if( p_info->sinit_max_attempts > 0 )
                {
                    p_ep->sk_config.max_init_retrans =
                        p_info->sinit_max_attempts;
                }

                p_ep->sk_config.init_timeo = p_info->sinit_max_init_timeo;
            }
            break;

        case SCTP_SO_LINGER:
            {
                sctp_linger_st *p_info = ( sctp_linger_st * )optval;

                /*
                 * If its !TCP style then return failure.
                 */
                if( SCTP_SOCK_STREAM != p_ep->sk_type )
                {
                    sk_errno = SK_ENOPROTOOPT;
                    ret = ( SK_FAILURE );
                    break;
                }

                /* Check the validity of the linger option */
                if( p_info->l_linger < 0 )
                {
                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "sctp_setsockopt_socket: l_linger is -ve" ) );

                    sk_errno = SK_EINVAL;
                    ret = SK_FAILURE;
                    break;
                }

                /* Set the linger structure */
                p_ep->so_linger.l_linger = p_info->l_linger;
                p_ep->so_linger.l_onoff  = p_info->l_onoff;
            }
            break;

        case SCTP_SO_NONBLOCKING:
            {
                /*
                 * Set to make the socket non-blocking.
                 */
                sctp_S32bit on = *( ( int * )optval );

                SCTP_SOCK_SET_BLOCKING( p_ep->flags, ( on ? 0 : 1 ) );
            }
            break;

        case SCTP_SO_NODELAY:
            {
                sctp_S32bit on = *( ( int * )optval );

                SCTP_SOCK_SET_NODELAY( p_ep->flags, ( on ? 0 : 1 ) );
            }
            break;

        case SCTP_I_WANT_MAPPED_V4_ADDR:
            {
                /*Bug Id 22 fix starts*/
                /*sctp_S32bit on = *((int*)optval);*/
                sctp_U32bit on = *( ( unsigned int * )optval );
                /*Bug Id 22 fix ends*/


                p_ep->sk_config.want_mapped = on;
            }
            break;

        default:
            sk_errno = SK_ENOPROTOOPT;
            ret = ( SK_FAILURE );
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_getsockopt_sctp
 *
 *    DESCRIPTION : It is used to get the socket option at the SCTP level.
 *    i.e. for TCP and server sockets it will be same as at socket level.
 *         for UDP it shall be for individual associtions.
 *    The optval will contain the input assooc id required to identify
 *    association
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_getsockopt_sctp(
    sctp_ep_st            *p_ep,
    int                   optname,
    void                  *optval /*IN/Out param*/,
    sctp_socklen_t        *optlen )
{
    sctp_assoc_t assoc_id = SCTP_INVALID_ASSOC;
    int          ret      = SK_SUCCESS;
    sctp_tcb_st  *p_tcb   = SCTP_NULL;

    assoc_id = *( sctp_U32bit * )optval;
    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_getsockopt_sctp" ) );

    /*
    * This function gets the socket options at the assoc level. Incase the EP
    * doesn't have assocition within it then return failure.
    */
    switch( optname )
    {
        case SCTP_SO_RCVBUF:
            {
                int *p_size = ( int * )optval;
                *optlen = sizeof( int );


                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    *p_size =
                        sctp_db_main.min_rx_buffers * SCTP_MIN_FRAGSIZE +
                        sctp_db_main.med_rx_buffers * SCTP_MED_FRAGSIZE +
                        sctp_db_main.max_rx_buffers * SCTP_MAX_FRAGSIZE;
                    break;
                }

                *p_size = sctp_db_assoc_max_rx_window( p_tcb );
            }
            break;

        case SCTP_SO_SNDBUF:
            {
                int *p_size = optval;
                *optlen = sizeof( int );

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    *p_size =
                        sctp_db_get_number_txbuffers();
                    break;
                }

                *p_size = sctp_db_assoc_max_tx_window( p_tcb );
            }
            break;

        case SCTP_SO_RTOINFO:
            {
                sctp_rtoinfo_st *p_info = ( sctp_rtoinfo_st * )optval;
                *optlen = sizeof( sctp_rtoinfo_st );

                /* tune RTO parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb =
                        sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    p_info->srto_initial = p_ep->sk_config.rto_init;
                    p_info->srto_min     = p_ep->sk_config.rto_min;
                    p_info->srto_max     = p_ep->sk_config.rto_max;
                    break;
                }

                /* Copy the RTO info into the assoc structure */
                p_info->srto_initial = p_tcb->config.rto_init;
                p_info->srto_min     = p_tcb->config.rto_min;
                p_info->srto_max     = p_tcb->config.rto_max;
                /* SPR 4154 */
                p_info->srto_assoc_id = p_tcb->assoc_id;
            }
            break;

        case SCTP_SO_ASSOCINFO:
            {
                sctp_assocparams_st *p_info = ( sctp_assocparams_st * )optval;
                *optlen = sizeof( sctp_assocparams_st );

                /* Tune Assoc parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb =
                        sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    p_info->sasoc_asocmaxrxt  = p_ep->sk_config.assoc_max_retrans;
                    p_info->sasoc_cookie_life = p_ep->sk_config.valid_cookie_life;
                    p_info->sasoc_local_rwnd  = 0;
                    p_info->sasoc_number_peer_destinations  = 0;
                    p_info->sasoc_assoc_id    = -1;
                    p_info->sasoc_peer_rwnd   = 0;
                    break;
                }

                p_info->sasoc_assoc_id    = p_tcb->assoc_id;
                p_info->sasoc_asocmaxrxt  = p_tcb->config.assoc_max_retrans;
                p_info->sasoc_number_peer_destinations  =
                    p_tcb->transmit.num_remote_address;
                p_info->sasoc_peer_rwnd   = p_tcb->transmit.rwndthere;
                p_info->sasoc_local_rwnd  = sctp_db_assoc_rx_window( p_tcb );
                p_info->sasoc_cookie_life = p_tcb->config.valid_cookie_life;

            }
            break;

        case SCTP_SO_INITMSG:
            {
                sctp_prot_params_st   *p_config;
                sctp_initmsg_st *p_info = ( sctp_initmsg_st * )optval;
                *optlen = sizeof( sctp_initmsg_st );

                p_config = &( p_ep->sk_config );

                /* Tune Init parameters */
                p_info->sinit_max_instreams = p_config->num_in_streams;
                p_info->sinit_num_ostreams  = p_config->num_out_streams;
                p_info->sinit_max_attempts  = p_config->max_init_retrans;
                p_info->sinit_max_init_timeo = p_config->init_timeo;
            }
            break;

        case SCTP_SO_PRIMARY_ADDR:
            {
                sctp_setprim_st  *p_info = ( sctp_setprim_st * )optval;
                sctp_U32bit      sctp_index   = 0;
                *optlen = sizeof( sctp_setprim_st );

                /* Tune Assoc parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    sk_errno = SK_EBADF;
                    ret = SK_FAILURE;

                    break;
                }

                sctp_index = p_tcb->transmit.remote[p_tcb->transmit.\
                                                    primary_address_index].src_addr_index;

                p_info->ssp_assoc_id = p_tcb->assoc_id;

                SOCKADDR_STORAGE_FAMILY( p_info->ssp_addr ) =
                    ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

                sctp_conv_addr_sctp_to_os( 1,
                                           &( p_tcb->p_ep->this_address[sctp_index] ),
                                           &( p_info->ssp_addr ), p_tcb->p_ep->this_port, SCTP_NULL );
            }
            break;

        case SCTP_SO_PEER_PRIMARY_ADDR:
            {
                sctp_setpeerprim_st  *p_info = ( sctp_setpeerprim_st * )optval;
                sctp_U32bit          sctp_index   = 0;
                *optlen = sizeof( sctp_setpeerprim_st );

                /* Tune Assoc parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    sk_errno = SK_EINVAL;
                    ret = SK_FAILURE;

                    break;
                }

                sctp_index = p_tcb->transmit.primary_address_index;

                /* Get the primary address and return it to the user. */
                /* SPR 4160 */
                SOCKADDR_STORAGE_FAMILY( p_info->sspp_addr ) =
                    ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

                sctp_conv_addr_sctp_to_os( 1,
                                           &( p_tcb->transmit.remote[sctp_index].that_address ),
                                           &( p_info->sspp_addr ), p_tcb->transmit.that_port, SCTP_NULL );

                p_info->sspp_assoc_id = p_tcb->assoc_id;
            }
            break;

        case SCTP_SO_STATUS:
            {
                sctp_status_st *p_info = ( sctp_status_st * )optval;
                sctpbuffer_st  *sctp_buf = SCTP_NULL;
                sctp_U32bit    counter = 0;
                *optlen = sizeof( sctp_status_st );

                /* Tune Assoc parameters */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    /*SPR 5086*/
                    __MEMSET( p_info, 0, sizeof( sctp_status_st ) );

                    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
                    {
                        p_info->sstat_state = SCTP_LISTEN;
                    }

                    else if( SCTP_SOCK_IS_BOUND( p_ep->flags ) )
                    {
                        p_info->sstat_state = SCTP_BOUND;
                    }

                    /*otherwise its already set to SCTP_CLOSED (0)*/
                    /*SPR 5086*/
                    break;
                }

                /*woking on 5086*/
                switch( p_tcb->state )
                {
                    case SCTPSTATE_CLOSED:
                        p_info->sstat_state = SCTP_CLOSED;
                        break;

                    case SCTPSTATE_COOKIEWAIT:
                        p_info->sstat_state = SCTP_COOKIE_WAIT;
                        break;

                    case SCTPSTATE_COOKIESENT:
                        p_info->sstat_state = SCTP_COOKIE_ECHOED;
                        break;

                    case SCTPSTATE_SHUTDOWNPEND:
                        p_info->sstat_state = SCTP_SHUTDOWN_PENDING;
                        break;

                    case SCTPSTATE_SHUTDOWNRCVD:
                        p_info->sstat_state = SCTP_SHUTDOWN_RECEIVED;
                        break;

                    case SCTPSTATE_SHUTDOWNSENT:
                        p_info->sstat_state = SCTP_SHUTDOWN_SENT;
                        break;

                    case SCTPSTATE_SHUTDOWNACKSENT:
                        p_info->sstat_state = SCTP_SHUTDOWN_ACK_SENT;
                        break;

                    case SCTPSTATE_ESTABLISHED:
                        p_info->sstat_state = SCTP_ESTABLISHED;
                        break;

                    default:
                        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                                    ( "sctp_getsockopt_sctp:invalid state" ) );
                        break;

                }

                p_info->sstat_rwnd      = p_tcb->transmit.rwndthere;

                sctp_buf = ( sctpbuffer_st * )lstFirst( &p_tcb->transmit.
                                                        transmit_queue );

                p_info->sstat_unackdata = 0;
                p_info->sstat_penddata  = 0;

                for( ; sctp_buf != SCTP_NULL;
                        sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf ) )
                {
                    if( sctp_buf->sent == 1 )
                    {
                        p_info->sstat_unackdata++;
                    }
                }

                for( counter = 0; counter < p_tcb->receive.num_in_streams;
                        counter++ )
                {
                    /* change from counting chunks to counting messages ??*/
                    p_info->sstat_penddata +=
                        lstCount( &p_tcb->receive.stream[counter].receive_queue );
                }

                counter = p_tcb->transmit.primary_address_index;

                SOCKADDR_STORAGE_FAMILY( p_info->sstat_primary.spinfo_address ) =
                    ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

                sctp_conv_addr_sctp_to_os( 1,
                                           &( p_tcb->transmit.remote[counter].that_address ),
                                           &( p_info->sstat_primary.spinfo_address ),
                                           p_tcb->transmit.that_port,
                                           SCTP_NULL );

                p_info->sstat_primary.spinfo_state
                    = p_tcb->transmit.remote[counter].state;
                p_info->sstat_primary.spinfo_cwnd
                    = p_tcb->transmit.remote[counter].cwnd;
                p_info->sstat_primary.spinfo_srtt
                    = p_tcb->transmit.remote[counter].srtt;
                p_info->sstat_primary.spinfo_rto
                    = p_tcb->transmit.remote[counter].rto;
                p_info->sstat_primary.spinfo_assoc_id
                    = p_tcb->assoc_id;

                p_info->sstat_assoc_id = p_tcb->assoc_id;
                p_info->sstat_outstrms = p_tcb->transmit.num_out_streams;
                p_info->sstat_instrms  = p_tcb->receive.num_in_streams;

                p_info->sstat_fragmentation_point = SCTP_IS_STACK_UDP ?
                                                    SCTP_UDP_MAX_FRAG_SIZE( p_tcb->transmit.lowest_path_mtu ) :
                                                    SCTP_IP_MAX_FRAG_SIZE( p_tcb->transmit.lowest_path_mtu );
            }
            break;

        case SCTP_SO_NODELAY:
            {
                /*
                 * This works like disabling bundling. If u set the option
                 * NODELAY then the packets are sent as soon as possible
                 * without any delay.
                 */
                int *p_on = ( int * )optval;
                *optlen = sizeof( sctp_S32bit );

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( !p_tcb )
                {
                    *p_on = SCTP_SOCK_IS_NODELAY( p_ep->flags );
                }

                else
                {
                    *p_on = ( 0 == p_tcb->config.bundling_time );
                }

            }
            break;

        case SCTP_SO_AUTOCLOSE:
            {
                /* Only valid for UDP style sockets. */
                int *p_on = ( int * )optval;
                *optlen = sizeof( sctp_S32bit );

                /* Set the AUTO Close flag if UDP style socket. */
                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    *p_on = p_ep->aclose_time;
                }

                else
                {
                    sk_errno = SK_ENOPROTOOPT;
                    ret      = SK_FAILURE;
                }
            }
            break;

        case SCTP_SO_SET_ADAPTION_LAYER:
            {
                sctp_setadaption_st *p_ind = ( sctp_setadaption_st * )optval;
                *optlen = sizeof( sctp_setadaption_st );

                p_ind->ssb_adaption_ind = p_ep->sk_config.adaption_ind;
            }
            break;

        case SCTP_SO_DISABLE_FRAGMENTS:
            {
                sctp_S32bit on = *( ( int * )optval );
                *optlen = sizeof( sctp_S32bit );

                if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                else
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                /*for UDP style sockets use sctp_opt_info*/

                if( !p_tcb )
                {
                    on = p_ep->sk_config.dont_fragment;
                    break;
                }

                on = p_tcb->config.dont_fragment;
                /* SPR 21127 CHANGES START */
                SCTP_UNUSED_VARIABLE( on );
                /* SPR 21127 CHANGES START */
            }
            break;


        case SCTP_SO_DEFAULT_SEND_PARAM:
            {
                sctp_sndrcvinfo_st *p_info = ( sctp_sndrcvinfo_st * )optval;
                sctp_prot_params_st *p_config = SCTP_NULL;
                *optlen = sizeof( sctp_sndrcvinfo_st );

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    assoc_id = p_info->sinfo_assoc_id;

                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }


                p_config = ( ( p_tcb ) ? & ( p_tcb->config ) : & ( p_ep->sk_config ) );

                if( p_config->sndrcvinfo_valid )
                {
                    *p_info = p_config->def_sndrcvinfo;
                }

                else
                {
                    /*set the default send params*/
                    p_info->sinfo_context    = 0;
                    p_info->sinfo_flags      = 0;
                    p_info->sinfo_ppid       = p_ep->ppi;
                    p_info->sinfo_stream     = 0;
                    p_info->sinfo_timetolive = 0;
                }
            }
            break;


        case SCTP_PEER_ADDR_PARAMS:
        case SCTP_GET_PEER_ADDR_PARAMS:
            {
                *optlen = sizeof( sctp_paddrparams_st );

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( SCTP_NULL == p_tcb )
                {
                    sk_errno = SK_ENOTCONN;
                    ret = SK_FAILURE;
                    break;
                }

                ret = sctp_get_paddrparams( p_tcb, optval );
            }
            break;

        case SCTP_GET_PEER_ADDR_INFO:
            {
                *optlen = sizeof( sctp_paddrinfo_st );

                if( SCTP_SOCK_SEQPACKET == p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                else
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                if( SCTP_NULL == p_tcb )
                {
                    sk_errno = SK_ENOTCONN;
                    ret = SK_FAILURE;
                    break;
                }

                ret = sctp_get_paddrinfo( p_tcb, optval );
            }
            break;

        case SCTP_SO_SET_EVENTS:
            {
                sctp_event_subscribe_st *p_info =
                    ( sctp_event_subscribe_st * )optval;

                *optlen = sizeof( sctp_event_subscribe_st );

                p_info->sctp_data_io_event      =
                    SCTP_SOCK_IS_RECVDATAIO_ON( p_ep->ulpqueue.ntfy_flags );

                p_info->sctp_association_event  =
                    SCTP_SOCK_IS_RECVASSOC_ON( p_ep->ulpqueue.ntfy_flags );

                p_info->sctp_address_event      =
                    SCTP_SOCK_IS_RECVPADDR_ON( p_ep->ulpqueue.ntfy_flags ) ;

                p_info->sctp_send_failure_event =
                    SCTP_SOCK_IS_RECVSENDFAIL( p_ep->ulpqueue.ntfy_flags );

                p_info->sctp_peer_error_event   =
                    SCTP_SOCK_IS_RECVPEERERR_ON( p_ep->ulpqueue.ntfy_flags );

                p_info->sctp_shutdown_event     =
                    SCTP_SOCK_IS_RECVSHUTDOWN( p_ep->ulpqueue.ntfy_flags );

                p_info->sctp_adaption_layer_event   =
                    SCTP_SOCK_IS_RECVADAPTION( p_ep->ulpqueue.ntfy_flags );

                p_info->sctp_partial_delivery_event =
                    SCTP_SOCK_IS_RECVPARTDELIVERY( p_ep->ulpqueue.ntfy_flags );

            }
            break;

        case SCTP_MAXSEG:
            {
                sctp_S32bit *p_size = optval;
                *optlen = sizeof( sctp_S32bit );

                if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );
                }

                else
                {
                    p_tcb = sctp_db_get_association_ptr( p_ep->ep_id, assoc_id );
                }

                /*for UDP style sockets use sctp_opt_info*/

                if( !p_tcb )
                {
                    *p_size = p_ep->sk_config.maxseg;
                    break;
                }

                *p_size = p_tcb->config.maxseg;
            }
            break;

        default:
            sk_errno = SK_ENOPROTOOPT;
            ret = ( SK_FAILURE );
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_getsockopt_socket
 *
 *    DESCRIPTION : It is used to get the socket option
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_getsockopt_socket(
    sctp_ep_st            *p_ep,
    int                   optname,
    void                  *optval,
    sctp_socklen_t        *optlen )
{
    int ret = 0;

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_getsockopt_socket" ) );

    switch( optname )
    {
        case SCTP_SO_REUSEADDR:
            {
                /*
                * Specifies that the rules used  in  validating
                * addresses  supplied to bind should allow
                * reuse of local addresses,  if  this  is  sup-
                * ported by the protocol.  This option takes an
                * int value.
                    */
                int *p_on = ( int * )optval;
                *optlen = sizeof( sctp_S32bit );
                *p_on = SCTP_SOCK_IS_REUSEADDR( p_ep->flags );
            }
            break;

        case SCTP_SO_NONBLOCKING:
            {
                /*
                * Set to make the socket non-blocking.
                    */
                int *p_on = ( int * )optval;
                *optlen = sizeof( sctp_S32bit );

                *p_on = ( SCTP_SOCK_IS_BLOCKING( p_ep->flags ) ) ? 0 : 1;
            }
            break;

        case SCTP_SO_RTOINFO:
            {
                sctp_rtoinfo_st *p_info = ( sctp_rtoinfo_st * )optval;
                *optlen = sizeof( sctp_rtoinfo_st );

                /* tune RTO parameters */
                /* Copy the RTO info into the assoc structure */
                p_info->srto_initial = p_ep->sk_config.rto_init;
                p_info->srto_min     = p_ep->sk_config.rto_min;
                p_info->srto_max     = p_ep->sk_config.rto_max;
            }
            break;

        case SCTP_SO_ASSOCINFO:
            {
                sctp_assocparams_st *p_info = ( sctp_assocparams_st * )optval;
                sctp_tcb_st         *p_tcb = NULL;
                *optlen = sizeof( sctp_assocparams_st );

                /* Tune Assoc parameters */
                p_info->sasoc_asocmaxrxt = p_ep->sk_config.assoc_max_retrans;

                p_info->sasoc_cookie_life = p_ep->sk_config.valid_cookie_life;

                if( SCTP_SOCK_SEQPACKET != p_ep->sk_type )
                {
                    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

                    if( !p_tcb )
                    {
                        sk_errno = SK_EBADF;
                        ret = SK_FAILURE;

                        break;
                    }

                    p_info->sasoc_number_peer_destinations  =
                        p_tcb->transmit.num_remote_address;
                    p_info->sasoc_peer_rwnd   = p_tcb->transmit.rwndthere;
                    p_info->sasoc_local_rwnd  = sctp_db_assoc_rx_window( p_tcb );
                }

                else
                {
                    p_info->sasoc_number_peer_destinations  = 0;
                    p_info->sasoc_peer_rwnd   = 0;
                    p_info->sasoc_local_rwnd  = 0;
                }

                p_info->sasoc_assoc_id    = 0;

            }
            break;

        case SCTP_SO_INITMSG:
            {
                sctp_initmsg_st *p_info = ( sctp_initmsg_st * )optval;
                *optlen = sizeof( sctp_initmsg_st );

                /* Tune Init parameters */
                p_info->sinit_max_instreams = p_ep->sk_config.num_in_streams;
                p_info->sinit_num_ostreams  = p_ep->sk_config.num_out_streams;
                p_info->sinit_max_attempts  = p_ep->sk_config.max_init_retrans;
                p_info->sinit_max_init_timeo = p_ep->sk_config.init_timeo;
            }
            break;

        case SCTP_SO_LINGER:
            {
                sctp_linger_st *p_info = ( sctp_linger_st * )optval;
                *optlen = sizeof( sctp_linger_st );

                /*
                 * If its !TCP style then return failure.
                 */
                if( SCTP_SOCK_STREAM != p_ep->sk_type )
                {
                    sk_errno = SK_ENOPROTOOPT;
                    ret = ( SK_FAILURE );
                    break;
                }

                /* Check the validity of the linger option */
                if( p_info->l_linger < 0 )
                {
                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "sctp_getsockopt_socket: l_linger is -ve" ) );

                    sk_errno = SK_EINVAL;
                    ret = SK_FAILURE;
                    break;
                }

                /* Set the linger structure */
                p_info->l_linger = p_ep->so_linger.l_linger;
                p_info->l_onoff  = p_ep->so_linger.l_onoff;
            }
            break;

        case SCTP_SO_NODELAY:
            {
                /*
                 * This works like disabling bundling. If u set the option
                 * NODELAY then the packets are sent as soon as possible
                 * without any delay.
                 */
                int *p_on = ( int * )optval;
                *optlen = sizeof( sctp_S32bit );

                *p_on = SCTP_SOCK_IS_NODELAY( p_ep->flags );
            }
            break;

        case SCTP_I_WANT_MAPPED_V4_ADDR :
            {
                int *p_on = ( int * )optval;
                *optlen = sizeof( sctp_S32bit );

                *p_on = p_ep->sk_config.want_mapped;
            }
            break;

        default:
            sk_errno = SK_ENOPROTOOPT;
            ret = ( SK_FAILURE );
            break;
    }

    return ( ret );
}


/***************************************************************************
 *    FUNCTION    : sctp_set_paddrparams
 *
 *    DESCRIPTION : It is used to set the peer address parameters in the sock
 *    interface.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_set_paddrparams(
    sctp_tcb_st               *p_tcb,
    const void                *p_args )
{
    sctp_paddrparams_st *p_info = ( sctp_paddrparams_st * )p_args;
    sctp_sockaddr_st    sctp_addr;
    sctp_port_t         port  = 0;
    sctp_U32bit         aindex = 0;
    sctp_error_t        s_err;

    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC, ( "sctp_set_paddrparams" ) );

    /*
     * Set the heartbeat parameters and the heartbeat flag for the
     * address specified.
     */
    sctp_conv_addr_storg_os_to_sctp( 1, &( p_info->spp_address ), &sctp_addr,
                                     &port, SCTP_NULL );

    if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb, &sctp_addr,
                                                      &aindex ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                    ( "sctp_set_paddrparams: Couldnot get address aindex" ) );

        sk_errno = SK_EINVAL;
        return ( SK_FAILURE );
    }

    /* Set the Path max retrans */
    if( p_info->spp_pathmaxrxt != 0 )
    {
        /* SPR 4153 */
        p_tcb->transmit.remote[aindex].path_error_threshold
            = p_info->spp_pathmaxrxt;
    }

    if( !p_info->spp_hbinterval )
    {
        /* Disable the heartbeat */
        p_tcb->transmit.remote[aindex].allow_heartbeat = SCTP_FALSE;

        /* Stop the inactivity time if running */
        if( SCTP_NULL_TIMER_ID !=
                p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY] )
        {
            sctp_stop_timer( &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
                             SCTP_NULL, SCTP_NULL );
        }
    }

    else if( SCTP_U32bit_MAX == p_info->spp_hbinterval )
    {
        sctp_addrinfo_st    to;
        /*
         * This case we don't do anything...we just send a HB to the
         * destiantion specified.
         */

        to.checksum   = p_tcb->config.checksum_func;
        to.p_sockaddr = &p_tcb->transmit.remote[aindex].that_address;
        sctp_send_chunk_heartbeat( p_tcb,
                                   &p_tcb->p_ep->this_address[0],
                                   p_tcb->p_ep->this_port,
                                   p_tcb->transmit.that_port,
                                   aindex, p_tcb->remote_tag );


        /* SPR 21127 CHANGES START */
        SCTP_UNUSED_VARIABLE( to );
        /* SPR 21127 CHANGES END */
        sctp_stop_timer( &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
                         SCTP_NULL, SCTP_NULL );
    }

    else
    {
        p_tcb->transmit.remote[aindex].allow_heartbeat = SCTP_TRUE;

        /* Do calulation on the heartbeat interval */
        p_tcb->config.hb_interval = p_info->spp_hbinterval;

        /* start the Inactivity timer if not running  */
        if( ( p_tcb->state == SCTPSTATE_ESTABLISHED )
                && p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY]
                == SCTP_NULL_TIMER_ID )
        {
            sctp_start_timer(
                p_tcb,
                SCTPEVENT_TIMERINACTIVE,
                ( p_tcb->config.hb_interval + p_tcb->transmit.remote[aindex].rto ),
                aindex,
                &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
                &s_err );
        }
    }

    return ( SK_SUCCESS );
}


/***************************************************************************
 *    FUNCTION    : sctp_get_paddrparams
 *
 *    DESCRIPTION : It is used to get the peer address parameters in the sock
 *    interface.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_get_paddrparams(
    sctp_tcb_st               *p_tcb,
    void                      *p_args )
{
    sctp_paddrparams_st *p_info = ( sctp_paddrparams_st * )p_args;
    sctp_sockaddr_st    sctp_addr;
    sctp_port_t         port  = 0;
    sctp_U32bit         sctp_index = 0;

    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC, ( "sctp_get_paddrparams" ) );

    /*
     * Get the heartbeat parameters and the heartbeat flag for the
     * address specified.
     */
    sctp_conv_addr_storg_os_to_sctp( 1, &( p_info->spp_address ), &sctp_addr,
                                     &port, SCTP_NULL );

    if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb, &sctp_addr, &sctp_index ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                    ( "sctp_get_paddrparams: Couldnot get address index" ) );

        sk_errno = SK_EINVAL;
        return ( SK_FAILURE );
    }

    /* Get the Path Error Threshold */
    p_info->spp_pathmaxrxt = p_tcb->transmit.remote[sctp_index].path_error_threshold;
    p_info->spp_hbinterval = p_tcb->config.hb_interval;
    p_info->spp_assoc_id   = p_tcb->assoc_id;

    return ( SK_SUCCESS );
}


/***************************************************************************
 *    FUNCTION    : sctp_get_paddrinfo
 *
 *    DESCRIPTION : It is used to get the peer address info.
 *
 *    RETURN VALUE: On success 0 otherwise -1
 ****************************************************************************/
int
sctp_get_paddrinfo(
    sctp_tcb_st               *p_tcb,
    void                      *p_args )
{
    sctp_paddrinfo_st   *p_info = ( sctp_paddrinfo_st * )p_args;
    sctp_sockaddr_st    sctp_addr;
    sctp_port_t         port  = 0;
    sctp_U32bit         sctp_index = 0;

    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC, ( "sctp_get_paddrinfo" ) );

    /*
     * Get the heartbeat parameters and the heartbeat flag for the
     * address specified.
     */
    sctp_conv_addr_storg_os_to_sctp( 1, &( p_info->spinfo_address ), &sctp_addr,
                                     &port, SCTP_NULL );

    if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb, &sctp_addr,
                                                      &sctp_index ) )
    {
        SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                    ( "sctp_get_paddrinfo: Couldnot get address index" ) );

        sk_errno = SK_EINVAL;
        return ( SK_FAILURE );
    }

    /* Set the Path max retrans */
    p_info->spinfo_state = p_tcb->transmit.remote[sctp_index].status;
    p_info->spinfo_cwnd  = p_tcb->transmit.remote[sctp_index].cwnd;
    p_info->spinfo_srtt  = p_tcb->transmit.remote[sctp_index].srtt;
    p_info->spinfo_rto   = p_tcb->transmit.remote[sctp_index].rto;
    p_info->spinfo_assoc_id = p_tcb->assoc_id;
    p_info->spinfo_mtu   = p_tcb->transmit.lowest_path_mtu;

    return ( SK_SUCCESS );
}


/****************************************************************************
 *    FUNCTION    : sctp_sendmsg_tcp
 *
 *    DESCRIPTION : The  sendmsg()  function sends a message through a
 *    connection-mode. If the socket is connection-mode, the destination
 *    address in msghdr is ignored.
 *    the msghdr stucture->
 *    struct msghdr {
 *       void    *msg_name;               optional address
 *       size_t  msg_namelen;             size of address
 *       struct  iovec *msg_iov;          scatter/gather array
 *       int     msg_iovlen;              # elements in msg_iov
 *       void    *msg_control;            ancillary data
 *       size_t  msg_controllen;          ancillary data buffer len
 *       int     msg_flags;               flags on received message
 *    };
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_sendmsg_tcp(
    sctp_ep_st           *p_ep,
    sctp_msghdr_st       *p_message,
    int                  flags )
{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_error_t     s_err;
    sctp_U32bit      send_aindex = 0; /* sending address index */
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    struct cmsghdr   *p_cmsghdr  = SCTP_NULL;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi;
    sctp_U32bit      context     = 0;
    sctp_U32bit      timetolive  = 0;
    size_t           ret;
    sctp_sndrcvinfo_st *p_send = NULL;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    /*to avoid the warning*/

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_sendmsg_tcp" ) );

    /*
     * Check that the socket is in connected state. There will be just one
     * assoication within the endpoint.
     */
    if( !( p_tcb = sctp_db_get_assoc_from_ep( p_ep ) ) )
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: No Association for endpoint" ) );

        return ( SK_FAILURE );
    }

    /*
     * Association exists check the state.
     */
    if( SCTPSTATE_ESTABLISHED != p_tcb->state )
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: Association not in connected state" ) );

        return ( SK_FAILURE );
    }

    /*
     * Check if we actually have any iovec to send to the peer.
     */
    if( !( p_message->msg_iov ) || !( p_message->msg_iovlen ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: No iovecs to send to the peer" ) );

        return ( SK_FAILURE );
    }

    /*
     * Check if the msghdr structure msg_name field and length is valid.
     */
    if( p_message->msg_name )
    {
        if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );

        }

#ifdef SCTP_INET6_SUPPORT

        else if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET6 ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in6 ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

#endif
    }

    /*
     * Convert the address from the sockaddr storage format to the sctp format
     * and then check that the address must be a part of the association and
     * send the current message on this address. If the address is not a part
     * of the assoc then give a SCTP_SEND_FAILED notification.
     */
    if( p_message->msg_name )
    {
        sctp_sockaddr_st sockaddr;
        sctp_port_t      port = 0;

        if( SCTP_FAILURE ==
                sctp_conv_addr_os_to_sctp( 1, ( struct sockaddr * )
                                           p_message->msg_name, &sockaddr, &port, &s_err ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendmsg_tcp: couldnot convert address " ) );

            return ( SK_FAILURE );
        }

        /*
         * Get the index of the address in the tcb
         * The port is ignored. we don't even validate it.
         */
        if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb,
                                                          &sockaddr, &send_aindex ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendmsg_tcp:addr not in the association: giving send failed"
                          "notification" ) );

            sctp_ntfy_send_failure( p_tcb,
                                    0, /* Unknown context */
                                    SCTP_DATA_UNSENT,
                                    EINV_ADDRS,/* Cause for failure: Invalid address */
                                    0,
                                    SCTP_NULL );

            return ( SK_FAILURE );
        }
    }

    else
    {
        /* Select the primary address index */
        send_aindex =  p_tcb->transmit.primary_address_index;
    }

    /* Here we have the address index to which the data is to be sent. */
    /*
     * Now work on the ancillary data: If its initialization data then
     * return failure. It may happen that we donot have any ancillary data
     * also.
     */
    p_cmsghdr = ( struct cmsghdr * ) p_message->msg_control;

    if( p_message->msg_control )
    {
        p_send = ( sctp_sndrcvinfo_st * )( p_cmsghdr + 1 );

        if( ( IPPROTO_SCTP != p_cmsghdr->cmsg_level )
                || ( SCTP_SNDRCV !=  p_cmsghdr->cmsg_type )
                || ( p_cmsghdr->cmsg_len
                     < ( sizeof( struct cmsghdr ) + sizeof( sctp_sndrcvinfo_st ) ) ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendmsg_tcp: the control info is not correct" ) );

            return ( SK_FAILURE );
        }
    }

    if( !p_send && p_tcb->config.sndrcvinfo_valid )
    {
        p_send = &( p_tcb->config.def_sndrcvinfo );
    }

    if( p_send )
    {

        /* Extract the sendrcv info parameters */
        /*
         * The Flags parameter cannot be SCTP_MSG_ABORT, SCTP_MSG_EOF
         * MSG_ADDR_OVER flag is ignored.
         */
        if( ( p_send->sinfo_flags & SCTP_MSG_ABORT )
                || ( p_send->sinfo_flags & SCTP_MSG_EOF ) )
        {
            sk_errno = SK_EOPNOTSUPP;

            return ( SK_FAILURE );
        }

        /* Is ordered delivery asked for ? */
        ordered = ( 0 == ( p_send->sinfo_flags & SCTP_MSG_UNORDERED ) );
        stream  = p_send->sinfo_stream;
        ppi     = p_send->sinfo_ppid;

        timetolive = p_send->sinfo_timetolive;

        /* The Message context */
        context = p_send->sinfo_context;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: sendrcvinfo extracted successfully." ) );
    }

    else
    {
        /* The cmsghdr is NULL: choose the default values. */

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: Couldnot find sendrcvinfo:selecting default" ) );

        ordered = SCTP_TRUE;
        ppi     = p_ep->ppi;
        stream  = 0;
        context = 0;
        timetolive = 0;
    }

    /*
     * Now traverse the iovec and send the data. chk if blocking send is
     * enabled.
     */
    ret = sctp_send_iovec( p_tcb, p_message, send_aindex, ordered, ppi, stream,
                           context, timetolive );

    return ( ret );
}

/****************************************************************************
 *    FUNCTION    : sctp_sendmsg_udp
 *
 *    DESCRIPTION : The  sendmsg()  function sends a message through a
 *    connectionless-mode socket. The msg is send to the destination address in
 *    msghdr structure. If a connection does not exist to the destination a new
 *    sctp association is initiated.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_sendmsg_udp(
    sctp_ep_st           *p_ep,
    sctp_msghdr_st       *p_message,
    int                  flags )
{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_tcb_st      *p_tcb_temp = SCTP_NULL;
    sctp_error_t     s_err;
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    struct cmsghdr   *p_cmsghdr  = SCTP_NULL;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi         = 0;
    sctp_U32bit      context     = 0;
    sctp_U32bit      timetolive  = 0;
    sctp_sockaddr_st sockaddr;
    sctp_port_t      port        = 0;
    sctp_U32bit      send_aindex = 0;
    sctp_sndrcvinfo_st *p_send = NULL;
    int              ret         = SK_FAILURE;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    /*to avoid the warning*/



    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_sendmsg_udp" ) );

    if( ( p_message == SCTP_NULL ) ||
            ( ( !p_message->msg_iov || p_message->msg_iovlen == 0 ) &&
              ( !p_message->msg_control ) ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendmsg_udp: No iovecs to send to the peer" ) );

        return ( SK_FAILURE );

    }

    /*
     * Try to connect to the destination.
     */
    if( p_message->msg_control )
    {
        p_cmsghdr = ( struct cmsghdr * ) p_message->msg_control;

        switch( p_cmsghdr->cmsg_type )
        {
            case SCTP_SNDRCV:
                {
                    p_send = ( sctp_sndrcvinfo_st * )( p_cmsghdr + 1 );

                    if( ( IPPROTO_SCTP != p_cmsghdr->cmsg_level )
                            || ( p_cmsghdr->cmsg_len
                                 < ( sizeof( struct cmsghdr ) + sizeof( sctp_sndrcvinfo_st ) ) ) )
                    {
                        sk_errno = SK_EINVAL;

                        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                    ( "sctp_sendmsg_udp: the control info is not correct" ) );

                        return ( SK_FAILURE );
                    }
                }
                break;

            case SCTP_INIT:
                {
                    sctp_initmsg_st    *p_init
                        = ( sctp_initmsg_st * )( p_cmsghdr + 1 );

                    /*
                     * Initialize the ep with the init parameters
                     * validation check on the parameters.
                     */
                    if( p_init->sinit_max_instreams != 0 )
                    {
                        p_ep->sk_config.num_in_streams   = p_init->sinit_max_instreams;
                    }

                    if( p_init->sinit_num_ostreams != 0 )
                    {
                        p_ep->sk_config.num_out_streams  = p_init->sinit_num_ostreams;
                    }

                    if( p_init->sinit_max_attempts != 0 )
                    {
                        p_ep->sk_config.max_init_retrans = p_init->sinit_max_attempts;
                    }

                    if( p_init->sinit_max_init_timeo != 0 )
                    {
                        p_ep->sk_config.init_timeo = p_init->sinit_max_init_timeo;
                    }

                }
                break;

            default:
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_sendmsg_udp: the control info is not correct" ) );

                return ( SK_FAILURE );
        }
    }

    if( p_send && ( p_send->sinfo_flags & SCTP_MSG_SENDALL ) )
    {
        return sctp_send_sendall( p_ep, p_message, p_send );
    }

    else
    {
        /*
         * Convert the address from the sockaddr storage format to the sctp format
         */
        if( !p_message->msg_name )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendmsg_udp: No destination address given" ) );

            sk_errno = SK_EINVAL;

            return ( SK_FAILURE );
        }

        else
        {
            if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in ) ) )
            {
                sk_errno = SK_EINVAL;
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Service User API ERROR: Invalid addresses" ) );

                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            sk_errno, "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                return ( SK_FAILURE );

            }

#ifdef SCTP_INET6_SUPPORT

            else if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET6 ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in6 ) ) )
            {
                sk_errno = SK_EINVAL;
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Service User API ERROR: Invalid addresses" ) );

                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            sk_errno, "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                return ( SK_FAILURE );
            }

#endif

            /*SPR 5092*/
            if( ( SCTP_FAILURE ==
                    sctp_conv_addr_os_to_sctp( 1, ( struct sockaddr * )
                                               p_message->msg_name, &sockaddr, &port, &s_err ) ) ||
                    ( port == 0 ) )
                /*SPR 5092*/
            {
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_sendmsg_udp: couldnot convert address " ) );

                return ( SK_FAILURE );
            }
        }



        /* Changes for CSR: 1-5465175 */
        if( SCTP_FAILURE == sctp_db_is_tcb_in_ep( p_ep, port, 1, &sockaddr,
                                                  &p_tcb_temp, &s_err ) )
        {
            if( ( p_send ) && ( ( p_send->sinfo_flags & SCTP_MSG_EOF ) || ( p_send->sinfo_flags & SCTP_MSG_ABORT ) ) )
            {
                sk_errno = SK_ENOCONN;
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            sk_errno, "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "Connection does not exist" ) );
                return ( SK_FAILURE );

            }
        }

        /* Changes for CSR: 1-5465175 Ends. */

        /* Attempt a connection if not already connected */
        sctp_connect_udp( p_ep, &sockaddr, port, 1 );

        /*
         * We attempted the connection but are not sure whether it was successful.
         * So we try to locate an established connection in the endpoint.
         */
        if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_ep, port, 1, &sockaddr,
                                                  &p_tcb, &s_err ) )
        {
            /*
             * The Endpoint already has a connection to the said destination.
             * Check the connection state. if shutdown has received then return
             * failure
             * Check whether socket is Write Bloked
             */
            if( ( p_tcb->state != SCTPSTATE_ESTABLISHED )
                    && ( p_tcb->state != SCTPSTATE_COOKIEWAIT )
                    && ( p_tcb->state != SCTPSTATE_COOKIESENT ) )
            {
                sk_errno = SK_ESHUTDOWN;

                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                            "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sendmsg: The socket is Write Blocked" ) );

                return ( SK_FAILURE );
            }

            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendmsg_udp:Connection was successfully made: try sending" ) );
        }

        else
        {
            SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                        ( "sctp_sendmsg_udp: Association Cannot be connected" ) );

            /* Changes for CSR#1-4248072 */
            if( SK_EADDRINUSE != sk_errno )
            {
                sk_errno = SK_ECONNREFUSED;
            }

            /* Changes for CSR#1-4248072 Ends */
            return ( SK_FAILURE );
        }

        /*
         * Now work on the ancillary data: If its initialization data then
         * return failure. It may happen that we donot have any ancillary data
         * also.
         */

        send_aindex =  p_tcb->transmit.primary_address_index;

        if( !p_send && p_tcb->config.sndrcvinfo_valid )
        {
            p_send = &( p_tcb->config.def_sndrcvinfo );
        }

        if( p_send )
        {
            /* Extract the sndrcv info parameters */
            /* Check if we need to ABORT the association */
            if( p_send->sinfo_flags & SCTP_MSG_ABORT )
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                            ( "sctp_sendmsg_udp: Abort the association " ) );

                if( p_message->msg_iov && p_message->msg_iovlen )
                {
                    p_tcb->event.cmd_abort.cause_length =
                        ( sctp_U16bit )p_message->msg_iov[0].iov_len;
                    p_tcb->event.cmd_abort.cause_info   =
                        ( sctp_U8bit * )p_message->msg_iov[0].iov_base;
                }

                else
                {
                    p_tcb->event.cmd_abort.cause_length = 0;
                }

                sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDABORT );

                return ( SK_SUCCESS );
            }

            /* Check if we need to shutdown the association */
            else if( p_send->sinfo_flags & SCTP_MSG_EOF )
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                            ( "sctp_sendmsg_udp: shutdown the association " ) );

                sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );

                return ( SK_SUCCESS );
            }

            /* Check if MSG_ADDR_OVER is set */
            if( p_send->sinfo_flags & SCTP_MSG_ADDR_OVER )
            {
                /* find the index of the address passed in the tcb */
                if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb,
                                                                  &sockaddr, &send_aindex ) )
                {
                    sk_errno = SK_EINVAL;

                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "sctp_sendmsg_udp: This can never happen" ) );

                    return ( SK_FAILURE );
                }
            }

            /* Is ordered delivery asked for ? */
            ordered = ( p_send->sinfo_flags & SCTP_MSG_UNORDERED ) ?
                      SCTP_FALSE : SCTP_TRUE;

            stream  = p_send->sinfo_stream;
            ppi     = p_send->sinfo_ppid;

            timetolive = p_send->sinfo_timetolive;

            /*dont Set the ppi in the ep
             */

            context = p_send->sinfo_context;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendmsg_udp: sendrcvinfo extracted successfully." ) );

        }

        else
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendmsg_udp: Couldnot find sendrcvinfo:selecting default" ) );

            ordered = SCTP_TRUE;
            ppi     = p_ep->ppi;
            stream  = 0;
            context = 0;
            timetolive = 0;
        }

        /*SPR 5094*/
        /*
         * Check if we actually have any iovec to send to the peer.
         */
        if( !( p_message->msg_iov ) || !( p_message->msg_iovlen ) )
        {
            /* This implies that control information was present */
            return ( SK_SUCCESS );
        }

        /*SPR 5094*/
        /*
         * Now traverse the iovec and send the data. chk if blocking send is
         * enabled.
         */
        ret = sctp_send_iovec( p_tcb, p_message, send_aindex, ordered, ppi, stream,
                               context, timetolive );

        /*
         * Stop the autoclose timer and start it again if AUTOCLOSE option is set
         * for the socket and the send was successful.
         */
        if( ( ret >= 0 ) && SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
        {
            sctp_U32bit timerintrvl = 0;

            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_sendmsg_udp: send success: starting Autoclose timer" ) );

            /* Stop the autoclose timer. */
            sctp_stop_timer( &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                             SCTP_NULL, SCTP_NULL );

            /* Calculate the timer interval for autoclose timer */
            timerintrvl = p_tcb->p_ep->aclose_time * 1000;

            /* Start the autoclose timer. */
            sctp_start_timer( p_tcb,
                              SCTPEVENT_TIMERAUTOCLOSE, timerintrvl, 0,
                              &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                              SCTP_NULL );
        }

    }

    return ( ret );
}
/****************************************************************************
 *    FUNCTION    : sctp_sendx_tcp
 *
 *    DESCRIPTION : The  sctp_sendx()  function sends a message through a
 *    connection-mode. If the socket is connection-mode, the destination
 *    address in msghdr is ignored.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_sendx_tcp(
    sctp_ep_st                      *p_ep,
    void                            *msg,
    size_t                          len,
    struct sockaddr                 *addrs,
    int                             addrcnt,
    const struct sctp_sndrcvinfo    *p_sinfo,
    int                             flags )

{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_U32bit      send_aindex = 0; /* sending address index */
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi;
    sctp_U32bit      context     = 0;
    sctp_U32bit      timetolive  = 0;
    size_t           ret;
    sctp_msghdr_st message;
    sctp_iovec_st  p_iovec[1];


    /*To avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    SCTP_UNUSED_VARIABLE( addrcnt );
    /*to avoid the warning*/



    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_sendx_tcp" ) );

    p_iovec[0].iov_base = ( char * )msg;
    p_iovec[0].iov_len  = len;

    message.msg_iov         = ( struct iovec * )p_iovec;
    message.msg_iovlen      = 1;
    addrs = ( void * )addrs;

    /*
     * Check that the socket is in connected state. There will be just one
     * assoication within the endpoint.
     */
    if( !( p_tcb = sctp_db_get_assoc_from_ep( p_ep ) ) )
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: No Association for endpoint" ) );

        return ( SK_FAILURE );
    }

    /*
     * Association exists check the state.
     */
    if( SCTPSTATE_ESTABLISHED != p_tcb->state )
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: Association not in connected state" ) );

        return ( SK_FAILURE );
    }

    /* Select the primary address index */
    send_aindex =  p_tcb->transmit.primary_address_index;

    if( !p_sinfo && p_tcb->config.sndrcvinfo_valid )
    {
        p_sinfo = &( p_tcb->config.def_sndrcvinfo );
    }

    if( p_sinfo )
    {

        /* Extract the sendrcv info parameters */
        /*
         * The Flags parameter cannot be SCTP_MSG_ABORT, SCTP_MSG_EOF
         * MSG_ADDR_OVER flag is ignored.
         */
        if( ( p_sinfo->sinfo_flags & SCTP_MSG_ABORT )
                || ( p_sinfo->sinfo_flags & SCTP_MSG_EOF ) )
        {
            sk_errno = SK_EOPNOTSUPP;

            return ( SK_FAILURE );
        }

        /* Is ordered delivery asked for ? */
        ordered = ( 0 == ( p_sinfo->sinfo_flags & SCTP_MSG_UNORDERED ) );
        stream  = p_sinfo->sinfo_stream;
        ppi     = p_sinfo->sinfo_ppid;
        timetolive = p_sinfo->sinfo_timetolive;

        /* The Message context */
        context = p_sinfo->sinfo_context;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendx_tcp: sendrcvinfo extracted successfully." ) );
    }

    else
    {

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendmsg_tcp: Couldnot find sendrcvinfo:selecting default" ) );

        ordered = SCTP_TRUE;
        ppi     = p_ep->ppi;
        stream  = 0;
        context = 0;
        timetolive = 0;
    }

    /*
     * Now traverse the iovec and send the data. chk if blocking send is
     * enabled.
     */
    ret = sctp_send_iovec( p_tcb, &message, send_aindex, ordered, ppi, stream,
                           context, timetolive );

    return ( ret );

}
/****************************************************************************
 *    FUNCTION    : sctp_sendx_udp
 *
 *    DESCRIPTION : The  sctp_sendx()  function sends a message through a
 *    connectionless-mode socket. If a connection does not exist to
 *    the destinations a new sctp association is initiated.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_sendx_udp(
    sctp_ep_st                      *p_ep,
    void                            *p_msg,
    size_t                          len,
    struct sockaddr                 *addrs,
    int                             addrcnt,
    const struct sctp_sndrcvinfo    *p_sinfo,
    int                             flags )
{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_error_t     s_err;
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi         = 0;
    sctp_U32bit      context     = 0;
    sctp_U32bit      timetolive  = 0;
    sctp_sockaddr_list_st sctp_addr_list; /*SPR 21141 changes*/
    sctp_port_t      port        = 0;
    sctp_U32bit      send_aindex = 0;
    int              ret         = SK_FAILURE;
    sctp_msghdr_st   message;
    sctp_iovec_st    p_iovec[1];

    p_iovec[0].iov_base = ( char * )p_msg;
    p_iovec[0].iov_len  = len;

    message.msg_iov         = ( struct iovec * )p_iovec;
    message.msg_iovlen      = 1;

    /*To avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_sendx_udp" ) );

    if( p_sinfo && ( p_sinfo->sinfo_flags & SCTP_MSG_SENDALL ) )
    {
        /* call function to send Data to all assoc*/
        return sctp_send_sendall( p_ep, &message, p_sinfo );
    }

    /*
     * Convert the address from the sockaddr storage format to the sctp format
     */
    if( !addrs )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_sendx_udp: No destination address given" ) );

        sk_errno = SK_EINVAL;

        return ( SK_FAILURE );
    }

    /*SPR 21141 changes starts*/
    if( addrcnt > SCTP_MAX_TRANSPORT_ADDR )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_sendx_udp: No destination address given" ) );

        sk_errno = SK_EINVAL;

        return ( SK_FAILURE );
    }

    if( ( SCTP_FAILURE ==
            sctp_conv_addr_os_to_sctp( addrcnt, ( struct sockaddr * )
                                       addrs, sctp_addr_list, &port, &s_err ) ) ||
            ( port == 0 ) )
        /*SPR 5092*/
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_sendx_udp: couldnot convert address " ) );

        return ( SK_FAILURE );
    }

    /*SPR 21141 changes ends*/
    /* Attempt a connection if not already connected */
    sctp_connect_udp( p_ep, sctp_addr_list, port, addrcnt );

    /*
     * We attempted the connection but are not sure whether it was successful.
     * So we try to locate an established connection in the endpoint.
     */
    if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_ep, port, addrcnt, sctp_addr_list,
                                              &p_tcb, &s_err ) )
    {
        /*
         * The Endpoint already has a connection to the said destination.
         * Check the connection state. if shutdown has received then return
         * failure
         * Check whether socket is Write Bloked
         */
        if( ( p_tcb->state != SCTPSTATE_ESTABLISHED )
                && ( p_tcb->state != SCTPSTATE_COOKIEWAIT )
                && ( p_tcb->state != SCTPSTATE_COOKIESENT ) )
        {
            sk_errno = SK_ESHUTDOWN;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sendx: The socket is Write Blocked" ) );

            return ( SK_FAILURE );
        }

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendx_udp:Connection was successfully made: try sending" ) );
    }

    else
    {
        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendx_udp: Association Cannot be connected" ) );

        sk_errno = SK_ECONNREFUSED;
        return ( SK_FAILURE );
    }

    send_aindex =  p_tcb->transmit.primary_address_index;

    if( !p_sinfo && p_tcb->config.sndrcvinfo_valid )
    {
        p_sinfo = &( p_tcb->config.def_sndrcvinfo );
    }

    if( p_sinfo )
    {
        /* Extract the sndrcv info parameters */
        /* Check if we need to ABORT the association */
        if( p_sinfo->sinfo_flags & SCTP_MSG_ABORT )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendx_udp: Abort the association " ) );

            if( message.msg_iov && message.msg_iovlen )
            {
                p_tcb->event.cmd_abort.cause_length =
                    ( sctp_U16bit )message.msg_iov[0].iov_len;
                p_tcb->event.cmd_abort.cause_info   =
                    ( sctp_U8bit * )message.msg_iov[0].iov_base;
            }

            else
            {
                p_tcb->event.cmd_abort.cause_length = 0;
            }

            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDABORT );

            return ( SK_SUCCESS );
        }

        /* Check if we need to shutdown the association */
        else if( p_sinfo->sinfo_flags & SCTP_MSG_EOF )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendx_udp: shutdown the association " ) );

            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );

            return ( SK_SUCCESS );
        }

        /* Check if MSG_ADDR_OVER is set */
        if( p_sinfo->sinfo_flags & SCTP_MSG_ADDR_OVER )
        {
            /* find the index of the address passed in the tcb */
            if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb,
                                                              sctp_addr_list, &send_aindex ) )
            {
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_sendx_udp: This can never happen" ) );

                return ( SK_FAILURE );
            }
        }

        /* Is ordered delivery asked for ? */
        ordered = ( p_sinfo->sinfo_flags & SCTP_MSG_UNORDERED ) ?
                  SCTP_FALSE : SCTP_TRUE;

        stream  = p_sinfo->sinfo_stream;
        ppi = p_sinfo->sinfo_ppid;
        timetolive = p_sinfo->sinfo_timetolive;
        context = p_sinfo->sinfo_context;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendx_udp: sendrcvinfo extracted successfully." ) );

    }

    else
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendx_udp: Couldnot find sendrcvinfo:selecting default" ) );

        ordered = SCTP_TRUE;
        ppi     = p_ep->ppi;
        stream  = 0;
        context = 0;
        timetolive = 0;
    }

    /*
     * Check if we actually have any iovec to send to the peer.
     */
    if( !( p_msg ) || !( len ) )
    {
        /* This implies that control information was present */
        return ( SK_SUCCESS );
    }

    /*
     * Now traverse the iovec and send the data. chk if blocking send is
     * enabled.
     */
    ret = sctp_send_iovec( p_tcb, &message, send_aindex, ordered, ppi, stream,
                           context, timetolive );

    /*
     * Stop the autoclose timer and start it again if AUTOCLOSE option is set
     * for the socket and the send was successful.
     */
    if( ( ret >= 0 ) && SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
    {
        sctp_U32bit timerintrvl = 0;

        SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                    ( "sctp_sendx_udp: send success: starting Autoclose timer" ) );

        /* Stop the autoclose timer. */
        sctp_stop_timer( &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                         SCTP_NULL, SCTP_NULL );

        /* Calculate the timer interval for autoclose timer */
        timerintrvl = p_tcb->p_ep->aclose_time * 1000;

        /* Start the autoclose timer. */
        sctp_start_timer( p_tcb,
                          SCTPEVENT_TIMERAUTOCLOSE, timerintrvl, 0,
                          &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                          SCTP_NULL );
    }

    return ( ret );
}

/****************************************************************************
 *    FUNCTION    : sctp_send_sendall
 *
 *    DESCRIPTION : The  sctp_send_sendall()  function sends a message through a
 *    connectionless-mode socket. it send message to all existing assocs.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_send_sendall(
    sctp_ep_st                      *p_ep,
    sctp_msghdr_st                  *p_message,
    const struct sctp_sndrcvinfo    *p_sinfo )
{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi         = 0;
    sctp_U32bit      context     = 0;
    sctp_U32bit      timetolive  = 0;
    sctp_U32bit      send_aindex = 0;
    int              ret         = SK_FAILURE;
    int              assoc_count = 0;
    int              i_count;

    if( 0 == p_ep->assoc_list.count )
    {
        return ( SCTP_FAILURE );
    }

    assoc_count = p_ep->assoc_list.count;

    /*Extract the first TCB from the association list*/
    p_tcb = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );

    for( i_count = 0; i_count < assoc_count; i_count++ )
    {

        send_aindex =  p_tcb->transmit.primary_address_index;

        if( p_sinfo )
        {
            /* Check if we need to ABORT the association */
            if( p_sinfo->sinfo_flags & SCTP_MSG_ABORT )
            {
                if( p_message->msg_iov && p_message->msg_iovlen )
                {
                    p_tcb->event.cmd_abort.cause_length =
                        ( sctp_U16bit )p_message->msg_iov[0].iov_len;
                    p_tcb->event.cmd_abort.cause_info   =
                        ( sctp_U8bit * )p_message->msg_iov[0].iov_base;
                }

                else
                {
                    p_tcb->event.cmd_abort.cause_length = 0;
                }

                sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDABORT );

                /*Extract the next TCB in the list*/
                p_tcb = ( sctp_tcb_st * )lstNext( ( NODE * )p_tcb );

                if( p_tcb == SCTP_NULL )
                {
                    /*In this case there is only one tcb exist, hence delete
                     *one also. This is applicable to ABORT only, bcos in this
                     *case tcb is deleted */

                    p_tcb = ( sctp_tcb_st * )lstFirst( &p_ep->assoc_list );
                }

                continue;
            }

            /* Check if we need to shutdown the association */
            else if( p_sinfo->sinfo_flags & SCTP_MSG_EOF )
            {
                /*Call the state machine*/
                sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );
                p_tcb = ( sctp_tcb_st * )lstNext( ( NODE * )p_tcb );
                continue;
            }

            /* Is ordered delivery asked for ? */
            ordered = ( p_sinfo->sinfo_flags & SCTP_MSG_UNORDERED ) ?
                      SCTP_FALSE : SCTP_TRUE;
            stream  = p_sinfo->sinfo_stream;
            ppi     = p_sinfo->sinfo_ppid;
            timetolive = p_sinfo->sinfo_timetolive;
            context = p_sinfo->sinfo_context;
        } /*End of psend check*/

        else
        {
            /*Use default parameter*/
            ordered = SCTP_TRUE;
            ppi     = p_ep->ppi;
            stream  = 0;
            context = 0;
            timetolive = 0;
        }

        if( !( p_message->msg_iov ) || !( p_message->msg_iovlen ) )
        {
            /* This implies that control information was present */
            continue;
        }

        ret = sctp_send_iovec( p_tcb, p_message, send_aindex,
                               ordered, ppi, stream, context, timetolive );

        if( ( ret >= 0 ) && SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
        {
            sctp_U32bit timerintrvl = 0;

            SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                        ( "sctp_send_sendall: send success: starting Autoclose timer" ) );

            /* Stop the autoclose timer. */
            sctp_stop_timer( &p_tcb->     assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                             SCTP_NULL, SCTP_NULL );
            /* Calculate the timer interval for autoclose timer */
            timerintrvl = p_tcb->p_ep->aclose_time * 1000;
            /* Start the autoclose timer. */
            sctp_start_timer( p_tcb,
                              SCTPEVENT_TIMERAUTOCLOSE, timerintrvl, 0,
                              &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                              SCTP_NULL );
        }

        p_tcb = ( sctp_tcb_st * )lstNext( ( NODE * )p_tcb );
    }

    return ( SK_SUCCESS );
}
/****************************************************************************
 *    FUNCTION    : sctp_send_iovec
 *
 *    DESCRIPTION : Used to send the iovec elements to the destination in
 *    TCP style and UDP style calls of sendmsg API. Each sendrcv element
 *    corresponds to a data item.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_send_iovec(
    sctp_tcb_st     *p_tcb,
    sctp_msghdr_st  *p_message,
    sctp_U32bit     addr_index,
    sctp_Boolean_t  ordered,
    sctp_U32bit     ppi,
    sctp_stream_t   stream,
    sctp_U32bit     context,
    sctp_U32bit     timetolive )
{
    int              ret      = 0;
    sctp_error_t     s_err;

    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC, ( "sctp_send_iovec" ) );

    /* By now the socket MUST be in connected state */

    /* send the data */
    do
    {
        ret = sctp_send_general(
                  p_tcb,
                  &( p_tcb->transmit.remote[addr_index].that_address ),
                  p_message->msg_iovlen,
                  ( sctp_iovec_st * )p_message->msg_iov,
                  stream,
                  timetolive,
                  ppi,
                  !( SCTP_SOCK_IS_NODELAY( p_tcb->p_ep->ulpqueue.ntfy_flags ) ),
                  ordered,
                  context,
                  NULL,
                  &s_err );

        if( 0 == ret )
        {
            if( E_NORESOURCES != s_err )
            {
                /* Some other error occured : return failure */
                sk_errno = SK_EINVAL;

                return ( SK_FAILURE );
            }

            else
            {
                /* Check if we need to block. */
                sctp_U32bit      ep_id    = p_tcb->p_ep->ep_id;
                sctp_assoc_t     assoc_id = p_tcb->assoc_id;
                sctp_ep_st       *p_ep    = p_tcb->p_ep;

                /*SPR 5090*/
                /*for now, don't block*/
                if( SCTP_SOCK_IS_BLOCKING( p_ep->flags ) && SCTP_FALSE )
                    /*SPR 5090*/
                {
                    SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "sctp_send_iovec: Blocking till we have buffers" ) );

                    /* Sleep till we have buffers to send */
                    /* Put the Current Thread to sleep till ready for write. */
                    SCTP_SLEEP( p_ep->cond, p_tcb->p_ep->lock );

                    /* Get the ptr again */
                    p_ep = sctp_db_get_ep_ptr( ep_id );

                    if( p_ep )
                    {
                        p_tcb = sctp_db_get_association_ptr( ep_id, assoc_id );
                    }

                    if( !p_ep || !p_tcb )
                    {
                        SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_INTF_TRC,
                                    ( "sctp_send_iovec:Couldnot get assoc after sleep" ) );

                        return ( SK_FAILURE );
                    }

                    SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                                ( "sctp_send_iovec: Wokenup after sleeping for send" ) );
                    continue;
                }

                else
                {
                    SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                                ( "sctp_send_iovec: Non blocking socket" ) );

                    sk_errno = SK_EAGAIN;
                    return ( SK_FAILURE );
                }
            }
        }
    }
    while( ret == 0 );

    return ret;
}


/****************************************************************************
 *    FUNCTION    : sctp_recvmsg_tcp
 *
 *    DESCRIPTION : The recvmsg() from a tcp style socket.
 *
 *    RETURN VALUE: Upon successful completion, recvmsg() returns the length  of
 *    the  message  in  bytes.  If no messages are available to be
 *    received and the peer has  performed  an  orderly  shutdown,
 *    recvmsg() returns 0.  Otherwise, -1 is returned and errno is
 *    set to indicate the error.
 ****************************************************************************/
size_t
sctp_recvmsg_tcp(
    sctp_ep_st           *p_ep,
    sctp_msghdr_st       *p_message,
    int                  flags )
{
    struct iovec       *p_iovec         = p_message->msg_iov;
    sctp_sndrcvinfo_st *p_sndrcv        = SCTP_NULL;
    struct sockaddr    *p_retaddr = SCTP_NULL;
    sctp_sockaddr_st   daddr;
    sctp_tcb_st        *p_tcb           = SCTP_NULL;
    size_t             ret              = 0;
    struct cmsghdr     *p_cmsghdr       = SCTP_NULL;
    sctp_U32bit        msg_len = 0;
    /* SPR 21107 changes */
    sctp_error_t       ecode = E_NOERROR;
    /* SPR 21107 changes */

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_recvmsg_tcp" ) );

    p_message->msg_flags = 0;

    /*
     * Before we get on with reading the data we need to check if we have
     * any notifications on the notification queue. If yes they are "always"
     * returned on a priority.
     */
    if( p_ep->ulpqueue.n_ind > 0 )
    {
        if( SK_SUCCESS == sctp_chk_notification( p_ep, p_message ) )
        {
            /* Update the readable status of the endpoint. */
            sctp_db_update_readable_status( p_ep );

            return ( SK_SUCCESS );
        }

        /* No notification extracted : try returning data */
    }

    /*
     * check wether socket is listener or not
     */

    if( SCTP_SOCK_IS_LISTENER( p_ep->flags )
            && ( SCTP_SOCK_STREAM == p_ep->sk_type ) )
    {
        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvmsg:Listener socket" ) );

        return ( SK_FAILURE );
    }

    /* SPR 4074 */
    /*
     * check whether the socket is Read Blocked by  shutdown call
     */

    if( SCTP_SOCK_IS_RD_BLOCKED( p_ep->flags ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvmsg: The socket is Read Blocked" ) );

        return ( SK_FAILURE );
    }

    /* Get the pointer to the control info */
    /* SPR 4058 */
    if( p_message->msg_control &&
            ( p_message->msg_controllen >=
              ( sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr ) ) )
            && ( SCTP_SOCK_IS_RECVDATAIO_ON( p_ep->ulpqueue.ntfy_flags ) ) )
    {
        p_cmsghdr = ( struct cmsghdr * )p_message->msg_control;
        p_sndrcv  = ( sctp_sndrcvinfo_st * )
                    ( ( char * )p_message->msg_control + sizeof( struct cmsghdr ) );
    }

    /* Get a pointer to the return address of the message */
    if( p_message->msg_name )
    {
        if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );

        }

#ifdef SCTP_INET6_SUPPORT

        else if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET6 ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in6 ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

#endif

        p_retaddr = ( struct sockaddr * )p_message->msg_name;
    }

    /*
     * Only one TCB within the endpoint extract it.
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_tcb ||
            ( SCTPSTATE_CLOSED == p_tcb->state ) ||
            ( SCTPSTATE_COOKIEWAIT == p_tcb->state ) ||
            ( SCTPSTATE_COOKIESENT == p_tcb->state ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_recvmsg_tcp: No TCB within the endpoint " ) );

        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvmsg_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* These flags are not supported */
    if( ( flags & MSG_OOB ) || ( flags & MSG_WAITALL ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvmsg_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* Check the msghdr structure. */
    if( !p_message->msg_iov || ( !p_message->msg_iovlen ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvmsg_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_recvmsg_tcp: attempting read" ) );

    ret = sctp_receive_blocking_ext( p_tcb, ( sctp_iovec_st * )p_iovec,
                                     p_message->msg_iovlen, &msg_len,
                                     flags | SCTP_MSG_NOTIFICATION, &( daddr ), p_sndrcv,
                                     ( sctp_U32bit * ) & ( p_message->msg_flags ),
                                     SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &ecode );



    if( SK_SUCCESS == ret )
    {
        /*no data, notification is pending*/
        if( SK_SUCCESS == sctp_chk_notification( p_ep, p_message ) )
        {
            /* Update the readable status of the endpoint. */
            sctp_db_update_readable_status( p_ep );

            return ( SK_SUCCESS );
        }
    }

    if( 0 < ret )
    {
        /* Set the cmsghdr fileds */
        if( p_cmsghdr )
        {
            p_cmsghdr->cmsg_level = IPPROTO_SCTP;
            p_cmsghdr->cmsg_type  = SCTP_SNDRCV;
            p_cmsghdr->cmsg_len   = sizeof( struct cmsghdr ) +
                                    sizeof( sctp_sndrcvinfo_st );
        }

        /* Do we need to convert the sockaddr to storage format */
        if( p_retaddr )
        {
            /*
             * we whould return the source address of the message incase the
             * message is not from the primary destination address. But if
             * msgname field is not null we return the address whatever be
             * the case i.e. even if its the primary address.
             */
            SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_retaddr ) ) =
                ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

            sctp_conv_addr_sctp_to_sock_os( 1, &( daddr ), p_retaddr,
                                            p_tcb->transmit.that_port, SCTP_NULL );
        }

        /* Clear one data indication that was read. */
        if( !( flags & MSG_PEEK ) )
        {
            if( MSG_EOR & p_message->msg_flags )
            {
                sctp_db_clear_data_ind( p_tcb, 1 );
            }
        }

    }

    /*
     * If we're returning a data then we also need to check the
     * readable status of the enpoint. This is important as select uses this.
     * Not updating the readable status after a recvmsg can lead to redundant
     * reads which can be catastrophic in case the endpoint is blocking.
     */
    sctp_db_update_readable_status( p_ep );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_recvmsg_udp
 *
 *    DESCRIPTION : The  recvmsg() from a udp style socket.
 *
 *    RETURN VALUE: Upon successful completion, recvmsg() returns the length of
 *    the  message  in  bytes.  If no messages are available to be
 *    received and the peer has  performed  an  orderly  shutdown,
 *    recvmsg() returns 0.  Otherwise, -1 is returned and errno is
 *    set to indicate the error.
 ****************************************************************************/
size_t
sctp_recvmsg_udp(
    sctp_ep_st           *p_ep,
    sctp_msghdr_st       *p_message,
    int                  flags )
{
    struct iovec       *p_iovec         = p_message->msg_iov;
    sctp_sndrcvinfo_st *p_sndrcv        = SCTP_NULL;
    struct sockaddr    *p_retaddr = SCTP_NULL;
    sctp_sockaddr_st   daddr;
    sctp_data_indication_st   *p_dataind = SCTP_NULL;
    sctp_tcb_st        *p_tcb           = SCTP_NULL;
    size_t             ret              = 0;
    struct cmsghdr     *p_cmsghdr       = SCTP_NULL;
    sctp_U32bit        msg_len = 0;
    /* SPR 21107 changes */
    sctp_error_t       ecode = E_NOERROR;

    /* SPR 21107 changes */
    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_recvmsg_udp" ) );

    p_message->msg_flags = 0;

    /*
     * Before we get on with reading the data we need to check if we have
     * any notifications on the notification queue. If yes they are "always"
     * returned on a priority.
     */
    if( p_ep->ulpqueue.n_ind > 0 )
    {
        if( SK_SUCCESS == sctp_chk_notification( p_ep, p_message ) )
        {
            /* Update the readable status of the endpoint */
            sctp_db_update_readable_status( p_ep );

            return ( SK_SUCCESS );
        }

        /* No notification extracted : try returning data */
    }

    /* Get the pointer to the control info */
    /* SPR 4058 */
    if( p_message->msg_control &&
            ( p_message->msg_controllen >=
              ( sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr ) ) )
            && ( SCTP_SOCK_IS_RECVDATAIO_ON( p_ep->ulpqueue.ntfy_flags ) ) )
    {
        p_cmsghdr = ( struct cmsghdr * )p_message->msg_control;

        p_sndrcv = ( sctp_sndrcvinfo_st * )
                   ( ( char * )p_message->msg_control + sizeof( struct cmsghdr ) );
    }

    /* Get a pointer to the return address of the message */
    if( p_message->msg_name )
    {
        if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );

        }

#ifdef SCTP_INET6_SUPPORT

        else if( ( ( ( struct sockaddr * )( p_message->msg_name ) )->sa_family == AF_INET6 ) && ( p_message->msg_namelen < sizeof( struct sockaddr_in6 ) ) )
        {
            sk_errno = SK_EINVAL;
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Service User API ERROR: Invalid addresses" ) );

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            return ( SK_FAILURE );
        }

#endif

        p_retaddr = ( struct sockaddr * )p_message->msg_name;
    }

    /*
     * Check the data pending indication. Take the first node.. Get the assoc
     * id for the association it represents and do a receive on that tcb.
     */
    p_dataind = ( sctp_data_indication_st * )
                lstFirst( &( p_ep->ulpqueue.data_ind_list ) );

    if( p_dataind == SCTP_NULL )
    {
        sk_errno = SK_EAGAIN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvmsg_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    for( ; p_dataind != SCTP_NULL;
            p_dataind = ( sctp_data_indication_st * )lstNext( ( NODE * )p_dataind ) )
    {
        /* Get a pointer to the association that has the data */
        p_tcb = p_dataind->p_assoc;

        /* Try Reading data from the assoc. */
        /* SPR 21107 changes */
        /* CSR 69326 fix starts*/
        if( ( p_tcb ) &&
                ( ( SCTPSTATE_CLOSED     == p_tcb->state ) ||
                  ( SCTPSTATE_COOKIEWAIT == p_tcb->state ) ||
                  ( SCTPSTATE_COOKIESENT == p_tcb->state ) ) )
            /* CSR 69326 fix ends*/
            /* SPR 21107 changes */
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_recvmsg_udp: Invalid TCB on data ind queue :clearing"
                          "queue" ) );

            /* Clear all the data indications for this TCB. */
            sctp_db_clear_data_ind( p_tcb, 0 );

            /* Try to get the next TCB */
            p_tcb = SCTP_NULL;
            continue;
        }

        /* Found a valid Association to read from */
        break;
    }

    /* Check if were able to get a valid TCB */
    if( !p_tcb )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                    ( "sctp_recvmsg_udp: NO TCB available to read from" ) );

        /* Update the readable status of the endpoint */
        sctp_db_update_readable_status( p_ep );

        return ( SK_FAILURE );
    }

    /* These flags are not supported */
    if( ( flags & MSG_OOB ) || ( flags & MSG_WAITALL ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvmsg_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* Check the msghdr structure. */
    if( !p_message->msg_iov || ( !p_message->msg_iovlen ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvmsg_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_recvmsg_udp: attempting read" ) );

    /*Bug Id 22 fix starts*/
    /* ret = sctp_receive_blocking_ext(p_tcb,(sctp_iovec_st *)p_iovec,
                   p_message->msg_iovlen, &msg_len,
                   flags | SCTP_MSG_NOTIFICATION, &(daddr), p_sndrcv,
                   (sctp_U32bit*)&(p_message->msg_flags),
                   SCTP_SOCK_IS_BLOCKING(p_ep->flags), &ecode);*/
    ret = sctp_receive_blocking_ext( p_tcb, ( sctp_iovec_st * )p_iovec,
                                     p_message->msg_iovlen, &msg_len,
                                     flags | SCTP_MSG_NOTIFICATION, &( daddr ), p_sndrcv,
                                     ( sctp_U32bit * ) & ( p_message->msg_flags ),
                                     ( sctp_Boolean_t )SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &ecode );


    /*Bug Id 22 fix ends*/
    if( SK_SUCCESS == ret )
    {
        /*no data, notification is pending*/
        if( SK_SUCCESS == sctp_chk_notification( p_ep, p_message ) )
        {
            /* Update the readable status of the endpoint. */
            sctp_db_update_readable_status( p_ep );

            return ( SK_SUCCESS );
        }
    }

    if( 0 < ret )
    {
        /* Set the cmsghdr fileds */
        if( p_cmsghdr )
        {
            p_cmsghdr->cmsg_level = IPPROTO_SCTP;
            p_cmsghdr->cmsg_type  = SCTP_SNDRCV;
            p_cmsghdr->cmsg_len   = sizeof( struct cmsghdr ) +
                                    sizeof( sctp_sndrcvinfo_st );
        }

        /* Do we need to convert the sockaddr to storage format */
        if( p_retaddr )
        {
            /*
             * we whould return the source address of the message incase the
             * message is not from the primary destination address. But if
             * msgname field is not null we return the address whatever be
             * the case i.e. even if its the primary address.
             */
            SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_retaddr ) ) =
                ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

            sctp_conv_addr_sctp_to_sock_os( 1, &( daddr ), p_retaddr,
                                            p_tcb->transmit.that_port, SCTP_NULL );
        }

        /* Clear one data indication that was read. */
        if( !( flags & MSG_PEEK ) )
        {
            if( MSG_EOR & p_message->msg_flags )
            {
                sctp_db_clear_data_ind( p_tcb, 1 );
            }
        }

    }

    /*
     * If we're returning a data then we also need to check the
     * readable status of the enpoint. This is important as select uses this.
     * Not updating the readable status after a recv can lead to redundant
     * reads which can be catastrophic in case the endpoint is blocking.
     */
    sctp_db_update_readable_status( p_ep );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_chk_notification
 *
 *    DESCRIPTION : Extracts a notification from the endpoint notification
 *    queue if one exists. Else return SK_FAILURE.
 *
 *    RETRUN VALUE: On success returns SK_SUCCESS and returns SK_FAILURE if no
 *    notification is present.
 ****************************************************************************/
size_t
sctp_chk_notification(
    sctp_ep_st           *p_ep,
    sctp_msghdr_st       *p_message )
{
    sctp_ulpqueue_node_st *p_ulpnode = SCTP_NULL;
    sctp_notification_ut  *p_ntfy    = SCTP_NULL;
    sctp_sockaddr_storage_st *p_retaddr = SCTP_NULL;
    sctp_iovec_st            *p_iovec = ( sctp_iovec_st * )p_message->msg_iov;
    sctp_S32bit              iov_ctr;
    sctp_U32bit              buf_len = 0;
    sctp_S32bit              tmp_buf_len = 0;
    sctp_U8bit               *p_buf;

    SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                ( "sctp_chk_notification: Checking notification on endpoint." ) );

    /* Unqueue the first indication and return it to the user */
    p_ulpnode = sctp_db_ulpqueue_rem_ntfy( &( p_ep->ulpqueue ) );

    if( p_ulpnode == NULL )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_shk_notification: Couldnot get a notification : error" ) );

        return ( SK_FAILURE );
    }

    /* Extract the notification */
    p_ntfy = &( p_ulpnode->notification );

    for( iov_ctr = 0; iov_ctr < p_message->msg_iovlen; iov_ctr++ )
    {
        buf_len += p_iovec[iov_ctr].iov_len;

        if( buf_len >= p_ntfy->sn_header.sn_length )
        {
            break;
        }
    }

    if( buf_len < p_ntfy->sn_header.sn_length )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_chk_notifiation: msg_iov info ptr null-insufficient" ) );

        sk_errno = SK_ENOBUFS;
        return ( SK_FAILURE );
    }

    buf_len = p_ntfy->sn_header.sn_length;
    p_buf    = ( sctp_U8bit * )p_ntfy;

    tmp_buf_len = buf_len;

    /* Copy the notification */
    for( iov_ctr = 0; iov_ctr < p_message->msg_iovlen; iov_ctr++ )
    {
        sctp_S32bit iov_copy_len = ( tmp_buf_len > ( sctp_S32bit )p_iovec[iov_ctr].iov_len ) ?
                                   ( sctp_S32bit )p_iovec[iov_ctr].iov_len : tmp_buf_len;

        __MEMCOPY( p_iovec[iov_ctr].iov_base, p_buf, iov_copy_len );

        buf_len -= iov_copy_len;

        if( buf_len == 0 )
        {
            p_iovec[iov_ctr].iov_len = iov_copy_len;
            break;
        }

        p_buf += iov_copy_len;
    } /* for (iov_ctr = 0; iov_ctr < p_... */

    for( iov_ctr += 1; iov_ctr < p_message->msg_iovlen; iov_ctr++ )
    {
        p_iovec[iov_ctr].iov_len = 0;
    }

    /* Get a pointer to the return address of the message */
    if( p_message->msg_name &&
            ( p_message->msg_namelen >= sizeof( sctp_sockaddr_storage_st ) ) )
    {
        p_retaddr = ( sctp_sockaddr_storage_st * )p_message->msg_name;
        /* set the Address into msgname field */
        *p_retaddr = p_ulpnode->peer_addr;
    }

    /* set the MSG_FLAGS to notificaiton */
    p_message->msg_flags |= SCTP_MSG_NOTIFICATION;

    /* Free the ulpqueue notification node */
    sctp_db_free_ntfy_node( p_ulpnode );

    /*
     * If we're returning a notification then we also need to check the
     * readable status of the enpoint. This is important as select uses this.
     * Not updating the readable status after a recvmsg can lead to redundant
     * reads which can be catastrophic in case the endpoint is blocking.
     */
    sctp_db_update_readable_status( p_ep );

    return ( SK_SUCCESS );
}


/****************************************************************************
 *    FUNCTION    : sctp_sendto_tcp
 *
 *    DESCRIPTION : The  sendto()  function sends a message through a
 *    connection-mode. If the socket is connection-mode, the destination
 *    address in destaddr is ignored.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_sendto_tcp(
    sctp_ep_st               *p_ep,
    sctp_U8bit               *message,
    sctp_U32bit              length,
    int                      flags,
    const struct sockaddr    *destaddr,
    size_t                   dest_len )
{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_error_t     s_err;
    sctp_U32bit      send_aindex = 0; /* sending address index */
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi;
    size_t           ret;
    sctp_U32bit      context     = 0;
    sctp_U32bit      lifetime    = 0;
    sctp_iovec_st    p_iovec[1];

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    SCTP_UNUSED_VARIABLE( dest_len );
    /*to avoid the warning*/



    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_sendto_tcp" ) );

    /*
     * Check that the socket is in connected state. There will be just one
     * assoication within the endpoint.
     */
    if( !( p_tcb = sctp_db_get_assoc_from_ep( p_ep ) ) )
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendto_tcp: No Association for endpoint" ) );

        return ( SK_FAILURE );
    }

    /*
     * Association exists check the state.
     */
    if( SCTPSTATE_ESTABLISHED != p_tcb->state )
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendto_tcp: Association not in connected state" ) );

        return ( SK_FAILURE );
    }

    /*
     * Check if we actually have some data to send to the peer.
     */
    if( !( message ) || !( length ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendto_tcp: No Data to send to the peer" ) );

        return ( SK_FAILURE );
    }

    /*
     * Convert the address from the sockaddr storage format to the sctp format
     * and then check that the address must be a part of the association and
     * send the current message on this address. If the address is not a part
     * of the assoc then give a SCTP_SEND_FAILED notification.
     */
    if( destaddr )
    {
        sctp_sockaddr_st sockaddr;
        sctp_port_t      port = 0;

        if( SCTP_FAILURE ==
                sctp_conv_addr_os_to_sctp( 1, ( struct sockaddr * )
                                           destaddr, &sockaddr, &port, &s_err ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendto_tcp: couldnot convert address " ) );

            return ( SK_FAILURE );
        }

        /*
         * Get the index of the address in the tcb
         * The port is ignored. we don't even validate it.
         */
        if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb,
                                                          &sockaddr, &send_aindex ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendto_tcp:addr not in the association: giving send failed"
                          "notification" ) );

            sctp_ntfy_send_failure( p_tcb,
                                    0, /* Unknown context */
                                    SCTP_DATA_UNSENT,
                                    EINV_ADDRS,/* Cause for failure: Invalid address */
                                    0,
                                    SCTP_NULL );

            return ( SK_FAILURE );
        }
    }

    else
    {
        /* Select the primary address index */
        send_aindex =  p_tcb->transmit.primary_address_index;
    }

    /* Here we have the address index to which the data is to be sent. */

    /*check if user has configured default send receive info*/
    if( p_tcb->config.sndrcvinfo_valid )
    {
        sctp_sndrcvinfo_st  *p_send = &p_tcb->config.def_sndrcvinfo;

        /* Extract the sndrcv info parameters */
        /* Check if we need to ABORT the association */
        if( ( p_send->sinfo_flags & SCTP_MSG_ABORT ) ||
                ( p_send->sinfo_flags & SCTP_MSG_EOF ) )
        {
            sk_errno = SK_EINVAL;
            return ( SK_FAILURE );
        }

        /* Check if MSG_ADDR_OVER is set */
        if( p_send->sinfo_flags & SCTP_MSG_ADDR_OVER )
        {
            /* find the index of the address passed in the tcb */
            if( SCTP_NULL == destaddr )
            {
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "ADDROVER flag with no destination specified" ) );

                return ( SK_FAILURE );
            }
        }

        ordered = ( 0 == ( p_send->sinfo_flags & SCTP_MSG_UNORDERED ) );
        ppi     = p_send->sinfo_ppid;
        stream  = p_send->sinfo_stream;
        context = p_send->sinfo_context;
        lifetime = p_send->sinfo_timetolive;
    }

    else
    {
        ordered = SCTP_TRUE;;
        ppi     = p_ep->ppi;
        stream  = 0;
        context = 0;
        lifetime = 0;
    }

    /* By now the socket MUST be in connected state */
    /* send the data */
    p_iovec->iov_len  = length;
    p_iovec->iov_base = ( char * )message;

    if( 0 == ( ret = sctp_send_general(
                         p_tcb,
                         &( p_tcb->transmit.remote[send_aindex].that_address ),
                         1,
                         p_iovec,
                         stream,
                         lifetime,
                         ppi,
                         SCTP_TRUE, /* Bundle flag */
                         ordered,
                         context,
                         NULL,
                         &s_err ) ) )
    {
        if( E_NORESOURCES == s_err )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendto_tcp : no buffers: could block here" ) );

            /*SPR 5090*/
            /* Check if we need to block.
             * as of now, send will not block, we immidiately return error
             */
            if( SCTP_SOCK_IS_BLOCKING( p_tcb->p_ep->flags ) && SCTP_FALSE )
                /*SPR 5090*/
            {
                sctp_U32bit    ep_id     = p_tcb->p_ep->ep_id;
                sctp_U32bit    assoc_id  = p_tcb->assoc_id;

                SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_sendto_tcp: Blocking till we have buffers" ) );

                /* Sleep till we have buffers to send */
                /* Put the Current Thread to sleep till ready for write. */
                SCTP_SLEEP( p_tcb->p_ep->cond, p_tcb->p_ep->lock );

                /* Get the ptr again */
                p_ep = sctp_db_get_ep_ptr( ep_id );

                if( p_ep )
                {
                    p_tcb = sctp_db_get_association_ptr( ep_id, assoc_id );
                }

                if( !p_ep || !p_tcb )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_INTF_TRC,
                                ( "sctp_sendto_tcp:Couldnot get assoc after sleep" ) );

                    return ( SK_FAILURE );
                }

                SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                            ( "sctp_sendto_tcp: Wokenup after seeping for send" ) );
            }

            else
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                            ( "sctp_sendto_tcp: Non blocking socket" ) );

                sk_errno = SK_EAGAIN;
                return ( SK_FAILURE );
            }

        }

        /* Some other error occured : return failure */
        sk_errno = SK_EINVAL;

        /* But if we were able to send some data then ok */
        return ( SK_FAILURE );
    }

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_sendto_udp
 *
 *    DESCRIPTION : The  sendto()  function sends a message through a
 *    connectionless-mode socket. The msg is send to the destination address in
 *    destaddr. If a connection does not exist to the destination a new
 *    sctp association is initiated.
 *
 *    RETURN VALUE: Returns the number of bytes sent on success and (-1) on
 *    error.
 ****************************************************************************/
size_t
sctp_sendto_udp(
    sctp_ep_st              *p_ep,
    sctp_U8bit              *message,
    sctp_U32bit             length,
    int                     flags,
    const struct sockaddr   *destaddr,
    size_t                  dest_len )
{
    sctp_tcb_st      *p_tcb      = SCTP_NULL;
    sctp_error_t     s_err;
    sctp_Boolean_t   ordered     = SCTP_FALSE;
    sctp_stream_t    stream      = 0;
    sctp_U32bit      ppi         = 0;
    sctp_U32bit      context     = 0;
    sctp_sockaddr_st sockaddr;
    sctp_port_t      port        = 0;
    sctp_U32bit      send_aindex = 0;
    sctp_U32bit      lifetime    = 0;
    int              ret         = SK_SUCCESS; /* SPR 4068 */
    sctp_iovec_st    p_iovec[1];

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( flags );
    SCTP_UNUSED_VARIABLE( dest_len );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_sendto_udp" ) );

    /*
     * Check if we actually have any data to send to the peer.
     */
    if( !( message ) || !( length ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendto_udp: No data to send to the peer" ) );

        return ( SK_FAILURE );
    }

    /*
     * Check if the destaddr and dest_len is valid.
     */
    if( !destaddr )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_sendto_udp: The address in destaddr is of invalid length" ) );

        sk_errno = SK_EINVAL;

        return ( SK_FAILURE );
    }

    /*
     * Convert the address from the sockaddr storage format to the sctp format
     */
    if( SCTP_FAILURE ==
            sctp_conv_addr_os_to_sctp( 1, ( struct sockaddr * )
                                       destaddr, &sockaddr, &port, &s_err ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_sendto_udp: couldnot convert address " ) );

        return ( SK_FAILURE );
    }

    /*
     * Try to connect to the destination.
     */

    /* Attempt a connection if not already connected */
    sctp_connect_udp( p_ep, &sockaddr, port, 1 );

    /*
     * We attempted the connection but are not sure whether it was successful.
     * So we try to locate an established connection in the endpoint.
     */
    if( SCTP_SUCCESS == sctp_db_is_tcb_in_ep( p_ep, port, 1, &sockaddr,
                                              &p_tcb, &s_err ) )
    {
        /*
         * The Endpoint already has a connection to the said destination.
         * Check the connection state.
         *
         * Check whether socket is Write Bloked
         */
        if( ( p_tcb->state != SCTPSTATE_ESTABLISHED )
                && ( p_tcb->state != SCTPSTATE_COOKIEWAIT )
                && ( p_tcb->state != SCTPSTATE_COOKIESENT ) )
        {
            sk_errno = SK_ESHUTDOWN;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                        "Service User API ERROR" );
            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sendto: The socket is Write Blocked" ) );

            return ( SK_FAILURE );
        }

        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_sendto_udp:Connection was successfully made: try sending" ) );
    }

    else
    {
        sk_errno = SK_ENOCONN;

        SCTP_TRACE( p_ep, SCTP_ERR_TRC | SCTP_INTF_TRC,
                    ( "sctp_sendto_udp: Association not in connected state" ) );

        return ( SK_FAILURE );
    }


    /*check if user has configured default send receive info*/
    if( p_tcb->config.sndrcvinfo_valid )
    {
        sctp_sndrcvinfo_st  *p_send = &p_tcb->config.def_sndrcvinfo;

        /* Extract the sndrcv info parameters */
        /* Check if we need to ABORT the association */
        if( p_send->sinfo_flags & SCTP_MSG_ABORT )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendmsg_udp: Abort the association " ) );

            p_tcb->event.cmd_abort.cause_length = length;
            p_tcb->event.cmd_abort.cause_info   = message;
            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDABORT );

            return ( SK_SUCCESS );
        }

        /* Check if we need to shutdown the association */
        else if( p_send->sinfo_flags & SCTP_MSG_EOF )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                        ( "sctp_sendmsg_udp: shutdown the association " ) );

            sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDTERM );

            return ( SK_SUCCESS );
        }

        /* Check if MSG_ADDR_OVER is set */
        if( p_send->sinfo_flags & SCTP_MSG_ADDR_OVER )
        {
            /* find the index of the address passed in the tcb */
            if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_tcb,
                                                              &sockaddr, &send_aindex ) )
            {
                sk_errno = SK_EINVAL;

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_sendmsg_udp: This can never happen" ) );

                return ( SK_FAILURE );
            }
        }

        else
        {
            send_aindex =  p_tcb->transmit.primary_address_index;
        }

        ordered = ( 0 == ( p_send->sinfo_flags & SCTP_MSG_UNORDERED ) );
        ppi     = p_send->sinfo_ppid;
        stream  = p_send->sinfo_stream;
        context = p_send->sinfo_context;
        lifetime = p_send->sinfo_timetolive;
    }

    else
    {
        ordered = SCTP_TRUE;
        ppi     = p_ep->ppi;
        stream  = 0;
        context = 0;
        lifetime = 0;
    }

    p_iovec[0].iov_len = length;
    p_iovec[0].iov_base = ( char * )message;

    /* Now send the data */
    if( SCTP_FAILURE == ( ret = sctp_send_general(
                                    p_tcb,
                                    &( p_tcb->transmit.remote[send_aindex].that_address ),
                                    1,
                                    p_iovec,
                                    stream,
                                    lifetime,
                                    ppi,
                                    SCTP_TRUE, /* Bundle flag */
                                    ordered,
                                    context,
                                    NULL,
                                    &s_err ) ) )
    {
        if( E_NORESOURCES == s_err )
        {
            SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_sendto_tcp : no buffers: could block here" ) );

            /* Check if we need to block. */
            /*SPR 5090*/
            /* currnetly send does not block, need to change the design
             * existing depends upon the global p_ep->cond which
             * can be signalled for any event and does not gaurantee
             * buffer availbility.
             * Moreover the err E_NORESOURCES is not returned second time
             */
            if( SCTP_SOCK_IS_BLOCKING( p_tcb->p_ep->flags ) && SCTP_FALSE )
                /*SPR 5090*/
            {
                sctp_U32bit    ep_id     = p_tcb->p_ep->ep_id;
                sctp_U32bit    assoc_id  = p_tcb->assoc_id;

                SCTP_TRACE( p_tcb->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "sctp_sendto_tcp: Blocking till we have buffers" ) );

                /* Sleep till we have buffers to send */
                /* Put the Current Thread to sleep till ready for write. */
                SCTP_SLEEP( p_tcb->p_ep->cond, p_tcb->p_ep->lock );

                /* Get the ptr again */
                p_ep = sctp_db_get_ep_ptr( ep_id );

                if( p_ep )
                {
                    p_tcb = sctp_db_get_association_ptr( ep_id, assoc_id );
                }

                if( !p_ep || !p_tcb )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC | SCTP_INTF_TRC,
                                ( "sctp_sendto_tcp:Couldnot get assoc after sleep" ) );

                    return ( SK_FAILURE );
                }

                SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                            ( "sctp_sendto_tcp: Wokenup after seeping for send" ) );
            }

            else
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                            ( "sctp_sendto_tcp: Non blocking socket" ) );

                sk_errno = SK_EAGAIN;
                return ( SK_FAILURE );
            }
        }

        /* Some other error occured : return failure */
        sk_errno = SK_EINVAL;

        /* But if we were able to send some data then ok */
        return ( SK_FAILURE );
    }

    /*
     * Stop the autoclose timer and start it again if AUTOCLOSE option is set
     * for the socket and the send was successful.
     */
    if( ( ret > 0 ) && SCTP_SOCK_IS_AUTOCLOSE( p_tcb->p_ep->flags ) )
    {
        sctp_U32bit timerintrvl = 0;

        SCTP_TRACE( p_tcb->p_ep, SCTP_DBG_TRC,
                    ( "sctp_sendto_udp: send success: starting Autoclose timer" ) );

        /* Stop the autoclose timer. */
        sctp_stop_timer( &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                         SCTP_NULL, SCTP_NULL );

        /* Calculate the timer interval for autoclose timer */
        timerintrvl = p_tcb->p_ep->aclose_time * 1000;

        /* Start the autoclose timer. */
        sctp_start_timer( p_tcb,
                          SCTPEVENT_TIMERAUTOCLOSE, timerintrvl, 0,
                          &p_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                          SCTP_NULL );
    }

    return ( ret );
}

/****************************************************************************
 *    FUNCTION    : sctp_recvfrom_tcp
 *
 *    DESCRIPTION : The recvfrom() from a tcp style socket.
 *
 *    RETURN VALUE: Upon successful completion, recvfrom() returns the length
 *    of the  message  in  bytes.  If no messages are available to be
 *    received and the peer has  performed  an  orderly  shutdown,
 *    recvfrom() returns 0.  Otherwise, -1 is returned and errno is
 *    set to indicate the error.
 ****************************************************************************/
size_t
sctp_recvfrom_tcp(
    sctp_ep_st               *p_ep,
    sctp_U8bit               *message,
    sctp_U32bit              length,
    int                      flags,
    struct sockaddr          *srcaddr,
    sctp_socklen_t           *src_len )
{
    sctp_sndrcvinfo_st       sndrcv;
    struct sockaddr          *p_retaddr = SCTP_NULL;
    sctp_sockaddr_st         daddr;
    sctp_tcb_st              *p_tcb     = SCTP_NULL;
    size_t                   ret        = 0;
    sctp_U32bit              temp_flags = 0;
    sctp_iovec_st            p_iovec[1];

    /* SPR 21107 changes */
    sctp_error_t             ecode = E_NOERROR;
    /* SPR 21107 changes */

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_recvfrom_tcp" ) );

    /* SPR 4074 */
    if( SCTP_SOCK_IS_RD_BLOCKED( p_ep->flags ) )
    {
        sk_errno = SK_EINVAL;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, sk_errno,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "recvmsg: The socket is Read Blocked" ) );

        return ( SK_FAILURE );
    }


    /* Get a pointer to the return address of the message */
    if( srcaddr && src_len )
    {
        p_retaddr = srcaddr;
    }

    /*
     * Only one TCB within the endpoint extract it.
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_tcb ||
            ( SCTPSTATE_CLOSED == p_tcb->state ) ||
            ( SCTPSTATE_COOKIEWAIT == p_tcb->state ) ||
            ( SCTPSTATE_COOKIESENT == p_tcb->state ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_recvfrom_tcp: No TCB within the endpoint " ) );

        sk_errno = SK_ENOTCONN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvfrom_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    /* These flags are not supported */
    if( ( flags & MSG_OOB ) || ( flags & MSG_WAITALL ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvfrom_tcp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_recvfrom_tcp: attempting read" ) );

    p_iovec[0].iov_base = ( char * )message;
    p_iovec[0].iov_len = length;

    /*Bug Id 22 fix starts*/
    /*ret = sctp_receive_blocking_ext(p_tcb, p_iovec,
                  1, (sctp_U32bit*)&(length),
                  (flags | MSG_TRUNC) & ~SCTP_MSG_NOTIFICATION,
                  &(daddr), &sndrcv, &temp_flags,
                  SCTP_SOCK_IS_BLOCKING(p_ep->flags), &ecode);*/
    ret = sctp_receive_blocking_ext( p_tcb, p_iovec,
                                     1, ( sctp_U32bit * ) & ( length ),
                                     ( flags | MSG_TRUNC ) & ~SCTP_MSG_NOTIFICATION,
                                     &( daddr ), &sndrcv, &temp_flags,
                                     ( sctp_Boolean_t )SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &ecode );
    /*Bug Id 22 fix ends*/


    /* Do we need to convert the sockaddr to storage format */
    if( ( 0 < ret ) && ( SCTP_NULL != p_retaddr ) )
    {
        /*
         * we whould return the source address of the message incase the
         * message is not from the primary destination address. But if
         * srcaddr is not null we return the address whatever be
         * the case i.e. even if its the primary address.
         */
        SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_retaddr ) ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        sctp_conv_addr_sctp_to_sock_os( 1, &( daddr ), p_retaddr,
                                        p_tcb->transmit.that_port, SCTP_NULL );

        if( p_retaddr->sa_family == AF_INET )
        {
            *src_len = sizeof( struct sockaddr_in );
        }

#ifdef SCTP_INET6_SUPPORT

        else
        {
            *src_len = sizeof( struct sockaddr_in6 );
        }

#endif

    }

    /* Clear one data indication that was read. */
    sctp_db_clear_data_ind( p_tcb, 1 );

    /*
     * If we're returning a data then we also need to check the
     * readable status of the enpoint. This is important as select uses this.
     * Not updating the readable status after a recvfrom can lead to redundant
     * reads which can be catastrophic in case the endpoint is blocking.
     */
    sctp_db_update_readable_status( p_ep );

    return ( ret );
}


/****************************************************************************
 *    FUNCTION    : sctp_recvfrom_udp
 *
 *    DESCRIPTION : The  recvfrom() from a udp style socket.
 *
 *    RETURN VALUE: Upon successful completion, recvfrom() returns the length
 *    of the  message  in  bytes.  If no messages are available to be
 *    received and the peer has  performed  an  orderly  shutdown,
 *    recvfrom() returns 0.  Otherwise, -1 is returned and errno is
 *    set to indicate the error.
 ****************************************************************************/
size_t
sctp_recvfrom_udp(
    sctp_ep_st               *p_ep,
    sctp_U8bit               *message,
    sctp_U32bit              length,
    int                      flags,
    struct sockaddr          *srcaddr,
    sctp_socklen_t           *src_len )
{
    sctp_sndrcvinfo_st       sndrcv;
    struct sockaddr          *p_retaddr = SCTP_NULL;
    sctp_sockaddr_st         daddr;
    sctp_data_indication_st  *p_dataind = SCTP_NULL;
    sctp_tcb_st              *p_tcb     = SCTP_NULL;
    size_t                   ret        = 0;
    sctp_U32bit              temp_flags = 0;
    sctp_iovec_st            p_iovec[1];
    /* SPR 21107 changes */
    sctp_error_t             ecode = E_NOERROR;
    /* SPR 21107 changes */

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_recvfrom_udp" ) );

    /* Get a pointer to the return address of the message */
    if( srcaddr && src_len )
    {
        p_retaddr = srcaddr;
    }

    /*
     * Check the data pending indication. Take the first node.. Get the assoc
     * id for the association it represents and do a receive on that tcb.
     */
    p_dataind = ( sctp_data_indication_st * )
                lstFirst( &( p_ep->ulpqueue.data_ind_list ) );

    if( p_dataind == SCTP_NULL )
    {
        sk_errno = SK_EAGAIN;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvfrom_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    for( ; p_dataind != SCTP_NULL;
            p_dataind = ( sctp_data_indication_st * )lstNext( ( NODE * )p_dataind ) )
    {
        /* Get a pointer to the association that has the data */
        p_tcb = p_dataind->p_assoc;

        /* Try Reading data from the assoc. */
        /* SPR 21107 changes */
        /* CSR 69326 fix starts*/
        if( ( p_tcb ) &&
                ( ( SCTPSTATE_CLOSED     == p_tcb->state ) ||
                  ( SCTPSTATE_COOKIEWAIT == p_tcb->state ) ||
                  ( SCTPSTATE_COOKIESENT == p_tcb->state ) ) )
            /* CSR 69326 fix ends*/
            /* SPR 21107 changes */
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "sctp_recvfrom_udp: Invalid TCB on data ind queue :clearing"
                          "queue" ) );

            /* Clear all the data indications for this TCB. */
            sctp_db_clear_data_ind( p_tcb, 0 );

            /* Try to get the next TCB */
            p_tcb = SCTP_NULL;
            continue;
        }

        /* Found a valid Association to read from */
        break;
    }

    /* Check if were able to get a valid TCB */
    if( !p_tcb )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                    ( "sctp_recvfrom_udp: NO TCB available to read from" ) );

        /* Update the readable status of the endpoint */
        sctp_db_update_readable_status( p_ep );

        return ( SK_FAILURE );
    }

    /* These flags are not supported */
    if( ( flags & MSG_OOB ) || ( flags & MSG_WAITALL ) )
    {
        sk_errno = SK_EOPNOTSUPP;

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "sctp_recvfrom_udp: Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SK_FAILURE );
    }

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_recvfrom_udp: attempting read" ) );

    p_iovec[0].iov_base = ( char * )message;
    p_iovec[0].iov_len  = length;

    ret = sctp_receive_blocking_ext( p_tcb, p_iovec,
                                     1, ( sctp_U32bit * ) & ( length ),
                                     ( flags | MSG_TRUNC ) & ~SCTP_MSG_NOTIFICATION,
                                     &( daddr ), &( sndrcv ), &temp_flags,
                                     SCTP_SOCK_IS_BLOCKING( p_ep->flags ), &ecode );

    /* Do we need to convert the sockaddr to storage format */
    if( ( 0 < ret ) && ( SCTP_NULL != p_retaddr ) )
    {
        /*
         * we whould return the source address of the message incase the
         * message is not from the primary destination address. But if
         * srcaddr is not null we return the address whatever be
         * the case i.e. even if its the primary address.
         */
        SOCKADDR_STORAGE_FAMILY( ( *( sctp_sockaddr_storage_st * )p_retaddr ) ) =
            ( p_tcb->p_ep->sk_config.want_mapped ) ? SCTP_AF_INET6 : SCTP_AF_INET;

        sctp_conv_addr_sctp_to_sock_os( 1, &( daddr ), p_retaddr,
                                        p_tcb->transmit.that_port, SCTP_NULL );

        if( p_retaddr->sa_family == AF_INET )
        {
            *src_len = sizeof( struct sockaddr_in );
        }

#ifdef SCTP_INET6_SUPPORT

        else
        {
            *src_len = sizeof( struct sockaddr_in6 );
        }

#endif

    }

    /* Clear one data indication that was read. */
    sctp_db_clear_data_ind( p_tcb, 1 );

    /*
     * If we're returning a data then we also need to check the
     * readable status of the enpoint. This is important as select uses this.
     * Not updating the readable status after a recv can lead to redundant
     * reads which can be catastrophic in case the endpoint is blocking.
     */
    sctp_db_update_readable_status( p_ep );

    return ( ret );
}


#else  /* SCTP_UI_SOCK_API  */
#ifndef SCTP_UI_SU_MESSAGEBASED
#ifndef SCTP_NO_QUEUE

static sctp_notf_queue_st notf_queue;
/****************************************************************************
For thread synchronization
****************************************************************************/
static sctp_lock_t        q_lock;
/* SPR 20285 */
sctp_U32bit   sctp_indq_init_flag = SCTP_FALSE;
/* SPR 20285 */

#ifndef SCTP_INDICATION_THREAD
static sctp_timer_t       sctp_ind_q_timer_id;
#endif


/****************************************************************************
 *    FUNCTION    : sctp_su_init_notification_queue
 *
 *    DESCRIPTION : Initializes s notification queue.
 *
 *    RETURN VALUE: SCTP_SUCCESS/SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_su_init_notification_queue()
{
    /* SPR 20285 */
    if( sctp_indq_init_flag == SCTP_FALSE )
    {
        SCTP_INIT_LOCK( q_lock );
        sctp_indq_init_flag = SCTP_TRUE;
    }

    /* SPR 20285 */

    notf_queue.count = 0;
    notf_queue.is_empty = SCTP_TRUE;

#ifndef SCTP_INDICATION_THREAD

    if( SCTP_FAILURE == sctp_start_timer( SCTP_NULL, SCTPEVENT_TIMER_DEQUE,
                                          SCTPE_DEQUE_TIMEOUT, 0, &sctp_ind_q_timer_id, SCTP_NULL ) )
    {
        return SCTP_FAILURE;
    }

#endif
    return SCTP_SUCCESS;
}



/****************************************************************************
 *    FUNCTION    : sctp_su_deinit_notification_queue
 *
 *    DESCRIPTION : Deinits notification queue.
 *
 *    RETURN VALUE: SCTP_SUCCESS/SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_su_deinit_notification_queue()
{
#ifndef SCTP_INDICATION_THREAD
    sctp_U32bit    time_value;
    sctp_error_t   ecode;
#endif


#ifndef SCTP_INDICATION_THREAD
    sctp_stop_timer( &sctp_ind_q_timer_id, &time_value, &ecode );
#endif

    return SCTP_SUCCESS;
}

/****************************************************************************
 *    FUNCTION    : sctp_add_notification_to_queue
 *
 *    DESCRIPTION : Add a user notification to the queue.
 *
 *    RETURN VALUE: SCTP_SUCCESS/SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_add_notification_to_queue(
    sctp_ep_st    *p_ep,
    sctp_U32bit    event )
{
    sctp_U32bit num_nodes;
    sctp_notf_queue_node_st  *p_node;
    sctp_tcb_st *p_tcb;
    /*check that queue is not full*/
    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_add_notification" ) );

#ifdef SCTP_LOAD_TEST
    gettimeofday( &up_e_time, NULL );
    time_diff = TIME_DIFF( up_e_time, up_s_time );

    if( time_diff < min_uptime )
    {
        min_uptime = time_diff;
    }

    if( time_diff > max_uptime )
    {
        max_uptime = time_diff;
    }

    avg_uptime += time_diff;

    record_uptime = SCTP_FALSE;
#endif

    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL == p_tcb )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "sctp_add_notification: Assoc Already closed" ) );

        return ( SCTP_SUCCESS );
    }

    SCTP_LOCK( q_lock );
    num_nodes = ( ( notf_queue.last + SCTP_MAX_NOTF_Q_SIZE ) - notf_queue.first ) %
                SCTP_MAX_NOTF_Q_SIZE;

    if( num_nodes == SCTP_MAX_NOTF_Q_SIZE - 1 )
    {
        /*With No separate Thread, we make sure that this is never reached*/
        SCTP_TRACE( p_ep, SCTP_INTF_TRC,
                    ( "sctp_add_notification: Queue is Full, Must Increase the Size" ) );
        SCTP_UNLOCK( q_lock );
        return SCTP_FAILURE;
    }

    /* add the info at the end */
    p_node = &notf_queue.nodes_list[notf_queue.last];

    if( SCTP_SUCCESS != sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_node->p_callback_func, &p_node->p_appl_data ) )
    {
        SCTP_UNLOCK( q_lock );
        return SCTP_FAILURE;
    }

    p_node->event  = event;
    p_node->notice = p_tcb->notify;

    notf_queue.last = ( notf_queue.last + 1 ) % SCTP_MAX_NOTF_Q_SIZE;
    notf_queue.is_empty = SCTP_FALSE;
    notf_queue.count++;

#ifdef SCTP_DEQUEUE_IND

    if( ( SCTPNOTIFY_CONGESTION == event ) &&
            ( SCTP_CONGLEVEL_NOCONG != p_tcb->notify.congestion.cong_level ) )
    {
        if( notf_queue.count == 1 )
        {
            p_tcb->p_ep->cong_flag = 1; /* Congestion notification flag */
            SCTP_UNLOCK( q_lock );
            SCTP_SLEEP( p_tcb->p_ep->cond, p_tcb->p_ep->lock );
            SCTP_LOCK( q_lock );
        }
    }

#endif

    SCTP_UNLOCK( q_lock );
    SCTP_SIGNAL_INDICATION();
    return ( SCTP_SUCCESS );
}

/****************************************************************************
 *    FUNCTION    : sctp_remove_notifications_from_queue
 *
 *    DESCRIPTION : Remove a user notification and call the user function
 *
 *    RETURN VALUE: SCTP_SUCCESS/SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_remove_notifications_from_queue()
{
    if( sctp_stack_state != SCTP_STATE_UNINTIALISED )
    {
        sctp_ep_st         *p_ep  = SCTP_NULL;
#ifndef SCTP_INDICATION_THREAD
        sctp_notf_queue_node_st  *p_node;

        if( notf_queue.is_empty == SCTP_TRUE )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_LOCK( q_lock );

        if( notf_queue.first == notf_queue.last )
        {
            notf_queue.is_empty = SCTP_TRUE;
            SCTP_UNLOCK( q_lock );
            return ( SCTP_FAILURE );
        }

        /*
         * Need to make sure this function
         * is not called from multiple threads.
         * Right now this is true, so first is accessed outside lock
         * The #else part caters to that kind of requirement
         */

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "remove notification" ) );
        p_node = &( notf_queue.nodes_list[notf_queue.first] );
        notf_queue.first = ( notf_queue.first + 1 ) % SCTP_MAX_NOTF_Q_SIZE;

        if( notf_queue.first == notf_queue.last )
        {
            notf_queue.is_empty = SCTP_TRUE;
        }

        SCTP_UNLOCK( q_lock );

        if( ( SCTPNOTIFY_CONGESTION == p_node->event ) &&
                ( SCTP_CONGLEVEL_NOCONG != p_node->notice.congestion.cong_level ) )
        {
            p_ep = sctp_db_get_ep_ptr( p_node->notice.congestion.assoc_id );

            if( !p_ep )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_ASSOCID, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Invalid Association " ) );

                return ( SCTP_FAILURE );
            }

            if( p_ep->cong_flag == 1 )
            {
                p_ep->cong_flag = 0; /* Congestion notification flag */
                SCTP_WAKEUP( p_ep->cond );
            }
        }

        p_node->p_callback_func( p_node->event, &( p_node->notice ),
                                 p_node->p_appl_data );

        SCTP_LOCK( q_lock );
        notf_queue.count--;
        SCTP_UNLOCK( q_lock );
#else

        sctp_notf_queue_node_st  p_node[1];

        if( notf_queue.is_empty == SCTP_TRUE )
        {
            return ( SCTP_FAILURE );
        }

        SCTP_LOCK( q_lock );

        if( notf_queue.first == notf_queue.last )
        {
            notf_queue.is_empty = SCTP_TRUE;
            SCTP_UNLOCK( q_lock );
            return ( SCTP_FAILURE );
        }

        p_node[0] = notf_queue.nodes_list[notf_queue.first];
        notf_queue.first = ( notf_queue.first + 1 ) % SCTP_MAX_NOTF_Q_SIZE;

        if( notf_queue.first == notf_queue.last )
        {
            notf_queue.is_empty = SCTP_TRUE;
        }

        SCTP_UNLOCK( q_lock );

        if( ( SCTPNOTIFY_CONGESTION == p_node->event ) &&
                ( SCTP_CONGLEVEL_NOCONG != p_node->notice.congestion.cong_level ) )
        {
            p_ep = sctp_db_get_ep_ptr( p_node->notice.congestion.assoc_id );

            if( !p_ep )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_ASSOCID, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Invalid Association " ) );

                return ( SCTP_FAILURE );
            }

            if( p_ep->cong_flag == 1 )
            {
                p_ep->cong_flag = 0; /* Congestion notification flag */
                SCTP_WAKEUP( p_ep->cond );
            }
        }

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "remove notification" ) );
        p_node->p_callback_func( p_node->event, &( p_node->notice ),
                                 p_node->p_appl_data );

#endif
    }/* end of check for sctp state */

    else
    {
        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}

#ifndef SCTP_INDICATION_THREAD
/******************************************************************************
**  Function Name       : sctp_start_notification_queue_timer
**
**    Description       : Function to start the indication queue timer.
**
**    Returns           : None
**
******************************************************************************/
sctp_void_t sctp_start_notification_queue_timer()
{
    sctp_start_timer( SCTP_NULL, SCTPEVENT_TIMER_DEQUE,
                      SCTPE_DEQUE_TIMEOUT, 0, &sctp_ind_q_timer_id, SCTP_NULL );
}
#endif


#endif /* NO_QUEUE*/

#endif
#endif
