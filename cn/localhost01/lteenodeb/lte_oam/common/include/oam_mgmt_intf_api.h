/* vim:ts=4:et:ai:ci:sw=4
*/
/*****************************************************************************
 * File Name   : oam_mgmt_intf_api.h
 *
 * Description : This file contains common API Definition required by 
 *               OAM and Managemet Interface
 *
 * Revision History:
 *
 * Date         Author           Reference              Comments
 * -----        ------           ---------              --------
 * March 2014   Vinod Vyas       CSR 57877     
 * June, 2014        Vivek K Gupta  Release 3.0         Changes for xml 
 *                                                      add/delete frwk code
 * Aug 2014     Aditi Singh     SPR 13116 
 *
 *
 * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 ****************************************************************************/

/*******************************************************************************
*                        MACROS
*******************************************************************************/
#ifndef OAM_MGMT_INTF_API_H
#define OAM_MGMT_INTF_API_H 

/*CLI */
#define OAM_CONFIG_KPI_MAC_REQ                                       11
#define OAM_CONFIG_KPI_MAC_RESP                                      12
#define OAM_CONFIG_KPI_PDCP_REQ                                      13
#define OAM_CONFIG_KPI_PDCP_RESP                                     14
#define OAM_GET_KPI_MAC_REQ                                          15
#define OAM_GET_KPI_MAC_RESP                                         16
#define OAM_GET_KPI_PDCP_REQ                                         17
#define OAM_GET_KPI_PDCP_RESP                                        18
#define OAM_GET_LOG_CAT_MAC_REQ                                      19
#define OAM_GET_LOG_CAT_MAC_RESP                                     20
#define OAM_GET_LOG_CAT_RLC_REQ                                      22
#define OAM_GET_LOG_CAT_RLC_RESP                                     23
#define OAM_GET_LOG_CAT_PDCP_REQ                                     24
#define OAM_GET_LOG_CAT_PDCP_RESP                                    25
#define OAM_GET_LOG_LEVEL_MAC_REQ                                    30
#define OAM_GET_LOG_LEVEL_MAC_RESP                                   31
#define OAM_GET_LOG_LEVEL_RLC_REQ                                    32
#define OAM_GET_LOG_LEVEL_RLC_RESP                                   34
#define OAM_GET_LOG_LEVEL_PDCP_REQ                                   35
#define OAM_GET_LOG_LEVEL_PDCP_RESP                                  36
/* SPR_13116_CHANGE_START */
#define OAM_GET_LOG_LEVEL_RRM_REQ                                    39
#define OAM_GET_LOG_LEVEL_RRM_RESP                                   41
#define OAM_GET_LOG_LEVEL_SON_REQ                                    42
#define OAM_GET_LOG_LEVEL_SON_RESP                                   43
#define OAM_GET_LOG_LEVEL_OAM_REQ                                    44
#define OAM_GET_LOG_LEVEL_RESP                                       45
/*SPR 13617 START*/
#define OAM_GET_LOG_LEVEL_L3_REQ                                     56
#define OAM_GET_LOG_LEVEL_L3_RESP                                    48
#define OAM_GET_LOG_LEVEL_L3_X2AP_REQ                                95
#define OAM_GET_LOG_LEVEL_L3_X2AP_RESP                               96
#define OAM_GET_LOG_LEVEL_L3_S1AP_REQ                                97
#define OAM_GET_LOG_LEVEL_L3_S1AP_RESP                               98
/*SPR 13617 END*/
/* SPR_13116_CHANGE_END */
#define OAM_SET_LOG_LEVEL_RRM_REQ                                    57
#define OAM_SET_LOG_LEVEL_RRM_RESP                                   58
#define OAM_SET_LOG_LEVEL_SON_REQ                                    59
#define OAM_SET_LOG_LEVEL_SON_RESP                                   60
#define OAM_SET_LOG_LEVEL_L3_RRC_REQ                                 55
#define OAM_SET_LOG_LEVEL_L3_RRC_RESP                                56
#define OAM_SET_LOG_LEVEL_L3_X2AP_REQ                                91
#define OAM_SET_LOG_LEVEL_L3_X2AP_RESP                               92
#define OAM_SET_LOG_LEVEL_L3_S1AP_REQ                                93
#define OAM_SET_LOG_LEVEL_L3_S1AP_RESP                               94
#define OAM_SET_CALL_TRACE_START                                     70
#define OAM_SET_CALL_TRACE_STOP                                      71
#define OAM_ENODEB_NOT_ACTIVE_STATE                                 134
#define OAM_SHUTDOWN_REQ                                            133
/*TR069 n SNMP */
#define TR069_OAM_INIT_IND                                          135
#define TR069_OAM_CONFIG_REQ                                        136
#define TR069_OAM_CONFIG_RESP                                       137
#define OAM_FILE_DOWNLOAD_IND                                       160
#define OAM_FILE_DOWNLOAD_IND_RESP                                  161 
#define OAM_CONTROL_COMMAND_REQ                                     162 
#define OAM_CONTROL_COMMAND_RESP                                    163 
#define TR069_CONTROL_COMMAND_REQ                                   164 
#define TR069_CONTROL_COMMAND_RESP                                  165

