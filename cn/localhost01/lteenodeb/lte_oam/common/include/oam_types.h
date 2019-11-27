/* vim:ts=4:et:ai:ci:sw=4
*/
/*****************************************************************************
 * File Name   : oam_types.h
 *
 * Description : This file contains common typedefs required by OAM modules
 *              
 *
 * Revision History:
 *
 * Date           Author           Reference            Comments
 * -----          ------           ---------            --------
 * April,2012     Nitin Agarwal    OAM Design Document  Initial Version
 * October 2012   Vivek/Harsh                           Changes related to RRH
 * September,2013 Neeraj Kumar     OAM Design Doc       Changes related to HM
 * September,2013 Vinod Vyas       OAM Design Doc       Changes related to IPV6
 * September,2013 Nitin Vijay      OAM Design Doc       Changes related to RACH-OPT
 *  Nov, 2013        Vinod Vyas              CSR 00051709 Fix  
 *  April.2014    Vinod Vyas       CSR 57877 Fix                                  
 *  April, 2014    Dinesh Dua      CSR 00070293 Fix  
 * April, 2014     Ankita Jain     CSR 00058157 Fix 
 * May, 2014	   Avinash 		   SPR 11090 Fix
 * May, 2014       Ankita Jain     SPR 8158 Fix
 * June, 2014      Ankita Jain     SPR 11590 Fix
 * April 2014       Nitin/Vinod             Cell State handling for multicell 
 *  April.2014    Vinod Vyas       CSR 57877 Fix                                  
 *  April, 2014    Dinesh Dua      CSR 00070293 Fix  
 * April, 2014     Ankita Jain     CSR 00058157 Fix 
 * May,2014        Aditi Singh     UT_FIX_6.0_DEFECT_ID_02  rsrp and rsrq type
 *                                                          change to SInt16
 * June, 2014        Vivek K Gupta  Release 3.0         Changes for xml 
 *                                                      add/delete frwk code
 *
 *June 2014        Shalu SAgar      SPR 12225 FIX
 *July 2014        Ekta Jain      SPR 12001 FIX
 * July,2014     Aditi Singh        SPR 9620 Fix        Guard timer Added
* July,2014        Ankita Jain         SPR 9485 Fix       GU_GROUP_ID related enhancement
* Aug 2014          Aditi Singh         SPR 13065
* Aug 2014          Aditi Singh         SPR 13116
 *Oct 2014         Ankita Jain         SPR 13586 FIX
 * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef OAM_TYPES_H
#define OAM_TYPES_H

#include <stacklayer.h>
#include <lteTypes.h>
#include<alarm.h>
#include<libxml/parser.h>
#include<libxml/tree.h>
#include <oam_defines.h>
#include<curl/curl.h>

/*Layer 2*/

#ifndef OAM_COMPILE
#include <oam_compile.h>
#endif

#include <lteOamInterface.h>
#include <lteMacOAMInterface.h>
#include <lteRlcOamInterface.h>
/*SPR 17663 +*/
#include <ltePdcpOamInterface.h>
/*SPR 17663 -*/
#include <lteRelayOamInterface.h>
#include <ltePerfStats.h>

/*Layer 3*/
#include <rrc_oamh_intf.h>
#include <rrc_defines.h>
#include <rrc_oam_intf.h>
#include <rrc_x2apOam_intf.h>
#include <rrc_x2apCommon_intf.h>
#include <rrc_s1apOam_intf.h>
#include <rrc_s1apCommon_intf.h>
#ifdef LTE_EMBMS_SUPPORTED
#include "oam_embms_types.h"
#endif
/* LWA changes start*/
/* LWA changes end*/

/*RRM*/
#include "rrm_defines.h"
#include "rrm_oam_intf.h"
#include "rrm_api_types.h"
#include "rrm_oam_types.h"
#include "rrm_cell_rrmim_intf.h"
#include "rrm_utils.h"

#ifdef OAM_SON_ENABLE
/*SON*/
#include <son_defines.h>
#include <son_oam_intf.h>
#include <son_api_defines.h>
#include <son_pack_unpack_utils.h>
#endif

#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
/*RRH*/
#include<oam_rcm_if.h>
#endif
#endif


/// To remove compilation errors
#define MAX_ADDRESS_LENGTH           6
#define MAX_NUM_MY_ADDRESS           8
#define MAX_PORT_LENGTH            6

#define OAM_MAX_DIR_PATH 	512 /* maximum char for path for binaries transfer */
#define OAM_MAX_ADDRESS_LENGTH 64
#define OAM_MAX_STR_LEN 256
#define OAM_MAX_PEER_ADD 45
#define OAM_MAX_OUI_LENGTH      	 7
#define NRT_INFO_MAX_TIMESTAMP_LEN            21
#define OAM_SON_NW_SCAN_REQ_PCI_STR_LEN 256
#define OAM_SON_NW_SCAN_REQ_EARFCN_STR_LEN 1024 
#define OAM_SON_NW_SCAN_REQ_PLMN_STR_LEN 64 
#define OAM_SON_MAX_VALUE_EARFCN 65535
#define OAM_SON_NW_SCAN_MEAS_BW_VALUE 6
#define EGTPU_TRANSPORT_ADDR_IPV_4 4
#define EGTPU_TRANSPORT_ADDR_IPV_6 16
 /* CSR 00051709 Change Start*/
#define OAM_MAX_MME_LIST_LEN            640
 /* CSR 00051709 Change End*/
/* LWA changes start*/
/* LWA changes end*/

#define OAM_ASSERT(s) \
    if (s) {  } \
else { OAM_LOG(OAM,OAM_DETAILED,"\nASSERT FAILURE!"); \
}

#define OAM_NULL_CHECK(s) \
    if (s) { } \
else { OAM_LOG(OAM,OAM_DETAILED,"\nNULL CHECK FAILURE!");return OAM_FAILURE; \
}

#define TIME_UNIT_TO_MILLISEC          1000
#define MAC_KPI_BUFFER_LEN             200
#define OAM_PERCENTAGE_CONVERSION_VAL  100
/* SPR_20467_FIX_START */
#define OAM_RESP_NOT_RECEIVED          0xFF
#define OAM_ECU_SUCCESS_RECEIVED       1
#define OAM_ECU_FAILURE_RECEIVED       2
#define OAM_ECU_FAILURE_TIMER_EXPIRY   3
/* SPR_20467_FIX_END */
/* oam fm fix start  included header content */

typedef QTIMER oam_timer_t;

typedef enum
{
    XML_VALIDATION_FAILURE = 0,
    NMM_NETWORK_SCAN_FAILURE,
    PARAMETER_POPULATION_FAILURE,
    OAM_STARTUP_FAILURE,
    OPERATOR_INITIATED_SHUTDOWN,
    OAM_INTERNAL_FAILURE,
    OAM_HEALTH_CHECK_FAILURE
}oam_reset_reason_code_et;


typedef enum
{
    OAM_FM_STATE_UNACK_UNCLEAR,
    OAM_FM_STATE_ACK_UNCLEAR,
    OAM_FM_STATE_UNACK_CLEAR,
    OAM_FM_MAX_STATES
}oam_fm_states_et;

/* Cell State Change */
typedef enum
{
    OAM_CELL_ACTIVE     =             0x1,
    OAM_CELL_IDLE      =              0x2,
    OAM_CELL_STOP    =                0x4,
	/*SPR 21478 Start*/
	OAM_CELL_DELETE  =		      0X8	
	/*SPR 21478 End*/
} oam_cell_state_et;
/* Cell State Change */

typedef enum {
    CELL_CONFIG_RESP_RCVD = 0x1,
/* SPR 13024 FIX START */
    OAM_USER_TRIGGERED_STOP = 0x2,
    OAM_CELL_PCI_CONFLICT = 0x4,
    OAM_USER_TRIGGERED_START = 0x8
/* SPR 13024 FIX END */

} oam_cell_event_et;


typedef enum
{
    OAM_WF_INIT_RESP = 0,
    OAM_INIT_RESP_RECEIVED,
}oam_cell_status_et;
typedef enum
{
    COMMUNICATIONS_ALARM,
    PROCESSING_ERROR_ALARM,
    MEMORY_MISMATCH,
    CONFIGURATION_OR_CUSTOMIZATION_ERROR,
    ENVIRONMENTAL_ALARM,
    QUALITY_OF_SERVICE_ALARM,
    EQUIPMENT_ALARM,
    INTEGRITY_VIOLATION,
    OPERATIONAL_VIOLATION,
    PHYSICAL_VIOLATION,
    SECURITY_SERVICE_OF_MECHANISM_VIOLATION,
    TIME_DOMAIN_VIOLATION,
    NOTIFICATION_TYPE  
}alarm_event_type_et;

/* Probable Cause Enumeration */

typedef enum
{
    COMMUNICATIONS_RECEIVE_FAILURE=4,
    COMMUNICATIONS_TRANSMIT_FAILURE,
    OUT_OF_MEMORY=103,
    SOFTWARE_ERROR=106,
    COMMUNICATIN_SUBSYSTEM_FAILURE=158,
    SOFTWARE_PROGRAM_ERROR=212,
    MESSAGE_OUT_OF_SEQUENCE=236,
    CONNECTION_ESTABLISHMENT_ERROR=365,
    UNAVAILABLE
}alarm_probable_cause_et;

/* Enumeration for Severity */

typedef enum
{
    CRITICAL,
    MAJOR,
    MINOR,
    WARNING,
    INDETERMINATE,
    CLEARED
}FM_alarm_severity_et;


typedef enum
{
    NOTIFY_NEW_ALARM,
    NOTIFY_CHANGED_ALARM,
    NOTIFY_CLEARED_ALARM,
    NEW_NOTIFICATION
}alarm_notify_type_et;

/* Enumeration for Reporting Mechanism */
typedef enum
{
    EXPEDITED_ALARM,
    QUEUED_ALARM,
    LOGGED_ALARM,
    DISABLED_ALARM,
    REPORT_MECHANISM_UNDEFINED = -1
}alarm_report_mechanism_et;

typedef enum {
    OAM_FALSE = 0,
    OAM_TRUE
} oam_bool_et;

/* SPR 18930  FIX START */
typedef enum {
    FIRST_NR_ADD_NOT_REQD = 0,/* if There is no configured  nbr to sent to son */
    FIRST_NR_ADD_REQ_TO_BE_SENT, /* If NR add req is needs to be sent for configured nbr */
    FIRST_NR_ADD_REQ_SENT,/* If NR add req is sent for configured nbr */
    FIRST_NR_ADD_RESP_RECVD,/* If NR add resp is received for configured nbr */
} oam_first_nr_add_state_et;
/* SPR 18930  FIX END */
typedef struct __oam_cell_status_t {
    UInt8 cid;
    UInt32 cell_state; /*It is a bitmask taking values from enum oam_cell_state_et*/
    UInt32 cell_event;
    /* SPR 11243 FIx Start */
    UInt8 send_initialization_req; 
    /* SPR 11243 FIx End */
    UInt8 immediate_start_needed; 
}  oam_cell_status_t;



typedef struct __oam_cell_context_t {
	oam_cell_status_t cell_ctxt_status[OAM_MAX_CELL_SUPPORTED];
	UInt8 curr_cell_cid;
}  oam_cell_context_t;

typedef struct
{
    oam_reset_reason_code_et reset_reason_obj;
    char reason_code_msg[100];
}oam_reset_reason_msg_t;


typedef struct _oam_severity_t
{
    char* severity;
    UInt8 severity_id;
}oam_severity_t;

typedef struct _oam_report_t
{
    char* report_mechanism;
    UInt8 report_id;
}oam_report_t;

typedef struct _oam_event_t
{
    char* event_type;
    UInt8 event_type_id;
}oam_event_t;

typedef struct _oam_probable_t
{
    char* probable_cause;
    UInt16 probable_cause_id;
}oam_probable_t;


#define OAM_SPECIFIC_PROBLEM_SIZE 200
#define OAM_ALARM_DATE_TIME_MAX 100
#define OAM_ALARM_IDENTIFIER_SIZE 64
#define OAM_MANAGED_OBJECT_INSTANCE_SIZE 512
#define OAM_ADDITIONAL_TEXT_SIZE 256
#define OAM_ADDITIONAL_INFORMATION_SIZE 256
#define OAM_SUPPORTED_ALARM_MAX 200
#define OAM_HISTORY_ALARM_MAX 200
#define OAM_CURRENT_ALARM_MAX 100
#define OAM_EXPEDITED_ALARM_MAX 100
#define OAM_QUEUED_ALARM_MAX 100



/* FTP DEFINITIONS START*/
#define OAM_FTP_URL_NAME_SIZE                                        255   
#define OAM_FTP_SW_PACKAGE_NAME_SIZE                                 255
#define OAM_FTP_USERNAME_SIZE                                        255
#define OAM_FTP_PSWD_SIZE                                            255
#define OAM_LOCAL_PATH_NAME_SIZE                                     255
/* FTP DEFINITIONS END */

/*struct: oam_supported_alarm_info_t
 *  *brief : A structure that containing supported alarms params
 *   */
typedef struct _oam_supported_alarm_info_t
{
    UInt16 alarm_identifier;
    alarm_event_type_et event_type;
    alarm_probable_cause_et probable_cause;
    Char8 specific_problem[OAM_SPECIFIC_PROBLEM_SIZE];
    FM_alarm_severity_et perceived_severity;
    alarm_report_mechanism_et reporting_mechanism;
    Char8 event_type_str[OAM_ALARM_EVENT_TYPE_MAX_LEN];
    Char8 probable_cause_str[OAM_ALARM_PROB_CAUSE_STR_MAX_LEN];
    Char8 perceived_severity_str[OAM_ALARM_PERCEIVED_SEV_MAX_LEN];
    Char8 reporting_mechanism_str[OAM_ALARM_REPORTING_MECH_MAX_LEN];
    Char8 managed_object_instance[OAM_MANAGED_OBJECT_INSTANCE_SIZE];
}oam_supported_alarm_info_t;

/*struct: oam_expedited_alarm_info_t
 *  *brief : A structure that contains parameters required for expedited alarm list.
 *   */
typedef struct _oam_expedited_alarm_info_t
{
    Char8 event_time[OAM_ALARM_DATE_TIME_MAX];
    Char8 alarm_identifier[OAM_ALARM_IDENTIFIER_SIZE];
    alarm_notify_type_et notification_type;
    Char8 managed_object_instance[OAM_MANAGED_OBJECT_INSTANCE_SIZE];
    alarm_event_type_et event_type;
    alarm_probable_cause_et probable_cause;
    Char8 specific_problem[OAM_SPECIFIC_PROBLEM_SIZE];
    FM_alarm_severity_et perceived_severity;
    Char8 additional_text[OAM_ADDITIONAL_TEXT_SIZE];
    Char8 additional_information[OAM_ADDITIONAL_INFORMATION_SIZE];
    Char8 event_type_str[OAM_ALARM_EVENT_TYPE_MAX_LEN];
    Char8 probable_cause_str[OAM_ALARM_PROB_CAUSE_STR_MAX_LEN];
    Char8 perceived_severity_str[OAM_ALARM_PERCEIVED_SEV_MAX_LEN];
    Char8 reporting_mechanism_str[OAM_ALARM_REPORTING_MECH_MAX_LEN];
    Char8 notification_type_str[OAM_ALARM_NOTIF_TYPE_MAX_LEN];
    /* Spr 12880 Fix Start */
    UInt8 CellId; 
    /* Spr 12880 Fix End */
}oam_expedited_alarm_info_t;

/*struct: oam_queued_alarm_info_t
 *  *brief : A structure that contains parameters required for queued alarm list.
 *   */
typedef struct _oam_queued_alarm_info_t
{
    Char8 event_time[OAM_ALARM_DATE_TIME_MAX];
    Char8 alarm_identifier[OAM_ALARM_IDENTIFIER_SIZE];
    alarm_notify_type_et notification_type;
    Char8 managed_object_instance[OAM_MANAGED_OBJECT_INSTANCE_SIZE];
    alarm_event_type_et event_type;
    alarm_probable_cause_et probable_cause;
    Char8 specific_problem[OAM_SPECIFIC_PROBLEM_SIZE];
    FM_alarm_severity_et perceived_severity;
    Char8 additional_text[OAM_ADDITIONAL_TEXT_SIZE];
    Char8 additional_information[OAM_ADDITIONAL_INFORMATION_SIZE];
    Char8 event_type_str[OAM_ALARM_EVENT_TYPE_MAX_LEN];
    Char8 probable_cause_str[OAM_ALARM_PROB_CAUSE_STR_MAX_LEN];
    Char8 perceived_severity_str[OAM_ALARM_PERCEIVED_SEV_MAX_LEN];
    Char8 reporting_mechanism_str[OAM_ALARM_REPORTING_MECH_MAX_LEN];
    Char8 notification_type_str[OAM_ALARM_NOTIF_TYPE_MAX_LEN];
    /* Spr 12880 Fix Start */
    UInt8 CellId;
    /* Spr 12880 Fix End */
}oam_queued_alarm_info_t;

/*struct: oam_history_alarm_info_t
 *  *brief : A structure that contains parameters required for Alarm History
 *   */

typedef struct _oam_history_alarm_info_t
{
    Char8 event_time[OAM_ALARM_DATE_TIME_MAX];
    Char8 alarm_identifier[OAM_ALARM_IDENTIFIER_SIZE];
    alarm_notify_type_et notification_type;
    Char8 managed_object_instance[OAM_MANAGED_OBJECT_INSTANCE_SIZE];
    alarm_event_type_et event_type;
    alarm_probable_cause_et probable_cause;
    Char8 specific_problem[OAM_SPECIFIC_PROBLEM_SIZE];
    FM_alarm_severity_et perceived_severity;
    Char8 additional_text[OAM_ADDITIONAL_TEXT_SIZE];
    Char8 additional_information[OAM_ADDITIONAL_INFORMATION_SIZE];
    UInt16  alarm_list_type;
    Char8 event_type_str[OAM_ALARM_EVENT_TYPE_MAX_LEN];
    Char8 probable_cause_str[OAM_ALARM_PROB_CAUSE_STR_MAX_LEN];
    Char8 perceived_severity_str[OAM_ALARM_PERCEIVED_SEV_MAX_LEN];
    Char8 reporting_mechanism_str[OAM_ALARM_REPORTING_MECH_MAX_LEN];
    Char8 notification_type_str[OAM_ALARM_NOTIF_TYPE_MAX_LEN];
    /* Spr 12880 Fix Start */
    UInt8 CellId;
    /* Spr 12880 Fix End */
}oam_history_alarm_info_t;

/*struct:oam_current_alarm_info_t
 *  *brief :A structure that contains parameters required for
 *   *       Current Alarm.
 *    */

typedef struct _oam_current_alarm_info_t
{
    Char8  alarm_identifier[OAM_ALARM_IDENTIFIER_SIZE];
    int is_used;                   //  index is in used or not (for 1 used and for 0 unused).
    int is_cleared;                //used for getting information for alarm cleared or not.
    oam_fm_states_et alarm_state;     //used to get the alarm state.
    Char8 alarm_raised_time[OAM_ALARM_DATE_TIME_MAX];
    Char8 alarm_changed_time[OAM_ALARM_DATE_TIME_MAX];
    Char8 managed_object_instance[OAM_MANAGED_OBJECT_INSTANCE_SIZE];
    alarm_event_type_et event_type;
    alarm_probable_cause_et probable_cause;
    Char8 specific_problem[OAM_SPECIFIC_PROBLEM_SIZE];
    FM_alarm_severity_et perceived_severity;
    Char8 additional_text[OAM_ADDITIONAL_TEXT_SIZE];
    Char8 additional_information[OAM_ADDITIONAL_INFORMATION_SIZE];
    Char8 event_type_str[OAM_ALARM_EVENT_TYPE_MAX_LEN];
    Char8 probable_cause_str[OAM_ALARM_PROB_CAUSE_STR_MAX_LEN];
    Char8 perceived_severity_str[OAM_ALARM_PERCEIVED_SEV_MAX_LEN];
    Char8 reporting_mechanism_str[OAM_ALARM_REPORTING_MECH_MAX_LEN];
    /* Spr 12880 Fix Start */
    UInt8 CellId;
    /* Spr 12880 Fix End */
}oam_current_alarm_info_t;

/*struct:oam_fm_cntxt_t
 *  *brief: A structure that contains parameters required for FM FSM.
 *   */

