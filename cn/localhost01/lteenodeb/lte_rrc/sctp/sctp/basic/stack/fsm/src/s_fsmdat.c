/***************************************************************************
 *  FILE NAME  : s_fsmdat.c
 *
 *  DESCRIPTION: This file is part of Finite State Machine of SCTP. This will
 *               have functions related to data chunks processing .
 *
 *  DATE       NAME               REF#         REASON
 *  ------------------------------------------------------------
 *  02Jun00    Sandeep Mahajan     -           .Original Creation
 *  25Sep00    Sandeep Mahajan     -           SPR no 477
 *  13Oct00    samahajan          SPR593       Fix for this
 *  13Oct00    hbhondwe           SPR592       Fix for this
 *  14Feb01    Sandeep Mahajan    SPRXXX       Fix for PQ SPR
 *  16Aug01    gsheoran            -           REL 3.0 Update
 *  01Oct,02   mrajpal             -           Scatter buffer for IP
 *  18Oct,02   mrajpal            SPR 4970     Fragemnet data
 *  21Oct04    Amit Ray           Rel 6.0      SCTP/FUNC-IG-11
 *  3Nov04     Amit Ray           Rel 6.0      PERFORMANCE
 *  02Nov04    Svakumar           REL 6.0      SCTP/FUNC-CONGCON-5
 *  09May08    Raza               REL 6.2      SPR 18527
 *  07july,08  Shikha Kathpal     REl 6.2.0    RFC 4960 UPGRADE
 *  08july,08  Raza Mohamed       REl 6.2.0    RFC 4960 UPGRADE
 *  02Feb,10    Debasri Sarkar    SPR 20279 CSR 1-8238639
 *  24May10    Rajnesh            SPR 20411    Warning Fixed
 *  25mar,11   Akanksha           SPR 20859    Enhancement for Callback
 *  07Jul,11   Vaibhav Banga                   klocwork fix for IBM
 *  1May13     Pranav Sharma      SPR 21107    CSR 44034
 *  1May13  Pranav Sharma      SPR 21127    CSR 40731-GCC 4.7 warning removal
 *  21May14    Pinky Sinha        Bug Id 6     CSR 78264
 *  15Sep14    Priyanka Mishra    Bug Id 22    CSR 85223
*   6Oct15      Priyanka Mishra  Bug Id 81 fix s/e  CSR 106080
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#define SCTP_MODULE "FSM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_osmutx.h>
#include <s_es.h>
#include <s_ostmr.h>
#include <s_dbtcb.h>
#include <s_cobuf.h>
#include <s_dbbuf.h>
#include <s_mhbld.h>
#include <s_uisuif.h>
#include <s_in.h>
#include <s_stats.h>
#include <s_dbg.h>
#include <s_err.h>
#include <s_fsmrx.h>
#include <s_fsmtx.h>
#include <s_fsmass.h>
#include <s_fsmdat.h>
#include <s_dbutil.h>
#include <s_cocksm.h>

#define SSN_ROLLOVER_GUARD 32767
#define TSN_ROLLOVER_GUARD 2147483647

#define ROUND(x)    (((x) + 3) & ~3)


static sctp_return_t
sctp_build_data_chunk_header(
    sctp_U8bit     *p_buf,
    sctpbuffer_st  *sctp_buf );

/***************************************************************************
 *    FUNCTION :
 *        sctp_compare_ssn_for_smaller
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *        Returns SCTP_FAILURE if the ssn are equal or ssn1 > ssn2.
 **************************************************************************/
