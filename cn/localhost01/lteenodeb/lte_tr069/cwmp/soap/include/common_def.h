#ifndef _COMM_DEF_H
#define _COMM_DEF_H

#include	<soapStub.h>
#include 	<cspl.h>
#include  	<tr69_driver.h>
#include    <ipr_cwmp_enb_wrapper.h>

#define FAILED 1
#define SUCCESS 0
/* SPR 16451 Fix Start */
#define CRITICAL_ERROR 255
/* SPR 16451 Fix End */
#define TRUE 1
#define FALSE 0
#define RPC_SETPARAMETERVALUES 1
#define RPC_GETPARAMETERVALUES 2
#define RPC_PREDOWNLOAD	3
#define RPC_DOWNLOAD	4
#define RPC_SET_URL 5
#define RPC_SETPARAMATTRIBUTE 6
#define RPC_GETPARAMETERATTRIBUTE 7
#define RPC_SCHEDULE_DOWNLOAD 13 
#define RPC_PREUPLOAD 14
#define RPC_UPLOAD 15
#define RPC_PRESCHEDULE_DOWNLOAD 16

// Phase 2 
#define RPC_ADDOBJECT   8
#define RPC_DELETEOBJECT 9
#define RPC_REBOOT  10
#define RPC_FACTORYRESET 11
#define RPC_GETPARAMETERNAME 12
#define RPC_CANCELTRANSFER 17

/* SPR 17596 fixed start */
#define RPC_GETRPCMETHOD 16
/*SPR 17596 fixed end */

#define SOFTWARE_VERS "/SoftwareVers"
#define UPD_DWN_CMDKEY "/Upgrade_key"

#define DOWNLOAD_EVENT_CODE "M Download"
 

//#define TRACE logger_trace

#define EVENT_CODE_SIZE 64
#define COMMAND_KEY_SIZE 32 	
#define FAULT_CODE_SIZE 5
#define FAULT_STRING_SIZE 256

#define FAULT_CODE_9004 "9004"
#define FAULT_CODE_9004_STR "Resources exceeded"
#define FAULT_CODE_9004_STR_LEN 19 
#define FAULT_CODE_9003 "9003"
#define FAULT_CODE_9003_STR "Invalid arguments"
#define FAULT_CODE_9003_STR_LEN 17
#define FAULT_CODE_9002 "9002"
#define FAULT_CODE_9002_STR "Internal error"
#define FAULT_CODE_9002_STR_LEN 14

#define FAULT_CODE_9018 "9018"
#define FAULT_CODE_9018_STR "File transfer failure: file corrupted"

#define FAULT_9004 9004
#define FAULT_9002 9002


#define MAX_WAIT_TIME 600

#define SCH_INFRM_EVNT_CODE "M ScheduleInform"

#define FILE_TYPE_SIZE 64
#define URL_SIZE 256 
#define USR_NAME_SIZE 256
#define PWD_SIZE 256
#define TRG_NAME_SIZE 256
#define PARAM_KEY_SIZE 32
#define CWMP_RET 65535
#define U_INT_RANGE "4294967295"

#define BOOTSTRAP_CODE "0 BOOTSTRAP"

#define BOOT_CODE "1 BOOT"

//Phase 2
#define REBOOT_CODE "M Reboot"
#define FIRM_UPGRADE_FILE '1' 
#define VENDOR_CONFIG_FILE '3'

#define UINT4 unsigned int

typedef unsigned int UINT32;
typedef short UINT16;
typedef int SInt32;
typedef char CHAR8;
typedef long int LongInt;

#define MAX_SIZE_OUT 63

#define MAX_DW_LOG_NAME_LEN 266
#define DW_FILE_PATH_NAME "/Download/dwn"

#define MAX_UPL_LOG_NAME_LEN 266
#define UPL_FILE_PATH_NAME "/Download/upl"

#define SCH_DW_FILE_PATH_NAME "./Download/schdwn"


#define ENB_OPENDIR opendir
#define ENB_READDIR readdir
#define SOFT_VERS_SIZE 64




#define DW_DIR "/download"
#define PTHREAD_STACK_SIZE 1024*1024



enum eCpeRPCMethod {
	GET_RPC_METHODS = 1,
	SET_PARAMETER_VALUES,
	GET_PARAMETER_VALUES,
	GET_PARAMETER_NAMES,
	SET_PARAMETER_ATTRIBUTES,
	GET_PARAMETER_ATTRIBUTES,
	ADD_OBJECT,
	DELETE_OBJECT,
	DOWNLOAD,
	REBOOT,
	UPLOAD,
	FACTORY_RESET,
	GET_QUEUED_TRANSFERS,
	SCHEDULE_INFORM
};

enum eEvent { 
	BOOTSTRAP = 1,
	BOOT,
	PERIODIC,
	SCHEDULED,
	VALUE_CHANGE,
	KICKED,
	CONNECTION_REQUEST,
	TRANSFER_COMPLETE,
	DIAGNOSTICS_COMPLETE,
	REQUEST_DOWNLOAD,
	AUTONOMOUS_TRANSFER_COMPLETE,
	DU_STATE_CHANGE_COMPLETE,
	AUTONOMOUS_DU_STATE_CHANGE_COMPLETE
};


