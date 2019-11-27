#include <sig_typs.h>
#include <sig_util.h>
#include <sig_dbg.h>
#include <sig_err.h>

/******************************************************************************
** FUNCTION    :
**               sig_util_extract_1byte
**
** DESCRIPTION :
**               Extracts 1 bytes and move the ptr.
**  RETURN VALUE: None
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
**
**  RETURN VALUE: None
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
**
**  RETURN VALUE: None
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
**  RETURN VALUE: None
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
**  RETURN VALUE: None
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
**  RETURN VALUE: None
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

sigtran_return_t sig_util_ext_ip_port_frm_ipc_info(
    sig_ip_addr_st  *p_ipc_info,
    sigtran_U32bit  *p_ip_addr,
    sigtran_U16bit  *p_port,
    sigtran_U32bit  *p_flow_info,
    sigtran_U32bit  *p_scope_id,
    sigtran_error_t *p_ecode )
{
    if( p_ipc_info->ip_type == SIG_TRANS_ADDR_IPV4 )
    {
        /* CSR 69326 fix starts*/
        p_ip_addr[0] = p_ipc_info->ip_addr_ut.addr_v4.addr;
        /* CSR 69326 fix ends*/
        *p_port = p_ipc_info->ip_addr_ut.addr_v4.port;
    }

    else
    {
        /*
         * IP Type is IPV6.
         */
        p_ip_addr[0] = p_ipc_info->ip_addr_ut.addr_v6.ip_addr_ut.addr32[0];
        p_ip_addr[1] = p_ipc_info->ip_addr_ut.addr_v6.ip_addr_ut.addr32[1];
        p_ip_addr[2] = p_ipc_info->ip_addr_ut.addr_v6.ip_addr_ut.addr32[2];
        p_ip_addr[3] = p_ipc_info->ip_addr_ut.addr_v6.ip_addr_ut.addr32[3];

        if( p_port )
        {
            *p_port = p_ipc_info->ip_addr_ut.addr_v6.port;
        }

        if( p_flow_info )
        {
            *p_flow_info = p_ipc_info->ip_addr_ut.addr_v6.flow_info;
        }

        if( p_scope_id )
        {
            *p_scope_id  = p_ipc_info->ip_addr_ut.addr_v6.scope_id;
        }
    }

    SIGTRAN_SET_ECODE( p_ecode , ESIG_NO_ERROR );
    return SIGTRAN_SUCCESS;
}
sigtran_return_t sig_genrate_public_name( sig_stack_type_et stack_type,
                                          sig_dest_type_et  dest_type,
                                          sigtran_U16bit    handle,
                                          sigtran_U32bit    *p_ext_mod_id )
{
    /* this function genrates a unique public name
     * for doing set destination in cspl
     * set first 8bit stack type,next 8bit
     * are dest type
     * and next 16bit of handle
     */

    *p_ext_mod_id = ( stack_type << 24 );
    *p_ext_mod_id |= ( dest_type << 16 );
    *p_ext_mod_id |= handle;

    return SIGTRAN_SUCCESS;
}

