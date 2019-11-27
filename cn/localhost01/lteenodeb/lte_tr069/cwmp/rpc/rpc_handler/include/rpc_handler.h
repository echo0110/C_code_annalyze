/********************************************************************************
 *
 *      FILE NAME   :rpc_handler.h
 *
 *      DESCRIPTION : This file is used for including header
 *
 *      REVISION HISTORY:
 *
 *      NAME             DATE            REFERENCE               REASON
 *_______________________________________________________________________________
 *
 *   Vikas Upadhyay    Aug 17,2012   TR-069 Amendment 4     Initial Creation
 *   Anant K Sharma    Mar 29,2013                          Added HAVE_IPSEC
 *   Anant K Sharma    Apr 08,2013                          Added check for connectivity of servers after tunnel establishment
 *
 *
 *       All Rights Reserved. Copyright @ Aricent Group, 2012.
 *
 ********************************************************************************/
#ifndef __RPC_HANDLER_H__
#define __RPC_HANDLER_H__

#include <stdio.h>
#include <stdlib.h>
#include <common_def.h>
#include <tr69_driver.h>
#include <ManagementServerModel.h>
#include    <stub.h>
/* SPR 22314 Start */
#include <map_glue.h>
/* SPR 22314 End */

#define CPE_PARAM "InternetGatewayDevice." 
#define CPE_PARAM_PREFIX "InternetGatewayDevice.ManagementServer." 
#define CPE_PARAM_PREFIX_LEN 39 
#define FAP_PARAM_PREFIX_LEN 33 
/* SPR 16448 Fix Start */
#define MAX_PARAM_NAME_SIZE 300 
/* SPR 16448 Fix End */
#define MAX_PARAM_VAL_SIZE  512
#define FAULT_STRING_SIZE 256
#define FAULT_CODE_SIZE	5

#define RESULT_OK 0
#define COMMAND_KEY_SIZE 32
#define EVENT_CODE_SIZE 64
#define RPC_HANDLER  "RPC Handler"
#define DOWNLOAD_EVENT_CODE "M Download"
#define UPLOAD_EVENT_CODE "M Upload"
#define SCH_DOWNLOAD_EVENT_CODE "M ScheduleDownload"

#define FAULT_9005      "9005"
#define FAULT_9005_STR  "Invalid Parameter name"

#define FAULT_9007      "9007"
#define FAULT_9007_STR  "Invalid Parameter value"

#define FAULT_9003 "9003"
#define FAULT_9003_STR "Invalid Aruments"
/* SPR 20754 FIXED START */
/* SPR 20754 FIXED END */

#define MAX_THREADS_DWN 3

#define ENABLECWMP "EnableCWMP"
#define URL_STRING "URL"
#define USERNAME "Username"
#define PWD "Password"
#define PERIOD_INFORM_EN "PeriodicInformEnable"
#define PERIOD_INFORM_INT "PeriodicInformInterval"
#define PERIOD_INFORM_TIME "PeriodicInformTime"
#define PARAM_KEY "ParameterKey"
#define CONN_REQ_URL "ConnectionRequestURL"
#define CONN_REQ_USR "ConnectionRequestUsername"
#define CONN_REQ_PWD "ConnectionRequestPassword"
#define UPGRD_MANAGED "UpgradesManaged"
#define KICKURL "KickURL"
#define DWN_PROG_URL "DownloadProgressURL"
#define DF_ACT_NOTIF "DefaultActiveNotificationThrottle"
#define CWMP_RETRY_MIN_WAIT "CWMPRetryMinimumWaitInterval"
#define CWMP_RETRY_INT_MULT "CWMPRetryIntervalMultiplier"
#define UDP_CONN_REQ_ADDR "UDPConnectionRequestAddress"

//IPSec phase 2 code starts
#ifdef HAVE_IPSEC
#define PARENT_SECGW "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway."
#define PARENT_SECGW_LEN 69
#define SECGWS1 "SecGWServer1"
#define SECGWS2 "SecGWServer2"
#define SECGWS3 "SecGWServer3"
#define CHILD_SECGW_LEN 12