typedef struct _oam_fm_cntxt_t
{
    UInt8 isRaiseConfigAlarm;
    UInt16 SupportedAlarmNumberOfEntries;
    UInt16 MaxCurrentAlarmEntries;
    UInt16 CurrentAlarmNumberOfEntries;
    UInt16 HistoryEventNumberOfEntries;
    UInt16 ExpeditedEventNumberOfEntries;
    UInt16 QueuedEventNumberOfEntries;
    UInt16 Current_HistoryEventNumberOfEntries;
    UInt16 Current_ExpeditedEventNumberOfEntries;
    UInt16 Current_QueuedEventNumberOfEntries;
    UInt16 Current_List_Index;
    UInt16 History_List_Index;
    UInt16 Queued_List_Index;
    UInt16 Expedited_List_Index;
    UInt8 isHistoryRolled;
    UInt8 isCurrentRolled;
    UInt8 isExpeditedRolled;
    UInt8 isQueuedRolled;
    UInt16 fm_query_idx;
    oam_supported_alarm_info_t supported_alarm_list[OAM_SUPPORTED_ALARM_MAX];
    oam_history_alarm_info_t   history_alarm_list[OAM_HISTORY_ALARM_MAX];
    oam_current_alarm_info_t   current_alarm_list[OAM_CURRENT_ALARM_MAX];
    oam_expedited_alarm_info_t expedited_alarm_list[OAM_EXPEDITED_ALARM_MAX];
    oam_queued_alarm_info_t    queued_alarm_list[OAM_QUEUED_ALARM_MAX];
}oam_fm_cntxt_t;



/* oam fm fix end */

/* Spr 15438 Fix Start*/
#define OAM_SON_MAX_EUTRAN_NEIGHBORS  32 
/* Spr 15612  Fix Start*/
#define OAM_SON_MAX_UTRAN_NEIGHBORS  32 
/* Spr 15612  Fix End*/

#define OAM_SON_NEIGHBOUR_ENABLE_PRESENT 0x01
#define OAM_SON_NEIGHBOUR_MI_PRESENT 0x02
typedef struct
{
    UInt32                   bitmask;

    UInt8                    enable;
    /*^ O, OAM_SON_NEIGHBOUR_ENABLE_PRESENT , H, 0, 1 ^*/
    UInt8                    must_include;
    /*^ O, OAM_SON_NEIGHBOUR_MI_PRESENT , H, 0, 1 ^*/
}oam_son_nbr_ctxt_t;
/* Spr 15438 Fix End*/

/* SPR 14651 START */
#define OAM_SON_MEAS_ENABLE_PRESENT 0x01
typedef struct
{
   UInt32                   bitmask;
   UInt8                    enable;
/*^ O, OAM_SON_MEAS_ENABLE_PRESENT , H, 0, 1 ^*/
}oam_son_meas_obj_enable_t;
/* SPR 14651 END */

typedef pid_t oam_pid_t;
typedef struct ifaddrs oam_ifaddrs_t;
typedef struct sockaddr_in oam_sockaddr_ipfv4;
typedef struct sockaddr_in6 oam_sockaddr_ipv6;


typedef UInt16      oam_length_t;
typedef UInt16      oam_tag_t;
typedef UInt16      oam_module_id_t;
typedef UInt16      oam_counter_t;

/*Layer 2*/
typedef MacInitReqParams               oam_mac_init_req_params_t;
typedef MacUEStatusReq                     oam_mac_ue_status_req_t;
typedef MacLogLevel_Req                oam_mac_log_level_req_t;
typedef MacSetLoglevelResp             oam_mac_log_level_resp_t;
typedef MacGetLogLevelResp             oam_mac_get_log_level_resp_t;
typedef MacEnableLogCategoryReq        oam_mac_enable_log_category_req_t;
typedef MacGetLogCategoryResp          oam_mac_get_log_category_req_t; 
/*CSR 57877 Code Removed*/
typedef MacModifyQosPamams             oam_mac_modify_req_params_t;
typedef MacConfigureKpiStatsReqParams  oam_mac_config_KPI_stats_req_t;
typedef MacGetKpiStatsReqParams        oam_mac_get_KPI_stats_req_t;

typedef MacInitRespParams              oam_mac_init_resp_params_t;
typedef MacQoSModifyparamsResp         oam_mac_modify_resp_t;
typedef MacKpiResp                     oam_mac_Kpi_resp_t;
typedef MacStatsResp                   oam_mac_stats_resp_t;
typedef MacResetStatsResp              oam_mac_reset_stats_resp_t;
typedef MacUEStatusResp                oam_mac_ue_status_resp_t;
typedef MacKpiStatsIndParams           oam_mac_KPI_stats_ind_t;
typedef MacKpiThpStatsIndParams        oam_mac_KPI_THP_stats_ind_t;
typedef MacConfigureKpiStatsResp       oam_mac_config_KPI_stats_conf_t;
typedef MacUeSinrTaResp                oam_mac_ue_sinr_ta_resp_t;
typedef MacInitCellReqParams           oam_mac_init_cell_req_params_t;
typedef RlcSCellInfo                   oam_rlc_s_cell_info_t;
typedef PdcpSecondaryCellInfo          oam_pdcp_s_cell_info_t;
/***************RLC*******************/
typedef RlcOamInitLayerReq oam_rlc_init_layer_req_t;
/***************RLC*******************/

/***************PDCP*******************/
typedef PdcpOamInitLayerReq            oam_pdcp_init_layer_req_t;
typedef PdcpStatusReq                  oam_pdcp_status_req_t;
typedef PdcpOamSetLogLevelReq          oam_pdcp_log_level_req_t;
typedef PdcpOamGetLogLevelResp         oam_pdcp_get_log_level_resp_t;
typedef PdcpOamGetLogCategoryResp      oam_pdcp_get_log_category_resp_t;
typedef PdcpOamDisableLogCategoryReq   oam_pdcp_disable_log_category_req_t;
typedef PdcpOamEnableLogCategoryReq    oam_pdcp_enable_log_category_req_t;
typedef PdcpConfigureKpiStatsReqParams oam_pdcp_config_KPI_stats_req_t;
typedef PdcpGetKpiStatsReqParams       oam_pdcp_get_KPI_stats_req_t;

typedef PdcpOamCnf                     oam_pdcp_cnf_t;
typedef PdcpStatsCnf                   oam_pdcp_stats_cnf_t;
typedef PdcpUeStatusCnf                oam_pdcp_ue_status_cnf_t;
typedef PdcpConfigureKpiStatsResp      oam_pdcp_config_KPI_stats_conf_t;
typedef PdcpKpiStatsIndParams          oam_pdcp_KPI_stats_ind_t;
typedef PdcpKpiThpStatsIndParams       oam_pdcp_KPI_THP_stats_ind_t;
/***************PDCP*******************/

/**************** merging 1.1 change ****************************/
#define MAX_LC_IDENTITY 8
#define MAX_PDCP_LC_ID 11

#define MAX_QCI 9
#define CRC_ACK   0
#define MOD_SCHEME 3
#define MAX_RANK_INDICATION 2
#define MAX_CQI_INDEX 16
#define MAX_CFI 4
#define MAX_SINR_DIFF_INDEX 8
#define MAX_TA_RANGE 12
#define MAX_UTIL_INDEX 100
#define MAX_SRB 3

#define OAM_MAX_MANUFAC_LENGTH  64
#define OAM_MAX_TIME_LENGTH     64
#define OAM_MAX_DESCRIPTION_LENGTH 256
#define OAM_MAX_URL_LENGTH 256
#define OAM_MAX_REP_MEC_LENGTH 256
#define OAM_MAX_PERF_URL_LENGTH 256
#define OAM_MAX_USERNAME 256
#define OAM_MAX_PASSWORD 256
#define OAM_MAX_PERF_URL_LENGTH 256
#define OAM_MAX_USERNAME_LENGTH 256
#define OAM_MAX_PASSWORD_LENGTH 256
#define OAM_MAX_MAC_LENGTH 64
#define OAM_STRING_BAND 16

/*SPR 21889 Start*/
enum 
{
    OAM_REMOTE_SPAWN_FAILURE,
    OAM_REMOTE_SPAWN_SUCCESS,
    OAM_REMOTE_SPAWN_NO_STATUS
}oam_remote_spawn_status_et;
/*SPR 21889 End */

typedef struct _oam_egtpu_num_rab_rec_t
{
    UInt32 init_size;
    UInt32 step_size;
    UInt32 max_size;
}oam_egtpu_num_rab_rec_t;
typedef rrc_oam_handover_stats_ind_t oam_rrc_handover_stats_ind_t;

typedef rrc_handover_stats_t oam_rrc_handover_stats_t;

typedef rrc_oam_meas_on_neigh_cell_ho_stats_list_t oam_rrc_meas_on_neigh_cell_ho_stats_list_t;


typedef rrm_oam_ready_for_cell_block_ind_t oam_rrm_ready_for_cell_block_ind_t;
typedef rrm_oam_ue_release_req_t oam_rrm_ue_release_req_t;
typedef rrm_oam_cell_ecn_capacity_enhance_req_t oam_rrm_cell_ecn_capacity_enhance_req_t;



typedef rrm_enb_id_t oam_rrm_enb_id_t;
typedef rrm_rnc_id_t oam_rrm_rnc_id_t;
typedef rrm_lai_info_t oam_rrm_lai_info_t;
typedef rrm_tai_info_t oam_rrm_tai_info_t;
typedef mnc_t oam_mnc_t;
typedef plmn_identity_t oam_plmn_id_t;
typedef eutra_globalEnb_ecgi_t oam_eutra_globalEnb_ecgi_t;
typedef rrm_target_id_t oam_rrm_target_id_t;
typedef rrc_oam_ho_IratOutSucc_cause_t oam_rrc_ho_IratOutSucc_cause_t;
typedef rrc_oam_ho_IratOutAtt_cause_t oam_rrc_ho_IratOutAtt_cause_t;
typedef rrc_oam_inter_rat_ho_stats_t oam_rrc_inter_rat_ho_stats_t;
typedef rrc_oam_drx_non_drx_ho_stats_t oam_rrc_drx_non_drx_ho_stats_t;
typedef rrc_oam_inter_freq_ho_stats_t oam_rrc_inter_freq_ho_stats_t;
typedef rrc_oam_intra_freq_ho_stats_t oam_rrc_intra_freq_ho_stats_t;
typedef rrc_oam_ho_OutSuccTarget_cause_t oam_rrc_ho_OutSuccTarget_cause_t;
typedef rrc_oam_ho_OutAttTarget_cause_t oam_rrc_ho_OutAttTarget_cause_t;     
typedef rrc_oam_meas_on_neigh_cell_ho_stats_t oam_rrc_meas_on_neigh_cell_ho_stats_t;
typedef rrc_oam_ho_InterEnbOutSucc_cause_t oam_rrc_ho_InterEnbOutSucc_cause_t;
typedef rrc_oam_ho_InterEnbOutAtt_cause_t oam_rrc_ho_InterEnbOutAtt_cause_t;
typedef rrc_oam_inter_enb_ho_stats_t oam_rrc_inter_enb_ho_stats_t;
typedef rrc_oam_ho_IntraEnbOutSucc_cause_t oam_rrc_ho_IntraEnbOutSucc_cause_t;
typedef rrc_oam_ho_IntraEnbOutAtt_cause_t oam_rrc_ho_IntraEnbOutAtt_cause_t;
typedef rrc_oam_intra_enb_ho_stats_t oam_rrc_intra_enb_ho_stats_t;

typedef rrm_cgi_info_t oam_rrm_cgi_info_t;

typedef rrc_oam_irat_mobility_stats_ind_t oam_rrc_irat_mobility_stats_ind_t;
typedef rrc_irat_mobility_stats_t oam_rrc_irat_mobility_stats_t;
typedef rrc_oam_ue_assoc_s1_conn_stats_ind_t oam_rrc_ue_assoc_s1_conn_stats_ind_t;
typedef rrc_ue_assoc_s1_conn_stats_t oam_rrc_ue_assoc_s1_conn_stats_t;
typedef rrc_oam_paging_stats_ind_t oam_rrc_paging_stats_ind_t;
typedef rrc_paging_stats_t oam_rrc_paging_stats_t;

typedef rrc_oam_rrc_connection_stats_ind_t oam_rrc_connection_stats_ind_t;
typedef rrc_connection_stats_t  oam_rrc_connection_stats_t;
typedef rrc_oam_erab_stats_ind_t oam_rrc_erab_stats_ind_t;
typedef rrc_erab_stats_t oam_rrc_erab_stats_t;



typedef rrc_oam_add_lgw_req_t oam_rrc_add_lgw_req_t;
typedef rrc_oam_add_lgw_resp_t oam_rrc_add_lgw_resp_t;
typedef rrc_oam_delete_lgw_req_t oam_rrc_delete_lgw_req_t;
typedef rrc_oam_delete_lgw_resp_t oam_rrc_delete_lgw_resp_t;


/********** Structure/Enum Declaration locally *********/
typedef struct
{
    UInt8 ind_bit_mask;
    UInt8 cli_ind_bit_mask;
    void  *p_ue_buf;
    void  *p_cell_buf;
}oam_mac_perf_ind_handler_t;


typedef struct
{
    UInt8 ind_bit_mask;
    UInt8 cli_ind_bit_mask;
    void  *p_ue_buf;
    void  *p_cell_buf;
}oam_pdcp_perf_ind_handler_t;


typedef enum
{
    PM_HTTP,
    PM_HTTPS
}oam_pm_url_et;

typedef struct _oam_management_server_t
{
    UInt8 URL[OAM_MAX_URL_LENGTH];
    UInt8 ConReqURL[OAM_MAX_URL_LENGTH];
}oam_management_server_t;

typedef struct _oam_fault_mgmt_t
{
    UInt8 ReportingMechanism[OAM_MAX_REP_MEC_LENGTH];
    UInt32 SupportedAlarmNumberOfEntries;
}oam_fault_mgmt_t;

typedef struct _oam_tunnel_virtual_inter_t
{
    UInt8 Enable;
    SInt8 DSCPMarkPolicy;
    UInt8 CryptoProfile[OAM_MAX_STR_LEN];
}oam_tunnel_virtual_inter_t;

typedef struct _oam_crypto_profile_t
{
    UInt8  Enable;
    UInt8  AuthMethod[OAM_MAX_STR_LEN];
    UInt32 IKERekeyLifetime;
    UInt32 IPsecRekeyLifetimeTime;
    UInt32 DPDTimer;
    UInt32 NATTKeepaliveTimer;
    /* SPR 17378 START */
    UInt8  MaxChildSA;
    UInt8  IKEEncrypt[OAM_MAX_STR_LEN];
    UInt8  IKEPRF[OAM_MAX_STR_LEN];
    UInt8  IKEIntegrity[OAM_MAX_STR_LEN];
    UInt8  IKEDH[OAM_MAX_STR_LEN];
    UInt8  ESPEncrypt[OAM_MAX_STR_LEN];
    UInt8  ESPIntegrity[OAM_MAX_STR_LEN];
    UInt32 IPsecWindowSize;
    UInt32 IPsecRekeyLifetimeByte;
    UInt32 SecMaxFragSize;
    UInt8  SecDFBit[OAM_MAX_STR_LEN];
    /* SPR 17378 END */
} oam_crypto_profile_t;

typedef struct _oam_ikesa_t
{
    /* OAM BCOM Code Comment Changes Start */
    UInt8 Status[OAM_MAX_STR_LEN];
    UInt8 PeerAddress[OAM_MAX_STR_LEN];
    UInt8 CreationTime[OAM_MAX_STR_LEN];
    UInt8 IPAddress[OAM_MAX_STR_LEN];
    UInt8 SubnetMask[OAM_MAX_STR_LEN];
    UInt8 DNSServers[OAM_MAX_STR_LEN];
    UInt8 DHCPServers[OAM_MAX_STR_LEN];
    UInt8 IntegrityErrors[OAM_MAX_STR_LEN];
    UInt8 OtherErrors[OAM_MAX_STR_LEN];
    UInt8 AuthErrors[OAM_MAX_STR_LEN];
    /* OAM BCOM Code Comment Changes End */
}oam_ikesa_t;

#define OAM_MAX_CRYPTO_PROFILE 1
#define OAM_MAX_IKESA 1
#define OAM_MAX_VIRTUAL_PARAMS 1


typedef struct _oam_tunnel_t
{
    oam_crypto_profile_t oam_crypto_profile[OAM_MAX_CRYPTO_PROFILE];
    oam_ikesa_t oam_ikesa[OAM_MAX_IKESA];
    UInt32 CryptoProfileNumberOfEntries;
    UInt32 VirtualInterfaceNumberOfEntries;
    UInt32 IKESANumberOfEntries;
    UInt32 MaxVirtualInterfaces;
    oam_tunnel_virtual_inter_t virtual_interface_params[OAM_MAX_VIRTUAL_PARAMS];
    /* OAM BCOM Code Comment Changes Start */
    UInt32 DPDTimer;
    UInt8 Enable;
    UInt32 IKERekeylifetime;
    UInt32 IPsecRekeyLifetimeTime;
    UInt8 AuthMethod[OAM_MAX_STR_LEN];
    /* OAM BCOM Code Comment Changes End */
}oam_tunnel_t;

typedef struct _oam_capabilities_t
{
    UInt8 UMTSRxBandsSupported[OAM_STRING_BAND];
    UInt8 GSMRxBandsSupported[OAM_MAX_STR_LEN];
    UInt8 CDMA2000RxBandsSupported[OAM_STRING_BAND];
}oam_capabilities_t;

typedef struct _oam_perf_mgmt_t
{
    UInt32 ConfigNumberOfEntries;
    UInt8 URL[OAM_MAX_PERF_URL_LENGTH];
    UInt8 Username[OAM_MAX_USERNAME_LENGTH];
    UInt8 Password[OAM_MAX_PASSWORD_LENGTH];
    UInt32 Enable;
    UInt32 PeriodicUploadInterval;
    UInt8 PeriodicUploadTime[OAM_MAX_TIME_LENGTH];
    U16 sampling_interval_timer;
    U16 file_write_timer; 
}oam_perf_mgmt_t;



typedef struct _oam_device_info_t
{
    UInt8 Manufacturer[OAM_MAX_MANUFAC_LENGTH];
    UInt8 ManufacturerOUI[OAM_MAX_OUI_LENGTH];
    UInt8 ModelName[OAM_MAX_MANUFAC_LENGTH];
    UInt8 Description[OAM_MAX_DESCRIPTION_LENGTH];
    UInt8 ProductClass[OAM_MAX_MANUFAC_LENGTH];
    UInt8 SerialNumber[OAM_MAX_MANUFAC_LENGTH];
    UInt8 HardwareVersion[OAM_MAX_MANUFAC_LENGTH];
    UInt8 SoftwareVersion[OAM_MAX_MANUFAC_LENGTH];
    UInt8 ModemFirmwareVersion[OAM_MAX_MANUFAC_LENGTH];
    UInt8 AdditionalSoftwareVersion[OAM_MAX_MANUFAC_LENGTH];
    UInt32 UpTime;
    UInt8 FirstUseDate[OAM_MAX_TIME_LENGTH];
}oam_device_info_t;

typedef struct _oam_wan_device_t
{
    UInt8 MACAddress[OAM_MAX_MAC_LENGTH];
}oam_wan_device_t;

typedef struct _oam_services_t
{
    UInt8 DNPrefix[OAM_MAX_STR_LEN];
    UInt8 DeviceType[OAM_MAX_STR_LEN];
    UInt32 FAPServiceNumberOfEntries;
}oam_services_t;

typedef struct _oam_fap_control_t
{

    UInt8   SelfConfigEvents[OAM_MAX_STR_LEN];
    UInt8   EnclosureTamperingDetected;
    UInt8   RFTxStatus;
     /* CSR 00051709 Change Start*/
    UInt8   S1SigLinkServerList[OAM_MAX_MME_LIST_LEN];
      /* CSR 00051709 Change End*/
    Char8   S1ConnectionMode[OAM_S1CONNMODE_MAX_LEN];
       /* SPR 11090 Change Start*/ 
    UInt16    S1SigLinkPort;
       /* SPR 11090 Change End*/ 
    /* LWA changes start*/
    /* LWA changes end*/

}oam_fap_control_t;


typedef struct _oam_cell_config_t
{
    UInt32  TunnelNumberOfEntries;
    UInt32  Enable;
    UInt8   TunnelRef[OAM_MAX_STR_LEN];
    UInt8   PLMNID[OAM_MAX_STR_LEN];
}oam_cell_config_t;

/*OAM_REVIEW_CHANGES*/
extern UInt8 oam_nw_scan_status[5][32];
/*OAM_REVIEW_CHANGES*/

typedef struct _oam_nmm_time_stamp_t
{
    UInt16 year;
    UInt16 month;
    UInt16 day;
    UInt16 hour;
    UInt16 min;
    UInt16 sec;
}oam_time_stamp_t;


typedef struct _oam_nw_scan_status_st
{
    UInt8 scan_status[32];
/* CSR 00070293 FIX START */
    Char8 error_details[OAM_NMM_MAX_STR_SIZE];
/* CSR 00070293 FIX END */
    UInt16 max_cell_entry;
    UInt16 num_cell_entry;
    UInt32 max_carrier_meas_entry;
    UInt32 num_carrier_meas_entry;
    /*SPR_16048 Fix Start*/
    UInt8 last_scan_time[48]; //Increased size to store date/time format 
    /*SPR_16048 Fix Start*/

}oam_nw_scan_status_t;



