/****************************************************************************
 *    FILE NAME:
 *        s_uisuif.c
 *
 *    DESCRIPTION:
 *        This file is part of Service User Interface module. It includes
 *    functions that are called by Service user to use services of
 *    SCTP stack.
 *
 *    DATE          NAME             REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000   Sandeep Mahajan  -               .Original Creation
 *    15Oct  2000   Harsh Bhondwe    VXWORKS         porting
 *    20Nov  2000   Harsh Bhondwe    WIN2000         porting
 *    30Aug  2001   Yogesh Gahlaut   -               Rel 3.0 Changes
 *    27Aug02       mrajpal          SPR 4798
 *    10Jan03       mrajpal          SPR 5216        Fix
 *    02Jun03       mrajpal          CCR 1-628867    error code in accpet
 *    16Jul04       Nitin Khanna     SPR-6959        Added api sctp_api_connectx_ext
 *    02Nov04       Svakumar         REL 6.0         SCTP/FUNC-CONGCON-5
 *    09May08       Raza             REL 6.2         SPR 18527
 *    02Aug08       Shikha Kathpal   REL6.2          RFC 4960 UPGRADE
 *    25May10       Akanksha         SPR 20413       CSR Fixing
 *    04May10       Shivani Mehrotra SPR 20396       To achieve Complaince with Section 3.1
 *                                                   of RFC, i.e. PORT 0 must not be used.
 *    25Mar11       Akanksha         SPR 20859       Enhancement for Callback
  * 07Jul2011   Vaibhav Banga               changes for klocwork fix for IBM delivery
 *    18Apr13       Pranav           SPR 21127       CSR 40731-warning removal
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#define SCTP_MODULE "SM"

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_comac.h>
#include <s_cobuf.h>
#include <s_err.h>
#include <s_db.h>
#include <s_dbtcb.h>
#include <s_dbbuf.h>
#include <s_dbutil.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_osmutx.h>
#include <s_clpapi.h>
#include <s_fsmass.h>
#include <s_fsmdat.h>
#include <s_tlif.h>
#include <s_in.h>
#include <s_mhbld.h>
#include <s_dbg.h>
#include <s_stats.h>
#include <s_uisuap.h>
#include <s_uisuif.h>
#include <s_uisusk.h>
#include <s_uiutil.h>
#include <s_fsmmtu.h>
#ifdef SCTP_DEQUEUE_IND
#include <s_es.h>
#endif

#ifndef SCTP_UI_SOCK_API

/***************************************************************************
 *  FUNCTION :    sctp_api_register_su
 *
 *  DESCRIPTION:  Register sctp user
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_register_su(
    sctp_U16bit           *p_application_id,
    sctp_Pvoid_t          callback_func,
    sctp_Pvoid_t          p_appl_data,
    sctp_error_t          *p_ecode )
{
    sctp_return_t ret;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_register_su" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API    ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return( SCTP_FAILURE );
    }

    ret = sctp_db_register_new_appl( p_application_id,
                                     callback_func, p_appl_data, p_ecode );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_deregister_su
 *
 *  DESCRIPTION: Derigister SCTP user
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_deregister_su(
    sctp_U16bit     application_id,
    sctp_error_t    *p_ecode )
{
    sctp_return_t ret;

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_deregister_su" ) );

    ret = sctp_db_deregister_appl( application_id );

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_APPID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *p_ecode = EINV_APPID;
    }

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_socket
 *
 *  DESCRIPTION:
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_socket(
    sctp_U16bit      application_id,
    sctp_U32bit      addr_family,
    sctp_assoc_t     *p_assoc_id,
    sctp_error_t     *p_ecode )
{
    sctp_ep_st     *p_ep = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_socket" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( !p_assoc_id )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( ( addr_family != SCTP_AF_INET ) && ( addr_family != SCTP_AF_INET6 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_APPID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "InValid Address Family " ) );

        *p_ecode = E_UNSUPP_ADDR_TYPE;

        return ( SCTP_FAILURE );

    }

    if( SCTP_FAILURE == sctp_db_validate_appl( application_id ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_APPID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Application Validation Failed, Application Id = %d",
                      application_id ) );

        *p_ecode = EINV_APPID;

        return ( SCTP_FAILURE );
    }

    /* Get a free association */
    p_ep = sctp_db_get_free_ep( p_ecode );

    if( SCTP_NULL == p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_NORESOURCES, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL , SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Get Free Association Failed,No Resources" ) );

        *p_ecode = E_NORESOURCES;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( p_ep->ep_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::SU Module:New Socket Created, Association Id = %u",
                  p_ep->ep_id ) );

    /* Reset the flag CLOSE_CALLED */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_FALSE );

    p_ep->appl_id = application_id;

    p_ep->family = addr_family;
    p_ep->sk_type = SCTP_SOCK_STREAM; /* Only TCP Style socket supported */
    *p_assoc_id = p_ep->ep_id;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( p_ep->ep_id );

    return ( SCTP_SUCCESS );
}
/* Callback for Sending Function SPR 20859 starts*/
#if !defined (SCTP_STRUCT_BASED) && !defined (SCTP_UI_SU_MESSAGEBASED) && !defined (SCTP_UI_SOCK_API)
/***************************************************************************
 *  FUNCTION :    sctp_api_socket_ext
 *
 *  DESCRIPTION:
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_socket_ext(
    sctp_U16bit      application_id,
    sctp_U32bit      addr_family,
    sctp_assoc_t     *p_assoc_id,
    sctp_Pvoid_t     callback_send_func,
    sctp_error_t     *p_ecode )
{
    sctp_ep_st     *p_ep = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_socket_ext" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( !p_assoc_id )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );


        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( ( addr_family != SCTP_AF_INET ) && ( addr_family != SCTP_AF_INET6 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_APPID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "InValid Address Family " ) );

        *p_ecode = E_UNSUPP_ADDR_TYPE;

        return ( SCTP_FAILURE );

    }

    if( SCTP_FAILURE == sctp_db_validate_appl( application_id ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_APPID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Application Validation Failed, Application Id = %d",
                      application_id ) );

        *p_ecode = EINV_APPID;

        return ( SCTP_FAILURE );
    }

    if( callback_send_func == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC ,
                    ( "SCTP TRACE:: Callback Function NULL" ) );

    }


    /* Get a free association */
    p_ep = sctp_db_get_free_ep( p_ecode );

    if( SCTP_NULL == p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_NORESOURCES, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL , SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Get Free Association Failed,No Resources" ) );

        *p_ecode = E_NORESOURCES;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( p_ep->ep_id );

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                ( "SCTP TRACE::SU Module:New Socket Created, Association Id = %u",
                  p_ep->ep_id ) );

    /* Reset the flag CLOSE_CALLED */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_FALSE );

    p_ep->appl_id = application_id;

    p_ep->family = addr_family;
    p_ep->sk_type = SCTP_SOCK_STREAM; /* Only TCP Style socket supported */
    *p_assoc_id = p_ep->ep_id;
    p_ep->send_func = ( sctp_callback_sendmsg_fpt ) callback_send_func;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( p_ep->ep_id );

    return ( SCTP_SUCCESS );
}
#endif

/* Callback for Sending Function SPR 20859 Stops*/

