/***************************************************************************
 *  FILE NAME:   s_cobuf.c
 *
 *  DESCRIPTION:
 *   Buffer Manupulation related functions
 *
 *  DATE     NAME              REF#   REASON
 *  -------  ----------------  -----  -------------------------------------
 *  23Mar00  sigtran team             Original
 *  14Sep01  Yogesh Gahlaut           Rel 3.0 changes
 *  01Oct02  Manish Rajpal            Optimizations
 *  16Oct02  Manish Rajpal            SPR 4949
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **************************************************************************/

/*{{ Enter include files here }}*/
#include <s_os.h>
#include <s_cotyps.h>
#include <s_cobuf.h>
#include <s_osmem.h>



/****************************************************************************
 *  Function:    sctp_buffer_set_Buffer
 *
 *  Description:
 *      Add first count bytes from new buffer to the end of buf
 *      Return the end of the buf.
 *
 *  Returns:buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_set_Buffer(
    sctp_U8bit            *buf,
    sctp_U32bit           count,
    sctp_U8bit            *newbuffer )
{
    __MEMCOPY( buf, newbuffer, count );

    return ( buf + count );
}

/****************************************************************************
 *  Function:    sctp_buffer_get_Buffer
 *
 *  Description:
 *      Get count bytes from starting location *buf into newbuffer
 *      Return the updated ptr into buf.
 *
 *  Returns:buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_get_Buffer(
    sctp_U8bit            *buf,
    sctp_U32bit           count,
    sctp_U8bit            *newbuffer )
{

    __MEMCOPY( newbuffer, buf, count );

    return ( buf + count );
}

/****************************************************************************
 *  Function:    sctp_buffer_set_Uchar
 *
 *  Description:
 *      Add the value passed, to the end of bufffer, update buf pointer.
 *      Return the new buf ptr.
 *
 *  Returns:buffer offset
 *****************************************************************************/
sctp_U8bit  *
sctp_buffer_set_Uchar(
    sctp_U8bit            *buf,
    sctp_U8bit            value )
{
    buf[0] = value;
    return buf + 1;
}

/****************************************************************************
 *  Function:    sctp_buffer_get_Uchar
 *
 *  Description:
 *      Get one uchar from buf and advance buf pointer.
 *
 *  Returns:buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_get_Uchar(
    sctp_U8bit            *buf,
    sctp_U8bit            *value )
{
    *value = buf[0];
    return buf + 1;
}

/****************************************************************************
 *  Function:    sctp_buffer_get_UcharAtOffset
 *
 *  Description:
 *      Get one uchar from buf at offset from its present ptr location
 *
 *  Returns:buffer offset
 ****************************************************************************/
sctp_U8bit
sctp_buffer_get_UcharAtOffset(
    sctp_U8bit            *buf,
    sctp_U32bit            offset )
{
    return buf[offset];
}

/****************************************************************************
 *  Function:     sctp_buffer_set_UcharAtOffset
 *
 *  Description:  Sets a value in the buffer at the given offset.

 *
 *  Returns:      buffer offset
 *
 ****************************************************************************/
void
sctp_buffer_set_UcharAtOffset(
    sctp_U8bit            *buf,
    sctp_U32bit           offset,
    sctp_U8bit            value )
{
    buf[offset] =  value;
}

/****************************************************************************
 *  Function:     sctp_buffer_set_Ushort
 *
 *  Description:
 *        Append ushort value to buf
 *
 *  Returns: buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_set_Ushort(
    sctp_U8bit            *buf,
    sctp_U16bit           value )
{
    buf[0] = ( sctp_U8bit )( value >> 8 );
    buf[1] = ( sctp_U8bit )( value );
    return buf + 2;
}

/****************************************************************************
 *    Function:     sctp_buffer_get_Ushort
 *
 *    Description:
 *
 *             Get a  ushort value from buf, advance it
 *
 *   Returns:buffer offset
 ****************************************************************************/