typedef struct cwmp__FaultStruct cwmp__FaultStruct ;
typedef struct cwmp__ParameterValueStruct cwmp__ParameterValueStruct ;
typedef struct cwmp__ParameterInfoStruct cwmp__ParameterInfoStruct ;
typedef struct cwmp__SetParameterAttributesStruct cwmp__SetParameterAttributesStruct ;
typedef struct cwmp__ParameterAttributeStruct cwmp__ParameterAttributeStruct ;
typedef struct cwmp__QueuedTransferStruct cwmp__QueuedTransferStruct ;
typedef struct cwmp__AllQueuedTransferStruct cwmp__AllQueuedTransferStruct ;
typedef struct cwmp__OptionStruct cwmp__OptionStruct ;
typedef struct cwmp__ArgStruct  cwmp__ArgStruct ;
typedef struct ParameterValueList ParameterValueList;
typedef  struct ParameterNames ParameterNames;

typedef struct _cwmp__DownloadResponse DownloadResponse;
typedef struct _cwmp__SetParameterValues SetParameterValues;
typedef struct cwmp__ParameterValueStruct ParameterValueStruct;
typedef struct ParameterValueList ParameterList;

/* Phase 2 */

typedef struct ParameterInfoList  ParameterInfoList;
typedef cwmp__ParameterInfoStruct ParameterInfoStruct;

typedef struct _cwmp__InformResponse  sInformResponse ;
typedef struct cwmp__EventStruct  sEventStruct ;
typedef struct cwmp__DeviceIdStruct DeviceInfo ;
typedef struct _cwmp__Inform sInform ;
typedef struct EventList  sEventList;
typedef struct SOAP_ENV__Header sHeader;
typedef struct _cwmp__Download STDownload;
typedef  struct _cwmp__TransferComplete sTransferComplete;
typedef  struct _cwmp__TransferCompleteResponse sTransferCompleteResponse;

typedef struct _cwmp__AutonomousDUStateChangeComplete  sAutoDUStateChangeComplete ;
typedef struct _cwmp__AutonomousDUStateChangeCompleteResponse sAutoDUStateChangeCompResp ;


typedef struct _cwmp__AutonomousTransferComplete  sAutoTranComplete ;
typedef struct _cwmp__AutonomousTransferComplete STATransferComplete;

typedef struct _cwmp__AutonomousTransferCompleteResponse sAutoTransCompResp ;
typedef struct _cwmp__SetParameterValues sSetParameterValues;
typedef struct _cwmp__GetParameterValues sGetParameterValues;
// Phase 2
typedef struct _cwmp__AddObject sAddObject;
typedef struct _cwmp__DeleteObject sDeleteObject;

typedef struct _cwmp__Reboot sReboot;
typedef struct _cwmp__GetParameterNames sGetParameterNames;
typedef struct _cwmp__CancelTransfer sCancelTransfer;
typedef struct _cwmp__RequestDownload sRequestDownload;

 


typedef enum {PASS=0, DWN_FAILURE=1, FAULT=2} eDownStatus;


typedef enum
{
	TRANS_COMPLETE_REQ=135,
	AUTONOMOUS_COMPLETE_REQ=138,
	REQUEST_DOWNLOAD_REQ=142,
	GET_RPC_METHOD = 62,
	AUTONOMOUS_DU_STATE_CHANGE_COMPLETE_REQ=149
}ERequest;


typedef struct _clientRequest
{
        YSNODE anchor;
        ERequest  eReqtype;
        char*    EventCode;
        union
        {
                struct _cwmp__TransferComplete * pstTxComplete;
                struct _cwmp__AutonomousTransferComplete *pstAutoTxComplete;
                struct _cwmp__RequestDownload *pstReqDownload;
                struct _cwmp__AutonomousDUStateChangeComplete *pstAutoDUStateChangeComplete;
        };
}ClientRequest;



typedef enum {
                RPC_SUCCESS=0,
                RPC_PARTIAL=1,
                RPC_FAULT=2
}EStatus;

typedef enum {
                RPC_TRUE=0,
                RPC_FALSE=1,
}CancelStatus;

typedef enum {
                CANCEL_TRUE=0,
                CANCEL_FALSE=1,
}IsCancel;


typedef struct  _status{
                        EStatus eStatus;
                        struct _cwmp__Fault *stFault;

}StStatus;

typedef struct
{
        eDownStatus eStatus;
        DownloadResponse *sDownRes;
		unsigned int fault_code;
		char *fault_string;

}DownloadStatus;

typedef struct  {
                EStatus eStatus;
                struct ParameterValueList *parameterList;
                unsigned int fault_code;
}GetStatus;


struct threadDownload
{
        YSNODE anchor;
        char *dw_logfilename;
        STDownload *pstDownload;
        CancelStatus Status;
        IsCancel cancelStatus;
        time_t recv_time;
        S32 delay;
};

typedef struct threadDownload ThreadDownload;



typedef struct SetParameterAttributesList SetParameterAttributesList;
typedef struct cwmp__SetParameterAttributesStruct SetParameterAttributesStruct;

