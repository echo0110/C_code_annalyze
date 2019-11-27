
/********************************************************************************
*
*       FILE NAME   : 	event_handler.c
*
*       DESCRIPTION : 	This file contains the functionality of adding a
*				 new Event into a global queue maintained.										
*
*       REVISION HISTORY:
*
*       NAME                    DATE                  REFERENCE                      REASON
*_______________________________________________________________________________
*
*       Jatin Bajaj    Aug 16, 2012   TR-069 Amendment 4   Initial Creation
*
*
*       All Rights Reserved. Copyright @ Aricent Group, 2012.
*
**************************************************************************************/

#include    <cspl.h>
#include    <shell.h>
#include	<event_handler.h>
#include 	<pthread.h>
#include 	<common_def.h>
#include 	<logger.h>
#include 	<ManagementServerModel.h>
#include    <unistd.h>
#include    <signal.h>
 
/******************************************************************************
   Global Variables
******************************************************************************/
//Shared variable used for mutual exclusion 
pthread_mutex_t g_event_hnd_mutex;

// Global Queue maintained for storing events.
YSQUEUE g_event_hnd_eventQueue;

// Temporary Global Queue
YSQUEUE g_event_hnd_rec_Queue;

// For Thread of session initialization. 
pthread_t g_event_hnd_thSession;

/*SPR 16672 Fixed Start */
U8 g_inform_set_to_value = 0;

/*SPR 16672 Fixed End */

/* Periodic Inform Changes Start */
U8 g_periodic_inform_set_to_value = 0;
/* Periodic Inform Changes End */

extern int ipr_cwmp_SessionInit(sDeviceInfo * , char * , int , struct ParameterValueList * , char * , char *);

/* SPR 22314 Start */
extern void tr069_free_parameterValueList(struct ParameterValueList *ptr);
/* SPR 22314 End */

/******************************************************************************
  Function Definitions
******************************************************************************/

/*********************************************************************************
* FUNCTION:		        event_hnd_priorEvent 
*
* DESCRIPTION:          The function is the priority function used for event queue 
*
* ARGS:                 YSNODE *  (pointer to event node) 
*
* RETURNS:              returns default value i.e PRIORITY_NUMBER 
**********************************************************************************/

int event_hnd_priorEvent (void)
{
    return PRIORITY_NUMBER; 
}


typedef struct stSessionParam{
                char *URL;
                sDeviceInfo *devidstruct;
                int eventType;
                struct ParameterValueList *paramList;
                int status;
} STsessionParam;


/*********************************************************************************
 * FUNCTION:            	event_hnd_managment_server_info 
 *
 * DESCRIPTION:          The function is used to fetch sessiobn paramaters from  
 Management module Server . 
 * ARGS:
 *                      	STsessionParam *sSessionParam  (Pointer to Session parameter)
 * RETURNS:
 *                      	NONE. 
 *
 **********************************************************************************/

