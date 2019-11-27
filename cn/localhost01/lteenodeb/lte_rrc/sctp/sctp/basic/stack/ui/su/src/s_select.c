/****************************************************************************
 *    FILE NAME  : s_select.c
 *
 *    DESCRIPTION: This file contains the select call functionality.
 *
 *    DATE     NAME         REFERENCE              REASON
 *    ----     ----         ---------              ------
 *    09Spe01  gsheoran         -                  Create Original
 *    19Nov-1  mrajpal       SPR 4930              invalid socket error
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#define SCTP_MODULE "UI-SK"

#include <s_select.h>
#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_dbg.h>
#include <s_db.h>
#include <s_dbtcb.h>
#include <s_uisusk.h>
#include <s_os.h>
#include <s_ostmr.h>
#include <s_osmutx.h>

#ifdef SCTP_UI_SOCK_API

/* The Global list of selects invoked in the stack. */
sctp_select_db_st  select_db;

/*****************************************************************************
 *    FUNCTION    : sctp_do_select
 *
 *    DESCRIPTION : It is used to do a select on fd sets.
 *    When the Timer module invoked this then p_timeout is null and we don't
 *    start the timer.
 *
 *    RETURN VALUE: On success returns the number of fd's set and -1 on error.
 ****************************************************************************/
int
sctp_do_select(
    sctp_select_st      *p_select )
{
    int             retval      = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_do_select" ) );

    retval = sctp_select_get_no_fds_ready( p_select, SCTP_TRUE );

    if( retval )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "do_select: Found some waiting sockets" ) );

        return ( retval );
    }

    /*
     * If the timeval was all 0 then we return immediately.
     */
    if( !( p_select->p_timeout ?
            ( p_select->p_timeout->tv_sec || p_select->p_timeout->tv_usec ) :
            SCTP_TRUE ) )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "do_select: exiting :retval = %d", retval ) );

        /* Changes for CSR#1-6284004 */
        /* We need to return-back only those FDs in FD-Sets over which events
         * have occured. If retval = 0 every FD_set should in RESET mode */
        if( p_select->p_rfds )
        {
            *p_select->p_rfds = p_select->n_rfds;
        }

        if( p_select->p_wfds )
        {
            *p_select->p_wfds = p_select->n_wfds;
        }

        if( p_select->p_efds )
        {
            *p_select->p_efds = p_select->n_efds;
        }

        /* Changes for CSR#1-6284004 Ends */
        return ( retval );
    }

    /* Put the select node on the list of in-use select nodes */
    SCTP_SEL_ATTACH_NODE( p_select );

    /* else timer is NULL/valid start the timer. */
    retval = sctp_select_sleep_timeout( p_select );
    /* Remove the select node form the used list */
    SCTP_SEL_DETACH_NODE( p_select );

    return ( retval );
}

/* Changes for SPR#15820 */
/*****************************************************************************
 *    FUNCTION    : sctp_lock_cleanup_func
 *
 *    DESCRIPTION : It is used by OS to cleanup all the acquired locks by a
 *    sctp_select API in a thread before killing that thread.
 *
 *    RETURN VALUE: None.
 ****************************************************************************/
sctp_void_t
sctp_lock_cleanup_func(
    sctp_void_t               *p_arg )
{
    sctp_select_st           *p_select;
    p_select = ( sctp_select_st * )p_arg;

    SCTP_UNLOCK( p_select->lock );
    sctp_stop_timer( &( p_select->timer_id ),
                     SCTP_NULL, SCTP_NULL );
    SCTP_SEL_DETACH_NODE( p_select );
    SCTP_SEL_INIT_NODE( p_select );
    SCTP_SEL_FREE_NODE( p_select );

}
/* Changes for SPR#15820 Ends */



/*****************************************************************************
 *    FUNCTION    : sctp_select_sleep_timeout
 *
 *    DESCRIPTION : It is used to start a timer select and sleep if required.
 *
 *    RETURN VALUE: On success returns the number of fd's set and -1 on error.
 ****************************************************************************/
