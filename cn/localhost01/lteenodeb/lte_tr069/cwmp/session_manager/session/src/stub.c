#include 	<session_manager.h>
#include 	<session_retry.h>
#include        <logger.h>
#include        <stub.h>
#define UINT4 unsigned int
//static UINT4 retry_count = 0;

/********************************************************************************
*
*       Function Name : stub_print_event_list
*
*       Description   :
*
*       Returns       : void
*
*********************************************************************************/

void stub_print_event_list (sEventList * List)
{
	int i = 0;
	sEventStruct **eventList = NULL;
	eventList = List->__ptrEventStruct;
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
	 ENB_PRINTF ("\n  Size of event List  %d", List->__size);
	for (i = 0; i < List->__size; i++)
	{
		ENB_PRINTF ("\n Event number %d : EventCode = %s, CommandKey = %s\n", i,  eventList[i]->EventCode,  eventList[i]->CommandKey);
	}
}

void    *my_malloc( unsigned int size )
{
        return malloc( size );
}
void *my_calloc(unsigned int nmemb, unsigned int size)
{
        return calloc(nmemb, size);
}
void	my_free( void *m )
{
	if (m != NULL)
	free(m);
}





/********************************************************************************
*
*       Function Name : stub_print_param_list
*
*       Description   :
*
*       Returns       :  VOID
*
*********************************************************************************/

void stub_print_param_list (struct ParameterValueList * List)
{
        int i = 0;
        struct cwmp__ParameterValueStruct **paramList = NULL;

        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
	if (NULL == List)
	{
        	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s Param List is NULL",__FUNCTION__);
		return;
	}
        paramList = List->__ptrParameterValueStruct;
         ENB_PRINTF ("\n  Size of param List  %d", List->__size);
        for (i = 0; i < List->__size; i++)
        {
        /* SPR 13658 START */
                 ENB_PRINTF ("\n List node number %d : Name = %s, Value = %s\n", i, paramList[i]->Name, (char *)(paramList[i]->Value));
        /* SPR 13658 END */
        }
        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting in %s ",__FUNCTION__);
        return;
}





/********************************************************************************
*
*       Function Name : stub_for_sessionInit
*
*       Description   :
*
*       Returns       :  VOID
*
*********************************************************************************/

int stub_for_sessionInit ()
{

        DeviceInfo *devidstruct = NULL;
	char *username = NULL;
	char *password = NULL;
        char *URL = "http://10.203.152.163:8443/ACS_Aug9/ACSServlet";
	int session_status = 0;

        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering %s ",__FUNCTION__);

	devidstruct = (DeviceInfo *) ALLOCATE(sizeof(DeviceInfo) );
	if (NULL == devidstruct)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		return session_status;
	}
	devidstruct->Manufacturer= (char *)ALLOCATE(((strlen("Aricent") + 1) *  sizeof(char)) );
	if (NULL == devidstruct->Manufacturer)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		DEALLOCATE( devidstruct);        
		return session_status;
	}
	devidstruct->OUI =(char *) ALLOCATE ( ((strlen("ABCDEF") + 1)* sizeof(char)) );
	if (NULL == devidstruct->OUI )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		DEALLOCATE( devidstruct->Manufacturer);        
		DEALLOCATE( devidstruct);        
		return session_status;
	}
	devidstruct->ProductClass = (char *) ALLOCATE ((( strlen("ENODEB") + 1 ) * sizeof(char))  );
	if (NULL == devidstruct->ProductClass)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		DEALLOCATE( devidstruct->Manufacturer);        
		DEALLOCATE( devidstruct->OUI);        
		DEALLOCATE( devidstruct);        
		return session_status;
	}
	devidstruct->SerialNumber = (char *) ALLOCATE (((strlen("FFFFFF") + 1 ) * sizeof(char)) );
	if (NULL == devidstruct->SerialNumber)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		DEALLOCATE( devidstruct->ProductClass);        
		DEALLOCATE( devidstruct->Manufacturer);        
		DEALLOCATE( devidstruct->OUI);        
		DEALLOCATE( devidstruct);        
		return session_status;
	}

        tr_strncpy(devidstruct->Manufacturer ,"Aricent");
        tr_strncpy(devidstruct->OUI ,"ABCDEF");
        tr_strncpy(devidstruct->ProductClass ,"ENODEB");
        tr_strncpy(devidstruct->SerialNumber ,"FFFFFF");

        //paramList = stub_get_param_list();

	username = (char *) ALLOCATE (10 );
	if (NULL == username)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		DEALLOCATE( devidstruct->SerialNumber);        
		DEALLOCATE( devidstruct->ProductClass);        
		DEALLOCATE( devidstruct->Manufacturer);        
		DEALLOCATE( devidstruct->OUI);        
		DEALLOCATE( devidstruct);        
		return session_status;
	}
	password = (char *) ALLOCATE (10 );
	if (NULL == password)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with ALLOCATE FAILED",__FUNCTION__);
		DEALLOCATE( username);        
		DEALLOCATE( devidstruct->SerialNumber);        
		DEALLOCATE( devidstruct->ProductClass);        
		DEALLOCATE( devidstruct->Manufacturer);        
		DEALLOCATE( devidstruct->OUI);        
		DEALLOCATE( devidstruct);        
		return session_status;
	}
	tr_strncpy(username ,"abc");
	tr_strncpy(password ,"abc");

	session_status = ipr_cwmp_SessionInit(devidstruct, URL, (struct ParameterValueList *)SOAP_TYPE__cwmp__Inform, username, password);
	TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s ",__FUNCTION__);
	DEALLOCATE(username);                
	DEALLOCATE(password);                
	return session_status;
}




