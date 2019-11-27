 /*****************************************************************************************************
 *
 **  FILE NAME     :  glue_layer_def.h
 **
 **  DESCRIPTION   :  This file contains the common types. 
 **
 **
 **  DATE        AUTHOR      REF             REASON
 **  ----        ------      ---             ----------
 ** Jan 30, 2013 Anant K Sharma Added preprocessor directive to prevent multiple inlusion 
 ** Jan 30, 2013 Anant K Sharma Added ipr_cwmp_add_ip to save IP in persistent memory 
 ** Jan 30, 2013 Anant K Sharma Added ipr_cwmp_urltoip for extracting IP string from url string containing IP
 ** Copyright (c) 2013, Aricent Inc. All Rights Reserved
 **
 **  Revision Details
 **  ----------------
 **
 **
 ********************************************************************************************************/
#ifndef _GLU_LAYER_DEF
#define _GLU_LAYER_DEF

#include<lteTypes.h>
#include<ipr_cwmp_enb_wrapper.h>
/* SPR 13658 START */
#include<soapH.h>
/* SPR 13658 END */
#include<common_def.h>

/*Types ************************/

/*
typedef unsigned char   UInt8;
typedef unsigned short  UInt16;
typedef unsigned int    UInt32;
*/


/* SPR 17596 fixed start */
#ifndef SOAP_TYPE__cwmp__GetRPCMethodsResponse
#define SOAP_TYPE__cwmp__GetRPCMethodsResponse (63)
#endif
/* SPR 17596 fixed end */
typedef char CHAR8;
/* SPR 17625 FIXED START */ 
#define API_HEADER_SIZE 12
/* SPR 17625 FIXED END */

#define DEFAULT_PORT "7547"

#define CERT_PATH "CERT_PATH"
#define RECV_PORT "RECV_PORT"
#define GLUE_PORT "GLUE_PORT"
#define NULL_STR "NULL"
#define ENB_GL_PRINTF printf 

#define TR069_MODULE_ID 20
#define OAM_MGMT_INTF_MODULE_ID 13

#define INIT_IND_WAIT_INTERVAL "INIT_IND_WAIT_INTERVAL"

#define RPC_SETPARAMETERVALUES 1
#define RPC_GETPARAMETERVALUES 2
#define RPC_PREDOWNLOAD 3
#define RPC_DOWNLOAD    4
#define RPC_SETPARAMATTRIBUTE 6
/* SPR 17625 FIXED START */
#define RPC_GETPARAMETERATTRIBUTE 7
/* SPR 17625 FIXED END */
#define RPC_DELETEOBJECT 9
#define RPC_REBOOT  10
#define RPC_FACTORYRESET 11
#define RPC_CONTROL_CMD  18
#define RPC_PREUPLOAD 14


#define RPC_ADDOBJECT   8
#define RPC_DELETEOBJECT 9
#define RPC_REBOOT  10
#define RPC_FACTORYRESET 11
/* SPR 17717 fixed start */
#define RPC_RAISE_ALARM 19
/* SPR 17717 fixed end */
/* Spr 16990 Fix Start */
#define RPC_GETPARAMETERNAME 12
/* Spr 16990 Fix End */

/* SPR 17596 fixed start */
#define RPC_GETRPCMETHOD 16
/* SPR 17596 fixed end */
#define SUCCESS 0
#define FAILURE 1
#define DWN_FAULT 2

/* SPR 19021 fix start */
/* SRP 19162 FIXED START */
/* code removed */
/* SRP 19162 FIXED START */
/* SPR 19021 fix end */

#define GLUE_LAYER "Glue_Layer"

/* Spr 16751 changes Start */
/* Code Removed */
/* Spr 16751 changes End */

/* SPR 16608 Fix Start */
#define MAX_PARAM_NAME_SIZE 300
/* SPR 16608 Fix End */

#define FAULT_CODE_SIZE 5
#define FAULT_STRING_SIZE 256

#define DEFAULT_FAULT_CODE 9003
#define DEFAULT_FAULT_STR "Invalid arguments"
#define DEFAULT_FAULT_CODE_LEN 5
/* sg wait_limit change start*/
#define WAIT_LIMIT 240
/* sg wait_limit change end */

/*SPR 17154 Fixed Start */
#define WAIT_LIMIT_DOWNLOAD 6000
/*SPR 17154 Fixed End */
/* GPV PARTIAL PATH Change Start */
/* GPN Change Start */
#define WAIT_LIMIT_GPN_RESP 240 
/* GPN Change End */
/* GPV PARTIAL PATH Change End */
#define INTERNAL_FAILURE 9002

#define INTERNAL_FAILURE_STR "Internal Error"
#define INTERNAL_FAILURE_LEN 5

#define GLUE_INIT 11

#define URL_FILE_NAME "cpe_seconday_url"
#define RPC_SET_URL 5
#define SHUTDOWN "AdminControlCommand=shutdown"

#define INIT_CONFIG_CMD "AdminControlCommand=InitiateConfiguration"

//#define HEMSIPADDRESS "HeMSIPAddress"
//#define HENBSELF "HeNBSelfIPAddress"
#define HENBSELF "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.X_VENDOR_HENBIPADDRESS"

#define SECURITY_GATEWAY_FILE "./security_gateway_ips"
#define PARENT_SEC_GATEWAY_SPEC "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway"
#define PARENT_SEC_GATEWAY_SPEC_LEN 68

#define PARENT_CRYPTO_PROFILE "InternetGatewayDevice.FAP.Tunnel.CryptoProfile"
#define PARENT_CRYPTO_PROFILE_LEN 46

#define CHILD_CRYPTO_PROFILE "CryptoProfileNumberOfEntries"
#define CHILD_CRYPTO_PROFILE_LEN 28 
#define PARENT_CRYPTO_PROF "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1]."
#define PARENT_CRYPTO_PROF_LEN 51
#define PARENT_SECGW "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway."
#define PARENT_SECGW_LEN 69



#define URL_PARENT "InternetGatewayDevice.ManagementServer"
#define URL_PARENT_LEN 38


#define PASSWORD "PASSWORD"
#define USERNAME "USERNAME"
#define INFORMENABLE "INFORMENABLE"
#define INFORMINTERVAL "INFORMINTERVAL"
#define CONNPASSWORD "CONNPASSWORD"
#define CONNUSERNAME "CONNUSERNAME"
#define MANUFACTURER "MANUFACTURER"
#define OUI_ENV "OUI"
#define PRODUCTCLASS "PRODUCTCLASS"
#define SERIALNO "SERIALNO"
#define CONN_REQ_URL "CONN_REQ_URL"
/*download upload change starts*/
#define DWN_QUEUE_MAX "DWN_QUEUE_MAX"
#define UPL_QUEUE_MAX "UPL_QUEUE_MAX"
/*download upload change ends*/


