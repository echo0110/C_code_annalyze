/******************************************************************************
 * File Name:
 *      s_cspl_mutex.c
 *
 * Description:
 *      This file contains definition of semaphore related functions which
 *      will be different for various platforms.
 *
 * DATE          NAME           REFERENCE       REASON
 * ---------------------------------------------------
 * 06 Oct 03     Vivek Bansal
 *
 *****************************************************************************/
#include <sig_os.h>
#ifdef SIG_COMMON_WITH_CSPL
#include <cspl.h>
#endif
#include <sig_os_port.h>

sigtran_S8bit  *sig_os_get_ctime_r(
    sigtran_time_sec_t  *p_time,
    sigtran_S8bit       *p_time_buffer,
    sigtran_U32bit      *p_buf_len )
{
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    return ctime_r( p_time, p_time_buffer, *p_buf_len );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    return ( sigtran_S8bit * )ctime_r( p_time, ( sigtran_8bit * )p_time_buffer, *p_buf_len );
#endif
}


