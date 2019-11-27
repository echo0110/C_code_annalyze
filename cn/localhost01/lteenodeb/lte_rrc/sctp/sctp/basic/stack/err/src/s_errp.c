/***************************************************************************
 *  FILE NAME:
 *    s_err.c
 *
 *  DESCRIPTION:
 *    This file is a part of SCTP Error module. This lists all the Layer
 *     Management related interface functions for Error handling.
 *
 *  DATE     NAME              REF#   REASON
 *  -------  ----------------  -----  ---------------
 *  27Mar00  sigtran team             Original
 *  18Sep03  Manish Rajpal            Rel 5.0
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/

#define SCTP_MODULE "ERR"

#include <s_cogbl.h>
#include <s_cotyps.h>

#include <s_errhdl.h>
#include <s_stats.h>
#include <s_err.h>
#include <s_osutil.h>
#ifdef SCTP_ERROR_ENABLED

sctp_U8bit sctp_error_state = 0; /* disabled */
/* maintains the error alarm level for each alarm type */

sctp_error_level_et    sctp_error_table[SCTP_MAX_ERROR_FLAG + 1];

void
sctp_err_reporter(
    sctp_U32bit       error_type,
    sctp_U32bit       error_level,
    sctp_U32bit       ecode ,
    sctp_U8bit        *info_str );

sctp_return_t
sctp_init_error_module(
    sctp_error_t        *p_ecode );

#endif
/***************************************************************************
 *   FUNCTION:
 *     sctp_api_get_error_level
 *
 *   DESCRIPTION:
 *    This functions is called in SCTP layer management entity context
 *    to get the current level set for any alarm type
 *
 *   RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_get_error_level(
    sctp_error_flag_et     error_flag,
    sctp_error_level_et    *p_error_level,
    sctp_error_t           *p_ecode )
{

#ifdef SCTP_ERROR_ENABLED

    if( ( p_error_level == NULL ) || ( p_ecode == NULL ) )
    {
        return( SCTP_FAILURE );
    }

    /* if errors enabled */
    if( sctp_error_state != 1 )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_SCTP_ERROR_DISABLED, "System Management API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_SCTP_ERROR_DISABLED;

        return( SCTP_FAILURE );
    }

    /* check for error flag to be in limits */
    if( error_flag >  SCTP_MAX_ERROR_FLAG )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_SCTP_INVALID_ERROR_FLAG, "System Management API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_SCTP_INVALID_ERROR_FLAG;
        return( SCTP_FAILURE );
    }

    else
    {
        /* return the error level */
        *p_error_level = sctp_error_table[( sctp_U16bit )error_flag];
        return( SCTP_SUCCESS );
    }

#else

    if( p_ecode != SCTP_NULL )
    {
        *p_ecode = E_ERRORDISABLED;
    }

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( error_flag );
    SCTP_UNUSED_VARIABLE( p_error_level );
    /*to avoid the warning*/


    return ( SCTP_FAILURE );

#endif
}

/***************************************************************************
 *   FUNCTION:
 *     sctp_api_set_error_level
 *
 *   DESCRIPTION:
 *    This functions is called in SCTP layer management entity context
 *    to set the error level for an alarm type
 *
 *   RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_api_set_error_level(
    sctp_error_flag_et      error_flag ,
    sctp_error_level_et     error_level,
    sctp_error_t            *p_ecode )
{
#ifdef SCTP_ERROR_ENABLED
    sctp_U32bit counter;

    if( p_ecode == NULL )
    {
        return( SCTP_FAILURE );
    }

    /* Check if errors enabled */
    if( sctp_error_state != 1 )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_SCTP_ERROR_DISABLED, "System Management API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_SCTP_ERROR_DISABLED;

        return( SCTP_FAILURE );
    }

    /* check for error flag to be in limits */
    if( error_flag > SCTPERROR_FLAG_ALL_ERRORS )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_SCTP_INVALID_ERROR_FLAG, "System Management API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_SCTP_INVALID_ERROR_FLAG;

        return( SCTP_FAILURE );
    }

    /* check for error flag to be in limits */
    if( ( error_level != SCTPERROR_LEVEL_NOERROR )
            && ( error_level != SCTPERROR_LEVEL_MINOR )
            && ( error_level != SCTPERROR_LEVEL_MAJOR )
            && ( error_level != SCTPERROR_LEVEL_CRITICAL ) )
    {
        SCTP_ERROR( SCTPSTATS_API_ERRORS, SCTPERROR_LEVEL_MAJOR,
                    E_SCTP_INVALID_ERROR_LEVEL, "System Management API ERROR" );
        SCTP_INCR_ERRORSTAT( SCTPSTATS_API_ERRORS, 1 );
        *p_ecode = E_SCTP_INVALID_ERROR_LEVEL;

        return( SCTP_FAILURE );
    }


    /* if all alarm flag set for all */
    if( error_flag == SCTPERROR_FLAG_ALL_ERRORS )
    {
        for( counter = SCTP_MIN_ERROR_FLAG;
                counter <= SCTP_MAX_ERROR_FLAG;
                counter++ )
        {
            sctp_error_table[counter] = error_level;
        }
    }

    /* else set the level for the one specified */
    else
    {
        sctp_error_table[( sctp_U16bit )error_flag] = error_level;
    }

    return( SCTP_SUCCESS );
