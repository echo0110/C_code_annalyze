/* vim: ts=4:et:ai:ci:sw=4
 */
/***************************************************************************
 *
 *  ARICENT GROUP
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_mif_fsm.c v0.1 2012/01/24 10:50:45 $
 ****************************************************************************
 *
 *  File Description :
 *    This file comtains the RRM MgmtIfH FSM. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrm_mif_fsm.c  $
 * Initial code
 * DATE         AUTHOR          REFERENCE              REASON
 * 2014/05/14   gur29831        SPR 10869        Crash was observed on lte_rrm  
 *                                              corresponding changes were done
 *                                              by calling appropriate tcb function.                                                  
 *
 ****************************************************************************/

/****************************************************************************
 * Header File Includes
 ****************************************************************************/
#include "rrm_stats.h"

/*! \headerfile rrm_mif_fsm.h <>
 */
#include <rrm_mif_fsm.h>
/*! \headerfile rrm_mif_db.h <>
 */
#include <rrm_mif_db.h>

/*! \headerfile rrm_mif_msg_handler.h <>
 */
#include <rrm_mif_msg_handler.h>

#include <rrm_son_composer.h>
#include "rrm_utils.h"
#include "rrm_mem_utils.h"

const S8 * MIF_FSM_STATE_TRACE[]=
{  
    (const S8 *)"MIF_FSM_STATE_INITIAL",                              /*!< MIF_FSM_STATE_INITIAL*/
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_CONFIG_RESP",               /*!< MIF_FSM_STATE_W_FOR_CELL_CONFIG_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP",             /*!< MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP",               /*!< MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_START_RESP",                /*!< MIF_FSM_STATE_W_FOR_CELL_START_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_STOP_RESP",                 /*!< MIF_FSM_STATE_W_FOR_CELL_STOP_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP",               /*!< MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP",           /*!< MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP",            /*!< MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_SWITCH_ON",                      /*!< MIF_FSM_STATE_W_FOR_SWITCH_ON */
    (const S8 *)"MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP",    /*!< MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP */
    (const S8 *)"MIF_FSM_STATE_ACTIVE",                               /*!< MIF_FSM_STATE_ACTIVE */
    (const S8 *)"MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY",          /*!< MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY */
    (const S8 *)"MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP",        /*!< MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP */
    (const S8 *)"MIF_FSM_STATE_CELL_ALREADY_BLOCKED",                 /*!< MIF_FSM_STATE_CELL_ALREADY_BLOCKED */
    (const S8 *)"MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP",  /*!< MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP */
    (const S8 *)"MIF_FSM_STATE_W_FOR_CELL_RESUME_RESP",               /*!< MIF_FSM_STATE_W_FOR_CELL_RESUME_RESP */
    (const S8 *)"MIF_FSM_NUM_OF_STATES",
    (const S8 *)"MIF_FSM_INVALID_STATE",

};
const S8 * MIF_FSM_EVENT_TRACE[]=
{
    (const S8 *)"MIF_FSM_EVENT_CELL_CONFIG_REQUEST",                  /*!< MIF_FSM_EVENT_CELL_CONFIG_REQUEST */
    (const S8 *)"MIF_FSM_EVENT_CELL_RECONFIG_REQ",                    /*!< MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
    (const S8 *)"MIF_FSM_EVENT_CELL_DELETE_REQ",                      /*!< MIF_FSM_EVENT_CELL_DELETE_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_START_REQ",                       /*!< MIF_FSM_EVENT_CELL_START_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_STOP_REQ",                        /*!< MIF_FSM_EVENT_CELL_STOP_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_UPDATE_REQ",                      /*!< MIF_FSM_EVENT_CELL_UPDATE_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_SHUTDOWN_REQ",                    /*!< MIF_FSM_EVENT_CELL_SHUTDOWN_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_SWITCH_ON_REQ",                   /*!< MIF_FSM_EVENT_CELL_SWITCH_ON_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ",                  /*!< MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ */
    (const S8 *)"MIF_FSM_EVENT_CELL_CONFIG_RESP",                     /*!< MIF_FSM_EVENT_CELL_CONFIG_RESP */
    (const S8 *)"MIF_FSM_EVENT_CELL_RECONFIG_RESP",                   /*!< MIF_FSM_EVENT_CELL_RECONFIG_RESP */
    (const S8 *)"MIF_FSM_EVENT_CELL_DELETE_RESP",                     /*!< MIF_FSM_EVENT_CELL_DELETE_RESP */
    (const S8 *)"MIF_FSM_EVENT_CELL_START_RESP",                      /*!< MIF_FSM_EVENT_CELL_START_RESP */
    (const S8 *)"MIF_FSM_EVENT_CELL_STOP_RESP",                       /*!< MIF_FSM_EVENT_CELL_STOP_RESP */
    (const S8 *)"MIF_FSM_EVENT_CELL_UPDATE_RESP",                     /*!< MIF_FSM_EVENT_CELL_UPDATE_RESP */
    (const S8 *)"MIF_FSM_EVENT_START_ADMISSION_RESP",                 /*!< MIF_FSM_EVENT_START_ADMISSION_RESP */
    (const S8 *)"MIF_FSM_EVENT_STOP_ADMISSION_RESP",                  /*!< MIF_FSM_EVENT_STOP_ADMISSION_RESP */
    (const S8 *)"MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP",           /*!< MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP */
    (const S8 *)"MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY",     /*!< MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY */
    (const S8 *)"MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ",               /*!< MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ */
    (const S8 *)"MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP",              /*!< MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP */
    (const S8 *)"MIF_FSM_EVENT_CELL_RESUME_REQ",                      /*!< MIF_FSM_EVENT_CELL_RESUME_REQ */
    (const S8 *)"MIF_FSM_EVENT_BLOCK_CELL_REQ",                      /*!< MIF_FSM_EVENT_BLOCK_CELL_REQ */
    (const S8 *)"MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP",                      /*!< MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP */
    (const S8 *)"MIF_FSM_EVENT_UNBLOCK_CELL_CMD",                      /*!< MIF_FSM_EVENT_UNBLOCK_CELL_CMD */
    (const S8 *)"MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ",                  /*!<MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
    (const S8 *)"MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP",                  /*!<MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
    (const S8 *)"MIF_FSM_EVENT_NO_ACTIVE_UE_IND",                      /*!< MIF_FSM_EVENT_NO_ACTIVE_UE_IND */
    (const S8 *)"MIF_FSM_EVENT_CELL_PLATFORM_IND",                      /*!< MIF_FSM_EVENT_CELL_PLATFORM_IND*/
    (const S8 *)"MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ",                  /*!<MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
    (const S8 *)"MIF_FSM_EVENT_CELL_GET_KPI_REQ",                     /*!<MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
    (const S8 *)"MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP",                     /*!<MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
    (const S8 *)"MIF_FSM_EVENT_CELL_GET_KPI_RESP",                     /*!<MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    (const S8 *)"MIF_FSM_EVENT_CELL_RESUME_RESP",                     /*!< MIF_FSM_EVENT_CELL_RESUME_RESP */
    /*LTE_RRM_KLOCWORK_WARN_25_JUL_START*/
    (const S8 *)"MIF_FSM_EVENT_MEAS_CONFIG_REQ",                      /*!< MIF_FSM_EVENT_MEAS_CONFIG_REQ */
    /*LTE_RRM_KLOCWORK_WARN_25_JUL_END*/
    (const S8 *)"MIF_FSM_NUM_OF_EVENTS",                              /*!< MIF_FSM_NUM_OF_EVENTS */
    /*LTE_RRM_KLOCWORK_WARN_25_JUL_START*/
    (const S8 *)"MIF_FSM_INVALID_EVENT",                              /*!< MIF_FSM_INVALID_EVENT */
    /*LTE_RRM_KLOCWORK_WARN_25_JUL_END*/

};
static rmif_event_et     rcvd_event_id = MIF_FSM_INVALID_EVENT;
/* BUG_11576_FIX_START */
//code deleted
/* BUG_11576_FIX_END */
void rrm_check_for_rmif_init_state
(
 rrm_mif_gb_context_t*  p_gl_ctxt
 );

static void rrm_mif_fsm_send_cell_block_response
(
 rrm_error_et          fail_cause,
 rrm_return_et         response,
 U16                   trans_id,
    U8                    cell_index,
 void                  *p_msg
 );


RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_config_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_reconfig_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_delete_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_start_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_stop_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_update_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_shutdown_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_switch_on_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_switch_off_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_config_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_reconfig_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_delete_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_start_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_stop_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_update_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_start_admission_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_stop_admission_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_timer_expiry_handler_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_emerg_call_active_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_impossible_event_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_rac_enable_disable_request_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_rac_enable_disable_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_resume_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_block_cell_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_non_emrgncy_active_calls_ho_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_unblock_cell_cmd_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_already_blocked_cell_block_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_reconfig_ongoing_cell_block_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_delete_ongoing_cell_block_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_start_ongoing_cell_block_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_stop_ongoing_cell_block_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_switchon_ongoing_cell_block_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_ecn_capacity_enhance_req_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_no_active_ue_ind_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_platform_ind_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_config_kpi_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_get_kpi_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_config_kpi_resp_act);
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_get_kpi_resp_act);
/* SPR 18055 Start */
RRM_MIF_FSM_ACTION_ROUTINE(rrm_mif_fsm_cell_block_req_act_cell_block_ongoing);
/* SPR 18055 End */

static mif_fsm_action_routine rrm_mif_fsm_table[MIF_FSM_NUM_OF_STATES]
[MIF_FSM_NUM_OF_EVENTS] =
{
    /*MIF_FSM_STATE_INITIAL*/
    {
        rrm_mif_fsm_cell_config_request_act,                 /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_impossible_event_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_impossible_event_act,                       /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_impossible_event_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_impossible_event_act,                       /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_impossible_event_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_impossible_event_act                       /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/


    },
    /*MIF_FSM_STATE_W_FOR_CELL_CONFIG_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_cell_config_resp_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_impossible_event_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_impossible_event_act,                       /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_cell_reconfig_resp_act,                  /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_reconfig_ongoing_cell_block_req_act, /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                     /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                     /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_cell_delete_resp_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_delete_ongoing_cell_block_req_act,   /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_impossible_event_act,                       /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_CELL_START_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_cell_start_resp_act,                     /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_start_ongoing_cell_block_req_act,    /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_impossible_event_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                     /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_CELL_STOP_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_cell_stop_resp_act,                      /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_stop_ongoing_cell_block_req_act,     /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        /*Cell update start*/
        rrm_mif_fsm_cell_update_resp_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        /*Cell update end*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_reconfig_ongoing_cell_block_req_act, /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                     /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                     /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_start_admission_resp_act,                /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                     /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_stop_admission_resp_act,                 /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_SWITCH_ON*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_cell_switch_on_request_act,              /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_stop_admission_resp_act,                 /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_switchon_ongoing_cell_block_req_act, /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_impossible_event_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_emerg_call_active_resp_act,              /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        /* SPR 18055 Start */
        rrm_mif_fsm_cell_block_req_act_cell_block_ongoing,    /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        /* SPR 18055 End */
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                     /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },  
    /*MIF_FSM_STATE_ACTIVE*/
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_cell_reconfig_request_act,               /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_cell_start_request_act,                  /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_cell_stop_request_act,                   /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        /*Cell update start*/
        rrm_mif_fsm_cell_update_request_act,                 /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        /*Cell update end*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_cell_switch_on_request_act,              /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_cell_switch_off_request_act,             /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_rac_enable_disable_request_act,          /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_cell_resume_req_act,                      /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_block_cell_req_act,                      /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_unblock_cell_cmd_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY */
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_timer_expiry_handler_act,                /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        /* SPR 18055 Start */
        rrm_mif_fsm_cell_block_req_act_cell_block_ongoing,    /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        /* SPR 18055 End */
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /*MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP */
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                 /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_rac_enable_disable_resp_act,             /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        /* SPR 18055 Start */
        rrm_mif_fsm_impossible_event_act,                     /* MIF_FSM_EVENT_BLOCK_CELL_REQ */
        /* SPR 18055 End */
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_no_active_ue_ind_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /* "MIF_FSM_STATE_CELL_ALREADY_BLOCKED" */
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_cell_reconfig_request_act,               /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_cell_delete_request_act,                    /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        /* Fix for <SPR-5568> */
        rrm_mif_fsm_cell_start_request_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        /* Fix For <SPR-5568> */
        rrm_mif_fsm_cell_stop_request_act,                   /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_cell_shutdown_request_act,               /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        /* Fix for <SPR-5568> */
        rrm_mif_fsm_cell_reconfig_resp_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        /* Fix for <SPR-5568> */
		/* SPR 19043 FIX START */
        rrm_mif_fsm_cell_delete_resp_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
		/* SPR 19043 FIX END */
        /* Fix for <SPR-5568> */
        rrm_mif_fsm_cell_start_resp_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_cell_stop_resp_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        /* Fix for <SPR-5568> */
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        rrm_mif_fsm_cell_already_blocked_cell_block_req_act,    /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_unblock_cell_cmd_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_impossible_event_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                      /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    },
    /* "MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP" */
    {
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_REQUEST*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_REQ*/
        /* SPR-20656 START */
        rrm_mif_fsm_cell_stop_request_act,                   /*MIF_FSM_EVENT_CELL_STOP_REQ*/
        /* SPR-20656 END */
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SHUTDOWN_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_ON_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_SWITCH_OFF_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_CONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RECONFIG_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_DELETE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_START_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_STOP_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_UPDATE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_START_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_STOP_ADMISSION_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_ACTIVE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_EMERGENCY_CALL_DATA_TIMER_EXPIRY*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_REQ*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_RAC_ENABLE_DISABLE_RESP*/
        rrm_mif_fsm_impossible_event_act,                    /*MIF_FSM_EVENT_CELL_RESUME_REQ*/
        /* SPR 18055 Start */
        rrm_mif_fsm_cell_block_req_act_cell_block_ongoing,   /*MIF_FSM_EVENT_BLOCK_CELL_REQ*/
        /* SPR 18055 End */
        rrm_mif_fsm_non_emrgncy_active_calls_ho_resp_act,    /*MIF_FSM_EVENT_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP*/
        rrm_mif_fsm_impossible_event_act,                     /*MIF_FSM_EVENT_UNBLOCK_CELL_CMD*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_req_act,      /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ*/
        rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act,            /*MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_RESP*/
        rrm_mif_fsm_impossible_event_act,                      /*MIF_FSM_EVENT_NO_ACTIVE_UE_IND*/
        rrm_mif_fsm_cell_platform_ind_act,                     /*MIF_FSM_EVENT_CELL_PLATFORM_IND*/
        rrm_mif_fsm_cell_config_kpi_act,                       /*MIF_FSM_EVENT_CELL_CONFIG_KPI_REQ*/
        rrm_mif_fsm_cell_get_kpi_act,                          /*MIF_FSM_EVENT_CELL_GET_KPI_REQ*/
        rrm_mif_fsm_cell_config_kpi_resp_act,                  /*MIF_FSM_EVENT_CELL_CONFIG_KPI_RESP*/
        rrm_mif_fsm_cell_get_kpi_resp_act                      /*MIF_FSM_EVENT_CELL_GET_KPI_RESP*/
    }
};
/*ECN start*/
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                   Cell cONFIG rESPONSE event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_ecn_capacity_enhance_resp_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context            *p_mif_context,
 void            *p_msg
 )
{
    rrmcm_rmif_generic_resp_t  *p_in_cell_generic_resp = RRM_PNULL;
    /* coverity : CID 29707*/
    rrm_oam_cell_ecn_capacity_enhance_resp_t p_cell_ecn_capa_resp ;
    rrm_return_et ret_val = RRM_SUCCESS;
/*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
/*SPR 15993 Fix End*/

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    /* coverity : CID 29707*/
    RRM_MEMSET(&p_cell_ecn_capa_resp,RRM_ZERO,sizeof(rrm_oam_cell_ecn_capacity_enhance_resp_t));

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
            "MIF FSM: Initiated Cell Config Response procedure"
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    p_in_cell_generic_resp = (rrmcm_rmif_generic_resp_t *)p_msg;

/*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
                                    p_in_cell_generic_resp->transaction_id);
/*SPR 15993 Fix End*/
    RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    if (p_in_cell_generic_resp->response == RRM_FAILURE)
    {
        /*Storing the last fail cause received from cell*/
        ret_val = store_fail_cause_req_send_rcvd(p_in_cell_generic_resp->fail_cause, 
/*SPR 15993 Fix Start*/
                    p_mif_context, p_in_cell_generic_resp->transaction_id);
/*SPR 15993 Fix End*/
        /* Coverity ID 75541 Fix Start */
        if(ret_val != RRM_SUCCESS)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                    " Failed in store_fail_cause_req_send_rcvd ");
        }
        /* Coverity ID 75541 Fix End */
    }
    if (RRM_ZERO == p_gl_ctxt->bs_cellmap)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                "cell usage response rcvd for all cell");
        /* coverity  : CID 31025 */
        p_cell_ecn_capa_resp.fail_cause = (rrm_error_et)(fetch_fail_cause(p_gl_ctxt, 
/*SPR 15993 Fix Start*/
                                            p_in_cell_generic_resp->transaction_id));
/*SPR 15993 Fix End*/
        if (p_cell_ecn_capa_resp.fail_cause != RRM_NO_ERROR)
        {
            p_cell_ecn_capa_resp.response = RRM_FAILURE;
        }
        else
        {
            p_cell_ecn_capa_resp.response = RRM_SUCCESS;
            p_cell_ecn_capa_resp.fail_cause = RRM_NO_ERROR;

        }
        /*Send response to OAM */
/*SPR 15993 Fix Start*/
    if(RRM_OAM_MODULE_ID == src_id)
    {
/*SPR 15993 Fix End*/
        /* oam composer for radio congestion*/
        ret_val = rrm_oam_send_rrm_oam_cell_ecn_capacity_enhance_resp(
                (void *)(&p_cell_ecn_capa_resp),
                RRM_MIF_MODULE_ID,
                RRM_OAM_MODULE_ID,
                /*SPR 15993 Fix Start*/
                p_mif_context->currently_processed_api_trans_d,
                /*SPR 15993 Fix End*/
                p_in_cell_generic_resp->cellindex);

        if (RRM_SUCCESS == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "Send success for rrm_oam_send_rrm_oam_radio_usage_threshold_resp");
        }
        /*SPR 15993 Fix Start*/
    }
    /*SPR 15993 Fix End*/

        /*Spr 10869 fix start */
        /* De- allocated TCB */
        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                /*SPR 15993 Fix Start*/ 
                p_in_cell_generic_resp->transaction_id);
        /*SPR 15993 Fix End*/ 
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
        }
        /*Spr 10869 fix End */
    }

    RRM_UT_TRACE_EXIT();
}
/*ECN end*/


/****************************************************************************
 * Function Name  : rrm_mif_fsm_process_event
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. event_id       : Event received for MIF FSM.
 *              4. p_msg         : This could be null pointer or not null pointer.
 *              5. src_fsm        : Source FSM indicator.
 * Outputs        : 
 *                                 
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the entry point function for MIF FSM table.
 *                : No NULL check is done on p_msg as it is expected to be 
 *                : NULL or NON NULL.
 ****************************************************************************/
