#include "sig_os.h"

/******************************************************************************
** FUNCTION    :
**               sig_util_extract_1byte
**
** DESCRIPTION :
**               Extracts 1 bytes and move the ptr.
** Return Type :None
******************************************************************************/
void sig_util_extract_1byte
( sigtran_buffer_t  *p_buf,
  sigtran_U8bit     *p_value )
{
    *p_value = *( *p_buf )++;
}

/******************************************************************************
** FUNCTION    :
**               sig_util_extract_2byte
**
** DESCRIPTION :
**               Extracts 2 bytes and move the ptr.
** Return Type :None
******************************************************************************/
void sig_util_extract_2bytes
( sigtran_buffer_t   *p_buf,
  sigtran_U16bit    *p_value )
{
    *p_value = 0;
    *p_value = ( *( *p_buf )++ << 8 );
    *p_value = *p_value | ( *( *p_buf )++ );

}


/******************************************************************************
** FUNCTION    :
**               sig_util_extract_4byte
**
** DESCRIPTION :
**               Extracts 4 bytes and move the ptr.
** Return Type :None
******************************************************************************/
void sig_util_extract_4bytes
( sigtran_buffer_t   *p_buf,
  sigtran_U32bit    *p_value )
{

    *p_value = 0;

    *p_value = *p_value | ( ( *( *p_buf )++ ) << 24 );
    *p_value = *p_value | ( ( *( *p_buf )++ ) << 16 );
    *p_value = *p_value | ( ( *( *p_buf )++ ) << 8 );
    *p_value = *p_value | ( ( *( *p_buf )++ ) );
}


/******************************************************************************
** FUNCTION    :
**               sig_util_set_1byte
**
** DESCRIPTION :
**               Set 1 byte and move the ptr.
**
** Return Type :None
******************************************************************************/
void sig_util_set_1byte
( sigtran_buffer_t  *p_buf,
  sigtran_U8bit     value )
{
    *( *p_buf )++ = value;
}


/******************************************************************************
** FUNCTION    :
**               sig_util_set_2byte
**
** DESCRIPTION :
**               Set 2 byte and move the ptr.
**
** Return Type :None
******************************************************************************/
void sig_util_set_2bytes
( sigtran_buffer_t   *p_buf,
  sigtran_U16bit     val )
{

    *( *p_buf )++ = ( sigtran_U8bit )( val >> 8 );
    *( *p_buf )++ = ( sigtran_U8bit )( val );
}


/******************************************************************************
** FUNCTION    :
**               sig_util_set_4byte
**
** DESCRIPTION :
**               Set 4 byte and move the ptr.
**
** Return Type :None
******************************************************************************/
void sig_util_set_4bytes
( sigtran_buffer_t   *p_buf,
  sigtran_U32bit     val )
{

    *( *p_buf )++ = ( sigtran_U8bit )( val >> 24 );
    *( *p_buf )++ = ( sigtran_U8bit )( val >> 16 );
    *( *p_buf )++ = ( sigtran_U8bit )( val >> 8 );
    *( *p_buf )++ = ( sigtran_U8bit )( val );
}
/******************************************************************************
** FUNCTION    :
**               sig_util_ext_ip_port_frm_ipc_info
**
** DESCRIPTION :
**               This function will export the Ip port from IPC information
**
** Return Type :SIGTRAN_SUCEESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sig_util_ext_ip_port_frm_ipc_info( sig_ip_addr_st ipc_info,
                                                    sigtran_U32bit *p_ip_addr,
                                                    sigtran_U16bit *p_port,
                                                    sigtran_error_t *p_ecode )
{
    /* TBD need to put verifications here */
    if( ipc_info.ip_type == SIG_TRANS_ADDR_IPV4 )
    {
        *p_ip_addr = ipc_info.ip_addr_ut.addr_v4.addr;
        *p_port = ipc_info.ip_addr_ut.addr_v4.port;
    }

    return SIGTRAN_SUCEESS;
}
/******************************************************************************
** FUNCTION    :
**               sig_genrate_public_name
**
** DESCRIPTION :
**               This function generates the unique public name
**
** Return Type :SIGTRAN_SUCEESS/SIGTRAN_FAILURE
******************************************************************************/

sigtran_return_t sig_genrate_public_name( sigtran_U32bit stack_type,
                                          sigtran_U16bit *p_handle,
                                          sigtran_U32bit *p_ext_mod_id )
{
    /* this function genrates a unique public name
     * for doing set destination in cspl
     * set first 16bit to stacktype and next 16bit of handle
     * type to genrate the ext module id
     */
    *p_ext_mod_id = 0;
    *p_ext_mod_id = ( stack_type << 16 )0xffff0000;
    *p_ext_mod_id = *p_ext_mod_id | p_handle;
    return SIGTRAN_SUCEESS;
}


}



