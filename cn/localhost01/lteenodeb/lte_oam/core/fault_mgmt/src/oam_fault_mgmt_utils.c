/************************************************************************
 * File Name   : oam_fault_mgmt_utils.c
 *
 * Description : This file contains fault management functionality
 *
 * Revision History:
 * Date        Author   Reference             Comments
 * -----       ------   ---------             --------
 * March,2012  Vishwa	OAM Design Document   Initial Version
   Aug 23, 2013 Vivek   OAM 2.3               Change for SNMP interface trap
 *                                            handling
 * May, 2014      Ankita Jain      Klocwork, coverity fixes
 * August, 2014   Ankita Jain      SPR-12596
 * May 2014   Shalu Sagar    Klockwork Fix   
 * Nov,2014   Karuna Bhardwaj   EICIC Enhancement
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 ****************************************************************************
 ****************************************************************************
 * Project Includes
 ****************************************************************************/
#include<oam_fault_mgmt.h>
#include<oam_defines.h>
#include<oam_proto.h>
#include<lteShmLogger.h>
#include<oam_types.h>
#include<oam_config_tcb.h>
#include<oam_sim.h>
#include<oam_sim_utils.h>
#include<oam_rrm_parser.h>
#include<oam_rrm_composer.h>
#include<oam_s1apCommon_il_composer.h>
#include<oam_fm_alarm_defs.h>
#include<oam_son_parser.h>
#include<oam_s1ap_il_composer.h>
/****************************************************************************
 * Global Variables 
 ****************************************************************************/
extern UInt8 shutdown_flag;

extern oam_prov_req_t oam_prov_req;

#ifdef OAM_UT
UInt16 oam_fm_get_new_expedited_alarm_idx (void);
#else
LOCAL UInt16 oam_fm_get_new_expedited_alarm_idx (void);
#endif

#define FILE_NAME_LENGTH 45
LOCAL Char8  Alarm_log_file_name[FILE_NAME_LENGTH];
#define OAM_FM_STRING_SIZE 255

/* SPR 14630 Fix Start */

alarm_clearing_map_element_t g_alarm_clearing_map_table[] = {
    {
        RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID,
            RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID
    },
    {
        RRC_S1AP_CONNECTION_SUCCESS_ALARM_ID,
            RRC_S1AP_CONNECTION_FAILURE_ALARM_ID
    },
    {
        RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID,
            RRC_SCTP_ASSOCIATION_RECOVERY_ALARM_ID 
    },
    {
        RRC_S1AP_CONNECTION_FAILURE_ALARM_ID,
            RRC_S1AP_CONNECTION_SUCCESS_ALARM_ID
    },

};
/* SPR 14630 Fix End */



/*******************************************************************************
 * Functions for getting alarm index from diffrent diffrent list.
 * *****************************************************************************/

/****************************************************************************
 * Function Name  : oam_fm_get_new_history_alarm_idx
 * Inputs         : none
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return new alarm index of history alarm list.
 ****************************************************************************/
/* + SPR 17439 */
UInt16
oam_fm_get_new_history_alarm_idx(void)
/* - SPR 17439 */
{
    UInt16 index = oam_prov_req.fm_cntxt.History_List_Index;

    if((++oam_prov_req.fm_cntxt.History_List_Index) == OAM_HISTORY_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.isHistoryRolled = OAM_ONE;
        oam_prov_req.fm_cntxt.History_List_Index = OAM_ZERO;
    }
    return index;
}

/****************************************************************************
 * Function Name  : oam_fm_fetch_expedited_alarm_idx
 * Inputs         : alarm_id :Alarm ID
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return  alarm index of  expedited alarm list.
 ****************************************************************************/
    SInt32
oam_fm_fetch_expedited_alarm_idx (
    UInt16 alarm_id
    )
{
    SInt32 alarm_idx;
    for (alarm_idx = OAM_ZERO; alarm_idx < OAM_EXPEDITED_ALARM_MAX;alarm_idx++)
    {
        if (oam_atoi((Char8 *) oam_prov_req.fm_cntxt.expedited_alarm_list[alarm_idx].alarm_identifier) == alarm_id)
        {
            return alarm_idx;
        }
    }
    return OAM_FM_FAILURE;
}

/****************************************************************************
 * Function Name  : oam_fm_fetch_queued_alarm_idx
 * Inputs         : alarm_id :Alarm ID 
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return  alarm index of  queued alarm list.
 ****************************************************************************/
    SInt32
oam_fm_fetch_queued_alarm_idx (
    UInt16 alarm_id
    )
{
    SInt32 alarm_idx;
    for (alarm_idx = OAM_ZERO; alarm_idx < OAM_QUEUED_ALARM_MAX;alarm_idx++)
    {
        if (oam_atoi((Char8 *) oam_prov_req.fm_cntxt.queued_alarm_list[alarm_idx].alarm_identifier) == alarm_id)
        {
            return alarm_idx;
        }
    }
    return OAM_FM_FAILURE;
}

/****************************************************************************
 * Function Name  :oam_fm_get_new_expedited_alarm_idx
 * Inputs         : none
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return new alarm index of  expedited alarm list.
 ****************************************************************************/
    UInt16
oam_fm_get_new_expedited_alarm_idx (
    void
    )
{
    UInt16 index = oam_prov_req.fm_cntxt.Expedited_List_Index;
    
    if((++oam_prov_req.fm_cntxt.Expedited_List_Index) == OAM_EXPEDITED_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.isExpeditedRolled = OAM_ONE;
        oam_prov_req.fm_cntxt.Expedited_List_Index = OAM_ZERO;
    }

    return index;
}

/****************************************************************************
 * Function Name  : oam_fm_get_new_queued_alarm_idx
 * Inputs         : none 
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return new alarm index of  queued alarm list. 
 ****************************************************************************/

    UInt16
oam_fm_get_new_queued_alarm_idx (
    void
    )
{
    UInt16 index = oam_prov_req.fm_cntxt.Queued_List_Index;
    if((++oam_prov_req.fm_cntxt.Queued_List_Index) == OAM_QUEUED_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.isQueuedRolled = OAM_ONE;
        oam_prov_req.fm_cntxt.Queued_List_Index = OAM_ZERO;
    }

    return index;
}

/****************************************************************************
 * Function Name  : oam_fm_fetch_curr_alarm_idx
 * Inputs         : alarm_id :Alarm ID
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return alarm index of current alarm list.  
 ****************************************************************************/
    int
oam_fm_fetch_curr_alarm_idx (
/* SPR 14630 Fix Start */
    UInt16 alarm_id,
    UInt8 CellId
/* SPR 14630 Fix End */
    )
{
    SInt32 alarm_idx;
    for (alarm_idx = OAM_ZERO; alarm_idx < OAM_CURRENT_ALARM_MAX; alarm_idx++)
    {
        /* Spr 12880 Fix Start */
/* SPR 14630 Fix Start */
        if ((oam_atoi((Char8 *) oam_prov_req.fm_cntxt.current_alarm_list[alarm_idx].alarm_identifier) == alarm_id)&&
            (CellId == oam_prov_req.fm_cntxt.current_alarm_list[alarm_idx].CellId))
/* SPR 14630 Fix End */
        /* Spr 12880 Fix End */
        {
            return alarm_idx;
        }
    }
    return OAM_FM_FAILURE;
}

/****************************************************************************
 * Function Name  : oam_fm_get_new_curr_alarm_idx
 * Inputs         : alarm_id :Alarm ID
 * Outputs        : none
 * Returns        : alarm index
 * Description    : This function used to return new alarm index of current alarm list.
 ****************************************************************************/
    int
oam_fm_get_new_curr_alarm_idx (
    oam_alarm_info_t *p_alarm_info
    )
{
    UInt16 alarm_idx = OAM_ZERO;
    
    UInt16 index = oam_prov_req.fm_cntxt.Current_List_Index;
    if ((oam_prov_req.fm_cntxt.Current_List_Index + OAM_ONE) == OAM_CURRENT_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.Current_List_Index = OAM_ZERO;
        oam_prov_req.fm_cntxt.isCurrentRolled = OAM_ONE;
    }
    for (alarm_idx = OAM_ZERO; alarm_idx < OAM_CURRENT_ALARM_MAX; alarm_idx++)
    {
        /* Spr 12880 Fix Start */
        if ((oam_atoi((Char8 *) oam_prov_req.fm_cntxt.current_alarm_list[alarm_idx].alarm_identifier) == p_alarm_info->alarm_id)
            &&(p_alarm_info->CellId == oam_prov_req.fm_cntxt.current_alarm_list[alarm_idx].CellId))
        {
                oam_prov_req.fm_cntxt.current_alarm_list[alarm_idx].is_cleared++;
                return OAM_FM_FAILURE;
        /* Spr 12880 Fix End */
        }
    }
    return index;
}

/************************************************************************************
 ** ARRAY NAME   :   oam_rrm_table_protocol_event_id
 ** DESCRIPTION  :   Array of structures, to store rrm event_id string and their
 **                  corresponding value
 **
 ** FIELDS       :   event id,event id string
 *************************************************************************************/

oam_rrm_event_id_t oam_rrm_table_protocol_event_id[] = {
{"Cell Setup Reject",RRM_PE_CELL_SETUP_REJECT},
{"RRC Connection Setup Ready",RRM_PE_RRC_CONN_SETUP_READY},
{"Event UE Capability",RRM_PE_EVENT_UE_CAPABILITY},
{"Event UE Capability FGI Reject",RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT},
{"ERAB Setup Attempt",RRM_PE_ERAB_SETUP_ATTEMPT},
{"ERAB Setup Ready",RRM_PE_ERAB_SETUP_READY},
{"ERAB Admission Reject",RRM_PE_ERAB_ADMISSION_REJECT},
{"ERAB DRB Release",RRM_PE_ERAB_DRB_RELEASE},
{"ERAB Release Attempt",RRM_PE_ERAB_RELEASE_ATTEMPT},
{"ERAB Release Ready",RRM_PE_ERAB_RELEASE_READY},
{"ERAB Modify Ready",RRM_PE_ERAB_MODIFY_READY},
{"ERAB Modify Attempt",RRM_PE_ERAB_MODIFY_ATTEMPT},
{"ERAB DRB Allocated",RRM_PE_ERAB_DRB_ALLOCATED},
{"ERAB Setup QCI",RRM_PE_ERAB_SETUP_QCI},
{"ERAB Release Information",RRM_PE_EVENT_ERAB_RELEASE_INFO},
{"ERAB Setup Information",RRM_PE_EVENT_ERAB_SETUP_INFO},
{"MEAS Configuration A1",RRM_PE_EVENT_MEAS_CONFIG_A1},
{"MEAS Configuration A2",RRM_PE_EVENT_MEAS_CONFIG_A2},
{"MEAS Configuration A3",RRM_PE_EVENT_MEAS_CONFIG_A3},
{"MEAS Configuration A4",RRM_PE_EVENT_MEAS_CONFIG_A4},
{"MEAS Configuration A5",RRM_PE_EVENT_MEAS_CONFIG_A5},
{"MEAS Configuration B2 UTRA",RRM_PE_EVENT_MEAS_CONFIG_B2_UTRA},
{"MEAS Configuration B2 GERAN",RRM_PE_EVENT_MEAS_CONFIG_B2_GERAN},
{"UE MEAS Abort",RRM_PE_EVENT_UE_MEAS_ABORT},
{"MEAS Configuration Periodical EUTRA",RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_EUTRA},
{"MEAS Configuration Periodical UTRA",RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_UTRA},
{"MEAS Configuration Periodical GERAN",RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_GERAN},
{"MEAS Configuration B2 CDMA2000",RRM_PE_EVENT_MEAS_CONFIG_B2_CDMA2000},
{"HO IN Execution Complete",RRM_PE_HO_IN_EXEC_COMPLETE},
{"HO IN Prepration Complete",RRM_PE_HO_IN_PREP_COMPLETE},
{"HO IN Execution Attempt",RRM_PE_HO_IN_EXEC_ATTEMPT},
{"HO IN Execution Failed",RRM_PE_HO_IN_EXEC_FAILED},
{"HO IN Prepration Attempt",RRM_PE_EVENT_HO_IN_PREP_ATTEMPT},
{"HO IN Prepration Failure",RRM_PE_EVENT_HO_IN_PREP_FAILURE},
{"HO OUT Prepration Attempt",RRM_PE_HO_OUT_PREP_ATTEMPT},
{"HO OUT Prepration Failed",RRM_PE_HO_OUT_PREP_FAILED},
{"HO OUT Prepration Information",RRM_PE_HO_OUT_PREP_INFO},
{"HO OUT Execution Attempt",RRM_PE_HO_OUT_EXEC_ATTEMPT},
{"HO OUT Execution Failed",RRM_PE_HO_OUT_EXEC_FAILED},
{"HO OUT Execution Complete",RRM_PE_HO_OUT_EXEC_COMPLETE},
{"HO UE Detected",RRM_PE_EVENT_HO_UE_DETECTED},
{"HO OUT Execution Information",RRM_PE_HO_OUT_EXEC_INFO},
{"Too Early HO",RRM_PE_EVENT_TOO_EARLY_HO},
{"Too Late HO",RRM_PE_EVENT_TOO_LATE_HO},
{"HO Wrong Cell",RRM_PE_EVENT_HO_WRONG_CELL},
{"HO Wrong Cell Reset",RRM_PE_EVENT_HO_WRONG_CELL_REEST},
{"X2 RLF Indication",RRM_PE_EVENT_X2_RLF_INDICATION},
{"MME Configuration Transfer",RRM_PE_MME_CONFIGURATION_TRANSFER},
{"ENB Configuration Transfer",RRM_PE_ENB_CONFIGURATION_TRANSFER},
{"SRB1 Setup Reject",RRM_PE_SRB1_SETUP_REJECT}
};

/************************************************************************************
 ** ARRAY NAME   :   oam_rrm_table_event_type
 ** DESCRIPTION  :   Array of structures, to store rrm event_type string and their
 **                  corresponding value
 **
 ** FIELDS       :   event type,event type string
 *************************************************************************************/
oam_rrm_event_type_t oam_rrm_table_event_type[] = {
{"PROTOCOL_EVENT",PROTOCOL_EVENT},
{"ERROR_EVENT",ERROR_EVENT}
};

/************************************************************************************
 ** ARRAY NAME   :   oam_rrm_table_event_sub_type
 ** DESCRIPTION  :   Array of structures, to store rrm event_sub_type string and their
 **                  corresponding value
 **
 ** FIELDS       :   event_sub_type,event_sub_type string
 *************************************************************************************/
oam_rrm_event_sub_type_t oam_rrm_table_event_sub_type[] = {
{"ALL_SUB_CLASS",ALL_SUB_CLASS},
{"CELL_SPECIFIC",CELL_SPECIFIC},
{"UE_SPECIFIC",UE_SPECIFIC},
{"RAB_SPECIFIC",RAB_SPECIFIC},
{"MEASUREMENT_SPECIFIC",MEASUREMENT_SPECIFIC},
{"HO_IN_SPECIFIC",HO_IN_SPECIFIC},
{"HO_OUT_SPECIFIC",HO_OUT_SPECIFIC},
{"OTHER_PROTOCOL_EVENT",OTHER_PROTOCOL_EVENT},
{"TIMER_RELATED",TIMER_RELATED},
{"IPC_SEND_RECV_ERROR_RELATED",IPC_SEND_RECV_ERROR_RELATED}
};

/************************************************************************************
 ** ARRAY NAME   :   oam_table_alarm_report_mechanism
 ** DESCRIPTION  :   Array of structures, to store reporting mechanism and their
 **                  corresponding value
 **
 ** FIELDS       :   reporting mechanism, reporting mechanism string
 ************************************************************************************/
oam_alarm_report_t oam_table_alarm_report_mechanism[] = {
    {"0 Expedited", EXPEDITED_ALARM},
    {"1 Queued", QUEUED_ALARM},
    {"2 Logged", LOGGED_ALARM},
    {"3 Disabled", DISABLED_ALARM},
    {"4 Report Mechanism Undefined", REPORT_MECHANISM_UNDEFINED},
    {PNULL, 0}
};

/************************************************************************************
 ** ARRAY NAME   :   oam_table_alarm_event_type
 ** DESCRIPTION  :   Array of structures, to store event_type and their
 **                  corresponding value
 **
 ** FIELDS       :   event type,event type string
 ************************************************************************************/

