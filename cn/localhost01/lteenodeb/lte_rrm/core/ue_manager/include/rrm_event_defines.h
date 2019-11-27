/******************************************************************************
 *
 *   FILE NAME:   rrm_events_define.h 
 *
 *   DESCRIPTION:
 *       This file contains  
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   10 Nov 2012    gur19685     ---------       Initial
 *
 *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/

#ifndef _RRM_EVENTS_DEF_H_
#define _RRM_EVENTS_DEF_H_

#include "rrm_utils.h" 

#define  MAX_ERAB_ID                    16
#define  RRM_EVENT_VALID_VALUE          1
#define  RRM_EVENT_U8_INVALID_VALUE     0xf
#define  RRM_EVENT_U16_INVALID_VALUE    0xff
#define  RRM_EVENT_U32_INVALID_VALUE    0xffff
#define  RRM_EVENT_HO_IN_FAILED         1
#define  RRM_ERR_EVENT_MAX_ASSOCIATED_DATA_LEN   50


void
rrm_raise_error_event_notification(U32    err_event_id);


/**************** EVENT IDs *****************/
typedef enum 
{
    RRM_MIN_PROTOCOL_EVENT_ID,

    /* Cell specific protocol events */ 
    RRM_PE_CELL_SPECIFIC_MIN_ID = RRM_MIN_PROTOCOL_EVENT_ID,
    RRM_PE_CELL_SETUP_REJECT,
    RRM_PE_CELL_SPECIFIC_MAX_ID,

    /* UE specific protocol events*/
    RRM_PE_UE_SPECIFIC_MIN_ID,
    RRM_PE_RRC_CONN_SETUP_READY,
    RRM_PE_EVENT_UE_CAPABILITY,
    RRM_PE_EVENT_UE_CAPABILITY_FGI_REJECT,
    RRM_PE_UE_SPECIFIC_MAX_ID,

    /** Erab specific protocol events*/
    RRM_PE_ERAB_SPECIFIC_MIN_ID,
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
    RRM_PE_ERAB_SPECIFIC_MAX_ID,

    /*Meas-config specific protocol events*/
    RRM_PE_MEAS_CONFIG_SPECIFIC_MIN_ID,
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
    RRM_PE_EVENT_MEAS_CONFIG_PERIODICAL_CDMA,
    RRM_PE_EVENT_MEAS_CONFIG_B2_CDMA2000,


    RRM_PE_MEAS_CONFIG_SPECIFIC_MAX_ID,

    /*HO IN specific protocol events*/
    RRM_PE_HO_IN_SPECIFIC_MIN_ID,
    RRM_PE_HO_IN_EXEC_COMPLETE,
    RRM_PE_HO_IN_PREP_COMPLETE,
    RRM_PE_HO_IN_EXEC_ATTEMPT,
    RRM_PE_HO_IN_EXEC_FAILED,
    RRM_PE_EVENT_HO_IN_PREP_ATTEMPT,
    RRM_PE_EVENT_HO_IN_PREP_FAILURE,
    RRM_PE_HO_IN_SPECIFIC_MAX_ID,

    /*HO specific protocol events*/
    RRM_PE_HO_OUT_SPECIFIC_MIN_ID,
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
    RRM_PE_HO_OUT_SPECIFIC_MAX_ID,

    /*Others protocol events*/
    RRM_PE_OTHER_MIN_ID,
    RRM_PE_MME_CONFIGURATION_TRANSFER,
    RRM_PE_ENB_CONFIGURATION_TRANSFER,
    RRM_PE_SRB1_SETUP_REJECT,

    RRM_MAX_PROTOCOL_EVENT_ID
}rrm_event_message_id_et;

typedef enum
{
    RRM_EE_MEM_ALLOC_FAIL,
    RRM_EE_MEM_FREE_FAIL,
    RRM_EE_IPC_SEND_MSG_FAIL,
    RRM_EE_IPC_RECV_MSG_FAIL,
    RRM_EE_TIMER_START_FAIL,
    RRM_EE_TIMER_STOP_FAIL,
    RRM_MAX_ERR_EVENT_ID,
    /*SPR 14811 start*/
    RRM_EE_ABS_NOT_FOUND_LOW_LOAD,
    RRM_EE_ABS_NOT_FOUND_MID_LOAD,
    RRM_EE_ABS_NOT_FOUND_HIGH_LOAD,
    RRM_EE_ABS_NOT_FOUND_OVER_LOAD,
    RRM_EE_ABS_FOUND_LOW_LOAD,
    RRM_EE_ABS_FOUND_MID_LOAD,
    RRM_EE_ABS_FOUND_HIGH_LOAD,
    RRM_EE_ABS_FOUND_OVER_LOAD,
    /*ut_bug_13987_fix_start*/
    RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_LOW_LOAD,
    RRM_EE_AGGR_ABS_APPLIED_FOR_LOW_LOAD,
    RRM_EE_VICTM_ABS_NOT_APPLIED,
    /*ut_bug_13987_fix_end*/
    RRM_EE_VICTM_ABS_APPLIED,
    RRM_EE_VICTIM_ABS_NOT_FOUND,
    RRM_EE_VICTIM_ABS_FOUND,
    RRM_EE_VICTIM_MEAS_SUBSET_NOT_FOUND,
    RRM_EE_VICTIM_MEAS_SUBSET_FOUND,
    RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_MID_LOAD,
    RRM_EE_AGGR_ABS_APPLIED_FOR_MID_LOAD,
    RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_HIGH_LOAD,
    RRM_EE_AGGR_ABS_APPLIED_FOR_HIGH_LOAD,
    RRM_EE_AGGR_ABS_NOT_APPLIED_FOR_OVER_LOAD, 
    RRM_EE_AGGR_ABS_APPLIED_FOR_OVER_LOAD 
    /*SPR 14811 end*/
}rrm_error_event_id_et;


