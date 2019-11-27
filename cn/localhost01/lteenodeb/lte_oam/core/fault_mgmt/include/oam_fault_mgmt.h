/*****************************************************************************
 * File Name       : oam_fault_mgmt.h
 *
 * Description     : This file function decelerations, Enumerations and Structures 
 *                   for Fault Management
 * Revision History:
 * Date        	Author  Reference            Comments
 * ----- 		------  ---------            --------
 * March,2012   Vishwa	OAM Design Document  Initial Version
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef OAM_FAULT_MGMT_H
#define OAM_FAULT_MGMT_H

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/
#include <oam_xml_header.h>
#include <oam_config_mgmt.h>
#include <oam_types.h>

/****************************************************************************
 **  MACROS
 ****************************************************************************/
/* Macros for Fault Management */
#define OAM_FM_FAILURE 		               -1
#define OAM_MAX_ALARMS_TYPE 	           14
#define OAM_MAX_ALARMS 		              200
#define OAM_TIME_SIZE 		               30
#define OAM_MODULE_NAME_SIZE 	           10 
#define OAM_FILE_NAME_SIZE 	              256
#define OAM_MAX_TEXT_SIZE 	               80
#define OAM_MAX_ALARM_PARAMS 	           15
#define OAM_EVENT_CODE 		               15
#define OAM_PARAM_MAX_LEN 	              256
#define OAM_VENDOR_SPECIFIC_ALARM_START   200
#define OAM_VENDOR_SPECIFIC_ALARM_END     400
#define OAM_FM_MAX_SUP_PARAM_PARTIAL_PATH 10
#define OAM_FM_MAX_ALARM_ATTRIBUTES       10
#define OAM_MAX_EVENT_INFO_LEN            255
#define OAM_MAX_PARENT_NODE_NAME_LEN      60
#define OAM_MAX_PARENT_NAME_LEN           512
#define MAX_NUM_PARAM                     255
#define MAC_PHY_OAM_ERR_MSG_INVALID_STATE 1
#define MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD 2
#define MAC_PHY_OAM_ERR_MSG_BCH_MISSING 3 
#define MAC_PHY_OAM_ERR_MSG_HI_ERR 4 

#define FM_CNTXT                           oam_prov_req.fm_cntxt

/* Spr 12880 Fix Start */
/* SPR 17777 fix start */
#define OAM_ALARM(alarm_id, criticality, notification_type, event_type,\
                  probable_cause, reporting_mechanism, add_txt, add_info, cellid) \
              oam_internal_alarm(alarm_id, notification_type, \
                                 event_type, probable_cause, \
                                 reporting_mechanism, add_txt, add_info, cellid)