/***************************************************************************
 *  FUNCTION :   sctp_api_close
 *
 *  DESCRIPTION:
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_close(
    sctp_U32bit       assoc_id,
    sctp_Boolean_t    graceful,
    sctp_error_t      *p_ecode )
{
    sctp_ep_st  *p_ep       = SCTP_NULL;
    sctp_tcb_st *p_curr_tcb = SCTP_NULL;
    sctp_tcb_st *p_temp     = SCTP_NULL;

    if( !p_ecode )
    {
        return( SCTP_FAILURE );
    }

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_api_close" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( ( graceful != SCTP_TRUE ) && ( graceful != SCTP_FALSE ) )
    {
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* set the flag CLOSE_CALLED */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_TRUE );

    /* Do a Abort or a shutdown */
    if( SCTP_FALSE == graceful )
    {
        /* Check for the associations in EndPoint */
        p_curr_tcb = ( sctp_tcb_st * )lstFirst( &( p_ep->assoc_list ) );

        if( SCTP_NULL != p_curr_tcb )
        {
            /* If any association then call state machine function for all */
            while( p_curr_tcb )
            {
                p_temp = ( sctp_tcb_st * )lstNext( ( NODE * )p_curr_tcb );
                p_curr_tcb->event.cmd_abort.cause_length = 0;
                p_curr_tcb->event.msg_abort.cause_length = 0;
                p_curr_tcb->event.msg_abort.p_cause      = SCTP_NULL;
                sctp_assoc_state_machine( p_curr_tcb, SCTPEVENT_CMDABORT );
                p_curr_tcb = p_temp;
            }
        }

        else
        {
            /* if no assoc simply delete the ep */
            sctp_db_delete_ep( assoc_id, p_curr_tcb, SCTP_TRUE );
        }
    }

    else /* graceful close */
    {
        /* Check for the associations in EndPoint */
        p_curr_tcb = ( sctp_tcb_st * )lstFirst( &( p_ep->assoc_list ) );

        if( SCTP_NULL != p_curr_tcb )
        {
            /* If any association then call state machine function for all */
            while( p_curr_tcb )
            {
                p_temp = ( sctp_tcb_st * )lstNext( ( NODE * )p_curr_tcb );
                sctp_assoc_state_machine( p_curr_tcb, SCTPEVENT_CMDTERM );
                p_curr_tcb = p_temp;
            }

            /* Mark this endpoint as not "writeable" */
            SCTP_SOCK_SET_WR_BLOCKED( p_ep->flags, SCTP_FALSE );
        }

        else
        {
            /* if no assoc simply delete the ep */
            sctp_db_delete_ep( assoc_id, p_curr_tcb, SCTP_TRUE );
        }
    }


    SCTP_UNLOCK_EP( assoc_id );
    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :   sctp_api_close_ext
 *
 *  DESCRIPTION:
 *         This function is used for User Initiated ABORT.
 *         User can optionally specify the "cause" which is
 *         sent transperantly to the Peer SCTP.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t sctp_api_close_ext(
    sctp_assoc_t assoc_id,
    sctp_U16bit  cause_len,
    sctp_U8bit   *cause,
    sctp_error_t *p_ecode )
{
    sctp_ep_st  *p_ep       = SCTP_NULL;
    sctp_tcb_st *p_curr_tcb = SCTP_NULL;
    sctp_tcb_st *p_temp     = SCTP_NULL;

    if( !p_ecode )
    {
        return( SCTP_FAILURE );
    }

    if( cause_len > 0 && cause == SCTP_NULL )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR : Cause Length = %d and cause == NULL", cause_len ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );

    }

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_api_close_ext" ) );

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* set the flag CLOSE_CALLED */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_TRUE );

    /* Check for the associations in EndPoint */
    p_curr_tcb = ( sctp_tcb_st * )lstFirst( &( p_ep->assoc_list ) );

    if( SCTP_NULL != p_curr_tcb )
    {
        /* If any association then call state machine function for all */
        while( p_curr_tcb )
        {
            p_temp = ( sctp_tcb_st * )lstNext( ( NODE * )p_curr_tcb );
            p_curr_tcb->event.cmd_abort.cause_length = cause_len;
            p_curr_tcb->event.cmd_abort.cause_info   = cause;
            p_curr_tcb->event.msg_abort.cause_length = 0;
            p_curr_tcb->event.msg_abort.p_cause      = SCTP_NULL;
            sctp_assoc_state_machine( p_curr_tcb, SCTPEVENT_CMDABORT );
            p_curr_tcb = p_temp;
        }
    }

    else
    {
        /* if no assoc simply delete the ep */
        sctp_db_delete_ep( assoc_id, p_curr_tcb, SCTP_TRUE );
    }

    SCTP_UNLOCK_EP( assoc_id );
    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_bind
 *
 *  DESCRIPTION:
 *    Binds socket with a sctp port and initialize listen queue for accepting
 *    new connections on this port. If hostname is NULL and number of su addrs
 *    are zero, SCTP uses default local addresses.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_bind(
    sctp_U32bit            assoc_id,
    sctp_U16bit            sctp_port,
    sctp_U32bit            no_of_su_addrs,
    sctp_sockaddr_list_st  list_of_addrs,
    sctp_U8bit             *p_hostname,
    sctp_error_t           *p_ecode )
{
    sctp_sockaddr_list_st  tmp_addr_list;
    sctp_sockaddr_st       *p_addr_list = SCTP_NULL;
    sctp_U32bit            counter = 0;
    sctp_ep_st             *p_ep   = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_bind " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    /* Get the pointer to the endpoint */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC, ( "Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    /* Get the pointer to the endpoint */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;
        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_hostname != SCTP_NULL ) &&
            ( no_of_su_addrs == 0 ) &&
            ( p_hostname[0] != '\0' ) )
    {
        /* hostname non NULL, resolve, static check on hostname */
        for( counter = 0; counter < SCTP_MAX_HOSTNAME_LEN; counter++ )
        {
            if( p_hostname[counter] == '\0' )
            {
                break;
            }
        }

        if( counter == SCTP_MAX_HOSTNAME_LEN )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        EINV_PARAMS, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            /* Non NULL terminated Hostname string */
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "SCTP TRACE::SU Module:Invalid Hostname String !!!" ) );

            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        /* resolve hostname */
        if( ( sctp_TL_resolve_hostname( p_hostname, &no_of_su_addrs,
                                        &tmp_addr_list[0] ) == SCTP_FAILURE )
                || ( no_of_su_addrs == 0 ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_UNKNOWN_HOST, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            *p_ecode = E_UNKNOWN_HOST;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        p_addr_list = ( sctp_sockaddr_st * ) tmp_addr_list;

    }

    else
    {
        if( ( no_of_su_addrs > SCTP_MAX_TRANSPORT_ADDR )
                || ( no_of_su_addrs == 0 )
                || ( ( no_of_su_addrs != 0 ) && ( list_of_addrs == SCTP_NULL ) )
                || ( sctp_port == 0 ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        EINV_PARAMS, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid Address Parameters" ) );

            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        p_addr_list = ( sctp_sockaddr_st * ) list_of_addrs;

        if( SCTP_IS_INADDR_ANY( p_addr_list[0] ) )
        {
            no_of_su_addrs = 0;

            for( counter = 0; counter < sctp_db_main.num_local_addrs; counter++ )
            {

                if( p_ep->family == SCTP_AF_INET6 )
                {
                    p_addr_list[no_of_su_addrs++] =
                        sctp_db_main.addr_list[counter];
                }

                else
                {
                    if( p_ep->family == sctp_db_main.addr_list[counter].family )
                    {
                        p_addr_list[no_of_su_addrs++] =
                            sctp_db_main.addr_list[counter];
                    }
                }
            }

            if( no_of_su_addrs == 0 )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            E_UNSUPP_ADDR_TYPE, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Invalid Address Parameters" ) );

                *p_ecode = E_UNSUPP_ADDR_TYPE;

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );

            }
        }
    }

    /*
     * Check if the address family of the addresses matches the address family
     * supported by the socket.
     */
    if( SCTP_FALSE == sctp_db_is_af_correct( p_ep, no_of_su_addrs, p_addr_list ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Address format X" ) );

        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Check that the list of address to bind to are a valid list of the host
     * addresses.
     */
    if( sctp_db_validate_bind_address( no_of_su_addrs, p_addr_list )
            == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Invalid bind address */
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid Bind Address," ) );

        SCTP_TRACE_ADDR( p_ep, ( SCTP_INTF_TRC | SCTP_ERR_TRC ),
                         ( &p_addr_list[counter] ) );

        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Verify that the socket/Endpoint is not already Bound to
     */
    if( SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Request in Wrong State " ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Check that addresses passed are valid and do not create any conflict in
     * the stack.
     */
    if( sctp_db_addr_bindable( sctp_port, no_of_su_addrs, p_addr_list )
            == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Port Unavailable for Bind ,Port = %u ", sctp_port ) );

        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Bind the endpoint
     */
    if( sctp_db_bind_ep( assoc_id, sctp_port, no_of_su_addrs, p_addr_list,
                         p_ecode ) == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    *p_ecode, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "Socket bound" ) );

    /*Save the hostname Information with endpoint */
    if( p_hostname )
    {
        for( counter = 0; p_hostname[counter] != '\0'; counter++ )
        {
            p_ep->hostname[counter] = p_hostname[counter];
        }
    }

    p_ep->hostname[counter] = '\0';

    /* Mark the socket as bound */
    SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );

    /*
     * Add the socket/Endpoint to the Bind hash list
     */
    sctp_db_add_hash( SCTP_BIND_HASH, p_ep );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}






/***************************************************************************
 *  FUNCTION :    sctp_api_bindx
 *
 *  DESCRIPTION:
 *    Binds socket with a sctp port and initialize listen queue for accepting
 *    new connections on this port. If hostname is NULL and number of su addrs
 *    are zero, SCTP uses default local addresses.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_bindx(
    sctp_U32bit            assoc_id,
    sctp_U32bit            n_addr,
    sctp_sockaddr_list_st  sctp_addr_list,
    sctp_bindx_flag_et     flags,
    sctp_Boolean_t         block,
    sctp_error_t           *p_ecode )
{
    sctp_ep_st            *p_ep;
    sctp_tcb_st           *p_tcb = NULL;
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_U32bit           counter = 0;
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_bindx" ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( block );
    /*to avoid the warning*/

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        return ( SCTP_FAILURE );
    }

    /*
     * Get the endpoint corresponding to the socket.
     */
    if( !( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) )
    {

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );
        *p_ecode = EINV_ASSOCID;
        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    if( !( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) )
    {

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid socket descriptor " ) );
        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );
        return ( SCTP_FAILURE );
    }

    /*validate parameters*/
    *p_ecode = EINV_PARAMS;

    /*
     * Verify that the address passed is a valid address for the m/c.
     */
    if( !sctp_db_validate_bind_address( n_addr, sctp_addr_list ) )
    {

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address not of local machine" ) );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Validate the Bind Flag
     */
    if( !SCTP_VALID_BINDX_FLAGS( flags ) )
    {
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    sk_errno, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Bindx Flag" ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Verify that the address type passed is consistent with the family
     * the socket belongs to.
     */
    /* SPR 21127 changes */
    /*if (SCTP_FAILURE == sctp_db_is_af_correct(p_ep, n_addr, sctp_addr_list))*/
    if( SCTP_FAILURE == ( sctp_return_t )sctp_db_is_af_correct( p_ep, n_addr, sctp_addr_list ) )
    {

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address family is not right" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Check that the socket is already in Bound state. In case bind has not
     * been called for the socket then we cannot call bindx.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Socket is not bound or port is incorrect" ) );

        /* Unlock the Endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( p_tcb != SCTP_NULL )
    {
        /*asscoiation exists */
        if( SCTP_FAILURE == sctp_dynamic_ip_supported( p_tcb,
                                                       ( SCTP_BINDX_ADD_ADDR == flags ) ?
                                                       SCTP_ASCONF_PARAM_ADD_IP : SCTP_ASCONF_PARAM_DEL_IP ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Dynamic Ip not supported" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

        if( SCTP_BINDX_ADD_ADDR == flags )
        {
            sctp_U32bit     counter1 = 0;

            if( !sctp_db_addr_bindable( p_ep->this_port, n_addr, sctp_addr_list ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_PARAMS, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Address is not bindable" ) );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            /*above function might return success if RE_USE is set and
              address is already added to ep, check for that */
            if( SCTP_SOCK_IS_REUSEADDR( p_ep->flags ) )
            {
                /* SPR : 18527 */
                for( counter = 0; counter < SCTP_MAX_TRANSPORT_ADDR && counter < n_addr; counter++ )
                {
                    for( counter1 = 0; counter1 < SCTP_MAX_TRANSPORT_ADDR && counter1 < p_ep->naddr; counter1++ )
                        /* SPR : 18527 */
                    {
                        if( SCTP_SUCCESS == sctp_cmp_addr( &sctp_addr_list[counter],
                                                           &p_ep->this_address[counter1] ) )
                        {
                            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                                        EINV_PARAMS, "Service User API ERROR" );

                            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                        ( "Address is not bindable" ) );

                            /* Unlock the endpoint */
                            SCTP_UNLOCK_EP( assoc_id );

                            return ( SCTP_FAILURE );
                        }
                    }
                }
            }


            for( counter = 0; counter < n_addr; counter++ )
            {
                sctp_asconf_addip_param_st  *p_add_ip =
                    &( p_tcb->cmd_asconf.asconf_params[counter].add_ip );

                p_add_ip->param_type = SCTP_ASCONF_PARAM_ADD_IP;
                p_add_ip->corr_id = counter;
                p_add_ip->ipaddr = sctp_addr_list[counter];
            }
        }

        else
        {
            /*chek if the addess can be removed*/
            if( SCTP_FAILURE == sctp_db_chk_addr_for_delete( p_ep, p_ep->this_port,
                                                             n_addr, sctp_addr_list, p_ecode ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_PARAMS, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                            ( "Error Removing address from the Endpoint" ) );

                /* Unlock the Endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            for( counter = 0; counter < n_addr; counter++ )
            {
                sctp_asconf_addip_param_st  *p_del_ip =
                    &( p_tcb->cmd_asconf.asconf_params[counter].del_ip );

                p_del_ip->param_type = SCTP_ASCONF_PARAM_DEL_IP;
                p_del_ip->corr_id    = counter;
                p_del_ip->ipaddr     = sctp_addr_list[counter];
            }
        }

        p_tcb->cmd_asconf.num_params = counter;

        if( SCTP_FAILURE == sctp_chk_and_send_asconf( p_tcb, flags,
                                                      block, p_ecode ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_SUCCESS );
#endif
    }

    /*
     * Socket is not connected.
     */
    if( SCTP_BINDX_ADD_ADDR == flags )
    {
        /*
         * Verify that the address is "Bind'able".
         */
        if( !sctp_db_addr_bindable( p_ep->this_port, n_addr, sctp_addr_list ) )
        {
            sk_errno = SK_EADDRINUSE;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Address is not bindable" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        /*
         * Add the new addresses into the existing endpoint.
         */


        if( sctp_db_bind_ep( assoc_id, p_ep->this_port, n_addr, sctp_addr_list, p_ecode ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC,
                        ( "SCTP_BINDX_ADD_ADDR Executed" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_SUCCESS );
        }

        else
        {
            sk_errno = SK_EINVAL;
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "SCTP TRACE::SU Module:End-point already bound" ) );

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

    }

    else
    {
        /*
         * Remove an address from an endpoint. SCTP_BINDX_REM_ADDR.
         */

        /*
         * Rules for removing an address from an endpoint.
         * 1. The Address should be available in the endpoint else error.
         * 2. The Endpoint should not be set to listen preferably. In case the
         *    Endpoint is set to listen then we should not have any connection
         *    waiting to be accept'ed from this server.
         * 3. The Endpoint cannot have a tcb in any state except CLOSED.
         * The DB function sctp_db_rem_addr_after_chks does all these checks.
         */

        if( !sctp_db_rem_addr_after_chks( p_ep, p_ep->this_port, n_addr, sctp_addr_list,
                                          p_ecode ) )
        {
            sk_errno = SK_EINVAL;

            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        sk_errno, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_TRACE( SCTP_NULL, SCTP_ERR_TRC,
                        ( "Error Removing address from the Endpoint" ) );

            /* Unlock the Endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}




/**************************************************************************
 *  FUNCTION :  sctp_parse_api_config_socket
 *
 *  DESCRIPTION:
 *    This function performs static checks on parameter passed in
 *    config socket API.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_parse_api_config_socket(
    sctp_ep_st           *p_ep,
    sctp_prot_params_st  *p_prot_params,
    sctp_error_t         *p_ecode )
{
    sctp_U16bit rto_alpha;
    sctp_U16bit rto_beta;
    sctp_U32bit rto_init;
    sctp_U32bit rto_min;
    sctp_U32bit rto_max;
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_prot_params_st tmp_config;
    sctp_U16bit max_hb_infolen;


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_parse_api_config_socket" ) );

    if( !p_prot_params )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid Prot Parameters Structure" ) );

        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL == p_ass )
    {
        tmp_config = p_ep->sk_config;
    }

    else
    {
        tmp_config = p_ass->config;
    }


    if( p_prot_params->rto_min != 0 )
    {
        if( ( p_prot_params->rto_min <= SCTP_MAX_RTO_MIN ) &&
                ( p_prot_params->rto_min >= SCTP_MIN_RTO_MIN ) )
        {
            rto_min = p_prot_params->rto_min;
        }

        else
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "SU Module:Invalid RTO_MIN Prot Parameters Structure" ) );

            *p_ecode = EINV_RTO_MIN_PARAMS;

            return ( SCTP_FAILURE );
        }
    }

    else
    {
        rto_min = tmp_config.rto_min;
    }

    if( p_prot_params->rto_max != 0 )
    {
        if( ( p_prot_params->rto_max <= SCTP_MAX_RTO_MAX ) &&
                ( p_prot_params->rto_max >= SCTP_MIN_RTO_MAX ) )
        {
            rto_max = p_prot_params->rto_max;
        }

        else
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "SU Module:Invalid RTO_MAX Prot Parameters Structure" ) );

            *p_ecode = EINV_RTO_MAX_PARAMS;

            return ( SCTP_FAILURE );
        }
    }

    else
    {
        rto_max = tmp_config.rto_max;
    }

    /* static checks on RTO parameters */
    if( p_prot_params->rto_init != 0 )
    {
        if( ( p_prot_params->rto_init >= rto_min ) &&
                ( p_prot_params->rto_init <= rto_max ) )
        {
            rto_init = p_prot_params->rto_init;
        }

        else
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "SU Module:Invalid RTO_INIT Prot Parameters Structure" ) );

            *p_ecode = EINV_RTO_INIT_PARAMS;

            return ( SCTP_FAILURE );
        }
    }

    else
    {
        rto_init = tmp_config.rto_init;
    }

    /*
     * rto_max should be more than rto_min, rto_init and rto_max should be
     * more than rto_min.
     */
    if( ( rto_min > rto_init ) || ( rto_min > rto_max ) || ( rto_init > rto_max ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid Rto Parameters " ) );

        *p_ecode = EINV_RTO_COMBINATION_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( p_prot_params->valid_cookie_life != 0 )
    {
        if( ( p_prot_params->valid_cookie_life > SCTP_MAX_VALID_COOKIE_LIFE ) ||
                ( p_prot_params->valid_cookie_life < SCTP_MIN_VALID_COOKIE_LIFE ) )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "SU Module:Invalid cookie life Parameters " ) );

            *p_ecode = EINV_COOKIE_LIFE_PARAMS;

            return ( SCTP_FAILURE );

        }
    }

    if( p_prot_params->hb_interval != 0 )
    {
        if( ( p_prot_params->hb_interval * 1000 > SCTP_MAX_HB_INTERVAL )
                || ( p_prot_params->hb_interval * 1000 < SCTP_MIN_HB_INTERVAL ) )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "SU Module:Invalid HB_INTERVAL Parameters " ) );

            *p_ecode = EINV_HB_INTERVAL_PARAMS;
            return ( SCTP_FAILURE );
        }
    }

    if( ( p_prot_params->sack_frequency > SCTP_MAX_SACK_FREQUENCY ) ||
            ( p_prot_params->sack_frequency == 0 ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid SACK_FREQUENCY Parameters " ) );

        *p_ecode = EINV_SACK_FREQUENCY_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( p_prot_params->sack_period > SCTP_MAX_DELAYACK_TIME )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid SACK_PERIOD Parameters " ) );

        *p_ecode = EINV_SACK_PERIOD_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( ( p_prot_params->mtu_size < SCTP_MIN_MTUSIZE ) ||
            ( p_prot_params->mtu_size > SCTP_MAX_MTUSIZE ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid MTU_SIZE Parameters " ) );

        *p_ecode = EINV_MTU_SIZE_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL != p_ass )
    {

        max_hb_infolen = SCTP_IS_STACK_UDP ?
                         SCTP_UDP_MAX_HB_INFOLEN( p_ass->transmit.lowest_path_mtu ) :
                         SCTP_IP_MAX_HB_INFOLEN( p_ass->transmit.lowest_path_mtu );

        if( ( max_hb_infolen < p_prot_params->hb_info_length ) ||
                ( SCTP_MIN_HBINFO_LEN > p_prot_params->hb_info_length ) )
        {
            SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( "Invalid HB_INFO_LEN Parameters " ) );

            *p_ecode = EINV_HB_INFO_LEN;

            return ( SCTP_FAILURE );
        }
    }


    /* static checks on rto_alpha and rto_beta parameters*/
    rto_alpha = p_prot_params->rto_alpha_percent;
    rto_beta  = p_prot_params->rto_beta_percent;

    /* alpha & beta should be between 0-100, alpha should be less than beta*/

    if( ( rto_alpha > SCTP_MAX_PERCENTAGE_VALUE )
            || ( rto_beta  > SCTP_MAX_PERCENTAGE_VALUE )
            || ( ( rto_alpha != 0 ) && ( rto_beta != 0 ) && ( rto_alpha > rto_beta ) )
            || ( ( rto_alpha != 0 ) && ( rto_beta == 0 )
                 && ( rto_alpha > tmp_config.rto_beta_percent ) )
            || ( ( rto_alpha == 0 ) && ( rto_beta != 0 )
                 && ( rto_beta  < tmp_config.rto_alpha_percent ) ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Rto Percentage Parameters " ) );

        *p_ecode = EINV_ALPHA_BETA_PARAMS;

        return ( SCTP_FAILURE );
    }

    /* static checks on retrans parameters */
    if( ( ( p_prot_params->path_max_retrans != 0 )
            && ( p_prot_params->assoc_max_retrans != 0 )
            && ( p_prot_params->path_max_retrans > p_prot_params->assoc_max_retrans ) )
            || ( ( p_prot_params->path_max_retrans != 0 )
                 && ( p_prot_params->assoc_max_retrans == 0 )
                 && ( p_prot_params->path_max_retrans
                      > tmp_config.assoc_max_retrans ) )
            || ( ( p_prot_params->path_max_retrans == 0 )
                 && ( p_prot_params->assoc_max_retrans != 0 )
                 && ( p_prot_params->assoc_max_retrans
                      < tmp_config.path_max_retrans ) ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Parameter Max. Path Retrans more Max. Assoc Retrans" ) );

        *p_ecode = EINV_PATH_RETRANS_PARAMS;

        return ( SCTP_FAILURE );
    }

    if( ( p_prot_params->num_in_streams > sctp_db_main.max_streams_per_assoc )
            || ( p_prot_params->num_out_streams > sctp_db_main.max_streams_per_assoc ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:Invalid stream Parameters " ) );

        *p_ecode = EINV_STREAM_PARAMS;

        return ( SCTP_FAILURE );
    }

    return ( SCTP_SUCCESS );
}


/*************************************************************************
 *  FUNCTION :   sctp_process_api_config_socket
 *
 *  DESCRIPTION:
 *    Set configuration parameters for a particulas socket. Currently
 *    this api can be called only after bind and before connect or listen
 *  RETURN VALUE:
 *    None
 *
 **************************************************************************/
void
sctp_process_api_config_socket(
    sctp_ep_st            *p_ep,
    sctp_prot_params_st   *p_prot_params )
{
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_error_t  ecode;

    SCTP_TRACE( p_ep, SCTP_INTF_TRC , ( "sctp_process_api_config_socket " ) );

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_SOCK_SET_REUSEADDR( p_ep->flags, p_prot_params->re_use_addr );

    p_ep-> ppi  = p_prot_params->ppi;

    if( SCTP_NULL == p_ass )
    {
        if( p_prot_params->rto_max != 0 )
        {
            p_ep->sk_config.rto_max = p_prot_params->rto_max;
        }

        if( p_prot_params->rto_min != 0 )
        {
            p_ep->sk_config.rto_min = p_prot_params->rto_min;
        }

        if( p_prot_params->assoc_max_retrans != 0 )
        {
            p_ep->sk_config.assoc_max_retrans
                = p_prot_params->assoc_max_retrans;
        }

        if( p_prot_params->path_max_retrans != 0 )
        {
            p_ep->sk_config.path_max_retrans
                = p_prot_params->path_max_retrans;
        }

        /* heartbeat interval expected from  user in second */
        if( p_prot_params->hb_interval != 0 )
        {
            p_ep->sk_config.hb_interval = p_prot_params->hb_interval * 1000;
        }

        if( p_prot_params->rto_init != 0 )
        {
            p_ep->sk_config.rto_init = p_prot_params->rto_init;
        }

        if( p_prot_params->rto_alpha_percent != 0 )
        {
            p_ep->sk_config.rto_alpha_percent = p_prot_params->rto_alpha_percent;
        }

        if( p_prot_params->rto_beta_percent != 0 )
        {
            p_ep->sk_config.rto_beta_percent = p_prot_params->rto_beta_percent;
        }

        if( p_prot_params-> valid_cookie_life != 0 )
        {
            p_ep->sk_config.valid_cookie_life
                = p_prot_params->valid_cookie_life;
        }

        if( p_prot_params->max_init_retrans != 0 )
        {
            p_ep->sk_config.max_init_retrans = p_prot_params->max_init_retrans;
        }

        if( p_prot_params->max_init_path_retrans != 0 )
        {
            p_ep->sk_config.max_init_path_retrans = p_prot_params->max_init_path_retrans;
        }

        if( p_prot_params->num_in_streams != 0 )
        {
            p_ep->sk_config.num_in_streams = p_prot_params->num_in_streams;
        }

        if( p_prot_params->num_out_streams != 0 )
        {
            p_ep->sk_config.num_out_streams = p_prot_params->num_out_streams;
        }

        if( p_prot_params->adaption_ind != 0 )
        {
            p_ep->sk_config.adaption_ind = p_prot_params->adaption_ind;
        }

        if( p_prot_params->bundling_time != 0 )
        {
            p_ep->sk_config.bundling_time = p_prot_params->bundling_time;
        }

        p_ep->sk_config.dont_fragment = p_prot_params->dont_fragment;

        p_ep->sk_config.sack_period = p_prot_params->sack_period;

        p_ep->sk_config.sack_frequency = p_prot_params->sack_frequency;

        p_ep->sk_config.mtu_size = p_prot_params->mtu_size;

        p_ep->sk_config.pmtu_capable = p_prot_params->pmtu_capable;
        /* Rel 6.2.0 change start */
        p_ep->sk_config.hb_max_burst = p_prot_params->hb_max_burst;
        /* Rel 6.2.0 change end */
#ifdef SCTP_IP_HEADER_PARAMS
        p_ep->sk_config.ip_header_params.ttl = p_prot_params->ip_header_params.ttl;
        p_ep->sk_config.ip_header_params.tos = p_prot_params->ip_header_params.tos << 2;
        /* Rel 6.2.0 change start */
#ifdef SCTP_INET6_SUPPORT

#if defined (IPV6_HOPLIMIT)
        p_ep->sk_config.ip_header_params.hoplimit = p_prot_params->ip_header_params.hoplimit;
#else
        SCTP_TRACE( p_ep, SCTP_DBG_TRC | SCTP_ERR_TRC,
                    ( "sctp_process_api_config_socket: IPV6_HOPLIMIT not defined in the System." ) );
#endif

#if defined (IPV6_TCLASS)
        p_ep->sk_config.ip_header_params.tclass = p_prot_params->ip_header_params.tclass;
#else
        SCTP_TRACE( p_ep, SCTP_DBG_TRC | SCTP_ERR_TRC,
                    ( "sctp_process_api_config_socket: IPV6_TCLASS not defined in the System." ) );
#endif

#endif
        /* Rel 6.2.0 change end */
#endif

        /* length of heartbeat info parameter */
        p_ep->sk_config.hb_info_length = p_prot_params->hb_info_length;
    }

    else
    {
        if( p_prot_params->rto_max != 0 )
        {
            p_ass->config.rto_max = p_prot_params->rto_max;
        }

        if( p_prot_params->rto_min != 0 )
        {
            p_ass->config.rto_min = p_prot_params->rto_min;
        }

        if( p_prot_params->rto_alpha_percent != 0 )
        {
            p_ass->config.rto_alpha_percent = p_prot_params->rto_alpha_percent;
        }

        if( p_prot_params->rto_beta_percent != 0 )
        {
            p_ass->config.rto_beta_percent = p_prot_params->rto_beta_percent;
        }

        if( p_prot_params->assoc_max_retrans != 0 )
        {
            p_ass->config.assoc_max_retrans = p_prot_params->assoc_max_retrans;
        }

        if( p_prot_params->path_max_retrans != 0 )
        {
            p_ass->config.path_max_retrans = p_prot_params->path_max_retrans;
        }

        /* heartbeat interval expected from  user in second */
        if( p_prot_params->hb_interval != 0 )
        {
            p_ass->config.hb_interval = p_prot_params->hb_interval * 1000;
        }


        p_ass->config.bundling_time = p_prot_params->bundling_time;

        p_ep->sk_config.dont_fragment = p_prot_params->dont_fragment;

        p_ass->config.sack_period = p_prot_params->sack_period;

        p_ass->config.sack_frequency = p_prot_params->sack_frequency;

        /* length of heartbeat info parameter */
        p_ass->config.hb_info_length = p_prot_params->hb_info_length;
        /* REl 6.2.0 change start */
        p_ass->config.hb_max_burst = p_prot_params->hb_max_burst;
        /* REl 6.2.0 change end */
#ifdef SCTP_IP_HEADER_PARAMS

        p_ass->config.ip_header_params.ttl = p_prot_params->ip_header_params.ttl;
        p_ass->config.ip_header_params.tos = p_prot_params->ip_header_params.tos << 2;

#ifdef SCTP_INET6_SUPPORT

#if defined (IPV6_HOPLIMIT)
        p_ass->config.ip_header_params.hoplimit = p_prot_params->ip_header_params.hoplimit;
#else
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                    ( "sctp_process_api_config_socket: IPV6_HOPLIMIT not defined in the System." ) );
#endif

#if defined (IPV6_TCLASS)
        p_ass->config.ip_header_params.tclass   = p_prot_params->ip_header_params.tclass;
#else
        SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                    ( "sctp_process_api_config_socket: IPV6_TCLASS not defined in the System." ) );
#endif

#endif /* SCTP_INET6_SUPPORT */

#endif /* SCTP_IP_HEADER_PARAMS */


        if( p_ass->state == SCTPSTATE_CLOSED )
        {
            if( p_prot_params->rto_init != 0 )
            {
                p_ass->config.rto_init = p_prot_params->rto_init;
            }


            if( p_prot_params-> valid_cookie_life != 0 )
                p_ass->config.valid_cookie_life
                    = p_prot_params->valid_cookie_life;

            if( p_prot_params->max_init_retrans != 0 )
                p_ass->config.max_init_retrans
                    = p_prot_params->max_init_retrans;

            if( p_prot_params->max_init_path_retrans != 0 )
                p_ass->config.max_init_path_retrans
                    = p_prot_params->max_init_path_retrans;

            if( p_prot_params->num_in_streams != 0 )
            {
                p_ass->config.num_in_streams = p_prot_params->num_in_streams;
            }

            if( p_prot_params->num_out_streams != 0 )
                p_ass->config.num_out_streams
                    = p_prot_params->num_out_streams;

            p_ep->sk_config.adaption_ind = p_prot_params->adaption_ind;

            p_ass->config.mtu_size       = p_prot_params->mtu_size;

            p_ass->config.pmtu_capable   = p_prot_params->pmtu_capable;

#ifdef SCTP_IP_HEADER_PARAMS

            p_ass->config.ip_header_params.ttl = p_prot_params->ip_header_params.ttl;
            p_ass->config.ip_header_params.tos = p_prot_params->ip_header_params.tos << 2;

#ifdef SCTP_INET6_SUPPORT

#if defined (IPV6_HOPLIMIT)
            p_ass->config.ip_header_params.hoplimit = p_prot_params->ip_header_params.hoplimit;
#else
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                        ( "sctp_process_api_config_socket: IPV6_HOPLIMIT not defined in the System." ) );
#endif

#if defined (IPV6_TCLASS)
            p_ass->config.ip_header_params.tclass   = p_prot_params->ip_header_params.tclass;
#else
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                        ( "sctp_process_api_config_socket: IPV6_TCLASS not defined in the System." ) );
#endif

#endif /* SCTP_INET6_SUPPORT */
#endif /* SCTP_IP_HEADER_PARAMS */

        }

        else
        {
            sctp_set_pmtu_capable( p_ass, p_prot_params->pmtu_capable, &ecode );
            sctp_set_mtu_size( p_ass, p_prot_params->mtu_size, &ecode );
        }
    }

    return;
}


/***************************************************************************
 *  FUNCTION :    sctp_api_config_assoc
 *
 *
 *  DESCRIPTION:
 *    set configuration parameters for a particular socket.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_config_assoc(
    sctp_U32bit           assoc_id,
    sctp_prot_params_st   *p_prot_params,
    sctp_error_t          *p_ecode )
{
    sctp_return_t ret;
    sctp_ep_st    *p_ep = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_config_assoc " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL , SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL , SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association " ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    ret  = sctp_parse_api_config_socket( p_ep, p_prot_params, p_ecode );

    if( ret == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    sctp_process_api_config_socket( p_ep, p_prot_params );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_process_api_config_assoc_ctl
 *
 *  DESCRIPTION:
 *    set configuration parameters for a particular socket.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_process_api_config_assoc_ctl(
    sctp_prot_params_st   *p_prot_params,
    sctp_prot_params_et   prot_parameter,
    sctp_U32bit           parameter_value,
    sctp_Boolean_t        assoc_state_flag,
    sctp_error_t          *p_ecode )
{
    sctp_return_t ret = SCTP_SUCCESS;

    switch( prot_parameter )
    {
        case RTO_INIT:
            if( ( parameter_value >= p_prot_params->rto_min )
                    && ( parameter_value < p_prot_params->rto_max )
                    && ( parameter_value != 0 )
                    && ( SCTP_TRUE == assoc_state_flag ) )
            {
                p_prot_params->rto_init = parameter_value;
            }

            else
            {
                *p_ecode = EINV_RTO_INIT_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case RTO_MIN:
            if( ( parameter_value < p_prot_params->rto_max )
                    && ( parameter_value < p_prot_params->rto_init )
                    && ( parameter_value >= SCTP_MIN_RTO_MIN )
                    && ( parameter_value != 0 ) )
            {
                p_prot_params->rto_min = parameter_value;
            }

            else
            {
                *p_ecode = EINV_RTO_MIN_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case RTO_MAX:
            if( ( parameter_value > p_prot_params->rto_min )
                    && ( parameter_value > p_prot_params->rto_init )
                    && ( parameter_value <= SCTP_MAX_RTO_MAX )
                    && ( parameter_value != 0 ) )
            {
                p_prot_params->rto_max = parameter_value;
            }

            else
            {
                *p_ecode = EINV_RTO_MAX_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case RTO_ALPHA_PERCENT:
            if( ( parameter_value <= SCTP_MAX_PERCENTAGE_VALUE )
                    && ( parameter_value < p_prot_params->rto_beta_percent )
                    && ( parameter_value != 0 ) )
            {
                p_prot_params->rto_alpha_percent
                    = ( sctp_U16bit )parameter_value;
            }

            else
            {
                *p_ecode = EINV_ALPHA_BETA_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case RTO_BETA_PERCENT:
            if( ( parameter_value <= SCTP_MAX_PERCENTAGE_VALUE )
                    && ( parameter_value > p_prot_params->rto_alpha_percent )
                    && ( parameter_value != 0 ) )
            {
                p_prot_params->rto_beta_percent
                    = ( sctp_U16bit )parameter_value;
            }

            else
            {
                *p_ecode = EINV_ALPHA_BETA_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case VALID_COOKIE_LIFE:
            if( ( parameter_value != 0 )
                    && ( SCTP_TRUE == assoc_state_flag )
                    && ( parameter_value < SCTP_MAX_VALID_COOKIE_LIFE )
                    && ( parameter_value > SCTP_MIN_VALID_COOKIE_LIFE ) )
            {
                p_prot_params->valid_cookie_life = parameter_value;
            }

            else
            {
                *p_ecode = EINV_COOKIE_LIFE_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case ASSOC_MAX_RETRANS:
            if( ( parameter_value > p_prot_params->path_max_retrans )
                    && ( parameter_value != 0 ) )
            {
                p_prot_params->assoc_max_retrans = parameter_value;
            }

            else
            {
                *p_ecode = EINV_ASSOC_MAX_RETRANS_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case PATH_MAX_RETRANS:
            if( ( parameter_value < p_prot_params->assoc_max_retrans )
                    && ( parameter_value != 0 ) )
            {
                p_prot_params->path_max_retrans = parameter_value;
            }

            else
            {
                *p_ecode = EINV_PATH_MAX_RETRANS_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case MAX_INIT_RETRANS:
            if( ( parameter_value != 0 )
                    && ( SCTP_TRUE == assoc_state_flag ) )
            {
                p_prot_params->max_init_retrans = parameter_value;
            }

            else
            {
                *p_ecode = EINV_MAX_INIT_RETRANS_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case NUM_IN_STREAMS:
            if( ( parameter_value <= sctp_db_main.max_streams_per_assoc )
                    && ( parameter_value != 0 )
                    && ( SCTP_TRUE == assoc_state_flag ) )

            {
                p_prot_params->num_in_streams = ( sctp_U16bit )parameter_value;
            }

            else
            {
                *p_ecode = EINV_STREAM_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case NUM_OUT_STREAMS:
            if( ( parameter_value <= sctp_db_main.max_streams_per_assoc )
                    && ( parameter_value != 0 )
                    && ( SCTP_TRUE == assoc_state_flag ) )
            {
                p_prot_params->num_out_streams = ( sctp_U16bit )parameter_value;
            }

            else
            {
                *p_ecode = EINV_STREAM_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;


        case HB_INTERVAL:
            if( ( parameter_value != 0 )
                    && ( parameter_value * 1000 <= SCTP_MAX_HB_INTERVAL )
                    && ( parameter_value * 1000 >= SCTP_MIN_HB_INTERVAL ) )
            {
                p_prot_params->hb_interval = parameter_value * 1000;
            }

            else
            {
                *p_ecode = EINV_HB_INTERVAL_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

            /*SPR 4798 */
        case BUNDLING_TIME:
            p_prot_params->bundling_time = parameter_value;
            break;

            /*SPR 4798 */
        case SACK_PERIOD:
            if( parameter_value <= SCTP_MAX_DELAYACK_TIME )
            {
                p_prot_params->sack_period = parameter_value;
            }

            else
            {
                *p_ecode = EINV_SACK_PERIOD_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case SACK_FREQUENCY:
            if( ( parameter_value != 0 ) &&
                    ( parameter_value <= SCTP_MAX_SACK_FREQUENCY ) )
            {
                p_prot_params->sack_frequency = parameter_value;
            }

            else
            {
                *p_ecode = EINV_SACK_FREQUENCY_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

        case ADAPTION_IND:
            p_prot_params->adaption_ind = parameter_value;
            break;

        case DONT_FRAGMENT:
            p_prot_params->dont_fragment = parameter_value;
            break;

        case WANT_MAPPED_ADDR:
            p_prot_params->want_mapped = parameter_value;
            break;

        case NAT_CAPABLE:
            p_prot_params->nat_capable = parameter_value;
            break;

        case PMTU_CAPABLE:
            if( SCTP_TRUE == assoc_state_flag )
            {
                p_prot_params->pmtu_capable = parameter_value;
            }

            break;

        case MTU_SIZE:
            if( SCTP_TRUE == assoc_state_flag )
            {
                if( ( parameter_value < SCTP_MIN_MTUSIZE ) ||
                        parameter_value > SCTP_MAX_MTUSIZE )
                {
                    *p_ecode = EINV_MTU_SIZE_PARAMS;
                    ret = SCTP_FAILURE;
                }

                else
                {

                    p_prot_params->mtu_size   = parameter_value;
                }
            }

            break;

        case ALLOW_ASCONF:
            p_prot_params->allow_asconf = parameter_value;
            break;

        case MAX_INIT_TIMEO:
            p_prot_params->init_timeo = parameter_value;
            break;

        case CHECKSUM_FUNCTION:
            if( parameter_value < SCTP_NUM_CHECKSUM )
            {
                p_prot_params->checksum_func = parameter_value;
            }

            else
            {
                *p_ecode = EINV_CHECKSUM_PARAMS;
                ret = SCTP_FAILURE;
            }

            break;

            /* REL 6.2.0 change start */
        case HB_MAX_BURST:
            p_prot_params->hb_max_burst = parameter_value;
            break;
            /* REL 6.2.0 change end */

            /* Changes done for CSR#1-4948130 */
#ifdef SCTP_IP_HEADER_PARAMS

        case IP_CONF_TTL:
            p_prot_params->ip_header_params.ttl = parameter_value;
            break;

        case IP_CONF_TOS:
            p_prot_params->ip_header_params.tos = parameter_value << 2;
            break;
            /* Changes done for CSR#1-4948130 Ends*/
            /* REL 6.2.0 change start */
#ifdef SCTP_INET6_SUPPORT

        case HOP_LIMIT:
#if defined (IPV6_HOPLIMIT)
            p_prot_params->ip_header_params.hoplimit = parameter_value;
#else
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                        ( "sctp_process_api_config_assoc_ctl: IPV6_HOPLIMIT not defined in the System." ) );
#endif
            break;

        case TRAFFIC_CLASS:
#if defined (IPV6_TCLASS)
            p_prot_params->ip_header_params.tclass = parameter_value;
#else
            SCTP_TRACE( SCTP_NULL, SCTP_DBG_TRC | SCTP_ERR_TRC,
                        ( "sctp_process_api_config_assoc_ctl: IPV6_TCLASS not defined in the System." ) );
#endif
            break;

#endif
            /* REL 6.2.0 change end */
#endif

        default:
            ret = SCTP_FAILURE;
            break;
    }

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_config_assoc_ctl
 *
 *  DESCRIPTION:
 *    set configuration parameters for a particular socket.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_config_assoc_ctl(
    sctp_U32bit          assoc_id,
    sctp_prot_params_et  prot_parameter,
    sctp_U32bit          parameter_value,
    sctp_error_t         *p_ecode )
{
    sctp_return_t ret    = SCTP_SUCCESS;
    sctp_U16bit   max_hb_infolen;
    sctp_ep_st    *p_ep  = SCTP_NULL;
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_U32bit   counter;

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    /*SPR 4798 */
    if( assoc_id == SCTP_INVALID_ASSOC )
    {
        /*set global heartbeat/select timer*/
        switch( prot_parameter )
        {
            case BUNDLING_TIME:
                SCTPLOCK();
                sctp_db_main.sctp_def_prot_params.bundling_time =
                    parameter_value;
                SCTPUNLOCK();
                break;

                /*SPR 4800*/
            case ESHELL_SELECT_TIME:
                if( 0 == parameter_value )
                {
                    *p_ecode = EINV_PARAMS;
                    return ( SCTP_FAILURE );
                }

                SCTPLOCK();
                sctp_db_main.eshell_select_time = parameter_value;
                SCTPUNLOCK();
                /*SPR 4800*/
                break;

            default:
                *p_ecode = EINV_ASSOCID;
                return ( SCTP_FAILURE );

        }

        return ( SCTP_SUCCESS );
    }

    /*SPR 4798 */

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*initialize the error code*/
    *p_ecode = EINV_PARAMS;

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( SUPPORT_HOSTNAMES == prot_parameter )
    {
        p_ep->hostname_supported = parameter_value;
    }

    else if( prot_parameter == RE_USE_ADDR )
    {
        SCTP_SOCK_SET_REUSEADDR( p_ep->flags, parameter_value );
    }

    else if( prot_parameter == PAYLOAD_PROTOCOL_ID )
    {
        p_ep->ppi = parameter_value;
    }

    else if( SCTP_NULL == p_ass )
    {
        ret = sctp_process_api_config_assoc_ctl( &p_ep->sk_config,
                                                 prot_parameter, parameter_value, SCTP_TRUE, p_ecode );
    }

    else
    {
        if( p_ass->state == SCTPSTATE_CLOSED )
        {
            ret = sctp_process_api_config_assoc_ctl( &p_ass->config,
                                                     prot_parameter, parameter_value, SCTP_TRUE, p_ecode );
        }

        else
        {
            if( SNDBUF == prot_parameter )
            {
                ret = sctp_db_set_tx_buffers( p_ass, parameter_value, p_ecode );
            }

            else if( RCVBUF == prot_parameter )
            {
                ret = sctp_db_set_rx_buffers( p_ass, parameter_value, p_ecode );
            }

            else if( PMTU_CAPABLE == prot_parameter )
            {
                ret = sctp_set_pmtu_capable( p_ass, parameter_value, p_ecode );
            }

            else if( MTU_SIZE == prot_parameter )
            {
                if( ( parameter_value >= SCTP_MIN_MTUSIZE ) &&
                        ( parameter_value <= SCTP_MAX_MTUSIZE ) )
                {

                    ret = sctp_set_mtu_size( p_ass, parameter_value, p_ecode );
                }

                else
                {
                    ret = SCTP_FAILURE;
                }
            }

            else if( HB_INFO_LENGTH == prot_parameter )
            {
                max_hb_infolen = SCTP_IS_STACK_UDP ?
                                 SCTP_UDP_MAX_HB_INFOLEN( p_ass->transmit.lowest_path_mtu ) :
                                 SCTP_IP_MAX_HB_INFOLEN( p_ass->transmit.lowest_path_mtu );

                if( ( max_hb_infolen < parameter_value ) ||
                        ( SCTP_MIN_HBINFO_LEN > parameter_value ) )
                {
                    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "Invalid HB_INFO_LEN Parameters " ) );

                    *p_ecode = EINV_HB_INFO_LEN;

                    ret = SCTP_FAILURE;
                }

                else
                {
                    p_ass->config.hb_info_length = parameter_value;
                }
            }

            else
            {
                ret = sctp_process_api_config_assoc_ctl( &p_ass->config,
                                                         prot_parameter, parameter_value, SCTP_FALSE, p_ecode );
            }
        }

        if( ( ret == SCTP_SUCCESS ) && ( prot_parameter == PATH_MAX_RETRANS ) )
        {
            for( counter = 0; counter < SCTP_MAX_TRANSPORT_ADDR; counter++ )
            {
                p_ass->transmit.remote[counter].path_error_threshold
                    = p_ass->config.path_max_retrans;
            }
        }
    }

    if( ret == SCTP_FAILURE )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

    }

    SCTP_UNLOCK_EP( assoc_id );

    return ( ret );
}

/***************************************************************************
 *  FUNCTION :    sctp_api_config_txrx_num_frag
 *
 *  DESCRIPTION:  This function configure number of Rx/TX fragments changes for
 *  particular assoc.
 *
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_config_txrx_num_frag(
    sctp_U32bit          assoc_id,
    sctp_count_frags_st  count_frags,
    sctp_Boolean_t       is_transmit,
    sctp_Boolean_t       is_increment,
    sctp_error_t         *p_ecode )
{
    sctp_return_t ret    = SCTP_SUCCESS;
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_ep_st    *p_ep  = SCTP_NULL;

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;
        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*initialize the error code*/
    *p_ecode = EINV_PARAMS;

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL == p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    else
    {
        if( is_transmit == SCTP_TRUE )
        {
            ret = sctp_db_set_tx_frags( p_ass, count_frags, is_increment, p_ecode );
        }

        else
        {
            ret = sctp_db_set_rx_frags( p_ass, count_frags, is_increment, p_ecode );
        }
    }

    SCTP_UNLOCK_EP( assoc_id );
    return ( ret );
}