void rrm_mif_fsm_process_event(
        rrm_mif_gb_context_t     *p_gl_ctxt, 
        rrm_mif_context            *p_mif_context,
        U16                       event_id,
        void                     *p_msg,
        mif_fsm_type                 src_fsm
        )
{
    U8       current_state = MIF_FSM_INVALID_STATE;
    /* SPR 21496 Fix Start */
    /* Code Removed */
    /* SPR 21496 Fix End */

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);

    /* coverity : CID  16442*/
    rcvd_event_id = (rmif_event_et) event_id;
    current_state = RRM_MIF_FSM_GET_STATE(p_mif_context);
    RRM_ASSERT (current_state < MIF_FSM_NUM_OF_STATES);
    RRM_ASSERT (event_id < MIF_FSM_NUM_OF_EVENTS);
     
    (*rrm_mif_fsm_table[current_state][event_id])(p_gl_ctxt, p_mif_context, p_msg);

    /* SPR 21496 Fix Start */
    /* mif context is released by FSM hence should not be accessed */
    /* SPR 21496 Fix End */
    
    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_config_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when in Initial 
 *                  state Cell Config Setup Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_config_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_cell_config_req_t  *p_in_cell_config_req_from_oam = RRM_PNULL;
    rrmcm_rmif_cell_config_req_t    *p_out_cell_config_req_to_cellm = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Setup procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

    p_out_cell_config_req_to_cellm = (rrmcm_rmif_cell_config_req_t *)
        rrm_mem_get(sizeof(rrmcm_rmif_cell_config_req_t));
    if ( p_out_cell_config_req_to_cellm == RRM_PNULL)
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_out_cell_config_req_to_cellm failed" );
        RRM_UT_TRACE_EXIT();
        return ;
    }  
    RRM_MEMSET(p_out_cell_config_req_to_cellm,RRM_ZERO,sizeof(rrmcm_rmif_cell_config_req_t));
    
    /* BUG_11576_FIX_START */
    //code_deleted
    /* BUG_11576_FIX_END */
    p_in_cell_config_req_from_oam = (rrm_oam_cell_config_req_t *)p_msg; 
    build_and_send_cell_config_req_to_cellm(p_gl_ctxt, p_mif_context,
            p_in_cell_config_req_from_oam, 
            p_out_cell_config_req_to_cellm);

    RRM_MEM_FREE(p_out_cell_config_req_to_cellm);

    rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_INITIAL,
            MIF_FSM_STATE_W_FOR_CELL_CONFIG_RESP);

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_reconfig_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when in Initial 
 *                  state Cell ReConfig Setup Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_reconfig_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context          *p_mif_context,
 void                     *p_msg             
 )
{
    rrmcm_rmif_cell_reconfig_req_t  *p_out_cell_reconfig_req_to_cellm = RRM_PNULL;

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);

    RRM_UT_TRACE_ENTER();
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Reconfig procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

    p_out_cell_reconfig_req_to_cellm = (rrmcm_rmif_cell_reconfig_req_t *)
        rrm_mem_get(sizeof(rrmcm_rmif_cell_reconfig_req_t));
    if ( p_out_cell_reconfig_req_to_cellm == RRM_PNULL)
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_out_cell_reconfig_req_to_cellm failed" );
        RRM_UT_TRACE_EXIT();
        return ;
    }

    RRM_MEMSET(p_out_cell_reconfig_req_to_cellm,
            RRM_ZERO,
            sizeof(rrmcm_rmif_cell_reconfig_req_t));
    build_and_send_cell_reconfig_req_to_cellm(p_gl_ctxt, p_mif_context,
            (rrm_oam_cell_reconfig_req_t *)p_msg,
            p_out_cell_reconfig_req_to_cellm);

    RRM_MEM_FREE(p_out_cell_reconfig_req_to_cellm);

    /* Fix for <SPR-5568> */
    if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
    {
        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP);
    }
    /* Fix for <SPR-5568> */
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_delete_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when in Initial 
 *                  state Cell Delete Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_delete_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_cell_delete_req_t   out_cell_delete_req_to_cellm = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);

     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Delete procedure"  
            " for MIF ContextId [%d];Unused variable p_msg=%p", p_mif_context->mif_ctxt_id,p_msg);

    build_and_send_cell_del_req_to_cellm(p_mif_context,
            /*SPR 17777 +-*/
            /*SPR 21369 Start*/
            (rrm_oam_cell_delete_req_t *)p_msg,
            /*SPR 21369 Start*/
            &out_cell_delete_req_to_cellm);

    rrm_mif_fsm_set_state(p_mif_context,
            MIF_FSM_STATE_ACTIVE,
            MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP);
    if(RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state)
    {
        //set the bit
        RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_stop_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when in Initial 
 *                  state Cell Stop Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_stop_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_cell_stop_req_t    out_cell_stop_req_to_cellm = {RRM_ZERO};
    U16                                      src_id = RRM_ZERO;
    /*SPR 15993 Fix Start*/
    U16                                 tmp_trans_id = RRM_ZERO;
    /*SPR 15993 Fix End*/


    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);

    RRM_UT_TRACE_ENTER();

     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Stop procedure"  
            " for MIF ContextId [%d];Unused variable p_msg [%p]", p_mif_context->mif_ctxt_id,p_msg);
     /*SPR 17777 +-*/
    /*SPR 15993 Fix Start*/
    tmp_trans_id = p_mif_context->currently_processed_api_trans_d;
    src_id = return_src_id_from_tcb(p_mif_context,
            p_mif_context->currently_processed_api_trans_d);
    p_mif_context->currently_processed_api_trans_d = tmp_trans_id;
    /*SPR 15993 Fix End*/

    /*SPR_15766 FIX START*/
    p_gl_ctxt->cell_state = RRM_CELL_STOP_ONGOING;
    /*SPR_15766 FIX END*/
    
    build_and_send_cell_stop_req_to_cellm(p_mif_context,/*SPR 17777 +-*/
            &out_cell_stop_req_to_cellm);
    /* Fix for <SPR-5568> */
    if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
    {
        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                MIF_FSM_STATE_W_FOR_CELL_STOP_RESP);
    }
    /* Fix for <SPR-5568> */

    /* Fix for <SPR- 10869> */
    if(RRM_SON_NMM_MODULE_ID == src_id) 
    {
    /*STATIC ICIC START*/ 
    /* Set the sent bit for this cell in global context */
    RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    /*STATIC ICIC END*/
    }
    RRM_UT_TRACE_EXIT();
}

/*Cell update start*/
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_update_request_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when in Initial
 *                  state Cell Update Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_update_request_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrmcm_rmif_cell_update_req_t  *p_out_cell_update_req_to_cellm = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Update procedure"
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

    p_out_cell_update_req_to_cellm = (rrmcm_rmif_cell_update_req_t *)
        rrm_mem_get(sizeof(rrmcm_rmif_cell_update_req_t));
    if ( p_out_cell_update_req_to_cellm == RRM_PNULL)
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_out_cell_update_req_to_cellm failed;"
                "Unused variable p_gl_ctxt=%p",p_gl_ctxt);
        /*SPR 17777 +-*/
        RRM_UT_TRACE_EXIT();
        return ;
    }

    RRM_MEMSET(p_out_cell_update_req_to_cellm,
            RRM_ZERO,
            sizeof(rrmcm_rmif_cell_update_req_t));

    build_and_send_cell_update_req_to_cellm(p_mif_context,/*SPR 17777 +-*/
            (rrm_oam_cell_update_req_t *)p_msg,
            p_out_cell_update_req_to_cellm);

    RRM_MEM_FREE(p_out_cell_update_req_to_cellm);

    rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
            MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP);

    RRM_UT_TRACE_EXIT();
}
/*Cell update end*/

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_start_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when in Initial 
 *                  state Cell Start Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_start_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_cell_start_req_t    out_cell_start_req_to_cellm = {RRM_ZERO};
    U16                                      src_id = RRM_ZERO;
    /*SPR 15993 Fix Start*/
    U16                                 tmp_trans_id = RRM_ZERO;
    /*SPR 15993 Fix End*/


    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);

    RRM_UT_TRACE_ENTER();

     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Start procedure"  
            " for MIF ContextId [%d];Unused variable p_msg=%p", p_mif_context->mif_ctxt_id,p_msg);
     /*SPR 17777 +-*/
    /*SPR 15993 Fix Start*/
    tmp_trans_id = p_mif_context->currently_processed_api_trans_d;
    src_id = return_src_id_from_tcb(p_mif_context,
            p_mif_context->currently_processed_api_trans_d);
    p_mif_context->currently_processed_api_trans_d = tmp_trans_id;
    /*SPR 15993 Fix End*/

    /*SPR_15766 FIX START*/
    p_gl_ctxt->cell_state = RRM_CELL_START_ONGOING;
    /*SPR_15766 FIX END*/

    build_and_send_cell_start_req_to_cellm(p_mif_context,/*SPR 17777 +-*/
            &out_cell_start_req_to_cellm);
    /* Fix for <SPR-5568> */
    if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
    {
        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                MIF_FSM_STATE_W_FOR_CELL_START_RESP);
    }

    /* Fix for <SPR- 10869> */
    if(RRM_SON_NMM_MODULE_ID == src_id) 
    {
    /* Fix for <SPR-5568> */
    /*STATIC ICIC START*/ 
    /* Set the sent bit for this cell in global context */
    RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    /*STATIC ICIC END*/
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_shutdown_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Shutdown Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_shutdown_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_shutdown_req_t         *p_in_cell_shutdown_req_from_oam = RRM_PNULL;
    rrmcm_rmif_cell_stop_adm_req_t *p_out_cell_stop_admission_req_to_cellm = 
        RRM_PNULL;
    rrmcm_rmif_cell_delete_req_t   *p_out_cell_del_req_to_cellm = RRM_PNULL;

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();
    p_in_cell_shutdown_req_from_oam = (rrm_oam_shutdown_req_t *)p_msg;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Shutdown procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

    if (RRM_SHUTDOWN_MODE_FORCED == 
            p_in_cell_shutdown_req_from_oam->shutdown_mode)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Forceful Shutdown "
                "procedure");
        p_out_cell_del_req_to_cellm = (rrmcm_rmif_cell_delete_req_t *)
            rrm_mem_get(sizeof(rrmcm_rmif_cell_delete_req_t));
        if ( p_out_cell_del_req_to_cellm == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_out_cell_del_req_to_cellm failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        }

        build_and_send_cell_del_req_to_cellm(p_mif_context,/*SPR 17777 +-*/
                /*SPR 21369 Start*/
                (rrm_oam_cell_delete_req_t *)p_msg,
                /*SPR 21369 Start*/
                p_out_cell_del_req_to_cellm);

        RRM_MEM_FREE(p_out_cell_del_req_to_cellm);
        /* Fix for <SPR-5568> */
        if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
        {

            rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                    MIF_FSM_STATE_W_FOR_CELL_DELETE_RESP);
        }
        /* Fix for <SPR-5568> */
        /* Set the sent bit for this cell in global context */
        /*Coverity_fix_start_54936*/
        RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
        /*Coverity_fix_end_54936*/
    }
    else if (RRM_SHUTDOWN_MODE_GRACEFUL == 
            p_in_cell_shutdown_req_from_oam->shutdown_mode)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Graceful Shutdown "
                "procedure");
        p_out_cell_stop_admission_req_to_cellm = 
            (rrmcm_rmif_cell_stop_adm_req_t *)
            rrm_mem_get(sizeof(rrmcm_rmif_cell_stop_adm_req_t));
        if ( p_out_cell_stop_admission_req_to_cellm == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_out_cell_stop_admission_req_to_cellm failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        }

        build_and_send_cell_stop_adm_req_to_cellm(
                 p_mif_context,/*SPR 17777 +-*/
                p_out_cell_stop_admission_req_to_cellm);
        /* Fix for <SPR-5568> */
        p_mif_context->source_stop_adm |= MIF_SOURCE_SHUTDOWN_CELL;
        /* Fix for <SPR-5568> */
        RRM_MEM_FREE(p_out_cell_stop_admission_req_to_cellm);
        p_gl_ctxt->timer_val[MIF_TIME_TO_SHUTDOWN_TIMER] = 
            p_in_cell_shutdown_req_from_oam->time_to_shutdown;

        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP);

        /* Set the sent bit for this cell in global context */
        RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_switch_off_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Switch Off Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_switch_off_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_cell_stop_adm_req_t   *p_out_cell_stop_admission_req_to_cellm = 
        RRM_PNULL;
    rrm_son_cell_switch_off_req_t    *p_in_cell_switch_off_req_from_son = 
        RRM_PNULL;
    rrm_son_cell_switch_off_res_t     out_message_son;
    rrm_return_et                     ret_val = RRM_FAILURE;
    U16                                 src_id = RRM_ZERO;
    rrmuem_rmif_emergency_call_active_req_t *OutEmergCallActveReqToUem_p = RRM_PNULL;
/*SPR 15993 Fix Start*/
    U16                                 del_tcb_trans_id = RRM_ZERO;
/*SPR 15993 Fix End*/

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    rrm_memset_rrm_son_cell_switch_off_res(&out_message_son, RRM_ZERO);
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Switch Off procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    p_in_cell_switch_off_req_from_son = (rrm_son_cell_switch_off_req_t *)p_msg;

    if (RRM_SO_GRACEFUL == p_in_cell_switch_off_req_from_son->switchoff_type)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Graceful Request for Switch Off "
                "received");
        p_out_cell_stop_admission_req_to_cellm = 
            (rrmcm_rmif_cell_stop_adm_req_t *)
            rrm_mem_get(sizeof(rrmcm_rmif_cell_stop_adm_req_t));
        if ( p_out_cell_stop_admission_req_to_cellm == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_out_cell_stop_admission_req_to_cellm failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        }

        build_and_send_cell_stop_adm_req_to_cellm( p_mif_context,/*SPR 17777+-*/
                p_out_cell_stop_admission_req_to_cellm);
        /* Fix for <SPR-5568> */
        p_mif_context->source_stop_adm |= MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL;
        /* Fix for <SPR-5568> */
        RRM_MEM_FREE(p_out_cell_stop_admission_req_to_cellm);

        p_gl_ctxt->timer_val[MIF_TIME_TO_SHUTDOWN_TIMER] = 
            p_in_cell_switch_off_req_from_son->switchoff_time;
        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP);
    }
    else if (RRM_SO_FORCEFUL == 
            p_in_cell_switch_off_req_from_son->switchoff_type)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Forceful Request for Switch Off "
                "received");
        p_mif_context->source_stop_adm |= MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL;
        /*Send request for emergency call*/
        OutEmergCallActveReqToUem_p = (rrmuem_rmif_emergency_call_active_req_t *)
            rrm_mem_get(sizeof(rrmuem_rmif_emergency_call_active_req_t));
        if (OutEmergCallActveReqToUem_p == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to OutEmergCallActveReqToUem_p failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        }
/*SPR 15993 Fix Start*/
        build_and_send_emerg_call_active_req_to_uem(p_gl_ctxt, p_mif_context,
                OutEmergCallActveReqToUem_p,p_mif_context->currently_processed_api_trans_d);
/*SPR 15993 Fix End*/
        /*SPR 17777 +-*/
        rrm_mif_fsm_set_state(p_mif_context, 
                MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY,
                MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP);
        /* Coverity 54384 fix start */
        RRM_MEM_FREE(OutEmergCallActveReqToUem_p);
        /* Coverity 54384 fix end*/
    }
    else 
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: Incorrect Request for Switch Off "

                "received");
        /* Kshitij_start Coverity, CID: 11468 */
/*SPR 15993 Fix Start*/
       del_tcb_trans_id = p_mif_context->currently_processed_api_trans_d;
/*SPR 15993 Fix End*/
        if(RRM_ZERO == (src_id = return_src_id_from_tcb(p_mif_context, 
                        p_mif_context->currently_processed_api_trans_d)))
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                    "Msg return_src_id_from_tcb Failed");
        }           
        /*kshitij_end */

        out_message_son.cell_switchoff_res.result = RRM_FAILURE;
        /* coverity : CID 16439*/
        out_message_son.cell_switchoff_res.error_code = RRM_ERR_CELL_SW_OFF_FAILURE;
#ifdef RRM_UT_FLAG       
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F);
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F_cell_parsing);
#endif       

        /* COMPOSER send the message to SON */
        ret_val = rrm_son_send_rrm_son_cell_switch_off_res(
                (void *)&out_message_son,
                RRM_MIF_MODULE_ID,
                src_id,
                     p_mif_context->currently_processed_api_trans_d,
                     p_mif_context->cell_index); 
        /* De- allocated TCB */
        /* Coverity 76262 fix start */
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                    "rrm_son_send_rrm_son_cell_switch_off_res");
        }
        /* Coverity 76262 fix end */
        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                            del_tcb_trans_id);
/*SPR 15993 Fix End*/
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
        }
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_switch_on_request_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Shutdown Request event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_switch_on_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_cell_start_adm_req_t     
        *p_out_cell_start_adm_req_to_cellm = RRM_PNULL;

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Switch On procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

    p_out_cell_start_adm_req_to_cellm = (rrmcm_rmif_cell_start_adm_req_t *)
        rrm_mem_get(sizeof(rrmcm_rmif_cell_start_adm_req_t));
    if ( p_out_cell_start_adm_req_to_cellm == RRM_PNULL)
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_out_cell_start_adm_req_to_cellm failed;"
                "Unused variable p_gl_ctxt=%p p_msg=%p",p_gl_ctxt,p_msg);
        /*SPR 17777 +-*/
        RRM_UT_TRACE_EXIT();
        return ;
    }

    /* Setting the global Variable to differentiate between Cell Switch 
       On/OFF */

    p_mif_context->source_stop_adm |= MIF_SOURCE_SWITCHON_CELL;

    build_and_send_cell_start_adm_req_to_cellm(p_mif_context,
     /*SPR 17777 +-*/
            p_out_cell_start_adm_req_to_cellm);
    rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_W_FOR_SWITCH_ON,
            MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP);

    RRM_MEM_FREE(p_out_cell_start_adm_req_to_cellm);
    RRM_UT_TRACE_EXIT();
}


/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_config_resp_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell cONFIG rESPONSE event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_config_resp_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
)
{
    rrmcm_rmif_cell_config_resp  *p_in_cell_config_resp_from_cellm = RRM_PNULL;
    rrm_oam_cell_config_resp_t    out_generic_response_to_oam;
/*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
/*SPR 15993 Fix End*/

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    rrm_memset_rrm_oam_cell_config_resp(&out_generic_response_to_oam, RRM_ZERO);
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Config Response procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    p_in_cell_config_resp_from_cellm = (rrmcm_rmif_cell_config_resp *)p_msg;

/*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
                                    p_in_cell_config_resp_from_cellm->transaction_id);
    if(RRM_OAM_MODULE_ID == src_id)
    {
        /*SPR 15993 Fix End*/
    build_and_send_config_resp_to_oam(p_gl_ctxt, p_mif_context,
            p_in_cell_config_resp_from_cellm, 
            &out_generic_response_to_oam);
        /*SPR 15993 Fix Start*/             
    }                               
    /*SPR 15993 Fix End*/               
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_reconfig_resp_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Reconfig rESPONSE event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_reconfig_resp_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_cell_reconfig_resp_t     out_generic_response_to_oam;
    /* SPR_14127_start */
    //code deleted
    /* SPR_14127_stop */
    /*SPR 15993 Fix Start*/                                       
    U16                                 src_id = RRM_ZERO;    
    /*SPR 15993 Fix End*/                                         

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    rrm_memset_rrm_oam_cell_reconfig_resp(&out_generic_response_to_oam, RRM_ZERO);
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell ReConfig Response "
            "procedure for MIF ContextId [%d]",
            p_mif_context->mif_ctxt_id);

    /*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
            ((rrmcm_rmif_generic_resp_t *)p_msg)->transaction_id);
    if(RRM_OAM_MODULE_ID == src_id)
    {
        /*SPR 15993 Fix End*/
    build_and_send_reconfig_resp_to_oam(p_gl_ctxt, p_mif_context,
            (rrmcm_rmif_generic_resp_t *)p_msg,
            &out_generic_response_to_oam);
        /*SPR 15993 Fix Start*/ 
    }                   
    /*SPR 15993 Fix End*/   
    /* SPR_14127_start */
    //code deleted
    /* SPR_14127_stop */

    /* Fix for <SPR-5568> */
    if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
    {
        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_W_FOR_CELL_RECONFIG_RESP,
                MIF_FSM_STATE_ACTIVE);
    }
    /* Fix for <SPR-5568> */
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : free_cell_config_cfg_node
 * Inputs         : global_cell_id
 * Outputs        : None
 * Returns        : None
 * Description    : free the cell index
 *****************************************************************************/
/* SPR 20653 Fix Start */
/* Code Removed */
/* SPR 20653 Fix End */

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_delete_resp_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Delete Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_delete_resp_act
(                                       
 rrm_mif_gb_context_t *p_gl_ctxt, 
 rrm_mif_context      *p_mif_context,
 void                 *p_msg             
 )
{
    rrmcm_rmif_generic_resp_t       *p_in_cellm_generic_resp = RRM_PNULL;
    rrm_oam_delete_resp_t            out_generic_response_to_oam;
    rrm_oam_shutdown_resp_t          out_message_oam = {RRM_INDEX_ERR, RRM_NO_ERROR};
    rrm_return_et                    ret_val = RRM_FAILURE;
    /*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
    /*SPR 15993 Fix End*/

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    rrm_memset_rrm_oam_delete_resp(&out_generic_response_to_oam, RRM_ZERO);
    p_in_cellm_generic_resp = (rrmcm_rmif_generic_resp_t *) p_msg;

    /*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
            p_in_cellm_generic_resp->transaction_id);
    /*SPR 15993 Fix End*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Delete Response procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    /*SPR 15993 Fix Start*/
    if ((RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state) && (RRM_OAM_MODULE_ID == src_id))
        /*SPR 15993 Fix End*/
    {
        /* Clear the sent bit for this cell in global context */
        RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
        /* Irrespective of whether cell delete succeeds or fails at cellm, we
           always send a success towards oam. */
        /* Send response only when responses from all cells have been 
           received */
        if (RRM_ZERO == p_gl_ctxt->bs_cellmap)
        {
            out_message_oam.response = RRM_SUCCESS;
            out_message_oam.fail_cause = RRM_NO_ERROR;
#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_shutdown_resp_S);
#endif       
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",out_message_oam.fail_cause);
            /* COMPOSER send the message to OAM AGENT */
            ret_val = rrm_oam_send_rrm_oam_shutdown_resp(
                    (void *)&out_message_oam,
                    RRM_MIF_MODULE_ID,
                    RRM_OAM_MODULE_ID,
                    /*SPR 15993 Fix Start*/
                    p_mif_context->currently_processed_api_trans_d,
                    /*SPR 15993 Fix End*/
                    p_in_cellm_generic_resp->cellindex);
            if (RRM_SUCCESS == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, "Send Failed for " 
                        "rrm_oam_send_rrm_oam_shutdown_resp ");
            }

            /* Change MIF state to ACTIVE */
            p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
            /*Deallocate all tcb context for cell shutdown*/
            ret_val = de_allocate_all_tcb_from_mif_ctxt(p_gl_ctxt,
                    /*SPR 15993 Fix Start*/
                    p_in_cellm_generic_resp->transaction_id);
            /*SPR 15993 Fix End*/

            /* Coverity CID 75339 Start*/
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                        "MIF FSM: TCB De-Allocation Failed");
            }
            /* Coverity CID 75339 End*/
            /* SPR 20266 Fix Start */
            /* SPR 20653 Fix Start */
            /* Code Removed */
            /* SPR 20653 Fix End */
            /* SPR 20266 Fix End */
            /*De allocate all mif context*/
            rrm_dealloc_all_mif_context (p_gl_ctxt);
        }

    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                "Sending Cell Delete Response to OAM ");
        build_and_send_del_resp_to_oam(p_gl_ctxt, p_mif_context,
                p_in_cellm_generic_resp, 
                &out_generic_response_to_oam);

        /* De- allocated TCB */
        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                           p_in_cellm_generic_resp->transaction_id);
