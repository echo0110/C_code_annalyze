/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_def.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the hash defined values for
 *                     the ROHC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_def.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.7  2009/05/31 16:08:59  gur19479
 * modified values
 *
 * Revision 1.6  2009/05/29 09:36:53  gur19479
 * updated for state change configurable parameters
 *
 * Revision 1.5  2009/05/29 02:37:29  gur19479
 * updated to use wrapper funtions of framwork
 *
 * Revision 1.4  2009/05/28 05:18:04  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_DEF_H_
#define _ROHC_DEF_H_

#include "rohc_proto.h"
#include "lteFramework.h"
/************ INCOMING APIS ***********/

#define ROHC_IN_API_BASE            200     

/* APIs from PDCP-USER */
#define ROHC_USER_IN_API_BASE       (ROHC_IN_API_BASE+0)
#define ROHC_DATA_REQ               (ROHC_USER_IN_API_BASE+0)   /* 200 */
#define ROHC_USER_IN_API_LAST       ROHC_DATA_REQ

/* APIs from RRC */
#define ROHC_RRC_IN_API_BASE        (ROHC_USER_IN_API_LAST+1)
#define ROHC_CONFIG_REQ             (ROHC_RRC_IN_API_BASE+0)    /* 201 */
#define ROHC_RECONFIG_REQ           (ROHC_RRC_IN_API_BASE+1)    /* 202 */
#define ROHC_RELEASE_REQ            (ROHC_RRC_IN_API_BASE+2)    /* 203 */
#define ROHC_PACKET_SIZE_USED_REQ   (ROHC_RRC_IN_API_BASE+3)    /* 204 */
#define ROHC_RESET_REQ		    (ROHC_RRC_IN_API_BASE+4)	/* 205*/
#define ROHC_RRC_IN_API_LAST        ROHC_RESET_REQ


/* MISC APIs  - These APIs may come from any entity but are defined as
 * coming from MISC (virtual entity). For purposes of traces, we still assume
 * they come from RRC. */
#define ROHC_MISC_IN_API_BASE       (ROHC_RRC_IN_API_LAST+1)
#define ROHC_SET_ROHC_INFO_REQ      (ROHC_MISC_IN_API_BASE+0)   /* 206 */
#define ROHC_MISC_IN_API_LAST       ROHC_SET_ROHC_INFO_REQ

/* APIs from RLC */
#define ROHC_RLC_IN_API_BASE        (ROHC_MISC_IN_API_LAST+1)
#define ROHC_RLC_DATA_IND           (ROHC_RLC_IN_API_BASE+0)    /* 207 */
#define ROHC_RLC_IN_API_LAST        ROHC_RLC_DATA_IND

/* APIs from OAM : Stats/traces */
#define ROHC_OAM_IN_API_BASE            (ROHC_RLC_IN_API_LAST+1)
#define ROHC_RO_INITDB_REQ              (ROHC_OAM_IN_API_BASE+0)    /* 208 */
#define ROHC_GET_RO_STATS_REQ           (ROHC_OAM_IN_API_BASE+1)    /* 209 */
#define ROHC_GET_RO_INIT_STATS_REQ      (ROHC_OAM_IN_API_BASE+2)    /* 210 */
#define ROHC_INIT_RO_TRACE_LEVELS_REQ   (ROHC_OAM_IN_API_BASE+3)    /* 211 */
#define ROHC_GET_RO_TRACE_LEVELS_REQ    (ROHC_OAM_IN_API_BASE+4)    /* 211 */
#define ROHC_SET_RO_TRACE_LEVELS_REQ    (ROHC_OAM_IN_API_BASE+5)    /* 213 */
#define ROHC_OAM_IN_API_LAST            ROHC_SET_RO_TRACE_LEVELS_REQ

#define ROHC_IN_API_LAST            ROHC_OAM_IN_API_LAST


/************ OUTGOING APIS ***********/

#define ROHC_OUT_API_BASE               250     
#define ROHC_OAM_OUT_API_BASE           (ROHC_OUT_API_BASE+0)