/* Spr 12880 Fix End */
typedef rrm_oam_event_notification_t  oam_rrm_event_notification_t; 
/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
/* enum for rrm events */
typedef enum
{
    RRM_PE_CELL_SETUP_REJECT,
    RRM_PE_RRC_CONN_SETUP_READY,
    RRM_PE_EVENT_UE_CAPABILITY,
    RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT,
    RRM_PE_ERAB_SETUP_ATTEMPT,
    RRM_PE_ERAB_SETUP_READY,
    RRM_PE_ERAB_ADMISSION_REJECT,
    RRM_PE_ERAB_DRB_RELEASE,
    RRM_PE_ERAB_RELEASE_ATTEMPT,
    RRM_PE_ERAB_RELEASE_READY,
    RRM_PE_ERAB_MODIFY_READY,
    RRM_PE_ERAB_MODIFY_ATTEMPT,
    RRM_PE_ERAB_DRB_ALLOCATED,
    RRM_PE_ERAB_SETUP_QCI,
    RRM_PE_EVENT_ERAB_RELEASE_INFO,
    RRM_PE_EVENT_ERAB_SETUP_INFO,
    RRM_PE_EVENT_MEAS_CONFIG_A1,
    RRM_PE_EVENT_MEAS_CONFIG_A2,
    RRM_PE_EVENT_MEAS_CONFIG_A3,
    RRM_PE_EVENT_MEAS_CONFIG_A4,
    RRM_PE_EVENT_MEAS_CONFIG_A5,
    RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA,
    RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN,
    RRM_PE_EVENT_UE_MEAS_ABORT,
    RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA,
    RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA,
    RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN,
    RRM_PE_EVENT_MEAS_CONFIG_B2_CDMA2000,
    RRM_PE_HO_IN_EXEC_COMPLETE,
    RRM_PE_HO_IN_PREP_COMPLETE,
    RRM_PE_HO_IN_EXEC_ATTEMPT,
    RRM_PE_HO_IN_EXEC_FAILED,
    RRM_PE_EVENT_HO_IN_PREP_ATTEMPT,
    RRM_PE_EVENT_HO_IN_PREP_FAILURE,
    RRM_PE_HO_OUT_PREP_ATTEMPT,
    RRM_PE_HO_OUT_PREP_FAILED,
    RRM_PE_HO_OUT_PREP_INFO,
    RRM_PE_HO_OUT_EXEC_ATTEMPT,
    RRM_PE_HO_OUT_EXEC_FAILED,
    RRM_PE_HO_OUT_EXEC_COMPLETE,
    RRM_PE_EVENT_HO_UE_DETECTED,
    RRM_PE_HO_OUT_EXEC_INFO,
    RRM_PE_EVENT_TOO_EARLY_HO, 
    RRM_PE_EVENT_TOO_LATE_HO,
    RRM_PE_EVENT_HO_WRONG_CELL,
    RRM_PE_EVENT_HO_WRONG_CELL_REEST,
    RRM_PE_EVENT_X2_RLF_INDICATION,
    RRM_PE_MME_CONFIGURATION_TRANSFER,
    RRM_PE_ENB_CONFIGURATION_TRANSFER,
    RRM_PE_SRB1_SETUP_REJECT
}oam_rrm_event_id_et;


typedef enum
{
    PROTOCOL_EVENT,
    ERROR_EVENT
}oam_rrm_event_type_et;


typedef enum
{
    ALL_SUB_CLASS,
    CELL_SPECIFIC,
    UE_SPECIFIC,
    RAB_SPECIFIC,
    MEASUREMENT_SPECIFIC,
    HO_IN_SPECIFIC,
    HO_OUT_SPECIFIC,
    OTHER_PROTOCOL_EVENT,
    TIMER_RELATED,
    IPC_SEND_RECV_ERROR_RELATED
}oam_rrm_event_sub_type_et;

typedef struct _oam_rrm_event_id_t
{
    char    *event_id_str;
    UInt16  event_id;
}oam_rrm_event_id_t;

typedef struct _oam_rrm_event_type_t
{
    char    *event_type_str;
    UInt16  event_type;
}oam_rrm_event_type_t;

typedef struct _oam_rrm_event_sub_type_t
{
    char    *event_sub_type_str;
    UInt16  event_sub_type;
}oam_rrm_event_sub_type_t;

/* RRM structures and enum END */

/* fm enums */
typedef enum
{
    OAM_FM_EVENT_ALARM_ACKNOWLEDGED,
    OAM_FM_EVENT_ALARM_CLEARED,
    OAM_FM_EVENT_ALARM_CHANGED,
    OAM_FM_EVENT_NEW_ALARM_RAISED,
    OAM_FM_MAX_EVENTS
}oam_fm_events_et;


/* Structure for Reporting Mechanism and Reporting Mechanism Enum Value */
typedef struct _oam_alarm_report_t
{
    char  *report_type;
    UInt8 report_id;
}oam_alarm_report_t;

/* Structure for Event Type and Event Type Enum Value */
typedef struct _oam_alarm_event_t
{
    char    *event_type;
    UInt8   event_id;
}oam_alarm_event_t;