/*SPR 15993 Fix End*/
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                    "MIF FSM: TCB De-Allocation Failed");
        }
        /* SPR 20266 Fix Start */
        /* SPR 20653 Fix Start */
        /* Code Removed */
        /* SPR 20653 Fix End */
        /* SPR 20266 Fix End */
        rrm_dealloc_mif_context(p_mif_context, p_gl_ctxt);
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_start_resp_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Start Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_start_resp_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_cell_start_resp_t                out_generic_response_to_oam;
    U8                                       mif_ctxt_cnt = RRM_ZERO;
    U8                                       mif_ctxt_id = RRM_ZERO;
    rrm_return_et                            ret_val = RRM_FAILURE;
    U16                                      src_id = RRM_ZERO;
    rrmcm_rmif_generic_resp_t               *p_in_generic_response = RRM_PNULL;
    rrm_son_cell_switch_on_res_t             out_message_son;
    rrmcm_rmif_cell_stop_adm_req_t           out_cell_stop_adm_req_to_cellm = {RRM_ZERO};
    rrm_son_nmm_complete_res_t               out_msg_nmm_complete_resp_to_son = {{RRM_INDEX_ERR, RRM_NO_ERROR}};
    /* coverity : CID 16429*/
    rrm_error_et                             error_code = RRM_NO_ERROR; 

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    rrm_memset_rrm_oam_cell_start_resp(&out_generic_response_to_oam, RRM_ZERO);

    rrm_memset_rrm_son_cell_switch_on_res(&out_message_son, RRM_ZERO);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Start Response procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    p_in_generic_response = (rrmcm_rmif_generic_resp_t *)p_msg;

    src_id = return_src_id_from_tcb(p_mif_context,
            p_in_generic_response->transaction_id);
    ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
            p_in_generic_response->cellindex,
            &mif_ctxt_cnt,
            &mif_ctxt_id);
    /* Coverity 74112 Fix Start */
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off,
                p_rrm_mif_facility_name,
                RRM_ERROR,
                "Failed to  fetch mif ctxt for Cell"
                "Id[%d]",p_in_generic_response->cellindex);
        return;
    }
    /* Coverity 74112 Fix End */

    /*SPR_15766 FIX START*/
    p_gl_ctxt->cell_state = RRM_CELL_STARTED;
    /*SPR_15766 FIX END*/

    if(RRM_OAM_MODULE_ID == src_id)
    {
        build_and_send_start_resp_to_oam(p_gl_ctxt, p_mif_context,
                (rrmcm_rmif_generic_resp_t *)p_msg,
                &out_generic_response_to_oam);
        /* De- allocated TCB */
        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                           p_in_generic_response->transaction_id);
/*SPR 15993 Fix End*/
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
        }
        /* Fix for <SPR-5568> */
        if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
        {
            rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_W_FOR_CELL_START_RESP,
                    MIF_FSM_STATE_ACTIVE);
        }
        /* Fix for <SPR-5568> */
    }
    /*STATIC ICIC START*/ 
    else if(RRM_SON_NMM_MODULE_ID == src_id)
    {
        if (RRM_FAILURE == p_in_generic_response->response)
        {
            p_gl_ctxt->cell_resp_flag = RRM_FALSE;
            error_code = p_in_generic_response->fail_cause;
        }

        RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);

        if (RRM_ZERO == p_gl_ctxt->bs_cellmap)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                    "response recvd for all the cell");

            if (RRM_TRUE == p_gl_ctxt->cell_resp_flag) /*Kshitij 14173 rel1.3*/
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "sending nmm comp response success msg to nmm module"); 
                out_msg_nmm_complete_resp_to_son.complete_res.result = RRM_SUCCESS;
                out_msg_nmm_complete_resp_to_son.complete_res.error_code = RRM_NO_ERROR;
            }
            else
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "sending nmm comp response failure msg to nmm module"); 
                out_msg_nmm_complete_resp_to_son.complete_res.result = RRM_FAILURE;
                out_msg_nmm_complete_resp_to_son.complete_res.error_code = error_code;
            }

            ret_val = rrm_son_send_rrm_son_nmm_complete_res(
                    (void *)&out_msg_nmm_complete_resp_to_son,
                    RRM_MIF_MODULE_ID,
                    src_id,
		   p_mif_context->currently_processed_api_trans_d,
           p_mif_context->cell_index); 
            /* Coverity Fix 76054 Start */
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "Failure returned by function rrm_son_send_rrm_son_nmm_complete_res");
            }
            /* Coverity Fix 76054 End */

            /* De- allocated TCB */
            ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                    /*SPR 15993 Fix Start*/
                    p_in_generic_response->transaction_id);
            /*SPR 15993 Fix End*/
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                        "MIF FSM: TCB De-Allocation Failed");
            }

        }
        /*Change the state of cell*/
        rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_W_FOR_CELL_START_RESP,
                MIF_FSM_STATE_ACTIVE);
    } 
    /*STATIC ICIC END*/
    else if((RRM_SON_MODULE_ID == src_id) ||
            (RRM_SON_ANR_MODULE_ID == src_id) ||
            (RRM_SON_ES_MODULE_ID == src_id)) 
    {
        rrm_memcpy_rrm_oam_eutran_global_cell_id(
                &(out_message_son.cell_switchon_res.cell_id),
                &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));

        if (RRM_SUCCESS == p_in_generic_response->response)
        {
            out_message_son.cell_switchon_res.result = RRM_SUCCESS;
            out_message_son.cell_switchon_res.error_code = RRM_NO_ERROR;
        }
        else
        {
            out_message_son.cell_switchon_res.result = RRM_FAILURE;
            if(RRM_ERR_CELL_ALREADY_OPERATIONAL == p_in_generic_response->fail_cause)
            {
                out_message_son.cell_switchon_res.error_code = 
                    RRM_ERR_CELL_ALREADY_SWITCHED_ON;
            }
            else
            {
                /* coverity : CID 16438*/   
                out_message_son.cell_switchon_res.error_code = 
                    (rrm_error_et) p_in_generic_response->fail_cause;
            }
        }
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",
                out_message_son.cell_switchon_res.result);

        /* COMPOSER send the message to SON */
        ret_val = rrm_son_send_rrm_son_cell_switch_on_res(
                (void *)&out_message_son,
                RRM_MIF_MODULE_ID,
                src_id,
                     p_mif_context->currently_processed_api_trans_d,
                     p_mif_context->cell_index); 
        /* Coverity Fix 77639 Start */
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "Failure returned by function rrm_son_send_rrm_son_cell_switch_on_res");
        }
        /* Coverity Fix 77639 End */
        if (RRM_SUCCESS == p_in_generic_response->response)
        {
            /* De- allocated TCB */
            ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                      p_in_generic_response->transaction_id);
/*SPR 15993 Fix End*/
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, 
                        RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
            }
            /*response Sent To son reseti flag*/
            p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHON_CELL;

            rrm_mif_fsm_set_state(p_mif_context, 
                    MIF_FSM_STATE_W_FOR_CELL_START_RESP,
                    MIF_FSM_STATE_ACTIVE);
        }
        else
        {
            if (RRM_ERR_CELL_ALREADY_OPERATIONAL != p_in_generic_response->fail_cause)
            {
                /* In case of failure during  Cell Start send stop Admission to CellM */
                out_cell_stop_adm_req_to_cellm.transaction_id = 
/*SPR 15993 Fix Start*/
                      p_in_generic_response->transaction_id;
/*SPR 15993 Fix End*/
                out_cell_stop_adm_req_to_cellm.cellindex = p_mif_context->cell_index;
                /* Sending stop admission request to RRMCM*/
                rrm_mif_send_internal_msg(RRM_CM_MODULE_ID,
                        RRMCM_RMIF_CELL_STOP_ADMISSION_REQ,
                        sizeof(rrmcm_rmif_cell_stop_adm_req_t),
                        (void *)&out_cell_stop_adm_req_to_cellm);

                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_CELL_START_RESP,
                        MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP);
            }
            else
            {
                /*response Sent To son reseti flag*/
                p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHON_CELL;
                /* De- allocated TCB */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                      p_in_generic_response->transaction_id);
/*SPR 15993 Fix End*/
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, 
                            RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_CELL_START_RESP,
                        MIF_FSM_STATE_ACTIVE);
            }
        }
    }

    RRM_UT_TRACE_EXIT();
}

/* SPR 18055 Start */
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_block_req_act_cell_block_ongoing 
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                  cell block request received while cell 
 *                  block is already ongoing.
 ****************************************************************************/
static void 
rrm_mif_fsm_cell_block_req_act_cell_block_ongoing(
    rrm_mif_gb_context_t     *p_gl_ctxt,     
    rrm_mif_context          *p_mif_context,
    void                     *p_msg                   
    )
{
  rrm_oam_cell_block_resp_t                   block_cell_resp_to_oam;
  U8                                          mifCtxtCount = RRM_ZERO;
  rrm_return_et                               ret_val = RRM_SUCCESS;
  RRM_UT_TRACE_ENTER();

  RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, 
			"Unused variable p_msg=%p",p_msg);/*SPR 17777 +-*/
  for(mifCtxtCount = RRM_ZERO; mifCtxtCount < MAX_NUM_MIF_CTXT; mifCtxtCount++)
  {
    if ((RRM_FALSE != p_gl_ctxt->map_info[mifCtxtCount].is_info_valid) &&
        (p_mif_context->mif_ctxt_id == p_gl_ctxt->map_info[mifCtxtCount].mif_ctxt_id))
    {
      rrm_memcpy_rrm_oam_eutran_global_cell_id(
          &(block_cell_resp_to_oam.global_cell_id),
          &(p_gl_ctxt->map_info[mifCtxtCount].global_cell_id));
      break;
    }
  }
  block_cell_resp_to_oam.response = RRM_FAILURE;
  block_cell_resp_to_oam.fail_cause = RRM_ERR_CELL_BLOCK_INPROCESS;
  if (RRM_FAILURE == rrm_oam_send_rrm_oam_cell_block_resp (&block_cell_resp_to_oam,
        RRM_MIF_MODULE_ID, RRM_OAM_MODULE_ID,p_mif_context->currently_processed_api_trans_d,p_mif_context->cell_index))
  {
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
        "Failed to send RRM_OAM_CELL_BLOCK_RESP to OAM");
  }
  else
  {
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "Send Failed for "
        "rrm_oam_send_rrm_oam_block_cell_resp ");

    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
        p_mif_context->currently_processed_api_trans_d);
    if(RRM_FAILURE != ret_val)
    {
      RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
          "TCB De-Allocated successfully");
    }
  }
} 
/* SPR 18055 End */

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_stop_resp_act
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Stop Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_stop_resp_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_cell_stop_resp_t            out_generic_response_to_oam;
    U8                                  mif_ctxt_cnt = RRM_ZERO;
    U8                                  mif_ctxt_id = RRM_ZERO;
    rrm_return_et                       ret_val = RRM_FAILURE;
    U16                                 src_id = RRM_ZERO;
    rrm_son_cell_switch_off_res_t       out_message_son;
    rrm_son_nmm_prepare_res_t           out_son_prepare_resp = {{RRM_INDEX_ERR,  RRM_NO_ERROR}};
    rrmcm_rmif_generic_resp_t           *p_in_generic_response = RRM_PNULL;
    rrmcm_rmif_cell_start_adm_req_t     out_cell_start_adm_req_to_cellm = {RRM_ZERO};
    /* coverity : CID 19453*/
    rrm_error_et                        error_code = RRM_NO_ERROR;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    /*SPR_15766 FIX START*/
    p_gl_ctxt->cell_state = RRM_CELL_STOPPED;
    /*SPR_15766 FIX END*/

    rrm_memset_rrm_oam_cell_stop_resp(&out_generic_response_to_oam, RRM_ZERO);

    rrm_memset_rrm_son_cell_switch_off_res(&out_message_son, RRM_ZERO);

    p_in_generic_response = (rrmcm_rmif_generic_resp_t *)p_msg;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Stop Response procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

    src_id = return_src_id_from_tcb(p_mif_context, 
            p_in_generic_response->transaction_id);
    ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
            p_in_generic_response->cellindex,
            &mif_ctxt_cnt,
            &mif_ctxt_id);
    /* Coverity 74112 Fix Start */
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off,
                p_rrm_mif_facility_name,
                RRM_ERROR,
                "Failed to  fetch mif ctxt for Cell"
                "Id[%d]",p_in_generic_response->cellindex);
        return;
    }
    /* Coverity 74112 Fix End */
    if(RRM_OAM_MODULE_ID == src_id)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated Cell Stop Response "
                "procedure for OAM"); 
        build_and_send_stop_resp_to_oam(p_gl_ctxt, p_mif_context,
                (rrmcm_rmif_generic_resp_t *)p_msg,
                &out_generic_response_to_oam);

        /* De- allocated TCB */
        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                /*SPR 15993 Fix Start*/
                p_in_generic_response->transaction_id);
        /*SPR 15993 Fix End*/
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
        }
        /* Fix for <SPR-5568> */
        if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
        {
            rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_W_FOR_CELL_STOP_RESP,
                    MIF_FSM_STATE_ACTIVE);
        }
        /* Fix for <SPR-5568> */

    }
    /*STATIC ICIC START*/
    else if(RRM_SON_NMM_MODULE_ID == src_id)
    {
        if (RRM_FAILURE == p_in_generic_response->response)
        {
            /* coverity : CID 16437*/
            p_gl_ctxt->cell_resp_flag = RRM_FALSE;
            /* Coverity_ID : 54438 */
            error_code = p_in_generic_response->fail_cause;
#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_F);
#endif       
        }

        RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);

        if (RRM_ZERO == p_gl_ctxt->bs_cellmap)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "cell stop response rcvd for all cell");
            if (RRM_TRUE == p_gl_ctxt->cell_resp_flag) /*Kshitij 14174 rel1.3*/
            {  
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "sending nmm comp response success msg to nmm module"); 
                out_son_prepare_resp.prepare_res.result = RRM_SUCCESS;      
                out_son_prepare_resp.prepare_res.error_code = RRM_NO_ERROR;    
#ifdef RRM_UT_FLAG       
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_S);
#endif       
            }
            else
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "sending nmm comp response failure msg to nmm module");
                out_son_prepare_resp.prepare_res.result = RRM_FAILURE;
                out_son_prepare_resp.prepare_res.error_code = error_code;
#ifdef RRM_UT_FLAG       
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.nmm_prepare_resp_F);
#endif       
            }  

            /* COMPOSER send the message to SON */
            ret_val = rrm_son_send_rrm_son_nmm_prepare_res(
                    (void *)&out_son_prepare_resp,
                    RRM_MIF_MODULE_ID,
                    src_id,
		   p_mif_context->currently_processed_api_trans_d,
           p_mif_context->cell_index);
            /* Coverity Fix 77561 Start */
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                        "Failure returned by function rrm_son_send_rrm_son_nmm_prepare_res");
            }
            /* Coverity Fix 77561 End */

            /* De- allocated TCB */
            ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                      p_in_generic_response->transaction_id);
/*SPR 15993 Fix End*/
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
            }

            rrm_mif_fsm_set_state(p_mif_context,
                    MIF_FSM_STATE_W_FOR_CELL_STOP_RESP,
                    MIF_FSM_STATE_ACTIVE);
        }
    }
    /*STATIC ICIC END*/
    else if((RRM_SON_MODULE_ID == src_id) ||
            (RRM_SON_ANR_MODULE_ID == src_id) ||
            (RRM_SON_ES_MODULE_ID == src_id))
    {
        if((p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL) ||
                (p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL))
        {
            rrm_memcpy_rrm_oam_eutran_global_cell_id(
                    &(out_message_son.cell_switchoff_res.cell_id),
                    &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));

            if (RRM_SUCCESS == p_in_generic_response->response)
            {
                out_message_son.cell_switchoff_res.result = RRM_SUCCESS;
                out_message_son.cell_switchoff_res.error_code = RRM_NO_ERROR;
#ifdef RRM_UT_FLAG       
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_S);
#endif       
            }
            else
            {
                out_message_son.cell_switchoff_res.result = RRM_FAILURE;
                if(RRM_ERR_CELL_UNOPERATIONAL == p_in_generic_response->fail_cause)
                {
                    out_message_son.cell_switchoff_res.error_code = 
                        RRM_ERR_CELL_ALREADY_SWITCHED_OFF;
#ifdef RRM_UT_FLAG       
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F);
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F_already_switch_off);
#endif       
                }
                else
                {
                    out_message_son.cell_switchoff_res.error_code = 
                        (rrm_error_et) p_in_generic_response->fail_cause;
#ifdef RRM_UT_FLAG       
                    RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_off_resp_F);
#endif       
                }
            }
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",
                    out_message_son.cell_switchoff_res.result);

            /* COMPOSER send the message to SON */
            ret_val = rrm_son_send_rrm_son_cell_switch_off_res(
                    (void *)&out_message_son,
                    RRM_MIF_MODULE_ID,
                    src_id,
                    p_mif_context->currently_processed_api_trans_d,
                    p_mif_context->cell_index); 

            if (RRM_SUCCESS == p_in_generic_response->response)
            {
                /* De- allocated TCB */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                      p_in_generic_response->transaction_id);
/*SPR 15993 Fix End*/
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }

                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_CELL_STOP_RESP,
                        MIF_FSM_STATE_W_FOR_SWITCH_ON);

                /*unset the cell switch off ongoing*/
                if(p_mif_context->source_stop_adm & 
                        MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL)
                {
                    p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL;
                }
                if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL)
                {
                    p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL;
                }

            }
            else
            {
                if(p_mif_context->source_stop_adm & 
                        MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL)
                {
                    /* In case of failure during  Cell Stop send Start Admission 
                       Back to Cell M */
                    out_cell_start_adm_req_to_cellm.transaction_id = 
                        p_mif_context->currently_processed_api_trans_d;
                    out_cell_start_adm_req_to_cellm.cellindex = 
                        p_mif_context->cell_index ;

                    /* Sending stop admission request to RRMCM*/
                    rrm_mif_send_internal_msg(RRM_CM_MODULE_ID,
                            RRMCM_RMIF_CELL_START_ADMISSION_REQ,
                            sizeof(rrmcm_rmif_cell_start_adm_req_t),
                            (void *)&out_cell_start_adm_req_to_cellm);

                    rrm_mif_fsm_set_state(p_mif_context, 
                            MIF_FSM_STATE_W_FOR_CELL_STOP_RESP,
                            MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP);
                }
                else if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL)
                {
                    p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL;
                    rrm_mif_fsm_set_state(p_mif_context, 
                            MIF_FSM_STATE_W_FOR_CELL_STOP_RESP,
                            MIF_FSM_STATE_ACTIVE);
                }
            }
        }
    }
    RRM_UT_TRACE_EXIT();
}

/*Cell update start*/
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_update_resp_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                   Cell Update response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_update_resp_act(
        rrm_mif_gb_context_t    *p_gl_ctxt,
        rrm_mif_context         *p_mif_context,
        void                    *p_msg)
{
    rrm_oam_cell_update_resp_t     out_generic_response_to_oam;
    rrm_return_et                    ret_val = RRM_FAILURE;
/*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
/*SPR 15993 Fix End*/

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    RRM_MEMSET(&out_generic_response_to_oam, RRM_ZERO,
            sizeof(rrm_oam_cell_update_resp_t));

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Update Response "
            "procedure for MIF ContextId [%d]",
            p_mif_context->mif_ctxt_id);

/*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
                                    ((rrmcm_rmif_generic_resp_t *)p_msg)->transaction_id);
    if(RRM_OAM_MODULE_ID == src_id)
    {
/*SPR 15993 Fix End*/
    build_and_send_update_resp_to_oam(p_gl_ctxt, p_mif_context,
            (rrmcm_rmif_generic_resp_t *)p_msg,
            &out_generic_response_to_oam);

