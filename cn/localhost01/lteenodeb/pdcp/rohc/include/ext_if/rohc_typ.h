/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_typ.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the common data types which are to
 *                     be exported out of ROHC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_typ.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 05:18:04  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_TYP_H_
#define _ROHC_TYP_H_

#include "lteFramework.h"

/* basic data types */
#define rohc_U8bit_t        unsigned char
#define rohc_U16bit_t       unsigned short
#define rohc_U32bit_t       unsigned int
#define rohc_S8bit_t        char
#define rohc_S16bit_t       signed short
#define rohc_S32bit_t       signed int 
#define PNULL NULL
typedef void                rohc_void_t;
typedef void *              rohc_void_tp;

#define ROHC_FALSE      (0)
#define ROHC_TRUE       (1)             

#define ROHC_NULL       (0)

#define ROHC_INVALID    (0xFF)

typedef rohc_U8bit_t    rohc_return_t;
typedef rohc_U8bit_t    rohc_result_t;
typedef rohc_U32bit_t   rohc_len_t;

#define ROHC_SUCCESS    (1)
#define ROHC_FAILURE    (0)

/* To keep in sync with PDCP_SUCCESS and PDCP_FAILURE */
#define RO_SUCCESS    (0)
#define RO_FAILURE    (1)

typedef rohc_U32bit_t    rohc_error_t ;
typedef rohc_U8bit_t     rohc_error_class_t ;

/* Error classes can take the following values. */
#define     ROHC_DB_ERROR                   10
#define     ROHC_SYS_ERROR                  11
#define     ROHC_API_ERROR                  12
#define     ROHC_PROTOCOL_ERROR             13

/* Error flags can take the following values. */
typedef rohc_U8bit_t           rohc_error_flag_t;
#define     ROHC_PROTO_ERR              0x0001
#define     ROHC_SYS_ERR                0x0002
#define     ROHC_API_ERR                0x0004
#define     ROHC_DB_ERR                 0x0008

/* Error levels can the the following values. */
typedef rohc_U8bit_t            rohc_error_level_t;
#define ROHC_MINOR_ERROR        0
#define ROHC_MAJOR_ERROR        1
#define ROHC_CRITICAL_ERROR     2
#define ROHC_MAX_ERROR_LEVEL    2

typedef rohc_U8bit_t           rohc_bool_t ;

/* The type for the statistics stored */
typedef rohc_U16bit_t          rohc_stats_type_t ;

#define ROHC_STATS_ERRORS           0
#define ROHC_STATS_API              1
#define ROHC_STATS_INTERNAL         2

/* trace related type definitions */
typedef rohc_U16bit_t          rohc_trace_flag_t ;
#define ROHC_INCOMING_TRACE         0x00000001
#define ROHC_OUTGOING_TRACE         0x00000002
#define ROHC_FSM_TRACE              0x00000004
#define ROHC_ERROR_TRACE            0x00000010
#define ROHC_INIT_TRACE             0x00000020
#define ROHC_PROTO_EXCEPTION_TRACE  0x00000040

typedef rohc_U8bit_t       rohc_buff_type_t ;
#define     ROHC_INTERNAL_BUFFER            2
#define     ROHC_DYNAMIC_BUFFER             1
#define     ROHC_STATIC_BUFFER              0

/* Typedefs for CSPL data structures */

typedef YLIST   rohc_list_t;
typedef YTREE   rohc_rb_tree_t;
typedef YLNODE  rohc_list_node_t;
typedef YTNODE  rohc_tree_node_t;

typedef QTIMER  rohc_timer_id_t;
typedef QTIME   rohc_time_t;    

/* Typedefs for Time Stamp */
typedef rohc_U32bit_t rohc_timestamp_t;

typedef struct rohc_timer_data 
{
    rohc_U8bit_t timer_type; /* IR retun or FO return timer */

/*U-mode time type */
#define ROHC_IR_RET_TIMER 1U
#define ROHC_FO_RET_TIMER 2U
    rohc_void_tp p_csc_ptr;
    rohc_void_tp p_entity_ptr;
} rohc_timer_data_t;

/* Enum of error codes */
typedef enum
{
    ROHC_NO_ERR = 0,
    ROHC_TRACES_DISABLED=100,
    ROHC_STATS_DISABLED,
    ROHC_INVALID_STATS_TYPE,
    ROHC_INVALID_CID,
    ROHC_INVALID_PROTOCOL_TYPE,
    ROHC_PKT_DETERMINATION_FAILURE,
    ROHC_INVALID_CCPM_EVENT,
    ROHC_INVALID_DCPM_EVENT,
    ROHC_PROFILE_NOT_SUPPORTED,
    ROHC_INVALID_ACK_RECEIVED,
    ROHC_INVALID_NACK_RECEIVED,
    ROHC_INVALID_STATIC_NACK_RECEIVED,
    ROHC_MEMORY_FAILURE,
    ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK,
    ROHC_INVALID_IP_VERSION ,
    ROHC_INVALID_PKT,
    ROHC_INVALID_API_ID,
    ROHC_ENCODING_CALLED_FOR_RANDOM_IP_ID,
    ROHC_OUT_OF_SEQ_IP_ID_RECD,
    ROHC_CRC_CHECK_FAILED,
    ROHC_PACK_ERROR,
    ROHC_UNPACK_ERROR,
    ROHC_CSC_NOT_EXISTS_FOR_RECEIVED_FB_CID,
/* PQT SPR 1679 */ 
    ROHC_INVALID_TRACE_LEVEL,
/* PQT SPR 1679 */ 
	ROHC_DB_NOT_INIT,
	ROHC_MSN_NOT_VALID
}rohc_error_code_t;

#endif /* _ROHC_TYP_H_ */

