/****************************************************************************
*
* ARICENT -
*
* Copyright (C) 2012 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
* SPR-612 Coverity Fix Starts
* $Id: son_utils.c $
* SPR-612 Coverity Fix Ends
*
****************************************************************************
*
* File Description: This file contains the function definitions which are
*                   common to various SON modules. It includes APIs to
*                   compose, pack, unpack, alloc and send the message,
*                   compose the CSPL header etc.
*
****************************************************************************
* Revision Details
*----------------------
*   DATE            AUTHOR      REFERENCE       REASON
*   April, 2012     Gautam Shah/Shilpi Jain     Initial Code
*   July, 2014      Shilpi                      SPR 12708 Fix 
*   July, 2014      Shilpi                      SPR 10331 Fix
*   Aug, 2014       Shilpi                      SPR 13251 Fix
****************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/
#include <son_utils.h>
#include <son_oam_composer.h>
#include <son_types.h>
#include <son_mif_msg_handler.h>
#include <x2ap_api.h>
#include <math.h>
#include <pthread.h>

/****************************************************************************
* Macro Definitions
****************************************************************************/
#define FMT_MAX_LEN 255
#define UNKNOWN_API_NAME "UNKNOWN_API_ID"
#define UNKNOWN_MODULE "UNKNOWN_MODULE_ID"
/*SPR 22554 Fix Start*/
#define SON_THREAD_NAME_LEN 20
/*SPR 22554 Fix End*/
/****************************************************************************
 * Global Variables 
 ****************************************************************************/

extern son_global_config_params_t g_config_params;
/* Rel 3.0 Cell delete support changes Start */
son_s16 temp_txn_id_list[SON_MAX_TEMP_TXN_ID] = {[0 ... (SON_MAX_TEMP_TXN_ID - 1)] = -1};
/* Rel 3.0 Cell delete support changes Stop */

const QMANIFEST *p_manifest[] = {
    &son_manifest_mif,
    &son_manifest_apps,
    &son_manifest_es,
    &son_manifest_anr,
    &son_manifest_nmm,
    &son_manifest_x2,
#ifdef SON_MRO_FEATURE
    &son_manifest_mro,
#endif
    &son_manifest_mlb,
    &son_manifest_rach_opt

};
/*coverity 19361 fix start*/
const son_s8 *p_son_log_level_names[] =
{
    /* Typecasting strings to const son_s8 pointers to remove klocwork
     * warnings. */
    (const son_s8 *)"SON_OAM_LOG_LEVEL_CRITICAL",
    (const son_s8 *)"SON_OAM_LOG_LEVEL_ERROR",
    (const son_s8 *)"SON_OAM_LOG_LEVEL_WARNING",
    (const son_s8 *)"SON_OAM_LOG_LEVEL_INFO",
    (const son_s8 *)"SON_OAM_LOG_LEVEL_BRIEF",
    (const son_s8 *)"SON_OAM_LOG_LEVEL_DETAILED",
    (const son_s8 *)"SON_OAM_LOG_LEVEL_DETAILEDALL",
    (const son_s8 *)"SON_LOG_LEVEL_NONE"
};
/*coverity 19361 fix end*/

/*  SON_MODE_NAMES- Stores son_mode_et enum possible values as
 *  strings */
const son_s8 *SON_MODE_NAMES[] =
{
    (const son_s8 *)"SON_MODE_DISTRIBUTED",
    (const son_s8 *)"SON_MODE_CENTRALIZED",
    (const son_s8 *)"NOT_AVAILABLE"
};

/* Bug 121 Fix Start */
/* SON internal Module Names */
const son_s8 *SON_INTERNAL_MODULE_NAMES[] = 
{
    (const son_s8 *)"SON_MIF",
    (const son_s8 *)"SON_APPS",
    (const son_s8 *)"SON_ANR",
    (const son_s8 *)"SON_ES",
    (const son_s8 *)"SON_NMM",
    (const son_s8 *)"SON_MRO",
    (const son_s8 *)"SON_MLB",
    (const son_s8 *)"SON_RACH",
    (const son_s8 *)"SON_LOGGER",
    (const son_s8 *)"SON_X2",
    (const son_s8 *)"SON_MODULE_NAME_NOT_AVAILABLE",
};

const son_s8* SON_EXTERNAL_MODULE_NAMES[] = 
{
    (const son_s8 *)"SON_OAM_MODULE_ID",
    (const son_s8 *)"SON_RRM_MODULE_ID",
    (const son_s8 *)"SON_RRC_MODULE_ID",
    (const son_s8 *)"SON_PHY_MODULE_ID"
};

const son_s8* SON_X2AP_API_NAMES[] = 
{
    (const son_s8 *)"X2AP_ENB_CONFIG_UPDATE_REQ",
    (const son_s8 *)"X2AP_ENB_CONFIG_UPDATE_RES",
    (const son_s8 *)"X2AP_ENB_CONFIG_UPDATE_IND",
    (const son_s8 *)"X2AP_RESET_REQ",
    (const son_s8 *)"X2AP_RESET_RES",
    (const son_s8 *)"X2AP_RESET_IND",
    (const son_s8 *)"X2AP_RLF_IND",
    (const son_s8 *)"X2AP_ENB_LINK_UP_IND",
    (const son_s8 *)"X2AP_ENB_LINK_DOWN_IND",
    (const son_s8 *)"X2AP_ENB_CONFIG_UPDATE_WAIT_IND",
    (const son_s8 *)"X2AP_HO_REPORT_IND",
    (const son_s8 *)"X2AP_PEER_CELL_ACTIVATION_REQ",
    (const son_s8 *)"X2AP_PEER_CELL_ACTIVATION_RESP",
    (const son_s8 *)"X2AP_CELL_ACTIVATION_REQ",
    (const son_s8 *)"X2AP_CELL_ACTIVATION_RESP",
    (const son_s8 *)"X2AP_MOBILITY_CHANGE_REQ",
    (const son_s8 *)"X2AP_MOBILITY_CHANGE_RESP",
    (const son_s8 *)"X2AP_MOBILITY_SETTING_CHANGE_REQ",
    (const son_s8 *)"X2AP_MOBILITY_SETTING_CHANGE_RESP"
};

const son_s8* SON_RRC_API_NAMES[] = 
{
    (const son_s8 *)"X2AP_ADD_ENB_REQ",
    (const son_s8 *)"X2AP_ADD_ENB_RES",
    (const son_s8 *)"X2AP_DEL_ENB_REQ",
    (const son_s8 *)"X2AP_DEL_ENB_RES"
};

const son_s8* RRM_SON_API_NAME[] =
{
    (const son_s8 *)"RRM_SON_REGISTER_REQ",
    (const son_s8 *)"RRM_SON_DEREGISTER_REQ",
    (const son_s8 *)"RRM_SON_SET_ATTRIBUTE_REQ",
    (const son_s8 *)"RRM_SON_CELL_SWITCH_ON_REQ",
    (const son_s8 *)"RRM_SON_CELL_SWITCH_OFF_REQ",
    (const son_s8 *)"RRM_SON_NMM_PREPARE_REQ",
    (const son_s8 *)"RRM_SON_NMM_COMPLETE_REQ",
    (const son_s8 *)"RRM_SON_MEAS_CONFIG_REQ",
    (const son_s8 *)"RRM_SON_TNL_DISCOVERY_REQ",
    (const son_s8 *)"RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ",
    (const son_s8 *)"RRM_SON_RACH_CONFIG_REQ",
    (const son_s8 *)"RRM_SON_ENB_CONFIG_UPDATE_IND"
};

const son_s8* SON_RRM_API_NAME[] =
{
    (const son_s8 *)"RRM_SON_REGISTER_RESP",
    (const son_s8 *)"RRM_SON_DEREGISTER_RESP",
    (const son_s8 *)"RRM_SON_SET_ATTRIBUTE_RESP",
    (const son_s8 *)"RRM_SON_CELL_SWITCH_ON_RESP",
    (const son_s8 *)"RRM_SON_CELL_SWITCH_OFF_RESP",
    (const son_s8 *)"RRM_SON_CELL_STATE_CHANGE_IND",
    (const son_s8 *)"RRM_SON_NUM_ACTIVE_UE_REPORT",
    (const son_s8 *)"RRM_UE_COUNT_THRESHOLD_REACHED_IND",
    (const son_s8 *)"RRM_SON_NMM_PREPARE_RESP",
    (const son_s8 *)"RRM_SON_NMM_COMPLETE_RESP",
    (const son_s8 *)"RRM_SON_MEAS_CONFIG_RESP",
    (const son_s8 *)"RRM_SON_MEAS_RESULTS_IND",
    (const son_s8 *)"RRM_SON_HO_REPORT",
    (const son_s8 *)"RRM_SON_TNL_DISCOVERY_RESP",
    (const son_s8 *)"RRM_SON_HO_ATTEMPT_IND",
    (const son_s8 *)"RRM_SON_HO_FAILURE_REPORT",
    (const son_s8 *)"RRM_SON_LOAD_REPORT_IND",
    (const son_s8 *)"RRM_SON_HO_EXPIRE_IND",
    (const son_s8 *)"RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP",
    (const son_s8 *)"RRM_SON_RACH_CONFIG_RESP",
    (const son_s8 *)"RRM_SON_RACH_UE_INFO_IND",
    (const son_s8 *)"RRM_SON_L2_RACH_PERF_REPORT",
    (const son_s8 *)"RRM_SON_EICIC_ENABLE_DISABLE_IND"
};