oam_alarm_event_t oam_table_alarm_event_type[] = {
    {"Communication Alarm", COMMUNICATIONS_ALARM},
    {"Processing Error Alarm", PROCESSING_ERROR_ALARM},
    {"Memory Mismatch", MEMORY_MISMATCH},
    {"Configuration or Customization Error",
        CONFIGURATION_OR_CUSTOMIZATION_ERROR},
    {"Environmental Alarm", ENVIRONMENTAL_ALARM},
    {"Quality of Service Alarm", QUALITY_OF_SERVICE_ALARM},
    {"Equipment Alarm", EQUIPMENT_ALARM},
    {"Integrity Violation", INTEGRITY_VIOLATION},
    {"Operational Violation", OPERATIONAL_VIOLATION},
    {"Physical Violation", PHYSICAL_VIOLATION},
    {"Security Service of Mechanism Violation",
        SECURITY_SERVICE_OF_MECHANISM_VIOLATION},
    {"Time Domain Violation", TIME_DOMAIN_VIOLATION},
    /*eICIC_PHASE_1_2_CHANGES_START*/
    {"Notification Type", NOTIFICATION_TYPE}
    /*eICIC_PHASE_1_2_CHANGES_END*/
};

/************************************************************************************
 ** ARRAY NAME   :   oam_table_alarm_probable_cause
 ** DESCRIPTION  :   Array of structures, to store probable cause and their
 **                  corresponding value
 **
 ** FIELDS       :   probable cause,probable cause string
 ************************************************************************************/
oam_alarm_probable_t oam_table_alarm_probable_cause[] = {
    {"Out Of Memory", OUT_OF_MEMORY},
    {"Software Error", SOFTWARE_ERROR},
    {"Communication Transmit Failure", COMMUNICATIONS_TRANSMIT_FAILURE},
    {"Communication Receive Failure", COMMUNICATIONS_RECEIVE_FAILURE},
    {"Software Program Error", SOFTWARE_PROGRAM_ERROR},
    {"Connection Establishment Error", CONNECTION_ESTABLISHMENT_ERROR},
    {"Communication Subsystem Failure",COMMUNICATIN_SUBSYSTEM_FAILURE},
    {"Unavailable", UNAVAILABLE},
    {"MESSAGE_OUT_OF_SEQUENCE", MESSAGE_OUT_OF_SEQUENCE}
};

/************************************************************************************
 ** ARRAY NAME   :   oam_table_alarm_severity
 ** DESCRIPTION  :   Array of structures, to store severity  and their
 **                  corresponding value
 **
 ** FIELDS       :  severity, severity string
 ************************************************************************************/
oam_alarm_severity_t oam_table_alarm_severity[] = 
{
    {"Critical", CRITICAL},
    {"Major", MAJOR},
    {"Minor", MINOR},
    {"Warning", WARNING},
    {"Indeterminate", INDETERMINATE},
    {"Cleared", CLEARED}
};

/************************************************************************************
 ** ARRAY NAME   :  oam_table_alarm_notification
 ** DESCRIPTION  :   Array of structures, to store notification type  and their
 **                  corresponding value
 **
 ** FIELDS       :   notification type,notification type string
 ************************************************************************************/
oam_alarm_notif_t oam_table_alarm_notification[] = 
{
    {"NewAlarm ", NOTIFY_NEW_ALARM},
    {"ChangedAlarm", NOTIFY_CHANGED_ALARM},
    {"ClearedAlarm", NOTIFY_CLEARED_ALARM},
    {"Notification", NEW_NOTIFICATION},
};

/****************************************************************************
 * Function Name  : oam_create_timestamp
 * Inputs         : time buffer : Timer Buffer  
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function Handles the time stamp
 ****************************************************************************/
oam_return_et 
oam_create_timestamp (
    Char8  * time_buffer
    )
{
    const struct tm *tm_ptr;
    time_t now;
    now = OAM_TIME (OAM_NULL);
    tm_ptr = OAM_LOCALTIME (&now);
    if(OAM_NULL == tm_ptr)
    {
        OAM_LOG(OAM,OAM_WARNING,"localtime returns NULL for tm_ptr");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }            
    oam_memset (time_buffer, OAM_ZERO, OAM_TIME_SIZE);
    strftime (time_buffer, OAM_TIME_SIZE, "%Y%m%d_%H%M%S", tm_ptr);
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_create_timestamp_with_timezone
 * Inputs         : time buffer : Timer Buffer  
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function Handles the time stamp
 ****************************************************************************/
oam_return_et 
oam_create_timestamp_with_timezone (
    Char8  * time_buffer
    )
{
    const struct tm *tm_ptr;
    time_t now;
    now = OAM_TIME (OAM_NULL);
    tm_ptr = OAM_LOCALTIME (&now);
    if(OAM_NULL == tm_ptr)
    {
        OAM_LOG(OAM,OAM_WARNING,"localtime returns NULL for tm_ptr");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }            
    oam_memset (time_buffer, OAM_ZERO, OAM_TIME_SIZE);
    strftime (time_buffer, OAM_TIME_SIZE, "%Y%m%d_%H%M%S%Z", tm_ptr);
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_init_fault_mgmt
 * Inputs        : None
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : Fault Management Init Function
 ****************************************************************************/
oam_return_et 
oam_init_fault_mgmt (
    )
{
    /* coverity 35659 */
    oam_return_et  retStatus = OAM_SUCCESS;
    UInt16 curr_alarm_idx = OAM_ZERO;
    FILE *file_pointer=OAM_NULL;
    Char8  alarm_raised_time[OAM_TIME_SIZE] = { OAM_ZERO };
    oam_return_et retVal = OAM_SUCCESS;
    /* memset the alarm array */
    oam_prov_req.fm_cntxt.current_alarm_list[curr_alarm_idx].alarm_state =
        OAM_FM_STATE_UNACK_UNCLEAR;
    oam_memset (&oam_prov_req.fm_cntxt, OAM_ZERO, sizeof (oam_fm_cntxt_t));
    
    oam_prov_req.fm_cntxt.CurrentAlarmNumberOfEntries = OAM_ZERO;
    oam_prov_req.fm_cntxt.HistoryEventNumberOfEntries=OAM_HISTORY_ALARM_MAX;
    oam_prov_req.fm_cntxt.ExpeditedEventNumberOfEntries = OAM_EXPEDITED_ALARM_MAX;
    oam_prov_req.fm_cntxt.QueuedEventNumberOfEntries = OAM_QUEUED_ALARM_MAX;
    oam_prov_req.fm_cntxt.MaxCurrentAlarmEntries = OAM_CURRENT_ALARM_MAX;

    /* Coverity FIX 33638 */       
    retVal = oam_create_timestamp(alarm_raised_time);
    if(OAM_FAILURE == retVal)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_create_timestamp failed");
        return retVal;
    }

    oam_strncat(Alarm_log_file_name,"AlarmLog_",oam_strlen("AlarmLog_"));
    oam_strncat(Alarm_log_file_name,alarm_raised_time,sizeof(alarm_raised_time));
    oam_strncat(Alarm_log_file_name,".csv\0",oam_strlen(".csv\0"));
   
   	
    file_pointer=oam_fopen(Alarm_log_file_name,"w");
    
    if(OAM_NULL == file_pointer)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_fopen failed: error in opening AlarmLog file");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    oam_fprintf (file_pointer,"TimeStamp(MMDDYYYY:hh:mm:ss),ModuleName,AlarmId,NotificationType,EventType,ProbableCause,SpecificProblem,Criticality,ReportingMechanism,Description\n");
    OAM_FCLOSE(file_pointer);
    return retStatus;
}

/****************************************************************************
 * Function Name : oam_find_module_name
 * Inputs        : src_id :source identifier          
 *                 module_name :Pointer to module name  
 *Outputs        : None
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : Finds module Name corresponding to source id
 *****************************************************************************/
oam_return_et 
oam_find_module_name (
    UInt8 src_id, 
    Char8  * module_name
    )
{
    oam_return_et ret_status = OAM_SUCCESS;
    switch (src_id)
    {
        case RRC_MODULE_ID:
            oam_strcpy (module_name, "RRC");
            break;

        case PDCP_MODULE_ID:
            oam_strcpy (module_name, "PDCP");
            break;

        case RLC_MODULE_ID:
            oam_strcpy (module_name, "RLC");
            break;

        case MAC_MODULE_ID:
            oam_strcpy (module_name, "MAC");
            break;
        case OAM_MODULE_ID:
            oam_strcpy (module_name, "OAM");
            break;
        case RRM_MODULE_ID:
            oam_strcpy (module_name, "RRM");
            break;
        /* SPR FIX 10190 */
        case SON_MODULE_ID:
            oam_strcpy (module_name, "SON");
            break;
       /*SPR FIX 12977 */
	case EGTPU_MODULE_ID:
	    oam_strcpy (module_name, "EGTPU");
	    break;

      /*SPR FIX 12977 */

	default:
            OAM_LOG (OAM, OAM_WARNING, "Unrecognized src module id[%u]", 
                src_id);
            return OAM_FAILURE;
    }
    return ret_status;
}

/****************************************************************************
 * Function Name : oam_valid_alarmid
 * Inputs        : alarm id :Alarm ID
 * Outputs       : None
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : Checks Alarm Id is valid or not
 *******************************************************************************/
oam_return_et 
oam_valid_alarmid (
    UInt16 alarm_id
    )
{
    UInt16 validate_alarm_id;
    for (validate_alarm_id = OAM_ZERO; validate_alarm_id < OAM_SUPPORTED_ALARM_MAX; validate_alarm_id++)	//need to be updated 
    {
        if (oam_prov_req.fm_cntxt.supported_alarm_list[validate_alarm_id].alarm_identifier == alarm_id)
        {
            return OAM_SUCCESS;
        }
    }

    return OAM_FAILURE;

}
/****************************************************************************
 * Function Name : get_module_id_for_alarm
 * Inputs        : module_id
 * Outputs       : module_name
 * Returns       : name of the module based on module 
 * Description   : this function finds name of the module based on module id
 *******************************************************************************/

Char8* get_module_id_for_alarm(UInt16 module_id)
{
    Char8 *module_name;

    switch (module_id)
    {
        case RRC_MODULE_ID:
        {
           module_name = "[RRC]";
            break;
        }
        case MAC_MODULE_ID:
        {
           module_name = "[MAC]";
            break;
        }
        case PDCP_MODULE_ID:
        {
           module_name = "[PDCP]";
            break;
        }
        case OAM_MODULE_ID:
        {
           module_name = "[OAM]";
            break;
        }
	/*FIX SPR 10190 start*/
        case SON_MODULE_ID:
        {
           module_name = "[SON]";
            break;
        }
	/*FIX SPR 10190 end*/
        /*SPR FIX 12977 */
        case EGTPU_MODULE_ID:
        {
            module_name = "[EGTPU]";
            break;
        }
        /*SPR FIX 12977 */
        /*SPR 14811 start*/
        case RRM_MODULE_ID:
        {
           module_name = "[RRM]";
            break;
        }

        /*SPR 14811 end*/
     
       default:
        {
           module_name = " ";
           return module_name;
        }
    }
    return module_name;
}

/****************************************************************************
 ** Function Name : oam_handle_lower_layer_alarm
 ** Inputs        : p_api_buf :Pointer to api buffer 
 ** Outputs       : none
 ** Returns       : OAM_SUCCESS/OAM_FAILURE
 ** Description   : This function handles lower layer alarms
 *******************************************************************************/
oam_return_et 
oam_handle_lower_layer_alarm (
    void *p_api_buf
    )
{
    UInt16 alarm_id = OAM_ZERO;
/* SPR 14630 Fix code removed */
    UInt16 *alarm_id_ptr = OAM_NULL;
    UInt16 *criticality_ptr = OAM_NULL;
    UInt16 criticality = OAM_ZERO;
/* SPR 14630 Fix code removed */
    Char8 add_text[OAM_MAX_TEXT_SIZE] = { OAM_ZERO };
    UInt16 src_id = OAM_ZERO;
/* SPR 14630 Fix code removed */
    UInt8 current_loc = OAM_ZERO;
    UInt16 api_id = OAM_ZERO;
    oam_alarm_info_t *p_alarm_info={OAM_ZERO};
/*Coverity FIX */
    oam_error_code_et p_err= NO_ERROR; 
/* SPR 14630 Fix Start */
    oam_alarm_info_t *p_alarm_cleared={OAM_ZERO};
    /*  coverity 93919 fix start */
    oam_return_et ret_val;
    /*  coverity 93919 fix end */
/* SPR 14630 Fix End */
        /* Spr 12880 Fix Start */
    UInt8 CellId = OAM_INVALID_CELL_ID;
        /* Spr 12880 Fix End */
/*SPR FIX 10190 */

#ifdef OAM_SON_ENABLE
    SInt32 length_read = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO; 
    son_oam_event_notification_t son_oam_event_notification;
#endif


    OAM_FUN_ENTRY_TRACE ();
    api_id   = get_word_from_header((UInt8*)(p_api_buf) + OAM_EXT_API_ID_FROM_HEADER);
    
    if(OAM_FAILURE==oam_mem_alloc(sizeof(oam_alarm_info_t),(void *)&p_alarm_info,&p_err))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_alarm_info failed"
                " with Error Code:%d",
                p_err);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }

/* SPR 14630 Fix code removed */
/* SPR 14630 Fix Start */
     oam_memset(p_alarm_info,OAM_ZERO,sizeof(oam_alarm_info_t));
/* SPR 14630 Fix End */
        

    /* Extract Souce Id */
    src_id = get_word_from_header ((UInt8 *) (p_api_buf) + OAM_INTF_HDR_SRC_ID_OFFSET);

    if (api_id == MAC_PHY_OAM_ERROR_IND)
    {
        /* Extracting Cell_ID as Alarm is cell wise */
        /* Spr 12880 Fix Start */
        CellId = get_word_from_header ((UInt8 *) (p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET);
        /* Spr 12880 Fix End */
        ret_val = oam_mac_handle_error_inds(p_api_buf,
			    &alarm_id,
			    &criticality,
			    add_text);
        /*  coverity 93916 fix start */
        if(OAM_FAILURE == ret_val) {
            OAM_LOG (OAM, OAM_WARNING, "Unable to handle error indication");
            /* SPR 21077 Fixed Start */
           // oam_mem_free(p_alarm_info,&p_err);
           /* SPR 21077 Fixed Start */
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
           }
        /*  coverity 93916 fix end */
    }
/*SPR FIX 10190 start */
#ifdef OAM_SON_ENABLE
    else if(SON_MODULE_ID == src_id) {
        msg_len = get_word_from_header((U8*)(p_api_buf) +
                                        OAM_INTF_HDR_MSG_SIZE_OFFSET);

        msg_len -= LOWER_LAYER_INTERFACE_HEADER_SIZE;
        ret_val = oam_parse_son_oam_event_notification(
                               &son_oam_event_notification,
                               p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE,
                               msg_len,
                               &length_read);
        if(OAM_FAILURE == ret_val) {
            OAM_LOG (OAM, OAM_WARNING, "Unable to parse event notification");
            /* Coverity ID :72297  Fix Start*/
            oam_mem_free(p_alarm_info, &p_err);
            /* Coverity ID : 72297 Fix End*/
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
           }
        if(son_oam_event_notification.msg_header.event_type == (son_oam_event_class_et)SON_OAM_ERROR_EVENT)
        {
          OAM_LOG (OAM, OAM_WARNING, " Error Event found in SON ");
            alarm_id   = oam_son_get_alarm_id(son_oam_event_notification.msg_header.event_id);
/*COVERITY FIX START (CID 64324)*/
        if(alarm_id == BASE_GENERIC_ALARM_ID)
/*COVERITY FIX END (CID 64324)*/
            {
                OAM_LOG (OAM, OAM_WARNING, " Invalid Alarm ID[%d] received ", alarm_id);
                /* SPR_19838 : Coverity_11147 Fix Start */
                oam_mem_free(p_alarm_info,&p_err);
                /* SPR_19838 : Coverity_11147 Fix Stop */
                OAM_FUN_EXIT_TRACE ();
                return OAM_FAILURE;
            }
        else if(alarm_id == MEM_ALLOC_FAILED_ALARM_ID || alarm_id == MEM_FREE_FAILED_ALARM_ID || alarm_id == SEND_MSG_FAILED_ALARM_ID || alarm_id == RECV_MSG_FAILED_ALARM_ID)
            {
                criticality = MAJOR;
            }
        else if(alarm_id == TIMER_START_FAILED_ALARM_ID || alarm_id == TIMER_STOP_FAILED_ALARM_ID)
            {
                criticality = MINOR;
            }

            oam_strncpy(add_text,
                    (Char8*)son_oam_event_notification.api_data.associated_data,
                    OAM_MAX_TEXT_SIZE);
        }
    } 
#endif
/*SPR FIX 10190 end*/
/* SPR 13765 changes start */
    else if (PR_GTPU_ERROR_IND == api_id)
    {
        ret_val = oam_egtpu_handle_error_inds(p_api_buf,
			    &alarm_id,
			    &criticality,
			    add_text);
        if(OAM_FAILURE == ret_val) {
            OAM_LOG (OAM, OAM_WARNING, "Unable to handle error indication");
            oam_mem_free(p_alarm_info,&p_err);
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
           }

    }

/* SPR 13765 changes end */
    else 
    { 
        /* Extract Alarm Id */
        alarm_id_ptr = (p_api_buf +  LOWER_LAYER_INTERFACE_HEADER_SIZE);
        alarm_id = *(UInt8 *) alarm_id_ptr;
        alarm_id = alarm_id << OAM_EIGHT;
        alarm_id |= *((UInt8 *) alarm_id_ptr + OAM_ONE);

        current_loc = current_loc + OAM_TWO +LOWER_LAYER_INTERFACE_HEADER_SIZE;

        criticality_ptr = (p_api_buf + current_loc);
        criticality = *(UInt8 *) criticality_ptr;
        criticality = criticality << OAM_EIGHT;
        criticality |= *((UInt8 *) criticality_ptr + OAM_ONE);

        current_loc = current_loc + OAM_TWO;
        oam_strncpy (add_text, (p_api_buf + current_loc),
                OAM_MAX_TEXT_SIZE-1);
        add_text[OAM_MAX_TEXT_SIZE-1] = '\0';
    }
/* SPR 14630 Fix Start */
    p_alarm_info->alarm_id = alarm_id; 
    p_alarm_info->criticality = criticality; 
            p_alarm_info->module_id = src_id;
    oam_strcpy (p_alarm_info->add_txt, (Char8 *) add_text);
    add_text[OAM_ZERO]='\0';

    oam_populate_alarm_info(p_alarm_info); 

        /* Spr 12880 Fix Start */
    p_alarm_info->CellId = CellId; 
        /* Spr 12880 Fix End */
    if(OAM_FAILURE == oam_handle_alarm (p_alarm_info))
            {
        OAM_LOG(OAM,OAM_WARNING,"OAM Failed to Raise Alarm [%d]",
                p_alarm_info->alarm_id);
        oam_mem_free(p_alarm_info,&p_err);
        return OAM_FAILURE;
            }
    oam_mem_free(p_alarm_info,&p_err);

    /* Check wheather any alarm corresponding to this alarm needs to be cleared */
    if(OAM_SUCCESS == oam_find_alarm_clearing_info(alarm_id,&p_alarm_cleared,CellId))
            {
        if(OAM_NULL != p_alarm_cleared)
            {
            /* Copying Src ID Same as Clearing Alarm Should be raised from the same layer */
            p_alarm_cleared->module_id = src_id;

            oam_populate_alarm_info(p_alarm_cleared); 

            if(OAM_FAILURE == oam_handle_alarm (p_alarm_cleared))
            {
              OAM_LOG(OAM,OAM_WARNING,"Clearing for Alarm [%d] Failed from eNB DB",
                                                p_alarm_cleared->alarm_id);
               oam_mem_free(p_alarm_cleared, &p_err);
               return OAM_FAILURE;

        }
            OAM_LOG(OAM,OAM_INFO,"Clearing for Alarm [%d] Successfull from eNB DB",
                      p_alarm_cleared->alarm_id);

            oam_mem_free(p_alarm_cleared ,&p_err);
    }
 }

    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
/* SPR 14630 Fix End */


/* SPR 14630 Fix code removed */
}