/* Structure for Probable Cause and Probable Cause Enum Value */
typedef struct _oam_alarm_probable_t
{
    char   *probable_cause;
    UInt16 probable_cause_id;
}oam_alarm_probable_t;

/* Structure for Notification Type and Notification Type Enum Value */
typedef struct _oam_alarm_notif_t
{
    char  *notification_type;
    UInt8 notification_id;
}oam_alarm_notif_t;

/* Structure for Perceived Severity and Perceived Severity Enum Value */
typedef struct _oam_alarm_severity_t
{
    char  *perceived_severity;
    UInt8 severity_id;
}oam_alarm_severity_t;

/* structure for alarm information */


typedef struct _oam_alarm_info_t
{
    UInt16  module_id;
    Char8    module_name[OAM_MODULE_NAME_SIZE];
    UInt16  alarm_id;
    UInt16  notification_type;
    UInt16  event_type; 
    UInt16  probable_cause;
    Char8   managed_object_instance[OAM_MANAGED_OBJECT_INSTANCE_SIZE];
    UInt16  criticality;
    UInt16  reporting_mechanism;
    Char8    probable_cause_str[OAM_ALARM_PROB_CAUSE_STR_MAX_LEN]; 
    Char8    specific_problem[OAM_SPECIFIC_PROBLEM_SIZE];
    Char8    time_stamp[OAM_ALARM_TIMESTAMP_STR_MAX_LEN];
    Char8    add_txt[OAM_ALARM_ADD_TXT_MAX_LEN];
    Char8    add_info[OAM_ALARM_ADD_INFO_MAX_LEN];
/* Spr 12880 Fix Start */
    UInt8    CellId;
/* Spr 12880 Fix End */
}oam_alarm_info_t;

typedef struct
{
    UInt8 typeOfError;
}oam_mac_error_ind_handler_t;



/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/

/****************************************************************************
 * Function Name  : oam_create_timestamp
 * Inputs         : fullName
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function Handles the time stamp
 ****************************************************************************/
oam_return_et oam_create_timestamp
(
    Char8 *time_buffer
);

/****************************************************************************
 * Function Name : oam_init_fault_mgmt
 * Inputs        : p_api_buf, msg_len
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : Fault Management Init Function
****************************************************************************/
/* + SPR 17439 */
oam_return_et oam_init_fault_mgmt(void);
/* - SPR 17439 */

oam_return_et
oam_fm_intf_process_msg
(
    void *p_api_buf
);
/****************************************************************************
 * * Function Name: oam_fm_clear_history
 * * Description  : this function purges all data in HistoryEvent table
 * * Inputs       : NONE
 * * Outputs      : NONE
 * * Returns      : OAM_SUCCESS/OAM_FAILURE 
 * *************************************************************************/
/* + SPR 17439 */
oam_return_et 
oam_fm_clear_history(void);
/* - SPR 17439 */

/***************************************************************************
 * * Function Name: oam_get_alarm_event_by_event_id
 * * Description  : this function used to get event type by event id.
 * * Inputs       : event_id
 * * Outputs      : event_type
 * * Returns      : event_type
 * *************************************************************************/
char* 
oam_get_alarm_event_by_event_id
(
    UInt16  event_id
);

/***************************************************************************
 * * Function Name: oam_get_alarm_probable_cause_by_cause_id
 * * Description  : this function used to get probable cause type.
 * * Inputs       : cause id
 * * Outputs      : probable cause type.
 * * Returns      : probable caues type. 
 * *************************************************************************/
char* 
oam_get_alarm_probable_cause_by_cause_id
(
    UInt16  cause_id
);
/****************************************************************************
 * * Function Name: oam_get_alarm_notification_type_by_notification_id
 * * Description  : this function is used get notification type.
 * * Inputs       : notification_id
 * * Outputs      : notification type.
 * * Returns      : notification type. 
 * *************************************************************************/