/*spr 17596 fix start */
#define OAM_GET_RPC_METHOD                                          260
#define OAM_GET_RPC_METHOD_RESP                                     270
#define MAX_NUM_OF_RPC                                              13
/*spr 17596 fix end */


/* SPR 17631 fixed start */
#define OAM_FILE_UPLOAD_IND                                         210
#define OAM_FILE_UPLOAD_IND_RESP                                    211
/* SPR 17631 fixed end */

#define OAM_SET_PARAMETER_VALUES                                    185 
#define OAM_SET_PARAMETER_VALUES_RESP                               186
#define OAM_GET_PARAMETER_VALUES                                    187
#define OAM_GET_PARAMETER_VALUES_RESP                               188
#define OAM_NOTIFICATION_IND                                        189 
#define OAM_NOTIFICATION_IND_RESP                                   190 
#define OAM_ADD_OBJECT_REQ                                          191
#define OAM_ADD_OBJECT_RESP                                         192
#define OAM_DELETE_OBJECT_REQ                                       193
#define OAM_DELETE_OBJECT_RESP                                      194
/* Spr 16990 Fix Start */
#define OAM_GET_PARAMETER_NAME_REQ                                  195
#define OAM_GET_PARAMETER_NAME_RESP                                 196
/* Spr 16990 Fix End */

/* Spr 17463 Fix Start */
#define  OAM_GET_PARAMETER_VALUES_RESP_ALL                          197 
/* Spr 17463 Fix End */


/* SPR 17625 FIXED START */
#define  OAM_GET_PARAMETER_ATTRIBUTES_RESP                          198 
#define  OAM_SET_PARAMETER_ATTRIBUTE_RESP                          199 
#define  OAM_GET_PARAMETER_ATTRIBUTES                          200 
#define  OAM_SET_PARAMETER_ATTRIBUTE                          201
/* SPR 17625 FIXED END */
/*SPR 17627 FIXED START*/
#define  TR069_OAM_HEARTBEAT_REQ                          204
#define  TR069_OAM_HEARTBEAT_RESP                          205
/*SPR 17627 FIXED END*/


#define OAM_GET_KPI_RRC_REQ                                         225
#define OAM_GET_KPI_RRC_RESP                                        226
/* Spr 16890 Fix Start */
#define TR069_HEMS_SESSION_CLOSE_IND                                166
/* Spr 16890 Fix End */

/* SPR 17880 START */
#define OAM_TR069_SET_PARAMETER_VALUE_INDICATION                    227
/* SPR 17880 START */


/* SPR 17717 fixed start */
#define OAM_ALARM_IND                                               168    
/*SPR 17717 fixed end */                                     
/*SDM*/
#define OAM_SDM_REQUEST_UPGRADE_START                               401
#define SDM_OAM_RESPONSE_UPGRADE_START                              402 
#define OAM_SDM_ACK_UPGRADE_START                                   403
#define OAM_SDM_ACTIVE_IND                                          404
#define SDM_OAM_POLLING_REQ                                         405
#define OAM_SDM_POLLING_RESP                                        406
#define OAM_SDM_CONTROL_COMMAND_REQ                                 407
/* SPR 21889 start */

