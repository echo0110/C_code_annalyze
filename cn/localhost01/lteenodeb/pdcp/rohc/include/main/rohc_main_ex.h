/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_main_ex.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_main_ex.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:27:38  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_MAIN_EX_H_
#define _ROHC_MAIN_EX_H_

#include "rohc_typ.h"
#include "rohc_def.h"

#define ROHC_SIZEOF_ENTITY_ID           4


/* hash define values for API constants */
/* Convention: To denote that the payload does not exist - set
   ROHC_API_SET_START_PAYLOAD and ROHC_API_SET_END_PAYLOAD to 0 */

#define ROHC_MAX_API_ELMS_LEN               32  /* Added 2 for rlc_id change */

#define ROHC_MAX_UNCOM_PKT_LEN              1024

#define ROHC_SIZEOF_API_HDR                 16

#define ROHC_API_HDR_LEN                    ROHC_SIZEOF_API_HDR

#define ROHC_VERSION_NUMBER                 1

#define ROHC_DEF_PRIORITY                   0   /* This is system specific
                                                 * value so please change
                                                 * accordingly if this field of
                                                 * API header is used */

/* macros to set rohc_U8bit_t, U16bit and U24bit values from buffer */
/* 'p_buff' must be typecast to a (rohc_U8bit_t *) before using these macros */

/* to write a 16 bit value starting from the location p_buff */
#define     ROHC_SET_U16BIT(p_buff,val)                                     \
                *((rohc_U8bit_t*)(p_buff)) = (rohc_U8bit_t)((val) >> 8) ;   \
                *((rohc_U8bit_t*)(p_buff+1)) = (rohc_U8bit_t)val 

/* to write a 24 bit value starting from the location p_buff */
#define     ROHC_SET_U24BIT(p_buff,val)                                     \
                *((rohc_U8bit_t*)(p_buff)) = (rohc_U8bit_t)((val) >> 16) ;  \
                *((rohc_U8bit_t*)(p_buff+1)) = (rohc_U8bit_t)((val) >> 8) ; \
                *((rohc_U8bit_t*)(p_buff+2)) = (rohc_U8bit_t)(val) 

/* to write a 32 bit value starting from the location p_buff */
#define     ROHC_SET_U32BIT(p_buff,val)                                         \
                *((rohc_U8bit_t*)(p_buff)) = (rohc_U8bit_t)((val) >> 24) ;      \
                *((rohc_U8bit_t*)(p_buff+1)) = (rohc_U8bit_t)((val) >> 16) ;    \
                *((rohc_U8bit_t*)(p_buff+2)) = (rohc_U8bit_t)((val) >> 8) ;     \
                *((rohc_U8bit_t*)(p_buff+3)) = (rohc_U8bit_t)(val)


/* macros to read U16bit, U24bit vnd U32bit alues from buffer */
/* 'p_buff' must be typecast to a (U8bit *) before using these macros */
/* to read a 16 bit value starting at the location p_buff */
#define     ROHC_GET_U16BIT(p_buff)                                         \
               (rohc_U16bit_t)((rohc_U16bit_t)((*(p_buff)) << 8) +          \
               (rohc_U16bit_t)(*(p_buff+1)) )

/* to read a 24 bit value starting at the location p_buff */
#define     ROHC_GET_U24BIT(p_buff)                                         \
            ((rohc_U32bit_t)((*(p_buff)) << 16) +                           \
            (rohc_U32bit_t)((*(p_buff+1)) << 8) +                           \
            (rohc_U32bit_t)(*(p_buff+2)) )

/* to read a 32 bit value starting at the location p_buff */
#define     ROHC_GET_U32BIT(p_buff)                                         \
            ((rohc_U32bit_t)((*(p_buff)) << 24) +                           \
            (rohc_U32bit_t)((*(p_buff+1)) << 16) +                          \
            (rohc_U32bit_t)((*(p_buff+2)) << 8) +                           \
            (rohc_U32bit_t)(*(p_buff+3)) )