/****************************************************************************
 * Function Name : oam_handle_raised_alarm
 * Inputs        : p_alarm_info :Pointer to alarm_info structure.
 * Outputs       : None
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function is used for handle raised alarm.
 *****************************************************************************/
oam_return_et
oam_handle_raised_alarm (
    oam_alarm_info_t *p_alarm_info
    )
{
    OAM_FUN_ENTRY_TRACE ();
    FILE *file_pointer=OAM_NULL;
    SInt32 expedited_alarm_index = OAM_ZERO, queued_alarm_index = OAM_ZERO;
    SInt32 curr_alarm_idx = OAM_ZERO;
    if (p_alarm_info->reporting_mechanism == DISABLED_ALARM)
    {
        OAM_FUN_EXIT_TRACE ();
        return OAM_SUCCESS;
    }
    file_pointer=oam_fopen(Alarm_log_file_name,"a");
    if(OAM_NULL == file_pointer)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_fopen failed for Alarms_Log");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }    
    oam_fprintf (file_pointer,"%s,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
            p_alarm_info->time_stamp, p_alarm_info->module_name, 
            p_alarm_info->alarm_id, 
            oam_table_alarm_notification[p_alarm_info->notification_type].notification_type,
            oam_table_alarm_event_type[p_alarm_info->event_type].event_type,
            p_alarm_info->probable_cause_str, 
            p_alarm_info->specific_problem,
            oam_table_alarm_severity[p_alarm_info->criticality].perceived_severity, 
            oam_table_alarm_report_mechanism[p_alarm_info->reporting_mechanism].report_type,
            p_alarm_info->add_txt, p_alarm_info->add_info);
    OAM_FFLUSH(file_pointer);
    OAM_FCLOSE(file_pointer);

    if ((p_alarm_info->reporting_mechanism == EXPEDITED_ALARM)
            ||(p_alarm_info->reporting_mechanism == QUEUED_ALARM)
            || (p_alarm_info->reporting_mechanism == LOGGED_ALARM))
    {

        /** function to fetch current alarm index of  current alarm list */

        if (p_alarm_info->reporting_mechanism == EXPEDITED_ALARM)
        {
            /* SPR 15363 START */ //CSR #102101
            //expedited_alarm_index = oam_fm_get_new_expedited_alarm_idx ();
            //if(OAM_EXPEDITED_ALARM_MAX > expedited_alarm_index) 
            expedited_alarm_index = oam_fm_fetch_expedited_alarm_idx(p_alarm_info->alarm_id);
            if (expedited_alarm_index == OAM_FM_FAILURE)
            {
                expedited_alarm_index = oam_fm_get_new_expedited_alarm_idx ();
            }
            /* SPR 15363 END */
            oam_fm_populate_expedited_alarm_record (
                    &(oam_prov_req.fm_cntxt.expedited_alarm_list[expedited_alarm_index]),
                    p_alarm_info);
        }
        if (p_alarm_info->reporting_mechanism == QUEUED_ALARM)
        {
            /* SPR 15363 START */ //CSR #102101
            //queued_alarm_index = oam_fm_get_new_queued_alarm_idx ();
            //if(OAM_QUEUED_ALARM_MAX > queued_alarm_index)
            queued_alarm_index = oam_fm_fetch_queued_alarm_idx(p_alarm_info->alarm_id);
            if (queued_alarm_index == OAM_FM_FAILURE)
            {
                queued_alarm_index = oam_fm_get_new_queued_alarm_idx ();
            }
            /* SPR 15363 END */
            oam_fm_populate_queued_alarm_record (
                    &(oam_prov_req.fm_cntxt.queued_alarm_list[queued_alarm_index]),
                    p_alarm_info);
        }
        /*** function to make entry in current alarm list */

        curr_alarm_idx = oam_fm_get_new_curr_alarm_idx (p_alarm_info);

        if (curr_alarm_idx == OAM_FM_FAILURE)
        {
            /* SPR 15363 START */ //CSR #102101
            curr_alarm_idx = oam_fm_fetch_curr_alarm_idx (p_alarm_info->alarm_id,p_alarm_info->CellId);
            /* Coverity 94901 94895*/
            if(curr_alarm_idx != OAM_FM_FAILURE)
            {    
            oam_strncpy ((char *)oam_prov_req.fm_cntxt.current_alarm_list[curr_alarm_idx].alarm_changed_time,
                    (const char *) p_alarm_info->time_stamp,
                    oam_strlen((Char8 *) p_alarm_info->time_stamp) + 1);

            OAM_LOG (OAM, OAM_DETAILED, "updating the new changed alarm time for alarm id %d", p_alarm_info->alarm_id);
            }
            else
            {
                OAM_LOG (OAM, OAM_WARNING, " Alarm id %d not found for cell id %d !! ", p_alarm_info->alarm_id,p_alarm_info->CellId);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }
            /* SPR 15363 END */
        }
        else
        {
            ++oam_prov_req.fm_cntxt.Current_List_Index;

            if(OAM_CURRENT_ALARM_MAX > curr_alarm_idx){
            oam_fm_populate_current_alarm_record (
                    &(oam_prov_req.fm_cntxt.current_alarm_list[curr_alarm_idx]),
                    p_alarm_info);
            }
        }

        curr_alarm_idx = oam_fm_get_new_history_alarm_idx ();
        if(OAM_HISTORY_ALARM_MAX > curr_alarm_idx)
        oam_fm_populate_history_alarm_record (
                &(oam_prov_req.fm_cntxt.history_alarm_list[curr_alarm_idx]),
                p_alarm_info);
    }
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_handle_cleared_alarm
 * Inputs        : p_alarm_info : Pointer to alarm_info structure.
 * Outputs       : None 
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function is used for handle cleared alarm.
 *****************************************************************************/
oam_return_et
oam_handle_cleared_alarm (
    oam_alarm_info_t *p_alarm_info
    )
{
    OAM_FUN_ENTRY_TRACE ();
    FILE *file_pointer=OAM_NULL;
    SInt32 curr_alarm_idx = OAM_ZERO, expedited_alarm_index = OAM_ZERO, queued_alarm_index = OAM_ZERO;
    if (p_alarm_info->reporting_mechanism == DISABLED_ALARM)
    {
        OAM_FUN_EXIT_TRACE ();
        return OAM_SUCCESS;
    }
    file_pointer=oam_fopen(Alarm_log_file_name,"a");
    if(OAM_NULL == file_pointer)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_fopen failed For Alarms_Log");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }    
    oam_fprintf (file_pointer,"%s,%s,%d,%d,%d,%s,%s,%d,%d,%s:",
            p_alarm_info->time_stamp,p_alarm_info->module_name,p_alarm_info->alarm_id, 
            p_alarm_info->notification_type, p_alarm_info->event_type, p_alarm_info->probable_cause_str,p_alarm_info->specific_problem,
            p_alarm_info->criticality, p_alarm_info->reporting_mechanism,
            p_alarm_info->add_txt); 
    OAM_FFLUSH(file_pointer);
    oam_fclose(file_pointer);

  /* klocwork fix 138*/
  /* coverity 63867 63868 */
  if ((p_alarm_info->reporting_mechanism == EXPEDITED_ALARM)
      ||(p_alarm_info->reporting_mechanism == QUEUED_ALARM)
      || (p_alarm_info->reporting_mechanism == LOGGED_ALARM)
      || (p_alarm_info->reporting_mechanism == DISABLED_ALARM))
  {

    if (p_alarm_info->reporting_mechanism == EXPEDITED_ALARM)
    {
        expedited_alarm_index = oam_fm_get_new_expedited_alarm_idx ();
        if(OAM_EXPEDITED_ALARM_MAX > expedited_alarm_index)
        oam_fm_populate_expedited_alarm_record (
                &(oam_prov_req.fm_cntxt.expedited_alarm_list[expedited_alarm_index]),
                p_alarm_info);
    }
    if (p_alarm_info->reporting_mechanism == QUEUED_ALARM)
    {
        queued_alarm_index = oam_fm_get_new_queued_alarm_idx ();
        if(OAM_QUEUED_ALARM_MAX > queued_alarm_index)
        oam_fm_populate_queued_alarm_record (
                &(oam_prov_req.fm_cntxt.queued_alarm_list[queued_alarm_index]),
                p_alarm_info);
    }

    curr_alarm_idx = oam_fm_get_new_history_alarm_idx ();

    /* function to make entry into history list */
    if(OAM_HISTORY_ALARM_MAX > curr_alarm_idx)
    oam_fm_populate_history_alarm_record (
            &(oam_prov_req.fm_cntxt.history_alarm_list[curr_alarm_idx]),
            p_alarm_info);
    }
    else
    {
       OAM_LOG(OAM, OAM_WARNING, "INVALID/UNDEFINED REPORTING MECHANISM[curr_alarm_idx =%d]",
       curr_alarm_idx);
    }
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_fm_populate_queued_alarm_record
 * Inputs        : p_queued_alarm_record :Pointer to queued alarm record
 *                 p_alarm_info :Pointer to alarm_info structure
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function populate records in QUEUED_ALARM_LIST.
 *****************************************************************************/
oam_return_et
oam_fm_populate_queued_alarm_record (
    oam_queued_alarm_info_t *p_queued_alarm_record,
    oam_alarm_info_t * p_alarm_info
    )
{
    OAM_FUN_ENTRY_TRACE ();
    Char8  module_name[OAM_MODULE_NAME_SIZE] = { OAM_ZERO };

    oam_strncpy(p_queued_alarm_record->event_time,
                p_alarm_info->time_stamp,
                OAM_ALARM_DATE_TIME_MAX);

    oam_sprintf(p_queued_alarm_record->alarm_identifier, "%d",
            p_alarm_info->alarm_id);

    /* coverity 35656 */
    p_queued_alarm_record->notification_type = (alarm_notify_type_et)p_alarm_info->notification_type;
    oam_strncpy(p_queued_alarm_record->notification_type_str,
               oam_get_alarm_notification_type_by_notification_id(p_alarm_info->notification_type),
               OAM_ALARM_NOTIF_TYPE_MAX_LEN);

    if (OAM_FAILURE ==
            oam_find_module_name(p_alarm_info->module_id, module_name))
    {
        oam_strcpy (module_name, p_alarm_info->module_name);
    }

    oam_sprintf (p_queued_alarm_record->managed_object_instance,
            "%s[%d]", module_name, p_alarm_info->module_id);

    /* coverity 35655 */
    p_queued_alarm_record->event_type = (alarm_event_type_et)p_alarm_info->event_type;
    /* coverity 35658 */
    p_queued_alarm_record->probable_cause = (alarm_probable_cause_et)p_alarm_info->probable_cause;
    oam_strcpy(p_queued_alarm_record->probable_cause_str,p_alarm_info->probable_cause_str); 
    oam_strcpy (p_queued_alarm_record->specific_problem,p_alarm_info->specific_problem); 

    /* coverity 35657 */
    p_queued_alarm_record->perceived_severity = (FM_alarm_severity_et)p_alarm_info->criticality;
    oam_strcpy((char *)p_queued_alarm_record->managed_object_instance,(char *)p_alarm_info->managed_object_instance);

    if(!oam_strlen(p_alarm_info->add_txt)) {
        p_alarm_info->add_txt[0] = '-';
    }
    oam_strncpy(p_queued_alarm_record->additional_text,
                p_alarm_info->add_txt, OAM_ADDITIONAL_TEXT_SIZE);
    oam_strncpy(p_queued_alarm_record->additional_information,
                p_alarm_info->add_info, OAM_ADDITIONAL_INFORMATION_SIZE);
    oam_strncpy(p_queued_alarm_record->event_type_str,
               oam_get_alarm_event_by_event_id(p_alarm_info->event_type),
               OAM_ALARM_EVENT_TYPE_MAX_LEN);

    oam_memset(p_queued_alarm_record->perceived_severity_str, OAM_ZERO,
               sizeof(p_queued_alarm_record->perceived_severity_str));
    oam_strncpy(p_queued_alarm_record->perceived_severity_str,
       oam_get_alarm_severity_type_by_severity_id(p_alarm_info->criticality),
       OAM_ALARM_PERCEIVED_SEV_MAX_LEN);


    oam_strncpy(p_queued_alarm_record->reporting_mechanism_str,
            oam_table_alarm_report_mechanism[p_alarm_info->reporting_mechanism].report_type,
            OAM_ALARM_REPORTING_MECH_MAX_LEN);

        /* Spr 12880 Fix Start */
    p_queued_alarm_record->CellId =  p_alarm_info->CellId;
        /* Spr 12880 Fix End */

/* SPR-8133,34,25 FIX START */
    oam_prov_req.fm_cntxt.Current_QueuedEventNumberOfEntries++;
    if (oam_prov_req.fm_cntxt.Current_QueuedEventNumberOfEntries > OAM_QUEUED_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.Current_QueuedEventNumberOfEntries  = OAM_QUEUED_ALARM_MAX;
    }
/* SPR-8133,34,25 FIX END */
   
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_fm_populate_expedited_alarm_record
 * Inputs        : p_expedited_alarm_record :Pointer to expedited alarm record
 *                 p_alarm_info :Pointer to alarm_info structure
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function populate records in EXPEDITED_ALARM_LIST.
 *****************************************************************************/
oam_return_et
oam_fm_populate_expedited_alarm_record (
    oam_expedited_alarm_info_t *p_expedited_alarm_record,
    oam_alarm_info_t * p_alarm_info
    )
{
    OAM_FUN_ENTRY_TRACE ();
    Char8  module_name[OAM_MODULE_NAME_SIZE] = { OAM_ZERO };

    oam_strncpy (p_expedited_alarm_record->event_time,
            p_alarm_info->time_stamp,
            oam_strlen (p_alarm_info->time_stamp) + 1);


    oam_sprintf (p_expedited_alarm_record->alarm_identifier, "%d",
            p_alarm_info->alarm_id);

    /* coverity 35648 */
    p_expedited_alarm_record->notification_type = (alarm_notify_type_et)p_alarm_info->notification_type;
    oam_strcpy(p_expedited_alarm_record->notification_type_str,oam_get_alarm_notification_type_by_notification_id(p_alarm_info->notification_type));

    if (OAM_FAILURE ==
            oam_find_module_name (p_alarm_info->module_id, module_name))
    {
        oam_strcpy (module_name, p_alarm_info->module_name);
    }

     oam_strcpy(p_expedited_alarm_record->managed_object_instance,p_alarm_info->managed_object_instance);
    /* coverity 35647 */
    p_expedited_alarm_record->event_type = (alarm_event_type_et)p_alarm_info->event_type;
    /* coverity 35650 */
    p_expedited_alarm_record->probable_cause = (alarm_probable_cause_et)p_alarm_info->probable_cause;
    oam_strcpy(p_expedited_alarm_record->probable_cause_str,p_alarm_info->probable_cause_str); 
    oam_strcpy (p_expedited_alarm_record->specific_problem,p_alarm_info->specific_problem); 

    /* coverity 35649*/
    p_expedited_alarm_record->perceived_severity = (FM_alarm_severity_et)p_alarm_info->criticality;

    if(!oam_strlen(p_alarm_info->add_txt)) {
        p_alarm_info->add_txt[0] = '-';
    }
    oam_strncpy(p_expedited_alarm_record->additional_information,
                p_alarm_info->add_info, OAM_MAX_TEXT_SIZE);
    oam_strncpy(p_expedited_alarm_record->additional_text,
                p_alarm_info->add_txt, OAM_MAX_TEXT_SIZE);

    oam_strcpy(p_expedited_alarm_record->event_type_str,
               oam_get_alarm_event_by_event_id(p_alarm_info->event_type));

    oam_strncpy(p_expedited_alarm_record->perceived_severity_str,
       oam_get_alarm_severity_type_by_severity_id(p_alarm_info->criticality),
       OAM_ALARM_PERCEIVED_SEV_MAX_LEN);

    oam_strncpy(p_expedited_alarm_record->reporting_mechanism_str,
            oam_table_alarm_report_mechanism[p_alarm_info->reporting_mechanism].report_type,
            OAM_ALARM_REPORTING_MECH_MAX_LEN);
        /* Spr 12880 Fix Start */
    p_expedited_alarm_record->CellId =  p_alarm_info->CellId;
        /* Spr 12880 Fix End */

/* SPR-8133,34,35 FIX START */
    oam_prov_req.fm_cntxt.Current_ExpeditedEventNumberOfEntries++;
    if (oam_prov_req.fm_cntxt.Current_ExpeditedEventNumberOfEntries > OAM_EXPEDITED_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.Current_ExpeditedEventNumberOfEntries  = OAM_EXPEDITED_ALARM_MAX;
    }
   
/* SPR-8133,34,35 FIX END */
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_fm_populate_current_alarm_record
 * Inputs        : p_current_alarm_record :Pointer to current alarm record
 *                 p_alarm_info :Pointer to alarm_info structure
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function populates records in CURRENT_ALARM_LIST
 *****************************************************************************/
oam_return_et
oam_fm_populate_current_alarm_record (
    oam_current_alarm_info_t *p_current_alarm_record,
    oam_alarm_info_t * p_alarm_info
    )
{
    OAM_FUN_ENTRY_TRACE ();

    Char8  module_name[OAM_MODULE_NAME_SIZE] = { OAM_ZERO };

    oam_sprintf(p_current_alarm_record->alarm_identifier, "%d",
            p_alarm_info->alarm_id);

    oam_strncpy(p_current_alarm_record->alarm_raised_time,
                p_alarm_info->time_stamp, OAM_ALARM_IDENTIFIER_SIZE);

    oam_strncpy(p_current_alarm_record->alarm_changed_time,
                p_alarm_info->time_stamp, OAM_ALARM_DATE_TIME_MAX);

    if (OAM_FAILURE ==
            oam_find_module_name (p_alarm_info->module_id, module_name))
    {
        oam_strcpy (module_name, p_alarm_info->module_name);
    }

    /* coverity 35644 53535 53542*/
    p_current_alarm_record->event_type = (alarm_event_type_et)p_alarm_info->event_type;
    
    /* coverity 35646 */
    p_current_alarm_record->probable_cause = (alarm_probable_cause_et)p_alarm_info->probable_cause;
    oam_strncpy(p_current_alarm_record->probable_cause_str,
               p_alarm_info->probable_cause_str,
               OAM_ALARM_PROB_CAUSE_STR_MAX_LEN);   
    oam_strncpy(p_current_alarm_record->specific_problem,
                p_alarm_info->specific_problem,
                OAM_SPECIFIC_PROBLEM_SIZE);  

    /* coverity 35645 */
    p_current_alarm_record->perceived_severity = (FM_alarm_severity_et)p_alarm_info->criticality;


    if(!oam_strlen(p_alarm_info->add_txt)) {
        p_alarm_info->add_txt[0] = '-';
    }
    oam_strncpy(p_current_alarm_record->additional_information,
                p_alarm_info->add_info, OAM_MAX_TEXT_SIZE);
    oam_strncpy(p_current_alarm_record->additional_text,
                p_alarm_info->add_txt, OAM_MAX_TEXT_SIZE);

    oam_strncpy(p_current_alarm_record->event_type_str,
                oam_get_alarm_event_by_event_id(p_alarm_info->event_type),
                OAM_ALARM_EVENT_TYPE_MAX_LEN);
    oam_strncpy(p_current_alarm_record->perceived_severity_str,
       oam_get_alarm_severity_type_by_severity_id(p_alarm_info->criticality),
       OAM_ALARM_PERCEIVED_SEV_MAX_LEN);

     oam_strncpy(p_current_alarm_record->managed_object_instance,
                 p_alarm_info->managed_object_instance,
                 OAM_MANAGED_OBJECT_INSTANCE_SIZE);

    oam_strncpy(p_current_alarm_record->reporting_mechanism_str,
                oam_table_alarm_report_mechanism[p_alarm_info->reporting_mechanism].report_type,
                OAM_ALARM_REPORTING_MECH_MAX_LEN);
        /* Spr 12880 Fix Start */
    p_current_alarm_record->CellId =  p_alarm_info->CellId;
        /* Spr 12880 Fix End */

    oam_prov_req.fm_cntxt.CurrentAlarmNumberOfEntries++;
    if(oam_prov_req.fm_cntxt.CurrentAlarmNumberOfEntries > OAM_CURRENT_ALARM_MAX)
    {
        oam_prov_req.fm_cntxt.CurrentAlarmNumberOfEntries  = OAM_CURRENT_ALARM_MAX;
    }
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_fm_populate_history_Alarm_record
 * Inputs        : p_history_alarm_record :Pointer to history alarm record
 *                 p_alarm_info :Pointer to alarm_info structure
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function populates records in HISTORY_ALARM_LIST
 *****************************************************************************/

oam_return_et
oam_fm_populate_history_alarm_record (
    oam_history_alarm_info_t *p_history_alarm_record,
    oam_alarm_info_t * p_alarm_info
    )
{

    OAM_FUN_ENTRY_TRACE ();
    Char8  module_name[OAM_MODULE_NAME_SIZE] = { OAM_ZERO };


    oam_strncpy(p_history_alarm_record->event_time,
                p_alarm_info->time_stamp, OAM_ALARM_DATE_TIME_MAX);


    oam_sprintf(p_history_alarm_record->alarm_identifier, "%d",
            p_alarm_info->alarm_id);

    /* coverity 35652 */
    p_history_alarm_record->notification_type = (alarm_notify_type_et)p_alarm_info->notification_type;
    oam_strncpy(p_history_alarm_record->notification_type_str,
                oam_get_alarm_notification_type_by_notification_id(p_alarm_info->notification_type),
                OAM_ALARM_NOTIF_TYPE_MAX_LEN);

    if (OAM_FAILURE == oam_find_module_name(p_alarm_info->module_id, module_name)) {
        oam_strcpy (module_name, "---");
    }
    else {
        oam_strcpy ((Char8 *) module_name, (Char8 *) p_alarm_info->module_name);
    }

    /* coverity 35651 */
    p_history_alarm_record->event_type = (alarm_event_type_et)p_alarm_info->event_type;
    oam_strncpy(p_history_alarm_record->event_type_str,
               oam_get_alarm_event_by_event_id(p_alarm_info->event_type),
               OAM_ALARM_EVENT_TYPE_MAX_LEN);

    /* coverity 35654 */
    p_history_alarm_record->probable_cause = (alarm_probable_cause_et)p_alarm_info->probable_cause;
    oam_strncpy(p_history_alarm_record->probable_cause_str,
                p_alarm_info->probable_cause_str,
                OAM_ALARM_PROB_CAUSE_STR_MAX_LEN); 
    oam_strncpy(p_history_alarm_record->specific_problem,
                p_alarm_info->specific_problem,
                OAM_SPECIFIC_PROBLEM_SIZE);  

    /* coverity 35653 */
    p_history_alarm_record->perceived_severity = (FM_alarm_severity_et)p_alarm_info->criticality;
    oam_strncpy(p_history_alarm_record->perceived_severity_str,
            oam_get_alarm_severity_type_by_severity_id(p_alarm_info->criticality),
            OAM_ALARM_PERCEIVED_SEV_MAX_LEN);

    if(!oam_strlen(p_alarm_info->add_txt)) {
        p_alarm_info->add_txt[0] = '-';
    }
    oam_strncpy(p_history_alarm_record->additional_information,
                p_alarm_info->add_info,
                OAM_ADDITIONAL_INFORMATION_SIZE);
    oam_strncpy(p_history_alarm_record->additional_text,
                p_alarm_info->add_txt,
                OAM_ADDITIONAL_TEXT_SIZE);

    p_history_alarm_record->alarm_list_type = p_alarm_info->reporting_mechanism;

    oam_strncpy(p_history_alarm_record->managed_object_instance,
               p_alarm_info->managed_object_instance,
               OAM_MANAGED_OBJECT_INSTANCE_SIZE);

    oam_strncpy(p_history_alarm_record->reporting_mechanism_str,
            oam_table_alarm_report_mechanism[p_alarm_info->reporting_mechanism].report_type,
            OAM_ALARM_REPORTING_MECH_MAX_LEN);
    /* there shall not be any return before this statement */

        /* Spr 12880 Fix Start */
    p_history_alarm_record->CellId =  p_alarm_info->CellId;
        /* Spr 12880 Fix End */

/* SPR-8133,34,35 FIX START */
    if(++oam_prov_req.fm_cntxt.Current_HistoryEventNumberOfEntries
            > OAM_HISTORY_ALARM_MAX) {
        oam_prov_req.fm_cntxt.Current_HistoryEventNumberOfEntries =
                                                    OAM_HISTORY_ALARM_MAX;
    }
/* SPR-8133,34,35 FIX END */

    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name : oam_handle_reset_critical_alarm
 * Inputs        : p_alarm_info :Pointer to alarm_info structure 
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function handles alarms.
 *****************************************************************************/
oam_return_et
oam_handle_reset_critical_alarm
(
    oam_alarm_info_t *p_alarm_info
)
{
    /* coverity 41279 */
     oam_error_code_et err_code=NO_ERROR;
    oam_return_et send_return_status;
    //UInt8   admin_cmd_restart_flag = OAM_ZERO;
    UInt16 transaction_id = OAM_ZERO;
    oam_s1ap_close_mme_conn_t s1ap_close_mme_conn={OAM_ZERO};

/* Coverity 61347 */
    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
    cell_config_idx = oam_get_rrm_cell_config_idx(
                            oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cell_config_idx=%d]", cell_config_idx);
        return OAM_FAILURE;
    }
    
    oam_rrm_cell_config_req_t  *p_rrm_cell_config = &oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req[cell_config_idx];

    oam_return_et retVal = OAM_FAILURE;

    OAM_LOG(OAM, OAM_INFO, "alarm_id = %d",p_alarm_info->alarm_id);
     
    if((p_alarm_info->alarm_id == MEM_ALLOC_FAILED_ALARM_ID) || (p_alarm_info->alarm_id == OAM_PDCP_SECURITY_ENGINE_ERROR_ALARM_ID))
    {

        send_return_status = oam_rrm_send_rrm_oam_cell_stop_req(
                ((rrm_oam_cell_stop_req_t *)
                 &(p_rrm_cell_config->global_cell_info.eutran_global_cell_id)),
                OAM_MODULE_ID, RRM_MODULE_ID,
                transaction_id,
                oam_prov_req.cell_cntxt.curr_cell_cid);

        if (OAM_FAILURE == send_return_status)
        {
            OAM_LOG(OAM,OAM_ERROR,
                    "Error in function oam_send_cell_stop_req:"
                    " error code :%d",
                    err_code);

            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
        }
        /* Coverity FIX 33637 */
        retVal = oam_s1ap_il_send_s1ap_oam_close_mme_conn(
                            &s1ap_close_mme_conn,
                            OAM_MODULE_ID,
                            RRC_MODULE_ID,
                            transaction_id,
                            oam_prov_req.cell_cntxt.curr_cell_cid);
        if (OAM_FAILURE == retVal)
        {
            OAM_LOG(OAM,OAM_ERROR,"Sending S1AP_OAM_CLOSE_MME_CONN message failed, error code :%d",err_code);
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
        }
    }

    oam_inform_value_change_event(OAM_ZERO,OAM_MODULE_ID,TR069_MODULE_ID);

    return OAM_SUCCESS; 
}



/****************************************************************************
 * Function Name : oam_handle_alarm
 * Inputs        : p_alarm_info :Pointer to alarm_info structure 
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function handles alarms.
 *****************************************************************************/

oam_return_et 
oam_handle_alarm (
    oam_alarm_info_t *p_alarm_info
    )
{

    OAM_FUN_ENTRY_TRACE ();
    SInt32 curr_alarm_idx = OAM_ZERO;
    oam_return_et ret_status = OAM_SUCCESS;
    oam_error_code_et err_code;

    if (OAM_FAILURE == oam_valid_alarmid (p_alarm_info->alarm_id))
    {
        OAM_LOG (OAM, OAM_WARNING, " Invalid Alarm Id[%d] ", p_alarm_info->alarm_id);
        return OAM_FAILURE;	
    }

    if(p_alarm_info->criticality==CRITICAL)
    {
        if((p_alarm_info->alarm_id != RRC_SCTP_ASSOCIATION_FAILURE_ALARM_ID ) && (p_alarm_info->alarm_id != RRC_S1AP_CONNECTION_FAILURE_ALARM_ID)) 
        {
        oam_maintain_system_state(OAM_CHANGE_SYSTEM_STATE_FOR_CRITICAL_ALARM);
        oam_prov_req.system_status.op_state= ENODEB_OP_STATE_DISABLED;
        oam_prov_req.system_status.rftx_status = OAM_FALSE;
        /* Call handler to disable the radio transmission */
        ret_status = oam_radio_transmission_carrier_enable_disable(oam_prov_req.system_status.rftx_status);
        /* Coverity FIX 33635 */
        if(OAM_FAILURE == ret_status)
        {
            return OAM_FAILURE;
        }
        oam_handle_reset_critical_alarm(p_alarm_info);
        }
    }
    if(p_alarm_info->criticality==MAJOR)
    {
        oam_maintain_system_state(OAM_CHANGE_SYSTEM_STATE_FOR_MAJOR_ALARM);
    }

    /* COVERITY: 63959 FIX START */
    if ((p_alarm_info->notification_type == NOTIFY_NEW_ALARM) ||
        (p_alarm_info->notification_type == NEW_NOTIFICATION)) {
    /* COVERITY: 63959 FIX END */
/* SPR 14630 Fix Start */
       curr_alarm_idx = oam_fm_fetch_curr_alarm_idx (p_alarm_info->alarm_id,p_alarm_info->CellId);
/* SPR 14630 Fix End */
       if(curr_alarm_idx != OAM_FM_FAILURE)
       {
         p_alarm_info->notification_type = NOTIFY_CHANGED_ALARM;
       }
       else
       {
         OAM_LOG (OAM, OAM_INFO, " Alarm ID[%d] not present in current alarm list. ", p_alarm_info->alarm_id);
         curr_alarm_idx = OAM_ZERO;
       }
    }

    if ((p_alarm_info->notification_type == NOTIFY_NEW_ALARM) ||
        ((p_alarm_info->notification_type == NEW_NOTIFICATION))
            || (p_alarm_info->notification_type == NOTIFY_CHANGED_ALARM))
    {
        /* SPR 17777 fix */
        oam_fm_fsm_process_event (p_alarm_info,&oam_prov_req.fm_cntxt,	/*Pointer to FM FSM context */
                OAM_FM_EVENT_NEW_ALARM_RAISED, &err_code);	/*Pointer to error code */
    }
    else if (p_alarm_info->notification_type == NOTIFY_CLEARED_ALARM)
    {

        /** This part for alarm cleared **/
/* SPR 14630 Fix Start */
        curr_alarm_idx = oam_fm_fetch_curr_alarm_idx (p_alarm_info->alarm_id,p_alarm_info->CellId);
/* SPR 14630 Fix End */
        /* Coverity FIX 33636 */
        if(OAM_FM_FAILURE == curr_alarm_idx)
        {
            OAM_LOG (OAM, OAM_WARNING, " Value of curr_alarm_idx is -1.[%d] ", p_alarm_info->alarm_id);
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE; 
        }
        /* SPR-12596 START*/
        oam_prov_req.fm_cntxt.current_alarm_list[curr_alarm_idx].alarm_state = OAM_FM_STATE_ACK_UNCLEAR;
        /* SPR-12596 END*/
        /* SPR 17777 fix */
        oam_fm_fsm_process_event (p_alarm_info,	/*Pointer to input api buffer */
                &oam_prov_req.fm_cntxt,	/*Pointer to FM FSM context */
                OAM_FM_EVENT_ALARM_CLEARED, &err_code);	/*Pointer to error code */

    }

    OAM_FUN_EXIT_TRACE ();
    return ret_status;
}

/*****************************************************************************
 * Function Name : oam_fm_handle_alarm_acknowledged_resp
 * Inputs        : tcb_detail :TCB details
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function handles response of alarm acknowledge.
 ******************************************************************************/
oam_return_et
oam_fm_handle_alarm_acknowledge_resp (
    UInt32 tcb_detail
    )
{
    OAM_FUN_ENTRY_TRACE ();
    oam_alarm_info_t *p_alarm_info={OAM_ZERO};
    oam_error_code_et p_err = NO_ERROR;/* Coverity FIX 35641 */
    UInt16 src_id=OAM_ZERO;
    UInt16 alarm_id=OAM_ZERO,criticality=OAM_ZERO;
    /* SPR 17777 fix code removed */
    Char8 alarm_raised_time[OAM_TIME_SIZE] = { OAM_ZERO };
    Char8 module_name[OAM_MODULE_NAME_SIZE]={OAM_ZERO};
    /* coverity 35640 */
    oam_error_code_et err_code = NO_ERROR;
    int i=OAM_ZERO;
    char *name = OAM_NULL;
    oam_struct_t *node=OAM_NULL;
    UInt8 *p_temp_buf = OAM_NULL;
    UInt8 event_code[OAM_MAX_EVENT_INFO_LEN];
    UInt8 num_parent_nodes = OAM_ZERO;
    UInt8 parent_node_count = OAM_ZERO;
    UInt16 event_info_len = OAM_ZERO;
    UInt8 param_num = OAM_ZERO;
    UInt16 parent_node_name_len = OAM_ZERO;
    UInt8 parent_node_name[OAM_MAX_PARENT_NODE_NAME_LEN] = {OAM_ZERO};
    UInt8 parameter_count = OAM_ZERO;
    UInt16 parent_info_len = OAM_ZERO;
    UInt16 param_len = OAM_ZERO;	
    UInt8 name_val_pair[MAX_PARAMETER_NAME_LENGTH] = {OAM_ZERO};
    char *value = OAM_NULL;
    UInt16 cur_pos=OAM_ZERO;

    if(OAM_FAILURE==oam_mem_alloc(sizeof(oam_alarm_info_t),(void *)&p_alarm_info,&p_err))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_alarm_info failed"
                " with Error Code:%d",
                p_err);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }
    /* Coverity ID :72388  Fix Start*/
    oam_memset(p_alarm_info, OAM_ZERO, sizeof(oam_alarm_info_t));
    /* Coverity ID : 72388 Fix End*/
    if (OAM_FAILURE == oam_create_timestamp_with_timezone (alarm_raised_time))
    {
        OAM_LOG (OAM, OAM_WARNING, " oam_create_timestamp_with_timezone function failed ");
         /* Coverity ID :72296  Fix Start*/
         oam_mem_free(p_alarm_info, &p_err);
         /* Coverity ID : 72296 Fix End*/
        return OAM_FAILURE;
    }
    node = oam_tcb_get_detail (&tcb_detail);
    if(OAM_NULL==node)
    {

        OAM_LOG (OAM, OAM_WARNING, "Error in fetching TCB details.");
         /* Coverity ID :72296  Fix Start*/
         oam_mem_free(p_alarm_info, &p_err);
         /* Coverity ID : 72296 Fix End*/
        return OAM_FAILURE;
    }

    cur_pos +=OAM_CSPL_HDR_SIZE;   
    p_temp_buf  = node->oam_tcb_context;
    src_id = get_word_from_header ((UInt8 *) (p_temp_buf) + OAM_CSPL_HDR_SIZE+ OAM_TWO);  //for fetching source id.
    cur_pos += OAM_MGMT_INTF_HEADER_SIZE;


    cur_pos += OAM_TWO;

    /* Reserved Bytes */
    cur_pos += OAM_TWO;

    /* Reserved Bytes */
    cur_pos += OAM_FOUR;

    /* Extract Event Info Length */
    event_info_len = p_temp_buf[cur_pos++];

    /* Reserved Bytes */
    cur_pos += OAM_THREE;

    /* Extract Event Code */
    oam_strncpy((char *)event_code,(const char *)&p_temp_buf[cur_pos],event_info_len);//-17);//16 for additional event infor and 1 for null
    event_code[event_info_len]='\0';

    //p_temp_buf += event_info_len;
    cur_pos += event_info_len;//-16;

    /* Reserved Bytes */
    cur_pos += OAM_FOUR;

    /* Extract total length of parameter list information */
    cur_pos += OAM_TWO;

    /* Extract Total Number of parents */
    num_parent_nodes = p_temp_buf[cur_pos++];

    /* Reserved Bytes */
    cur_pos += OAM_ONE;

    /* Reserved Bytes */
    cur_pos += OAM_FOUR;

    /* Extract Parent Node Information */
    for(parent_node_count = OAM_ZERO; parent_node_count < num_parent_nodes ; parent_node_count++)
    {
        /* Extract total length of parent information */
        cur_pos += OAM_TWO;

        param_num = p_temp_buf[cur_pos++];


        /* Reserved Bytes */

        cur_pos += OAM_ONE;

        /* Reserved Bytes */

        cur_pos += OAM_TWO;

        /* Extract Length of Parent Node Name */

        parent_node_name_len = get_word_from_header((UInt8 *)(p_temp_buf+cur_pos));

        cur_pos += OAM_TWO;

        /* Extract Parent Node Name */
        if(parent_node_name_len < OAM_MAX_PARENT_NODE_NAME_LEN)
        {
        oam_strncpy((char *)parent_node_name,(const char *)p_temp_buf+cur_pos,parent_node_name_len);
        parent_node_name[parent_node_name_len]='\0';
        }
        cur_pos += parent_node_name_len;

        for(parameter_count = OAM_ZERO; parameter_count < param_num; parameter_count++)
        {
            parent_info_len = get_word_from_header((UInt8 *)(p_temp_buf+cur_pos));
            cur_pos += OAM_TWO;
            /* Reserved Bytes */
            cur_pos += OAM_TWO;

            /* Extract Name Value Pair */
            param_len = parent_info_len-OAM_FOUR;
            if(param_len < STR_LEN_128)
            {
            oam_strncpy((char *)name_val_pair,(const char *)p_temp_buf+cur_pos,param_len);
            name_val_pair[param_len]=OAM_ZERO;
            }
            name=strtok((char *)name_val_pair,"=");

            /*+-coverity_86034_fix*/ 

            /* Coverity CID 72296 Fix Start */
            if(NULL == name)
            {
                OAM_LOG(OAM,OAM_WARNING,"OAM NULL Check failure !!!");
                oam_mem_free(p_alarm_info, &p_err);
                return OAM_FAILURE;
            }
            /* Coverity CID 72296 Fix End */
            value=strtok(OAM_NULL,"=");
            /* Coverity CID 72296 Fix Start */
            if(NULL == value)
            {
                OAM_LOG(OAM,OAM_WARNING,"OAM NULL Check failure !!!");
                oam_mem_free(p_alarm_info, &p_err);
                return OAM_FAILURE;
            }
            OAM_NULL_CHECK(NULL != value);/* Coverity FIX 35418 */
            if(strcmp(name,"AlarmIdentifier") == OAM_ZERO)
            {
                alarm_id=oam_atoi(value);
            }
            if(strcmp(name,"PerceivedSeverity") == OAM_ZERO)
            {
                criticality=oam_atoi(value);
                break;
            }
            oam_fprintf(stdout,"%-25s\t=\t%-20s\n",name,value);
            cur_pos += param_len;
        }
    }

    for (i = OAM_ZERO; i < oam_prov_req.fm_cntxt.SupportedAlarmNumberOfEntries;i++)
    {
        if (alarm_id ==oam_prov_req.fm_cntxt.supported_alarm_list[i].alarm_identifier)
        {
            p_alarm_info->module_id =src_id;
            oam_memset (p_alarm_info->module_name, OAM_ZERO, OAM_MODULE_NAME_SIZE);
            if(OAM_FAILURE ==
                    oam_find_module_name (p_alarm_info->module_id, module_name))
            {
                oam_strcpy ((char *) module_name,"No Fetch");
            }
            oam_strcpy((char *)p_alarm_info->module_name,module_name);

            p_alarm_info->alarm_id = alarm_id;
            p_alarm_info->notification_type = NOTIFY_CHANGED_ALARM;
            p_alarm_info->event_type =
                oam_prov_req.fm_cntxt.supported_alarm_list[i].event_type;
            p_alarm_info->probable_cause =
                oam_prov_req.fm_cntxt.supported_alarm_list[i].probable_cause;
            p_alarm_info->criticality = criticality;
            p_alarm_info->reporting_mechanism =
                oam_prov_req.fm_cntxt.supported_alarm_list[i].reporting_mechanism;
            oam_strcpy (p_alarm_info->time_stamp,alarm_raised_time);
            oam_strcpy (p_alarm_info->add_txt, (char *) "Alarm raised");
            oam_strcpy ((char *)p_alarm_info->managed_object_instance,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].managed_object_instance);
            oam_strcat ((char *)p_alarm_info->managed_object_instance,(char*) get_module_id_for_alarm(p_alarm_info->module_id)); 
            break;

        }
    }
    /* SPR 17777 fix */
    oam_fm_fsm_process_event (p_alarm_info,	/*Pointer to input api buffer */
            &(oam_prov_req.fm_cntxt),	/*Pointer to FM FSM context */
            OAM_FM_EVENT_ALARM_ACKNOWLEDGED, &err_code);	/*Pointer to error code */


    OAM_FUN_EXIT_TRACE ();
    /* Coverity ID 72296 Fix Start*/
    oam_mem_free(p_alarm_info, &p_err);
    /* Coverity ID 72296 Fix End*/
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_process_alarm_handler
 * Inputs        : p_api_buf :Pointer to api buffer 
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function handles alarm notification from multiple modules
 ****************************************************************************/

oam_return_et
oam_fm_intf_process_msg (
    void *p_api_buf
    )
{
    UInt16 src_id = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE ();

    src_id = get_word_from_header ((UInt8 *) (p_api_buf) + OAM_INTF_HDR_SRC_ID_OFFSET);

    switch (src_id)
    {
        case RRC_MODULE_ID:
        case PDCP_MODULE_ID:
        case RLC_MODULE_ID:
        case MAC_MODULE_ID:
        /*SPR FIX 12977 */
        case EGTPU_MODULE_ID:
        /*SPR FIX 12977 */
 
        /*SPR FIX 10190 */ 
        case SON_MODULE_ID:
            oam_handle_lower_layer_alarm (p_api_buf);
            break;
        case RRM_MODULE_ID:
            oam_handle_rrm_alarm(p_api_buf);
            break;
        default:
            OAM_LOG (OAM, OAM_WARNING, "Unrecognized src module id[%u]", 
                src_id);
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_fm_clear_history
 * Inputs        : none
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function used for clear history.
 ****************************************************************************/

/* + SPR 17439 */
oam_return_et
oam_fm_clear_history(void)
/* - SPR 17439 */
{
    OAM_FUN_ENTRY_TRACE ();
    oam_memset (&oam_prov_req.fm_cntxt.history_alarm_list, OAM_ZERO,
            (sizeof (oam_history_alarm_info_t) * OAM_HISTORY_ALARM_MAX));
    oam_prov_req.fm_cntxt.History_List_Index=OAM_ZERO;
    oam_prov_req.fm_cntxt.isHistoryRolled = OAM_ZERO;
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name : oam_fm_clear_current_alarm_list
 * Inputs        : none
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function used for clear history.
 ****************************************************************************/

    oam_return_et
oam_fm_clear_current_alarm_list(void)
{
    OAM_FUN_ENTRY_TRACE ();
    oam_memset (&oam_prov_req.fm_cntxt.current_alarm_list, OAM_ZERO,
            (sizeof (oam_current_alarm_info_t) * OAM_CURRENT_ALARM_MAX));
    oam_prov_req.fm_cntxt.Current_List_Index=OAM_ZERO;
    oam_prov_req.fm_cntxt.CurrentAlarmNumberOfEntries=OAM_ZERO;
    oam_prov_req.fm_cntxt.isCurrentRolled = OAM_ZERO;
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name : oam_fm_set_reporting_mechanism
 * Inputs        : param_name :Parameter name
                   param_value :Parameter value
 * Outputs       : none
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 * Description   : This function used for set reporting mechanism.
 ****************************************************************************/

oam_return_et
oam_fm_set_reporting_mechanism (
    Char8 *param_name, 
    Char8 *param_value
    )
{
    Char8 ch_str[4] = { OAM_ZERO };
    UInt8 count = OAM_ZERO;
    UInt32 index = OAM_ZERO;
    if(snscanf (param_name,
                oam_strlen("InternetGatewayDevice.FaultMgmt.SupportedAlarm.{%4s}.ReportingMechanism")+4,
                "InternetGatewayDevice.FaultMgmt.SupportedAlarm.{%4s}.ReportingMechanism",
                ch_str) == 0)
    {
        OAM_LOG(OAM,OAM_ERROR,"function snscanf returned FAILURE"); /*COVERITY FIX 25may*/
        return OAM_FAILURE;
    }
    index = oam_atoi (ch_str);
    for (count = OAM_ZERO; count <= DISABLED_ALARM; count++)
    {
        if(oam_strncmp(param_value, oam_table_alarm_report_mechanism[count].report_type, 
                    oam_strlen(oam_table_alarm_report_mechanism[count].report_type)) == OAM_ZERO)
        {
            break;
        }
    }

    if(count <= DISABLED_ALARM)
    {
        /* coverity 35633 */
        oam_prov_req.fm_cntxt.supported_alarm_list[index].reporting_mechanism =
            (alarm_report_mechanism_et)count;
    }
    else
    {
        OAM_LOG(OAM,OAM_WARNING,"The enum range not supported[count = %d]", count); 
        return OAM_FAILURE;
    }
    return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name: oam_get_alarm_event_by_event_id
 * Description  : this function get alarm event id and return alarm event string.
 * Inputs       : event_id :event id
 * Outputs      : NONE
 * Returns      : alarm event string. 
 ********************************************************************************/
Char8 *
oam_get_alarm_event_by_event_id (
    UInt16 event_id
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_table_alarm_event_type); loop++)
    {
        if (oam_table_alarm_event_type[loop].event_id == event_id)
        {
            return oam_table_alarm_event_type[loop].event_type;
        }
    }
    return " ";
}

/*******************************************************************************
 * Function Name: oam_get_alarm_probable_cause_by_cause_id
 * Description  : this function get alarm probable cause id and return alarm probable cause string.
 * Inputs       : cause_id :cause id
 * Outputs      : NONE
 * Returns      : probable cause string.
 ********************************************************************************/

Char8 *
oam_get_alarm_probable_cause_by_cause_id (
    UInt16 cause_id
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_table_alarm_probable_cause);
            loop++)
    {
        if (oam_table_alarm_probable_cause[loop].probable_cause_id == cause_id)
        {
            return oam_table_alarm_probable_cause[loop].probable_cause;
        }
    }
    return " ";
}


/*******************************************************************************
 * Function Name: oam_get_alarm_notification_type_by_notification_id
 * Description  : this function get alarm notification type id and return alarm notification type string.
 * Inputs       : notification_id :notification id
 * Outputs      : NONE
 * Returns      : notification type string. 
 ********************************************************************************/

Char8 *
oam_get_alarm_notification_type_by_notification_id (
    UInt16 notification_id
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_table_alarm_notification); loop++)
    {
        if (oam_table_alarm_notification[loop].notification_id ==
                notification_id)
        {
            return oam_table_alarm_notification[loop].notification_type;
        }
    }
    return " ";
}

/*******************************************************************************
 * Function Name: oam_get_alarm_severity_type_by_severity_id
 * Description  : this function get alarm severity id and return alarm severity string.
 * Inputs       : severity_id :severity id
 * Outputs      : NONE
 * Returns      : severity type string. 
 ********************************************************************************/

Char8 *
oam_get_alarm_severity_type_by_severity_id (
    UInt16 severity_id
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_table_alarm_severity); loop++)
    {
        if (oam_table_alarm_severity[loop].severity_id == severity_id)
        {
            return oam_table_alarm_severity[loop].perceived_severity;
        }
    }
    return " ";
}