/* SPR  14568 Fix Start */
#define  NOTIF_PARENT_FAPCONTROL "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE"
#define NOTIF_PARENT_FAPCONTROL_LEN 60
/* SPR  14568 Fix End */

#define FAULT_MGMT_EXPEDITED_STR "InternetGatewayDevice.FaultMgmt.ExpeditedEvent"
#define FAULT_MGMT_EXPEDITED_STR_LEN 46 

#define FAULT_MGMT_QUEUED_EVT "InternetGatewayDevice.FaultMgmt.QueuedEvent"
#define FAULT_MGMT_QUEUED_EVT_LEN 43 

#define OPSTATE "OpState"
#define OPSTATE_LEN 7

#define RFXSTATUS "RFTxStatus"
#define RFXSTATUS_LEN 9

/*CSR 57877 Changes*/
/* Removed API */


#define ERROR_CODE_CONFIG 2001
//#define ERR_STR "HeMSIPAddress not found"
//#define ERR_STR_LEN 0x18 

#define ERR_STR "Failed to establish tunnel"
#define ERR_STR_LEN 0x1B //IPSEC SIT changed from 18 to 1B

#define TRO69_IP_SAVE "./tr069_ip_save" 

#define DEFAULT_FILE_TYPE "3 Vendor Configuration File" 
#define DEFAULT_FILE_TYPE_LEN 28

#define TRG_NAME_SIZE 256
#define URL_SIZE 256

#define FACTORY_RESET_INTERVAL  "FACTORY_RESET_INTERVAL"


#define GLUE_SUCCESS 1 
#define GLUE_FAILURE 0 
/* SPR 22314 Start */
#define SET_PARAM_PAYLOAD_SIZE 8192
#define DEVICE_INFO_NAME_VALUE_MAX_LEN    1350
/* SPR 22314 End */ 
/* SPR 17625 FIXED START */
#define GET_PARAM_ATTRIBUTE_PAYLOAD_SIZE 60000
/* SPR 17625 FIXED END */
#define MAX_BUFF_SIZE 255 

//Phase 2 Macros

/*CSR 57877 FIX */
/* Removed API */
#define ADD_OBJECT_BUFFER_LEN 276
#define UPLOAD_STR 128
#define SMALL_BUFFER_LEN 256
#define MEDIUM_BUFFER_LEN 512
#define UPL_FAIL_BUFFER 100

#define FAULT_CODE_9011 9011
#define FAULT_CODE_9011_STR "Upload Failure"
#define FAULT_CODE_9011_STR_LEN 14

#define SECGWS1 "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.SecGWServer1"
#define SECGWS2 "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.SecGWServer2"
#define SECGWS3 "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.SecGWServer3"
#define SECGWS_LEN 81
/* SPR 17880 START */
#define MANAGEMENT_SERVER_ENABLE_CWMP            "InternetGatewayDevice.ManagementServer.EnableCWMP"
#define MANAGEMENT_SERVER_PERIODIC_INFORMTIME    "InternetGatewayDevice.ManagementServer.PeriodicInformTime"
#define MANAGEMENT_SERVER_PARAMETER_KEY          "InternetGatewayDevice.ManagementServer.ParameterKey"
#define MANAGEMENT_SERVER_CWMP_RETRY_MINIMUM_WAIT_INTERVAL          "InternetGatewayDevice.ManagementServer.CWMPRetryMinimumWaitInterval"
#define MANAGEMENT_SERVER_CWMP_RETRY_INTERVAL_MULTIPLIER   "InternetGatewayDevice.ManagementServer.CWMPRetryIntervalMultiplier"
#define MANAGEMENT_SERVER_CONNECTION_REQUEST_USERNAME   "InternetGatewayDevice.ManagementServer.ConnectionRequestUsername"
#define MANAGEMENT_SERVER_CONNECTION_REQUEST_PASSWORD   "InternetGatewayDevice.ManagementServer.ConnectionRequestPassword"
#define MANAGEMENT_SERVER_PERIODIC_INFORM_INTERVAL        "InternetGatewayDevice.ManagementServer.PeriodicInformInterval"
#define MANAGEMENT_SERVER_PERIODIC_INFORM_ENABLE        "InternetGatewayDevice.ManagementServer.PeriodicInformEnable"
#define MANAGEMENT_SERVER_CONNECTION_REQUEST_URL        "InternetGatewayDevice.ManagementServer.ConnectionRequestURL"
#define MANAGEMENT_SERVER_SHEMS_USERNAME                "InternetGatewayDevice.ManagementServer.Username"
#define MANAGEMENT_SERVER_SHEMS_PASSWORD                "InternetGatewayDevice.ManagementServer.Password"
/* SPR 17880 END */
#define HEMSIPADDRESS "InternetGatewayDevice.ManagementServer.URL"
#define HEMSIPADDRESS_LEN 42
/*TLS CURL CHANGES*/
#define HEMS_FQDN       "InternetGatewayDevice.ManagementServer.X_VENDOR_IHEMS_FQDN"
#define IHEMS_URL_NAME "InternetGatewayDevice.ManagementServer.X_VENDOR_IHEMS_URL"
#define CONNECT_TO_IHEMS "InternetGatewayDevice.ManagementServer.X_VENDOR_CONNECT_TO_IHEMS"
#define LTE_SCANTIME "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.LastScanTime"

#define LTE_SCANTIME_PARENT "InternetGatewayDevice.Services.FAPService.[1].REM.LTE"
#define LTE_SCAN_TIME_LEN 53

#define LTE_RAN_PARENT "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.NeighborListInUse"
#define LTE_RAN_PARENT_LEN 82 



#define DEVICEINFO_MANU "InternetGatewayDevice.DeviceInfo.Manufacturer"
#define DEVICEINFO_OUI "InternetGatewayDevice.DeviceInfo.ManufacturerOUI"
#define DEVICEINFO_PROD_CLASS "InternetGatewayDevice.DeviceInfo.ProductClass"
#define DEVICEINFO_SRN_NO "InternetGatewayDevice.DeviceInfo.SerialNumber"
#define DEVICEINFO_HWD_VERS "InternetGatewayDevice.DeviceInfo.HardwareVersion"
#define DEVICEINFO_SF_VERS "InternetGatewayDevice.DeviceInfo.SoftwareVersion"
 /* SPR 16568 FIX START */