/********************************************************************************
*
*       Function Name : stub_createInformReq
*
*       Description   :
*
*       Returns       :  VOID
*
*********************************************************************************/

sInform * stub_createInformReq (DeviceInfo *devidstruct,            // sDeviceInfo will contain info of device
                           struct EventList *eventList)         // List of event for which Infor will be created
{
        sInform *informReq = NULL;
        struct ParameterValueList *tmp_list = NULL;

        TRACE (LOG_CRITICAL , SESSION_MANAGER, "Entering in %s ",__FUNCTION__);
        informReq = (sInform *) my_malloc (sizeof (sInform));
        if (NULL == informReq)
        {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
                return NULL;
        }

        informReq->DeviceId = devidstruct;
        informReq->RetryCount = ipr_cwmp_get_retry_count();
        informReq->MaxEnvelopes = 1;
        time(&(informReq->CurrentTime));

        tmp_list = (struct ParameterValueList *) calloc (1, sizeof(struct ParameterValueList));
        if (NULL == tmp_list)
        {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
//coverity fix
ENB_FREE(informReq);                
return NULL;
        }

        if (NULL == eventList)
        {
                informReq->Event = stub_get_event_list();
                informReq->ParameterList = stub_get_param_list();
        }
//coverity fix
ENB_FREE(tmp_list);
	return informReq;
}





/********************************************************************************
*
*       Function Name : stub_get_event_list
*
*       Description   :
*
*       Returns       :  VOID
*
*********************************************************************************/

