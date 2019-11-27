/***************************************************************************
 *  FILE NAME  : s_dbg.c
 *
 *  DESCRIPTION: Contains the Functions for the Debug Module.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  17Sep03    Manish Rajpal             Rel 5.0
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_ostmr.h>
#include <s_db.h>
#include <s_dbg.h>
#include <s_dbtcb.h>
#include <s_comac.h>
#include <s_osutil.h>
#include <s_ostmr.h>

#ifdef SCTP_DEBUG_ENABLED

/*
 * This data struture contains the current global trace level for
 * each trace flags.
 */
sctp_U8bit  sctp_gbl_trace;
#endif

/***************************************************************************
 *    FUNCTION :
 *        sctp_api_enable_assoc_trc
 *
 *    DESCRIPTION:
 *        This function is called by the local layer management entity for
 *    SCTP module and it enables trace for an association to the level requeted.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_enable_assoc_trc(
    sctp_U32bit    ep_id,
    sctp_trc_et    ttype,
    sctp_error_t   *p_ecode )
{
#ifdef SCTP_DEBUG_ENABLED
    sctp_return_t ret_val  = SCTP_SUCCESS;
    sctp_ep_st    *p_ep    = SCTP_NULL;
    sctp_U8bit    *p_flags = SCTP_NULL;

    if( p_ecode == SCTP_NULL )
    {
        return ( SCTP_FAILURE );
    }

    if( SCTP_INVALID_ENDPOINT == ep_id )
    {
        /* If the Endpoint is invalid then disable the global trace */
        p_flags = &sctp_gbl_trace;
    }

    else
    {
        /* Else disable the traces for the socket specified */
        p_ep = sctp_db_get_ep_ptr( ep_id );

        if( !p_ep )
        {
            SCTP_SET_ECODE( p_ecode, EINV_ASSOCID );
            return ( SCTP_FAILURE );
        }

        p_flags = &p_ep->trc_flags;
    }

    /* check that trace flag is valid  */
    switch( ttype )
    {
        case SCTP_INTF_TRC:
            SCTP_ENABLE_INTF_TRC( *p_flags );
            break;

        case SCTP_ERR_TRC:
            SCTP_ENABLE_ERR_TRC( *p_flags );
            break;

        case SCTP_DBG_TRC:
            SCTP_ENABLE_DBG_TRC( *p_flags );
            break;

        case SCTP_ALL_TRC:
            SCTP_ENABLE_ALL_TRC( *p_flags );
            break;

        default:
            SCTP_SET_ECODE( p_ecode, EINV_TRCTYPE );
            ret_val = SCTP_FAILURE;
    }

    return ( ret_val );
#else

    if( p_ecode != SCTP_NULL )
    {
        *p_ecode = E_TRCDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ep_id );
    SCTP_UNUSED_VARIABLE( ttype );
    /*to avoid the warning*/

    return ( SCTP_FAILURE );
#endif
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_disable_assoc_trc
 *
 *    DESCRIPTION:
 *        This function is called by the local layer management entity for
 *    SCTP module and it disables all traces for a single or all associations.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_disable_assoc_trc(
    sctp_U32bit      ep_id,
    sctp_trc_et      ttype,
    sctp_error_t     *p_ecode )
{
#ifdef SCTP_DEBUG_ENABLED
    sctp_return_t ret_val = SCTP_SUCCESS;
    sctp_ep_st    *p_ep   = SCTP_NULL;
    sctp_U8bit    *p_flags  = SCTP_NULL;

    if( p_ecode == SCTP_NULL )
    {
        return ( SCTP_FAILURE );
    }

    if( SCTP_INVALID_ENDPOINT == ep_id )
    {
        /* If the Endpoint is invalid then disable the global trace */
        p_flags = &sctp_gbl_trace;
    }

    else
    {
        /* Else disable the traces for the socket specified */
        p_ep = sctp_db_get_ep_ptr( ep_id );

        if( !p_ep )
        {
            SCTP_SET_ECODE( p_ecode, EINV_ASSOCID );
            return ( SCTP_FAILURE );
        }

        p_flags = &p_ep->trc_flags;
    }


    /* check that trace flag is valid  */
    switch( ttype )
    {
        case SCTP_INTF_TRC:
            SCTP_DISABLE_INTF_TRC( *p_flags );
            break;

        case SCTP_ERR_TRC:
            SCTP_DISABLE_ERR_TRC( *p_flags );
            break;

        case SCTP_DBG_TRC:
            SCTP_DISABLE_DBG_TRC( *p_flags );
            break;

        case SCTP_ALL_TRC:
            SCTP_DISABLE_ALL_TRC( *p_flags );
            break;

        default:
            SCTP_SET_ECODE( p_ecode, EINV_TRCTYPE );
            ret_val = SCTP_FAILURE;
    }

    return ( ret_val );
#else

    if( p_ecode != SCTP_NULL )
    {
        *p_ecode = E_TRCDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ep_id );
    SCTP_UNUSED_VARIABLE( ttype );
    /*to avoid the warning*/

    return ( SCTP_FAILURE );
#endif
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_is_trace_enabled
 *
 *    DESCRIPTION: Checks a trace type is enabled or not
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_is_trace_enabled(
    sctp_ep_st    *p_ep,
    sctp_trc_et   trace_type )
{
#ifdef SCTP_DEBUG_ENABLED

    /*
     * If the global trace is enabled for the particular type then we need
     * not care about the Endpoint specific trace.
     */
    if( sctp_gbl_trace & trace_type )
    {
        return ( SCTP_SUCCESS );
    }

    /*
     * Means that global trace is not enabled. Check that the per endpoint
     * Trace is enabled or not.
     */
    if( p_ep && ( p_ep->trc_flags & trace_type ) )
    {
        return ( SCTP_SUCCESS );
    }

#endif

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ep );
    SCTP_UNUSED_VARIABLE( trace_type );
    /*to avoid the warning*/

    return ( SCTP_FAILURE );
}