#define OAM_AGENT_MODULE_MESSAGES                                   500
#define AGENT_OAM_INIT_IND                          OAM_AGENT_MODULE_MESSAGES + 1
#define OAM_AGENT_SPAWN_PROCESS_REQ                 OAM_AGENT_MODULE_MESSAGES + 2
#define AGENT_OAM_SPAWN_PROCESS_RESP                OAM_AGENT_MODULE_MESSAGES + 3
#define OAM_AGENT_KILL_PROCESS_REQ                  OAM_AGENT_MODULE_MESSAGES + 4
#define AGENT_OAM_KILL_PROCESS_RESP                 OAM_AGENT_MODULE_MESSAGES + 5
#define OAM_AGENT_GET_LOGS                          OAM_AGENT_MODULE_MESSAGES + 6
#define AGENT_OAM_GET_LOGS_RESP                         OAM_AGENT_MODULE_MESSAGES + 7
#define OAM_AGENT_GET_LOGS_CNF                      OAM_AGENT_MODULE_MESSAGES + 8
#define AGENT_OAM_IM_OK_IND                         OAM_AGENT_MODULE_MESSAGES + 9
#define OAM_AGENT_RESTART_IND                       OAM_AGENT_MODULE_MESSAGES + 10
/* SPR 21889 end */

/*PM*/
#define OAM_GET_L2_RLC_STATS_REQ         363
#define SHOW_STATS_L2_MAC                362
#define SHOW_STATS_L3_S1AP               371
#define SHOW_STATS_L2_PDCP               364
#define SHOW_STATS_L2_EGTPU              365
#define CONFIG_RRC_STATS_REQ             566
#define CONFIG_RRC_STATS_RESP            567
#define MAX_TRANSPORT_LAYER_ADDR_SIZE           20
#define TRACE_REF_LENGTH   6


#define OAM_MGMT_INTF_HEADER_SIZE  			12


/* SPR-16334 START */
typedef void oam_void_t;
/* SPR-16334 END */

/*******************************************************************************
*                           Enumrations
*******************************************************************************/
typedef enum
{
    NO_ERROR,
    OAM_MALLOC_FAIL,
    OAM_SEND_FAIL, 
    OAM_NULL_POINTER_ERR,
    OAM_INSUFFICIENT_BUFFER,
    OAM_MODULE_DISABLED,
    OAM_INVALID_REQ,
    OAM_PARSE_MSG_DMG,
    OAM_SET_PARAMETER_INPUT_INCORRECT,
    OAM_GET_PARAMETER_INPUT_INCORRECT,
    OAM_SET_PARAMETER_MODE_MISMATCH,
    OAM_TCB_ENTRY_FETCH_ERROR,
    OAM_PARAMETER_NOT_FOUND,
    OAM_INTERNAL_ERROR,
    /* SPR 11156 FIX START */
    OAM_INVALID_ARFCN_VALUE,
    OAM_INVALID_CELL_CONTEXT,
    /* SPR 11156 FIX END */
    OAM_RESOURCE_EXCCEED,
    OAM_PARAM_NAME_TOO_LONG,
    OAM_ATTEMPT_TO_DELETE_PRIM_PLMNID,
    OAM_FEATURE_NOT_SUPPORTED,
    OAM_FILE_OPERATION_ERR,
    OAM_ERR_DATA_EXISTS,
    /* SPR 14941 Changes Start */
	OAM_ERR_ATTEMPT_SET_FOR_NON_WRITABLE_PARAM_VAL,
    OAM_M2AP_MCE_LINK_DOWN,
    /* SPR 14941 Changes End */
    /* OAM BCOM Code Comment Changes Start */
	OAM_INVALID_UARFCN_VALUE,
	/*SPR 21478 Start*/
	OAM_CELL_DELETE_IN_PROGRESS,
		/*SPR 21478 End*/
    /* OAM BCOM Code Comment Changes End*/
    /* TDD Changes */
    OAM_MBMS_DISABLED_WRONG_TDD_CONFIG,
    OAM_MBMS_SIB2_IS_DISABLED,
    OAM_MBMS_DISABLED_WRONG_NEIGHCELLCONFIG
/** !!!!NOTE  !!!
 * Please update the following macro OAM_MAX_NUM_INTERNAL_ERR_CODE value to
 * the num entries in oam_error_code_et enumeration type
 */
} oam_error_code_et;


#define OAM_MAX_NUM_INTERNAL_ERR_CODE       28 /* SPR 14941 Change */
/* TDD Changes */

#ifdef OAM_SON_ENABLE
typedef enum _setson
{   
    OAM_SON_CRITICALLOG = LTE_LOG_CRITICAL,
    OAM_SON_ERRORLOG = LTE_LOG_ERROR,
    OAM_SON_WARNINGLOG = LTE_LOG_WARNING,
    OAM_SON_INFOLOG = LTE_LOG_INFO,
    OAM_SON_BRIEFLOG = LTE_LOG_BRIEF,
    OAM_SON_DETAILEDLOG = LTE_LOG_DETAILED,
    OAM_SON_DETAILEDALLLOG = LTE_LOG_DETAILEDALL
}setson;
#endif

