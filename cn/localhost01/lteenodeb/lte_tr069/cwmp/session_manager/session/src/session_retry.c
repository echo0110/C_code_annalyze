/********************************************************************************
 *
 *      FILE NAME   : session_retry.c
 *
 *      DESCRIPTION : This file is used for maintaing the session
 *
 *      REVISION HISTORY:
 *
 *      NAME             DATE            REFERENCE               REASON
 *_______________________________________________________________________________
 *
 *   Vikas Upadhyay    Aug 17,2012   TR-069 Amendment 4     Initial Creation
 *   Anant K Sharma    Mar 25, 2013                         removed compilation warnings 
 *
 *
 *       All Rights Reserved. Copyright @ Aricent Group, 2012.
 *
 ********************************************************************************/


#include        <soapStub.h>
#include        <stdlib.h>
#include        <logger.h>
#include        <session_retry.h>
#include        <ip_manager.h>
#include        <math.h>
#include <signal.h>
/* SPR 13359 Fix Start */
#include <map_glue.h>
/* SPR 13359 Fix End */

/******************************************************************************
  Global Variables
 ******************************************************************************/

/* Count for no. of time session retried. */
static UINT4 session_manager_retry_count = 0;
/* SPR 16327 Fixed Start  */
extern U8 g_Reboot_recv;
extern UInt8 MaxSessionRetryCount;
extern void glue_sessionClosedCallback(char *);
/* SPR 16327 Fixed End  */
/* SPR 21002 FIXED START */
extern UInt8 ABSPath[CONFIG_LEN];
/* SPR 21002 FIXED END */


/******************************************************************************
  Function Definitions
 ******************************************************************************/

static SInt32 power(SInt32 x, SInt32 y)
{
    SInt32 ret = 1,i = 0;
    for (i = 1; i<= y; i++)
    {
        ret = ret * x;
    }
    return ret;
}



/********************************************************************************
 *
 *       Function Name : ipr_cwmp_Session_Retry
 *
 *       Description   : This function returns the retry count for
 *                       for event  for which session is failed
 *
 *       Returns       : status
 *
 * *********************************************************************************/