sctp_return_t
sctp_compare_ssn_for_smaller(
    sctp_U16bit ssn1,
    sctp_U16bit ssn2 )
{
    if( ( ssn1 != ssn2 )
            && ( ( ( ssn1 < ssn2 ) && ( ssn2 - ssn1 < SSN_ROLLOVER_GUARD ) )
                 || ( ( ssn1 > ssn2 ) && ( ssn1 - ssn2 > SSN_ROLLOVER_GUARD ) ) ) )
    {
        return ( SCTP_SUCCESS );
    }

    else
    {
        return ( SCTP_FAILURE );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_compare_ssn_for_larger
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *        Returns SCTP_FAILURE if the ssn are equal or ssn1 < ssn2.
 *************************************************************************/
sctp_return_t
sctp_compare_ssn_for_larger(
    sctp_U16bit ssn1,
    sctp_U16bit ssn2 )
{
    if( ( ssn1 != ssn2 )
            && ( ( ( ssn1 < ssn2 ) && ( ssn2 - ssn1 > SSN_ROLLOVER_GUARD ) )
                 || ( ( ssn1 > ssn2 ) && ( ssn1 - ssn2 < SSN_ROLLOVER_GUARD ) ) ) )
    {
        return ( SCTP_SUCCESS );
    }

    else
    {
        return ( SCTP_FAILURE );
    }

}


/***************************************************************************
 *    FUNCTION :
 *        sctp_compare_tsn_for_smaller
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *        Returns SCTP_FAILURE if the tsn are equal or tsn1 > tsn2.
 **************************************************************************/
sctp_return_t
sctp_compare_tsn_for_smaller(
    sctp_U32bit tsn1,
    sctp_U32bit tsn2 )
{
    if( ( tsn1 != tsn2 )
            && ( ( ( tsn1 < tsn2 ) && ( tsn2 - tsn1 < TSN_ROLLOVER_GUARD ) )
                 || ( ( tsn1 > tsn2 ) && ( tsn1 - tsn2 > TSN_ROLLOVER_GUARD ) ) ) )
    {
        return ( SCTP_SUCCESS );
    }

    else
    {
        return ( SCTP_FAILURE );
    }
}


/***************************************************************************
 *    FUNCTION :
 *    sctp_compare_tsn_for_larger
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *        Returns SCTP_FAILURE if the tsn are equal or tsn1 < tsn2.
 **************************************************************************/
sctp_return_t
sctp_compare_tsn_for_larger(
    sctp_U32bit tsn1,
    sctp_U32bit tsn2 )
{
    if( ( tsn1 != tsn2 )
            && ( ( ( tsn1 < tsn2 ) && ( tsn2 - tsn1 > TSN_ROLLOVER_GUARD ) )
                 || ( ( tsn1 > tsn2 ) && ( tsn1 - tsn2 < TSN_ROLLOVER_GUARD ) ) ) )
    {
        return ( SCTP_SUCCESS );
    }

    else
    {
        return ( SCTP_FAILURE );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_is_ssn_present
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_is_ssn_present(
    sctp_tcb_st *p_ass,
    sctp_U16bit stream,
    sctp_U16bit ssn )
{
    LIST          *queue;
    sctpbuffer_st *p_buf;
    sctp_U32bit   expected_tsn;

    queue = &p_ass->receive.stream[stream].receive_queue;

    for( p_buf = ( sctpbuffer_st * ) lstFirst( queue ); p_buf;
            p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
    {
        if( ( p_buf->unordered == 1 ) || ( p_buf->ssn != ssn ) )
        {
            /* SPR593 */
            continue;
        }

        if( p_buf->begin != 1 )
        {
            return ( SCTP_FAILURE );
        }

        if( p_buf->end != 1 )
        {
            /* Not the first fragment of a message */
            expected_tsn = p_buf->tsn + 1;
            ssn = p_buf->ssn;

            /* Scan to see if we have all fragments */
            for( ;; )
            {
                p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );

                if( ( p_buf == NULL ) || ( p_buf->tsn != expected_tsn ) ||
                        ( p_buf->unordered == 1 ) || ( p_buf->ssn != ssn ) )
                {
                    /* Complete message not available yet */
                    return ( SCTP_FAILURE );
                }

                if( p_buf->end == 1 )
                {
                    /* Got last fregmet */
                    break;
                }

                expected_tsn++;
            }
        } /* If Loop */

        return ( SCTP_SUCCESS );
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_remove_any_unorder_chunk
 *
 *    DESCRIPTION:
 *        This function is called when there are no reveier buffers left. This
 *    function will try to delete any out of order data chunk to give way
 *    for this chunk.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_remove_any_unorder_chunk(
    sctp_tcb_st    *p_ass,
    sctp_U16bit    stream,
    sctp_U32bit    tsn,
    sctp_U32bit    length )
{
    sctpbuffer_st *p_buf;

    /* to be coded */
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_remove_any_unorder_chunk,stream=%d,tsn=%u,len:%u",
                  stream, tsn, length ) );

    /*
     * If this tsn is smaller than the last tsn present on the
     * receive queue of this stream , drop the last tsn for
     * giving way to this
     */
    p_buf =
        ( sctpbuffer_st * ) lstLast( &p_ass->receive.stream[stream].receive_queue );

    for( ;; )
    {
        if( !p_buf )
        {
            return SCTP_FAILURE;
        }

        /*
         * Tsn of buffer in queue has to be larger than current tsn
         * if not return failure
         */
        if( ( sctp_compare_tsn_for_larger( p_buf->tsn, tsn ) != SCTP_SUCCESS )
                || ( sctp_compare_tsn_for_larger( p_buf->tsn, p_ass->receive.cumtsn )
                     != SCTP_SUCCESS ) )
        {
            return SCTP_FAILURE;
        }

        /*
         * If size of this buffer is too less to fit in  new buffer
         * skip this buffer
         */
        if( p_buf->id < length )
        {
            p_buf = ( sctpbuffer_st * ) lstPrevious( ( NODE * ) p_buf );
            continue;
        }

        /* update sacklist for removal of this tsn */
        if( sctp_negative_ack_sacklist( p_ass, p_buf->tsn ) != SCTP_SUCCESS )
        {
            return SCTP_FAILURE;
        }

        /* Got a buffer that can be removed to give way for this new tsn  */
        lstDelete( &p_ass->receive.stream[stream].receive_queue, ( NODE * ) p_buf );
        sctp_free_receive_buffer( p_ass, p_buf );

        return SCTP_SUCCESS;
    } /* for loop */

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return SCTP_FAILURE;
#endif
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_partial_ind_clear_buffers
 *
 *    DESCRIPTION:
 *        This function is called after request for new buffer has failed.
 *    This function detects if partial notification can be used
 *    to cleear some buffers.
 *
 *    RETURN VALUE:
 *        void
 **************************************************************************/
void
sctp_send_partial_ind_clear_buffers(
    sctp_tcb_st   *p_ass,
    sctp_U16bit   stream )
{
    sctp_return_t ret = SCTP_FAILURE;
    sctpbuffer_st  *p_buf;
    sctp_U32bit   expected_tsn;

    if( p_ass->partial_data.data_assembly_in_progress == 1 )
    {
        /*
         * partial data assembly already in place
         * check if we have got expected tsn or not
         */
        p_buf =
            ( sctpbuffer_st * )lstFirst( &p_ass->receive.stream[stream].receive_queue );

        for( ; p_buf != NULL; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
        {
            if( p_ass->partial_data.expected_tsn == p_buf->tsn )
            {
                ret = SCTP_SUCCESS;
                break;
            }
        }
    }

    else   /* partial data assembly not already in progress */
    {
        /*
         * partial data assembly not in place, check we have some
         * partial chunks, This will be checked in releation of last
         * chunk in the queue. Some optimization may be done
         * in logic here
         */

        p_buf = ( sctpbuffer_st * )
                lstLast( &p_ass->receive.stream[stream].receive_queue );

        /*
         * We hav got the end last chunk in queue.Either we have got the
         * whole datagram or if some chunks were missing and this is
         * an out of order end chunk. It will be dropped. Partial
         * delivery not needed
         */
        if( ( p_buf == NULL ) || ( p_buf->end == 1 ) )
        {
            return;
        }

        expected_tsn = p_buf->tsn;

        for( ; p_buf != NULL;
                p_buf = ( sctpbuffer_st * ) lstPrevious( ( NODE * ) p_buf ) )
        {
            /* scan till we get chunk with begin bit set */
            if( p_buf->tsn != expected_tsn )
            {
                return;
            }

            if( p_buf->begin == 1 )
            {
                p_ass->partial_data.ssn = p_buf->ssn;
                p_ass->partial_data.unordered = p_buf->unordered;
                p_ass->partial_data.expected_tsn = p_buf->tsn;

                ret = SCTP_SUCCESS;
                break;
            }

            expected_tsn--;
        }
    } /* else loop */

    if( ret == SCTP_SUCCESS )
    {
        /* No space for incmoing message, ask user to get partial data */
        p_ass->partial_data.partial_data_pending = 1;
        p_ass->partial_data.stream = stream;

        /* Notify user of data pending */
        sctp_ntfy_partial_data( p_ass, stream );

        /* wakeup SU in case of blocking read */
        SCTP_WAKEUP( p_ass->p_ep->cond );
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_chk_send_par_notf
 *
 *    DESCRIPTION:
 *        This function send partial data only if it detects that it has
 *    received end segment of message for which it has already sent partial
 *    to upper layer. Returns failure if no partial data pending indication
 *    sent.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_chk_send_par_notf(
    sctp_tcb_st    *p_ass,
    sctp_U16bit    stream,
    sctp_U32bit    tsn )
{
    sctpbuffer_st *p_buf;
    sctp_U32bit  expected_tsn;

    p_buf =
        ( sctpbuffer_st * ) lstFirst( &p_ass->receive.stream[stream].receive_queue );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_chk_send_par_notf, stream=%d,tsn=%u", stream, tsn ) );

    if( p_ass->partial_data.data_assembly_in_progress != 1 )
    {
        return SCTP_FAILURE;
    }

    for( ; ( p_buf != NULL ); p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
    {
        if( p_buf->tsn != p_ass->partial_data.expected_tsn )
        {
            continue;
        }

        /*
         * for this to be sent we should have all chunks after expected tsn
         * till we get end segment
         */

        expected_tsn = p_ass->partial_data.expected_tsn;

        for( ;; )
        {
            if( ( !p_buf ) || ( p_buf->tsn != expected_tsn ) )
            {
                return SCTP_FAILURE;
            }

            if( p_buf->end == 1 )
            {
                /* Got end segment of partial data */
                p_ass->partial_data.partial_data_pending = 1;
                p_ass->partial_data.stream = stream;

                /* Notify user of data pending */
                sctp_ntfy_partial_data( p_ass, stream );

                /*SPR data pending after partial data*/
                if( p_buf->unordered == SCTP_FALSE )
                {
                    p_ass->receive.stream[stream].nextssn_notify++;
                }

                /*SPR */
                /* wakeup SU in case of blocking read */
                SCTP_WAKEUP( p_ass->p_ep->cond );

                return SCTP_SUCCESS;
            }

            p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );
            expected_tsn++;
        } /* for loop */
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( tsn );
    /*to avoid the warning*/


    /* No partial data pending indication sent */

    return SCTP_FAILURE;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_is_notification_required
 *
 *    DESCRIPTION: This function checks notification need to be given or not.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_is_notification_required(
    sctp_tcb_st    *p_ass,
    sctp_U16bit    stream,
    sctp_U32bit    tsn,
    sctp_U32bit    *notify )
{
    /* need to add code for partial data api  */
    sctpbuffer_st *p_buf, *save;
    sctp_U32bit expected_tsn, no_notify = 0;
    sctp_U16bit ssn;

    p_buf =
        ( sctpbuffer_st * ) lstLast( &p_ass->receive.stream[stream].receive_queue );

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_is_notification_required, stream=%d, tsn=%u", stream, tsn ) );

    for( ; p_buf != NULL; p_buf = ( sctpbuffer_st * ) lstPrevious( ( NODE * ) p_buf ) )
    {
        if( p_buf->tsn == tsn )
        {
            break;
        }
    }

    /* SPR : 18527 */
    if( p_buf != SCTP_NULL && p_buf->end != 1 )
        /* SPR : 18527 */
    {
        save = p_buf;

        /* Not the last fragment of a message */
        expected_tsn = p_buf->tsn + 1;
        ssn = p_buf->ssn;

        /* Scan to see if we have all fragments */
        for( ;; )
        {
            p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );

            if( ( p_buf == NULL ) || ( p_buf->tsn != expected_tsn )
                    || ( ( p_buf->unordered == 0 ) && ( p_buf->ssn != ssn ) ) )
            {
                /* Complete message not available yet */
                return ( SCTP_FAILURE );
            }

            if( p_buf->end == 1 ) /* Got last fregmet */
            {
                break;
            }

            expected_tsn++;
        }

        p_buf = save;
    }

    /* SPR : 18527 */
    if( p_buf != SCTP_NULL && p_buf->begin != 1 )
        /* SPR : 18527 */
    {
        save = p_buf;

        /* Not the first fragment of a message */
        expected_tsn = p_buf->tsn - 1;
        ssn = p_buf->ssn;

        /* Scan  to see if we have all fragments */
        for( ;; )
        {
            p_buf = ( sctpbuffer_st * ) lstPrevious( ( NODE * ) p_buf );

            if( ( p_buf == NULL ) || ( p_buf->tsn != expected_tsn )
                    || ( ( p_buf->unordered == 0 ) && ( p_buf->ssn != ssn ) ) )
            {
                /* Complete message not available yet */
                return ( SCTP_FAILURE );
            }

            if( p_buf->begin == 1 ) /* Got first fregmet */
            {
                break;
            }

            expected_tsn--;
        }

        p_buf = save;
    }

    /* We got the whole data,for ordered data ssn should match the
     * next ssn expected on this stream to notify
     */
    /* SPR : 18527 */
    if( p_buf != SCTP_NULL && p_buf->unordered == 0 )
        /* SPR : 18527 */
    {

        /* This ssn should be more than next ssn expected , if no return
           failure */
        if( sctp_compare_ssn_for_smaller( p_buf->ssn,
                                          p_ass->receive.stream[stream].nextssn_notify ) == SCTP_SUCCESS )
        {
            return ( SCTP_FAILURE );
        }

        ssn = p_ass->receive.stream[stream].nextssn_notify;

        for( ;; ssn++ )
        {
            if( sctp_is_ssn_present( p_ass, stream, ssn ) == SCTP_FAILURE )
            {
                if( no_notify == 0 )
                {
                    return ( SCTP_FAILURE );
                }

                else
                {
                    p_ass->receive.stream[stream].nextssn_notify = ssn;
                    *notify = no_notify;
                    return ( SCTP_SUCCESS );
                }
            }

            no_notify++;
        }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
        /* CSR#1-4825810 Changes done for warning removal activity
         * for compilation with SUN cc */
        return ( SCTP_SUCCESS );
#endif
    }

    else
    {
        no_notify++;
        *notify = no_notify;
        return ( SCTP_SUCCESS );
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return ( SCTP_FAILURE );
#endif
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_check_data_arrival
 *
 *    DESCRIPTION: This function checks for data in receive queue
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctpbuffer_st *
sctp_check_data_arrival(
    sctp_tcb_st    *p_ass,
    sctp_U16bit    stream,
    sctp_U32bit    *size )
{
    LIST         *queue;
    sctpbuffer_st *p_buf, *save;
    sctp_U32bit  nexttsn;
    sctp_U16bit  ssn, nextssn;

    queue = &p_ass->receive.stream[stream].receive_queue;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_check_data_arrival, stream id=%d,", stream ) );

    p_buf = ( sctpbuffer_st * ) lstFirst( queue );
    nextssn = p_ass->receive.stream[stream].nextssn;

    for( ; ( p_buf != NULL ); p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
    {
        if( ( p_buf->ssn != nextssn ) && ( p_buf->unordered == 0 ) )
        {
            /* Not the next SSN and not unordered */
            continue;
        }

        if( p_buf->begin == 0 )
        {
            /* Not the first fragment of a message */
            continue;
        }

        nexttsn = p_buf->tsn;
        ssn = p_buf->ssn;
        save = p_buf;

        /*
         * Scan to see if all fragments are in sequence and
         * the end fragment is there
         */
        *size = 0;

        for( ; p_buf != NULL; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
        {
            /* increment size variable */
            *size += ( p_buf->size - p_buf->buffer_read_index );

            if( ( p_buf != NULL ) && ( p_buf->end == 0 )
                    && ( ( p_buf->unordered == 1 ) || ( p_buf->ssn == ssn ) )
                    && ( p_buf->tsn == nexttsn ) )
            {
                nexttsn++;
                continue;
            }

            if( ( p_buf != NULL )
                    && ( p_buf->end == 1 )
                    && ( ( p_buf->unordered == 1 ) || ( p_buf->ssn == ssn ) )
                    && ( p_buf->tsn == nexttsn ) ) /* Got some data */
            {
                return save;
            }

            p_buf = save;
            break;

        }

        if( p_buf == NULL )
        {
            break;
        }
    }

    return NULL;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_read_message_list
 *
 *    DESCRIPTION: This function checks the data arrival and reads the
 *        message list.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_read_message_list(
    sctp_tcb_st    *p_ass,
    sctp_U16bit    stream,
    sctp_U32bit    buffer_len,
    sctp_U8bit     *buffer,
    sctp_U32bit    *msg_len,
    sctp_U32bit    *ppi,
    sctp_Boolean_t peek_flag,
    sctp_error_t   *p_ecode )
{
    sctpbuffer_st *p_buf, *save;
    sctp_U32bit  copy_len = 0, data_len = 0;
    /* Bug Id 6 fix starts*/
    /*sctp_U32bit       old_cong_reg, new_cong_reg;*/
    sctp_U32bit       old_cong_reg = 0;
    sctp_U32bit       new_cong_reg = 0;
    /* Bug Id 6 fix ends*/
    /* SPR 20573 Starts for CSR 1-7088501 */
    sctp_U8bit          unordered;
    sctp_U8bit          end = 0;
    /* SPR 20573 Ends for CSR 1-7088501 */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_read_message_list,stream = %d", stream ) );

    p_buf = sctp_check_data_arrival( p_ass, stream, msg_len );

    if( p_buf == NULL )
    {
        *p_ecode = E_NOTHING_IN_QUEUE;
        return ( SCTP_FAILURE );
    }

    if( ( peek_flag != SCTP_TRUE ) && ( buffer_len < *msg_len ) )
    {
        *p_ecode = E_BUFFER_TOO_SMALL;
        return ( SCTP_FAILURE );
    }

    /* Read and return the message */
    *ppi = p_buf->ppi;


    for( ;; )
    {
        save = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );
        copy_len = ( ( buffer_len - data_len ) > ( sctp_U32bit ) p_buf->size )
                   ? ( sctp_U32bit )p_buf->size : buffer_len - data_len;

        if( copy_len > 0 )
        {
            __MEMCOPY( buffer + data_len, &p_buf->buffer[0], copy_len );
        }

        if( peek_flag == SCTP_FALSE )
        {
            lstDelete( &p_ass->receive.stream[stream].receive_queue, ( NODE * ) p_buf );
            /*SPR 20573 Starts for CSR 1-7088501  */
            end = p_buf->end;
            unordered = p_buf->unordered;
            /*SPR 20573 Ends for CSR 1-7088501 */

            sctp_free_receive_buffer( p_ass, p_buf );
        }

        /*SPR 20573 Starts for CSR 1-7088501  */
        else
        {
            end = p_buf->end;
            unordered = p_buf->unordered;
        }

        /*SPR 20573 Ends for CSR 1-7088501 */

        data_len += copy_len;

        /* SPR 20573 Starts for CSR 1-7088501  */
        if( 1 == end )
            /* SPR 20573 Ends for CSR 1-7088501  */
        {
            break;
        }

        p_buf = save;
    }

    *msg_len = data_len;

    if( peek_flag == SCTP_TRUE )
    {
        return ( SCTP_SUCCESS );
    }

    /* Update nextssn parameter */
    /*SPR 20573 Starts for CSR 1-7088501 */
    if( 0 == unordered )
        /*SPR 20573 Ends for CSR 1-7088501 */
    {
        p_ass->receive.stream[stream].nextssn++;
    }

    if( SCTP_SUCCESS == sctp_calc_rcv_cong_reg( p_ass, &new_cong_reg ) )
    {
        old_cong_reg = p_ass->receive.congestion_event_sent;

        /*
         * Check whether we have new congestion. And if so then give congestion
         * indication to the user.
         */
        if( new_cong_reg != old_cong_reg )
        {
            p_ass->receive.congestion_event_sent = new_cong_reg;

            /* notify su of change in congestion level */
            sctp_ntfy_recv_congestion( p_ass, new_cong_reg );
        }
    }


    return ( SCTP_SUCCESS );
}


#ifdef SCTP_UI_SOCK_API
/***************************************************************************
 *    FUNCTION :
 *        sctp_read_message_list_ext
 *
 *    DESCRIPTION:
 *        This function is used only is socket interface
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_read_message_list_ext(
    sctp_tcb_st        *p_ass,
    sctp_U16bit        stream,
    sctp_U32bit        buffer_len,
    sctp_U8bit         *buffer,
    sctp_U32bit        *msg_len,
    sctp_U32bit        *p_ppi,
    sctp_U32bit        flags,
    sctp_sockaddr_st   *p_daddr, /* address from which data is received */
    sctp_U32bit        *p_ssn,   /* ssn for the message */
    sctp_U32bit        *p_tsn,   /* tsn for socket draft*/
    sctp_U32bit        *p_context,
    sctp_Boolean_t     *p_unordered,
    sctp_Boolean_t     *p_partial_data_read,
    sctp_error_t       *p_ecode )
{
    sctpbuffer_st     *p_buf = SCTP_NULL;
    sctpbuffer_st     *save;
    sctp_U32bit       copy_len = 0;
    sctp_U32bit       data_len = 0;
    /* Bug Id 6 fix starts*/
    /*sctp_U32bit       old_cong_reg, new_cong_reg;*/
    sctp_U32bit       old_cong_reg = 0;
    sctp_U32bit       new_cong_reg = 0;
    /* Bug Id 6 fix ends*/
    sctp_iovec_st     *p_iovec = ( sctp_iovec_st * )buffer;
    sctp_U32bit       num_iovec = buffer_len;
    sctp_U32bit       iov_ctr = 0, iov_rem_len = 0;
    sctp_U32bit       iov_copy_len = 0;
    sctp_U32bit       buffer_cpied = 0;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_read_message_list_ext, stream = %d", stream ) );

    p_buf = sctp_check_data_arrival( p_ass, stream, msg_len );

    if( !p_buf )
    {
        *p_ecode = E_NOTHING_IN_QUEUE;
        return ( SCTP_FAILURE );
    }

    /*
     * Return the Message specific data.
     *
     *the function is called from one place in the stack,
     * No need to check
     */
    *p_ppi = p_buf->ppi;

    *p_daddr = p_ass->transmit.remote[p_buf->dest_addr_index].that_address;

    *p_ssn = p_buf->ssn;

    *p_context = p_buf->context;

    *p_unordered = p_buf->unordered;

    *p_tsn = p_buf->tsn;

    /* Calculate the total iovector buffer length */
    buffer_len = 0;

    for( iov_ctr = 0; iov_ctr < num_iovec; iov_ctr++ )
    {
        buffer_len += p_iovec[iov_ctr].iov_len;
    }

    iov_ctr     = 0;
    buffer      = ( sctp_U8bit * )p_iovec[iov_ctr].iov_base;
    iov_rem_len = p_iovec[iov_ctr].iov_len;

    for( ;; )
    {
        buffer_cpied = 0;

        save = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );
        copy_len = ( ( buffer_len - data_len ) > ( sctp_U32bit )( p_buf->size -
                                                                  p_buf->buffer_read_index ) ) ?
                   ( p_buf->size - p_buf->buffer_read_index ) : buffer_len - data_len;

        data_len += copy_len;

        while( copy_len > 0 )
        {
            while( 0 == iov_rem_len )
            {
                iov_ctr++;
                buffer      = ( sctp_U8bit * )p_iovec[iov_ctr].iov_base;
                iov_rem_len = p_iovec[iov_ctr].iov_len;
            }

            iov_copy_len = ( iov_rem_len > copy_len ) ? copy_len : iov_rem_len;

            __MEMCOPY( buffer, &p_buf->buffer[p_buf->buffer_read_index + buffer_cpied],
                       iov_copy_len );

            iov_rem_len -= iov_copy_len;
            copy_len    -= iov_copy_len;
            buffer      += iov_copy_len;
            buffer_cpied += iov_copy_len;
        }

        if( !( flags & MSG_PEEK ) )
        {
            /*
             * If the buffer has been completly read, then delete the buffer. Or
             * Truncate message option is set.
             */
            if( buffer_cpied == ( p_buf->size - p_buf->buffer_read_index ) )
            {
                lstDelete( &p_ass->receive.stream[stream].receive_queue, ( NODE * ) p_buf );
                sctp_free_receive_buffer( p_ass, p_buf );
            }

            else
            {
                if( flags & MSG_TRUNC )
                {
                    lstDelete( &p_ass->receive.stream[stream].receive_queue, ( NODE * ) p_buf );
                    sctp_free_receive_buffer( p_ass, p_buf );
                }

                /*
                 * If the length copied is less than the size of the message, then
                 * set the flag to indicate that only partial data has been read.
                 * Increment the buffer read index by the length of the buffer copied.
                 * Also set the begin flag in the buffer so that the same message
                 * is read when next recv API is invoked.
                 */
                *p_partial_data_read = SCTP_TRUE;
                p_buf->buffer_read_index += buffer_cpied;
                p_buf->begin = 1;
            }
        }

        /*
         * Ed of the buffer is reached.
         */
        if( p_buf->end == 1 )
        {
            break;
        }

        p_buf = save;
    }

    /* Now fill the correct length in remaining iovectors */
    p_iovec[iov_ctr].iov_len -= iov_rem_len;

    for( iov_ctr = iov_ctr + 1; iov_ctr < num_iovec; iov_ctr++ )
    {
        p_iovec[iov_ctr].iov_len = 0;
    }

    *msg_len = data_len;

    if( flags & MSG_PEEK )
    {
        return SCTP_SUCCESS;
    }

    /* Update next ssn parameter */
    if( ( ( p_buf->unordered == 0 ) && ( SCTP_FALSE == *p_partial_data_read ) )
            || ( ( p_buf->unordered == 0 ) && ( flags & MSG_TRUNC ) ) )
    {
        p_ass->receive.stream[stream].nextssn++;
    }

    if( SCTP_SUCCESS == sctp_calc_rcv_cong_reg( p_ass, &new_cong_reg ) )
    {
        old_cong_reg = p_ass->receive.congestion_event_sent;

        /*
         * Check whether we have new congestion. And if so then give congestion
         * indication to the user.
         */
        if( new_cong_reg != old_cong_reg )
        {
            p_ass->receive.congestion_event_sent = new_cong_reg;

            /* notify su of change in congestion level */
            sctp_ntfy_recv_congestion( p_ass, new_cong_reg );
        }
    }

    return ( SCTP_SUCCESS );
}
#endif

/***************************************************************************
 *    FUNCTION :
 *        sctp_read_data_mesg
 *
 *    DESCRIPTION:
 *        This function is used only is function interface
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_read_data_mesg(
    sctp_tcb_st        *p_ass,
    sctp_U16bit        stream,
    sctp_U32bit        buffer_len,
    sctp_U8bit         *buffer,
    sctp_U32bit        *msg_len,
    sctp_U32bit        *p_ppi,
    sctp_U32bit        flags,
    sctp_sockaddr_st   *p_daddr, /* address from which data is received */
    sctp_Boolean_t     *p_unordered,
    sctp_Boolean_t     *p_partial_data_read,
    sctp_error_t       *p_ecode )
{
    sctpbuffer_st     *p_buf = SCTP_NULL;
    sctpbuffer_st     *save;
    sctp_U32bit       copy_len = 0;
    sctp_U32bit       data_len = 0;
    /* Bug Id 6 fix starts*/
    /*sctp_U32bit       old_cong_reg, new_cong_reg;*/
    sctp_U32bit       old_cong_reg = 0;
    sctp_U32bit       new_cong_reg = 0;
    /* Bug Id 6 fix ends*/

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_read_message_list_ext, stream = %d", stream ) );

    p_buf = sctp_check_data_arrival( p_ass, stream, msg_len );

    if( !p_buf )
    {
        *p_ecode = E_NOTHING_IN_QUEUE;

        return ( SCTP_FAILURE );
    }

    /*
     * Return the Message specific data.
     *
     * The function is called from one place in the stack,
     * No need to check
     */
    *p_ppi = p_buf->ppi;
    *p_daddr = p_ass->transmit.remote[p_buf->dest_addr_index].that_address;

    *p_unordered = p_buf->unordered;

    for( ;; )
    {
        save = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf );
        copy_len = ( ( buffer_len - data_len ) > ( sctp_U32bit )( p_buf->size -
                                                                  p_buf->buffer_read_index ) ) ?
                   ( p_buf->size - p_buf->buffer_read_index ) : buffer_len - data_len;

        if( copy_len > 0 )
        {
            __MEMCOPY( buffer + data_len, &p_buf->buffer[p_buf->buffer_read_index],
                       copy_len );
        }

        if( !( flags & SCTP_MESG_PEEK ) )
        {
            /*
             * If the buffer has been completly read, then delete the buffer.
             */
            if( copy_len == ( p_buf->size - p_buf->buffer_read_index ) )
            {
                lstDelete( &p_ass->receive.stream[stream].receive_queue, ( NODE * ) p_buf );
                sctp_free_receive_buffer( p_ass, p_buf );
            }

            else
            {
                if( flags & SCTP_FLUSH_MESG_AFTER_READ )
                {
                    lstDelete( &p_ass->receive.stream[stream].receive_queue, ( NODE * ) p_buf );
                    sctp_free_receive_buffer( p_ass, p_buf );
                }

                /*
                 * If the length copied is less than the size of the message, then
                 * set the flag to indicate that only partial data has been read.
                 * Increment the buffer read index by the length of the buffer copied.
                 * Also set the begin flag in the buffer so that the same message
                 * is read when next recv API is invoked.
                 */
                *p_partial_data_read = SCTP_TRUE;
                p_buf->buffer_read_index += copy_len;
                p_buf->begin = 1;
            }
        }

        data_len += copy_len;

        if( p_buf->end == 1 )
        {
            break;
        }

        p_buf = save;
    }

    if( flags & SCTP_MESG_PEEK )
    {
        return SCTP_SUCCESS;
    }

    /* Update next ssn parameter */

    if( ( ( p_buf->unordered == 0 ) && ( SCTP_FALSE == *p_partial_data_read ) ) ||
            ( ( p_buf->unordered == 0 ) && ( flags & SCTP_FLUSH_MESG_AFTER_READ ) ) )
    {
        p_ass->receive.stream[stream].nextssn++;
    }

    if( SCTP_SUCCESS == sctp_calc_rcv_cong_reg( p_ass, &new_cong_reg ) )
    {
        old_cong_reg = p_ass->receive.congestion_event_sent;

        /*
         * Check whether we have new congestion. And if so then give congestion
         * indication to the user.
         */
        if( new_cong_reg != old_cong_reg )
        {
            p_ass->receive.congestion_event_sent = new_cong_reg;

            /* notify su of change in congestion level */
            sctp_ntfy_recv_congestion( p_ass, new_cong_reg );
        }
    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
*    FUNCTION :
*        sctp_dynamic_chk_tsn
*
*    DESCRIPTION:
*
*    RETURN VALUE:
*       SCTP_SUCCESS
*       SCTP_FAILURE
**************************************************************************/
sctp_return_t
sctp_dynamic_chk_tsn(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    tsn )
{
    sctp_U32bit    diff;
    sctp_U32bit    range;
    sctpsackbuf_st *p_sack;
    sctp_U32bit    cumtsn = p_ass->receive.cumtsn;
    LIST           *sacklist = &p_ass->receive.sacklist;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_dynamic_chk_tsn, tsn = %u", tsn ) );

    if( sctp_compare_tsn_for_larger( tsn, cumtsn ) != SCTP_SUCCESS )
    {
        return ( SCTP_FAILURE );
    }

    p_sack = ( sctpsackbuf_st * ) lstFirst( sacklist );

    for( ; p_sack; p_sack = ( sctpsackbuf_st * ) lstNext( ( NODE * ) p_sack ) )
    {
        diff = ( tsn - p_sack->first );
        range = ( p_sack->last - p_sack->first );

        /* Remember, we are doing unsigned arithmetic here! */
        if( diff <= range )
        {
            return ( SCTP_FAILURE );
        }
    }

    return ( SCTP_SUCCESS );
}



/***************************************************************************
 *    FUNCTION :
 *        sctp_negative_ack_sacklist
 *
 *    DESCRIPTION:
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_negative_ack_sacklist(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    tsn )
{
    LIST           *sacklist     = &p_ass->receive.sacklist;
    LIST           *sackfreelist = &p_ass->receive.sackfreelist;
    sctpsackbuf_st *p_sack;
    sctpsackbuf_st *p_sacksave;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_negative_ack_sacklist, tsn = %u", tsn ) );

    if( ( tsn == p_ass->receive.cumtsn )
            || ( sctp_compare_tsn_for_smaller( tsn, p_ass->receive.cumtsn ) ==
                 SCTP_SUCCESS ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                    E_INTERNAL_ERROR, "Internal error updating sacklist" );
    }

    p_sack = ( sctpsackbuf_st * ) lstFirst( sacklist );

    for( ; p_sack; p_sack = ( sctpsackbuf_st * ) lstNext( ( NODE * ) p_sack ) )
    {
        /* check tsn is not smaller than the start of segment */
        if( sctp_compare_tsn_for_smaller( tsn, p_sack->first ) == SCTP_SUCCESS )
        {
            return SCTP_FAILURE;
        }

        if( tsn == p_sack->first )
        {
            if( p_sack->first == p_sack->last )
            {
                lstDelete( sacklist, ( NODE * ) p_sack );
                lstAdd( sackfreelist, ( NODE * ) p_sack );
            }

            else
            {
                p_sack->first++;
            }

            return ( SCTP_SUCCESS );
        }

        else if( ( sctp_compare_tsn_for_larger( tsn, p_sack->first ) ==
                   SCTP_SUCCESS )
                 && ( sctp_compare_tsn_for_smaller( tsn, p_sack->last ) ==
                      SCTP_SUCCESS ) )
        {
            /* need to add one more segment */
            p_sacksave = ( sctpsackbuf_st * ) lstFirst( sackfreelist );

            if( p_sacksave == 0 )
            {
                return ( SCTP_FAILURE );
            }

            p_sacksave->first = tsn + 1;
            p_sacksave->last = p_sack->last;
            p_sack->last = tsn - 1;
            lstDelete( sackfreelist, ( NODE * ) p_sacksave );

            /* Fixing problem reported by Shanghai Bell */
            lstInsert( sacklist, ( NODE * )p_sack, ( NODE * )p_sacksave );

            return ( SCTP_SUCCESS );
        }

        else if( tsn == p_sack->last )
        {
            p_sack->last--;

            return ( SCTP_SUCCESS );
        }
    }

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_update_sacklist
 *
 *    DESCRIPTION: This function is to update sacklist
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_update_sacklist(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    tsn )
{
    LIST           *sacklist = &p_ass->receive.sacklist;
    LIST           *sackfreelist = &p_ass->receive.sackfreelist;
    sctpsackbuf_st *p_sack, *p_sacksave;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_update_sacklist, tsn = %u", tsn ) );

    p_sack = ( sctpsackbuf_st * ) lstFirst( sacklist );

    if( tsn == ( p_ass->receive.cumtsn + 1 ) )
    {
        p_ass->receive.cumtsn++;

        if( ( p_sack != NULL ) && ( ( p_ass->receive.cumtsn + 1 ) == p_sack->first ) )
        {
            p_ass->receive.cumtsn = p_sack->last;
            lstDelete( sacklist, ( NODE * ) p_sack );
            lstAdd( sackfreelist, ( NODE * ) p_sack );
        }

        return ( SCTP_SUCCESS );
    }

    p_sack = ( sctpsackbuf_st * ) lstFirst( sacklist );

    for( ; p_sack; p_sack = ( sctpsackbuf_st * ) lstNext( ( NODE * ) p_sack ) )
    {
        if( tsn == ( p_sack->first - 1 ) )
        {
            p_sack->first--;

            return ( SCTP_SUCCESS );
        }

        else if( tsn == ( p_sack->last + 1 ) )
        {
            p_sacksave = ( sctpsackbuf_st * ) lstNext( ( NODE * ) p_sack );
            p_sack->last++;

            if( p_sacksave && ( ( p_sack->last + 1 ) == p_sacksave->first ) )
            {
                p_sack->last = p_sacksave->last;
                lstDelete( sacklist, ( NODE * ) p_sacksave );
                lstAdd( sackfreelist, ( NODE * ) p_sacksave );
            }

            return ( SCTP_SUCCESS );
        }

        else if( sctp_compare_tsn_for_larger( p_sack->first, tsn ) ==
                 SCTP_SUCCESS )
        {
            p_sacksave = ( sctpsackbuf_st * ) lstFirst( sackfreelist );

            if( p_sacksave == 0 )
            {
                return ( SCTP_FAILURE );
            }

            p_sacksave->first = p_sacksave->last = tsn;
            lstDelete( sackfreelist, ( NODE * ) p_sacksave );
            lstInsert( sacklist, lstPrevious( ( NODE * ) p_sack ),
                       ( NODE * ) p_sacksave );

            return ( SCTP_SUCCESS );
        }
    }

    p_sack = ( sctpsackbuf_st * ) lstFirst( sackfreelist );

    if( p_sack == NULL )
    {
        return ( SCTP_FAILURE );
    }

    p_sack->first = p_sack->last = tsn;
    lstDelete( sackfreelist, ( NODE * ) p_sack );
    lstAdd( sacklist, ( NODE * ) p_sack );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
**    FUNCTION :
**    sctp_update_message_list
**
****************************************************************************
**    DESCRIPTION:
**
**    RETURN VALUE:
**    None
**************************************************************************/
void
sctp_update_message_list(
    sctp_tcb_st    *p_ass,
    sctp_U16bit    stream,
    sctpbuffer_st   *p_buf )
{
    sctpbuffer_st *scan;
    LIST *queue = &p_ass->receive.stream[stream].receive_queue;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_update_message_list" ) );

    scan = ( sctpbuffer_st * ) lstFirst( queue );

    if( scan == 0 )
    {
        /* Empty queue,add and get out */
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "buffer added in Message list" ) );

        lstAdd( queue, ( NODE * ) p_buf );
    }

    else
    {
        while( scan )
        {
            if( scan->tsn == p_buf->tsn )
            {
                /* FIXME Panic Time, shouldn't happen */
            }

            if( sctp_compare_tsn_for_larger( scan->tsn, p_buf->tsn ) ==
                    SCTP_SUCCESS )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "buffer Inserted in Message list, previous tsn:%u, "
                              "new tsn:%u", scan->tsn, p_buf->tsn ) );

                lstInsert( queue, lstPrevious( ( NODE * ) scan ), ( NODE * ) p_buf );
                break;
            }

            scan = ( sctpbuffer_st * ) lstNext( ( NODE * ) scan );
        }

        if( !scan )
        {
            /* Scanned to the end of list ... */
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "buffer added in Message list" ) );

            lstAdd( queue, ( NODE * ) p_buf );
        }
    } /* Else Loop */
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_process_data_message_in_shutS
 *
 *    DESCRIPTION: This function is to process data message received in
 *        shutdownSent state
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_process_data_message_in_shutS(
    sctp_tcb_st    *p_ass,
    sctp_Boolean_t overrule_delayack )
{
    sctp_U8bit     flags;
    sctp_return_t  ret;
    sctp_U32bit    tsn, new_msg = 0;
    /* SPR 20279 */
    sctp_U32bit    sctp_index = 0;
    /* SPR 20279 */
    sctp_U16bit    stream, ssn;
    sctp_Boolean_t acknowledge = SCTP_FALSE;
    sctpbuffer_st   *p_buf;
    sctp_U32bit    length;
    sctp_U8bit     *chunkptr = p_ass->event.msg_data.chunk_data;
    /* Bug Id 6 fix starts */
#if 0
    /* SPR 21127 start */
    sctp_U32bit    old_cong_reg, new_cong_reg /*,old_rx_window */ ;
    /* SPR 21127 ends */
#endif
    sctp_U32bit       old_cong_reg = 0;
    sctp_U32bit       new_cong_reg = 0;
    /* Bug Id 6 fix ends*/

    sctp_U32bit    saddr_index  = 0;
    sctp_Boolean_t recv_failed = SCTP_FALSE;


    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_process_data_message" ) );

    tsn = sctp_buffer_get_UlongAtOffset( chunkptr, SCTPCHUNKOFFSET_TSN );
    stream = sctp_buffer_get_UshortAtOffset( chunkptr, SCTPCHUNKOFFSET_STREAM );
    flags = p_ass->event.msg_data.chunk_flag;
    ssn = sctp_buffer_get_UshortAtOffset( chunkptr, SCTPCHUNKOFFSET_SSN );

    /* calculate addres index */
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_data.from_addr,
                                        &sctp_index );

    /* SPR 20279 and SPR 20411 Starts */
    if( SCTP_FAILURE == ret )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "Remote Address Invalid" ) );

        /* drop message */
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                    SCTPERROR_LEVEL_CRITICAL, E_INTERNAL_ERROR,
                    "Remote Address Invalid" );

        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                     SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );
        return;
    }

    /* SPR 20279 and SPR 20411 Ends */

    if( sctp_dynamic_chk_tsn( p_ass, tsn ) == SCTP_FAILURE )
    {

        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "Duplicat TSN,TSN= %u", tsn ) );

        /* Duplicate Tsn, send immediate sack, inform duplicate tsn */
        acknowledge = SCTP_TRUE;
        overrule_delayack = SCTP_TRUE;

        if( p_ass->receive.number_duplicate_tsn < SCTP_MAXDUPLICATE_TSNS )
        {
            p_ass->receive.duplicate_tsn_list[p_ass->receive.number_duplicate_tsn] = tsn;
            p_ass->receive.number_duplicate_tsn++;
        }
    }

    else
    {
        length = sctp_buffer_get_UshortAtOffset( chunkptr,
                                                 SCTPCHUNKOFFSET_LENGTH );

        length = length - SCTPCHUNKOFFSET_PAYLOAD;
        /* SPR 21127 changes starts */
        /* Stored old receiving window */
        /* old_rx_window = sctp_db_assoc_rx_window(p_ass); */
        /* SPR 21127 changes ends */
        p_buf = sctp_get_receive_buffer( p_ass, length );

        if( p_buf == NULL )
        {
            /* no buffer received, error
             * remove any out of order chunk in the same receive queue
             */
            /*Bug Id 81 fix s*/
            if( stream < SCTP_MAX_NUMSTREAM )
            {
                /*Bug Id 81 fix e*/
                if( sctp_remove_any_unorder_chunk( p_ass, stream, tsn, length )
                        == SCTP_SUCCESS )
                {
                    p_buf = sctp_get_receive_buffer( p_ass, length );
                }

                /*Bug Id 81 fix s*/
            }

            /*Bug Id 81 fix e*/
        }

        if( p_buf != NULL )
        {
            p_buf->begin = ( flags & SCTPFLAGS_B ) ? 1 : 0;
            p_buf->end = ( flags & SCTPFLAGS_E ) ? 1 : 0;
            p_buf->unordered = ( flags & SCTPFLAGS_U ) ? 1 : 0;

            /* mark it unread */
            p_buf->read_flag = 0;
            p_buf->tsn = tsn;
            p_buf->stream = stream;
            p_buf->ssn = ssn;
            p_buf->buffer_read_index = 0;

            p_buf->ppi = sctp_buffer_get_UlongAtOffset( chunkptr,
                                                        SCTPCHUNKOFFSET_PPI );
            p_buf->size = p_ass->event.msg_data.chunk_len -
                          SCTPCHUNKOFFSET_PAYLOAD;

            /* copy the Dest addr into the Buffer */


            p_buf->dest_addr_index = sctp_index;

            if( ( stream < p_ass->receive.num_in_streams )
                    && ( p_buf->unordered == 0 )
                    && ( sctp_is_ssn_present( p_ass, stream, p_buf->ssn ) ==
                         SCTP_SUCCESS ) )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "Data with Duplicate ssn received" ) );

                /* No space in sack list , drop message */
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                            SCTPERROR_LEVEL_CRITICAL, E_INTERNAL_ERROR,
                            "Data with Duplicate ssn received" );

                SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                             SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );

                sctp_free_receive_buffer( p_ass, p_buf );
                acknowledge = SCTP_TRUE;
            }

            else
            {
                if( sctp_update_sacklist( p_ass, tsn ) != SCTP_SUCCESS )
                {
                    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                ( "No Place in Sackfree list drop message" ) );

                    /* No space in sack list , drop message */
                    SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                                SCTPERROR_LEVEL_CRITICAL, E_INTERNAL_ERROR,
                                "No Place in Sackfree list " );

                    SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                                 SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );

                    sctp_free_receive_buffer( p_ass, p_buf );
                    acknowledge = SCTP_TRUE;
                }

                else
                {
                    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                ( "\n GOT DATA!!, Stream=%d,ssn =%d,tsn =%u\n",
                                  p_buf->stream, p_buf->ssn, tsn ) );


                    /* Copy Payload data in local buffers */
                    sctp_buffer_get_Buffer( chunkptr + SCTPCHUNKOFFSET_PAYLOAD,
                                            p_buf->size, p_buf->buffer );

                    if( p_buf->stream < p_ass->receive.num_in_streams )
                    {
                        sctp_update_message_list( p_ass, p_buf->stream, p_buf );
                        new_msg = 1;
                    }

                    else
                    {
                        recv_failed = SCTP_TRUE;
                        sctp_free_receive_buffer( p_ass, p_buf );
                    }

                    acknowledge = SCTP_TRUE;