char *
oam_get_alarm_notification_type_by_notification_id
(
    UInt16 notification_idi
);
/****************************************************************************
 * * Function Name: oam_get_alarm_severity_type_by_severity_id
 * * Description  : this function processes fm get req
 * * Inputs       : severity_id
 * * Outputs      : severity type
 * * Returns      : severity type
 * *************************************************************************/
char* 
oam_get_alarm_severity_type_by_severity_id
(
    UInt16  severity_id
);

/****************************************************************************
 *  * Function Name : oam_handle_alarm
 *  * Inputs        : alarm_info 
 *  * Outputs       : none
 *  * Returns       : OAM_SUCCESS/OAM_FAILURE
 *  * Description   : This function handles alarms
 *  *****************************************************************************/
oam_return_et
oam_handle_alarm
(
    oam_alarm_info_t  *p_alarm_info
);

oam_return_et
oam_internal_alarm
(
    UInt16  alarm_id,
    /* SPR 17777 fix code removed */
    UInt16  notification_type,
    UInt16  event_type,
    UInt16  probable_cause,
    UInt16  reporting_mechanism,
    Char8   *add_txt,
    Char8   *add_info,
/* Spr 12880 Fix Start */
    UInt8   cellid 
/* Spr 12880 Fix End */
); 


/****************************************************************************
 * * Function Name  : oam_fm_fsm_process_event
 * * Inputs         : p_msg :Pointer to input api buffer
 * *                  p_fm_cntxt :Pointer to FM FSM context
 * *                  alarm_id:Identity of Alarm
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * * Description    : Entry function for OAM FM FSM
 * ****************************************************************************/

oam_return_et
oam_fm_fsm_process_event
(
    oam_alarm_info_t  *p_alarm_info, /*Pointer to input api buffer*/
    oam_fm_cntxt_t    *p_fm_cntxt,   /*Pointer to FM FSM context*/
    /* SPR 17777 fix code removed */
    oam_fm_events_et   current_event,
    oam_error_code_et *p_err_code
);

/****************************************************************************
 * * Function Name  : current_alarm_list_roll_over
 * * Inputs         : alarm_idx
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * * Description    : to roll over current alarm list.
 * ****************************************************************************/
oam_return_et
current_alarm_list_roll_over
(
    int  alarm_idx
);

/****************************************************************************
 * * Function Name : oam_fm_populate_current_alarm_record
 * * Inputs        : alarm_info and pointer to current alarm list.
 * * Outputs       : none
 * * Returns       : OAM_SUCCESS/OAM_FAILURE
 * * Description   : This function handles alarms
 * *****************************************************************************/
oam_return_et
oam_fm_populate_current_alarm_record
(
    oam_current_alarm_info_t  *p_current_alarm_record,
    oam_alarm_info_t          *p_alarm_info
);

/****************************************************************************
 * * Function Name  : oam_fm_fetch_curr_alarm_idx
 * * Inputs         : alarm_id
 * * Outputs        : none
 * * Returns        : alarm index
 * * Description    : This function used to return alarm index of current alarm list.  
 * ****************************************************************************/

int 
oam_fm_fetch_curr_alarm_idx
(
/* SPR 14630 Fix Start */
  UInt16 alarm_id,
  UInt8 CellId
/* SPR 14630 Fix End */
);

/****************************************************************************
 * * Function Name : oam_fm_populate_history_Alarm_record
 * * Inputs        : alarm_info and pointer to history alarm list.
 * * Outputs       : none
 * * Returns       : OAM_SUCCESS/OAM_FAILURE
 * * Description   : This function handles alarms
 * *****************************************************************************/
oam_return_et
oam_fm_populate_history_alarm_record
(
    oam_history_alarm_info_t  *p_history_alarm_record,
    oam_alarm_info_t          *p_alarm_info
);

/****************************************************************************
 * * Function Name : oam_handle_raised_alarm
 * * Inputs        : alarm_info structure.
 * * Outputs       : 
 * * Returns       : OAM_SUCCESS/OAM_FAILURE
 * * Description   : This function is used for handle raised alarm.
 * *****************************************************************************/