/*SPR 15993 Fix Start*/
    }
/*SPR 15993 Fix End*/
    /* De- allocated TCB */
    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
/*SPR 15993 Fix Start*/
                    ((rrmcm_rmif_generic_resp_t *)p_msg)->transaction_id);
/*SPR 15993 Fix End*/
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                "MIF FSM: TCB De-Allocation Failed");
    }
    rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_W_FOR_CELL_UPDATE_RESP,
            MIF_FSM_STATE_ACTIVE);

    RRM_UT_TRACE_EXIT();
}
/*Cell update end*/

/****************************************************************************
 * Function Name  :rrm_mif_fsm_impossible_event_act 
 * Inputs         : 
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        : 
 *                                  
 * Returns        : 
 *              1. void
 * Variables      : 
 * Description    : This function is the action routine when  
 *                   Cell Stop Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_impossible_event_act 
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    U8 state = RRM_ZERO;
    rrm_return_et ret_val = RRM_SUCCESS;
    /*SPR 15993 Fix Start*/
    U16                                 del_tcb_trans_id = RRM_ZERO;
    /*SPR 15993 Fix End*/


    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    state = RRM_MIF_FSM_GET_STATE(p_mif_context);
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, 
            "Unexpected Event:%s[%d],State:%s[%d]", 
            MIF_FSM_EVENT_TRACE[rcvd_event_id],rcvd_event_id, 
            MIF_FSM_STATE_TRACE[state],state); 
    /*SPR 15993 Fix Start*/
    del_tcb_trans_id = p_mif_context->currently_processed_api_trans_d;

    /* De- allocated TCB */
    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
            del_tcb_trans_id);
    /*SPR 15993 Fix End*/
    if(RRM_FAILURE == ret_val)
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed"
                ";Unused variable p_gl_ctxt=%p p_msg=%p",p_gl_ctxt,p_msg);
        /*SPR 17777 +-*/
    }
    RRM_UT_TRACE_EXIT();
}
/* BUG_11576_FIX_START */
//code_deleted
/* BUG_11576_FIX_END */
/****************************************************************************
 * Function Name  :rrm_mif_fsm_set_state 
 * Inputs         : 
 *              1. p_mif_context : ccUeContext Pointer.
 *              2. prev_ste : previous state of UEFSM.
 *              3. next_state : next State of UEFSM.
 * Outputs        : 
 * Returns        : void
 * Variables      : 
 * Description    : This function sets the new state 
 ****************************************************************************/
void rrm_mif_fsm_set_state(rrm_mif_context *p_mif_context, 
        U16 prev_state, 
        U16 next_state)
{
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_MIF_FSM_SET_STATE(p_mif_context, next_state);
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, 
            "Mif Fsm:State Changed From :%s[%d] To :%s[%d]", 
            MIF_FSM_STATE_TRACE[prev_state],prev_state,
            MIF_FSM_STATE_TRACE[next_state],next_state); 

}


/****************************************************************************
 * Function Name  : rrm_mif_fsm_stop_admission_resp_act 
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                   Cell Stop Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_stop_admission_resp_act 
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context            *p_mif_context,
 void            *p_msg
 )
{
    rrmcm_rmif_generic_resp_t          *p_generic_resp = RRM_PNULL;
    rrm_oam_shutdown_resp_t             out_message_oam = {RRM_INDEX_ERR, RRM_NO_ERROR};
    mif_timer_buf                       o_mif_timer_buf = {MIF_TIME_TO_SHUTDOWN_TIMER, RRM_ZERO, NULL_FSM, RRM_ZERO, RRM_ZERO};
    U8                                  mif_ctxt_cnt = RRM_ZERO;
    U8                                  mif_ctxt_id = RRM_ZERO;
    rrm_return_et                       ret_val = RRM_FAILURE;
    U16                                 src_id = RRM_ZERO;
    rrm_bool_et                         timer_start_flag = RRM_FALSE;
    rrm_oam_cell_block_resp_t           out_message_oam_for_block_cell;
    rrmuem_rmif_emergency_call_active_req_t  *OutEmergCallActveReqToUem_p = RRM_PNULL;
    rrm_bool_et                         block_cell_timer_start_flag = RRM_FALSE;

    RRM_UT_TRACE_ENTER();


    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Stop Admission Resp for Context Id [%d] "
            "and Cell Id[%d]", 
            p_mif_context->mif_ctxt_id, 
            p_mif_context->cell_index);

    p_generic_resp = (rrmcm_rmif_generic_resp_t *)p_msg;

    /* SPR-22532 START */
    src_id = return_src_id_from_tcb(p_mif_context,
            p_generic_resp->transaction_id);
    /* SPR-22532 END */
    /*SPR 17127 Start*/
    ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
            p_mif_context->cell_index,
            &mif_ctxt_cnt,
            &mif_ctxt_id);
    /*SPR 17127 End*/
    /* Coverity 74112 Fix Start */
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off,
                p_rrm_mif_facility_name,
                RRM_ERROR,
                "Failed to  fetch mif ctxt for Cell"
                "Id[%d]", p_mif_context->cell_index);
        return;
    }
    /* Coverity 74112 Fix End */
    if(RRM_OAM_MODULE_ID == src_id)
    {
        /* Response only expected when state is SHUTDOWN_ONGOING */
        if (RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Response: %d", p_generic_resp->response);
            /* Clear the sent bit for this cell in global context */
            RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);

            /* If it is a failure response then send failure to oam and change state */
            if (RRM_FAILURE == p_generic_resp->response)
            {
                ret_val = store_fail_cause_req_send_rcvd(p_generic_resp->fail_cause, 
                        /*SPR 15993 Fix Start*/
                        p_mif_context, p_generic_resp->transaction_id);
                /*SPR 15993 Fix End*/
            }      
            /*If all the response recieved from */
            if(RRM_ZERO == p_gl_ctxt->bs_cellmap)
            {
#ifdef RRM_UT_FLAG       
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.cell_shutdown_resp_F);
#endif       
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",out_message_oam.fail_cause);
                /* If failure is recieved from any of the cell send failure of Shutdown to OAM */
                /* Coverity_ID : 31023 */
                /*SPR 15993 Fix Start*/
                out_message_oam.fail_cause = (rrm_error_et)fetch_fail_cause(p_gl_ctxt, p_generic_resp->transaction_id);
                /*SPR 15993 Fix End*/
                if(RRM_NO_ERROR == out_message_oam.fail_cause)
                {
                    timer_start_flag = RRM_TRUE;
                    out_message_oam.response = RRM_SUCCESS;
                } 
                else
                {
                    timer_start_flag = RRM_FALSE;
                    out_message_oam.response = RRM_FAILURE;
                } 
                if(RRM_NO_ERROR != out_message_oam.fail_cause)
                {
                    /* COMPOSER send the message to OAM AGENT */
                    ret_val = rrm_oam_send_rrm_oam_shutdown_resp(
                            (void *)&out_message_oam,
                            RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            /*SPR 15993 Fix Start*/
                            p_mif_context->currently_processed_api_trans_d,
                            /*SPR 15993 Fix End*/
                            p_generic_resp->cellindex);
                    if (RRM_SUCCESS != ret_val)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                                "rrm_oam_send_rrm_oam_shutdown_resp ");
                    }
                    /*Unset the error for a particular trans_id*/
                    /*SPR 15993 Fix Start*/
                    ret_val = store_fail_cause_req_send_rcvd(RRM_NO_ERROR, p_mif_context,  p_generic_resp->transaction_id);
                    /*SPR 15993 Fix End*/
                    p_gl_ctxt->current_state = RMIF_STATE_ACTIVE; 
                }              
                ret_val = de_allocate_all_tcb_from_mif_ctxt(p_gl_ctxt,
                        /*SPR 15993 Fix Start*/
                        p_generic_resp->transaction_id);
                /*SPR 15993 Fix End*/
                /* Coverity ID 77886 Fix Start */
                if(ret_val != RRM_SUCCESS)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                /* Coverity ID 77886 Fix End */
            }                      
            /* Change MIF state to ACTIVE */
            rrm_mif_fsm_set_state(p_mif_context,
                    MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,
                    MIF_FSM_STATE_ACTIVE);
        }      
        else if (RMIF_STATE_ACTIVE == p_gl_ctxt->current_state)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Response: %d", p_generic_resp->response);

            /* If it is a failure response then send failure to oam and change state */
            if (RRM_FAILURE == p_generic_resp->response)
            {
                RRM_MEMCPY(&(out_message_oam_for_block_cell.global_cell_id),
                        &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id),
                        sizeof(rrm_oam_eutran_global_cell_id_t));
                out_message_oam_for_block_cell.response = RRM_FAILURE;
                /* coverity  : CID : 31024 */
                out_message_oam_for_block_cell.fail_cause = (rrm_error_et)p_generic_resp->fail_cause;
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",out_message_oam_for_block_cell.fail_cause);
                /* COMPOSER send the message to OAM AGENT */
                ret_val = rrm_oam_send_rrm_oam_cell_block_resp(
                        (void *)&out_message_oam_for_block_cell,
                        RRM_MIF_MODULE_ID,
                        RRM_OAM_MODULE_ID,
                        /*SPR 15993 Fix Start*/
                        p_mif_context->currently_processed_api_trans_d,
                        /*SPR 15993 Fix End*/
                        p_generic_resp->cellindex);
                if (RRM_SUCCESS != ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                            "rrm_oam_send_rrm_oam_block_cell_resp ");
                }
                /* BUG_11576_FIX_START */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                        /*SPR 15993 Fix Start*/
                        p_generic_resp->transaction_id);
                /*SPR 15993 Fix End*/
                if(RRM_FAILURE != ret_val)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_DETAILED,
                              "TCB De-Allocated successfully");
                }
                /* BUG_11576_FIX_END */

                /* Change MIF state to ACTIVE */
                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,
                        MIF_FSM_STATE_ACTIVE);
            }
            else
            {
                /* Stop admission success
                   Start the emergincy call timer after receiving responses from a cell */
                if (RRM_CELL_BLOCK_LOW_PRIORITY == p_mif_context->prev_block_cell_priority)
                {
                    block_cell_timer_start_flag = RRM_TRUE;
                }
                else
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name,
                            RRM_DETAILED, "MIF FSM: Timer Should not be started for High Priority Block Cell");

                    OutEmergCallActveReqToUem_p = (rrmuem_rmif_emergency_call_active_req_t *)
                        rrm_mem_get(sizeof(rrmuem_rmif_emergency_call_active_req_t));
                    if ( OutEmergCallActveReqToUem_p == RRM_PNULL)
                    {
                        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                                "Memory allocation to OutEmergCallActveReqToUem_p failed" );
                        RRM_UT_TRACE_EXIT();
                        return ;
                    }

                    build_and_send_emerg_call_active_req_to_uem(p_gl_ctxt, p_mif_context,
                            /*SPR 15993 Fix Start*/
                            OutEmergCallActveReqToUem_p, p_generic_resp->transaction_id);
                    /*SPR 15993 Fix End*/
                    rrm_mif_fsm_set_state(p_mif_context,
                            MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,
                            MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP);
                    RRM_MEM_FREE (OutEmergCallActveReqToUem_p);
                }
            }
        }
        else
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "MIF FSM: Unexpected message in state: %d",
                    p_gl_ctxt->current_state);
        }
    }
    else if((RRM_SON_MODULE_ID == src_id)||
            (RRM_SON_ANR_MODULE_ID == src_id) ||
            (RRM_SON_ES_MODULE_ID == src_id) ||
            (RRM_SON_NMM_MODULE_ID == src_id))
    {
        if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL)
        { 
            if(RRM_FAILURE == p_generic_resp->response)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, 
                        RRM_DETAILED, "MIF FSM: Stop Admission Response failure received for Cell Switch Off");
                build_and_send_non_registration_based_resp_to_son(p_gl_ctxt,
                        p_mif_context,
                        p_generic_resp,
                        RRM_SON_CELL_SWITCH_OFF_RESP);
                /* De- allocated TCB */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                        /*SPR 15993 Fix Start*/
                        p_generic_resp->transaction_id);
                /*SPR 15993 Fix End*/
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                /*unset the source bit*/
                p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL;
                timer_start_flag = RRM_FALSE;
                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,
                        MIF_FSM_STATE_ACTIVE);
            }
            else{
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Need to start emergency call for Cell Switch Off");
                timer_start_flag = RRM_TRUE;
            }
        }
        else if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL)
        {
            p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL;
        }
        else if (p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHON_CELL)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                    "MIF FSM: Stop admission response for Switch On");
            /*Unset the bit for switch on*/
            p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHON_CELL;
        }
    }
    if (RRM_TRUE == timer_start_flag)
    {
        o_mif_timer_buf.timer_type = MIF_TIME_TO_SHUTDOWN_TIMER;
        o_mif_timer_buf.mif_ctxt_id = p_mif_context->mif_ctxt_id;
        o_mif_timer_buf.fsm_index = MIF_FSM;
        o_mif_timer_buf.source = src_id;
        /*SPR_14763_changes_start*/
        o_mif_timer_buf.transaction_id = p_mif_context->currently_processed_api_trans_d;
        /*SPR_14763_changes_end*/


        RRM_ASSERT (MIF_TIME_TO_SHUTDOWN_TIMER <= MIF_MAX_NUM_TIMER_TYPE);
        mif_start_timer (MIF_TIME_TO_SHUTDOWN_TIMER,
                (void *)&o_mif_timer_buf, sizeof(mif_timer_buf), 
                p_gl_ctxt); 
        /*SPR 15993 Fix Start*/
        p_mif_context->timer_expiry_trans_id = p_generic_resp->transaction_id; 
        /*SPR 15993 Fix End*/
        rrm_mif_fsm_set_state(p_mif_context, 
                MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,
                MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY);
    }
    if (RRM_TRUE == block_cell_timer_start_flag)
    {
        o_mif_timer_buf.timer_type = MIF_BLOCK_CELL_TIMER;
        o_mif_timer_buf.mif_ctxt_id = p_mif_context->mif_ctxt_id;
        o_mif_timer_buf.fsm_index = MIF_FSM;
        o_mif_timer_buf.source = src_id;
        /*SPR_14763_changes_start*/
        o_mif_timer_buf.transaction_id = p_mif_context->currently_processed_api_trans_d;
        /*SPR_14763_changes_end*/


        RRM_ASSERT (MIF_BLOCK_CELL_TIMER <= MIF_MAX_NUM_TIMER_TYPE);
        mif_start_timer (MIF_BLOCK_CELL_TIMER,
                (void *)&o_mif_timer_buf, sizeof(mif_timer_buf),
                p_gl_ctxt);

        /*SPR 15993 Fix Start*/
        p_mif_context->timer_expiry_trans_id = p_generic_resp->transaction_id; 
        /*SPR 15993 Fix End*/
        rrm_mif_fsm_set_state(p_mif_context,
                MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP,
                MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY);
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_start_admission_resp_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                   Cell Stop Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_start_admission_resp_act 
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context            *p_mif_context,
 void            *p_msg
 )
{
    rrmcm_rmif_generic_resp_t          *p_generic_resp = RRM_PNULL;
    rrm_bool_et                         dealloc_flag = RRM_FALSE;
    rrmcm_rmif_cell_start_req_t         out_cell_start_req_to_cellm = {RRM_ZERO};
    rrm_oam_resume_service_resp_t       out_resume_resp_to_oam      = {RRM_INDEX_ERR, RRM_NO_ERROR};
    rrm_return_et                       ret_val = RRM_FAILURE;
    /*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
    /*SPR 15993 Fix End*/
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Start Admission Resp for Context Id [%d]"
            " and Cell Id[%d]", 
            p_mif_context->mif_ctxt_id, 
            p_mif_context->cell_index);

    p_generic_resp = (rrmcm_rmif_generic_resp_t *)p_msg;
    /*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
            ((rrmcm_rmif_generic_resp_t *)p_msg)->transaction_id);
    if ((RMIF_STATE_RESUME_ONGOING == p_gl_ctxt->current_state) && (RRM_OAM_MODULE_ID == src_id))
        /*SPR 15993 Fix End*/
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                "MIF FSM: Start Admission Response received during RESUME");

        /* Clear the sent bit for this cell in global context */
        RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);

        if (RRM_FAILURE == p_generic_resp->response)
        {
#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.resume_srvc_resp_F);
#endif       
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: Start Admission Failed. "
                    "Sending Response to OAM");
            out_resume_resp_to_oam.response = RRM_FAILURE;
            /* coverity : CID 16436*/
            out_resume_resp_to_oam.fail_cause = (rrm_error_et) p_generic_resp->fail_cause;
            ret_val = rrm_oam_send_rrm_oam_resume_service_resp(
                    (void *)&out_resume_resp_to_oam,
                    RRM_MIF_MODULE_ID,
                            RRM_OAM_MODULE_ID,
                            p_mif_context->currently_processed_api_trans_d,
                            p_mif_context->cell_index);
            if (RRM_SUCCESS == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "Send Failed for rrm_oam_send_rrm_oam_"
                        "resume_service_resp");
            }

            /* Change MIF state to ACTIVE */
            p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
        }
        else
        {
            /* If response has been received from all cells then send response
               to oam */
            if (RRM_ZERO == p_gl_ctxt->bs_cellmap)
            {
#ifdef RRM_UT_FLAG       
                RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.resume_srvc_resp_S);
#endif       
                out_resume_resp_to_oam.response = RRM_SUCCESS;
                out_resume_resp_to_oam.fail_cause = RRM_NO_ERROR;

                ret_val = rrm_oam_send_rrm_oam_resume_service_resp(
                        (void *)&out_resume_resp_to_oam,
                        RRM_MIF_MODULE_ID,
                                RRM_OAM_MODULE_ID,
                                p_mif_context->currently_processed_api_trans_d,
                                p_mif_context->cell_index);
                if (RRM_SUCCESS == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for rrm_oam_send_rrm_"
                            "oam_resume_service_resp");
                }

                /* Change MIF state to ACTIVE */
                p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
            }
        }
    }
    else if (p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHON_CELL)
    {

        if(RRM_FAILURE == p_generic_resp->response)
        {
#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_F);
#endif       
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "MIF FSM: Start Admission Response Failure "
                    "received for Cell Switch On");
            build_and_send_non_registration_based_resp_to_son(p_gl_ctxt,
                    p_mif_context,
                    p_generic_resp,
                    RRM_SON_CELL_SWITCH_ON_RESP);
            /*Unsetting the flag of cell switch on is ongoing*/
            p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHON_CELL;
            dealloc_flag = RRM_TRUE;
        }
        else if(RRM_SUCCESS == p_generic_resp->response)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Start Procedure Cell Start for "
                    "Cell Switch On");
            /*SPR 15993 Fix Start*/
            out_cell_start_req_to_cellm.transaction_id = 
                ((rrmcm_rmif_generic_resp_t *)p_msg)->transaction_id;
            /*SPR 15993 Fix End*/
            out_cell_start_req_to_cellm.cellindex = p_mif_context->cell_index;

#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmSonDebugStats.cell_switch_on_resp_S);
#endif       
            /* Sending start request to RRMCM*/
            rrm_mif_send_internal_msg(RRM_CM_MODULE_ID,
                    RRMCM_RMIF_CELL_START_REQ,
                    sizeof(rrmcm_rmif_cell_start_req_t),
                    (void *)&out_cell_start_req_to_cellm);
            dealloc_flag = RRM_FALSE;

            rrm_mif_fsm_set_state(p_mif_context, 
                    MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP,
                    MIF_FSM_STATE_W_FOR_CELL_START_RESP);
        }
    }
    else if (p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                "MIF FSM: Stop Admission Response[%d] for Cell Switch Off", p_generic_resp->response);
        dealloc_flag = RRM_TRUE;
        p_mif_context->source_stop_adm &= ~MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL;
    }
    /* Fix for <SPR-5568> */
    else if ((MIF_SOURCE_BLOCK_CELL & p_mif_context->source_stop_adm) &&
            (RMIF_STATE_ACTIVE == p_gl_ctxt->current_state))
    {
        if (RRM_SUCCESS == p_generic_resp->response)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "MIF FSM: Start Admission Success Response for UnBlock Cell Ind");
            /* Fix for <SPR-5568> */
            p_mif_context->source_stop_adm &=  (~MIF_SOURCE_BLOCK_CELL);
            /* Fix for <SPR-5568> */
        }
        else
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
                    "MIF FSM: Start Admission Failure Response for UnBlock Cell Ind");

        dealloc_flag = RRM_TRUE;
    }
    else if (((RRM_SUCCESS == p_generic_resp->response) ||
                (RRM_FAILURE == p_generic_resp->response))) 
    {
        if (RRM_SUCCESS == p_generic_resp->response)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Stop Admission Success Response for Cell Switch Off");
        }
        else
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "MIF FSM: Stop Admission Failure Response for Cell Switch Off");
        }
        dealloc_flag = RRM_TRUE;
    }

    if (RRM_TRUE == dealloc_flag)
    {
        /* De- allocated TCB */
        ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                /*SPR 15993 Fix Start*/
                ((rrmcm_rmif_generic_resp_t *)p_msg)->transaction_id);
        /*SPR 15993 Fix End*/
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
        }

        rrm_mif_fsm_set_state(p_mif_context, 
                MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP,
                MIF_FSM_STATE_ACTIVE);
    }
    RRM_UT_TRACE_EXIT();
}