int
sctp_select_sleep_timeout(
    sctp_select_st *p_select )
{
    sctp_U32bit     timeintrvl  = 0;
    sctp_S32bit     ret = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_select_sleep_timeout" ) );

    /* Check if timeout parameter is NULL */
    if( !p_select->p_timeout )
    {
        /* Set the Maximum time interval */
        timeintrvl = SCTP_MAX_SELECT_TIMEOUT;
    }

    else
    {
        /* Calculate the time interval */
        timeintrvl = p_select->p_timeout->tv_sec * 1000 +
                     p_select->p_timeout->tv_usec / 1000;

        /* Less than the least granuality */
        if( timeintrvl < 10 )
        {
            sk_errno = SK_EINVAL;
            return ( SK_FAILURE );
        }
    }

    /*
     * Before the timer is started we want to mark the ep's as selected.
     * This is because when an event on an assoc happens then we check
     * if it was ever "selected" if it was then we have to call this
     * function do_select otherwise we don't bother.
     */
    sctp_mark_ep_selected( p_select, SCTP_TRUE );

    /* Start a timer for the maximum time available */
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "sctp_select_sleep_timeout: Starting timer for %lu", timeintrvl ) );

    sctp_start_timer( SCTP_NULL,   /* Association id */
                      SCTPEVENT_TIMER_SELECT,    /* Event */
                      timeintrvl,                /* Interval */
                      0,                         /* addr_index */
                      & ( p_select->timer_id ), /* Timer id */
                      SCTP_NULL );               /* Error code */

    SCTP_LOCK( p_select->lock );

    /* Changes for SPR#15820 */
    SCTP_PTHREAD_CLEANUP_PUSH( sctp_lock_cleanup_func, p_select );
    /* Changes for SPR#15820 Ends */

    SCTP_SLEEP( p_select->cond, p_select->lock );

    /* Changes for SPR#15820 */
    SCTP_PTHREAD_CLEANUP_POP( 0 );
    /* Changes for SPR#15820 Ends */

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "sctp_select_sleep_timeout: Woken up after select timeout\n" ) );


    ret = p_select->retval;
    SCTP_UNLOCK( p_select->lock );

    /* Changes for CSR#1-4665106 */
    sctp_stop_timer( &( p_select->timer_id ),
                     SCTP_NULL, SCTP_NULL );
    /* Changes for CSR#1-4665106 Ends */

    if( 0 > ret )
    {
        /*do nothing and return error*/
        return SK_FAILURE;
    }

    /* Unmark the endpoints as not selected */
    sctp_mark_ep_selected( p_select, SCTP_FALSE );

    /*
     * Incase we had more than 1 fd that's ready then copy the fd lists from
     * the temporary ones to the actual ones. Even if we have 0 fds ready we
     * copy the list coz we need to reset the fd sets.
     */
    if( p_select->p_rfds )
    {
        *p_select->p_rfds = p_select->n_rfds;
    }

    if( p_select->p_wfds )
    {
        *p_select->p_wfds = p_select->n_wfds;
    }

    if( p_select->p_efds )
    {
        *p_select->p_efds = p_select->n_efds;
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "sctp_select_sleep_timeout: returning from select" ) );

    /*
     * We were sleeping and were woken up. either by the timer or by RWE event
     */
    return ( ret );
}


/*****************************************************************************
 *    FUNCTION    : sctp_mark_ep_selected
 *
 *    DESCRIPTION : It is used to mark a socket as selected.
 *
 *    RETURN VALUE: void
 ****************************************************************************/
void
sctp_mark_ep_selected(
    sctp_select_st      *p_select,
    sctp_Boolean_t      onoff )
{
    sctp_U32bit     count       = 0;
    sctp_U32bit     offset      = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_mark_ep_selected" ) );

    /*
     * Loop till the max fd-1. n_fds = max_fd -1;
     */
    for( count = 0 ; count < p_select->nfds; count++ )
    {
        sctp_U32bit    bit   = __S_BIT( count );
        sctp_ep_st    *p_ep = SCTP_NULL;

        offset = count / SCTP_NFDBITS;

        /* Check if any of RWE is set */
        if( !( ( p_select->p_rfds ?
                 ( bit & ( p_select->p_rfds )->fds_bits[offset] ) : 0 )
                || ( p_select->p_wfds ?
                     ( bit & ( p_select->p_wfds )->fds_bits[offset] ) : 0 )
                || ( p_select->p_efds ?
                     ( bit & ( p_select->p_efds )->fds_bits[offset] ) : 0 ) ) )
        {
            /* If not then chuck this bit and continue. */
            continue;
        }

        /* Atleast one bit is set. */
        /* If set then get the corresponding socket/Endpoint */
        p_ep = sctp_db_get_ep_ptr( count );

        /*
         * If the Endpoint does not exist then we skip it and reset flag.
         * We do not check for !p_ep->n_assoc because for server sockets there
         * maynot be a association.
         */
        if( !p_ep )
        {
            continue;
        }

        /* Lock the EP */
        SCTP_LOCK_EP( p_ep->ep_id );

        /* Mark the Endpoint as selected/deselected. */
        SCTP_SOCK_SET_SELECTED( p_ep, onoff );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                    ( "sctp_mark_ep_selected: Ep marked as selected\n" ) );

        /* Unlock the EP */
        SCTP_UNLOCK_EP( p_ep->ep_id );
    }
}


