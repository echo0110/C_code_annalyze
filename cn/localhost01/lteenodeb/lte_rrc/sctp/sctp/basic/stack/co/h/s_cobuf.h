/***************************************************************************
 *  FILE NAME:    s_cobuf.h
 *
 *  DESCRIPTION:
 *    Buffer Manupulation related functions
 *
 *    DATE        NAME              REF#    REASON
 *    -------     ----------------  -----   --------------------------------
 *    23Mar00     sigtran team              Original
 *    14Sep01     Yogesh Gahlaut            Rel 3.0 Changes
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 **************************************************************************/
#ifndef    __FILE_sctp_buffer_SEEN__
#define    __FILE_sctp_buffer_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    extern sctp_U8bit *
    sctp_buffer_set_Buffer(
        sctp_U8bit    *buf,
        sctp_U32bit   count,
        sctp_U8bit    *newbuffer );

    extern sctp_U8bit *
    sctp_buffer_get_Buffer(
        sctp_U8bit    *buf,
        sctp_U32bit   count,
        sctp_U8bit    *newbuffer );

    extern sctp_U8bit *
    sctp_buffer_set_Uchar(
        sctp_U8bit    *buf,
        sctp_U8bit    value );

    extern sctp_U8bit *
    sctp_buffer_get_Uchar(
        sctp_U8bit    *buf,
        sctp_U8bit    *value );

    extern sctp_U8bit
    sctp_buffer_get_UcharAtOffset(
        sctp_U8bit    *buf,
        sctp_U32bit   offset );

    extern void
    sctp_buffer_set_UcharAtOffset(
        sctp_U8bit    *buf,
        sctp_U32bit   offset,
        sctp_U8bit    value );

    extern sctp_U8bit *
    sctp_buffer_set_Ushort(
        sctp_U8bit    *buf,
        sctp_U16bit   value );

    extern sctp_U8bit *
    sctp_buffer_get_Ushort(
        sctp_U8bit    *buf,
        sctp_U16bit   *value );

    extern sctp_U16bit
    sctp_buffer_get_UshortAtOffset(
        sctp_U8bit    *buf,
        sctp_U32bit   offset );

    extern void
    sctp_buffer_set_UshortAtOffset(
        sctp_U8bit    *buf,
        sctp_U32bit   offset,
        sctp_U16bit   value );

    extern sctp_U8bit *
    sctp_buffer_set_Ulong(
        sctp_U8bit    *buf,
        sctp_U32bit   value );

    extern sctp_U8bit *
    sctp_buffer_get_Ulong(
        sctp_U8bit    *buf,
        sctp_U32bit   *value );

    extern sctp_U32bit
    sctp_buffer_get_UlongAtOffset(
        sctp_U8bit    *buf,
        sctp_U32bit   offset );

    extern void
    sctp_buffer_set_UlongAtOffset(
        sctp_U8bit    *buf,
        sctp_U32bit   offset,
        sctp_U32bit   value );

    extern sctp_U8bit *
    sctp_buffer_setZero(
        sctp_U8bit    *buf,
        sctp_U32bit   count );

    extern sctp_U8bit *
    sctp_buffer_Skip(
        sctp_U8bit    *buf,
        sctp_U32bit   count );

    extern sctp_U8bit *
    sctp_buffer_PadTo(
        sctp_U8bit    *scan,
        sctp_U8bit    *start,
        sctp_U8bit    to );

    extern sctp_U8bit *
    sctp_buffer_PadToLen(
        sctp_U8bit    *start,
        sctp_U32bit    len );

    extern sctp_U8bit *
    sctp_buffer_pack_sockaddr(
        sctp_U8bit         *buf,
        sctp_sockaddr_st   *addr );

    extern sctp_U8bit *
    sctp_buffer_unpack_sockaddr(
        sctp_U8bit         *buf,
        sctp_sockaddr_st   *addr );

#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_buffer_SEEN__   */