typedef enum
{
  OAM_RRM_CRITICAL = LTE_LOG_CRITICAL,
  OAM_RRM_ERROR = LTE_LOG_ERROR,
  OAM_RRM_WARNING = LTE_LOG_WARNING,
  OAM_RRM_INFO = LTE_LOG_INFO,
  OAM_RRM_BRIEF = LTE_LOG_BRIEF,
  OAM_RRM_DETAILED = LTE_LOG_DETAILED,
  OAM_RRM_DETAILEDALL = LTE_LOG_DETAILEDALL
}oam_rrm_log_level_et;
#define OAM_RRM_NONE OAM_RRM_CRITICAL

typedef enum
{ 
    SYS_CRITICAL = LTE_LOG_CRITICAL,
    SYS_ERROR = LTE_LOG_ERROR,
    SYS_WARNING = LTE_LOG_WARNING,
    SYS_INFO = LTE_LOG_INFO,
    SYS_BRIEF = LTE_LOG_BRIEF,
    SYS_DETAILED = LTE_LOG_DETAILED,
    SYS_DETAILEDALL = LTE_LOG_DETAILEDALL
}oam_sys_log_level_et;

typedef enum _setloglevell3rrccommands
{
    OAM_RRC_CRITICALLOG = LTE_LOG_CRITICAL,
    OAM_RRC_ERRORLOG = LTE_LOG_ERROR,
    OAM_RRC_WARNINGLOG = LTE_LOG_WARNING,
    OAM_RRC_INFOLOG = LTE_LOG_INFO,
    OAM_RRC_BRIEFLOG = LTE_LOG_BRIEF,
    OAM_RRC_DETAILEDLOG = LTE_LOG_DETAILED,
    OAM_RRC_DETAILEDALLLOG = LTE_LOG_DETAILEDALL
}setloglevell3rrccommands;

typedef enum _setloglevelX2APcommands
{
    OAM_X2AP_CRITICALLOG = LTE_LOG_CRITICAL,
    OAM_X2AP_ERRORLOG = LTE_LOG_ERROR,
    OAM_X2AP_WARNINGLOG = LTE_LOG_WARNING,
    OAM_X2AP_INFOLOG = LTE_LOG_INFO,
    OAM_X2AP_BRIEFLOG = LTE_LOG_BRIEF,
    OAM_X2AP_DETAILEDLOG = LTE_LOG_DETAILED,
    OAM_X2AP_DETAILEDALLLOG = LTE_LOG_DETAILEDALL
}setloglevelX2APcommands;

typedef enum _setloglevelS1APcommands
{
    OAM_S1AP_CRITICALLOG = LTE_LOG_CRITICAL,
    OAM_S1AP_ERRORLOG = LTE_LOG_ERROR,
    OAM_S1AP_WARNINGLOG = LTE_LOG_WARNING,
    OAM_S1AP_INFOLOG = LTE_LOG_INFO,
    OAM_S1AP_BRIEFLOG = LTE_LOG_BRIEF,
    OAM_S1AP_DETAILEDLOG = LTE_LOG_DETAILED,
    OAM_S1AP_DETAILEDALLLOG = LTE_LOG_DETAILEDALL
}setloglevelS1APcommands;

typedef enum _setloglevell2maccommands
{       
    MAC_CRITICALLOG = LTE_L2_LOG_CRITICAL,
    MAC_ERRORLOG = LTE_L2_LOG_ERROR,
    MAC_WARNINGLOG = LTE_L2_LOG_WARNING,
    MAC_INFOLOG = LTE_L2_LOG_INFO,
    MAC_BRIEFLOG = LTE_L2_LOG_BRIEF,
    MAC_DETAILEDLOG = LTE_L2_LOG_DETAILED,
    MAC_DETAILEDTTILOG = LTE_L2_LOG_DETAILEDALL
}setloglevell2maccommands;