/**
 * @name    rrm_mif_rrmcm_init_ind_handler
 * @brief   This is the handler function for RRMCM_MIF_INIT_IND msg
 * @return  None
 */
void rrm_mif_rrmcm_init_ind_handler
(
 rrm_mif_gb_context_t   *p_gl_ctxt
     /*SPR 17777 +-*/
 )
{
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

    p_gl_ctxt->init_ind |= RMIF_RRMCM_INIT_RECD ;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, " Bitmask for Init Ind = %d", p_gl_ctxt->init_ind);
    rrm_check_for_rmif_init_state(p_gl_ctxt);
    RRM_UT_TRACE_EXIT();
}

/**
 * @name    rrm_mif_rrmcm_init_config_resp_handler
 * @brief   This is the handler function for RMIF_RRMCM_INIT_CONFIG_RESP_RECD msg
 * @return  None
 */
void rrm_mif_rrmcm_init_config_resp_handler
(
 rrm_mif_gb_context_t   *p_gl_ctxt,
 void                    *p_msg
 )
{
    rrmcm_rmif_generic_non_cell_specific_api_resp_t  *p_cm_init_config_resp = 
        RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

    p_cm_init_config_resp = (rrmcm_rmif_generic_non_cell_specific_api_resp_t *)
        p_msg;

    if (RRM_SUCCESS == p_cm_init_config_resp->response)
    {  
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Success received in Init Config Resp");
        p_gl_ctxt->init_config_ind |= RMIF_RRMCM_INIT_CONFIG_SUCCESS_RESP_RECD;
    }
    else
    {  
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "Failure(%d) received in Init Config Resp",
                p_cm_init_config_resp->fail_cause);
        p_gl_ctxt->init_config_ind |= RMIF_RRMCM_INIT_CONFIG_FAILURE_RESP_RECD;
    }
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, " Bitmask for Init Config Response = %d", 
            p_gl_ctxt->init_config_ind);
    rrm_check_for_rmif_active_state(p_gl_ctxt,
            p_cm_init_config_resp->transaction_id);
    RRM_UT_TRACE_EXIT();
}

/**
 * @name    rrm_mif_rrmuem_init_config_resp_handler
 * @brief   This is the handler function for RMIF_RRMUEM_INIT_CONFIG_RESP_RECD
 msg
 * @return  None
 */
void rrm_mif_rrmuem_init_config_resp_handler
(
 rrm_mif_gb_context_t   *p_gl_ctxt,
 void                    *p_msg
 )
{
    rrmuem_rmif_init_config_res_t     *p_uem_init_config_resp = RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

    p_uem_init_config_resp = (rrmuem_rmif_init_config_res_t *)p_msg;

    if (RRM_TRUE == p_uem_init_config_resp->response) /*Kshitij 14175 rel1.3*/
    {  
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Success received in Init Config Resp");
        p_gl_ctxt->init_config_ind |= RMIF_RRMUEM_INIT_CONFIG_SUCCESS_RESP_RECD;
    }
    else
    {  
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "Failure received in Init Config Resp");
        p_gl_ctxt->init_config_ind |= RMIF_RRMUEM_INIT_CONFIG_FAILURE_RESP_RECD;
    }

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, " Bitmask for Init Config Response = %d", 
            p_gl_ctxt->init_config_ind);
    rrm_check_for_rmif_active_state(p_gl_ctxt, 
            p_uem_init_config_resp->transaction_id);
    RRM_UT_TRACE_EXIT();
}

/**
 * @name    rrm_mif_rrmuem_init_ind_handler
 * @brief   This is the handler function for RRMUEM_RMIF_INIT_IND msg
 * @return  None
 */
void rrm_mif_rrmuem_init_ind_handler
(
 rrm_mif_gb_context_t   *p_rmif_gb_context
     /*SPR 17777 +-*/
 )
{
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT(RRM_PNULL != p_rmif_gb_context);

    p_rmif_gb_context->init_ind |= RMIF_RRMUEM_INIT_RECD ;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, " Bitmask for Init Ind = %d", 
            p_rmif_gb_context->init_ind);
    rrm_check_for_rmif_init_state(p_rmif_gb_context);
    RRM_UT_TRACE_EXIT();
}


/**
 * @name    rrm_check_for_rmif_init_state
 * @brief   This function checks preconditions for switching FSM to
 *          RMIF_STATE_INIT state and perform state's switching
 *          if it necessary
 * @return  None
 */
void rrm_check_for_rmif_init_state
(
 rrm_mif_gb_context_t*  p_gl_ctxt
 )
{
    QTIMER                       mif_timer_id = RRM_PNULL;
    mif_timer_buf                o_mif_timer_buf = {MIF_TIME_TO_SHUTDOWN_TIMER, RRM_ZERO, 
        NULL_FSM, RRM_ZERO, RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT( RRM_PNULL != p_gl_ctxt );
    /* Check if init_indication messages received from all RRM modules */
    if ((p_gl_ctxt->init_ind & RMIF_RRMCM_INIT_RECD) &&
            (p_gl_ctxt->init_ind & RMIF_RRMUEM_INIT_RECD))
    {

        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Received Init from both CELLM and UEM");

        build_and_send_init_ind_to_oam();

        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "OAM Init Sent Successfully");

#ifdef RRM_UT_FLAG       
        /*
         ** Initialize the data memory segment in the write mode for the performance counters
         */
        if((gp_rrmStats = rrm_stat_shm_init(W_MODE)) == NULL)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                    "Error in initializing data space to shared memory segment\n");
            RRM_ASSERT(RRM_PNULL != gp_rrmStats);
        }

        RRM_MEMSET(&(gp_rrmStats->rrmSonDebugStats),RRM_ZERO,sizeof(rrmSonDebugStats_t));
        RRM_MEMSET(&(gp_rrmStats->rrmOamDebugStats),RRM_ZERO,sizeof(rrmOamDebugStats_t));
        RRM_MEMSET(&(gp_rrmStats->rrmRrcDebugStats),RRM_ZERO,sizeof(rrmRrcDebugStats_t));
#endif       

        /*Reset current state's flags*/
        p_gl_ctxt->init_ind = RRM_ZERO;

        /*Change state*/
        p_gl_ctxt->current_state = RMIF_STATE_INIT;

        /* Start the INIT_IND timer to periodically send INIT_IND's*/

        o_mif_timer_buf.timer_type = MIF_INIT_INDICATION_TIMER;

        RRM_ASSERT (MIF_INIT_INDICATION_TIMER <= MIF_MAX_NUM_TIMER_TYPE);
        mif_timer_id = mif_start_timer(MIF_INIT_INDICATION_TIMER,
                (void *)&o_mif_timer_buf, 
                sizeof(mif_timer_buf),
                p_gl_ctxt);

        /* Save the timer id in global context */
        p_gl_ctxt->timer_id[MIF_INIT_INDICATION_TIMER] = mif_timer_id;
    }
    RRM_UT_TRACE_EXIT();
}


void rrm_check_for_rmif_active_state
(
 rrm_mif_gb_context_t*  p_gl_ctxt,
 U16     trans_id
 )
{
    rrm_oam_init_config_resp_t  out_message   = {RRM_INDEX_ERR, RRM_NO_ERROR};
    rrm_return_et               ret_val = RRM_INDEX_ERR;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT( RRM_PNULL != p_gl_ctxt );
    /* Check if init config response messages received from all RRM modules */
    if (
            (p_gl_ctxt->init_config_ind & RMIF_RRMCM_INIT_CONFIG_SUCCESS_RESP_RECD)
            &&
            (p_gl_ctxt->init_config_ind & RMIF_RRMUEM_INIT_CONFIG_SUCCESS_RESP_RECD)
       )
    {

        /* Initialise Active Pool Index for MIF Contexts */
        ret_val = rrm_mif_create_pool(p_gl_ctxt);
        if(RRM_SUCCESS == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "rrm_mif_create_pool: Memory Pool Allocated SUCC");
            out_message.response = RRM_SUCCESS;
            out_message.fail_cause = RRM_NO_ERROR;
#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_resp_S);
#endif       
        }
        else
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "rrm_mif_create_pool: Memory Pool Allocated FAIL");
            out_message.response = RRM_FAILURE;
            out_message.fail_cause = RRM_ERR_SYS_MEM_ALLOC_FAILURE;
#ifdef RRM_UT_FLAG       
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_resp_F);
            RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_resp_F_sys_mem_alloc);
#endif       
        }


        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",out_message.response);

        /*Reset current state's flags*/
        p_gl_ctxt->init_config_ind= RRM_ZERO;
        /*Change state*/
        p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
        /* COMPOSER send the message to OAM AGENT */
        ret_val = rrm_oam_send_rrm_oam_init_config_resp(
                (void *)&out_message,
                RRM_MIF_MODULE_ID,
        RRM_OAM_MODULE_ID,
        trans_id,
        RRM_ZERO);
        if (RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "rrm_oam_send_rrm_oam_init_config_resp failed.");
        }
    }
    else if ( ((p_gl_ctxt->init_config_ind & 
                    RMIF_RRMCM_INIT_CONFIG_FAILURE_RESP_RECD) &&
                (p_gl_ctxt->init_config_ind & 
                 RMIF_RRMUEM_INIT_CONFIG_FAILURE_RESP_RECD)) ||
            ((p_gl_ctxt->init_config_ind & 
              RMIF_RRMCM_INIT_CONFIG_FAILURE_RESP_RECD) &&
             (p_gl_ctxt->init_config_ind & 
              RMIF_RRMUEM_INIT_CONFIG_SUCCESS_RESP_RECD)) ||
            ((p_gl_ctxt->init_config_ind & 
              RMIF_RRMCM_INIT_CONFIG_SUCCESS_RESP_RECD) &&
             (p_gl_ctxt->init_config_ind & 
              RMIF_RRMUEM_INIT_CONFIG_FAILURE_RESP_RECD))
            )
    {
        out_message.response = RRM_FAILURE;
        out_message.fail_cause = RRM_ERR_INIT_CONFIG_FAILURE;
#ifdef RRM_UT_FLAG       
        RRM_INCR_MSG_LOG_COUNTER(gp_rrmStats->rrmOamDebugStats.init_config_resp_F);
#endif       
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING, "Response[%d] ",out_message.response);

        /*Reset current state's flags*/
        p_gl_ctxt->init_config_ind= RRM_ZERO;
        /*Change state back to INIT*/
        p_gl_ctxt->current_state = RMIF_STATE_INIT;
        /* COMPOSER send the message to OAM AGENT */
        ret_val = rrm_oam_send_rrm_oam_init_config_resp(
                (void *)&out_message,
                RRM_MIF_MODULE_ID,
                                                RRM_OAM_MODULE_ID,
                                                trans_id,
                                                RRM_ZERO);
        /* Coverity ID 78639 Fix Start*/
        if (RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "rrm_oam_send_rrm_oam_init_config_resp failed.");
        }
        /* Coverity ID 78639 Fix End*/
    }


    RRM_UT_TRACE_EXIT();

}

/****************************************************************************
 * Function Name    : rrm_mif_fsm_timer_expiry_handler_act 
 * Inputs           :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3.p_msg         : it's is a null pointer 
 * Outputs          :
 * Returns          : void 
 * Variables        : None
 * Description      : This function will  
 *                      
 ****************************************************************************/
static void rrm_mif_fsm_timer_expiry_handler_act(
        rrm_mif_gb_context_t     *p_gl_ctxt, 
        rrm_mif_context            *p_mif_context,
        void                    *p_msg
        )
{
    rrmuem_rmif_emergency_call_active_req_t  *OutEmergCallActveReqToUem_p = 
        RRM_PNULL;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);

    OutEmergCallActveReqToUem_p = (rrmuem_rmif_emergency_call_active_req_t *)
        rrm_mem_get(sizeof(rrmuem_rmif_emergency_call_active_req_t));
    if ( OutEmergCallActveReqToUem_p == RRM_PNULL)
    {
     /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to OutEmergCallActveReqToUem_p failed"
                ";Unused variable p_msg=%p",p_msg);
     /*SPR 17777 +-*/
        RRM_UT_TRACE_EXIT();
        return ;
    }
    build_and_send_emerg_call_active_req_to_uem(p_gl_ctxt, p_mif_context,
                         OutEmergCallActveReqToUem_p,
                         /*SPR 15993 Fix Start*/
                         p_mif_context->timer_expiry_trans_id);
                         /*SPR 15993 Fix End*/
    rrm_mif_fsm_set_state(p_mif_context, 
            MIF_FSM_STATE_W_FOR_SHUTDOWN_TIMER_EXPIRY,
            MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP);
    /*Need to set the bit if shutdown is ongoing*/
    if(RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state)
    {
        /* Set the sent bit for this cell in global context */
        RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    }

    /* COVERITY : RESOURCE LEAK : CID : 54386 fix start */
    if( RRM_NULL != OutEmergCallActveReqToUem_p )
        RRM_MEM_FREE(OutEmergCallActveReqToUem_p);
    /* COVERITY : RESOURCE LEAK : CID : 54386 fix end*/
    RRM_UT_TRACE_EXIT();

}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_emerg_call_active_resp_act 
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  Emergency Call Active Response event is received.
 ****************************************************************************/