typedef struct _oam_plmn_list_t
{
    Char8 plmnid[6];
    UInt8 cell_reserver_for_operator_use;
}oam_plmn_list_t;

#define OAM_MAX_PLMN_ID_INFO_SIZE 6
typedef struct _oam_bcch_data_t
{
    UInt16 dl_bandwidth;
    UInt16 ul_bandwidth;
    SInt8  rx_tx_power;
    UInt16 tac;
    UInt32 cell_identity;
    UInt32 csg_identity;
    UInt8  cell_barred;
    UInt8  csg_indication;
    UInt8  max_plmn_entry;
    UInt8  num_plm_entry;
    oam_plmn_list_t oam_plmn_list[OAM_MAX_PLMN_ID_INFO_SIZE];
}oam_bcch_data_t;

typedef struct _oam_cell_discover_t
{
    UInt16 carrier_earfcn;
    UInt16 phy_cell_id;
    SInt16 rsrp;
    SInt16 rsrq;
    SInt16 rssi;
    oam_bcch_data_t oam_bcch_data;
}oam_cell_discover_t;

#define OAM_MAX_NUM_CELL_COUNT   10

typedef struct _oam_nw_scan_result_t
{
    UInt16 num_cell_discover;
    oam_cell_discover_t oam_cell_discover[OAM_MAX_NUM_CELL_COUNT];
}oam_nw_scan_result_t;

typedef struct _oam_rem_lte_t
{
    UInt8  in_service_handling[OAM_MAX_STR_LEN];
    UInt8  scan_on_boot;
    UInt8  scan_periodically;
    UInt32  periodic_interval;
    UInt8  periodic_time[OAM_MAX_TIME_LENGTH];
    UInt32 scan_timeout;
    UInt8  scan_status[OAM_MAX_STR_LEN];
    UInt8  error_details[OAM_MAX_STR_LEN];
    UInt8  last_scan_time[OAM_MAX_TIME_LENGTH];
    UInt32 max_cell_entries;
    UInt32 cell_number_of_entries;
    UInt32 max_carrier_meas_entries;
    UInt32 carrier_meas_number_of_entries;
    UInt32 eutra_carrier_arfcn;
    SInt16 rsrp;  /* before UInt32 */
    SInt16 rsrq;  /* before UInt32 */
    SInt8 rssi;  /* before UInt32 */
    SInt8 rstxpower; /* before UInt32 */
    UInt32 tac;
    UInt32 cell_id;
    UInt8  cell_barred;
    UInt32 max_plmn_list_entries;
    UInt32 plmn_list_number_of_entries;
    UInt8  plmn_id[OAM_MAX_STR_LEN];
    UInt8  cell_reserved_for_operator_use;
/* CSR 00070293 FIX START */
    SInt32 carrier_arfcndl;
/* CSR 00070293 FIX END */
    SInt8 carrier_chwidth;
    UInt32 carrier_rssi;
    SInt8 dl_bdwth;
    /*SPR_15770 Fix start*/
#ifdef OAM_SON_ENABLE 
    SInt8 is_nr_scan_required;
    son_meas_bandwidth_earfcn_list_t known_earfcn_list; /*To store known earfcn list to be used during NR SCAN */
#endif    
    /*SPR_15770 Fix end*/

}oam_rem_lte_t;

typedef struct _oam_time_t
{
    UInt8 Enable;
    UInt8 Status[OAM_MAX_STR_LEN];
    UInt8 NTPServer1[OAM_MAX_STR_LEN];
    UInt8 CurrentLocalTime[OAM_MAX_STR_LEN];
    UInt8 LocalTimeZoneName[OAM_MAX_STR_LEN];
    UInt32 SyncMonitoringInterval;
    UInt32 SyncMonitoringPollInterval;
}oam_time_t;

typedef struct _oam_perf_mgmt_params_t
{
    U32 ConfigNumberOfEntries;
    U32 Enable;
    oam_pm_url_et URL;
    U8* Username;
    U8* Password;
    U8 PeriodicUploadInterval;
    U8* PeriodicUploadTime;
    U8* PMConfig;
}oam_perf_mgmt_params_t;

typedef struct _oam_nms_ftp_server_params_t
{
    U8 Enable;
    U8 ftp_server_url[OAM_FTP_URL_NAME_SIZE];
    U8 sw_package_name[OAM_FTP_SW_PACKAGE_NAME_SIZE];	
    U8 ftp_server_username[OAM_FTP_USERNAME_SIZE];
    U8 ftp_server_password[OAM_FTP_PSWD_SIZE];
    U8 local_download_upload_path[OAM_LOCAL_PATH_NAME_SIZE];
}oam_nms_ftp_server_params_t;

#define OAM_LENGTH_DEV_SUMMARY 1024

#ifdef OAM_SHARED_MEM_LOGGER
typedef struct
{
    U32     wait_interval;
    U16     shm_size;
    U16     log_file_size;
    U16     log_chunk_count;
    char    log_path[OAM_LOCAL_PATH_NAME_SIZE];
    U8      cpu_bitmap;
    U8      file_count;
    U8      is_shared_mem_conf_present;
}oam_shared_mem_conf_t;
#endif

typedef struct
{
    oam_device_info_t device_info_params;
    oam_management_server_t management_server_params;
    oam_fault_mgmt_t fault_mgmt_params;
    oam_tunnel_t tunnel_params;
    oam_capabilities_t capabilities_params;
    oam_perf_mgmt_t perf_mgmt_params;
    oam_wan_device_t wan_device_params;
    oam_services_t services_params;
    oam_fap_control_t fap_control_params;
    oam_cell_config_t cell_config_params;
    oam_rem_lte_t rem_lte_params;
    oam_time_t time_params;
    oam_nw_scan_status_t nw_scan_status;
    UInt8 DeviceSummary[OAM_LENGTH_DEV_SUMMARY];
#ifdef OAM_SHARED_MEM_LOGGER
    oam_shared_mem_conf_t oam_shared_mem_conf;
#endif
}oam_igd_params_t;

#define MAC_OAM_VAR_SIZE(x) 1
#define PDCP_OAM_VAR_SIZE(x) 1
#define RLC_OAM_VAR_SIZE(x) 1




/********************************************/
/* SPR 14019 FIX Start */
#ifdef PERF_STATS
/* SPR 14019 FIX End */
typedef MacGetPerfStatsReqParams  oam_mac_get_perf_stats_req_t  ;
typedef MacConfigurePerfStatsReqParams oam_mac_config_perf_stats_req_t;
typedef MacGetUePerfStatsCnfParams oam_mac_get_ue_perf_stats_cnf;
typedef MacGetCellPerfStatsCnfParams oam_mac_get_cell_perf_stats_cnf;

typedef RlcConfigurePerfStatsReq oam_rlc_configure_perf_stats_req_t;
typedef RlcGetPerfStatsReq oam_rlc_get_perf_stats_req_t;
typedef RlcGetUePerfStatsCnf oam_rlc_get_perf_stats_cnf_t;

typedef PdcpGetPerfStatsReqParams  oam_pdcp_get_perf_stats_req_t;
typedef PdcpGetCellPerfStatsCnfParams oam_pdcp_get_cell_perf_stats_cnf_t;
typedef PdcpGetUePerfStatsCnfParams oam_pdcp_get_ue_perf_stats_cnf_t;
typedef PdcpConfigurePerfStatsReqParams oam_pdcp_config_perf_stats_req_t;
typedef PdcpConfigurePerfStatsCnfParams oam_pdcp_config_perf_stats_cnf_t;
typedef PdcpUePerfStatsIndParams oam_pdcp_ue_perf_stats_ind_t;
typedef PdcpCellPerfStatsIndParams oam_pdcp_cell_perf_stats_ind_t;
/* SPR 14019 FIX Start */
#endif
/* SPR 14019 FIX End */

/************************ merging 1.1 change****************************/

typedef relayGtpuInitDBReq oam_egtpu_initdb_req_t;
typedef relayGtpuGetMaxRetriesReq oam_relay_Gtpu_Get_Max_Retries_Req_t;
typedef relayGtpuSetMaxRetriesReq oam_relay_Gtpu_Set_Max_Retries_Req_t;
typedef relayGtpuSetTimerReq oam_relay_Gtpu_Set_Timer_Req_t;
typedef relayGtpuGetTimerReq oam_relay_Gtpu_Get_Timer_Req_t;
typedef relaySetTraceLvlReq oam_relay_Set_Trace_Lvl_Req_t;
typedef relayGetTraceLvlReq oam_relay_Get_Trace_Lvl_Req_t;
typedef relayInitTraceLvlCnf oam_relay_Init_Trace_Lvl_Cnf_t;
typedef relayGtpuGetTimerCnf oam_relay_Gtpu_Get_Timer_Cnf_t;
typedef relayGtpuSetTimerCnf oam_relay_Gtpu_Set_Timer_Cnf_t;
typedef relayGetTraceLvlCnf oam_relay_Get_Trace_Lvl_Cnf_t;
typedef relaySetTraceLvlCnf oam_relay_Set_Trace_Lvl_Cnf_t;
typedef relayGtpuGetMaxRetriesCnf relay_Gtpu_Get_Max_Retries_Cnf_t;
typedef relayGtpuSetMaxRetriesCnf oam_relay_Gtpu_Set_Max_Retries_Cnf_t;
typedef relayGtpuSetLogLevReq oam_relay_Gtpu_Set_Log_Lvl_Req_t;
typedef relayGtpuGetLogLevResp oam_relay_Gtpu_Get_Log_Lvl_Req_t;

/***************egptu*******************/


/*Layer 3*/
typedef rrc_connEstabTime_t  oam_rrc_connEstabTime_t;
typedef  rrc_oam_config_stats_resp_t oam_rrc_config_stats_resp_t;
typedef rrc_oam_config_stats_req_t oam_rrc_config_stats_req_t;

typedef rrc_oam_ext_comm_info_t oam_rrc_ext_comm_info_t;
typedef rrc_oam_communication_info_req_t oam_rrc_communication_info_req_t;
typedef rrc_oam_timer_info_t oam_rrc_timer_info_t;
typedef rrc_oam_integrity_algorithms_t oam_rrc_integrity_algorithms_t;
typedef rrc_oam_ciphering_algorithms_t oam_rrc_ciphering_algorithms_t;
typedef rrc_oam_supported_security_algorithms_t oam_rrc_supported_security_algorithms_t;
typedef rrc_oam_llim_timer_info_t oam_rrc_llim_timer_info_t;
typedef rrc_oam_csc_timer_info_t oam_rrc_csc_timer_info_t;
typedef rrc_oam_uecc_timer_info_t oam_rrc_uecc_timer_info_t;
typedef rrc_oam_provision_req_t oam_rrc_provision_req_t;
/*CSR 57877 Code Removed*/
typedef rrc_oam_log_enable_req_t oam_rrc_log_enable_req_t;
typedef rrc_oam_get_cell_stats_req_t oam_rrc_get_cell_stats_req_t;
typedef rrc_oam_get_cell_status_req_t oam_rrc_get_cell_status_req_t;
typedef rrc_oam_get_ue_status_req_t oam_rrc_get_ue_status_req_t;
typedef rrc_oam_reset_cell_stats_req_t oam_rrc_reset_cell_stats_req_t;
/*CSR 57877 Code Removed*/
typedef rrc_oam_cell_traffic_trace_stop_t oam_rrc_cell_traffic_trace_stop_t;
typedef rrc_oam_trace_file_t oam_rrc_trace_file_t;
typedef rrc_oam_trace_file_transfer_complete_ind_t oam_rrc_trace_file_transfer_complete_ind_t;
typedef rrc_oam_init_ind_t oam_rrc_init_ind_t;
typedef rrc_oam_ext_connection_status_t oam_rrc_ext_connection_status_t;
typedef rrc_oam_communication_info_resp_t oam_rrc_communication_info_resp_t;
typedef rrc_oam_provision_resp_t oam_rrc_provision_resp_t;
typedef rrc_oam_set_log_level_resp_t oam_rrc_set_log_level_resp_t;
/* SPR 13617 START */
typedef rrc_oam_get_log_level_resp_t oam_rrc_get_log_level_resp_t;
/* SPR 13617 END */
typedef rrc_oam_log_enable_resp_t oam_rrc_log_enable_resp_t;
typedef rrc_oam_cell_status_t oam_rrc_cell_status_t;
typedef rrc_oam_get_cell_status_resp_t oam_rrc_get_cell_status_resp_t;
typedef rrc_oam_ue_status_t oam_rrc_ue_status_t;
typedef rrc_oam_ue_status_list_t oam_rrc_ue_status_list_t;
typedef rrc_oam_get_ue_status_resp_t oam_rrc_get_ue_status_resp_t;
typedef rrc_oam_reset_cell_stats_resp_t oam_rrc_reset_cell_stats_resp_t;
typedef rrc_oam_cleanup_resp_t oam_rrc_cleanup_resp_t;
typedef rrc_oam_cell_traffic_trace_start_error_ind_t oam_rrc_cell_traffic_trace_start_error_ind_t;
typedef rrc_oam_e_utran_trace_id_t oam_rrc_e_utran_trace_id_t;
typedef rrc_oam_trace_file_transfer_start_ind_t oam_rrc_trace_file_transfer_start_ind_t;
typedef rrc_trace_failure_list_item_t oam_rrc_trace_failure_list_item_t;
typedef rrc_oam_trace_start_error_ind_t oam_rrc_trace_start_error_ind_t;
typedef rrc_trace_failure_list_t oam_rrc_trace_failure_list_t;
typedef rrc_uecc_oam_ue_ctxt_rel_req_fail_cause_t oam_rrc_ue_ctxt_rel_req_fail_cause_t;
typedef rrc_uecc_oam_stat_ue_context_release_t oam_rrc_stat_ue_context_release_t;
typedef rrc_uecc_oam_stat_connection_reestablishment_t oam_rrc_stat_connection_reestablishment_t;
typedef rrc_uecc_oam_stat_connection_establishment_t oam_rrc_stat_connection_establishment_t;
typedef rrc_oam_connection_related_stats_t oam_rrc_connection_related_stats_t;
typedef rrc_csc_oam_stat_paging_t oam_rrc_csc_stat_paging_t;
typedef rrc_uecc_oam_stat_ue_assoc_s1_connection_t oam_rrc_stat_ue_assoc_s1_connection_t;
typedef rrc_uecc_oam_stat_erab_number_t oam_rrc_stat_erab_number_t;
typedef rrc_uecc_oam_stat_erab_mod_fail_cause_t oam_rrc_stat_erab_mod_fail_cause_t;
typedef rrc_uecc_oam_stat_erab_modification_t oam_rrc_stat_erab_modification_t;
typedef rrc_uecc_oam_rel_fail_cause_t oam_rrc_rel_fail_cause_t;
typedef rrc_uecc_oam_erab_rel_enb_cause_t oam_rrc_erab_rel_enb_cause_t;
typedef rrc_uecc_oam_stat_erab_release_t oam_rrc_stat_erab_release_t;
typedef rrc_uecc_oam_erab_estab_init_fail_t oam_rrc_erab_estab_init_fail_t;
typedef rrc_uecc_oam_stat_erab_setup_t oam_rrc_stat_erab_setup_t;
typedef rrc_uecc_oam_erab_estab_add_fail_t oam_rrc_erab_estab_add_fail_t;
typedef rrc_oam_erab_related_stats_t oam_rrc_erab_related_stats_t;
typedef rrc_oam_get_cell_stats_resp_t oam_rrc_get_cell_stats_resp_t;
typedef rrc_erab_estabTime_t oam_rrc_erab_estabTime_t;
/*****************************S1AP***********************************************/
typedef ip_addr_t oam_ip_addr_t;
typedef s1ap_plmn_identity_t oam_s1ap_plmn_identity_t;
typedef home_enb_id_t oam_home_enb_id_t;
typedef enb_id_info_t oam_enb_id_info_t;
typedef macro_enb_id_t oam_macro_enb_id_t;
typedef gb_enb_id_t oam_gb_enb_id_t;
typedef bc_plmn_list_t oam_bc_plmn_list_t;
typedef supp_ta_t oam_supp_ta_t;
typedef csg_id_info_t oam_csg_id_info_t;
typedef csg_id_list_t oam_csg_id_list_t;
typedef supp_ta_list_t oam_s1ap_supp_ta_list_t;
typedef s1_setup_req_t oam_s1_setup_req_t;
typedef enb_comm_info_t oam_s1_enb_comm_info_t;
typedef mme_comm_info_t oam_mme_comm_info_t;
typedef s1apInfo_t oam_s1apInfo_t;
typedef s1ap_oam_provision_req_t oam_s1ap_provision_req_t;
typedef s1ap_oam_enb_config_update_t oam_s1ap_enb_config_update_t;
typedef s1ap_oam_mme_id_list_info_t oam_s1ap_mme_id_list_info_t;
typedef s1ap_oam_provision_resp_t oam_s1ap_provision_resp_t;
typedef s1ap_oam_close_mme_conn_t oam_s1ap_close_mme_conn_t;
typedef s1ap_oam_reestab_mme_conn_t oam_s1ap_reestab_mme_conn_t;
typedef s1ap_oam_s1ap_link_status_ind_t oam_s1ap_link_status_ind_t;
typedef active_mme_ctx_t oam_active_mme_ctx_t;
/* CSR 00058587 Fix Start */
typedef s1ap_oam_status_resp_t oam_s1ap_status_resp_t;
/* CSR 00058587 Fix End */
typedef s1ap_oam_reset_resp_t oam_s1ap_reset_resp_t;
typedef s1ap_oam_mme_conn_info_t oam_s1ap_enb_config_update_response_info_t;
typedef s1ap_oam_enb_config_update_response_t oam_s1ap_enb_config_update_response_t;
typedef s1ap_oam_get_log_level_resp_t oam_s1ap_get_log_level_resp_t;
typedef s1ap_oam_log_enable_resp_t oam_s1ap_log_enable_resp_t;
typedef s1ap_oam_log_enable_req_t oam_s1ap_log_enable_req_t;
/* SPR-9485 START */
#ifdef OAM_SON_ENABLE
typedef son_gu_group_id_list_t local_oam_anr_gu_group_info_t; 
typedef son_oam_anr_gu_group_id_config_req_t oam_son_anr_gu_group_id_config_req_t;
#endif
/* SPR-9485 END */

/*SCTP Changes*/
typedef s1ap_oam_get_sctp_status_req_t oam_s1ap_get_sctp_status_req_t;  
typedef s1ap_oam_get_sctp_status_resp_t oam_s1ap_get_sctp_status_resp_t;
typedef x2ap_oam_get_sctp_status_resp_t oam_x2ap_get_sctp_status_resp_t;
typedef x2ap_oam_get_sctp_status_req_t oam_x2ap_get_sctp_status_req_t;

/*CHANGE_1_3_1*/
typedef s1ap_add_mme_res_t oam_s1ap_add_mme_res_t;
typedef s1ap_add_mme_req_t oam_s1ap_add_mme_req_t; 
/**************************X2AP*********************************************/
typedef x2ap_delete_enb_res_t oam_x2ap_delete_enb_res_t;
typedef x2_gb_enb_id_t oam_x2_gb_enb_id_t;
typedef x2ap_delete_enb_req_t oam_x2ap_delete_enb_req_t;
typedef x2_enb_comm_info_t oam_x2_enb_comm_info_t;
typedef x2ap_enb_comm_info_list_t oam_x2ap_enb_comm_info_list_t;
typedef x2_ip_addr_t oam_x2_ip_addr_t;
typedef x2ap_add_enb_res_t oam_x2ap_add_enb_res_t;
typedef x2ap_add_enb_req_t oam_x2ap_add_enb_req_t;
/* SPR-16334 START */
typedef x2ap_enb_config_update_req_t oam_x2ap_enb_config_update_req_t;
typedef x2ap_enb_config_update_resp_t oam_x2ap_enb_config_update_resp_t;
typedef x2ap_gu_group_id_list_t local_oam_gu_group_id_list_t;
/* SPR-16334 END */