/* Set the version and priority. */
#define ROHC_API_SET_VER_NUM(p_buff)    \
    *((rohc_U8bit_t*)p_buff) = ROHC_VERSION_NUMBER
#define ROHC_API_SET_PRIORITY(p_buff)   \
    *((rohc_U8bit_t*)(p_buff+1)) = ROHC_DEF_PRIORITY

/* set the API ID */
#define ROHC_API_SET_API_ID(p_buff,api_id)              \
ROHC_SET_U16BIT( ((rohc_U8bit_t*)(p_buff)+6), api_id) 

/* set the Message Length */
#define ROHC_API_SET_MSG_LEN(p_buff,msg_len)            \
ROHC_SET_U16BIT( ((rohc_U8bit_t*)(p_buff)+8), msg_len) 

/* set the End of Payload (offset from start of API buffer) */
#define ROHC_API_SET_END_PAYLOAD(p_buff,end_pl)         \
ROHC_SET_U16BIT( ((rohc_U8bit_t*)(p_buff)+10), end_pl) 

/* set the Start of Payload (offset from start of API buffer) */
#define ROHC_API_SET_START_PAYLOAD(p_buff,strt_pl)      \
ROHC_SET_U16BIT(((rohc_U8bit_t*)(p_buff)+12), strt_pl) 

/* set the End of Parameter list (offset from start of API buffer) */
#define ROHC_API_SET_END_PARAM_LIST(p_buff,end_param)   \
ROHC_SET_U16BIT(((rohc_U8bit_t*)(p_buff)+14),end_param)


/* get the API Version Number */
#define ROHC_API_GET_VER_NUM(p_buff)        *((rohc_U8bit_t*)p_buff)

/* get the API Priority */
#define ROHC_API_GET_PRIORITY(p_buff)       *((rohc_U8bit_t*)p_buff+1)

/* get the Source Entity ID */
#define ROHC_API_GET_SRC_ID(p_buff)         ROHC_GET_U16BIT(((rohc_U8bit_t*)(p_buff)+2))

/* get the Destination Entity ID */
#define ROHC_API_GET_DST_ID(p_buff)         ROHC_GET_U16BIT(((rohc_U8bit_t*)(p_buff)+4))

/* get the API ID */
#define ROHC_API_GET_API_ID(p_buff)         ROHC_GET_U16BIT(((rohc_U8bit_t*)(p_buff)+6))

/* get the Message Length, which is the number of bytes from start of 
   API buffer, till the end of buffer */
/* This should be the size of buffer that was allocated. */
#define ROHC_API_GET_MSG_LEN(p_buff)        ROHC_GET_U16BIT(((rohc_U8bit_t*)(p_buff)+8))

/* get the End of the Payload (offset from the start of the API buffer) */
#define ROHC_API_GET_END_PAYLOAD(p_buff)    ROHC_GET_U16BIT(((rohc_U8bit_t*)(p_buff)+10))

/* get the Start of the Payload (offset from the start of the API buffer) */
#define ROHC_API_GET_START_PAYLOAD(p_buff)  ROHC_GET_U16BIT(((rohc_U8bit_t*)(p_buff)+12))

extern rohc_return_t pd_create_snd_data_req
        (rohc_U8bit_t* p_in_buff, rohc_U16bit_t len,
                                         rohc_U16bit_t payload_st,
                 rohc_U32bit_t entity_id,rohc_U32bit_t *p_ecode);
/* + SPR 17439 */
rohc_return_t   rfc3095_process_msg 	ROHC_PROTO
	(( rohc_void_t 	*p_ro_entity ,
	  rohc_U8bit_t 	*p_in_buff ,
	  rohc_U8bit_t	**p_p_out_buff ,
	  rohc_void_t 	*p_api_buff_info ,
	  rohc_error_t 	*p_ecode
	));
/* - SPR 17439 */    
#endif /* _ROHC_MAIN_EX_H_ */
