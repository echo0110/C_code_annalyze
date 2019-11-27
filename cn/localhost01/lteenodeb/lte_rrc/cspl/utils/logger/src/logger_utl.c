/*****************************************************************************
*
* FILENAME      :  logger_utl.c
*
* DESCRIPTION   :  This file contains the utility function used in SME.
*
*
*
* DATE              NAME            REFERENCE           REASON
* 07 Oct, 2001      kumarp           original
* 06 Feb, 2003      svaid            modified
*
*
*
*  Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/

#include "logger.h"

/*****************************************************************************
*
* FUNCTION NAME : pack_u8
*
* DESCRIPTION   :This function Packs U8 element to the buffer p_to from 
*                p_from
*                
*
* PARAMETERS    :1. Pointer to the buffer
*                2. Pointer to the U8 element value 
*
* RETURNS       : void
*
*****************************************************************************/

void pack_u8(U8** p_to, U8 *p_from)
{
	**p_to = *p_from;
	++(*p_to);
}

/*****************************************************************************
*
* FUNCTION NAME : pack_u16
*
* DESCRIPTION   : This functions packs the 16 short integer into the buffer
*
* PARAMETERS    :1. Pointer to the pointer of buffer
*                2. Pointer of U16 element value.
*
* RETURNS       :void
*
*****************************************************************************/

void pack_u16(U8 **p_to, U16 *p_from)
{
	U8 lsb = (U8)(*p_from & 0x00FF);

	**p_to = (U8)((*p_from RT_SHIFT 8) & 0x00FF);
	++(*p_to);
	**p_to = lsb;
	++(*p_to);
} 

/*****************************************************************************
*
* FUNCTION NAME : pack_u32
*
* DESCRIPTION   : This function packs the U32 element into the buffer.
*
* PARAMETERS    :1. Pointer to the pointer of buffer.
*                2. Pointer to the U32 element value.
*
* RETURNS       :void
*
*****************************************************************************/
void pack_u32(U8 **p_to, U32 *p_from)
{
	U8 lsb1 = (U8) (*p_from & 0x000000FF);
	U8 lsb2 = (U8) ((*p_from RT_SHIFT 8) & 0x000000FF);
	U8 lsb3 = (U8) ((*p_from RT_SHIFT 16) & 0x000000FF);

	**p_to = (U8)((*p_from RT_SHIFT 24) & 0x000000FF);
    ++(*p_to);
	**p_to = lsb3;
	++(*p_to);
	**p_to = lsb2;
	++(*p_to);
	**p_to = lsb1;
	++(*p_to);
}

/*****************************************************************************
*
* FUNCTION NAME : xtract_u8
*
* DESCRIPTION   : This function extract U8 element from the buffer.
*
* PARAMETERS    :1. Pointer to the U8 element value.
*                2. Pointer to the pointer of buffer
*
* RETURNS       : void
*
*****************************************************************************/
void xtract_u8(U8 *p_to, U8 **p_from)
{
	*p_to = **p_from;
	++(*p_from);
}

/*****************************************************************************
*
* FUNCTION NAME : xtract_u16
*
* DESCRIPTION   : This function extract U16 element from the buffer
*
* PARAMETERS    :1. Pointer to the U16 element value.
*                2. Pointer to the pointer of buffer
*
* RETURNS       :void
*
*****************************************************************************/
void xtract_u16(U16 *p_to, U8 **p_from)
{
    *p_to = (((U16)(**p_from)) LT_SHIFT 8) & 0xff00;
    ++(*p_from);
	*p_to |= (U16)(**p_from);
    ++(*p_from);
}

/*****************************************************************************
*
* FUNCTION NAME : xtract_u32
*
* DESCRIPTION   : This function extract U32 element from the buffer
*
* PARAMETERS    :1. Pointer to the U32 element value.
*                2. Pointer to the pointer of buffer
*
* RETURNS       :void
*
*****************************************************************************/
void xtract_u32(U32 *p_to, U8 **p_from)
{
	*p_to = (((U32)(**p_from)) LT_SHIFT 24 ) & 0xFF000000;
	++(*p_from);
	*p_to |= (((U32)(**p_from)) LT_SHIFT 16 ) & 0xFF0000;
	++(*p_from);
	*p_to |= (((U32)(**p_from)) LT_SHIFT 8 ) & 0xFF00;
	++(*p_from);
	*p_to |= (U32)(**p_from);
	++(*p_from);
}
	