/*CHANGE_1_3_1*/
typedef x2ap_prach_configuration_info_t oam_x2ap_prach_configuration_info_t;
typedef x2ap_subframe_allocation_t oam_x2ap_subframe_allocation_t;
typedef son_comm_info_t oam_x2ap_son_comm_info_t;
typedef x2ap_mbsfn_info_t oam_x2ap_mbsfn_info_t;
typedef x2ap_mbsfn_subframe_info_t oam_x2ap_mbsfn_subframe_info_t;
typedef x2ap_oam_provision_resp_t oam_x2ap_provision_resp_t  ;
typedef x2apOam_srvd_cells_t oam_x2ap_srvd_cells_t;
typedef x2apOam_srvd_cell_arry_t oam_x2ap_srvd_cell_arry_t;
typedef x2apOam_srvd_cell_info_t oam_x2ap_srvd_cell_info_t;
typedef x2ap_tac_t oam_x2ap_tac_t;
typedef x2ap_broadcast_plmn_t oam_x2ap_broadcast_plmn_t;
typedef x2ap_choice_eutra_mode_info_t oam_x2ap_choice_eutra_mode_info_t;
typedef x2ap_choice_eutra_mode_t oam_x2ap_choice_eutra_mode_t;
typedef x2ap_fdd_info_t oam_x2ap_fdd_info_t;
typedef x2ap_earfcn_t oam_x2ap_earfcn_t;
typedef x2ap_tdd_info_t oam_x2ap_tdd_info_t;
typedef x2ap_special_subframe_info_t oam_x2ap_special_subframe_info_t;
typedef x2ap_nbour_cell_arry_t oam_x2ap_nbour_cell_arry_t;
typedef x2ap_neighbour_info_t oam_x2ap_neighbour_info_t;
typedef x2ap_oam_x2_link_down_t oam_x2ap_x2_link_down_t;
typedef x2ap_oam_x2_link_up_t oam_x2ap_x2_link_up_t;
typedef x2ap_ecgi_t oam_x2ap_ecgi_t;
typedef x2ap_gu_grp_ids_t oam_x2ap_gu_grp_ids_t;
typedef x2ap_gu_group_id_t oam_x2ap_gu_group_id_t;
typedef x2ap_oam_provision_req_t oam_x2ap_provision_req_t;
typedef x2ap_info_t oam_x2ap_info_t;
typedef x2ap_conf_param_t oam_x2ap_conf_param_t;
typedef x2ap_sctp_conf_info_t oam_x2ap_sctp_conf_info_t;
typedef x2ap_con_tim_info_t oam_x2ap_con_tim_info_t;
typedef x2ap_plmn_identity_t oam_x2ap_plmn_identity_t;
typedef x2ap_oam_get_log_level_resp_t oam_x2ap_get_log_level_resp_t;
typedef x2ap_oam_log_enable_resp_t oam_x2ap_log_enable_resp_t;
typedef x2ap_oam_log_enable_req_t oam_x2ap_log_enable_req_t;

/**************************X2AP*********************************************/

/* LWA changes start*/
/**************************XWAP********************************************/
/* LWA changes end*/

typedef rrm_oam_cell_reselection_params_cdma2000_t oam_rrm_cell_reselection_params_cdma2000_t;
typedef rrm_oam_secondary_pre_reg_zone_id_list_hrpd_t oam_rrm_secondary_pre_reg_zone_id_list_hrpd_t;
typedef rrm_oam_pre_reg_info_hrpd_t oam_rrm_pre_reg_info_hrpd_t;
typedef rrm_oam_band_class_info_cdma2000_t oam_rrm_band_class_info_cdma2000_t;
typedef rrm_oam_ac_barring_config_1_xrtt_r9_t oam_rrm_ac_barring_config_1_xrtt_r9_t;
typedef rrm_oam_idle_mode_mobility_inter_rat_cdma2000_params_t oam_rrm_idle_mode_mobility_inter_rat_cdma2000_params_t;

/*RRM*/
typedef rrm_oam_cell_config_req_t oam_rrm_cell_config_req_t;
typedef rrm_oam_cell_plmn_info_t oam_rrm_cell_plmn_info_t;
typedef rrm_oam_global_cell_id_t oam_rrm_global_cell_id_t;
typedef rrm_oam_eutran_global_cell_id_t oam_rrm_eutran_global_cell_id_t;
typedef rrm_oam_pdsch_t oam_rrm_pdsch_t;
typedef rrm_oam_srs_t oam_rrm_srs_t;
typedef rrm_oam_prach_t oam_rrm_prach_t;
typedef rrm_oam_pucch_t oam_rrm_pucch_t;
typedef rrm_oam_pusch_t oam_rrm_pusch_t;
typedef rrm_oam_ul_reference_signal_t oam_rrm_ul_reference_signal_t;
typedef rrm_oam_uplink_power_control_t oam_rrm_uplink_power_control_t;
typedef rrm_oam_prs_t oam_rrm_prs_t;
typedef available_gbr_limit_t oam_available_gbr_limit_t;
typedef rrm_oam_physical_layer_params_t oam_rrm_physical_layer_params_t;
typedef rrm_oam_preamble_info_t oam_rrm_preamble_info_t;
typedef rrm_oam_rach_t oam_rrm_rach_t;
typedef rrm_oam_drx_config_t oam_rrm_drx_config_t;
typedef rrm_oam_short_drx_cycle_config_t oam_rrm_short_drx_cycle_config_t;
typedef rrm_oam_drx_t oam_rrm_drx_t;
typedef rrm_oam_mac_layer_params_t oam_rrm_mac_layer_params_t;
typedef rrm_oam_srb_info_t oam_rrm_srb_info_t;
typedef rrm_oam_srb_t oam_rrm_srb_t;
typedef rrm_oam_rlc_layer_params_t oam_rrm_rlc_layer_params_t;
typedef rrm_oam_speed_state_params_t oam_rrm_speed_state_params_t;
typedef rrm_oam_common_params_t oam_rrm_common_params_t;
typedef rrm_oam_speed_scale_factors_t oam_rrm_speed_scale_factors_t;
typedef rrm_oam_intra_freq_params_t oam_rrm_intra_freq_params_t;
typedef rrm_oam_inter_freq_params_t oam_rrm_inter_freq_params_t;
typedef rrm_oam_idle_mode_mobility_params_t oam_rrm_idle_mode_mobility_params_t;
typedef rrm_oam_mobility_params_t oam_rrm_mobility_params_t;
typedef rrm_oam_rrc_timers_t oam_rrm_rrc_timers_t;
typedef rrm_oam_rrc_constants_t oam_rrm_rrc_constants_t;
typedef rrm_oam_rrc_timers_and_constants_t oam_rrm_rrc_timers_and_constants_t;
typedef rrm_oam_rf_configurations_t oam_rrm_rf_configurations_t;
typedef rrm_oam_rf_params_t oam_rrm_rf_params_t;
typedef rrm_oam_s1ap_params_t oam_rrm_s1ap_params_t;
typedef rrm_oam_intra_freq_cells_t oam_rrm_intra_freq_cells_t;
typedef rrm_oam_inter_freq_cells_t oam_rrm_inter_freq_cells_t;
typedef rrm_oam_lte_ncl_t oam_rrm_lte_ncl_t;
typedef rrm_oam_ncl_params_t oam_rrm_ncl_params_t;
typedef rrm_oam_ran_t oam_rrm_ran_t;
typedef rrm_oam_general_epc_params_t oam_rrm_general_epc_params_t;
typedef rrm_oam_qos_config_params_t oam_rrm_qos_config_params_t;
typedef rrm_oam_epc_params_t oam_rrm_epc_params_t;
typedef rrm_oam_epc_t oam_rrm_epc_t;
typedef rrm_oam_rrmc_mac_config_t oam_rrm_rrmc_mac_config_t;
typedef rrm_oam_phy_phich_configuration_t oam_rrm_phy_phich_configuration_t;
typedef rrm_oam_cell_selection_info_v920_t oam_rrm_cell_selection_info_v920_t;
typedef rrm_oam_sib_type_1_info_t oam_rrm_sib_type_1_info_t;
typedef rrm_oam_radio_resource_config_common_sib_t oam_rrm_radio_resource_config_common_sib_t;
typedef rrm_oam_sib_type_2_info_t oam_rrm_sib_type_2_info_t;
typedef rrm_oam_intra_freq_cell_reselection_info_t oam_rrm_intra_freq_cell_reselection_info_t;
typedef rrm_oam_s_intra_search_v920_t oam_rrm_s_intra_search_v920_t;
typedef rrm_oam_s_non_intra_search_v920_t oam_rrm_s_non_intra_search_v920_t;
typedef rrm_oam_sib_type_3_info_t oam_rrm_sib_type_3_info_t;
typedef rrm_oam_csg_cell_id_range_t oam_rrm_csg_cell_id_range_t;
typedef rrm_oam_sib_type_4_info_t oam_rrm_sib_type_4_info_t;
typedef rrm_oam_admission_control_info_t oam_rrm_admission_control_info_t;
typedef rrm_oam_operator_info_t oam_rrm_operator_info_t;
typedef rrm_oam_access_mgmt_params_t oam_rrm_access_mgmt_params_t;
typedef rrm_oam_cell_reconfig_req_t oam_rrm_cell_reconfig_req_t;
typedef rrm_oam_cell_delete_req_t oam_rrm_cell_delete_req_t;
typedef rrm_oam_cell_start_req_t oam_rrm_cell_start_req_t;
typedef rrm_oam_cell_stop_req_t oam_rrm_cell_stop_req_t;
typedef rrm_oam_init_config_req_t oam_rrm_init_config_req_t;
typedef rrm_oam_module_init_config_t oam_rrm_module_init_config_t;
typedef rrm_oam_init_config_resp_t oam_rrm_init_config_resp_t;
/*CSR 57877 Code Removed*/
typedef rrm_oam_log_enable_disable_resp_t oam_rrm_log_enable_disable_resp_t;
typedef rrm_oam_set_log_level_resp_t oam_rrm_set_log_level_resp_t;
typedef rrm_oam_cell_config_resp_t oam_rrm_cell_config_resp_t;
typedef rrm_oam_cell_start_resp_t oam_rrm_cell_start_resp_t;
typedef rrm_oam_cell_stop_resp_t oam_rrm_cell_stop_resp_t;
typedef rrm_oam_cell_reconfig_resp_t oam_rrm_cell_reconfig_resp_t;
typedef rrm_oam_delete_resp_t oam_rrm_delete_resp_t;
typedef rrm_oam_rac_enable_disable_resp_t oam_rrm_rac_enable_disable_resp_t;
typedef rrm_oam_rac_enable_disable_req_t oam_rrm_rac_enable_disable_req_t;
typedef rrm_oam_shutdown_resp_t oam_rrm_shutdown_resp_t;
typedef rrm_oam_resume_service_resp_t oam_rrm_resume_service_resp_t;
typedef rrm_oam_thresholdx_q_r9_t oam_rrm_thresholdx_q_r9_t;
typedef rrm_oam_cell_access_restriction_params_t oam_rrm_cell_access_restriction_params_t;
typedef rrm_oam_cell_info_t oam_rrm_cell_info_t;
typedef rrm_oam_inter_frequency_params_list_t oam_rrm_inter_frequency_params_list_t;
typedef rrm_oam_idle_mode_mobility_inter_rat_utra_params_t
oam_rrm_idle_mode_mobility_inter_rat_utra_params_t;
typedef rrm_oam_idle_mode_mobility_irat_eutra_to_utra_reselection_params_t
oam_rrm_idle_mode_mobility_irat_eutra_to_utra_reselection_params_t;
typedef rrm_oam_irat_eutran_to_utran_fdd_list_t oam_rrm_irat_eutran_to_utran_fdd_list_t;
typedef rrm_oam_log_config_t oam_rrm_log_config_t;
typedef irat_eutran_to_utran_fdd_t oam_rrm_irat_eutran_to_utran_fdd_t; //Nitin - Simulator path
typedef rrm_oam_preamble_groupA_info_t oam_rrm_preamble_groupA_info_t;
typedef rrm_oam_shutdown_req_t oam_rrm_shutdown_req_t;
typedef rrm_oam_plmn_access_info_t oam_rrm_plmn_access_info_t;
typedef rrm_oam_thresx_rsrq_r9_t oam_rrm_thresx_rsrq_r9_t;
typedef rrm_oam_irat_eutran_to_utran_fdd_carriers_t oam_rrm_irat_eutran_to_utran_fdd_carriers_t;
/*CSR 57877 Code Removed*/

typedef rrm_oam_tdd_frame_structure_t oam_rrm_tdd_frame_structure_t;
typedef rrm_oam_addl_ul_reference_signal_params_t oam_rrm_addl_ul_reference_signal_params_t;
typedef rrm_oam_rohc_profiles_t oam_rrm_rohc_profiles_t;
typedef rrm_oam_pdcp_rohc_params_t oam_rrm_pdcp_rohc_params_t;
typedef rrm_oam_sn_field_len_t oam_rrm_sn_field_len_t;
typedef rrm_oam_sps_config_dl_t oam_rrm_sps_config_dl_t;
typedef rrm_oam_sps_config_ul_t oam_rrm_sps_config_ul_t;
typedef rrm_oam_sps_config_data_t oam_rrm_sps_config_data_t;

typedef rrm_enb_tnl_address_t oam_rrm_enb_tnl_address_t;
typedef rrm_oam_addl_pusch_config_t oam_rrm_addl_pusch_config_t;
typedef rrm_oam_irat_parameters_cdma2000_v920_t oam_rrm_irat_parameters_cdma2000_v920_t;
typedef rrm_oam_cdma2000_hrpd_cell_identifier_t oam_rrm_cdma2000_hrpd_cell_identifier_t;
typedef rrm_oam_cdma2000_1xrtt_cell_identifier_t oam_rrm_cdma2000_1xrtt_cell_identifier_t;
typedef rrm_oam_cdma2000_rand_t oam_rrm_cdma2000_rand_t; 
typedef rrm_oam_cdma2000_cell_param_t oam_rrm_cdma2000_cell_param_t;
typedef rrm_oam_mobility_sib_8_params_t oam_rrm_mobility_sib_8_params_t;

typedef rrm_oam_irat_eutran_to_utran_tdd_carriers_t oam_rrm_irat_eutran_to_utran_tdd_carriers_t;
typedef rrm_oam_irat_eutran_to_utran_tdd_list_t oam_rrm_irat_eutran_to_utran_tdd_list_t;

typedef rrm_oam_tpc_rnti_range_t oam_rrm_tpc_rnti_range_t;
typedef rrm_oam_power_control_enable_t oam_rrm_power_control_enable_t;
typedef rrm_oam_power_control_params_t oam_rrm_power_control_params_t;
typedef rrm_oam_sps_crnti_range_t oam_rrm_sps_crnti_range_t;
typedef rrm_oam_adl_pkt_scheduling_params_t oam_rrm_adl_pkt_scheduling_params_t;
typedef rrm_oam_adl_cell_params_t oam_rrm_adl_cell_params_t;
typedef rrm_oam_load_params_t oam_rrm_load_params_t;
typedef rrm_oam_tm_mode_table_row_t oam_rrm_tm_mode_table_row_t;
typedef rrm_oam_transmission_mode_table_t oam_rrm_transmission_mode_table_t;
typedef rrm_oam_mimo_codebook_subset_restriction_t oam_rrm_mimo_codebook_subset_restriction_t;
typedef rrm_oam_mimo_mode_params_t oam_rrm_mimo_mode_params_t;

typedef rrm_oam_meas_gap_config_t oam_rrm_meas_gap_config_t;
typedef rrm_oam_meas_config_t oam_rrm_meas_config_t;
typedef rrm_oam_target_cell_selection_params_t oam_rrm_target_cell_selection_params_t;
/* SPR 22311 Changes - Starts */
/* Code Deleted */
/* SPR 22311 Changes - Ends */
typedef rrm_oam_ho_retry_params_t oam_rrm_ho_retry_params_t;
typedef rrm_oam_ho_config_params_t oam_rrm_ho_config_params_t;
typedef rrm_oam_ue_generic_cdma2000_params_t oam_rrm_ue_generic_cdma2000_params_t;
typedef rrm_oam_fpc_fch_included_t oam_rrm_fpc_fch_included_t;
typedef rrm_oam_pz_hyst_parameters_included_t oam_rrm_pz_hyst_parameters_included_t;
typedef rrm_oam_cell_specific_params_t oam_rrm_cell_specific_params_t;
typedef rrm_oam_cdma2000_freq_cells_t oam_rrm_cdma2000_freq_cells_t;

typedef rrm_oam_addl_pucch_config_t oam_rrm_addl_pucch_config_t;
typedef rrm_oam_addl_phy_params_t oam_rrm_addl_phy_params_t;
typedef rrm_oam_updated_plmn_info_t oam_rrm_updated_plmn_info_t;
typedef rrm_oam_cell_update_req_t oam_rrm_cell_update_req_t;
typedef rrm_oam_cell_update_resp_t oam_rrm_cell_update_resp_t;
typedef rrm_global_enb_id_t oam_rrm_global_enb_id_t;
typedef rrm_oam_access_class_barring_information_t oam_rrm_access_class_barring_information_t;
typedef rrm_oam_ssac_barring_for_mmtel_r9_t oam_rrm_ssac_barring_for_mmtel_r9_t;
typedef rrm_oam_access_barring_info_t oam_rrm_access_barring_info_t;
typedef rrm_oam_spid_configuration_t oam_rrm_spid_configuration_t;
typedef rrm_oam_spid_table_t oam_rrm_spid_table_t;
typedef rrm_utran_cell_id_t oam_rrm_utran_cell_id_t;
typedef rrm_lai_t oam_rrm_lai_t;
typedef rrm_rai_t oam_rrm_rai_t;
typedef rrm_oam_band_class_list_cdma2000_t oam_rrm_band_class_list_cdma2000_t;
typedef rrm_oam_irat_t oam_rrm_irat_t;
typedef rrm_oam_common_params_for_eutra_t oam_rrm_common_params_for_eutra_t;
typedef rrm_oam_connected_mode_mobility_params_t oam_rrm_connected_mode_mobility_params_t;
typedef rrm_oam_geran_freq_cells_t oam_rrm_geran_freq_cells_t;
typedef rrm_oam_utran_freq_cells_t oam_rrm_utran_freq_cells_t;
typedef rrm_oam_inter_rat_ncl_t oam_rrm_inter_rat_ncl_t;
typedef rrm_oam_enb_config_req_t oam_rrm_enb_config_req_t;
typedef rrm_oam_enb_config_resp_t oam_rrm_enb_config_resp_t;
/*CHANGE_1_3_1*/
typedef rrm_oam_sib_mapping_info_t oam_rrm_sib_mapping_info_t;
typedef rrm_oam_scheduling_info_t oam_rrm_scheduling_info_t;
typedef rrm_csfb_tgt_selection_t oam_rrm_csfb_tgt_selection_t;
typedef rrm_oam_serving_enb_cell_info_t oam_rrm_serving_enb_cell_info_t;
typedef rrm_oam_load_config_req_t oam_rrm_load_config_req_t;
typedef rrm_oam_load_config_resp_t oam_rrm_load_config_resp_t;
typedef rrm_oam_resource_load_info_t oam_rrm_resource_load_info_t;
typedef rrm_comp_avl_cap_t oam_rrm_comp_avl_cap_t;
typedef rrm_oam_bsr_config_t oam_rrm_bsr_config_t;
typedef rrm_oam_phr_config_t oam_rrm_phr_config_t;
typedef rrm_oam_addl_mac_params_t oam_rrm_addl_mac_params_t;
typedef rrm_oam_addl_rlc_params_t oam_rrm_addl_rlc_params_t;
typedef rrm_hw_load_ind_t oam_rrm_hw_load_ind_t;
typedef rrm_s1_tnl_load_ind_t oam_rrm_s1_tnl_load_ind_t;
typedef rrm_oam_resrc_info_t oam_rrm_resrc_info_t;
typedef rrm_oam_load_def_t oam_rrm_load_def_t;
typedef rrm_configure_qci_bitrate_t oam_rrm_configure_qci_bitrate_t;
typedef rrm_qci_bitrate_info_t oam_rrm_qci_bitrate_info_t;
typedef rrm_ecn_configure_cell_list_t oam_rrm_ecn_configure_cell_list_t;
typedef rrm_oam_watermark_t oam_rrm_watermark_t;
typedef rrm_oam_cell_ecn_capacity_enhance_resp_t oam_rrm_cell_ecn_capacity_enhance_resp_t;
typedef rrm_oam_get_ver_id_resp_t oam_rrm_get_ver_id_resp_t;
typedef rrm_rrs_load_ind_t oam_rrm_rrs_load_ind_t;
typedef rrm_comp_avl_cap_grp_t oam_rrm_comp_avl_cap_grp_t;
typedef rrm_cell_load_info_t oam_rrm_cell_load_info_t;
typedef rrm_oam_load_report_ind_t oam_rrm_load_report_ind_t;
/* SPR_13116_CHANGE_START */
typedef rrm_oam_get_log_level_req_t oam_rrm_get_log_level_req_t;
typedef rrm_oam_get_log_level_resp_t oam_rrm_get_log_level_resp_t;
/* SPR_13116_CHANGE_END */
/*CHANGE_1_3_1*/
typedef rrm_oam_cell_block_req_t oam_rrm_cell_block_req_t;
typedef rrm_oam_cell_block_resp_t oam_rrm_cell_block_resp_t;
typedef rrm_oam_cell_unblock_cmd_t oam_rrm_cell_unblock_cmd_t;

