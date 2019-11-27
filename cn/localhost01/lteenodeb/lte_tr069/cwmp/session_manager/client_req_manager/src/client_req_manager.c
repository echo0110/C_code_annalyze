/********************************************************************************
*
*               FILE NAME   : 	client_req_manager.c
*
*               DESCRIPTION : 	This file contains the functionality of adding a
*				 new client request into a  client request queue										.
*
*
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Aug 17, 2012   TR-069 Amendment 4   Initial Creation
*       Anant K Sharma Mar 25, 2013                        removed warnings from the code 
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/

#include <client_req_manager.h>
#include <stdio.h>
#include <common_def.h>
#include <logger.h>
#include <event_handler.h>
#include <ipr_cwmp_enb_wrapper.h>

/******************************************************************************
  Macros 
******************************************************************************/

#define FALSE 0
#define TRUE 1
#define CLIENT_RQUEST "Client Request Manager"
#define EVENT_CODE_TXN_COM "7 TRANSFER COMPLETE"
#define MAX_CODE_SIZE 65

/******************************************************************************
  Global Variables
******************************************************************************/
/* Queue maintained for Client Requests */
YSQUEUE g_clientRequestQ;

/******************************************************************************
   Function Definitions
******************************************************************************/

/********************************************************************************
*       Function Name : priorEventClient 
*       Description   : It's a priority function for queue used for Client Request    
*       		        which is returning default 1 
*	    Args	      : const YSNODE *  - Node in Queue
*       Returns       : returns only nteger value 1 for all nodes 
*********************************************************************************/
int ipr_cwmp_priorEventClient (const YSNODE *N)
{
    if(N)
    {
	   TRACE(LOG_CRITICAL , CLIENT_RQUEST, "Entering %s  YSNODE N is valid",__FUNCTION__);
    }    
    return 1; 
}



/********************************************************************************
*       Function Name :	client_manager_init
*       Description   : Initialization of ClientManager which involves initializing
*			            queue of Event.
*       Returns       : 1 -  Success
*			            0 -  Failure
*********************************************************************************/
int client_manager_init (void)
{
	TRACE(LOG_CRITICAL , CLIENT_RQUEST, "Entering %s  Function ",__FUNCTION__);
	int ret = FALSE; 
 	ysInit(&g_clientRequestQ, ipr_cwmp_priorEventClient);	
	TRACE(LOG_CRITICAL , CLIENT_RQUEST, "Exiting %s  Function ",__FUNCTION__);
	return ret;
}