/*****************************************************************************
*
* FUNCTION NAME : getu16at
*
* DESCRIPTION   : This function extracts the U16 value from the buffer
*                 from the specified offset.
*
* PARAMETERS    :1. Pointer to the buffer
*                2. Offset value
*
* RETURNS       :U16 element value at the specified offset of buffer
*
*****************************************************************************/
U16 getu16at(U8 *p_buf, U16 offset)
{
	U16 value;
	
	p_buf = p_buf + offset;

	value = (((U16)(*p_buf)) LT_SHIFT 8) & 0xff00;
    ++(p_buf);
    value |= (U16)(*p_buf);

	return value;
}

/*****************************************************************************
*
* FUNCTION NAME : getu32at
*
* DESCRIPTION   : This function extracts the U32 value from the buffer
*                 from the specified offset.
*
* PARAMETERS    :1. Pointer to the buffer
*                2. Offset value
*
* RETURNS       :U32 element value at the specified offset of buffer
*
*****************************************************************************/
U32 getu32at(U8 *p_buf, U16 offset)
{
	U32 value;
	
	p_buf = p_buf + offset;

	value = (((U32)(*p_buf)) LT_SHIFT 24) & 0xff000000;
    ++(p_buf);
	value |= (((U32)(*p_buf)) LT_SHIFT 16) & 0xff0000;
    ++(p_buf);
	value |= (((U32)(*p_buf)) LT_SHIFT 8) & 0xff00;
    ++(p_buf);
	value |= (U32)(*p_buf);
    ++(p_buf);

	return value;
}

/*****************************************************************************
*
* FUNCTION NAME : getu8at
*
* DESCRIPTION   : This function extracts the U8 value from the buffer
*                 from the specified offset.
*
* PARAMETERS    :1. Pointer to the buffer
*                2. Offset value
*
* RETURNS       :U8 element value at the specified offset of buffer
*
*****************************************************************************/
U8 getu8at(U8 *p_buf, U16 offset)
{
	U8 value;
	
	p_buf = p_buf + offset;
    value = (*p_buf);

	return value;
}

/*****************************************************************************
*
* FUNCTION NAME : setu16at
*
* DESCRIPTION   : This function sets the U16 element value to the buffer
*                  at the specified offset.
*
* PARAMETERS    :1. Pointer to pointer of the buffer
*                2. Pointer to the U16 element value
*                3. Offset value
*
* RETURNS       :void
*
*****************************************************************************/
void setu16at(U8 **p_buf, U16 *p_value , U16 offset)
{
    U8 lsb = (U8)(*p_value & 0x00FF);
	U8 *p_buf_tmp = NULL;
	p_buf_tmp = *p_buf;
	*p_buf = (*p_buf + offset);
    **p_buf = (U8)((*p_value RT_SHIFT 8) & 0x00FF);
    ++(*p_buf);
    **p_buf = lsb;
	*p_buf = p_buf_tmp;
}

/*****************************************************************************
*
* FUNCTION NAME : setu32at
*
* DESCRIPTION   : This function sets the U32 element value to the buffer
*                  at the specified offset.
*
* PARAMETERS    :1. Pointer to pointer of the buffer
*                2. Pointer to the U16 element value
*                3. Offset value
*
* RETURNS       :void
*
*****************************************************************************/
void setu32at(U8 **p_buf, U32 *p_value , U16 offset)
{

	U8 lsb1 = (U8) (*p_value & 0x000000FF);
    U8 lsb2 = (U8) ((*p_value RT_SHIFT 8) & 0x000000FF);
    U8 lsb3 = (U8) ((*p_value RT_SHIFT 16) & 0x000000FF);

	*p_buf = (*p_buf + offset);
    **p_buf = (U8)((*p_value RT_SHIFT 24) & 0x000000FF);
    ++(*p_buf);
    **p_buf = lsb3;
    ++(*p_buf);
    **p_buf = lsb2;
    ++(*p_buf);
    **p_buf = lsb1;
	
	*p_buf = (*p_buf - offset - 3);
}

/*****************************************************************************
*
* FUNCTION NAME : setu8at
*
* DESCRIPTION   : This function sets the U8 element value to the buffer
*                  at the specified offset.
*
* PARAMETERS    :1. Pointer to pointer of the buffer
*                2. Pointer to the U16 element value
*                3. Offset value
*
* RETURNS       :void
*
*****************************************************************************/
void setu8at(U8 **p_buf, U8 *p_value , U16 offset)
{
	*p_buf = (*p_buf + offset);	

	**p_buf = *p_value;
	*p_buf = (*p_buf - offset);
}