/* APIs to OAM */
#define ROHC_RO_INITDB_CNF              (ROHC_OAM_OUT_API_BASE+0)       /* 250 */
#define ROHC_PACKET_SIZE_USED_RESP      (ROHC_OAM_OUT_API_BASE+1)       /* 251 */
#define ROHC_SET_ROHC_INFO_RESP         (ROHC_OAM_OUT_API_BASE+2)       /* 252 */
#define ROHC_GET_RO_STATS_CNF           (ROHC_OAM_OUT_API_BASE+3)       /* 253 */
#define ROHC_GET_RO_INIT_STATS_CNF      (ROHC_OAM_OUT_API_BASE+4)       /* 254 */
#define ROHC_INIT_RO_TRACE_LEVELS_CNF   (ROHC_OAM_OUT_API_BASE+5)       /* 255 */
#define ROHC_GET_RO_TRACE_LEVELS_CNF    (ROHC_OAM_OUT_API_BASE+6)       /* 256 */
#define ROHC_SET_RO_TRACE_LEVELS_CNF    (ROHC_OAM_OUT_API_BASE+7)       /* 257 */
#define ROHC_OAM_OUT_API_LAST           (ROHC_SET_ROHC_INFO_RESP)


#define ROHC_OUT_API_LAST               ROHC_OAM_OUT_API_LAST

#define ROHC_STACK_PRINT                qvPrintf
#define ROHC_ERROR_REPORTER(x)          NULL

#define ROHC_MEMCPY                     memCpy
#define ROHC_MALLOC                     rohc_mem_get
#define ROHC_FREE                       rohc_mem_free
#define ROHC_MEMMOVE                    memMove
#define ROHC_MEMSET                     memSet
#define ROHC_MEMCMP                     memCmp
#define ROHC_MSG_MALLOC                 qvMsgAllocBySize

#define ROHC_START_TIMER        rohc_start_timer

#define ROHC_STOP_TIMER(tdata,tid)\
do{\
         rohc_stop_timer(tdata,tid);\
	 tid = ROHC_NULL;\
}while(0)

/* Default values for max stats supported */
#define ROHC_MAX_ENTITY_STATS   (50U)   
#define ROHC_MAX_PROFILE_STATS  (80U)

/*************Default values for some fields ********/

#define ROHC_MAX_STATIC_INFO_COUNTER    (2U)
#define ROHC_MAX_DYN_INFO_COUNTER       (2U)
#define ROHC_MAX_P0_IR_COUNTER       	(2U)
#define ROHC_MAX_DYN_INFO_COUNTER       (2U)
#define ROHC_MAX_IR_RETURN_TIME         (4000U) /*for testing in ms=4 sec*/
#define ROHC_MAX_P6_IR_STATIC_INFO_COUNTER    (3U)

/*
** START_SPR_1099_FIX : Updated the value of ROHC_MAX_FO_RETURN_TIME, such that
** the expiry of IR & FO timers at the same time will not occur.
*/ 
#define ROHC_MAX_FO_RETURN_TIME         (2500U) /*for testing in ms=2.5 sec*/
/* END_SPR_1099_FIX */

#define ROHC_MAX_K_1_VAL                (10U)
#define ROHC_MAX_K_2_VAL                (10U)
#define ROHC_MAX_N_1_VAL                (10U)
#define ROHC_MAX_N_2_VAL                (10U)
#define ROHC_MAX_M_1_VAL                (10U)
#define ROHC_MAX_M_2_VAL                (10U)
#define ROHC_MAX_N_VAL                  (10U)

/******************Default values for mode trigger**********/
/****************this mechanism will replaced by taking the things from OAM*/

#define ROHC_TRIGGER_U_TO_O_MODE    (200U)
#define ROHC_TRIGGER_U_TO_R_MODE    (300U) /* for testing purpose */
#define ROHC_TRIGGER_O_TO_U_MODE    (400U) /* taken 1 */
#define ROHC_TRIGGER_O_TO_R_MODE    (500U)
#define ROHC_TRIGGER_R_TO_U_MODE    (600U)
#define ROHC_TRIGGER_R_TO_O_MODE    (700U)


#define ROHC_MAX_ELEM_LEN       (108U)
#endif /* _ROHC_DEF_H_ */