#define CONNECTION_REQ_URL "InternetGatewayDevice.ManagementServer.ConnectionRequestURL"
#define CONNECTION_REQ_SIZE 60 
 /* SPR 16568 FIX END */


#define NULL_IP_ADD "0.0.0.0"
#define NULL_IP_ADD_LEN 7

#define DEVICEINFO_MANU_LEN 45
#define DEVICEINFO_OUI_LEN 48 
#define DEVICEINFO_PROD_CLASS_LEN 45
#define DEVICEINFO_SRN_NO_LEN 45
#define DEVICEINFO_HWD_VERS_LEN 48 
#define DEVICEINFO_SF_VERS_LEN 48 


#define FIRM_UPGRADE '1'
#define VENDOR_CONFIG '3'

#define OPSTATE_INDX 0
#define RFTXSTATUS_INDX 1
#define LASTSCANTIME_INDX 2
#define UPDATETIME_INDX 3

#define LASTSCANTIME_STR "LastScanTime"
#define NRT_UPDATETIME_STR "X_VENDOR_LastNRTUpdateTime"

#define LASTSCANTIME_STR_LEN 12
#define RFTXSTATUS_STR_LEN 10
#define NRT_UPDATETIME_STR_LEN 26

#define UPD_DWN_URL "/Upgrade_url"
/* SPR 13658 START */
#define PAR_TYPE_STRING SOAP_TYPE_string
#define PAR_TYPE_SINT SOAP_TYPE_int
#define PAR_TYPE_UINT SOAP_TYPE_unsignedInt
#define PAR_TYPE_DATETIME SOAP_TYPE_time
#define PAR_TYPE_BOOLEAN SOAP_TYPE_xsd__boolean
/* SPR 13658 END */

typedef short Uint16;
//typedef unsigned int UInt32;
typedef char UINT8;
typedef int SInt32;

enum AddObjectResponse_Status {AddObjectResponse_Status__0 = 0, AddObjectResponse_Status__1 = 1};
/* cspl fix start */
/* cspl structure declaration */
/* SPR 22314 Start */
void tr069_free_parameterValueList(struct ParameterValueList *ptr);
#define TR069_FREE_PARAMMETERVALUELIST(PARAMVALUELIST) do\
    {\
        tr069_free_parameterValueList(PARAMVALUELIST);\
        PARAMVALUELIST = NULL;\
    }while(TR069_ZERO)

/* SPR 22314 End */
typedef	struct	qpool {
	unsigned long	magic;
	unsigned int	size, nbuf, alloc, peakVal;
	YLIST		chunks;
	YPOOL		list;

	QLOCK		lock;
} qpool;

typedef	struct	qbuf {
	union {
		YPNODE	__header__;
		qpool	*pool;
	} u;

	unsigned char	allocated;
	/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
	unsigned char *func;
	unsigned int   line;
#endif
	/*SPR_18125_END*/
#ifdef CSPL_LEAK_NONL2_DEBUG
	int nptrs;
	/* Max. No. of frames in Backtrace */
#define MAX_FRAMES_IN_BT 10
	void *buffer[MAX_FRAMES_IN_BT];
#endif
} qbuf;

/* cspl fix end */


/*SPR 17596 fixed start */
/*struct _cwmp__GetRPCMethodsResponse
{   
    struct MethodList *MethodList;  // required element of type ArrayOfstring /
}; 


struct MethodList
{   
    char **__ptrstring;
    int __size;
};
*/

/* SPR 17596 fixed end */


struct sParameterNames
{
        char **__ptrstring;
        int __size;
};


/* SPR 17625 FIXED START */
struct sParameterAttr
{
    char **__ptrstring;
    int __size;
};
/* SPR 17625 FIXED END */




/* SPR 17625 FIXED START */
typedef struct
{
        EStatus eStatus;
            Uint16 fault_code;
                UINT8  *fault_string;
}sSetParamAttrRes_t;
/* SPR 17625 FIXED END */



/* SPR 13658 START */
/* Code Removed */
/* SPR 13658 END */
/* Spr 16990 Fix Start */

typedef struct _cwmp__GetParameterNames GetParameterNames;
/* Spr 16990 Fix End */

struct sDownload
{
        char *CommandKey;       /* required element of type cwmp:CommandKeyType */
        char *FileType; /* required element of type cwmp:DownloadFileType */
        char *URL;      /* required element of type xsd:string */
        char *Username; /* required element of type xsd:string */
        char *Password; /* required element of type xsd:string */
        unsigned int FileSize;  /* required element of type xsd:unsignedInt */
        char *TargetFileName;   /* required element of type xsd:string */
        unsigned int DelaySeconds;      /* required element of type xsd:unsignedInt */
        char *SuccessURL;       /* required element of type xsd:string */
        char *FailureURL;       /* required element of type xsd:string */
};



enum sDownloadResponse_Status {DownloadResponse_Status__0 = 0, DownloadResponse_Status__1 = 1};

typedef struct SetParamAttrRes  SetParamAttrRes;


typedef struct
{
    char *ObjectName;   
    char *ParameterKey; 
}AddObject;


typedef struct  {
                EStatus eStatus;
                UInt16 instance_number;
                UInt16 fault_code;
                UInt8 * fault_string;
}sAddObjectStatus_t;

typedef struct
{
    UInt8 *url;
    UInt8 *user;
    UInt8 *pwd;
    UInt8 *dir_path;
}Upload_Dir;

typedef struct
{
    Char8 *CommandKey;   
    Char8 *FileType; 
    Char8 *URL;  
    Char8 *Username; 
    Char8 *Password; 
     UInt32 DelaySeconds; 
}sUpload;




void ipr_cwmp_urltoip(UInt8** url);
void ipr_cwmp_add_ips(config_res_t *ips);

/* SPR 13658 START */
#define MAX_PARENT_NAME_LENGTH                                        256
#define MAX_PARAMETER_NAME_LENGTH                                     128

typedef struct
{
    char          parameter_name[MAX_PARENT_NAME_LENGTH + MAX_PARAMETER_NAME_LENGTH];
    UInt8         parameter_type;
}parameter_type_mapping_table_t;
/* SPR 13658 END */

typedef struct  {
                EStatus eStatus;;
                Uint16 fault_code;
                UINT8 * fault_string;
}sDeleteObjectStatus_t;

typedef struct  {
                EStatus eStatus;
                Uint16 fault_code;
                UINT8 * fault_string;
}sControl_CmdStatus_t;

/* SPR 13658 START */  
int ipr_cwmp_find_parameter_type(char *param_name);

