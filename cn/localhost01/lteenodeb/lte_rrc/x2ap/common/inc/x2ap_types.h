/******************************************************************************
*
*   FILE NAME   : x2ap_types.h
*
*   DESCRIPTION : This file contains basic X2AP types definitions.
*
*   DATE            AUTHOR         REFERENCE       REASON
*   24-JUNE-2010    Vaibhav Singh  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _X2AP_TYPES_H_
#define _X2AP_TYPES_H_

/* Include CSPL types */
#include "cspl.h"
#include "stacklayer.h"

/* define X2AP MACROS */
#define X2AP_NULL   0
#define X2AP_P_NULL ( (void*)0)

#define X2AP_TRANSACTION_ID_ABSENT 0
#ifdef LTE_EMBMS_SUPPORTED
#define X2AP_ZERO   0
#endif
#define X2AP_SOCKET_DESC_SIZE    4  
#define X2AP_SCTP_STREAM_ID_SIZE 2

#define X2AP_VERSION_ID          0x01
/* Define X2AP types */

/* Values for x2ap_return_et, x2ap_return_t */
#define x2ap_return_et rrc_return_et
#define X2AP_FAILURE RRC_FAILURE
#define X2AP_SUCCESS RRC_SUCCESS
/* Values for x2ap_bool_et, x2ap_bool_t */
typedef enum
{
    X2AP_FALSE,
    X2AP_TRUE
} x2ap_bool_et;

/* data types */
typedef U16             x2ap_tag_t;
typedef U16             x2ap_counter_t;
typedef size_t          x2ap_size_t;
typedef size_t          x2ap_length_t;
typedef QTIMER          x2ap_timer_t;
typedef U8              x2ap_bool_t;
typedef U16             x2ap_module_id_t;
#endif
