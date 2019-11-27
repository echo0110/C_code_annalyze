/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name       : oam_agnt_utils.c
 *
 * Description     : This file contains utility based functions. 
 * Revision History:
 *
 * Date         Author                 Reference            Comments
 * -----        ------                 ---------           --------
 * January,2017   Vaibhav Kala     Agent Document   Initial Version
 *
 * Copyright (c) 2013-2017, Aricent Inc. All Rights Reserved
 *******************************************************************************/

#ifndef _OAM_AGNT_UTILS_H_
#define _OAM_AGNT_UTILS_H_

#include "cspl.h"
#include "lteTypes.h"
#include "advlogging.h"

#define OAM_AGNT_VERSION 1
#define OAM_AGNT_LOG(source, log_priority, formate, ...) \
	oam_agnt_trace_message(log_priority,(SInt8 *) __FILE__,(SInt8 *)__FUNCTION__,__LINE__,(SInt8 *)formate,##__VA_ARGS__)

#ifndef OAM_AGNT_DEBUG
#define OAM_AGNT_FUN_ENTRY_TRACE()
#define OAM_AGNT_FUN_EXIT_TRACE()

#else
#define OAM_AGNT_FUN_ENTRY_TRACE()\
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED, "Entering function: %s in file: %s\n",\
            __FUNCTION__, __FILE__)
  
#define OAM_AGNT_FUN_EXIT_TRACE()\
    OAM_AGNT_LOG(AGNT, OAM_AGNT_DETAILED, "Exiting function: %s in file: %s\n",\
            __FUNCTION__, __FILE__)
#endif

#define LOCAL                                    static
#define FORK_ERROR                              -1
#define FORK_CHILD                               0
#define OAM_AGNT_NULL                          NULL
#define OAM_INVALID_RETURN                      -1
#define OAM_AGNT_ZERO                             0
#define OAM_AGNT_ONE                              1
#define OAM_AGNT_STD_ERROR                       -1
#define OAM_AGNT_POLL_TIME                       10   
#define OAM_BIT_ONE                              1
#define MAX_RX_DATA_BUFF_SIZE                11000
#define OAM_NEG_ONE                             -1
#define OAM_AGNT_TIMEOUT                         1
#define TIME_SIZE                               20

#define OAM_AGNT_INTERFACE_API_HEADER_SIZE      16
#define OAM_AGNT_API_HEADER_SIZE                16
#define OAM_AGNT_SRC_ID_HDR_IDX                 2
#define OAM_AGNT_DST_ID_HDR_IDX                 4
#define OAM_AGNT_API_ID_HDR_IDX                 6
#define OAM_AGNT_MSG_SZ_HDR_IDX                 8

#define OAM_AGNT_MODULE_ID                       30 
#define OAM_MODULE_ID                            1
#define OAM_TIMEOUT				               132
#define AGNT_OAM_ACTIVE_TIME_OUT                180  
#define OAM_NOTIFICATION                       189
#define OAM_CSPL_HDR_SIZE                       16

#define OAM_AGNT_API_VERSION                       1
#define OAM_BIT_ZERO                              0
#define OAM_BYTESIZE                              8
#define OAM_AGNT_TIME_CONV_S_TO_MS              1000
#define OAM_AGNT_RECV_BUFFER_SIZE               5000  
#define OAM_AGNT_TRACE_SIZE                     1024  

#define OAM_AGNT_TWO                               2
#define OAM_AGNT_THREE                             3
#define OAM_AGNT_FOUR                              4
#define OAM_AGNT_FIVE                              5
#define OAM_AGNT_SIX                               6
#define OAM_AGNT_SEVEN                             7
#define OAM_AGNT_EIGHT                             8
#define OAM_AGNT_NINE                              9
#define OAM_AGNT_TEN                              10
#define OAM_AGNT_ELEVEN                           11
#define OAM_AGNT_TWELVE                          12
#define OAM_AGNT_THIRTEEN                         13
#define OAM_AGNT_FOURTEEN                         14
#define OAM_AGNT_FIFTEEN                          15
#define OAM_AGNT_SIXTEEN                          16
#define OAM_AGNT_SEVNTEEN                         17
#define OAM_AGNT_EIGHTEEN                         18
#define OAM_AGNT_NIGHNTEEN                        19
#define OAM_AGNT_TWENTY                           20
#define OAM_AGNT_TWENTY_FOUR                      24
#define OAM_AGNT_TWO_FIFTY_FIVE                  255
#define OAM_AGNT_UPPER_EIGHT_BITS_SET         0xFF00
#define OAM_AGNT_LOWER_EIGHT_BITS_SET         0x00FF
#define OAM_AGNT_ALL_BIT_SET                    0xFF
#define OAM_AGNT_ALL_BIT_ZERO                   0x00
#define OAM_AGNT_MAX_CMD_SIZE                   256

typedef UInt16      oam_agnt_length_t;
typedef UInt16      oam_agnt_counter_t;

typedef enum
{
  OAM_AGNT_FAILURE = 0,
  OAM_AGNT_SUCCESS
}oam_agnt_return_et;

