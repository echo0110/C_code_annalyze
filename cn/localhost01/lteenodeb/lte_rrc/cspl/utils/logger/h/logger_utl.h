/*****************************************************************************
*
* FILENAME      : logger_utl.h
*
* DESCRIPTION   : This file contains the function prototype used in the
*                 logger_utl.c 
*
*
*
* DATE              NAME            REFERENCE           REASON
* 11 Oct, 2001      kumarp           original
*  6 Feb, 2003      svaid            modified
*
*
*  Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/

#ifndef _LOGGER_UTL_H_
#define _LOGGER_UTL_H_

#define LT_SHIFT <<
#define RT_SHIFT >>

#define FOURTH_BYTE(x)       (((x&0xff000000)>>24)&0xff)
#define THIRD_BYTE(x)        (((x&0x00ff0000)>>16)&0xff)
#define SECOND_BYTE(x)       (((x&0x0000ff00)>>8 )&0xff)
#define FIRST_BYTE(x)        (((x&0x000000ff)>>0 )&0xff)


/*** function declaration ***/
void pack_u8(U8 **p_to, U8 *p_from);
void pack_u16(U8 **p_to, U16 *p_from);
void pack_u32(U8 **p_to, U32 *p_from);
void xtract_u8(U8 *p_to, U8 **p_from);
void xtract_u16(U16 *p_to, U8 **p_from);
void xtract_u32(U32 *p_to, U8 **p_from);
void setu16at(U8 **p_buf, U16 *p_value , U16 offset);
void setu32at(U8 **p_buf, U32 *p_value , U16 offset);
void setu8at(U8 **p_buf, U8 *p_value , U16 offset);
U8 getu8at(U8 *p_buf, U16 offset);
U16 getu16at(U8 *p_buf, U16 offset);
U32 getu32at(U8 *p_buf, U16 offset);
void fillapiheader(U8** p_buffer, U8 version, U16 src_id, \
                     U16 dst_id, U16 api_id, U16 buf_len,\
                     U16 start_payload, U16 payload_len, \
                     U8 priority);
U16 getbuflen(U8* p_buf, U8* end);

void logger_copy(U8** to, U8** from, U16 length);
U32 string_to_u32(U8 s[]);
U16 string_to_u16(U8 s[]);
U8 string_to_u8(U8 s[]);
#endif