/***************************************************************************
 *  FUNCTION :    sctp_api_get_free_txrx_num_frag
 *
 *  DESCRIPTION:  This function returns number of Rx/TX fragments available for
 *  particular assoc.
 *
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_get_free_txrx_num_frag(
    sctp_U32bit          assoc_id,
    sctp_count_frags_st  *tx_num_frags,
    sctp_count_frags_st  *rx_num_frags,
    sctp_error_t         *p_ecode )

{
    sctp_return_t ret    = SCTP_SUCCESS;
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_ep_st    *p_ep  = SCTP_NULL;

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;
        return ( SCTP_FAILURE );
    }

    if( assoc_id == SCTP_INVALID_ASSOC )
    {
        SCTPLOCK();
        ret = sctp_db_get_res_txrx_frags( tx_num_frags, rx_num_frags );
        SCTPUNLOCK();
        return ( SCTP_SUCCESS );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*initialize the error code*/
    *p_ecode = EINV_PARAMS;

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL == p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    else
    {
        ret = sctp_db_get_txrx_frags( p_ass, tx_num_frags, rx_num_frags );
    }

    SCTP_UNLOCK_EP( assoc_id );
    return ( ret );
}
/***************************************************************************
 *  FUNCTION :    sctp_api_config_cong_reg_def
 *
 *  DESCRIPTION:  This function configure congestion region definition for
 *  tranmist or Receive congestion notifications.
 *
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_config_cong_reg_def(
    sctp_U32bit          assoc_id,
    sctp_cong_reg_def_st cong_reg_def,
    sctp_Boolean_t       is_transmit,
    sctp_error_t         *p_ecode )
{
    sctp_return_t ret    = SCTP_SUCCESS;
    sctp_ep_st    *p_ep  = SCTP_NULL;
    sctp_tcb_st   *p_ass = SCTP_NULL;

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;
        return ( SCTP_FAILURE );
    }

    if( SCTP_FAILURE == sctp_db_verify_cong_reg_def( cong_reg_def, p_ecode ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        return ( SCTP_FAILURE );

    }

    if( assoc_id == SCTP_INVALID_ASSOC )
    {
        SCTPLOCK();

        if( is_transmit == SCTP_TRUE )
        {
            __MEMCOPY( ( sctp_U8bit * ) & ( sctp_db_main.trx_cong_reg_def ),
                       ( sctp_U8bit * )&cong_reg_def, sizeof( sctp_cong_reg_def_st ) );
        }

        else
        {
            __MEMCOPY( ( sctp_U8bit * ) & ( sctp_db_main.rcv_cong_reg_def ),
                       ( sctp_U8bit * )&cong_reg_def, sizeof( sctp_cong_reg_def_st ) );
        }

        SCTPUNLOCK();
        return ( SCTP_SUCCESS );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*initialize the error code*/
    *p_ecode = EINV_PARAMS;

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL == p_ass )
    {
        if( is_transmit == SCTP_TRUE )
        {
            __MEMCOPY( ( sctp_U8bit * ) & ( p_ep->trx_cong_reg_def ),
                       ( sctp_U8bit * )&cong_reg_def, sizeof( sctp_cong_reg_def_st ) );
        }

        else
        {
            __MEMCOPY( ( sctp_U8bit * ) & ( p_ep->rcv_cong_reg_def ),
                       ( sctp_U8bit * )&cong_reg_def, sizeof( sctp_cong_reg_def_st ) );
        }

    }

    else
    {
        if( is_transmit == SCTP_TRUE )
        {
            __MEMCOPY( ( sctp_U8bit * ) & ( p_ass->trx_cong_reg_def ),
                       ( sctp_U8bit * )&cong_reg_def, sizeof( sctp_cong_reg_def_st ) );
        }

        else
        {
            __MEMCOPY( ( sctp_U8bit * ) & ( p_ass->rcv_cong_reg_def ),
                       ( sctp_U8bit * )&cong_reg_def, sizeof( sctp_cong_reg_def_st ) );
        }

    }

    SCTP_UNLOCK_EP( assoc_id );
    return ( ret );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_get_cong_reg_def
 *
 *  DESCRIPTION:
 *    This api returns congestion region definition of transmit or receive
 *    queue.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_get_cong_reg_def(
    sctp_U32bit          assoc_id,
    sctp_cong_reg_def_st *p_cong_reg_def,
    sctp_Boolean_t       is_transmit,
    sctp_error_t         *p_ecode )
{
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_ep_st  *p_ep  = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_get_cong_reg_def" ) );

    /* SPR : 18527 */
    if( p_ecode == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        return ( SCTP_FAILURE );
    }

    if( p_cong_reg_def == SCTP_NULL )
        /* SPR : 18527 */
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API    ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( assoc_id == SCTP_INVALID_ASSOC )
    {
        SCTPLOCK();

        if( is_transmit == SCTP_TRUE )
        {
            __MEMCOPY( ( sctp_U8bit * )p_cong_reg_def,
                       ( sctp_U8bit * ) & ( sctp_db_main.trx_cong_reg_def ),
                       sizeof( sctp_cong_reg_def_st ) );
        }

        else
        {
            __MEMCOPY( ( sctp_U8bit * )p_cong_reg_def,
                       ( sctp_U8bit * ) & ( sctp_db_main.rcv_cong_reg_def ),
                       sizeof( sctp_cong_reg_def_st ) );
        }

        SCTPUNLOCK();
        return ( SCTP_SUCCESS );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( p_ass )
    {
        if( is_transmit == SCTP_TRUE )
        {
            __MEMCOPY( ( sctp_U8bit * )p_cong_reg_def,
                       ( sctp_U8bit * ) & ( p_ass->trx_cong_reg_def ),
                       sizeof( sctp_cong_reg_def_st ) );
        }

        else
        {
            __MEMCOPY( ( sctp_U8bit * )p_cong_reg_def,
                       ( sctp_U8bit * ) & ( p_ass->rcv_cong_reg_def ),
                       sizeof( sctp_cong_reg_def_st ) );
        }
    }

    else
    {
        if( is_transmit == SCTP_TRUE )
        {
            __MEMCOPY( ( sctp_U8bit * )p_cong_reg_def,
                       ( sctp_U8bit * ) & ( p_ep->trx_cong_reg_def ),
                       sizeof( sctp_cong_reg_def_st ) );
        }

        else
        {
            __MEMCOPY( ( sctp_U8bit * )p_cong_reg_def,
                       ( sctp_U8bit * ) & ( p_ep->rcv_cong_reg_def ),
                       sizeof( sctp_cong_reg_def_st ) );
        }
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );
    return ( SCTP_SUCCESS );
}



/***************************************************************************
 *  FUNCTION :    sctp_api_set_failure_threshold
 *
 *  DESCRIPTION:
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_set_failure_threshold(
    sctp_U32bit        assoc_id,
    sctp_sockaddr_st   *p_daddr,
    sctp_U32bit        path_max_retrans,
    sctp_error_t       *p_ecode )
{
    sctp_ep_st  *p_ep  = SCTP_NULL;
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_U32bit sctp_index;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_set_failure_threshold " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;
        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( SCTP_SOCK_IS_BOUND( p_ep->flags ) == 0 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Request in Wrong State" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 1 )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Request in Wrong State" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Request in Wrong State" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( !p_daddr )
            || ( sctp_db_validate_remote_addr( p_ass, p_daddr, &sctp_index )
                 == SCTP_FAILURE ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( path_max_retrans < p_ass->config.assoc_max_retrans )
    {
        p_ass->transmit.remote[sctp_index].path_error_threshold = path_max_retrans;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_SUCCESS );
    }

    *p_ecode = EINV_PARAMS;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_listen
 *
 *  DESCRIPTION:
 *    This api sets the socket in listening mode.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_listen(
    sctp_U32bit        assoc_id,
    sctp_U32bit        listen_back_log,
    sctp_error_t       *p_ecode )
{
    sctp_ep_st       *p_ep = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_listen" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, ( SCTP_INTF_TRC | SCTP_ERR_TRC ),
                    ( "Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;
        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* check whether socket is already connected. i.e. has a TCB */
    if( 0 != p_ep->n_assoc )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE,  "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* set the listener backlog and listen flag */
    if( listen_back_log > SCTP_MAX_LISTEN_BACKLOG )
    {
        listen_back_log = SCTP_MAX_LISTEN_BACKLOG;
    }

    /* check if socket is already a listner*/

    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {
        /*just set the back log */
        p_ep->l_backlog = listen_back_log;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_SUCCESS );
    }

    /* The Socket should be already bound to be made a listner */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Parameter ,new_assoc_id" ) );

        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }


    /* set the listener backlog and listen flag */
    p_ep->l_backlog = listen_back_log;

    /* Set the socket as listener */
    SCTP_SOCK_SET_LISTENER( p_ep->flags, SCTP_TRUE );

    /* Remove from Bind Hash Table and add to Listen Hash Table */
    sctp_db_del_hash( SCTP_BIND_HASH, p_ep );
    sctp_db_add_hash( SCTP_LISTEN_HASH, p_ep );

    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_accept_ext
 *
 *  DESCRIPTION:
 *    This api can be invoked as  blocked or unblocked call.If
 *    invoked as blocked call will block the calling process till a
 *    new connection arrives. If invoked as unblocked call will return
 *    a valid association id if one is pending else return error.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_accept_ext(
    sctp_U32bit             assoc_id,
    sctp_U32bit             ppi,
    sctp_U16bit             *p_num_ostreams,
    sctp_U16bit             *p_num_istreams,
    sctp_U32bit             *p_num_daddrs,
    sctp_sockaddr_list_st   list_of_dest_address,
    sctp_U16bit             *p_sctp_port,
    sctp_U32bit             *p_new_assoc_id,
    sctp_Boolean_t          block,
    sctp_error_t            *p_ecode )
{
    sctp_U32bit      counter;
    sctp_S32bit      nassoc_id;
    sctp_tcb_st      *p_new_tcb = SCTP_NULL;
    sctp_ep_st       *p_ep  = SCTP_NULL;
    sctp_ep_st       *p_new_ep = SCTP_NULL;
    sctp_sockaddr_st *addr_list = ( sctp_sockaddr_st * ) list_of_dest_address;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_accept_ext" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( " Service User API ERROR" ) );

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;
        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( !p_new_assoc_id )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module: Invalid Parameter ,new_assoc_id" ) );
        *p_ecode = EINV_PARAMS;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*
     * We can do an accept only on a socket that is a listener.
     */
    if( !SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE,  "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Request in Wrong State" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( block == SCTP_TRUE )
    {
        nassoc_id = sctp_accept_blocking( p_ep );
    }

    else
    {
        nassoc_id = sctp_accept_nonblocking( p_ep );
    }

    /* Check if the accept was successful. */
    if( 0 > nassoc_id )
    {
        /*CCR 1-628867*/
        switch( sk_errno )
        {
            case SK_ENOBUFS:
            case SK_EAGAIN:
                *p_ecode = E_NORESOURCES;
                break;

            default:
                *p_ecode = E_NO_PENDING_REQUEST;
                break;
        }

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    *p_ecode, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Cannot accept new connection " ) );

        /*CCR 1-628867*/
        /* Unlock the server socket */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* Unlock the server socket */
    SCTP_UNLOCK_EP( assoc_id );

    /* Lock the new association */
    SCTP_LOCK_EP( nassoc_id );

    /* Set the new association id */
    *p_new_assoc_id = nassoc_id;

    /* Get the pointer to the new endpoint and the new tcb in the endpoint */
    p_new_ep = sctp_db_get_ep_ptr( nassoc_id );

    if( !p_new_ep )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "accept_ext: Error getting new ep pointer" ) );

        SCTP_UNLOCK_EP( nassoc_id );

        *p_ecode = E_NO_PENDING_REQUEST;

        return ( SCTP_FAILURE );
    }

    /* Get the new TCB */
    p_new_tcb = sctp_db_get_assoc_from_ep( p_new_ep );

    if( !p_new_tcb )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "accept_ext: Error getting new tcb pointer" ) );

        SCTP_UNLOCK_EP( nassoc_id );

        *p_ecode = E_NO_PENDING_REQUEST;

        return ( SCTP_FAILURE );
    }

    /*
     * Change accordingly  value in out params is to be only in case of
     * blocking call.
     */
    /* Set the number of destination addresses */
    if( p_num_daddrs )
    {
        *p_num_daddrs = p_new_tcb->transmit.num_remote_address;
    }

    if( list_of_dest_address != NULL )
    {
        for( counter = 0; counter < p_new_tcb->transmit.num_remote_address;
                counter++ )
        {
            /* copy the whole address structure */
            addr_list[counter]
                = p_new_tcb->transmit.remote[counter].that_address;
        }
    }

    /* Get the sctp port */
    if( p_sctp_port )
    {
        *p_sctp_port = p_new_tcb->transmit.that_port;
    }

    /* ret stream parameters */
    if( p_num_istreams )
    {
        *p_num_istreams = p_new_tcb->receive.num_in_streams;
    }

    if( p_num_ostreams )
    {
        *p_num_ostreams = p_new_tcb->transmit.num_out_streams;
    }

    /* copy payload prot id, bug reported by gsheoran  */
    p_new_ep->ppi = ppi;

    /*
     * If there are no more assocs in the endpoint then we set is as not
     * readable.
     */
    if( p_ep->n_assoc <= 0 )
    {
        /* The Listener socket is no longer ready for read */
        SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );
    }

    for( counter = 0; counter < p_new_tcb->receive.num_in_streams; counter++ )
    {
        if( 0 != p_new_tcb->receive.stream[counter].nextssn_notify )
        {
            /*there is data penfding on new association*/
            sctp_start_timer(
                p_new_tcb,
                SCTPEVENT_TIMERAUTOCLOSE,
                10, /* 10 MS only just add in the list at front */
                0,
                &p_new_tcb->assoc_timer_id[SCTP_ASSOCTIMER_AUTOCLOSE],
                SCTP_NULL );
            break;
        }
    }

    /* Unlock the new endpoint */
    SCTP_UNLOCK_EP( nassoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_accept
 *
 *  DESCRIPTION: This function takes default values of input and output streams
 *               and calls sctp_api_accept_ext.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_accept(
    sctp_U32bit        assoc_id,
    sctp_U32bit        payload_prot_id,
    sctp_U32bit        *p_num_daddrs,
    sctp_sockaddr_list_st list_of_dest_addrs,
    sctp_U16bit        *p_sctp_port,
    sctp_U32bit        *p_new_assoc_id,
    sctp_Boolean_t     block,
    sctp_error_t       *p_ecode )
{
    sctp_U16bit num_out_streams = 0;
    sctp_U16bit num_in_streams  = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_accept" ) );

    return sctp_api_accept_ext( assoc_id,
                                payload_prot_id,
                                &num_out_streams,
                                &num_in_streams,
                                p_num_daddrs,
                                list_of_dest_addrs,
                                p_sctp_port,
                                p_new_assoc_id,
                                block,
                                p_ecode );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_connect_ext
 *
 *  DESCRIPTION:  This api is used for sctp connect.
 *
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_connect_ext(
    sctp_U32bit             association_id,
    sctp_U16bit             dest_sctp_port,
    sctp_U16bit             *p_num_ostreams,
    sctp_U16bit             *p_num_istreams,
    sctp_U32bit             *p_num_daddrs,
    sctp_sockaddr_list_st   list_of_dest_addrs,
    sctp_sockaddr_st        *p_connect_daddr,
    sctp_U32bit             payload_prot_id,
    sctp_Boolean_t          block,
    sctp_error_t            *p_ecode )
{
    sctp_U32bit      counter;
    sctp_U16bit      sctp_port;
    sctp_tcb_st      *p_tcb = SCTP_NULL;
    sctp_ep_st       *p_ep  = SCTP_NULL;
    sctp_sockaddr_st *addr_list = ( sctp_sockaddr_st * )list_of_dest_addrs;
    sctp_return_t    ret;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_connect_ext" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( !p_connect_daddr )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( association_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;

        return SCTP_FAILURE;
    }

    /*SPR 20396 Starts*/
    if( dest_sctp_port == 0 )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, E_INVALID_SCTP_PORT,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Port" ) );

        *p_ecode = E_INVALID_SCTP_PORT;

        return ( SCTP_FAILURE );
    }

    /*SPR 20396 Ends*/

    /* Lock the endpoint */
    SCTP_LOCK_EP( association_id );

    p_ep = sctp_db_get_ep_ptr( association_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( association_id );

        return SCTP_FAILURE;
    }


    /*
     * Connection from a server socket not allowed.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;

        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }


    /*
     * Check if the socket is bound. If not then bind the socket.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        /*
         * Here socket is not bound so first we have to bind it.
         * we select a unused port and copy it to ep .
         */
        ret  = sctp_db_get_free_sctpport( &sctp_port );

        if( ret == SCTP_FAILURE )
        {
            /* No free port*/
            *p_ecode = E_NORESOURCES;

            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        p_ep->this_port = sctp_port;

        /* Set the local addresses */
        if( !sctp_db_bind_ep_with_af_check( association_id, sctp_port, p_ecode ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        *p_ecode, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* Set the socket as bound */
        SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );
        sctp_db_add_hash( SCTP_BIND_HASH, p_ep );
    }

    /*SPR 5216*/
    /*moved address compatibilty check to sctp_db_addr_connectable*/
    /*SPR 5216*/
    /*
     * Check if a connection to the address would create any inconsistency.
     */
    if( sctp_db_addr_connectable( p_ep, dest_sctp_port, 1, p_connect_daddr,
                                  p_ecode ) == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ADDRS, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address is not Connectable" ) );

        *p_ecode = EINV_ADDRS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Check if the socket has already an association if yes then report error
     * because its TCP style socket so can have only one assoc atmost .
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL != p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SOcket Already connected" ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    /*SPR 6591*/

    if( ( p_num_ostreams )
            && ( *p_num_ostreams != 0 )
            && ( *p_num_ostreams <= sctp_db_main.max_streams_per_assoc ) )
    {
        p_ep->sk_config.num_out_streams = *p_num_ostreams;
    }


    if( ( p_num_istreams )
            && ( *p_num_istreams != 0 )
            && ( *p_num_istreams <= sctp_db_main.max_streams_per_assoc ) )
    {
        p_ep->sk_config.num_in_streams =  *p_num_istreams;
    }

    p_ep->ppi = payload_prot_id;




    /* Create a new TCB for the connection.*/
    p_tcb = sctp_db_create_tcb( p_ep, dest_sctp_port, 1, p_connect_daddr,
                                p_ecode );

    if( SCTP_NULL == p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_NORESOURCES, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Resources not available" ) );

        *p_ecode = E_NORESOURCES;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDCONNECT );

    if( block == SCTP_TRUE )
    {
        association_id = p_ep->ep_id;

        /* Sleep till we're successfully connected */
        SCTP_SLEEP( p_ep->cond, p_ep->lock );

        /* Get the ep ptr again */
        p_ep = sctp_db_get_ep_ptr( association_id );

        if( !p_ep )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_ASSOC_DELETED, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( ":Association deleted while connect call" ) );

            *p_ecode = E_ASSOC_DELETED;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* Get the association pointer */
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );

        if( !p_tcb )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_ASSOC_DELETED, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( ":Association deleted while connect call" ) );

            *p_ecode = E_ASSOC_DELETED;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* If the Assoc is not establsihed then error. */
        if( p_tcb->state != SCTPSTATE_ESTABLISHED )
        {
            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( association_id );

            /* delete the association and return it to the free pool */
            sctp_db_delete_association( p_tcb );

            return ( SCTP_FAILURE );
        }

        else
        {
            if( p_num_daddrs )
            {
                *p_num_daddrs = p_tcb->transmit.num_remote_address;
            }

            if( list_of_dest_addrs != SCTP_NULL )
            {
                for( counter = 0; counter < p_tcb->transmit.num_remote_address;
                        counter++ )
                {
                    /* copy the address structure */
                    addr_list[counter]
                        = p_tcb->transmit.remote[counter].that_address;
                }
            }

            /* ret stream parameters */
            if( p_num_istreams )
            {
                *p_num_istreams = p_tcb->receive.num_in_streams;
            }

            if( p_num_ostreams )
            {
                *p_num_ostreams = p_tcb->transmit.num_out_streams;
            }
        }
    } /* If Loop */


    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( association_id );

    return ( SCTP_SUCCESS );
}