const son_s8 *SON_PROCEDURE_CODE_NAMES[] = 
{
    (const son_s8 *)"SONNMM_INIT_IND",
    (const son_s8 *)"SONNMM_START_SCAN_REQ",
    (const son_s8 *)"SONNMM_PERIODIC_SCAN_REQ",
    (const son_s8 *)"SONNMM_STOP_SCAN_REQ",
    (const son_s8 *)"SONNMM_START_SCAN_RES",
    (const son_s8 *)"SONNMM_STOP_SCAN_RES",
    (const son_s8 *)"SONNMM_LAST_SCAN_TIMESTAMP_IND",
    (const son_s8 *)"SONANR_ENABLE_REQ",
    (const son_s8 *)"SONANR_DISABLE_REQ",
    (const son_s8 *)"SONANR_NR_ADD_REQ",
    (const son_s8 *)"SONANR_NR_DELETE_REQ",
    (const son_s8 *)"SONANR_NR_UPDATE_REQ",
    (const son_s8 *)"SONANR_NR_RETRIEVE_REQ",
    (const son_s8 *)"SONANR_NR_SCAN_REQ",
    (const son_s8 *)"SONANR_MEAS_CONFIG_REQ",
    (const son_s8 *)"SONANR_PCI_REG_DEREG_REQ",
    (const son_s8 *)"SONANR_PCI_IN_USE_LIST_REQ",
    (const son_s8 *)"SONANR_MODIFY_UE_COUNT_REQ",
    (const son_s8 *)"SONANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ",
    (const son_s8 *)"SONANR_NR_DELETE_FROM_REMOVE_LIST_REQ",
    (const son_s8 *)"SONANR_MODIFY_REMOVAL_ATTRIBUTES_REQ",
    (const son_s8 *)"SONANR_MODIFY_PCI_CONFUSION_CFG_REQ",
    (const son_s8 *)"SONANR_ENABLE_DISABLE_X2_REQ",
    (const son_s8 *)"SONANR_ENABLE_DISABLE_LIMITED_MODE_REQ",
    (const son_s8 *)"SONANR_NR_ENB_ADD_REQ",
    (const son_s8 *)"SONANR_NR_ENB_UPDATE_REQ",
    (const son_s8 *)"SONANR_NR_ENB_DELETE_REQ",
    (const son_s8 *)"SONANR_NR_ENB_RETRIEVE_REQ",
    (const son_s8 *)"SONANR_NEIGHBOR_DEFAULT_CONFIG_REQ",
    /* SPR-10331 Fix Starts */
    /* Line Deleted */
    /* SPR-10331 Fix Ends */
    (const son_s8 *)"SONANR_MEAS_CONFIG_OBJ_REMOVE_REQ",
    /* SPR 20653 Fix Start */
    (const son_s8 *)"SONANR_MODIFY_ATTRIBUTE_REQ",
    /* SPR 20653 Fix End */
    (const son_s8 *)"SONANR_ENABLE_RES",
    (const son_s8 *)"SONANR_DISABLE_RES",
    (const son_s8 *)"SONANR_NR_ADD_RES",
    (const son_s8 *)"SONANR_NR_DELETE_RES",
    (const son_s8 *)"SONANR_NR_UPDATE_RES",
    (const son_s8 *)"SONANR_NR_RETRIEVE_RES",
    (const son_s8 *)"SONANR_NR_SCAN_RES",
    (const son_s8 *)"SONANR_MEAS_CONFIG_RES",
    (const son_s8 *)"SONANR_PCI_REG_DEREG_RES",
    (const son_s8 *)"SONANR_PCI_IN_USE_LIST_RES",
    (const son_s8 *)"SONANR_PCI_CONFLICT_CONFUSION_IND",
    (const son_s8 *)"SONANR_UPDATED_NRT_INFO",
    (const son_s8 *)"SONANR_MODIFY_UE_COUNT_RES",
    (const son_s8 *)"SONANR_MODIFY_MEAS_REPORTING_INTERVAL_RES",
    (const son_s8 *)"SONANR_NR_DELETE_FROM_REMOVE_LIST_RES",
    (const son_s8 *)"SONANR_MODIFY_REMOVAL_ATTRIBUTES_RES",
    (const son_s8 *)"SONANR_MODIFY_PCI_CONFUSION_CFG_RES",
    (const son_s8 *)"SONANR_ENABLE_DISABLE_X2_RES",
    (const son_s8 *)"SONANR_ENABLE_DISABLE_LIMITED_MODE_RES",
    (const son_s8 *)"SONANR_NR_ENB_ADD_RESP",
    (const son_s8 *)"SONANR_NR_ENB_UPDATE_RESP",
    (const son_s8 *)"SONANR_NR_ENB_DELETE_RESP",
    (const son_s8 *)"SONANR_NR_ENB_RETRIEVE_RESP",
    (const son_s8 *)"SONANR_LIMITED_MODE_PCI_INFO",
    (const son_s8 *)"SONANR_FEATURE_STATE_CHANGE_IND",
    (const son_s8 *)"SONANR_DISABLE_IND",
    (const son_s8 *)"SONANR_NEIGHBOR_DEFAULT_CONFIG_RESP",
    /* SPR-10331 Fix Starts */
    /* Line Deleted */
    /* SPR-10331 Fix Ends */
    (const son_s8 *)"SONANR_MEAS_CONFIG_OBJ_REMOVE_RES",
    /* SPR 20653 Fix Start */
    (const son_s8 *)"SONANR_MODIFY_ATTRIBUTE_RES",
    /* SPR 20653 Fix End */
    (const son_s8 *)"SONES_ENABLE_REQ",
    (const son_s8 *)"SONES_DISABLE_REQ",
    (const son_s8 *)"SONES_MODIFY_UE_COUNT_INTERVAL_REQ",
    (const son_s8 *)"SONES_MODIFY_UE_COUNT_THRESHOLD_REQ",
    (const son_s8 *)"SONES_CELL_SWITCH_ON_REQ",
    (const son_s8 *)"SONES_CELL_SWITCH_OFF_REQ",
    (const son_s8 *)"SONES_SWITCHED_OFF_CELL_INFO_REQ",
    (const son_s8 *)"SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ",
    (const son_s8 *)"SONES_FEATURE_STATE_CHANGE_IND",
    /* SPR_19619 start */
    (const son_s8 *)"SONES_PEER_CELL_ACTIVATION_REQ",
    /* SPR_19619 stop */
    (const son_s8 *)"SONES_ENABLE_RES",
    (const son_s8 *)"SONES_DISABLE_RES",
    (const son_s8 *)"SONES_MODIFY_UE_COUNT_INTERVAL_RES",
    (const son_s8 *)"SONES_MODIFY_UE_COUNT_THRESHOLD_RES",
    (const son_s8 *)"SONES_CELL_SWITCH_ON_RES",
    (const son_s8 *)"SONES_CELL_SWITCH_OFF_RES",
    (const son_s8 *)"SONES_SWITCHED_OFF_CELL_INFO_RES",
    (const son_s8 *)"SONES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP",
    (const son_s8 *)"SONES_ACTIVE_UE_COUNT_REPORT",
    (const son_s8 *)"SONES_THRESHOLD_HIT_IND",
    (const son_s8 *)"SONES_REQUEST_TO_SWITCH_ON_CELL",
    (const son_s8 *)"SONES_CELL_SWITCH_ON_OFF_IND",
    (const son_s8 *)"SONAPPS_NW_SCAN_REQ",
    (const son_s8 *)"SONAPPS_CARRIER_FREQ_SELECTION_REQ",
    (const son_s8 *)"SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_REQ",
    (const son_s8 *)"SONAPPS_PCI_SELECTION_REQ",
    (const son_s8 *)"SONAPPS_PCI_RESELECTION_REQ",
    (const son_s8 *)"SONAPPS_PERIODIC_NW_SCAN_REQ",
    (const son_s8 *)"SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_REQ",
    (const son_s8 *)"SONAPPS_NW_SCAN_RES",
    (const son_s8 *)"SONAPPS_CARRIER_FREQ_SELECTION_RES",
    (const son_s8 *)"SONAPPS_PCI_CONFLICT_ENABLE_DISABLE_RES",
    (const son_s8 *)"SONAPPS_PCI_SELECTION_RES",
    (const son_s8 *)"SONAPPS_PCI_RESELECTION_RES",
    (const son_s8 *)"SONAPPS_PCI_CONFLICT_CONFUSION_IND",
    (const son_s8 *)"SONAPPS_CARRIER_FREQ_AND_DL_TX_POWER_RESP",
    (const son_s8 *)"SONMRO_ENABLE_REQ",
    (const son_s8 *)"SONMRO_DISABLE_REQ",
    (const son_s8 *)"SONMRO_MODIFY_CONFIG_PARAM_REQ",
    (const son_s8 *)"SONMRO_ENABLE_RES",
    (const son_s8 *)"SONMRO_DISABLE_RES",
    (const son_s8 *)"SONMRO_MODIFY_CONFIG_PARAM_RES",
    (const son_s8 *)"SONMRO_EUTRAN_TTT_UPDATE_IND",
    (const son_s8 *)"SONMRO_FEATURE_STATE_CHANGE_IND",
    (const son_s8 *)"SONMRO_HO_COUNTER_REPORT",
    (const son_s8 *)"SONMRO_OPTIMIZATION_NOTIFICATION",
    (const son_s8 *)"SONMLB_ENABLE_REQ",
    (const son_s8 *)"SONMLB_DISABLE_REQ",
    (const son_s8 *)"SONMLB_MODIFY_ATTRIBUTES_REQ",
    (const son_s8 *)"SONMLB_MOBILITY_CHANGE_REQ",
    (const son_s8 *)"SONMLB_OFFSET_CHANGE_REQ",
    (const son_s8 *)"SONMLB_PEER_OFFSET_CHANGE_RESP",
    /*Rel 3.0 Multi Sector code changes Start*/
    (const son_s8 *)"SONMLB_INTERNAL_MOBILITY_CHANGE_REQ",
    /*Rel 3.0 Multi Sector code changes Stop*/
    (const son_s8 *)"SONMLB_ENABLE_RES",
    (const son_s8 *)"SONMLB_DISABLE_RES",
    (const son_s8 *)"SONMLB_MODIFY_ATTRIBUTES_RES",
    (const son_s8 *)"SONMLB_MOBILITY_CHANGE_ACK",
    (const son_s8 *)"SONMLB_MOBILITY_CHANGE_FAILURE",
    (const son_s8 *)"SONMLB_FEATURE_STATE_CHANGE_IND",
    (const son_s8 *)"SONMLB_HO_LOAD_PRARAMS_INFO",
    (const son_s8 *)"SONMLB_OFFSET_CHANGE_RESP",
    (const son_s8 *)"SONMLB_PEER_OFFSET_CHANGE_REQ",
    (const son_s8 *)"SONMLB_NBR_CELL_STATUS_IND",
    /*Rel 3.0 Multi Sector code changes Start*/
    (const son_s8 *)"SONMLB_INTERNAL_MOBILITY_CHANGE_ACK",
    (const son_s8 *)"SONMLB_INTERNAL_MOBILITY_CHANGE_FAILURE",
    /*Rel 3.0 Multi Sector code changes Stop*/
    (const son_s8 *)"SONRACH_OPT_ENABLE_REQ",
    (const son_s8 *)"SONRACH_OPT_DISABLE_REQ",
    (const son_s8 *)"SONRACH_OPT_MODIFY_CONFIG_PARAMS_REQ",
    (const son_s8 *)"SONRACH_OPT_MODIFY_RACH_OPTMODE_REQ",
    (const son_s8 *)"SONRACH_OPT_ENABLE_RES",
    (const son_s8 *)"SONRACH_OPT_DISABLE_RES",
    (const son_s8 *)"SONRACH_OPT_MODIFY_CONFIG_PARAMS_RES",
    (const son_s8 *)"SONRACH_OPT_MODIFY_RACH_OPTMODE_RES",
    (const son_s8 *)"SONRACH_OPT_PARAMETER_UPDATE_REQ",
    (const son_s8 *)"SONRACH_OPT_FAILURE_NOTIFICATION",
    (const son_s8 *)"SONRACH_OPT_UE_INFO_REPORT",
    (const son_s8 *)"SONRACH_OPT_L2_PERF_REPORT",
    (const son_s8 *)"SONRACH_OPT_FEATURE_STATE_CHANGE_IND",
    (const son_s8 *)"SON_OAM_INIT_IND",
    (const son_s8 *)"SON_OAM_INIT_CONFIG_REQ",
    (const son_s8 *)"SON_OAM_INIT_CONFIG_RESP",
    (const son_s8 *)"SON_OAM_SHUTDOWN_REQ",
    (const son_s8 *)"SON_OAM_SHUTDOWN_RESP",
    (const son_s8 *)"SON_OAM_SET_LOG_LEVEL_REQ",
    (const son_s8 *)"SON_OAM_SET_LOG_LEVEL_RESP",
    (const son_s8 *)"SON_OAM_LOG_ENABLE_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_LOG_ENABLE_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_CELL_INFO_IND",
    (const son_s8 *)"SON_OAM_ENABLE_DISABLE_X2_REQ",
    (const son_s8 *)"SON_OAM_ENABLE_DISABLE_X2_RESP",
    (const son_s8 *)"SON_OAM_X2_LINK_UP_IND",
    (const son_s8 *)"SON_OAM_X2_LINK_DOWN_IND",
    (const son_s8 *)"SON_OAM_SET_NW_SCAN_CONFIG_CMD",
    (const son_s8 *)"SON_OAM_PERIODIC_NW_SCAN_REQ",
    (const son_s8 *)"SON_OAM_NW_SCAN_REQ",
    (const son_s8 *)"SON_OAM_BULK_NW_SCAN_RESP",
    (const son_s8 *)"SON_OAM_PCI_CONFLICT_IND",
    (const son_s8 *)"SON_OAM_NR_SCAN_REQ",
    (const son_s8 *)"SON_OAM_NR_SCAN_RESP",
    (const son_s8 *)"SON_OAM_NR_ADD_REQ",
    (const son_s8 *)"SON_OAM_NR_ADD_RESP",
    (const son_s8 *)"SON_OAM_NR_UPDATE_REQ",
    (const son_s8 *)"SON_OAM_NR_UPDATE_RESP",
    (const son_s8 *)"SON_OAM_NR_DELETE_REQ",
    (const son_s8 *)"SON_OAM_NR_DELETE_RESP",
    (const son_s8 *)"SON_OAM_NR_RETRIEVE_REQ",
    (const son_s8 *)"SON_OAM_NR_RETRIEVE_RESP",
    (const son_s8 *)"SON_OAM_ANR_UPDATED_NRT_INFO",
    (const son_s8 *)"SON_OAM_ANR_ENABLE_REQ",
    (const son_s8 *)"SON_OAM_ANR_ENABLE_RESP",
    (const son_s8 *)"SON_OAM_ANR_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_ANR_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_ANR_MEAS_CONFIG_REQ",
    (const son_s8 *)"SON_OAM_ANR_MEAS_CONFIG_RESP",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_UE_COUNT_REQ",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_UE_COUNT_RESP",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_REQ",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_MEAS_REPORTING_INTERVAL_RESP",
    (const son_s8 *)"SON_OAM_NR_DELETE_FROM_REMOVE_LIST_REQ",
    (const son_s8 *)"SON_OAM_NR_DELETE_FROM_REMOVE_LIST_RESP",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_REQ",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_REMOVAL_ATTRIBUTES_RESP",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_REQ",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_PCI_CONFUSION_CFG_RESP",
    (const son_s8 *)"SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_REQ",
    (const son_s8 *)"SON_OAM_ANR_ENABLE_DISABLE_LIMITED_MODE_RESP",
    (const son_s8 *)"SON_OAM_NR_ENB_ADD_REQ",
    (const son_s8 *)"SON_OAM_NR_ENB_ADD_RESP",
    (const son_s8 *)"SON_OAM_NR_ENB_UPDATE_REQ",
    (const son_s8 *)"SON_OAM_NR_ENB_UPDATE_RESP",
    (const son_s8 *)"SON_OAM_NR_ENB_DELETE_REQ",
    (const son_s8 *)"SON_OAM_NR_ENB_DELETE_RESP",
    (const son_s8 *)"SON_OAM_NR_ENB_RETRIEVE_REQ",
    (const son_s8 *)"SON_OAM_NR_ENB_RETRIEVE_RESP",
    (const son_s8 *)"SON_OAM_ANR_LIMITED_MODE_PCI_INFO",
    (const son_s8 *)"SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_REQ",
    (const son_s8 *)"SON_OAM_ANR_NEIGHBOR_DEFAULT_CONFIG_RESP",
    (const son_s8 *)"SON_OAM_ANR_GU_GROUP_ID_CONFIG_REQ",
    (const son_s8 *)"SON_OAM_ANR_GU_GROUP_ID_CONFIG_RESP",
    (const son_s8 *)"SON_OAM_ES_ENABLE_REQ",
    (const son_s8 *)"SON_OAM_ES_ENABLE_RESP",
    (const son_s8 *)"SON_OAM_ES_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_ES_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_REQ",
    (const son_s8 *)"SON_OAM_ES_MODIFY_UE_COUNT_INTERVAL_RESP",
    (const son_s8 *)"SON_OAM_ES_ACTIVE_UE_COUNT_REPORT",
    (const son_s8 *)"SON_OAM_ES_CELL_SWITCH_OFF_REQ",
    (const son_s8 *)"SON_OAM_ES_CELL_SWITCH_OFF_RESP",
    (const son_s8 *)"SON_OAM_ES_CELL_SWITCH_ON_REQ",
    (const son_s8 *)"SON_OAM_ES_CELL_SWITCH_ON_RESP",
    (const son_s8 *)"SON_OAM_ES_SWITCHED_OFF_CELL_INFO_REQ",
    (const son_s8 *)"SON_OAM_ES_SWITCHED_OFF_CELL_INFO_RESP",
    (const son_s8 *)"SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_REQ",
    (const son_s8 *)"SON_OAM_ES_MODIFY_UE_COUNT_THRESHOLD_RESP",
    (const son_s8 *)"SON_OAM_ES_UE_COUNT_THRESHOLD_HIT_IND",
    (const son_s8 *)"SON_OAM_ES_REQUEST_TO_SWITCH_ON_CELL",
    (const son_s8 *)"SON_OAM_ES_CELL_SWITCH_ON_OFF_IND",
    (const son_s8 *)"SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_PCI_CONFLICT_ENABLE_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_PCI_SELECTION_REQ",
    (const son_s8 *)"SON_OAM_PCI_SELECTION_RESP",
    (const son_s8 *)"SON_OAM_PCI_RESELECTION_REQ",
    (const son_s8 *)"SON_OAM_PCI_RESELECTION_RESP",
    (const son_s8 *)"SON_OAM_PCI_CONFLICT_CONFUSION_IND",
    (const son_s8 *)"SON_OAM_CARRIER_FREQ_SELECTION_REQ",
    (const son_s8 *)"SON_OAM_CARRIER_FREQ_SELECTION_RESP",
    (const son_s8 *)"SON_OAM_PROC_SUP_REQ",
    (const son_s8 *)"SON_OAM_PROC_SUP_RESP",
    (const son_s8 *)"SON_OAM_FEATURE_STATE_CHANGE_IND",
    (const son_s8 *)"SON_OAM_MRO_ENABLE_REQ",
    (const son_s8 *)"SON_OAM_MRO_ENABLE_RESP",
    (const son_s8 *)"SON_OAM_MRO_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_MRO_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_MRO_MODIFY_CONFIG_PARAM_REQ",
    (const son_s8 *)"SON_OAM_MRO_MODIFY_CONFIG_PARAM_RESP",
    (const son_s8 *)"SON_OAM_MRO_EUTRAN_TTT_UPDATE_IND",
    (const son_s8 *)"SON_OAM_MRO_HO_COUNTER_REPORT",
    (const son_s8 *)"SON_OAM_MRO_OPTIMIZATION_NOTIFICATION",
    (const son_s8 *)"SON_OAM_MLB_ENABLE_REQ",
    (const son_s8 *)"SON_OAM_MLB_ENABLE_RESP",
    (const son_s8 *)"SON_OAM_MLB_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_MLB_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_MLB_MODIFY_ATTRIBUTES_REQ",
    (const son_s8 *)"SON_OAM_MLB_MODIFY_ATTRIBUTES_RESP",
    (const son_s8 *)"SON_OAM_EVENT_NOTIFICATION",
    (const son_s8 *)"SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_REQ",
    (const son_s8 *)"SON_OAM_CARRIER_FREQ_AND_DL_UL_POWER_SELECTION_RES",
    (const son_s8 *)"SON_OAM_LAST_SCAN_TIMESTAMP_IND",
    (const son_s8 *)"SON_OAM_MLB_PRARAMS_INFO",
    (const son_s8 *)"SON_OAM_MLB_OFFSET_CHANGE_REQ",
    (const son_s8 *)"SON_OAM_MLB_OFFSET_CHANGE_RESP",
    (const son_s8 *)"SON_OAM_MLB_PEER_OFFSET_CHANGE_REQ",
    (const son_s8 *)"SON_OAM_MLB_PEER_OFFSET_CHANGE_RESP",
    (const son_s8 *)"SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_REQ",
    (const son_s8 *)"SON_OAM_ES_AUTONOMOUS_SWITCH_OFF_CONFIG_RESP",
    (const son_s8 *)"SON_OAM_CHK_HEALTH_REQ",
    (const son_s8 *)"SON_OAM_CHK_HEALTH_RESP",
    (const son_s8 *)"SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_REQ",
    (const son_s8 *)"SON_OAM_ANR_MEAS_CONFIG_OBJ_REMOVE_RESP",
    (const son_s8 *)"SON_OAM_RACH_OPT_ENABLE_REQ",
    (const son_s8 *)"SON_OAM_RACH_OPT_ENABLE_RESP",
    (const son_s8 *)"SON_OAM_RACH_OPT_DISABLE_REQ",
    (const son_s8 *)"SON_OAM_RACH_OPT_DISABLE_RESP",
    (const son_s8 *)"SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_REQ",
    (const son_s8 *)"SON_OAM_RACH_OPT_MODIFY_RACH_OPTMODE_RESP",
    (const son_s8 *)"SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_REQ",
    (const son_s8 *)"SON_OAM_RACH_OPT_MODIFY_CONFIG_PARAMS_RESP",
    (const son_s8 *)"SON_OAM_RACH_OPT_PARAMETER_UPDATE_REQ",
    (const son_s8 *)"SON_OAM_RACH_OPT_FAILURE_NOTIFICATION",
    (const son_s8 *)"SON_OAM_RACH_OPT_UE_INFO_REPORT",
    (const son_s8 *)"SON_OAM_RACH_OPT_L2_PERF_REPORT",
    /* Rel 3.0 Cell delete support changes Start */
    (const son_s8 *)"SON_OAM_CELL_DELETE_REQ",
    (const son_s8 *)"SON_OAM_CELL_DELETE_RESP",
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    (const son_s8 *)"SON_OAM_GET_LOG_LEVEL_REQ",
    (const son_s8 *)"SON_OAM_GET_LOG_LEVEL_RESP",
    /* SPR_19619 start */
    (const son_s8 *)"SON_OAM_PEER_CELL_ACTIVATION_REQ",
    (const son_s8 *)"SON_OAM_PEER_CELL_ACTIVATION_RESP",
    /* SPR_19619 stop */
    /* SPR-13251 Fix Ends */
    (const son_s8 *)"SMIF_INIT_CONFIG_REQ",
    (const son_s8 *)"SMIF_SHUTDOWN_REQ",
    /* Rel 3.0 Cell delete support changes Start */
    (const son_s8 *)"SMIF_CELL_DELETE_REQ",
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    (const son_s8 *)"SMIF_GET_LOG_LEVEL_REQ",
    /* SPR-13251 Fix Ends */
    (const son_s8 *)"SMIF_INIT_CONFIG_RES",
    (const son_s8 *)"SMIF_SHUTDOWN_RES",
    (const son_s8 *)"SMIF_GENERIC_RES",
    (const son_s8 *)"SMIF_LOG_ENABLE_DISABLE_CMD",
    (const son_s8 *)"SMIF_SET_LOG_LEVEL_CMD",
    (const son_s8 *)"SMIF_CELL_INFO_IND",
    (const son_s8 *)"SMIF_INIT_IND",
    (const son_s8 *)"SMIF_SET_SCAN_CONFIG_CMD",
    /* Rel 3.0 Cell delete support changes Start */
    (const son_s8 *)"SMIF_CELL_DELETE_RESP",
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    (const son_s8 *)"SMIF_GET_LOG_LEVEL_RESP",
     /* SPR_19619 start */
    (const son_s8 *)"SMIF_PEER_CELL_ACTIVATION_RESP",
     /* SPR_19619 stop */
    /* SPR-13251 Fix Ends */
    (const son_s8 *)"SONX2_ADD_ENB_REQ",
    (const son_s8 *)"SONX2_DELETE_ENB_REQ",
    (const son_s8 *)"SONX2_NRT_UPDATE_IND",
    (const son_s8 *)"SONX2_ENB_RESET_REQ",
    (const son_s8 *)"SONX2_SWITCH_ON_OFF_IND",
    (const son_s8 *)"SONX2_MOBILITY_CHANGE_REQ",
    (const son_s8 *)"SONX2_MOBILITY_CHANGE_ACK",
    (const son_s8 *)"SONX2_MOBILITY_CHANGE_FAILURE",
    /* SPR-10331 Fix Starts */
    (const son_s8 *)"SONX2_GU_GROUP_ID_CONFIG_REQ",
     /* SPR_19619 start */
    (const son_s8 *)"SONX2_PEER_CELL_ACTIVATION_REQ",
     /* SPR_19619 stop */
    /* SPR-10331 Fix Ends */
    (const son_s8 *)"SONX2_ADD_ENB_RES",
    (const son_s8 *)"SONX2_LINK_UP_IND",
    (const son_s8 *)"SONX2_LINK_DOWN_IND",
    (const son_s8 *)"SONX2_DELETE_ENB_RES",
    (const son_s8 *)"SONX2_ENB_CONFIG_UPDATE_IND",
    (const son_s8 *)"SONX2_ENB_RESET_RES",
    (const son_s8 *)"SONX2_HO_REPORT_IND",
    (const son_s8 *)"SONX2AP_HO_REPORT_IND",
    /* SPR-10331 Fix Starts */
    (const son_s8 *)"SONX2_GU_GROUP_ID_CONFIG_RES",
    /* SPR-10331 Fix Ends */
    (const son_s8 *)"SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ",
    (const son_s8 *)"SONES_CELL_ACTIVATION_REQ",
    (const son_s8 *)"SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES",
    (const son_s8 *)"SONES_CELL_ACTIVATION_RESP",
     /* SPR_19619 start */
    (const son_s8 *)"SONX2_PEER_CELL_ACTIVATION_RESP",
     /* SPR_19619 stop */
    /* Rel 3.0 Multi Sector code changes Start */
    (const son_s8 *)"SONANR_CELL_SWITCH_ON_OFF_IND",
    /* Rel 3.0 Multi Sector code changes Stop */
    /* SPR 20653 Fix Start */
    (const son_s8 *)"SON_OAM_ANR_MODIFY_ATTRIBUTE_REQ",
    (const son_s8 *)"SON_OAM_ANR_MODIFY_ATTRIBUTE_RESP",
    /* SPR 20653 Fix End */
    (const son_s8 *)"NOT_AVAILABLE"
};
/* Bug 121 Fix Ends */