/*******************************************************************************
 * Function Name: oam_internal_alarm
 * Description  : this function handle oam internal alarm.
 * Inputs       : alarm_id :Alarm ID
 *                criticality :Criticality
 *                notification_type :notification type
 *                evevnt_type :Event type
 *                probable_cause :Probable cause
 *                reporting_mechanism :Reporting mechanism 
 *                add_txt : additional text
                  add_info: additional information
 * Outputs      : NONE
 * Returns      : OAM_SUCCESS/OAM_FAILURE 
 ********************************************************************************/

oam_return_et 
oam_internal_alarm(
    UInt16 alarm_id,
    /* SPR 17777 fix code removed */
    UInt16 notification_type,
    UInt16 event_type,
    UInt16 probable_cause, 
    UInt16 reporting_mechanism, 
    Char8 *add_txt,
    Char8 *add_info,
        /* Spr 12880 Fix Start */
    UInt8 CellId
        /* Spr 12880 Fix End */
   
    )
{
    UInt8 i = OAM_ZERO;
    Char8  alarm_raised_time[OAM_TIME_SIZE] = { OAM_ZERO };
    oam_alarm_info_t *p_alarm_info={OAM_ZERO};
    oam_error_code_et p_err = NO_ERROR; /*Coverity FIX 35642 */ 
    if(OAM_FAILURE==oam_mem_alloc(sizeof(oam_alarm_info_t),(void *)&p_alarm_info,&p_err)) 
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_alarm_info failed"
                " with Error Code:%d",
                p_err);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }

    /* SPR_19838 :Coverity_11493 Fix Start */
    oam_memset(p_alarm_info,OAM_ZERO,sizeof(oam_alarm_info_t));
    /* SPR_19838 :Coverity_11493 Fix Stop */
    OAM_FUN_ENTRY_TRACE ();

    if (OAM_FAILURE == oam_create_timestamp_with_timezone (alarm_raised_time))
    {
        OAM_LOG (OAM, OAM_WARNING, " oam_create_timestamp_with_timezone function failed ");
        /* Coverity ID :72295  Fix Start*/
        oam_mem_free(p_alarm_info, &p_err);
        /* Coverity ID : 72295 Fix End*/
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }
    for (i = OAM_ZERO; i < oam_prov_req.fm_cntxt.SupportedAlarmNumberOfEntries;i++)
    {
        if (alarm_id ==oam_prov_req.fm_cntxt.supported_alarm_list[i].alarm_identifier)
        {
    p_alarm_info->module_id = OAM_MODULE_ID;
    oam_strcpy (p_alarm_info->module_name, "OAM");
    p_alarm_info->alarm_id = alarm_id;
    p_alarm_info->criticality = oam_prov_req.fm_cntxt.supported_alarm_list[i].perceived_severity;
    p_alarm_info->notification_type = notification_type;
    p_alarm_info->event_type = event_type;
    p_alarm_info->probable_cause = probable_cause;
    oam_strncpy(p_alarm_info->probable_cause_str,
                oam_get_alarm_probable_cause_by_cause_id(probable_cause),
                OAM_ALARM_PROB_CAUSE_STR_MAX_LEN);
    p_alarm_info->reporting_mechanism = reporting_mechanism;
    oam_strncpy(p_alarm_info->specific_problem,
                SUPP_ALARM_LIST(i).specific_problem,
                OAM_SPECIFIC_PROBLEM_SIZE);
    oam_strncpy(p_alarm_info->time_stamp, alarm_raised_time,
                OAM_ALARM_TIMESTAMP_STR_MAX_LEN);

    oam_strncpy(p_alarm_info->add_txt, add_txt ? add_txt : "-",
                OAM_ALARM_ADD_TXT_MAX_LEN);

    oam_strncpy(p_alarm_info->add_info, add_info ? add_info : "-",
                OAM_ALARM_ADD_INFO_MAX_LEN);

    oam_strncpy(p_alarm_info->managed_object_instance,
                SUPP_ALARM_LIST(i).managed_object_instance,
                OAM_MANAGED_OBJECT_INSTANCE_SIZE);

    oam_strcat(p_alarm_info->managed_object_instance,
               get_module_id_for_alarm(p_alarm_info->module_id));
        }
    }
        /* Spr 12880 Fix Start */
    p_alarm_info->CellId = CellId;
        /* Spr 12880 Fix End */
    if (OAM_FAILURE == oam_handle_alarm (p_alarm_info))
    {
        /* Coverity CID 72295 Fix Start */
        oam_mem_free(p_alarm_info, &p_err);
        /* Coverity CID 72295 Fix End */
        OAM_LOG (OAM, OAM_WARNING, " Unable to handle internal alarm");
        OAM_FUN_EXIT_TRACE ();
        /*+- coverity_87805_fix*/
        return OAM_FAILURE;
    }
    OAM_FUN_EXIT_TRACE ();
    /* Coverity ID :72295  Fix Start*/
    oam_mem_free(p_alarm_info, &p_err);
    /* Coverity ID : 72295 Fix End*/
    return OAM_SUCCESS;
}


