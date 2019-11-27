/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLog.h,v 1.1.1.1.14.1 2010/10/25 10:35:14 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLog.h,v $
 * Revision 1.1.1.1.14.1  2010/10/25 10:35:14  gur23971
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.11.4.3  2009/07/28 08:44:10  gur11318
 * updated for log handling
 *
 * Revision 1.11.4.2  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.11.4.1  2009/04/23 05:54:09  gur18569
 * Enable logs when compiled with UT_TESTING flag.
 * Revision 1.11  2009/04/01 06:23:11  gur19140
 * Ingeration testing related changes
 *
 * Revision 1.10  2009/03/27 14:33:54  gur19140
 * Initgration testing related changes
 *
 * Revision 1.9  2009/03/25 07:38:38  gur20439
 * new function added as part of enahncement of logging.
 *
 * Revision 1.8  2008/11/18 04:52:50  gur11912
 * macro defined LTE_LOG
 *
 * Revision 1.7  2008/10/22 12:41:11  gur11912
 * fixed for performance changes
 *
 * Revision 1.6  2008/09/30 11:35:51  gur19836
 * Added Log level for Warnings
 *
 * Revision 1.5  2008/09/18 07:30:12  gur19836
 * Review comments incorporated
 *
 * Revision 1.4  2008/09/09 06:47:45  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.3  2008/08/29 06:18:39  ssinghal
 * logging states introduced
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/

#ifndef _LTE_LOG_
#define _LTE_LOG_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "mempool.h"
#include "lteMemPool.h"
#include "lteTypes.h"
#include "lteSync.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*#define LTE_LOG(param1,...)\
	    if(enableLog_g)\
	    lteLog(param1,__VA_ARGS__)*/

#ifdef LINUX_PC_TEST
short test_log(unsigned short usModId, unsigned short logLevel, char *fmt, ...);
#define LTE_LOG(logLevel, format, ...)	test_log(0x55, logLevel, (char *)format, ##__VA_ARGS__)
#else
#define LTE_LOG(logLevel, format, ...)
#endif

#define MAXFILENAME 256
#define MAXFILESIZE 2 * 1024 * 1024
/****************************************************************************
 * Exported Types
 ****************************************************************************/
enum {
	LOG_NONE = QVLOG_NONE,
	LOG_CRITICAL = QVLOG_CRITICAL,
	LOG_WARNING = QVLOG_WARNING,
	LOG_MAJOR = QVLOG_MAJOR,
	LOG_MINOR = QVLOG_MINOR,
	LOG_INFO = QVLOG_INFO,
	LOG_DETAIL = QVLOG_DETAIL,
	LOG_DUMP = QVLOG_DUMP,
	MAX_LOG_LEVEL = QVLOG_LIBRARY
};

typedef enum LogStateT {
	LOG_OFF,
	LOG_ON
} LogState;

typedef struct ModuleLogDetailT {
	FILE *fp;
	Char8 filename[MAXFILENAME];
	UInt32 level;
	LTE_SEM logLock;
} ModuleLogDetail;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 enableLog_g;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      Wrapper API to log.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32  lteLog(UInt32 level, ModuleLogDetail *det, const Char8 *format, ...);

/****************************************************************************
 *  function(): Description.
 *      Sets log level.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void lteLogLevel(UInt8 level);


/****************************************************************************
 *  function(): Description.
 *      Used to set the log state to ON or OFF.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void lteSetLogState(LogState logState);


/****************************************************************************
 *  function(): Description.
 *      Used to raise a critical problem.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void ltePanic(const Char8 *format, ...);

/* SPR_7214_CHANGE_START */
/****************************************************************************
 *  function(): Description.
 *      Used to raise a problem when packets are dropped.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

void lteLogDropedPkt(const Char8 *format, ...);
/* SPR_7214_CHANGE_END   */

/****************************************************************************
 *  function(): Description.
 *      Used to raise a warning.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
#ifdef LOG_UT_ENABLED
/*SPR 18490 +-*/
void lteWarning(const Char8 *format, ...);
/*SPR 18490 +-*/
#define ltewarning(format, ...);\
lteWarning(const Char8 *format, ...);
#else

#ifdef LINUX_PC_TEST
#define lteWarning(format, ...) test_log(0x56, 2, (char*)format, ##__VA_ARGS__)
#else
#define lteWarning(format, ...);
#endif

#endif
/* +- SPR 17777 */

/****************************************************************************
 *  function(): Description.
 *      Used to add timestamp in filename.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void addTimeStamp(char *buf);

/****************************************************************************
 *  function(): Description.
 *      Used to add time in log file.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
UInt32 addTime(char *buf);

/****************************************************************************
 *  function(): Description.
 *      Used to create log file.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void createLogFile(Char8 *file);

#endif