/*  RA_CONFIG_FDD_DATA_FOR_PREAMBLE - Array storing possible values of 
    Preamble format and density value for all possible PRACH configuraion index     for frame type 1. PRACH configuraion index range from 0 to 63 so each entry
    in this array corresponds to data for particular PRACH configuration index
    starting at index 0.
 */
const frame_type_1_ra_config_data_for_preamble RA_CONFIG_FDD_DATA_FOR_PREAMBLE[
SON_MAX_PRACH_CONFIG_IDX] =
{
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 0 */
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 1 */
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 2 */
    {PREAMBLE_FORMAT_0, 1.0}, /* PRACH configuration Index 3 */
    {PREAMBLE_FORMAT_0, 1.0}, /* PRACH configuration Index 4 */
    {PREAMBLE_FORMAT_0, 1.0}, /* PRACH configuration Index 5 */
    {PREAMBLE_FORMAT_0, 2.0}, /* PRACH configuration Index 6 */
    {PREAMBLE_FORMAT_0, 2.0}, /* PRACH configuration Index 7 */
    {PREAMBLE_FORMAT_0, 2.0}, /* PRACH configuration Index 8 */
    {PREAMBLE_FORMAT_0, 3.0}, /* PRACH configuration Index 9 */
    {PREAMBLE_FORMAT_0, 3.0}, /* PRACH configuration Index 10 */
    {PREAMBLE_FORMAT_0, 3.0}, /* PRACH configuration Index 11 */
    {PREAMBLE_FORMAT_0, 5.0}, /* PRACH configuration Index 12 */
    {PREAMBLE_FORMAT_0, 5.0}, /* PRACH configuration Index 13 */
    {PREAMBLE_FORMAT_0, 10.0}, /* PRACH configuration Index 14 */
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 15 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 16 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 17 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 18 */
    {PREAMBLE_FORMAT_1, 1.0}, /* PRACH configuration Index 19 */
    {PREAMBLE_FORMAT_1, 1.0}, /* PRACH configuration Index 20 */
    {PREAMBLE_FORMAT_1, 1.0}, /* PRACH configuration Index 21 */
    {PREAMBLE_FORMAT_1, 2.0}, /* PRACH configuration Index 22 */
    {PREAMBLE_FORMAT_1, 2.0}, /* PRACH configuration Index 23 */
    {PREAMBLE_FORMAT_1, 2.0}, /* PRACH configuration Index 24 */
    {PREAMBLE_FORMAT_1, 3.0}, /* PRACH configuration Index 25 */
    {PREAMBLE_FORMAT_1, 3.0}, /* PRACH configuration Index 26 */
    {PREAMBLE_FORMAT_1, 3.0}, /* PRACH configuration Index 27 */
    {PREAMBLE_FORMAT_1, 5.0}, /* PRACH configuration Index 28 */
    {PREAMBLE_FORMAT_1, 5.0}, /* PRACH configuration Index 29 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 30 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 31 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 32 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 33 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 34 */
    {PREAMBLE_FORMAT_2, 1.0}, /* PRACH configuration Index 35 */
    {PREAMBLE_FORMAT_2, 1.0}, /* PRACH configuration Index 36 */
    {PREAMBLE_FORMAT_2, 1.0}, /* PRACH configuration Index 37 */
    {PREAMBLE_FORMAT_2, 2.0}, /* PRACH configuration Index 38 */
    {PREAMBLE_FORMAT_2, 2.0}, /* PRACH configuration Index 39 */
    {PREAMBLE_FORMAT_2, 2.0}, /* PRACH configuration Index 40 */
    {PREAMBLE_FORMAT_2, 3.0}, /* PRACH configuration Index 41 */
    {PREAMBLE_FORMAT_2, 3.0}, /* PRACH configuration Index 42 */
    {PREAMBLE_FORMAT_2, 3.0}, /* PRACH configuration Index 43 */
    {PREAMBLE_FORMAT_2, 5.0}, /* PRACH configuration Index 44 */
    {PREAMBLE_FORMAT_2, 5.0}, /* PRACH configuration Index 45 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 46 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 47 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 48 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 49 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 50 */
    {PREAMBLE_FORMAT_3, 1.0}, /* PRACH configuration Index 51 */
    {PREAMBLE_FORMAT_3, 1.0}, /* PRACH configuration Index 52 */
    {PREAMBLE_FORMAT_3, 1.0}, /* PRACH configuration Index 53 */
    {PREAMBLE_FORMAT_3, 2.0}, /* PRACH configuration Index 54 */
    {PREAMBLE_FORMAT_3, 2.0}, /* PRACH configuration Index 55 */
    {PREAMBLE_FORMAT_3, 2.0}, /* PRACH configuration Index 56 */
    {PREAMBLE_FORMAT_3, 3.0}, /* PRACH configuration Index 57 */
    {PREAMBLE_FORMAT_3, 3.0}, /* PRACH configuration Index 58 */
    {PREAMBLE_FORMAT_3, 3.0}, /* PRACH configuration Index 59 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 60 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 61 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 62 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 63 */
};

/*  RA_CONFIG_TDD_DATA_FOR_PREAMBLE - Array storing possible values of 
    Preamble format and density value for all possible PRACH configuraion index     for frame type 2. PRACH configuraion index range from 0 to 63 so each entry
    in this array corresponds to data for particular PRACH configuration index
    starting at index 0.
 */
const frame_type_2_ra_config_data_for_preamble RA_CONFIG_TDD_DATA_FOR_PREAMBLE[
SON_MAX_PRACH_CONFIG_IDX] = 
{
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 0 */
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 1 */
    {PREAMBLE_FORMAT_0, 0.5}, /* PRACH configuration Index 2 */
    {PREAMBLE_FORMAT_0, 1.0}, /* PRACH configuration Index 3 */
    {PREAMBLE_FORMAT_0, 1.0}, /* PRACH configuration Index 4 */
    {PREAMBLE_FORMAT_0, 1.0}, /* PRACH configuration Index 5 */
    {PREAMBLE_FORMAT_0, 2.0}, /* PRACH configuration Index 6 */
    {PREAMBLE_FORMAT_0, 2.0}, /* PRACH configuration Index 7 */
    {PREAMBLE_FORMAT_0, 2.0}, /* PRACH configuration Index 8 */
    {PREAMBLE_FORMAT_0, 3.0}, /* PRACH configuration Index 9 */
    {PREAMBLE_FORMAT_0, 3.0}, /* PRACH configuration Index 10 */
    {PREAMBLE_FORMAT_0, 3.0}, /* PRACH configuration Index 11 */
    {PREAMBLE_FORMAT_0, 4.0}, /* PRACH configuration Index 12 */
    {PREAMBLE_FORMAT_0, 4.0}, /* PRACH configuration Index 13 */
    {PREAMBLE_FORMAT_0, 4.0}, /* PRACH configuration Index 14 */
    {PREAMBLE_FORMAT_0, 5.0}, /* PRACH configuration Index 15 */
    {PREAMBLE_FORMAT_0, 5.0}, /* PRACH configuration Index 16 */
    {PREAMBLE_FORMAT_0, 5.0}, /* PRACH configuration Index 17 */
    {PREAMBLE_FORMAT_0, 6.0}, /* PRACH configuration Index 18 */
    {PREAMBLE_FORMAT_0, 6.0}, /* PRACH configuration Index 19 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 20 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 21 */
    {PREAMBLE_FORMAT_1, 0.5}, /* PRACH configuration Index 22 */
    {PREAMBLE_FORMAT_1, 1.0}, /* PRACH configuration Index 23 */
    {PREAMBLE_FORMAT_1, 1.0}, /* PRACH configuration Index 24 */
    {PREAMBLE_FORMAT_1, 2.0}, /* PRACH configuration Index 25 */
    {PREAMBLE_FORMAT_1, 3.0}, /* PRACH configuration Index 26 */
    {PREAMBLE_FORMAT_1, 4.0}, /* PRACH configuration Index 27 */
    {PREAMBLE_FORMAT_1, 5.0}, /* PRACH configuration Index 28 */
    {PREAMBLE_FORMAT_1, 6.0}, /* PRACH configuration Index 29 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 30 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 31 */
    {PREAMBLE_FORMAT_2, 0.5}, /* PRACH configuration Index 32 */
    {PREAMBLE_FORMAT_2, 1.0}, /* PRACH configuration Index 33 */
    {PREAMBLE_FORMAT_2, 1.0}, /* PRACH configuration Index 34 */
    {PREAMBLE_FORMAT_2, 2.0}, /* PRACH configuration Index 35 */
    {PREAMBLE_FORMAT_2, 3.0}, /* PRACH configuration Index 36 */
    {PREAMBLE_FORMAT_2, 4.0}, /* PRACH configuration Index 37 */
    {PREAMBLE_FORMAT_2, 5.0}, /* PRACH configuration Index 38 */
    {PREAMBLE_FORMAT_2, 6.0}, /* PRACH configuration Index 39 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 40 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 41 */
    {PREAMBLE_FORMAT_3, 0.5}, /* PRACH configuration Index 42 */
    {PREAMBLE_FORMAT_3, 1.0}, /* PRACH configuration Index 43 */
    {PREAMBLE_FORMAT_3, 1.0}, /* PRACH configuration Index 44 */
    {PREAMBLE_FORMAT_3, 2.0}, /* PRACH configuration Index 45 */
    {PREAMBLE_FORMAT_3, 3.0}, /* PRACH configuration Index 46 */
    {PREAMBLE_FORMAT_3, 4.0}, /* PRACH configuration Index 47 */
    {PREAMBLE_FORMAT_4, 0.5}, /* PRACH configuration Index 48 */
    {PREAMBLE_FORMAT_4, 0.5}, /* PRACH configuration Index 49 */
    {PREAMBLE_FORMAT_4, 0.5}, /* PRACH configuration Index 50 */
    {PREAMBLE_FORMAT_4, 1.0}, /* PRACH configuration Index 51 */
    {PREAMBLE_FORMAT_4, 1.0}, /* PRACH configuration Index 52 */
    {PREAMBLE_FORMAT_4, 2.0}, /* PRACH configuration Index 53 */
    {PREAMBLE_FORMAT_4, 3.0}, /* PRACH configuration Index 54 */
    {PREAMBLE_FORMAT_4, 4.0}, /* PRACH configuration Index 55 */
    {PREAMBLE_FORMAT_4, 5.0}, /* PRACH configuration Index 56 */
    {PREAMBLE_FORMAT_4, 6.0}, /* PRACH configuration Index 57 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 58 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 59 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 60 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 61 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 62 */
    {PREAMBLE_FORMAT_NA, 0.0}, /* PRACH configuration Index 63 */
};

const son_8 *DUPLEX_MODE_OF_CELL[SON_MAX_NUM_OF_DUPLEX_MODES] =
                    {"SON_FDD_MODE","SON_TDD_MODE" };

/* SPR-11639 Fix Starts */
/* Possible CIO Enum value names */
const son_s8 *CIO_NAMES[] =
{
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_24",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_22",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_20",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_18",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_16",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_14",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_12",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_10",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_8",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_6",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_5",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_4",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_3",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_2",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB_1",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB0",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB1",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB2",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB3",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB4",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB5",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB6",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB8",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB10",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB12",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB14",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB16",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB18",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB20",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB22",
    (const son_s8 *)"SON_Q_OFFSET_RANGE_DB24",
    (const son_s8 *)"NOT_AVAILABLE"
};
/* SPR-11639 Fix Ends */

/* Used in logger for identifying SON UTILS */
const son_8 *son_utils_facility_name = "SON UTILS";

/* Variable storing the status of threads in SON*/
static struct timeval son_thread_state[SON_MAX_NUMBER_OF_THREADS];

extern pthread_key_t cell_and_ue_index_key;

#if (LOGGER_TYPE == SON_ADV_LOGGER)

AdvL_ContextIndex g_advl_ctxt[SON_MAX_INTERNAL_MODULES];

/*****************************************************************************
 * Function Name  : son_init_log
 * Inputs         : p_log_file_name - Pointer to the log file name
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to initialize advance logger
 ******************************************************************************/
son_void_t
son_init_log
(
    const son_8 *p_log_file_name
)
{
    son_u16 module_id = 0;

    module_id = qvGetServiceName(qvSelf());

    if (0 != module_id)
    {
        if (SON_PNULL == p_log_file_name)
        {
/* coverity_88701 start */
	    if(LogSuccess != init_log(SON_LOG_CTXT_FILE, SON_DEFAULT_LOG_FILE,
				&g_advl_ctxt[module_id - SON_MIN_INT_MODULE_ID]))
	    {
		return ;
	    }
/* coverity_88701 stop */
        }
        else
        {
/* coverity_88701 start */
            if(LogSuccess != init_log(SON_LOG_CTXT_FILE, p_log_file_name,
                &g_advl_ctxt[module_id - SON_MIN_INT_MODULE_ID]))
	    {
		return ;
	    }
/* coverity_88701 stop */
        }
    }
} /* son_init_log */

/*****************************************************************************
 * Function Name  : son_set_module_loglevel_limit
 * Inputs         : log_level - Log level to be used(SON_ERROR, SON_BRIEF etc.)
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the module log level to advance logger
 ****************************************************************************/
son_void_t
son_set_module_loglevel_limit
(
    son_u8 log_level
)
{
    son_u8 son_log_level = 0xFF;
    son_u16 module_id = 0;
    /* Bug 121 Fix Start */
    son_u16 module_name_index = 0;
    /* Bug 121 Fix Ends */

    module_id = qvGetServiceName(qvSelf());

    /*Coverity CID 81011 Fix Start - If clause modified*/

    if((module_id >= SON_MIN_INT_MODULE_ID) && (module_id <= SON_MAX_MODULE_ID))
    {

        switch (log_level)
        {
            case SON_OAM_LOG_LEVEL_CRITICAL:
            {
                son_log_level = SON_CRITICAL;
                break;
            }

            case SON_OAM_LOG_LEVEL_ERROR:
            {
                son_log_level = SON_ERROR;
                break;
            }

            case SON_OAM_LOG_LEVEL_WARNING:
            {
                son_log_level = SON_WARNING;
                break;
            }

            case SON_OAM_LOG_LEVEL_INFO:
            {
                son_log_level = SON_INFO;
                break;
            }

            case SON_OAM_LOG_LEVEL_BRIEF:
            {
                son_log_level = SON_BRIEF;
                break;
            }

            case SON_OAM_LOG_LEVEL_DETAILED:
            {
                son_log_level = SON_DETAILED;
                break;
            }

            case SON_OAM_LOG_LEVEL_DETAILEDALL:
            {
                son_log_level = SON_DETAILEDALL;
                break;
            }

            default:
            {
                son_log_level = SON_LOG_LEVEL_NONE;
                break;
            }
        } /* switch */

    /* Coverity ID 81011 Fix Start*/
    if (0xFF != son_log_level && log_level <= SON_OAM_LOG_LEVEL_DETAILEDALL && SON_MIN_INT_MODULE_ID <= module_id)
        /* Coverity ID 81011 Fix End*/
        {
            set_loglevel_limit(g_advl_ctxt[module_id - SON_MIN_INT_MODULE_ID],
                    (AdvL_LogLevel)son_log_level);
            /* Bug 121 Fix Start */
            module_name_index = module_id - SON_MIN_MODULE_ID;
            if (module_name_index < (sizeof(SON_INTERNAL_MODULE_NAMES) / 
                        sizeof(SON_INTERNAL_MODULE_NAMES[0])))
            {
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                        "Log level is set to %s "
                        "for %s", p_son_log_level_names[log_level], 
                        SON_INTERNAL_MODULE_NAMES[module_name_index]);
            }
            /* Bug 121 Fix Ends */
        }
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                "Module_id %d "
                "is not valid", module_id);
    }
} /* son_set_module_loglevel_limit */