/******************************************************************************
 * Function Name  : oam_fm_populate_n_send_data_to_management_interface
 * Inputs         : p_alarm_info :Pointer to alarm_info structure 
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function populates and send data to management interface
 *******************************************************************************/

oam_return_et
oam_fm_populate_n_send_data_to_mangement_interface
(
    oam_alarm_info_t *p_alarm_info
    )
{
    SInt32 alarm_idx = OAM_ZERO;  /*COVERITY FIX 25may*/
    /* coverity 35661 */
    Char8 parent_name_local[MAX_PARENT_NAME_LEN] = {OAM_ZERO};
    Char8 temp_str[OAM_PARAM_MAX_LEN] = {OAM_ZERO};
    Char8 temp_param[OAM_PARAM_MAX_LEN] = {OAM_ZERO};
    oam_inform_api_t *p_oam_inform_api = OAM_NULL;
    /* coverity 35641 */
    oam_error_code_et err = NO_ERROR;
    oam_return_et retVal = OAM_FAILURE;
    UInt16 rem_len = OAM_ZERO;
    UInt8 param_cnt = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE ();

    /* BUG ID-- 909 If same alarm Id raised again then alarm should not sent user interface*/
   
    if(p_alarm_info->notification_type == NOTIFY_CHANGED_ALARM) {
        OAM_LOG(OAM, OAM_INFO, "Same alarm received again, "
                "alarm not to be reported to management interface");
        OAM_FUN_EXIT_TRACE();
        return retVal;
    }

    retVal = oam_mem_alloc(sizeof(oam_inform_api_t),
                           (void**)&p_oam_inform_api, &err);
    if(OAM_FAILURE == retVal) {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_oam_inform_api failed"
                " with Error Code:%d",
                err);
        OAM_FUN_EXIT_TRACE();
        return retVal;
    }

    if(EXPEDITED_ALARM == p_alarm_info->reporting_mechanism) {
        alarm_idx = oam_fm_fetch_expedited_alarm_idx(p_alarm_info->alarm_id);
        if(OAM_FM_FAILURE == alarm_idx) {
            oam_mem_free(p_oam_inform_api, &err);
            OAM_LOG (OAM, OAM_INFO, "No alarm present in the list..");
            retVal = OAM_FAILURE;
            OAM_FUN_EXIT_TRACE();
            return retVal;
        }

        oam_sprintf(parent_name_local,
                "InternetGatewayDevice.FaultMgmt.ExpeditedEvent.[%d]",
                alarm_idx);
        oam_strcpy(p_oam_inform_api->parent_name, parent_name_local);
        p_oam_inform_api->num_of_param = OAM_FM_MAX_ALARM_ATTRIBUTES;

        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                     EXPD_ALARM_LIST(alarm_idx).event_time);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "EventTime=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                   temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);

        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_memset(temp_str, OAM_ZERO, sizeof(temp_str));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%d",
                    p_alarm_info->alarm_id);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "AlarmIdentifier=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                   temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset (temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                oam_get_alarm_notification_type_by_notification_id(p_alarm_info->notification_type));
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "NotificationType=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);

        oam_memset (temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                     p_alarm_info->managed_object_instance);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "ManagedObjectInstance=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                oam_get_alarm_event_by_event_id (p_alarm_info->event_type));
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "EventType=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        if(oam_strlen(p_alarm_info->probable_cause_str)) {
            oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                         p_alarm_info->probable_cause_str);
        }
        else {
            oam_strcpy(temp_str, "--"); /*Coverity FIX 40889 */
        }

        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "ProbableCause=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        if(oam_strlen(p_alarm_info->specific_problem)) {
            oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                         EXPD_ALARM_LIST(alarm_idx).specific_problem);
        }
        else {
            oam_strcpy(temp_str, "--"); /*Coverity FIX 40889 */
        }

        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "SpecificProblem=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                oam_get_alarm_severity_type_by_severity_id(p_alarm_info->criticality));
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "PerceivedSeverity=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);

        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        if(oam_strlen(p_alarm_info->add_txt)) {
            oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                            p_alarm_info->add_txt);
        }
        else {
            oam_strcpy(temp_str, "--");
        }

        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "AdditionalText=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);



        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        if(oam_strlen(EXPD_ALARM_LIST(alarm_idx).additional_information)) {
            oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                EXPD_ALARM_LIST(alarm_idx).additional_information);
        }
        else {
            oam_strcpy(temp_str, "--");
        }

        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "AdditionalInformation=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);
    }
    else if(QUEUED_ALARM == p_alarm_info->reporting_mechanism) {
        alarm_idx = oam_fm_fetch_queued_alarm_idx(p_alarm_info->alarm_id);
        if(OAM_FM_FAILURE == alarm_idx) {
            oam_mem_free(p_oam_inform_api, &err);
            OAM_LOG (OAM, OAM_INFO, "No alarm present in the list..");
            retVal = OAM_FAILURE;
            OAM_FUN_ENTRY_TRACE ();
            return retVal;
        }
        oam_sprintf (parent_name_local,
                "InternetGatewayDevice.FaultMgmt.QueuedEvent.[%d]",
                alarm_idx);
        oam_strcpy(p_oam_inform_api->parent_name, parent_name_local);
        p_oam_inform_api->num_of_param = OAM_FM_MAX_ALARM_ATTRIBUTES;

        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                QUEUED_ALARM_LIST(alarm_idx).event_time);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "EventTime=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);

        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%d",
                     p_alarm_info->alarm_id);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "AlarmIdentifier=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                oam_get_alarm_notification_type_by_notification_id
                (p_alarm_info->notification_type));
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "NotificationType=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                     p_alarm_info->managed_object_instance);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "ManagedObjectInstance=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);



        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                oam_get_alarm_event_by_event_id (p_alarm_info->event_type));
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "EventType=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                     p_alarm_info->probable_cause_str); 
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "ProbableCause=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);

        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                QUEUED_ALARM_LIST(alarm_idx).specific_problem);
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "SpecificProblem=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);


        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));
        oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                oam_get_alarm_severity_type_by_severity_id(p_alarm_info->criticality));
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "PerceivedSeverity=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);

        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));

        if(oam_strlen(p_alarm_info->add_txt)) {
            oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                     p_alarm_info->add_txt);
        }
        else {
            oam_strcpy(temp_str, "--");
        }

        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "AdditionalText=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strcpy ((Char8 *) temp_param, "AdditionalText=");
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);



        oam_memset(temp_str, OAM_ZERO, sizeof (temp_str));
        oam_memset(temp_param, OAM_ZERO, sizeof(temp_param));

        if(oam_strlen(p_alarm_info->add_info)) {
            oam_snprintf(temp_str, OAM_PARAM_MAX_LEN, "%s",
                       QUEUED_ALARM_LIST(alarm_idx).additional_information);
        }
        else {
            oam_strcpy(temp_str, "--");
        }
        rem_len = OAM_PARAM_MAX_LEN;
        oam_strncpy(temp_param, "AdditionalInformation=", rem_len);
        rem_len -= oam_strlen(temp_param);
        oam_strncat(temp_param, temp_str, rem_len);
        oam_strncpy(p_oam_inform_api->param_list[param_cnt++],
                    temp_param, OAM_INFORM_API_PARAM_NAME_MAX_LEN);
    }

    if((EXPEDITED_ALARM == p_alarm_info->reporting_mechanism) ||
            (QUEUED_ALARM == p_alarm_info->reporting_mechanism)) {
        retVal = oam_compose_and_send_inform_api(&p_oam_inform_api,
                                                 OAM_ONE,
                                                 (UInt8*)"Value Change",
                                                 OAM_QCLI_TRAP_MODULE_ID,
                                                 OAM_SUB_MODULE_FM);


        retVal = oam_compose_and_send_inform_api(&p_oam_inform_api,
                                                     OAM_ONE,
                                                     (UInt8*)"SNMP TRAP",
                                                     OAM_SNMP_INTF_MODULE_ID,
                                                     OAM_SUB_MODULE_FM);


        if(oam_prov_req.system_status.tr069_configured) {
            retVal = oam_compose_and_send_inform_api(&p_oam_inform_api,
                                                     OAM_ONE,
                                                     (UInt8*)"Value Change",
                                                     TR069_MODULE_ID,
                                                     OAM_SUB_MODULE_FM);           
        }
        else {
            OAM_LOG(OAM, OAM_INFO, "TR not configured yet!!");
        }
    }

    oam_mem_free(p_oam_inform_api, &err);

    OAM_FUN_EXIT_TRACE ();
    return retVal;
}

