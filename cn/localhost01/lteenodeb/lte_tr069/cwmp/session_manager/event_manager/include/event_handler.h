#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <soapStub.h>
#include <common_def.h>
/* SPR 22314 Start */
#include <map_glue.h>
/* SPR 22314 End */
extern YSQUEUE g_event_eventQueue;
extern YSQUEUE g_event_clientRequestQ;

#define PRIORITY_NUMBER 1
#define PUSH ysPushTail
#define POP ysPopHead
#define MAX_SIZE_OUT 63

#define EVENT_HANDLER "Event Handler"


//typedef enum eventType
//{
//	 BOOTSTRAP = 0,
//	 BOOT, 
//	 PERIODIC,
//	 SCHEDULED,
//	 VALUE_CHANGED,
//	 KICKED,
//	 CONNECTION_REQUEST,
//	 TRANSFER_COMPLETE,
//	 DIAGNOSTICS_COMPLETE,
//	 REQUEST_DOWNLOAD,
//	 AUTONOMOUS_TRANSFER_COMPLETE,
//} eEventType; 
//
//typedef struct cwmp__DeviceIdStruct sDeviceInfo;


//struct cwmp__EventStruct
//{
//        char *EventCode;        /* required element of type cwmp:EventCodeType */
//        char *CommandKey;       /* required element of type cwmp:CommandKeyType */
//} ;
//
typedef struct cwmp__EventStruct EventStruct;

//struct EventList
//{
//    struct cwmp__EventStruct*            *__ptrEventStruct              ;
//    int                                  __size                        ;
//};
//
//typedef struct EventList sEventList;

typedef struct stEvent
{
        YSNODE anchor;
        EventStruct * sEvent;
	ParameterValueList  *sParamValList;
}sEventNode;


/* SPR 22314 Start */
void tr069_free_parammetervaluelist_soap_mem(struct ParameterValueList *ptr);
/* SPR 22314 End */
void  event_manager_init(void);
int ipr_cwmp_addEvent (EventStruct * , ParameterValueList *);
YSQUEUE * getAll (YSQUEUE *);
void lock_EQueue(void);
void unlock_EQueue(void);


#endif 