static void rrm_mif_fsm_emerg_call_active_resp_act 
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context            *p_mif_context,
 void            *p_msg
 )
{
    rrmuem_rmif_emergency_call_active_res_t *InEmerCallActiveResp_p = RRM_PNULL;
    rrm_son_cell_switch_off_res_t            out_message_son;
    rrm_oam_shutdown_resp_t                  out_message_oam = {RRM_INDEX_ERR, RRM_NO_ERROR};
    rrm_oam_cell_block_resp_t                out_message_oam_for_block_cell;
    U8                                       mif_ctxt_cnt = RRM_ZERO;
    U8                                       mif_ctxt_id = RRM_ZERO;
    rrm_return_et                            ret_val = RRM_FAILURE;
    U16                                      src_id = RRM_ZERO;
    rrmcm_rmif_cell_start_adm_req_t     
        *p_out_cell_start_adm_req_to_cellm = RRM_PNULL;
    U8										 num_of_active_calls = RRM_ZERO;


    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
    RRM_ASSERT (RRM_PNULL != p_msg);

    rrm_memset_rrm_oam_cell_block_resp(&out_message_oam_for_block_cell, RRM_ZERO);

    InEmerCallActiveResp_p = (rrmuem_rmif_emergency_call_active_res_t *)p_msg;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Emergency Call Active Response procedure"
            " for Cell Id [%d]", InEmerCallActiveResp_p->cell_index);
    if(RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state)
    {
        /* Clear the sent bit for this cell in global context */
        RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    }
    if ( InEmerCallActiveResp_p->emergency_call_ongoing )
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Emergency Calls Running");
        src_id = return_src_id_from_tcb(p_mif_context, 
                InEmerCallActiveResp_p->transaction_id);
        ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
                InEmerCallActiveResp_p->cell_index,
                &mif_ctxt_cnt,
                &mif_ctxt_id);
        /* Coverity 74112 Fix Start */
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off,
                    p_rrm_mif_facility_name,
                    RRM_ERROR,
                    "Failed to  fetch mif ctxt for Cell"
                    "Id[%d]",InEmerCallActiveResp_p->cell_index);
            return;
        }
        /* Coverity 74112 Fix End */
        if(RRM_OAM_MODULE_ID == src_id && 
                RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state)
        {
            ret_val = store_fail_cause_req_send_rcvd(RRM_ERR_EMERGENCY_CALL_ONGOING, 
                    p_mif_context, 
                    /*SPR 15993 Fix Start*/
                    InEmerCallActiveResp_p->transaction_id);
            /*SPR 15993 Fix End*/
            /* Coverity ID 74370 Fix Start */
            if(ret_val != RRM_SUCCESS)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                        " Failed in store_fail_cause_req_send_rcvd ");
            }
            /* Coverity ID 74370 Fix End */

            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",out_message_oam.fail_cause);
            if(RRM_ZERO == p_gl_ctxt->bs_cellmap)
            {
                /*This is a failure scenario*/
                out_message_oam.response = RRM_FAILURE;
                /* Coverity_ID : 31023 */
                /*SPR 15993 Fix Start*/
                out_message_oam.fail_cause = (rrm_error_et)fetch_fail_cause(p_gl_ctxt, InEmerCallActiveResp_p->transaction_id);
                /*SPR 15993 Fix End*/
                /* COMPOSER send the message to OAM AGENT */
                if(RRM_SUCCESS != (ret_val = rrm_oam_send_rrm_oam_shutdown_resp(
                                (void *)&out_message_oam,
                                RRM_MIF_MODULE_ID,
                                RRM_OAM_MODULE_ID,
                                /*SPR 15993 Fix Start*/
                                p_mif_context->currently_processed_api_trans_d,
                                /*SPR 15993 Fix End*/
                                InEmerCallActiveResp_p->cell_index)))
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                            "Send Failed for rrm_oam_send_rrm_oam_shutdown_resp ");

                }
                /*SPR 15993 Fix Start*/
                ret_val = store_fail_cause_req_send_rcvd(RRM_NO_ERROR, p_mif_context,InEmerCallActiveResp_p->transaction_id ); 
                /*SPR 15993 Fix End*/

                /* Coverity ID 74370 Fix Start */
                if(ret_val != RRM_SUCCESS)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                            " Failed in store_fail_cause_req_send_rcvd ");
                }
                /* Coverity ID 74370 Fix End */
                /* De- allocated TCB */
                ret_val = de_allocate_all_tcb_from_mif_ctxt(p_gl_ctxt,
                        /*SPR 15993 Fix Start*/
                        InEmerCallActiveResp_p->transaction_id);
                /*SPR 15993 Fix End*/
                /* Coverity ID 74370 Fix Start */
                if(ret_val != RRM_SUCCESS)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                /* Coverity ID 74370 Fix End */
                /* Change MIF state to ACTIVE */
                p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
            }
            /* Fix for <SPR-5568> */
            if ( MIF_FSM_STATE_CELL_ALREADY_BLOCKED != RRM_MIF_FSM_GET_STATE (p_mif_context))
            {
                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_ACTIVE);
            }
            /* Fix for <SPR-5568> */
        }
        else if (RRM_OAM_MODULE_ID == src_id &&
                RMIF_STATE_ACTIVE == p_gl_ctxt->current_state)
        {
            rrm_memcpy_rrm_oam_eutran_global_cell_id(
                    &(out_message_oam_for_block_cell.global_cell_id),
                    &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));
            out_message_oam_for_block_cell.response = RRM_FAILURE;
            out_message_oam_for_block_cell.fail_cause = RRM_ERR_EMERGENCY_CALL_ONGOING;

            /* Clear the sent bit for this cell in global context */
            //RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);

            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",out_message_oam_for_block_cell.fail_cause);

            /* COMPOSER send the message to OAM AGENT */

            /* Kshitij start CID */
            if(RRM_SUCCESS != rrm_oam_send_rrm_oam_cell_block_resp(
                        (void *)&out_message_oam_for_block_cell,
                        RRM_MIF_MODULE_ID,
                        RRM_OAM_MODULE_ID,
                        /*SPR 15993 Fix Start*/
                        p_mif_context->currently_processed_api_trans_d,
                        /*SPR 15993 Fix End*/
                        InEmerCallActiveResp_p->cell_index))
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                        "Send Failed for rrm_oam_send_rrm_oam_block_cell_resp ");
                ret_val = RRM_FAILURE;
            }
            /* Kshitij end */
            /* SPR-20656 START */
            /* send active_calls_ho_req to L3 via cell_mgr*/
            /* let below code be as failure needs to be sent to OAM */
            /* set flag for emergency call as emergency call exists */
            num_of_active_calls = rrm_get_non_emergengy_active_call (p_mif_context->cell_index);
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Number of Active Calls = %d ", num_of_active_calls);
            if (RRM_ZERO != num_of_active_calls)
            {
                build_and_send_non_emrgncy_active_calls_ho_req (
                        p_mif_context->cell_index,
                        InEmerCallActiveResp_p->transaction_id
                        );

                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP);
            }
            else
            {
		/*De-allocate TCB if no non-emergency UE exists*/
            	/* De- allocated TCB */
            	ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                    /*SPR 15993 Fix Start*/
                    InEmerCallActiveResp_p->transaction_id);
            	/*SPR 15993 Fix End*/
            	if(RRM_FAILURE == ret_val)
            	{
                	RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
            	}
                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_CELL_ALREADY_BLOCKED);
                p_mif_context->block_cell_ongoing = RRM_FALSE;
            }

            p_mif_context->emergcy_call_ongoing = RRM_TRUE;
            /* Change MIF state to ACTIVE */

            /* SPR-20656 END */
        }
        else if((RRM_SON_MODULE_ID == src_id) ||
                (RRM_SON_ANR_MODULE_ID == src_id) ||
                (RRM_SON_ES_MODULE_ID == src_id) ||
                (RRM_SON_NMM_MODULE_ID == src_id))
        {
            if((p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL) ||
                    (p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL))
            {
                rrm_memcpy_rrm_oam_eutran_global_cell_id(
                        &(out_message_son.cell_switchoff_res.cell_id),
                        &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));

                out_message_son.cell_switchoff_res.result = RRM_FAILURE;
                out_message_son.cell_switchoff_res.error_code = RRM_ERR_EMERGENCY_CALL_ONGOING;

                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",
                        out_message_son.cell_switchoff_res.error_code);

                /* COMPOSER send the message to SON */
                ret_val = rrm_son_send_rrm_son_cell_switch_off_res(
                        (void *)&out_message_son,
                        RRM_MIF_MODULE_ID,
                        src_id,
                        /*SPR 15993 Fix Start*/
                        p_mif_context->currently_processed_api_trans_d, 
                        /*SPR 15993 Fix End*/
                        InEmerCallActiveResp_p->cell_index); 
                /* Coverity 76262 fix start */
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                            "rrm_son_send_rrm_son_cell_switch_off_res");
                }
                /* Coverity 76262 fix end */

                /* SPR: 15922 Fix Start */
                /*Code removed*/
                /* SPR: 15922 Fix End */

                if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL)
                {
                    /* SPR: 15922 Fix Start */
                    p_out_cell_start_adm_req_to_cellm = (rrmcm_rmif_cell_start_adm_req_t *)
                        rrm_mem_get(sizeof(rrmcm_rmif_cell_start_adm_req_t));
                    RRM_ASSERT (RRM_PNULL != p_out_cell_start_adm_req_to_cellm);
                    /* SPR: 15922 Fix End */
                    build_and_send_cell_start_adm_req_to_cellm( p_mif_context,
                    /*SPR 17777 +-*/
                            p_out_cell_start_adm_req_to_cellm);
                    rrm_mif_fsm_set_state(p_mif_context, 
                            MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                            MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP);
                }
                if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL)
                {
                    rrm_mif_fsm_set_state(p_mif_context,
                            MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                            MIF_FSM_STATE_ACTIVE);
                }
                /* COVERITY : RESOURCE LEAK : CID : 54385 fix start */
                /*COV_fix_63563_start*/
                RRM_MEM_FREE(p_out_cell_start_adm_req_to_cellm);
                /*COV_fix_63563_end*/
                /* COVERITY : RESOURCE LEAK : CID : 54385 fix end*/
            }
        }
    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Emergency Calls Not Running");

        src_id = return_src_id_from_tcb(p_mif_context, 
                InEmerCallActiveResp_p->transaction_id);
        ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
                InEmerCallActiveResp_p->cell_index,
                &mif_ctxt_cnt,
                &mif_ctxt_id);
        /* Coverity 74112 Fix Start */
        if(RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off,
                    p_rrm_mif_facility_name,
                    RRM_ERROR,
                    "Failed to  fetch mif ctxt for Cell"
                    "Id[%d]",InEmerCallActiveResp_p->cell_index);
            return;
        }
        /* Coverity 74112 Fix End */
        if(RRM_OAM_MODULE_ID == src_id && 
                RMIF_STATE_SHUTDOWN_ONGOING == p_gl_ctxt->current_state)
        {

            if(RRM_ZERO == p_gl_ctxt->bs_cellmap)
            {
                /* coverity  : CID 31023 */
                /*SPR 15993 Fix Start*/
                out_message_oam.fail_cause = (rrm_error_et)fetch_fail_cause(p_gl_ctxt, InEmerCallActiveResp_p->transaction_id);
                /*SPR 15993 Fix End*/
                if(RRM_NO_ERROR != out_message_oam.fail_cause)
                {
                    /*This is a failure scenario*/
                    out_message_oam.response = RRM_FAILURE;
                    /* COMPOSER send the message to OAM AGENT */
                    if(RRM_SUCCESS != (ret_val = rrm_oam_send_rrm_oam_shutdown_resp(
                                    (void *)&out_message_oam,
                                    RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    /*SPR 15993 Fix Start*/
                                    p_mif_context->currently_processed_api_trans_d,
                                    /*SPR 15993 Fix End*/
                                    InEmerCallActiveResp_p->cell_index)))
                    {
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                                "Send Failed for rrm_oam_send_rrm_oam_shutdown_resp ");

                    }
                    /*SPR 15993 Fix Start*/
                    ret_val = store_fail_cause_req_send_rcvd(RRM_NO_ERROR, p_mif_context,InEmerCallActiveResp_p->transaction_id); 
                    /*SPR 15993 Fix End*/
                    /* Coverity ID 74370 Fix Start */
                    if(ret_val != RRM_SUCCESS)
                    {
                        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                                " Failed in store_fail_cause_req_send_rcvd ");
                    }
                    /* Coverity ID 74370 Fix End */
                    p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
                }
                /* De- allocated TCB */
                if(RRM_FAILURE == (ret_val = de_allocate_all_tcb_from_mif_ctxt(p_gl_ctxt,
                                /*SPR 15993 Fix Start*/
                                InEmerCallActiveResp_p->transaction_id)))
                    /*SPR 15993 Fix End*/
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                /*State of all cell need to set*/
                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_ACTIVE);
            }
        }
        else if (RRM_OAM_MODULE_ID == src_id &&
                RMIF_STATE_ACTIVE == p_gl_ctxt->current_state)
        {
            rrm_memcpy_rrm_oam_eutran_global_cell_id(
                    &(out_message_oam_for_block_cell.global_cell_id),
                    &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));
            num_of_active_calls = rrm_get_non_emergengy_active_call (p_mif_context->cell_index);
            /* SPR-20656 START */
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Number of Active Calls = %d ", num_of_active_calls);
            /* SPR-20656 END */
            if (RRM_ZERO == num_of_active_calls)
            {
                out_message_oam_for_block_cell.response = RRM_SUCCESS;
                out_message_oam_for_block_cell.fail_cause = RRM_NO_ERROR;


                /* COMPOSER send the message to OAM AGENT */
                ret_val = rrm_oam_send_rrm_oam_cell_block_resp(
                        (void *)&out_message_oam_for_block_cell,
                        RRM_MIF_MODULE_ID,
                        RRM_OAM_MODULE_ID,
                        /*SPR 15993 Fix Start*/
                        p_mif_context->currently_processed_api_trans_d,
                        /*SPR 15993 Fix End*/
                        InEmerCallActiveResp_p->cell_index);
                if (RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                            "rrm_oam_send_rrm_oam_block_cell_resp ");
                }
                /* De- allocated TCB */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                        /*SPR 15993 Fix Start*/
                        InEmerCallActiveResp_p->transaction_id);
                /*SPR 15993 Fix End*/
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_CELL_ALREADY_BLOCKED);
                p_mif_context->block_cell_ongoing = RRM_FALSE;
            }
            else
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Sending HO_REQ to UEM for Non-emergency active calls");

                build_and_send_non_emrgncy_active_calls_ho_req (
                        p_mif_context->cell_index,
                        InEmerCallActiveResp_p->transaction_id
                        );

                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP);
            }
            /* Change MIF state to ACTIVE */
            p_gl_ctxt->current_state = RMIF_STATE_ACTIVE;
        }
        else if((RRM_SON_MODULE_ID == src_id) ||
                (RRM_SON_ANR_MODULE_ID == src_id) ||
                (RRM_SON_ES_MODULE_ID == src_id) ||
                (RRM_SON_NMM_MODULE_ID == src_id))
        {
            /*send HO req to normal ongoing calls*/

            build_and_send_non_emrgncy_active_calls_ho_req (
                    p_mif_context->cell_index,
                    InEmerCallActiveResp_p->transaction_id
                    );

            rrm_mif_fsm_set_state(p_mif_context,
                    MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                    MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP);
        }
    }
    RRM_UT_TRACE_EXIT();
}


/**
 * @name    rrm_send_internal_msg
 * @brief   This function sends message to RRM internal modules
 * @return  None
 */
rrm_return_et rrm_mif_send_internal_msg
(
 rrm_module_id_t   dst_module_id,  /** Destination module identifier */
 U16               api_id,         /** API Id */
 /* SPR 20653 Fix start */
 U32               msg_size,       /** Size of message (payload) */
 /* SPR 20653 Fix End */
 void              *p_msg          /** Message specific info (payload) */
 )
{
    U8 *p_api          = RRM_PNULL;
    /* SPR 20653 Fix start */
    U32 msg_api_length = (msg_size + RRM_API_HEADER_SIZE);
    /* SPR 20653 Fix start */

    RRM_UT_TRACE_ENTER();

    /* Allocate buffer */
    if (RRMCM_RMIF_UPDATED_NRT_INFO_FROM_ANR_REQ == api_id)
    {
        p_api = rrm_msg_mem_get(sizeof(rrmcm_rmif_updated_nrt_info_t) + RRM_API_HEADER_SIZE);
    }
    else
    {	
        p_api = rrm_msg_mem_get(msg_api_length);
    }

    if (RRM_PNULL == p_api)
    {
        RRM_UT_TRACE_EXIT();
        return RRM_FAILURE;
    }

    RRM_MEMSET(p_api, RRM_ZERO, msg_api_length);
    rrm_construct_api_header(p_api, RRM_VERSION_ID, RRM_MIF_MODULE_ID,
#ifdef LTE_EMBMS_SUPPORTED
		    dst_module_id, api_id, (U16)msg_api_length);
#else
    dst_module_id, api_id, msg_api_length);
#endif

    if (RRMCM_RMIF_UPDATED_NRT_INFO_FROM_ANR_REQ == api_id)
    {
        RRM_MEMCPY(p_api + RRM_API_HEADER_SIZE, p_msg, sizeof(rrmcm_rmif_updated_nrt_info_t));
    }
    else
    {
        RRM_MEMCPY(p_api + RRM_API_HEADER_SIZE, p_msg, msg_size);
    }

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, " Sending API(%d), Size(%d), To(%d)", api_id, 
            msg_size, dst_module_id);

    /* Send message to */
    rrm_send_message(p_api, dst_module_id);

    RRM_UT_TRACE_EXIT();

    return RRM_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_rac_enable_disable_request_act
 * Inputs         : 
 *                  1. p_gl_ctxt     : is not null pointer.
 *                  2. p_mif_context 
 *                  3. p_msg         : is not null pointer.
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is invoked when MIF receives an
 *                  RAC enable disable request from OAMH.
 ****************************************************************************/
static void rrm_mif_fsm_rac_enable_disable_request_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context          *p_mif_context,
 void                     *p_msg             
 )
{
    rrm_oam_rac_enable_disable_req_t    *p_in_rac_enable_disable_req_msg 
        = RRM_PNULL;
    rrmcm_rmif_rac_enable_disable_req_t *p_out_rac_enable_disable_req_to_cellm 
        = RRM_PNULL;

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_msg);

    RRM_UT_TRACE_ENTER();

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated RAC Enable Disable Request "
            "procedure");  

    p_in_rac_enable_disable_req_msg = (rrm_oam_rac_enable_disable_req_t *)p_msg;

    /* Allocate memory for internal message to be sent to CELLM. */
    p_out_rac_enable_disable_req_to_cellm = 
        (rrmcm_rmif_rac_enable_disable_req_t *)
        rrm_mem_get(sizeof(rrmcm_rmif_rac_enable_disable_req_t));
    if ( p_out_rac_enable_disable_req_to_cellm == RRM_PNULL)
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_out_rac_enable_disable_req_to_cellm failed" );
        RRM_UT_TRACE_EXIT();
        return ;
    }
    /* Construct and send the internal message to CELLM. */
    build_and_send_rac_enable_disable_req_to_cellm(p_mif_context,/*SPR 17777 +-*/
            p_in_rac_enable_disable_req_msg,
            p_out_rac_enable_disable_req_to_cellm);

    RRM_MEM_FREE(p_out_rac_enable_disable_req_to_cellm);

    if (RRM_FALSE == (RMIF_GLOBAL_CELL_ID_PRESENT(p_in_rac_enable_disable_req_msg)))
    {
        /* Set the sent bit for this cell in global context */
        RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    }

    /* Change state to MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP */

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Changing state from: MIF_FSM_STATE_ACTIVE "
            "to: MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP");

    rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
            MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP);

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_rac_enable_disable_resp_act
 * Inputs         : 
 *                  1. p_gl_ctxt     : is not null pointer.
 *                  2. p_mif_context 
 *                  3. p_msg         : is not null pointer.
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is invoked when MIF receives an
 *                  RAC enable disable response from UEM.
 ****************************************************************************/
static void rrm_mif_fsm_rac_enable_disable_resp_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context          *p_mif_context,
 void                     *p_msg             
 )
{
    rrm_bool_et  send_response = RRM_FALSE;
    /*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
    /*SPR 15993 Fix End*/

    rrmcm_rmif_rac_enable_disable_res_t  *p_in_rac_enable_disable_resp_from_cellm 
        = RRM_PNULL;
    rrm_oam_rac_enable_disable_resp_t    *p_out_rac_enable_disable_resp_to_oam  
        = RRM_PNULL;
    /* BUG_11576_FIX_START */
    rrm_return_et               ret_val = RRM_INDEX_ERR;
    /* BUG_11576_FIX_END */

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_msg);

    RRM_UT_TRACE_ENTER();

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated RAC Enable Disable Response "
            "procedure");  

    p_in_rac_enable_disable_resp_from_cellm = 
        (rrmcm_rmif_rac_enable_disable_res_t *)p_msg;

    /* If the request was sent to multiple cells then we must wait for all
       cells to return success before send a success to OAM. However, if
       any of the cells return failure, we immediately send the failure
       to OAM. */

    /* Clear the sent bit for this cell in global context */
    /* SPR 10811 start */
    RMIF_CLEAR_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    /* SPR 10811 end */

    /* SPR 10811 start */
    if (RRM_TRUE == (RMIF_GLOBAL_CELL_ID_PRESENT(p_in_rac_enable_disable_resp_from_cellm)))
    {
        /* SPR 10811 end */

        /* Send a response only if 
           No response have already been sent to OAM
           AND
           This is a failure response from cellm
           OR
           All cells have responded with success. */
        if (RRM_FALSE == p_gl_ctxt->rac_response_sent_to_oam)
        {
            if ((RRM_FAILURE == p_in_rac_enable_disable_resp_from_cellm->response) ||
                    (RRM_ZERO == p_gl_ctxt->bs_cellmap))
            {
                send_response = RRM_TRUE;
            }
        }
    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: As there was no Global Cell ID present "
                "in RAC enable disable request from OAM so sending response to OAM");  
        /* SPR 10811 start */
        if (RRM_ZERO == p_gl_ctxt->bs_cellmap)
        {
            send_response = RRM_TRUE;
        }
        /* SPR 10811 end */
    }
    if (RRM_TRUE == send_response)
    {
        /* Allocate memory for internal message to be sent to UEM. */
        p_out_rac_enable_disable_resp_to_oam = (rrm_oam_rac_enable_disable_resp_t *)
            rrm_mem_get(sizeof(rrm_oam_rac_enable_disable_resp_t));
        if ( p_out_rac_enable_disable_resp_to_oam == RRM_PNULL)
        {
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_out_rac_enable_disable_resp_to_oam failed" );
            RRM_UT_TRACE_EXIT();
            return ;
        } 
        /* Construct and send the message to OAM. */
        /*SPR 15993 Fix Start*/
        src_id = return_src_id_from_tcb(p_mif_context, 
                p_in_rac_enable_disable_resp_from_cellm->transaction_id);
        if(RRM_OAM_MODULE_ID == src_id)
        {
            /*SPR 15993 Fix End*/
            build_and_send_rac_enable_disable_resp_to_oam(p_gl_ctxt, p_mif_context,
                    p_in_rac_enable_disable_resp_from_cellm,
                    p_out_rac_enable_disable_resp_to_oam);
            p_gl_ctxt->rac_response_sent_to_oam = RRM_TRUE;
            /*SPR 15993 Fix Start*/
        }
        /*CID 77638 Start*/
        RRM_MEM_FREE(p_out_rac_enable_disable_resp_to_oam);
        /*CID 77638 Stop*/
        /*SPR 15993 Fix End*/
    }
    /* Change state to MIF_FSM_STATE_ACTIVE */

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Changing state from: "
            "MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP "
            "to: MIF_FSM_STATE_ACTIVE");
    rrm_mif_fsm_set_state(p_mif_context, 
            MIF_FSM_STATE_W_FOR_RAC_ENABLE_DISABLE_RESP,
            MIF_FSM_STATE_ACTIVE);
    /* BUG_11576_FIX_START */
    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, 
            /*SPR 15993 Fix Start*/
            p_in_rac_enable_disable_resp_from_cellm->transaction_id);
    /*SPR 15993 Fix End*/
    if(RRM_FAILURE != ret_val)
    {
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
                "TCB De-Allocated successfully");
    }
    /* BUG_11576_FIX_END */

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_resume_req_act
 * Inputs         : 
 *                  1. p_gl_ctxt     : is not null pointer.
 *                  2. p_mif_context 
 *                  3. p_msg         : is not null pointer.
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is invoked when MIF receives a
 *                  CELL RESUME request from OAMH.
 ****************************************************************************/
static void rrm_mif_fsm_cell_resume_req_act
(                                       
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context          *p_mif_context,
 void                     *p_msg             
 )
{
    rrmcm_rmif_cell_start_adm_req_t   *p_out_start_adm_to_cellm = RRM_PNULL;
    rrm_son_cell_switch_on_req_t      dummy_object; 

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);

    RRM_UT_TRACE_ENTER();

    rrm_memset_rrm_son_cell_switch_on_req(&dummy_object, RRM_ZERO);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated CELL Resume Request procedure");  


    /* Allocate memory for internal message to be sent to CELLM. */
    p_out_start_adm_to_cellm = (rrmcm_rmif_cell_start_adm_req_t *)
        rrm_mem_get(sizeof(rrmcm_rmif_cell_start_adm_req_t));
    if ( p_out_start_adm_to_cellm == RRM_PNULL)
    {
        /*SPR 17777 +-*/
        RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                "Memory allocation to p_out_start_adm_to_cellm failed; Unused variable p_msg=%p",p_msg );
        /*SPR 17777 +-*/
        RRM_UT_TRACE_EXIT();
        return ;
    }
    /* Construct and send the internal message to CELLM. */
    build_and_send_cell_start_adm_req_to_cellm(p_mif_context,
            p_out_start_adm_to_cellm);

    RRM_MEM_FREE(p_out_start_adm_to_cellm);

    /* Set the sent bit for this cell in global context */
    RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);

    /* Change state to MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP */

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, "MIF FSM: Changing state from: MIF_FSM_STATE_ACTIVE "
            "to: MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP");
    rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
            MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP);

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_block_cell_req_act
 * Inputs         :
 *                  1. p_gl_ctxt     : is not null pointer.
 *                  2. p_mif_context
 *                  3. p_msg         : is not null pointer.
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is invoked when MIF receives a
 *                  BLOCK CELL REQUEST from OAMH.
 ****************************************************************************/