sEventList *stub_get_event_list ()
{
	sEventList *ret_eventList = NULL;
	sEventStruct *e1;
	sEventStruct *e2;
	sEventStruct *e3;
	sEventStruct *e4;

	e1 = ( sEventStruct *) malloc (sizeof(sEventStruct));
	if (NULL == e1)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		return NULL;
	}
	e1->EventCode = calloc (1,8);
	if (NULL == e1->EventCode)
	{

		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e1);
		return NULL;
	}
	tr_strncpy (e1->EventCode, "Event_1");
	e1->CommandKey = calloc (1,8);
	if (NULL == e1->CommandKey)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e1->EventCode);
		free(e1);
		return NULL;
	}
	tr_strncpy (e1->CommandKey, "M_CMD_1");
	e2 = ( sEventStruct *) malloc (sizeof(sEventStruct));
	if (NULL == e2)
	{
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		return NULL;
	}

	e2->EventCode = calloc (1,8);
	if (NULL == e2->EventCode)
	{
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		return NULL;
	}
	tr_strncpy (e2->EventCode, "Event_2");
	e2->CommandKey = calloc (1,8);
	if (NULL == e2->CommandKey)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		return NULL;
	}
	tr_strncpy (e2->CommandKey, "M_CMD_2");

	e3 = ( sEventStruct *) malloc (sizeof(sEventStruct));
	if (NULL == e3)
	{
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		return NULL;
	}
	e3->EventCode = calloc (1,8);
	if (NULL == e3->EventCode )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		return NULL;
	}
	tr_strncpy (e3->EventCode, "Event_3");
	e3->CommandKey = calloc (1,8);
	if (NULL == e3->CommandKey)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e3->EventCode );
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		return NULL;
	}
	tr_strncpy (e3->CommandKey, "M_CMD_3");

	e4 = ( sEventStruct *) malloc (sizeof(sEventStruct));
	if (NULL == e4)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		free(e3->CommandKey );
		free(e3->EventCode );
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		return NULL;
	}
	e4->EventCode = calloc (1,8);
	if (NULL == e4->EventCode)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e4);
		free(e3->CommandKey );
		free(e3->EventCode );
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		return NULL;
	}
	tr_strncpy (e4->EventCode, "Event_4");
	e4->CommandKey = calloc (1,8);
	if (NULL == e4->CommandKey)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e4->EventCode);
		free(e4);
		free(e3->CommandKey );
		free(e3->EventCode );
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->CommandKey);
		free(e1->EventCode);
		free(e1);
		return NULL;
	}
	tr_strncpy (e4->CommandKey, "M_CMD_4");

	ret_eventList = (sEventList *) my_malloc (sizeof(sEventList));
	if (NULL == ret_eventList)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		free(e4->CommandKey);
		free(e4->EventCode);
		free(e4);
		free(e3->CommandKey );
		free(e3->EventCode );
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->EventCode);
		free(e1->CommandKey);
		free(e1);
		return NULL;
	}
	ret_eventList->__ptrEventStruct = (EventStruct **)my_calloc (4, sizeof(EventStruct *));
	if (NULL == ret_eventList->__ptrEventStruct)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s calloc failed",__FUNCTION__);
		//coverity fix
		free(ret_eventList);
		free(e4->CommandKey);
		free(e4->EventCode);
		free(e4);
		free(e3->CommandKey );
		free(e3->EventCode );
		free(e3);
		free(e2->CommandKey);
		free(e2->EventCode);
		free(e2);
		free(e1->CommandKey);
		free(e1->EventCode);
		free(e1);
		return NULL;
	}
	ret_eventList->__ptrEventStruct[0] = e1; 
	ret_eventList->__ptrEventStruct[1] = e2;
	ret_eventList->__ptrEventStruct[2] = e3;
	ret_eventList->__ptrEventStruct[3] = e4;
	ret_eventList->__size = 4;
	stub_print_event_list (ret_eventList);

	return ret_eventList;
}





/********************************************************************************
*
*       Function Name : stub_get_param_list
*
*       Description   :
*
*       Returns       :  VOID
*
*********************************************************************************/

struct ParameterValueList *stub_get_param_list()
{
	struct ParameterValueList *ret_paramList;
	struct cwmp__ParameterValueStruct *p1;
	struct cwmp__ParameterValueStruct *p2;
	struct cwmp__ParameterValueStruct *p3;
	struct cwmp__ParameterValueStruct *p4;

