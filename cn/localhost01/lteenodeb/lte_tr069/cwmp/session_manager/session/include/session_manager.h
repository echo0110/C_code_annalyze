#ifndef __SESSION_MANAGER_H__
#define __SESSION_MANAGER_H__

#include	<soapStub.h>
#include	<stdlib.h>
#include	<client_req_manager.h>
#include	<rpc_handler.h>
#include	<event_handler.h>
#include	<stub.h>
#include	<soap_encoder.h>
/* SPR 16547 Fix Start */
#include <curl/curl.h>
/* SPR 16547 Fix End */


#define TRUE 1
#define MAX_URL_LEN 256
#define QUEUE ysPushTail
#define DEQUEUE ysPopHead
#define DEBUG_FLAG 0
#define getClientReq  DEQUEUE
#define SESSION_MANAGER  "Session_Manager"
#define HEX_STR_LEN 32
/* SPR 22314 Start */
void tr069_free_ssetparametervalues(sSetParameterValues *ptr);
void  free_informReq_buffer(sInform *informReq);
#define TR069_FREE_SINFORM(ptr) do\
    {\
        free_informReq_buffer(ptr); \
        ptr = NULL;\
    }while(TR069_ZERO)
/* SPR 22314 End */

extern YSQUEUE g_event_hnd_eventQueue;
extern YSQUEUE g_clientRequestQ;
extern pthread_mutex_t g_event_hnd_mutex;


UINT4 ipr_cwmp_isSessionTerminated(char * resp_xml, UINT4 flag);
SInt32 ipr_cwmp_SessionInit(DeviceInfo *devidstruct, char *URL, struct ParameterValueList *paramListi, char *, char *);
sEventList *ipr_cwmp_get_event_list (YSQUEUE *ieventQ);
sInformResponse * ipr_cwmp_Inform(sInform *, char *, int *, sHeader **, char *, char *i, char **);
sInform * ipr_cwmp_createInformReq(DeviceInfo *, struct ParameterValueList *,  sEventList *);
sTransferCompleteResponse * ipr_cwmp_TransferComplete(sTransferComplete *, char *, int *,char *, char *);
struct _cwmp__GetRPCMethodsResponse *ipr_cwmp_GetRPCMethods_from_acs(char *, int *, char *, char*);
struct _cwmp__RequestDownloadResponse* ipr_cwmp_RequestDownload(struct _cwmp__RequestDownload *, char *, int *, char *, char *);
sAutoTransCompResp * ipr_cwmp_AutoTransCompReq(sAutoTranComplete *, char *, int *,char *, char *);
char *ipr_cwmp_SendRespToDriver(void *, int ,sHeader );
struct MethodList *ipr_cwmp_GetRPCMethods(void);
void ipr_cwmp_free_paramlist(struct ParameterValueList *ptr);

// Phase 2
/* SPR 13359 Fix Start */
sAutoDUStateChangeCompResp * ipr_cwmp_AutoDUStateChangeCompleteReq(sAutoDUStateChangeComplete *,char *, SInt32 *, char *, char *);
/* SPR 13359 Fix End */
/* SPR 21563 FIXED START*/
void Free_SPA_Request(struct _cwmp__SetParameterAttributes *Setparamattr_t);
void Free_GPA_Request(struct _cwmp__GetParameterAttributes *l_getparamAttr_t);
void Free_GPA_Parameters(sParameterList *p_get_stat_attr_t);
/* SPR 21563 FIXED END */

/* SPR 21563_p1 FIXED START */
void Free_Upload_Req(struct _cwmp__Upload *l_inp_t);
/* SPR 21563_p1 FIXED END */
#endif