#ifdef OAM_SON_ENABLE
/*SON*/
typedef son_nbr_cell_status_t oam_son_nbr_cell_status_t;
/*CSR 57877 Code Removed*/
typedef son_oam_module_init_config_t oam_son_module_init_config_t;
typedef son_cell_plmn_info_t oam_son_cell_plmn_info_t;
typedef son_intra_rat_global_cell_id_t oam_son_intra_rat_global_cell_id_t;
/* Start: CSR 00053250 */
/* typedef son_meas_bandwidth_per_earfcn_t oam_son_meas_bandwidth_per_earfcn_t;
typedef son_meas_bandwidth_earfcn_list_t oam_son_meas_bandwidth_earfcn_list_t; */
/*End: CSR 00053250 end */
typedef son_start_intra_rat_scan_req_t oam_son_start_intra_rat_scan_req_t;
typedef son_start_utra_scan_req_t oam_son_start_utra_scan_req_t;
typedef son_start_geran_scan_req_t oam_son_start_geran_scan_req_t;
typedef son_start_cdma_scan_req_t oam_son_start_cdma_scan_req_t;
typedef son_start_inter_rat_scan_req_t oam_son_start_inter_rat_scan_req_t;
typedef son_rf_cell_num_t oam_son_rf_cell_num_t;
typedef son_rf_freq_channel_num_t oam_son_rf_freq_channel_num_t;
typedef son_bcch_cell_num_t oam_son_bcch_cell_num_t;
typedef son_start_intra_rat_bulk_scan_resp_t oam_son_start_intra_rat_scan_resp_t; //Nitin - Simulator path
typedef son_start_inter_rat_bulk_scan_resp_t oam_son_start_inter_rat_scan_resp_t; //Nitin - Simulator path
typedef son_oam_bulk_nw_scan_resp_t oam_son_oam_bulk_nw_scan_resp_t; //Nitin - Simulator path
typedef son_bulk_nw_scan_resp_t oam_son_bulk_nw_scan_resp_t;
typedef son_cell_status_t oam_son_cell_status_t;
typedef son_intra_rat_neighbor_info_t oam_son_intra_rat_neighbor_info_t; //Nitin - Simulator path
typedef son_inter_rat_neighbor_info_t oam_son_inter_rat_neighbor_info_t; //Nitin - Simulator path
typedef son_neighbor_info_t oam_son_neighbor_info_t;
typedef son_utran_geran_global_cell_id_t oam_son_utran_geran_global_cell_id_t;
typedef son_others_global_cell_id_t oam_son_others_global_cell_id_t;
typedef son_inter_rat_global_cell_id_t oam_son_inter_rat_global_cell_id_t;
typedef son_global_cell_id_t oam_son_global_cell_id_t;
typedef son_oam_shutdown_resp_t oam_son_shutdown_resp_t;
typedef son_oam_init_config_req_t oam_son_init_config_req_t;
typedef son_oam_nw_scan_req_t oam_son_nw_scan_req_t;
typedef son_oam_set_nw_scan_config_cmd_t oam_son_set_nw_scan_config_cmd_t;
typedef son_oam_periodic_nw_scan_req_t oam_son_periodic_nw_scan_req_t;
typedef son_oam_init_config_resp_t oam_son_init_config_resp_t;
typedef son_oam_shutdown_req_t oam_son_shutdown_req_t;
/*CSR 57877 Code Removed*/
/* SPR_13116_CHANGE_START */
typedef son_oam_get_log_level_req_t oam_son_get_log_level_req_t;
typedef son_oam_get_log_level_resp_t oam_son_get_log_level_resp_t;
/* SPR_13116_CHANGE_END */
typedef son_oam_set_log_level_resp_t oam_son_set_log_level_resp_t;
typedef son_oam_log_enable_disable_resp_t oam_son_log_enable_disable_resp_t;
typedef son_oam_cell_info_ind_t oam_son_cell_info_ind_t;
/* SPR_19619 start */
typedef son_oam_peer_cell_activation_req_t oam_son_peer_cell_activation_req_t;
typedef son_oam_peer_cell_activation_resp_t oam_son_peer_cell_activation_resp_t;
/* SPR_19619 stop */

typedef son_oam_nr_scan_req_t oam_son_nr_scan_req_t;
typedef son_oam_nr_scan_resp_t oam_son_nr_scan_resp_t;
typedef son_oam_nr_add_req_t oam_son_nr_add_req_t;
typedef son_oam_nr_add_resp_t oam_son_nr_add_resp_t;
typedef son_oam_nr_update_req_t oam_son_nr_update_req_t;
typedef son_oam_nr_update_resp_t oam_son_nr_update_resp_t;
typedef son_oam_nr_delete_req_t oam_son_nr_delete_req_t;
typedef son_oam_nr_delete_resp_t oam_son_nr_delete_resp_t;
typedef son_oam_nr_retrieve_req_t oam_son_nr_retrieve_req_t;
typedef son_oam_nr_retrieve_resp_t oam_son_nr_retrieve_resp_t;
typedef son_oam_anr_updated_nrt_info_t oam_son_anr_updated_nrt_info_t;
typedef son_oam_es_enable_req_t oam_son_es_enable_req_t;
typedef son_oam_es_enable_resp_t oam_son_es_enable_resp_t;
typedef son_oam_es_disable_req_t oam_son_es_disable_req_t;
typedef son_oam_es_disable_resp_t oam_son_es_disable_resp_t;
typedef son_oam_es_modify_ue_count_interval_req_t oam_son_es_modify_ue_count_interval_req_t;
typedef son_oam_es_modify_ue_count_interval_resp_t oam_son_es_modify_ue_count_interval_resp_t;
typedef son_oam_es_cell_switch_off_req_t oam_son_es_cell_switch_off_req_t;
typedef son_oam_es_cell_switch_off_resp_t oam_son_es_cell_switch_off_resp_t;
typedef son_oam_es_switched_off_cell_info_resp_t oam_son_es_switched_off_cell_info_resp_t;
typedef son_oam_es_cell_switch_on_req_t oam_son_es_cell_switch_on_req_t;
typedef son_oam_es_active_ue_count_report_t oam_son_es_active_ue_count_report_t;
typedef son_oam_es_modify_ue_count_threshold_req_t oam_son_es_modify_ue_count_threshold_req_t;
typedef son_oam_es_modify_ue_count_threshold_resp_t oam_son_es_modify_ue_count_threshold_resp_t;
typedef son_oam_es_ue_count_threshold_hit_ind_t oam_son_es_ue_count_threshold_hit_ind_t;
typedef son_oam_es_request_to_switch_on_cell_t oam_son_es_request_to_switch_on_cell_t;
typedef son_oam_es_cell_switch_on_resp_t oam_son_es_cell_switch_on_resp_t;
typedef son_oam_mro_enable_req_t oam_son_mro_enable_req_t;
typedef son_oam_mro_enable_resp_t oam_son_mro_enable_resp_t;
typedef son_oam_mro_disable_req_t oam_son_mro_disable_req_t;
typedef son_oam_mro_disable_resp_t oam_son_mro_disable_resp_t;
typedef son_oam_mlb_enable_req_t oam_son_mlb_enable_req_t;
typedef son_oam_mlb_enable_resp_t oam_son_mlb_enable_resp_t;
typedef son_oam_mlb_disable_req_t  oam_son_mlb_disable_req_t;
typedef son_oam_mlb_disable_resp_t oam_son_mlb_disable_resp_t;


/*RELEASE 0.1*/
typedef son_cell_t oam_son_cell_t;
typedef son_speed_state_scale_factors_t oam_son_speed_state_scale_factors_t;
typedef son_meas_config_eutran_t oam_son_meas_config_eutran_t;
typedef son_utran_pci_fdd_range_t oam_son_utran_pci_fdd_range_t;


typedef son_meas_config_utran_tdd_t oam_son_meas_config_utran_tdd_t;
typedef son_meas_config_utran_fdd_t oam_son_meas_config_utran_fdd_t;
typedef son_meas_config_utran_t oam_son_meas_config_utran_t;
typedef son_oam_anr_meas_config_req_t oam_son_anr_meas_config_req_t;
typedef son_oam_anr_meas_config_res_t oam_son_anr_meas_config_res_t;
typedef son_oam_anr_modify_ue_count_req_t oam_son_anr_modify_ue_count_req_t;
typedef son_oam_anr_modify_ue_count_resp_t oam_son_anr_modify_ue_count_resp_t;
typedef son_oam_anr_modify_meas_reporting_interval_req_t oam_son_anr_modify_meas_reporting_interval_req_t;
typedef son_oam_anr_modify_meas_reporting_interval_resp_t oam_son_anr_modify_meas_reporting_interval_resp_t;
/* SPR 20653 Fix Start */
typedef son_oam_anr_modify_attribute_req_t oam_son_anr_modify_attribute_req_t;
typedef son_oam_anr_modify_attribute_resp_t oam_son_anr_modify_attribute_resp_t;
/* SPR 20653 Fix End */
typedef son_neighbor_handover_stats_t oam_son_neighbor_handover_stats_t;
typedef son_utran_neighbor_info_t oam_son_utran_neighbor_info_t;
typedef son_geran_neighbor_info_t oam_son_geran_neighbor_info_t;
typedef son_utran_physical_cell_id_t oam_son_utran_physical_cell_id_t;
typedef son_geran_physical_cell_id_t oam_son_geran_physical_cell_id_t;
typedef son_oam_nr_enb_update_resp_t oam_son_nr_enb_update_resp_t;
typedef son_oam_nr_enb_delete_resp_t oam_son_nr_enb_delete_resp_t;
typedef son_oam_nr_enb_retrieve_resp_t oam_son_nr_enb_retrieve_resp_t;
typedef son_geran_following_arfcns_t oam_son_geran_following_arfcns_t;
typedef son_geran_carrier_freqs_t oam_son_geran_carrier_freqs_t;
typedef son_geran_carrier_freq_info_t oam_son_geran_carrier_freq_info_t;
typedef son_meas_config_geran_t oam_son_meas_config_geran_t;
typedef son_anr_meas_config_inter_rat_t oam_son_anr_meas_config_inter_rat_t;

/* SPR-11590_START */
typedef son_oam_pci_conflict_enable_disable_req_t oam_son_pci_conflict_enable_disable_req_t;
typedef son_oam_pci_conflict_enable_disable_resp_t oam_son_pci_conflict_enable_disable_resp_t;
/* SPR-11590_END */
typedef son_global_enb_id_t oam_son_global_enb_id_t;
typedef son_enb_status_t oam_son_enb_status_t;
typedef son_oam_nr_enb_add_resp_t oam_son_nr_enb_add_resp_t;
typedef son_enb_tnl_address_t oam_son_enb_tnl_address_t;
typedef son_neighbor_enb_info_t oam_son_neighbor_enb_info_t;
typedef son_oam_x2_link_up_ind_t oam_son_x2_link_up_ind_t;
typedef son_oam_x2_link_down_ind_t oam_son_x2_link_down_ind_t;
typedef son_fdd_info_t oam_son_fdd_info_t;
typedef son_special_subframe_info_t oam_son_special_subframe_info_t;
typedef son_tdd_info_t oam_son_tdd_info_t;
typedef son_choice_eutra_mode_t oam_son_choice_eutra_mode_t;
/*CHANGE_1_3_1*/
typedef son_oam_nr_enb_retrieve_req_t oam_son_nr_enb_retrieve_req_t;
typedef son_oam_nr_enb_delete_req_t oam_son_nr_enb_delete_req_t;
typedef son_oam_nr_enb_update_req_t oam_son_nr_enb_update_req_t;
typedef son_oam_nr_enb_add_req_t oam_son_nr_enb_add_req_t;
/*eICIC_PHASE_1_2_CHANGES_START*/
typedef son_oam_anr_neighbor_default_config_req_t oam_son_anr_neighbor_default_config_req_t;
/*eICIC_PHASE_1_2_CHANGES_END*/
typedef son_neighbor_removal_info_t oam_son_neighbor_removal_info_t;
typedef son_oam_anr_modify_removal_attributes_req_t oam_son_anr_modify_removal_attributes_req_t;
typedef son_neighbor_pci_confusion_info_t oam_son_neighbor_pci_confusion_info_t;
typedef son_oam_anr_modify_pci_confusion_cfg_req_t oam_son_anr_modify_pci_confusion_cfg_req_t;
typedef son_oam_anr_enable_disable_limited_mode_req_t oam_son_anr_enable_disable_limited_mode_req_t;
typedef son_limited_mode_eutran_cell_info_t oam_son_limited_mode_eutran_cell_info_t;
typedef son_oam_anr_limited_mode_pci_info_t oam_son_anr_limited_mode_pci_info_t;
typedef son_oam_anr_modify_removal_attributes_resp_t oam_son_anr_modify_removal_attributes_resp_t;
typedef son_oam_anr_modify_pci_confusion_cfg_resp_t oam_son_anr_modify_pci_confusion_cfg_resp_t;
typedef son_oam_anr_enable_disable_limited_mode_resp_t oam_son_anr_enable_disable_limited_mode_resp_t;

/*CHANGE_1_3_1*/
typedef son_oam_pci_selection_req_t oam_son_pci_selection_req_t;
typedef son_oam_pci_selection_resp_t oam_son_pci_selection_resp_t;
typedef son_oam_pci_reselection_req_t oam_son_pci_reselection_req_t;
typedef son_oam_pci_reselection_resp_t oam_son_pci_reselection_resp_t;
typedef son_oam_pci_conflict_confusion_ind_t oam_son_pci_conflict_confusion_ind_t;
typedef son_oam_carrier_freq_selection_req_t oam_son_carrier_freq_selection_req_t;
typedef son_oam_carrier_freq_selection_resp_t oam_son_carrier_freq_selection_resp_t;

/*SON 2.0*/
/*CHANGES 2.2*/
typedef son_oam_mlb_modify_attributes_req_t oam_son_mlb_modify_attributes_req_t;
typedef son_oam_mlb_modify_attributes_resp_t oam_son_mlb_modify_attributes_resp_t;
typedef son_oam_feature_state_change_ind_t  oam_son_feature_state_change_ind_t;
typedef son_oam_mro_modify_config_params_req_t oam_son_mro_modify_config_params_req_t;
typedef son_oam_mro_modify_config_params_resp_t oam_son_mro_modify_config_params_resp_t;
typedef son_oam_mro_eutran_ttt_update_ind_t oam_son_mro_eutran_ttt_update_ind_t ;
/*CHANGES 2.2*/
typedef son_oam_anr_enable_req_t oam_son_anr_enable_req_t;
typedef son_oam_anr_enable_resp_t oam_son_anr_enable_resp_t;
typedef son_oam_anr_disable_req_t oam_son_anr_disable_req_t;
typedef son_oam_anr_disable_resp_t oam_son_anr_disable_resp_t;
typedef son_oam_nr_delete_from_remove_list_req_t        oam_son_nr_delete_from_remove_list_req_t;
typedef son_oam_nr_delete_from_remove_list_resp_t       oam_son_nr_delete_from_remove_list_resp_t; 
/* SPR-8158 Start */
typedef son_oam_anr_meas_config_obj_remove_req_t        oam_son_anr_meas_config_obj_remove_req_t;
typedef son_oam_anr_meas_config_obj_remove_res_t        oam_son_anr_meas_config_obj_remove_res_t;
/* SPR-8158 End */
typedef son_oam_enable_disable_x2_req_t oam_son_enable_disable_x2_req_t;
typedef son_oam_enable_disable_x2_resp_t oam_son_enable_disable_x2_resp_t;
typedef son_oam_es_autonomous_switch_off_config_req_t oam_son_oam_es_autonomous_switch_off_config_req_t;
typedef son_oam_es_autonomous_switch_off_config_resp_t oam_son_es_autonomous_switch_off_config_resp_t;
typedef son_oam_mro_optimization_notification_t oam_son_mro_optimization_notification_t;
#ifdef OAM_SON_ENABLE

typedef son_oam_rach_opt_enable_req_t oam_son_rach_opt_enable_req_t;
typedef son_oam_rach_opt_disable_req_t oam_son_rach_opt_disable_req_t;
typedef son_oam_rach_opt_modify_rach_optmode_req_t oam_son_rach_opt_modify_rach_optmode_req_t;
typedef son_oam_rach_opt_enable_resp_t oam_son_rach_opt_enable_resp_t;
typedef son_oam_rach_opt_disable_resp_t oam_son_rach_opt_disable_resp_t;
typedef son_oam_rach_opt_modify_rach_optmode_resp_t oam_son_rach_opt_modify_rach_optmode_resp_t;
typedef son_oam_rach_opt_modify_config_params_req_t oam_son_rach_opt_modify_config_params_req_t;
typedef son_oam_rach_opt_modify_config_params_resp_t oam_son_rach_opt_modify_config_params_resp_t;
typedef son_oam_rach_opt_parameter_update_req_t oam_son_rach_opt_parameter_update_req_t;
typedef son_oam_rach_opt_ue_info_report_t oam_son_rach_opt_ue_info_report_t;
typedef son_oam_rach_opt_failure_notification_t oam_son_rach_opt_failure_notification_t;
typedef son_oam_rach_opt_l2_perf_report_t oam_son_rach_opt_l2_perf_report_t;

/* SPR 14651 START */
/* SPR 18314 START */
/* Code  deleted */
/* SPR 18314 END */
/* SPR 14651 END */
#endif
/* SPR 13065 CHANGE START */
/* SPR 13065 CHANGE END */
typedef son_oam_carrier_freq_and_dl_tx_power_req_t oam_son_carrier_freq_and_dl_tx_power_req_t;
typedef son_oam_carrier_freq_and_dl_tx_power_resp_t oam_son_carrier_freq_and_dl_tx_power_resp_t;
#endif
/*SON 2.0*/

#define OAM_MINUS_ONE -1

typedef enum
{
    OAM_FAILURE = 0,
    OAM_SUCCESS
} oam_return_et;

typedef enum
{
    OAM_INITIALIZATION_ONGOING,
    OAM_INITIALIZATION_COMPLETE,
    OAM_CHANGE_ADMIN_TO_UNLOCK,
    OAM_CHANGE_SYSTEM_STATE_FOR_MAJOR_ALARM,
    OAM_CHANGE_SYSTEM_STATE_FOR_CRITICAL_ALARM,
    OAM_CHANGE_SYSTEM_STATE_FOR_CLEAR_MAJOR_ALARM,
    OAM_CHANGE_SYSTEM_STATE_FOR_CLEAR_CRITICAL_ALARM,
    OAM_CHANGE_ADMIN_TO_LOCK
} oam_system_state_event_et;

typedef enum
{
    SIM_NO_ERROR,
    SIM_FORK_FAILED,
    SIM_BNRY_NOT_FOUND,
    SIM_NOT_ENGH_MEM_EXEC,
    SIM_GENERAL_ERROR_EXEC
}oam_sim_error_info_et;

/*CSR 57877 Code Removed*/

typedef enum
{
    OAM_UMTS,
    OAM_LTE,
    OAM_CDMA1x,
    OAM_CDMAHRPD
}supported_system_et;

typedef enum _log_priority_et
{
    OAM_CRITICAL = LTE_LOG_CRITICAL,        /* Critical conditions, such as hard device errors */
    OAM_ERROR = LTE_LOG_ERROR,              /* Error messages */
    OAM_WARNING = LTE_LOG_WARNING,          /* Warning messages */
    OAM_INFO = LTE_LOG_INFO,                /* Info about the resources*/
    OAM_BRIEF = LTE_LOG_BRIEF,              /* Brief about the resources*/
    OAM_DETAILED = LTE_LOG_DETAILED,        /* Detail about all the messg */
    OAM_DETAILEDALL = LTE_LOG_DETAILEDALL,  /* Detail including comp parse logs about all the messg */
} log_priority_et;
#define OAM_COMP_PARSE OAM_DETAILEDALL 
typedef struct _oam_loglevel_id_t
{
    const UInt8* string_id;
    UInt64 string_val;
} oam_loglevel_id_t;\

    typedef enum {
        ENODEB_ADMIN_STATE_LOCKED = 0,	
        ENODEB_ADMIN_STATE_UNLOCKED
    }oam_admin_state_et;

typedef enum {
    ENODEB_AVAIL_STATE_INITIALIZING = 0,	
    ENODEB_AVAIL_STATE_AVAILABLE,
    ENODEB_AVAIL_STATE_DEGRADED,
    ENODEB_AVAIL_STATE_FAILED
}oam_avail_state_et;

typedef enum {
    ENODEB_OP_STATE_DISABLED = 0,
    ENODEB_OP_STATE_ENABLED = 1
}oam_op_state_et;

extern oam_loglevel_id_t oam_loglevel_table[];