#ifdef SCTP_STATS_ENABLED

                    if( recv_failed == SCTP_FALSE )
                    {
                        /* update stats */
                        if( ( p_buf->begin == 0 ) && ( p_buf->end == 1 ) )
                        {
                            SCTP_INCR_GLOBAL_TRAFFICSTAT(
                                SCTPSTATS_NO_REASSEMBLE_USR_MSG, 1 );
                        }

                        if( p_buf->unordered == 0 )
                        {
                            SCTP_INCR_GLOBAL_TRAFFICSTAT(
                                SCTPSTATS_NO_ORD_DATACHUNKS_RECV, 1 );
                        }

                        else
                        {
                            SCTP_INCR_GLOBAL_TRAFFICSTAT(
                                SCTPSTATS_NO_UNORD_DATACHUNKS_RECV, 1 );
                        }

                        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATABYTES_RECV,
                                                      p_buf->size );

                        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATACHUNKS_RECV,
                                                      1 );

                        SCTP_INCR_ASSOCSTAT( p_ass, sctp_index,
                                             SCTPSTATS_ASSOC_DATA_RECV, 1 );

                        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                                     SCTPSTATS_ASSOC_STREAM_DATA_RECD, 1 );

                        SCTP_INCR_ASSOCSTAT( p_ass, sctp_index,
                                             SCTPSTATS_ASSOC_DATABYTES_RECV, p_buf->size );
                    }

