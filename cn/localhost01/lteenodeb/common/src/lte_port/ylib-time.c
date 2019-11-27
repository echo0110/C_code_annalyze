/***************************************************************************
*
* ARICENT -
*
* Copyright (C) 2014 Aricent Inc . All Rights Reserved.
*
****************************************************************************
*
* \$Id: ylib-time.c
*
****************************************************************************
*
* File Description: This file contains APIs for retrieving system time and
*                   performing various operations on this time.
*
****************************************************************************
* Revision Details
* ----------------------
*
***************************************************************************/

#include <assert.h>
#include <sys/timeb.h>
#include "ylib.h"

/*****************************************************************************
 * FUNCTION: ytGetSystemTime
 *
 * DESCRIPTION:
 *          This function returns current system time, which consists
 *          of seconds and milliseconds.
 *
 * ARGS:
 *          None
 *
 * RETURNS:
 *          YTIME_T - structure which represents current system time
 *
 ******************************************************************************/
YTIME_T ytGetSystemTime(void)
{
	YTIME_T curtime;
	struct timeb systime;

	ftime(&systime);

	curtime.sec = (unsigned int)systime.time;
	curtime.msec = systime.millitm;

	return curtime;
}

/*****************************************************************************
 * FUNCTION: ytTimeDiff
 *
 * DESCRIPTION:
 *          This function returns difference in milliseconds between two timestamps
 *
 * ARGS:
 *          YTIME_T* - pointer to structure which represents time
 *          YTIME_T* - pointer to structure which represents time
 *
 * RETURNS:
 *          signed long - difference between two timestamps
 *
 ******************************************************************************/
signed long ytTimeDiff(const YTIME_T *ptm_one, const YTIME_T *ptm_two)
{
    return (((ptm_one->sec) % 1000000) * 1000 + ptm_one->msec)
			- (((ptm_two->sec) % 1000000) * 1000 + ptm_two->msec);
}

/*****************************************************************************
 * FUNCTION: ytTimeAdd
 *
 * DESCRIPTION:
 *          This function adds two times, first represents by YTIME_T,
 *          second represents by milliseconds
 *
 * ARGS:
 *          YTIME_T* - pointer to structure which represents time
 *          signed long - milliseconds to add
 *
 * RETURNS:
 *          YTIME_T - new time value
 *
 ******************************************************************************/
YTIME_T ytTimeAdd(const YTIME_T *ptm, signed long msec)
{
    YTIME_T newtime;
    unsigned long ppart = (ptm->sec % 1000000) * 1000 + ptm->msec + msec;

    newtime.sec = (ptm->sec / 1000000 ) * 1000000 + ppart / 1000;
    newtime.msec = ppart % 1000;

    return newtime;
}
