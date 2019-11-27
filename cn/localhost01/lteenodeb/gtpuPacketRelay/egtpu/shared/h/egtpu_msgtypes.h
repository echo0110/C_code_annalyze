/***********************************************************************
**  FUNCTION:
**	GTP-U Protocol Message and API type definitions
************************************************************************
**
**  FILE NAME:
**	gtpu_msgtypes.h
**
**  DESCRIPTION:
**	Contains API/message type definitions.
**      Incoming and outgoing ranges of APIs are defined per external
**      entity. This assures an easy introduction of new APIs or
**      even a new external entity with minimal effect on other
**      API headers.
**
**  DATE	    NAME			REFERENCE		REASON
**  ----	    ----			---------		------
**  Oct 2009    Vipul Aggarwal        			Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef _EGTPU_MSGTYPES_DECL_
#define _EGTPU_MSGTYPES_DECL_

/***********************************************************************

Protocol message type definitions

***********************************************************************/
#define EGTPU_MSGT_ECHO_REQUEST       1 
#define EGTPU_MSGT_ECHO_RESPONSE      2
#define EGTPU_MSGT_ERROR_INDICATION   26 
#define EGTPU_MSGT_SUPP_EXT_HDR_NOTI  31
#define EGTPU_MSGT_END_MARKER         254
#define EGTPU_MSGT_TPDU               255

/***********************************************************************

Message Lengths

***********************************************************************/
/** 2 bytes of sequence number + 2 spare bytes + 2 bytes for recovery IE **/
#define EGTPU_MIN_ECHO_RSP_LENGTH		6
/** 2 bytes of sequence number + 2 spare bytes **/
#define EGTPU_MIN_ECHO_REQ_LENGTH		4
/** 2 bytes seq number + 2 spare bytes + 3 bytes extn hdr type list IE **/
#define EGTPU_MIN_SUPP_EXT_HDR_LENGTH		7
										
/***********************************************************************

Incoming APIs

***********************************************************************/
/* API from UDP */
#define EGTPU_UDP_IN_API_BASE         100
#define EGTPU_UNITDATA_IND            (EGTPU_UDP_IN_API_BASE)
#define EGTPU_UDP_LAST_IN_API         (EGTPU_UNITDATA_IND)

/* API from Control Plane */
#define EGTPU_CP_IN_API_BASE          200
#define EGTPU_SETUP_SAP_REQ           (EGTPU_CP_IN_API_BASE)
#define EGTPU_RELEASE_SAP_REQ         (EGTPU_CP_IN_API_BASE + 1)
#define EGTPU_CP_LAST_IN_API          (EGTPU_RELEASE_SAP_REQ)

/* API from Relay Entity */
#define EGTPU_RELAY_IN_API_BASE       300
#define EGTPU_DATA_REQ                (EGTPU_RELAY_IN_API_BASE)
#define EGTPU_RELOC_DATA_REQ          (EGTPU_RELAY_IN_API_BASE + 1)
#define EGTPU_SEND_EMI_TO_PEER_REQ    (EGTPU_RELAY_IN_API_BASE + 2)
#ifdef ENDC_ENABLED
#define EGTPU_X2U_DELIVERY_STATUS      (EGTPU_RELAY_IN_API_BASE + 3)
#define EGTPU_RELAY_LAST_IN_API       (EGTPU_X2U_DELIVERY_STATUS)
#else
#define EGTPU_RELAY_LAST_IN_API       (EGTPU_SEND_EMI_TO_PEER_REQ)
#endif

/* API from Layer Management */
#define EGTPU_LM_IN_API_BASE          400
#define EGTPU_INITDB_REQ              (EGTPU_LM_IN_API_BASE)
#define EGTPU_SET_TIMER_REQ           (EGTPU_LM_IN_API_BASE + 1)
#define EGTPU_GET_TIMER_REQ           (EGTPU_LM_IN_API_BASE + 2)
#define EGTPU_GET_MAX_RETRIES_REQ     (EGTPU_LM_IN_API_BASE + 3)
#define EGTPU_SET_MAX_RETRIES_REQ     (EGTPU_LM_IN_API_BASE + 4)
#define EGTPU_GET_STATS_REQ           (EGTPU_LM_IN_API_BASE + 5)
#define EGTPU_GET_INIT_STATS_REQ      (EGTPU_LM_IN_API_BASE + 6)
#define EGTPU_INIT_TRACE_LEVEL_REQ    (EGTPU_LM_IN_API_BASE + 7)
#define EGTPU_SET_TRACE_LEVEL_REQ     (EGTPU_LM_IN_API_BASE + 8)
#define EGTPU_GET_TRACE_LEVEL_REQ     (EGTPU_LM_IN_API_BASE + 9)
#define EGTPU_GET_CTXT_INFO_REQ       (EGTPU_LM_IN_API_BASE + 10)
#define EGTPU_MODIFY_REORDERING_IE_REQ (EGTPU_LM_IN_API_BASE + 11)
#define EGTPU_LM_LAST_IN_API          (EGTPU_MODIFY_REORDERING_IE_REQ)