parameter_type_mapping_table_t g_parameter_lte_l3_sctp_list[] =
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Enable",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.HBInterval",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.MaxPathRetransmits",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.MaxAssociationRetransmits",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.RTOInitial",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.RTOMax",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.RTOMin",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.ValCookieLife",
        PAR_TYPE_UINT,
    }

};
parameter_type_mapping_table_t g_parameter_lte_l3_Assoc_list[] =
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc.Status",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc.PrimaryPeerAddress",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc.LocalPort",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc.InStreams",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc.OutStreams",
        PAR_TYPE_STRING,

    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Transport.SCTP.Assoc.StartTime",
        PAR_TYPE_STRING,

    }



};

parameter_type_mapping_table_t g_param_intra_rat_neigh_list_in_use[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.PhyCellID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.CID",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.QOffset",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.EUTRACarrierARFCN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_TAC",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_NUM_PLMN_ID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_PLMN_ID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_CSGID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.Blacklisted",
        PAR_TYPE_BOOLEAN,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_RSRP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_RSRQ",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_NR_STATUS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECell.X_VENDOR_SUSPECTED_PCI",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_param_inter_rat_neigh_list_in_use[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.LAC",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.CID",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.RAC",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.PCPICHScramblingCode",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.RNCID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_CELL_SPECIFIC_OFFSET",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_NUM_PLMN_ID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_PLMN_ID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_CSGID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.UARFCNDL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_RSCP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_ECNO",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_NR_STATUS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTS.X_VENDOR_HO_STATUS",
        PAR_TYPE_STRING,
    }
};


parameter_type_mapping_table_t g_parameter_lte_intra_rat_neighbor_list[] = 
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.PLMNID",
        PAR_TYPE_STRING,
    },    
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.CID",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.EUTRACarrierARFCN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."	
            "NeighborList.LTECell.PhyCellID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.QOffset",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.CIO",
        PAR_TYPE_SINT,
    },
   
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.Blacklisted",
        PAR_TYPE_BOOLEAN,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."	
            "NeighborList.LTECell.X_VENDOR_NUMPLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_PLMN_ID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_RSRP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."	
            "NeighborList.LTECell.X_VENDOR_CSGID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_NR_STATUS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_RSRQ",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_TAC",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."	
            "NeighborList.LTECell.X_VENDOR_SUSPECTED_PCI",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_BROADCAST_STATUS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_ACCESS_MODE",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_PRACH_HIGH_SPEED_FLAG",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_PRACH_FREQUENCY_OFFSET",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.X_VENDOR_PRACH_CONFIGURATION_INDEX",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_lte_inter_rat_neighbor_list[] =
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.RNCID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.CID",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.LAC",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.RAC",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.PCPICHScramblingCode",
        PAR_TYPE_STRING,
    }, 
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_RSCP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_CELL_SPECIFIC_OFFSET",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_NUMPLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_PLMN_ID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_CSGID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_ECNO",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_NR_STATUS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_HO_STATUS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_PS_HO_SUPPORTED",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_VOIP_CAPABLE",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.UARFCNDL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.X_VENDOR_ACCESS_MODE",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_TR_InterFreq_Carrier_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.EUTRACarrierARFCN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.QOffsetFreq",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.QRxLevMinSIB5",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.PMax",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.TReselectionEUTRA",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN." 
            "Mobility.IdleMode.InterFreq.Carrier.TReselectionEUTRASFMedium",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.TReselectionEUTRASFHigh",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.ThreshXHigh",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.ThreshXLow",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.CellReselectionPriority",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_Q_QUAL_MIN_R9",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_THRESHX_HIGHQ_R9",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_CM_OFFSET_FREQ",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_THRESHX_LOWQ_R9",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_OPEN_PHY_CELLID_START",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_OPEN_PHY_CELLID_RANGE",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_CSG_PHY_CELLID_START",
        PAR_TYPE_STRING,
    },

    { 
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_CSG_PHY_CELLID_RANGE",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_HYBRID_PHY_CELLID_START",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.X_VENDOR_HYBRID_PHY_CELLID_RANGE",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_TR_UTRA_FDD_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.UTRACarrierARFCN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.X_VENDOR_OFFSET_FREQUENCY",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.CellReselectionPriority",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.ThreshXHigh",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.ThreshXLow",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.QRxLevMin",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.PMaxUTRA",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.QQualMin",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq."
            "X_VENDOR_THRESHX_HIGHQ_R9",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq.X_VENDOR_THRESHX_LOWQ_R9",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_UTRA_TDD_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.UTRA.IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS."
        "UTRA_CARRIER_ARFCN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.UTRA.IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS."
        "CELL_RESELECTION_PRIORITY",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.UTRA.IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS."
        "THRESH_X_HIGH",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.UTRA.IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS."
        "THRESH_X_LOW",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.UTRA.IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS."
        "Q_RX_LEV_MIN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.UTRA.IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS."
        "P_MAX_UTRA",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_TR_UTRA_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreqNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.TReselectionUTRA",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.TReselectionUTRASFMedium",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.UTRA.TReselectionUTRASFHigh",
        PAR_TYPE_STRING,
    }
};


parameter_type_mapping_table_t g_parameter_son_nw_scan_req[] = 
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.InServiceHandling",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanOnBoot",
        PAR_TYPE_BOOLEAN,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanPeriodically",
        PAR_TYPE_BOOLEAN,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.PeriodicInterval",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.PeriodicTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.EUTRACarrierARFCNDLList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanTimeout",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.X_VENDOR_MEASUREMENT_PERIOD",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.X_VENDOR_RETRY_COUNT",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.REMBandList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.REMPLMNList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.X_VENDOR_MEAS_BANDWIDTH",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.X_VENDOR_PCI_LIST",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.REMPLMNList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.CarrierARFCNDL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.CarrierChWidth",
        PAR_TYPE_STRING,
    }    
};