#endif
                } /* else */ /* successful sacklist update */
            } /* else */ /* message for valid stream */

            /* SPR 21127 CHANGES START */
            SCTP_UNUSED_VARIABLE( recv_failed );

            /* SPR 21127 CHANGES END */
            if( SCTP_SUCCESS == sctp_calc_rcv_cong_reg( p_ass, &new_cong_reg ) )
            {
                old_cong_reg = p_ass->receive.congestion_event_sent;

                /*
                 * Check whether we have new congestion. And if so then give congestion
                 * indication to the user.
                 */
                if( new_cong_reg != old_cong_reg )
                {
                    p_ass->receive.congestion_event_sent = new_cong_reg;

                    /* notify su of change in congestion level */
                    sctp_ntfy_recv_congestion( p_ass, new_cong_reg );
                }
            }


        } /*  Got buffer to keep data packet */

        else
        {
            /* send partial data pending indication if applicable */
            if( p_ass->partial_data.partial_data_pending == 0 )
            {
                /*Bug Id 81 fix s*/
                if( stream < SCTP_MAX_NUMSTREAM )
                {
                    /*Bug Id 81 fix e*/
                    sctp_send_partial_ind_clear_buffers( p_ass, stream );
                    /*Bug Id 81 fix s*/
                }

                /*Bug Id 81 fix e*/

            }

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "No Receive Buffer, message dropped" ) );

            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "No Space for incoming message  !!" );

            acknowledge = SCTP_TRUE;
        }
    }

    if( acknowledge == SCTP_TRUE )
    {
        if( p_ass->event.msg_data.last_chunk_flag == SCTP_TRUE )
        {
            if( lstFirst( &p_ass->receive.sacklist ) != NULL )
            {
                overrule_delayack = SCTP_TRUE;
            }

            /* Fill in Event data */
            p_ass->receive.overrule_delayack = overrule_delayack;

            if( ( ( p_ass->receive.number_duplicate_tsn > 0 ) ||
                    ( lstFirst( &p_ass->receive.sacklist ) ) != NULL ) )
            {
                sctp_rx_state_machine( p_ass, SCTP_RX_EVENT_DATA,  sctp_index );
            }

            else
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "No duplicate or Gap block TSN, dinot require SACK" ) );
            }
        }

        /* Check if notification is required. */

        /*
         * Changed the order of execution of calls ...Vakul
        * Check for any partial data pending indication
               * There was no partial data pending indication, check for
               * any normal data pending indication
               */

        if( new_msg == 1 )
        {
            sctp_U32bit  notify = 0;


            ret = sctp_is_notification_required( p_ass, stream,
                                                 tsn, &notify );

            if( ret == SCTP_SUCCESS )
            {
                if( ( p_ass->partial_data.partial_data_pending == 1 )
                        && ( p_ass->partial_data.stream == stream )
                        && ( p_ass->partial_data.ssn == ssn ) )
                {
                    /*
                     * Do not give indication to upper layer as a partial
                     * data pending indication has already been given. The
                     * message reassembly is complete now and whole of the
                     * message will be given to the upper layer on invoking
                     * sctp_api_receive_partial.... Vakul
                     */
                }
                else
                {
                    sctp_ntfy_data_pending( p_ass, stream, ssn, notify );
                    SCTP_WAKEUP( p_ass->p_ep->cond );
                }
            }

            else
            {
                sctp_chk_send_par_notf( p_ass, stream, tsn );
            }
        }

        /*
         * If association is ECN capable and CE Bit is set
         * send ECN Echo message and start timer
         */

        if( ( p_ass->event.msg_data.CE_bit == SCTP_TRUE )
                && ( p_ass->ecn.ecn_capable == SCTP_TRUE )
                && ( p_ass->ecn.ecn_sent == 0 ) )
        {
            sctp_addrinfo_st    to;
            p_ass->ecn.ecn_sent = 1;
            p_ass->ecn.last_sent_lowest_tsn = tsn;

            /*
             * ECN is an association timer, but we
             * need address info also So call destination timer api
             */

            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERECN,
                              p_ass->transmit.remote[sctp_index].rto,
                              sctp_index,
                              &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ECN], SCTP_NULL );

            /*
             * Get a source address index corresponding to the destination
             * address type.
             */
            saddr_index = p_ass->transmit.remote[sctp_index].src_addr_index;

            to.checksum   = p_ass->config.checksum_func;
            to.p_sockaddr = &( p_ass->event.msg_data.from_addr );
            sctp_send_chunk_ecne( p_ass,
                                  &( p_ass->p_ep->this_address[saddr_index] ),
                                  p_ass->p_ep->this_port,
                                  p_ass->transmit.that_port,
                                  &to,
                                  p_ass->remote_tag, tsn );
        }
    }

    return;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_process_data_message
 *
 *    DESCRIPTION: This function is used to process data message
 *
 *    RETURN VALUE:
 *        None
 *************************************************************************/
