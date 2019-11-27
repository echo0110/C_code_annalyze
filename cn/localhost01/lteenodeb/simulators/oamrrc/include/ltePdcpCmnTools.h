/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpCmnTools.h,v 1.1.4.1 2010/08/02 08:57:18 gur20491 Exp $
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
 * Revision 1.1.4.1  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:25  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1.2.1  2009/11/12 11:29:25  gur19479
 * updated to remove unncessary fucntions
 *
 * Revision 1.1.2.1  2009/06/16 10:26:13  gur19836
 * *** empty log message ***
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

#ifdef UT_TESTING
#define PDCP_LOG(param1,...) LTE_LOG(param1,pdcpModuleLogDetail_p,__VA_ARGS__);

#define CHECK_PNULL(pointer_par) if(PNULL==(pointer_par)) {ltePanic("%s:PNULL detected",__FUNCTION__);}
#define CHECK_PNULL_RET(pointer_par,ret_par) if(PNULL==(pointer_par)) {PDCP_LOG(LOG_WARNING,"%s:PNULL detected",__FUNCTION__);return ret_par;}
#define CHECK_EXP(expression) if(expression) {ltePanic("%s:CHECK_EXP expresson is FALSE",__FUNCTION__);}
#define CHECK_EXP_RET(expression,ret_par) if(expression) {PDCP_LOG(LOG_WARNING,"%s:CHECK_EXP_RET expresson is FALSE",__FUNCTION__);return ret_par;}
#else
#define PDCP_LOG(param1,...)
#define CHECK_PNULL(pointer_par)
#define CHECK_PNULL_RET(pointer_par,ret_par) if(PNULL==(pointer_par)) {return ret_par;}
#define CHECK_EXP(expression)
#define CHECK_EXP_RET(expression,ret_par) if(expression) {return ret_par;}
#endif

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern ModuleLogDetail *pdcpModuleLogDetail_p;

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

#endif