static void rrm_mif_fsm_block_cell_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_oam_cell_block_req_t         *p_in_block_cell_req_from_oam = RRM_PNULL;
    rrmcm_rmif_cell_stop_adm_req_for_cell_block_t *p_out_cell_stop_admission_req_to_cellm = RRM_PNULL;
    /* SPR-20656 START */
	/* Code deleted */
    /* SPR-20656 END */
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et ret_val = RRM_FAILURE;
    /*SPR 15993 Fix End*/

    /* SPR 18269 Start */
    rrm_error_et          error = RRM_NO_ERROR; 
    /* SPR 18269 End */
    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_UT_TRACE_ENTER();

    p_in_block_cell_req_from_oam = (rrm_oam_cell_block_req_t *)p_msg;

    if (!p_mif_context->block_cell_ongoing)
    {
        p_mif_context->block_cell_ongoing = RRM_TRUE;
        /* SPR 18269 Start */
        p_mif_context->prev_block_cell_priority = p_in_block_cell_req_from_oam->cell_block_priority;

	/* SPR 18269 Start */
        if ((RRM_CELL_BLOCK_LOW_PRIORITY == p_in_block_cell_req_from_oam->cell_block_priority) &&
                ((!(p_in_block_cell_req_from_oam->bitmask & RRM_OAM_CELL_BLOCK_WAIT_TIMER_PRESENT)) ||
		(RRM_NULL >= p_in_block_cell_req_from_oam->cell_block_resource_cleanup_timer)))
     /* SPR 18269 End */
        {
            /*SPR 15993 Fix Start*/
            de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;
            src_id = return_src_id_from_tcb(p_mif_context, 
                            p_mif_context->currently_processed_api_trans_d);

            if(RRM_OAM_MODULE_ID == src_id)
            {
		/* SPR 18269 Start */
		if(!(p_in_block_cell_req_from_oam->bitmask & RRM_OAM_CELL_BLOCK_WAIT_TIMER_PRESENT))
        {
             /*SPR 15993 Fix End*/
            RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                    "BLOCK CELL REQ for Low priority not having timer value");
		    error = RRM_ERR_BLOCK_CELL_TIMER_NOT_PRESENT;
		}
		else
		{
		    RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
              			"BLOCK CELL REQ for Low priority having timer value "  
              			"[%d] <= ZERO",p_in_block_cell_req_from_oam->cell_block_resource_cleanup_timer);
          	    error = RRM_ERR_INVALID_PARAMS;
		}

            	rrm_mif_fsm_send_cell_block_response (error,
                    RRM_FAILURE,
                                                    p_mif_context->currently_processed_api_trans_d,
                                                     p_mif_context->cell_index, p_msg);
		/* SPR 18269 End */
            p_mif_context->block_cell_ongoing = RRM_FALSE;
            	/*SPR 15993 Fix Start*/
            }
            /*Need to deallocate the tcb*/
             ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
            RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                    "Low prioirty Block delete tcb success[%d] ",ret_val);
            /*SPR 15993 Fix End*/
        }
        else
        {
            if (RRM_CELL_BLOCK_LOW_PRIORITY == p_in_block_cell_req_from_oam->cell_block_priority)
            {
                p_gl_ctxt->timer_val[MIF_BLOCK_CELL_TIMER] =
                    p_in_block_cell_req_from_oam->cell_block_resource_cleanup_timer;
            }
            else
            {
                p_gl_ctxt->timer_val[MIF_BLOCK_CELL_TIMER] = RRM_ZERO;
            }
            p_out_cell_stop_admission_req_to_cellm = (rrmcm_rmif_cell_stop_adm_req_for_cell_block_t *)
                rrm_mem_get(sizeof(rrmcm_rmif_cell_stop_adm_req_for_cell_block_t));
            if ( p_out_cell_stop_admission_req_to_cellm == RRM_PNULL)
            {
                RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                        "Memory allocation to p_out_cell_stop_admission_req_to_cellm failed" );
                RRM_UT_TRACE_EXIT();
                return ;
            } 
            /*SPR 17777 +-*/
            build_and_send_cell_stop_adm_req_for_cell_block_to_cellm(p_mif_context,
                    p_out_cell_stop_admission_req_to_cellm);
            rrm_mif_fsm_set_state(p_mif_context, MIF_FSM_STATE_ACTIVE,
                    MIF_FSM_STATE_W_FOR_STOP_ADMISSION_RESP);
            /* Fix for <SPR-5568> */
            p_mif_context->source_stop_adm |= MIF_SOURCE_BLOCK_CELL;
            /* Fix for <SPR-5568> */

            RRM_MEM_FREE(p_out_cell_stop_admission_req_to_cellm);
        }
    }
    /* SPR-20656 START */
    else
    {
        /* SPR-20656 END */
        /* Send Reject Msg to OAM */
            /* Send Reject Msg to OAM */
            RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                    "MIF Cell Alredy in process for BLOCK_CELL_REQUEST");
            /*SPR 15993 Fix Start*/
            de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;
            src_id = return_src_id_from_tcb(p_mif_context, 
                            p_mif_context->currently_processed_api_trans_d);

            if(RRM_OAM_MODULE_ID == src_id)
            {
            /*SPR 15993 Fix End*/
            rrm_mif_fsm_send_cell_block_response (RRM_ERR_CELL_BLOCK_INPROCESS,
                    RRM_FAILURE, 
                                                    p_mif_context->currently_processed_api_trans_d, 
                                                     p_mif_context->cell_index, p_msg);
            /*SPR 15993 Fix Start*/
            }
            /*Need to deallocate the tcb*/
             ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
            RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR,
                    "Cell Block delete tcb success[%d] ",ret_val);
            /*SPR 15993 Fix End*/
    /* SPR-20656 START */
    /* code deleted */
    /* SPR-20656 END */
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_unblock_cell_cmd_act
 * Inputs         :
 *                  1. p_gl_ctxt     : is not null pointer.
 *                  2. p_mif_context
 *                  3. p_msg         : is not null pointer.
 * Outputs        : None
 * Returns        : None
 * Description    : This action routine is invoked when MIF receives a
 *                  UNBLOCK CELL IND from OAMH.
 ****************************************************************************/
static void 
rrm_mif_fsm_unblock_cell_cmd_act(
        rrm_mif_gb_context_t     *p_gl_ctxt,
        rrm_mif_context          *p_mif_context,
        void                     *p_msg
        )
{
    rrmcm_rmif_cell_start_adm_req_for_cell_unblock_t   *p_out_start_adm_to_cellm_for_cell_unblock = RRM_PNULL;
    rrm_son_cell_switch_on_req_t      dummy_object;

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);

    RRM_UT_TRACE_ENTER();

    rrm_memset_rrm_son_cell_switch_on_req(&dummy_object, RRM_ZERO);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: Initiated CELL Unblock Command!!");
    /* Fix for <SPR-5568> */
    if ((MIF_FSM_STATE_CELL_ALREADY_BLOCKED != p_mif_context->fcb[MIF_FSM].fsmState)||
            !(p_mif_context->source_stop_adm & MIF_SOURCE_BLOCK_CELL))
    {
        RRM_TRACE (g_mif_log_on_off, p_rrm_mif_facility_name, RRM_WARNING,
                "MIF Context Cell is not blocked !!");
    }
    else
    {
        /* Allocate memory for internal message to be sent to CELLM. */
        p_out_start_adm_to_cellm_for_cell_unblock = (rrmcm_rmif_cell_start_adm_req_for_cell_unblock_t *)
            rrm_mem_get(sizeof(rrmcm_rmif_cell_start_adm_req_for_cell_unblock_t));
        if ( p_out_start_adm_to_cellm_for_cell_unblock == RRM_PNULL)
        {
            /*SPR 17777 +-*/
            RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                    "Memory allocation to p_out_start_adm_to_cellm_for_cell_unblock failed"
                    ";Unused variable p_msg=%p p_gl_ctxt=%p",p_msg,p_gl_ctxt);
            /*SPR 17777 +-*/
            RRM_UT_TRACE_EXIT();
            return ;
        }
        /* Construct and send the internal message to CELLM. */
        build_and_send_cell_start_adm_req_for_cell_unblock_to_cellm(p_mif_context,
        /*SPR 17777 +-*/
                p_out_start_adm_to_cellm_for_cell_unblock);

        RRM_MEM_FREE(p_out_start_adm_to_cellm_for_cell_unblock);

        /* Change state to MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP */

        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, 
                "MIF FSM: Changing state from: %d to: \
                MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP", 
                p_mif_context->fcb[MIF_FSM].fsmState);

        rrm_mif_fsm_set_state(p_mif_context, p_mif_context->fcb[MIF_FSM].fsmState,
                MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP);
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_non_emrgncy_active_calls_ho_resp_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  NON_EMRGNCY_ACTIVE_CALLS_HO_RESP event is received.
 ****************************************************************************/
static void rrm_mif_fsm_non_emrgncy_active_calls_ho_resp_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *non_emrgncy_active_calls_ho_resp_p = RRM_PNULL;
    rrm_oam_cell_block_resp_t                out_message_oam_for_block_cell;
    U8                                       mif_ctxt_cnt = RRM_ZERO;
    U8                                       mif_ctxt_id = RRM_ZERO;
    rrm_return_et                            ret_val = RRM_FAILURE;
    U16                                      src_id = RRM_ZERO;
    U16                                      num_of_active_calls = RRM_ZERO;
    rrmcm_rmif_cell_stop_req_t    out_cell_stop_req_to_cellm = {RRM_ZERO};
    rrmcm_rmif_cell_start_adm_req_t     
        *p_out_cell_start_adm_req_to_cellm = RRM_PNULL;
    rrm_son_cell_switch_off_res_t     out_message_son;

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
    RRM_ASSERT (RRM_PNULL != p_msg);

    non_emrgncy_active_calls_ho_resp_p = (rrmuem_rmif_non_emrgncy_active_calls_ho_resp_t *)p_msg;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: NON EMRGNCY ACTIVE CALLS HO RESPONSE procedure"
            " for Cell Id [%d]", non_emrgncy_active_calls_ho_resp_p->cell_index);

    RRM_MEMSET(&out_message_son, RRM_ZERO,sizeof(rrm_son_cell_switch_off_res_t)); 
    ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
            non_emrgncy_active_calls_ho_resp_p->cell_index,
            &mif_ctxt_cnt,
            &mif_ctxt_id);
    /* Coverity 74112 Fix Start */
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off,
                p_rrm_mif_facility_name,
                RRM_ERROR,
                "Failed to  fetch mif ctxt for Cell"
                "Id[%d]",non_emrgncy_active_calls_ho_resp_p->cell_index);
        return;
    }
    /* Coverity 74112 Fix End */

    src_id = return_src_id_from_tcb(p_mif_context, 
            non_emrgncy_active_calls_ho_resp_p->transaction_id);

    rrm_memcpy_rrm_oam_eutran_global_cell_id(
            &(out_message_oam_for_block_cell.global_cell_id),
            &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));


    if ( non_emrgncy_active_calls_ho_resp_p->response )
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "MIF FSM: HO of Non-emergency active calls successful");

        if(RRM_OAM_MODULE_ID == src_id)
        {
            /* SPR-20656 START */
            /* check if emergency call exists, if doesnot exists then send response */
            if(!p_mif_context->emergcy_call_ongoing)
            {
                /* SPR-20656 END */
            out_message_oam_for_block_cell.response = RRM_SUCCESS;
            out_message_oam_for_block_cell.fail_cause = RRM_NO_ERROR;
            /* COMPOSER sends the message to OAM AGENT */
            ret_val = rrm_oam_send_rrm_oam_cell_block_resp(
                    (void *)&out_message_oam_for_block_cell,
                    RRM_MIF_MODULE_ID,
                    RRM_OAM_MODULE_ID,
                    /*SPR 15993 Fix Start*/
                    p_mif_context->currently_processed_api_trans_d,
                    /*SPR 15993 Fix End*/
                    non_emrgncy_active_calls_ho_resp_p->cell_index
                    );
            if (RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                        "rrm_oam_send_rrm_oam_block_cell_resp ");
            }
            /* De- allocated TCB */
            ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                    /*SPR 15993 Fix Start*/
                    non_emrgncy_active_calls_ho_resp_p->transaction_id);
            /*SPR 15993 Fix End*/
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
            }
                /* SPR-20656 START */
            }
            	/* SPR-20656 END */
            rrm_mif_fsm_set_state(p_mif_context,
                    MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,
                    MIF_FSM_STATE_CELL_ALREADY_BLOCKED);
        }
        else if((RRM_SON_MODULE_ID == src_id) ||
                (RRM_SON_ANR_MODULE_ID == src_id) ||
                (RRM_SON_ES_MODULE_ID == src_id)) 
        {
            /*In case of forceful and graceful if non emergncy ho is success*/
            /* Send Cell Stop Request to CellM */
            out_cell_stop_req_to_cellm.transaction_id =
                /*SPR 15993 Fix Start*/
                non_emrgncy_active_calls_ho_resp_p->transaction_id;
            /*SPR 15993 Fix End*/
            out_cell_stop_req_to_cellm.cellindex = p_mif_context->cell_index;

            /* Sending stop request to RRMCM*/
            rrm_mif_send_internal_msg(RRM_CM_MODULE_ID,
                    RRMCM_RMIF_CELL_STOP_REQ,
                    sizeof(rrmcm_rmif_cell_stop_req_t),
                    (void *)&out_cell_stop_req_to_cellm);

            rrm_mif_fsm_set_state(p_mif_context, 
                    MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                    MIF_FSM_STATE_W_FOR_CELL_STOP_RESP);
        }
    }
    else
    {
        if(RRM_OAM_MODULE_ID == src_id)
        {
            if (!(p_mif_context->prev_block_cell_priority)) /* Sending forceful release request to UEM for all active non-emengency calls here*/
            {       
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, "Sending Ue release forcefully req to UEM for "
                        "All Non-emergency active calls for High Priority block cell req");

                num_of_active_calls = rrm_get_non_emergengy_active_call (p_mif_context->cell_index);

                if (num_of_active_calls)
                {
                    build_and_send_release_all_existing_ue_forcefully_req( p_mif_context->cell_index,
                            non_emrgncy_active_calls_ho_resp_p->transaction_id);
                }

                /* SPR-20656 START */
                /* check if emergency call exists, if doesnot exists then send response */
                if(!p_mif_context->emergcy_call_ongoing)
                {
                    /* SPR-20656 END */
                out_message_oam_for_block_cell.response = RRM_SUCCESS;
                out_message_oam_for_block_cell.fail_cause = RRM_NO_ERROR;

                /* COMPOSER sends the message to OAM AGENT */
                ret_val = rrm_oam_send_rrm_oam_cell_block_resp(
                                                        (void *)&out_message_oam_for_block_cell,
                                                        RRM_MIF_MODULE_ID, 
                                                        RRM_OAM_MODULE_ID,
                        /*SPR 15993 Fix Start*/
                        p_mif_context->currently_processed_api_trans_d,
                        /*SPR 15993 Fix End*/
                                                        non_emrgncy_active_calls_ho_resp_p->cell_index
                                                        );
                if(RRM_FAILURE == ret_val) 
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                            "rrm_oam_send_rrm_oam_block_cell_resp");
                }
                /* De-allocated TCB */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,
                        /*SPR 15993 Fix Start*/
                        non_emrgncy_active_calls_ho_resp_p->transaction_id);
                /*SPR 15993 Fix End*/
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                }
                    /* SPR-20656 START */
                }
                /* SPR-20656 END */
                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,
                        MIF_FSM_STATE_CELL_ALREADY_BLOCKED);
            }
            /* SPR 13675 Fix Start */
            else
            {
                /* SPR-20656 START */
                /* check if emergency call exists, if doesnot exists then send response */
                if(!p_mif_context->emergcy_call_ongoing)
                {
                    /* SPR-20656 END */
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, "Sending Block Cell resp FAILURE to OAM for "
                        "Low Priority block cell req");
                out_message_oam_for_block_cell.response = RRM_FAILURE;
                out_message_oam_for_block_cell.fail_cause = RRM_ERR_CELL_BLOCK_HO_FAILURE;

                /* COMPOSER send the message to OAM AGENT */
                /* Kshitij Coverity, CID 11469 */
                if(RRM_FAILURE == (ret_val = rrm_oam_send_rrm_oam_cell_block_resp(
                                    (void *)&out_message_oam_for_block_cell,
                                    RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                /*SPR 15993 Fix Start*/
                                p_mif_context->currently_processed_api_trans_d,
                                /*SPR 15993 Fix End*/
                                    non_emrgncy_active_calls_ho_resp_p->cell_index)))
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                            "rrm_oam_send_rrm_oam_block_cell_resp");
                }
                /* Kshitij end */
		    /* De-allocated TCB */
                ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, p_mif_context->currently_processed_api_trans_d);
		    if(RRM_FAILURE == ret_val)
                    {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
                    }
                    /* SPR-20656 START */
                }
                rrm_mif_fsm_set_state(p_mif_context,
                        MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,
                        MIF_FSM_STATE_CELL_ALREADY_BLOCKED);
                /* SPR-20656 END */
	        }
            /* SPR 13675 Fix Stop */
        }
        else if((RRM_SON_MODULE_ID == src_id) ||
                (RRM_SON_ANR_MODULE_ID == src_id) ||
                (RRM_SON_ES_MODULE_ID == src_id)) 
        {
            /*If forceful switch_off send cell stop */
            if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_FORCEFUL_CELL)
            {
                /* Send Cell Stop Request to Cell M */
                out_cell_stop_req_to_cellm.transaction_id =
                    /*SPR 15993 Fix Start*/
                    non_emrgncy_active_calls_ho_resp_p->transaction_id;
                /*SPR 15993 Fix End*/
                out_cell_stop_req_to_cellm.cellindex = p_mif_context->cell_index;

                /* Sending stop request to RRMCM*/
                rrm_mif_send_internal_msg(RRM_CM_MODULE_ID,
                        RRMCM_RMIF_CELL_STOP_REQ,
                        sizeof(rrmcm_rmif_cell_stop_req_t),
                        (void *)&out_cell_stop_req_to_cellm);

                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,
                        MIF_FSM_STATE_W_FOR_CELL_STOP_RESP);
            }
            else if(p_mif_context->source_stop_adm & MIF_SOURCE_SWITCHOFF_GRACEFUL_CELL)
            {
                /* coverity  28796 : fixed */
                RRM_MEMCPY(&(out_message_son.cell_switchoff_res.cell_id),
                        &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id),
                        sizeof(rrm_oam_eutran_global_cell_id_t));

                out_message_son.cell_switchoff_res.result = RRM_FAILURE;
                out_message_son.cell_switchoff_res.error_code = RRM_ERR_CALL_ONGOING;

                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Response[%d] ",
                        out_message_son.cell_switchoff_res.error_code);

                /* COMPOSER send the message to SON */
                ret_val = rrm_son_send_rrm_son_cell_switch_off_res(
                        (void *)&out_message_son,
                        RRM_MIF_MODULE_ID,
                        src_id,
                        /*SPR 15993 Fix Start*/
                        p_mif_context->currently_processed_api_trans_d,
                        /*SPR 15993 Fix End*/
                        non_emrgncy_active_calls_ho_resp_p->cell_index); 
                /* Coverity 75047 fix start */
                if(RRM_FAILURE == ret_val)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                            "rrm_son_send_rrm_son_cell_switch_off_res");
                }
                /* Coverity 75047 fix end */
                /*send graceful switch off request to son in case of graceful 
                  and send start admission request to cellm*/
                p_out_cell_start_adm_req_to_cellm = (rrmcm_rmif_cell_start_adm_req_t *)
                    rrm_mem_get(sizeof(rrmcm_rmif_cell_start_adm_req_t));
                if ( p_out_cell_start_adm_req_to_cellm == RRM_PNULL)
                {
                    RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_ERROR,
                            "Memory allocation to p_out_cell_start_adm_req_to_cellm failed" );
                    RRM_UT_TRACE_EXIT();
                    return ;
                }

                build_and_send_cell_start_adm_req_to_cellm(p_mif_context,
                /*SPR 17777 +-*/
                        p_out_cell_start_adm_req_to_cellm);

                RRM_MEM_FREE(p_out_cell_start_adm_req_to_cellm);
                rrm_mif_fsm_set_state(p_mif_context, 
                        MIF_FSM_STATE_W_FOR_EMERGENCY_CALLS_ACTIVE_RESP,
                        MIF_FSM_STATE_W_FOR_START_ADMISSION_RESP);
            }
        }
        else
        {
            /* SPR-20656 START */
            if(!p_mif_context->emergcy_call_ongoing)
            {
            /* SPR-20656 END */
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, "Sending Block Cell resp FAILURE to OAM for "
                    "Low Priority block cell req");
            out_message_oam_for_block_cell.response = RRM_FAILURE;
            out_message_oam_for_block_cell.fail_cause = RRM_ERR_CELL_BLOCK_HO_FAILURE;

            /* COMPOSER send the message to OAM AGENT */
            /* Kshitij Coverity, CID 11469 */
            if(RRM_FAILURE == (ret_val = rrm_oam_send_rrm_oam_cell_block_resp(
                                                                (void *)&out_message_oam_for_block_cell, 
                                                                RRM_MIF_MODULE_ID, 
                                                                RRM_OAM_MODULE_ID,
                            /*SPR 15993 Fix Start*/
                            p_mif_context->currently_processed_api_trans_d,
                            /*SPR 15993 Fix End*/
                                                                non_emrgncy_active_calls_ho_resp_p->cell_index
                                                                )))
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Send Failed for "
                        "rrm_oam_send_rrm_oam_block_cell_resp");
            }
            /* Kshitij end */
            /* De-allocated TCB */
            ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, 
                    /*SPR 15993 Fix Start*/
                    non_emrgncy_active_calls_ho_resp_p->transaction_id);
            /*SPR 15993 Fix End*/
            if(RRM_FAILURE == ret_val)
            {
                RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
            }
            /* SPR-20656 START */
            }
            rrm_mif_fsm_set_state(p_mif_context,
                    MIF_FSM_STATE_W_FOR_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,
                    MIF_FSM_STATE_CELL_ALREADY_BLOCKED);
            /* SPR-20656 END */
        }
    }
    /* Change MIF state to ACTIVE */
    p_mif_context->block_cell_ongoing = RRM_FALSE;

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_already_blocked_cell_block_req_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  MIF_FSM_EVENT_BLOCK_CELL_REQ event is received when Cell is already blocked.
 ****************************************************************************/
