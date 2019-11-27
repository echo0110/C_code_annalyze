#define STUB_SETPARAMETERVALUES SOAP_TYPE__cwmp__SetParameterValues
#define STUB_GETPARAMETERVALUES SOAP_TYPE__cwmp__GetParameterValues
#define STUB_GETPARAMETERNAMES SOAP_TYPE__cwmp__GetParameterNames
#define STUB_SETPARAMETERATTRIBUTES SOAP_TYPE__cwmp__SetParameterAttributes
#define STUB_GETPARAMETERATTRIBUTES SOAP_TYPE__cwmp__GetParameterAttributes
#define STUB_ADDOBJECT SOAP_TYPE__cwmp__AddObject
#define STUB_DELETEOBJECT SOAP_TYPE__cwmp__DeleteObject
#define STUB_DOWNLOAD SOAP_TYPE__cwmp__Download
#define STUB_REBOOT SOAP_TYPE__cwmp__Reboot
#define STUB_GETRPCMETHODS SOAP_TYPE__cwmp__GetRPCMethods
#define STUB_INFORM_RESPONSE SOAP_TYPE__cwmp__Inform



//typedef struct cwmp__ParameterValueStruct sparamStruct;

void *my_malloc( unsigned int size );
void *my_calloc(unsigned int nmemb, unsigned int size);
void  my_free( void *m );
struct ParameterValueList *stub_get_param_list(void);
sEventList *stub_get_event_list (void);
struct ParameterValueList *stub_get_param_list (void);
sInform * stub_createInformReq (DeviceInfo *,struct EventList *);
int stub_for_sessionInit (void);
int stub_send_http_msg(char *, char *, char **, char *, char *, int);
void stub_print_param_list (struct ParameterValueList * List);