/*****************************************************************************
*
* FUNCTION NAME : fillapiheader
*
* DESCRIPTION   : This Function Fills API HEADER of SME
*
* PARAMETERS    : 1. p_buffer => pointer to start of api buffer.
*                 2. version  => SME VERSION
*                 3. src_id   => Source ID
*                 4. dst_id   => Destination ID
*                 5. api_id   => API IDENTIFIER
*                 6. buf_len  => number of octects
*                 7. start_payload => start of payload buffer
*                 8  payload_len => payload length
*                 9. priority => Priority of the message
*
* RETURNS       : void
*
*****************************************************************************/
void fillapiheader(U8** p_buf, U8 version, U16 src_id, \
					 U16 dst_id, U16 api_id, U16 buf_len,\
					 U16 start_payload, U16 payload_len, \
					 U8 priority)
{
	U8 *p_buffer 	= *p_buf;
	U16 unused 		= 0;

	pack_u8(&p_buffer,  &version);
	pack_u16(&p_buffer, &src_id);
	pack_u16(&p_buffer, &dst_id);
	pack_u16(&p_buffer, &api_id);
	pack_u16(&p_buffer, &buf_len);
	pack_u16(&p_buffer, &start_payload);
	pack_u16(&p_buffer, &payload_len);
	pack_u8(&p_buffer,  &priority);
	pack_u16(&p_buffer, &unused);

	*p_buf = p_buffer;
}
    	
/*****************************************************************************
*
* FUNCTION NAME : logger_copy
*
* DESCRIPTION   : This function copies "from" buffer to the "to" buffer to 
*                 the specified length given in "length"
*
* PARAMETERS    :1. destination buffer
*                2. source buffer
*                3. length to be copied
*
* RETURNS       :void
*
*****************************************************************************/
void logger_copy(U8** to, U8** from, U16 length)
{
	U16 count = 0;
	for (count = 0; count < length; count++)
	{
		**to = **from;
		++(*to);
		++(*from);
	}				
}

/****************************************************************************
*
* FUNCTION NAME : string_to_u32
*
* DESCRIPTION   : This function converts numeric strings to U32 numbers.
*
* PARAMETERS    :1. numeric string to be passed
*
* RETURNS       : U32 number
*
*****************************************************************************/
U32 string_to_u32(U8 s[])
{
    U32 		i = 0;
	U32		n = 0;

    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
	{
        n = (10 * n) + (s[i] - '0');
	}

	if ( n > 0xFFFFFFFF)
	{
		LOGGER_TRACE(("WRONG TYPE CASTING TO U32. \n"));
	}

    return n;
}

/*****************************************************************************
*
* FUNCTION NAME : string_to_u16
*
* DESCRIPTION   : This function converts numeric strings to U16 numbers.
*
* PARAMETERS    :1. numeric string to be passed
*
* RETURNS       : U16 number
*
*****************************************************************************/
U16 string_to_u16(U8 s[])
{
    U32 		i = 0;
	U16		n = 0;

    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
	{
        n = (10 * n) + (s[i] - '0');
	}

	if ( n > 0xFFFF)
	{
		LOGGER_TRACE(("WRONG TYPE CASTING TO U16. \n"));
	}

    return n;
}


/*****************************************************************************
*
* FUNCTION NAME : string_to_u8
*
* DESCRIPTION   : This function converts numeric strings to U8 numbers.
*
* PARAMETERS    :1. numeric string to be passed
*
* RETURNS       : U8 number
*
*****************************************************************************/
U8 string_to_u8(U8 s[])
{
    U32 		i = 0;
	U8		n = 0;

    for (i = 0; s[i] >= '0' && s[i] <= '9'; i++)
	{
        n = (10 * n) + (s[i] - '0');
	};
	if ( n > 0xFF)
	{
		LOGGER_TRACE(("WRONG TYPE CASTING TO U8. \n"));
	}

    return n;
}

/*****************************************************************************
*
* FUNCTION NAME : getbuflen
*
* DESCRIPTION   : This function calculates the buffer length.
*
* PARAMETERS    :1. starting pointer of the buffer
*                2. end pointer of the buffer
*
* RETURNS       :length of the buffer
*
*****************************************************************************/
U16 getbuflen(U8* p_buf, U8* end)
{
	U16 		buflen 		= 0 ;
	buflen = (U16)(end - p_buf);
	return (buflen);
}