#elif (LOGGER_TYPE == SON_INTERNAL_LOGGER) || (LOGGER_TYPE == SON_IN_MEM_LOGGER)

/*****************************************************************************
 * Function Name  : son_trace_message
 * Inputs         : log_level - log level of this message
 *                  p_facility_name - Pointer to the string used to identify
 *                  who logs message
 *                  p_format - Pointer to the format specifier string
 *                  ... - Any number of optional arguments
 * Outputs        : None
 * Returns        : None
 * Description    : This function implements the trace functionality.
 ****************************************************************************/
son_void_t
son_trace_message
(
    son_s32 log_level,         /* log level of this message */
    const son_8    *p_facility_name,   /* identify who log message */
    const son_s8    *p_format,          /* format string like in printf */
    ...                                 /* optional parameters */
)
{
    son_s8 tmp_fmt[FMT_MAX_LEN + 1] = {0};
    son_size_t temp_facility_len = 0;

    va_list arglist;

    SON_VA_START(arglist, p_format);

    if (SON_PNULL != p_facility_name)
    {
        temp_facility_len = SON_STRLEN(p_facility_name);

        /* 1 is added to the length to store the NULL
         *  character */
        SON_SNPRINTF((char *)tmp_fmt, temp_facility_len + 1, "%s",
            (const char *)p_facility_name);
    }

    switch (log_level)
    {
        case SON_CRITICAL:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <CRITICAL>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        case SON_ERROR:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <ERROR>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        case SON_WARNING:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <WARNING>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        case SON_INFO:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <INFO>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        case SON_BRIEF:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <BRIEF>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        case SON_DETAILED:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <DETAILED>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        case SON_COMP_PARSE:
        {
            SON_SNPRINTF(tmp_fmt + temp_facility_len,
                FMT_MAX_LEN - temp_facility_len,
                " <DETAILEDALL>: %s", (char *)p_format);
            SON_QVLOGVA(log_level, tmp_fmt, arglist);
            break;
        }

        default:
        {
            SON_QVLOGVA(log_level, p_format, arglist);
            break;
        }
    } /* switch */

    SON_VA_END(arglist);
} /* son_trace_message */

/*****************************************************************************
 * Function Name  : set_module_log_level
 * Inputs         : log_level - Log level to be used(SON_ERROR, SON_BRIEF etc.)
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the module log level to CSPL logging
 *                  framework.
 ****************************************************************************/
son_void_t
set_module_log_level
(
    son_u8 log_level
)
{
    son_u8 son_log_level = 0xFF;
    /* Bug 121 Fix Start */
    son_u16 module_name_index = 0;
    /* Bug 121 Fix Ends */

    switch (log_level)
    {
        case SON_OAM_LOG_LEVEL_CRITICAL:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_CRITICAL;
/*SPR 22364 Fix End*/
            break;
        }

        case SON_OAM_LOG_LEVEL_ERROR:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_ERROR;
/*SPR 22364 Fix End*/
            break;
        }

        case SON_OAM_LOG_LEVEL_WARNING:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_WARNING;
/*SPR 22364 Fix End*/
            break;
        }

        case SON_OAM_LOG_LEVEL_INFO:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_INFO;
/*SPR 22364 Fix End*/
            break;
        }

        case SON_OAM_LOG_LEVEL_BRIEF:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_BRIEF;
/*SPR 22364 Fix End*/
            break;
        }

        case SON_OAM_LOG_LEVEL_DETAILED:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_DETAILED;
/*SPR 22364 Fix End*/
            break;
        }

        case SON_OAM_LOG_LEVEL_DETAILEDALL:
        {
/*SPR 22364 Fix Start*/
            son_log_level = SON_DETAILEDALL;
/*SPR 22364 Fix End*/
            break;
        }

        default:
        {
/*SPR 22364 Fix Start*/
	/*Code deleted*/
/*SPR 22364 Fix End*/
            break;
        }
    } /* switch */

    if (0xFF != son_log_level && log_level < SON_MAX_NO_OF_LOG_LEVELS)
    {
       if (0 != qvSelf())
        {
            qvSetModuleLogLevel(qvSelf(), son_log_level);
            /* Bug 121 Fix Start */
            module_name_index = qvName() - SON_MIN_MODULE_ID;
            if (module_name_index < (sizeof(SON_INTERNAL_MODULE_NAMES) /
                        sizeof(SON_INTERNAL_MODULE_NAMES[0])))
            {
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                        "Log level is set to %s for %s",
                        p_son_log_level_names[log_level], 
                        SON_INTERNAL_MODULE_NAMES[module_name_index]);
            }
            /* Bug 121 Fix Ends */
        }
        else
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                    "qvSelf returned NULL");
        }
    }
} /* set_module_log_level */

#endif

/*****************************************************************************
 * Function Name  : son_assert
 * Inputs         : p_file - Pointer to the file name
 *                  line - Line number containing the assert statement
 *                  p_expression - Expression which cause the assert
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to exit using the panic function
 *                  of CSPL. It prints the file name, line number and the
 *                  expression because of which the program needs to terminate
 ****************************************************************************/
/*SPR-12970-Start - SON_DEBUG applied to son_assert*/
#ifdef SON_DEBUG
son_void_t
son_assert
(
    void    *p_file,
    son_u32 line,
    void    *p_expression
)
{
    qvPanic("[%s:%u] Assertion Failed <%s>", (son_u8 *)p_file, line,
        (son_u8 *)p_expression);
    son_abort();
}
#endif /* ifdef SON_DEBUG */
/*SPR-12970-End*/

/*****************************************************************************
 * Function Name  : son_fill_time_stamp
 * Inputs         : p_time_stamp -  pointer to time stamp structure
 * Outputs        : None
 * Returns        : Pointer to pointer to time stamp structure
 * Description    : This function fills current time stamp.
 ****************************************************************************/
static son_void_t
son_fill_time_stamp
(
    son_oam_time_stamp_t *p_time_stamp
)
{
    SON_UT_TRACE_ENTER();

    time_t currentTimestamp;
    struct tm  *currentTime;

    son_time(&currentTimestamp);

    /*localtime function will fill the structure currentTime*/
    currentTime = son_localtime(&currentTimestamp);
    if (SON_PNULL != currentTime)
    {
        p_time_stamp->year  = (1900 + currentTime->tm_year);
        p_time_stamp->month = ++currentTime->tm_mon;
        p_time_stamp->day   = currentTime->tm_mday;
        p_time_stamp->hour  = currentTime->tm_hour;
        p_time_stamp->min   = currentTime->tm_min;
        p_time_stamp->sec   = currentTime->tm_sec;
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "localtime function not working properly ");
    }

    SON_UT_TRACE_EXIT();
} /* son_fill_time_stamp */

/*****************************************************************************
 * Function Name  : son_send_oam_notification
 * Inputs         : event_type - Type of event
 *                  event_sub_type - Subtype of event
 *                  event_id - Event identity
 *                  error_info - Error information
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function send SON_OAM_EVENT_NOTIFICATION to OAM.
 ****************************************************************************/
son_return_et
son_send_oam_notification
(
    son_oam_event_class_et event_type,
    son_oam_event_subclass_et event_sub_type,
    son_u16 event_id,
    son_oam_error_info_t error_info
)
{
    SON_UT_TRACE_ENTER();
    son_oam_event_notification_t event_notify;
    son_u16 module_id = 0;

    SON_MEMSET(&event_notify, 0, sizeof(event_notify));

    /*get module id*/
    module_id = qvGetServiceName(qvSelf());
    if (0 != module_id)
    {
        event_notify.bitmask |= SON_OAM_EVENT_API_DATA_PRESENT;
        /*----------fill event header-------------
         *fill time stamp*/
        son_fill_time_stamp(&event_notify.msg_header.time_stamp);
        /*fill event type data*/
        event_notify.msg_header.event_type = event_type;
        /*fill event sub-type data*/
        event_notify.msg_header.event_sub_type = event_sub_type;
        /*fill event id.right now only 1 event_id is presnet*/
        event_notify.msg_header.event_id = event_id;

        /*----------fill api data-------------*/
        event_notify.api_data.error_type = error_info.error_type;
        event_notify.api_data.bitmask |= error_info.bitmask;
        event_notify.api_data.error_cause = error_info.error_cause;
        event_notify.api_data.bitmask |= error_info.bitmask;
        event_notify.api_data.severity = error_info.severity;
        son_oam_send_son_oam_event_notification(&event_notify,
            module_id, SON_OAM_MODULE_ID,
            SON_NULL, SON_NULL);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Invalid module Id[0] ");
        return SON_FAILURE;
    }

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_send_oam_notification */

/*****************************************************************************
 * Function Name  : son_mem_get
 * Inputs         : size - Size of memory to be allocated
 * Outputs        : None
 * Returns        : Pointer to memory buffer or NULL in case of failure
 * Description    : This function returns memory buffer from memory pool.
 *                  Function is used for general memory management purposes.
 ****************************************************************************/
son_void_t *
son_mem_get
(
    son_size_t size /* Size of buffer which will be allocated */
)
{
    son_void_t *p_buf = SON_PNULL;
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));
#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = son_malloc(size);
#else
    p_buf = qvAlloc(size, SON_PNULL);
#endif
    if (p_buf == SON_PNULL)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Memory allocation failed");
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.error_type = SON_OAM_MEMORY_UNAVAILABLE_ERROR;
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        error_info.error_cause = SON_OAM_NO_FREE_BUFFER;
        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;

        event_id = (son_u16)SON_OAM_MEMORY_ALLOCATION_FAIL;
        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_MEMORY_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }

    return p_buf;
} /* son_mem_get */

/*****************************************************************************
 * Function Name  : son_mem_free
 * Inputs         : p_buffer -  Pointer to memory buffer to be freed
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees memory buffer allocated in pool.
 *                  Function is used for general memory management purposes.
 ****************************************************************************/
son_void_t
son_mem_free
(
    son_void_t *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free(p_buffer);
#else
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));

    if (p_buffer != SON_PNULL)
    {
        qvFree(p_buffer);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_WARNING,
            "null pointer passed");
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        error_info.error_cause = SON_OAM_INVALID_PARAMS;
        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;

        event_id = (son_u16)SON_OAM_MEMORY_FREE_FAIL;
        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_MEMORY_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }
#endif
} /* son_mem_free */

/*****************************************************************************
 * Function Name  : son_sys_mem_get
 * Inputs         : size -  Size of buffer which will be allocated
 * Outputs        : None
 * Returns        : Pointer to memory buffer or NULL in case of failure
 * Description    : This function returns memory buffer using direct OS call
 *                  allocation routine. Function is used for managing
 *                  static buffers like son_gb_ctx.
 ****************************************************************************/
son_void_t *
son_sys_mem_get
(
    son_size_t size /* Size of buffer which will be allocated */
)
{
    void *p_buf = SON_PNULL;

#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = son_malloc(size);
#else
    p_buf = (void *)qvSysMalloc(size);
#endif
    if (SON_PNULL == p_buf)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Memory allocation failed");
    }

    return (p_buf);
}

/*****************************************************************************
 * Function Name  : son_sys_mem_free
 * Inputs         : p_buffer - Pointer to buffer which will be freed
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees memory buffer using direct OS free
 *                  routine
 ****************************************************************************/
son_void_t
son_sys_mem_free
(
    son_void_t *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free(p_buffer);
#else
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));

    if (SON_PNULL != p_buffer)
    {
        qvSysFree(p_buffer);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_WARNING,
            "null pointer passed");
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        error_info.error_cause = SON_OAM_INVALID_PARAMS;
        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;

        event_id = (son_u16)SON_OAM_MEMORY_FREE_FAIL;
        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_MEMORY_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }
#endif
} /* son_sys_mem_free */

/*****************************************************************************
 * Function Name  : son_start_timer
 * Inputs         : timer_duration - Duration (in seconds) of the timer
 *                  to be started
 *                  p_timer_data - Pointer to the timer  data that will be
 *                  attached to timer
 *                  timer_data_size - Size of timer data
 *                  is_repeated - Indicates whether timer is repeated
 * Outputs        : None
 * Returns        : Timer Id as allocated by the CSPL
 * Description    : This function is used for starting timer by SON.
 *                  Timer allocates buffer which has size timer_data_size and
 *                  this data will be freed in son_stop_timer.
 ****************************************************************************/
son_timer_t
son_start_timer
(
    /* SPR-12708 Fix Starts */
    /* Duration (in seconds) of the timer to be started */
    /* SPR-12708 Fix Ends */
    son_u32 timer_duration,
    /* Timer data that will be attached to timer */
    void        *p_timer_data,
    son_u16 timer_data_size,   /* Size of timer data */
    son_bool_et is_repeated         /* Indicates whether timer is repeated */
)
{
    QTIME qtime;
    void *p_timer_buf = SON_PNULL;
    son_timer_t timer = {0};
    /*Start: OAM notification changes*/
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));
    /*End: OAM notification changes*/

    SON_ASSERT(0 != timer_duration);

    if (SON_PNULL != p_timer_data)
    {
        p_timer_buf = son_mem_get(timer_data_size);

        if (SON_PNULL == p_timer_buf)
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "Failed to get memory");
            return SON_PNULL;
        }

        SON_MEMCPY(p_timer_buf, p_timer_data, timer_data_size);
    }

    /* SPR-12708 Fix Starts */
    qtime.s = timer_duration;
    qtime.us = 0;
    /* SPR-12708 Fix Ends */

    timer = qvTimerStart(&qtime, p_timer_buf, is_repeated);

    if (SON_PNULL == timer)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Timer Start failed");

        /* Release the memory allocated for storing the timer data */
        son_mem_free(p_timer_buf);
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
        error_info.error_cause = SON_OAM_OTHER;
        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;
        event_id = (son_u16)SON_OAM_TIMER_START_FAIL;
        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_TIMER_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
            "%s Timer Id [%p]: Duration: [%d]msec", (SON_TRUE == is_repeated) ? "Repeated" : "Non Repeated",
            timer, timer_duration);
    }

    return timer;
} /* son_start_timer */

/*****************************************************************************
 * Function Name  : son_stop_timer
 * Inputs         : timer - Timer Id as allocated by the CSPL
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for stopping timer by SON.
 ****************************************************************************/
son_void_t
son_stop_timer
(
    son_timer_t timer /* Timer Id as allocated by the CSPL */
)
{
    if (SON_PNULL == timer)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "NULL timer passed!");
    }
    else
    {
        void *p_timer_data = SON_PNULL;
        /*Start: OAM notification changes*/
        son_u16 event_id = SON_NULL;
        son_oam_error_info_t error_info;

        SON_MEMSET(&error_info, 0, sizeof(error_info));
        /*End: OAM notification changes*/

        if (SON_TRUE == son_is_timer_running(timer))
        {
            p_timer_data = qvTimerData(timer);

            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                "Stop Timer Id [0x%x]", timer);

            if (SON_PNULL != p_timer_data)
            {
                son_mem_free(p_timer_data);
            }
            else
            {
                /*+ SPR 11452 Fix +*/
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                    "Timer Id [0x%x] contains "
                    "null data", timer);
                /*- SPR 11452 Fix -*/
            }

            qvTimerStop(timer, SON_PNULL);
        }
        else
        {
            /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

            /*fill error info structure*/
            error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
            error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
            error_info.error_cause = SON_OAM_TIMER_ALREADY_STOP;
            error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
            error_info.severity = SON_OAM_MAJOR_ERROR;

            event_id = (son_u16)SON_OAM_TIMER_STOP_FAIL;
            if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_TIMER_ERROR,
                    event_id, error_info))
            {
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                    "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
            }
        }
    }
} /* son_stop_timer */

/*****************************************************************************
 * Function Name  : son_get_timer_data
 * Inputs         : timer - Timer Id as allocated by the CSPL
 * Outputs        : None
 * Returns        : Pointer to the data associated with given timer
 * Description    : This function is used for retrieving timer data
 ****************************************************************************/
son_void_t *
son_get_timer_data
(
    son_timer_t timer /* Timer Id as allocated by the CSPL */
)
{
    void *p_timer_data = SON_PNULL;

    if (SON_PNULL == timer)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "NULL timer passed!");
    }
    else
    {
        p_timer_data = qvTimerData(timer);
    }

    return p_timer_data;
}