void
sctp_process_data_message(
    sctp_tcb_st    *p_ass,
    sctp_Boolean_t overrule_delayack )
{
    sctp_U8bit     flags;
    sctp_return_t  ret;
    sctp_U32bit    tsn, new_msg = 0;
    /* SPR 20279 */
    sctp_U32bit    sctp_index = 0;
    /* SPR 20279 */
    sctp_U16bit    stream, ssn;
    sctp_Boolean_t acknowledge = SCTP_FALSE;
    sctpbuffer_st   *p_buf;
    sctp_U32bit    length;
    sctp_U8bit     *chunkptr = p_ass->event.msg_data.chunk_data;
    /* Bug Id 6 fix starts */
#if 0
    /* SPR 21127 start */
    sctp_U32bit    old_cong_reg, new_cong_reg /*,old_rx_window */ ;
    /* SPR 21127 ends ends */
#endif
    sctp_U32bit       old_cong_reg = 0;
    sctp_U32bit       new_cong_reg = 0;
    /* Bug Id 6 fix ends*/
    sctp_U32bit    saddr_index  = 0;
    sctp_Boolean_t recv_failed = SCTP_FALSE;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "sctp_process_data_message" ) );

    tsn = sctp_buffer_get_UlongAtOffset( chunkptr, SCTPCHUNKOFFSET_TSN );
    stream = sctp_buffer_get_UshortAtOffset( chunkptr, SCTPCHUNKOFFSET_STREAM );
    flags = p_ass->event.msg_data.chunk_flag;
    ssn = sctp_buffer_get_UshortAtOffset( chunkptr, SCTPCHUNKOFFSET_SSN );

    /* calculate addres index */
    ret = sctp_db_validate_remote_addr( p_ass, &p_ass->event.msg_data.from_addr,
                                        &sctp_index );

    /* SPR 20279 and SPR 20411 Starts */
    if( SCTP_FAILURE == ret )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "Remote Address Invalid" ) );

        /* drop message */
        SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                    SCTPERROR_LEVEL_CRITICAL, E_INTERNAL_ERROR,
                    "Remote Address Invalid" );

        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                     SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );
        return;
    }

    /* SPR 20279 and SPR 20411 Ends */

    if( sctp_dynamic_chk_tsn( p_ass, tsn ) == SCTP_FAILURE )
    {


        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC, ( "Duplicat TSN,TSN= %u", tsn ) );

        /* Duplicate Tsn, send immediate sack, inform duplicate tsn */
        acknowledge = SCTP_TRUE;
        overrule_delayack = SCTP_TRUE;

        if( p_ass->receive.number_duplicate_tsn < SCTP_MAXDUPLICATE_TSNS )
        {
            p_ass->receive.duplicate_tsn_list[p_ass->receive.number_duplicate_tsn] = tsn;
            p_ass->receive.number_duplicate_tsn++;
        }
    }

    else
    {
        length = sctp_buffer_get_UshortAtOffset( chunkptr,
                                                 SCTPCHUNKOFFSET_LENGTH );

        length = length - SCTPCHUNKOFFSET_PAYLOAD;

        /* SPR 21127 starts */
        /* Stored old receiving window */
        /* old_rx_window = sctp_db_assoc_rx_window(p_ass); */
        /* SPR 21127 ends */

        p_buf = sctp_get_receive_buffer( p_ass, length );

        if( p_buf == NULL )
        {
            /* no buffer received, error
             * remove any out of order chunk in the same receive queue
             */
            /*Bug Id 81 fix s*/
            if( stream < SCTP_MAX_NUMSTREAM )
            {
                /*Bug Id 81 fix e*/
                if( sctp_remove_any_unorder_chunk( p_ass, stream, tsn, length )
                        == SCTP_SUCCESS )
                {
                    p_buf = sctp_get_receive_buffer( p_ass, length );
                }

                /*Bug Id 81 fix s*/
            }

            /*Bug Id 81 fix e*/
        }

        if( p_buf != NULL )
        {
            p_buf->begin = ( flags & SCTPFLAGS_B ) ? 1 : 0;
            p_buf->end = ( flags & SCTPFLAGS_E ) ? 1 : 0;
            p_buf->unordered = ( flags & SCTPFLAGS_U ) ? 1 : 0;

            /* mark it unread */
            p_buf->read_flag = 0;
            p_buf->tsn = tsn;
            p_buf->stream = stream;
            p_buf->ssn = ssn;
            p_buf->buffer_read_index = 0;

            p_buf->ppi = sctp_buffer_get_UlongAtOffset( chunkptr,
                                                        SCTPCHUNKOFFSET_PPI );
            /* SCTP Rel 6.2.0 change start */
#if defined (SCTP_HOST_PPID)
            p_buf->ppi = htonl( p_buf->ppi );
#endif
            /* SCTP Rel 6.2.0 change end */
            p_buf->size = p_ass->event.msg_data.chunk_len -
                          SCTPCHUNKOFFSET_PAYLOAD;

            /* copy the Dest addr into the Buffer */

            p_buf->dest_addr_index = sctp_index;

            if( ( stream < p_ass->receive.num_in_streams )
                    && ( p_buf->unordered == 0 )
                    && ( sctp_is_ssn_present( p_ass, stream, p_buf->ssn ) ==
                         SCTP_SUCCESS ) )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "Data with Duplicate ssn [%d] received", p_buf->ssn ) );

                /* No space in sack list , drop message */
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                            SCTPERROR_LEVEL_CRITICAL, E_INTERNAL_ERROR,
                            "Data with Duplicate ssn received" );

                SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                             SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );

                recv_failed = SCTP_TRUE;

                sctp_free_receive_buffer( p_ass, p_buf );
                acknowledge = SCTP_TRUE;
            }

            else
            {
                if( sctp_update_sacklist( p_ass, tsn ) != SCTP_SUCCESS )
                {
                    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                ( "No Place in Sackfree list drop message" ) );

                    /* No space in sack list , drop message */
                    SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                                SCTPERROR_LEVEL_CRITICAL, E_INTERNAL_ERROR,
                                "No Place in Sackfree list " );

                    SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                                 SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );

                    recv_failed = SCTP_TRUE;

                    sctp_free_receive_buffer( p_ass, p_buf );
                    acknowledge = SCTP_TRUE;
                }

                else
                {

                    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                ( "\n GOT DATA!!, Stream=%d,ssn =%d,tsn =%u\n", p_buf->stream, p_buf->ssn, tsn ) );

                    /* Copy Payload data in local buffers */
                    sctp_buffer_get_Buffer( chunkptr + SCTPCHUNKOFFSET_PAYLOAD,
                                            p_buf->size, p_buf->buffer );

                    if( p_buf->stream < p_ass->receive.num_in_streams )
                    {
                        sctp_update_message_list( p_ass, p_buf->stream, p_buf );
                        new_msg = 1;
                    }

                    else
                    {
                        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                                    ( "Invalid Stream-Id [%d] received, dropping message",
                                      p_buf->stream ) );

                        recv_failed = SCTP_TRUE;
                        sctp_free_receive_buffer( p_ass, p_buf );
                    }

                    acknowledge = SCTP_TRUE;

#ifdef SCTP_STATS_ENABLED

                    if( recv_failed == SCTP_FALSE )
                    {
                        /* update stats */
                        if( ( p_buf->begin == 0 ) && ( p_buf->end == 1 ) )
                        {
                            SCTP_INCR_GLOBAL_TRAFFICSTAT(
                                SCTPSTATS_NO_REASSEMBLE_USR_MSG, 1 );
                        }

                        if( p_buf->unordered == 0 )
                        {
                            SCTP_INCR_GLOBAL_TRAFFICSTAT(
                                SCTPSTATS_NO_ORD_DATACHUNKS_RECV, 1 );
                        }

                        else
                        {
                            SCTP_INCR_GLOBAL_TRAFFICSTAT(
                                SCTPSTATS_NO_UNORD_DATACHUNKS_RECV, 1 );
                        }

                        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATABYTES_RECV,
                                                      p_buf->size );

                        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATACHUNKS_RECV,
                                                      1 );

                        SCTP_INCR_ASSOCSTAT( p_ass, sctp_index,
                                             SCTPSTATS_ASSOC_DATA_RECV, 1 );

                        SCTP_INCR_ASSOCSTAT( p_ass, sctp_index,
                                             SCTPSTATS_ASSOC_DATABYTES_RECV, p_buf->size );

                        /* REL 6.2.0 change start */
                        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                                     SCTPSTATS_ASSOC_STREAM_DATA_RECD, 1 );
                        /* REL 6.2.0 change end */
                    }

#endif
                } /* else */ /* successful sacklist update */
            } /* else */ /* message for valid stream */

            if( SCTP_SUCCESS == sctp_calc_rcv_cong_reg( p_ass, &new_cong_reg ) )
            {
                old_cong_reg = p_ass->receive.congestion_event_sent;

                /*
                 * Check whether we have new congestion. And if so then give congestion
                 * indication to the user.
                 */
                if( new_cong_reg != old_cong_reg )
                {
                    p_ass->receive.congestion_event_sent = new_cong_reg;

                    /* notify su of change in congestion level */
                    sctp_ntfy_recv_congestion( p_ass, new_cong_reg );
                }
            }


        } /*  Got buffer to keep data packet */

        else
        {
            /* send partial data pending indication if applicable */
            if( p_ass->partial_data.partial_data_pending == 0 )
            {
                /*Bug Id 81 fix s*/
                if( stream < SCTP_MAX_NUMSTREAM )
                {
                    /*Bug Id 81 fix e*/
                    sctp_send_partial_ind_clear_buffers( p_ass, stream );
                    /*Bug Id 81 fix s*/
                }

                /*Bug Id 81 fix e*/
            }

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "No Receive Buffer, message dropped" ) );
            /* Rel 6.2.0 change start */
            SCTP_INCR_ASSOC_STREAM_STAT( p_ass, stream,
                                         SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL, 1 );
            /* Rel 6.2.0 change end */
            SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR, SCTPERROR_LEVEL_CRITICAL,
                        E_INTERNAL_ERROR, "No Space for incoming message  !!" );

            acknowledge = SCTP_TRUE;
        }
    }

    if( acknowledge == SCTP_TRUE )
    {
        if( p_ass->event.msg_data.last_chunk_flag == SCTP_TRUE )
        {
            if( lstFirst( &p_ass->receive.sacklist ) != NULL )
            {
                overrule_delayack = SCTP_TRUE;
            }

            /* Fill in Event data */
            p_ass->receive.overrule_delayack = overrule_delayack;
            /* Changes For CSR#1-5037386 */
            /* Fill in Sack destination related fields */
            p_ass->transmit.default_sack_dest_index = sctp_index;
            p_ass->transmit.remote[sctp_index].prefered_sack_dest = SCTP_TRUE;
            /* Changes For CSR#1-5037386 Ends */
            sctp_rx_state_machine( p_ass, SCTP_RX_EVENT_DATA, sctp_index );
        }

        /* Check if notification is required. */

        /*
         * Changed the order of execution of calls ...Vakul
        * Check for any partial data pending indication
               * There was no partial data pending indication, check for
               * any normal data pending indication
               */

        if( new_msg == 1 )
        {
            sctp_U32bit  notify = 0;

            ret = sctp_is_notification_required( p_ass, stream,
                                                 tsn, &notify );

            if( ret == SCTP_SUCCESS )
            {
                if( ( p_ass->partial_data.partial_data_pending == 1 )
                        && ( p_ass->partial_data.stream == stream )
                        && ( p_ass->partial_data.ssn == ssn ) )
                {
                    /*
                     * Do not give indication to upper layer as a partial
                     * data pending indication has already been given. The
                     * message reassembly is complete now and whole of the
                     * message will be given to the upper layer on invoking
                     * sctp_api_receive_partial.... Vakul
                     */
                }
                else
                {
                    sctp_ntfy_data_pending( p_ass, stream, ssn, notify );
                    SCTP_WAKEUP( p_ass->p_ep->cond );
                }
            }

            else
            {
                sctp_chk_send_par_notf( p_ass, stream, tsn );
            }
        }

        /*
         * If association is ECN capable and CE Bit is set
         * send ECN Echo message and start timer
         */

        if( ( p_ass->event.msg_data.CE_bit == SCTP_TRUE )
                && ( p_ass->ecn.ecn_capable == SCTP_TRUE )
                && ( p_ass->ecn.ecn_sent == 0 ) )
        {
            sctp_addrinfo_st    to;
            p_ass->ecn.ecn_sent = 1;
            p_ass->ecn.last_sent_lowest_tsn = tsn;

            /*
             * ECN is an association timer, but we
             * need address info also So call destination timer api
             */

            sctp_start_timer( p_ass,
                              SCTPEVENT_TIMERECN,
                              p_ass->transmit.remote[sctp_index].rto,
                              sctp_index,
                              &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_ECN], SCTP_NULL );

            /*
             * Get a source address index corresponding to the destination
             * address type.
             */
            saddr_index = p_ass->transmit.remote[sctp_index].src_addr_index;

            to.checksum   = p_ass->config.checksum_func;
            to.p_sockaddr = &( p_ass->event.msg_data.from_addr );
            sctp_send_chunk_ecne( p_ass,
                                  &( p_ass->p_ep->this_address[saddr_index] ),
                                  p_ass->p_ep->this_port,
                                  p_ass->transmit.that_port,
                                  &to,
                                  p_ass->remote_tag, tsn );
        }
    }

    /* SPR 21127 CHANGES START */
    SCTP_UNUSED_VARIABLE( recv_failed );
    /* SPR 21127 CHANGES END */

    return;
}

/***************************************************************************
**
** FUNCTION NAME:       sctp_calc_recv_cong_reg
**
** DESCRIPTION  :       This function evaluate transmit congestion region.
**                      If there is change in cong region give notify.
**
** RETURNS      :      None
**
***************************************************************************/
sctp_return_t   sctp_calc_rcv_cong_reg(
    sctp_tcb_st         *p_assoc,
    sctp_U32bit         *p_new_cong_reg )

{
    sctp_U32bit      new_buffer_occupancy = 0;
    sctp_U32bit      max_rx_window        = 0;
    sctp_U32bit      new_rx_window        = 0;
    sctp_U32bit      old_cong_reg     = p_assoc->receive.congestion_event_sent;
    sctp_cong_reg_def_st cong_reg_def = p_assoc->rcv_cong_reg_def;

    new_rx_window = sctp_db_assoc_rx_window( p_assoc );
    max_rx_window = sctp_db_assoc_max_rx_window( p_assoc );

    /* Calculate buffer occupancy */
    new_buffer_occupancy =  100 - ( 100 * new_rx_window / max_rx_window );

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
        /* Congestion region is LCR */
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
 *    FUNCTION :
 *        sctp_build_data_chunk_header
 *
 *    DESCRIPTION: This function builds header for DATA chunk
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *************************************************************************/
sctp_return_t
sctp_build_data_chunk_header(
    sctp_U8bit     *p_buf,
    sctpbuffer_st  *sctp_buf )
{
    sctp_U8bit flags = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC, ( "sctp_build_data_chunk_header" ) );

    if( sctp_buf->unordered )
    {
        flags |= SCTPFLAGS_U;
    }

    if( sctp_buf->begin )
    {
        flags |= SCTPFLAGS_B;
    }

    if( sctp_buf->end )
    {
        flags |= SCTPFLAGS_E;
    }

    p_buf = sctp_buffer_set_Uchar( p_buf, SCTPCHUNK_DATA ); /* Type */
    p_buf = sctp_buffer_set_Uchar( p_buf, flags ); /* Flags */
    p_buf = sctp_buffer_set_Ushort( p_buf,
                                    ( sctp_U16bit )( sctp_buf->size + SCTPCHUNKOFFSET_PAYLOAD ) );

    p_buf = sctp_buffer_set_Ulong( p_buf, sctp_buf->tsn ); /* TSN  */
    p_buf = sctp_buffer_set_Ushort( p_buf, sctp_buf->stream ); /* Stream */
    p_buf = sctp_buffer_set_Ushort( p_buf, sctp_buf->ssn );  /* SSN */
    p_buf = sctp_buffer_set_Ulong( p_buf, sctp_buf->ppi ); /* Prot Identifier*/

    /* SPR 21107 changes start */
    SCTP_UNUSED_VARIABLE( p_buf );
    /* SPR 21107 changes end */
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_update_tsn_and_ssn
 *
 *    DESCRIPTION: This function updates TSN and SSN feilds of DATA and
 *        builds DATA chunk header
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *************************************************************************/
sctp_return_t
sctp_update_tsn_and_ssn(
    sctp_tcb_st   *p_ass,
    sctpbuffer_st  *p_buf )
{
    sctpbuffer_st *tmp = SCTP_NULL;

    if( p_buf->begin == 1 )
    {
        p_buf->tsn = p_ass->transmit.tsn++;

        if( !p_buf->unordered )
        {
            p_buf->ssn = p_ass->transmit.ssn[p_buf->stream];
        }

        if( p_buf->end != 1 )
        {
            for( tmp = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ); ( tmp != NULL );
                    tmp = ( sctpbuffer_st * ) lstNext( ( NODE * ) tmp ) )
            {
                tmp->tsn = p_ass->transmit.tsn++;

                if( !p_buf->unordered )
                {
                    tmp->ssn = p_ass->transmit.ssn[p_buf->stream];
                }

                if( tmp->end == 1 )
                {
                    break;
                }
            }

            if( !tmp )
            {
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                            SCTPERROR_LEVEL_CRITICAL, E_INVALID_SACK_CHUNK,
                            "Undating TSN and SSN , ERROR !!" );

                return ( SCTP_FAILURE );
            }
        }

        if( !p_buf->unordered )
        {
            p_ass->transmit.ssn[p_buf->stream]++;
        }
    }

    /*SPR 4970*/
    sctp_build_data_chunk_header( p_buf->buffer - SCTPCHUNKOFFSET_PAYLOAD,
                                  p_buf );
    sctp_buffer_PadTo( p_buf->buffer + p_buf->size, p_buf->buffer, 4 );
    /*SPR 4970*/
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_senddata_on_alternate_addr
 *
 *    DESCRIPTION: This function send data on alternate address index
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 *************************************************************************/
sctp_return_t
sctp_senddata_on_alternate_addr(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    prev_addr_index,
    sctp_U32bit    new_addr_index )
{
    sctp_U32bit send_status = SCTP_SUCCESS;
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_senddata_on_alternate_addr: Called"
                  " prev_addr_index=%u new_addr_index=%u", prev_addr_index,
                  new_addr_index ) );

    if( p_ass->transmit.remote[prev_addr_index].retrans_packet_count > 0 )
    {
        send_status = sctp_send_one_mtu_on_alternate_addr(
                          p_ass, prev_addr_index, new_addr_index );
    }

    else
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_senddata_on_alternate_addr: not called "
                      "sctp_senddata_one_mtu_on_alternate_addr as Not required" ) );
    }

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_senddata_on_alternate_addr: going to call"
                  " sctp_senddata_on_addr for prev_addr_index" ) );

    if( ( p_ass->transmit.remote[prev_addr_index].unsent_packet_count > 0 )
            && ( SCTP_NULL_TIMER_ID == p_ass->transmit.remote[prev_addr_index].dest_timer_id[SCTP_DESTTIMER_RTX] )
            && ( SCTP_NULL_TIMER_ID == p_ass->transmit.remote[prev_addr_index].dest_timer_id[SCTP_DESTTIMER_BUNDLING] ) )
    {
        if( SCTP_SUCCESS == sctp_senddata_on_addr_if_possible( p_ass, prev_addr_index ) )
        {
            p_ass->transmit.remote[prev_addr_index].state = SCTP_TX_STATE_AWTACK;
        }
    }

    else
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_senddata_on_alternate_addr: Not call"
                      " sctp_senddata_on_addr for prev_addr_index as not needed" ) );

    }

    return send_status;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_get_sack_dest_address_and_reset_flags
 *
 *    DESCRIPTION:
 *        This function check whether sack can be sent on provided
 *        dest_index. if not it returns last dest addr index over which data
 *        has been received from peer. It also resets sack deliverable flag
 *        for all destination addresses.
 *
 *    RETURN VALUE:
 *        destination index where sack can be sent
 *************************************************************************/
