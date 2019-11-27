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
#include <cspl.h>
#include <sig_os_port.h>

sigtran_S8bit  *sig_os_get_ctime_r(
    sigtran_time_sec_t  *p_time,
    sigtran_S8bit       *p_time_buffer,
    sigtran_U32bit      *p_buf_len )
{
    p_time_buffer = p_time_buffer;
    p_buf_len = p_buf_len;
    return ctime( p_time );
}