typedef enum
{
    L2_INIT_ONGOING = 0,
    L2_INIT_COMPLETE,
    L3_INIT_ONGOING,
    L3_INIT_COMPLETE
}oam_main_state_et;


typedef enum {
    L2_ONGOING = 0,
    L2_COMPLETE,
    WAIT_FOR_RRC_OAM_COMN_INFO_RESP,
    WAIT_FOR_RRC_OAM_PROV_RESP,
    RRC_INIT_COMPLETE,
    WAIT_FOR_S1AP_OAM_PROV_RESP,
    WAIT_FOR_X2AP_OAM_PROV_RESP,
    S1AP_INIT_COMPLETE,
    X2AP_INIT_COMPLETE
} oam_sub_state_et;

typedef struct _oam_l2_init_status_t
{
    oam_sub_state_et       mac_state;
    oam_sub_state_et       rlc_state;
    oam_sub_state_et       pdcp_state;
} oam_l2_init_status_t;


typedef struct _oam_status_t
{
    oam_main_state_et      oam_state;
    oam_l2_init_status_t   oam_l2_init;
}oam_status_t;

typedef enum
{
    MODE_TCP,
    MODE_UDP
} mode_et;

typedef struct _com_params_t
{
    mode_et                 mode;
    U8                 port[MAX_PORT_LENGTH];
    U8                 ipaddr[MAX_IP_ADD_LEN_STR];
} com_params_t;

typedef struct _oam_ext_com_config_t
{
    com_params_t             oam_com_params;
    com_params_t             son_com_params;
    com_params_t             rrc_s1ap_com_params;
    /* NMM_AB+ */
    com_params_t  nmm_com_params;
    /* NMM_AB- */
    com_params_t    sdm_com_params; /* SDM Info */
    com_params_t    snmp_com_params; /* SNMP Info */
} oam_ext_com_config_t;

typedef struct __oam_multi_l2_info_t {
    char l2_ip_addr[20];
    int pdcp_port;
    int rlc_port;
    int mac_port;
    int gtpu_port;
}oam_multi_l2_info_t;

typedef struct __oam_cli_comm_info_t {
    char oam_ip_addr[20];
    int oam_port;
    char cli_ip_addr[20];
    int cli_port;
#ifdef OAM_RADIO_HEAD_SUPPORT
    char rrh_ip_addr[20];
    int rrh_port;
#endif
    char rrm_ip_addr[20];
    int rrm_port;
    char son_ip_addr[20];
    int son_port;
    char l3_ip_addr[20];
    int l3_port;

    oam_multi_l2_info_t l2_socket[OAM_MAX_CELL_SUPPORTED];

    char tr069_ip_addr[20];
    int tr069_port;
    char snmp_ip_addr[20];
    int snmp_port;
} oam_cli_comm_info_t;


typedef struct _prov_req_t
{
    /* File pointer for schema log file*/
    FILE                                *p_schema_log;
    /* File pointer for loging*/
    FILE                                *p_out;
    /* File pointer for loging*/
    FILE                                *p_fp;
    /* Log level for Loging*/
    UInt8                               log_level;
    oam_rrc_communication_info_req_t    commn_info;
    UInt8 buff[96];
} prov_request_t;

/* SPR_13116_CHANGE_START */


typedef struct get_log_level_oam_resp_t
{
     UInt8                               log_level;
}get_log_level_oam_resp;

/* SPR_13116_CHANGE_END */


typedef struct _oam_logcategory_id_t
{
    const UInt8* string_id;
    UInt64 string_val;
} oam_logcategory_id_t;

typedef struct
{
    UInt16 id;
    UInt16 src_id;
    UInt16 dest_id;
    UInt16 api_id;
    UInt16 trans_id;
    UInt16 size;
} oam_qcli_header;

extern oam_logcategory_id_t oam_logcategory_table[];
typedef struct RlcOamCnfT Oam_Rlc_Cnf_t;

typedef  lteRlcUEStatusReq oam_rlc_ue_status_req_t;

typedef lteRlcUEStatusCnf oam_Rlc_UE_Status_Cnf_t; 

typedef struct lteRlcGetStatCnfT Oam_Rlc_Get_Stat_Cnf_t;

/*CSR 57877 Code Removed*/

typedef RlcOamSetLogLevelReq Oam_LogLevel_Req_t;


typedef struct
{
    oam_mac_init_req_params_t mac_init_req;
    oam_mac_log_level_req_t mac_set_log_level_req;
    oam_mac_modify_req_params_t mac_modify_req; 
    oam_mac_init_cell_req_params_t mac_init_cell_req[OAM_MAX_CELL_SUPPORTED - 1];
    oam_mac_get_log_level_resp_t mac_get_log_level_resp;
} oam_mac_req_t;

typedef struct
{
    oam_rlc_init_layer_req_t rlc_init_req;
} oam_rlc_req_t;

typedef struct
{
    oam_pdcp_init_layer_req_t pdcp_init_req;
} oam_pdcp_req_t;

UInt8 eGtpu_Ipv4Addr[8][4];
UInt8 eGtpu_Ipv6Addr[8][16];

typedef struct
{
    oam_egtpu_initdb_req_t egtpu_initdb_req;
} oam_egtpu_req_t;

/*SPR 21889 Start*/
#define OAM_MAX_SPAWN_RETRY_COUNT   2
typedef struct _oam_agent_fsm_ctxt
{
    oam_timer_t     req_guard_timer; /* Timer started when a request is sent to the particular instance */
    UInt8           main_state; /* Main state of the particular agent (oam_agent_fsm_main_state_et) */
    UInt8           procdre_state; /* Procedure ongoing with the agent (oam_agent_fsm_procdre_state_et) */
    UInt8           spawn_retry_cnt; /* Number of time spawn request is sent */
}oam_agent_fsm_ctxt_t;

typedef struct _oam_agnt_info
{
    oam_agent_fsm_ctxt_t    agent_fsm_ctxt;
    Char8                   bin_path[OAM_MAX_DIR_PATH];
}oam_agnt_info_t;
/*SPR 21889 End */

typedef struct
{
    oam_mac_req_t oam_mac_req;
    oam_rlc_req_t oam_rlc_req;
    oam_pdcp_req_t oam_pdcp_req;
    oam_egtpu_req_t oam_egtpu_req;
    /*SPR 21889 Start*/
    oam_agnt_info_t oam_agnt_info;
    /*SPR 21889 End */
} oam_l2_req_t;

typedef struct
{
    oam_rrc_provision_req_t rrc_oam_prov;
    oam_rrc_trace_file_transfer_complete_ind_t rrc_trace_file_transfer_complete_ind;
/*CSR 57877 FIX Start*/
    rrc_oam_cell_traffic_trace_start_t rrc_cell_traffic_trace_start; 
/*CSR 57877 FIX End*/
} oam_rrc_req_t;

typedef struct
{
    oam_s1ap_provision_req_t s1ap_oam_prov;
    oam_s1ap_enb_config_update_t enb_config_update;
} oam_s1ap_req_t;


/* LWA changes start*/
/* LWA changes end*/

/* SPR-16334 START */

#define OAM_MAX_ENB_POOLS       32

#define OAM_MCC_OCTET_SIZE      3
#define OAM_MAX_MNC_OCTET_SIZE  3
/*! \  oam_cell_plmn_info_t
 *  \brief   This structure stores the PLMN info
 *  \param   mcc[SON_MCC_OCTET_SIZE]      Mobile Country Code
 *  \param   num_mnc_digit                This can be 2 or 3
 *  \param   mnc[SON_MAX_MNC_OCTET_SIZE]  Mobile Network Code
 */
typedef struct _oam_cell_plmn_info_t
{
    UInt8         mcc[OAM_MCC_OCTET_SIZE];
    UInt8         num_mnc_digit;
    UInt8         mnc[OAM_MAX_MNC_OCTET_SIZE];
}oam_cell_plmn_info_t;

typedef enum
{
  OAM_HOME_ENB = 1,        /*!< Home ENB Id */
  OAM_MACRO_ENB,           /*!< macro eNB Id */
  OAM_ENB_TYPE_UNDEFINED = 255
}oam_enb_type_et;


/*! \ oam_global_enb_id_t
 *  \brief    Structure to define the global eNB Id 
 *  \param    enb_type - eNB Type i.e. HOME or MACRO
 *  \param    plmn_id  - Public Land Mobile Network Identifier
 *  \param    enb_id   - 28 bits Home eNB id or 20 bits Macro eNB id.
 *                       If eNB type is SON_HOME_ENB, this field is
 *                       equal to the 28 leftmost bits of the Cell
 *                       Identity contained in the E-UTRAN CGI of
 *                       the cell served by the eNB.If eNB type is
 *                       SON_MACRO_ENB, this field is equal to the 20
 *                       leftmost bits of the Cell Identity contained
 *                       in the E-UTRAN CGI of each cell served by the eNB.
 */
#define OAM_ENB_ID_OCTET_SIZE   4
 typedef struct _oam_global_enb_id_t
 {
    oam_enb_type_et        enb_type;

    oam_cell_plmn_info_t   plmn_id;

    U8                     enb_id[OAM_ENB_ID_OCTET_SIZE];
 }oam_global_enb_id_t;

#define IPV4_ADD_PRESENT        0x01
#define IPV6_ADD_PRESENT        0x02

#define OAM_IPV4_SIZE                       4
#define OAM_IPV6_SIZE                       16

/*! \ son_enb_tnl_address_t
 *  \brief    Structure to define the Transport Layer Address 
 *  \param    bitmask - To indicate if the optional fields are present or not 
 *                      Only one of this bit should be set.Bit 1: ipv4_add and
 *                      Bit 2: ipv6_add
 *  \param    ipv4_add - 32bits IPV4 IP Address.
 *  \param    ipv6_add - 128bits IPV6 IP Address.
 */
 typedef struct _oam_enb_tnl_address_t
{
   U32 bitmask;
   U8    ipv4_add[OAM_IPV4_SIZE];
   /*IPV4_ADD_PRESENT*/
   U8    ipv6_add[OAM_IPV6_SIZE];
   /*IPV6_ADD_PRESENT,OCTET_STRING*/
 }oam_enb_tnl_address_t;

/* SPR-16334 END */


typedef struct
{
    oam_x2ap_provision_req_t x2ap_oam_prov;
    /* SPR-16334 START */
    oam_x2ap_add_enb_req_t      oam_x2ap_add_enb_req;
    UInt32 bitmap_x2_enb; 
    oam_x2ap_enb_config_update_req_t enb_config_update_req;
    UInt8 action;
    /* SPR-16334 END */
} oam_x2ap_req_t;

typedef struct
{
    UInt32             status;  
    UInt32             in_streams; 
    UInt32             out_streams;
    UInt32             local_port; 
    rrc_timeval_t    start_time; 
    UInt8              primary_peer_addr[MAX_IP_ADDRESS_LENGTH]; 

}oam_sctp_assoc_params_t;

typedef struct
{
    UInt8  enable;

    /* LWA changes start*/
        oam_sctp_assoc_params_t sctp_assoc_params[MAX_NUM_MME + MAX_PEER_ENB];
    /* LWA changes end*/

    UInt16 current_no_of_s1ap_conns;
    UInt16 current_no_of_x2ap_conns;
}oam_sctp_params_t;

typedef struct
{
    UInt8 num_mme_id;
    UInt8 mme_id[MAX_NUM_MME];

}oam_mme_data_t;

#ifdef ENDC_ENABLED
#define OAM_GB_ENB_ID_PRESENT    0x01
#define OAM_GB_GNB_ID_PRESENT    0x02
#endif
typedef struct
{
#ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    UInt8             bitmask;

    x2_gb_gnb_id_t    gNB_id[OAM_MAX_NUM_PEER_ENB]; 
    /* MENB CHANGES - END */
#endif

    UInt8 num_peer_enb;
    x2_gb_enb_id_t eNB_id[OAM_MAX_NUM_PEER_ENB]; 
      /* SPR-16334 START */
    x2_enb_comm_info_t enb_comm_info[OAM_MAX_ENB_POOLS];
    /* SPR-16334 END */
}oam_peer_enb_data_t;

typedef struct
{
    oam_rrc_req_t oam_rrc_req;
    oam_s1ap_req_t oam_s1ap_req;
    oam_x2ap_req_t oam_x2ap_req;
    oam_sctp_params_t sctp_params;
    oam_mme_data_t mme_data;
    oam_peer_enb_data_t peer_enb_data;
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    oam_m2ap_req_t oam_m2ap_req;
#endif
    /* EMBMS Changes End */
    /* LWA changes start*/
    /* LWA changes end*/

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    oam_peer_enb_data_t    peer_gnb_data;
    /* MENB CHANGES - END */
    #endif
}oam_l3_req_t;

/* This structure is maintained to supplement the missing TR-196 PLMNList
 * parameters (viz. Enable, IsPrimary) in rrm structure
 */
typedef struct {
    /* Specifies if the entry is valid or not */
    UInt8                       is_primary;

    /* Maps to Enable field */
    UInt8                       enable;

    /* Cell Reserved for Operator Use */
    rrm_oam_cell_reserved_for_operatoruse_et  reserve_operator_use;

    /* PLMN ID */
    rrm_oam_cell_plmn_info_t    plmn_id;
} oam_rrm_plmn_data_t;

typedef struct
{
   UInt16 monitoring_interval;
   UInt16 nonresponsive_monitoring_interval;
   UInt8 max_retries; 
}oam_hm_config_param_t;

typedef struct __oam_rrm_cell_plmnlist_data_t {
    UInt8                   cid;
    UInt16                  max_plmn_entries;
    UInt16                  curr_plmn_entries;
    UInt8                   plmnlist_cache_idx;
    oam_rrm_plmn_data_t     plmnlist_cache;
    UInt32                  is_valid_bitmap;
    oam_rrm_plmn_data_t     plmn_entries[RRM_OAM_MAX_NUM_PLMNS];
    UInt8                   plmnlist_query_idx;
    UInt8                   last_added_instance_id;
} oam_rrm_cell_plmnlist_data_t;

/* SPR_20467_FIX_START */
/* This structure is used to store the enb_config_update response
 * received for all the connected mme's, total number of mme's
 * for which response has been received and store that the
 * triggered ecu is new ecu or old ecu.
 */
typedef struct __oam_s1ap_ecu_resp_data_t
{
    UInt8                   ecu_success_resp_received;
    UInt8                   ecu_resp_received[MAX_NUM_MME];
    UInt8                   mme_id_response_cnt;
    UInt8                   old_ecu_sent_flag;
}oam_s1ap_ecu_resp_data_t;
/* SPR_20467_FIX_END */

/* SPR 15527 START */
typedef struct {
  UInt8 is_valid_entry;
    oam_bool_et qos_enable;
    rrm_oam_qos_config_params_t qos_qci_info;
    oam_bool_et drx_enabled;
    rrm_oam_drx_config_t        drx_per_qci_info;
}oam_qos_config_params_t;

typedef struct __oam_rrm_qos_data_t {
    UInt16                  max_qos_entries;
    UInt16                  num_qos_entries;
    oam_qos_config_params_t qos_obj_list[RRM_OAM_MAX_NUM_QCI_CLASSES];
    UInt16                  last_added_qos_instance_id;
    UInt8                   qos_query_idx;
} oam_rrm_qos_data_t;
/* SPR 15527 END */


typedef struct
{
    oam_rrm_init_config_req_t  oam_rrm_init_config_req;
    oam_rrm_cell_config_req_t  oam_rrm_cell_config_req[OAM_MAX_CELL_SUPPORTED];
/*CSR 57877 FIX Start*/
    rrm_oam_log_enable_disable_req_t oam_rrm_log_enable_disable_req;
    rrm_oam_set_log_level_req_t oam_send_oam_rrm_set_log_level_req;
/*CSR 57877 FIX End*/
    oam_rrm_set_log_level_resp_t   oam_send_oam_rrm_set_log_level_resp;
    oam_rrm_cell_config_resp_t     oam_send_oam_rrm_cell_config_resp;
    oam_rrm_get_ver_id_resp_t      oam_send_oam_rrm_ver_id_resp;
    /*SPR 13518 START*/
    oam_rrm_cell_block_req_t    rrm_cell_block_req;
    /*SPR 13518 START*/
    /* Added for ADD/Delete Object support */
    oam_rrm_cell_plmnlist_data_t plmnlist_data[OAM_MAX_CELL_SUPPORTED];
    /* XML CODE CHANGE START*/
    oam_rrm_cell_ecn_capacity_enhance_req_t   oam_ecn_req;
    UInt8 enable_load_config;
    oam_rrm_load_config_req_t                 oam_load_config_req;
    /*  XML CODE CHANGE END */
    oam_rrm_enb_config_req_t oam_rrm_enb_config_req;
    oam_rrm_enb_config_resp_t oam_rrm_enb_config_resp;
    /* SPR 15527 START */
    oam_rrm_qos_data_t qos_data[OAM_MAX_CELL_SUPPORTED];
    /* SPR 15527 END */

} oam_rrm_req_t;

#ifdef OAM_SON_ENABLE
/*SON ENERGY SAVING*/

typedef struct {
    UInt8                         cid;
    UInt16                        MaxCarrierEntries;
    /* RT SPR 9019 FIX START */
    UInt16                        max_utran_fdd_freq;
    UInt32 			              utran_tdd_meas_entry_bitmap;
    /* RT SPR 9019 FIX END */
    /* RT SPR 8154 FIX START */
    UInt16                        max_geran_freq_entries;
    /* RT SPR 8154 FIX END */
    UInt32 			              eutran_meas_entry_bitmap;
    /* RT SPR 9019 FIX START */
    UInt32 			              utran_fdd_meas_entry_bitmap;
    /* RT SPR 9019 FIX END */
    /* RT SPR 8154 FIX START */
    UInt32 			              geran_meas_entry_bitmap;
    /* RT SPR 8154 FIX END */
    oam_son_anr_meas_config_req_t oam_son_anr_meas_config_req;

    /* OAM BCOM Code Comment Changes Start */
    UInt8                         is_valid_inter_freq[RRM_OAM_MAX_NUM_INTER_FREQ_CARRIERS];    
    /* OAM BCOM Code Comment Changes End */

    /* ADD/DELETE Support, Array index + 1 is the unique instance id */
    UInt8                         eutran_last_inst_id;
    UInt8                         utran_fdd_last_inst_id;
    UInt8                         geran_freq_group_last_inst_id;
    /* RT SPR 8154 FIX START */
    SInt8                         query_index;
    /* RT SPR 8154 FIX END */
    UInt8                         multi_parent_req;

    /* OAM BCOM Code Comment Changes Start */
    /* These Structure holds the enable field for meas object */
    oam_son_meas_obj_enable_t     eutran_meas_enable[RRM_OAM_MAX_NUM_INTER_FREQ_CARRIERS];
    oam_son_meas_obj_enable_t     utran_meas_enable[RRM_MAX_NUM_UTRA_FDD_CARRIERS];
    oam_son_meas_obj_enable_t     geran_meas_enable[MAX_GERAN_FREQ];
    /* OAM BCOM Code Comment Changes End */
    /* A bitmask to indicate ongoing event on this structure */
    
    UInt8                         config_ongoing;
} oam_son_inter_freq_carrier_t;

typedef struct
{
    UInt8 auto_switch_on_off;
    UInt8 es_enable_disable;
    UInt8 EnergySavingControl;
    UInt16 ActiveUECountIntervalSeconds;
    UInt8 ActiveUECountThreshold;
    UInt8 CellSwitchOffMode;
} EnergySavingConfig_t;

#define OAM_TEMP_EARFCN_LIST_PRESENT 0x01
#define OAM_TEMP_CARRIER_ARFCN_DL_PRESENT 0x02
/*SPR 12001 FIX START*/
#define OAM_TEMP_PCI_LIST_PRESENT 0x04
/*SPR 12001 FIX END*/
typedef struct oam_son_nw_scan_params_st
{
    Char8 temp_bitmask;
    Char8 nw_scan_pci_list[OAM_SON_NW_SCAN_REQ_PCI_STR_LEN];
    Char8 nw_scan_earfcn_list[OAM_SON_NW_SCAN_REQ_EARFCN_STR_LEN];
    Char8 nw_scan_plmn_list[OAM_SON_NW_SCAN_REQ_PLMN_STR_LEN];
    UInt8 scan_on_boot;
    /* OAM BCOM Code Comment Changes Start */
    UInt8 CarrierChWidth;
    UInt16 CarrierARFCNDL;
    /* OAM BCOM Code Comment Changes End */
}oam_son_nw_scan_params_t;