sigtran_return_t sig_get_ipc_info_frm_service_name
( sigtran_UL32bit  service_name,
  sig_stack_type_et *p_stack_type,
  sig_dest_type_et  *p_dest_type,
  sigtran_U16bit    *p_handle,
  sigtran_error_t   *p_ecode )
{
    /* this function extracts stack type ,destination type
     * and ipc handle from the service name given
     * first 8 bits are stack_type,next 8 bit are dest type,
     * next 16 bit is handle
     */
    sigtran_U32bit input;

    input = ( sigtran_U32bit ) service_name;

    *p_stack_type = input >> 24;
    *p_dest_type = ( input >> 16 ) & 0x000000ff;
    *p_handle = input & 0x0000ffff;

    SIGTRAN_SET_ECODE( p_ecode , ESIG_NO_ERROR );
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_memcpy
**
**  DESCRIPTION:  This function Copies size bytes from p_mem2 to p_mem1
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_memcpy(
    sigtran_pvoid_t         p_mem1,
    sigtran_pvoid_t         p_mem2,
    sigtran_U32bit          size )
{
    /* vxworks porting */
    if( memcpy( p_mem1, p_mem2, size ) )
    {
        return SIGTRAN_SUCCESS;
    }

    else
    {
        return SIGTRAN_FAILURE;
    }
}
/******************************************************************************
**  FUNCTION :    sigtran_memset
**
**  DESCRIPTION:  This function Sets "Size" bytes starting from p_mem to val.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
******************************************************************************/
sigtran_return_t sigtran_memset(
    sigtran_pvoid_t         p_mem,
    sigtran_U32bit          val,
    sigtran_U32bit          size )
{
    /* vxworks porting */
    if( memset( p_mem, val, size ) )
    {
        return SIGTRAN_SUCCESS;
    }

    else
    {
        return SIGTRAN_FAILURE;
    }
}
/******************************************************************************
**  FUNCTION :    sigtran_memcmp
**
**  DESCRIPTION:  This function compares its arguments, looking at the first
**                'size' bytes.
**
**  RETURN VALUE: Value less than, equal to, or greater than 0, according as
**                p_mem1 is lexicographically less than, equal to, or greater
**                than p_mem2
******************************************************************************/
sigtran_S32bit sigtran_memcmp(
    sigtran_pvoid_t         p_mem1,
    sigtran_pvoid_t         p_mem2,
    sigtran_U32bit          size )
{
    return memcmp( p_mem1, p_mem2, size );
}
/******************************************************************************
**  FUNCTION :    sigtran_strcmp
**
**  DESCRIPTION:  Compares two 'null-terminated 'strings byte-by-byte,
**                according to the ordering of the machine's character set.
**
**  RETURN VALUE: Value greater than, equal to, or less than 0, if the
**                string pointed to by p_str1 is greater than, equal to, or less
**                than the string pointed to by p_str2 respectively
******************************************************************************/
sigtran_S32bit sigtran_strcmp(
    sigtran_pvoid_t         p_str1,
    sigtran_pvoid_t         p_str2 )
{
    return strcmp( p_str1, p_str2 );
}
/******************************************************************************
**  FUNCTION :    sigtran_strncmp
**
**  DESCRIPTION:  Compares 'size' bytes of two strings byte-by-byte,
**                according to the ordering of the machine's character set.
**
**  RETURN VALUE: Value greater than, equal to, or less than 0, if the
**                string pointed to by p_str1 is greater than, equal to, or less
**                than the string pointed to by p_str2 respectively
******************************************************************************/
sigtran_S32bit sigtran_strncmp(
    sigtran_pvoid_t         p_str1,
    sigtran_pvoid_t         p_str2,
    sigtran_U32bit          size )
{
    return strncmp( p_str1, p_str2, size );
}
/******************************************************************************
**  FUNCTION :    sigtran_strcpy
**
**  DESCRIPTION:   Copies string src to dst including the  terminating
**                 null  character,  stopping after the null character has been
**                 copied
**
**  RETURN VALUE:  Dst string is returned.
******************************************************************************/
sigtran_pvoid_t sigtran_strcpy(
    sigtran_pvoid_t         p_dst,
    sigtran_pvoid_t         p_src )
{
    return strcpy( p_dst, p_src );
}
/******************************************************************************
**  FUNCTION :    sigtran_strlen
**
**  DESCRIPTION:  Returns the string length of the null terminated string.
**
**  RETURN VALUE: Number of bytes in p_str, not including the
     terminating null character.
******************************************************************************/
sigtran_U32bit sigtran_strlen(
    sigtran_pvoid_t         p_str )
{
    return strlen( p_str );
}
/******************************************************************************
**  FUNCTION :    sigtran_strrchr
**
**  DESCRIPTION:  Returns the pointer to last occurance of char into the string.
**
**  RETURN VALUE: Pointer to last occurance of the char into string
******************************************************************************/
sigtran_pvoid_t sigtran_strrchr(
    sigtran_pvoid_t         p_str,
    sigtran_U32bit          chr )
{
    return strrchr( p_str, chr );
}







