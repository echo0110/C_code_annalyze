/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLog.c,v 1.1.1.1.16.1 2010/10/25 10:29:25 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper implementation of the Logs functionality.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLog.c,v $
 * Revision 1.1.1.1.16.1  2010/10/25 10:29:25  gur23971
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.13.4.1  2009/06/09 11:24:51  gur19140
 * first wave of Integration
 *
 * Revision 1.16  2009/05/28 13:43:18  gur19140
 * *** empty log message ***
 *
 * Revision 1.15  2009/05/25 09:32:43  gur18569
 *  returning string length from addTime func
 *
 * Revision 1.14  2009/05/06 05:39:24  gur20548
 * pdcp common merged with lte common
 *
 * Revision 1.1  2009/05/04 14:52:04  gur20548
 * pdcp files added
 *
 * Revision 1.13  2009/04/07 05:43:29  gur19140
 * *** empty log message ***
 *
 * Revision 1.12  2009/04/06 10:13:52  gur19140
 * log level related fix
 *
 * Revision 1.11  2009/04/01 06:23:11  gur19140
 * Ingeration testing related changes
 *
 * Revision 1.10  2009/03/27 14:33:54  gur19140
 * Initgration testing related changes
 *
 * Revision 1.9  2009/03/25 06:32:58  gur20439
 * letLog function changed to support log dump into file for each module.:
 *
 * Revision 1.9  2009/03/25 11:26:05  gur20439
 * logs can be directed in files for each module
 *
 * Revision 1.8  2009/02/20 08:02:35  gur19140
 * logs can be directed in files mac simulator option
 *
 * Revision 1.7  2008/11/18 04:53:07  gur11912
 * macro defined LTE_LOG
 *
 * Revision 1.6  2008/10/22 12:42:26  gur11912
 * fixed for performance changes
 *
 * Revision 1.5  2008/09/18 07:30:12  gur19836
 * Review comments incorporated
 *
 * Revision 1.4  2008/09/09 11:11:53  ssinghal
 * comments added to functions
 *
 * Revision 1.3  2008/08/29 06:17:05  ssinghal
 * state function setting given
 *
 * Revision 1.2  2008/08/25 10:05:02  ssinghal
 * Template applied and minor changes done
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLog.h"
#include <lteMisc.h>
#include <sys/timeb.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

/****************************************************************************
  Private Definitions
 ****************************************************************************/

#define MAX_LOG_BUF_LEN 1500

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
UInt8 enableLog_g;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
/****************************************************************************
 * Function Name  : lteLogLevel
 * Inputs         : level - the level of logging required.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This sets the log level of the module.
 ****************************************************************************/
void lteLogLevel(UInt8 level)
{
	qvLogLevel(level);
}

/****************************************************************************
 * Function Name  : lteSetLogState
 * Inputs         : logState - Whether to ON or OFF the logs.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Sets the log state to ON or OFF.
 ****************************************************************************/
void lteSetLogState(LogState logState)
{
	if (logState == LOG_ON) {
		qvLogLevel(MAX_LOG_LEVEL);
	} else if (logState == LOG_OFF) {
		fprintf(stderr, "set log off\n");
		abort_wrapper();
		qvLogLevel(LOG_NONE);
	}
}

/****************************************************************************
 * Function Name  : ltePanic
 * Inputs         : format - string as similar to printf.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used for indicating a major failure in the system.
 ****************************************************************************/
void ltePanic(const Char8 *format, ...)
{
	/* SPR 20430 Changes Start */
#ifdef __x86_64__
	/* Coverity 54680*/
	va_list A = {{0} };
	/* Coverity 54680*/
#else
	/* SPR 20430 Changes End */
	/* Coverity 54680*/
	va_list A = {0};
	/* Coverity 54680*/
	/* SPR 20430 Changes Start */
#endif
	/* SPR 20430 Changes End */
	va_start(A, format);
	qvPanic(format, A);
	va_end(A);

	qvAbort();
}
/* SPR_7214_CHANGE_START */
/****************************************************************************
 * Function Name  : lteLogDropedPkt
 * Inputs         : format - string as similar to printf.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used for indicating a packet water generated in the system.
 ****************************************************************************/
void lteLogDropedPkt(const Char8 *format, ...)
{
	/* SPR 20430 Changes Start */
#ifdef __x86_64__
	/* Coverity 55051*/
	va_list A = {{0} };
	/* Coverity 55051*/
#else
	/* SPR 20430 Changes End */
	/* Coverity 55051*/
	va_list A = {0};
	/* Coverity 55051*/
	/* SPR 20430 Changes Start */
#endif
	/* SPR 20430 Changes End */
	va_start(A, format);
	qvPrintfVa(format, A);
	va_end(A);
}

/* SPR_7214_CHANGE_END */

/****************************************************************************
 * Function Name  : lteWarning
 * Inputs         : format - string as similar to printf.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Used for indicating a warning generated in the system.
 ****************************************************************************/
/* +- SPR 17777 */
#ifdef LOG_UT_ENABLED
void lteWarning(const Char8 *format, ...)
{
    va_list A;

    va_start(A, format);
    qvWarning(format, A);
    va_end(A);
}
#endif
/* +- SPR 17777 */