oam_return_et
oam_handle_raised_alarm
(
    oam_alarm_info_t  *alarm_info
);

/****************************************************************************
 * * Function Name : oam_fm_populate_queued_alarm_record
 * * Inputs        : alarm_info and pointer to queued alarm list.
 * * Outputs       : none
 * * Returns       : OAM_SUCCESS/OAM_FAILURE
 * * Description   : This function populate records in QUEUED_ALARM_LIST.
 ******************************************************************************/
oam_return_et
oam_fm_populate_queued_alarm_record
(
    oam_queued_alarm_info_t  *p_queued_alarm_record,
    oam_alarm_info_t         *p_alarm_info
);

/****************************************************************************
 * * Function Name : oam_fm_populate_expedited_alarm_record
 * * Inputs        : alarm_info and pointer to expedited alarm list.
 * * Outputs       : none
 * * Returns       : OAM_SUCCESS/OAM_FAILURE
 * * Description   : This function populate records in EXPEDITED_ALARM_LIST.
 * *****************************************************************************/
oam_return_et
oam_fm_populate_expedited_alarm_record
(
    oam_expedited_alarm_info_t  *p_expedited_alarm_record,
    oam_alarm_info_t            *p_alarm_info
);

/*******************************************************************************
 * * Function Name: oam_fm_send_history_alarm_list_to_qcli
 * * Description  : this function send response of get fm req to user
 * * Inputs       : trans id,src_id,parent_node_name 
 * *               
 * * Outputs      : NONE
 * * Returns      : OAM_SUCCESS/OAM_FAILURE
 * ********************************************************************************/

oam_return_et
oam_fm_send_history_alarm_list_to_qcli
(
    UInt16  trans_id,
    UInt16  src_id,
    char    *parent_node_name
);

/*******************************************************************************
 * * Function Name: oam_fm_send_current_alarm_list_to_qcli
 * * Description  : this function send response of get fm req to user
 * * Inputs       : trans id,src_id,parent_node_name          
 * * Outputs      : NONE
 * * Returns      : OAM_SUCCESS/OAM_FAILURE
 * ********************************************************************************/
oam_return_et
oam_fm_send_current_alarm_list_to_qcli
(
    UInt16  trans_id,
    UInt16  src_id,
    char    *parent_node_name
);

/*******************************************************************************
 * * Function Name: oam_fm_send_expedited_alarm_list_to_qcli
 * * Description  : this function send response of get fm req to user
 * * Inputs       : trans id
 * *                src_id
 * *                parent_node_name
 * *               
 * * Outputs      :NONE
 * * Returns      :OAM_SUCCESS/OAM_FAILURE 
 * ********************************************************************************/

oam_return_et
oam_fm_send_expedited_alarm_list_to_qcli
(
    UInt16  trans_id,
    UInt16  src_id,
    char    *parent_node_name
);

/*******************************************************************************
 * * Function Name: oam_fm_send_queued_alarm_list_to_qcli
 * * Description  : this function send response of get fm req to user
 * * Inputs       : trans id
 * *                src_id 
 * *                parent_node_name
 * *               
 * * Outputs      : NONE
 * * Returns      : OAM_SUCCESS/OAM_FAILURE
 * ********************************************************************************/
oam_return_et
oam_fm_send_queued_alarm_list_to_qcli
(
    UInt16  trans_id,
    UInt16  src_id,
    char    *parent_node_name
);

/****************************************************************************
 * * Function Name : oam_handle_cleared_alarm
 * * Inputs        : alarm_info structure.
 * * Outputs       : 
 * * Returns       : OAM_SUCCESS/OAM_FAILURE
 * * Description   : This function is used for handle raised alarm.
 ******************************************************************************/
oam_return_et
oam_handle_cleared_alarm
(
    oam_alarm_info_t  *p_alarm_info
);