sInformResponse * ipr_cwmp_Session_Retry(sInform *informReq,
        Uint8 *URL,
        sHeader * header,
        Uint8 * username,
        Uint8 * password,
        Uint8 **session_id)
{
	UINT4 wait_interval = 0;
	UINT4 interval_multiplier = 0;
	SInt32 status = 0;
	sInformResponse *informResp = NULL;
       /* SPR 20703 FIXED START */
        /* code removed */
       /* SPR 20703 FIXED END */
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
	sInform *new_informReq = NULL;
	UINT4 time = 0;
	UINT4 min_time = 0;
	UINT4 max_time = 0;
   /* SPR 21002 FIXED START */
    FILE *fp = NULL;
		UInt8 ABSPath3[CONFIG_LEN] = {'\0'};
		strcpy((char *)ABSPath3,(char *)ABSPath);
		strncat((char *)ABSPath3 ,"/reset" , strlen("/reset") + 1);
   /* SPR 21002 FIXED END */
	/* SPR 16327 Fixed Start  */
   /* SPR 21002 FIXED START */
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "debug MaxSessionRetryCount %d session_manager_retry_count= %d abs path = %s abs path original=%s",MaxSessionRetryCount, session_manager_retry_count, ABSPath3, ABSPath);
	if (MaxSessionRetryCount <= (session_manager_retry_count-1))
   {
       /*SPR 18016 Fixed Start */	
       fp = fopen((char *)ABSPath3, "w");
       if (fp == NULL)
       {
           TRACE (LOG_MAJOR , SESSION_MANAGER, "File creation Fail\n");

       }
       /* SPR 21002 FIXED END */
       TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with NULL XML exceed retry count",__FUNCTION__);
       g_Reboot_recv = 1;
       glue_sessionClosedCallback((char *)session_id);
       /* SPR 18228 FIXED START */
       /* SPR 20703 FIXED START */
       informReq->RetryCount = 0;
       session_manager_retry_count = 0;
       /* SPR 20703 FIXED END */
       /* SPR 18228 FIXED END */
       /*spr 18016 fixed end */

       /* SPR 16327 Fixed End */

       /* coverity 113254/113381 fix start */
       if (NULL != fp)
           fclose(fp);
       /* coverity 113254/113381 fix start */
       return NULL;
   }
   else
   {
       wait_interval = ipr_cwmp_getCWMPRetryMinimumWaitInterval();
		interval_multiplier = ipr_cwmp_getCWMPRetryIntervalMultiplier();
		TRACE (LOG_CRITICAL , "" ,"\nwait_interval  = %d\t interval_multiplier =%d\n",wait_interval, interval_multiplier);
			
   /* SPR 21002 FIXED START */
        /* code removed */
			min_time = wait_interval * power(interval_multiplier / 1000, informReq->RetryCount);
			max_time = wait_interval * power(interval_multiplier / 1000, informReq->RetryCount + 1);
			time  = ipr_cwmp_iprand(min_time  , max_time);	

   /* SPR 21002 FIXED END */
		TRACE (LOG_CRITICAL , SESSION_MANAGER,"\nmin_time = %d\tmax_time = %d\twaiting_time = %d\tRetryCount = %d",
				min_time, max_time,time,informReq->RetryCount + 1);
		sleep(time);
		TRACE (LOG_CRITICAL , SESSION_MANAGER,"debug 2 in retry fun\n");
		new_informReq = ipr_cwmp_createInformReq(informReq->DeviceId, informReq->ParameterList, informReq->Event);
		if(NULL == new_informReq)
		{
			/*SPR 18016 Fixed Start */
		    /* SPR 18228 FIXED START */
        /*	DEALLOCATE(informReq);*/
            /* SPR 18228 FIXED END */
			/*SPR 18016 Fixed End */

			return NULL;		
		}

		informResp = ipr_cwmp_Inform(new_informReq, URL,  &status, &header, username, password, session_id);

		ipr_cwmp_set_retry_count(status);

		if (status != SUCCESS)
		{
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "In %s status is FAILED",__FUNCTION__);
        /* SPR 20703 FIXED START */
          TRACE (LOG_CRITICAL , SESSION_MANAGER, "In %s status is FAILED, new_informReq->RetryCount %d",
                                                     __FUNCTION__,new_informReq->RetryCount);
        /* SPR 20703 FIXED END */
			informResp = ipr_cwmp_Session_Retry(new_informReq, URL, header ,username,password, session_id);
			/*SPR 18016 Fixed Start */
            /* SPR 22541 Start */
            if(NULL != new_informReq)
            {
                if (NULL != new_informReq->Event)
                {
                    if (NULL != new_informReq->Event->__ptrEventStruct)
                    {
                        DEALLOCATE(new_informReq->Event->__ptrEventStruct);
                    }
                    DEALLOCATE(new_informReq->Event);
                }
                if (NULL != new_informReq->ParameterList)
                {
                    if (NULL != new_informReq->ParameterList->__ptrParameterValueStruct)
                    {
                        DEALLOCATE(new_informReq->ParameterList->__ptrParameterValueStruct);
                    }
                    DEALLOCATE(new_informReq->ParameterList);
                }
                DEALLOCATE(new_informReq);
            }
            /* SPR 22541 End */
			/*SPR 18016 Fixed End */

			return informResp;
		}
		else
		{
        /* SPR 20703 FIXED START */
        /* code removed */
        /* SPR 20703 FIXED END */
         
			/*SPR 18016 Fixed Start */
            /* SPR 22541 Start */
            if(NULL != new_informReq)
            {
                if (NULL != new_informReq->Event)
                {
                    if (NULL != new_informReq->Event->__ptrEventStruct)
                    {
                        DEALLOCATE(new_informReq->Event->__ptrEventStruct);
                    }
                    DEALLOCATE(new_informReq->Event);
                }
                if (NULL != new_informReq->ParameterList)
                {
                    if (NULL != new_informReq->ParameterList->__ptrParameterValueStruct)
                    {
                        DEALLOCATE(new_informReq->ParameterList->__ptrParameterValueStruct);
                    }
                    DEALLOCATE(new_informReq->ParameterList);
                }
                DEALLOCATE(new_informReq);
            }
            /* SPR 22541 End */
			/*SPR 18016 Fixed End */
			TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with SUCCESS ",__FUNCTION__);
			return informResp;
		}
	}
}



/********************************************************************************
 *
 *       Function Name :  ipr_cwmp_get_retry_count
 *
 *       Description   : This function returns the retry count for
 *                       for event  for which session is failed
 *
 *       Returns       : status
 *
 * *********************************************************************************/