typedef enum _log_priority_et
{
    OAM_AGNT_CRITICAL = LTE_LOG_CRITICAL,        /* Critical conditions, such as hard device errors */
    OAM_AGNT_ERROR = LTE_LOG_ERROR,              /* Error messages */
    OAM_AGNT_WARNING = LTE_LOG_WARNING,          /* Warning messages */
    OAM_AGNT_INFO = LTE_LOG_INFO,                /* Info about the resources*/
    OAM_AGNT_BRIEF = LTE_LOG_BRIEF,              /* Brief about the resources*/
    OAM_AGNT_DETAILED = LTE_LOG_DETAILED,        /* Detail about all the messg */
    OAM_AGNT_DETAILEDALL = LTE_LOG_DETAILEDALL,  /* Detail including comp parse logs about all the messg */
} log_priority_et;
#define OAM_AGNT_COMP_PARSE OAM_AGNT_DETAILEDALL 

typedef struct _LISTENER
{
    SInt16 rx_tx_sd;			/*Receiver discriptor */
} LISTENER;			/*The porting function for UNIX paltform.
                      Structure QSYSOP is initialized with these functions */

typedef enum
{       
/*CSR 57877 Fix Start */  
    OAM_AGNT_NO_ERROR, 
/*CSR 57877 Fix End*/  
    OAM_AGNT_MALLOC_FAIL,
    OAM_AGNT_SEND_FAIL,
    OAM_AGNT_NULL_POINTER_ERR,
    OAM_AGNT_INSUFFICIENT_BUFFER,
    OAM_AGNT_MODULE_DISABLED,
    OAM_AGNT_INVALID_REQ,
} oam_agnt_error_code_et;


typedef enum {
	OAM_AGNT_FALSE = 0,
	OAM_AGNT_TRUE
} oam_agnt_bool_et;

typedef enum
{
    OAM_AGNT_INIT_IND_TIMER,
    OAM_AGNT_IM_OK_IND_TIMER,
    OAM_AGNT_SHUT_DOWN_TIMER,
    OAM_AGNT_MAX_TIMER
}oam_agnt_timer_type_et;

typedef QTIMER oam_agent_timer_t;

typedef struct _oam_agent_context
{
    oam_agent_timer_t   timer_id[OAM_AGNT_MAX_TIMER];
}oam_agent_context_t;

typedef struct _oam_agent_timer_data
{
    oam_agnt_timer_type_et  timer_type;   /*!< Type of timer*/
    void*                   p_timer_data; /*!< Timer Data */
    U16                     timer_data_len; /*!< Timer Data length */
}oam_agent_timer_data_t;

oam_agent_timer_t
oam_agnet_start_timer 
(
 /* Duration (in milliseconds) of the timer to be started */
 U32         timer_duration,
 oam_agent_timer_data_t *p_timer_data,
 oam_agnt_bool_et is_repeated         /* Indicates whether timer is repeated */
 );

void oam_agent_stop_timer(oam_agent_timer_t timer_id);

void oam_agnt_trace_message(
		SInt32 log_level,              /* log level of this message */
		SInt8 *pFile,
		SInt8 *pFun,
		UInt16 Line,
		SInt8* format,         /* format string like in printf */
		...                         /* optional parameters */
		);

void
oam_agnt_construct_interface_api_header(UInt8 * p_msg,
        UInt16 trans_id,
        UInt16 src_mod_id,
        UInt16 dst_mod_id,
        UInt16 api_id,
        UInt16 api_buf_size,
        UInt8 resvd);

oam_agnt_return_et 
oam_agnt_mem_alloc(size_t size, void **p_msg, oam_agnt_error_code_et *p_error_code);

oam_agnt_return_et
oam_agnt_mem_free
( void *p_buffer, oam_agnt_error_code_et *p_error_code);

oam_agnt_bool_et
agent_chk_file_exist(Char8 *p_filename, SInt16 *p_errno);

void
oam_agnt_construct_api_header (
		UInt8 * p_header,	/* CSPL header */
		UInt8 version_id,	/* API version Id */
		UInt16 src_module_id,	/* Source module Id */
		UInt16 dst_module_id,	/* Destination module Id */
		UInt16 api_id,	/* API Id */
		UInt16 api_buf_size	/* API buffer size */
		);
oam_agnt_return_et
oam_agnt_send_message (void *p_msg, int dst_module);
oam_agnt_return_et
oam_agnt_get_cksum(Char8 *p_filename, UInt64 *p_cksum);
void
oam_agnt_stop_timer(oam_agent_timer_t timer_id);
oam_agent_timer_t
oam_agnt_start_timer 
(
 U32         timer_duration,
 oam_agent_timer_data_t *p_timer_data, oam_agnt_bool_et is_repeated);

UInt8 oam_agnt_get_my_id();
void oam_agnt_set_my_id(UInt8 inst_id);
oam_agnt_return_et
oam_agnt_get_word_from_header (UInt8 * p_header);

oam_agnt_return_et
oam_agnt_cp_pack_UInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_UInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_UInt64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_UInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_SInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_SInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_SInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_UInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_UInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_UInt64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_UInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_SInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_SInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_SInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_pack_Char8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

oam_agnt_return_et
oam_agnt_cp_unpack_Char8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

UInt8* oam_agnt_get_api_start(UInt8 *p_api_buf);
SInt32 oam_agnt_get_api_len(UInt8 *p_api_buf);
AdvL_LogLevel oam_agnt_convert_log_level (log_priority_et agnt_log_level);
oam_agnt_return_et oam_agnt_rename_latest_img(Char8 *p_bin_name);

#endif