	p1 = ( struct cwmp__ParameterValueStruct *) malloc (sizeof(struct cwmp__ParameterValueStruct));
	if (NULL == p1)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		return NULL;
	}
	p1->Name = calloc (1,8);
	if (NULL == p1->Name)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p1);                
		return NULL;
	}
	tr_strncpy (p1->Name, "param_1");
	p1->Value = calloc (1,8);
	if (NULL == p1->Value)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return NULL;
	}
	tr_strncpy (p1->Value, "VALUE_1");

	p2 = ( struct cwmp__ParameterValueStruct *) malloc (sizeof(struct cwmp__ParameterValueStruct));
	if (NULL == p2)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return NULL;
	}
	p2->Name = calloc (1,8);
	if (NULL == p2->Name )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return NULL;
	}
	tr_strncpy (p2->Name, "param_2");
	p2->Value = calloc (1,8);
	if (NULL == p2->Value )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return NULL;
	}
	tr_strncpy (p2->Value, "VALUE_2");

	p3 = ( struct cwmp__ParameterValueStruct *) malloc (sizeof(struct cwmp__ParameterValueStruct));
	if (NULL == p3)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   	
		return NULL;
	}
	p3->Name = calloc (1,8);
	if (NULL == p3->Name)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return NULL;
	}
	tr_strncpy (p3->Name, "param_3");
	p3->Value = calloc (1,8);
	if (NULL == p3->Value )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return NULL;
	}
	tr_strncpy (p3->Value, "VALUE_3");

	p4 = ( struct cwmp__ParameterValueStruct *) malloc (sizeof(struct cwmp__ParameterValueStruct));
	if (NULL == p4)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return NULL;
	}
	p4->Name = calloc (1,8);
	if (NULL == p4->Name )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return NULL;
	}
	tr_strncpy (p4->Name, "param_4");
	p4->Value = calloc (1,8);
	if (NULL == p4->Value)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p4->Name);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return NULL;
	}
	tr_strncpy (p4->Value, "VALUE_4");

	ret_paramList = (struct ParameterValueList *) my_malloc (sizeof(struct ParameterValueList));
	if (NULL == ret_paramList)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(p4->Value);                
		ENB_FREE(p4->Name);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return NULL;
	}
	ret_paramList->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct **)my_calloc (
			4, sizeof(struct cwmp__ParameterValueStruct *));
	if (NULL == ret_paramList->__ptrParameterValueStruct)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s calloc failed",__FUNCTION__);
		ENB_FREE(p4->Value);                
		ENB_FREE(p4->Name);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   	
		ENB_FREE(ret_paramList);        
		return NULL;
	}

	ret_paramList->__ptrParameterValueStruct[0] = p1; 
	ret_paramList->__ptrParameterValueStruct[1] = p2;
	ret_paramList->__ptrParameterValueStruct[2] = p3;
	ret_paramList->__ptrParameterValueStruct[3] = p4;
	ret_paramList->__size = 4;
	stub_print_param_list ( ret_paramList );
	return ret_paramList;
}
/********************************************************************************
*
*       Function Name : stub_send_http_msg
*
*       Description   :
*
*       Returns       :  VOID
*
*********************************************************************************/





/********************************************************************************
*
*       Function Name : main
*
*       Description   : 
*
*       Returns       :  VOID
*
*********************************************************************************/

int stub_main(void)
{
        int session_status = 0;

        logger_trace ( 1 , "TR-069" , "Entring : %s", __FUNCTION__);
        logger_checknAssign ();
        logger_init ();
	logger_setLogLevel(1);


	session_status = stub_for_sessionInit();
	ENB_PRINTF ("\n status = %d\n\n",session_status);
        logger_trace ( 1 , "TR-069" , "Exiting : %s", __FUNCTION__);
    return 0;
}

struct _cwmp__Fault * stub_fill_fault_code_struct(int code_fault)
{
	struct _cwmp__Fault *stFault = NULL;
	stFault = (struct _cwmp__Fault *) calloc (1, sizeof(struct _cwmp__Fault ));
	if (NULL == stFault)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		return NULL;
	}
	stFault->FaultCode = calloc(1, 8);
	if (NULL == stFault->FaultCode)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(stFault);                
		return NULL;
	}
	stFault->FaultString = calloc(1, 1000);
	if (NULL == stFault->FaultString)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(stFault->FaultCode);                
		ENB_FREE(stFault);                
		return NULL;
	}
	if (code_fault == 9001 )
	{
		tr_strncpy(stFault->FaultCode, "9001");
		tr_strncpy(stFault->FaultString, "Request denied (no reason specified)");
	}
	else if (code_fault == 9002 )
	{
		tr_strncpy(stFault->FaultCode, "9002");
		tr_strncpy(stFault->FaultString, "Internal error");
	}
	else if (code_fault == 9003 )
	{
		tr_strncpy(stFault->FaultCode, "9003");
		tr_strncpy(stFault->FaultString, "Invalid arguments");
	}
	else if (code_fault == 9004 )
	{
		tr_strncpy(stFault->FaultCode, "9004");
		tr_strncpy(stFault->FaultString, "Resources exceeded");
	}
	else if (code_fault == 9005 )
	{
		tr_strncpy(stFault->FaultCode, "9005");
		tr_strncpy(stFault->FaultString, "Invalid Parameter name");
	}
	else
	{
		//coverity fix
		ENB_FREE(stFault->FaultString);                
		ENB_FREE(stFault->FaultCode);                
		ENB_FREE(stFault);                
		return NULL;
	}
	return stFault;
}



