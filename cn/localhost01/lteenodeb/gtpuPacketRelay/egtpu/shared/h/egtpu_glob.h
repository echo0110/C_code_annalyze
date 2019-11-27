/***********************************************************************
**  FUNCTION:
**	Basic definitions for GTP-U Protocol Stack
************************************************************************
**
**  FILE NAME:
**	egtpu_glob.h
**
**  DESCRIPTION:
**	Contains basic data type definitions
**
**  DATE	    NAME			REFERENCE		REASON
**  ----	    ----			---------		------
**  Oct 2009    Vipul Aggarwal        			Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_GLOB_DECL__
#define __EGTPU_GLOB_DECL__

#include<msgpool.h>
#include<lteMemPool.h>
/* SPR 2030 fix Start*/
/* SPR 2241 fix Start*/
#ifndef UE_SIM_TESTING
#include <logging.h>
#include <alarm.h> 
#endif
/* SPR 2241 fix End*/
#include <lteLayer2CommanTypes.h>
/* SPR 2030 fix End*/

/* SPR 2030 fix Start*/
/* SPR 15909 fix start */
extern tickType_t relayGtpuCurrentTick_g;
/* SPR 15909 fix end */
/* SPR 2030 fix End*/
/* Basic data types */
typedef unsigned char             U8bit;
typedef unsigned short            U16bit;
typedef unsigned int              U32bit;
typedef char                      S8bit;
typedef short                     S16bit;
typedef int                       S32bit;
    /* SPR 20430 Changes Start */
typedef unsigned long int         U64bit;
    /* SPR 20430 Changes End */


typedef U8bit                     egtpu_boolean_t;

typedef void                      egtpu_void_t;
typedef U8bit                     egtpu_return_t;
typedef U8bit                     egtpu_error_t;
typedef U32bit                    egtpu_index_t;
typedef QMPOOL                    egtpu_pool_t;
/*reordering*/

typedef STACKCPOOL                egtpu_hash_pool_t;  
typedef U32bit                    egtpu_timer_drtn;
typedef U16bit                    egtpu_window_size;      
/*default values for window and reorder timer in different different ip traffic classes*/
#define EGTPU_WINDOW_SZ_SUBSCRIPTION_TRAFFIC_CLASS			128
#define EGTPU_WINDOW_SZ_CONVERSATIONAL_TRAFFIC_CLASS			128
#define EGTPU_WINDOW_SZ_STREAMING_TRAFFIC_CLASS				128
#define EGTPU_WINDOW_SZ_INTERACTIVE_TRAFFIC_CLASS			128
#define EGTPU_WINDOW_SZ_BKGROUND_TRAFFIC_CLASS				128
/* This value is used to init the pool. Kindly insure it is greater
 * than the least value of window size for a particular class */
#define EGTPU_INIT_WINDOW_SZ									64

#define EGTPU_REORDER_TMR_DRTN_SUBSCRIPTION_TRAFFIC_CLASS    500
#define EGTPU_REORDER_TMR_DRTN_CONVERSATIONAL_TRAFFIC_CLASS  500
#define EGTPU_REORDER_TMR_DRTN_STREAMING_TRAFFIC_CLASS		500
#define EGTPU_REORDER_TMR_DRTN_INTERACTIVE_TRAFFIC_CLASS		500
#define EGTPU_REORDER_TMR_DRTN_BKGROUND_TRAFFIC_CLASS	    500
/* definitions to indicate result of some operation */
#define EGTPU_SUCCESS              0
#define EGTPU_FAILURE              1
/* definitions to indicate result of some expression */
#define EGTPU_FALSE                0
#define EGTPU_TRUE                 1

/* the elementary ZERO */
#define EGTPU_NULL                 0
#define EGTPU_INVALID_INDEX		  0xffffffff	
#define EGTPU_INVALID_QOS_ID       0xffffffff
/* API header field type definitions */
typedef U16bit                    egtpu_api_id_t;
typedef U16bit                    egtpu_entity_id_t;

/* Entity Identitifers */
/* #define EGTPU_EI_GTPU              0 */
#define EGTPU_EI_GTPU              9
#define EGTPU_EI_UDP               1
/* #define EGTPU_EI_RELAY             2 */
#define EGTPU_EI_RELAY             4
#define EGTPU_EI_CP                3
/* #define EGTPU_EI_LM                4 */
#define EGTPU_EI_LM                1
#define EGTPU_EI_TS                5
#define EGTPU_EI_UDP6              6

#define EGTPU_NUM_EI               6

/* buffer type definitions */
#define EGTPU_STATIC_BUFFER        0
#define EGTPU_DYNAMIC_BUFFER       1

/* Trace types */
#define EGTPU_INCOMING_TRACE       0x0001
#define EGTPU_OUTGOING_TRACE       0x0002
#define EGTPU_FSM_TRACE            0x0004
#define EGTPU_ERROR_TRACE          0x0008

/* trace level type definitions */
#define EGTPU_NO_TRACE_LEVEL       0
#define EGTPU_BRIEF_TRACE_LEVEL    1
#define EGTPU_DETAILED_TRACE_LEVEL 2

/* EGTPU_NO_TRACE_LEVEL is not counted */
#define EGTPU_MAX_TRACE_LEVELS     2

/* error flag definitions */
#define EGTPU_PROTO_ERR            0x0001
#define EGTPU_SYS_ERR              0x0002
#define EGTPU_API_ERR              0x0004
#define EGTPU_DB_ERR               0x0008