#else

    if( p_ecode != SCTP_NULL )
    {
        *p_ecode = E_ERRORDISABLED;
    }


    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( error_flag );
    SCTP_UNUSED_VARIABLE( error_level );
    /*to avoid the warning*/


    return ( SCTP_FAILURE );
#endif

}

#ifdef SCTP_ERROR_ENABLED

/***************************************************************************
 *   FUNCTION :
 *    sctp_err_reporter
 *
 *   DESCRIPTION:
 *
 *     This functions is called from all SCTP modules whenever  it
 *    reports any error to system management. This will check  the
 *    Current level of Error flag. If the error needs to be reported
 *    it will call a function to print this error.
 *
 *   RETURN VALUE:
 *      None
 *
 ****************************************************************************/
void
sctp_err_reporter(
    sctp_U32bit        error_type,
    sctp_U32bit        error_level,
    sctp_U32bit        ecode ,
    sctp_U8bit         *info_str )
{
    /* if errors enabled */
    if( sctp_error_state == 1 )
    {
        /* if the required level is set */
        if( ( sctp_error_table[error_type] != SCTPERROR_LEVEL_NOERROR )
                && ( error_level >= ( sctp_U32bit )sctp_error_table[error_type] ) )
            /*send alarm to  SME */
        {
            SCTP_ERROR_HANDLER( ecode, info_str );
        }
    }

    return;

}

/***************************************************************************
 *   FUNCTION :
 *    sctp_init_error_module
 *
 *   DESCRIPTION:
 *   This function enables error repoting and initialises the error level.
 *
 *   RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 ****************************************************************************/
sctp_return_t
sctp_init_error_module(
    sctp_error_t        *p_ecode )
{
    /* enable error reporting */
    sctp_error_state = 1;

    /** enable critical errors **/
    sctp_api_set_error_level( SCTPERROR_FLAG_ALL_ERRORS,
                              SCTPERROR_LEVEL_NOERROR, p_ecode );

    return( SCTP_SUCCESS );

}

/**************************************************************************

  For Exception Handling

***************************************************************************/

static sctp_sig_info_st sctp_sig_info;



/***************************************************************************
 *   FUNCTION :
 *   sctp_err_default_exception_handler
 *
 *   DESCRIPTION:
 *   This function calls the error handler
 *
 *   RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 ****************************************************************************/
sctp_return_t
sctp_err_default_exception_handler(
    sctp_signal_et              signo,
    sctp_error_t                *p_ecode )
{
    SCTP_UNUSED_VARIABLE( p_ecode );

    SCTP_ERROR_HANDLER( E_INTERNAL_ERROR,
                        "Caught an OS exception, terminatinmg..." );

    if( sctp_sig_info.user_handler_fn )
    {
        sctp_sig_info.user_handler_fn( signo, sctp_sig_info.arg );
    }

    /*can save the DB*/
    abort();
    return SCTP_SUCCESS;
}

#endif

/***************************************************************************
 *   FUNCTION :
 *    sctp_api_enable_exception_handling
 *
 *   DESCRIPTION:This function enables exception handling .
 *
 *
 *   RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 ****************************************************************************/
sctp_return_t
sctp_api_enable_exception_handling(
    user_handler_fn_pt          user_handler,
    void                       *arg,
    sctp_error_t                *p_ecode )
{
#ifdef SCTP_ERROR_ENABLED

    if( SCTP_TRUE != sctp_sig_info.enabled )
    {
        if( SCTP_SUCCESS == sctp_os_set_signals( SCTP_TRUE, p_ecode ) )
        {
            sctp_sig_info.user_handler_fn = user_handler;
            sctp_sig_info.enabled         = SCTP_TRUE;
            sctp_sig_info.arg             = arg;
            return SCTP_SUCCESS;
        }
    }

    if( SCTP_NULL != p_ecode )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;
    }

#endif
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( user_handler );
    SCTP_UNUSED_VARIABLE( arg );
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/


    return SCTP_FAILURE;
}


/***************************************************************************
 *   FUNCTION :
 *    sctp_api_disable_exception_handling
 *
 *   DESCRIPTION:This function disables exception handling.
 *
 *
 *   RETURN VALUE:
 *    SCTP_SUCCESS
 *    SCTP_FAILURE
 *
 ****************************************************************************/
sctp_return_t
sctp_api_disable_exception_handling(
    sctp_error_t                *p_ecode )
{
#ifdef SCTP_ERROR_ENABLED

    if( SCTP_TRUE == sctp_sig_info.enabled )
    {
        if( SCTP_SUCCESS == sctp_os_set_signals( SCTP_FALSE, p_ecode ) )
        {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            sctp_sig_info.user_handler_fn = SCTP_NULL;
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            sctp_sig_info.user_handler_fn = ( user_handler_fn_pt ) SCTP_NULL;
#endif
            sctp_sig_info.enabled         = SCTP_FALSE;
            sctp_sig_info.arg             = SCTP_NULL;
            return SCTP_SUCCESS;
        }
    }

    if( SCTP_NULL != p_ecode )
    {
        *p_ecode = E_REQ_IN_WRONG_STATE;
    }

#endif
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_ecode );
    /*to avoid the warning*/

    return SCTP_FAILURE;
}