void stub_print_acs_rpc_list(struct MethodList *List)
{
        if( NULL == List ||  List->__ptrstring == NULL )
        {
                TRACE (LOG_CRITICAL , SESSION_MANAGER, "ACS RPC LIST is NULL");
                return ;
        }
        int idx = 0;
        for ( idx = 0; idx < List->__size; idx++ )
        {

                ENB_PRINTF ("\n List [%d] = %s\n",idx, List->__ptrstring[idx]);
        }
}


void stub_create_AutoTransComp(struct _cwmp__AutonomousTransferComplete  *autoTranComeReq)
{
	autoTranComeReq->AnnounceURL = calloc (1, 257);
	if (NULL == autoTranComeReq->AnnounceURL)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		return ;
	}
	tr_strncpy(autoTranComeReq->AnnounceURL, "http://10.203.152.150:9898/ACSServlet");

	autoTranComeReq->TransferURL = calloc (1, 257);
	if (NULL == autoTranComeReq->TransferURL)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(autoTranComeReq->AnnounceURL);                
		return ;
	}
	tr_strncpy(autoTranComeReq->TransferURL, "http://10.203.152.150:9090/ACSServlet");

	autoTranComeReq->IsDownload = xsd__boolean__true_;

	autoTranComeReq->FileType = calloc(1, 10);
	if (NULL == autoTranComeReq->FileType )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(autoTranComeReq->TransferURL);                
		ENB_FREE(autoTranComeReq->AnnounceURL);                
		return ;
	}
	tr_strncpy(autoTranComeReq->FileType, "textfile");

	autoTranComeReq->TargetFileName = calloc(1, 20);
	if (NULL == autoTranComeReq->TargetFileName)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(autoTranComeReq->FileType );                
		ENB_FREE(autoTranComeReq->TransferURL);                
		ENB_FREE(autoTranComeReq->AnnounceURL);                
		return ;
	}
	tr_strncpy(autoTranComeReq->TargetFileName, "TargetFileName");

	autoTranComeReq->FileSize = 100;

	autoTranComeReq->FaultStruct = calloc(1,
			sizeof(struct cwmp__TransferCompleteFaultStruct));
	if (NULL == autoTranComeReq->FaultStruct)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(autoTranComeReq->FileType );                
		ENB_FREE(autoTranComeReq->TargetFileName );                
		ENB_FREE(autoTranComeReq->TransferURL);                
		ENB_FREE(autoTranComeReq->AnnounceURL);                
		return ;
	}
	autoTranComeReq->FaultStruct->FaultCode = calloc(1, 8);
	if (NULL == autoTranComeReq->FaultStruct->FaultCode)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(autoTranComeReq->FaultStruct );                
		ENB_FREE(autoTranComeReq->FileType );                
		ENB_FREE(autoTranComeReq->TargetFileName );                
		ENB_FREE(autoTranComeReq->TransferURL);                
		ENB_FREE(autoTranComeReq->AnnounceURL);                
		return ;
	}
	tr_strncpy(autoTranComeReq->FaultStruct->FaultCode, "9001");

	autoTranComeReq->FaultStruct->FaultString = calloc(1, 1000);
	if (NULL == autoTranComeReq->FaultStruct->FaultString)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with calloc FAILED",__FUNCTION__);
		ENB_FREE(autoTranComeReq->FaultStruct->FaultCode );                
		ENB_FREE(autoTranComeReq->FaultStruct );                
		ENB_FREE(autoTranComeReq->FileType );                
		ENB_FREE(autoTranComeReq->TargetFileName );                
		ENB_FREE(autoTranComeReq->TransferURL);                
		ENB_FREE(autoTranComeReq->AnnounceURL);                
		return ;
	}
	tr_strncpy(autoTranComeReq->FaultStruct->FaultString, "Request denied");

	time(&(autoTranComeReq->StartTime));
	sleep(2);
	time(&(autoTranComeReq->CompleteTime));
}