sctp_U8bit    *
sctp_buffer_get_Ushort(
    sctp_U8bit            *buf,
    sctp_U16bit           *value )
{
    *value = ( buf[0] << 8 ) + buf[1];
    return buf + 2;
}

/****************************************************************************
 *    Function:     sctp_buffer_get_UshortAtOffset
 *
 *    Description:
 *
 *             Get a  ushort value from buf from an offset
 *
 *   Returns:buffer offset
 ****************************************************************************/
sctp_U16bit
sctp_buffer_get_UshortAtOffset(
    sctp_U8bit            *buf,
    sctp_U32bit           offset )
{
    return ( ( buf[offset] << 8 ) | buf[1 + offset] );
}

/****************************************************************************
 *    Function:     sctp_buffer_set_UshortAtOffset
 *
 *    Description:
 *
 *             Set a  ushort value in buf at an offset
 *
 *      Returns: None
 *
 ****************************************************************************/
void
sctp_buffer_set_UshortAtOffset(
    sctp_U8bit         *buf,
    sctp_U32bit        offset,
    sctp_U16bit        value )
{
    buf[offset]   = ( sctp_U8bit )( value >> 8 );
    buf[1 + offset] = ( sctp_U8bit )( value );
}

/****************************************************************************
 *    Function:     sctp_buffer_set_Ulong
 *
 *    Description:
 *
 *             Add a u long value  to buf, update buf ptr
 *
 *   Returns:buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_set_Ulong(
    sctp_U8bit         *buf,
    sctp_U32bit        value )
{
    buf[3] = ( sctp_U8bit )value;
    value >>= 8;

    buf[2] = ( sctp_U8bit )value;
    value >>= 8;

    buf[1] = ( sctp_U8bit )value;
    value >>= 8;

    buf[0] = ( sctp_U8bit )value;
    return buf + 4;
}

/****************************************************************************
 *    Function:     sctp_buffer_get_Ulong
 *
 *    Description:
 *
 *             Get a ulong value from buf, update buf ptr
 *
 *   Returns:buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_get_Ulong(
    sctp_U8bit         *buf,
    sctp_U32bit        *value )
{
    *value  = buf[0];
    *value  = *value  << 8;
    *value  |= buf[1];
    *value  = *value  << 8;
    *value  |= buf[2];
    *value  = *value  << 8;
    *value  |= buf[3];

    return buf + 4;
}

/****************************************************************************
 *    Function:     sctp_buffer_get_UlongAtOffset
 *
 *    Description:
 *
 *             Get a ulong value from buf from an offset
 *
 *    Returns:buffer offset
 ****************************************************************************/
sctp_U32bit
sctp_buffer_get_UlongAtOffset(
    sctp_U8bit     *buf,
    sctp_U32bit    offset )
{
    sctp_U32bit value = 0;

    value |= buf[offset + 0];
    value = value << 8;
    value |= buf[offset + 1];
    value = value << 8;
    value |= buf[offset + 2];
    value = value << 8;
    value |= buf[offset + 3];
    return value;
}

/****************************************************************************
 *    Function:     sctp_buffer_set_UlongAtOffset
 *
 *    Description:
 *
 *             Set a ulong value at buf at an offset
 *
 *    Returns:buffer offset
 ****************************************************************************/
void
sctp_buffer_set_UlongAtOffset(
    sctp_U8bit      *buf,
    sctp_U32bit     offset,
    sctp_U32bit     value )
{
    buf[3 + offset] = ( sctp_U8bit )value;
    value >>= 8;
    buf[2 + offset] = ( sctp_U8bit )value;
    value >>= 8;
    buf[1 + offset] = ( sctp_U8bit )value;
    value >>= 8;
    buf[0 + offset] = ( sctp_U8bit )value;
}


/****************************************************************************
 *    Function:     sctp_buffer_setZero
 *
 *    Description:
 *
 *             Add count 0s to the buf, update buf ptr
 *
 *    Returns:buffer offset
 ****************************************************************************/