static void rrm_mif_fsm_cell_already_blocked_cell_block_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_error_et                              fail_cause = RRM_NO_ERROR;
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et                   ret_val = RRM_FAILURE;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    fail_cause = RRM_ERR_CELL_ALREADY_BLOCKED;

    /*SPR 15993 Fix Start*/
    de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;

    src_id = return_src_id_from_tcb(p_mif_context, 
                       p_mif_context->currently_processed_api_trans_d);

    if(RRM_OAM_MODULE_ID == src_id)
    {
        rrm_mif_fsm_send_cell_block_response(fail_cause, RRM_FAILURE, p_mif_context->currently_processed_api_trans_d, 
                p_mif_context->cell_index, p_msg);
    }

    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
    }
    /*SPR 15993 Fix End*/

    RRM_UT_TRACE_EXIT();

}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_reconfig_ongoing_cell_block_req_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  MIF_FSM_EVENT_BLOCK_CELL_REQ event is received when MIF
 *                  is waiting for Cell Reconfig response
 ****************************************************************************/
static void rrm_mif_fsm_cell_reconfig_ongoing_cell_block_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_error_et                              fail_cause = RRM_NO_ERROR;
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et                   ret_val = RRM_FAILURE;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);

     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/
    fail_cause = RRM_ERR_CELL_RECONFIG_ONGOING;

    /*SPR 15993 Fix Start*/
    de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;

    src_id = return_src_id_from_tcb(p_mif_context, 
                       p_mif_context->currently_processed_api_trans_d);
    if(RRM_OAM_MODULE_ID == src_id)
    {
        rrm_mif_fsm_send_cell_block_response(fail_cause, RRM_FAILURE, p_mif_context->currently_processed_api_trans_d, 
                p_mif_context->cell_index, p_msg);
    }

    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
    }
    /*SPR 15993 Fix End*/
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_delete_ongoing_cell_block_req_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  MIF_FSM_EVENT_BLOCK_CELL_REQ event is received when MIF
 *                  is waiting for Cell Delete response
 ****************************************************************************/
static void rrm_mif_fsm_cell_delete_ongoing_cell_block_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_error_et                              fail_cause = RRM_NO_ERROR;
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et                   ret_val = RRM_FAILURE;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    fail_cause = RRM_ERR_CELL_DELETION_ONGOING;

    /*SPR 15993 Fix Start*/
    de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;

    src_id = return_src_id_from_tcb(p_mif_context, 
                       p_mif_context->currently_processed_api_trans_d);

    if(RRM_OAM_MODULE_ID == src_id)
    {
        rrm_mif_fsm_send_cell_block_response(fail_cause, RRM_FAILURE, p_mif_context->currently_processed_api_trans_d, 
                p_mif_context->cell_index, p_msg);
    }
    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
    }
    /*SPR 15993 Fix End*/
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_start_ongoing_cell_block_req_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  MIF_FSM_EVENT_BLOCK_CELL_REQ event is received when MIF
 *                  is waiting for Cell start response
 ****************************************************************************/
static void rrm_mif_fsm_cell_start_ongoing_cell_block_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_error_et                              fail_cause = RRM_NO_ERROR;
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et                   ret_val = RRM_FAILURE;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    fail_cause = RRM_ERR_CELL_START_ONGOING;

    /*SPR 15993 Fix Start*/
    de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;

    src_id = return_src_id_from_tcb(p_mif_context, 
                       p_mif_context->currently_processed_api_trans_d);

    if(RRM_OAM_MODULE_ID == src_id)
    {
        rrm_mif_fsm_send_cell_block_response(fail_cause, RRM_FAILURE, p_mif_context->currently_processed_api_trans_d, 
                p_mif_context->cell_index, p_msg);
    }

    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
    }
    /*SPR 15993 Fix End*/
    RRM_UT_TRACE_EXIT();
} 

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_stop_ongoing_cell_block_req_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  MIF_FSM_EVENT_BLOCK_CELL_REQ event is received when MIF
 *                  is waiting for Cell Stop response
 ****************************************************************************/
static void rrm_mif_fsm_cell_stop_ongoing_cell_block_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_error_et                              fail_cause = RRM_NO_ERROR;
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et                   ret_val = RRM_FAILURE;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    fail_cause = RRM_ERR_CELL_STOP_ONGOING;

    /*SPR 15993 Fix Start*/
    de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;

    src_id = return_src_id_from_tcb(p_mif_context, 
                       p_mif_context->currently_processed_api_trans_d);

    if(RRM_OAM_MODULE_ID == src_id)
    {
        rrm_mif_fsm_send_cell_block_response(fail_cause, RRM_FAILURE, p_mif_context->currently_processed_api_trans_d, 
                p_mif_context->cell_index, p_msg);
    }

    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
    }
    /*SPR 15993 Fix End*/
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_switchon_ongoing_cell_block_req_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                  MIF_FSM_EVENT_BLOCK_CELL_REQ event is received when MIF
 *                  is waiting for Cell Switchon request
 ****************************************************************************/
static void rrm_mif_fsm_cell_switchon_ongoing_cell_block_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrm_error_et                              fail_cause = RRM_NO_ERROR;
/*SPR 15993 Fix Start*/
    U16                             src_id = RRM_ZERO;
    U16                             de_allocate_trans_d = RRM_ZERO;
    rrm_return_et                   ret_val = RRM_FAILURE;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    fail_cause = RRM_ERR_CELL_SWITCHON_INPROGRESS;

    /*SPR 15993 Fix Start*/
    de_allocate_trans_d = p_mif_context->currently_processed_api_trans_d;

    src_id = return_src_id_from_tcb(p_mif_context, 
                       p_mif_context->currently_processed_api_trans_d);

    if(RRM_OAM_MODULE_ID == src_id)
    {
        rrm_mif_fsm_send_cell_block_response(fail_cause, RRM_FAILURE, p_mif_context->currently_processed_api_trans_d, 
                p_mif_context->cell_index, p_msg);
    }

    ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context, de_allocate_trans_d);
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "MIF FSM: TCB De-Allocation Failed");
    }
    /*SPR 15993 Fix End*/
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_send_cell_block_response
 * Inputs         :
 *              1. fail_cause : reason of failure, if any
 *              2. response : RRM_SUCCESS or RRM_FAILURE
 *              3. trans_id         : transaction id
 *              4. cell_index   : cell index
 *              4. p_msg   
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function sends the SUCCESS or FAILURE in the RRM_OAM_CELL_BLOCK_RESP
 *                  to OAM.
 ****************************************************************************/

static void rrm_mif_fsm_send_cell_block_response 
(
 rrm_error_et          fail_cause,
 rrm_return_et         response,
 U16                   trans_id,
 U8                    cell_index,
 void                  *p_msg
 )
{
    RRM_UT_TRACE_ENTER();
    rrm_oam_cell_block_req_t                  *p_in_block_cell_req_msg = RRM_PNULL;
    rrm_oam_cell_block_resp_t                 block_cell_resp_to_oam ;

    p_in_block_cell_req_msg = (rrm_oam_cell_block_req_t *)(p_msg);

    RRM_ASSERT (RRM_PNULL != p_msg);
    rrm_memcpy_rrm_oam_eutran_global_cell_id(
            &(block_cell_resp_to_oam.global_cell_id),
            &(p_in_block_cell_req_msg->global_cell_id));

    block_cell_resp_to_oam.response = response;
    block_cell_resp_to_oam.fail_cause = fail_cause;

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF,
            "Failure: Sending Block Cell Resp with fail_cause(%d) to OAM", fail_cause);

    /* Coverity : CID 11469 Rel2.1.3 */
    if(RRM_FAILURE == rrm_oam_send_rrm_oam_cell_block_resp(
                                        &block_cell_resp_to_oam, 
                                        RRM_MIF_MODULE_ID, 
                                        RRM_OAM_MODULE_ID,
                                        trans_id,
                                        cell_index))
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF,
                "Failed to send Cell Block resp to OAM");
    }

    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_no_active_ue_ind_act
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                    MIF_FSM_EVENT_NO_ACTIVE_UE_IND event is received.
 ****************************************************************************/
static void rrm_mif_fsm_no_active_ue_ind_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_mif_context          *p_mif_context,
 void                     *p_msg
 )
{
    rrmuem_rmif_no_active_ue_ind_t *p_no_active_ue_ind = RRM_PNULL;
    rrm_oam_ready_for_cell_block_ind_t out_message_ready_for_cell_block_ind;
    U8                                       mif_ctxt_cnt = RRM_ZERO;
    U8                                       mif_ctxt_id = RRM_ZERO;
    rrm_return_et                            ret_val = RRM_FAILURE;
    /* warnigs removal: 'src_id' is unused, 10/08/2012 */
    /* U16                                      src_id = 0; */

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL != p_mif_context);
    RRM_ASSERT (RRM_PNULL != p_msg);

    p_no_active_ue_ind = (rrmuem_rmif_no_active_ue_ind_t *)p_msg;

    ret_val = fetch_mif_ctxt_id_based_on_cell_id(p_gl_ctxt,
            p_no_active_ue_ind->cell_index,
            &mif_ctxt_cnt,
            &mif_ctxt_id);
    /* Coverity 74112 Fix Start */
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off,
                p_rrm_mif_facility_name,
                RRM_ERROR,
                "Failed to  fetch mif ctxt for Cell"
                "Id[%d]",p_no_active_ue_ind->cell_index);
        return;
    }
    /* Coverity 74112 Fix End */

    rrm_memset_rrm_oam_ready_for_cell_block_ind(&out_message_ready_for_cell_block_ind, RRM_ZERO);

    /* Fix for <SPR-5568>*/
    if ((MIF_SOURCE_BLOCK_CELL & p_mif_context->source_stop_adm) && !p_mif_context->block_cell_ongoing)
    {
        /* SPR-20656 START */
        p_mif_context->emergcy_call_ongoing = RRM_FALSE;
        /* SPR-20656 END */
        /* Filling Global Cell Id */
        rrm_memcpy_rrm_oam_eutran_global_cell_id(
                &(out_message_ready_for_cell_block_ind.global_cell_id),
                &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));

        /* Sending RRM_OAM_READY_FOR_CELL_BLOCK_IND API to OAM */
        ret_val = rrm_oam_send_rrm_oam_ready_for_cell_block_ind (
                (void *)&out_message_ready_for_cell_block_ind,
                RRM_MIF_MODULE_ID,
                RRM_OAM_MODULE_ID,
                p_mif_context->currently_processed_api_trans_d,
                p_mif_context->cell_index);
        if (RRM_FAILURE == ret_val)
        {
            RRM_TRACE(g_mif_log_on_off,
                    p_rrm_mif_facility_name,
                    RRM_ERROR,
                    "Send Failed for rrm_oam_send_rrm_oam_ready_for_cell_block_ind");
        }
    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name,
                RRM_BRIEF,
                "Cell Block is not done, so RRMUEM_RMIF_NO_ACTIVE_UE_IND discarded by MIF");
    }

    RRM_UT_TRACE_EXIT();
}
/*ECN start*/
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_ecn_capacity_enhance_req_act 
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                   MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_ecn_capacity_enhance_req_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_ecn_configure_cell_list_t *p_in_cell_ecn_confgiure_bitrate= RRM_PNULL;
    rrmcm_rmif_cell_ecn_config_req_t p_out_cell_ecn_config_req= {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Congestion procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    p_in_cell_ecn_confgiure_bitrate= (rrm_ecn_configure_cell_list_t *)p_msg;

     /*SPR 17777 +-*/
    RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    build_and_send_cell_ecn_capacity_enhance_req_to_cellm(p_mif_context,
            p_in_cell_ecn_confgiure_bitrate, 
            &p_out_cell_ecn_config_req);
     /*SPR 17777 +-*/
    RRM_UT_TRACE_EXIT();
}
/*ECN end*/
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_get_kpi_resp_act 
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Returns        :
 *              1. void
 * Description    : This function is the action routine when
 *                   CELL_GET_KPI_ERSP event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_get_kpi_resp_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_get_kpi_resp_t *p_in_get_kpi = RRM_PNULL;
    rrm_oam_get_kpi_resp_t    out_oam_get_kpi_resp= {RRM_ZERO};
    rrm_return_et             ret_val = RRM_SUCCESS;
/*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
/*SPR 15993 Fix End*/

    RRM_UT_TRACE_ENTER();

    p_in_get_kpi = (rrmcm_rmif_get_kpi_resp_t *) p_msg;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Get Kpi Response procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

/*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
                                    p_in_get_kpi->trans_id);
/*SPR 15993 Fix End*/
    ret_val = copy_global_cell_id_from_ctxt(p_gl_ctxt,
            &(out_oam_get_kpi_resp.cell_id),
            p_in_get_kpi->cell_index);
    if (RRM_SUCCESS == ret_val)
    {
        out_oam_get_kpi_resp.bitmask |= RRM_OAM_GET_KPI_CELL_ID_PRESENT; 
    }
    if(RRM_FAILURE == ret_val || RRM_FAILURE == p_in_get_kpi->response)
    { 
        out_oam_get_kpi_resp.response = RRM_FAILURE;
        out_oam_get_kpi_resp.fail_cause = RRM_ERR_CELL_UNCONFIGURED;
    }
    else
    {
        /*mapping of kpi param received from cellm form OAM message*/ 
        map_kpi_from_cellm(&(p_in_get_kpi->kpi),&(out_oam_get_kpi_resp.kpi_data));
        out_oam_get_kpi_resp.response = p_in_get_kpi->response;
        out_oam_get_kpi_resp.fail_cause = p_in_get_kpi->fail_cause;
    }
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Get Kpi Response[%d] fail cause[%d]",  
            out_oam_get_kpi_resp.response,out_oam_get_kpi_resp.fail_cause);
    /*Send response to OAM*/ 
    /*SPR 15993 Fix Start*/
    if(RRM_OAM_MODULE_ID == src_id)
    {
    /*SPR 15993 Fix End*/
    rrm_oam_send_rrm_oam_get_kpi_resp(
            &out_oam_get_kpi_resp,
            RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    p_mif_context->currently_processed_api_trans_d,
                                    p_mif_context->cell_index);
    /*SPR 15993 Fix Start*/
    }
    /*SPR 15993 Fix End*/

   /*dealloc tcb*/
    ret_val = de_allocate_all_tcb_from_mif_ctxt(p_gl_ctxt,
/*SPR 15993 Fix Start*/
                                  p_in_get_kpi->trans_id);
/*SPR 15993 Fix End*/
    if(RRM_FAILURE == ret_val )
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                "MIF FSM: dealloc fail for trans_id[%d]",
                p_mif_context->currently_processed_api_trans_d); 
    }
    RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_config_kpi_resp_act 
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Returns        :
 *              1. void
 * Description    : This function is the action routine when
 *                   CELL_CONFIG_KPI_ERSP event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_config_kpi_resp_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrmcm_rmif_generic_resp_t       *p_in_cellm_generic_resp = RRM_PNULL;
    rrm_oam_config_kpi_resp_t       out_config_kpi_to_oam = {RRM_ZERO};
    rrm_return_et                   ret_val                  = RRM_SUCCESS;
/*SPR 15993 Fix Start*/
    U16                                 src_id = RRM_ZERO;
/*SPR 15993 Fix End*/
    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    p_in_cellm_generic_resp = (rrmcm_rmif_generic_resp_t *) p_msg;
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Config Kpi Response procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);

/*SPR 15993 Fix Start*/
    src_id = return_src_id_from_tcb(p_mif_context, 
                                    p_in_cellm_generic_resp->transaction_id);
/*SPR 15993 Fix End*/
    ret_val = copy_global_cell_id_from_ctxt(p_gl_ctxt,
            &(out_config_kpi_to_oam.cell_id),
            p_in_cellm_generic_resp->cellindex);
    if (RRM_SUCCESS == ret_val)
    {
        out_config_kpi_to_oam.bitmask |= RRM_OAM_CONFIG_KPI_CELL_ID_PRESENT; 
    }
    if(RRM_FAILURE == ret_val)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                "Incorrect Mif Context for Cell Id %d ", 
                p_in_cellm_generic_resp->cellindex);
        out_config_kpi_to_oam.response = RRM_FAILURE;
        out_config_kpi_to_oam.fail_cause = RRM_ERR_CELL_UNCONFIGURED;
    }
    else if (RRM_SUCCESS == p_in_cellm_generic_resp->response)
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, 
                "Cell Id[%d] Kpi Config Resp Success", 
                p_in_cellm_generic_resp->cellindex);
        out_config_kpi_to_oam.response = RRM_SUCCESS;
        out_config_kpi_to_oam.fail_cause = p_in_cellm_generic_resp->fail_cause;
    }
    else
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                "Cell Id[%d] Kpi Config Resp Failure Cause[%d]", 
                p_in_cellm_generic_resp->cellindex,
                p_in_cellm_generic_resp->fail_cause);
        out_config_kpi_to_oam.response = RRM_FAILURE;
        out_config_kpi_to_oam.fail_cause = p_in_cellm_generic_resp->fail_cause;
    }
/*SPR 15993 Fix Start*/
    if(RRM_OAM_MODULE_ID == src_id)
    {
/*SPR 15993 Fix End*/
    rrm_oam_send_rrm_oam_config_kpi_resp(
            &out_config_kpi_to_oam,
            RRM_MIF_MODULE_ID,
                                    RRM_OAM_MODULE_ID,
                                    p_mif_context->currently_processed_api_trans_d,
                                    p_mif_context->cell_index);
/*SPR 15993 Fix Start*/
     }
/*SPR 15993 Fix End*/

    /*dealloc tcb*/
    ret_val = de_allocate_all_tcb_from_mif_ctxt(p_gl_ctxt,
/*SPR 15993 Fix Start*/
                                                 p_in_cellm_generic_resp->transaction_id);
/*SPR 15993 Fix End*/
    if(RRM_FAILURE == ret_val )
    {
        RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                "MIF FSM: dealloc fail for trans_id[%d]",
                p_mif_context->currently_processed_api_trans_d); 
    }
    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_get_kpi_act 
 * Inputs         :
 *              1. p_gl_ctxt : Mif global context.
 *              2. p_mif_context : Mif context.
 *              3. p_msg         : message need to send to cellm.
 * Returns        :
 *              1. void
 * Description    : This function is the action routine when
 *                   event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_get_kpi_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_get_kpi_req_t      *p_rrm_oam_get_kpi_req= RRM_PNULL;

    RRM_UT_TRACE_ENTER();
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    p_rrm_oam_get_kpi_req = (rrm_oam_get_kpi_req_t *)p_msg;
    /*build and send get kpi request to cellm*/
    build_and_send_get_kpi_request_to_cellm(p_rrm_oam_get_kpi_req,
            p_mif_context);

    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  :rrm_mif_fsm_cell_config_kpi_act 
 * Inputs         :
 *              1. p_gl_ctxt : mif global context.
 *              2. p_mif_context : Mif context.
 *              3. p_msg         : Message need to send to cellm.
 * Returns        :
 *              1. void
 * Description    : This function is the action routine when
 *                   event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_config_kpi_act
(
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_oam_config_kpi_req_t    *p_in_oam_config_kpi_req = RRM_PNULL;

    RRM_UT_TRACE_ENTER();
     /*SPR 17777 +-*/
    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILEDALL, "Unused variable p_gl_ctxt=%p",p_gl_ctxt);/*SPR 17777 +-*/

    p_in_oam_config_kpi_req = (rrm_oam_config_kpi_req_t *)p_msg;
    build_and_send_kpi_config_req_to_cellm(p_in_oam_config_kpi_req,
            p_mif_context);

    RRM_UT_TRACE_EXIT();
}
/****************************************************************************
 * Function Name  : rrm_mif_fsm_cell_platform_ind_act 
 * Inputs         :
 *              1. p_gl_ctxt : is not null pointer.
 *              2. p_mif_context : is not null pointer.
 *              3. p_msg         : is not null pointer.
 * Outputs        :
 *
 * Returns        :
 *              1. void
 * Variables      :
 * Description    : This function is the action routine when
 *                   MIF_FSM_EVENT_ECN_CAPACITY_ENHANCE_REQ event is received.
 ****************************************************************************/
static void rrm_mif_fsm_cell_platform_ind_act 
(
 rrm_mif_gb_context_t     *p_gl_ctxt, 
 rrm_mif_context            *p_mif_context,
 void            *p_msg             
 )
{
    rrm_plat_cell_load_t   *p_in_cell_plat_ind = RRM_PNULL;
    rrmcm_rmif_cell_platform_t p_out_cell_platform = {RRM_ZERO};

    RRM_UT_TRACE_ENTER();

    RRM_ASSERT (RRM_PNULL !=p_gl_ctxt);
    RRM_ASSERT (RRM_PNULL !=p_mif_context);
    RRM_ASSERT (RRM_PNULL !=p_msg);

    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
            "MIF FSM: Initiated Cell Congestion procedure"  
            " for MIF ContextId [%d]", p_mif_context->mif_ctxt_id);
    p_in_cell_plat_ind = (rrm_plat_cell_load_t *)p_msg;

     /*SPR 17777 +-*/
    RMIF_SET_CELL_REQUEST_SENT(p_gl_ctxt, p_mif_context->cell_index);
    build_and_send_cell_platform_to_cellm(p_mif_context,
            p_in_cell_plat_ind, 
            &p_out_cell_platform);
     /*SPR 17777 +-*/
    RRM_UT_TRACE_EXIT();
}