typedef struct __oam_son_neigh_list_in_use_t {
    /* Following are two bitmaps for valid entries in nrt info structure */
    UInt16 cid; 
    UInt32 intra_rat_neigh_valid_bitmap;
    /* SPR-13586 START */
    UInt32 inter_rat_umts_neigh_valid_bitmap;
    UInt32 inter_rat_gsm_neigh_valid_bitmap;
    /* Following 4 num values just represent their TR-196 counter parts */
    UInt32 inter_rat_umts_neigh_size;
    UInt32 inter_rat_gsm_neigh_size;
    UInt16 max_lte_cell_entries;
    UInt16 max_umts_entries;
    UInt16 max_gsm_entries;
    /* SPR-13586 END */
/* Spr 15438 Fix Start*/
    UInt8                         intra_rat_last_added_instance_id;
    UInt8                         intra_rat_is_valid[32];
    UInt8                         inter_rat_last_added_instance_id;
    UInt8                         inter_rat_is_valid[32];
/* Spr 15438 Fix End*/

    Char8 last_nrt_upd_time[NRT_INFO_MAX_TIMESTAMP_LEN];
    UInt8 nr_list_query_idx;
    oam_son_anr_updated_nrt_info_t nrt_info;
    /* SPR 15604 START */
    /* There Should Be Direct Mapping With NLIU neighbour index */
/* SPR 18930  FIX START */
    /* Code Removed */
/* SPR 18930  FIX END */
    oam_bool_et is_detected_intra_neighbour[SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS];
/* Spr 15438 Fix End*/
/* Spr 15612  Fix Start*/
    oam_bool_et is_detected_utran_neighbour[SON_MAX_NO_INTER_RAT_NEIGHBOUR_CELLS];
    
/* SPR 18930  FIX START */
    /* Code Removed */
/* SPR 18930  FIX END */

    /* SPR 15604 END */

} oam_son_neigh_list_in_use_t;

typedef struct _mro_config_t
{
    UInt8 mro_enable_disable_flag;
}mro_config_t;

typedef struct _mlb_config_t
{
    UInt8 mlb_enable_disable_flag;
}mlb_config_t;

/* SPR 18930  FIX START */
typedef struct _oam_saved_son_nr_add_req_t
{
    UInt16 trans_id;
    oam_son_nr_add_req_t * p_nr_add_req;
}oam_saved_son_nr_add_req_t;
#define OAM_MAX_NR_ADD_TRANS 4
/* SPR 18930  FIX END */
typedef struct {
    UInt8    cid;
    /* Parameters for inter and intra rat cells */
    /* SPR-13586 START */
    UInt8                         inter_rat_umts_last_added_instance_id;
    UInt8                         inter_rat_gsm_last_added_instance_id;
    UInt8                         intra_rat_last_added_instance_id;

    UInt16   max_nr_intra_entries;
    UInt16   max_nr_umts_inter_entries;
    UInt16   max_nr_gsm_inter_entries;
    UInt16   inter_rat_umts_list_size;
    UInt16   inter_rat_gsm_list_size;


    /* This specifies the index number of the neighbor in the nr_list for 
     * requested operation
     */
    UInt8 nr_list_query_idx;

    /* a bitmap to identify the valid entries in neighbor list */
    UInt32 intra_rat_nr_entry_bitmap;

    UInt32 inter_rat_umts_entry_bitmap;
    UInt32 inter_rat_gsm_entry_bitmap;
/* SPR 18930  FIX START */
    UInt32 inter_rat_cdma_entry_bitmap;
/* SPR 18930  FIX END */
    /* SPR-13586 END */
    oam_son_nr_add_req_t        oam_son_nr_add_req;
/* SPR 18930  FIX START */
    oam_son_nbr_ctxt_t  eutran_nbr_ctxt[OAM_SON_MAX_EUTRAN_NEIGHBORS];
    oam_son_nbr_ctxt_t  utran_nbr_ctxt[OAM_SON_MAX_UTRAN_NEIGHBORS];
    oam_first_nr_add_state_et first_nr_add_state;
    UInt8 curr_num_of_nr_add_req_sent;
    UInt8 bitmap_for_nr_add_req_transactions;
    oam_saved_son_nr_add_req_t *p_saved_nr_add_req[OAM_MAX_NR_ADD_TRANS];
/* SPR 18930  FIX END */
} oam_cell_neighbourlist_info_t;


typedef struct
{
    UInt8 rachopt_enable_disable_flag;
    UInt8 rachopt_mode;	
}rachopt_config_t;


typedef struct __oam_son_earfcn_pci_selection_t {
    UInt8 cid;
    /* SPR 13065 CHANGE START */
    /* SPR 13065 CHANGE END */
    oam_son_carrier_freq_and_dl_tx_power_req_t
                             oam_son_carrier_freq_and_dl_tx_power_req;
    oam_son_pci_selection_req_t             oam_son_pci_selection_req;
    oam_son_pci_selection_resp_t           oam_son_pci_selection_resp;
    oam_son_pci_reselection_req_t          oam_son_pci_reselection_req;
} oam_son_earfcn_pci_selection_t;


typedef struct
{
    oam_son_init_config_req_t oam_son_init_config_req;
    oam_son_shutdown_req_t    oam_son_shutdown_req;
/*CSR 57877 FIX Start*/
    son_oam_set_log_level_req_t oam_son_set_log_level_req;
/*CSR 57877 FIX End*/
    /* SPR 21702 Start*/
    oam_son_cell_info_ind_t     oam_son_cell_info_ind[OAM_MAX_CELL_SUPPORTED];
    /* SPR 21702 End*/
    EnergySavingConfig_t oam_energy_saving_config[OAM_MAX_CELL_SUPPORTED];
    oam_son_nr_delete_from_remove_list_req_t    nr_delete_from_remove_list;
    /* SPR 11156 FIX START */
    /* CODE DELETED */
    /* SPR 11156 FIX END */
    oam_son_anr_modify_ue_count_req_t   oam_son_anr_modify_ue_count_req[OAM_MAX_CELL_SUPPORTED] ;
    oam_son_anr_modify_meas_reporting_interval_req_t    oam_son_anr_modify_meas_reporting_interval_req[OAM_MAX_CELL_SUPPORTED] ;
    oam_son_anr_modify_removal_attributes_req_t oam_son_anr_modify_removal_attributes_req[OAM_MAX_CELL_SUPPORTED] ;
    oam_son_anr_modify_pci_confusion_cfg_req_t  oam_son_anr_modify_pci_confusion_cfg_req[OAM_MAX_CELL_SUPPORTED] ;
    /* SPR 20653 Fix Start */
    oam_son_anr_modify_attribute_req_t          oam_son_anr_modify_attribute_req[OAM_MAX_CELL_SUPPORTED];
    /* SPR 20653 Fix End */
    oam_son_anr_enable_disable_limited_mode_req_t       oam_son_anr_enable_disable_limited_mode_req ;
    oam_son_nr_enb_add_req_t    oam_son_nr_enb_add_req ;
    /*eICIC_PHASE_1_2_CHANGES_START*/
    oam_son_anr_neighbor_default_config_req_t oam_son_anr_neighbor_default_config_req;
    /*eICIC_PHASE_1_2_CHANGES_END*/
    oam_son_pci_reselection_resp_t oam_son_pci_reselection_resp; 
    oam_son_pci_conflict_confusion_ind_t oam_son_pci_conflict_confusion_ind;
    /* BUG_11590_START*/
    oam_son_pci_conflict_enable_disable_req_t   oam_son_pci_conflict_enable_disable_req ;
    /* BUG_11590_END */ 
    oam_son_es_modify_ue_count_interval_req_t oam_son_es_modify_ue_count_interval_req;
    oam_son_es_modify_ue_count_threshold_req_t oam_son_es_modify_ue_count_threshold_req;
    oam_son_es_active_ue_count_report_t oam_son_es_active_ue_count_report;
    /*Drop_4 SON*/
    /*This can be list of max size 32*/
    oam_son_inter_freq_carrier_t
                   oam_son_inter_freq_carrier_params[OAM_MAX_CELL_SUPPORTED];

    oam_son_anr_enable_req_t anr_enable[OAM_MAX_CELL_SUPPORTED];
    oam_son_anr_disable_req_t anr_disable[OAM_MAX_CELL_SUPPORTED];
    oam_son_enable_disable_x2_req_t     x2_enable_disable;
    oam_son_enable_disable_x2_resp_t x2_enable_disable_resp;
    oam_son_nw_scan_req_t nw_scan;
    oam_son_set_nw_scan_config_cmd_t nw_scan_config;
    oam_son_nr_scan_req_t nr_scan;
    oam_son_periodic_nw_scan_req_t periodic_nw_scan;
    oam_son_oam_bulk_nw_scan_resp_t  bulk_scan_resp;
    /** Temp structures for storing untokenized lists **/
    oam_son_nw_scan_params_t temp_values;

    oam_son_oam_es_autonomous_switch_off_config_req_t
                                            autonomous_switch_off_config_req;

    oam_son_mro_modify_config_params_req_t
                        mro_modify_config_params_req[OAM_MAX_CELL_SUPPORTED];
    /*DROP 4 CODE*/
#define OAM_NR_NO_ACTION         0
#define OAM_NR_ACTION_DELETE_EUTRAN    1
#define OAM_NR_ACTION_DELETE_UTRAN     2
/* SPR-13586 START */
#define OAM_NR_ACTION_DELETE_GERAN     4
/* SPR-13586 END */
/* Spr 15438 Fix Start*/
#define OAM_NLIU_ACTION_DELETE_EUTRAN    0x00000400
/* Spr 15438 Fix End*/
/* Spr 15612  Fix Start*/
#define OAM_NLIU_ACTION_DELETE_UTRAN    0x00000800
/* Spr 15612  Fix End*/

    UInt16 nr_action;

#define OAM_NR_ENB_ACTION_ADD    1
#define OAM_NR_ENB_ACTION_UPDATE 2
#define OAM_NR_ENB_ACTION_DELETE 3

    UInt16 nr_enb_action;

#define OAM_SON_NR_CELL_INTRA_RETREIVE_ONGOING   1
#define OAM_SON_NR_CELL_INTER_RETREIVE_ONGOING   2
#define OAM_SON_NR_ENB_INTRA_RETREIVE_ONGOING    3
#define OAM_SON_ES_CELL_SWITCHED_OFF_INFO_RETREIVE_ONGOING 4 

    UInt16            retreive_ongoing;
    /*DROP 4 END*/
/* SPR 13065 CHANGE START */
/* SPR 13065 CHANGE END */
    oam_son_earfcn_pci_selection_t
                          earfcn_pci_selection[OAM_MAX_CELL_SUPPORTED];
    oam_nw_scan_result_t oam_nw_scan_result;

    /* structure for updated NRT info */
    oam_son_neigh_list_in_use_t neigh_list_in_use[OAM_MAX_CELL_SUPPORTED];
   /* SPR 12932 FIX START */
/* SPR-13586 START */
    UInt32 inter_rat_umts_daho_bitmask;
    UInt32 inter_rat_gsm_daho_bitmask;
/* SPR-13586 END */
    UInt32 inter_rat_daho_bitmask;
    UInt32 intra_rat_daho_bitmask;
   /* SPR 12932 FIX END */

    oam_cell_neighbourlist_info_t
                      cell_neighbourlist_info[OAM_MAX_CELL_SUPPORTED];

    mro_config_t    mro_config;
    mlb_config_t    mlb_config;
    /*changes 2.2*/
    oam_son_mlb_modify_attributes_req_t
                        mlb_modify_attributes_req[OAM_MAX_CELL_SUPPORTED];
    oam_son_feature_state_change_ind_t  feature_state_change;
    /*changes 2.2*/
    /*BUG_FIXED_11744_START */
    oam_son_rach_opt_modify_config_params_req_t rach_opt_modify_config_params[OAM_MAX_CELL_SUPPORTED];	
    /*BUG_FIXED_11744_END */

    rachopt_config_t rachopt_config;	 
    /* SPR 15604 START */
/* SPR 18930  FIX START */
    /* Code Removed */
/* SPR 18930  FIX END */
    /* SPR 15799 START */
    UInt8 tnl_discovery_enable_disable_flag;
    /* SPR 15799 END */
    /* Spr 15438 Fix End*/
    /* SPR_19619 start */
    oam_son_peer_cell_activation_req_t     oam_son_peer_cell_activation_req;
    /* SPR_19619 stop */
} oam_son_req_t;
#endif


#define OAM_MAX_ADDRESS_LENGTH		64
#define OAM_MAX_PLMNID 			6
#define OAM_MAX_REM_BANDLIST 		64
#define OAM_MAX_EUTRA_CARRIER_ARFCND 	64
#define OAM_MAX_REM_PLMN	 	32
#define OAM_MAX_PHY_CELL_ID	 	64
#define OAM_MAX_STR_LEN			256
/* SPR 17880 START*/
#define MAX_PARAMETER_KEY_LEN   32
/* SPR 17880 END */
typedef enum 
{
    OAM_NW_SYSTEM_UMTS,
    OAM_NW_SYSTEM_LTE,
    OAM_NW_SYSTEM_CDMA1x,
    OAM_NW_SYSTEM_CDMAHRPD
}oam_supported_system_et;

typedef enum
{
    OAM_BANDWIDTH_N6,
    OAM_BANDWIDTH_N15,
    OAM_BANDWIDTH_N25,
    OAM_BANDWIDTH_N50,
    OAM_BANDWIDTH_N75,
    OAM_BANDWIDTH_N100
}oam_bandwidth_et;  

    /* SPR 17880 START*/
#define         ENABLE_CWMP_PRESENT                        0x0001
#define         PERIODIC_INFORM_ENABLE_PRESENT             0x0002
#define         PERIODIC_INFORM_INTERVAL_PRESENT           0x0004
#define         PERIODIC_INFORM_TIME_PRESENT               0x0008
#define         CONNECTION_REQUEST_USERNAME_PRESENT        0x0010
#define         CONNECTION_REQUEST_PASSWORD_PRESENT        0x0020
#define         CWMP_RETRY_MINIMUM_WAIT_INTERVAL_PRESENT   0x0040
#define         CWMP_RETRY_INTERVAL_MULTIPLIER_PRESENT     0x0080
#define         CON_REQ_URL_PRESENT                        0x0100
#define         SERVING_HEMS_ADDRESS_PRESENT               0x0200
#define         INITIAL_HEMS_ADDRESS_PRESENT               0x0400
#define         SERVING_HEMS_USERNAME_PRESENT              0x0800
#define         SERVING_HEMS_PASSWORD_PRESENT              0x1000
#define         USERNAME_PRESENT        0x2000
#define         PASSWORD_PRESENT        0x4000
    /* SPR 17880 END */


typedef struct _oam_init_tr069_params_t
{
    /* SPR 17880 START*/
    UInt32              bitmask;
    /* SPR 17880 END */
    UInt8               initial_hems_address[OAM_MAX_ADDRESS_LENGTH];
    UInt8               henb_self_address[OAM_MAX_ADDRESS_LENGTH];
    UInt8               serving_hems_address[OAM_MAX_STR_LEN];
    UInt8               henb_address[OAM_MAX_STR_LEN];
    /* Parameters related to TR196 model */
    supported_system_et supported_system;
    UInt8               Beacon;
    UInt32              MaxTxPower;
    UInt8               GPSEquipped;
    UInt8 		bands_supported[OAM_STRING_BAND];
    UInt8               NNSFSupported;
    UInt8               DuplexMode[OAM_MAX_STR_LEN];
    UInt8 		UMTSRx_supported;
    UInt8 		GSMRx_supported;
    UInt8 		CDMA2000Rx_supported;
    UInt8               serving_sec_gw_server1[OAM_MAX_ADDRESS_LENGTH]; //string, IP or FQDN
    UInt8               serving_sec_gw_server2[OAM_MAX_ADDRESS_LENGTH]; //string, IP or FQDN
    UInt8               serving_sec_gw_server3[OAM_MAX_ADDRESS_LENGTH]; //string, IP or FQDN
    UInt16 		phy_cell_id[OAM_MAX_PHY_CELL_ID];
    UInt32 		freq_band_indicator;
    UInt32       	dl_bandwidth; 
    UInt32 	        ul_bandwidth;
    UInt32              earfcnDl; 
    UInt32              earfcnUl; 
    UInt8 		rem_band_list[OAM_MAX_REM_BANDLIST]; 
    UInt8		eutra_carrier_arfcnd_list[OAM_MAX_EUTRA_CARRIER_ARFCND]; 
    UInt8 		rem_plmn_list[OAM_MAX_REM_PLMN];
    /* SPR 17880 START*/
    UInt8                       Username[OAM_MAX_STR_LEN];
    UInt8                       Password[OAM_MAX_STR_LEN];
    oam_bool_et                 EnableCWMP;
    oam_bool_et                 PeriodicInformEnable;
    UInt32                      PeriodicInformInterval;
    UInt8                       PeriodicInformTime[OAM_MAX_STR_LEN + 1];
    UInt8                       ParameterKey[MAX_PARAMETER_KEY_LEN];
    UInt8                       ConnectionRequestUsername[OAM_MAX_STR_LEN + 1];
    UInt8                       ConnectionRequestPassword[OAM_MAX_STR_LEN + 1];
    UInt32                      CWMPRetryMinimumWaitInterval;
    UInt32                      CWMPRetryIntervalMultiplier;
    UInt8                       ConReqURL[OAM_MAX_URL_LENGTH];
    /* SPR 17880 END */

}oam_init_tr069_params_t;

typedef struct _oam_tr069_req_t
{
    oam_init_tr069_params_t 	tr069_init_params;

    UInt8                       tr069_mode;
    UInt8                       connect_to_iHeMS;

    /* SPR 17880 START*/
    oam_bool_et                 EnableCWMP;
    oam_bool_et                 PeriodicInformEnable;
    UInt32                      PeriodicInformInterval;
    time_t                      PeriodicInformTime;
    UInt8                       ParameterKey[MAX_PARAMETER_KEY_LEN];
    UInt8                       ConnectionRequestUsername[OAM_MAX_STR_LEN + 1];
    UInt8                       ConnectionRequestPassword[OAM_MAX_STR_LEN + 1];
    UInt32                      CWMPRetryMinimumWaitInterval;
    UInt32                      CWMPRetryIntervalMultiplier;
    UInt8                       Username[OAM_MAX_STR_LEN];
    UInt8                       Password[OAM_MAX_STR_LEN];
    /* SPR 17880 END */
}oam_tr069_req_t;

/*TR069 ADMIN/OP STATE CHANGES*/
#define OAM_FILE_DOWNLOAD_RESP_PENDING          0x00000001
#define OAM_SET_ADMIN_STATE_RESP_PENDING        0x00000002
    /* RT SPR 8154 FIX START */
#define USR_REQ_TO_ANR_MEAS_CONFIG_ONGOING      0x00000004
    /* RT SPR 8154 FIX END */
#define OAM_S1AP_CLOSE_MME_CONN_REQ_MASK        0x00000008
#define OAM_RFTX_ON_MASK                        0x00000010
#define OAM_RFTX_OFF_MASK                       0x00000020
#define OAM_CELL_SETUP_MASK                     0x00000040
#define OAM_SON_ADD_NR_ONGOING                  0x00000080
#define OAM_SON_ADD_NR_ENB_ONGOING              0x00000100
#define OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING    0x00000400
#define OAM_SON_CARRIER_FREQ_ONGOING            0x00000800
#define OAM_CELL_PLMNLIST_CONFIG_ONGOING        0x00001000
#define OAM_ADD_OBJ_ONGOING                     0x00002000
#define OAM_FSM_INIT_TIME_USR_REQ_ONGOING       0x00004000
#define OAM_SON_NMM_PERIODIC_SCAN_ONGOING       0x00008000
/* SPR-13586 START */
#define OAM_SON_ADD_NR_UMTS_ONGOING             0x00010000
#define OAM_SON_ADD_NR_GSM_ONGOING              0x00020000
#define OAM_SON_UPD_NR_UMTS_ONGOING             0x00040000
#define OAM_SON_UPD_NR_GSM_ONGOING              0x00080000
/* SPR 16603 Fix Start */
#define TR069_ADD_OBJ_ONGOING                   0x00000001
/* SPR 16603 Fix End */
/* SPR-13586 END */

/* SPR 15604 START */
#define OAM_NLIU_UPDATION_ONGOING               0x00100000
/* SPR 15604 END */

/* SPR-16334 START */
#define OAM_X2AP_ENB_ADD_ONGOING                0x00008000
#define OAM_X2AP_ENB_DEL_ONGOING                0x00010000
/* SPR-16334 END */

/* SPR 20270 changes start */
#define OAM_SON_RACH_OPT_PARAMETER_UPDATE_ONGOING 0x00200000
/* SPR 20270 changes end */

/* SPR_20467_FIX_START */
#define OAM_CELL_PLMNLIST_RRM_RECONFIG_ONGOING    0x01000000
/* SPR_20467_FIX_END */