struct SetParamAttrRes
{
    EStatus eStatus;
    UINT16 fault_code;
    CHAR8  *fault_string;
};

typedef struct SetParamAttrRes  SetParamAttrRes;
 
struct GetParamAttrRes
{
    EStatus eStatus;
    UINT16 fault_code;
	/*GPA FIX Start*/
    CHAR8  *fault_string;
	/*GPA FIX End*/
    struct ParameterAttributeList *parameterList;
};

typedef struct GetParamAttrRes GetParamAttrRes;

 

// Phase 2
typedef struct  {
                EStatus eStatus;
                UINT16 instance_number;
                UINT16 fault_code;
                /* SPR 13359 Fix Start */
                char * fault_string;
                /* SPR 13359 Fix End */
}AddObjectStatus_t;

typedef struct  {
                EStatus eStatus;;
                UINT16 fault_code;
                /* SPR 13359 Fix Start */
                char * fault_string;
                /* SPR 13359 Fix End */
}DeleteObjectStatus_t;

typedef struct  {
                EStatus eStatus;
                UINT16 fault_code;
                /* SPR 13359 Fix Start */
                char * fault_string;
                /* SPR 13359 Fix End */
}RebootStatus_t;

/* SPR 17596 fixed start */
typedef struct {
                EStatus eStatus;
                int fault_code;
                struct _cwmp__GetRPCMethodsResponse *resp;
                char* fault_string;
}GetRPC_t;
/* SPR 17596 fixed end */

    /* SPR 19268 FIXED START*/

typedef struct UploadResponse
{   
    EStatus eStatus;
    UINT16 fault_code;
    CHAR8  *fault_string;
    CHAR8  *filepath;
}UpldResp;

    /* SPR 19268 FIXED END*/

typedef struct  {
                EStatus eStatus;
                UINT16 fault_code;
                /* SPR 13359 Fix Start */
                char * fault_string;
                /* SPR 13359 Fix End */
}FactoryResetStatus_t;

typedef struct  {
                EStatus eStatus;
                struct ParameterInfoList  *parameterList;
                UINT16 fault_code;
                /* SPR 13359 Fix Start */
                char * fault_string;
                /* SPR 13359 Fix End */
}GetParameterNamesStatus_t;

typedef struct _cwmp__ScheduleDownload ScheduleDownload;
typedef struct  {
                EStatus eStatus;
                UINT16 fault_code;
                /* SPR 13359 Fix Start */
                char * fault_string;
                /* SPR 13359 Fix End */
}CancelTransferStatus_t;

typedef struct _cwmp__Upload Upload;


typedef struct
{
    YSNODE anchor;
    CHAR8 *upl_logfilename;
    CHAR8 *local_path;
    Upload *pstupload;
    time_t recv_time;
    S32 delay;
    CancelStatus Status;
    IsCancel cancelStatus;
}ThreadUpload;

typedef struct
{
    YSNODE anchor;
    ScheduleDownload *ptr_dwn_t;
    S32 timeWin_1_start;
    S32 timeWin_1_end;
    S32 timeWin_2_end;
    S32 timeWin_2_start;
    time_t rec_time;
    CancelStatus Status;
    IsCancel cancelStatus;
    char *dw_logfilename;
}ThreadSchDwn;

typedef struct AllTransferList AllTransferList;

 typedef struct _cwmp__ScheduleInform ScheduleInform;


/* All declarations for Forced notification */

/* SPR 16568 FIX START */
#define NUM_FORCED_PARAM 5 
/* SPR 16568 FIX END */
 /* SPR 13658 START */
/* SPR  14568 Fix Start */
#define FAPCONTROL_OPSTATE "InternetGatewayDevice.Services.FAPService.1.FAPControl.LTE.OpState"
#define FAPCONTROL_OPSTATE_LEN 67

#define FAPCONTROL_RFTXSTATUS "InternetGatewayDevice.Services.FAPService.1.FAPControl.LTE.RFTxStatus"
#define FAPCONTROL_RFTXSTATUS_LEN 70
/* SPR  14568 Fix End */

#define LTE_LASTSCANTIME "InternetGatewayDevice.Services.FAPService.1.REM.LTE.LastScanTime"
#define LTE_LASTSCANTIME_LEN 65


#define LTE_NRT_UPDATETIME "InternetGatewayDevice.Services.FAPService.1.CellConfig.LTE.RAN.NeighborListInUse.X_VENDOR_LastNRTUpdateTime"
#define LTE_NRT_UPDATETIME_LEN 108
 /* SPR 13658 END */

/* SPR 16568 FIX START */
#define CONNECTION_REQ_URL "InternetGatewayDevice.ManagementServer.ConnectionRequestURL"
#define CONNECTION_REQ_URL_LEN 65
#define CONNECTION_REQ_URL_INDX 4
/* SPR 16568 FIX END */

#define OPSTATE_INDX 0
#define RFTXSTATUS_INDX 1
#define LASTSCANTIME_INDX 2
#define UPDATETIME_INDX 3

#define UPD_DWN_URL "/Upgrade_url"



#endif




                                                
