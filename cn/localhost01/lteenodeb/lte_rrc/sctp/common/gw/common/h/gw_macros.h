/*******************************************************************************
*
* FUNCTION : General purpose macros for the HSS Gateway.
*
********************************************************************************
*
* FILENAME : gw_macros.h
*
* DESCRIPTION : Contains global macros. Contain OCTET buffer conversion macros
*
*
* DATE        NAME           REFERENCE         REASON
* ----------  -------------- ----------------- ---------------------------------
*
* 19/1/2001   Sachin Mittal         -          Initial
*
* 01/06/2001  Prashant Jain         -          Added Macros for ANSI and
*                                              pre-ANSI compliance.
*
* 12/06/2002  Prashant Jain         -          Added Macros, which would be used
*                                              as a qualifier with the function
*                                              arguments.
* Copyright 2001 - 2002,  Hughes Software Systems Ltd.
*
*******************************************************************************/

/* Check to see if this file has been included already. */

#ifndef _GW_MACROS_H_
#define _GW_MACROS_H_

/******************************************************************************************
**
**                 MACRO Definitions
**
******************************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IN
#undef IN
#endif

#ifdef OUT
#undef OUT
#endif



#ifdef _ANSI_C_

#define _ARGS_(x)                           x
#define _STATIC_                            static
#define IN                                  const

# else

# define _ARGS_(x)                          ()
# define _STATIC_
#define IN

#endif


#define INOUT
#define OUT


#define GW_FIRST_BYTE_MASK          0xff
#define GW_SECOND_BYTE_MASK         0xff00
#define GW_THIRD_BYTE_MASK          0xff0000
#define GW_FOURTH_BYTE_MASK         0xff000000
#define GW_OCT_CONST                8

#define GW_CNVT_OCT_BUF_FIRST_BYTE(oct_buf_byte, item, type)\
    {\
        item = (type)(oct_buf_byte & 0xff);\
    }

#define GW_CNVT_OCT_BUF_SECOND_BYTE(oct_buf_byte, item, type)\
    {\
        item = (type) (item |((oct_buf_byte & 0xff) << GW_OCT_CONST));\
    }

#define GW_CNVT_OCT_BUF_THIRD_BYTE(oct_buf_byte, item, type)\
    {\
        item = (type) (item |((oct_buf_byte & 0xff) << (2 * GW_OCT_CONST)));\
    }

#define GW_CNVT_OCT_BUF_FOURTH_BYTE(oct_buf_byte, item, type)\
    {\
        item = (type) (item |((oct_buf_byte & 0xff) << (3 * GW_OCT_CONST)));\
    }


#define GW_CNVT_OCTBUF_32bit(oct_buf, item, count, type)\
    {\
        GW_CNVT_OCT_BUF_FIRST_BYTE(oct_buf[count++], item, type)\
        GW_CNVT_OCT_BUF_SECOND_BYTE(oct_buf[count++], item, type)\
        GW_CNVT_OCT_BUF_THIRD_BYTE(oct_buf[count++], item, type)\
        GW_CNVT_OCT_BUF_FOURTH_BYTE(oct_buf[count++], item, type)\
    }

#define GW_CNVT_OCTBUF_16bit(oct_buf, item, count, type)\
    {\
        GW_CNVT_OCT_BUF_FIRST_BYTE(oct_buf[count++], item, type)\
        GW_CNVT_OCT_BUF_SECOND_BYTE(oct_buf[count++], item, type)\
    }

#define GW_CNVT_OCTBUF_8bit(oct_buf, item, count, type)\
    {\
        GW_CNVT_OCT_BUF_FIRST_BYTE(oct_buf[count++], item, type)\
    }


#define GW_MAKE_OCT_BUF_FIRST_BYTE(oct_buf_byte, item)\
    {\
        oct_buf_byte = (item & GW_FIRST_BYTE_MASK);\
    }

#define GW_MAKE_OCT_BUF_SECOND_BYTE(oct_buf_byte, item)\
    {\
        oct_buf_byte = ((GW_U16bit)(item & GW_SECOND_BYTE_MASK)) >> (GW_OCT_CONST);\
    }

#define GW_MAKE_OCT_BUF_THIRD_BYTE(oct_buf_byte, item)\
    {\
        oct_buf_byte = ((GW_U32bit)(item & GW_THIRD_BYTE_MASK)) >> (2 * GW_OCT_CONST);\
    }

#define GW_MAKE_OCT_BUF_FOURTH_BYTE(oct_buf_byte, item)\
    {\
        oct_buf_byte = ((GW_U32bit)(item & GW_FOURTH_BYTE_MASK)) >> (3 * GW_OCT_CONST);\
    }


#define GW_MAKE_OCTBUF_32bit(oct_buf, item, count)\
    {\
        GW_MAKE_OCT_BUF_FIRST_BYTE(oct_buf[count++], item)\
        GW_MAKE_OCT_BUF_SECOND_BYTE(oct_buf[count++], item)\
        GW_MAKE_OCT_BUF_THIRD_BYTE(oct_buf[count++], item)\
        GW_MAKE_OCT_BUF_FOURTH_BYTE(oct_buf[count++], item)\
    }

#define GW_MAKE_OCTBUF_16bit(oct_buf, item, count)\
    {\
        GW_MAKE_OCT_BUF_FIRST_BYTE(oct_buf[count++], item)\
        GW_MAKE_OCT_BUF_SECOND_BYTE(oct_buf[count++], item)\
    }

#define GW_MAKE_OCTBUF_8bit(oct_buf, item, count)\
    {\
        GW_MAKE_OCT_BUF_FIRST_BYTE(oct_buf[count++], item)\
    }

#ifdef __cplusplus
}
#endif
#endif