typedef struct _oam_system_status_t
{
    /* Administrative State of FAP */
    oam_admin_state_et     admin_state;
    oam_admin_state_et     admin_state_bak;

    /* Operational State of FAP */
    oam_op_state_et        op_state;

    /* Radio Transmission Status */
    oam_bool_et            rftx_status;   
    /* Availability Status */
    oam_avail_state_et     availability_status;   
    oam_avail_state_et     availability_status_bak;   

    /* Indicates whether cell is configured */
    oam_bool_et            cell_setup_complete;

    UInt32                  event_ongoing;    /*COVERITY FIX 29may*/
    
/* SPR 16603 Fix Start */
    UInt32                  tr_event_ongoing;    
/* SPR 16603 Fix End */

    /* Temporary buffer to store API received 
       from Management Interface */
    UInt8		   *p_api_buf;

    UInt16                  trans_id;
    UInt16                  dest_id;
    /* Indicates whether TR069 agent is configured or not */
    oam_bool_et            tr069_configured;

    /* Temporary pointer to xml doc */
    xmlDoc                 *p_xml_doc;
    /* Temporary pointer to xml doc */
    xmlDoc                 *p_propriety_xml_doc;
#ifdef OAM_SON_ENABLE
    xmlDoc                 *p_nw_scan_result_file; 
#endif
    /* Timer Id */
    oam_timer_t            timer_id;

    /* Last Scan time added*/  
    char      last_scan_time[21];

    /* 
     * is_connected_to_shems 
     * 1 when connected to sHems server
     * 0 when connected to iHems server
     * 
     */
    UInt8 is_connected_to_shems;

    /* 
     * is_nmm_enabled 
     * 1 when nmm is enabled
     * 0 when nmm is disabled
     * 
     */
    UInt8 is_nmm_enabled;
    UInt32 curr_event;  /* Bitmask taking values from enum oam_enb_event_et*/
    UInt32 curr_state;
}oam_system_status_t;

typedef struct __oam_cell_carrier_params_t {
    Char8                  earfcn_list[EARFCN_LIST_MAX_LENGTH];
/* CSR 58157 START */
    UInt8                  earfcnul_list[EARFCN_LIST_MAX_LENGTH];
    UInt8                  earfcnul_list_size;
    UInt8                  earfcndl_list_size;
    UInt16                 earfcn_list_dl[EARFCN_LIST_MAX_LENGTH];
    UInt16                 earfcn_list_ul[EARFCN_LIST_MAX_LENGTH];
/* CSR 58157 END */
    UInt8                  reference_signal_power_list[512];
} oam_cell_carrier_params_t; 

/*CELL_SETUP_2.0*/
typedef struct  
{
    UInt8                  auto_cell_setup;
    /* Parameters which can contain list of values */
    oam_cell_carrier_params_t cell_carrier_params[OAM_MAX_CELL_SUPPORTED];
    UInt8                  ue_trans_power_list[128];
    UInt16                 temp_trans_id;  
    UInt8 oam_log_archival_supported;
    oam_cell_state_et cell_state;
}oam_params_t;
/*bitmask for cells of each instance of l2*/
#define L2_CELL1_INIT_REQ_SENT  0x1
#define L2_CELL2_INIT_REQ_SENT  0x2
typedef struct __oam_cell_info_t {
    oam_cell_status_et cell_status;
    UInt8 cell_idx;
//phy release fix
    UInt8 is_phy_rel_13_capable;
} oam_cell_info_t;

/* SPR 21889 Start */
typedef struct __oam_l2_cell_info_t {
    UInt8 bitmask;
    UInt8 num_cells;
    oam_cell_info_t cell_info[OAM_MAX_NUM_CELLS_PER_L2_INSTANCE * OAM_MAX_NUM_L2_INSTANCE];
} oam_l2_cell_info_t;

typedef struct __oam_l2_instance_info_t {
    oam_l2_cell_info_t l2_cell_info;
    oam_l2_req_t    oam_l2_req;
} oam_l2_instance_info_t;
/* SPR 21889 End */

typedef struct __oam_l2_setup_params_t {
    UInt8 num_instances; /* Keep number of registered instances */
    UInt8 log_req_cnt; /* Keep track of number of requst for log is sent across all agents */
    oam_l2_instance_info_t instance_info[OAM_MAX_NUM_L2_INSTANCE]; /* Keep information about the particular instance */
} oam_l2_setup_params_t;

/* SPR 20908 Start */
#define MAC_OAM_INIT_CNF     0x01
#define RLC_OAM_INIT_CNF     0x02
#define PDCP_OAM_INIT_CNF    0x04
#define EGTPU_OAM_INIT_CNF   0x08
typedef struct __lower_layer_init_info_t{
   UInt8 bitmask;
    UInt8 init_flag;
    rrm_oam_l2_comm_info_t mac_comm_info; 
    rrm_oam_l2_comm_info_t rlc_comm_info;
    rrm_oam_pdcp_comm_info_t cran_pdcp_comm_info;
    rrm_oam_l2_comm_info_t pdcp_comm_info;
    rrm_oam_l2_comm_info_t egtpu_comm_info; 
    rrm_oam_l2_comm_info_t phy_comm_info; 
/*SPR 21889 Start*/
    rrm_oam_l2_comm_info_t agnt_comm_info; 
/*SPR 21889 End */
}lower_layer_init_info_t;
/* SPR 20908 End */

typedef enum{
    S1AP_LINK_DOWN_RCVD = 0x01,
    S1AP_LINK_UP_RCVD = 0x02,
    /* LWA changes start*/
    /* LWA changes end*/

}oam_enb_event_et;

/* SPR-11133 FIX Start */
typedef struct 
{
    UInt8 num_dl_schedulable;
    UInt8 num_ul_schedulable;
}oam_prb_usage_factor_t;
/* SPR-11133 FIX End */

/* SPR 15527 START */
#define OAM_MODIFY_QOS_OBJ_ONGOING              0x00001000
/* SPR 15527 END */
typedef struct 
{
/*SPR 21889 +- */
    oam_l2_req_t oam_l2_req[OAM_MAX_NUM_L2_INSTANCE];
    oam_l3_req_t oam_l3_req;
    oam_rrm_req_t oam_rrm_req;
    oam_tr069_req_t oam_tr069_req;
#ifdef OAM_SON_ENABLE
    oam_son_req_t oam_son_req;
#endif
#ifdef OAM_RADIO_HEAD_SUPPORT
    UInt32 rrh_flag;
#ifdef OAM_RRH_TYPE_1_SUPPORT
    oam_rcm_req_t oam_rcm_req;
    UInt32 rcm_flag;
#endif
#endif
    oam_system_status_t  system_status;
    oam_device_info_t	tr069_device_info;
    oam_igd_params_t oam_igd_params;
    oam_perf_mgmt_params_t oam_pm_params;
    oam_device_info_t device_info_params; 
    oam_nms_ftp_server_params_t oam_nms_ftp_server_params;
    /*CELL_SETUP_2.0*/
    oam_params_t oam_params;
    oam_fm_cntxt_t fm_cntxt;
/* SPR 13024 FIX START */
    /* Lines deleted */
/* SPR 13024 FIX END */
    UInt8 son_enb_tnl_ip_address[2][39];
   oam_hm_config_param_t          hm_config_param;
    UInt8   num_cells_configured;
    oam_l2_setup_params_t l2_setup_info;
    oam_cell_context_t cell_cntxt;
/* SPR 13024 FIX START */
    /* Lines deleted */
/* SPR 13024 FIX END */
    /* SPR-11133 FIX Start */
    oam_prb_usage_factor_t     prb_usage_factor;
    /* SPR-11133 FIX End */
} oam_prov_req_t;

/*struct: oam_inform_api_t
 *  *brief : A structure that contains parameters sent in INFORM API
 *  *        sent to mgmt interface from OAM
 *   */
typedef struct _oam_inform_api_t
{
    Char8 parent_name[MAX_PARENT_NAME_LEN];
    UInt8 num_of_param;
    Char8 param_list[OAM_INFORM_API_MAX_NUM_PARAMS][OAM_INFORM_API_PARAM_NAME_MAX_LEN];
} oam_inform_api_t;

typedef struct _oam_egtpu_initdb_cnf_t
{
    UInt8 result;
}oam_egtpu_initdb_cnf_t;


typedef struct __oam_set_param_vals_resp_t {
    UInt8   status;
    UInt16  err_code;
    UInt8   resvd1;
    UInt8   resvd2[3];
    UInt8   err_str_len;
    UInt8*  err_str;
}  oam_set_param_vals_resp_t;





/*CSR 57877 Code Removed*/


typedef enum _setloglevell2rlccommands
{
    RLC_DISABLED = LTE_L2_LOG_NONE,
    RLC_CRITICALLOG = LTE_L2_LOG_CRITICAL,
    RLC_ERRORLOG = LTE_L2_LOG_ERROR,
    RLC_WARNINGLOG = LTE_L2_LOG_WARNING,
    RLC_INFOLOG = LTE_L2_LOG_INFO,
    RLC_BRIEFLOG = LTE_L2_LOG_BRIEF,
    RLC_DETAILEDLOG = LTE_L2_LOG_DETAILED,
    RLC_DETAILEDALLLOG = LTE_L2_LOG_DETAILEDALL
}setloglevell2rlccommands;

typedef enum setloglevell2pdcpcommands
{
    PDCP_INVALIDLOG = LTE_L2_LOG_INVALID,
    PDCP_CRITICALLOG = LTE_L2_LOG_CRITICAL,
    PDCP_ERRORLOG = LTE_L2_LOG_ERROR,
    PDCP_WARNINGLOG = LTE_L2_LOG_WARNING,
    PDCP_INFOLOG = LTE_L2_LOG_INFO,
    PDCP_BRIEFLOG = LTE_L2_LOG_BRIEF,
    PDCP_DETAILEDLOG = LTE_L2_LOG_DETAILED,
    PDCP_DETAILEDALLLOG = LTE_L2_LOG_DETAILEDALL
}setloglevell2pdcpcommands;
/*CSR 57877 Code Removed*/


typedef struct __oam_timer_data_t {
    /* SPR_9620_CHANGES_START */
    UInt16 trans_id;
    /* SPR_9620_CHANGES_END */
    UInt16 module_id;
    UInt16 timer_type;
    void   *buf;
    UInt16 buf_size;
} oam_timer_data_t;


typedef enum __oam_son_neigh_type_et {
    OAM_SON_NEIGH_TYPE_INTRA_RAT,
   /* SPR-13586 START */
    OAM_SON_NEIGH_TYPE_INTER_RAT_UTRAN,
    OAM_SON_NEIGH_TYPE_INTER_RAT_GERAN
    /* SPR-13586 END */
} oam_son_neigh_type_et;

typedef enum __oam_son_meas_config_type_et {
    OAM_SON_MEAS_CONFIG_TYPE_EUTRAN,
    OAM_SON_MEAS_CONFIG_TYPE_UTRAN_FDD,
    /* RT SPR 8154 FIX START */
    OAM_SON_MEAS_CONFIG_TYPE_GERAN,
    /* RT SPR 8154 FIX END */
    OAM_SON_MEAS_CONFIG_TYPE_UTRAN_TDD
} oam_son_meas_config_type_et;

    /* RT SPR 9019 FIX START */
typedef enum __oam_freq_type_et {
    OAM_UTRAN_FREQ_FDD,
    OAM_UTRAN_FREQ_TDD,
    /* SPR-13586 START */
    OAM_EUTRAN_FREQ,
    OAM_GERAN_FREQ,
    /* SPR-13586 END */
    OAM_FREQ_NOT_AVAILABLE
} oam_freq_type_et;

typedef enum __oam_arfcn_type_et {
    OAM_ARFCN_EUTRAN,
    /* SPR-13586 START */
    OAM_ARFCN_UTRAN,
    OAM_ARFCN_GERAN
    /* SPR-13586 END */
} oam_arfcn_type_et;
    /* RT SPR 9019 FIX END */

/*FM_STRUCTURES*/


typedef enum _oam_ip_interface_type_et
{
    OAM_TR069_INTERFACE,
    OAM_S1AP_INTERFACE,
    OAM_X2AP_INTERFACE,
    OAM_GTPU_INTERFACE,
    OAM_M2AP_INTERFACE,
    /* LWA changes start*/
//    #ifdef LWA_ENABLED
    OAM_XWAP_INTERFACE,
  //  #endif
    /* LWA changes end*/
    OAM_INVALID_INTERFACE
}oam_ip_interface_type_et;


/* Error Events enums */
typedef enum rrm_error_event_type_et
{ 
    RRM_EE_TYPE_SW_PROCESSING_ERR,
    RRM_EE_TYPE_MEM_UNAVAILABLE,
    RRM_EE_INVALID_TYPE         
}rrm_error_event_type_et;

typedef enum _rrm_error_event_severity_et
{
    RRM_EE_INFORMATIONAL,
    RRM_EE_MINOR,
    RRM_EE_MAJOR,
    RRM_EE_CRITICAL
}rrm_error_event_severity_et ;

typedef enum _rrm_error_event_cause_et
{
    RRM_EE_CAUSE_INVALID_PARAMS,
    RRM_EE_CAUSE_NO_FREE_BUFFER,
    RRM_EE_CAUSE_INVALID_IPC_HANDLE
}
rrm_error_event_cause_et;


typedef struct _oam_rrm_time_stamp_t
{
    U16      year;  /*^ M, 0, N, 0, 0 ^*/
    U16      month; /*^ M, 0, N, 0, 0 ^*/
    U16      day;   /*^ M, 0, N, 0, 0 ^*/
    U16      hour;  /*^ M, 0, N, 0, 0 ^*/
    U16      min;   /*^ M, 0, N, 0, 0 ^*/
    U16      sec;   /*^ M, 0, N, 0, 0 ^*/
}oam_rrm_time_stamp_t;

#define  OAM_RRM_ERR_EVENT_MAX_ASSOCIATED_DATA_LEN   50

#define  RRM_ERROR_EVENT_CAUSE_PRESENT      0x01
#define  RRM_ERROR_EVENT_SEVERITY_PRESENT   0x02
#define  RRM_ERROR_EVENT_DATA_PRESENT       0x04


typedef struct _oam_ul_dl_qci_cli_input
{

    UInt8 flag;
    /* SPR +- 18268 */
    UInt16 num_of_ue;
    /* SPR +- 18268 */
    UInt8 qci;
    UInt64 max_bitrate_ul;
    UInt64 min_bitrate_ul;
    UInt64 max_bitrate_dl;
    UInt64 min_bitrate_dl;
    UInt8  bitmask;
}oam_ul_dl_qci_cli_input_t;

typedef struct _oam_ul_dl_qci_cli_flag
{
    UInt8 flag;
}oam_ul_dl_qci_cli_flag_t;
oam_ul_dl_qci_cli_flag_t oam_ul_dl_qci_cli_flag; 

typedef struct _oam_ecn_cap_flag
{
    UInt8 flag;
}oam_ecn_cap_flag_t; 
oam_ecn_cap_flag_t oam_ecn_cap_flag;

typedef struct _oam_ecn_cap_bit_rate_counter
{
    UInt8  bitrate_counter;
}oam_ecn_cap_bit_rate_counter_t;
oam_ecn_cap_bit_rate_counter_t oam_ecn_cap_bit_rate_counter; 


typedef struct _oam_rrm_event_error_info_t
{
    rrm_bitmask_t               bitmask;
    rrm_error_event_type_et     error_type;
    rrm_error_event_cause_et    error_cause; /*OPTIONAL*/
    rrm_error_event_severity_et severity;    /*OPTIONAL*/
    U8                          associtated_data[OAM_RRM_ERR_EVENT_MAX_ASSOCIATED_DATA_LEN]; /*OPTIONAL*/
}oam_rrm_event_error_info_t; 

typedef struct _oam_rrm_event_header_t
{  
    oam_rrm_time_stamp_t       time_stamp;  /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_event_class_et     event_type; /*^ M, 0, N, 0, 0 ^*/ 
    rrm_oam_event_subclass_et  event_sub_type; /*^ M, 0, N, 0, 0 ^*/ 
    U16                        event_id;  /*^ M, 0, N, 0, 0 ^*/
}oam_rrm_event_header_t;


#define RRM_OAM_API_DATA_PRESENT  0x01
typedef struct _oam_rrm_event_notification_handler_t
{
    rrm_bitmask_t             bitmask; /*^ BITMASK ^*/
    oam_rrm_event_header_t    msg_header; /*^ M, 0, N, 0, 0 ^*/
    oam_rrm_event_error_info_t api_data;  
}oam_rrm_event_notification_handler_t; /*^ API, RRM_OAM_EVENT_NOTIFICATION ^*/
/*FM_STRUCTURES*/
/* CSR 00051709 Change start*/
typedef struct _oam_ip_addr_t
{
    UInt8 ip_addr[40];
    UInt8 status;
}oam_ipaddr_t;

typedef struct _oam_s1SigLinkServerList_t
{
   UInt8 num_ip_addr;
   oam_ipaddr_t oam_mme_info[MAX_NUM_MME];
}oam_s1SigLinkServerList_t;

/* SPR-9485 START */

#define MAX_NUM_GROUP_ID 16
#define MAX_NUM_MME 16
typedef struct
{
  UInt8                     num_plmn_id_group_id;
  s1ap_plmn_identity_t      plmn_id_per_gp_id[MAX_NUM_GROUP_ID];/*OCTET_STRING,VARIABLE */
  s1ap_served_group_id_t    group_id_per_plmn_id[MAX_NUM_GROUP_ID];/*OCTET_STRING,VARIABLE */
}oam_internal_mme_info_t;

typedef struct _oam_connected_mme_info
{
  UInt8 num_mme;
  s1ap_oam_mme_info_t oam_s1ap_mme_info[MAX_NUM_MME];
  oam_internal_mme_info_t   internal_mme_info[MAX_NUM_MME];
} oam_connected_mme_info_t;

/* SPR-9485 END */

/* CSR 00051709 Change End*/

/*struct: oam_layer_intf_resp_code_t
 *  *brief : This structure is a mapping of response codes returnd from
 *  *        layers to their equivalent string literals
 *   */
typedef struct __oam_layer_intf_resp_code_t {
    UInt16 resp_code;
    Char8* resp_code_str;
} oam_layer_intf_resp_code_t;

typedef struct __oam_tcb_cntxt_data_t {
    UInt16  usr_req_trans_id;
    UInt16  usr_intf_src_id;
    xmlNode *p_usr_msg_xml_parent;
    xmlNode *p_orig_xml_parent;
    xmlNode **p_parent_xml_struct_addr;
} oam_tcb_cntxt_data_t;
/*spr 22474 start*/
/* This structure provides the POOL stats */
typedef struct _oam_PoolStatsT {
    /* the size of the buffer allocated in this pool */
    U32 bufSize; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers in this pool */
    U32 numOfBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Total number of Buffers allocated from this pool */
    U32 numOfAllocBuf; /*^ M, 0, N, 0, 0 ^*/
    /* the Peak of Total number of Buffers allocated from this pool during the system is running */
    U32 numOfPeakBuf; /*^ M, 0, N, 0, 0 ^*/
}oam_PoolStats;

/* This structure provides the MEM POOL and MSG POOL stats */
typedef struct _oam_MemPoolStatsT {
    /* This array is terminated when bufSize is 0 for an entry, other entries are not valid */
    oam_PoolStats stats[NVARPOOL]; /*^ M, 0, OCTET_STRING, FIXED ^*/
}oam_MemPoolStats;

/* OAM_GET_DEBUG_INFO_REQ */
/*! \typedef son_oam_get_debug_info_req_t 
 *  \brief   This API is sent from OAM to SON
 *           to request for debug stats
 */
typedef struct _oam_get_debug_info_req_t
{
   U16                   debug_type;/*^ M, 0, N, 0, 0 ^*/
   /*oam_debug_info_req_et*/
}oam_get_debug_info_req_t; /*^ API, OAM_GET_DEBUG_INFO_REQ ^*/

/* OAM_GET_DEBUG_INFO_RESP */
/*! \typedef oam_get_debug_info_resp_t
 *  \brief   This API is sent 
 *           in the response of oam_get_debug_info_req_t 
 */
#define OAM_DEBUG_GET_MEM_POOL_PRESENT 0x01
#define OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT 0x02
typedef struct _oam_get_debug_info_resp_t
{
    UInt32  bitmask; /*^ BITMASK ^*/
    SInt32  response;/*^ M, 0, N, 0, 0 ^*/
    SInt32  fail_cause;/*^ O, OAM_GET_DEBUG_INFO_FAILURE_CAUSE_PRESENT, N, 0, 0 ^*/
    /* oam_debug_info_fail_cause_et */
    oam_MemPoolStats csplMemPoolStats;/*^ O, OAM_DEBUG_GET_MEM_POOL_PRESENT, N, 0, 0 ^*/
}oam_get_debug_info_resp_t;/*^ API, OAM_GET_DEBUG_INFO_RESP ^*/

/*spr 22474 end*/

#endif