parameter_type_mapping_table_t g_parameter_son_nw_scan_status[] =
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ScanStatus",
        PAR_TYPE_STRING,

    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.ErrorDetails",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.LastScanTime",
        PAR_TYPE_STRING,
    },   
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.MaxCellEntries",
        PAR_TYPE_UINT,
    },   
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.CellNumberOfEntries",
        PAR_TYPE_UINT,
    }, 
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.MaxCarrierMeasEntries",
        PAR_TYPE_STRING,
    },
     {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.CarrierMeasNumberOfEntries",
        PAR_TYPE_UINT,
    }

};
parameter_type_mapping_table_t g_parameter_son_nw_scan_results_rf []= 
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF.EUTRACarrierARFCN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF.RSSI",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF.PhyCellID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF.RSRP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF.RSRQ",
        PAR_TYPE_STRING,
    }
};
parameter_type_mapping_table_t g_parameter_pm_gpv_spv_param [] =
{
    {
        "InternetGatewayDevice.FAP.PerfMgmt.ConfigNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.X_VENDOR_FileWriteTimer",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.X_VENDOR_SamplingInterval",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.Enable",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.PeriodicUploadInterval",
        PAR_TYPE_UINT,
    }, 
    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.PeriodicUploadTime",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.URL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.Username",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.PerfMgmt.Config.Password",
        PAR_TYPE_STRING,
    }
};
parameter_type_mapping_table_t g_parameter_son_rem_lte_carrierMeas[] =
{
 {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE."
            "CarrierMeas.CarrierARFCNDL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE."
            "CarrierMeas.CarrierChWidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE."
            "CarrierMeas.CarrierRSSI",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_rach_opt_config_params [] =
{
    {
        "RACHOPT_REPORT_PERIODICITY",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_MIN_UE_RACH_REPORT_COUNT",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_START_THRESHOLD",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_STOP_THRESHOLD",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_DEGRADE_STOP_OFFSET",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_CONTENTION_THRESHOLD",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_POWER_RAMPUP_THRESHOLD",
        PAR_TYPE_STRING,
    },		

    {
        "RACHOPT_MIN_CFRA_PREAMBLES",
        PAR_TYPE_STRING,
    }		


};




parameter_type_mapping_table_t g_parameter_son_nw_scan_results_bcch_plmnlist[] =
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.PLMNList.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.PLMNList.CellReservedForOperatorUse",
        PAR_TYPE_SINT,
    },
};

parameter_type_mapping_table_t g_parameter_son_nw_scan_results_bcch [] =
{
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.DLBandwidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.ULBandwidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.RSTxPower",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.TAC",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.CellID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.CellBarred",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.CSGIndication",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.CSGIdentity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.MaxPLMNListEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH.PLMNListNumberOfEntries",
        PAR_TYPE_UINT,
    }
};

parameter_type_mapping_table_t g_parameter_TR_QOS_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "QoS.QCI",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "QoS.Type",
         PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "QoS.PacketDelayBudget",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "QoS.Priority",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "QoS.X_VENDOR_PACKETERRORLOSSRATE",
        PAR_TYPE_STRING,
    }

};

