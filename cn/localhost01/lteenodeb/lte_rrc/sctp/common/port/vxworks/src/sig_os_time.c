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
 * 11Sept 11     Sandeep Singh  SPR-21022       Vxworks-6.8 Porting Changes
 *
 *  Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include <sig_os.h>
#include <cspl.h>

sigtran_S8bit  *sig_os_get_ctime_r(
    sigtran_time_sec_t  *p_sig_time,
    sigtran_S8bit       *p_time_buffer,
    sigtran_U32bit      *p_buf_len )
{
    /* Changes for SPR-21022 Starts here */
#ifdef VXWORKS_6_9_SUPPORT
    struct tm *tmp;
    tmp = localtime( p_sig_time );
    return ( sigtran_S8bit * ) strftime( ( sigtran_8bit * )p_time_buffer, ( size_t )p_buf_len, "\%a \%b \%d %H:%M:\%S", tmp );
#else
    return ctime_r( p_sig_time, p_time_buffer, ( size_t * )p_buf_len );
#endif
    /* Changes for SPR-21022 Ends here */
}