sctp_U32bit sctp_get_sack_dest_address_and_reset_flags(
    sctp_tcb_st *p_ass,
    sctp_U32bit dest_index )
{
    sctp_U32bit ret_index;
    sctp_U32bit counter;
    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_get_sack_dest_address_and_reset_flags: Called" ) );
    ret_index = ( p_ass->transmit.remote[dest_index].prefered_sack_dest ) ? dest_index :
                p_ass->transmit.default_sack_dest_index;

    for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
    {
        p_ass->transmit.remote[counter].prefered_sack_dest = SCTP_FALSE;
    }

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_get_sack_dest_address_and_reset_flags: Exit addr_index=%u"
                  " returned addr_index =%u", dest_index, ret_index ) );
    return ret_index;
}

/***************************************************************************
 *    FUNCTION :
 *        sctp_send_one_mtu_on_alternate_addr
 *
 *    DESCRIPTION: This function send data on alternate address index
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 *************************************************************************/
sctp_U32bit
sctp_send_one_mtu_on_alternate_addr(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    prev_addr_index,
    sctp_U32bit    new_addr_index )
{
    sctp_U8bit       *P;
    sctp_U8bit       *sctp_offset;
    sctp_U8bit       *buffer;
    sctpbuffer_st    *p_buf;
    sctp_U32bit      numchunks = 0;
    sctp_U32bit      resentdatachunks = 0;
    sctp_U32bit      sack_included = 0;
    sctp_U32bit      sack_len = 0;
    sctp_sockaddr_st *p_to;
    sctp_U32bit      saddr_index;
    sctp_U32bit      lowest_tsn = 0;
    sctpmtubuf_st    *p_mtubuf = &( p_ass->mtu_buffer );

    sctp_iovec_st    p_iovec[SCTP_MAX_IP_SCATTER_BUFFERS];
    sctp_U32bit      packet_size = 0;
    sctp_U32bit      new_packet_size = 0;

    /* Changes for SPR-21022 Starts here (Klocwork) */
    if( ( p_ass == SCTP_NULL ) || ( p_ass->p_ep == SCTP_NULL ) )
    {
        return SCTP_FAILURE;
    }

    /* Changes for SPR-21022 Ends here (Klocwork) */


    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_send_one_mtu_on_alternate_addr, old:%u,new:%u",
                  prev_addr_index, new_addr_index ) );

    buffer = p_mtubuf->buffer;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_send_one_mtu_on_alternate_addr, old:%u,new:%u",
                  prev_addr_index, new_addr_index ) );

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    p_iovec[1].iov_base = ( char * )sctp_offset;

    P = sctp_build_common_header( sctp_offset, p_ass->p_ep->this_port,
                                  p_ass->transmit.that_port, p_ass->remote_tag );


    /* Optionally include Sack chunk */
    p_ass->rx_event.evt_cmduserdata.buffer_offset = P;
    sctp_rx_state_machine( p_ass, SCTP_RX_EVENT_USERDATA, new_addr_index );

    if( p_ass->rx_event.evt_cmduserdata.buffer_offset != P )
    {
        sctp_U32bit sack_dest_index;

        if( new_addr_index ==
                ( sack_dest_index =
                      sctp_get_sack_dest_address_and_reset_flags( p_ass,
                                                                  new_addr_index ) ) )
        {
            /* set sack included parameter */
            sack_included = 1;
            sack_len = p_ass->rx_event.evt_cmduserdata.buffer_offset - P;
            P = p_ass->rx_event.evt_cmduserdata.buffer_offset;
        }

        else
        {
            sctp_U8bit       *sack_chunk_end;
            sack_chunk_end = p_ass->rx_event.evt_cmduserdata.buffer_offset;
            sctp_insert_chksum( sack_chunk_end, sctp_offset, p_ass->config.checksum_func );

            p_to = &( p_ass->transmit.remote[sack_dest_index].that_address );
            saddr_index
                = p_ass->transmit.remote[sack_dest_index].src_addr_index;

            /* Callback for Sending Func SPR 20859 Starts */
            if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
            {
                p_ass->p_ep->send_func( buffer, ( sack_chunk_end - buffer ), SCTP_FALSE, p_to, &p_ass->p_ep->this_address[saddr_index], SCTP_FALSE );
            }

            else
            {
                /* df_flag is set to SCTP_FALSE as only SACK is being sent */
                sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
                    buffer, ( sack_chunk_end - buffer ), SCTP_FALSE, p_to,
                    &p_ass->p_ep->this_address[saddr_index] );
            }

            /* Callback for Sending Func SPR 20859 Stops */

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );
        }
    }

    p_iovec[1].iov_len  = P - sctp_offset;
    packet_size         = P - buffer;

    p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

    for( ;; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
    {
        if( p_buf == NULL ) /* No data chunk */
        {
            break;
        }

        if( p_buf->sent == 0 )
        {
            continue;
        }

        if( p_buf->ack_flag == SCTP_ACKFLAG_ACKBYGAPBLOCK )
        {
            /* This chunk was acked by a gap block. Hence it need not be
             * retransmitted.
             */

            continue;
        }

        /* If packet has already been re-transmitted and it not yet marked
         * for next retransmission, skip this packet */
        if( p_buf->retrans_flag != SCTP_RETRANSFLAG_MARK )
        {
            /* skip already sent datagram */
            continue;
        }

        if( p_buf->dest_addr_index != prev_addr_index )
        {
            continue;
        }

        /* SPR 2467 */
        /* Send SACK chunk in a different packet, if inclusion of a single
         * data chunk will cross the MTU
         */
        new_packet_size =
            packet_size + SCTPCHUNKOFFSET_PAYLOAD + ROUND( p_buf->size );

        if( ( numchunks == 0 ) && ( sack_included == 1 )
                && ( new_packet_size >= p_ass->transmit.lowest_path_mtu ) )
        {
            sctp_insert_chksum( P, sctp_offset, p_ass->config.checksum_func );

            p_to = &( p_ass->transmit.remote[new_addr_index].that_address );
            saddr_index
                = p_ass->transmit.remote[new_addr_index].src_addr_index;

            /* Callback for Sending Func SPR 20859 Starts */
            if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
            {
                p_ass->p_ep->send_func( buffer, ( P - buffer ), SCTP_FALSE, p_to,
                                        &p_ass->p_ep->this_address[saddr_index], SCTP_FALSE );
            }

            else
            {
                /* df_flag is set to SCTP_FALSE as only SACK is being sent */
                sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
                    buffer, ( P - buffer ), SCTP_FALSE, p_to,
                    &p_ass->p_ep->this_address[saddr_index] );
            }

            /* Callback for Sending Func SPR 20859 Stops*/
            /* Does not contain sack */
            p_iovec[1].iov_len  -= sack_len;
            packet_size -= sack_len;
            new_packet_size -= sack_len;

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );
        }

        /* if it is a first data chunk bypass the MTU
         * rule and allow the chunk to be sent else whenever addition of new
         * chunk will result in the MTU being crossed, donot include the chunk
         */

        if( ( numchunks != 0 )
                && ( new_packet_size > p_ass->transmit.lowest_path_mtu ) )
        {
            /* Would cross MTU ... */

            break;
        }

        /* OK, now we are commited to transmitting this one ... */
        /*
         * Use Destination Address index instead.
        */

        p_buf->dest_addr_index = new_addr_index;

        p_iovec[numchunks + 2].iov_base =
            ( char * )( p_buf->buffer - SCTPCHUNKOFFSET_PAYLOAD );

        p_iovec[numchunks + 2].iov_len =
            ROUND( p_buf->size ) + SCTPCHUNKOFFSET_PAYLOAD;
        packet_size = new_packet_size;


        /* Start Timer if not already running */
        if( ( p_ass->transmit.remote[new_addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX] == SCTP_NULL_TIMER_ID ) ||
                ( sctp_compare_tsn_for_smaller( p_buf->tsn,
                                                p_ass->transmit.remote[new_addr_index].lowest_tsn ) == SCTP_SUCCESS ) )
        {
            sctp_stop_timer(
                & p_ass->transmit.remote[new_addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );

            sctp_start_timer(
                p_ass,
                SCTPEVENT_TIMERDATA,
                p_ass->transmit.remote[new_addr_index].rto,
                new_addr_index,
                &p_ass->transmit.remote[new_addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL );

            p_ass->transmit.remote[new_addr_index].lowest_tsn = p_buf->tsn;
            p_ass->transmit.remote[new_addr_index].state = SCTP_TX_STATE_AWTACK;
        }

        /* update outstanding values */
        if( p_buf->ack_flag == SCTP_ACKFLAG_UNACKED )
        {
            /* This chunk was sent but not acked on prev addr,
             * update outstanding only
             */
            p_ass->transmit.remote[prev_addr_index].outstanding -= p_buf->size;
            p_ass->transmit.remote[new_addr_index].outstanding += p_buf->size;
        }

        /* This is retransmission case, update previous addr parameters */
        p_ass->transmit.remote[prev_addr_index].num_chunks_await_ack--;
        resentdatachunks++;

        p_ass->transmit.remote[new_addr_index].num_chunks_await_ack++;

        if( p_ass->transmit.rwndthere >= ( sctp_U32bit )p_buf->size )
        {
            p_ass->transmit.rwndthere -= p_buf->size;
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATABYTES_SENT, p_buf->size );

        if( p_buf->unordered == 1 )
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNORD_DATACHUNKS_SENT, 1 );
        }

        else
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ORD_DATACHUNKS_SENT, 1 );
        }

        SCTP_INCR_ASSOCSTAT( p_ass, new_addr_index,
                             SCTPSTATS_ASSOC_DATABYTES_SENT, p_buf->size );
        /* SCTP Rel 6.2.0 changes start */
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATABYTES_SENT, p_buf->size );
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATA_SENT, 1 );
        /* Rel 6.2.0 change end */

        /* SCTP Rel 6.2.0 changes end */
        numchunks++;

        /* Update retrans flag before sending datagram */
        if( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )
        {
            p_buf->retrans_flag = SCTP_RETRANSFLAG_RETRANSMITTED;
            p_ass->transmit.remote[prev_addr_index].retrans_packet_count--;

            SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_CHUNKS_RETRANS, 1 );
            SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream, SCTPSTATS_ASSOC_STREAM_DATA_RESENT, 1 );

        }

        p_buf->missed = 0;
    }

    if( prev_addr_index != new_addr_index )
    {
        if( p_ass->transmit.remote[prev_addr_index].outstanding == 0 )
        {
            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctp_send_one_mtu_on_alternate_addr:NO more data on addr index: %u,"
                          "Stop Timer", prev_addr_index ) );

            p_ass->transmit.remote[prev_addr_index].state = SCTP_TX_STATE_NULL;

            if( p_ass->transmit.remote[prev_addr_index].
                    dest_timer_id[SCTP_DESTTIMER_RTX] != SCTP_NULL_TIMER_ID )
            {
                SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                            ( "sctp_send_one_mtu_on_alternate_addr:NO data on index: %u, Timer was still Running",
                              prev_addr_index ) );
                sctp_stop_timer(
                    & p_ass->transmit.remote[prev_addr_index].
                    dest_timer_id[SCTP_DESTTIMER_RTX],
                    SCTP_NULL, SCTP_NULL );
            }
        }

        else
        {

            SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                        ( "sctp_send_one_mtu_on_alternate_addr:updating lowest tsn on addr index= %u", prev_addr_index ) );

            if( sctp_get_lowest_tsn_on_addr( p_ass, prev_addr_index,
                                             &lowest_tsn ) != SCTP_SUCCESS )
            {
                SCTP_ERROR( SCTPERROR_FLAG_INTERNAL_ERROR,
                            SCTPERROR_LEVEL_CRITICAL, E_INVALID_SACK_CHUNK,
                            "At sctp_send_one_mtu_on_alternate_addr:"
                            " Updating lowest tsn on prev addr, Invalid tsn" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_INTERNAL_ERRORS, 1 );

                /*
                 * Consider Sending sending Abort and deleting association in
                 * these cases
                 */
                return SCTP_FAILURE;
            }

            /* In this case, we need to start timer when lowest TSN is not
             * equal to TSN for which timer is running
             */
            if( lowest_tsn !=
                    p_ass->transmit.remote[prev_addr_index].lowest_tsn )
            {
                sctp_stop_timer(
                    & p_ass->transmit.remote[prev_addr_index].
                    dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );

                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERDATA,
                    p_ass->transmit.remote[prev_addr_index].rto,
                    prev_addr_index,
                    & p_ass->transmit.remote[prev_addr_index].
                    dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL );

                p_ass->transmit.remote[prev_addr_index].lowest_tsn = lowest_tsn;
                p_ass->transmit.remote[prev_addr_index].state =
                    SCTP_TX_STATE_AWTACK;
            }

            else if( p_ass->transmit.remote[prev_addr_index].
                     dest_timer_id[SCTP_DESTTIMER_RTX] == SCTP_NULL_TIMER_ID )
            {
                /* Start retransmission timer */
                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERDATA,
                    p_ass->transmit.remote[prev_addr_index].rto,
                    prev_addr_index,
                    & p_ass->transmit.remote[prev_addr_index].
                    dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL );

                p_ass->transmit.remote[prev_addr_index].lowest_tsn = lowest_tsn;
                p_ass->transmit.remote[prev_addr_index].state =
                    SCTP_TX_STATE_AWTACK;

            }
        }
    }

    if( !numchunks && !sack_included )
    {
        return SCTP_FAILURE;
    }

    sctp_buffer_set_UlongAtOffset( sctp_offset, SCTP_OFFSET_CKSUM, 0 );

    sctp_buffer_set_UlongAtOffset( sctp_offset, SCTP_OFFSET_CKSUM,
                                   sctp_compute_cksm_iovec[p_ass->config.checksum_func](
                                       p_iovec + 1, numchunks + 1 ) );

    SCTP_INCR_ASSOCSTAT( p_ass, new_addr_index,
                         SCTPSTATS_ASSOC_DATA_SENT, numchunks );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATACHUNKS_SENT, numchunks );
    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "sctp_send_one_mtu_on_alternate_addr:Chunk DATA Sent,Src Port=%d,Dest Port=%d, "
                  "Ver Tag=%u", p_ass->p_ep->this_port, p_ass->transmit.that_port,
                  p_ass->remote_tag ) );

    SCTP_TRACE_ADDR( p_ass->p_ep, SCTP_DBG_TRC,
                     ( &( p_ass->transmit.remote[new_addr_index].that_address ) ) );

    /* Sends the message to the appropriate node */
    p_to = &( p_ass->transmit.remote[new_addr_index].that_address );
    saddr_index = p_ass->transmit.remote[new_addr_index].src_addr_index;

    p_iovec[0].iov_base = ( char * )buffer;
    p_iovec[0].iov_len  = packet_size;  /*this required in IP header*/

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( ( sctp_U8bit * )p_iovec, numchunks + 2, SCTP_FALSE, p_to,
                                &( p_ass->p_ep->this_address[saddr_index] ), SCTP_TRUE );
    }

    else
    {

        /* df_flag is set to SCTP_FALSE as this data being retramsnitted */
        sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4_IOV : SCTP_V6_IOV](
            ( sctp_U8bit * )p_iovec, numchunks + 2, SCTP_FALSE, p_to,
            &( p_ass->p_ep->this_address[saddr_index] ) );
    }

    /* Callback for Sending Func SPR 20859 Stops*/

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_RETRANS_DATACHUNKS, resentdatachunks );

    if( p_ass->zero_win_prob_on != SCTP_TRUE )
    {
        /* Update the last used time parameter if heartbeat enabled */
        if( p_ass->transmit.remote[new_addr_index].allow_heartbeat == 1 )
        {
            __GETSYSTIME( &p_ass->transmit.remote[new_addr_index].last_used_time );
        }
    }

    return SCTP_SUCCESS;
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_send_one_mtu
 *
 *    DESCRIPTION: This function is to send data of one mtu size.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 *************************************************************************/