/*****************************************************************************
 *    FUNCTION    : sctp_wakeup_selected
 *
 *    DESCRIPTION : It is used to check if a sock is selected in the select
 *    list of the stack. If the socket is selected it releases any locks on the
 *    socket.
 *
 *    RETURN VALUE: void
 ****************************************************************************/
void
sctp_wakeup_selected(
    sctp_U32bit        ep_id )
{
    sctp_select_st *p_select = SCTP_NULL;
    int            retval = 0;


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ep_id );
    /*to avoid the warning*/


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_wakeup_selected" ) );

    /*
     * Invoke do_select for each of the select blocks in the list and release
     * the locks.
     * Note: We could do this search only for the endpoint that is passed.
     * (Maybe)
     */

    /* Lock the select list */
    SCTP_LOCK( select_db.Select_list_lock );

    for( p_select = ( sctp_select_st * ) lstFirst( &( select_db.Select_list ) );
            p_select; p_select = ( sctp_select_st * ) lstNext( ( NODE * ) p_select ) )
    {
        /* do_select on p_select */
        SCTP_LOCK( p_select->lock );
        retval = sctp_select_get_no_fds_ready( p_select, SCTP_FALSE );

        if( retval != 0 )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_wakeup_selected: Waking up" ) );

            /* Set the number of fds selected */
            p_select->retval = retval;

            /* Wakeup the select node */
            SCTP_WAKEUP( p_select->cond );
        }

        SCTP_UNLOCK( p_select->lock );
    }

    /* Unlock the select list. */
    SCTP_UNLOCK( select_db.Select_list_lock );
}


/*****************************************************************************
 *    FUNCTION    : sctp_select_get_no_fds_ready
 *
 *    DESCRIPTION : It is used to get the number of FDs ready in the select
 *    block.
 *
 *    RETURN VALUE: int 0 == no fds waiting, otherwise number of fds waiting.
 ****************************************************************************/