/*******************************************************
 ******* 	     EVENTS APIs  	      **********
 *******************************************************/
typedef enum _rrm_estb_cause_et
{
    RRM_ESTAB_CAUSE_EMERGENCY,
    RRM_ESTAB_CAUSE_HIGHPRIORITY_ACCESS,
    RRM_ESTAB_CAUSE_MT_ACCESS,
    RRM_ESTAB_CAUSE_MO_SIGNALLING,
    RRM_ESTAB_CAUSE_MO_DATA,
    /*Rel10 CR changes start*/
    RRM_ESTAB_CAUSE_DELAYTOLERANTACCESS_V1020
        /*Rel10 CR changes end*/
}rrm_estb_cause_et;


typedef enum _rrm_erab_adm_rej_result_et
{
    RRM_EVENT_SUCCESS,
    RRM_EVENT_INVALID_PARAMS_REJECT,
    RRM_EVENT_ERAB_ID_CONFLICT,  
    RRM_EVENT_TNL_REJECT,
    RRM_EVENT_UE_CAPABILITY_FGI_REJECT ,
    RRM_EVENT_GBR_UL_CAPACITY_REJECT,
    RRM_EVENT_GBR_DL_CAPACITY_REJECT,
    RRM_EVENT_LACK_OF_RESOURCES
}rrm_erab_adm_rej_result_et;


typedef enum _rrm_cell_setup_fail_cause_et
{
    RRM_EVENT_NO_ERROR,
    RRM_EVENT_CELL_SETUP_REJ_INVALID_PARAMS
}rrm_cell_setup_fail_cause_et;

typedef enum
{
    RRM_EVENT_VALUE_RECEIVED,
    RRM_EVENT_VALUE_SENT,
    RRM_EVENT_DIRECTION_NONE
}ext_msg_direction_et;

typedef enum _rrm_meas_requester_et
{
    RRM_EVENT_VALUE_TRAFFIC=0,
    RRM_EVENT_VALUE_ANR=1,
    RRM_EVENT_VALUE_PM=2
}rrm_meas_requester_et;


typedef enum _rrm_meas_trigger_quantity_et
{
    RRM_EVENT_VALUE_RSRP=0,
    RRM_EVENT_VALUE_RSRQ=1
}rrm_meas_trigger_quantity_et;

typedef enum _rrm_trigger_quantity_utra_et
{
    RRM_EVENT_VALUE_RSCP=0,
    RRM_EVENT_VALUE_ECN0=1
}rrm_trigger_quantity_utra_et;

typedef enum _rrm_meas_report_interval_et
{
    RRM_EVENT_VALUE_120_MS=0,
    RRM_EVENT_VALUE_240_MS=1,
    RRM_EVENT_VALUE_480_MS=2,
    RRM_EVENT_VALUE_640_MS=3,
    RRM_EVENT_VALUE_1024_MS=4,
    RRM_EVENT_VALUE_2048_MS=5,
    RRM_EVENT_VALUE_5120_MS=6,
    RRM_EVENT_VALUE_10240_MS=7,
    RRM_EVENT_VALUE_1_MIN=8,
    RRM_EVENT_VALUE_6_MIN=9,
    RRM_EVENT_VALUE_12_MIN=10,
    RRM_EVENT_VALUE_30_MIN=11,
    RRM_EVENT_VALUE_60_MIN=12
}rrm_meas_report_interval_et;

typedef enum _rrm_meas_report_amount_et
{
    RRM_EVENT_VALUE_1=0,
    RRM_EVENT_VALUE_2=1,
    RRM_EVENT_VALUE_4=2,
    RRM_EVENT_VALUE_8=3,
    RRM_EVENT_VALUE_16=4,
    RRM_EVENT_VALUE_32=5,
    RRM_EVENT_VALUE_64=6,
    RRM_EVENT_VALUE_INFINITY=7
}rrm_meas_report_amount_et;

typedef enum _rrm_ho_source_target_type_et
{
    RRM_HO_LTE_INTRA_FREQ,
    RRM_HO_LTE_INTER_FREQ,
    RRM_HO_UTRAN,
    RRM_HO_GERAN,
    RRM_HO_UTRAN_SOURCE
}rrm_ho_source_target_type_et;

typedef enum _rrm_ho_type_et
{
    RRM_EVENT_VALUE_S1,
    RRM_EVENT_VALUE_INTRA_ENB,
    RRM_EVENT_VALUE_X2
}rrm_ho_type_et;

typedef enum _rrm_ho_target_selection_type_et
{
    RRM_EVENT_VALUE_BLIND,
    RRM_EVENT_VALUE_MEAS_BASED
}rrm_ho_target_selection_type_et;



typedef enum _rrm_event_ho_src_rat_et
{
    RRM_EVENT_LTE_INTRA_FREQ,
    RRM_EVENT_LTE_INTER_FREQ,
    RRM_EVENT_LTE_WCDMA,
    RRM_EVENT_LTE_GERAN,
    RRM_EVENT_LTE_CDMA2000
}rrm_event_ho_src_rat_et;

typedef enum _rrm_event_ho_prep_fail_cause_et
{
    RRM_EVENT_NUMBER_OF_ACTIVE_USERS_REACHED,
    RRM_EVENT_CANNOT_HANDLE_BEARER_COMBINATION,
    RRM_EVENT_HANDOVER_TARGET_NOT_ALLOWED
}rrm_event_ho_prep_fail_cause_et;


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
#endif 