/*******************************************************************************
 ** Function Name: oam_rrm_get_alarm_event_id_string_by_event_id
 ** Description  : this function get alarm event id and return rrm event id string.
 ** Inputs       : event_id :event id
 ** Outputs      : NONE
 ** Returns      : event id string. 
 *********************************************************************************/

Char8 *
oam_rrm_get_alarm_event_id_string_by_event_id (
    UInt16 event_id
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_rrm_table_protocol_event_id); loop++)
    {
        if (oam_rrm_table_protocol_event_id[loop].event_id == event_id)
        {
            return oam_rrm_table_protocol_event_id[loop].event_id_str;
        }
    }
    return "Invalid";
}

/*******************************************************************************
 ** Function Name: oam_rrm_get_alarm_event_type_string_by_event_type
 ** Description  : this function get rrm event type and return rrm event type string.
 ** Inputs       : event_type :event type
 ** Outputs      : NONE
 ** Returns      : event type string. 
 *********************************************************************************/

Char8 *
oam_rrm_get_alarm_event_type_string_by_event_type (
    UInt16 event_type
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_rrm_table_event_type); loop++)
    {
        if (oam_rrm_table_event_type[loop].event_type == event_type)
        {
            return oam_rrm_table_event_type[loop].event_type_str;
        }
    }
    return "Invalid";
}

/*******************************************************************************
 ** Function Name: oam_rrm_get_alarm_event_sub_type_string_by_event_sub_type
 ** Description  : this function get rrm event sub type and return rrm event sub type string.
 ** Inputs       : event_sub_type :event sub type
 ** Outputs      : NONE
 ** Returns      : event sub type string. 
 *********************************************************************************/

Char8 *
oam_rrm_get_event_sub_type_string_by_event_sub_type (
    UInt16 event_sub_type
    )
{
    UInt16 loop = OAM_ZERO;
    for (loop = OAM_ZERO; loop < ARRSIZE (oam_rrm_table_event_sub_type); loop++)
    {
        if (oam_rrm_table_event_sub_type[loop].event_sub_type == event_sub_type)
        {
            return oam_rrm_table_event_sub_type[loop].event_sub_type_str;
        }
    }
    return "Invalid";
}

/******************************************************************************
 ** Function Name  : oam_rrm_get_alarm_id
 ** Inputs         : event_id_rrm :event id of rrm 
 ** Outputs        : alarm_id_rrm :event id of rrm
 ** Returns        : alarm_id_rrm :event id of rrm
 ** Description    : This function is to get alarm id of RRM.
 ********************************************************************************/