/*****************************************************************************
 *SPR 20172 FIX End
 * Function Name  : son_send_msg_ex
 *SPR 20172 FIX End
 * Inputs         : p_msg - Pointer to the messge to be send
 *                  dst_module_id - destination Module Id
 * Outputs        : None
 * Returns        : None
 * Description    : This function send the message to the destination module
 *                  using qvSend
 ****************************************************************************/
son_void_t
/*SPR 20172 FIX Start*/
son_send_msg_ex
/*SPR 20172 FIX End*/
(
    son_void_t  *p_msg,         /* Message that will be passed */
    son_u16 dst_module_id /* Module id for which message will be passed */
)
{
    son_s32 error_code = 0;
    QMODULE service = qvGetServiceEx(dst_module_id, (int *)&error_code);

    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;
/* Bug 121 Fix Start */
    son_u16 dest_index = 0;
/* Bug 121 Fix Ends */

    SON_MEMSET(&error_info, 0, sizeof(error_info));

    if (SON_PNULL == p_msg)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Pointer to the message is NULL");
    }
    else if (QVERROR_NONE != error_code)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Cannot get service name[%s], dst_module_id[%u]",
            error_code == QVERROR_INVALID_ARGUMENT ?
            "invalid argument passed" :
            "module allocation failed", dst_module_id);
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        if (error_code == QVERROR_INVALID_ARGUMENT)
        {
            error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
            error_info.error_cause = SON_OAM_INVALID_PARAMS;
        }
        else
        {
            error_info.error_type = SON_OAM_MEMORY_UNAVAILABLE_ERROR;
            error_info.error_cause = SON_OAM_NO_FREE_BUFFER;
        }

        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;
        event_id = (son_u16)SON_OAM_SEND_FAIL;

        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_SOCKET_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }
    else
    {
        son_u16 api_id   = son_get_word_from_header(&(((U8 *)p_msg)[5]));
        
        /* SPR 17498 changes start*/
        /* CV_HDRSIZE is decremented here as composer adds CSPL Header
         *          * to Message Length */
/* coverity_94854 start */
        son_u16 msg_size = (son_get_word_from_header(&(((U8 *)p_msg)[7])) - (son_u16)CV_HDRSIZE);
/* coverity_94854 stop */
        /* SPR 17498 changes end*/
        if((dst_module_id == SON_OAM_MODULE_ID) ||
                (dst_module_id == SON_RRM_MODULE_ID) )
        {
            SON_LOG_HEX_DUMP("SEND", p_msg, msg_size, dst_module_id, api_id,
                    SON_VERSION_ID);
        }

        qvSend(service, 0, p_msg);
/* Bug 121 Fix Start */
        if ((SON_MIN_MODULE_ID <= dst_module_id) && 
                (SON_MAX_MODULE_ID >= dst_module_id))
        {
            dest_index = dst_module_id - SON_MIN_MODULE_ID; 

            if (dest_index < (sizeof(SON_INTERNAL_MODULE_NAMES) /
                        sizeof(SON_INTERNAL_MODULE_NAMES[0])))
            {
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
                        "Message with destination %s is written"
                        " into cspl message queue", 
                        SON_INTERNAL_MODULE_NAMES[dest_index]);
            }
        }
        else if (SON_OAM_MODULE_ID == dst_module_id)
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
                    "Message with destination LTE OAM is written"
                    " into cspl message queue");
        }
        else if (SON_RRM_MODULE_ID == dst_module_id)
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
                    "Message with destination LTE RRM is written"
                    " into cspl message queue");
        }
        else if (SON_RRC_MODULE_ID == dst_module_id)
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
                    "Message with destination LTE RRC is written"
                    " into cspl message queue");
        }
        else if (SON_PHY_MODULE_ID == dst_module_id)
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
                    "Message with destination LTE PHY is written"
                    " into cspl message queue");
        }
        else
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
                    "Message with destination module Id = %u "
                    "is written into cspl message queue", dst_module_id);
        }
    }
    /* Bug 121 Fix Ends */
} /* son_send_msg */

/*****************************************************************************
 * Function Name  : son_msg_mem_get
 * Inputs         : size - Size of buffer which will be allocated
 * Outputs        : None
 * Returns        : Pointer to the allocated buffer
 * Description    : This function returns memory buffer from memory pool.
 *                  Function is used for messages management purposes.
 ****************************************************************************/
son_void_t *
son_msg_mem_get
(
    son_size_t size     /* Size of buffer which will be allocated */
)
{
    son_void_t *p_buf = SON_PNULL;
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));

#ifdef MEM_CHECK_DOUBLE_FREE
    p_buf = son_malloc(size);
#else
    p_buf = qvMsgAlloc(SON_PNULL, 0, 0, size);
#endif
    if (SON_PNULL == p_buf)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "qvMsgAlloc failed");
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.error_type = SON_OAM_MEMORY_UNAVAILABLE_ERROR;
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        error_info.error_cause = SON_OAM_NO_FREE_BUFFER;
        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;

        event_id = (son_u16)SON_OAM_MEMORY_ALLOCATION_FAIL;
        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_MEMORY_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }

    return p_buf;
} /* son_msg_mem_get */

/*****************************************************************************
 * Function Name  : son_msg_mem_free
 * Inputs         : p_buffer - Pointer to the buffer which will be freed
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees memory buffer allocated to memory pool.
 *                  Function is used for messages management purposes.
 ****************************************************************************/
son_void_t
son_msg_mem_free
(
    son_void_t *p_buffer /* Pointer to buffer which will be freed */
)
{
#ifdef MEM_CHECK_DOUBLE_FREE
    free(p_buffer);
#else
    son_u16 event_id = SON_NULL;
    son_oam_error_info_t error_info;

    SON_MEMSET(&error_info, 0, sizeof(error_info));

    if (SON_PNULL != p_buffer)
    {
        qvMsgFree(p_buffer);
    }
    else
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_WARNING,
            "NULL pointer passed");
        /*send SON_OAM_EVENT_NOTIFICATION to OAM*/

        /*fill error info structure*/
        error_info.error_type = SON_OAM_SW_PROCESSING_ERROR;
        error_info.bitmask |= SON_OAM_EVENT_ERROR_CAUSE_PRESENT;
        error_info.error_cause = SON_OAM_INVALID_PARAMS;
        error_info.bitmask |= SON_OAM_EVENT_SEVERITY_PRESENT;
        error_info.severity = SON_OAM_MAJOR_ERROR;

        event_id = (son_u16)SON_OAM_MEMORY_FREE_FAIL;
        if (SON_SUCCESS == son_send_oam_notification(SON_OAM_ERROR_EVENT, SON_OAM_MEMORY_ERROR,
                event_id, error_info))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                "SON_OAM_EVENT_NOTIFICATION SUCCESSFULLY SENT");
        }
    }
#endif
} /* son_msg_mem_free */

/*****************************************************************************
 * Function Name  : son_construct_cspl_header
 * Inputs         : p_buf - Pointer to the buffer where CSPL header needs to be
 *                  added
 *                  version - version of API
 *                  from - Source module Id
 *                  to - Destination module Id
 *                  api_id - API Identifier
 *                  priority - Priority of the message
 *                  paramlen - length of the parameter
 * Outputs        : None
 * Returns        : None
 * Description    : This functions appends the CSPL Header on the top of
 *                  the passed buffer.
 ****************************************************************************/
son_void_t
son_construct_cspl_header
(
    son_void_t  *p_buf,
    son_u8 version,
    son_u16 from,
    son_u16 to,
    son_u16 api_id,
    son_u8 priority,
    son_u16 paramlen
)
{
    STACKAPIHDR cspl_hdr;

    if (SON_PNULL == p_buf)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_WARNING,
            "NULL pointer passed");
    }
    else
    {
        cspl_hdr.priority = priority;
        cspl_hdr.version = version;
        cspl_hdr.from = from;
        cspl_hdr.to = to;
        cspl_hdr.paramlen = paramlen;
        cspl_hdr.payloadlen = 0;
        cspl_hdr.api = api_id;

        /* Append CSPL Header on top of buf_p.
         *  The buf_len is not being used so this function is not returning
         *  this value. */
        cvApiBuildHeader((son_u8 *)p_buf, &cspl_hdr);
    }
} /* son_construct_cspl_header */

/*****************************************************************************
 * Function Name  : son_parse_cspl_header
 * Inputs         : p_buf - Pointer to the buffer where CSPL header is present
 * Outputs        : p_cspl_hdr - Pointer to CSPL header
 * Returns        : None
 * Description    : This functions parse the CSPL Header and populates the
 *                  STACKAPIHDR structure
 ****************************************************************************/
son_void_t
son_parse_cspl_header
(
    son_void_t  *p_buf,
    STACKAPIHDR *p_cspl_hdr
)
{
    if (SON_PNULL == p_buf || SON_PNULL == p_cspl_hdr)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_WARNING,
            "NULL pointer passed");
    }
    else
    {
        /* The len_msg is not being used so this function is not returning
         *  this value. */
        /* SPR 19078: CID 88716 Fix Start */
        (void)cvApiParseHeader(p_buf, CV_HDRSIZE, p_cspl_hdr);
        /* SPR 19078: CID 88716 Fix End */
    }
}


/*****************************************************************************
 * Function Name  : son_get_word_from_header
 * Inputs         : p_header - Pointer to the location from where data is to
 *                  be read.
 * Outputs        : None
 * Returns        : 2 bytes data read
 * Description    : It is used to read a word of 2 bytes from given memory
 *                  location.
 ****************************************************************************/
son_u16
son_get_word_from_header
(
    son_u8 *p_header
)
{
    son_u16 value = 0xFFFF;

    if (SON_PNULL == p_header)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_WARNING,
            "Null data pointer received");
    }
    else
    {
        value = (son_u16)(p_header[0] << 8 | p_header[1]);
    }

    return value;
}

/* Bug 121 Fix Start */
/*****************************************************************************
 * Function Name  : son_find_index_of_procedure_code_array
 * Inputs         : api     Procedure code
 * Outputs        : None
 * Returns        : The array index of SON_PROCEDURE_CODE_NAMES array.
 * Description    : This function is used to find the array index of 
 *                  SON_PROCEDURE_CODE_NAMES array to print the API name in logs
 ****************************************************************************/
static son_u32
son_find_index_of_procedure_code_array
(
    son_procedure_code_et api
)
{
    son_u32 api_index = SON_PROCEDURE_UNDEFINED;
    son_u32 base_index_for_sonanr_req_msgs = NMM_NMMCTL_UNDEFINED - 
        NMM_FSM_EX_MSG_BASE + 1; /* 1 has been added for SONNMM_INIT_IND API */
    son_u32 base_index_for_sonanr_res_msgs = (SONANR_REQ_MAX_API - 
            SONANR_REQ_BASE_API_ID + 1) + base_index_for_sonanr_req_msgs;
    son_u32 base_index_for_sones_req_msgs = (SONANR_RES_MAX_API - 
            SONANR_RES_BASE_API_ID + 1) + base_index_for_sonanr_res_msgs;
    son_u32 base_index_for_sones_res_msgs = (SONES_REQ_MAX_API - 
            SONES_REQ_BASE_API_ID  + 1) + base_index_for_sones_req_msgs;
    son_u32 base_index_for_sonapps_req_msgs = (SONES_RES_MAX_API - 
            SONES_RES_BASE_API_ID + 1) + base_index_for_sones_res_msgs;
    son_u32 base_index_for_sonapps_res_msgs = (SONAPPS_REQ_MAX_API - 
            SONAPPS_REQ_BASE_API_ID) + base_index_for_sonapps_req_msgs;
    son_u32 base_index_for_sonmro_req_msgs = (SONAPPS_RES_MAX_API - 
            SONAPPS_RES_BASE_API_ID) + base_index_for_sonapps_res_msgs;
    son_u32 base_index_for_sonmro_res_msgs = (SONMRO_REQ_MAX_API -
            SONMRO_REQ_BASE_API_ID) + base_index_for_sonmro_req_msgs;
    son_u32 base_index_for_sonmlb_req_msgs = (SONMRO_RES_MAX_API - 
            SONMRO_RES_BASE_API_ID) + base_index_for_sonmro_res_msgs;
    son_u32 base_index_for_sonmlb_res_msgs = (SONMLB_REQ_MAX_API - 
            SONMLB_REQ_BASE_API_ID) + base_index_for_sonmlb_req_msgs;
    son_u32 base_index_for_sonrach_req_msgs = (SONMLB_RES_MAX_API -
            SONMLB_RES_BASE_API_ID) + base_index_for_sonmlb_res_msgs;
    son_u32 base_index_for_sonrach_res_msgs = (SONRACH_OPT_REQ_MAX_API - 
            SONRACH_OPT_REQ_BASE_API_ID) + base_index_for_sonrach_req_msgs;
    son_u32 base_index_for_son_oam_intf_msgs = (SONRACH_OPT_RES_MAX_API - 
            SONRACH_OPT_RES_BASE_API_ID) + base_index_for_sonrach_res_msgs; 
    son_u32 base_index_for_smif_req_msgs = (SON_OAM_MAX_API_ID - 
            SON_OAM_INIT_IND) + base_index_for_son_oam_intf_msgs;
    son_u32 base_index_for_smif_res_msgs = (SMIF_REQ_MAX_API -
            SMIF_REQ_BASE_API_ID) + base_index_for_smif_req_msgs;
    son_u32 base_index_for_sonx2_req_msgs = (SMIF_RES_MAX_API - 
            SMIF_RES_BASE_API_ID) + base_index_for_smif_res_msgs;
    son_u32 base_index_for_sonx2_res_msgs = (SONX2_REQ_MAX_API - 
            SONX2_REQ_BASE_API_ID + 1) + base_index_for_sonx2_req_msgs;
    son_u32 base_index_for_anr_es_req_msgs = (SONX2_RES_MAX_API - 
            SONX2_RES_BASE_API_ID + 1) + base_index_for_sonx2_res_msgs;
    son_u32 base_index_for_anr_es_res_msgs = (SONES_X2_REQ_MAX_API - 
            SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ + 1) + 
        base_index_for_anr_es_req_msgs;
    SON_UT_TRACE_ENTER();

    if (SONNMM_INIT_IND == api)
    {
        api_index = 0;
    }
    else if (api >= NMM_FSM_EX_MSG_BASE && api < NMM_NMMCTL_UNDEFINED)
    {
        api_index = api - NMM_FSM_EX_MSG_BASE + 1;
    }
    else if (api >= SONANR_REQ_BASE_API_ID && api <= SONANR_REQ_MAX_API)
    {
        api_index = api - SONANR_REQ_BASE_API_ID + 
            base_index_for_sonanr_req_msgs;
    }
    else if (api >= SONANR_RES_BASE_API_ID && api <= SONANR_RES_MAX_API)
    {
        api_index = api - SONANR_RES_BASE_API_ID + 
            base_index_for_sonanr_res_msgs;
    }
    else if (api >= SONES_REQ_BASE_API_ID && api <= SONES_REQ_MAX_API)
    {
        api_index = api - SONES_REQ_BASE_API_ID + base_index_for_sones_req_msgs;
    }
    else if (api >= SONES_RES_BASE_API_ID && api <= SONES_RES_MAX_API)
    {
        api_index = api - SONES_RES_BASE_API_ID + base_index_for_sones_res_msgs;
    }
    else if (api >= SONAPPS_REQ_BASE_API_ID && api < SONAPPS_REQ_MAX_API)
    {
        api_index = api - SONAPPS_REQ_BASE_API_ID + 
            base_index_for_sonapps_req_msgs;
    }
    else if (api >= SONAPPS_RES_BASE_API_ID && api < SONAPPS_RES_MAX_API)
    {
        api_index = api - SONAPPS_RES_BASE_API_ID + 
            base_index_for_sonapps_res_msgs;
    }
    else if (api >= SONMRO_REQ_BASE_API_ID && api < SONMRO_REQ_MAX_API)
    {
        api_index = api - SONMRO_REQ_BASE_API_ID + 
            base_index_for_sonmro_req_msgs;
    }
    else if (api >= SONMRO_RES_BASE_API_ID && api < SONMRO_RES_MAX_API)
    {
        api_index = api - SONMRO_RES_BASE_API_ID + 
            base_index_for_sonmro_res_msgs;
    }
    else if (api >= SONMLB_REQ_BASE_API_ID && api < SONMLB_REQ_MAX_API)
    {
        api_index = api - SONMLB_REQ_BASE_API_ID + 
            base_index_for_sonmlb_req_msgs;
    }
    else if (api >= SONMLB_RES_BASE_API_ID && api < SONMLB_RES_MAX_API)
    {
        api_index = api - SONMLB_RES_BASE_API_ID + 
            base_index_for_sonmlb_res_msgs;
    }
    else if (api >= SONRACH_OPT_REQ_BASE_API_ID && 
            api < SONRACH_OPT_REQ_MAX_API)
    {
        api_index = api - SONRACH_OPT_REQ_BASE_API_ID + 
            base_index_for_sonrach_req_msgs;
    }
    else if (api >= SONRACH_OPT_RES_BASE_API_ID && 
            api < SONRACH_OPT_RES_MAX_API)
    {
        api_index = api - SONRACH_OPT_RES_BASE_API_ID + 
            base_index_for_sonrach_res_msgs;
    }
    else if (api >= SON_OAM_INIT_IND && api < SON_OAM_MAX_API_ID)
    {
        api_index = api - SON_OAM_INIT_IND + base_index_for_son_oam_intf_msgs;
    }
    else if (api >= SMIF_REQ_BASE_API_ID && api < SMIF_REQ_MAX_API)
    {
        api_index = api - SMIF_REQ_BASE_API_ID + base_index_for_smif_req_msgs;
    }
    else if (api >= SMIF_RES_BASE_API_ID && api < SMIF_RES_MAX_API)
    {
        api_index = api - SMIF_RES_BASE_API_ID + base_index_for_smif_res_msgs;
    }
    else if (api >= SONX2_REQ_BASE_API_ID && api < SONX2_REQ_MAX_API)
    {
        api_index = api - SONX2_REQ_BASE_API_ID + base_index_for_sonx2_req_msgs;
    }
    else if (api >= SONX2_RES_BASE_API_ID && api < SONX2_RES_MAX_API)
    {
        api_index = api - SONX2_RES_BASE_API_ID + base_index_for_sonx2_res_msgs;
    }
    else if (api <= SONES_X2_REQ_MAX_API)
    {
        api_index = api - SONES_CELL_SWITCH_ON_FOR_X2_SETUP_REQ + 
            base_index_for_anr_es_req_msgs;
    }
    else if (api <= SONES_X2_RES_MAX_API)
    {
        api_index = api - SONES_CELL_SWITCH_ON_FOR_X2_SETUP_RES + 
            base_index_for_anr_es_res_msgs;
    }

    SON_UT_TRACE_EXIT();
    return api_index;
}
/* Bug 121 Fix Ends */