/****************************************************************************
 *  FUNCTION :    sctp_api_connect
 *
 *  DESCRIPTION:
 *    This function takes default values of input and output streams
 *    and calls sctp_api_connect_ext.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_connect(
    sctp_U32bit             assoc_id,
    sctp_U16bit             dest_sctp_port,
    sctp_U32bit             *p_num_daddrs,
    sctp_sockaddr_list_st   list_of_dest_addrs,
    sctp_sockaddr_st        *p_connect_daddr,
    sctp_U32bit             payload_prot_id,
    sctp_Boolean_t          block,
    sctp_error_t            *p_ecode )
{
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_U16bit   num_out_streams = 0;
    sctp_U16bit   num_in_streams = 0;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_connect" ) );

    ret  = sctp_api_connect_ext( assoc_id,
                                 dest_sctp_port,
                                 &num_out_streams,
                                 &num_in_streams,
                                 p_num_daddrs,
                                 list_of_dest_addrs,
                                 p_connect_daddr,
                                 payload_prot_id,
                                 block,
                                 p_ecode );

    return ( ret );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_send_ext
 *
 *  DESCRIPTION:
 *    To send data on an association
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_send_ext(
    sctp_U32bit        assoc_id,
    sctp_U32bit        context,
    sctp_sockaddr_st   *p_daddr,
    sctp_U32bit        data_size,
    sctp_Pvoid_t       p_data,
    sctp_stream_t      stream_id,
    sctp_U32bit        life_time,
    sctp_Boolean_t     order_flag,
    sctp_Boolean_t     bundle_flag,
    sctp_U32bit        payload_prot_id,
    sctp_error_t       *p_ecode )
{
    sctp_tcb_st     *p_tcb = SCTP_NULL;
    sctp_ep_st      *p_ep  = SCTP_NULL;
    sctp_return_t   ret    = SCTP_SUCCESS;
    sctp_iovec_st   p_iovec[1];
#ifdef SCTP_DEQUEUE_IND
    sctp_U32bit     cong_level;
    sctp_Boolean_t  deque_ind = SCTP_FALSE;
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_send_ext" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    /*
     * Get the pointer to the association/Endpoint.
     */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        /* Get the association from the endpoint */
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );
        *p_ecode = EINV_ASSOCID;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

#ifdef SCTP_DEQUEUE_IND
    cong_level = p_tcb->transmit.congestion_event_sent;
#endif

    p_iovec->iov_len  = data_size;
    p_iovec->iov_base = p_data;

    if( 0 == sctp_send_general(
                p_tcb,
                p_daddr,
                1,
                p_iovec,
                stream_id,
                life_time,
                payload_prot_id,
                bundle_flag,
                order_flag,
                context,
                NULL,
                p_ecode ) )
    {
        ret = SCTP_FAILURE;
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

#ifdef SCTP_DEQUEUE_IND

    if( cong_level != p_tcb->transmit.congestion_event_sent )
    {
        deque_ind = SCTP_TRUE;
    }

    if( SCTP_TRUE == deque_ind )
    {
        while( SCTP_FAILURE != sctp_remove_notifications_from_queue() );
    }

#endif
    return ( ret );
}



/***************************************************************************
 *  FUNCTION :    sctp_api_send_ext2
 *
 *  DESCRIPTION:
 *    To send data on an association, for Integrated HSS UA/SCTP Solution
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_send_ext2(
    sctp_U32bit        assoc_id,
    sctp_U32bit        context,
    sctp_sockaddr_st   *p_daddr,
    sctp_U32bit        iovec_len,
    sctp_Pvoid_t       p_iovec,
    sctp_stream_t      stream_id,
    sctp_U32bit        life_time,
    sctp_Boolean_t     order_flag,
    sctp_Boolean_t     bundle_flag,
    sctp_U32bit        payload_prot_id,
    sctpbuffer_st      **p_p_tx_buf,
    sctp_error_t       *p_ecode )
{
    sctp_tcb_st     *p_tcb = SCTP_NULL;
    sctp_ep_st      *p_ep  = SCTP_NULL;
    sctp_return_t   ret    = SCTP_SUCCESS;
    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_send_ext" ) );

    if( !p_ecode || !p_p_tx_buf )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    /*
     * Get the pointer to the association/Endpoint.
     */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        /* Get the association from the endpoint */
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );
        *p_ecode = EINV_ASSOCID;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( 0 == sctp_send_general(
                p_tcb,
                p_daddr,
                iovec_len,
                p_iovec,
                stream_id,
                life_time,
                payload_prot_id,
                bundle_flag,
                order_flag,
                context,
                p_p_tx_buf,
                p_ecode ) )
    {
        ret = SCTP_FAILURE;
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( ret );
}