/******************************************************************************
 * * Function Name  : oam_fm_populate_n_send_data_to_management_interface
 * * Inputs         : oam_alarm_info_t
 * * Outputs        : none
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * * Description    : This is the message handling function of OAM.
 * *******************************************************************************/

oam_return_et
oam_fm_populate_n_send_data_to_mangement_interface
(
    oam_alarm_info_t  *p_alarm_info
);


/******************************************************************************
 * * Function Name  : oam_fm_handle_alarm_acknowledge_resp
 * * Inputs         : tcb_detail
 * * Outputs        : none
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * * Description    : This is the ack handling function of OAM.
 * *******************************************************************************/
oam_return_et
oam_fm_handle_alarm_acknowledge_resp 
(
    UInt32  tcb_detail
); 

/******************************************************************************
 * * Function Name  : oam_fm_populate_n_send_data_to_management_interface
 * * Inputs         : param_name,param_value
 * * Outputs        : none
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * * Description    : This is used to set reporting mechanism through management interfaces.
 * *******************************************************************************/
oam_return_et
oam_fm_set_reporting_mechanism 
(
    Char8  *param_name, 
    Char8  *param_value
);

/******************************************************************************
 ** Function Name  : oam_handle_rrm_alarm
 ** Inputs         : p_api_buf
 ** Outputs        : none
 ** Returns        : OAM_SUCCESS/OAM_FAILURE
 ** Description    : This is alarm  handling function of RRM.
 ********************************************************************************/
oam_return_et
oam_handle_rrm_alarm
(
    void  *p_api_buf
);


oam_return_et
oam_handle_fm_get_set_requests (
    void *p_api_buf
);


void oam_fill_managed_object_instance
(
     UInt8 *managed_object_instance,
     UInt16 module_id,UInt16 alarm_id
);

oam_return_et 
oam_fm_clear_current_alarm_list(void);

oam_return_et
oam_mac_handle_error_inds(void *p_api_buf,
			  UInt16* alarm_id,
			  UInt16* criticality,
			  Char8* add_text);
/* SPR 13765 changes start */
/******************************************************************************
 ** Function Name  : oam_egtpu_handle_error_inds
 ** Inputs         : p_api_buf
 ** Outputs        : none
 ** Returns        : OAM_SUCCESS/OAM_FAILURE
 ** Description    : This is alarm  handling function of EGTPU.
 ********************************************************************************/
oam_return_et
oam_egtpu_handle_error_inds(void *p_api_buf,
			  UInt16* alarm_id,
			  UInt16* criticality,
			  Char8* add_text);
/* SPR 13765 changes end */

/*SPR FIX 10190 */
/******************************************************************************
 *  ** Function Name  : oam_son_get_alarm_id
 *  ** Inputs         : event_id_son
 *  ** Outputs        : none
 *  ** Returns        : alarm_id
 *  ** Description    : This is a function for getting the alarm_id from event_id of SON.
 *  ********************************************************************************/

UInt16
oam_son_get_alarm_id
(
    UInt16 event_id_son
);

/* SPR 14630 Fix Start */
typedef struct 
{
    SInt32         alarm_id;   
    SInt32         clearing_cond_for_alarm_id;
}alarm_clearing_map_element_t;

oam_return_et
oam_populate_alarm_info(oam_alarm_info_t *p_alarm_info);

oam_return_et
oam_find_alarm_clearing_info(UInt16 alarm_id,
                             oam_alarm_info_t **p_alarm_cleaed,
                             UInt8 CellId);

oam_return_et
oam_fm_state_1_alarm_ack(
    oam_alarm_info_t       *p_alarm_info,                             /*Pointer to input api buffer*/
    oam_fm_cntxt_t      *p_fm_cntxt,              /*Pointer to FM FSM context*/
    int               curr_alarm_idx,
    oam_error_code_et   *p_err);
/* SPR 14630 Fix End */



#endif /* OAM_FAULT_MGMT_H */