UINT4 ipr_cwmp_get_retry_count(void)
{
    /* SPR 21002 FIXED START */
    UInt8 ABSPath3[CONFIG_LEN] = {'\0'};
    strcpy((char *)ABSPath3,(char *)ABSPath);
    strncat((char *)ABSPath3 ,"/reset" , strlen("/reset"));

    UInt8 ABSPath4[CONFIG_LEN] = {'\0'};
    strcpy((char *)ABSPath4,(char *)ABSPath);
    strncat((char *)ABSPath4 ,"/retry" , strlen("/retry"));
    FILE *fp = fopen((char *)ABSPath4, "r");
    FILE *fp1 = fopen((char *)ABSPath3, "r");
    UINT4 retry_count = 0;
    UINT4 len = 0;
    Uint8 buffer[10] = {0};


    /* coverity 113253 fix start */
    if (NULL != fp && NULL != fp1)
    {
        fgets(buffer, FOUR, fp);
        len = strlen(buffer);
        if (buffer[len-1] == '\n')
        {
            buffer[len-1] = '\0';

        }
        retry_count = atoi(buffer);
        fclose(fp);
        fclose(fp1);
        return retry_count;
    }

    /* coverity 113253 fix start */
    /* SPR 21342 FIXED START */
    else
    {
        if (NULL != fp)
        {
            fgets(buffer, FOUR, fp);
            len = strlen(buffer);
            if (buffer[len-1] == '\n')
            {
                buffer[len-1] = '\0';

            }
            retry_count = atoi(buffer);
            fclose(fp);
            return retry_count;
        }

    }
    /* SPR 21342 FIXED END */


    /* SPR 21002 FIXED END */
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);

    /* coverity 113253 fixed start */
    if (NULL != fp1)
        fclose(fp1);
    /* coverity 113253 fixed end */
    return session_manager_retry_count;
}




/********************************************************************************
 *
 *       Function Name : ipr_cwmp_set_retry_count
 *
 *       Description   : This function increse the retry count for event
 *                               for which session is failed
 *                       This function  sets retry count to ZERO for event
 *                               for which session is PASSED.
 *
 *       Returns       : retry_count
 *
 *********************************************************************************/

void ipr_cwmp_set_retry_count(UINT4 status)
{
    /* SPR 21002 FIXED START */
    UInt8 ABSPath3[CONFIG_LEN] = {'\0'};
    strcpy((char *)ABSPath3,(char *)ABSPath);
    strncat((char *)ABSPath3 ,"/reset" , strlen("/reset"));

    UInt8 ABSPath4[CONFIG_LEN] = {'\0'};
    strcpy((char *)ABSPath4,(char *)ABSPath);
    strncat((char *)ABSPath4 ,"/retry" , strlen("/retry"));
    UINT4 count = 0;
    Uint8 buffer[10] = {0};
    FILE *fp = NULL;
    FILE *fp1 = NULL;
    UINT4 len = 0;
    /* SPR 21002 FIXED END */
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
        /* SPR 16451 Fix Start */
    if (status != SUCCESS && status != CRITICAL_ERROR)
        /* SPR 16451 Fix End */
    {
        session_manager_retry_count++;
   /* SPR 21002 FIXED START */
        fp = fopen((char *)ABSPath4, "r");
        if (fp != NULL)
        {
            fgets(buffer, FOUR, fp);
            len = strlen(buffer);
            if (buffer[len-1] == '\n')
            {
                buffer[len-1] = '\0';

            }
            count = atoi(buffer);
            TRACE (LOG_DETAIL , SESSION_MANAGER, "retry count = %d", count);
            fclose(fp);
        }
        fp = fopen((char *)ABSPath4, "w");
        fp1 = fopen((char *)ABSPath3, "r");
	     /* coverity 113250 fix start */
        if (fp1 != NULL && fp != NULL)
            /* coverity 113250 fix end */
        {
            fprintf(fp, "%d", count+1);
        }
        else
            /* coverity 113380/113382 fixed start */
        {
            if (NULL != fp)
            {
                fprintf(fp, "%d", session_manager_retry_count);

            }
        }
        if (NULL != fp1)
            fclose(fp1);
        if (NULL != fp)
            /* coverity 113380/113382 fixed end */
            fclose(fp);
        /* SPR 21002 FIXED END */
    }
    else
    {
        session_manager_retry_count = 0;
    }
    TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with rty cnt %d",__FUNCTION__,session_manager_retry_count);
}