/**************************************************************************
 *    FUNCTION :    sctp_api_free_transmit_buffer
 *
 *    DESCRIPTION:
 *        This function returns buffer passed to the Tx-freelist.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_api_free_transmit_buffer(
    sctp_U32bit          assoc_id,
    sctpbuffer_st        *p_buf )
{
    sctp_ep_st  *p_ep = SCTP_NULL;
    sctp_tcb_st *p_tcb = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_free_transmit_buffer" ) );

    if( !p_buf )
    {
        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        SCTP_LOCK_EP( assoc_id );
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
        sctp_user_free_transmit_buffer( p_tcb, p_buf );
        SCTP_UNLOCK_EP( assoc_id );
    }

    else
    {
        SCTPLOCK();
        sctp_user_free_transmit_buffer( SCTP_NULL, p_buf );
        SCTPUNLOCK();
    }

    return ( SCTP_SUCCESS );
}



/**************************************************************************
 *    FUNCTION :    sctp_api_free_receive_buffer
 *
 *    DESCRIPTION:
 *        This function returns buffer passed to the Rx-freelist.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_api_free_receive_buffer(
    sctp_U32bit          assoc_id,
    sctpbuffer_st        *p_buf )
{
    sctp_ep_st  *p_ep = SCTP_NULL;
    sctp_tcb_st *p_tcb = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_free_receive_buffer" ) );

    if( !p_buf )
    {
        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        SCTP_LOCK_EP( assoc_id );
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
        sctp_user_free_receive_buffer( p_tcb, p_buf );
        SCTP_UNLOCK_EP( assoc_id );
    }

    else
    {
        SCTPLOCK();
        sctp_user_free_receive_buffer( SCTP_NULL, p_buf );
        SCTPUNLOCK();
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_send
 *
 *  DESCRIPTION:
 *    To send data on an association
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_send(
    sctp_U32bit       assoc_id,
    sctp_U32bit       context,
    sctp_U16bit       stream_id,
    sctp_sockaddr_st  *p_daddr,
    sctp_U32bit       data_size,
    sctp_Pvoid_t      p_data,
    sctp_error_t      *p_ecode )
{
    sctp_tcb_st   *p_tcb = SCTP_NULL;
    sctp_ep_st    *p_ep  = SCTP_NULL;
    sctp_return_t ret = SCTP_SUCCESS;
    sctp_iovec_st p_iovec[1];
#ifdef SCTP_DEQUEUE_IND
    sctp_U32bit     cong_level;
    sctp_Boolean_t  deque_ind = SCTP_FALSE;
#endif

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_send" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        *p_ecode = EINV_ASSOCID;
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    /*
     * Get the pointer to the endpoint/socket. Also check that the endpoint has
     * a association within it.
     */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_tcb )
    {
        *p_ecode = EINV_ASSOCID;
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

#ifdef SCTP_DEQUEUE_IND
    cong_level = p_tcb->transmit.congestion_event_sent;
#endif

    /*
     * Call sctp_api_send_ext to send data.
     */
    p_iovec->iov_len  = data_size;
    p_iovec->iov_base = p_data;

    if( 0 == sctp_send_general(
                p_tcb,
                p_daddr,
                1,
                p_iovec,
                stream_id,
                SCTP_MAX_LIFE_TIME,
                p_ep->ppi,
                SCTP_TRUE,
                SCTP_TRUE,
                context,
                NULL,
                p_ecode ) )
    {
        ret = SCTP_FAILURE;
    }


    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );
#ifdef SCTP_DEQUEUE_IND

    if( cong_level != p_tcb->transmit.congestion_event_sent )
    {
        deque_ind = SCTP_TRUE;
    }

    if( SCTP_TRUE == deque_ind )
    {
        while( SCTP_FAILURE != sctp_remove_notifications_from_queue() );
    }

#endif

    return ( ret );
}







/***************************************************************************
 *  FUNCTION :    sctp_api_send_iovec
 *
 *  DESCRIPTION:
 *               To send  data(iovec structure) on an association
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_send_iovec(
    sctp_U32bit         assoc_id,
    sctp_U32bit         context,
    sctp_sockaddr_st    *p_daddr,
    sctp_U32bit         iovec_len,
    sctp_iovec_st       *p_iovec,
    sctp_stream_t       stream_id,
    sctp_U32bit         life_time,
    sctp_Boolean_t      order_flag,
    sctp_Boolean_t      bundle_flag,
    sctp_U32bit         payload_prot_id,
    sctp_error_t        *p_ecode )
{
    sctp_tcb_st     *p_tcb = SCTP_NULL;
    sctp_ep_st      *p_ep  = SCTP_NULL;
    sctp_return_t   ret    = SCTP_SUCCESS;
#ifdef SCTP_DEQUEUE_IND
    sctp_U32bit     cong_level;
    sctp_Boolean_t  deque_ind = SCTP_FALSE;
#endif


    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_send_iovec" ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    /*
     * Get the pointer to the association/Endpoint.
     */
    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep )
    {
        /* Get the association from the endpoint */
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Association " ) );
        *p_ecode = EINV_ASSOCID;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

#ifdef SCTP_DEQUEUE_IND
    cong_level = p_tcb->transmit.congestion_event_sent;
#endif


    if( 0 == sctp_send_general(
                p_tcb,
                p_daddr,
                iovec_len,
                p_iovec,
                stream_id,
                life_time,
                payload_prot_id,
                bundle_flag,
                order_flag,
                context,
                NULL,
                p_ecode ) )
    {
        ret = SCTP_FAILURE;
    }


    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

#ifdef SCTP_DEQUEUE_IND

    if( cong_level != p_tcb->transmit.congestion_event_sent )
    {
        deque_ind = SCTP_TRUE;
    }

    if( SCTP_TRUE == deque_ind )
    {
        while( SCTP_FAILURE != sctp_remove_notifications_from_queue() );
    }

#endif

    return ( ret );
}




