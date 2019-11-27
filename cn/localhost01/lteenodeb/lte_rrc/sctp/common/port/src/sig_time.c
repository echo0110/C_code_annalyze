/*****************************************************************************
 *    FILE NAME:
 *        sig_time.c
 *
 *    DESCRIPTION:
 *       This file has porting function for System Time Related functions.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    14Sept02    Sandeep Mahajan     -            .Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include "sig_os.h"
#ifdef SIGTRAN_PORT_WINNT
#include <sys/timeb.h>
#endif

/******************************************************************************
**  FUNCTION : sigtran_get_time
**
**  DESCRIPTION: Returns the value of time in Seconds.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_get_time(
    sigtran_time_sec_t           *p_sig_time )
{
    if( time( p_sig_time ) == ( sigtran_time_sec_t ) - 1 )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION : sigtran_gettimeofday
**
**  DESCRIPTION: This function will get the Date and Time
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_gettimeofday(
    sigtran_time_t           *p_sig_time )
{
#ifdef SIGTRAN_LOCAL_TIME_THREAD

    sigtran_get_local_time( p_sig_time );

#else /* SIGTRAN_LOCAL_TIME_THREAD */

    /* vxworks porting */
#if defined(SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE)
    sigtran_gettime_t  gettime;

    if( clock_gettime( CLOCK_REALTIME, &gettime ) != 0 )
#endif
#if defined(SIGTRAN_PORT_LINUX)
#if defined(SIGTRAN_CLOCK_MONOTONIC)
#if defined(CLOCK_MONOTONIC)

        sigtran_gettime_t  gettime;

    if( clock_gettime( CLOCK_MONOTONIC, &gettime ) != 0 )
#else
#error "clock_gettime not supported with clock_id = CLOCK_MONOTONIC"
#endif
#endif
#endif
#if defined(SIGTRAN_PORT_SOLARIS) || defined(SIGTRAN_PORT_LYNX) || (defined(SIGTRAN_PORT_LINUX) && !defined(SIGTRAN_CLOCK_MONOTONIC) )
        if( gettimeofday( p_sig_time, NULL ) != 0 )
#endif
#ifdef SIGTRAN_PORT_WINNT
            struct _timeb    ts;

    _ftime( &ts );

    p_sig_time->tv_sec = ts.time;

    p_sig_time->tv_usec = ts.millitm * 1000;

    if( p_sig_time->tv_sec < 0 )
#endif

    {
        return SIGTRAN_FAILURE;
    }

#if defined(SIGTRAN_PORT_VXWORKS) || defined(SIGTRAN_PORT_VX_PNE) || (defined(SIGTRAN_PORT_LINUX) && defined (SIGTRAN_CLOCK_MONOTONIC))

    p_sig_time->tv_sec = gettime.tv_sec;
    p_sig_time->tv_usec = gettime.tv_nsec / 1000;
#endif
#endif  /* else of SIGTRAN_LOCAL_TIME_THREAD */
    return SIGTRAN_SUCCESS;
}


/******************************************************************************
**  Function Name       : sigtran_get_ctime_r
**
**    Description       :This function will get thet time in character string
**
**    Returns           :Pointer to chracter string
**
**
******************************************************************************/
sigtran_S8bit *sigtran_get_ctime_r(
    sigtran_time_sec_t  *p_sig_time,
    sigtran_S8bit       *p_time_buffer,
    sigtran_U32bit      *p_buf_len )
{
    return sig_os_get_ctime_r( p_sig_time, p_time_buffer, p_buf_len );
}