/***************************************************************************
 *    FUNCTION :
 *        sctp_api_get_trace_info
 *
 *    DESCRIPTION: This function gets the trace info for a particular ep_id
 *                 passed as paramater.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_get_trace_info(
    sctp_U32bit          ep_id,
    sctp_U8bit           *trace_info,
    sctp_error_t         *p_ecode )
{
#ifdef SCTP_DEBUG_ENABLED

    sctp_ep_st   *p_ep;

    if( ( trace_info == SCTP_NULL )
            || ( p_ecode == SCTP_NULL ) )
    {
        return ( SCTP_FAILURE );
    }

    if( SCTP_INVALID_ENDPOINT == ep_id )
    {
        /* If the Endpoint is invalid then disable the global trace */
        *trace_info = sctp_gbl_trace;
    }

    else
    {
        /* Else disable the traces for the socket specified */
        p_ep = sctp_db_get_ep_ptr( ep_id );

        if( !p_ep )
        {
            SCTP_SET_ECODE( p_ecode, EINV_ASSOCID );
            return ( SCTP_FAILURE );
        }

        *trace_info = p_ep->trc_flags;
    }

    return( SCTP_SUCCESS );
#else

    if( p_ecode != SCTP_NULL )
    {
        *p_ecode = E_TRCDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( ep_id );
    SCTP_UNUSED_VARIABLE( trace_info );
    /*to avoid the warning*/


    return ( SCTP_FAILURE );
#endif
}

#ifdef SCTP_DEBUG_ENABLED

/***************************************************************************
**    FUNCTION :
**    sctp_hex_dump
****************************************************************************
**    DESCRIPTION: Checks if traces are analbed. If enabled it dumps the
**                 traces for hexvalues of the buffer passed as parameter.
**    RETURN VALUE:
**    None
****************************************************************************/
void
sctp_hex_dump(
    sctp_ep_st      *p_ep,
    sctp_trc_et     ttype,
    sctp_U8bit      *p_buffer,
    sctp_U32bit     noctets )
{
    sctp_return_t ret;

    ret = sctp_is_trace_enabled( p_ep, ttype ) ;

    if( ret == SCTP_SUCCESS )
    {
        SCTP_PRINT_HEX( p_buffer, noctets );
    }
}


/***************************************************************************
 *    FUNCTION :
 *    sctp_init_debg_module
 *
 *    DESCRIPTION: This function disables the association traces.
 *
 *    RETURN VALUE:
 *        SCTP_SUCCESS
 *        SCTP_FAILURE
 ***************************************************************************/
sctp_return_t
sctp_init_debg_module(
    sctp_error_t    *p_ecode )
{
    return sctp_api_disable_assoc_trc( SCTP_INVALID_ENDPOINT,
                                       SCTP_ALL_TRC, p_ecode );
}


/***************************************************************************
 *    FUNCTION :
 *        givetime
 *
 *    DESCRIPTION:
 *        Used to print the current time.
 *
 *    RETURN VALUE:
 *        void
 ***************************************************************************/
void givetime(
    void )
{
    char tmp_time[30];
    sctp_time_t tmp_clock;

    __GET_ABSOLUTE_SYSTIME( &tmp_clock );

    sctp_get_ctime( tmp_clock.tv_sec, tmp_time );

    tmp_time[20] = 0;    /* Truncate date */

    /* Remove date as only time needed */
    SCTP_REPORT( ( "%s:", ( tmp_time + 11 ) ) );
}


#endif /* SCTP_DEBUG_ENABLED */