#define PARENT_FAP_TUNNEL_PREFIX "InternetGatewayDevice.FAP.Tunnel." 
#define PARENT_FAP_TUNNEL_PREFIX_LEN 33
#define CHILD_FAP_TUNNEL "CryptoProfileNumberOfEntries"
#define CHILD_FAP_TUNNEL_LEN 28

#define PARENT_CRYPTO_PROF "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1]."
#define PARENT_CRYPTO_PROF_LEN 51
#define CHILD_CRYPTO_PROF_ENABLE "Enable"
#define CHILD_CRYPTO_PROF_ENABLE_LEN 6
#define CHILD_CRYPTO_PROF_IKEREKEYLIFETIME "IKERekeyLifetime"
#define CHILD_CRYPTO_PROF_IKEREKEYLIFETIME_LEN 16
#define CHILD_CRYPTO_PROF_IPSECREKEYLIFETIMETIME "IPsecRekeyLifetimeTime"
#define CHILD_CRYPTO_PROF_IPSECREKEYLIFETIMETIME_LEN 22
#define CHILD_CRYPTO_PROF_AUTHMETHOD "AuthMethod"
#define CHILD_CRYPTO_PROF_AUTHMETHOD_LEN 10 
#endif
//IPSec phase code ends

//#define MAX_DW_LOG_NAME_LEN 266
//#define DW_FILE_PATH_NAME "./Download/dwn"




/*
typedef char* ParameterKey;
typedef struct cwmp__ParameterValueStruct ParameterValueStruct;
typedef struct ParameterValueList ParameterList;
typedef struct _cwmp__SetParameterValues SetParameterValues;
typedef struct _cwmp__DownloadResponse DownloadResponse;
*/
//typedef struct ParameterNames ParameterNames;		

extern YLIST g_Txn_List_Download;

typedef char* ParameterKey;
typedef  struct _cwmp__Fault STFault;
void free_getparam (ParameterNames *);
StStatus*  ipr_cwmp_setParameterValues(ParameterList *);
UInt16  ipr_cwmp_Download(STDownload *);
void *  ipr_cwmp_actualDownload(void  * arg);
StStatus  *  ipr_cwmp_ManagementServerModuleSet(SetParameterValues *stSetParamVal);
GetStatus *  ipr_cwmp_getParameterValues(ParameterNames *pNames);
SetParamAttrRes * ipr_cwmp_setParameterAttribute (SetParameterAttributesList *arg_paramList_t);

void create_transfer_complete (CHAR8 * CommandKey , CHAR8 *FaultCode ,
                               CHAR8 * FaultString , time_t t1 , time_t t2);

AddObjectStatus_t  * ipr_cwmp_AddObject(sAddObject *stSetParamVal);
DeleteObjectStatus_t  * ipr_cwmp_deleteobject(sDeleteObject *ParamVal);
RebootStatus_t  * ipr_cwmp_reboot(void);
FactoryResetStatus_t  * ipr_cwmp_factoryreset(void);
GetParameterNamesStatus_t  * ipr_cwmp_getparametername(sGetParameterNames *ParamVal);
CancelTransferStatus_t  * ipr_cwmp_canceltransfer(sCancelTransfer *ParamVal);
SInt16 ipr_cwmp_startScheduleDownload (ScheduleDownload * arg_schDwn_t);    
GetParamAttrRes * ipr_cwmp_GetParameterAttribute (ParameterNames  *arg_pNames_t);
SInt32 ipr_cwmp_ScheduleInform (ScheduleInform *arg_Sh_Inform_t);
AllTransferList* ipr_cwmp_GetAllQueuedTransfers(void);
/* SPR 21563 FIXED START */
void Free_GetParameterAttribute(ParameterNames *newParamName);
/* SPR 21563 FIXED END */
/* SPR 22314 Start */
void tr069_free_parameterValueList(struct ParameterValueList *ptr);
/* SPR 22314 End */
SInt16 ipr_cwmp_Upload (Upload *arg_upload_t);
#endif 