/*****************************************************************************
 * Function Name  : son_create_send_buffer
 * Inputs         : p_mem   Pointer to the message
 *                : from    ID of the sender
 *                : to      ID of the receiver
 *                : api      procedure code
 *                : length   length of the message
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function creates the message and send it to the
 *                   required destination.
 ****************************************************************************/
son_void_t
son_create_send_buffer
(
    son_u8                  *p_mem,
    son_module_id_et from,
    son_module_id_et to,
    son_procedure_code_et api,
    son_u16 length
)
{
    son_u8 *p_buf = SON_PNULL;
/* Bug 121 Fix Start */
    son_u32 api_index = SON_PROCEDURE_UNDEFINED;
    son_u16 from_index = 0;
    son_u16 to_index = 0;
/* Bug 121 Fix Ends */

    p_buf  =  son_msg_mem_get(CV_HDRSIZE + length);
    if (SON_PNULL == p_buf)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "memory allocation failed");
    }
    else
    {
        son_construct_cspl_header(p_buf, SON_VERSION_ID, from, to,
            api, SON_API_PRIORITY, length);

        /* Check if data is present. */
        if (0 != length && SON_PNULL != p_mem)
        {
            /* Copy the data in the message memory */
            SON_MEMCPY(p_buf + CV_HDRSIZE, p_mem, length);
        }

/* Bug 121 Fix Start */
        if (((SON_MIN_MODULE_ID <= from) && 
                    (SON_MAX_MODULE_ID >= from)) && 
                ((SON_MIN_MODULE_ID <= to) && 
                 (SON_MAX_MODULE_ID >= to)))
        {
            api_index = son_find_index_of_procedure_code_array(api);
            from_index = from - SON_MIN_MODULE_ID; 
            to_index = to - SON_MIN_MODULE_ID;

            if ((SON_PROCEDURE_UNDEFINED != api_index &&
                        (api_index < (sizeof(SON_PROCEDURE_CODE_NAMES) /
                                      sizeof(SON_PROCEDURE_CODE_NAMES[0])))) &&
                    (from_index < (sizeof(SON_INTERNAL_MODULE_NAMES) /
                                   sizeof(SON_INTERNAL_MODULE_NAMES[0]))) &&
                    (to_index < (sizeof(SON_INTERNAL_MODULE_NAMES) /
                                 sizeof(SON_INTERNAL_MODULE_NAMES[0]))))
            {
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_INFO,
                        "%s->%s:%s",
                        SON_INTERNAL_MODULE_NAMES[from_index], 
                        SON_INTERNAL_MODULE_NAMES[to_index], 
                        SON_PROCEDURE_CODE_NAMES[api_index]);
            }
        }
/* Bug 121 Fix Ends */

        son_send_msg(p_buf, to);
    }
} /* son_create_send_buffer */

/*****************************************************************************
 * Function Name  : son_exit
 * Inputs         : value - The exit value of the process
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used to exit the process.
 ****************************************************************************/
son_void_t
son_exit
(
    son_s32 value
)
{
    exit(value);
}

/*****************************************************************************
 * Function Name  : son_get_transaction_id
 * Inputs         : p_api - Pointer to the external API message
 * Outputs        : None
 * Returns        : Transaction Identifier
 * Description    : This function retreives transaction_id from external API
 *                  message
 ****************************************************************************/
son_u16
son_get_transaction_id
(
    void *p_api /* external API message */
)
{
    return (son_get_word_from_header((U8 *)p_api + CV_HDRSIZE));
}

/*****************************************************************************
 * Function Name  : son_construct_interface_api_header
 * Inputs         : p_header - SON interface header
 *                  transaction_id - Interface transaction identifier
 *                  src_module_id - Source module identifier
 *                  dst_module_id - Destination module identifie
 *                  api_id - API identifier
 *                  api_buf_size - API buffer size
 *                  cell_index - Cell Index value 
 * Outputs        : p_header - Pointer to the SON interface header constructed
 * Returns        : None
 * Description    : This function constructs interface api header from given
 *                  parameters
 ****************************************************************************/
son_void_t
son_construct_interface_api_header
(
    son_u8              *p_header,      /* SON interface header */
    son_u16 transaction_id, /* Interface transaction identifier */
    son_module_id_et src_module_id, /* Source module identifier */
    son_module_id_et dst_module_id, /* Destination module identifier */
    son_u16 api_id,        /* API identifier */
    son_u16 api_buf_size, /* API buffer size */
    son_cell_index_t cell_index /* Cell Index */
)
{
    if (SON_PNULL == p_header)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Pointer to SON interface "
            "header is NULL");
    }
    else
    {
        /* transaction identifier (HI) */
        p_header[0] = (son_u8)((transaction_id & 0xFF00) >> 8);

        /* transaction identifier (LOW) */
        p_header[1] = (son_u8)(transaction_id & 0x00FF);

        /* from (HI) */
        p_header[2] = (son_u8)((src_module_id & 0xFF00) >> 8);

        /* from (LOW) */
        p_header[3] = (son_u8)(src_module_id & 0x00FF);

        /* to (HI) */
        p_header[4] = (son_u8)((dst_module_id & 0xFF00) >> 8);

        /* to (LOW) */
        p_header[5] = (son_u8)(dst_module_id & 0x00FF);

        /* api id (HI) */
        p_header[6] = (son_u8)((api_id & 0xFF00) >> 8);

        /* api id (LOW) */
        p_header[7] = (son_u8)(api_id & 0x00FF);

        /*size includes length of header*/
        api_buf_size = (son_u16)(api_buf_size + EXT_MSG_API_HDR_SIZE);

        /* api size (HI) */
        p_header[8] = (son_u8)((api_buf_size & 0xFF00) >> 8);

        /* api size (LOW) */
        p_header[9] = (son_u8)(api_buf_size & 0x00FF);

        p_header[10] = cell_index;
    }
} /* son_construct_interface_api_header */

/*****************************************************************************
 * Function Name  : son_generate_txn_id
 * Inputs         : None
 * Outputs        : None
 * Returns        : Transcation Id
 * Description    : This function generates the transaction identity value
 ******************************************************************************/
/* + SPR 17439 */
son_u16
son_generate_txn_id(void)
/* - SPR 17439 */
{
    static son_u16 tx_id = SON_MIN_TRANSACTION_ID;

    if (tx_id < SON_MAX_TRANSACTION_ID)
    {
        tx_id++;
    }
    else
    {
        tx_id = SON_MIN_TRANSACTION_ID;
    }

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
        "Trans_Id generated = %u", tx_id);
    return tx_id;
}

/*****************************************************************************
 * Function Name  : son_compare_cell_id
 * Inputs         : p_val_1     Pointer to the cell id as void pointer
 *                : p_val_2     Pointer to the cell id as void pointer
 * Outputs        : None
 * Returns        : Zero -  If matches
 *                  Non Zero - If not matches
 * Description    : This function compares the cell ids
 ******************************************************************************/
son_s32
son_compare_cell_id
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
)
{
    son_s32 ret_value = -1;

    const son_intra_rat_global_cell_id_t  *p_cell_id_1 = SON_PNULL;
    const son_intra_rat_global_cell_id_t  *p_cell_id_2 = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Pointer to the cell Id is NULL");
    }
    else
    {
        p_cell_id_1 = (son_intra_rat_global_cell_id_t  *)p_val_1;
        p_cell_id_2 = (son_intra_rat_global_cell_id_t  *)p_val_2;
        ret_value = SON_MEMCMP(p_cell_id_1, p_cell_id_2,
            sizeof(son_intra_rat_global_cell_id_t));
    }

    SON_UT_TRACE_EXIT();
    return ret_value;
} /* son_compare_cell_id */
/*SPR_19279_START*/
son_s32
son_compare_cell_index
(
    const son_void_t *p_val_1,
    const son_void_t *p_val_2
)
{
    son_s32 ret_value = -1;

    const son_u8  *p_cell_idx_1 = SON_PNULL;
    const son_u8  *p_cell_idx_2 = SON_PNULL;

    SON_UT_TRACE_ENTER();

    if (SON_PNULL == p_val_1 || SON_PNULL == p_val_2)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Pointer to the cell Id is NULL");
    }
    else
    {
        p_cell_idx_1 = (son_u8  *)p_val_1;
        p_cell_idx_2 = (son_u8 *)p_val_2;
        ret_value = SON_MEMCMP(p_cell_idx_1, p_cell_idx_2,
            sizeof(son_u8));
    }

    SON_UT_TRACE_EXIT();
    return ret_value;
} /* son_compare_cell_index */
/*SPR_19279_END*/

/*****************************************************************************
 * Function Name  : convert_meas_bw_in_db
 * Inputs         : meas_bw   Enum value(0-5) corresponding to Number of RBs in BW
 * Outputs        : None
 * Returns        : Value of measurement bandwidth in DB or 0 in case of failure
 * Description    : This function is used to convert number of RBs into their
 *                  DB equivalent using formula 10 * log(number of RBs). The
 *                  log values for all possible RBs are already stored
 *                  in meas_log_array array.
 ******************************************************************************/
son_float
convert_meas_bw_in_db
(
    son_u8 meas_bw
)
{
    /* Array storing DB equivalent of number of RBs calculated using
     *  10 * log (number of RB) */
    son_float meas_log_array[SON_MAX_RBS] = {7.781512, 11.760912,
                                             13.97940, 16.98970, 18.75061, 20};
    son_float meas_bw_in_db = 0.0;
    son_u8 index = 0;

    /* SPR 9781 Start */
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_INFO,
        "Index value received %u", meas_bw);
    /* SPR 9781 End */

    /* Calculate the meas_log_array index for given RB */
    for (index = 0; index < SON_MAX_RBS; index++)
    {
        if (meas_bw == index)
        {
            meas_bw_in_db = meas_log_array[index];
            /* SPR 9781 Start */
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_INFO,
                "Converted Meas BW in DB %f", meas_bw_in_db);
            /* SPR 9781 End */
        }
    }

    if (0 == meas_bw_in_db)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "Failed to find the DB equivalent of "
            "number for RBs %u", meas_bw);
    }

    return meas_bw_in_db;
} /* convert_meas_bw_in_db */

/*****************************************************************************
 * Function Name  : son_is_timer_running
 * Inputs         : timer - Timer Id as allocated by the CSPL
 * Outputs        : None
 * Returns        : SON_TRUE - If timer is running,
 *                  SON_FALSE - Otherwise.
 * Description    : This function is called to check whether the timer is
 *                  running or it has already expired. User is recommended to
 *                  use this function before stopping the timer using
 *                  son_stop_timer.
 ******************************************************************************/
son_bool_et
son_is_timer_running
(
    son_timer_t timer
)
{
    son_bool_et is_running = SON_FALSE;

    SON_UT_TRACE_ENTER();
    if (SON_PNULL == timer)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
            "NULL timer passed!");
    }
    else if (1 == qvTimerRunning(timer))
    {
        is_running = SON_TRUE;
    }

    SON_UT_TRACE_EXIT();
    return is_running;
} /* son_is_timer_running */

/*****************************************************************************
 * Function Name  : son_convert_char_cell_id_to_int_cell_id
 * Inputs         : char *cell_id
 * Outputs        : None
 * Returns        : *p_cell_id
 * Description    : is used to convert a char array value to int value
 ********************************************************************************/
son_u32
son_convert_char_cell_id_to_int_cell_id
(
    son_u8 *cell_id
)
{
    son_u32 *p_cell_id = (son_u32 *)cell_id;

    return SON_NTOHL(*p_cell_id);
}

/****************************************************************************
* Function Name  : son_logger_init
* Inputs         : p_son_logger_init_data, initial data
* Outputs        : None
* Returns        : None
* Description    : Start the monitoring of the memory
****************************************************************************/
son_void_t
*son_logger_init(	
		son_void_t *p_son_logger_init_data)
{
    cpu_set_t   cpuSetSch;
    U32         coreNum = SON_NULL;
    U32         bitmap = SON_NULL;
    QTIME                   time;
    /*SPR 17777 +-*/ 
    (void)p_son_logger_init_data;
    SON_UT_TRACE_ENTER();

    SON_CPU_ZERO(&cpuSetSch);
    bitmap = g_config_params.son_logger_cfg.log_cpu_bitmap;

    if (SON_NULL != bitmap)
    {
        while (SON_NULL != bitmap)
        {
            if (bitmap & SON_ONE)
            {
                SON_CPU_SET(coreNum ,&cpuSetSch);
            }
            bitmap = bitmap >> SON_ONE;
            coreNum++;
        }
#ifdef SON_IN_MEM_LOGGER
        if (0 != pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuSetSch))
        {
            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_BRIEF,
                    "Core binding for the logger failed");
        }
#endif        
    }

    SET_MODULE_LOG_LEVEL(SON_ERROR);

    lgr_process_log_segments();

    time.s = g_config_params.son_logger_cfg.log_wait_interval;
    time.us = SON_NULL;
    qvTimerStart(&time, SON_NULL, SON_NULL);

    SON_UT_TRACE_EXIT();
    return SON_PNULL;
}

/****************************************************************************
 * Function Name  : son_logger_process_timer_msg
 * Inputs         : timer_id, p_buf, p_context
 * Outputs        : None
 * Returns        : None
 * Description    : Function for handling the timer expiry of the logger thread
 ****************************************************************************/
son_void_t
son_logger_process_timer_msg
(
 QTIMER timer_id,
 son_void_t *p_buf,
 son_void_t *p_context)
{
/*SPR 17777 +-*/ 
    (void)p_context;
    (void)timer_id;
    (void)p_buf;
    QTIME                   time;
    
    lgr_process_log_segments();

    time.s = g_config_params.son_logger_cfg.log_wait_interval;
    time.us = SON_NULL;
    qvTimerStart(&time, SON_NULL, SON_NULL);

    return;
}

static const unsigned long loggerDest[] =
{
	0
};

const   QMANIFEST sonLoggerEntity =
{
	"SON_LOGGER_MODULE_ID",
	SON_LOGGER_MODULE_ID,

	{ SON_PNULL, son_logger_init },
	{ SON_PNULL, son_logger_process_timer_msg },
	{ SON_PNULL, SON_PNULL, SON_PNULL },
	loggerDest/* Does not send message to any module */
};



/****************************************************************************
* Function Name  : son_thread_start
* Inputs         : Thread argument need for the initialization of the thread
* Outputs        : None
* Returns        : Null
* Description    : Entry function of the thread
****************************************************************************/
son_void_t
*son_thread_start(void * thread_arg)
{
    pthread_key_create(&cell_and_ue_index_key, free_cell_and_ue_index);
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);

    /*Get the logical thread id pthread_self*/
    son_thrd_arg_t *p_thread_arg = (son_thrd_arg_t *)thread_arg;
    lgr_logger_register_thread(p_thread_arg->logical_thid);
   
    qvDriverLoop(p_thread_arg->driver_id, p_thread_arg->p_qwait);

    return SON_PNULL;
}

/*****************************************************************************
 * Function Name  : son_convert_const_char_cell_id_to_int_cell_id
 * Inputs         : const char *cell_id
 * Outputs        : None
 * Returns        : *p_cell_id
 * Description    : is used to convert a const char array value to int value
 ********************************************************************************/
son_u32
son_convert_const_char_cell_id_to_int_cell_id
(
    const son_u8 *cell_id
)
{
    son_u32 *p_cell_id = (son_u32 *)cell_id;

    return *p_cell_id;
}