void ipr_cwmp_managment_server_info (STsessionParam *sSessionParam)
{
	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Entering %s ",__FUNCTION__);

	sDeviceInfo *devidstruct = NULL;
	devidstruct = (sDeviceInfo *)ALLOCATE(sizeof(sDeviceInfo));
	if(NULL == devidstruct)
	{
		TRACE(LOG_CRITICAL , EVENT_HANDLER, " Error in Mem allocation in %s ",__FUNCTION__);
		exit(0);
	}
	memset(devidstruct ,'\0' ,sizeof(sDeviceInfo));

	char * Manufacturer =    (char *)ipr_cwmp_getHost();
	char * ManufacturerOUI = (char *)ipr_cwmp_getManufacturerOUI();
	char * ProductClass =    (char *)ipr_cwmp_getProductClass();	
	char *SerialNumber =     (char *)ipr_cwmp_getSerialNumber();

	if (Manufacturer!=NULL)

	{	devidstruct->Manufacturer= (char *)ALLOCATE((strlen(Manufacturer) + 1)*sizeof(char));
		if(NULL == devidstruct->Manufacturer)
		{
			TRACE(LOG_CRITICAL , EVENT_HANDLER, " Error in Mem allocation in %s ",__FUNCTION__);
			exit(0);
		}
	}
	if (ManufacturerOUI!=NULL)
	{
		devidstruct->OUI =(char *) ALLOCATE((strlen(ManufacturerOUI) + 1)*sizeof(char));
		if(NULL == devidstruct->OUI)
		{
			TRACE(LOG_CRITICAL , EVENT_HANDLER, " Error in Mem allocation in %s ",__FUNCTION__);
			exit(0);
		}
	}
	if (ProductClass!=NULL)
	{
		devidstruct->ProductClass = (char *)ALLOCATE((strlen(ProductClass) + 1)*sizeof(char));
		if(NULL == devidstruct->ProductClass)
		{
			TRACE(LOG_CRITICAL , EVENT_HANDLER, " Error in Mem allocation in %s ",__FUNCTION__);
			exit(0);
		}
	}
	if (SerialNumber!=NULL)
	{
		devidstruct->SerialNumber = (char *)ALLOCATE((strlen(SerialNumber) + 1)*sizeof(char)); 
		if(NULL == devidstruct->SerialNumber)
		{
			TRACE(LOG_CRITICAL , EVENT_HANDLER, " Error in Mem allocation in %s ",__FUNCTION__);
			exit(0);
		}
	}

	if (Manufacturer!=NULL)
	{
		tr_strncpy(devidstruct->Manufacturer ,Manufacturer);
	}
	if (ManufacturerOUI!=NULL)
	{
		tr_strncpy(devidstruct->OUI ,ManufacturerOUI);
	}
	if (ProductClass!=NULL)
	{
		tr_strncpy(devidstruct->ProductClass ,ProductClass);
	}
	if (SerialNumber!=NULL) 
	{
		tr_strncpy(devidstruct->SerialNumber ,SerialNumber);
	}
	sSessionParam->devidstruct = devidstruct;
	sSessionParam->URL = (char *)ipr_cwmp_getACSURL();
	sSessionParam->paramList = NULL;

	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Exiting %s ",__FUNCTION__);
}

/*****************************************************************************
* FUNCTION:		        event_manager_init 
*
* DESCRIPTION:
*                       The function is used to initialize the g_event_hnd_eventQueue. 
*
* ARGS:
*                       None
*
* RETURNS:
*                       None.
*
******************************************************************************/

void event_manager_init(void)
{
	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Entering %s ",__FUNCTION__);

	pthread_mutex_init(&g_event_hnd_mutex, NULL);
	//Initialize the Queue 
 	ysInit(&g_event_hnd_eventQueue, (int (*)(const YSNODE *))event_hnd_priorEvent);	
	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Exiting %s ",__FUNCTION__);	
}

/*****************************************************************************
* FUNCTION:		        startSession 
*
* DESCRIPTION:          The function is the thread  function, it calls to initialize 
*                       session based on the argument provided. 
*
* ARGS:                 void * arg 
*
* RETURNS:
*                       void *   ( Returns NULL by default ).  
******************************************************************************/

void *ipr_cwmp_startSession(void * arg)
{
    TRACE(LOG_CRITICAL , EVENT_HANDLER, "Entering %s ",__FUNCTION__);

    //int ret = 0 ;
    STsessionParam *sSessionParam = (STsessionParam *)arg;

	while (!ysEmpty(&g_event_hnd_eventQueue))
	{
	    ipr_cwmp_managment_server_info (sSessionParam);
	    sSessionParam->status = ipr_cwmp_SessionInit(sSessionParam->devidstruct,  
                                                            sSessionParam->URL, 
                                                            sSessionParam->eventType, 
                                                            NULL, (char *)ipr_cwmp_getUsername()
                                                            , (char *)ipr_cwmp_getPassword());
	}
	
    if (sSessionParam->devidstruct)
    {
        DEALLOCATE(sSessionParam->devidstruct->Manufacturer);
        DEALLOCATE(sSessionParam->devidstruct->OUI);
        DEALLOCATE(sSessionParam->devidstruct->ProductClass);
        DEALLOCATE(sSessionParam->devidstruct->SerialNumber);
        DEALLOCATE(sSessionParam->devidstruct);
    }

	DEALLOCATE(sSessionParam);
    /*  coverity 86594 fix start */
	/* code removed */
    /*  coverity 86594 fix end */

	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Exiting %s ",__FUNCTION__);
	return NULL; 
}

void lock_EQueue(void){
 pthread_mutex_lock(&g_event_hnd_mutex);
}

void unlock_EQueue(void){
 pthread_mutex_unlock(&g_event_hnd_mutex);
}