sctp_U32bit
sctp_send_one_mtu(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_U8bit       *buffer;
    sctp_U8bit       *P;
    sctp_U8bit       *sctp_offset;
    sctpbuffer_st    *p_buf;
    sctp_U32bit      numchunks = 0;
    sctp_U32bit      sentdatachunks = 0;
    sctp_U32bit      resentdatachunks = 0;
    sctp_U32bit      sack_included = 0;
    sctp_U32bit      sack_len = 0;
    sctp_sockaddr_st *p_to;
    sctp_U32bit      saddr_index;
    sctp_U32bit      new_chunk = 0;

    sctp_iovec_st    p_iovec[SCTP_MAX_IP_SCATTER_BUFFERS];
    sctp_U32bit      packet_size = 0;
    sctp_U32bit      new_packet_size = 0;
    sctp_return_t    ret = SCTP_SUCCESS;
    sctpmtubuf_st    *p_mtubuf = &( p_ass->mtu_buffer );

    buffer = p_mtubuf->buffer;

    /* Changes for SPR-21022 Starts here (Klocwork) */
    if( ( p_ass == SCTP_NULL ) || ( p_ass->p_ep == SCTP_NULL ) )
    {
        return SCTP_FAILURE;
    }

    /* Changes for SPR-21022 Ends here (Klocwork) */

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_send_one_mtu,addr_index=%u", addr_index ) );

    if( SCTP_IS_STACK_UDP )
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE + SCTP_UDP_HDR_SIZE;
    }

    else
    {
        sctp_offset = buffer + SCTP_MAX_IP_HEADER_SIZE;
    }

    P = sctp_offset;
    p_iovec[1].iov_base = ( char * )P;

    P = sctp_build_common_header( P, p_ass->p_ep->this_port,
                                  p_ass->transmit.that_port, p_ass->remote_tag );

    /* Optionally include Sack chunk */
    p_ass->rx_event.evt_cmduserdata.buffer_offset = P;
    sctp_rx_state_machine( p_ass, SCTP_RX_EVENT_USERDATA, addr_index );

    if( p_ass->rx_event.evt_cmduserdata.buffer_offset != P )
    {
        sctp_U32bit sack_dest_index;

        if( addr_index ==
                ( sack_dest_index =
                      sctp_get_sack_dest_address_and_reset_flags( p_ass,
                                                                  addr_index ) ) )
        {
            /* set sack included parameter */
            sack_included = 1;
            sack_len = p_ass->rx_event.evt_cmduserdata.buffer_offset - P;
            P = p_ass->rx_event.evt_cmduserdata.buffer_offset;
        }

        else
        {
            sctp_U8bit  *sack_chunk_end;
            sack_chunk_end = p_ass->rx_event.evt_cmduserdata.buffer_offset;
            sctp_insert_chksum( sack_chunk_end, sctp_offset, p_ass->config.checksum_func );

            p_to = &( p_ass->transmit.remote[sack_dest_index].that_address );
            saddr_index
                = p_ass->transmit.remote[sack_dest_index].src_addr_index;

            /* Callback for Sending Func SPR 20859 Starts */
            if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
            {
                p_ass->p_ep->send_func( buffer, ( sack_chunk_end - buffer ), SCTP_FALSE,
                                        p_to, &p_ass->p_ep->this_address[saddr_index], SCTP_FALSE );

            }

            else
            {

                /* df_flag is set to SCTP_FALSE as only SACK is being sent */
                sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
                    buffer, ( sack_chunk_end - buffer ), SCTP_FALSE, p_to,
                    &p_ass->p_ep->this_address[saddr_index] );
            }

            /* Callback for Sending Func SPR 20859 Stops */

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );
        }
    }

    p_iovec[1].iov_len = P - sctp_offset;
    packet_size = P - buffer;

    p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

    for( ;; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
    {
        new_chunk = 0;

        if( p_buf == SCTP_NULL ) /* No data */
        {
            break;
        }

        if( p_buf->dest_addr_index != addr_index )
        {
            continue;
        }

        if( p_buf->sent == 1 )
        {
            continue;
        }

        new_packet_size =
            packet_size + SCTPCHUNKOFFSET_PAYLOAD + ROUND( p_buf->size );

        if( ( numchunks == 0 ) && ( sack_included == 1 )
                && ( new_packet_size  >= p_ass->transmit.lowest_path_mtu ) )
        {
            sctp_insert_chksum( P, sctp_offset, p_ass->config.checksum_func );

            p_to = &( p_ass->transmit.remote[addr_index].that_address );
            saddr_index
                = p_ass->transmit.remote[addr_index].src_addr_index;

            /* Callback for Sending Func SPR 20859 Starts */
            if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
            {
                p_ass->p_ep->send_func( buffer, ( P - buffer ), SCTP_FALSE, p_to,
                                        &p_ass->p_ep->this_address[saddr_index], SCTP_FALSE );

            }

            else
            {
                /* df_flag is set to SCTP_FALSE as only SACK is being sent */
                sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4 : SCTP_V6](
                    buffer, ( P - buffer ), SCTP_FALSE, p_to,
                    &p_ass->p_ep->this_address[saddr_index] );
            }

            /* Callback for Sending Func SPR 20859 Stops*/

            /* Does not contain sack */
            p_iovec[1].iov_len  -= sack_len;
            packet_size -= sack_len;
            new_packet_size -= sack_len;

            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );
        }

        /* if it is the first data chunk bypass the MTU
         * rule and allow the chunk to be sent else whenever addition of new
         * chunk will result in MTU being crossed, donot include the chunk
         */

        if( ( numchunks != 0 )
                && ( new_packet_size > p_ass->transmit.lowest_path_mtu ) )
        {
            /* Would cross MTU ..., */
            break;
        }

        /*update the congestion window*/
        if( ( new_packet_size + ( sctp_db_main.sctp_def_prot_params.max_burst *
                                  p_ass->transmit.remote[addr_index].path_mtu ) ) <
                p_ass->transmit.remote[addr_index].cwnd )
        {
            p_ass->transmit.remote[addr_index].cwnd =
                new_packet_size + ( sctp_db_main.sctp_def_prot_params.max_burst *
                                    p_ass->transmit.remote[addr_index].path_mtu );
        }

        if( p_ass->transmit.remote[addr_index].outstanding + p_buf->size
                > ( p_ass->transmit.remote[addr_index].cwnd
                    + p_ass->transmit.remote[addr_index].path_mtu - 1 ) )
        {
            /* Would cross congestion window ... */
            break;
        }

        if( p_ass->transmit.total_outstanding + p_buf->size >
                p_ass->transmit.rwndthere )
        {
            /* Would cross receiver buffer window*/
            break;
        }

        p_iovec[numchunks + 2].iov_base =
            ( char * )( p_buf->buffer - SCTPCHUNKOFFSET_PAYLOAD );

        p_iovec[numchunks + 2].iov_len =
            ROUND( p_buf->size ) + SCTPCHUNKOFFSET_PAYLOAD;
        packet_size = new_packet_size;

        /* update outstanding values, when we have never sent them before */
        /*
         * Update TSN and SSN fields,
         * This function also build the chunk in case it is to be sent
         * for first time
         */
        sctp_update_tsn_and_ssn( p_ass, p_buf );
        p_ass->transmit.remote[addr_index].outstanding += p_buf->size;
        p_ass->transmit.total_outstanding += p_buf->size;
        new_chunk = 1;
        p_buf->sent = 1;
        p_ass->total_sent++;
        p_ass->transmit.remote[addr_index].num_chunks_await_ack++;
        p_ass->transmit.remote[addr_index].unsent_packet_count--;
        p_ass->transmit.highest_outstanding = p_buf->tsn;
        p_ass->transmit.highest_tsn_sent    = p_buf->tsn;
        sentdatachunks++;

        p_ass->transmit.rwndthere -= p_buf->size;

        /* Start Timer if not already running */
        if( ( p_ass->transmit.remote[addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX] == SCTP_NULL_TIMER_ID ) ||
                ( sctp_compare_tsn_for_smaller( p_buf->tsn,
                                                p_ass->transmit.remote[addr_index].lowest_tsn ) == SCTP_SUCCESS ) )
        {
            sctp_stop_timer(
                & p_ass->transmit.remote[addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );

            sctp_start_timer( p_ass, SCTPEVENT_TIMERDATA,
                              p_ass->transmit.remote[addr_index].rto,
                              addr_index,
                              & p_ass->transmit.remote[addr_index].
                              dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL );

            p_ass->transmit.remote[addr_index].lowest_tsn = p_buf->tsn;
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATABYTES_SENT, p_buf->size );

        if( ( p_buf->begin == 0 ) && ( p_buf->end == 1 ) && ( new_chunk == 1 ) )
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_FRAG_USR_MSG, 1 );
        }

        if( p_buf->unordered == 1 )
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNORD_DATACHUNKS_SENT, 1 );
        }

        else
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ORD_DATACHUNKS_SENT, 1 );
        }

        SCTP_INCR_ASSOCSTAT( p_ass, addr_index,
                             SCTPSTATS_ASSOC_DATABYTES_SENT, p_buf->size );
        /* Rel 6.2.0 change start */
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATABYTES_SENT, p_buf->size );
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATA_SENT, 1 );
        /* Rel 6.2.0 change start */

        /* Rel 6.2.0 change end */
        numchunks++;

        /* Update retrans flag before sending datagram */
        if( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )
        {
            p_buf->retrans_flag = SCTP_RETRANSFLAG_RETRANSMITTED;

            SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_CHUNKS_RETRANS, 1 );
            /* Rel 6.2.0 change start */
            SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                         SCTPSTATS_ASSOC_STREAM_DATA_RESENT, 1 );
            /* Rel 6.2.0 change end */
        }

        p_buf->missed = 0;
    }

    /* Check for the deadlock condition of OUTSTANDING=0  and no new data */
    p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

    if( ( p_buf != NULL )
            && ( numchunks == 0 )
            /* Changes For SS7P CSR#1-6073229 */
            /* We should allow 0-window probing data to be bundled with SACK chuck also. */
            /*        && (sack_included == 0) */
            /* Changes For SS7P CSR#1-6073229 Ends */
            && ( p_ass->transmit.total_outstanding == 0 ) )
    {
        /*
         * We will send just the topmost chunk, to probe the RWND.
         * We DO NOT update the RWND, as it is already 0
         */

        /* this section still needs some closer look */
        addr_index = p_buf->dest_addr_index;

        /* update outstanding values, when we have never sent them before */
        if( p_buf->sent == 0 )
        {
            /* Update tsn and SSN */
            sctp_update_tsn_and_ssn( p_ass, p_buf );
            p_ass->transmit.remote[addr_index].outstanding += p_buf->size;
            p_ass->transmit.total_outstanding += p_buf->size;

            p_buf->sent = 1;
            p_ass->total_sent++;
            p_ass->transmit.remote[addr_index].num_chunks_await_ack++;
            p_ass->transmit.remote[addr_index].unsent_packet_count--;
            p_ass->transmit.highest_outstanding = p_buf->tsn;
            p_ass->transmit.highest_tsn_sent    = p_buf->tsn;
            sentdatachunks++;
        }

        else
        {
            resentdatachunks++;
        }

        p_iovec[2].iov_base =
            ( char * )( p_buf->buffer - SCTPCHUNKOFFSET_PAYLOAD );

        p_iovec[2].iov_len = ROUND( p_buf->size ) + SCTPCHUNKOFFSET_PAYLOAD;

        if( SCTP_IS_STACK_UDP )
        {
            packet_size = p_iovec[1].iov_len + SCTP_MAX_IP_HEADER_SIZE +
                          SCTP_UDP_HDR_SIZE;
        }

        else
        {
            packet_size = p_iovec[1].iov_len + SCTP_MAX_IP_HEADER_SIZE;
        }

        new_packet_size =
            packet_size + SCTPCHUNKOFFSET_PAYLOAD + ROUND( p_buf->size );

        packet_size = new_packet_size;

        /* Start Timer if not already running */
        if( ( p_ass->transmit.remote[addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX] == SCTP_NULL_TIMER_ID ) ||
                ( sctp_compare_tsn_for_smaller( p_buf->tsn,
                                                p_ass->transmit.remote[addr_index].lowest_tsn ) == SCTP_SUCCESS ) )
        {
            sctp_stop_timer(
                & p_ass->transmit.remote[addr_index].
                dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL, SCTP_NULL );

            sctp_start_timer( p_ass, SCTPEVENT_TIMERDATA,
                              p_ass->transmit.remote[addr_index].rto,
                              addr_index,
                              & p_ass->transmit.remote[addr_index].
                              dest_timer_id[SCTP_DESTTIMER_RTX], SCTP_NULL );

            p_ass->transmit.remote[addr_index].lowest_tsn = p_buf->tsn;
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATABYTES_SENT, p_buf->size );

        SCTP_INCR_ASSOCSTAT( p_ass, addr_index,
                             SCTPSTATS_ASSOC_DATABYTES_SENT, p_buf->size );
        /* SCTP REl 6.2.0 changes start */
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATABYTES_SENT, p_buf->size );
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATA_SENT, 1 );
        /* SCTP REl 6.2.0 changes end */
        numchunks++;

        /* Update retrans flag before sending datagram */
        if( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )
        {
            p_buf->retrans_flag = SCTP_RETRANSFLAG_RETRANSMITTED;
            p_ass->transmit.remote[addr_index].retrans_packet_count--;
            SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_CHUNKS_RETRANS, 1 );

            SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream, SCTPSTATS_ASSOC_STREAM_DATA_RESENT, 1 );
        }

        p_buf->missed = 0;
        ret = 2;
        p_ass->zero_win_prob_on = SCTP_TRUE;

        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                    ( "Enabled zero window probe!! Chunk DATA Sent,Src Port=%d,Dest Port=%d, "
                      "Ver Tag=%u,To Addr: ", p_ass->p_ep->this_port,
                      p_ass->transmit.that_port, p_ass->remote_tag ) );
    }

    if( !numchunks && !sack_included )
    {
        return ( SCTP_FAILURE );
    }


    sctp_buffer_set_UlongAtOffset( sctp_offset, SCTP_OFFSET_CKSUM, 0 );

    sctp_buffer_set_UlongAtOffset( sctp_offset, SCTP_OFFSET_CKSUM,
                                   sctp_compute_cksm_iovec[p_ass->config.checksum_func]
                                   ( p_iovec + 1, numchunks + 1 ) );

    SCTP_INCR_ASSOCSTAT( p_ass, addr_index, SCTPSTATS_ASSOC_DATA_SENT,
                         numchunks );



    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATACHUNKS_SENT, numchunks );

    SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC,
                ( "Chunk DATA Sent,Src Port=%d,Dest Port=%d, "
                  "Ver Tag=%u,To Addr: ", p_ass->p_ep->this_port,
                  p_ass->transmit.that_port, p_ass->remote_tag ) );

    SCTP_TRACE_ADDR( SCTP_NULL, SCTP_DBG_TRC,
                     ( &( p_ass->transmit.remote[addr_index].that_address ) ) );

    /* Sends the message to the appropriate node */
    p_to = &( p_ass->transmit.remote[addr_index].that_address );
    saddr_index = p_ass->transmit.remote[addr_index].src_addr_index;

    p_iovec[0].iov_base = ( char * )buffer;
    p_iovec[0].iov_len  = packet_size;  /*this required in IP header*/

    /* Callback for Sending Func SPR 20859 Starts */
    if( p_ass != SCTP_NULL && p_ass->p_ep->send_func != SCTP_NULL )
    {
        p_ass->p_ep->send_func( ( sctp_U8bit * )p_iovec, numchunks + 2, SCTP_TRUE, p_to,
                                &( p_ass->p_ep->this_address[saddr_index] ), SCTP_TRUE );

    }

    else
    {

        /* df_flag is set to SCTP_TRUE as this data is transmitted for 1st time*/
        sctp_send_message[( p_to->family == SCTP_AF_INET ) ? SCTP_V4_IOV : SCTP_V6_IOV](
            ( sctp_U8bit * )p_iovec, numchunks + 2, SCTP_TRUE, p_to,
            &( p_ass->p_ep->this_address[saddr_index] ) );
    }

    /* Callback for Sending Func SPR 20859 Stops*/

    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_PACKETS_SENT, 1 );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_TRANS_DATACHUNKS, sentdatachunks );
    SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_RETRANS_DATACHUNKS, resentdatachunks );

    if( p_ass->zero_win_prob_on != SCTP_TRUE )
    {
        /* Update the last used time parameter if heartbeat enabled */
        if( p_ass->transmit.remote[addr_index].allow_heartbeat == 1 )
        {
            __GETSYSTIME( &p_ass->transmit.remote[addr_index].last_used_time );
        }

    }

    if( !numchunks )
    {
        return ( SCTP_FAILURE );
    }

    else
    {
        return ( ret );
    }

}

