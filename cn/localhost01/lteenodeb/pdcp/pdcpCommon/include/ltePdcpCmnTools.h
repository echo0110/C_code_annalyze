/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpCmnTools.h,v 1.1.1.1.16.1 2010/10/25 08:05:46 gur21010 Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions to work with ZCB and other PDCP common parts 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpCmnTools.h,v $
 * Revision 1.1.1.1.16.1  2010/10/25 08:05:46  gur21010
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.2  2009/07/28 15:36:30  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.1.2.1  2009/06/09 11:30:19  gur19140
 * first wave of Integration
 *
 * Revision 1.1  2009/05/20 13:46:16  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/14 13:09:16  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _CMN_TOOLS_DEF_
#define _CMN_TOOLS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "ltePdcpCmnLteFramework.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#ifdef WIN32
#define LTE_CONSOLE_DEV "con"
#else
#define LTE_CONSOLE_DEV "/dev/tty"
#endif

#define PDCP_LOG(param1,...)
#define CHECK_PNULL(pointer_par)
#define CHECK_PNULL_RET(pointer_par,ret_par) if(PNULL==(pointer_par)) {return ret_par;}
#define CHECK_EXP(expression)
#define CHECK_EXP_RET(expression,ret_par) if(expression) {return ret_par;}

#define LOG_MAX_STRING_LEN  1024
/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct ZCB_state_struct
{
	void *buffer;
	UInt32 cur_pos;
	MSGSEGMENT segment;
	void *last;
	UInt8 isEnd;
} ZCB_state, *ZCB_state_PTR;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* SPR 5599_129178 Fix Start */
extern ModuleLogDetail *pdcpModuleLogDetail_p;
/* SPR 5599_129178 Fix End */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : initPdcpLog
 * Inputs         : void *fn - filename for log, PNULL if use LTE_CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inits pdcp log context 
 ****************************************************************************/
void initPdcpLog(void *fn);

/****************************************************************************
 * Function Name  : redirectPdcpLog
 * Inputs         : void *fn - filename for log, PNULL if use LTE_CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : redirect pdcp log output
 ****************************************************************************/
void redirectPdcpLog(void *fn);

/****************************************************************************
 * Function Name  : pdcpLogLevel
 * Inputs         : UInt32 level
 * Outputs        : None.
 * Returns        : None.
 * Description    : Sets pdcp log level
 ****************************************************************************/
void pdcpLogLevel(UInt32 level);

/****************************************************************************
 * Function Name  : pdcpGetLogLevel
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : Returns pdcp log level
 * Description    : Returns pdcp log level
 ****************************************************************************/
UInt32 pdcpGetLogLevel(void);

/****************************************************************************
 * Function Name  : cleanPdcpLog
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans pdcp log context 
 ****************************************************************************/
void cleanPdcpLog(void);

/****************************************************************************
 * Function Name  : pdcpSwap16
 * Inputs         : UInt16 value
 * Outputs        : None.
 * Returns        : value with swapped bytes
 * Description    : calculates value with swapped bytes
 ****************************************************************************/
UInt16 pdcpSwap16(UInt16 value);

/****************************************************************************
 * Function Name  : pdcpSwap32
 * Inputs         : UInt32 value
 * Outputs        : None.
 * Returns        : value with swapped bytes
 * Description    : calculates value with swapped bytes
 ****************************************************************************/
UInt32 pdcpSwap32(UInt32 value);

/*Initialize ZCB iterator (ZCB_state)*/
void Init_ZCB_state(void *buffer,ZCB_state_PTR st);

/*Returns next BYTE from ZCB. When reading after end of buffer, returns 0*/
UInt8 GetNextUInt8_ZCB(ZCB_state_PTR st);

/*Copies next size BYTEs from ZCB to dst_buf. Returns number of copied bytes*/
UInt32 GetNextZCB(UInt8 *dst_buf,SInt32 size,ZCB_state_PTR st);

/*Seek to specified position in ZCB. Returns new position offset in ZCB*/
UInt32 SeekZCB(SInt32 position,ZCB_state_PTR st);

/*Returns next DWORD from ZCB*/
UInt32 GetNextUInt32_ZCB(ZCB_state_PTR st);

/*Returns next WORD from ZCB*/
UInt16 GetNextUInt16_ZCB(ZCB_state_PTR st);

/*Make DWORD from byte[4] array*/
UInt32 MakeUInt32(UInt8 *x);

/*Move to next ZCB. Return st->last, 0 if no next segment*/
void *ZCB_next_segment(ZCB_state_PTR st);

void userPrintRawBuffer(const Char8* strHeader, const UInt8* buf, UInt32 len_buf, UInt32 fmt);

char* pdcp_change_data_to_str(UInt16 len, UInt8* data);
#endif