/*****************************************************************************
   * Function Name  : son_register_modules
   * Inputs         : None
   * Outputs        : None
   * Returns        : None
   * Description    : It registers the modules to the CSPL. The module is
   *                  described by its respective manifest data structure
   ****************************************************************************/
  son_void_t
  son_register_modules
  (
/*SPR 17777 +-*/ 
#ifdef SON_IN_MEM_LOGGER
   son_thrd_arg_t *p_thrd_arg
#else
   void
#endif
/*SPR 17777 +-*/ 
  )
  {
  #ifdef SON_IN_MEM_LOGGER
      pthread_t       tid     = SON_NULL;
  #endif
      son_u32 index = 0;
 /*SPR 22554 Fix Start*/
      son_8  threadName[SON_THREAD_NAME_LEN] = "son_logger";
 /*SPR 22554 Fix End*/
      SON_UT_TRACE_ENTER();

      for (index = 0; index < sizeof(p_manifest)/sizeof(QMANIFEST *); index++)
      {
          qvRegister(0, 0, 0, p_manifest[index], 0);
      }

  #ifdef SON_IN_MEM_LOGGER
      qvRegister(p_thrd_arg->driver_id, 0, 0, &sonLoggerEntity, 0);
     pthread_create(&tid, 0, son_thread_start, (void *)p_thrd_arg);
 /*SPR 22554 Fix Start*/

    /* Set name of the SON module thread */
    if (0 != pthread_setname_np(tid, threadName))
    {
        fprintf(stdout, "Failed to set name of the SON module thread \n");
    }
/*SPR 22554 Fix End*/
  #endif
      SON_UT_TRACE_EXIT();
      return;
}

/*****************************************************************************
 * Function Name  : son_oam_set_thread_status
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function sets the status of threads in SON
 ****************************************************************************/
son_void_t
son_oam_set_thread_status
(
 son_u8  thread_id
 )
{
    struct timeval timestamp = {SON_NULL};
    SON_UT_TRACE_ENTER();

    son_gettimeofday(&timestamp, SON_PNULL);

    son_thread_state[thread_id].tv_sec = timestamp.tv_sec;

    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_DETAILED,
            "Status marked for thread[%u]", thread_id);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_oam_get_thread_status
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function gets the status of threads in SON
 ****************************************************************************/
son_return_et
son_oam_get_thread_status
(
 son_u16   time_prd
 )
{
    son_u16 index = 0;
    struct timeval timestamp = {SON_NULL};
    son_gettimeofday(&timestamp, SON_PNULL);
    son_bool_et     all_threads_are_responsive  = SON_TRUE;
    son_return_et   ret_val                     = SON_FAILURE;

    SON_UT_TRACE_ENTER();

    for (index = 0; index < SON_MAX_NUMBER_OF_THREADS; index ++)
    {
        if (timestamp.tv_sec - son_thread_state[index].tv_sec >
                time_prd )
        {
            all_threads_are_responsive = SON_FALSE;

            SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                    "Thread[%d] is non-responsive", index);
        }
    }

    if(all_threads_are_responsive)
    {
        ret_val = SON_SUCCESS;
    }

    SON_UT_TRACE_EXIT();
    return ret_val;
}

/****************************************************************************
 * Function Name  : son_fill_param_value
 * Inputs         : param_name, readString, fptr_p and type
 * Outputs        : NONE
 * Returns        : NONE
 * Description    : This function fills param value in global cfg structure
 *****************************************************************************/

son_void_t
son_fill_param_value
( 
 void *param_name, 
 son_8 *readString, 
 FILE    *fptr_p,
 son_param_type_et type
 )
{
    son_s32     param_value = SON_NULL;
    while (SON_PNULL != son_fgets (readString, MAX_BUFF_SIZE, fptr_p))
    {
        if ('\n' == readString[SON_STRNLEN(readString, MAX_BUFF_SIZE) - 1])
        readString[SON_STRNLEN(readString, MAX_BUFF_SIZE) - 1] = '\0';
        if('#' == readString[0])
        {
            continue;
        }
        param_value = son_atoi(readString);
        switch (type)
        {        
            case CHAR_TYPE :     
                *(son_u8 *)param_name = param_value;
                break;
            case U32_TYPE : 
                *(son_u32 *)param_name = param_value;                
                break;
            case U16_TYPE : 
                *(son_u16 *)param_name = param_value;                
                break;
            case ARR_TYPE :
                SON_MEMCPY((son_u8 *)param_name, readString, SON_STRLEN(readString));
                break;
            default : 
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name, SON_ERROR,
                        "Parameter Type is incorrect ");
                break;
        }
        SON_MEMSET(readString, SON_NULL, MAX_BUFF_SIZE);
        break;
    }
}


/****************************************************************************
 * Function Name  : parse_and_store_son_oam_cfg_params
 * Inputs         : NONE
 * Outputs        : NONE
 * Returns        : SUCCESS/FAILURE
 * Description    : This function Reads the params from config file and 
                    store them in global context.
 *****************************************************************************/
/* + SPR 17439 */
son_return_et parse_and_store_son_oam_cfg_params(void)
/* - SPR 17439 */
{
    son_8    readString[MAX_BUFF_SIZE] = {0};
    son_8    *file_name = "../cfg/son_oam.cfg";
    son_u16     param_length = SON_NULL;
    FILE    *fptr_p = SON_PNULL;

    SON_UT_TRACE_ENTER();
    /*Open config the file */
    if ((fptr_p = SON_FOPEN(file_name, "r")) == (FILE *)0)
    {
        /* SPR 10897: Fix Starts */
        SON_ENCAPS_WARNING("error in opening config file %s",
                file_name);
        /* SPR 10897: Fix Ends */
        return SON_FAILURE;
    }
    while (SON_PNULL != son_fgets (readString, MAX_BUFF_SIZE, fptr_p))
    {
        if ('\n' == readString[SON_STRNLEN(readString, MAX_BUFF_SIZE) - 1])
        readString[SON_STRNLEN(readString, MAX_BUFF_SIZE) - 1] = '\0';
        param_length = SON_STRNLEN(readString, MAX_BUFF_SIZE);
        if('#' == readString[0])
        {
            continue;
        }
        else if (!SON_STRNCMP ("mro", readString, 3))
        {
            /*Klockwork Fix Starts*/
            if (!SON_STRNCMP ("mro_mode", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_mode, readString, fptr_p, U32_TYPE);
            } 
            else if (!SON_STRNCMP ("mro_param", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_param, readString, fptr_p, U32_TYPE);
            }
            else if (!SON_STRNCMP ("mro_ho_failure_accumulation_duration", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_ho_failure_accumulation_duration, readString, fptr_p, CHAR_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_ho_attempts_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_ho_attempts_threshold, readString, fptr_p, U16_TYPE);
            }
            else if (!SON_STRNCMP ("mro_ho_optimization_start_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_ho_optimization_start_threshold, readString, fptr_p, U16_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_ho_optimization_stop_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_ho_optimization_stop_threshold, readString, fptr_p, U16_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_ho_fail_rate_for_optimization", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_ho_fail_rate_for_optimization, readString, fptr_p, U16_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_ttt", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.ttt, readString, fptr_p, U32_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_expected_ho_improvement_for_single_step", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.mro_expected_ho_improvement_for_single_step, readString, fptr_p, U16_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_delta_cio", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.delta_cio, readString, fptr_p, CHAR_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_delta_ttt", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.delta_ttt, readString, fptr_p, CHAR_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_min_ttt", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.min_ttt, readString, fptr_p, U32_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_max_ttt", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.max_ttt, readString, fptr_p, U32_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_min_cio", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.min_cio, readString, fptr_p, U32_TYPE);
            }    
            else if (!SON_STRNCMP ("mro_max_cio", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mro_config_params.max_cio, readString, fptr_p, U32_TYPE);
            }    
            else
            {
                /* SPR 10897: Fix Starts */
                SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                       readString); 
                /* SPR 10897: Fix Ends */
                son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
            }    
        }
        else if (!SON_STRNCMP ("mlb", readString, 3))
        {
            if (!SON_STRNCMP ("mlb_max_ho_sample", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.max_ho_sample, readString, fptr_p, U32_TYPE);
            } 
            else if (!SON_STRNCMP ("mlb_q_offset_min", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.q_offset_min, readString, fptr_p, U32_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_q_offset_max", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.q_offset_max, readString, fptr_p, U32_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_eutra_threshold_rsrp", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.eutra_threshold_rsrp, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_small_step_cell_offset", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.small_step_cell_offset, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_eutra_load_threshold_idle", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.eutra_load_threshold_idle, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_eutra_load_threshold_conn", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.eutra_load_threshold_conn, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_eutra_offset_min_step", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.eutra_offset_min_step, readString, fptr_p, U32_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_eutra_ocn_min_step", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.eutra_ocn_min_step, readString, fptr_p, U32_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_self_load_report_interval", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.self_load_report_interval, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_nbr_load_report_interval", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.nbr_load_report_interval, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_num_of_mlb_iteration", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.num_of_mlb_iteration, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_start_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.mlb_start_threshold, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_stop_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.mlb_stop_threshold, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_min_capacity_gain", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.min_capacity_gain, readString, fptr_p, CHAR_TYPE);
            }   
            else if (!SON_STRNCMP ("mlb_mode", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.mlb_config_params.mlb_mode, readString, fptr_p, U32_TYPE);
            }   
            else
            {
                /* SPR 10897: Fix Starts */
                SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                        readString); 
                /* SPR 10897: Fix Ends */
                son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
            }    
        }    
        else if (!SON_STRNCMP ("icic", readString, 4))
        {
            if (!SON_STRNCMP ("icic_dl_tx_alpha", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.icic_config_param.dl_tx_alpha, readString, fptr_p, CHAR_TYPE);
            } 
            else if (!SON_STRNCMP ("icic_dl_tx_beta", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.icic_config_param.dl_tx_beta, readString, fptr_p, CHAR_TYPE);
            } 
            else if (!SON_STRNCMP ("icic_ul_tx_alpha", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.icic_config_param.ul_tx_alpha, readString, fptr_p, CHAR_TYPE);
            } 
            else if (!SON_STRNCMP ("icic_ul_offset_k", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.icic_config_param.ul_offset_k, readString, fptr_p, CHAR_TYPE);
            } 
            else
            {
                /* SPR 10897: Fix Starts */
                SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                        readString); 
                /* SPR 10897: Fix Ends */
                son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
            }    
        }
        else if (!SON_STRNCMP ("log", readString, 3))
        {
            if (!SON_STRNCMP ("log_wait_interval", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_wait_interval, readString, fptr_p, U16_TYPE);
            } 
            else if (!SON_STRNCMP ("log_path", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_path, readString, fptr_p, ARR_TYPE);
            } 
            else if (!SON_STRNCMP ("log_shm_size", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_shm_size, readString, fptr_p, U32_TYPE);
            } 
            else if (!SON_STRNCMP ("log_file_size", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_file_sz, readString, fptr_p, U32_TYPE);
            } 
            else if (!SON_STRNCMP ("log_chunk_cnt", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_chunk_cnt, readString, fptr_p, U16_TYPE);
            }
            else if (!SON_STRNCMP ("log_cpu_bitmap", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_cpu_bitmap, readString, fptr_p, U32_TYPE);
            }
            else if (!SON_STRNCMP ("log_file_cnt", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.son_logger_cfg.log_file_cnt, readString, fptr_p, CHAR_TYPE);
            } 
            else
            {
                /* SPR 10897: Fix Starts */
                SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                        readString); 
                /* SPR 10897: Fix Ends */
                son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
            }    
        }
        else if (!SON_STRNCMP ("rachopt", readString, 7))
        {
            if (!SON_STRNCMP ("rachopt_mode", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.
                        rachopt_mode, readString, fptr_p, U32_TYPE);
            } 
            else if (!SON_STRNCMP ("rachopt_report_periodicity", readString, 
                        param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.
                        rach_cell_level_config_params.
                        rachopt_report_periodicity, 
                        readString, fptr_p, U16_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask 
                    |= SON_RACHOPT_REPORT_PERIODICITY_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_min_ue_rach_report_count", readString,
                        param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.
                        rach_cell_level_config_params.
                        rachopt_min_ue_rach_report_count, readString, 
                        fptr_p, U16_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |= SON_RACHOPT_MIN_UE_RACH_REPORT_COUNT_PRESENT;

            } 
            else if (!SON_STRNCMP ("rachopt_ue_rach_attempts_threshold", 
                        readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_ue_rach_attempts_threshold, readString, fptr_p, CHAR_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |= SON_RACHOPT_UE_RACH_ATTEMPTS_THRESHOLD_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_start_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_start_threshold, readString, fptr_p,U16_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |= SON_RACHOPT_START_THRESHOLD_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_stop_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_stop_threshold, readString, fptr_p, U16_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |= SON_RACHOPT_STOP_THRESHOLD_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_degrade_stop_offset", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_degrade_stop_offset, readString, fptr_p,U16_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |= SON_RACHOPT_DEGRADE_STOP_OFFSET_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_max_iterations_to_reach_stop_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_max_iterations_to_reach_stop_threshold, readString, fptr_p, CHAR_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |=
                    SON_RACHOPT_MAX_ITERATIONS_TO_REACH_STOP_THRESHOLD_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_contention_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_contention_threshold, readString, fptr_p, CHAR_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |=
                    SON_RACHOPT_CONTENTION_THRESHOLD_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_power_rampup_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_power_rampup_threshold, readString, fptr_p, CHAR_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |=
                    SON_RACHOPT_POWER_RAMPUP_THRESHOLD_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_min_cfra_preambles", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.rach_cell_level_config_params.rachopt_min_cfra_preambles, readString, fptr_p, CHAR_TYPE);
                g_config_params.rach_config_params.rach_cell_level_config_params.bitmask |=
                    SON_RACHOPT_MIN_CFRA_PREAMBLES_PRESENT;
            } 
            else if (!SON_STRNCMP ("rachopt_strong_cell_count", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.read_only_config_params.rachopt_strong_cell_count, readString, fptr_p, CHAR_TYPE);
            } 
            else if (!SON_STRNCMP ("rachopt_no_of_windows_to_skip_for_optimization", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.read_only_config_params.rachopt_no_of_windows_to_skip_for_optimization, readString, fptr_p, CHAR_TYPE);
            }
            else if (!SON_STRNCMP ("rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.read_only_config_params.rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold, readString, fptr_p, CHAR_TYPE);
            }
            else if (!SON_STRNCMP ("rachopt_cfra_unused_threshold", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.rach_config_params.read_only_config_params.rachopt_cfra_unused_threshold, readString, fptr_p, CHAR_TYPE);
            }
            else
            {
                /* SPR 10897: Fix Starts */
                SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                        readString); 
                /* SPR 10897: Fix Ends */
                son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
            }   

        }
        /*SRVCC-Start*/
        else if (!SON_STRNCMP ("anr", readString, 3))
        {
            if (!SON_STRNCMP ("anr_ps_ho_supported", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.anr_config_params.anr_ps_ho_supported, readString, fptr_p, U32_TYPE);
            }
            else if (!SON_STRNCMP ("anr_voip_capable", readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.anr_config_params.anr_voip_capable, readString, fptr_p, U32_TYPE);
            }
            else if (!SON_STRNCMP ("anr_max_num_cgi_request", readString, param_length)) /*Added for SPR-9759*/
            {
                son_fill_param_value((void *)&g_config_params.anr_config_params.anr_max_num_cgi_request, readString, fptr_p, CHAR_TYPE);
                /*Klockwork Fix Ends*/
            }
            /* Rel 3.0 Multi Sector code changes Start */
            else if (!SON_STRNCMP ("anr_auto_maintain_local_cell_as_nbr", 
                        readString, param_length))
            {
                son_fill_param_value((void *)&g_config_params.anr_config_params.
                        anr_auto_maintain_local_cell_as_nbr, readString, 
                        fptr_p, U32_TYPE);
            }
            /*Klockwork Fix Ends*/
            /* Rel 3.0 Multi Sector code changes Stop */
            else
            {
                /* SPR 10897: Fix Starts */
                SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                       readString); 
                /* SPR 10897: Fix Ends */
                son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
            }
        } 
        /*SRVCC-End*/
      else
        {
            /* SPR 10897: Fix Starts */
            SON_ENCAPS_WARNING("son_oam.cfg parameter %s is not supported", 
                    readString); 
            /* SPR 10897: Fix Ends */
            son_fill_param_value(SON_PNULL, readString, fptr_p, NO_TYPE);
        }
    }    
    if(fptr_p)
    {
        SON_FCLOSE(fptr_p);
    }

    /* SPR 10897: Fix Starts */
    /*Lines deleted*/
    /* SPR 10897: Fix Ends */

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
}    

/*****************************************************************************
 * Function Name  : son_nearbyintf
 * Inputs         : value - Argument to be rounded off
 * Outputs        : None 
 * Returns        : The rounded integer value
 * Description    : This is the SON wrapper function of C library function
 *                  nearbyintf().
 ******************************************************************************/
son_float
son_nearbyintf
(
    son_float value
)
{
    return nearbyintf(value);
}

/*****************************************************************************
 * Function Name  : son_fabsf
 * Inputs         : value - Floating-point number whose absolute value is needed
 * Outputs        : None
 * Returns        : The absolute value
 * Description    : This is the SON wrapper function of C library function
 *                  fabsf().
 ******************************************************************************/