/********************************************************************************
*       Function Name : addClientRequest 
*
*       Description   : inserts new node .i.e new Client request into the Client
*			request queue which is poped up by session manager. 
*
*       Returns       : 0 -  FAIL
*       		1 - SUCCESS
*
*********************************************************************************/
int ipr_cwmp_addClientRequest (ClientRequest *stCRequest)
{
	TRACE(LOG_CRITICAL , CLIENT_RQUEST, "Entering %s  Function ",__FUNCTION__);
	int ret = TRUE ; 	
	QUEUE(&g_clientRequestQ , (YSNODE *)stCRequest);
	sEventStruct* sEvent = NULL;

	sEventStruct* sEvent2 = NULL;

	ERequest  eReqtype = stCRequest->eReqtype;


	sEvent = (sEventStruct *)ALLOCATE(sizeof(sEventStruct));

	//	sEvent = (sEventStruct *)calloc(1, sizeof(sEventStruct));
	if(NULL == sEvent)
	{
		TRACE (LOG_CRITICAL , CLIENT_RQUEST, "ERROR memory not allocated in %s  Function ",__FUNCTION__);
		return FALSE;
	}

	sEvent->EventCode = (char *)ALLOCATE(MAX_CODE_SIZE); 
//		sEvent->EventCode = (char *)calloc(1,MAX_CODE_SIZE); 
	if(NULL == sEvent->EventCode)
	{
		TRACE (LOG_CRITICAL , CLIENT_RQUEST, "ERROR memory not allocated in %s  Function ",__FUNCTION__);
		DEALLOCATE(sEvent);
		return FALSE;
	}
		
	sEvent->CommandKey = (char *)ALLOCATE(MAX_CODE_SIZE);
	//sEvent->CommandKey = (char *)calloc(1,MAX_CODE_SIZE);
	if(NULL == sEvent->CommandKey)
	{
		TRACE (LOG_CRITICAL , CLIENT_RQUEST, "ERROR memory not allocated in %s  Function ",__FUNCTION__);
	  DEALLOCATE(sEvent->EventCode);
		DEALLOCATE(sEvent);
		return FALSE;
	}
	 
	lock_EQueue();
	TRACE(LOG_CRITICAL , CLIENT_RQUEST, "received eReqtype %d",eReqtype);
	if (eReqtype == TRANS_COMPLETE_REQ)
	{
		if (stCRequest->EventCode != NULL)
			strncpy(sEvent->EventCode ,stCRequest->EventCode, MAX_CODE_SIZE);
		else
			ret = FALSE;

		if (stCRequest->pstTxComplete != NULL)
			strncpy(sEvent->CommandKey ,stCRequest->pstTxComplete->CommandKey, MAX_CODE_SIZE); 
		else
			ret = FALSE;

		if (ret)
		{
			//	sEvent2 = (sEventStruct *)calloc(1, sizeof(sEventStruct));
			sEvent2 = (sEventStruct *)ALLOCATE(sizeof(sEventStruct));
			if(NULL == sEvent2)
			{
				TRACE (LOG_CRITICAL , CLIENT_RQUEST, "ERROR memory not allocated in %s  Function ",__FUNCTION__);
				DEALLOCATE(sEvent->CommandKey);
				DEALLOCATE(sEvent->EventCode);
				DEALLOCATE(sEvent);
				return FALSE;
			}
			
			sEvent2->EventCode = (char *)ALLOCATE(MAX_CODE_SIZE);
			sEvent2->CommandKey = (char *)ALLOCATE(MAX_CODE_SIZE);
			/*		
			sEvent2->EventCode = (char *)calloc(1,MAX_CODE_SIZE);
			sEvent2->CommandKey = (char *)calloc(1,MAX_CODE_SIZE);
					*/	
			if ( (sEvent2->CommandKey != NULL) && (sEvent2->EventCode != NULL))  
			{
				strncpy(sEvent2->EventCode , EVENT_CODE_TXN_COM, MAX_CODE_SIZE);
				tr_strncpy(sEvent2->CommandKey, "") ;
				ipr_cwmp_addEvent(sEvent2, NULL);
			}
			else
				ret = FALSE;
		}	
	} 	
	else if (eReqtype == AUTONOMOUS_COMPLETE_REQ)
	{
		if (stCRequest->EventCode != NULL)
			strncpy(sEvent->EventCode ,stCRequest->EventCode, MAX_CODE_SIZE);
		else
		{
			if (sEvent->EventCode && sEvent->CommandKey)
			{
				strncpy(sEvent->EventCode , "10 AUTONOMOUS TRANSFER COMPLETE", MAX_CODE_SIZE);
				tr_strncpy(sEvent->CommandKey, "");
			}
		}
	}
	else if (eReqtype == REQUEST_DOWNLOAD_REQ)
	{
		if (stCRequest->EventCode != NULL)
			strncpy(sEvent->EventCode ,stCRequest->EventCode, MAX_CODE_SIZE);
		else
		{
			if (sEvent->EventCode && sEvent->CommandKey)
			{
				TRACE (LOG_CRITICAL , CLIENT_RQUEST, "DOWNLOAD REQUEST %s  Function ",__FUNCTION__);
				strncpy(sEvent->EventCode , "M Download", MAX_CODE_SIZE);
            /* SPR 19951 FIXED START */
				strncpy(sEvent->CommandKey, stCRequest->pstTxComplete->CommandKey, MAX_CODE_SIZE);
            /* SPR 19951 FIXED END */
			}
		}
	}
	else if (eReqtype == AUTONOMOUS_DU_STATE_CHANGE_COMPLETE_REQ)
	{
		if (stCRequest->EventCode != NULL)
			strncpy(sEvent->EventCode ,stCRequest->EventCode, MAX_CODE_SIZE);
		else
		{
			if (sEvent->EventCode && sEvent->CommandKey)
			{
				TRACE (LOG_CRITICAL , CLIENT_RQUEST, "Received AUTONOMOUS_DU_STATE_CHANGE_COMPLETE REQUEST in %s  Function ",__FUNCTION__);
				strncpy(sEvent->EventCode , "12 AUTONOMOUS DU STATE CHANGE COMPLETE", MAX_CODE_SIZE);
				tr_strncpy(sEvent->CommandKey, "");
			}
		}
	}

	if (ret)
		ipr_cwmp_addEvent(sEvent ,NULL);
	unlock_EQueue();
	TRACE (LOG_CRITICAL , CLIENT_RQUEST, "Exiting %s  Function ",__FUNCTION__);
	return ret ;
}




/********************************************************************************
 *       Function Name : removeClientRequest
 *
 *       Description   : Removing the node i.e removing the client request node
 *       		from the client request queue .
 *
 *
 *       Returns       :
 *
 *********************************************************************************/

/*
   void * removeClientRequest (void)
   {

   TRACE (LOG_CRITICAL , CLIENT_RQUEST, "Entering %s  Function ",__FUNCTION__);

   ClientRequest *fetch_node = NULL;

   if ( clientRequestQ.count > 0)
   fetch_node = (ClientRequest *) DEQUEUE (&clientRequestQ);

   TRACE (LOG_CRITICAL , CLIENT_RQUEST, "Exiting %s  Function ",__FUNCTION__);

   return fetch_node;
   }
   */