/*****************************************************************************
* FUNCTION:		        ipr_cwmp_addEvent
*
* DESCRIPTION:          The function add new event into a g_event_hnd_eventQueue and starts
*                       a new thread for sessioninit. 
*
* ARGS:                	EventStruct *event   ( Event Structre ) 
*                      	sParameterValueList * spstParam  ( Parameter List or NULL
*                      	by default).	
*
* RETURNS:
*                       1   : success.
*                       0   : Failure.	
*
******************************************************************************/

int ipr_cwmp_addEvent (EventStruct *event , ParameterValueList * spstParam)
{
	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Entering %s ",__FUNCTION__);

	sEventNode* newNode = NULL;
	STsessionParam *sSessionParam = NULL;
	//int success = 0 ;
	static int event_add_atatus = 1;
	//void *status = NULL;
	int ret = 1 ;

	if (NULL == event)
	{
		TRACE(LOG_CRITICAL , EVENT_HANDLER, "New Event is not added in queue because it was NULL");
		return 0;
	}

	//	newNode = ALLOCATE( sizeof(sEventNode));
	newNode = (sEventNode *)ALLOCATE(sizeof(sEventNode));
	if (newNode == NULL)
		return 0;

	newNode->sEvent = event;
	newNode->sParamValList = spstParam;

	if (event_add_atatus != 1)
	{
		TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : , Sending Signal SIGUSR2 ", __FUNCTION__);
        /*SPR 18172 Fixed Start */ 
	//	pthread_kill(g_event_hnd_thSession , SIGUSR2);
        /*SPR 18172 Fixed End */ 
		ret = pthread_kill(g_event_hnd_thSession , 0);
		event_add_atatus = 0 ;
	}


	if (ret!=0)
	{
		event_add_atatus = 0;

		/* 
		 * Management Server Module retreive the parameters
		 */
		sSessionParam = ALLOCATE(sizeof(STsessionParam));

		if (sSessionParam == NULL)
		{
			/*SPR 16747 Fixed Start*/
            /* Periodic Inform Changes Start */
            DEALLOCATE(newNode->sEvent);
            newNode->sEvent = NULL;
            /* Periodic Inform Changes End */
            /* SPR 21563_p1 FIXED START */
            DEALLOCATE(newNode->sParamValList);
            /* SPR 21563_p1 FIXED END */
			DEALLOCATE(newNode);
			/*SPR 16747 Fixed End*/
			return 0;
		}
		sSessionParam->status = FALSE;

		/*SPR 16672 Fixed Start */
		if ((g_inform_set_to_value == 1) && (!strcmp((char *)event->EventCode, "4 VALUE CHANGE")))
		{

            /* Periodic Inform Changes Start */
            TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s :Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode",
            __FUNCTION__);

              /* SPR 19441 FIXED START*/            
                /* Code remove */
              /* SPR 19441 FIXED END*/            
            /*SPR 16747 Fixed Start*/
            DEALLOCATE(newNode->sEvent);
            /*SPR 16747 Fixed End*/


            TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode", __FUNCTION__);
        }
        else if ((g_periodic_inform_set_to_value == 1) && ((!strcmp((char *)event->EventCode, "2 PERIODIC"))))
        {    
            TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s :Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode %s",
                    __FUNCTION__,event->EventCode);

            DEALLOCATE(newNode->sEvent);
            /* SPR 21563_p1 FIXED START */
            DEALLOCATE(newNode->sParamValList);
            /* SPR 21563_p1 FIXED END */
            DEALLOCATE(newNode);

            TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode", __FUNCTION__);
            /* SPR 19441 FIXED START*/            
            /* Code remove */

            /* SPR 19441 FIXED END*/            
            /*  coverity 94843 fix start */
            DEALLOCATE(sSessionParam);
            /*  coverity 94843 fix end */
            return ret;
        }
        /* Periodic Inform Changes End */

		TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s :Pushed new Node into queue g_event_hnd_eventQueue with evntcode", __FUNCTION__);

		PUSH(&g_event_hnd_eventQueue, (YSNODE *)newNode);


		/*SPR 16672 Fixed End */


		int rc = 0;

        /* Periodic Inform Changes Start */
		//TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : ,Pushed new Node into queue g_event_hnd_eventQueue with evntcode %s ",__FUNCTION__,newNode->sEvent->EventCode);
        /* Periodic Inform Changes End */

		TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : , Starting new Thread for Session ",__FUNCTION__);

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, (PTHREAD_STACK_SIZE*2));
		rc = pthread_create(&g_event_hnd_thSession, &attr, ipr_cwmp_startSession, (void *)sSessionParam);
		pthread_attr_destroy(&attr);
		if (rc)
		{
			return 0;
		}
		pthread_detach(g_event_hnd_thSession);
	}
	else
	{

		/*SPR 16672 Fixed Start */
		if ((g_inform_set_to_value == 1) && (!strcmp((char *)event->EventCode, "4 VALUE CHANGE")))
		{

            /* SPR 19441 FIXED START*/            
            /* Code remove */

            /* SPR 19441 FIXED END*/            

            /* Periodic Inform Changes Start */

			TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode", __FUNCTION__);
		}
        else if ((g_periodic_inform_set_to_value == 1) && ((!strcmp((char *)event->EventCode, "2 PERIODIC"))))
        {    
            TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s :Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode %s",
                    __FUNCTION__,event->EventCode);

            DEALLOCATE(newNode->sEvent);
            newNode->sEvent = NULL;
            /* SPR 21563_p1 FIXED START */
            DEALLOCATE(newNode->sParamValList);
            /* SPR 21563_p1 FIXED END */
            DEALLOCATE(newNode);

            TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Not Pushed new Node into queue g_event_hnd_eventQueue with evntcode", __FUNCTION__);
            return ret;
        }
        /* Periodic Inform Changes End */

		TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Only Pushing into Event Queue ", __FUNCTION__);
		PUSH(&g_event_hnd_eventQueue, (YSNODE *)newNode);
        /* Periodic Inform Changes Start */
		//TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : ,Pushed new Node into queue g_event_hnd_eventQueue with evntcode %s ",__FUNCTION__,newNode->sEvent->EventCode);
        /* Periodic Inform Changes End */

		/*SPR 16672 Fixed Start */
		TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : JOSHI123 %s", __FUNCTION__ ,event->EventCode);
        /*SPR 18317 FIXED START */
        /* crash 21 jan fixed */
        if(event->EventCode)
        {
            /* crash 21 jan fixed */
            if((strcmp((char *)event->EventCode, "4 VALUE CHANGE") == 0) && (newNode->sEvent != NULL))
                /*SPR 18317 FIXED END */
            {
                g_inform_set_to_value = 1;

                TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : JOSHI123 %d", __FUNCTION__, g_inform_set_to_value);

                TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Set the variable g_inform_set_to_value ", __FUNCTION__);
            }
            /*SPR 16672 Fixed End */

            /* Periodic Inform Changes Start */
            /*SPR 18317 FIXED START */
            if((strcmp((char *)event->EventCode, "2 PERIODIC") == 0) && (newNode->sEvent != NULL))
                /*SPR 18317 FIXED END */

            {
                g_periodic_inform_set_to_value = 1;

                TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : JOSHI123 %d", __FUNCTION__, g_periodic_inform_set_to_value);

                TRACE(LOG_CRITICAL , EVENT_HANDLER, "In %s : Set the variable g_periodic_inform_set_to_value ", __FUNCTION__);
            }
            /* Periodic Inform Changes End */

            /* Periodic Inform Changes End */
            /* crash 21 jan fixed */
        }
        else
        {
          return 0;
        }
        /* crash 21 jan fixed */

	}

	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Exiting %s ",__FUNCTION__);
	return ret;
}


/*****************************************************************************
* FUNCTION:             getAll 
* DESCRIPTION:          The functio Pop all elements from event and fill in   
*                       rec_queue.
* ARGS:                 YSQUEUE * - (Pointer to Queue).
* RETURNS:              Identifier for the driver.
******************************************************************************/

YSQUEUE * getAll (YSQUEUE *ieventQ)
{
	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Entering %s ",__FUNCTION__);

	int icount = 0 ; 
	int index = 1;
	sEventNode *fetch_node= NULL;

	pthread_mutex_lock(&g_event_hnd_mutex);
	icount = ieventQ->count;

	for (;(index <= icount) && (index < MAX_SIZE_OUT);
			index++)
	{
  fetch_node = (sEventNode *) POP (ieventQ);

  if (fetch_node)
PUSH (&g_event_hnd_rec_Queue, (YSNODE *) fetch_node);
	} 

	pthread_mutex_unlock(&g_event_hnd_mutex);
	TRACE(LOG_CRITICAL , EVENT_HANDLER, "Exiting %s ",__FUNCTION__);
	return &g_event_hnd_rec_Queue;
} 




