/***********************************************************************
**  FUNCTION:
**	eGTP-U Protocol Message and API error type definitions
************************************************************************
**
**  FILE NAME:
**	egtpu_error.h
**
**  DESCRIPTION:
**	Errors are divided into following categories:
**          parse errors (PERR)
**          database errors (DBERR)
**          system errors (SERR)
**          common errors (CERR)
**
**  DATE	    NAME			REFERENCE	REASON
**  ----	    ----		-	--------	------
**  Oct 2009    Vipul Aggarwal        		Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_ERROR_DECL__
#define __EGTPU_ERROR_DECL__

/* parse error codes (starts from 0) */
#define EGTPU_PERR_CODE_BASE             0
#define EGTPU_PERR_UNKNOWN_MSG           (EGTPU_PERR_CODE_BASE + 0)
#define EGTPU_PERR_UNKNOWN_API           (EGTPU_PERR_CODE_BASE + 1)
#define EGTPU_PERR_UNEXPECTED_MSG        (EGTPU_PERR_CODE_BASE + 2)
#define EGTPU_PERR_UNEXPECTED_API        (EGTPU_PERR_CODE_BASE + 3)
#define EGTPU_PERR_MSG_TOO_SHORT         (EGTPU_PERR_CODE_BASE + 4)
#define EGTPU_PERR_VERSION_NOT_SUPP      (EGTPU_PERR_CODE_BASE + 5)
#define EGTPU_PERR_PROTO_TYPE_INCORRECT  (EGTPU_PERR_CODE_BASE + 6)
#define EGTPU_PERR_INV_MSG_FORMAT        (EGTPU_PERR_CODE_BASE + 7)
#define EGTPU_PERR_MAND_IE_MISSING       (EGTPU_PERR_CODE_BASE + 8)
#define EGTPU_PERR_MAND_IE_INCORRECT     (EGTPU_PERR_CODE_BASE + 9)
#define EGTPU_PERR_ECHO_TIMER_EXP        (EGTPU_PERR_CODE_BASE + 10)
#define EGTPU_PERR_INV_TIMER_TYPE        (EGTPU_PERR_CODE_BASE + 11)
#define EGTPU_PERR_UNEXPECTED_TIMER_EXP  (EGTPU_PERR_CODE_BASE + 12)
#define EGTPU_PERR_EXT_HDR_NOT_SUPP      (EGTPU_PERR_CODE_BASE + 13)
#define EGTPU_PERR_INV_TRAFFIC_CLASS     (EGTPU_PERR_CODE_BASE + 14)
#define EGTPU_LAST_PERR_CODE             (EGTPU_PERR_INV_TRAFFIC_CLASS)

/* CDB error codes (starts from 14) */
#define EGTPU_DBERR_CODE_BASE            (EGTPU_LAST_PERR_CODE + 1)
#define EGTPU_DBERR_INSERT_DB_FAILURE    (EGTPU_DBERR_CODE_BASE + 0)
#define EGTPU_DBERR_KEY_NOT_PRESENT      (EGTPU_DBERR_CODE_BASE + 1)
#define EGTPU_LAST_DBERR_CODE            (EGTPU_DBERR_KEY_NOT_PRESENT)

/* System error codes (starts from 16) */
#define EGTPU_SERR_CODE_BASE             (EGTPU_LAST_DBERR_CODE + 1)
#define EGTPU_SERR_STATIC_MEM_NOT_AVAIL  (EGTPU_SERR_CODE_BASE + 0)
#define EGTPU_SERR_DYN_MEM_NOT_AVAIL     (EGTPU_SERR_CODE_BASE + 1)
#define EGTPU_SERR_START_TIMER_FAILURE   (EGTPU_SERR_CODE_BASE + 2)
#define EGTPU_LAST_SERR_CODE             (EGTPU_SERR_START_TIMER_FAILURE)

/* common error codes (starts from 19) */
#define EGTPU_CERR_CODE_BASE             (EGTPU_LAST_SERR_CODE + 1)
#define EGTPU_CERR_INIT_NOT_DONE         (EGTPU_CERR_CODE_BASE + 0)
#define EGTPU_CERR_TRACES_DISABLED       (EGTPU_CERR_CODE_BASE + 1)
#define EGTPU_CERR_INV_TRACE_LEVEL       (EGTPU_CERR_CODE_BASE + 2)
#define EGTPU_CERR_STATS_DISABLED        (EGTPU_CERR_CODE_BASE + 3)
#define EGTPU_CERR_INV_STATS_TYPE        (EGTPU_CERR_CODE_BASE + 4)
#define EGTPU_CERR_SEND_MSG_FAILURE      (EGTPU_CERR_CODE_BASE + 5)
#define EGTPU_CERR_PATH_FAILURE          (EGTPU_CERR_CODE_BASE + 6)
#define EGTPU_LAST_CERR_CODE             (EGTPU_CERR_PATH_FAILURE)

#endif