parameter_type_mapping_table_t g_parameter_TR_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.LTECellNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.MaxLTECellEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.UMTSNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborListInUse.InterRATCell.MaxUMTSEntries",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECellNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.MaxLTECellEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.ACTION",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTSNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.MaxUMTSEntries",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.InterRATCell.UMTS.ACTION",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1].RF."
            "EUTRACarrierARFCN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1].RF."
            "RSRP",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1].RF."
            "RSRQ",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1].RF."
            "RSSI",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.RSTxPower",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.TAC",
        PAR_TYPE_UINT,
    },      
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.CellID",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.CellBarred",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1].BCCH."
            "MaxPLMNListEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.PLMNListNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.DLBandwidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceSummary",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Time.Enable",
        PAR_TYPE_BOOLEAN,
    },
    {
        "InternetGatewayDevice.Time.Status",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Time.NTPServer1",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.Manufacturer",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.ManufacturerOUI",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.ModelName",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.Description",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.ProductClass",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.SerialNumber",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.HardwareVersion",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.SoftwareVersion",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.ModemFirmwareVersion",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.AdditionalSoftwareVersion",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.DeviceInfo.UpTime",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.DeviceInfo.FirstUseDate",
        PAR_TYPE_STRING,
    },
    {
        "Config.SaveConfig",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.VirtualInterfaceNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfileNumberOfEntries",
        PAR_TYPE_UINT,
    },

    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].AuthMethod",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IPsecRekeyLifetimeTime",
        PAR_TYPE_SINT,
    },

    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IKERekeylifetime",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].Enable",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].DPDTimer",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].NATTKeepaliveTimer",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.IKESA.PeerAddress",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].DeviceType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].DNPrefix",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl."
            "SelfConfigEvents",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl."
            "EnclosureTamperingDetected",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE."
            "Gateway.S1SigLinkServerList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE."
            "Gateway.S1ConnectionMode",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE."
            "TunnelNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE."
            "Tunnel.Enable",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE."
            "Tunnel.TunnelRef",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE."
            "Tunnel.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE."
            "CDMA2000RxBandsSupported",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE."
            "GSMRxBandsSupported",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE."
            "UMTSRxBandsSupported",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService[1].FAPControl.LTE."
            "Gateway.X_VENDOR_HENBIPADDRESS",
        PAR_TYPE_STRING,	
    },
    {
        "InternetGatewayDevice.WANDevice.[1].WANEthernetInterfaceConfig."
            "MACAddress",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPServiceNumberOfEntries",
        PAR_TYPE_UINT,
    },



    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PUSCH.ULRS.GroupAssignmentPUSCH",
        PAR_TYPE_STRING,
    },
    {      
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PUSCH.ULRS.CyclicShift",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.ULPowerControl.DeltaFPUCCHFormat1",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.ULPowerControl.DeltaFPUCCHFormat1b",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.ULPowerControl.DeltaFPUCCHFormat2",
        PAR_TYPE_SINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.ULPowerControl.DeltaFPUCCHFormat2a",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.ULPowerControl.DeltaFPUCCHFormat2b",
        PAR_TYPE_SINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.ULPowerControl.DeltaPreambleMsg3",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.AntennaInfo.AntennaPortsCount",
        PAR_TYPE_UINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.MBSFN.NeighCellConfig",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "S1AP.TRelocPrep",
        PAR_TYPE_UINT,    
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.DRX.OnDurationTimer",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].AccessMgmt.LTE.AccessMode",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].AccessMgmt.LTE.MaxUEsServed",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].AccessMgmt.LTE.CSGID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].AccessMgmt.LTE."
            "MaxCSGMembers",
        PAR_TYPE_STRING,  
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].AccessMgmt.LTE."
            "MaxNonCSGMembers",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RF.PBCHPowerOffset",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRS.PRSConfigurationIndex",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRS.NumConsecutivePRSSubfames",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
         "PHY.PRS.X_VENDOR_PRS_ENABLE",
                PAR_TYPE_STRING,
        },
        {
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
                        "PHY.PRS.X_VENDOR_MUTING_CONFIG_TWO",
                PAR_TYPE_STRING,
        },
         {
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
                        "PHY.PRS.X_VENDOR_MUTING_CONFIG_FOUR",
                PAR_TYPE_STRING,
        },
        {
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
                        "PHY.PRS.X_VENDOR_MUTING_CONFIG_EIGHT",
                PAR_TYPE_STRING,
        },
        {
                "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
                        "PHY.PRS.X_VENDOR_MUTING_CONFIG_SIXTEEN",
                PAR_TYPE_STRING,
        },
        {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_PRS_TRANSMISSION_POWER",
	   PAR_TYPE_STRING,
          },
        {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_LATITUDE_SIGN",
          PAR_TYPE_STRING,
          },
        {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_DEG_OF_LATITUDE",
          PAR_TYPE_STRING,
         }, 
        {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_DEG_OF_LONGITUDE",
	  PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_DIR_OF_ALTITUDE",
          PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_ALTITUDE",
          PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_UNCERTAINTY_SEMI_MAJOR",
          PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_UNCERTAINTY_SEMI_MINOR",
          PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_ORIENTATION_OF_MAJOR_AXIS",
          PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_UNCERTAINTY_ALTITUDE",
          PAR_TYPE_STRING,
         }, 
         {
          "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
          "PHY.PRS.X_VENDOR_CONFIDENCE",
          PAR_TYPE_STRING,
         }, 
         {
             "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
                 "RF.PSCHPowerOffset",
             PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RF.SSCHPowerOffset",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "S1AP.TRelocOverall",
        PAR_TYPE_UINT, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRS.NumConsecutivePRSSubframes",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRS.NumPRSResourceBlocks",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.DRX.DRXInactivityTimer",             
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.DRX.DRXRetransmissionTimer",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "EAID",
        PAR_TYPE_STRING, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "QoSNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "MaxPLMNListEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "AllowedCipheringAlgorithmList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "AllowedIntegrityProtectionAlgorithmList",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].AccessMgmt.LTE."
            "HNBName",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RF.DLBandwidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.QRxLevMinSIB1",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.QRxLevMinSIB3",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.SIntraSearch",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.SNonIntraSearch",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN." 
            "MAC.DRX.ShortDRXCycle",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.Common.TEvaluation",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.Common.THystNormal",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RLC.SRB1.TPollRetransmit",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RLC.SRB2.TPollRetransmit",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RLC.SRB1.TReordering",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RLC.SRB2.TReordering",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.TReselectionEUTRA",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.TReselectionEUTRASFHigh",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.TReselectionEUTRASFMedium",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RLC.SRB1.TStatusProhibit",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RLC.SRB2.TStatusProhibit",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC."
            "TAC",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Common.CellIdentity",
        PAR_TYPE_UINT,            
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "CellRestriction.BarringForEmergency",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "CellRestriction.IntraFreqCellReselectionIndicator",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "CellRestriction.CellBarred",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "BCCH.DLBandwidth",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.[1]."
            "RF.PhyCellID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IntraFreq.QRxLevMinOffset",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.InterFreq.Carrier.Pmax",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RF.FreqBandIndicator",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.NumberOfRaPreambles",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.SizeOfRaGroupA",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.MessageSizeGroupA",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.MessagePowerOffsetGroupB",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.PowerRampingStep",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.PreambleInitialReceivedTargetPower",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.PreambleTransMax",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.ResponseWindowSize",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.ContentionResolutionTimer",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "MAC.RACH.MaxHARQMsg3Tx",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRACH.RootSequenceIndex",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRACH.ConfigurationIndex",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRACH.HighSpeedFlag",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRACH.ZeroCorrelationZoneConfig",
        PAR_TYPE_UINT, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PRACH.FreqOffset",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "RF.ReferenceSignalPower",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PDSCH.Pb",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PUSCH.NSB",
        PAR_TYPE_UINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUSCH.HoppingMode",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUSCH.HoppingOffset",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "PHY.PUSCH.Enable64QAM",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUSCH.ULRS.GroupHoppingEnabled",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUSCH.ULRS.SequenceHoppingEnabled",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.DeltaPUCCHShift",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.NCSAN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.NRBCQI",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.N1PUCCHAN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.SRS.SRSBandwidthConfig",
        PAR_TYPE_STRING,   
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.SRS.AckNackSRSSimultaneousTransmission",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.SRS.SRSSubframeConfig",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.SRS.SRSMaxUpPTS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.ULPowerControl.P0NominalPUSCH",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.ULPowerControl.Alpha",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.ULPowerControl.P0NominalPUCCH",
        PAR_TYPE_SINT, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.TDDFrame.SubFrameAssignment",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.TDDFrame.SpecialSubframePatterns",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T300",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T301",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T310",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.N310",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T311",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.N311",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T302",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T304EUTRA",
  		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T304IRAT",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RRCTimers.T320",
		PAR_TYPE_UINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RF.ULBandwidth",
  		PAR_TYPE_STRING,
    }, 
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PDSCH.Pa",
  		PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.SRS.SRSEnabled",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.CQIPUCCHResourceIndex",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.TDDAckNackFeedbackMode",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.PHY.PUCCH.PUCCHCqiSinrValue",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.MAC.DRX.DRXEnabled",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.Common.Qhyst",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.Common.QHystSFMedium",
		PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.Common.QHystSFHigh",
		PAR_TYPE_SINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq.CellReselectionPriority",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.MAC.DRX.DRXShortCycleTimer",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.MAC.DRX.DRXStartOffset",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq.MaxCarrierEntries",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq.CarrierNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB1.DefaultConfiguration",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB2.DefaultConfiguration",
        PAR_TYPE_SINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.MAC.DRX.LongDRXCycle",
		PAR_TYPE_UINT,	
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB1.MaxRetxThreshold",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB2.MaxRetxThreshold",
		PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.Common.NCellChangeHigh",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.Common.NCellChangeMedium",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB1.PollByte",
		PAR_TYPE_STRING,

    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB2.PollByte",
		PAR_TYPE_STRING,

    },

    { 
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB1.PollPDU",
		PAR_TYPE_STRING,  
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RLC.SRB2.PollPDU",
		PAR_TYPE_STRING,  
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq.ThreshServingLow",
        PAR_TYPE_UINT, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.IdleMode.IntraFreq.PMax",
        PAR_TYPE_SINT, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.SupportedSystems",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.GPSEquipped",
        PAR_TYPE_BOOLEAN,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.MaxTxPower",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.Beacon",
        PAR_TYPE_BOOLEAN,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.PLMNListNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE.BandsSupported",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE.UMTSRxSupported",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE.GSMRxSupported",
        PAR_TYPE_SINT,
    },  
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE.CDMA2000RxSupported",
        PAR_TYPE_SINT,   
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE.DuplexMode",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].Capabilities.LTE.NNSFSupported",
        PAR_TYPE_SINT,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.SecGWServer1",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.SecGWServer2",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.SecGWServer3",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.Gateway.S1SigLinkPort",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RF.PhyCellID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RF.EARFCNDL",
        PAR_TYPE_STRING, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RF.EARFCNDL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RF.EARFCNUL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.RF.EARFCNUL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.AdminState",
        /* SPR 18816 FIXED START*/
        PAR_TYPE_STRING,   
        /* SPR 18816 FIXED END*/
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.OpState",
        /* SPR 18816 FIXED START*/
        PAR_TYPE_STRING,   
        /* SPR 18816 FIXED END*/

    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.RFTxStatus",
        /* SPR 18816 FIXED START*/
        PAR_TYPE_STRING,   
        /* SPR 18816 FIXED END*/
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].FAPControl.LTE.AvailabilityStatus",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.ManagementServer.URL",
        PAR_TYPE_STRING,
    }, 
    {
        "InternetGatewayDevice.ManagementServer.X_VENDOR_IHEMS_URL",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.FilterCoefficientRSRP",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.FilterCoefficientRSRQ",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A1ThresholdRSRP",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A1ThresholdRSRQ",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A2ThresholdRSRP",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A2ThresholdRSRQ",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A3Offset",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.ReportOnLeave",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A4ThresholdRSRP",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A4ThresholdRSRQ",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A5Threshold1RSRP",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A5Threshold1RSRQ",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A5Threshold2RSRP",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.A5Threshold2RSRQ",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.Hysteresis",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.TimeToTrigger",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.TriggerQuantity",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.ReportQuantity",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.ReportInterval",
		PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.ReportAmount",
	       PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.EUTRA.PsHoEnabled",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.QoffsettUTRA",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.FilterCoefficientUTRA",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.QoffsetCDMA2000",
        PAR_TYPE_STRING, 
    },
    {
        "LoadReporting.OverLoadLevel.Percentage",
        PAR_TYPE_STRING, 
    },
    {
        "LoadReporting.OverLoadLevel.Action",
        PAR_TYPE_STRING,
    },
    {
        "LoadReporting.HighLoadLevel.Percentage",
        PAR_TYPE_STRING,
    },
    {
        "LoadReporting.HighLoadLevel.Action",
        PAR_TYPE_STRING,
    },
    {
        "LoadReporting.MidLoadLevel.Percentage",
        PAR_TYPE_STRING,
    },
    {
        "LoadReporting.MidLoadLevel.Action",
        PAR_TYPE_STRING,
    },
    {
        "LoadReporting.serving_cell.ReportingInterval",
        PAR_TYPE_STRING,
    },
    {
        "LoadReporting.neighbor_cell.ReportingInterval",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.MeasQuantityCDMA2000",
        PAR_TYPE_STRING, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B1ThresholdCDMA2000",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.MeasQuantityUTRAFDD",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.MeasQuantityUTRATDD",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B1ThresholdUTRARSCP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B1ThresholdUTRAEcN0",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.QoffsetGERAN",
        PAR_TYPE_STRING, 
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.FilterCoefficientGERAN",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B1ThresholdGERAN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B2Threshold2UTRARSCP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B2Threshold2UTRAEcN0",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B2Threshold2GERAN",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B2Threshold2CDMA2000",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.Hysteresis",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.TimeToTrigger",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.MaxReportCells",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.ReportInterval",
		PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.ReportAmount",
		PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B2Threshold1RSRP",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN.Mobility.ConnMode.IRAT.B2Threshold1RSRQ",
        PAR_TYPE_STRING,
    },

    {
        "InternetGatewayDevice.ManagementServer.ConnectionRequestURL",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Time.CurrentLocalTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Time.LocalTimeZoneName",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.IKESANumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.MaxVirtualInterfaces",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarmNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEventNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEventNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEventNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.X_VENDOR_CurrentHistoryEventNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.X_VENDOR_CurrentExpeditedEventNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.X_VENDOR_CurrentQueuedEventNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.MaxCurrentAlarmEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.SupportedAlarmNumberOfEntries",
        PAR_TYPE_UINT,
    },


    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "NeighborList.LTECell.RSTxPower",
         PAR_TYPE_SINT,
    }



};