void stub_create_RequestDownload (struct _cwmp__RequestDownload *reqdwReq)
{
	reqdwReq->FileType = calloc(1, 20);
	if (NULL == reqdwReq->FileType)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		return ;
	}

	tr_strncpy(reqdwReq->FileType, "textfile");

	struct FileTypeArg *tmp_FileTypeArg;
	struct cwmp__ArgStruct *p1;
	struct cwmp__ArgStruct *p2;
	struct cwmp__ArgStruct *p3;
	struct cwmp__ArgStruct *p4;

	p1 = ( struct cwmp__ArgStruct *) malloc (sizeof(struct cwmp__ArgStruct));
	if (NULL == p1)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		return ;
	}
	p1->Name = calloc (1,8);
	if (NULL == p1->Name)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p1);                
		return ;
	}
	tr_strncpy (p1->Name, "param_1");
	p1->Value = calloc (1,8);
	if (NULL == p1->Value)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return ;
	}
	tr_strncpy (p1->Value, "VALUE_1");

	p2 = ( struct cwmp__ArgStruct *) malloc (sizeof(struct cwmp__ArgStruct));
	if (NULL == p2)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return ;
	}
	p2->Name = calloc (1,8);
	if (NULL == p2->Name )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return ;
	}
	tr_strncpy (p2->Name, "param_2");
	p2->Value = calloc (1,8);
	if (NULL == p2->Value )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);                
		return ;
	}
	tr_strncpy (p2->Value, "VALUE_2");

	p3 = ( struct cwmp__ArgStruct *) malloc (sizeof(struct cwmp__ArgStruct));
	if (NULL == p3)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   	
		return ;
	}
	p3->Name = calloc (1,8);
	if (NULL == p3->Name)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return ;
	}
	tr_strncpy (p3->Name, "param_3");
	p3->Value = calloc (1,8);
	if (NULL == p3->Value )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return ;
	}
	tr_strncpy (p3->Value, "VALUE_3");

	p4 = ( struct cwmp__ArgStruct *) malloc (sizeof(struct cwmp__ArgStruct));
	if (NULL == p4)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return ;
	}
	p4->Name = calloc (1,8);
	if (NULL == p4->Name )
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return ;
	}
	tr_strncpy (p4->Name, "param_4");
	p4->Value = calloc (1,8);
	if (NULL == p4->Value)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s with malloc FAILED",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p4->Name);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   		
		return ;
	}
	tr_strncpy (p4->Value, "VALUE_4");   

	tmp_FileTypeArg = (struct FileTypeArg *) my_malloc (sizeof(struct FileTypeArg));
	if (NULL == tmp_FileTypeArg)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s calloc failed",__FUNCTION__);
		ENB_FREE(reqdwReq->FileType);                
		ENB_FREE(p4->Value);                
		ENB_FREE(p4->Name);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   	
		return ;
	}

	tmp_FileTypeArg->__ptrArgStruct = (struct cwmp__ArgStruct **)my_calloc (
			4, sizeof(struct cwmp__ArgStruct *));
	if (NULL == tmp_FileTypeArg->__ptrArgStruct)
	{
		TRACE (LOG_CRITICAL , SESSION_MANAGER, "Exiting %s calloc failed",__FUNCTION__);
		ENB_FREE(p4->Value);                
		ENB_FREE(p4->Name);                
		ENB_FREE(p4);                
		ENB_FREE(p3->Value);                
		ENB_FREE(p3->Name);                
		ENB_FREE(p3);                
		ENB_FREE(p2->Value);                
		ENB_FREE(p2->Name);                
		ENB_FREE(p2);                
		ENB_FREE(p1->Value);                
		ENB_FREE(p1->Name);                
		ENB_FREE(p1);   	
		ENB_FREE(tmp_FileTypeArg);        
		return ;
	}

	tmp_FileTypeArg->__ptrArgStruct[0] = p1;
	tmp_FileTypeArg->__ptrArgStruct[1] = p2;
	tmp_FileTypeArg->__ptrArgStruct[2] = p3;
	tmp_FileTypeArg->__ptrArgStruct[3] = p4;
	tmp_FileTypeArg->__size = 4;

	reqdwReq->FileTypeArg = tmp_FileTypeArg;
}

