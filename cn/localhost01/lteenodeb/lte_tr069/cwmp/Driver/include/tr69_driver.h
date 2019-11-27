#ifndef __TR69_DRIVER__
#define __TR69_DRIVER__


#include <stdbool.h>
#include <stdlib.h>
//#include <common_def.h>

enum eFlag {E_TRUE = 1, E_FALSE = 0};

typedef struct 
{
        char *Manufacturer;     /* required element of type xsd:string */
        char *OUI;      /* required element of type xsd:string */
        char *ProductClass;     /* required element of type xsd:string */
        char *SerialNumber;     /* required element of type xsd:string */
}sDeviceInfo;


/* SPR 17625 FIXED START*/
/*------ Ruby ------*/
enum eParameterAttributeNotificationValueType {eParameterAttributeNotificationValueType__0 = 0, eParameterAttributeNotificationValueType__1 = 1,             eParameterAttributeNotificationValueType__2 = 2, eParameterAttributeNotificationValueType__3 = 3, eParameterAttributeNotificationValueType__4 = 4,           eParameterAttributeNotificationValueType__5 = 5, eParameterAttributeNotificationValueType__6 = 6};

enum ParameterAttributeNotificationValueType {ParameterAttributeNotificationValueType__0 = 0, ParameterAttributeNotificationValueType__1 = 1,                ParameterAttributeNotificationValueType__2 = 2, ParameterAttributeNotificationValueType__3 = 3, ParameterAttributeNotificationValueType__4 = 4,              ParameterAttributeNotificationValueType__5 = 5, ParameterAttributeNotificationValueType__6 = 6};

enum eSetAttr_boolean {set_attr_false_ = 0, set_attr_true_ = 1};     /*--------- Ruby -------*/
enum eAccessListValueType {eAccessListValueType__Subscriber = 0};  /*------- Ruby -------*/

enum AccessListValueType {AccessListValueType__Subscriber = 0};

struct nSetParameterAttributesStruct
{
    char *Name; /* required element of type xsd:string */
    enum eSetAttr_boolean NotificationChange;   /* required element of type xsd:boolean */
    enum eParameterAttributeNotificationValueType Notification;    /* required element of type cwmp:ParameterAttributeNotificationValueType */
    enum eSetAttr_boolean AccessListChange; /* required element of type xsd:boolean */
    struct nAccessList *AccessList;  /* required element of type ArrayOfAccessListValueType */
};   /*------ Ruby -------*/

struct nAccessList
{
    enum eAccessListValueType *__ptrstring;
    int __size;
}; /*------- Ruby -------*/

struct nParameterAttributesList
{
        struct nSetParameterAttributesStruct **__ptrSetParameterAttributesStruct;
            int __size;
};  /*---------- Ruby -----------*/

typedef struct nParameterAttributesList sParameterAttributesList;
/* SPR 17625 FIXED END*/

#define RPC_SETPARAMETERVALUES 1
/* SPR 18262 FIXED START*/
#define BOOTSTRAP_FILE_NAME "/Bootstrap_cpe"
/* SPR 18262 FIXED END*/

#define EVENT_CODE_FILE "Event_Code"
#define REBOOT_CMD_KEY "Reboot_cmd_key"

/*SPR 17625 FIXED START*/
typedef struct ParamAccessListStruct
{
        enum AccessListValueType *__ptrstring;
            int __size;
}ParamAccessList;

typedef struct ParameterAttribute
{
    char *Name;
    enum ParameterAttributeNotificationValueType Notification;
    ParamAccessList *AccessList;
}ParameterAttributeStruct;

struct nParameterList
{
        ParameterAttributeStruct **__ptrParameterAttributeStruct;
        int __size;
};

typedef struct nParameterList sParameterList;


/*SPR 17625 FIXED END*/
void sessionInitCallback (char *sessionId ,char * URL);
void sessionClosedCallback (char *sessionId);
void sessionClosedCallback (char *sessionId );
void* invokeCallback (void *msg, int type, char *sessionId);



typedef struct Policy{
        		bool 		EnableCWMP;
        		char 		*URL;
        		char 		*Username;
        		char 		*Password;
        		bool 		PeriodicInformEnable;
        		unsigned int 	PeriodicInformInterval;   // [1:]
        		time_t 		PeriodicInformTime;
        		char 		*ParameterKey;
        		char 		*ConnectionRequestURL;
        		char 		*ConnectionRequestUsername;
        		char 		*ConnectionRequestPassword;
        		bool 		UpgradesManaged;
        		char 		*KickURL;
        		char 		*DownloadProgressURL;
        		unsigned int 	DefaultActiveNotificationThrottle;
        		unsigned int 	CWMPRetryMinimumWaitInterval;  // [1:65535]
        		unsigned int 	CWMPRetryIntervalMultiplier;    // [1000:65535]
        		char 		*UDPConnectionRequestAddress;
        		bool 		STUNEnable;
        		char 		*STUNServerAddress;
        		unsigned int	STUNServerPort;  // [0:65535]
        		char 		*STUNUsername;
        		char 		*STUNPassword;
        		int 		STUNMaximumKeepAlivePeriod;  //[-1:]
        		unsigned int 	STUNMinimumKeepAlivePeriod;
        		bool 		NATDetected;
        		unsigned int 	ManageableDeviceNumberOfEntries;
                #ifdef VENDOR_SPECIFIC
                char        *HardwareVers;
                char        *SoftwareVers;
                #endif
}Tr69Parameters_t;


typedef struct  CallbackFunction {
			
			void (* sessionInitCallback) (char *sessionId, char* URL );
			/* SPR 16547 Fix Start */
			void (* sessionFailedCallback) (char *sessionId ,int curl_err_code, char* curl_err_str);
			/* SPR 16547 Fix End */
            void (*sessionClosedCallback) (char *sessionId );
			void* (*invokeCallback) (void *msg, int type, char *sessionId);      		

}sCallbackFunList_t;

extern sCallbackFunList_t  stCallbackFunList;
extern void lock_EQueue(void);
extern void unlock_EQueue(void);

/*
extern void initialize_TR069_Client ( sDeviceInfo 		sDeviceIdInfo, 
				sParameterValueList 	sParamValList,     
				Tr69Parameters_t 	tr69Parameters,
				sCallbackFunList_t 	sCallbackFunList, 
				char * 			HeMS_URL,
				bool 			Init_flag);
*/

/*
extern void initiate_TR069_Inform_session (char * , sParameterValueList ParamValList);
extern int autonomousTransferComplete (STATransferComplete *);	
extern int getRPCMethod_ACS ();
*/
/*SPR 16920 Fixed Start */
void initialize_tr69Parameters_parameter(Tr69Parameters_t  *tr69Parameters);
/*SPR 16920 Fixed End */

/* SPR 16732 FIX START */
int snscanf(const char *str, int n, const char *format, ...);
/* SPR 16732 FIX START */

#endif 