/***************************************************************************
 *    FUNCTION :
 *        sctp_senddata_on_addr_if_possible
 *
 *    DESCRIPTION: This function checks and sends the data on same address
 *    till a failure is returned.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_senddata_on_addr_if_possible(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_return_t ret = SCTP_FAILURE;
    sctp_U32bit   send_status;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_senddata_on_addr_if_possible,addr_index=%u", addr_index ) );

    /* Stop bundling timer if already runing */
    sctp_stop_timer( &p_ass->transmit.remote[addr_index].
                     dest_timer_id[SCTP_DESTTIMER_BUNDLING], SCTP_NULL, SCTP_NULL );

    for( ;; )
    {
        /* Call function to send one mtu till a failure is returned */
        send_status = sctp_send_one_mtu( p_ass, addr_index );

        if( send_status != SCTP_SUCCESS )
        {
            if( ( p_ass->transmit.remote[addr_index].outstanding == 0 )
                    && ( p_ass->transmit.remote[addr_index].unsent_packet_count > 0 ) )
            {
                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERBUNDLE,
                    p_ass->config.bundling_time,
                    addr_index,
                    &p_ass->transmit.remote[addr_index].
                    dest_timer_id[SCTP_DESTTIMER_BUNDLING],
                    SCTP_NULL );
            }

            if( 2 == send_status )
            {
                sctpbuffer_st *p_buf = SCTP_NULL;
                p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

                if( p_buf )
                {
                    p_ass->transmit.remote[p_buf->dest_addr_index].state = SCTP_TX_STATE_AWTACK;
                }

            }

            return ret;
        }

        ret = SCTP_SUCCESS;
    }
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_senddata_on_addr
 *
 *    DESCRIPTION: This function sends data on possible active address index
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS/SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_senddata_on_addr(
    sctp_tcb_st    *p_ass,
    sctp_U32bit    addr_index )
{
    sctp_return_t ret = SCTP_FAILURE;
    sctp_U32bit   send_status;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_senddata_on_addr,addr_index=%u", addr_index ) );

    if( p_ass->transmit.remote[addr_index].retrans_packet_count > 0 )
    {
        sctp_U32bit new_addr_index;
        new_addr_index = sctp_get_possible_next_active_addr_index( p_ass,
                                                                   addr_index );

        for( ; ; )
        {
            send_status = sctp_send_one_mtu_on_alternate_addr( p_ass, addr_index
                                                               , new_addr_index );

            if( send_status == SCTP_FAILURE )
            {
                break;
            }
        }
    }

    /* Stop bundling timer if already runing */
    sctp_stop_timer( &p_ass->transmit.remote[addr_index].
                     dest_timer_id[SCTP_DESTTIMER_BUNDLING], SCTP_NULL, SCTP_NULL );

    for( ;; )
    {
        /* Call function to send one mtu till a failure is returned */
        send_status = sctp_send_one_mtu( p_ass, addr_index );

        if( send_status != SCTP_SUCCESS )
        {
            if( p_ass->transmit.remote[addr_index].outstanding > 0 )
            {
                p_ass->transmit.remote[addr_index].state = SCTP_TX_STATE_AWTACK;
            }

            else if( p_ass->transmit.remote[addr_index].unsent_packet_count > 0 )
            {
                sctp_start_timer(
                    p_ass,
                    SCTPEVENT_TIMERBUNDLE,
                    p_ass->config.bundling_time,
                    addr_index,
                    &p_ass->transmit.remote[addr_index].
                    dest_timer_id[SCTP_DESTTIMER_BUNDLING],
                    SCTP_NULL );
            }

            if( 2 == send_status )
            {
                sctpbuffer_st *p_buf = SCTP_NULL;
                p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

                if( p_buf )
                {
                    p_ass->transmit.remote[p_buf->dest_addr_index].state = SCTP_TX_STATE_AWTACK;
                }

            }

            return ret;
        }

        ret = SCTP_SUCCESS;
    }
}




/***************************************************************************
 *    FUNCTION :
 *        sctp_add_data_chunks
 *
 *    DESCRIPTION: This function is to add data chunks to be bundled
 *
 *    RETURN VALUE:
 *        pointer to the buffer
 **************************************************************************/
sctp_U8bit *
sctp_add_data_chunks(
    sctp_tcb_st    *p_ass,
    sctp_U8bit     *p_buffer,
    sctp_U32bit    remain_len,
    sctp_U16bit    *df_bit )
{
    sctp_U32bit   addr_index, saddr_index;
    sctp_U32bit   new_packet_size, packet_size = 0;
    sctp_error_t  s_err;
    sctpbuffer_st *p_buf;
    sctp_Boolean_t new_chunk;

    SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                ( "sctp_add_data_chunks" ) );

    if( SCTP_FAILURE == sctp_db_get_primary_addr_index( p_ass, &saddr_index,
                                                        &addr_index, &s_err ) )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_add_data_chunks:could not get primary address" ) );
        return p_buffer;
    }

    p_buf = ( sctpbuffer_st * ) lstFirst( &p_ass->transmit.transmit_queue );

    if( p_buf == SCTP_NULL )
    {
        SCTP_TRACE( p_ass->p_ep, SCTP_DBG_TRC,
                    ( "sctp_add_data_chunks:No data to be bundled " ) );

        return p_buffer;
    }

    p_ass->transmit.remote[addr_index].lowest_tsn = p_buf->tsn;

    for( ;; p_buf = ( sctpbuffer_st * ) lstNext( ( NODE * ) p_buf ) )
    {
        if( p_buf == SCTP_NULL )
        {
            return p_buffer;
        }

        /*Bug Id 22 fix starts*/
        /*new_chunk = 0;*/
        new_chunk = SCTP_FALSE;
        /*Bug Id 22 fix ends*/

        if( p_buf->dest_addr_index != addr_index )
        {
            continue;
        }

        new_packet_size =
            packet_size + SCTPCHUNKOFFSET_PAYLOAD + ROUND( p_buf->size );


        if( new_packet_size > remain_len )
        {
            /* Would cross MTU ..., */
            break;
        }

        /* update outstanding values, when we have never sent them before */
        /*
         * Update TSN and SSN fields,
         * This function also build the chunk in case it is to be sent
         * for first time
         */
        if( p_buf->sent == 0 )
        {
            sctp_update_tsn_and_ssn( p_ass, p_buf );
            p_ass->transmit.remote[addr_index].outstanding += p_buf->size;
            p_ass->transmit.total_outstanding += p_buf->size;
            p_buf->sent = 1;
            p_ass->total_sent++;
            /* fix added */
            /*Bug Id 22 fix starts*/
            /*new_chunk = 1;*/
            new_chunk = SCTP_TRUE;
            /*Bug Id 22 fix ends*/
            /* fix ended */
            p_ass->transmit.remote[addr_index].num_chunks_await_ack++;
            p_ass->transmit.highest_outstanding = p_buf->tsn;
            p_ass->transmit.highest_tsn_sent    = p_buf->tsn;
            p_ass->transmit.rwndthere -= p_buf->size;
            /* As data is this sent for first time so set df_bit */
            *df_bit = 1;
            p_ass->transmit.remote[addr_index].unsent_packet_count--;

        }

        /*copy the data chunk*/
        p_buffer = sctp_buffer_set_Buffer( p_buffer,
                                           ROUND( p_buf->size ) + SCTPCHUNKOFFSET_PAYLOAD,
                                           ( sctp_U8bit * )( p_buf->buffer - SCTPCHUNKOFFSET_PAYLOAD ) );

        packet_size = new_packet_size;

        /* Start Timer if not already running */
        if( sctp_compare_tsn_for_smaller( p_buf->tsn,
                                          p_ass->transmit.remote[addr_index].lowest_tsn ) == SCTP_SUCCESS )
        {
            p_ass->transmit.remote[addr_index].lowest_tsn = p_buf->tsn;
        }

        SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_DATABYTES_SENT, p_buf->size );

        if( ( p_buf->begin == 0 ) && ( p_buf->end == 1 ) && ( new_chunk == 1 ) )
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_FRAG_USR_MSG, 1 );
        }

        if( p_buf->unordered == 1 )
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_UNORD_DATACHUNKS_SENT, 1 );
        }

        else
        {
            SCTP_INCR_GLOBAL_TRAFFICSTAT( SCTPSTATS_NO_ORD_DATACHUNKS_SENT, 1 );
        }

        SCTP_INCR_ASSOCSTAT( p_ass, addr_index,
                             SCTPSTATS_ASSOC_DATABYTES_SENT, p_buf->size );
        /* fix added */
        SCTP_INCR_ASSOCSTAT( p_ass, addr_index,
                             SCTPSTATS_ASSOC_DATA_SENT, 1 );
        /* fix ended */
        /* SCTP REl 6.2.0 changes start */
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATABYTES_SENT, p_buf->size );
        SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream,
                                     SCTPSTATS_ASSOC_STREAM_DATA_SENT, 1 );

        /* SCTP REl 6.2.0 changes end */
        /* Update retrans flag before sending datagram */
        if( p_buf->retrans_flag == SCTP_RETRANSFLAG_MARK )
        {
            p_buf->retrans_flag = SCTP_RETRANSFLAG_RETRANSMITTED;
            p_ass->transmit.remote[addr_index].retrans_packet_count--;

            SCTP_INCR_ASSOCSTAT( p_ass, 0, SCTPSTATS_ASSOC_NO_CHUNKS_RETRANS, 1 );

            SCTP_INCR_ASSOC_STREAM_STAT( p_ass, p_buf->stream, SCTPSTATS_ASSOC_STREAM_DATA_RESENT, 1 );

        }

        p_buf->missed = 0;
    }

    return p_buffer;
}
