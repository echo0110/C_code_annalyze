/*****************************************************************************
*    FILE NAME:
*        sig_tmr.c
*
*    DESCRIPTION:
*       This file has porting function for Timer Related functions.
*
*    DATE           NAME          REFERENCE        REASON
*    ----------------------------------------------------
*    14Sept02    Sandeep Mahajan     -            .Original Creation
*    10Aug11       Supreet Jain   SPR 21004          CSR 5758
*
*     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/
#include <sig_os.h>


#ifdef SIGTRAN_LOCAL_TIME_THREAD
static sigtran_time_t  sigtran_local_sys_time;
void *sigtran_local_time_thread( void    *A );
#endif


/************************************************************************
** FUNCTION    : sua_create_thread
**
** DESCRIPTION : start a new thread
**
** RETURN VALUE: SIGTRAN_SUCCESS
*************************************************************************/
#ifndef SIGTRAN_SINGLE_THREAD
sigtran_return_t sigtran_create_thread
( sigtran_thread_t             *p_th_id,
  sigtran_start_func_ptr_t     start_func,
  sigtran_pvoid_t               args,
  sigtran_error_t              *p_ecode )
{
    SIGTRAN_SET_ECODE( p_ecode , ESIG_NO_ERROR );
#ifdef SIG_COMMON_WITH_CSPL
    qvThreadCreate( p_th_id, start_func, args );
#else
    sg_create_thread( p_th_id, start_func, args );
#endif
    return( SIGTRAN_SUCCESS );
}
#endif
#ifdef SIGTRAN_LOCAL_TIME_THREAD
/***************************************************************************
 *    FUNCTION :
 *        sigtran_create_localtime_thread
 *
 *    DESCRIPTION:
 *        This function implement the sigtran Clock thread.
 *        It updates the time
 *
 *    RETURN VALUE:SIGTRAN_FAILURE/SIGTRAN_SUCCESS
 *
 **************************************************************************/
sigtran_return_t sigtran_create_localtime_thread( sigtran_error_t *p_ecode )
{
    sigtran_error_t ecode;
    /* SPR 21004 Starts */
    sigtran_thread_t  th_id = 0;
    /* SPR 21004 Ends */

    if( sigtran_create_thread( ( sigtran_thread_t * )&th_id,
                               &sigtran_local_time_thread, ( void * )SIGTRAN_NULL,
                               &ecode ) != SIGTRAN_SUCCESS )
    {
        SIGTRAN_SET_ECODE( p_ecode, ecode );
        return ( SIGTRAN_FAILURE );
    }

    return SIGTRAN_SUCCESS;
}

/***************************************************************************
 *    FUNCTION :
 *        sigtran_local_time_thread
 *
 *    DESCRIPTION:
 *        This function updates the local time.
 *
 *    RETURN VALUE: NONE
 **************************************************************************/
void *sigtran_local_time_thread( void    *A )
{
    sigtran_U32bit usec_interval = SIG_TIME_UPDATE_INTERVAL * 1000;
    sigtran_time_t timeout;

    /* vxworks porting */
#if defined(SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE)
    sigtran_gettime_t  gettime;

    if( clock_gettime( CLOCK_REALTIME, &gettime ) != 0 )
#endif

#if defined(SIGTRAN_PORT_SOLARIS) || defined(SIGTRAN_PORT_LYNX) || defined(SIGTRAN_PORT_LINUX)
        if( gettimeofday( &sigtran_local_sys_time, 0 ) != 0 )
#endif

#ifdef SIGTRAN_PORT_WINNT
            struct _timeb    ts;

    _ftime( &ts );

    sigtran_local_sys_time.tv_sec = ts.time;
    sigtran_local_sys_time.tv_usec = ts.millitm * 1000;

    if( sigtran_local_sys_time.tv_sec < 0 )
#endif
    {
        return SIGTRAN_FAILURE;
    }

#if defined(SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE)
    sigtran_local_sys_time.tv_sec  = gettime.tv_sec;
    sigtran_local_sys_time.tv_usec = gettime.tv_nsec / 1000;
#endif

    A = A; /* To remove Unused Variable Warning */

    for( ; ; )
    {
        timeout.tv_sec  = 0;
        timeout.tv_usec = usec_interval;

        if( select( 0, NULL, NULL, NULL, &timeout ) < 0 )
        {
            continue;
        }

        /* update local system time */
        sigtran_local_sys_time.tv_usec += usec_interval;
        sigtran_local_sys_time.tv_sec  += sigtran_local_sys_time.tv_usec / 1000000;
        sigtran_local_sys_time.tv_usec  = sigtran_local_sys_time.tv_usec % 1000000;
    }
}

/***************************************************************************
 *    FUNCTION :
 *        sigtran_get_local_time
 *
 *    DESCRIPTION:
 *        This function assigns the Local Time Value in the
 *        input parameter.
 *
 *    RETURN VALUE:
 *        None
 **************************************************************************/

void sigtran_get_local_time( sigtran_time_t *p_sig_time )
{
    p_sig_time->tv_sec  = sigtran_local_sys_time.tv_sec;
    p_sig_time->tv_usec = sigtran_local_sys_time.tv_usec;
}

#endif

