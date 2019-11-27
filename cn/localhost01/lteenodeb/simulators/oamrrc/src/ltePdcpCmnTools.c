/**************************************************************************************
*
*  FILE NAME  : cmn_tools.c
*
*  DESCRIPTION: functions to work with ZCB and other PDCP common parts
*
*
*	DATE	NAME	REFERENCE	REASON
*
*  Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "ltePdcpCmnTools.h"

/****************************************************************************
 * Variables
 ****************************************************************************/

#ifdef UT_TESTING
static ModuleLogDetail PDCPModuleLogDetail_g;
#endif
ModuleLogDetail *pdcpModuleLogDetail_p = PNULL;

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : initPdcpLog
 * Inputs         : void *fn - filename for log, PNULL if use LTE_CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inits pdcp log context 
 ****************************************************************************/
void initPdcpLog(void *fn)
{
#ifdef UT_TESTING
	if(PNULL==pdcpModuleLogDetail_p)
	{
		pdcpModuleLogDetail_p=&PDCPModuleLogDetail_g;
		semInit(&pdcpModuleLogDetail_p->logLock,1);
		pdcpModuleLogDetail_p->fp = PNULL;
		pdcpModuleLogDetail_p->level = LOG_NONE;
		if(PNULL!=fn)
		{
			strcpy(pdcpModuleLogDetail_p->filename,(UChar8*)fn);
			createLogFile(pdcpModuleLogDetail_p->filename);
			pdcpModuleLogDetail_p->fp=fopen(pdcpModuleLogDetail_p->filename,"w");
			if(PNULL==pdcpModuleLogDetail_p->fp)
			{
				ltePanic("Unable to open file. %s", pdcpModuleLogDetail_p->filename);
			}
		}
	}
#endif
}

/****************************************************************************
 * Function Name  : redirectPdcpLog
 * Inputs         : void *fn - filename for log, PNULL if use LTE_CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : redirect pdcp log output
 ****************************************************************************/
void redirectPdcpLog(void *fn)
{
#ifdef UT_TESTING
	FILE *fp=PNULL;
	if(PNULL!=pdcpModuleLogDetail_p)
	{
		/*redirect log to other file*/
		if(PNULL!=fn)
		{
			strcpy(pdcpModuleLogDetail_p->filename,(UChar8*)fn);
			createLogFile(pdcpModuleLogDetail_p->filename);
			if(PNULL!=pdcpModuleLogDetail_p->fp)
			{
				fp=freopen(pdcpModuleLogDetail_p->filename,"w",pdcpModuleLogDetail_p->fp);
			}
			else
			{
				fp=fopen(pdcpModuleLogDetail_p->filename,"w");
			}
			if(PNULL!=fp)
			{
				pdcpModuleLogDetail_p->fp = fp;
			}
			else
			{
				ltePanic("Unable to open file. %s", pdcpModuleLogDetail_p->filename);
			}
		}
		else
		{
			if(PNULL!=pdcpModuleLogDetail_p->fp)
			{
				fclose(pdcpModuleLogDetail_p->fp);
				pdcpModuleLogDetail_p->fp=PNULL;
			}
		}
	}
#endif
}

/****************************************************************************
 * Function Name  : pdcpLogLevel
 * Inputs         : UInt32 level
 * Outputs        : None.
 * Returns        : None.
 * Description    : Sets pdcp log level
 ****************************************************************************/
void pdcpLogLevel(UInt32 level)
{
	if(PNULL!=pdcpModuleLogDetail_p)
	{
		pdcpModuleLogDetail_p->level=level;
	}
}

/****************************************************************************
 * Function Name  : pdcpGetLogLevel
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : Returns pdcp log level
 * Description    : Returns pdcp log level
 ****************************************************************************/
UInt32 pdcpGetLogLevel(void)
{
	if(PNULL!=pdcpModuleLogDetail_p)
	{
		return pdcpModuleLogDetail_p->level;
	}
	else
	{
		return LOG_NONE;
	}
}

/****************************************************************************
 * Function Name  : cleanPdcpLog
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans pdcp log context 
 ****************************************************************************/
void cleanPdcpLog(void)
{
	/*do not clean pdcp context to preserve filename*/
#ifndef UT_TESTING
	if(PNULL!=pdcpModuleLogDetail_p)
	{
		if(PNULL!=pdcpModuleLogDetail_p->fp)
		{
			fclose(pdcpModuleLogDetail_p->fp);
		}
		semDestroy(&pdcpModuleLogDetail_p->logLock);
	}
	pdcpModuleLogDetail_p=PNULL;
#endif
}
