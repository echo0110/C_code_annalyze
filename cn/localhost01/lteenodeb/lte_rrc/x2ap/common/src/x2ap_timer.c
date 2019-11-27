/*******************************************************************************
*
*  FILE NAME   : x2ap_timer.c
*
*  DESCRIPTION : This file contains the definition of functions which will be 
*                used for start time amd stop timer and other timer related 
*                activities.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#include "x2ap_timer.h"
#include "commoninc.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"

/******************************************************************************
*   FUNCTION NAME: x2ap_start_timer
 * INPUTS        : timer_duration - Duration (in milliseconds) of the timer
 *                 p_timer_data - Timer data that will be attached to timer
 *                 timer_data_size - Size of timer data
 *                 is_repeated - Indicates whether timer is repeated
 * OUTPUTS       : None
 * RETURNS       : Timer Id as allocated by the CSPL 
 * DESCRIPTION   : This function is used for starting timer.
 * NOTES         : Timer allocates buffer which has size timer_data_size and
*       this data will be freed in x2ap_stop_timer
******************************************************************************/
x2ap_timer_t x2ap_start_timer(
    /* Duration (in milliseconds) of the timer to be started */
    U32         timer_duration,
    /* Timer data that will be attached to timer */
    void        *p_timer_data,
    U16         timer_data_size,    /* Size of timer data */
    x2ap_bool_t  is_repeated)       /* Indicates whether timer is repeated */
{
    QTIME qtime;
    void *p_timer_buf = X2AP_NULL;
    x2ap_timer_t timer = X2AP_NULL;

    X2AP_ASSERT(0 != timer_duration);

    p_timer_buf = x2ap_mem_get(timer_data_size);

    if (X2AP_NULL == p_timer_buf)
    {
        return X2AP_NULL;
    }

    X2AP_MEMCPY(p_timer_buf, p_timer_data, timer_data_size);

    qtime.s = timer_duration / MILISECS_1SEC;
    qtime.us = (timer_duration % MILISECS_1SEC) * MILISECS_1SEC;

    timer = qvTimerStart(&qtime, p_timer_buf, is_repeated);

    X2AP_TRACE(X2AP_BRIEF,
        "x2ap_start_timer: Timer Id [%p]: Duration: [%d]msec",
        timer,
        timer_duration);

    if (X2AP_NULL == timer)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_start_timer: Timer Start failed");
        ALARM_MSG_L3(RRC_MODULE_ID, TIMER_START_FAILED_ALARM_ID,CRITICAL_ALARM,TIMER_CREATE_NO_BUFF_IN_TIMER_POOL_ERR_CAUSE_ID);

        /* Release the memory allocated for storing the timer data */
        x2ap_mem_free(p_timer_buf);
    }

    return timer;
}


/******************************************************************************
*   FUNCTION NAME: x2ap_stop_timer
 * INPUTS        : timer - Timer Id as allocated by the CSPL
 * OUTPUTS       : None
 * RETURNS       : None
 * DESCRIPTION   : This function is used for stopping timer.
******************************************************************************/
void x2ap_stop_timer(
    x2ap_timer_t timer) /* Timer Id as allocated by the CSPL */
{
    /*SPR_21694_START*/
    /* Code Removed */
    /*SPR_21694_END*/

    if (X2AP_NULL == timer)
    {
        X2AP_TRACE(X2AP_ERROR, "x2ap_stop_timer: NULL timer passed!");
    }
    else
    {
        /*SPR_21694_START*/
        if(qvTimerRunning(timer))
        {
        void *p_timer_data = X2AP_NULL;

        p_timer_data = qvTimerData(timer);

        X2AP_TRACE(X2AP_DETAILED, "x2ap_stop_timer: Timer Id [%p]", timer);

        if (p_timer_data != X2AP_NULL)
        {
            x2ap_mem_free(p_timer_data);
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING, "x2ap_stop_timer: Timer Id [%p] contains "\
                "null data", timer);
        }

        qvTimerStop(timer, X2AP_NULL);
        }
        else
        {
            X2AP_TRACE(X2AP_INFO, "Timer Id [%p] already stopped", timer);
        }
        /*SPR_21694_END*/
    }
}