SInt16
oam_rrm_get_alarm_id(
    UInt16 event_id_rrm,
    Char8 *add_text,
    UInt8 *CellId

    )
{
    SInt16 alarm_id_rrm=OAM_ZERO;
    OAM_FUN_ENTRY_TRACE ();
    if(event_id_rrm ==OAM_ZERO)
    {
        alarm_id_rrm=MEM_ALLOC_FAILED_ALARM_ID;   // Alarm for memory alloc failed.
        /*eICIC_PHASE_1_2_CHANGES_START*/
        *CellId = OAM_INVALID_CELL_ID; 
        /*eICIC_PHASE_1_2_CHANGES_END*/
    }
    else if(event_id_rrm==OAM_ONE)
    {
        alarm_id_rrm=MEM_FREE_FAILED_ALARM_ID;  // Alarm for memory free failed.
        /*eICIC_PHASE_1_2_CHANGES_START*/
        *CellId = OAM_INVALID_CELL_ID; 
        /*eICIC_PHASE_1_2_CHANGES_END*/
    }

    else if(event_id_rrm==OAM_TWO)
    {
        alarm_id_rrm=SEND_MSG_FAILED_ALARM_ID;  // Alarm for transmit message failed.
        /*eICIC_PHASE_1_2_CHANGES_START*/
        *CellId = OAM_INVALID_CELL_ID; 
        /*eICIC_PHASE_1_2_CHANGES_END*/
    }
    else if(event_id_rrm==OAM_THREE)
    {
        alarm_id_rrm=RECV_MSG_FAILED_ALARM_ID; // Alarm for receive message failed.
        /*eICIC_PHASE_1_2_CHANGES_START*/
        *CellId = OAM_INVALID_CELL_ID; 
        /*eICIC_PHASE_1_2_CHANGES_END*/
    }
    else if(event_id_rrm==OAM_FOUR)
    {
        alarm_id_rrm=TIMER_START_FAILED_ALARM_ID;  // Alarm for timer start
        /*eICIC_PHASE_1_2_CHANGES_START*/
        *CellId = OAM_INVALID_CELL_ID; 
        /*eICIC_PHASE_1_2_CHANGES_END*/
    }
    else if(event_id_rrm==OAM_FIVE)
    {
        alarm_id_rrm=TIMER_STOP_FAILED_ALARM_ID;  // Alarm for timer stop
        /*eICIC_PHASE_1_2_CHANGES_START*/
        *CellId = OAM_INVALID_CELL_ID; 
        /*eICIC_PHASE_1_2_CHANGES_END*/
    }
    /*eICIC_PHASE_1_2_CHANGES_START*/
    /*SPR 14811 start*/
    else if(event_id_rrm == OAM_SEVEN)
    {
        alarm_id_rrm = ABS_PATTERN_NOT_FOUND_FOR_LOW_LOAD_ALARM_ID; //Alarm when no ABS Pattern is Found for LOWLOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_NOT_FOUND_FOR_LOWLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_EIGHT)
    {
        alarm_id_rrm = ABS_PATTERN_NOT_FOUND_FOR_MID_LOAD_ALARM_ID; //Alarm when no ABS Pattern is Found for MIDLOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_NOT_FOUND_FOR_MIDLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_NINE)
    {
        alarm_id_rrm = ABS_PATTERN_NOT_FOUND_FOR_HIGH_LOAD_ALARM_ID; //Alarm when no ABS Pattern is Found for HIGHLOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_NOT_FOUND_FOR_HIGHLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TEN)
    {
        alarm_id_rrm = ABS_PATTERN_NOT_FOUND_FOR_OVER_LOAD_ALARM_ID; //Alarm when no ABS Pattern is Found for OVERLOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_NOT_FOUND_FOR_OVERLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_ELEVEN)
    {
        alarm_id_rrm = ABS_PATTERN_FOUND_FOR_LOW_LOAD_ALARM_ID; //Alarm when ABS Pattern is Found for LOW LOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_FOUND_FOR_LOWLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWELVE)
    {
        alarm_id_rrm = ABS_PATTERN_FOUND_FOR_MID_LOAD_ALARM_ID; //Alarm when ABS Pattern is Found for MID LOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_FOUND_FOR_MIDLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_THIRTEEN)
    {
        alarm_id_rrm = ABS_PATTERN_FOUND_FOR_HIGH_LOAD_ALARM_ID; //Alarm when ABS Pattern is Found for HIGH LOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_FOUND_FOR_HIGHLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_FOURTEEN)
    {
        alarm_id_rrm = ABS_PATTERN_FOUND_FOR_OVER_LOAD_ALARM_ID; //Alarm when ABS Pattern is Found for OVER LOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "ABS_PATTERN_FOUND_FOR_OVERLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_FIFTEEN)
    {
        alarm_id_rrm = AGGR_ABS_NOT_APPLIED_FOR_LOW_LOAD_ALARM_ID; //Alarm when ABS Pattern is not applied for LOWLOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "AGGR_ABS_NOT_APPLIED_FOR_LOWLOAD for Cell Id [%d]",*CellId);
    }else if(event_id_rrm == OAM_SIXTEEN)
    {
        alarm_id_rrm = AGGR_ABS_APPLIED_FOR_LOW_LOAD_ALARM_ID;     //Alarm when ABS Pattern is applied for LOWLOAD 
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "AGGR_ABS_APPLIED_FOR_LOWLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_SEVENTEEN)
    {
        alarm_id_rrm = USABLE_ABS_NOT_APPLIED_ALARM_ID;     //Alarm when Usable ABS Pattern is not applied
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "USABLE_ABS_NOT_APPLIED for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_EIGHTEEN)
    {
        alarm_id_rrm = USABLE_ABS_APPLIED_ALARM_ID;     //Alarm when Usable ABS Pattern is applied
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "USABLE_ABS_APPLIED for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_NINTEEN)
    {
        alarm_id_rrm = VICTM_ABS_NOT_FOUND_ALARM_ID;  //Alarm when Victim ABS PATTERN is not found.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "VICTM_ABS_NOT_FOUND for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY)
    {
        alarm_id_rrm = VICTM_ABS_FOUND_ALARM_ID;  //Alarm when Victim ABS PATTERN is found.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "VICTM_ABS_FOUND for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_ONE)
    {
        alarm_id_rrm = VICTM_MEAS_SUBSET_NOT_FOUND_ALARM_ID;  //Alarm when Victim MEAS SUBSET is not found.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "VICTM_MEAS_SUBSET_NOT_FOUND for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_TWO)
    {
        alarm_id_rrm = VICTM_MEAS_SUBSET_FOUND_ALARM_ID;  //Alarm when Victim MEAS SUBSET is found.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "VICTM_MEAS_SUBSET_FOUND for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_THREE)
    {
        alarm_id_rrm = AGGR_ABS_NOT_APPLIED_FOR_MID_LOAD_ALARM_ID;  //Alarm when Aggressor ABS PATTERN is not applied for MIDLOAD.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "AGGR_ABS_NOT_APPLIED_FOR_MIDLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_FOUR)
    {
        alarm_id_rrm = AGGR_ABS_APPLIED_FOR_MID_LOAD_ALARM_ID;  //Alarm when Aggressor ABS PATTERN is applied for MIDLOAD.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "AGGR_ABS_APPLIED_FOR_MIDLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_FIVE)
    {
        alarm_id_rrm = AGGR_ABS_NOT_APPLIED_FOR_HIGH_LOAD_ALARM_ID;  //Alarm when Aggressor ABS PATTERN is not applied for HIGHLOAD
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "AGGR_ABS_NOT_APPLIED_FOR_HIGHLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_SIX)
    {
        alarm_id_rrm = AGGR_ABS_APPLIED_FOR_HIGH_LOAD_ALARM_ID;  //Alarm when Aggressor ABS PATTERN is applied for HIGHLOAD.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "AGGR_ABS_APPLIED_FOR_HIGHLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_SEVEN)
    {
        alarm_id_rrm = AGGR_ABS_NOT_APPLIED_FOR_OVER_LOAD_ALARM_ID;  //Alarm when Aggressor ABS PATTERN is not applied for OVERLOAD.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "AGGR_ABS_NOT_APPLIED_FOR_OVERLOAD for Cell Id [%d]",*CellId);
    }
    else if(event_id_rrm == OAM_TWENTY_EIGHT)
    {
        alarm_id_rrm = AGGR_ABS_APPLIED_FOR_OVER_LOAD_ALARM_ID;  //Alarm when Aggressor ABS PATTERN is applied for OVERLOAD.
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "AGGR_ABS_APPLIED_FOR_OVER_LOAD for Cell Id [%d]",*CellId);
    }

    /*SPR 14811 end*/

    /*eICIC_PHASE_1_2_CHANGES_END*/
    else
    {
        OAM_LOG (OAM, OAM_WARNING, " Event id passed[%u] is not valid. ", event_id_rrm);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FM_FAILURE;
    }
    OAM_FUN_EXIT_TRACE ();
    return alarm_id_rrm;
}

/******************************************************************************
 ** Function Name  : oam_rrm_get_criticality
 ** Inputs         : severity_rrm :severity_rrm
 ** Outputs        : none
 ** Returns        : criticality
 ** Description    : This function is to get criticality  of RRM.
 ********************************************************************************/

SInt16
oam_rrm_get_criticality(
    UInt16 severity_rrm
    )
{
    SInt16 criticality_rrm=OAM_ZERO;
    if     (severity_rrm== OAM_ZERO)
    {
        criticality_rrm=WARNING;
    }
    else if(severity_rrm == OAM_ONE)
    {
        criticality_rrm=MINOR;
    }
    else if(severity_rrm == OAM_TWO)
    {
        criticality_rrm=MAJOR;
    }
    else if(severity_rrm == OAM_THREE)
    {
        criticality_rrm=CRITICAL;
    }
    else if(severity_rrm == OAM_FOUR)
    {
        criticality_rrm=INDETERMINATE;
    }
    else if(severity_rrm == OAM_FIVE)
    {
        criticality_rrm=CLEARED;
    }
    else
    {
        OAM_LOG (OAM, OAM_WARNING, " Severity[%u] passed is not valid.", 
            severity_rrm);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FM_FAILURE;
    }
    return criticality_rrm;
}

/******************************************************************************
 ** Function Name  : oam_handle_rrm_alarm
 ** Inputs         : p_api_buf :Pointer to api buffer
 ** Outputs        : none
 ** Returns        : OAM_SUCCESS/OAM_FAILURE
 ** Description    : This is alarm  handling function of RRM.
 ********************************************************************************/

oam_return_et
oam_handle_rrm_alarm(
    void *p_api_buf
    )
{
    Char8 protocol_event_info[200];
    SInt16 alarm_id=OAM_ZERO;
    //SInt16 criticality= MAJOR;
    UInt16 i = OAM_ZERO;
    Char8  module_name[OAM_MODULE_NAME_SIZE]={OAM_ZERO};
    /*eICIC_PHASE_1_2_CHANGES_START*/
    Char8 add_text[OAM_MAX_TEXT_SIZE] = { OAM_ZERO };
    /*eICIC_PHASE_1_2_CHANGES_END*/
    UInt16 src_id = OAM_ZERO;
    Char8  alarm_raised_time[OAM_TIME_SIZE] = { OAM_ZERO };
    oam_alarm_info_t *p_alarm_info={OAM_ZERO};
    /* coverity 35642 */
    oam_error_code_et p_err=NO_ERROR; 
    oam_return_et ret_val = OAM_SUCCESS;
    oam_rrm_event_notification_t *p_oam_rrm_info_struct={OAM_ZERO};
    SInt32 length_read = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;
        /* Spr 12880 Fix Start */
    UInt8 CellId = OAM_INVALID_CELL_ID;
        /* Spr 12880 Fix End */
    OAM_FUN_ENTRY_TRACE ();
    /*eICIC_PHASE_1_2_CHANGES_START*/
    CellId   = get_word_from_header((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET);
    /*eICIC_PHASE_1_2_CHANGES_END*/


   if(OAM_FAILURE==oam_mem_alloc(sizeof(oam_rrm_event_notification_t),(void *)&p_oam_rrm_info_struct,&p_err))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_oam_rrm_info_struct failed"
                " with Error Code:%d",
                p_err);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }
    /* COV_FIX_18_JUN */
    msg_len = get_word_from_header((U8*)(p_api_buf) + OAM_EIGHT);
    ret_val=oam_parse_rrm_oam_event_notification(p_oam_rrm_info_struct,p_api_buf+LOWER_LAYER_INTERFACE_HEADER_SIZE,
                            msg_len-LOWER_LAYER_INTERFACE_HEADER_SIZE, &length_read); 
    /* COV_FIX_18_JUN */
     if(ret_val== OAM_FAILURE)
           {
           OAM_LOG (OAM, OAM_WARNING, " Unable to parse event notification. ");
            oam_mem_free(p_oam_rrm_info_struct,&p_err);
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
           }


    if(p_oam_rrm_info_struct->msg_header.event_type == (rrm_oam_event_class_et)RRM_OAM_ERROR_EVENT_TYPE)
    {
        OAM_LOG (OAM, OAM_WARNING, " Error Event found in RRM ");
        /*eICIC_PHASE_1_2_CHANGES_START*/
        alarm_id   =oam_rrm_get_alarm_id(p_oam_rrm_info_struct->msg_header.event_id, add_text, &CellId);
        /*eICIC_PHASE_1_2_CHANGES_END*/
        if(alarm_id == OAM_FM_FAILURE)
        {
            OAM_LOG (OAM, OAM_WARNING, " Event ID not valid. ");
            oam_mem_free(p_oam_rrm_info_struct,&p_err);
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
        }
    }
    else
    {

        oam_sprintf(protocol_event_info,"Received RRM Protocol Event= Time-<%d:%d:%d:%d:%d:%d>,Event ID:%s,Event Sub Type:%s.",
                p_oam_rrm_info_struct->msg_header.time_stamp.year,
                p_oam_rrm_info_struct->msg_header.time_stamp.month,
                p_oam_rrm_info_struct->msg_header.time_stamp.day,
                p_oam_rrm_info_struct->msg_header.time_stamp.hour,
                p_oam_rrm_info_struct->msg_header.time_stamp.min,
                p_oam_rrm_info_struct->msg_header.time_stamp.sec,
                oam_rrm_get_alarm_event_id_string_by_event_id(p_oam_rrm_info_struct->msg_header.event_id),
                oam_rrm_get_event_sub_type_string_by_event_sub_type(p_oam_rrm_info_struct->msg_header.event_sub_type));

        OAM_LOG (OAM, OAM_DETAILED,
                "%s",protocol_event_info);

        oam_mem_free(p_oam_rrm_info_struct,&p_err);
        OAM_FUN_EXIT_TRACE ();
        return  ret_val;
    }

    if(OAM_FAILURE==oam_mem_alloc(sizeof(oam_alarm_info_t),(void *)&p_alarm_info,&p_err))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_alarm_info failed"
                " with Error Code:%d",
                p_err);
        oam_mem_free(p_oam_rrm_info_struct,&p_err);
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }

    /* SPR_19838 :Coverity_11439 Fix Start */
    oam_memset(p_alarm_info,OAM_ZERO,sizeof(oam_alarm_info_t));
    /* SPR_19838 :Coverity_11439 Fix Stop */

    oam_sprintf(alarm_raised_time,"%d:%d:%d:%d:%d:%d",
            p_oam_rrm_info_struct->msg_header.time_stamp.year,
            p_oam_rrm_info_struct->msg_header.time_stamp.month,
            p_oam_rrm_info_struct->msg_header.time_stamp.day,
            p_oam_rrm_info_struct->msg_header.time_stamp.hour,
            p_oam_rrm_info_struct->msg_header.time_stamp.min,
            p_oam_rrm_info_struct->msg_header.time_stamp.sec);

    /* Extract Souce Id */
    src_id = get_word_from_header ((UInt8 *) (p_api_buf) + OAM_INTF_HDR_SRC_ID_OFFSET);


    for (i = OAM_ZERO; i < oam_prov_req.fm_cntxt.SupportedAlarmNumberOfEntries;i++)
    {
        if (alarm_id ==oam_prov_req.fm_cntxt.supported_alarm_list[i].alarm_identifier)
        {
            p_alarm_info->module_id = src_id;
            oam_memset (p_alarm_info->module_name, OAM_ZERO, OAM_MODULE_NAME_SIZE);

            if(OAM_FAILURE ==
                    oam_find_module_name (p_alarm_info->module_id, module_name))
            {
                oam_strcpy ((Char8 *) module_name,"No Fetch");
            }
            oam_strcpy((Char8 *)p_alarm_info->module_name,module_name);

            p_alarm_info->alarm_id = alarm_id;
            /*eICIC_PHASE_1_2_CHANGES_START*/
            /*SPR 14811 start*/
            /*code deleted*/
            /*SPR 14811 end*/
            p_alarm_info->notification_type = NOTIFY_NEW_ALARM; 
            /*eICIC_PHASE_1_2_CHANGES_END*/
            p_alarm_info->event_type =
                oam_prov_req.fm_cntxt.supported_alarm_list[i].event_type;
            p_alarm_info->probable_cause =
                oam_prov_req.fm_cntxt.supported_alarm_list[i].probable_cause;
            /*SPR 14811 start*/
            oam_strcpy(p_alarm_info->specific_problem,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].specific_problem);  
            oam_strcpy(p_alarm_info->probable_cause_str,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].probable_cause_str); 
            /*SPR 14811 end*/
            p_alarm_info->criticality = oam_prov_req.fm_cntxt.supported_alarm_list[i].perceived_severity;
            p_alarm_info->reporting_mechanism =
                oam_prov_req.fm_cntxt.supported_alarm_list[i].reporting_mechanism;
            oam_strcpy (p_alarm_info->time_stamp, alarm_raised_time);
            oam_strcpy (p_alarm_info->add_txt, (Char8 *) add_text);
            oam_strcpy ((char *)p_alarm_info->managed_object_instance,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].managed_object_instance);
            oam_strcat ((char *)p_alarm_info->managed_object_instance,(char*) get_module_id_for_alarm(p_alarm_info->module_id));
        }
    }
                    
    p_alarm_info->CellId = CellId;
    ret_val=oam_handle_alarm (p_alarm_info);
    oam_mem_free(p_alarm_info,&p_err);
    oam_mem_free(p_oam_rrm_info_struct,&p_err);

    OAM_FUN_ENTRY_TRACE ();
    return ret_val;
}
/******************************************************************************
 ** Function Name  : oam_mac_handle_error_inds
 ** Inputs         : p_api_buf :Pointer to api buffer
 ** Outputs        : none
 ** Returns        : OAM_SUCCESS/OAM_FAILURE
 ** Description    : This is error  handling function of MAC.
 ******************************************************************************/

oam_return_et
oam_mac_handle_error_inds
(
 void *p_api_buf,UInt16* alarm_id,UInt16* criticality,Char8* add_text)
{
        /* Spr 12880 Fix Start */
    UInt16 CellId   = get_word_from_header((UInt8*)(p_api_buf) + OAM_INTF_HDR_CELL_IDX_OFFSET);
        /* Spr 12880 Fix End */
    UInt8 error_flag=OAM_FALSE;

    oam_mac_error_ind_handler_t g_mac_error_ind_handler;

    OAM_FUN_ENTRY_TRACE ();
    /* BUG_9728_9713_LB_START */
    oam_memcpy(&g_mac_error_ind_handler.typeOfError, p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE, sizeof(g_mac_error_ind_handler.typeOfError));
    /* BUG_9728_9713_LB_END */
    OAM_LOG(OAM,OAM_INFO, "MAC_PHY_OAM_ERROR_IND:Error_Code is %d", g_mac_error_ind_handler.typeOfError);
    if(g_mac_error_ind_handler.typeOfError == MAC_PHY_OAM_ERR_MSG_INVALID_STATE){
        error_flag = OAM_TRUE;
        OAM_LOG(OAM,OAM_INFO, "Raising alarm for MAC_PHY_OAM_ERROR_IND : MAC_PHY_OAM_ERR_MSG_INVALID_STATE Recieved from MAC");
        *alarm_id = OAM_PHY_MAC_MESSAGE_ERROR_ONE_ID;
        *criticality = OAM_ONE;
    }
    else if(g_mac_error_ind_handler.typeOfError == MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD){
        error_flag = OAM_TRUE;
        OAM_LOG(OAM,OAM_INFO, "Raising alarm for MAC_PHY_OAM_ERROR_IND : MAC_PHY_OAM_ERR_SFN_OUT_OF_SYNC_BY_MORE_THAN_THRESHOLD Recieved from MAC");
        *alarm_id = OAM_PHY_MAC_MESSAGE_ERROR_TWO_ID;
        *criticality = OAM_ONE;
    }
    else if(g_mac_error_ind_handler.typeOfError == MAC_PHY_OAM_ERR_MSG_BCH_MISSING){
        error_flag = OAM_TRUE;
        OAM_LOG(OAM,OAM_INFO, "Raising alarm for MAC_PHY_OAM_ERROR_IND : MAC_PHY_OAM_ERR_MSG_BCH_MISSING Recieved from MAC");
        *alarm_id = OAM_PHY_MAC_MESSAGE_ERROR_THREE_ID;
        *criticality = OAM_ONE;
    }
    else if(g_mac_error_ind_handler.typeOfError == MAC_PHY_OAM_ERR_MSG_HI_ERR){
        error_flag = OAM_TRUE;
        *alarm_id = OAM_PHY_MAC_MESSAGE_ERROR_FOUR_ID;
        *criticality = OAM_ONE;
        OAM_LOG(OAM,OAM_INFO, "Raising alarm for MAC_PHY_OAM_ERROR_IND : MAC_PHY_OAM_ERR_MSG_HI_ERR Recieved from MAC");
    }


    OAM_FUN_EXIT_TRACE ();
    if(error_flag){
        /* Spr 12880 Fix Start */
            oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                    "FAPI error indication for cell_index [%d]: Recommendation for Operator Cell Restart",CellId);
        /* Spr 12880 Fix End */
        return OAM_SUCCESS;
    }else{
        return OAM_FAILURE;
    }
}

/* SPR 13765 changes start */
/******************************************************************************
 ** Function Name  : oam_egtpu_handle_error_inds
 ** Inputs         : p_api_buf :Pointer to api buffer
 ** Outputs        : none
 ** Returns        : OAM_SUCCESS/OAM_FAILURE
 ** Description    : This is error  handling function for EGTPU.
 ******************************************************************************/
oam_return_et
oam_egtpu_handle_error_inds
(
  void *p_api_buf,
  UInt16* alarm_id,
  UInt16* criticality,
  Char8* add_text
)
{
    relayGtpuErrorIndMsg relayGtpuErrIndMsg; 
    oam_return_et retVal = OAM_FAILURE; /* Coverity 69693 Fix */
    OAM_FUN_ENTRY_TRACE ();

    /* SPR 13833 Changes Start */
    oam_cp_unpack_UInt32((void *)(&(relayGtpuErrIndMsg.typeOfError)), 
            (p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE)
            /* SPR 20908 Start */
            ,"header"
            /* SPR 20908 End */
            ); 
    /* SPR 13833 Changes End */
    /* SPR 13834 Changes Start */
    oam_strncpy((char *) relayGtpuErrIndMsg.errStr,
            ((const char *)p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE + sizeof(relayGtpuErrIndMsg.typeOfError)),
            oam_strlen((p_api_buf + LOWER_LAYER_INTERFACE_HEADER_SIZE + sizeof(relayGtpuErrIndMsg.typeOfError))) + 1 );
    /* SPR 13834 Changes End */

    if(S1U_LINK_FAILURE == relayGtpuErrIndMsg.typeOfError)
    {
        OAM_LOG(OAM,OAM_INFO, "Raising alarm for PR_GTPU_ERROR_IND :"
                                  " S1U_LINK_FAILURE Recieved from EGTPU"
                                  "IP:[%s]",relayGtpuErrIndMsg.errStr);
        *alarm_id = EGTPU_S1U_LINK_DOWN_IND_ALARM_ID;
        *criticality = OAM_ONE;
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "S1U Link failure for SGW IP Address:[%s]",relayGtpuErrIndMsg.errStr);
        retVal = OAM_SUCCESS;
    }
    else if(S1U_LINK_FAILURE_RECOVERED == relayGtpuErrIndMsg.typeOfError)
    {
        OAM_LOG(OAM,OAM_INFO, "Clearing alarm for PR_GTPU_ERROR_IND :"
                                  " S1U_LINK_FAILURE_RECOVERY Recieved from EGTPU"
                                  "IP:[%s]",relayGtpuErrIndMsg.errStr);
        *alarm_id = EGTPU_S1U_LINK_UP_IND_ALARM_ID;
        *criticality = OAM_THREE;
        oam_snprintf(add_text, OAM_MAX_TEXT_SIZE-OAM_ONE,
                "S1U Link failure recovery for SGW IP Address:[%s]",relayGtpuErrIndMsg.errStr);
        retVal = OAM_SUCCESS;
    }
    else
    {
        retVal = OAM_FAILURE;
    }
    OAM_FUN_EXIT_TRACE ();
    return retVal;

}

/* SPR 13765 changes end */

/* SPR FIX 10190 */
/******************************************************************************
 *  *  ** Function Name  : oam_son_get_alarm_id
 *  *  ** Inputs         : event_id_son
 *  *  ** Outputs        : none
 *  *  ** Returns        : alarm_id
 *  *  ** Description    : This is a function for getting the alarm_id from event_id of SON.
 *  *  ********************************************************************************/
 
UInt16
oam_son_get_alarm_id(
    UInt16 event_id_son
    )

{
    UInt16 alarm_id_son=OAM_ZERO;
    OAM_FUN_ENTRY_TRACE ();
    if(event_id_son ==OAM_ONE)
    {
        alarm_id_son = MEM_ALLOC_FAILED_ALARM_ID;   // Alarm for memory alloc failed.
    }
    else if(event_id_son==OAM_TWO)
    {
        alarm_id_son = MEM_FREE_FAILED_ALARM_ID;  // Alarm for memory free failed.
    }

    else if(event_id_son==OAM_THREE)
    {
        alarm_id_son = TIMER_START_FAILED_ALARM_ID;  // Alarm for transmit message failed.
    }
    else if(event_id_son==OAM_FOUR)
    {
        alarm_id_son = TIMER_STOP_FAILED_ALARM_ID;  // Alarm for transmit message failed.
    }
    else if(event_id_son==OAM_FIVE)
    {
        alarm_id_son = SEND_MSG_FAILED_ALARM_ID;  // Alarm for transmit message failed.
    }
    else if(event_id_son==OAM_SIX)
    {
        alarm_id_son = RECV_MSG_FAILED_ALARM_ID;  // Alarm for transmit message failed.
    }
    
    
    else
    {
        OAM_FUN_EXIT_TRACE ();
/*COVERITY FIX START (CID 64324)*/
        return BASE_GENERIC_ALARM_ID;
/*COVERITY FIX END (CID 64324)*/
    }
      
    OAM_FUN_EXIT_TRACE ();
    return alarm_id_son;
}
/* SPR 14630 Fix Start */
/******************************************************************************
 *Function Name  : oam_find_alarm_clearing_info 
 *Inputs         : alarm_id , p_alarm_cleared
 *Outputs        : alarm buf for clearing alarm
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *Description    : This Function find if alarm id is 
                   clearing condition for any alarm  
 ******************************************************************************/
oam_return_et
oam_find_alarm_clearing_info(UInt16 alarm_id, 
                                   oam_alarm_info_t ** p_alarm_cleared,
                                   UInt8 CellId)
{
    UInt8 max_element = OAM_ZERO;
    UInt8 index = OAM_ZERO;
    SInt32 alarm_id_to_be_cleared = OAM_ZERO;
    SInt32 curr_alarm_idx = OAM_ZERO;
    oam_error_code_et p_err= NO_ERROR; 
    oam_alarm_info_t * p_temp_alarm_cleared;

    OAM_FUN_ENTRY_TRACE ();

    max_element = (ARRSIZE(g_alarm_clearing_map_table));

    for(index= OAM_ZERO;index< max_element;index++)
    {
        /*Comparison between given Alarm Id and database stored Alarm ID */
        if(alarm_id == g_alarm_clearing_map_table[index].alarm_id) 
        {
            alarm_id_to_be_cleared = g_alarm_clearing_map_table[index].clearing_cond_for_alarm_id;
            break;
        } 
    }

    if (OAM_ZERO != alarm_id_to_be_cleared)
    { 
        curr_alarm_idx = oam_fm_fetch_curr_alarm_idx (alarm_id_to_be_cleared,CellId);
        if(OAM_FM_FAILURE == curr_alarm_idx)
        {
            OAM_LOG(OAM,OAM_INFO,"Alarm [%d] is not raised yet",alarm_id);
        }
        else
        {
            OAM_LOG(OAM,OAM_INFO,"Alarm [%d] needs to be cleared",
                                     alarm_id_to_be_cleared);

            if(OAM_FAILURE==oam_mem_alloc(sizeof(oam_alarm_info_t),(void *)&p_temp_alarm_cleared,&p_err))
            {
                OAM_LOG(OAM, OAM_ERROR,
                        "Memory allocation to p_temp_alarm_cleared failed"
                        " with Error Code:%d",
                        p_err);
                OAM_FUN_EXIT_TRACE ();
                return OAM_FAILURE;
            }

            oam_memset(p_temp_alarm_cleared,OAM_ZERO,sizeof(oam_alarm_info_t));

            p_temp_alarm_cleared ->alarm_id = alarm_id_to_be_cleared;
            p_temp_alarm_cleared ->criticality = CLEARED;
            oam_strcpy (p_temp_alarm_cleared ->add_txt,
                        (Char8 *)oam_prov_req.fm_cntxt.
                        current_alarm_list[curr_alarm_idx].additional_text);
           *p_alarm_cleared = p_temp_alarm_cleared; 

        }
    }
    else
    {
        OAM_LOG(OAM,OAM_INFO,"Alarm [%d] is not clearing condition for any alarm",alarm_id); 
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;

    }
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}

/******************************************************************************
 *Function Name  : oam_populate_alarm_info 
 *Inputs         : p_alarm_info
 *Outputs        : buf for alarm
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *Description    : This Function populates alarm info from supported alarm List
 ******************************************************************************/
oam_return_et
oam_populate_alarm_info(oam_alarm_info_t *p_alarm_info)
{
    UInt8 i = OAM_ZERO;
    Char8  module_name[OAM_MODULE_NAME_SIZE]={OAM_ZERO};
    Char8  alarm_raised_time[OAM_TIME_SIZE] = { OAM_ZERO };

    OAM_FUN_ENTRY_TRACE ();
    /*skipping Alarm recieved RRC_LICENSE_EXPIRY_ALARM_ID */
    if (RRC_LICENSE_EXPIRY_ALARM_ID == p_alarm_info->alarm_id)
    {    
        OAM_LOG (OAM, OAM_INFO,
                "Ignoring Alarm recieved RRC_LICENSE_EXPIRY_ALARM_ID  " );
        OAM_FUN_EXIT_TRACE ();
        return OAM_FAILURE;
    }    
    else
    {
        if (OAM_FAILURE == oam_create_timestamp_with_timezone (alarm_raised_time))
        {
            OAM_LOG (OAM, OAM_WARNING, " oam_create_timestamp_with_timezone function failed ");
            OAM_FUN_EXIT_TRACE ();
            return OAM_FAILURE;
        }

        for (i = OAM_ZERO; i < oam_prov_req.fm_cntxt.SupportedAlarmNumberOfEntries;i++)
        {
            if (p_alarm_info->alarm_id ==oam_prov_req.fm_cntxt.supported_alarm_list[i].alarm_identifier)
            {
                oam_memset (p_alarm_info->module_name, OAM_ZERO, OAM_MODULE_NAME_SIZE);

                if(OAM_FAILURE ==
                        oam_find_module_name (p_alarm_info->module_id, module_name))
                {
                    oam_strcpy ((Char8 *) module_name,"No fetch");
                }
                oam_strcpy((Char8 *)p_alarm_info->module_name,module_name);

                if (p_alarm_info->criticality == CLEARED)
                {
                    p_alarm_info->notification_type = NOTIFY_CLEARED_ALARM;
                }
                else
                {
                    p_alarm_info->notification_type = NOTIFY_NEW_ALARM;
                }
                p_alarm_info->event_type =
                    oam_prov_req.fm_cntxt.supported_alarm_list[i].event_type;

                oam_strcpy(p_alarm_info->specific_problem,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].specific_problem);  
                oam_strcpy(p_alarm_info->probable_cause_str,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].probable_cause_str); 

                p_alarm_info->probable_cause =
                    oam_prov_req.fm_cntxt.supported_alarm_list[i].probable_cause;
                p_alarm_info->criticality = oam_prov_req.fm_cntxt.supported_alarm_list[i].perceived_severity;
                p_alarm_info->reporting_mechanism =
                    oam_prov_req.fm_cntxt.supported_alarm_list[i].reporting_mechanism;
                oam_strcpy (p_alarm_info->time_stamp, alarm_raised_time);
                oam_strcpy ((char *)p_alarm_info->managed_object_instance,(char *)oam_prov_req.fm_cntxt.supported_alarm_list[i].managed_object_instance);
                oam_strcat ((char *)p_alarm_info->managed_object_instance,(char*) get_module_id_for_alarm(p_alarm_info->module_id));
            }
        }
    }
    OAM_FUN_EXIT_TRACE ();
    return OAM_SUCCESS;
}
/* SPR 14630 Fix End */