parameter_type_mapping_table_t g_parameter_FTP_Params[]={
    {
        "InternetGatewayDevice.FTPParams.enable",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FTPParams.FTPServerurl",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FTPParams.FTPServerUserName",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FTPParams.FTPServerpassword",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FTPParams.FilePath",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_multi_instanse_FM_SUPP_GET_SET[]={
    {
        "InternetGatewayDevice.FaultMgmt.SupportedAlarm.ReportingMechanism",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.SupportedAlarm.EventType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.SupportedAlarm.ProbableCause",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.SupportedAlarm.SpecificProblem",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.SupportedAlarm.PerceivedSeverity",
        PAR_TYPE_STRING,
    }
};


parameter_type_mapping_table_t g_parameter_multi_instanse_FM_CURR_GET_SET[]={
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.AlarmIdentifier",
        PAR_TYPE_UINT,
    },

    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.AlarmRaisedTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.AlarmChangedTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.ManagedObjectInstance",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.EventType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.ProbableCause",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.SpecificProblem",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.PerceivedSeverity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.AdditionalText",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.CurrentAlarm.AdditionalInformation",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_multi_instanse_FM_HIST_GET_SET[]={
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.EventTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.AlarmIdentifier",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.NotificationType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.ManagedObjectInstance",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.EventType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.ProbableCause",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.SpecificProblem",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.PerceivedSeverity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.AdditionalText",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.HistoryEvent.AdditionalInformation",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_multi_instanse_FM_EXPD_GET_SET[]={
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.EventTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.AlarmIdentifier",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.NotificationType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.ManagedObjectInstance",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.EventType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.ProbableCause",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.SpecificProblem",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.PerceivedSeverity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.AdditionalText",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.AdditionalInformation",
        PAR_TYPE_STRING,
    }
};
parameter_type_mapping_table_t g_parameter_multi_instanse_FM_QUED_GET_SET[]={
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.EventTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.AlarmIdentifier",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.NotificationType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.ManagedObjectInstance",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.EventType",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.ProbableCause",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.SpecificProblem",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.PerceivedSeverity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.AdditionalText",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FaultMgmt.QueuedEvent.AdditionalInformation",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_NONTR_element[] = {
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.phich_config.phich_duration",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].action",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].enb_type",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].plmn_id",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].enb_id",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].tac",
        PAR_TYPE_UINT,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].enb_tnl_address0",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].enb_tnl_address1",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].x2_status",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.oam_son_req.oam_son_nr_enb_add_req.nbr_enb_list[255].x2_connection_status",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.phich_config.phich_resource",
        PAR_TYPE_STRING 
    }, 
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.cell_info.eutran_global_cell_id.primary_plmn_id.mcc",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.cell_info.eutran_global_cell_id.primary_plmn_id.mnc",
        PAR_TYPE_STRING                
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.cell_info.cell_access_params.intraFreqReselection",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.additional_sib_2_info.modification_period_coeff",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.additional_sib_2_info.default_paging_cycle",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.additional_sib_2_info.nB",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.ran_info.physical_layer_param_srs.srs_subframe_config",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.additional_sib_2_info.ul_cyclic_prefix_length",
        PAR_TYPE_STRING 
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.additional_sib_2_info.additional_spectrum_emission",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.operator_specific_info.additional_sib_2_info.time_alignment_timer_common",
        PAR_TYPE_STRING 
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.ran_info.rlc_layer_params.num_valid_srb_info",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.ran_info.ncl_params.lte_ncl.intra_freq_cells.blacklisted",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.ran_info.ncl_params.lte_ncl.intra_freq_cells.cell_individual_offset",
        PAR_TYPE_STRING
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams.RRMParams.cellConfig.ran_info.rf_params.dl_earfcn",
        PAR_TYPE_STRING 
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams."
            "RRMParams.cellConfig.operator_info.measurement_configuration."
            "EUTRAN.gapOffsetGP0",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams."
            "RRMParams.cellConfig.operator_info.measurement_configuration."
            "EUTRAN.gapOffsetGP1",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams."
            "RRMParams.cellConfig.operator_info.measurement_configuration."
            "UTRAN.gapOffsetGP0",
        PAR_TYPE_STRING,
    },
    {
        "eNodeBParams.CellParams.Parameters_Cell1.RadioCtrlAndMgmtParams."
            "RRMParams.cellConfig.operator_info.measurement_configuration."
            "UTRAN.gapOffsetGP1",
        PAR_TYPE_STRING,
    },
    {   "Show.Version.VersionID",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_TR_PLMNList_element[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.PLMNList.Enable",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.PLMNList.IsPrimary",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.PLMNList.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.EPC.PLMNList.CellReservedForOperatorUse",
        PAR_TYPE_SINT,
    }
};