typedef enum _setloglevelallcommands
{
    MAC_FATALLOG_ALL = 1,
    MAC_ERRORLOG_ALL,
    MAC_WARNINGLOG_ALL, 
    MAC_INFOLOG_ALL,
    MAC_DEBUGLOG_ALL,
    PDCP_INVALIDLOG_ALL,
    PDCP_FATALLOG_ALL,
    PDCP_ERRORLOG_ALL,
    PDCP_WARNINGLOG_ALL,
    PDCP_INFOLOG_ALL,
    PDCP_DEBUGLOG_ALL,
    RLC_DISABLED_ALL,
    RLC_FATALLOG_ALL,
    RLC_ERRORLOG_ALL,
    RLC_WARNINGLOG_ALL,
    RLC_INFOLOG_ALL,
    RLC_DEBUGLOG_ALL,
    RRM_ERRORLOG_ALL,
    RRM_WARNINGLOG_ALL,
    RRM_BRIEFLOG_ALL,
    RRM_DETAILEDLOG_ALL,
    SON_ERRORLOG_ALL,
    SON_WARNINGLOG_ALL,
    SON_BRIEFLOG_ALL,
    SON_DETAILEDLOG_ALL,
    GTPU_INCOMING_TRACE_ALL,
    GTPU_OUTGOING_TRACE_ALL,
    GTPU_FSM_TRACE_ALL,
    GTPU_ERROR_TRACE_ALL
}setloglevelallcommands;

typedef enum __oam_resp_status_et{
    OAM_RESULT_SUCCESS,
    OAM_RESULT_FAILURE
} oam_resp_status_et;

/*******************************************************************************
*                            Structures
*******************************************************************************/

typedef struct oam_mac_log_category_t{
    UInt64 logCategory;
} oam_mac_log_category;

typedef struct oam_rlc_log_category_t 
{
    UInt64 logCategory;
} oam_rlc_log_category_t;

#ifdef OAM_SON_ENABLE
typedef struct _oam_son_set_log_level_req_t
{
    SInt32 module_id;    
    SInt32 log_level;   
}oam_son_set_log_level_req_t;    
#endif 

typedef struct _oam_rrm_set_log_level_req_t
{
        SInt32 module_id;  
        SInt32 log_level; 
}oam_rrm_set_log_level_req_t;

typedef struct _oam_rrm_enable_disable_log_config_t
{
   UInt32 bitmask;   
   SInt32 log_on_off;
   SInt32 log_level;
}oam_rrm_enable_disable_log_config_t;


typedef struct _oam_rrm_log_enable_disable_req_t
{
    SInt32 module_id;  
    oam_rrm_enable_disable_log_config_t   log_config; 
} oam_rrm_log_enable_disable_req_t;

typedef struct _oam_son_module_log_config_t
{       
    UInt32 bitmask;      
    SInt32 log_on_off;  
    /* SPR 19703 changes start */
    UInt8  log_level; 
    /* SPR 19703 changes end */
}oam_son_module_log_config_t;

typedef struct _oam_son_log_enable_disable_req_t
{
    SInt32 module_id; 
    oam_son_module_log_config_t log_configuration;
    SInt32 feature_id;
} oam_son_log_enable_disable_req_t;

typedef struct _oam_rrc_set_log_level_req_t
{
    UInt8                  log_level;
} oam_rrc_set_log_level_req_t;

typedef struct _oam_X2AP_set_log_level_req_t
{
    UInt8                  log_level;
} oam_X2AP_set_log_level_req_t;


typedef struct _oam_S1AP_set_log_level_req_t
{
    UInt8                  log_level;
} oam_S1AP_set_log_level_req_t;

typedef struct _oam_mac_disable_log_category_req_t{
    UInt64 logCategory;
}oam_mac_disable_log_category_req_t;

typedef struct OamRlcEnableLogCategoryReq_t
{
    UInt64 logCategory;
}Oam_LogCategory_Req_t;

typedef struct _calltracestop
{
    UInt8 cell_index;
} oam_call_trace_stop_t;

typedef struct _oam_rrc_trace_collection_entity_t
{
    UInt8     length; 

    UInt8     data[MAX_TRANSPORT_LAYER_ADDR_SIZE];

} oam_rrc_trace_collection_entity_t;

typedef struct _oam_rrc_cell_traffic_trace_start_t
{
    UInt8                  cell_index;

    UInt8                  interface_to_trace;

    UInt8                  trace_depth;

    oam_rrc_trace_collection_entity_t trace_collection_entity;
    
    UInt8    traceRef[TRACE_REF_LENGTH];

} oam_rrc_cell_traffic_trace_start_t; 

struct add_object_parameter
{                            
    int C_add;
    char  **V_add;           
    char  **E_add; 
    char  **T_add;
    void *arg_add;
};

typedef struct __oam_usr_intf_err_tbl_t {
    Char8 *err_str;
    oam_error_code_et internal_err_code;
    UInt16 usr_err_code;
} oam_usr_intf_err_tbl_t;

#endif /* OAM_MGMT_INTF_API_H */