/* error level definitions */
#define EGTPU_NO_ERROR             0
#define EGTPU_CRITICAL_ERROR       1
#define EGTPU_MAJOR_ERROR          2
#define EGTPU_ALL_ERROR            3

/* GTPU Statistics Types */
#define EGTPU_STATS_ERROR          0
#define EGTPU_STATS_API            1
#define EGTPU_STATS_PROTO_MSG      2
#define EGTPU_STATS_ERR_PROTO_MSG  3
#define EGTPU_STATS_INTERNAL       4    

/* maximum API/MSG size possible */
#define EGTPU_MAX_MSG_SIZE         8192
#define EGTPU_API_MSG_SIZE         128

#ifndef LINUX_PC_TEST
/* standard GTP-U port */
#define EGTPU_UDP_PORT             2152
#else
#define EGTPU_UDP_PORT             21520
#endif

/* macros to read U16bit, U24bit, and U32bit values from buffer */
/* p_buff must be typecast to a (U8bit *) before using these macros */

/* to read a 16 bit value starting at the location p_buff */
    /* SPR 20430 Changes Start */
#ifdef __x86_64__
#define EGTPU_GET_ADDRESS(tbuf)  EGTPU_GET_U64BIT(tbuf)
#define EGTPU_SET_ADDRESS(tbuf, data)  EGTPU_SET_U64BIT(tbuf, (UInt64)data)

#else
#define EGTPU_GET_ADDRESS(tbuf)  EGTPU_GET_U32BIT(tbuf)
#define EGTPU_SET_ADDRESS(tbuf, data)  EGTPU_SET_U32BIT(tbuf, (UInt32)data)
#endif

    /* SPR 20430 Changes End */
#define EGTPU_GET_U16BIT(p_buff)                                        \
    ((U16bit)(*(p_buff) << 8) |                                        \
        (U16bit)(*(p_buff + 1)))

/* to read a 24 bit value starting at the location p_buff */
#define EGTPU_GET_U24BIT(p_buff)                                        \
    ((U32bit)(*(p_buff) << 16) |                                       \
        (U32bit)(*(p_buff + 1) << 8) |                                 \
        (U32bit)(*(p_buff + 2)))


    /* SPR 20430 Changes Start */

    /* SPR 20636 Changes Start */
#define EGTPU_GET_U64BIT(p_buff)                                   \
	(((U64bit)(*(p_buff)) << 56) |                                       \
	 ((U64bit)(*(p_buff + 1)) << 48) |                                \
	 ((U64bit)(*(p_buff + 2)) << 40) |                                 \
	 ((U64bit)(*(p_buff + 3)) << 32) |                                      \
         ((U64bit)(*(p_buff + 4)) << 24) |                                \
	 ((U64bit)(*(p_buff + 5)) << 16) |                                 \
	 ((U64bit)(*(p_buff + 6)) << 8) |                                 \
	 (U64bit)(*(p_buff + 7))) \

    /* SPR 20636 Changes End */
    /* SPR 20430 Changes End */
/* to read a 32 bit value starting at the location p_buff */
    /* SPR 20636 Changes Start */
#define EGTPU_GET_U32BIT(p_buff)                                        \
    (((U32bit)(*(p_buff)) << 24) |                                       \
        (((U32bit)(*(p_buff + 1))) << 16) |                                \
        (((U32bit)(*(p_buff + 2))) << 8) |                                 \
        ((U32bit)(*(p_buff + 3))))

    /* SPR 20636 Changes End */
/* macros to set U16bit, U24bit, and U32bit values onto buffer */
/* p_buff must be typecast to a (U8bit *) before using these macros */

/* to write a 16 bit value starting from the location p_buff */
#define EGTPU_SET_U16BIT(p_buff, val)                                   \
/* Cyclomatic Complexity changes */                                    \
    {                                                               \
        *(p_buff) = (U8bit)((val) >> 8) ;                              \
        *(p_buff + 1) = (U8bit)(val);                                  \
    }

/* to write a 24 bit value starting from the location p_buff */
#define EGTPU_SET_U24BIT(p_buff, val)                                   \
/* Cyclomatic Complexity changes */                                    \
    {                                                               \
        *(p_buff) = (U8bit)((val) >> 16);                              \
        *(p_buff + 1) = (U8bit)((val) >> 8);                           \
        *(p_buff + 2) = (U8bit)(val);                                  \
    }

/* to write a 32 bit value starting from the location p_buff */
    /* SPR 20430 Changes Start */
#define EGTPU_SET_U64BIT(p_buff, val)                                   \
/* Cyclomatic Complexity changes */                                    \
{                                                               \
	*(p_buff) = (U16bit)((val) >> 56);                              \
	*(p_buff + 1) = (U16bit)((val) >> 48);                          \
	*(p_buff + 2) = (U16bit)((val) >> 40);                           \
	*(p_buff + 3) = (U16bit)((val) >> 32);                                  \
	*(p_buff + 4) = (U16bit)((val) >> 24);                          \
	*(p_buff + 5) = (U16bit)((val) >> 16);                           \
	*(p_buff + 6) = (U16bit)((val) >> 8);                                  \
	*(p_buff + 7) = (U16bit)(val);                                  \
}
    /* SPR 20430 Changes End */
#define EGTPU_SET_U32BIT(p_buff, val)                                   \
/* Cyclomatic Complexity changes */                                    \
    {                                                               \
        *(p_buff) = (U8bit)((val) >> 24);                              \
        *(p_buff + 1) = (U8bit)((val) >> 16);                          \
        *(p_buff + 2) = (U8bit)((val) >> 8);                           \
        *(p_buff + 3) = (U8bit)(val);                                  \
    }

#endif