parameter_type_mapping_table_t g_parameter_tunnel_crypto_profile[] =
{
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].Enable",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].AuthMethod",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].MaxChildSA",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IKEEncrypt",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IKEPRF",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IKEIntegrity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IKEDH",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].ESPEncrypt",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].ESPIntegrity",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IPsecWindowSize",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IKERekeyLifetime",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IPsecRekeyLifetimeByte",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IPsecRekeyLifetimeTime",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].DPDTimer",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].NATTKeepaliveTimer",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].SecMaxFragSize",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].SecDFBit",
        PAR_TYPE_STRING,
    }
}; 

parameter_type_mapping_table_t g_parameter_tunnel_ikesa[] = 
{
    {
        "InternetGatewayDevice.FAP.Tunnel.IKESA.[1].Status",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.IKESA.[1].PeerAddress",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.IKESA.[1].CreationTime",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IPAddress",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].SubnetMask",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].DNSServers",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].DHCPServers",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].IntegrityErrors",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].OtherErrors",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.CryptoProfile.[1].AuthErrors",
        PAR_TYPE_STRING,
    }
};


parameter_type_mapping_table_t g_parameter_tunnel_virtualInterface[] =
{
    {
        "InternetGatewayDevice.FAP.Tunnel.VirtualInterface.[1].Enable",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.VirtualInterface.[1].DSCPMarkPolicy",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.FAP.Tunnel.VirtualInterface.[1].CryptoProfile",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t  g_parameter_lte_rem[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF.EUTRACarrierARFCN",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF."
            "RSRP",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF."
            "RSRQ",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.RF."
            "RSSI",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.RSTxPower",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.TAC",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.CellID",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.CellBarred",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell.BCCH."
            "MaxPLMNListEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.PLMNListNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.DLBandwidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.PLMNList.PLMNID",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.PLMNList.CellReservedForOperatorUse",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "CarrierMeas.[1].CarrierARFCNDL",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "CarrierMeas.[1].CarrierChWidth",
        PAR_TYPE_SINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "CarrierMeas.[1].CarrierRSSI",
        PAR_TYPE_SINT,
    }
};

parameter_type_mapping_table_t g_parameter_son_pci_reselction[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "BCCH.DLBandwidth",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].REM.LTE.Cell."
            "RF.PhyCellID",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_son_anr_geran_params[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.MaxGERANFreqGroupEntries",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroupNumberOfEntries",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.TReselectionGERAN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.TReselectionGERANSFMedium",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.TReselectionGERANSFHigh",
        PAR_TYPE_STRING,
    }
};

parameter_type_mapping_table_t g_parameter_son_anr_geranFDDFreq_params[] = {
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.X_VENDOR_P_MAX_GERAN",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.X_VENDOR_NCC_PERMITTED",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
            "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.QRxLevMin",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.CellReselectionPriority",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.ThreshXLow",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.ThreshXHigh",
        PAR_TYPE_UINT,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.BandIndicator",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.X_VENDOR_ARFCN_SPACING",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup."
        "X_VENDOR_NUM_OF_FOLLOWING_ARFCNS",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup."
        "X_VENDOR_EXPLICIT_ARFCN_LIST",
        PAR_TYPE_STRING,
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup."
        "X_VENDOR_ARFCN_BITMAP",
        PAR_TYPE_STRING
    },
    {
        "InternetGatewayDevice.Services.FAPService.[1].CellConfig.LTE.RAN."
        "Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup.BCCHARFCN",
        PAR_TYPE_STRING
    }
};
/* SPR 13658 END */  
/* Spr 16990 Fix Start */
void* handle_get_parameter_name_req(void * msg);
void * create_msg_to_oam_for_gpn(void * p_req ,UInt16 *msg_size);
UInt32 decode_gpn_response(UInt8 *buffer,
                         GetParameterNamesStatus_t *p_gpn_res_t,
                         UInt8* end_mark);
void free_gpn_resp_buffer (GetParameterNamesStatus_t *p_gpn_res_t);

typedef struct cwmp__ParameterInfoStruct ParameterInfoStruct;
/* Spr 16990 Fix Start */
/* Spr 17463 Fix Start */
UInt32 decode_gpv_response(UInt8 *buffer,
                         GetStatus *p_get_stat_t, 
                         UInt8 * end_mark);
/* Spr 17463 Fix End */
#endif