int
sctp_select_get_no_fds_ready(
    sctp_select_st    *p_select,
    sctp_Boolean_t    lock )
{
    sctp_U32bit     count       = 0;
    sctp_U32bit     offset      = 0;
    int             retval      = 0;
    sctp_ep_st      *p_ep       = SCTP_NULL;
    unsigned long   bit         = __S_BIT( count );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_select_get_no_fds_ready" ) );

    /*
    * Loop till the max fd-1. n_fds = max_fd -1;
    */
    for( count = 0 ; count < p_select->nfds; count++ )
    {
        bit   = __S_BIT( count );
        p_ep  = SCTP_NULL;

        offset = count / SCTP_NFDBITS;

        /* Check if any of RWE is set */
        if( !( ( p_select->p_rfds ?
                 ( bit & ( p_select->p_rfds )->fds_bits[offset] ) : 0 )
                || ( p_select->p_wfds ?
                     ( bit & ( p_select->p_wfds )->fds_bits[offset] ) : 0 )
                || ( p_select->p_efds ?
                     ( bit & ( p_select->p_efds )->fds_bits[offset] ) : 0 ) ) )
        {
            /* If not then chuck this bit and continue. */
            continue;
        }

        /* Atleast one bit is set. */

        /* If set then get the corresponding socket/Endpoint */
        p_ep = sctp_db_get_ep_ptr( count );

        /*
        * If the Endpoint does not exist then we skip it and reset flag.
        * We do not check for !p_ep->n_assoc because for server sockets there
        * maynot be a association.
        */
        if( !p_ep )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "sctp_select_get_no_fds_ready:INVALID endpoint" ) );
            /*SPR #4930*/
            sk_errno = SK_EBADF;

            return SK_FAILURE;
            /*SPR 4930*/
        }

        if( lock )
        {
            /*
             * Try to lock the endpoint: If we're unable to lock then hardluck
             * But we may still live at the end of it (98%).
             * Probably change this later...not doing this caused a problem
             * when we got a data from the dest (i.e. notify) and also the
             * select timer expired simultaneously.
             */
            if( SCTP_TRYLOCK_EP( p_ep->ep_id )  != 0 )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                            ( "sctp_select_get_no_fds_ready: Failed to lock the endpoint"
                              "risking it going ahead without a lock." ) );
                lock = SCTP_FALSE;
            }

            p_ep = sctp_db_get_ep_ptr( count );

            if( SCTP_NULL == p_ep )
            {
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                            ( "sctp_select_get_no_fds_ready:INVALID endpoint" ) );

                return SK_FAILURE;
            }
        }

        /* Check if the socket is set for read */
        if( p_select->p_rfds )
        {
            if( ( SCTP_SOCK_IS_READABLE( p_ep->flags ) )
                    && ( bit & ( p_select->p_rfds )->fds_bits[offset] ) )
            {
                /* Set the Socket as readable. */
                SCTP_FD_SET( p_ep->ep_id, &p_select->n_rfds );

                retval++;
            }
        }

        /* Check if the socket is writeable */
        if( p_select->p_wfds )
        {
            if( ( SCTP_SOCK_IS_WRITEABLE( p_ep->flags ) )
                    && ( bit & ( p_select->p_wfds )->fds_bits[offset] ) )
            {
                /* Set the socket as writeable */
                SCTP_FD_SET( p_ep->ep_id, &p_select->n_wfds );

                retval++;
            }
        }

        if( p_select->p_efds )
        {
            if( ( SCTP_SOCK_IS_SK_ERROR( p_ep->flags ) )
                    && ( bit & ( p_select->p_efds )->fds_bits[offset] ) )
            {
                SCTP_FD_SET( p_ep->ep_id, &p_select->n_efds );

                retval++;
            }
        }

        if( lock )
        {
            /* Lock the endpoint */
            SCTP_UNLOCK_EP( p_ep->ep_id );
        }
    }

    if( retval != 0 )
    {
        /* if retval is nonzero then write the new values in fd sets */
        if( p_select->p_rfds )
        {
            *p_select->p_rfds = p_select->n_rfds;
        }

        if( p_select->p_wfds )
        {
            *p_select->p_wfds = p_select->n_wfds;
        }

        if( p_select->p_efds )
        {
            *p_select->p_efds = p_select->n_efds;
        }
    }

    return ( retval );
}


/*****************************************************************************
 *    FUNCTION    : sctp_handle_select_timeout
 *
 *    DESCRIPTION : It is used to handle a select timeout.
 *
 *    RETURN VALUE: void
 ****************************************************************************/
void
sctp_handle_select_timeout(
    sctp_timer_t     timer_id )
{
    sctp_select_st *p_select = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_handle_select_timeout" ) );

    /*
     * Search for the select node with the matching timer id.
     * Then wake up the select node.
     */

    /* Lock the select list */
    SCTP_LOCK( select_db.Select_list_lock );

    for( p_select = ( sctp_select_st * ) lstFirst( &( select_db.Select_list ) );
            p_select; p_select = ( sctp_select_st * ) lstNext( ( NODE * ) p_select ) )
    {
        SCTP_LOCK( p_select->lock );

        if( timer_id == p_select->timer_id )
        {
            /* do_select on p_select */
            p_select->retval = sctp_select_get_no_fds_ready( p_select, SCTP_TRUE );

            /*
             * if we have more than one socket waiting on it then we
             * wakeup the node.
             */
            SCTP_WAKEUP( p_select->cond );
            SCTP_UNLOCK( p_select->lock );
            break;
        }

        SCTP_UNLOCK( p_select->lock );
    }

    /* Unlock the select list. */
    SCTP_UNLOCK( select_db.Select_list_lock );

}


#endif /* SCTP_UI_SOCK_API */