son_float
son_fabsf
(
    son_float value
)
{
    return fabsf(value);
}

/*****************************************************************************
 * Function Name  : son_roundf
 * Inputs         : value - Value to be rounded off
 * Outputs        : None
 * Returns        : Rounded integer value
 * Description    : This is the SON wrapper function of C library function
 *                  roundf().
 ******************************************************************************/
son_float
son_roundf
(
    son_float value
)
{
    return roundf(value);
}

/*****************************************************************************
 * Function Name  : son_localtime
 * Inputs         : p_timer - Calendar time
 * Outputs        : None
 * Returns        : The return value points to a statically allocated struct 
 *                  of type tm
 * Description    : This is the SON wrapper function of C library function
 *                  localtime().
 ******************************************************************************/
struct tm *
son_localtime
(
    const time_t *p_timer
)
{
    return localtime(p_timer);
}

/*****************************************************************************
 * Function Name  : son_time 
 * Inputs         : None
 * Outputs        : p_value - The value of time in seconds since the Epoch
 * Returns        : Time since the Epoch (00:00:00 UTC, January 1, 1970), 
 *                  measured in seconds.
 * Description    : This is the SON wrapper function of C library function
 *                  time().
 ******************************************************************************/
time_t 
son_time
(
    time_t *p_value
)
{
    return time(p_value);
}

/*****************************************************************************
 * Function Name  : son_gettimeofday
 * Inputs         : p_timezone - timezone structure (obsolete now)
 * Outputs        : p_time - Number of seconds and microseconds since the Epoch
 * Returns        : return 0 for success, or -1 for failure
 * Description    : This is the SON wrapper function of C library function
 *                  gettimeofday().
 ******************************************************************************/
son_s32
son_gettimeofday
(
    struct timeval  *p_time, 
    struct timezone *p_timezone
)
{
    return gettimeofday(p_time, p_timezone);
}

/*****************************************************************************
 * Function Name  : son_difftime
 * Inputs         : time1 - 1st calendar  time
 *                  time0 - 2nd calendar  time
 * Outputs        : None 
 * Returns        : The number of seconds elapsed between time time1 and time 
 *                  time0
 * Description    : This is the SON wrapper function of C library function
 *                  difftime().
 ******************************************************************************/
son_double 
son_difftime
(
    time_t time1, 
    time_t time0
)
{
    return difftime(time1, time0);
}

/*****************************************************************************
 * Function Name  : son_atoi 
 * Inputs         : p_value Pointer to the input string
 * Outputs        : None 
 * Returns        : The converted value if the value can be represented.
 * Description    : This is the SON wrapper function of C library function
 *                  atoi().
 ******************************************************************************/
son_s32 
son_atoi
(
    const son_8 *p_value
)
{
    return atoi(p_value);
}

/*****************************************************************************
 * Function Name  : son_inet_pton
 * Inputs         : af      The family of the address.
 *                  p_src   Pointer to the string being  passed in
 * Outputs        : p_dst   Pointer to a buffer into which the function 
 *                          stores the numeric address
 * Returns        : 1 if the conversion succeeds, 
 *                  0 if the input is not a valid IPv4 dotted-decimal string  
 *                  or a valid IPv6 address string, or 
 *                  -1 if the af argument is unknown.
 * Description    : This is the SON wrapper function of C library function
 *                  inet_pton().
 ******************************************************************************/
son_s32 
son_inet_pton
(
    son_s32     af, 
    const son_8 *p_src, 
    son_void_t  *p_dst
)
{
    return inet_pton(af, p_src, p_dst);
}

/*****************************************************************************
 * Function Name  : son_inet_ntop
 * Inputs         : af      The family of the address.
 *                  p_src   Pointer to the buffer holding an IPv4 address
 *                          if the af argument is AF_INET, or an IPv6  address
 *                          if  the  af argument is AF_INET6
 * Outputs        : p_dst   Pointer to a buffer which stores the resulting 
 *                          text string 
 *                  size    Size of the buffer
 * Returns        : Pointer to the buffer containing the text string if the 
 *                  conversion succeeds, and NULL otherwise
 * Description    : This is the SON wrapper function of C library function
 *                  inet_ntop().
 ******************************************************************************/
const son_8 *
son_inet_ntop
(
    son_s32             af, 
    const son_void_t    *p_src,
    son_8               *p_dst, 
    socklen_t           size
)
{
    return inet_ntop(af, p_src, p_dst, size);
}

/*****************************************************************************
 * Function Name  : son_ntohl
 * Inputs         : netlong     Value in network byte order
 * Outputs        : None 
 * Returns        : The argument value converted from network to host byte 
 *                  order.
 * Description    : This is the SON wrapper function of C library function
 *                  ntohl().
 ******************************************************************************/
son_u32
son_ntohl
(
    son_u32 netlong
)
{
    return ntohl(netlong);
}

/*****************************************************************************
 * Function Name  : son_sched_setaffinity
 * Inputs         : pid     process ID
 *                  p_mask  Pointer to the mask value to be set
 *                  cpusetsize  length (in bytes) of the data pointed to by mask
 * Outputs        : None 
 * Returns        : 0 on success
 *                  -1 on failure
 * Description    : This is the SON wrapper function of C library function
 *                  sched_setaffinity().
 ******************************************************************************/
son_s32 
son_sched_setaffinity
(
    pid_t       pid, 
    size_t      cpusetsize,
    cpu_set_t   *p_mask
)
{
    return sched_setaffinity(pid, cpusetsize, p_mask);
}

/*****************************************************************************
 * Function Name  : son_fgets
 * Inputs         : size        1 less then the number of characters to be read
 *                  p_stream    Pointer to the stream to be read
 * Outputs        : p_out_buf   Pointer to the output buffer 
 * Returns        : p_out_buf on success
 *                  NULL on error or when end of file occurs while no 
 *                  characters have been read.
 * Description    : This is the SON wrapper function of C library function
 *                  fgets().
 ******************************************************************************/
son_8 *
son_fgets
(
    son_8   *p_out_buf, 
    son_s32 size, 
    FILE    *p_stream
)
{
    return fgets(p_out_buf, size, p_stream);
}

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
 * Function Name  : son_insert_txn_id_in_temp_txn_id_list
 * Inputs         : txn_id  Transaction Id
 * Outputs        : None
 * Returns        : None
 * Description    : This is the helper function to insert transaction ids in
 *                  temporary transaction id list. It is used to store txn id 
 *                  of de-register request triggered towards rrm due to cell 
 *                  deletion request from oam.
 ******************************************************************************/
son_void_t
son_insert_txn_id_in_temp_txn_id_list
(
    son_u16 txn_id
)
{
    son_bool_et is_inserted = SON_FALSE;
    son_u8 idx = 0;
    SON_UT_TRACE_ENTER();

    for (; idx < SON_MAX_TEMP_TXN_ID; idx++)
    {
        if (-1 == temp_txn_id_list[idx])
        {
            temp_txn_id_list[idx] = txn_id;
            is_inserted = SON_TRUE;
            break;
        }
    }

    if (SON_FALSE == is_inserted)
    {
        temp_txn_id_list[0] = txn_id;
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,
                SON_DETAILED,
                "The global temporary transaction Id list is full so replacing "
                "first value in the list with the new transaction Id");
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_find_txn_id_in_temp_txn_id_list
 * Inputs         : txn_id  Transaction Id
 * Outputs        : None
 * Returns        : SON_SUCCESS - If transaction is found and deleted from list
 *                  SON_FAILURE - Otherwise
 * Description    : This is the helper function to find and delete transaction 
 *                  Ids from temporary transaction Id list (Used to store txn 
 *                  Id of de-register request triggered towards rrm due to cell 
 *                  deletion request from oam.
 ******************************************************************************/
son_return_et
son_find_txn_id_in_temp_txn_id_list
(
    son_u16 txn_id
)
{
    son_u8 idx = 0;
    SON_UT_TRACE_ENTER();

    for (idx = 0; idx < SON_MAX_TEMP_TXN_ID; idx++)
    {
        if (temp_txn_id_list[idx] == txn_id)
        {
            temp_txn_id_list[idx] = -1;
            SON_UT_TRACE_EXIT();
            return SON_SUCCESS;
        }
    }

    SON_UT_TRACE_EXIT();
    return SON_FAILURE;
}
/* Rel 3.0 Cell delete support changes Stop */

/*SPR-11554-Start*/

/*****************************************************************************
 * Function Name  : son_determine_pci_range_end_value
 * Inputs         : pci_range  - pci range received
 * Outputs        : Integer value to calculate the pci range end
 * Returns        : son_s32
 * Description    : This function is used to calculate the pci range end value
 *****************************************************************************/
son_u16
son_determine_pci_range_end_value
(
 son_oam_cell_id_range_et   pci_range
 )
{
    son_u16   ret_value = 0;

    SON_UT_TRACE_ENTER();

    switch(pci_range)
    {
        case  SON_OAM_N4:
            {
                ret_value = 4;
                break;
            }
        case  SON_OAM_N8:
            {
                ret_value = 8;
                break;
            }
        case  SON_OAM_N12:
            {
                ret_value = 12;
                break;
            }
        case  SON_OAM_N16:
            {
                ret_value = 16;
                break;
            }
        case  SON_OAM_N24:
            {
                ret_value = 24;
                break;
            }
        case  SON_OAM_N32:
            {
                ret_value = 32;
                break;
            }
        case  SON_OAM_N48:
            {
                ret_value = 48;
                break;
            }
        case  SON_OAM_N64:
            {
                ret_value = 64;
                break;
            }
        case  SON_OAM_N84:
            {
                ret_value = 84;
                break;
            }
        case  SON_OAM_N96:
            {
                ret_value = 96;
                break;
            }
        case  SON_OAM_N128:
            {
                ret_value = 128;
                break;
            }
        case  SON_OAM_N168:
            {
                ret_value = 168;
                break;
            }
        case  SON_OAM_N252:
            {
                ret_value = 252;
                break;
            }
        case  SON_OAM_N504:
            {
                ret_value = 504;
                break;
            }
        default:
            {
                SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,
                        SON_ERROR,
                        "unexpected pci range received");
            }
    }

    SON_UT_TRACE_EXIT();
    return ret_value;
}

/*SPR-11554-End*/

/******************************************************************************
*   FUNCTION NAME: son_get_module 
*   INPUT        : son_module_id_t  moduleId
*   OUTPUT       : None
*   RETURNS      : pointer to char 
*   DESCRIPTION  : This Function returns module name
******************************************************************************/
const char* son_get_module(son_module_id_t  moduleId) 
{
    if((moduleId >= SON_MIN_MODULE_ID) &&
            (moduleId <= SON_MAX_MODULE_ID))
    {
        return ((char*)(SON_INTERNAL_MODULE_NAMES[moduleId - SON_MIN_MODULE_ID]));
    }
    else if((moduleId >= SON_OAM_MODULE_ID) &&
            (moduleId <= SON_RRC_MODULE_ID))
    {
        return ((char*)(SON_EXTERNAL_MODULE_NAMES[moduleId - SON_OAM_MODULE_ID]));
    }
    else if(moduleId == SON_PHY_MODULE_ID)
    {
        return ((char*)(SON_EXTERNAL_MODULE_NAMES[3]));
    }
    else 
    {
        return UNKNOWN_MODULE;
    }
}

/******************************************************************************
*   FUNCTION NAME: son_get_msg_api_name 
*   INPUT        : U16 apiId
*                  son_module_id_t moduleId
*   OUTPUT       : None
*   RETURNS      : pointer to char 
*   DESCRIPTION  : This Function returns api name
******************************************************************************/
const char* son_get_msg_api_name(U16 apiId, son_module_id_t moduleId) 
{
    if(((moduleId >= SON_MIN_MODULE_ID) && (moduleId <= SON_MAX_MODULE_ID)) ||
            (moduleId == SON_PHY_MODULE_ID) || (moduleId == SON_OAM_MODULE_ID))
    {
/* coverity_96485 start */
/* coverity_94763 start */
        U16 api_index = (U16)son_find_index_of_procedure_code_array((son_procedure_code_et)apiId);/*cov 94851+-*/
/* coverity_94763 stop */
/* coverity_96485 stop */
        if(api_index < sizeof(SON_PROCEDURE_CODE_NAMES) /
                sizeof(SON_PROCEDURE_CODE_NAMES[0]))
        {
            return ((char*)(SON_PROCEDURE_CODE_NAMES[api_index]));
        }
        else
        {
            return UNKNOWN_API_NAME;
        }
    }
    else if(moduleId == SON_RRM_MODULE_ID)
    {
/* coverity_94763 start */
        if((apiId >= (U16)RRM_SON_API_BASE) &&
           (apiId <= (U16)RRM_SON_ENB_CONFIG_UPDATE_IND))
        {
            U16 api_index = apiId - (U16)RRM_SON_API_BASE;
/* coverity_94763 stop */
            if(api_index < sizeof(RRM_SON_API_NAME) /
                    sizeof(RRM_SON_API_NAME[0]))
            {
                return ((char*)(RRM_SON_API_NAME[api_index]));
            }
            else
            {
                return UNKNOWN_API_NAME;
            }
        }
/* coverity_94763 start */
        else if((apiId >= (U16)SON_RRM_API_BASE) &&
                (apiId <= (U16)RRM_SON_EICIC_ENABLE_DISABLE_IND))
        {
            U16 api_index = apiId - (U16)RRM_SON_API_BASE;
/* coverity_94763 stop */
            if(api_index < sizeof(SON_RRM_API_NAME) /
                    sizeof(SON_RRM_API_NAME[0]))
            {
                return ((char*)(SON_RRM_API_NAME[api_index]));
            }
            else
            {
                return UNKNOWN_API_NAME;
            }
        }
        else
        {
            return UNKNOWN_API_NAME;
        }
    }
    else if(moduleId == SON_RRC_MODULE_ID)
    {
        if((apiId >= X2AP_ADD_ENB_REQ) &&
           (apiId <= X2AP_DEL_ENB_RES))
        {
            U16 api_index = apiId - X2AP_ADD_ENB_REQ;
            if(api_index < sizeof(SON_RRC_API_NAMES) /
                    sizeof(SON_RRC_API_NAMES[0]))
            {
                return ((char*)(SON_RRC_API_NAMES[moduleId]));
            }
            else
            {
                return UNKNOWN_API_NAME;
            }
        }
        else if((apiId >= X2AP_ENB_CONFIG_UPDATE_REQ) &&
                (apiId <= X2AP_MOBILITY_SETTING_CHANGE_RESP))
        {
            U16 api_index = apiId - X2AP_ENB_CONFIG_UPDATE_REQ;
            if(api_index < sizeof(SON_X2AP_API_NAMES) /
                    sizeof(SON_X2AP_API_NAMES[0]))
            {
                return ((char*)(SON_X2AP_API_NAMES[api_index]));
            }
            else
            {
                return UNKNOWN_API_NAME;
            }
        }
        else
        {
            return UNKNOWN_API_NAME;
        }
    }
    else 
    {
        return UNKNOWN_API_NAME;
    }
}


/* Spr 17753 Changes Start */
/**********************************************************************
 * Function Name  : son_anr_get_random_val 
 * Inputs         : start_val - starting val 
 *                : end_val -   end val 
 * Outputs        : 
 * Returns        : random value between start_val & end_val  
 * Description    : This function return the random value between 
 *                  the start_val and end_val 
 **********************************************************************/
son_u32
son_anr_get_random_val(son_u32 start_val, son_u32 end_val)
{

    son_u32 random_val                              = SON_NULL;

    son_void_t * p_value;

    SON_UT_TRACE_ENTER();
    /* Taking memory address for randomizer */
    p_value =  son_mem_get(sizeof(son_u8));

    if (SON_PNULL == p_value)
    {
        SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,
                SON_ERROR, 
                "Memory Allocation failure");
        SON_UT_TRACE_EXIT();
        return start_val;
    }
/* SPR 20636 Changes Start*/
    random_val = (son_u32_u64_Bit)p_value % end_val;

/* SPR 20636 Changes End*/
    if(random_val < start_val)
    {
        if((random_val + start_val) < end_val)
        {
            random_val = random_val + start_val;
        }
        else
        {
            random_val = start_val;
        }
    }
/* SPR 20636 Changes Start*/
    SON_LOG(SON_OAM_LOG_ON, son_utils_facility_name,
            SON_BRIEF, 
            " start val=%u end val=%u randomizer=%u random num generated=%u",
            start_val, end_val, (son_u32_u64_Bit)p_value, random_val);
/* SPR 20636 Changes End*/

    /* Free the allocated memory */
    son_mem_free(p_value);

    SON_UT_TRACE_EXIT();
    return random_val;
}
/* Spr 17753 Changes End */
/* SPR 22304 Fix Start */
#ifdef ATTRIBUTE_COMP
void son_trace_dbg(
        U32 is_log_enable,const son_8 *son_log_facility_name,U8 log_level,
        const char* format, ...)
{
    /* Empty Definition */
}

void son_trace_dbg_1(
        U8 log_level,
        const char* format, ...)
{
    /* Empty Definition */
}
#endif
/* SPR 22304 Fix End */

#ifdef LINUX_PC_TEST
void pc_test_forward_timer_msg(U32 moduldId,const void* pdata,U8 timeType)
{
    /* Empty Definition */
}
#endif