/* API from GTPU Timer */
#define EGTPU_TIMEOUT_IN_API_BASE     500
#define EGTPU_ECHO_TIMER_EXPIRY       (EGTPU_TIMEOUT_IN_API_BASE)
#define EGTPU_REORDER_TIMER_EXPIRY    (EGTPU_TIMEOUT_IN_API_BASE+1)
#define EGTPU_TIMEOUT_LAST_IN_API     (EGTPU_REORDER_TIMER_EXPIRY)

/*******************************************************************

Outgoing APIs

********************************************************************/
/* API to UDP */
#define EGTPU_UDP_OUT_API_BASE        150
#define EGTPU_UDP_UNITDATA_REQ        (EGTPU_UDP_OUT_API_BASE)
#define EGTPU_UDP_LAST_OUT_API        (EGTPU_UDP_UNITDATA_REQ)

/* API to Control PLane */
#define EGTPU_CP_OUT_API_BASE         250
#define EGTPU_CP_SETUP_SAP_CNF        (EGTPU_CP_OUT_API_BASE)
#define EGTPU_CP_RELEASE_SAP_CNF      (EGTPU_CP_OUT_API_BASE + 1)
#define EGTPU_CP_ERROR_IND            (EGTPU_CP_OUT_API_BASE + 2)
#define EGTPU_CP_PATH_FAILURE_IND     (EGTPU_CP_OUT_API_BASE + 3)
#define EGTPU_CP_END_MARKER_IND       (EGTPU_CP_OUT_API_BASE + 4)
/*+SPR 3456 */
#define EGTPU_CP_PATH_SUCCESS_IND     (EGTPU_CP_OUT_API_BASE + 5)
#define EGTPU_CP_LAST_OUT_API         (EGTPU_CP_PATH_SUCCESS_IND)
/*-SPR 3456 */

/* API to Relay */
#define EGTPU_RELAY_OUT_API_BASE      350
#define EGTPU_RELAY_DATA_IND          (EGTPU_RELAY_OUT_API_BASE)
#define EGTPU_RELAY_LAST_OUT_API      (EGTPU_RELAY_DATA_IND)

/* API to Layer management */
#define EGTPU_LM_OUT_API_BASE         450
#define EGTPU_LM_INITDB_CNF           (EGTPU_LM_OUT_API_BASE)
#define EGTPU_LM_SET_TIMER_CNF        (EGTPU_LM_OUT_API_BASE + 1)
#define EGTPU_LM_GET_TIMER_CNF        (EGTPU_LM_OUT_API_BASE + 2)
#define EGTPU_LM_GET_MAX_RETRIES_CNF  (EGTPU_LM_OUT_API_BASE + 3)
#define EGTPU_LM_SET_MAX_RETRIES_CNF  (EGTPU_LM_OUT_API_BASE + 4)
#define EGTPU_LM_GET_STATS_CNF        (EGTPU_LM_OUT_API_BASE + 5)
#define EGTPU_LM_GET_INIT_STATS_CNF   (EGTPU_LM_OUT_API_BASE + 6)
#define EGTPU_LM_INIT_TRACE_LEVEL_CNF (EGTPU_LM_OUT_API_BASE + 7)
#define EGTPU_LM_SET_TRACE_LEVEL_CNF  (EGTPU_LM_OUT_API_BASE + 8)
#define EGTPU_LM_GET_TRACE_LEVEL_CNF  (EGTPU_LM_OUT_API_BASE + 9)
#define EGTPU_LM_GET_CTXT_INFO_CNF    (EGTPU_LM_OUT_API_BASE + 10)
#define EGTPU_LM_ALARM_NOTIFICATION   (EGTPU_LM_OUT_API_BASE + 11)
#define EGTPU_LM_MODIFY_REORDERING_IE_CNF (EGTPU_LM_OUT_API_BASE + 12)
#define EGTPU_LM_LAST_OUT_API         (EGTPU_LM_MODIFY_REORDERING_IE_CNF)
 
#endif
