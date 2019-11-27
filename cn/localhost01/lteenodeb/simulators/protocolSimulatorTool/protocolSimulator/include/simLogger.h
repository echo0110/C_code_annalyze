/**************************************************************************************
*
*  FILE NAME  : simLogger.h
*
*  DESCRIPTION: Simulator logger engine definition
*
*
*   DATE    NAME    REFERENCE   REASON
*
*  Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef _SIM_LOGGER_DEF_
#define _SIM_LOGGER_DEF_

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "simTypes.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/


#define SIM_LOG(level,...) loggerLog(level, __VA_ARGS__);

#ifdef UT_TESTING
#define CHECK_PNULL(pointer_par) if(PNULL==(pointer_par)) {SIM_LOG(SIM_LOG_CRITICAL,"%s:PNULL detected",__FUNCTION__); abort();}
#define CHECK_PNULL_RET(pointer_par,ret_par) if(PNULL==(pointer_par)) {SIM_LOG(SIM_LOG_WARNING,"%s:PNULL detected",__FUNCTION__);return ret_par;}
#define CHECK_EXP(expression) if(expression) {SIM_LOG(SIM_LOG_CRITICAL, "%s:CHECK_EXP expresson is FALSE",__FUNCTION__); abort();}
#define CHECK_EXP_RET(expression,ret_par) if(expression) {SIM_LOG(SIM_LOG_WARNING,"%s:CHECK_EXP_RET expresson is FALSE",__FUNCTION__);return ret_par;}
#else
#define CHECK_PNULL(pointer_par)
#define CHECK_PNULL_RET(pointer_par,ret_par) if(PNULL==(pointer_par)) {return ret_par;}
#define CHECK_EXP(expression)
#define CHECK_EXP_RET(expression,ret_par) if(expression) {return ret_par;}
#endif

enum {
    SIM_LOG_NONE = 0,
    SIM_LOG_CRITICAL,
    SIM_LOG_WARNING,
    SIM_LOG_MAJOR,
    SIM_LOG_MINOR,
    SIM_LOG_INFO,
    SIM_LOG_DETAIL,
    SIM_LOG_DUMP,
    SIM_MAX_LOG_LEVEL = 255
};

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : loggerInit
 * Inputs         : void *fileName - filename for log, PNULL for using CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inits log context
 ****************************************************************************/
void loggerInit(void *fileName);

/****************************************************************************
 * Function Name  : loggerRedirectLog
 * Inputs         : void *fileName - filename for log, PNULL for using CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : redirect log output
 ****************************************************************************/
void loggerRedirectLog(void *fileName);

/****************************************************************************
 * Function Name  : loggerSetLogLevel
 * Inputs         : UInt32 level
 * Outputs        : None.
 * Returns        : None.
 * Description    : Sets log level
 ****************************************************************************/
void loggerSetLogLevel(UInt32 level);

/****************************************************************************
 * Function Name  : loggerGetLogLevel
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : Returns log level
 * Description    : Returns log level
 ****************************************************************************/
UInt32 loggerGetLogLevel(void);

/****************************************************************************
 * Function Name  : loggerCleanLog
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans log context
 ****************************************************************************/
void loggerCleanLog(void);

/****************************************************************************
 * Function Name  : loggerLog
 * Inputs         : level - Log level.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Puts log record to log storage.
 ****************************************************************************/
void loggerLog(UInt16 level, const UChar8 *format, ... );

#endif /*_SIM_LOGGER_DEF_*/
