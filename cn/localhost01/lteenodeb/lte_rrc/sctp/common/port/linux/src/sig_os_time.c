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
/******************************************************************************
 * **  FUNCTION :   sig_os_get_ctime_r
 * **
 * **  DESCRIPTION:  To get the time in character string format.
 * **
 * **  RETURN VALUE: pointer to character string
 * ******************************************************************************/

sigtran_S8bit   *sig_os_get_ctime_r(
    sigtran_time_sec_t  *p_time,
    sigtran_S8bit       *p_time_buffer,
    sigtran_U32bit      *p_buf_len )
{
    p_buf_len = p_buf_len;
    return ( sigtran_S8bit * )( ctime_r( ( time_t * )p_time, ( char * ) p_time_buffer ) );
}