sctp_U8bit    *
sctp_buffer_setZero(
    sctp_U8bit        *buf,
    sctp_U32bit       count )
{
    __MEMSET( buf, 0, count );
    return buf + count;
}

/****************************************************************************
 *    Function:     sctp_buffer_Skip
 *
 *    Description:
 *
 *             Jump ptr by count
 *
 *    Returns:buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_Skip(
    sctp_U8bit        *buf,
    sctp_U32bit       count )
{
    return buf + count;
}

/****************************************************************************
 *    Function:     sctp_buffer_PadTo
 *
 *    Description:
 *
 *             Pad to char from scan to start with zero
 *
 *    Returns: Pointer to the buffer
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_PadTo(
    sctp_U8bit        *scan,
    sctp_U8bit        *start,
    sctp_U8bit        to )
{
    sctp_U32bit mod = ( ( scan - start ) % to );

    if( mod )
    {
        __MEMSET( scan, 0, to - mod );
        return scan + to - mod;
    }

    return scan;
}


/****************************************************************************
 *    Function:     sctp_buffer_PadToLen
 *
 *    Description:
 *
 *             Pad to char from start upto next 'len' characters with zero
 *
 *    Returns: Pointer to the buffer
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_PadToLen(
    sctp_U8bit        *start,
    sctp_U32bit        len )
{
    if( len )
    {
        __MEMSET( start, 0, len );
        return start + len;
    }

    return start;
}

/****************************************************************************
 *  Function:     sctp_buffer_pack_sockaddr
 *
 *  Description:
 *         Set a  sockaddr_st value to buf, advance it
 *
 *    Returns:  buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_pack_sockaddr(
    sctp_U8bit            *buf,
    sctp_sockaddr_st      *addr )
{
    sctp_U8bit    *P = buf;
    sctp_U32bit   u32_temp = 0;

    u32_temp = ( sctp_U32bit )addr->family;
    P = sctp_buffer_set_Ulong( P, u32_temp );
    P = sctp_buffer_set_Ulong( P, addr->ip.v6.flow_info );
    P = sctp_buffer_set_Ulong( P, addr->ip.v6.scope_id );
    P = sctp_buffer_set_Ulong( P, addr->ip.v6.addr.s_u6_addr32[0] );
    P = sctp_buffer_set_Ulong( P, addr->ip.v6.addr.s_u6_addr32[1] );
    P = sctp_buffer_set_Ulong( P, addr->ip.v6.addr.s_u6_addr32[2] );
    P = sctp_buffer_set_Ulong( P, addr->ip.v6.addr.s_u6_addr32[3] );

    return P;
}


/****************************************************************************
 *  Function:     sctp_buffer_unpack_sockaddr
 *
 *  Description:
 *      Get a sockaddr_st value from buf, advance it
 *
 *    Returns:  buffer offset
 ****************************************************************************/
sctp_U8bit  *
sctp_buffer_unpack_sockaddr(
    sctp_U8bit            *buf,
    sctp_sockaddr_st      *addr )
{
    sctp_U8bit    *P = buf;
    sctp_U32bit   u32_temp = 0;

    P = sctp_buffer_get_Ulong( P, &u32_temp );
    addr->family = ( sctp_sa_family_t )u32_temp;
    P = sctp_buffer_get_Ulong( P, &addr->ip.v6.flow_info );
    P = sctp_buffer_get_Ulong( P, &addr->ip.v6.scope_id );
    P = sctp_buffer_get_Ulong( P, ( sctp_U32bit * )&addr->ip.v6.addr.s_u6_addr32[0] );
    P = sctp_buffer_get_Ulong( P, ( sctp_U32bit * )&addr->ip.v6.addr.s_u6_addr32[1] );
    P = sctp_buffer_get_Ulong( P, ( sctp_U32bit * )&addr->ip.v6.addr.s_u6_addr32[2] );
    P = sctp_buffer_get_Ulong( P, ( sctp_U32bit * )&addr->ip.v6.addr.s_u6_addr32[3] );

    return P;
}