/***************************************************************************
 *  FUNCTION :   sctp_api_receive
 *
 *  DESCRIPTION:
 *    This API copies data from sctp buffer to application specified buffer and
 *    sctp buffer is freed. It assumes peek flag value of false.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_receive(
    sctp_U32bit        assoc_id,
    sctp_U32bit        *p_ppi,
    sctp_U32bit        *p_data_size,
    sctp_U16bit        *p_stream_id,
    sctp_U8bit         *p_data,
    sctp_Boolean_t     block,
    sctp_error_t       *p_ecode )
{
    sctp_tcb_st    *p_ass = SCTP_NULL;
    sctp_ep_st     *p_ep = SCTP_NULL;
    sctp_U32bit    msg_len;
    sctp_U32bit    counter;
    sctp_U32bit    lessbuf_datasize = 0;
    sctp_return_t  ret;
    sctp_Boolean_t less_buff_flag = SCTP_FALSE;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_receive " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association " ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( ( p_ep ) && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
    {
        p_ass = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( ( p_ep == SCTP_NULL ) || ( p_ass == SCTP_NULL ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_CLOSED )
            || ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SU Module:API Requested in Invalid State " ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( !p_ppi ) || ( !p_data_size ) || ( !p_data ) || ( !p_stream_id ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Input Parameters " ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    for( ; ; )
    {
        for( counter = 0; counter < p_ass->receive.num_in_streams; counter++ )
        {
            ret = sctp_read_message_list( p_ass,
                                          ( sctp_U16bit )counter,
                                          *p_data_size,
                                          p_data,
                                          &msg_len,
                                          p_ppi,
                                          SCTP_FALSE,
                                          p_ecode );

            /* Variable nextssn is incremented by above called function */
            if( ret == SCTP_SUCCESS )
            {
                *p_data_size = ( sctp_U32bit ) msg_len;
                *p_stream_id = ( sctp_U16bit )counter;

                /*
                 * Check if the if any partial indication has been given
                 * data being delivered previously. If yes, then clear it...
                 */

                if( ( p_ass->partial_data.partial_data_pending == 1 ) &&
                        ( p_ass->partial_data.stream == *p_stream_id ) &&
                        ( p_ass->partial_data.ssn ==
                          p_ass->receive.stream[*p_stream_id].nextssn - 1 ) )
                {
                    p_ass->partial_data.partial_data_pending = 0;
                }

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_SUCCESS );
            }

            else if( *p_ecode == E_BUFFER_TOO_SMALL )
            {
                less_buff_flag = SCTP_TRUE;
                lessbuf_datasize = msg_len;
            }
        }

        if( block == SCTP_TRUE )
        {
            if( SCTP_TRUE == less_buff_flag )
            {
                *p_data_size = lessbuf_datasize;
                *p_ecode = E_BUFFER_TOO_SMALL;
                SCTP_UNLOCK_EP( assoc_id );
                return ( SCTP_FAILURE );
            }

            /* Sleep till we have something to read */
            SCTP_SLEEP( p_ep->cond, p_ep->lock );

            /* Get the ptr again */
            p_ep = sctp_db_get_ep_ptr( assoc_id );

            if( ( p_ep ) && ( !SCTP_SOCK_IS_LISTENER( p_ep->flags ) ) )
            {
                p_ass = sctp_db_get_assoc_from_ep( p_ep );
            }

            if( ( !p_ep ) || ( !p_ass ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_ASSOCID, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Association deleted." ) );

                *p_ecode = E_ASSOC_DELETED;

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            if( ( p_ass->state  == SCTPSTATE_CLOSED )
                    || ( p_ass->state == SCTPSTATE_COOKIEWAIT )
                    || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,  1 );
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:API Requested in Invalid State " ) );

                *p_ecode = E_REQ_IN_WRONG_STATE;

                /*
                 * Since we do not have anything to read set the readable flag
                 * to false
                 */
                SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return SCTP_FAILURE;
            }
        }

        else
        {
            break;
        }
    }

    if( SCTP_TRUE == less_buff_flag )
    {
        *p_data_size = lessbuf_datasize;
        *p_ecode = E_BUFFER_TOO_SMALL;
    }

    else
    {
        *p_ecode = E_NOTHING_IN_QUEUE;
    }

    /* Since we do not have anything to read set the readable flag to false */
    SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_NULL );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_receive_ext
 *
 *  DESCRIPTION:
 *    This API copies data from sctp buffer to application specified buffer
 *  and sctp buffer is freed.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_receive_ext(
    sctp_U32bit        assoc_id,
    sctp_U16bit        stream_id,
    sctp_sockaddr_st   *p_dest_address,
    sctp_U32bit        *p_ppi,
    sctp_U32bit        *p_data_size,
    sctp_U8bit         *p_data,
    sctp_Boolean_t     block,
    sctp_Boolean_t     peek_flag,
    sctp_error_t       *p_ecode )
{
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_ep_st    *p_ep  = SCTP_NULL;
    sctp_return_t ret;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_dest_address );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_api_receive_ext " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, ( SCTP_INTF_TRC | SCTP_ERR_TRC ),
                    ( "Service User API ERROR" ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( ( p_ep != SCTP_NULL ) && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
    {
        p_ass = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_CLOSED )
            || ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:API Requested in Invalid State " ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( !p_ppi ) || ( !p_data_size )
            || ( *p_data_size == 0 ) || ( !p_data )
            || ( stream_id >= p_ass->receive.num_in_streams ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Input Parameters" ) );
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    for( ; ; )
    {
        ret = sctp_read_message_list( p_ass,
                                      stream_id,
                                      *p_data_size,
                                      p_data,
                                      p_data_size,
                                      p_ppi,
                                      peek_flag,
                                      p_ecode );


        if( ret == SCTP_SUCCESS )
        {
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_SUCCESS );
        }


        /* block if requested by SU */
        if( ( block == SCTP_TRUE ) && ( *p_ecode != E_BUFFER_TOO_SMALL ) )
        {
            /* Sleep till we have something to read */
            SCTP_SLEEP( p_ep->cond, p_ep->lock );

            /* Get the endpoint pointer again */
            p_ep = sctp_db_get_ep_ptr( assoc_id );

            if( ( p_ep != SCTP_NULL )
                    && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
            {
                p_ass = sctp_db_get_assoc_from_ep( p_ep );
            }

            if( ( !p_ep ) || ( !p_ass ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_ASSOCID, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:Association deleted" ) );

                *p_ecode = E_ASSOC_DELETED;

                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            if( ( p_ass->state == SCTPSTATE_CLOSED )
                    || ( p_ass->state  == SCTPSTATE_COOKIEWAIT )
                    || ( p_ass->state  == SCTPSTATE_COOKIESENT ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:API Requested in Invalid State " ) );
                *p_ecode = E_REQ_IN_WRONG_STATE;

                /*
                 * Since we do not have anything to read set the readable flag
                 * to false.
                 */
                SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }
        }

        else
        {
            break;
        }
    }

    if( E_BUFFER_TOO_SMALL != *p_ecode )
    {
        *p_ecode = E_NOTHING_IN_QUEUE;
    }

    /* Since we do not have anything to read set the readable flag to false */
    SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}




/***************************************************************************
 *  FUNCTION :   sctp_api_receive_ext2
 *
 *  DESCRIPTION:
 *    This API copies data from sctp buffer to application specified buffer
 *  and sctp buffer is freed.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_receive_ext2(
    sctp_U32bit        assoc_id,
    sctp_U16bit        stream_id,
    sctp_sockaddr_st   *p_dest_address,
    sctp_U32bit        *p_ppi,
    sctp_U32bit        *p_data_size,
    sctp_U8bit         *p_data,
    sctp_Boolean_t     block,
    sctp_Boolean_t     peek_flag,
    sctpbuffer_st      **p_p_buffer,    /*output buffer, */
    sctp_error_t       *p_ecode )
{
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_ep_st    *p_ep  = SCTP_NULL;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_dest_address );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_api_receive_ext2 " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, ( SCTP_INTF_TRC | SCTP_ERR_TRC ),
                    ( "Service User API ERROR" ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( ( p_ep != SCTP_NULL ) && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
    {
        p_ass = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_CLOSED )
            || ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:API Requested in Invalid State " ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( !p_ppi ) || ( !p_data_size ) || ( !p_p_buffer )
            || ( *p_data_size == 0 ) || ( !p_data )
            || ( stream_id >= p_ass->receive.num_in_streams ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Input Parameters" ) );
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    for( ; ; )
    {
        sctpbuffer_st   *p_buf = SCTP_NULL;
        p_buf = sctp_check_data_arrival( p_ass, stream_id, p_data_size );

        if( SCTP_NULL != p_buf )
        {

            if( SCTP_TRUE == p_buf->end )
            {
                /*just give the pointer to this buffer, no copy*/
                *p_p_buffer  = p_buf;
                *p_data_size = p_buf->size;
                *p_ppi       = p_buf->ppi;

                if( SCTP_TRUE != peek_flag )
                {
                    /* Update nextssn parameter */
                    if( p_buf->unordered == 0 )
                    {
                        p_ass->receive.stream[stream_id].nextssn++;
                    }

                    lstDelete( &p_ass->receive.stream[stream_id].receive_queue,
                               ( NODE * ) p_buf );
                    /*no need to check for congestion level, not freeing the buffer*/
                }

                SCTP_UNLOCK_EP( assoc_id );
                return ( SCTP_SUCCESS );
            }

            else
            {
                *p_p_buffer = SCTP_NULL;

                /*do a copy */
                if( SCTP_SUCCESS == sctp_read_message_list( p_ass, stream_id,
                                                            *p_data_size, p_data, p_data_size, p_ppi,
                                                            peek_flag, p_ecode ) )
                {
                    SCTP_UNLOCK_EP( assoc_id );
                    return ( SCTP_SUCCESS );
                }
            }
        }

        /* block if requested by SU */
        if( block == SCTP_TRUE )
        {
            /* Sleep till we have something to read */
            SCTP_SLEEP( p_ep->cond, p_ep->lock );

            /* Get the endpoint pointer again */
            p_ep = sctp_db_get_ep_ptr( assoc_id );

            if( ( p_ep != SCTP_NULL )
                    && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
            {
                p_ass = sctp_db_get_assoc_from_ep( p_ep );
            }

            if( ( !p_ep ) || ( !p_ass ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_ASSOCID, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:Association deleted" ) );

                *p_ecode = E_ASSOC_DELETED;

                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            if( ( p_ass->state == SCTPSTATE_CLOSED )
                    || ( p_ass->state  == SCTPSTATE_COOKIEWAIT )
                    || ( p_ass->state  == SCTPSTATE_COOKIESENT ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:API Requested in Invalid State " ) );
                *p_ecode = E_REQ_IN_WRONG_STATE;

                /*
                 * Since we do not have anything to read set the readable flag
                 * to false.
                 */
                SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }
        }

        else
        {
            break;
        }
    }

    *p_ecode = E_NOTHING_IN_QUEUE;

    /* Since we do not have anything to read set the readable flag to false */
    SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}



/***************************************************************************
 *  FUNCTION :  sctp_api_receive_partial_data
 *
 *  DESCRIPTION:
 *    This API copies data from sctp buffer to application specified buffer and
 *    sctp buffer is freed.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t  sctp_api_receive_partial_data(
    sctp_U32bit    assoc_id,
    sctp_U16bit    stream_id,
    sctp_U32bit    *p_ppi,
    sctp_U32bit    *p_data_size,
    sctp_U8bit     *p_data,
    sctp_U16bit    *p_ssn,
    sctp_U16bit    *p_delivery_num,
    sctp_Boolean_t peek_flag,
    sctp_error_t   *p_ecode )
{
    /* peek flag to be implemented */
    /* ordering to be added also */
    sctp_tcb_st    *p_ass = SCTP_NULL;
    sctp_ep_st     *p_ep = SCTP_NULL;
    sctpbuffer_st  *sctp_buf, *save;
    sctp_U32bit    end_segment = 0;
    sctp_U32bit    copy_len;
    sctp_U32bit    msg_len;
    sctp_U32bit    total_msg_size;
    sctp_U32bit    expected_tsn;
    sctp_U32bit    buffer_len;
    sctp_U32bit    remain_buffer_len;
    sctp_U32bit    old_cong_reg, new_cong_reg;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_receive_partial_data " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( ( p_ep ) && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
    {
        p_ass = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( !p_data_size )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Input Parameters" ) );

        *p_ecode = EINV_PARAMS;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_CLOSED )
            || ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT )
            || ( p_ass->partial_data.partial_data_pending == 0 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:API Requested in Invalid State " ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( !p_ppi ) || ( !p_data_size )
            || ( *p_data_size == 0 ) || ( !p_data )
            || ( stream_id >= p_ass->receive.num_in_streams )
            || ( stream_id != p_ass->partial_data.stream )
            || ( !p_ssn ) || ( !p_delivery_num ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service    User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Input Parameters " ) );

        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* Added by Vakul */
    if( ( p_ass->receive.stream[stream_id].nextssn != p_ass->partial_data.ssn )
            && ( p_ass->partial_data.unordered == 0 ) )
    {
        /*
         * Check that this partial receive api is not invoked out of order
         * i.e. it is not invoked if some normal full data is pending before
         * the partial data
         */

        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:API Requested in Invalid State" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    buffer_len = *p_data_size;
    remain_buffer_len = buffer_len;
    msg_len = 0;
    total_msg_size = 0;

    sctp_buf = ( sctpbuffer_st * )
               lstFirst( &p_ass->receive.stream[stream_id].receive_queue );

    for( ; sctp_buf != SCTP_NULL;
            sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf ) )
    {
        if( !( ( ( sctp_buf->ssn == p_ass->partial_data.ssn )
                 && ( sctp_buf->unordered == 0 ) )
                || ( ( sctp_buf->tsn == p_ass->partial_data.expected_tsn )
                     && ( sctp_buf->unordered == 1 ) ) ) )
        {
            continue;
        }

        save = sctp_buf; /*Save the location of starting buffer*/

        *p_ssn = p_ass->partial_data.ssn;
        *p_ppi = sctp_buf->ppi;

        end_segment = 0;
        expected_tsn = sctp_buf->tsn;

        /* Upto here we have got the required receive
           buffer from which the partial data starts */

        /* Check if the buffer size supplied is less than total size of partial
           data and peek flag is true, then return only the size data */

        if( peek_flag == SCTP_TRUE ) /*Added by Vakul*/
        {
            /* Counting the total number of bytes of partial data */
            for( ; ( sctp_buf != NULL ); )
            {
                /* look for all chunks with expected tsn */
                if( sctp_buf->tsn != expected_tsn )
                {
                    break;
                }

                total_msg_size += sctp_buf->size;;
                expected_tsn++;

                if( sctp_buf->end == 1 )
                {
                    end_segment = 1;
                    break;
                }

                sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf );
            }

            if( total_msg_size > buffer_len )
            {
                *p_data_size = total_msg_size;
                *p_ecode = E_BUFFER_TOO_SMALL;

                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            else
            {
                /*Buffer size is appropriate. Copy the partial data in buffer*/
                /*Restore the location of starting buffer*/
                sctp_buf = save;
                end_segment = 0;
                expected_tsn = sctp_buf->tsn;
            }
        }

        if( sctp_buf->begin == 1 )
        {
            *p_delivery_num = 1;
            p_ass->partial_data.delivery_number = 1;
        }

        else
        {
            p_ass->partial_data.delivery_number++;
            *p_delivery_num = ( sctp_U16bit )p_ass->partial_data.delivery_number;
        }

        for( ; ( sctp_buf != SCTP_NULL ); )
        {
            /* look for all chunks with expected tsn */
            if( sctp_buf->tsn != expected_tsn )
            {
                break;
            }

            save = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf );

            if( remain_buffer_len > 0 )
            {
                if( remain_buffer_len > ( sctp_U32bit )sctp_buf->size )
                {
                    copy_len = sctp_buf->size;
                }

                else
                {
                    copy_len = remain_buffer_len;
                }
            }

            else /*No space left in buffer. Drop rest of partial data*/
            {
                copy_len = 0;    /*Copy 0 bytes*/
            }

            __MEMCOPY( p_data + msg_len, &sctp_buf->buffer[0], copy_len );

            msg_len += copy_len;

            /*Length of buffer remaining*/
            remain_buffer_len = buffer_len - msg_len;

            lstDelete( &p_ass->receive.stream[stream_id].receive_queue,
                       ( NODE * )sctp_buf );

            sctp_free_receive_buffer( p_ass, sctp_buf );
            expected_tsn++;

            if( sctp_buf->end == 1 )
            {
                end_segment = 1;
                /*
                 * Indicate that this is last part of whole of the message
                 * being delivered by partial receive api by setting
                 */
                p_ass->partial_data.delivery_number = 0;
                *p_delivery_num = 0;

                break;
            }

            sctp_buf = save;
        } /* for loop */

        if( end_segment != 1 )
        {
            /*
             * This is for the case when we get end segment, partial data will
             * be invoked again
             */
            p_ass->partial_data.data_assembly_in_progress = 1;
            p_ass->partial_data.expected_tsn = expected_tsn;
        }

        else  /*Added by Vakul*/
        {
            /* To clear previously set value */
            p_ass->partial_data.data_assembly_in_progress = 0;

            /* Increment the nextssn value... vakul */
            if( p_ass->partial_data.unordered == 0 )
            {
                p_ass->receive.stream[stream_id].nextssn++;
            }
        }

        *p_data_size = msg_len;
        p_ass->partial_data.partial_data_pending = 0;

        /* Check whether the receiving window has grown enough to
           give a no congestion notification to SU */

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

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_SUCCESS );
    }

    *p_ecode = E_NOTHING_IN_QUEUE;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :   sctp_api_recv_mesg
 *
 *  DESCRIPTION:
 *    This API copies data from sctp buffer to application specified buffer
 *  and sctp buffer is freed. This provides some extra features over other
 *  APIs. If the user specified buffer is less than the size of message, then
 *  the only the sizeof buffer passed is copied and rest of the buffer can be
 *  read in subsequent read calls. If the flag SCTP_FLUSH_
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_recv_mesg(
    sctp_assoc_t                assoc_id,
    sctp_U32bit                 *p_ppi,
    sctp_api_recv_struct_st     *p_recv_struct,
    sctp_U8bit                  *p_data,
    sctp_U32bit                 *p_data_size,
    sctp_error_t                *p_ecode )
{
    sctp_tcb_st         *p_ass = SCTP_NULL;
    sctp_ep_st          *p_ep  = SCTP_NULL;
    sctp_U32bit         stream_id = 0;
    sctp_U32bit         actual_mesg_size = 0;
    sctp_U32bit         flags = 0;
    sctp_sockaddr_st    *p_daddr = SCTP_NULL;
    sctp_Boolean_t      b_mesg_unordered = SCTP_FALSE;
    sctp_Boolean_t      b_partial_data_delv = SCTP_FALSE;
    sctp_Boolean_t      b_block = SCTP_FALSE;
    sctp_return_t       retval = SCTP_FAILURE;

    SCTP_TRACE( p_ep, SCTP_INTF_TRC, ( "sctp_api_recv_mesg " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, ( SCTP_INTF_TRC | SCTP_ERR_TRC ),
                    ( "Service User API ERROR" ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( ( p_ep != SCTP_NULL ) && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
    {
        p_ass = sctp_db_get_assoc_from_ep( p_ep );
    }

    if( !p_ep || !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_CLOSED )
            || ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:API Requested in Invalid State " ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( !p_ppi ) || ( !p_data ) || ( !p_data_size ) || ( *p_data_size == 0 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Input Parameters" ) );
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( SCTP_NULL != p_recv_struct )
    {
        if( p_recv_struct->recv_struct_flag & SCTP_STREAMID_PRESENT )
        {
            if( p_recv_struct->stream_id >= p_ass->receive.num_in_streams )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_PARAMS, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "Invalid Input Parameters" ) );
                *p_ecode = EINV_PARAMS;

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            stream_id = p_recv_struct->stream_id;
        }

        flags = p_recv_struct->recv_opr_flags;
        p_daddr = &( p_recv_struct->dest_address );
        p_recv_struct->recv_struct_flag |= SCTP_DEST_ADDR_PRESENT;

        if( p_recv_struct->recv_opr_flags & SCTP_BLOCKING_RECV )
        {
            b_block = SCTP_TRUE;
        }
    }

    /* changes for klocwork fix for IBM delivery  */
    if( SCTP_NULL == p_daddr )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_PARAMS, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Invalid Input Parameter" ) );
        *p_ecode = EINV_PARAMS;

        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* changes for klocwork fix for IBM delivery  */

    for( ; ; )
    {
        /* SPR : 18527 */
        if( SCTP_NULL != p_recv_struct && p_recv_struct->recv_struct_flag & SCTP_STREAMID_PRESENT )
            /* SPR : 18527 */
        {
            retval = sctp_read_data_mesg( p_ass, ( sctp_U16bit )stream_id, *p_data_size,
                                          p_data, &actual_mesg_size, p_ppi, flags, p_daddr, &b_mesg_unordered,
                                          &b_partial_data_delv, p_ecode );
        }

        else
        {
            for( stream_id = 0; stream_id < p_ass->receive.num_in_streams; stream_id++ )
            {
                retval = sctp_read_data_mesg( p_ass, ( sctp_U16bit )stream_id, *p_data_size,
                                              p_data, &actual_mesg_size, p_ppi, flags, p_daddr, &b_mesg_unordered,
                                              &b_partial_data_delv, p_ecode );

                if( retval == SCTP_SUCCESS )
                {
                    break;
                }
            }
        }

        /* Variable nextssn is incremented by above called function */
        if( retval == SCTP_SUCCESS )
        {
            if( SCTP_NULL != p_recv_struct )
            {
                p_recv_struct->stream_id = stream_id;
                p_recv_struct->recv_struct_flag |= SCTP_STREAMID_PRESENT;

                if( SCTP_TRUE == b_partial_data_delv )
                {
                    p_recv_struct->recv_struct_flag |= SCTP_REM_LENGTH_PRESENT;
                    p_recv_struct->rem_length = ( actual_mesg_size - *p_data_size );
                }
            }

            if( actual_mesg_size < *p_data_size )
            {
                *p_data_size = ( sctp_U32bit ) actual_mesg_size;
            }

            /*
             * Check if the if any partial indication has been given
             * data being delivered previously. If yes, then clear it...
             */

            if( ( p_ass->partial_data.partial_data_pending == 1 ) &&
                    ( p_ass->partial_data.stream == stream_id ) &&
                    ( p_ass->partial_data.ssn ==
                      p_ass->receive.stream[stream_id].nextssn - 1 ) )
            {
                p_ass->partial_data.partial_data_pending = 0;
            }

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_SUCCESS );
        }

        /* block if requested by SU */
        if( b_block == SCTP_TRUE )
        {
            /* Sleep till we have something to read */
            SCTP_SLEEP( p_ep->cond, p_ep->lock );

            /* Get the endpoint pointer again */
            p_ep = sctp_db_get_ep_ptr( assoc_id );

            if( ( p_ep != SCTP_NULL )
                    && ( SCTP_SOCK_IS_LISTENER( p_ep->flags ) == 0 ) )
            {
                p_ass = sctp_db_get_assoc_from_ep( p_ep );
            }

            if( ( !p_ep ) || ( !p_ass ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            EINV_ASSOCID, "Service User API ERROR" );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:Association deleted" ) );

                *p_ecode = E_ASSOC_DELETED;

                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            if( ( p_ass->state == SCTPSTATE_CLOSED )
                    || ( p_ass->state  == SCTPSTATE_COOKIEWAIT )
                    || ( p_ass->state  == SCTPSTATE_COOKIESENT ) )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SU Module:API Requested in Invalid State " ) );
                *p_ecode = E_REQ_IN_WRONG_STATE;

                /*
                 * Since we do not have anything to read set the readable flag
                 * to false.
                 */
                SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }
        }

        else
        {
            break;
        }
    }

    *p_ecode = E_NOTHING_IN_QUEUE;

    /* Since we do not have anything to read set the readable flag to false */
    SCTP_SOCK_SET_READABLE( p_ep->flags, SCTP_FALSE );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}

/***************************************************************************
 *  FUNCTION :    sctp_api_heartbeat_action
 *
 *  DESCRIPTION:
 *    This function enables, disables or request heartbeat on a association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_heartbeat_action(
    sctp_U32bit         assoc_id,
    sctp_sockaddr_st    *p_daddr,
    sctp_HB_action_et   heartbeat_action,
    sctp_U32bit         heartbeat_interval,
    sctp_error_t        *p_ecode )
{
    sctp_U32bit sctp_index;
    sctp_U32bit counter;
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_ep_st  *p_ep  = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_HB_action " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = E_REQ_IN_WRONG_STATE;
        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*SPR 21127 changes */
    if( ( ( SCTP_NULL != p_daddr )
            && ( ( sctp_return_t )sctp_db_is_af_correct( p_ep, 1, p_daddr ) == SCTP_FAILURE ) )
            || ( ( SCTP_HB_ENABLE != heartbeat_action )
                 && ( SCTP_HB_DISABLE != heartbeat_action )
                 && ( SCTP_HB_REQUEST != heartbeat_action ) ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( ( SCTP_NULL == p_ass )
            || ( SCTPSTATE_ESTABLISHED != p_ass->state ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( SCTP_NULL != p_daddr ) && ( sctp_db_validate_remote_addr( p_ass, p_daddr,         &sctp_index ) == SCTP_FAILURE ) )
    {
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Parameters " ) );
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( heartbeat_action == SCTP_HB_ENABLE )
    {
        /* heartbeat interval expected from user in second */
        if( heartbeat_interval != 0 )
        {
            if( ( heartbeat_interval * 1000 <= SCTP_MAX_HB_INTERVAL )
                    && ( heartbeat_interval * 1000 >= SCTP_MIN_HB_INTERVAL ) )
            {
                p_ass->config.hb_interval = heartbeat_interval * 1000;
            }

            else
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                            ( "SCTP TRACE::SU Module:Invalid Parameters " ) );
                *p_ecode = EINV_HB_INTERVAL_PARAMS;
                SCTP_UNLOCK_EP( assoc_id );
                return ( SCTP_FAILURE );
            }
        }

        if( p_daddr )
        {
            p_ass->transmit.remote[sctp_index].allow_heartbeat = 1;

            /* Update last used parameter */
            __GETSYSTIME( &p_ass->transmit.remote[sctp_index].last_used_time );
        }

        else
        {
            for( counter = 0; counter < p_ass->transmit.num_remote_address;
                    counter++ )
            {
                p_ass->transmit.remote[counter].allow_heartbeat = 1;

                /* Update last used parameter */
                __GETSYSTIME( &p_ass->transmit.remote[counter].last_used_time );
            }

            sctp_index = 0;
        }

        /* start the Inactivity timer if not running  */
        if( ( p_ass->state == SCTPSTATE_ESTABLISHED )
                && p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY]
                == SCTP_NULL_TIMER_ID )
        {
            sctp_start_timer(
                p_ass,
                SCTPEVENT_TIMERINACTIVE,
                p_ass->config.hb_interval + p_ass->transmit.remote[sctp_index].rto,
                sctp_index,
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY], SCTP_NULL );
        }
    }

    else if( heartbeat_action == SCTP_HB_DISABLE )
    {
        if( p_daddr )
        {
            p_ass->transmit.remote[sctp_index].allow_heartbeat = SCTP_FALSE;
        }

        else
        {
            for( counter = 0; counter < p_ass->transmit.num_remote_address;
                    counter++ )
            {
                p_ass->transmit.remote[counter].allow_heartbeat = SCTP_FALSE;
            }
        }

        /* If heartbeat is disabled on all addresses
           ,stop heartbeat and inactivity timer  */
        for( counter = 0; counter < p_ass->transmit.num_remote_address;
                counter++ )
        {
            if( p_ass->transmit.remote[counter].allow_heartbeat == 1 )
            {
                break;
            }
        }

        if( counter == p_ass->transmit.num_remote_address )
        {
            sctp_stop_timer(
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
                SCTP_NULL, SCTP_NULL );

            /*Stop the Inactivity timer*/
            sctp_stop_timer(
                &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_INACTIVITY],
                SCTP_NULL, SCTP_NULL );
        }
    } /* heartbeat request */

    else if( heartbeat_action == SCTP_HB_REQUEST )
    {
        sctp_addrinfo_st   to;

        /* If Null is passed as addeess, send heartbeat on primary address  */
        if( !p_daddr )
        {
            sctp_index = p_ass->transmit.primary_address_index;
        }

        /*Changes for CSR#1-4267892*/
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Path status is  %d, index = %d",
                      p_ass->transmit.remote[sctp_index].status, sctp_index ) );
        /*Changes for CSR#1-4267892 Ends*/


        to.checksum   = p_ass->config.checksum_func;
        to.p_sockaddr = &p_ass->transmit.remote[sctp_index].that_address;
        sctp_send_chunk_heartbeat( p_ass,
                                   &p_ep->this_address[0],
                                   p_ep->this_port,
                                   p_ass->transmit.that_port,
                                   sctp_index, p_ass->remote_tag );

        /* SPR 21127 changes start*/
        SCTP_UNUSED_VARIABLE( to );
        /* SPR 21127 changes end*/

        sctp_stop_timer(
            &p_ass->assoc_timer_id[SCTP_ASSOCTIMER_HBACK],
            SCTP_NULL, SCTP_NULL );
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_set_primary
 *
 *  DESCRIPTION:
 *    This api set primary address for an association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_set_primary(
    sctp_U32bit         assoc_id,
    sctp_sockaddr_st    *p_saddr,
    sctp_sockaddr_st    *p_daddr,
    sctp_error_t        *p_ecode )
{
    /* source address to be supported */
    sctp_U32bit sctp_index;
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_ep_st  *p_ep  = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_set_primary " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( !( p_saddr || p_daddr ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;


        return ( SCTP_FAILURE );
    }


    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }


    if( p_daddr && !( SCTP_IS_INADDR_ANY( p_daddr[0] ) ) )
    {
        if( SCTP_FAILURE == sctp_db_validate_remote_addr( p_ass, p_daddr,
                                                          &sctp_index ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                        "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        /* SPR FIX  20413 starts --to avoid setting a down path as primary path*/
        if( p_ass->transmit.remote[sctp_index].status == SCTP_COMMUNICATION_UP )
        {
            if( p_ass->transmit.primary_address_index
                    == p_ass->transmit.configured_primary_path_index )
            {
                p_ass->transmit.primary_address_index =  sctp_index;
                p_ass->transmit.configured_primary_path_index = sctp_index;
            }

            else
            {
                p_ass->transmit.configured_primary_path_index = sctp_index;
            }
        }

        else
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                        "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            *p_ecode = E_REQ_IN_WRONG_STATE;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        /* SPR FIX  20413 stops*/
    }

    if( p_saddr && !( SCTP_IS_INADDR_ANY( p_saddr[0] ) ) )
    {
        if( SCTP_FAILURE ==
                sctp_db_validate_local_addr( p_ass->p_ep, p_saddr, &sctp_index ) )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                        ( "sctp_setsockopt_sctp: Couldnot get address index" ) );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

        if( SCTP_FAILURE == sctp_dynamic_ip_supported( p_ass,
                                                       SCTP_ASCONF_PARAM_SET_PRIM ) )
        {
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                        ( "sctp_setsockopt_sctp: Couldnot get address index" ) );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }

#ifdef SCTP_DYNAMIC_IP_SUPPORT

        else
        {
            sctp_asconf_setprim_param_st  *p_set_prim =
                &p_ass->cmd_asconf.asconf_params[0].set_primary;

            p_ass->cmd_asconf.num_params = 1;
            p_set_prim->corr_id = 0;
            p_set_prim->ipaddr = *p_saddr;
            p_set_prim->param_type = SCTP_ASCONF_PARAM_SET_PRIM;

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT

            if( SCTP_FAILURE == sctp_chk_and_send_asconf( p_ass,
                                                          SCTP_ASCONF_PARAM_SET_PRIM, SCTP_FALSE, p_ecode ) )
#else

            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            if( SCTP_FAILURE == sctp_chk_and_send_asconf( p_ass,
                                                          ( sctp_bindx_flag_et )SCTP_ASCONF_PARAM_SET_PRIM, SCTP_FALSE, p_ecode ) )
#endif
            {
                SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_DBG_TRC,
                            ( "sctp_setsockopt_sctp: Couldnot get address index" ) );

                SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }
        }

#endif
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_get_srtt
 *
 *  DESCRIPTION:
 *    This api returns SRTT value for an association.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_get_srtt(
    sctp_U32bit       assoc_id,
    sctp_sockaddr_st  *p_daddr,
    sctp_U32bit       *p_srtt_value,
    sctp_error_t      *p_ecode )
{
    sctp_U32bit sctp_index;
    sctp_tcb_st *p_ass = SCTP_NULL;
    sctp_ep_st  *p_ep  = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_get_srtt " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( ( !p_daddr ) || ( !p_srtt_value ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API    ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /*SPR 21127 changes */
    if( SCTP_FAILURE == ( sctp_return_t )( sctp_db_is_af_correct( p_ep, 1, p_daddr ) )
            || ( sctp_db_validate_remote_addr( p_ass, p_daddr, &sctp_index )
                 == SCTP_FAILURE ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    *p_srtt_value = p_ass->transmit.remote[sctp_index].srtt;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_get_status
 *
 *  DESCRIPTION:
 *    This API return current value of various association specific parameters.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_get_status(
    sctp_U32bit            assoc_id,
    sctp_status_params_st  *p_status_params,
    sctp_error_t           *p_ecode )
{
    sctp_tcb_st   *p_ass = SCTP_NULL;
    sctp_ep_st    *p_ep  = SCTP_NULL;
    sctpbuffer_st *sctp_buf = SCTP_NULL;
    sctp_U32bit   counter;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_get_status " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( p_status_params == NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the Endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_status_params->connection_state = p_ass->state;

    p_status_params->num_out_streams = p_ass->transmit.num_out_streams;

    p_status_params->num_in_streams = p_ass->receive.num_in_streams;

    p_status_params->curr_receiver_win_size = p_ass->transmit.rwndthere;

    sctp_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );
    p_status_params->num_data_chnks_awaiting_ack = 0;
    p_status_params->num_data_chnks_awaiting_sent = 0;

    for( ; sctp_buf; sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf ) )
    {
        if( sctp_buf->sent == 1 )
        {
            p_status_params->num_data_chnks_awaiting_ack++;
        }

        else
        {
            p_status_params->num_data_chnks_awaiting_sent++;
        }
    }

    p_status_params->last_acked_TSN = p_ass->receive.cumtsn;

    p_status_params->last_sent_TSN = p_ass->transmit.tsn - 1;

    counter = p_ass->transmit.primary_address_index;
    p_status_params->primary_dest_addr
        = p_ass->transmit.remote[counter].that_address;

    p_status_params->num_msgs_awaiting_recv = 0;

    for( counter = 0; counter < p_ass->receive.num_in_streams; counter++ )
    {
        /* change from counting chunks to counting messages ??*/
        p_status_params->num_msgs_awaiting_recv
        += lstCount( &p_ass->receive.stream[counter].receive_queue );
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_get_dest_addr_status
 *
 *  DESCRIPTION:
 *    This API return current value of various association specific parameters.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_get_dest_addr_status(
    sctp_U32bit                      assoc_id,
    sctp_U32bit                      *p_num_daddrs,
    sctp_sockaddr_list_st            list_of_dest_addrs,
    sctp_dest_status_param_list_st   *p_dparam_list,
    sctp_sockaddr_st                 *p_dpriaddr,
    sctp_error_t                     *p_ecode )
{
    sctp_ep_st                *p_ep = SCTP_NULL;
    sctp_tcb_st               *p_ass = SCTP_NULL;
    sctp_U32bit               counter;
    sctp_U32bit               sctp_index;
    sctp_sockaddr_st          *addr_list
        = ( sctp_sockaddr_st * )list_of_dest_addrs;
    sctp_dest_status_param_st  *param_list
        = ( sctp_dest_status_param_st * )p_dparam_list;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_get_dest_addr_status " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }


    if( p_num_daddrs )
    {
        *p_num_daddrs = p_ass->transmit.num_remote_address;
    }

    if( addr_list != SCTP_NULL )
    {
        for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
        {
            addr_list[counter] = p_ass->transmit.remote[counter].that_address;
        }
    }

    sctp_index  = p_ass->transmit.primary_address_index;

    if( p_dpriaddr )
    {
        *p_dpriaddr = p_ass->transmit.remote[sctp_index].that_address;
    }

    if( param_list != SCTP_NULL )
    {
        for( counter = 0; counter < p_ass->transmit.num_remote_address; counter++ )
        {
            param_list[counter].srtt
                = p_ass->transmit.remote[counter].srtt;
            param_list[counter].rto
                = p_ass->transmit.remote[counter].rto;
            param_list[counter].mtu_size
                = p_ass->transmit.remote[counter].path_mtu;
            param_list[counter].cwnd
                = p_ass->transmit.remote[counter].cwnd;
            /* Changes for CSR# 1-4267892 */
            param_list[counter].allow_heartbeat
                = p_ass->transmit.remote[counter].allow_heartbeat;
            param_list[counter].status
                = p_ass->transmit.remote[counter].status;
            /* Changes for CSR# 1-4267892 Ends*/
            param_list[counter].is_confirmed
                = p_ass->transmit.remote[counter].is_confirmed;
        }
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :     sctp_api_flush_buffers
 *
 *  DESCRIPTION:
 *     This api will remove from tx queue as many unsent buffers as it can.
 *
 *  RETURN VALUE:
 *     SCTP_SUCCESS
 *     SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_flush_buffers(
    sctp_U32bit    assoc_id,
    sctp_error_t   *p_ecode )
{
    sctpbuffer_st *sctp_buf = SCTP_NULL;
    sctpbuffer_st *save     = SCTP_NULL;
    sctp_tcb_st   *p_ass    = SCTP_NULL;
    sctp_ep_st    *p_ep     = SCTP_NULL;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_flush_buffers " ) );

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    sctp_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    for( ; ( sctp_buf != SCTP_NULL ); )
    {
        /* look for unsent chunk with begin bit set */
        if( !( ( sctp_buf->begin == 1 ) && ( sctp_buf->sent == 0 ) ) )
        {
            sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * ) sctp_buf );
            continue;
        }

        /* Look for all chunks of this message and remove them */
        for( ; ; )
        {
            if( sctp_buf == SCTP_NULL )
            {
                /* This is system errror */

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return( SCTP_FAILURE );
            }

            save = ( sctpbuffer_st * ) lstNext( ( NODE * )sctp_buf );
            lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )sctp_buf );
            sctp_free_transmit_buffer( p_ass, sctp_buf );

            if( sctp_buf->end != 1 )
            {
                sctp_buf = save;
                continue;
            }

            else
            {
                break;
            }
        }

        if( save != NULL )
        {
            sctp_buf = save;
        }

        else
        {
            break;
        }
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_retrieve_unsent
 *
 *  DESCRIPTION:
 *    This API retrives first unsent datagram form the transmit queue.
 *    This API can be called any time while the association is up or
 *    association is in down retrievable state.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_retrieve_unsent(
    sctp_U32bit  assoc_id,
    sctp_U32bit  *context,
    sctp_U8bit   *data,
    sctp_U32bit  *data_len,
    sctp_U16bit  *stream_id,
    sctp_U32bit  *payload_prot_id,
    sctp_error_t *p_ecode )
{
    sctp_ep_st    *p_ep;
    sctp_tcb_st   *p_ass;
    sctpbuffer_st *sctp_buf;
    sctpbuffer_st *save;
    sctp_U32bit   buffer_len;
    sctp_U32bit   msg_len;
    sctp_U32bit   copy_len;

    if( !p_ecode )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( ( context == SCTP_NULL ) || ( data == SCTP_NULL )
            || ( data_len == SCTP_NULL ) || ( stream_id == SCTP_NULL )
            || ( payload_prot_id == SCTP_NULL ) || ( *data_len == 0 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( p_ep == SCTP_NULL )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    buffer_len = *data_len;
    sctp_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    for( ; sctp_buf; sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf ) )
    {
        /* look for unsent chunk with begin bit set */
        if( !( ( sctp_buf->begin == 1 ) && ( sctp_buf->sent == 0 ) ) )
        {
            continue;
        }

        copy_len = buffer_len > ( sctp_U32bit ) sctp_buf->size ?
                   ( sctp_U32bit )sctp_buf->size : buffer_len;

        if( copy_len > 0 )
        {
            __MEMCOPY( data, &sctp_buf->buffer[0], copy_len );
        }

        msg_len = copy_len;

        /* copy message related parameters */
        *context = sctp_buf->context;
        *stream_id = sctp_buf->stream;
        *payload_prot_id = sctp_buf->ppi;

        save = ( sctpbuffer_st * ) lstNext( ( NODE * )sctp_buf );
        lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )sctp_buf );
        sctp_free_transmit_buffer( p_ass, sctp_buf );

        if( sctp_buf->end != 1 )
        {
            /* Not the last fragment of a message*/
            sctp_buf = save;

            /* Scan to see if we have all fragments */
            for( ; ; )
            {
                if( ( sctp_buf == SCTP_NULL ) || ( sctp_buf->sent != 0 ) )
                {
                    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( ":Unexpected chunk in tx queue" ) );

                    *p_ecode = E_INTERNAL_ERROR;

                    /* Unlock the endpoint */
                    SCTP_UNLOCK_EP( assoc_id );

                    return ( SCTP_FAILURE );
                }

                copy_len = ( buffer_len - msg_len ) > ( sctp_U32bit ) sctp_buf->size
                           ? ( sctp_U32bit ) sctp_buf->size : ( buffer_len - msg_len );

                if( copy_len > 0 )
                {
                    __MEMCOPY( data + msg_len,  &sctp_buf->buffer[0], copy_len );
                }

                msg_len += copy_len;

                save = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf );
                lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )sctp_buf );
                sctp_free_transmit_buffer( p_ass, sctp_buf );

                if( sctp_buf->end == 1 )
                {
                    break;
                }

                sctp_buf = save;
            }
        } /* If Loop */

        *data_len = msg_len;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_SUCCESS );
    }

    *p_ecode = E_NOTHING_IN_QUEUE;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_api_retrieve_unacked
 *
 *  DESCRIPTION:
 *    This API retrives first unacked datagram form the transmit queue.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_retrieve_unacked(
    sctp_U32bit    assoc_id,
    sctp_U32bit    *context,
    sctp_U8bit     *data,
    sctp_U32bit    *data_len,
    sctp_Boolean_t *incomplete_flag,
    sctp_U16bit    *stream_id,
    sctp_U16bit    *ssn,
    sctp_U32bit    *payload_prot_id,
    sctp_error_t   *p_ecode )
{
    sctp_ep_st    *p_ep;
    sctp_tcb_st   *p_ass;
    sctpbuffer_st *sctp_buf, *save;
    sctp_U32bit   buffer_len;
    sctp_U32bit   msg_len;
    sctp_U32bit   copy_len;

    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    if( ( context == SCTP_NULL ) || ( data == SCTP_NULL )
            || ( data_len == SCTP_NULL ) || ( stream_id == SCTP_NULL )
            || ( payload_prot_id == SCTP_NULL ) || ( incomplete_flag == SCTP_NULL )
            || ( ssn == SCTP_NULL ) || ( *data_len == 0 ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( p_ass->state != SCTPSTATE_CLOSED )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    *incomplete_flag = SCTP_FALSE;
    buffer_len = *data_len;
    sctp_buf = ( sctpbuffer_st * )lstFirst( &p_ass->transmit.transmit_queue );

    for( ; sctp_buf; sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf ) )
    {
        /* Filter out unsent message */
        if( ( sctp_buf->begin == 1 ) && ( sctp_buf->sent == 0 ) )
        {
            for( ; ( sctp_buf != SCTP_NULL );
                    sctp_buf = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf ) )
            {
                if( sctp_buf->end == 1 )
                {
                    break;
                }
            }

            if( sctp_buf == SCTP_NULL )
            {
                SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                            E_INTERNAL_ERROR, "Internal Error !!" );
                *p_ecode = E_INTERNAL_ERROR;

                /* Unlock the endpoint */
                SCTP_UNLOCK_EP( assoc_id );

                return ( SCTP_FAILURE );
            }

            continue;
        }

        copy_len = buffer_len > ( sctp_U32bit ) sctp_buf->size ?
                   ( sctp_U32bit )sctp_buf->size : buffer_len;

        if( copy_len > 0 )
        {
            __MEMCOPY( data, &sctp_buf->buffer[0], copy_len );
        }

        msg_len = copy_len;

        if( sctp_buf->begin != 1 )
        {
            *incomplete_flag = SCTP_TRUE;
        }

        *context = sctp_buf->context;
        *stream_id = sctp_buf->stream;
        *payload_prot_id = sctp_buf->ppi;
        *ssn = sctp_buf->ssn;

        save = ( sctpbuffer_st * ) lstNext( ( NODE * )sctp_buf );
        lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )sctp_buf );
        sctp_free_transmit_buffer( p_ass, sctp_buf );

        if( sctp_buf->end != 1 )
        {
            /* Not the last fragment of a message*/
            sctp_buf = save;

            /* Scan to see if we have all fragments */
            for( ; ; )
            {
                if( sctp_buf == SCTP_NULL )
                {
                    SCTP_TRACE( p_ass->p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                                ( "SU Module:Unexpected chunk in tx queue" ) );
                    *p_ecode = E_INTERNAL_ERROR;

                    /* Unlock the endpoint */
                    SCTP_UNLOCK_EP( assoc_id );

                    return ( SCTP_FAILURE );
                }

                copy_len = ( buffer_len - msg_len ) >
                           ( sctp_U32bit ) sctp_buf->size
                           ? ( sctp_U32bit )sctp_buf->size : ( buffer_len - msg_len );

                if( copy_len > 0 )
                {
                    __MEMCOPY( data + msg_len,  &sctp_buf->buffer[0], copy_len );
                }

                msg_len += copy_len;

                save = ( sctpbuffer_st * )lstNext( ( NODE * )sctp_buf );
                lstDelete( &p_ass->transmit.transmit_queue, ( NODE * )sctp_buf );
                sctp_free_transmit_buffer( p_ass, sctp_buf );

                if( sctp_buf->end == 1 )
                {
                    break;
                }

                sctp_buf = save;
            }
        } /* If Loop */

        *data_len = msg_len;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_SUCCESS );
    }

    *p_ecode = E_NOTHING_IN_QUEUE;

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *  FUNCTION :    sctp_set_restart_option
 *
 *  DESCRIPTION: This api sets restart option.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_set_restart_option(
    sctp_U32bit              assoc_id,
    sctp_restart_options_et  restart_option,
    sctp_error_t             *p_ecode )
{
    sctp_ep_st    *p_ep   = SCTP_NULL;
    sctp_tcb_st   *p_ass  = SCTP_NULL;
    sctp_return_t ret     = SCTP_SUCCESS;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_set_restart_option" ) );

    if( !p_ecode )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    switch( restart_option )
    {
        case SCTP_RESTARTOPTION_REQUE:
            p_ass->restart_option = SCTP_RESTARTOPTION_REQUE;
            break;

        case SCTP_RESTARTOPTION_RETRIEVE:
            p_ass->restart_option = SCTP_RESTARTOPTION_RETRIEVE;
            break;

        default:
            *p_ecode = EINV_PARAMS;
            ret = SCTP_FAILURE;
            break;
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( ret );
}


/****************************************************************************
 *  FUNCTION :    sctp_api_get_stream_status
 *
 *  DESCRIPTION:
 *    This api returns last ssn received on stream specified.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_get_stream_status(
    sctp_U32bit        assoc_id,
    sctp_U16bit        stream_id ,
    sctp_U16bit        *ssn,
    sctp_error_t       *p_ecode )
{
    sctp_ep_st  *p_ep;
    sctp_tcb_st *p_ass ;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_get_stream_status " ) );

    if( !p_ecode )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;
        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    /* this api is allowed in all states  other than in connection
         setup state */
    if( ( p_ass->state == SCTPSTATE_COOKIEWAIT )
            || ( p_ass->state == SCTPSTATE_COOKIESENT ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:API Requested in Invalid State" ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return( SCTP_FAILURE );
    }

    if( stream_id >= p_ass->receive.num_in_streams )
    {
        *p_ecode = EINV_PARAMS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    if( ssn != NULL )
    {
        *ssn = p_ass->receive.stream[stream_id].nextssn - 1;

        if( *ssn == ( sctp_U16bit )( -1 ) ) /*No msg has been given yet on this stream*/
        {
            *p_ecode = E_REQ_IN_WRONG_STATE;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( assoc_id );

            return ( SCTP_FAILURE );
        }
    }

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );
}

/****************************************************************************
 *  FUNCTION :    sctp_api_abort_retrieve
 *
 *  DESCRIPTION:
 *    This api send the abort to the peer and give down retrievable
 *  notification to the ULP
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_abort_retrieve(
    sctp_U32bit        assoc_id,
    sctp_Boolean_t     *data_pending,
    sctp_error_t       *p_ecode )
{
    sctp_ep_st   *p_ep;
    sctp_tcb_st  *p_ass;
    sctp_U32bit  pr_index;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_abort_retrieve " ) );

    if( ( SCTP_NULL == p_ecode ) || ( SCTP_NULL == data_pending ) )
    {
        return( SCTP_FAILURE );
    }

    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        *p_ecode = E_REQ_IN_WRONG_STATE;
        return ( SCTP_FAILURE );
    }

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        return ( SCTP_FAILURE );
    }

    /* Lock the endpoint */
    SCTP_LOCK_EP( assoc_id );

    p_ep = sctp_db_get_ep_ptr( assoc_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_API_FAILURE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    if( !p_ass )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_ASSOCID,
                    "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Service User API ERROR" ) );
        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return ( SCTP_FAILURE );
    }


    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "API Requested in Invalid State" ) );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( assoc_id );

        return( SCTP_FAILURE );
    }

    /* set the flag CLOSE_CALLED */
    SCTP_SOCK_SET_CLOSE_CALLED( p_ep->flags, SCTP_TRUE );

    if( SCTP_NULL == lstFirst( &p_ass->transmit.transmit_queue ) )
    {
        *data_pending = SCTP_TRUE;
    }

    else
    {
        *data_pending = SCTP_FALSE;
    }

    pr_index = p_ass->transmit.primary_address_index;

    sctp_send_chunk_abort( p_ass,
                           &p_ep->this_address[0],
                           p_ep->this_port,
                           p_ass->transmit.that_port,
                           &p_ass->transmit.remote[pr_index].that_address,
                           p_ass->remote_tag,
                           SCTP_FALSE, 0, 0, SCTP_NULL, 0, p_ass->config.checksum_func );

    p_ass->event.msg_abort.cause_length = 0;

    sctp_assoc_state_machine( p_ass, SCTPEVENT_MSGABORT );

    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( assoc_id );

    return ( SCTP_SUCCESS );

}


/*
 * Notification API
 */

/***************************************************************************
 *   FUNCTION :    sctp_api_op_error_peer
 *
 *   DESCRIPTION:
 *     This API will notify application user of operation error received from
 *     Peer.
 *   RETURN VALUE:
 *     SCTP_SUCCESS
 *     SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_op_error_peer(
    sctp_U32bit     assoc_id,
    sctp_U16bit     error_code,
    sctp_U16bit     error_length,
    sctp_Pvoid_t    sys_error_info )
{
    sctp_ep_st   *p_ep;
    sctp_tcb_st  *p_ass;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Funcion sctp_api_op_error_peer, \
        ecode:%d,len:%d", error_code, error_length ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( error_code );
    SCTP_UNUSED_VARIABLE( error_length );
    /*to avoid the warning*/


    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function*/
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_OPERROR,
                         ( sctp_notice_ut * )&p_ass->notify.operror,
                         p_appl_data );
    }

    sys_error_info = SCTP_NULL; /* To remove warning */


    /* SPR 21127 changes start*/
    SCTP_UNUSED_VARIABLE( sys_error_info );
    /* SPR 21127 changes end*/

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *   FUNCTION :     sctp_api_destination_status_chg
 *
 *   DESCRIPTION:
 *     This API will notify application of change in destination address status.
 *
 *   RETURN VALUE:
 *     SCTP_SUCCESS
 *     SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_destination_status_chg(
    sctp_U32bit           assoc_id,
    sctp_sockaddr_st      *destination_ip_addr,
    sctp_sock_spc_event_et   dest_act_status )
{
    sctp_ep_st     *p_ep;
    sctp_tcb_st    *p_ass;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_destination_status_chg, \
      Status:%d: Addr", dest_act_status ) );
    SCTP_TRACE_ADDR( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC, destination_ip_addr );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( destination_ip_addr );
    SCTP_UNUSED_VARIABLE( dest_act_status );
    /*to avoid the warning*/

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_DESTSTATCHANGE,
                         ( sctp_notice_ut * ) &p_ass->notify.dststatchange, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *   FUNCTION :     sctp_api_endpoint_status_chg
 *
 *   DESCRIPTION:
 *     This API will notify application of change in endpoint status to either
 *     accessible or inaccessible.
 *   RETURN VALUE:
 *     SCTP_SUCCESS
 *     SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_endpoint_status_chg(
    sctp_U32bit                assoc_id,
    sctp_sock_sac_event_et     status,
    sctp_endpoint_chg_data_ut  *staus_change_data )
{
    sctp_ep_st  *p_ep;
    sctp_tcb_st *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:InFunction sctp_api_endpoint_status_chg,"
                  "Status:%d", status ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( status );
    SCTP_UNUSED_VARIABLE( staus_change_data );
    /*to avoid the warning*/


    if( SCTP_SUCCESS ==  sctp_db_get_callback_info( p_ep->appl_id,
                                                    &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_ENDSTATCHANGE,
                         ( sctp_notice_ut * ) &p_ass->notify.endstatchange, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_congestion
 *
 *  DESCRIPTION:
 *    This API will notify application to stop sending data as there is no
 *    transmit Buffer available with SCTP.
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_congestion(
    sctp_U32bit         assoc_id,
    sctp_cong_level_et  cong_level )
{
    sctp_ep_st            *p_ep;
    sctp_tcb_st           *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t          p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_congestion,Level:%d",
                  cong_level ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( cong_level );
    /*to avoid the warning*/


    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_CONGESTION,
                         ( sctp_notice_ut * )&p_ass->notify.congestion, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_recv_congestion
 *
 *  DESCRIPTION:
 *    This API will notify application that SCTP stack is experiencing a
 *    shortage of recv buffers.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_recv_congestion(
    sctp_U32bit         assoc_id,
    sctp_cong_level_et  cong_level )
{
    sctp_ep_st   *p_ep;
    sctp_tcb_st  *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_recv_congestion, Level:%d",
                  cong_level ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( cong_level );
    /*to avoid the warning*/



    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the callback function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_RECVCONGESTION,
                         ( sctp_notice_ut * )&p_ass->notify.recv_congestion, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_sys_error
 *
 *  DESCRIPTION:
 *    This API will notify application of any system error.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_sys_error(
    sctp_U32bit     assoc_id,
    sctp_error_t    errorno )
{
    sctp_ep_st     *p_ep;
    sctp_tcb_st    *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_sys_error, ecode:%d",
                  errorno ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( errorno );
    /*to avoid the warning*/



    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_SYSERROR,
                         ( sctp_notice_ut * ) &p_ass->notify.endstatchange, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_connection_pending
 *
 *  DESCRIPTION:
 *    This API will notify application that a connection is pending for it.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_connection_pending(
    sctp_U32bit        assoc_id )
{
    sctp_ep_st           *p_ep;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t         p_appl_data = SCTP_NULL;
    sctp_notice_ut       indication;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_api_connection_pending" ) );

    indication.connpending.assoc_id = assoc_id;

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_CONNPENDING,
                         ( sctp_notice_ut * )&indication.connpending, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_data_pending
 *
 *  DESCRIPTION:
 *    This API will notify application that there is new data pending to be read
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_data_pending(
    sctp_U32bit    assoc_id,
    sctp_U16bit    stream_id )
{
    sctp_ep_st     *p_ep;
    sctp_tcb_st    *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_data_pending, stream:%d",
                  stream_id ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( stream_id );
    /*to avoid the warning*/



    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_DATAPENDING,
                         ( sctp_notice_ut * ) &p_ass->notify.datapending, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_send_failure
 *
 *  DESCRIPTION:
 *    This API will notify application that Data Send failed.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_send_failure(
    sctp_U32bit     assoc_id,
    sctp_U32bit     context,
    sctp_error_t    cause_code )
{
    sctp_ep_st     *p_ep;
    sctp_tcb_st    *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_send_failure,context:%u, \
         cause:%d", context, cause_code ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( context );
    SCTP_UNUSED_VARIABLE( cause_code );
    /*to avoid the warning*/

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_SENDFAILURE,
                         ( sctp_notice_ut * ) &p_ass->notify.sendfailure, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_partial_data_pending
 *
 *  DESCRIPTION:
 *    This API will notify application that there is partial
 *    data pending to be read.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_partial_data_pending(
    sctp_U32bit    assoc_id,
    sctp_U16bit    stream_id )
{
    sctp_ep_st     *p_ep;
    sctp_tcb_st    *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_partial_data_pending, \
        stream:%d", stream_id ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( stream_id );
    /*to avoid the warning*/

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_PARTIAL_DATAPENDING,
                         ( sctp_notice_ut * ) &p_ass->notify.partial_datapending, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}


/***************************************************************************
 *  FUNCTION :   sctp_api_adaption_ind
 *
 *  DESCRIPTION:
 *    This API will notify application with an adaption indication
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_adaption_ind(
    sctp_U32bit    assoc_id,
    sctp_U32bit    adaption_bits )
{
    sctp_ep_st     *p_ep;
    sctp_tcb_st    *p_ass ;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t   p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "SCTP TRACE::SU Module:In Function sctp_api_adaption_ind,"
                  "value:0x%x", adaption_bits ) );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( adaption_bits );
    /*to avoid the warning*/

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_ADAPTION_IND,
                         ( sctp_notice_ut * ) &p_ass->notify.adaption_ind, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :   sctp_api_peer_data_drop
 *
 *  DESCRIPTION:
 *    This API will notify application that a peer data is dropped by sctp.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_peer_data_drop(
    sctp_U32bit        assoc_id,
    sctp_error_t       ecode,
    sctp_U8bit         *p_buffer,
    sctp_U32bit        buffer_len )
{
    sctp_ep_st           *p_ep;
    sctp_tcb_st          *p_ass;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t         p_appl_data = SCTP_NULL;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ecode );
    SCTP_UNUSED_VARIABLE( p_buffer );
    SCTP_UNUSED_VARIABLE( buffer_len );
    /*to avoid the warning*/


    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_api_peer_data_drop" ) );

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_PEER_DATA_DROP,
                         ( sctp_notice_ut * )&p_ass->notify.peer_data_drop, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}

/***************************************************************************
 *  FUNCTION :   sctp_api_shutdown_event
 *
 *  DESCRIPTION:
 *    This API will notify application that a connection is pending for it.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_shutdown_event(
    sctp_U32bit        assoc_id )
{
    sctp_ep_st           *p_ep;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_callback_func_t p_callback_func = SCTP_NULL;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_callback_func_t p_callback_func = ( sctp_callback_func_t )SCTP_NULL;
#endif
    sctp_Pvoid_t         p_appl_data = SCTP_NULL;
    sctp_notice_ut       indication;

    /* SPR : 18527 */
    if( ( p_ep = sctp_db_get_ep_ptr( assoc_id ) ) == SCTP_NULL )
    {
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "ERROR :: sctp_db_get_ep_ptr(%d) failed.", assoc_id ) );

        return SCTP_FAILURE;
    }

    /* SPR : 18527 */

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_api_shutdown_event" ) );

    indication.shutdown_event.assoc_id = assoc_id;

    if( SCTP_SUCCESS == sctp_db_get_callback_info( p_ep->appl_id,
                                                   &p_callback_func, &p_appl_data ) )
    {
        /* call the call back function  */
        p_callback_func( ( sctp_U32bit )SCTPNOTIFY_SHUTDOWN_EVENT,
                         ( sctp_notice_ut * )&indication.shutdown_event, p_appl_data );
    }

    return ( SCTP_SUCCESS );
}



/***************************************************************************
 *  FUNCTION :    sctp_generic_su_notification
 *
 *  DESCRIPTION:
 *    This function will call the necessary function based on the event.
 *
 *  RETURN VALUE:
 *    void
 **************************************************************************/
void
sctp_generic_su_notification(
    sctp_ep_st    *p_ep,
    sctp_U32bit    event )
{
#ifdef SCTP_NO_QUEUE
    sctp_tcb_st *p_ass = SCTP_NULL;
#endif

#ifdef SCTP_UI_SU_MESSAGEBASED
    sctp_U16bit   su_id;
    sctp_tcb_st *p_ass = SCTP_NULL;
#endif

    SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                ( "sctp_generic_su_notification" ) );


#ifdef SCTP_UI_SU_MESSAGEBASED

    /* SPR : 18527 */
    if( ( p_ass = sctp_db_get_assoc_from_ep( p_ep ) ) != SCTP_NULL )
    {
        /* SPR : 18527 */
        if( sctp_db_get_su_id( p_ep->appl_id, &su_id ) == SCTP_SUCCESS )
        {
            sctp_ui_send_notifications( su_id, event, &p_ass->notify );
        }
    }

#else

#ifndef SCTP_NO_QUEUE
    sctp_add_notification_to_queue( p_ep, event );
#else
    /*  Get the association id for the connection */
    p_ass = sctp_db_get_assoc_from_ep( p_ep );

    switch( event )
    {
        case SCTPNOTIFY_OPERROR:
            sctp_api_op_error_peer( p_ep->ep_id,
                                    p_ass->notify.operror.err_code,
                                    p_ass->notify.operror.err_length,
                                    p_ass->notify.operror.sys_err_info );
            break;

        case SCTPNOTIFY_DESTSTATCHANGE:

            sctp_api_destination_status_chg( p_ep->ep_id,
                                             &p_ass->notify.dststatchange.destination_ip_addr,
                                             p_ass->notify.dststatchange.status );
            break;

        case SCTPNOTIFY_ENDSTATCHANGE:

            sctp_api_endpoint_status_chg( p_ep->ep_id,
                                          p_ass->notify.endstatchange.status,
                                          &p_ass->notify.endstatchange.status_data );
            break;

        case SCTPNOTIFY_CONGESTION:
            sctp_api_congestion( p_ep->ep_id,
                                 p_ass->notify.congestion.cong_level );
            break;

        case SCTPNOTIFY_SYSERROR:
            sctp_api_sys_error( p_ep->ep_id,
                                p_ass->notify.syserror.errorno );
            break;

        case SCTPNOTIFY_CONNPENDING:
            sctp_api_connection_pending( p_ep->ep_id );
            break;

        case SCTPNOTIFY_DATAPENDING:
            sctp_api_data_pending( p_ep->ep_id,
                                   p_ass->notify.datapending.stream );
            break;

        case SCTPNOTIFY_SENDFAILURE:
            sctp_api_send_failure( p_ep->ep_id,
                                   p_ass->notify.sendfailure.context,
                                   p_ass->notify.sendfailure.cause_code );
            break;

        case SCTPNOTIFY_PARTIAL_DATAPENDING:
            sctp_api_partial_data_pending( p_ep->ep_id,
                                           p_ass->notify.datapending.stream );
            break;

        case SCTPNOTIFY_RECVCONGESTION:
            sctp_api_recv_congestion( p_ep->ep_id,
                                      p_ass->notify.recv_congestion.cong_level );
            break;

        case SCTPNOTIFY_ADAPTION_IND:
            sctp_api_adaption_ind( p_ep->ep_id,
                                   p_ass->notify.adaption_ind.adaption_bits );
            break;

        case SCTPNOTIFY_PEER_DATA_DROP:
            sctp_api_peer_data_drop( p_ep->ep_id, p_ass->notify.peer_data_drop.ecode,
                                     p_ass->notify.peer_data_drop.buffer, p_ass->notify.peer_data_drop.packet_len );
            break;

        case SCTPNOTIFY_SHUTDOWN_EVENT:
            sctp_api_shutdown_event( p_ep->ep_id );
            break;

            /*
             * Intentionaly removed the default case to let compiler
             * complian aboput missing handling
             */
    }

#endif
#endif
}

/* SPR 6959 : Enhancement SPR */

/***************************************************************************
 *  FUNCTION :    sctp_api_connectx_ext
 *
 *  DESCRIPTION:
 *    This api can be invoked as  blocked or unblocked call.If
 *    invoked as blocked call will block the calling process till a
 *    new connection arrives. If invoked as unblocked call will return
 *    a valid association id if one is pending else return error.
 *
 *    This api differs from sctp_api_connect_ext as here multiple destination
 *    address can be specified.
 *
 *  PARAMETERS:
 * [IN]  association_id   : endpoint identifier
 *
 * [IN]  dest_sctp_port   : port of the destination
 *
 * [IN]  payload_prot_id  : identifier of the protocol whose payload will be
 *                          carried in this association.
 *
 * [IN]  block            : whether the call is to be blocked till connection
 *                          is established.
 *
 * [IN]  num_dest_addrs   : number of destination addresses provided by UL
 *
 * [IN]  p_connect_daddrs : array of addresses of the remote machine.
 *
 * [I/O] p_num_ostreams   : UL specifies the desired number of out streams,
 *                          the call returns the negotiated value
 *
 * [I/O] p_num_istreams   : UL specifies the desired number of input streams,
 *                          the call returns the negotiated value.
 *
 * [OUT] p_num_daddrs     : Number of destination addresses of the peer
 *                          with whom the connection was established.
 *
 * [OUT] list_of_dest_addrs : the list of addresses of the peer.
 *
 * [OUT] p_ecode          : incase the call fails, this value specifies the
 *                          error code.
 *
 *  RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 **************************************************************************/
sctp_return_t
sctp_api_connectx_ext(
    sctp_U32bit             association_id,
    sctp_U16bit             dest_sctp_port,
    sctp_U32bit             payload_prot_id,
    sctp_Boolean_t          block,
    sctp_U32bit             num_dest_addrs,
    sctp_sockaddr_st        *p_connect_daddrs,
    sctp_U16bit             *p_num_ostreams,
    sctp_U16bit             *p_num_istreams,
    sctp_U32bit             *p_num_daddrs,
    sctp_sockaddr_list_st   list_of_dest_addrs,
    sctp_error_t            *p_ecode )
{
    sctp_U32bit      counter;
    sctp_U16bit      sctp_port;
    sctp_tcb_st      *p_tcb = SCTP_NULL;
    sctp_ep_st       *p_ep  = SCTP_NULL;
    sctp_sockaddr_st *addr_list = ( sctp_sockaddr_st * )list_of_dest_addrs;
    sctp_return_t    ret;

    SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC, ( "sctp_api_connectx_ext" ) );

    /*
     * if no memory has been allocated for the variable in which the error code
     * is to be returned, the api call fails
     *
     */
    if( !p_ecode )
    {
        return ( SCTP_FAILURE );
    }

    /*
     * if the stack has not been initialized the call fails.
     *
     * We notify the UL, that this api has been invoked in the wrong state, by
     * storing E_REQ_IN_WRONG_STATE value in the location pointed to by the
     * p_ecode variable.
     *
     */
    if( sctp_stack_state != SCTP_STATE_ACTIVE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_REQ_IN_WRONG_STATE, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_REQ_IN_WRONG_STATE;

        return ( SCTP_FAILURE );
    }

    /*
     * If the array of destination addresses contains a pointer to NULL of the
     * number of addresses are 0 or more than SCTP_MAX_TRANSPORT_ADDR the call
     * fails.
     *
     * The UL is notified, indicating invalid parameters by storing EINV_PARAMS
     * in the location pointed to by p_ecode variable
     *
     */
    if( !p_connect_daddrs || !num_dest_addrs ||
            num_dest_addrs > SCTP_MAX_TRANSPORT_ADDR )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, EINV_PARAMS,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = EINV_PARAMS;

        return ( SCTP_FAILURE );
    }

    /*
     * retrieve the endpoint corresponding to that association id (SCTP
     * endpoint). If no enpoint could be located the call fails
     * notifying appropriate error reason i.e. EINV_ASSOCID.
     *
     */

    p_ep = sctp_db_get_ep_ptr( association_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;

        return SCTP_FAILURE;
    }

    /*SPR 20396 Starts*/
    if( dest_sctp_port == 0 )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR, E_INVALID_SCTP_PORT,
                    "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Port" ) );

        *p_ecode = E_INVALID_SCTP_PORT;

        return ( SCTP_FAILURE );
    }

    /*SPR 20396 Ends*/



    /* Lock the endpoint */
    SCTP_LOCK_EP( association_id );

    /*
     * We need to retrieve this endpoint again because it might be possible
     * that some other event destroys the endpoint while the lock was being
     * acquired.
     *
     */
    p_ep = sctp_db_get_ep_ptr( association_id );

    if( !p_ep )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;
        SCTP_UNLOCK_EP( association_id );

        return SCTP_FAILURE;
    }


    /*
     * Connection from a server socket not allowed.
     */
    if( SCTP_SOCK_IS_LISTENER( p_ep->flags ) )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );
        SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SCTP TRACE::SU Module:Invalid Association" ) );

        *p_ecode = EINV_ASSOCID;

        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Check if the socket is bound. If not then bind the socket.
     */
    if( !SCTP_SOCK_IS_BOUND( p_ep->flags ) )
    {
        /*
         * Here socket is not bound so first we have to bind it.
         * we select a unused port and copy it to ep .
         */
        ret  = sctp_db_get_free_sctpport( &sctp_port );

        if( ret == SCTP_FAILURE )
        {
            /* No free port could be located, hence the call fails citing
             * lack of resources as the cause.
             */
            *p_ecode = E_NORESOURCES;

            /* Unlock the association before we return */
            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* Update the SCTP databases so as to indicate that the port has been
         * binded. In case the function call fails we return returning the
         * cause of failure returned by the sctp_db_bind_ep_with_af_check
         * function as the cause of our failure
         */
        if( !sctp_db_bind_ep_with_af_check( association_id, sctp_port, p_ecode ) )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        *p_ecode, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* Set the socket as bound */
        SCTP_SOCK_SET_BOUND( p_ep->flags, SCTP_TRUE );
        /* Add the endpoint to the local database of bound sockets */
        sctp_db_add_hash( SCTP_BIND_HASH, p_ep );
    }

    /*SPR 5216*/
    /*moved address compatibilty check to sctp_db_addr_connectable*/
    /*SPR 5216*/
    /*
     * Check if a connection to the address would create any inconsistency.
     *
     * In case a probable inconsistency is detected then the call returns
     * failure, citing invalid addresses (EINV_ADDRS) as the cause.
     *
     */
    if( sctp_db_addr_connectable( p_ep, dest_sctp_port, num_dest_addrs, p_connect_daddrs,
                                  p_ecode ) == SCTP_FAILURE )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ADDRS, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Address is not Connectable" ) );

        *p_ecode = EINV_ADDRS;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    /*
     * Check if the socket has already an association if yes then report error
     * because its TCP style socket so can have only one assoc atmost .
     *
     * In case the endpoint has an associated assoc the current call execution
     * fails, citing invalid association (EINV_ASSOC) as the reason.
     *
     */
    p_tcb = sctp_db_get_assoc_from_ep( p_ep );

    if( SCTP_NULL != p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    EINV_ASSOCID, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS,    1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "SOcket Already connected" ) );

        *p_ecode = EINV_ASSOCID;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    /*SPR 6591*/

    /* If the UL has specified the number of outstreams expected for
     * this association, then verify whether the value is valid if yes
     * update the association creation parameters else we use the default
     * values which max streams that can be supported by the SCTP stack.
     *
     * This value is specified at init time.

     */
    if( ( p_num_ostreams )
            && ( *p_num_ostreams != 0 )
            && ( *p_num_ostreams <= sctp_db_main.max_streams_per_assoc ) )
    {
        p_ep->sk_config.num_out_streams = *p_num_ostreams;
    }

    /* If the UL has specified the number of instreams expected for
     * this association, then verify whether the value is valid if yes
     * update the association creation parameters else we use the default
     * values which max streams that can be supported by the SCTP stack.
     *
     * This value is specified at init time.
     */
    if( ( p_num_istreams )
            && ( *p_num_istreams != 0 )
            && ( *p_num_istreams <= sctp_db_main.max_streams_per_assoc ) )
    {
        p_ep->sk_config.num_in_streams =  *p_num_istreams;
    }

    /* Update the payload protocol identifier for this SCTP endpoint */
    p_ep->ppi = payload_prot_id;


    /*
     * Create a new TCB for the connection. If no new tcb could be created
     * which could be because, the stack is already servicing maximum number
     * of assocs it has been configured to serve (specified at init time),
     * the call fails citing E_NORESOURCES as the possible reason.
     *
     */
    p_tcb = sctp_db_create_tcb( p_ep, dest_sctp_port, num_dest_addrs, p_connect_daddrs,
                                p_ecode );

    if( SCTP_NULL == p_tcb )
    {
        SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                    E_NORESOURCES, "Service User API ERROR" );

        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );

        SCTP_TRACE( SCTP_NULL, SCTP_INTF_TRC | SCTP_ERR_TRC,
                    ( "Resources not available" ) );

        *p_ecode = E_NORESOURCES;

        /* Unlock the endpoint */
        SCTP_UNLOCK_EP( association_id );

        return ( SCTP_FAILURE );
    }

    /* call the finite state machine with the ASSOCIATE primitive, for this
     * assoc. The ASSOCIATE primitive is mapped to SCTPEVENT_CMDCONNECT in
     * our implementation.
     */
    sctp_assoc_state_machine( p_tcb, SCTPEVENT_CMDCONNECT );

    /* The value of block indicates whether the function is to be executed in
     * blocking or non blocking mode.
     */
    if( block == SCTP_TRUE )
    {
        association_id = p_ep->ep_id;

        /* Sleep till we're successfully connected, or there is an error
         * signalled by the stack
         */
        SCTP_SLEEP( p_ep->cond, p_ep->lock );

        /*
         * Get the ep ptr again. If the endpoint pointer is SCTP_NULL, it implies
         * that connection establishment failed due to some reason. We notify this
         * to the UL by updating the location pointed to by p_ecode with
         * E_ASSOC_DELETED.
         *
         */
        p_ep = sctp_db_get_ep_ptr( association_id );

        if( !p_ep )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_ASSOC_DELETED, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( ":Association deleted while connect call" ) );

            *p_ecode = E_ASSOC_DELETED;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* Get the association pointer, corresponding to the endpoint. Since
         * there can be only one assoc corresponding to that endpoint hence
         * any memory location returned by this function would be that of
         * the association which has been created possibly created as the
         * result of this call.
         */
        p_tcb = sctp_db_get_assoc_from_ep( p_ep );

        if( !p_tcb )
        {
            SCTP_ERROR( SCTPERROR_FLAG_APIERROR, SCTPERROR_LEVEL_MAJOR,
                        E_ASSOC_DELETED, "Service User API ERROR" );

            SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
            SCTP_TRACE( p_ep, SCTP_INTF_TRC | SCTP_ERR_TRC,
                        ( ":Association deleted while connect call" ) );

            *p_ecode = E_ASSOC_DELETED;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( association_id );

            return ( SCTP_FAILURE );
        }

        /* If the Assoc is not establsihed then return error. */
        if( p_tcb->state != SCTPSTATE_ESTABLISHED )
        {
            *p_ecode = EINV_PARAMS;

            /* Unlock the endpoint */
            SCTP_UNLOCK_EP( association_id );

            /* delete the association and return it to the free pool */
            sctp_db_delete_association( p_tcb );

            return ( SCTP_FAILURE );
        }

        else
        {
            /* If p_num_daddrs point to some memory address then update that location
             * to the number of destination addresses with which the connection was
             * established.
             */
            if( p_num_daddrs )
            {
                *p_num_daddrs = p_tcb->transmit.num_remote_address;
            }

            if( list_of_dest_addrs != SCTP_NULL )
            {
                for( counter = 0; counter < p_tcb->transmit.num_remote_address;
                        counter++ )
                {
                    /* copy the address structure */
                    addr_list[counter]
                        = p_tcb->transmit.remote[counter].that_address;
                }
            }

            /* return in stream parameter if the p_num_istreams point to some
             * location. This is the value which was negotiated and settled
             * upon by the two SCTP stack * entities, in accordance to the RFC.
             */
            if( p_num_istreams )
            {
                *p_num_istreams = p_tcb->receive.num_in_streams;
            }

            /* return out stream parameter if the p_num_ostreams point to some
             * location. This is the value which was negotiated and settled
             * upon by the two SCTP stack * entities, in accordance to the RFC.
             */

            if( p_num_ostreams )
            {
                *p_num_ostreams = p_tcb->transmit.num_out_streams;
            }
        }
    } /* If construct */


    /* Unlock the endpoint */
    SCTP_UNLOCK_EP( association_id );

    return ( SCTP_SUCCESS );
}

/* SPR 6959 : Enhancement SPR Ends here */

#endif /* SCTP_UI_SOCK_API */





